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


#ifndef _IOMUX_H_
#define _IOMUX_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'iomux'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// IOMUX_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_IOMUX_BASE             0x0510C000
#else
#define REG_IOMUX_BASE             0x5010C000
#endif

typedef volatile struct
{
    REG32                          pad_spi_flash_clk_cfg_reg;    //0x00000000
    REG32                          pad_spi_flash_cs0_cfg_reg;    //0x00000004
    REG32                          pad_spi_flash_cs1_cfg_reg;    //0x00000008
    REG32                          pad_spi_flash_sio_0_cfg_reg;  //0x0000000C
    REG32                          pad_spi_flash_sio_1_cfg_reg;  //0x00000010
    REG32                          pad_spi_flash_sio_2_cfg_reg;  //0x00000014
    REG32                          pad_spi_flash_sio_3_cfg_reg;  //0x00000018
    REG32                          pad_sdmmc1_clk_cfg_reg;       //0x0000001C
    REG32                          pad_sdmmc1_cmd_cfg_reg;       //0x00000020
    REG32                          pad_sdmmc1_data_0_cfg_reg;    //0x00000024
    REG32                          pad_sdmmc1_data_1_cfg_reg;    //0x00000028
    REG32                          pad_sdmmc1_data_2_cfg_reg;    //0x0000002C
    REG32                          pad_sdmmc1_data_3_cfg_reg;    //0x00000030
    REG32                          pad_i2s1_bck_cfg_reg;         //0x00000034
    REG32                          pad_i2s1_lrck_cfg_reg;        //0x00000038
    REG32                          pad_i2s1_sdat_i_cfg_reg;      //0x0000003C
    REG32                          pad_i2s1_sdat_o_cfg_reg;      //0x00000040
    REG32                          pad_adi_sda_cfg_reg;          //0x00000044
    REG32                          pad_adi_sync_cfg_reg;         //0x00000048
    REG32                          pad_adi_scl_cfg_reg;          //0x0000004C
    REG32                          pad_spi_lcd_sio_cfg_reg;      //0x00000050
    REG32                          pad_spi_lcd_sdc_cfg_reg;      //0x00000054
    REG32                          pad_spi_lcd_clk_cfg_reg;      //0x00000058
    REG32                          pad_spi_lcd_cs_cfg_reg;       //0x0000005C
    REG32                          pad_spi_lcd_select_cfg_reg;   //0x00000060
    REG32                          pad_lcd_rstb_cfg_reg;         //0x00000064
    REG32                          pad_lcd_fmark_cfg_reg;        //0x00000068
    REG32                          pad_camera_rst_l_cfg_reg;     //0x0000006C
    REG32                          pad_camera_pwdn_cfg_reg;      //0x00000070
    REG32                          pad_camera_ref_clk_cfg_reg;   //0x00000074
    REG32                          pad_camera_vsync_cfg_reg;     //0x00000078
    REG32                          pad_camera_pix_clk_cfg_reg;   //0x0000007C
    REG32                          pad_camera_href_cfg_reg;      //0x00000080
    REG32                          pad_camera_data_in_0_cfg_reg; //0x00000084
    REG32                          pad_camera_data_in_1_cfg_reg; //0x00000088
    REG32                          pad_camera_data_in_2_cfg_reg; //0x0000008C
    REG32                          pad_camera_data_in_3_cfg_reg; //0x00000090
    REG32                          pad_camera_data_in_4_cfg_reg; //0x00000094
    REG32                          pad_camera_data_in_5_cfg_reg; //0x00000098
    REG32                          pad_camera_data_in_6_cfg_reg; //0x0000009C
    REG32                          pad_camera_data_in_7_cfg_reg; //0x000000A0
    REG32                          pad_ap_jtag_tck_cfg_reg;      //0x000000A4
    REG32                          pad_ap_jtag_trst_cfg_reg;     //0x000000A8
    REG32                          pad_ap_jtag_tms_cfg_reg;      //0x000000AC
    REG32                          pad_ap_jtag_tdi_cfg_reg;      //0x000000B0
    REG32                          pad_ap_jtag_tdo_cfg_reg;      //0x000000B4
    REG32                          pad_pwl_out_1_cfg_reg;        //0x000000B8
    REG32                          pad_pwl_out_0_cfg_reg;        //0x000000BC
    REG32                          pad_pwm_lpg_out_cfg_reg;      //0x000000C0
    REG32                          pad_pwt_out_cfg_reg;          //0x000000C4
    REG32                          pad_gpio_0_cfg_reg;           //0x000000C8
    REG32                          pad_gpio_1_cfg_reg;           //0x000000CC
    REG32                          pad_gpio_2_cfg_reg;           //0x000000D0
    REG32                          pad_gpio_3_cfg_reg;           //0x000000D4
    REG32                          pad_gpio_4_cfg_reg;           //0x000000D8
    REG32                          pad_gpio_5_cfg_reg;           //0x000000DC
    REG32                          pad_gpio_10_cfg_reg;          //0x000000E0
    REG32                          pad_gpio_11_cfg_reg;          //0x000000E4
    REG32                          pad_gpio_12_cfg_reg;          //0x000000E8
    REG32                          pad_gpio_6_cfg_reg;           //0x000000EC
    REG32                          pad_gpio_7_cfg_reg;           //0x000000F0
    REG32                          pad_gpio_8_cfg_reg;           //0x000000F4
    REG32                          pad_gpio_9_cfg_reg;           //0x000000F8
    REG32                          pad_i2c_m4_scl_cfg_reg;       //0x000000FC
    REG32                          pad_i2c_m4_sda_cfg_reg;       //0x00000100
    REG32                          pad_uart_1_rxd_cfg_reg;       //0x00000104
    REG32                          pad_uart_1_txd_cfg_reg;       //0x00000108
    REG32                          pad_uart_1_cts_cfg_reg;       //0x0000010C
    REG32                          pad_uart_1_rts_cfg_reg;       //0x00000110
    REG32                          pad_uart_2_rxd_cfg_reg;       //0x00000114
    REG32                          pad_uart_2_txd_cfg_reg;       //0x00000118
    REG32                          pad_uart_2_cts_cfg_reg;       //0x0000011C
    REG32                          pad_uart_2_rts_cfg_reg;       //0x00000120
    REG32                          pad_gpo_0_cfg_reg;            //0x00000124
    REG32                          pad_gpo_1_cfg_reg;            //0x00000128
    REG32                          pad_gpo_2_cfg_reg;            //0x0000012C
    REG32                          pad_gpo_3_cfg_reg;            //0x00000130
    REG32                          pad_gpo_4_cfg_reg;            //0x00000134
    REG32                          pad_i2c_m1_scl_cfg_reg;       //0x00000138
    REG32                          pad_i2c_m1_sda_cfg_reg;       //0x0000013C
    REG32                          pad_i_keyon_cfg_reg;          //0x00000140
    REG32                          pad_keyin_0_cfg_reg;          //0x00000144
    REG32                          pad_keyin_1_cfg_reg;          //0x00000148
    REG32                          pad_keyin_2_cfg_reg;          //0x0000014C
    REG32                          pad_keyin_3_cfg_reg;          //0x00000150
    REG32                          pad_keyin_4_cfg_reg;          //0x00000154
    REG32                          pad_keyin_5_cfg_reg;          //0x00000158
    REG32                          pad_keyout_0_cfg_reg;         //0x0000015C
    REG32                          pad_keyout_1_cfg_reg;         //0x00000160
    REG32                          pad_keyout_2_cfg_reg;         //0x00000164
    REG32                          pad_keyout_3_cfg_reg;         //0x00000168
    REG32                          pad_keyout_4_cfg_reg;         //0x0000016C
    REG32                          pad_keyout_5_cfg_reg;         //0x00000170
    REG32                          pad_debug_host_rx_cfg_reg;    //0x00000174
    REG32                          pad_debug_host_tx_cfg_reg;    //0x00000178
    REG32                          pad_debug_host_clk_cfg_reg;   //0x0000017C
    REG32                          pad_sim_1_clk_cfg_reg;        //0x00000180
    REG32                          pad_sim_1_dio_cfg_reg;        //0x00000184
    REG32                          pad_sim_1_rst_cfg_reg;        //0x00000188
    REG32                          pad_sim_2_clk_cfg_reg;        //0x0000018C
    REG32                          pad_sim_2_dio_cfg_reg;        //0x00000190
    REG32                          pad_sim_2_rst_cfg_reg;        //0x00000194
    REG32                          pad_lte_reset_cfg_reg;        //0x00000198
    REG32                          pad_lte_sysclken_cfg_reg;     //0x0000019C
    REG32                          pad_lte_rfspi_cs_cfg_reg;     //0x000001A0
    REG32                          pad_lte_rfspi_clk_cfg_reg;    //0x000001A4
    REG32                          pad_lte_rfspi_dio_cfg_reg;    //0x000001A8
    REG32                          pad_lte_rbdp_rx_0_cfg_reg;    //0x000001AC
    REG32                          pad_lte_rbdp_rx_1_cfg_reg;    //0x000001B0
    REG32                          pad_lte_rbdp_rx_2_cfg_reg;    //0x000001B4
    REG32                          pad_lte_rbdp_rx_3_cfg_reg;    //0x000001B8
    REG32                          pad_lte_rbdp_rx_4_cfg_reg;    //0x000001BC
    REG32                          pad_lte_rbdp_rx_5_cfg_reg;    //0x000001C0
    REG32                          pad_lte_rbdp_rx_6_cfg_reg;    //0x000001C4
    REG32                          pad_lte_rbdp_rx_7_cfg_reg;    //0x000001C8
    REG32                          pad_lte_rbdp_rx_8_cfg_reg;    //0x000001CC
    REG32                          pad_lte_rbdp_rx_9_cfg_reg;    //0x000001D0
    REG32                          pad_lte_rbdp_rx_10_cfg_reg;   //0x000001D4
    REG32                          pad_lte_rbdp_rx_11_cfg_reg;   //0x000001D8
    REG32                          pad_lte_rbdp_tx_0_cfg_reg;    //0x000001DC
    REG32                          pad_lte_rbdp_tx_1_cfg_reg;    //0x000001E0
    REG32                          pad_lte_rbdp_tx_2_cfg_reg;    //0x000001E4
    REG32                          pad_lte_rbdp_tx_3_cfg_reg;    //0x000001E8
    REG32                          pad_lte_rbdp_tx_4_cfg_reg;    //0x000001EC
    REG32                          pad_lte_rbdp_tx_5_cfg_reg;    //0x000001F0
    REG32                          pad_lte_rbdp_tx_6_cfg_reg;    //0x000001F4
    REG32                          pad_lte_rbdp_tx_7_cfg_reg;    //0x000001F8
    REG32                          pad_lte_rbdp_tx_8_cfg_reg;    //0x000001FC
    REG32                          pad_lte_rbdp_tx_9_cfg_reg;    //0x00000200
    REG32                          pad_lte_rbdp_tx_10_cfg_reg;   //0x00000204
    REG32                          pad_lte_rbdp_tx_11_cfg_reg;   //0x00000208
    REG32                          pad_lte_mclk_cfg_reg;         //0x0000020C
    REG32                          pad_lte_rxframe_cfg_reg;      //0x00000210
    REG32                          pad_lte_fclk_cfg_reg;         //0x00000214
    REG32                          pad_lte_txframe_cfg_reg;      //0x00000218
    REG32                          pad_lte_rxtxen_cfg_reg;       //0x0000021C
    REG32                          pad_lte_rxtxdata_cfg_reg;     //0x00000220
    REG32                          pad_rfdig_gpio_0_cfg_reg;     //0x00000224
    REG32                          pad_rfdig_gpio_1_cfg_reg;     //0x00000228
    REG32                          pad_rfdig_gpio_2_cfg_reg;     //0x0000022C
    REG32                          pad_rfdig_gpio_3_cfg_reg;     //0x00000230
    REG32                          pad_rfdig_gpio_4_cfg_reg;     //0x00000234
    REG32                          pad_rfdig_gpio_5_cfg_reg;     //0x00000238
    REG32                          pad_rfdig_gpio_6_cfg_reg;     //0x0000023C
    REG32                          pad_rfdig_gpio_7_cfg_reg;     //0x00000240
    REG32                          pad_rfdig_gpio_8_cfg_reg;     //0x00000244
    REG32                          pad_rfdig_gpio_9_cfg_reg;     //0x00000248
    REG32                          pad_rf_gge_xen_cfg_reg;       //0x0000024C
    REG32                          pad_rf_gge_pdn_cfg_reg;       //0x00000250
    REG32                          pad_rf_gge_rfspi_cs_cfg_reg;  //0x00000254
    REG32                          pad_rf_gge_rfspi_sclk_cfg_reg; //0x00000258
    REG32                          pad_rf_gge_rfspi_sdio_cfg_reg; //0x0000025C
    REG32                          pad_rf_gge_strobe_cfg_reg;    //0x00000260
    REG32                          pad_codec_mclk_cfg_reg;       //0x00000264
    REG32                          pad_chip_pd_cfg_reg;          //0x00000268
} HWP_IOMUX_T;

#define hwp_iomux                  ((HWP_IOMUX_T*) REG_ACCESS_ADDRESS(REG_IOMUX_BASE))


//pad_spi_flash_clk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_flash_clk_sel : 4; // [3:0]
        REG32 pad_spi_flash_clk_ana : 4; // [7:4]
        REG32 pad_spi_flash_clk_pull_dn : 1; // [8]
        REG32 pad_spi_flash_clk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_flash_clk_pull_frc : 1; // [16]
        REG32 pad_spi_flash_clk_oen_reg : 1; // [17]
        REG32 pad_spi_flash_clk_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_flash_clk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_flash_clk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_flash_clk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_FLASH_CLK_CFG_REG_T;

//pad_spi_flash_cs0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_flash_cs0_sel : 4; // [3:0]
        REG32 pad_spi_flash_cs0_ana : 4; // [7:4]
        REG32 pad_spi_flash_cs0_pull_dn : 1; // [8]
        REG32 pad_spi_flash_cs0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_flash_cs0_pull_frc : 1; // [16]
        REG32 pad_spi_flash_cs0_oen_reg : 1; // [17]
        REG32 pad_spi_flash_cs0_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_flash_cs0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_flash_cs0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_flash_cs0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_FLASH_CS0_CFG_REG_T;

//pad_spi_flash_cs1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_flash_cs1_sel : 4; // [3:0]
        REG32 pad_spi_flash_cs1_ana : 4; // [7:4]
        REG32 pad_spi_flash_cs1_pull_dn : 1; // [8]
        REG32 pad_spi_flash_cs1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_flash_cs1_pull_frc : 1; // [16]
        REG32 pad_spi_flash_cs1_oen_reg : 1; // [17]
        REG32 pad_spi_flash_cs1_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_flash_cs1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_flash_cs1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_flash_cs1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_FLASH_CS1_CFG_REG_T;

//pad_spi_flash_sio_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_flash_sio_0_sel : 4; // [3:0]
        REG32 pad_spi_flash_sio_0_ana : 4; // [7:4]
        REG32 pad_spi_flash_sio_0_pull_dn : 1; // [8]
        REG32 pad_spi_flash_sio_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_flash_sio_0_pull_frc : 1; // [16]
        REG32 pad_spi_flash_sio_0_oen_reg : 1; // [17]
        REG32 pad_spi_flash_sio_0_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_flash_sio_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_flash_sio_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_flash_sio_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_FLASH_SIO_0_CFG_REG_T;

//pad_spi_flash_sio_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_flash_sio_1_sel : 4; // [3:0]
        REG32 pad_spi_flash_sio_1_ana : 4; // [7:4]
        REG32 pad_spi_flash_sio_1_pull_dn : 1; // [8]
        REG32 pad_spi_flash_sio_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_flash_sio_1_pull_frc : 1; // [16]
        REG32 pad_spi_flash_sio_1_oen_reg : 1; // [17]
        REG32 pad_spi_flash_sio_1_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_flash_sio_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_flash_sio_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_flash_sio_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_FLASH_SIO_1_CFG_REG_T;

//pad_spi_flash_sio_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_flash_sio_2_sel : 4; // [3:0]
        REG32 pad_spi_flash_sio_2_ana : 4; // [7:4]
        REG32 pad_spi_flash_sio_2_pull_dn : 1; // [8]
        REG32 pad_spi_flash_sio_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_flash_sio_2_pull_frc : 1; // [16]
        REG32 pad_spi_flash_sio_2_oen_reg : 1; // [17]
        REG32 pad_spi_flash_sio_2_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_flash_sio_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_flash_sio_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_flash_sio_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_FLASH_SIO_2_CFG_REG_T;

//pad_spi_flash_sio_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_flash_sio_3_sel : 4; // [3:0]
        REG32 pad_spi_flash_sio_3_ana : 4; // [7:4]
        REG32 pad_spi_flash_sio_3_pull_dn : 1; // [8]
        REG32 pad_spi_flash_sio_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_flash_sio_3_pull_frc : 1; // [16]
        REG32 pad_spi_flash_sio_3_oen_reg : 1; // [17]
        REG32 pad_spi_flash_sio_3_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_flash_sio_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_flash_sio_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_flash_sio_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_FLASH_SIO_3_CFG_REG_T;

//pad_sdmmc1_clk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sdmmc1_clk_sel : 4; // [3:0]
        REG32 pad_sdmmc1_clk_ana : 4; // [7:4]
        REG32 pad_sdmmc1_clk_pull_dn : 1; // [8]
        REG32 pad_sdmmc1_clk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sdmmc1_clk_pull_frc : 1; // [16]
        REG32 pad_sdmmc1_clk_oen_reg : 1; // [17]
        REG32 pad_sdmmc1_clk_drv_strength : 2; // [19:18], write set
        REG32 pad_sdmmc1_clk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sdmmc1_clk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sdmmc1_clk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SDMMC1_CLK_CFG_REG_T;

//pad_sdmmc1_cmd_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sdmmc1_cmd_sel : 4; // [3:0]
        REG32 pad_sdmmc1_cmd_ana : 4; // [7:4]
        REG32 pad_sdmmc1_cmd_pull_dn : 1; // [8]
        REG32 pad_sdmmc1_cmd_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sdmmc1_cmd_pull_frc : 1; // [16]
        REG32 pad_sdmmc1_cmd_oen_reg : 1; // [17]
        REG32 pad_sdmmc1_cmd_drv_strength : 2; // [19:18], write set
        REG32 pad_sdmmc1_cmd_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sdmmc1_cmd_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sdmmc1_cmd_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SDMMC1_CMD_CFG_REG_T;

//pad_sdmmc1_data_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sdmmc1_data_0_sel : 4; // [3:0]
        REG32 pad_sdmmc1_data_0_ana : 4; // [7:4]
        REG32 pad_sdmmc1_data_0_pull_dn : 1; // [8]
        REG32 pad_sdmmc1_data_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sdmmc1_data_0_pull_frc : 1; // [16]
        REG32 pad_sdmmc1_data_0_oen_reg : 1; // [17]
        REG32 pad_sdmmc1_data_0_drv_strength : 2; // [19:18], write set
        REG32 pad_sdmmc1_data_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sdmmc1_data_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sdmmc1_data_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_0_CFG_REG_T;

//pad_sdmmc1_data_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sdmmc1_data_1_sel : 4; // [3:0]
        REG32 pad_sdmmc1_data_1_ana : 4; // [7:4]
        REG32 pad_sdmmc1_data_1_pull_dn : 1; // [8]
        REG32 pad_sdmmc1_data_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sdmmc1_data_1_pull_frc : 1; // [16]
        REG32 pad_sdmmc1_data_1_oen_reg : 1; // [17]
        REG32 pad_sdmmc1_data_1_drv_strength : 2; // [19:18], write set
        REG32 pad_sdmmc1_data_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sdmmc1_data_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sdmmc1_data_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_1_CFG_REG_T;

//pad_sdmmc1_data_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sdmmc1_data_2_sel : 4; // [3:0]
        REG32 pad_sdmmc1_data_2_ana : 4; // [7:4]
        REG32 pad_sdmmc1_data_2_pull_dn : 1; // [8]
        REG32 pad_sdmmc1_data_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sdmmc1_data_2_pull_frc : 1; // [16]
        REG32 pad_sdmmc1_data_2_oen_reg : 1; // [17]
        REG32 pad_sdmmc1_data_2_drv_strength : 2; // [19:18], write set
        REG32 pad_sdmmc1_data_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sdmmc1_data_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sdmmc1_data_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_2_CFG_REG_T;

//pad_sdmmc1_data_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sdmmc1_data_3_sel : 4; // [3:0]
        REG32 pad_sdmmc1_data_3_ana : 4; // [7:4]
        REG32 pad_sdmmc1_data_3_pull_dn : 1; // [8]
        REG32 pad_sdmmc1_data_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sdmmc1_data_3_pull_frc : 1; // [16]
        REG32 pad_sdmmc1_data_3_oen_reg : 1; // [17]
        REG32 pad_sdmmc1_data_3_drv_strength : 2; // [19:18], write set
        REG32 pad_sdmmc1_data_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sdmmc1_data_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sdmmc1_data_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_3_CFG_REG_T;

//pad_i2s1_bck_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_i2s1_bck_sel : 4; // [3:0]
        REG32 pad_i2s1_bck_ana : 4; // [7:4]
        REG32 pad_i2s1_bck_pull_dn : 1; // [8]
        REG32 pad_i2s1_bck_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_i2s1_bck_pull_frc : 1; // [16]
        REG32 pad_i2s1_bck_oen_reg : 1; // [17]
        REG32 pad_i2s1_bck_drv_strength : 2; // [19:18], write set
        REG32 pad_i2s1_bck_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_i2s1_bck_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_i2s1_bck_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_I2S1_BCK_CFG_REG_T;

//pad_i2s1_lrck_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_i2s1_lrck_sel : 4; // [3:0]
        REG32 pad_i2s1_lrck_ana : 4; // [7:4]
        REG32 pad_i2s1_lrck_pull_dn : 1; // [8]
        REG32 pad_i2s1_lrck_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_i2s1_lrck_pull_frc : 1; // [16]
        REG32 pad_i2s1_lrck_oen_reg : 1; // [17]
        REG32 pad_i2s1_lrck_drv_strength : 2; // [19:18], write set
        REG32 pad_i2s1_lrck_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_i2s1_lrck_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_i2s1_lrck_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_I2S1_LRCK_CFG_REG_T;

//pad_i2s1_sdat_i_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_i2s1_sdat_i_sel : 4; // [3:0]
        REG32 pad_i2s1_sdat_i_ana : 4; // [7:4]
        REG32 pad_i2s1_sdat_i_pull_dn : 1; // [8]
        REG32 pad_i2s1_sdat_i_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_i2s1_sdat_i_pull_frc : 1; // [16]
        REG32 pad_i2s1_sdat_i_oen_reg : 1; // [17]
        REG32 pad_i2s1_sdat_i_drv_strength : 2; // [19:18], write set
        REG32 pad_i2s1_sdat_i_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_i2s1_sdat_i_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_i2s1_sdat_i_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_I2S1_SDAT_I_CFG_REG_T;

//pad_i2s1_sdat_o_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_i2s1_sdat_o_sel : 4; // [3:0]
        REG32 pad_i2s1_sdat_o_ana : 4; // [7:4]
        REG32 pad_i2s1_sdat_o_pull_dn : 1; // [8]
        REG32 pad_i2s1_sdat_o_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_i2s1_sdat_o_pull_frc : 1; // [16]
        REG32 pad_i2s1_sdat_o_oen_reg : 1; // [17]
        REG32 pad_i2s1_sdat_o_drv_strength : 2; // [19:18], write set
        REG32 pad_i2s1_sdat_o_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_i2s1_sdat_o_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_i2s1_sdat_o_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_I2S1_SDAT_O_CFG_REG_T;

//pad_adi_sda_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_adi_sda_sel : 4; // [3:0]
        REG32 pad_adi_sda_ana : 4; // [7:4]
        REG32 pad_adi_sda_pull_dn : 1; // [8]
        REG32 pad_adi_sda_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_adi_sda_pull_frc : 1; // [16]
        REG32 pad_adi_sda_oen_reg : 1; // [17]
        REG32 pad_adi_sda_drv_strength : 2; // [19:18], write set
        REG32 pad_adi_sda_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_adi_sda_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_adi_sda_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_ADI_SDA_CFG_REG_T;

//pad_adi_sync_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_adi_sync_sel : 4; // [3:0]
        REG32 pad_adi_sync_ana : 4; // [7:4]
        REG32 pad_adi_sync_pull_dn : 1; // [8]
        REG32 pad_adi_sync_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_adi_sync_pull_frc : 1; // [16]
        REG32 pad_adi_sync_oen_reg : 1; // [17]
        REG32 pad_adi_sync_drv_strength : 2; // [19:18], write set
        REG32 pad_adi_sync_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_adi_sync_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_adi_sync_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_ADI_SYNC_CFG_REG_T;

//pad_adi_scl_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_adi_scl_sel : 4; // [3:0]
        REG32 pad_adi_scl_ana : 4; // [7:4]
        REG32 pad_adi_scl_pull_dn : 1; // [8]
        REG32 pad_adi_scl_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_adi_scl_pull_frc : 1; // [16]
        REG32 pad_adi_scl_oen_reg : 1; // [17]
        REG32 pad_adi_scl_drv_strength : 2; // [19:18], write set
        REG32 pad_adi_scl_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_adi_scl_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_adi_scl_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_ADI_SCL_CFG_REG_T;

//pad_spi_lcd_sio_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_lcd_sio_sel : 4; // [3:0]
        REG32 pad_spi_lcd_sio_ana : 4; // [7:4]
        REG32 pad_spi_lcd_sio_pull_dn : 1; // [8]
        REG32 pad_spi_lcd_sio_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_lcd_sio_pull_frc : 1; // [16]
        REG32 pad_spi_lcd_sio_oen_reg : 1; // [17]
        REG32 pad_spi_lcd_sio_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_lcd_sio_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_lcd_sio_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_lcd_sio_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_LCD_SIO_CFG_REG_T;

//pad_spi_lcd_sdc_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_lcd_sdc_sel : 4; // [3:0]
        REG32 pad_spi_lcd_sdc_ana : 4; // [7:4]
        REG32 pad_spi_lcd_sdc_pull_dn : 1; // [8]
        REG32 pad_spi_lcd_sdc_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_lcd_sdc_pull_frc : 1; // [16]
        REG32 pad_spi_lcd_sdc_oen_reg : 1; // [17]
        REG32 pad_spi_lcd_sdc_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_lcd_sdc_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_lcd_sdc_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_lcd_sdc_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_LCD_SDC_CFG_REG_T;

//pad_spi_lcd_clk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_lcd_clk_sel : 4; // [3:0]
        REG32 pad_spi_lcd_clk_ana : 4; // [7:4]
        REG32 pad_spi_lcd_clk_pull_dn : 1; // [8]
        REG32 pad_spi_lcd_clk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_lcd_clk_pull_frc : 1; // [16]
        REG32 pad_spi_lcd_clk_oen_reg : 1; // [17]
        REG32 pad_spi_lcd_clk_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_lcd_clk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_lcd_clk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_lcd_clk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_LCD_CLK_CFG_REG_T;

//pad_spi_lcd_cs_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_lcd_cs_sel : 4; // [3:0]
        REG32 pad_spi_lcd_cs_ana : 4; // [7:4]
        REG32 pad_spi_lcd_cs_pull_dn : 1; // [8]
        REG32 pad_spi_lcd_cs_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_lcd_cs_pull_frc : 1; // [16]
        REG32 pad_spi_lcd_cs_oen_reg : 1; // [17]
        REG32 pad_spi_lcd_cs_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_lcd_cs_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_lcd_cs_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_lcd_cs_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_LCD_CS_CFG_REG_T;

//pad_spi_lcd_select_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_spi_lcd_select_sel : 4; // [3:0]
        REG32 pad_spi_lcd_select_ana : 4; // [7:4]
        REG32 pad_spi_lcd_select_pull_dn : 1; // [8]
        REG32 pad_spi_lcd_select_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_spi_lcd_select_pull_frc : 1; // [16]
        REG32 pad_spi_lcd_select_oen_reg : 1; // [17]
        REG32 pad_spi_lcd_select_drv_strength : 2; // [19:18], write set
        REG32 pad_spi_lcd_select_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_spi_lcd_select_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_spi_lcd_select_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SPI_LCD_SELECT_CFG_REG_T;

//pad_lcd_rstb_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lcd_rstb_sel : 4; // [3:0]
        REG32 pad_lcd_rstb_ana : 4; // [7:4]
        REG32 pad_lcd_rstb_pull_dn : 1; // [8]
        REG32 pad_lcd_rstb_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lcd_rstb_pull_frc : 1; // [16]
        REG32 pad_lcd_rstb_oen_reg : 1; // [17]
        REG32 pad_lcd_rstb_drv_strength : 2; // [19:18], write set
        REG32 pad_lcd_rstb_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lcd_rstb_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lcd_rstb_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LCD_RSTB_CFG_REG_T;

//pad_lcd_fmark_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lcd_fmark_sel : 4; // [3:0]
        REG32 pad_lcd_fmark_ana : 4; // [7:4]
        REG32 pad_lcd_fmark_pull_dn : 1; // [8]
        REG32 pad_lcd_fmark_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lcd_fmark_pull_frc : 1; // [16]
        REG32 pad_lcd_fmark_oen_reg : 1; // [17]
        REG32 pad_lcd_fmark_drv_strength : 2; // [19:18], write set
        REG32 pad_lcd_fmark_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lcd_fmark_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lcd_fmark_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LCD_FMARK_CFG_REG_T;

//pad_camera_rst_l_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_rst_l_sel : 4; // [3:0]
        REG32 pad_camera_rst_l_ana : 4; // [7:4]
        REG32 pad_camera_rst_l_pull_dn : 1; // [8]
        REG32 pad_camera_rst_l_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_rst_l_pull_frc : 1; // [16]
        REG32 pad_camera_rst_l_oen_reg : 1; // [17]
        REG32 pad_camera_rst_l_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_rst_l_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_rst_l_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_rst_l_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_RST_L_CFG_REG_T;

//pad_camera_pwdn_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_pwdn_sel : 4; // [3:0]
        REG32 pad_camera_pwdn_ana : 4; // [7:4]
        REG32 pad_camera_pwdn_pull_dn : 1; // [8]
        REG32 pad_camera_pwdn_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_pwdn_pull_frc : 1; // [16]
        REG32 pad_camera_pwdn_oen_reg : 1; // [17]
        REG32 pad_camera_pwdn_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_pwdn_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_pwdn_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_pwdn_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_PWDN_CFG_REG_T;

//pad_camera_ref_clk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_ref_clk_sel : 4; // [3:0]
        REG32 pad_camera_ref_clk_ana : 4; // [7:4]
        REG32 pad_camera_ref_clk_pull_dn : 1; // [8]
        REG32 pad_camera_ref_clk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_ref_clk_pull_frc : 1; // [16]
        REG32 pad_camera_ref_clk_oen_reg : 1; // [17]
        REG32 pad_camera_ref_clk_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_ref_clk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_ref_clk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_ref_clk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_REF_CLK_CFG_REG_T;

//pad_camera_vsync_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_vsync_sel : 4; // [3:0]
        REG32 pad_camera_vsync_ana : 4; // [7:4]
        REG32 pad_camera_vsync_pull_dn : 1; // [8]
        REG32 pad_camera_vsync_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_vsync_pull_frc : 1; // [16]
        REG32 pad_camera_vsync_oen_reg : 1; // [17]
        REG32 pad_camera_vsync_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_vsync_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_vsync_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_vsync_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_VSYNC_CFG_REG_T;

//pad_camera_pix_clk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_pix_clk_sel : 4; // [3:0]
        REG32 pad_camera_pix_clk_ana : 4; // [7:4]
        REG32 pad_camera_pix_clk_pull_dn : 1; // [8]
        REG32 pad_camera_pix_clk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_pix_clk_pull_frc : 1; // [16]
        REG32 pad_camera_pix_clk_oen_reg : 1; // [17]
        REG32 pad_camera_pix_clk_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_pix_clk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_pix_clk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_pix_clk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_PIX_CLK_CFG_REG_T;

//pad_camera_href_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_href_sel : 4; // [3:0]
        REG32 pad_camera_href_ana : 4; // [7:4]
        REG32 pad_camera_href_pull_dn : 1; // [8]
        REG32 pad_camera_href_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_href_pull_frc : 1; // [16]
        REG32 pad_camera_href_oen_reg : 1; // [17]
        REG32 pad_camera_href_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_href_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_href_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_href_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_HREF_CFG_REG_T;

//pad_camera_data_in_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_data_in_0_sel : 4; // [3:0]
        REG32 pad_camera_data_in_0_ana : 4; // [7:4]
        REG32 pad_camera_data_in_0_pull_dn : 1; // [8]
        REG32 pad_camera_data_in_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_data_in_0_pull_frc : 1; // [16]
        REG32 pad_camera_data_in_0_oen_reg : 1; // [17]
        REG32 pad_camera_data_in_0_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_data_in_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_data_in_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_data_in_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_DATA_IN_0_CFG_REG_T;

//pad_camera_data_in_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_data_in_1_sel : 4; // [3:0]
        REG32 pad_camera_data_in_1_ana : 4; // [7:4]
        REG32 pad_camera_data_in_1_pull_dn : 1; // [8]
        REG32 pad_camera_data_in_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_data_in_1_pull_frc : 1; // [16]
        REG32 pad_camera_data_in_1_oen_reg : 1; // [17]
        REG32 pad_camera_data_in_1_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_data_in_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_data_in_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_data_in_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_DATA_IN_1_CFG_REG_T;

//pad_camera_data_in_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_data_in_2_sel : 4; // [3:0]
        REG32 pad_camera_data_in_2_ana : 4; // [7:4]
        REG32 pad_camera_data_in_2_pull_dn : 1; // [8]
        REG32 pad_camera_data_in_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_data_in_2_pull_frc : 1; // [16]
        REG32 pad_camera_data_in_2_oen_reg : 1; // [17]
        REG32 pad_camera_data_in_2_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_data_in_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_data_in_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_data_in_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_DATA_IN_2_CFG_REG_T;

//pad_camera_data_in_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_data_in_3_sel : 4; // [3:0]
        REG32 pad_camera_data_in_3_ana : 4; // [7:4]
        REG32 pad_camera_data_in_3_pull_dn : 1; // [8]
        REG32 pad_camera_data_in_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_data_in_3_pull_frc : 1; // [16]
        REG32 pad_camera_data_in_3_oen_reg : 1; // [17]
        REG32 pad_camera_data_in_3_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_data_in_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_data_in_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_data_in_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_DATA_IN_3_CFG_REG_T;

//pad_camera_data_in_4_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_data_in_4_sel : 4; // [3:0]
        REG32 pad_camera_data_in_4_ana : 4; // [7:4]
        REG32 pad_camera_data_in_4_pull_dn : 1; // [8]
        REG32 pad_camera_data_in_4_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_data_in_4_pull_frc : 1; // [16]
        REG32 pad_camera_data_in_4_oen_reg : 1; // [17]
        REG32 pad_camera_data_in_4_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_data_in_4_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_data_in_4_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_data_in_4_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_DATA_IN_4_CFG_REG_T;

//pad_camera_data_in_5_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_data_in_5_sel : 4; // [3:0]
        REG32 pad_camera_data_in_5_ana : 4; // [7:4]
        REG32 pad_camera_data_in_5_pull_dn : 1; // [8]
        REG32 pad_camera_data_in_5_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_data_in_5_pull_frc : 1; // [16]
        REG32 pad_camera_data_in_5_oen_reg : 1; // [17]
        REG32 pad_camera_data_in_5_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_data_in_5_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_data_in_5_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_data_in_5_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_DATA_IN_5_CFG_REG_T;

//pad_camera_data_in_6_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_data_in_6_sel : 4; // [3:0]
        REG32 pad_camera_data_in_6_ana : 4; // [7:4]
        REG32 pad_camera_data_in_6_pull_dn : 1; // [8]
        REG32 pad_camera_data_in_6_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_data_in_6_pull_frc : 1; // [16]
        REG32 pad_camera_data_in_6_oen_reg : 1; // [17]
        REG32 pad_camera_data_in_6_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_data_in_6_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_data_in_6_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_data_in_6_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_DATA_IN_6_CFG_REG_T;

//pad_camera_data_in_7_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_camera_data_in_7_sel : 4; // [3:0]
        REG32 pad_camera_data_in_7_ana : 4; // [7:4]
        REG32 pad_camera_data_in_7_pull_dn : 1; // [8]
        REG32 pad_camera_data_in_7_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_camera_data_in_7_pull_frc : 1; // [16]
        REG32 pad_camera_data_in_7_oen_reg : 1; // [17]
        REG32 pad_camera_data_in_7_drv_strength : 2; // [19:18], write set
        REG32 pad_camera_data_in_7_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_camera_data_in_7_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_camera_data_in_7_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CAMERA_DATA_IN_7_CFG_REG_T;

//pad_ap_jtag_tck_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_ap_jtag_tck_sel : 4; // [3:0]
        REG32 pad_ap_jtag_tck_ana : 4; // [7:4]
        REG32 pad_ap_jtag_tck_pull_dn : 1; // [8]
        REG32 pad_ap_jtag_tck_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_ap_jtag_tck_pull_frc : 1; // [16]
        REG32 pad_ap_jtag_tck_oen_reg : 1; // [17]
        REG32 pad_ap_jtag_tck_drv_strength : 2; // [19:18], write set
        REG32 pad_ap_jtag_tck_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_ap_jtag_tck_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_ap_jtag_tck_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_AP_JTAG_TCK_CFG_REG_T;

//pad_ap_jtag_trst_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_ap_jtag_trst_sel : 4; // [3:0]
        REG32 pad_ap_jtag_trst_ana : 4; // [7:4]
        REG32 pad_ap_jtag_trst_pull_dn : 1; // [8]
        REG32 pad_ap_jtag_trst_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_ap_jtag_trst_pull_frc : 1; // [16]
        REG32 pad_ap_jtag_trst_oen_reg : 1; // [17]
        REG32 pad_ap_jtag_trst_drv_strength : 2; // [19:18], write set
        REG32 pad_ap_jtag_trst_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_ap_jtag_trst_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_ap_jtag_trst_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_AP_JTAG_TRST_CFG_REG_T;

//pad_ap_jtag_tms_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_ap_jtag_tms_sel : 4; // [3:0]
        REG32 pad_ap_jtag_tms_ana : 4; // [7:4]
        REG32 pad_ap_jtag_tms_pull_dn : 1; // [8]
        REG32 pad_ap_jtag_tms_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_ap_jtag_tms_pull_frc : 1; // [16]
        REG32 pad_ap_jtag_tms_oen_reg : 1; // [17]
        REG32 pad_ap_jtag_tms_drv_strength : 2; // [19:18], write set
        REG32 pad_ap_jtag_tms_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_ap_jtag_tms_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_ap_jtag_tms_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_AP_JTAG_TMS_CFG_REG_T;

//pad_ap_jtag_tdi_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_ap_jtag_tdi_sel : 4; // [3:0]
        REG32 pad_ap_jtag_tdi_ana : 4; // [7:4]
        REG32 pad_ap_jtag_tdi_pull_dn : 1; // [8]
        REG32 pad_ap_jtag_tdi_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_ap_jtag_tdi_pull_frc : 1; // [16]
        REG32 pad_ap_jtag_tdi_oen_reg : 1; // [17]
        REG32 pad_ap_jtag_tdi_drv_strength : 2; // [19:18], write set
        REG32 pad_ap_jtag_tdi_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_ap_jtag_tdi_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_ap_jtag_tdi_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_AP_JTAG_TDI_CFG_REG_T;

//pad_ap_jtag_tdo_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_ap_jtag_tdo_sel : 4; // [3:0]
        REG32 pad_ap_jtag_tdo_ana : 4; // [7:4]
        REG32 pad_ap_jtag_tdo_pull_dn : 1; // [8]
        REG32 pad_ap_jtag_tdo_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_ap_jtag_tdo_pull_frc : 1; // [16]
        REG32 pad_ap_jtag_tdo_oen_reg : 1; // [17]
        REG32 pad_ap_jtag_tdo_drv_strength : 2; // [19:18], write set
        REG32 pad_ap_jtag_tdo_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_ap_jtag_tdo_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_ap_jtag_tdo_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_AP_JTAG_TDO_CFG_REG_T;

//pad_pwl_out_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_pwl_out_1_sel : 4; // [3:0]
        REG32 pad_pwl_out_1_ana : 4; // [7:4]
        REG32 pad_pwl_out_1_pull_dn : 1; // [8]
        REG32 pad_pwl_out_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_pwl_out_1_pull_frc : 1; // [16]
        REG32 pad_pwl_out_1_oen_reg : 1; // [17]
        REG32 pad_pwl_out_1_drv_strength : 2; // [19:18], write set
        REG32 pad_pwl_out_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_pwl_out_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_pwl_out_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_PWL_OUT_1_CFG_REG_T;

//pad_pwl_out_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_pwl_out_0_sel : 4; // [3:0]
        REG32 pad_pwl_out_0_ana : 4; // [7:4]
        REG32 pad_pwl_out_0_pull_dn : 1; // [8]
        REG32 pad_pwl_out_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_pwl_out_0_pull_frc : 1; // [16]
        REG32 pad_pwl_out_0_oen_reg : 1; // [17]
        REG32 pad_pwl_out_0_drv_strength : 2; // [19:18], write set
        REG32 pad_pwl_out_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_pwl_out_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_pwl_out_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_PWL_OUT_0_CFG_REG_T;

//pad_pwm_lpg_out_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_pwm_lpg_out_sel : 4; // [3:0]
        REG32 pad_pwm_lpg_out_ana : 4; // [7:4]
        REG32 pad_pwm_lpg_out_pull_dn : 1; // [8]
        REG32 pad_pwm_lpg_out_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_pwm_lpg_out_pull_frc : 1; // [16]
        REG32 pad_pwm_lpg_out_oen_reg : 1; // [17]
        REG32 pad_pwm_lpg_out_drv_strength : 2; // [19:18], write set
        REG32 pad_pwm_lpg_out_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_pwm_lpg_out_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_pwm_lpg_out_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_PWM_LPG_OUT_CFG_REG_T;

//pad_pwt_out_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_pwt_out_sel : 4; // [3:0]
        REG32 pad_pwt_out_ana : 4; // [7:4]
        REG32 pad_pwt_out_pull_dn : 1; // [8]
        REG32 pad_pwt_out_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_pwt_out_pull_frc : 1; // [16]
        REG32 pad_pwt_out_oen_reg : 1; // [17]
        REG32 pad_pwt_out_drv_strength : 2; // [19:18], write set
        REG32 pad_pwt_out_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_pwt_out_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_pwt_out_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_PWT_OUT_CFG_REG_T;

//pad_gpio_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_0_sel : 4; // [3:0]
        REG32 pad_gpio_0_ana : 4; // [7:4]
        REG32 pad_gpio_0_pull_dn : 1; // [8]
        REG32 pad_gpio_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_0_pull_frc : 1; // [16]
        REG32 pad_gpio_0_oen_reg : 1; // [17]
        REG32 pad_gpio_0_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_0_CFG_REG_T;

//pad_gpio_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_1_sel : 4; // [3:0]
        REG32 pad_gpio_1_ana : 4; // [7:4]
        REG32 pad_gpio_1_pull_dn : 1; // [8]
        REG32 pad_gpio_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_1_pull_frc : 1; // [16]
        REG32 pad_gpio_1_oen_reg : 1; // [17]
        REG32 pad_gpio_1_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_1_CFG_REG_T;

//pad_gpio_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_2_sel : 4; // [3:0]
        REG32 pad_gpio_2_ana : 4; // [7:4]
        REG32 pad_gpio_2_pull_dn : 1; // [8]
        REG32 pad_gpio_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_2_pull_frc : 1; // [16]
        REG32 pad_gpio_2_oen_reg : 1; // [17]
        REG32 pad_gpio_2_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_2_CFG_REG_T;

//pad_gpio_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_3_sel : 4; // [3:0]
        REG32 pad_gpio_3_ana : 4; // [7:4]
        REG32 pad_gpio_3_pull_dn : 1; // [8]
        REG32 pad_gpio_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_3_pull_frc : 1; // [16]
        REG32 pad_gpio_3_oen_reg : 1; // [17]
        REG32 pad_gpio_3_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_3_CFG_REG_T;

//pad_gpio_4_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_4_sel : 4; // [3:0]
        REG32 pad_gpio_4_ana : 4; // [7:4]
        REG32 pad_gpio_4_pull_dn : 1; // [8]
        REG32 pad_gpio_4_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_4_pull_frc : 1; // [16]
        REG32 pad_gpio_4_oen_reg : 1; // [17]
        REG32 pad_gpio_4_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_4_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_4_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_4_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_4_CFG_REG_T;

//pad_gpio_5_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_5_sel : 4; // [3:0]
        REG32 pad_gpio_5_ana : 4; // [7:4]
        REG32 pad_gpio_5_pull_dn : 1; // [8]
        REG32 pad_gpio_5_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_5_pull_frc : 1; // [16]
        REG32 pad_gpio_5_oen_reg : 1; // [17]
        REG32 pad_gpio_5_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_5_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_5_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_5_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_5_CFG_REG_T;

//pad_gpio_10_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_10_sel : 4; // [3:0]
        REG32 pad_gpio_10_ana : 4; // [7:4]
        REG32 pad_gpio_10_pull_dn : 1; // [8]
        REG32 pad_gpio_10_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_10_pull_frc : 1; // [16]
        REG32 pad_gpio_10_oen_reg : 1; // [17]
        REG32 pad_gpio_10_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_10_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_10_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_10_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_10_CFG_REG_T;

//pad_gpio_11_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_11_sel : 4; // [3:0]
        REG32 pad_gpio_11_ana : 4; // [7:4]
        REG32 pad_gpio_11_pull_dn : 1; // [8]
        REG32 pad_gpio_11_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_11_pull_frc : 1; // [16]
        REG32 pad_gpio_11_oen_reg : 1; // [17]
        REG32 pad_gpio_11_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_11_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_11_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_11_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_11_CFG_REG_T;

//pad_gpio_12_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_12_sel : 4; // [3:0]
        REG32 pad_gpio_12_ana : 4; // [7:4]
        REG32 pad_gpio_12_pull_dn : 1; // [8]
        REG32 pad_gpio_12_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_12_pull_frc : 1; // [16]
        REG32 pad_gpio_12_oen_reg : 1; // [17]
        REG32 pad_gpio_12_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_12_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_12_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_12_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_12_CFG_REG_T;

//pad_gpio_6_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_6_sel : 4; // [3:0]
        REG32 pad_gpio_6_ana : 4; // [7:4]
        REG32 pad_gpio_6_pull_dn : 1; // [8]
        REG32 pad_gpio_6_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_6_pull_frc : 1; // [16]
        REG32 pad_gpio_6_oen_reg : 1; // [17]
        REG32 pad_gpio_6_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_6_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_6_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_6_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_6_CFG_REG_T;

//pad_gpio_7_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_7_sel : 4; // [3:0]
        REG32 pad_gpio_7_ana : 4; // [7:4]
        REG32 pad_gpio_7_pull_dn : 1; // [8]
        REG32 pad_gpio_7_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_7_pull_frc : 1; // [16]
        REG32 pad_gpio_7_oen_reg : 1; // [17]
        REG32 pad_gpio_7_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_7_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_7_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_7_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_7_CFG_REG_T;

//pad_gpio_8_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_8_sel : 4; // [3:0]
        REG32 pad_gpio_8_ana : 4; // [7:4]
        REG32 pad_gpio_8_pull_dn : 1; // [8]
        REG32 pad_gpio_8_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_8_pull_frc : 1; // [16]
        REG32 pad_gpio_8_oen_reg : 1; // [17]
        REG32 pad_gpio_8_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_8_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_8_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_8_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_8_CFG_REG_T;

//pad_gpio_9_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpio_9_sel : 4; // [3:0]
        REG32 pad_gpio_9_ana : 4; // [7:4]
        REG32 pad_gpio_9_pull_dn : 1; // [8]
        REG32 pad_gpio_9_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpio_9_pull_frc : 1; // [16]
        REG32 pad_gpio_9_oen_reg : 1; // [17]
        REG32 pad_gpio_9_drv_strength : 2; // [19:18], write set
        REG32 pad_gpio_9_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpio_9_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpio_9_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPIO_9_CFG_REG_T;

//pad_i2c_m4_scl_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_i2c_m4_scl_sel : 4; // [3:0]
        REG32 pad_i2c_m4_scl_ana : 4; // [7:4]
        REG32 pad_i2c_m4_scl_pull_dn : 1; // [8]
        REG32 pad_i2c_m4_scl_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_i2c_m4_scl_pull_frc : 1; // [16]
        REG32 pad_i2c_m4_scl_oen_reg : 1; // [17]
        REG32 pad_i2c_m4_scl_drv_strength : 2; // [19:18], write set
        REG32 pad_i2c_m4_scl_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_i2c_m4_scl_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_i2c_m4_scl_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_I2C_M4_SCL_CFG_REG_T;

//pad_i2c_m4_sda_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_i2c_m4_sda_sel : 4; // [3:0]
        REG32 pad_i2c_m4_sda_ana : 4; // [7:4]
        REG32 pad_i2c_m4_sda_pull_dn : 1; // [8]
        REG32 pad_i2c_m4_sda_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_i2c_m4_sda_pull_frc : 1; // [16]
        REG32 pad_i2c_m4_sda_oen_reg : 1; // [17]
        REG32 pad_i2c_m4_sda_drv_strength : 2; // [19:18], write set
        REG32 pad_i2c_m4_sda_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_i2c_m4_sda_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_i2c_m4_sda_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_I2C_M4_SDA_CFG_REG_T;

//pad_uart_1_rxd_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_1_rxd_sel : 4; // [3:0]
        REG32 pad_uart_1_rxd_ana : 4; // [7:4]
        REG32 pad_uart_1_rxd_pull_dn : 1; // [8]
        REG32 pad_uart_1_rxd_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_uart_1_rxd_pull_frc : 1; // [16]
        REG32 pad_uart_1_rxd_oen_reg : 1; // [17]
        REG32 pad_uart_1_rxd_drv_strength : 2; // [19:18], write set
        REG32 pad_uart_1_rxd_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_uart_1_rxd_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_uart_1_rxd_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_UART_1_RXD_CFG_REG_T;

//pad_uart_1_txd_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_1_txd_sel : 4; // [3:0]
        REG32 pad_uart_1_txd_ana : 4; // [7:4]
        REG32 pad_uart_1_txd_pull_dn : 1; // [8]
        REG32 pad_uart_1_txd_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_uart_1_txd_pull_frc : 1; // [16]
        REG32 pad_uart_1_txd_oen_reg : 1; // [17]
        REG32 pad_uart_1_txd_drv_strength : 2; // [19:18], write set
        REG32 pad_uart_1_txd_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_uart_1_txd_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_uart_1_txd_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_UART_1_TXD_CFG_REG_T;

//pad_uart_1_cts_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_1_cts_sel : 4; // [3:0]
        REG32 pad_uart_1_cts_ana : 4; // [7:4]
        REG32 pad_uart_1_cts_pull_dn : 1; // [8]
        REG32 pad_uart_1_cts_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_uart_1_cts_pull_frc : 1; // [16]
        REG32 pad_uart_1_cts_oen_reg : 1; // [17]
        REG32 pad_uart_1_cts_drv_strength : 2; // [19:18], write set
        REG32 pad_uart_1_cts_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_uart_1_cts_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_uart_1_cts_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_UART_1_CTS_CFG_REG_T;

//pad_uart_1_rts_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_1_rts_sel : 4; // [3:0]
        REG32 pad_uart_1_rts_ana : 4; // [7:4]
        REG32 pad_uart_1_rts_pull_dn : 1; // [8]
        REG32 pad_uart_1_rts_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_uart_1_rts_pull_frc : 1; // [16]
        REG32 pad_uart_1_rts_oen_reg : 1; // [17]
        REG32 pad_uart_1_rts_drv_strength : 2; // [19:18], write set
        REG32 pad_uart_1_rts_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_uart_1_rts_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_uart_1_rts_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_UART_1_RTS_CFG_REG_T;

//pad_uart_2_rxd_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_2_rxd_sel : 4; // [3:0]
        REG32 pad_uart_2_rxd_ana : 4; // [7:4]
        REG32 pad_uart_2_rxd_pull_dn : 1; // [8]
        REG32 pad_uart_2_rxd_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_uart_2_rxd_pull_frc : 1; // [16]
        REG32 pad_uart_2_rxd_oen_reg : 1; // [17]
        REG32 pad_uart_2_rxd_drv_strength : 2; // [19:18], write set
        REG32 pad_uart_2_rxd_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_uart_2_rxd_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_uart_2_rxd_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_UART_2_RXD_CFG_REG_T;

//pad_uart_2_txd_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_2_txd_sel : 4; // [3:0]
        REG32 pad_uart_2_txd_ana : 4; // [7:4]
        REG32 pad_uart_2_txd_pull_dn : 1; // [8]
        REG32 pad_uart_2_txd_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_uart_2_txd_pull_frc : 1; // [16]
        REG32 pad_uart_2_txd_oen_reg : 1; // [17]
        REG32 pad_uart_2_txd_drv_strength : 2; // [19:18], write set
        REG32 pad_uart_2_txd_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_uart_2_txd_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_uart_2_txd_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_UART_2_TXD_CFG_REG_T;

//pad_uart_2_cts_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_2_cts_sel : 4; // [3:0]
        REG32 pad_uart_2_cts_ana : 4; // [7:4]
        REG32 pad_uart_2_cts_pull_dn : 1; // [8]
        REG32 pad_uart_2_cts_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_uart_2_cts_pull_frc : 1; // [16]
        REG32 pad_uart_2_cts_oen_reg : 1; // [17]
        REG32 pad_uart_2_cts_drv_strength : 2; // [19:18], write set
        REG32 pad_uart_2_cts_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_uart_2_cts_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_uart_2_cts_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_UART_2_CTS_CFG_REG_T;

//pad_uart_2_rts_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_uart_2_rts_sel : 4; // [3:0]
        REG32 pad_uart_2_rts_ana : 4; // [7:4]
        REG32 pad_uart_2_rts_pull_dn : 1; // [8]
        REG32 pad_uart_2_rts_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_uart_2_rts_pull_frc : 1; // [16]
        REG32 pad_uart_2_rts_oen_reg : 1; // [17]
        REG32 pad_uart_2_rts_drv_strength : 2; // [19:18], write set
        REG32 pad_uart_2_rts_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_uart_2_rts_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_uart_2_rts_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_UART_2_RTS_CFG_REG_T;

//pad_gpo_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpo_0_sel : 4; // [3:0]
        REG32 pad_gpo_0_ana : 4; // [7:4]
        REG32 pad_gpo_0_pull_dn : 1; // [8]
        REG32 pad_gpo_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpo_0_pull_frc : 1; // [16]
        REG32 pad_gpo_0_oen_reg : 1; // [17]
        REG32 pad_gpo_0_drv_strength : 2; // [19:18], write set
        REG32 pad_gpo_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpo_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpo_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPO_0_CFG_REG_T;

//pad_gpo_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpo_1_sel : 4; // [3:0]
        REG32 pad_gpo_1_ana : 4; // [7:4]
        REG32 pad_gpo_1_pull_dn : 1; // [8]
        REG32 pad_gpo_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpo_1_pull_frc : 1; // [16]
        REG32 pad_gpo_1_oen_reg : 1; // [17]
        REG32 pad_gpo_1_drv_strength : 2; // [19:18], write set
        REG32 pad_gpo_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpo_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpo_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPO_1_CFG_REG_T;

//pad_gpo_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpo_2_sel : 4; // [3:0]
        REG32 pad_gpo_2_ana : 4; // [7:4]
        REG32 pad_gpo_2_pull_dn : 1; // [8]
        REG32 pad_gpo_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpo_2_pull_frc : 1; // [16]
        REG32 pad_gpo_2_oen_reg : 1; // [17]
        REG32 pad_gpo_2_drv_strength : 2; // [19:18], write set
        REG32 pad_gpo_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpo_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpo_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPO_2_CFG_REG_T;

//pad_gpo_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpo_3_sel : 4; // [3:0]
        REG32 pad_gpo_3_ana : 4; // [7:4]
        REG32 pad_gpo_3_pull_dn : 1; // [8]
        REG32 pad_gpo_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpo_3_pull_frc : 1; // [16]
        REG32 pad_gpo_3_oen_reg : 1; // [17]
        REG32 pad_gpo_3_drv_strength : 2; // [19:18], write set
        REG32 pad_gpo_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpo_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpo_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPO_3_CFG_REG_T;

//pad_gpo_4_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_gpo_4_sel : 4; // [3:0]
        REG32 pad_gpo_4_ana : 4; // [7:4]
        REG32 pad_gpo_4_pull_dn : 1; // [8]
        REG32 pad_gpo_4_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_gpo_4_pull_frc : 1; // [16]
        REG32 pad_gpo_4_oen_reg : 1; // [17]
        REG32 pad_gpo_4_drv_strength : 2; // [19:18], write set
        REG32 pad_gpo_4_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_gpo_4_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_gpo_4_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_GPO_4_CFG_REG_T;

//pad_i2c_m1_scl_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_i2c_m1_scl_sel : 4; // [3:0]
        REG32 pad_i2c_m1_scl_ana : 4; // [7:4]
        REG32 pad_i2c_m1_scl_pull_dn : 1; // [8]
        REG32 pad_i2c_m1_scl_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_i2c_m1_scl_pull_frc : 1; // [16]
        REG32 pad_i2c_m1_scl_oen_reg : 1; // [17]
        REG32 pad_i2c_m1_scl_drv_strength : 2; // [19:18], write set
        REG32 pad_i2c_m1_scl_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_i2c_m1_scl_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_i2c_m1_scl_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_I2C_M1_SCL_CFG_REG_T;

//pad_i2c_m1_sda_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_i2c_m1_sda_sel : 4; // [3:0]
        REG32 pad_i2c_m1_sda_ana : 4; // [7:4]
        REG32 pad_i2c_m1_sda_pull_dn : 1; // [8]
        REG32 pad_i2c_m1_sda_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_i2c_m1_sda_pull_frc : 1; // [16]
        REG32 pad_i2c_m1_sda_oen_reg : 1; // [17]
        REG32 pad_i2c_m1_sda_drv_strength : 2; // [19:18], write set
        REG32 pad_i2c_m1_sda_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_i2c_m1_sda_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_i2c_m1_sda_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_I2C_M1_SDA_CFG_REG_T;

//pad_i_keyon_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_i_keyon_sel : 4; // [3:0]
        REG32 pad_i_keyon_ana : 4; // [7:4]
        REG32 pad_i_keyon_pull_dn : 1; // [8]
        REG32 pad_i_keyon_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_i_keyon_pull_frc : 1; // [16]
        REG32 pad_i_keyon_oen_reg : 1; // [17]
        REG32 pad_i_keyon_drv_strength : 2; // [19:18], write set
        REG32 pad_i_keyon_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_i_keyon_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_i_keyon_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_I_KEYON_CFG_REG_T;

//pad_keyin_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyin_0_sel : 4; // [3:0]
        REG32 pad_keyin_0_ana : 4; // [7:4]
        REG32 pad_keyin_0_pull_dn : 1; // [8]
        REG32 pad_keyin_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyin_0_pull_frc : 1; // [16]
        REG32 pad_keyin_0_oen_reg : 1; // [17]
        REG32 pad_keyin_0_drv_strength : 2; // [19:18], write set
        REG32 pad_keyin_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyin_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyin_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYIN_0_CFG_REG_T;

//pad_keyin_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyin_1_sel : 4; // [3:0]
        REG32 pad_keyin_1_ana : 4; // [7:4]
        REG32 pad_keyin_1_pull_dn : 1; // [8]
        REG32 pad_keyin_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyin_1_pull_frc : 1; // [16]
        REG32 pad_keyin_1_oen_reg : 1; // [17]
        REG32 pad_keyin_1_drv_strength : 2; // [19:18], write set
        REG32 pad_keyin_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyin_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyin_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYIN_1_CFG_REG_T;

//pad_keyin_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyin_2_sel : 4; // [3:0]
        REG32 pad_keyin_2_ana : 4; // [7:4]
        REG32 pad_keyin_2_pull_dn : 1; // [8]
        REG32 pad_keyin_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyin_2_pull_frc : 1; // [16]
        REG32 pad_keyin_2_oen_reg : 1; // [17]
        REG32 pad_keyin_2_drv_strength : 2; // [19:18], write set
        REG32 pad_keyin_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyin_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyin_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYIN_2_CFG_REG_T;

//pad_keyin_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyin_3_sel : 4; // [3:0]
        REG32 pad_keyin_3_ana : 4; // [7:4]
        REG32 pad_keyin_3_pull_dn : 1; // [8]
        REG32 pad_keyin_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyin_3_pull_frc : 1; // [16]
        REG32 pad_keyin_3_oen_reg : 1; // [17]
        REG32 pad_keyin_3_drv_strength : 2; // [19:18], write set
        REG32 pad_keyin_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyin_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyin_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYIN_3_CFG_REG_T;

//pad_keyin_4_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyin_4_sel : 4; // [3:0]
        REG32 pad_keyin_4_ana : 4; // [7:4]
        REG32 pad_keyin_4_pull_dn : 1; // [8]
        REG32 pad_keyin_4_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyin_4_pull_frc : 1; // [16]
        REG32 pad_keyin_4_oen_reg : 1; // [17]
        REG32 pad_keyin_4_drv_strength : 2; // [19:18], write set
        REG32 pad_keyin_4_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyin_4_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyin_4_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYIN_4_CFG_REG_T;

//pad_keyin_5_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyin_5_sel : 4; // [3:0]
        REG32 pad_keyin_5_ana : 4; // [7:4]
        REG32 pad_keyin_5_pull_dn : 1; // [8]
        REG32 pad_keyin_5_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyin_5_pull_frc : 1; // [16]
        REG32 pad_keyin_5_oen_reg : 1; // [17]
        REG32 pad_keyin_5_drv_strength : 2; // [19:18], write set
        REG32 pad_keyin_5_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyin_5_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyin_5_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYIN_5_CFG_REG_T;

//pad_keyout_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyout_0_sel : 4; // [3:0]
        REG32 pad_keyout_0_ana : 4; // [7:4]
        REG32 pad_keyout_0_pull_dn : 1; // [8]
        REG32 pad_keyout_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyout_0_pull_frc : 1; // [16]
        REG32 pad_keyout_0_oen_reg : 1; // [17]
        REG32 pad_keyout_0_drv_strength : 2; // [19:18], write set
        REG32 pad_keyout_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyout_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyout_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYOUT_0_CFG_REG_T;

//pad_keyout_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyout_1_sel : 4; // [3:0]
        REG32 pad_keyout_1_ana : 4; // [7:4]
        REG32 pad_keyout_1_pull_dn : 1; // [8]
        REG32 pad_keyout_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyout_1_pull_frc : 1; // [16]
        REG32 pad_keyout_1_oen_reg : 1; // [17]
        REG32 pad_keyout_1_drv_strength : 2; // [19:18], write set
        REG32 pad_keyout_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyout_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyout_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYOUT_1_CFG_REG_T;

//pad_keyout_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyout_2_sel : 4; // [3:0]
        REG32 pad_keyout_2_ana : 4; // [7:4]
        REG32 pad_keyout_2_pull_dn : 1; // [8]
        REG32 pad_keyout_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyout_2_pull_frc : 1; // [16]
        REG32 pad_keyout_2_oen_reg : 1; // [17]
        REG32 pad_keyout_2_drv_strength : 2; // [19:18], write set
        REG32 pad_keyout_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyout_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyout_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYOUT_2_CFG_REG_T;

//pad_keyout_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyout_3_sel : 4; // [3:0]
        REG32 pad_keyout_3_ana : 4; // [7:4]
        REG32 pad_keyout_3_pull_dn : 1; // [8]
        REG32 pad_keyout_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyout_3_pull_frc : 1; // [16]
        REG32 pad_keyout_3_oen_reg : 1; // [17]
        REG32 pad_keyout_3_drv_strength : 2; // [19:18], write set
        REG32 pad_keyout_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyout_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyout_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYOUT_3_CFG_REG_T;

//pad_keyout_4_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyout_4_sel : 4; // [3:0]
        REG32 pad_keyout_4_ana : 4; // [7:4]
        REG32 pad_keyout_4_pull_dn : 1; // [8]
        REG32 pad_keyout_4_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyout_4_pull_frc : 1; // [16]
        REG32 pad_keyout_4_oen_reg : 1; // [17]
        REG32 pad_keyout_4_drv_strength : 2; // [19:18], write set
        REG32 pad_keyout_4_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyout_4_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyout_4_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYOUT_4_CFG_REG_T;

//pad_keyout_5_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_keyout_5_sel : 4; // [3:0]
        REG32 pad_keyout_5_ana : 4; // [7:4]
        REG32 pad_keyout_5_pull_dn : 1; // [8]
        REG32 pad_keyout_5_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_keyout_5_pull_frc : 1; // [16]
        REG32 pad_keyout_5_oen_reg : 1; // [17]
        REG32 pad_keyout_5_drv_strength : 2; // [19:18], write set
        REG32 pad_keyout_5_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_keyout_5_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_keyout_5_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_KEYOUT_5_CFG_REG_T;

//pad_debug_host_rx_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_debug_host_rx_sel : 4; // [3:0]
        REG32 pad_debug_host_rx_ana : 4; // [7:4]
        REG32 pad_debug_host_rx_pull_dn : 1; // [8]
        REG32 pad_debug_host_rx_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_debug_host_rx_pull_frc : 1; // [16]
        REG32 pad_debug_host_rx_oen_reg : 1; // [17]
        REG32 pad_debug_host_rx_drv_strength : 2; // [19:18], write set
        REG32 pad_debug_host_rx_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_debug_host_rx_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_debug_host_rx_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_DEBUG_HOST_RX_CFG_REG_T;

//pad_debug_host_tx_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_debug_host_tx_sel : 4; // [3:0]
        REG32 pad_debug_host_tx_ana : 4; // [7:4]
        REG32 pad_debug_host_tx_pull_dn : 1; // [8]
        REG32 pad_debug_host_tx_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_debug_host_tx_pull_frc : 1; // [16]
        REG32 pad_debug_host_tx_oen_reg : 1; // [17]
        REG32 pad_debug_host_tx_drv_strength : 2; // [19:18], write set
        REG32 pad_debug_host_tx_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_debug_host_tx_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_debug_host_tx_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_DEBUG_HOST_TX_CFG_REG_T;

//pad_debug_host_clk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_debug_host_clk_sel : 4; // [3:0]
        REG32 pad_debug_host_clk_ana : 4; // [7:4]
        REG32 pad_debug_host_clk_pull_dn : 1; // [8]
        REG32 pad_debug_host_clk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_debug_host_clk_pull_frc : 1; // [16]
        REG32 pad_debug_host_clk_oen_reg : 1; // [17]
        REG32 pad_debug_host_clk_drv_strength : 2; // [19:18], write set
        REG32 pad_debug_host_clk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_debug_host_clk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_debug_host_clk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_DEBUG_HOST_CLK_CFG_REG_T;

//pad_sim_1_clk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sim_1_clk_sel : 4; // [3:0]
        REG32 pad_sim_1_clk_ana : 4; // [7:4]
        REG32 pad_sim_1_clk_pull_dn : 1; // [8]
        REG32 pad_sim_1_clk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sim_1_clk_pull_frc : 1; // [16]
        REG32 pad_sim_1_clk_oen_reg : 1; // [17]
        REG32 pad_sim_1_clk_drv_strength : 2; // [19:18], write set
        REG32 pad_sim_1_clk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sim_1_clk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sim_1_clk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SIM_1_CLK_CFG_REG_T;

//pad_sim_1_dio_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sim_1_dio_sel : 4; // [3:0]
        REG32 pad_sim_1_dio_ana : 4; // [7:4]
        REG32 pad_sim_1_dio_pull_dn : 1; // [8]
        REG32 pad_sim_1_dio_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sim_1_dio_pull_frc : 1; // [16]
        REG32 pad_sim_1_dio_oen_reg : 1; // [17]
        REG32 pad_sim_1_dio_drv_strength : 2; // [19:18], write set
        REG32 pad_sim_1_dio_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sim_1_dio_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sim_1_dio_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SIM_1_DIO_CFG_REG_T;

//pad_sim_1_rst_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sim_1_rst_sel : 4; // [3:0]
        REG32 pad_sim_1_rst_ana : 4; // [7:4]
        REG32 pad_sim_1_rst_pull_dn : 1; // [8]
        REG32 pad_sim_1_rst_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sim_1_rst_pull_frc : 1; // [16]
        REG32 pad_sim_1_rst_oen_reg : 1; // [17]
        REG32 pad_sim_1_rst_drv_strength : 2; // [19:18], write set
        REG32 pad_sim_1_rst_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sim_1_rst_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sim_1_rst_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SIM_1_RST_CFG_REG_T;

//pad_sim_2_clk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sim_2_clk_sel : 4; // [3:0]
        REG32 pad_sim_2_clk_ana : 4; // [7:4]
        REG32 pad_sim_2_clk_pull_dn : 1; // [8]
        REG32 pad_sim_2_clk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sim_2_clk_pull_frc : 1; // [16]
        REG32 pad_sim_2_clk_oen_reg : 1; // [17]
        REG32 pad_sim_2_clk_drv_strength : 2; // [19:18], write set
        REG32 pad_sim_2_clk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sim_2_clk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sim_2_clk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SIM_2_CLK_CFG_REG_T;

//pad_sim_2_dio_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sim_2_dio_sel : 4; // [3:0]
        REG32 pad_sim_2_dio_ana : 4; // [7:4]
        REG32 pad_sim_2_dio_pull_dn : 1; // [8]
        REG32 pad_sim_2_dio_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sim_2_dio_pull_frc : 1; // [16]
        REG32 pad_sim_2_dio_oen_reg : 1; // [17]
        REG32 pad_sim_2_dio_drv_strength : 2; // [19:18], write set
        REG32 pad_sim_2_dio_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sim_2_dio_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sim_2_dio_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SIM_2_DIO_CFG_REG_T;

//pad_sim_2_rst_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_sim_2_rst_sel : 4; // [3:0]
        REG32 pad_sim_2_rst_ana : 4; // [7:4]
        REG32 pad_sim_2_rst_pull_dn : 1; // [8]
        REG32 pad_sim_2_rst_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_sim_2_rst_pull_frc : 1; // [16]
        REG32 pad_sim_2_rst_oen_reg : 1; // [17]
        REG32 pad_sim_2_rst_drv_strength : 2; // [19:18], write set
        REG32 pad_sim_2_rst_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_sim_2_rst_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_sim_2_rst_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_SIM_2_RST_CFG_REG_T;

//pad_lte_reset_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_reset_sel : 4; // [3:0]
        REG32 pad_lte_reset_ana : 4; // [7:4]
        REG32 pad_lte_reset_pull_dn : 1; // [8]
        REG32 pad_lte_reset_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_reset_pull_frc : 1; // [16]
        REG32 pad_lte_reset_oen_reg : 1; // [17]
        REG32 pad_lte_reset_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_reset_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_reset_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_reset_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RESET_CFG_REG_T;

//pad_lte_sysclken_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_sysclken_sel : 4; // [3:0]
        REG32 pad_lte_sysclken_ana : 4; // [7:4]
        REG32 pad_lte_sysclken_pull_dn : 1; // [8]
        REG32 pad_lte_sysclken_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_sysclken_pull_frc : 1; // [16]
        REG32 pad_lte_sysclken_oen_reg : 1; // [17]
        REG32 pad_lte_sysclken_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_sysclken_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_sysclken_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_sysclken_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_SYSCLKEN_CFG_REG_T;

//pad_lte_rfspi_cs_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rfspi_cs_sel : 4; // [3:0]
        REG32 pad_lte_rfspi_cs_ana : 4; // [7:4]
        REG32 pad_lte_rfspi_cs_pull_dn : 1; // [8]
        REG32 pad_lte_rfspi_cs_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rfspi_cs_pull_frc : 1; // [16]
        REG32 pad_lte_rfspi_cs_oen_reg : 1; // [17]
        REG32 pad_lte_rfspi_cs_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rfspi_cs_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rfspi_cs_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rfspi_cs_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RFSPI_CS_CFG_REG_T;

//pad_lte_rfspi_clk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rfspi_clk_sel : 4; // [3:0]
        REG32 pad_lte_rfspi_clk_ana : 4; // [7:4]
        REG32 pad_lte_rfspi_clk_pull_dn : 1; // [8]
        REG32 pad_lte_rfspi_clk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rfspi_clk_pull_frc : 1; // [16]
        REG32 pad_lte_rfspi_clk_oen_reg : 1; // [17]
        REG32 pad_lte_rfspi_clk_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rfspi_clk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rfspi_clk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rfspi_clk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RFSPI_CLK_CFG_REG_T;

//pad_lte_rfspi_dio_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rfspi_dio_sel : 4; // [3:0]
        REG32 pad_lte_rfspi_dio_ana : 4; // [7:4]
        REG32 pad_lte_rfspi_dio_pull_dn : 1; // [8]
        REG32 pad_lte_rfspi_dio_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rfspi_dio_pull_frc : 1; // [16]
        REG32 pad_lte_rfspi_dio_oen_reg : 1; // [17]
        REG32 pad_lte_rfspi_dio_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rfspi_dio_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rfspi_dio_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rfspi_dio_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RFSPI_DIO_CFG_REG_T;

//pad_lte_rbdp_rx_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_0_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_0_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_0_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_0_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_0_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_0_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_0_CFG_REG_T;

//pad_lte_rbdp_rx_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_1_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_1_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_1_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_1_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_1_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_1_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_1_CFG_REG_T;

//pad_lte_rbdp_rx_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_2_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_2_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_2_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_2_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_2_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_2_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_2_CFG_REG_T;

//pad_lte_rbdp_rx_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_3_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_3_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_3_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_3_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_3_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_3_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_3_CFG_REG_T;

//pad_lte_rbdp_rx_4_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_4_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_4_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_4_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_4_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_4_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_4_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_4_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_4_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_4_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_4_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_4_CFG_REG_T;

//pad_lte_rbdp_rx_5_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_5_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_5_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_5_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_5_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_5_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_5_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_5_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_5_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_5_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_5_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_5_CFG_REG_T;

//pad_lte_rbdp_rx_6_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_6_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_6_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_6_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_6_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_6_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_6_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_6_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_6_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_6_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_6_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_6_CFG_REG_T;

//pad_lte_rbdp_rx_7_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_7_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_7_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_7_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_7_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_7_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_7_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_7_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_7_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_7_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_7_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_7_CFG_REG_T;

//pad_lte_rbdp_rx_8_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_8_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_8_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_8_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_8_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_8_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_8_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_8_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_8_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_8_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_8_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_8_CFG_REG_T;

//pad_lte_rbdp_rx_9_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_9_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_9_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_9_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_9_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_9_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_9_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_9_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_9_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_9_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_9_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_9_CFG_REG_T;

//pad_lte_rbdp_rx_10_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_10_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_10_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_10_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_10_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_10_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_10_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_10_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_10_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_10_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_10_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_10_CFG_REG_T;

//pad_lte_rbdp_rx_11_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_rx_11_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_rx_11_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_rx_11_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_rx_11_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_rx_11_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_rx_11_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_rx_11_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_rx_11_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_rx_11_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_rx_11_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_RX_11_CFG_REG_T;

//pad_lte_rbdp_tx_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_0_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_0_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_0_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_0_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_0_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_0_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_0_CFG_REG_T;

//pad_lte_rbdp_tx_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_1_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_1_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_1_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_1_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_1_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_1_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_1_CFG_REG_T;

//pad_lte_rbdp_tx_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_2_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_2_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_2_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_2_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_2_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_2_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_2_CFG_REG_T;

//pad_lte_rbdp_tx_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_3_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_3_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_3_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_3_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_3_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_3_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_3_CFG_REG_T;

//pad_lte_rbdp_tx_4_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_4_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_4_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_4_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_4_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_4_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_4_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_4_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_4_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_4_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_4_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_4_CFG_REG_T;

//pad_lte_rbdp_tx_5_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_5_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_5_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_5_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_5_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_5_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_5_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_5_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_5_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_5_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_5_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_5_CFG_REG_T;

//pad_lte_rbdp_tx_6_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_6_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_6_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_6_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_6_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_6_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_6_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_6_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_6_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_6_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_6_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_6_CFG_REG_T;

//pad_lte_rbdp_tx_7_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_7_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_7_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_7_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_7_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_7_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_7_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_7_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_7_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_7_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_7_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_7_CFG_REG_T;

//pad_lte_rbdp_tx_8_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_8_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_8_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_8_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_8_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_8_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_8_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_8_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_8_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_8_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_8_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_8_CFG_REG_T;

//pad_lte_rbdp_tx_9_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_9_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_9_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_9_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_9_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_9_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_9_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_9_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_9_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_9_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_9_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_9_CFG_REG_T;

//pad_lte_rbdp_tx_10_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_10_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_10_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_10_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_10_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_10_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_10_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_10_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_10_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_10_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_10_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_10_CFG_REG_T;

//pad_lte_rbdp_tx_11_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rbdp_tx_11_sel : 4; // [3:0]
        REG32 pad_lte_rbdp_tx_11_ana : 4; // [7:4]
        REG32 pad_lte_rbdp_tx_11_pull_dn : 1; // [8]
        REG32 pad_lte_rbdp_tx_11_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rbdp_tx_11_pull_frc : 1; // [16]
        REG32 pad_lte_rbdp_tx_11_oen_reg : 1; // [17]
        REG32 pad_lte_rbdp_tx_11_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rbdp_tx_11_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rbdp_tx_11_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rbdp_tx_11_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RBDP_TX_11_CFG_REG_T;

//pad_lte_mclk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_mclk_sel : 4; // [3:0]
        REG32 pad_lte_mclk_ana : 4; // [7:4]
        REG32 pad_lte_mclk_pull_dn : 1; // [8]
        REG32 pad_lte_mclk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_mclk_pull_frc : 1; // [16]
        REG32 pad_lte_mclk_oen_reg : 1; // [17]
        REG32 pad_lte_mclk_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_mclk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_mclk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_mclk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_MCLK_CFG_REG_T;

//pad_lte_rxframe_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rxframe_sel : 4; // [3:0]
        REG32 pad_lte_rxframe_ana : 4; // [7:4]
        REG32 pad_lte_rxframe_pull_dn : 1; // [8]
        REG32 pad_lte_rxframe_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rxframe_pull_frc : 1; // [16]
        REG32 pad_lte_rxframe_oen_reg : 1; // [17]
        REG32 pad_lte_rxframe_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rxframe_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rxframe_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rxframe_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RXFRAME_CFG_REG_T;

//pad_lte_fclk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_fclk_sel : 4; // [3:0]
        REG32 pad_lte_fclk_ana : 4; // [7:4]
        REG32 pad_lte_fclk_pull_dn : 1; // [8]
        REG32 pad_lte_fclk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_fclk_pull_frc : 1; // [16]
        REG32 pad_lte_fclk_oen_reg : 1; // [17]
        REG32 pad_lte_fclk_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_fclk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_fclk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_fclk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_FCLK_CFG_REG_T;

//pad_lte_txframe_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_txframe_sel : 4; // [3:0]
        REG32 pad_lte_txframe_ana : 4; // [7:4]
        REG32 pad_lte_txframe_pull_dn : 1; // [8]
        REG32 pad_lte_txframe_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_txframe_pull_frc : 1; // [16]
        REG32 pad_lte_txframe_oen_reg : 1; // [17]
        REG32 pad_lte_txframe_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_txframe_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_txframe_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_txframe_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_TXFRAME_CFG_REG_T;

//pad_lte_rxtxen_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rxtxen_sel : 4; // [3:0]
        REG32 pad_lte_rxtxen_ana : 4; // [7:4]
        REG32 pad_lte_rxtxen_pull_dn : 1; // [8]
        REG32 pad_lte_rxtxen_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rxtxen_pull_frc : 1; // [16]
        REG32 pad_lte_rxtxen_oen_reg : 1; // [17]
        REG32 pad_lte_rxtxen_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rxtxen_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rxtxen_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rxtxen_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RXTXEN_CFG_REG_T;

//pad_lte_rxtxdata_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_lte_rxtxdata_sel : 4; // [3:0]
        REG32 pad_lte_rxtxdata_ana : 4; // [7:4]
        REG32 pad_lte_rxtxdata_pull_dn : 1; // [8]
        REG32 pad_lte_rxtxdata_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_lte_rxtxdata_pull_frc : 1; // [16]
        REG32 pad_lte_rxtxdata_oen_reg : 1; // [17]
        REG32 pad_lte_rxtxdata_drv_strength : 2; // [19:18], write set
        REG32 pad_lte_rxtxdata_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_lte_rxtxdata_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_lte_rxtxdata_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_LTE_RXTXDATA_CFG_REG_T;

//pad_rfdig_gpio_0_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_0_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_0_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_0_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_0_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_0_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_0_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_0_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_0_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_0_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_0_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_0_CFG_REG_T;

//pad_rfdig_gpio_1_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_1_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_1_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_1_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_1_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_1_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_1_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_1_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_1_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_1_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_1_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_1_CFG_REG_T;

//pad_rfdig_gpio_2_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_2_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_2_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_2_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_2_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_2_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_2_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_2_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_2_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_2_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_2_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_2_CFG_REG_T;

//pad_rfdig_gpio_3_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_3_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_3_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_3_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_3_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_3_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_3_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_3_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_3_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_3_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_3_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_3_CFG_REG_T;

//pad_rfdig_gpio_4_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_4_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_4_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_4_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_4_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_4_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_4_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_4_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_4_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_4_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_4_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_4_CFG_REG_T;

//pad_rfdig_gpio_5_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_5_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_5_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_5_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_5_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_5_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_5_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_5_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_5_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_5_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_5_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_5_CFG_REG_T;

//pad_rfdig_gpio_6_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_6_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_6_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_6_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_6_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_6_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_6_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_6_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_6_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_6_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_6_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_6_CFG_REG_T;

//pad_rfdig_gpio_7_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_7_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_7_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_7_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_7_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_7_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_7_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_7_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_7_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_7_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_7_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_7_CFG_REG_T;

//pad_rfdig_gpio_8_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_8_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_8_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_8_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_8_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_8_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_8_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_8_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_8_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_8_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_8_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_8_CFG_REG_T;

//pad_rfdig_gpio_9_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rfdig_gpio_9_sel : 4; // [3:0]
        REG32 pad_rfdig_gpio_9_ana : 4; // [7:4]
        REG32 pad_rfdig_gpio_9_pull_dn : 1; // [8]
        REG32 pad_rfdig_gpio_9_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rfdig_gpio_9_pull_frc : 1; // [16]
        REG32 pad_rfdig_gpio_9_oen_reg : 1; // [17]
        REG32 pad_rfdig_gpio_9_drv_strength : 2; // [19:18], write set
        REG32 pad_rfdig_gpio_9_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rfdig_gpio_9_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rfdig_gpio_9_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_9_CFG_REG_T;

//pad_rf_gge_xen_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rf_gge_xen_sel : 4; // [3:0]
        REG32 pad_rf_gge_xen_ana : 4; // [7:4]
        REG32 pad_rf_gge_xen_pull_dn : 1; // [8]
        REG32 pad_rf_gge_xen_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rf_gge_xen_pull_frc : 1; // [16]
        REG32 pad_rf_gge_xen_oen_reg : 1; // [17]
        REG32 pad_rf_gge_xen_drv_strength : 2; // [19:18], write set
        REG32 pad_rf_gge_xen_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rf_gge_xen_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rf_gge_xen_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RF_GGE_XEN_CFG_REG_T;

//pad_rf_gge_pdn_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rf_gge_pdn_sel : 4; // [3:0]
        REG32 pad_rf_gge_pdn_ana : 4; // [7:4]
        REG32 pad_rf_gge_pdn_pull_dn : 1; // [8]
        REG32 pad_rf_gge_pdn_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rf_gge_pdn_pull_frc : 1; // [16]
        REG32 pad_rf_gge_pdn_oen_reg : 1; // [17]
        REG32 pad_rf_gge_pdn_drv_strength : 2; // [19:18], write set
        REG32 pad_rf_gge_pdn_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rf_gge_pdn_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rf_gge_pdn_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RF_GGE_PDN_CFG_REG_T;

//pad_rf_gge_rfspi_cs_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rf_gge_rfspi_cs_sel : 4; // [3:0]
        REG32 pad_rf_gge_rfspi_cs_ana : 4; // [7:4]
        REG32 pad_rf_gge_rfspi_cs_pull_dn : 1; // [8]
        REG32 pad_rf_gge_rfspi_cs_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rf_gge_rfspi_cs_pull_frc : 1; // [16]
        REG32 pad_rf_gge_rfspi_cs_oen_reg : 1; // [17]
        REG32 pad_rf_gge_rfspi_cs_drv_strength : 2; // [19:18], write set
        REG32 pad_rf_gge_rfspi_cs_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rf_gge_rfspi_cs_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rf_gge_rfspi_cs_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RF_GGE_RFSPI_CS_CFG_REG_T;

//pad_rf_gge_rfspi_sclk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rf_gge_rfspi_sclk_sel : 4; // [3:0]
        REG32 pad_rf_gge_rfspi_sclk_ana : 4; // [7:4]
        REG32 pad_rf_gge_rfspi_sclk_pull_dn : 1; // [8]
        REG32 pad_rf_gge_rfspi_sclk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rf_gge_rfspi_sclk_pull_frc : 1; // [16]
        REG32 pad_rf_gge_rfspi_sclk_oen_reg : 1; // [17]
        REG32 pad_rf_gge_rfspi_sclk_drv_strength : 2; // [19:18], write set
        REG32 pad_rf_gge_rfspi_sclk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rf_gge_rfspi_sclk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rf_gge_rfspi_sclk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RF_GGE_RFSPI_SCLK_CFG_REG_T;

//pad_rf_gge_rfspi_sdio_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rf_gge_rfspi_sdio_sel : 4; // [3:0]
        REG32 pad_rf_gge_rfspi_sdio_ana : 4; // [7:4]
        REG32 pad_rf_gge_rfspi_sdio_pull_dn : 1; // [8]
        REG32 pad_rf_gge_rfspi_sdio_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rf_gge_rfspi_sdio_pull_frc : 1; // [16]
        REG32 pad_rf_gge_rfspi_sdio_oen_reg : 1; // [17]
        REG32 pad_rf_gge_rfspi_sdio_drv_strength : 2; // [19:18], write set
        REG32 pad_rf_gge_rfspi_sdio_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rf_gge_rfspi_sdio_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rf_gge_rfspi_sdio_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RF_GGE_RFSPI_SDIO_CFG_REG_T;

//pad_rf_gge_strobe_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_rf_gge_strobe_sel : 4; // [3:0]
        REG32 pad_rf_gge_strobe_ana : 4; // [7:4]
        REG32 pad_rf_gge_strobe_pull_dn : 1; // [8]
        REG32 pad_rf_gge_strobe_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_rf_gge_strobe_pull_frc : 1; // [16]
        REG32 pad_rf_gge_strobe_oen_reg : 1; // [17]
        REG32 pad_rf_gge_strobe_drv_strength : 2; // [19:18], write set
        REG32 pad_rf_gge_strobe_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_rf_gge_strobe_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_rf_gge_strobe_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_RF_GGE_STROBE_CFG_REG_T;

//pad_codec_mclk_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_codec_mclk_sel : 4; // [3:0]
        REG32 pad_codec_mclk_ana : 4; // [7:4]
        REG32 pad_codec_mclk_pull_dn : 1; // [8]
        REG32 pad_codec_mclk_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_codec_mclk_pull_frc : 1; // [16]
        REG32 pad_codec_mclk_oen_reg : 1; // [17]
        REG32 pad_codec_mclk_drv_strength : 2; // [19:18], write set
        REG32 pad_codec_mclk_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_codec_mclk_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_codec_mclk_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CODEC_MCLK_CFG_REG_T;

//pad_chip_pd_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 pad_chip_pd_sel : 4; // [3:0]
        REG32 pad_chip_pd_ana : 4; // [7:4]
        REG32 pad_chip_pd_pull_dn : 1; // [8]
        REG32 pad_chip_pd_pull_up : 1; // [9]
        REG32 __15_10 : 6;
        REG32 pad_chip_pd_pull_frc : 1; // [16]
        REG32 pad_chip_pd_oen_reg : 1; // [17]
        REG32 pad_chip_pd_drv_strength : 2; // [19:18], write set
        REG32 pad_chip_pd_out_reg : 1; // [20]
        REG32 __23_21 : 3;
        REG32 pad_chip_pd_out_frc : 1; // [24]
        REG32 __27_25 : 3;
        REG32 pad_chip_pd_oen_frc : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_IOMUX_PAD_CHIP_PD_CFG_REG_T;


//pad_spi_flash_clk_cfg_reg
#define IOMUX_PAD_SPI_FLASH_CLK_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_FLASH_CLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_FLASH_CLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_FLASH_CLK_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_FLASH_CLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_FLASH_CLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_FLASH_CLK_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_FLASH_CLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_FLASH_CLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_FLASH_CLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_FLASH_CLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_FLASH_CLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_FLASH_CLK_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_FLASH_CLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_FLASH_CLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_FLASH_CLK_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_FLASH_CLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_FLASH_CLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_FLASH_CLK_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_FLASH_CLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_FLASH_CLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_FLASH_CLK_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_FLASH_CLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_FLASH_CLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_FLASH_CLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_FLASH_CLK_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_FLASH_CLK_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_FLASH_CLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_FLASH_CLK_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_FLASH_CLK_SEL_SHIFT (0)

//pad_spi_flash_cs0_cfg_reg
#define IOMUX_PAD_SPI_FLASH_CS0_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_FLASH_CS0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_FLASH_CS0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_FLASH_CS0_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_FLASH_CS0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_FLASH_CS0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_FLASH_CS0_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_FLASH_CS0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_FLASH_CS0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_FLASH_CS0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_FLASH_CS0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_FLASH_CS0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_FLASH_CS0_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_FLASH_CS0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_FLASH_CS0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_FLASH_CS0_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_FLASH_CS0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_FLASH_CS0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_FLASH_CS0_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_FLASH_CS0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_FLASH_CS0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_FLASH_CS0_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_FLASH_CS0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_FLASH_CS0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_FLASH_CS0_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_FLASH_CS0_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_FLASH_CS0_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_FLASH_CS0_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_FLASH_CS0_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_FLASH_CS0_SEL_SHIFT (0)

//pad_spi_flash_cs1_cfg_reg
#define IOMUX_PAD_SPI_FLASH_CS1_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_FLASH_CS1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_FLASH_CS1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_FLASH_CS1_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_FLASH_CS1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_FLASH_CS1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_FLASH_CS1_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_FLASH_CS1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_FLASH_CS1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_FLASH_CS1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_FLASH_CS1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_FLASH_CS1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_FLASH_CS1_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_FLASH_CS1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_FLASH_CS1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_FLASH_CS1_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_FLASH_CS1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_FLASH_CS1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_FLASH_CS1_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_FLASH_CS1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_FLASH_CS1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_FLASH_CS1_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_FLASH_CS1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_FLASH_CS1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_FLASH_CS1_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_FLASH_CS1_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_FLASH_CS1_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_FLASH_CS1_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_FLASH_CS1_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_FLASH_CS1_SEL_SHIFT (0)

//pad_spi_flash_sio_0_cfg_reg
#define IOMUX_PAD_SPI_FLASH_SIO_0_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_FLASH_SIO_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_FLASH_SIO_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_FLASH_SIO_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_FLASH_SIO_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_FLASH_SIO_0_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_FLASH_SIO_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_FLASH_SIO_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_FLASH_SIO_0_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_FLASH_SIO_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_FLASH_SIO_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_FLASH_SIO_0_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_FLASH_SIO_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_FLASH_SIO_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_FLASH_SIO_0_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_FLASH_SIO_0_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_FLASH_SIO_0_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_FLASH_SIO_0_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_FLASH_SIO_0_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_FLASH_SIO_0_SEL_SHIFT (0)

//pad_spi_flash_sio_1_cfg_reg
#define IOMUX_PAD_SPI_FLASH_SIO_1_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_FLASH_SIO_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_FLASH_SIO_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_FLASH_SIO_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_FLASH_SIO_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_FLASH_SIO_1_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_FLASH_SIO_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_FLASH_SIO_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_FLASH_SIO_1_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_FLASH_SIO_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_FLASH_SIO_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_FLASH_SIO_1_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_FLASH_SIO_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_FLASH_SIO_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_FLASH_SIO_1_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_FLASH_SIO_1_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_FLASH_SIO_1_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_FLASH_SIO_1_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_FLASH_SIO_1_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_FLASH_SIO_1_SEL_SHIFT (0)

//pad_spi_flash_sio_2_cfg_reg
#define IOMUX_PAD_SPI_FLASH_SIO_2_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_FLASH_SIO_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_FLASH_SIO_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_FLASH_SIO_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_FLASH_SIO_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_FLASH_SIO_2_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_FLASH_SIO_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_FLASH_SIO_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_FLASH_SIO_2_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_FLASH_SIO_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_FLASH_SIO_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_FLASH_SIO_2_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_FLASH_SIO_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_FLASH_SIO_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_FLASH_SIO_2_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_FLASH_SIO_2_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_FLASH_SIO_2_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_FLASH_SIO_2_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_FLASH_SIO_2_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_FLASH_SIO_2_SEL_SHIFT (0)

//pad_spi_flash_sio_3_cfg_reg
#define IOMUX_PAD_SPI_FLASH_SIO_3_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_FLASH_SIO_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_FLASH_SIO_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_FLASH_SIO_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_FLASH_SIO_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_FLASH_SIO_3_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_FLASH_SIO_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_FLASH_SIO_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_FLASH_SIO_3_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_FLASH_SIO_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_FLASH_SIO_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_FLASH_SIO_3_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_FLASH_SIO_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_FLASH_SIO_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_FLASH_SIO_3_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_FLASH_SIO_3_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_FLASH_SIO_3_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_FLASH_SIO_3_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_FLASH_SIO_3_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_FLASH_SIO_3_SEL_SHIFT (0)

//pad_sdmmc1_clk_cfg_reg
#define IOMUX_PAD_SDMMC1_CLK_OEN_FRC (1<<28)
#define IOMUX_PAD_SDMMC1_CLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SDMMC1_CLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SDMMC1_CLK_OUT_FRC (1<<24)
#define IOMUX_PAD_SDMMC1_CLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SDMMC1_CLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SDMMC1_CLK_OUT_REG (1<<20)
#define IOMUX_PAD_SDMMC1_CLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SDMMC1_CLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SDMMC1_CLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SDMMC1_CLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SDMMC1_CLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SDMMC1_CLK_OEN_REG (1<<17)
#define IOMUX_PAD_SDMMC1_CLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SDMMC1_CLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SDMMC1_CLK_PULL_FRC (1<<16)
#define IOMUX_PAD_SDMMC1_CLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SDMMC1_CLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SDMMC1_CLK_PULL_UP (1<<9)
#define IOMUX_PAD_SDMMC1_CLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SDMMC1_CLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SDMMC1_CLK_PULL_DN (1<<8)
#define IOMUX_PAD_SDMMC1_CLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SDMMC1_CLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SDMMC1_CLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SDMMC1_CLK_ANA_MASK (15<<4)
#define IOMUX_PAD_SDMMC1_CLK_ANA_SHIFT (4)
#define IOMUX_PAD_SDMMC1_CLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SDMMC1_CLK_SEL_MASK (15<<0)
#define IOMUX_PAD_SDMMC1_CLK_SEL_SHIFT (0)

//pad_sdmmc1_cmd_cfg_reg
#define IOMUX_PAD_SDMMC1_CMD_OEN_FRC (1<<28)
#define IOMUX_PAD_SDMMC1_CMD_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SDMMC1_CMD_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SDMMC1_CMD_OUT_FRC (1<<24)
#define IOMUX_PAD_SDMMC1_CMD_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SDMMC1_CMD_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SDMMC1_CMD_OUT_REG (1<<20)
#define IOMUX_PAD_SDMMC1_CMD_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SDMMC1_CMD_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SDMMC1_CMD_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SDMMC1_CMD_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SDMMC1_CMD_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SDMMC1_CMD_OEN_REG (1<<17)
#define IOMUX_PAD_SDMMC1_CMD_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SDMMC1_CMD_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SDMMC1_CMD_PULL_FRC (1<<16)
#define IOMUX_PAD_SDMMC1_CMD_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SDMMC1_CMD_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SDMMC1_CMD_PULL_UP (1<<9)
#define IOMUX_PAD_SDMMC1_CMD_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SDMMC1_CMD_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SDMMC1_CMD_PULL_DN (1<<8)
#define IOMUX_PAD_SDMMC1_CMD_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SDMMC1_CMD_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SDMMC1_CMD_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SDMMC1_CMD_ANA_MASK (15<<4)
#define IOMUX_PAD_SDMMC1_CMD_ANA_SHIFT (4)
#define IOMUX_PAD_SDMMC1_CMD_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SDMMC1_CMD_SEL_MASK (15<<0)
#define IOMUX_PAD_SDMMC1_CMD_SEL_SHIFT (0)

//pad_sdmmc1_data_0_cfg_reg
#define IOMUX_PAD_SDMMC1_DATA_0_OEN_FRC (1<<28)
#define IOMUX_PAD_SDMMC1_DATA_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SDMMC1_DATA_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SDMMC1_DATA_0_OUT_FRC (1<<24)
#define IOMUX_PAD_SDMMC1_DATA_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SDMMC1_DATA_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SDMMC1_DATA_0_OUT_REG (1<<20)
#define IOMUX_PAD_SDMMC1_DATA_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SDMMC1_DATA_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SDMMC1_DATA_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SDMMC1_DATA_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SDMMC1_DATA_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SDMMC1_DATA_0_OEN_REG (1<<17)
#define IOMUX_PAD_SDMMC1_DATA_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SDMMC1_DATA_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SDMMC1_DATA_0_PULL_FRC (1<<16)
#define IOMUX_PAD_SDMMC1_DATA_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SDMMC1_DATA_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SDMMC1_DATA_0_PULL_UP (1<<9)
#define IOMUX_PAD_SDMMC1_DATA_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SDMMC1_DATA_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SDMMC1_DATA_0_PULL_DN (1<<8)
#define IOMUX_PAD_SDMMC1_DATA_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SDMMC1_DATA_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SDMMC1_DATA_0_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SDMMC1_DATA_0_ANA_MASK (15<<4)
#define IOMUX_PAD_SDMMC1_DATA_0_ANA_SHIFT (4)
#define IOMUX_PAD_SDMMC1_DATA_0_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SDMMC1_DATA_0_SEL_MASK (15<<0)
#define IOMUX_PAD_SDMMC1_DATA_0_SEL_SHIFT (0)

//pad_sdmmc1_data_1_cfg_reg
#define IOMUX_PAD_SDMMC1_DATA_1_OEN_FRC (1<<28)
#define IOMUX_PAD_SDMMC1_DATA_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SDMMC1_DATA_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SDMMC1_DATA_1_OUT_FRC (1<<24)
#define IOMUX_PAD_SDMMC1_DATA_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SDMMC1_DATA_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SDMMC1_DATA_1_OUT_REG (1<<20)
#define IOMUX_PAD_SDMMC1_DATA_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SDMMC1_DATA_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SDMMC1_DATA_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SDMMC1_DATA_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SDMMC1_DATA_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SDMMC1_DATA_1_OEN_REG (1<<17)
#define IOMUX_PAD_SDMMC1_DATA_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SDMMC1_DATA_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SDMMC1_DATA_1_PULL_FRC (1<<16)
#define IOMUX_PAD_SDMMC1_DATA_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SDMMC1_DATA_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SDMMC1_DATA_1_PULL_UP (1<<9)
#define IOMUX_PAD_SDMMC1_DATA_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SDMMC1_DATA_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SDMMC1_DATA_1_PULL_DN (1<<8)
#define IOMUX_PAD_SDMMC1_DATA_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SDMMC1_DATA_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SDMMC1_DATA_1_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SDMMC1_DATA_1_ANA_MASK (15<<4)
#define IOMUX_PAD_SDMMC1_DATA_1_ANA_SHIFT (4)
#define IOMUX_PAD_SDMMC1_DATA_1_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SDMMC1_DATA_1_SEL_MASK (15<<0)
#define IOMUX_PAD_SDMMC1_DATA_1_SEL_SHIFT (0)

//pad_sdmmc1_data_2_cfg_reg
#define IOMUX_PAD_SDMMC1_DATA_2_OEN_FRC (1<<28)
#define IOMUX_PAD_SDMMC1_DATA_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SDMMC1_DATA_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SDMMC1_DATA_2_OUT_FRC (1<<24)
#define IOMUX_PAD_SDMMC1_DATA_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SDMMC1_DATA_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SDMMC1_DATA_2_OUT_REG (1<<20)
#define IOMUX_PAD_SDMMC1_DATA_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SDMMC1_DATA_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SDMMC1_DATA_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SDMMC1_DATA_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SDMMC1_DATA_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SDMMC1_DATA_2_OEN_REG (1<<17)
#define IOMUX_PAD_SDMMC1_DATA_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SDMMC1_DATA_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SDMMC1_DATA_2_PULL_FRC (1<<16)
#define IOMUX_PAD_SDMMC1_DATA_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SDMMC1_DATA_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SDMMC1_DATA_2_PULL_UP (1<<9)
#define IOMUX_PAD_SDMMC1_DATA_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SDMMC1_DATA_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SDMMC1_DATA_2_PULL_DN (1<<8)
#define IOMUX_PAD_SDMMC1_DATA_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SDMMC1_DATA_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SDMMC1_DATA_2_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SDMMC1_DATA_2_ANA_MASK (15<<4)
#define IOMUX_PAD_SDMMC1_DATA_2_ANA_SHIFT (4)
#define IOMUX_PAD_SDMMC1_DATA_2_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SDMMC1_DATA_2_SEL_MASK (15<<0)
#define IOMUX_PAD_SDMMC1_DATA_2_SEL_SHIFT (0)

//pad_sdmmc1_data_3_cfg_reg
#define IOMUX_PAD_SDMMC1_DATA_3_OEN_FRC (1<<28)
#define IOMUX_PAD_SDMMC1_DATA_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SDMMC1_DATA_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SDMMC1_DATA_3_OUT_FRC (1<<24)
#define IOMUX_PAD_SDMMC1_DATA_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SDMMC1_DATA_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SDMMC1_DATA_3_OUT_REG (1<<20)
#define IOMUX_PAD_SDMMC1_DATA_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SDMMC1_DATA_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SDMMC1_DATA_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SDMMC1_DATA_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SDMMC1_DATA_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SDMMC1_DATA_3_OEN_REG (1<<17)
#define IOMUX_PAD_SDMMC1_DATA_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SDMMC1_DATA_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SDMMC1_DATA_3_PULL_FRC (1<<16)
#define IOMUX_PAD_SDMMC1_DATA_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SDMMC1_DATA_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SDMMC1_DATA_3_PULL_UP (1<<9)
#define IOMUX_PAD_SDMMC1_DATA_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SDMMC1_DATA_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SDMMC1_DATA_3_PULL_DN (1<<8)
#define IOMUX_PAD_SDMMC1_DATA_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SDMMC1_DATA_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SDMMC1_DATA_3_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SDMMC1_DATA_3_ANA_MASK (15<<4)
#define IOMUX_PAD_SDMMC1_DATA_3_ANA_SHIFT (4)
#define IOMUX_PAD_SDMMC1_DATA_3_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SDMMC1_DATA_3_SEL_MASK (15<<0)
#define IOMUX_PAD_SDMMC1_DATA_3_SEL_SHIFT (0)

//pad_i2s1_bck_cfg_reg
#define IOMUX_PAD_I2S1_BCK_OEN_FRC (1<<28)
#define IOMUX_PAD_I2S1_BCK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_I2S1_BCK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_I2S1_BCK_OUT_FRC (1<<24)
#define IOMUX_PAD_I2S1_BCK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_I2S1_BCK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_I2S1_BCK_OUT_REG (1<<20)
#define IOMUX_PAD_I2S1_BCK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_I2S1_BCK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_I2S1_BCK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_I2S1_BCK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_I2S1_BCK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_I2S1_BCK_OEN_REG (1<<17)
#define IOMUX_PAD_I2S1_BCK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_I2S1_BCK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_I2S1_BCK_PULL_FRC (1<<16)
#define IOMUX_PAD_I2S1_BCK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_I2S1_BCK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_I2S1_BCK_PULL_UP (1<<9)
#define IOMUX_PAD_I2S1_BCK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_I2S1_BCK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_I2S1_BCK_PULL_DN (1<<8)
#define IOMUX_PAD_I2S1_BCK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_I2S1_BCK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_I2S1_BCK_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_I2S1_BCK_ANA_MASK (15<<4)
#define IOMUX_PAD_I2S1_BCK_ANA_SHIFT (4)
#define IOMUX_PAD_I2S1_BCK_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_I2S1_BCK_SEL_MASK (15<<0)
#define IOMUX_PAD_I2S1_BCK_SEL_SHIFT (0)

//pad_i2s1_lrck_cfg_reg
#define IOMUX_PAD_I2S1_LRCK_OEN_FRC (1<<28)
#define IOMUX_PAD_I2S1_LRCK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_I2S1_LRCK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_I2S1_LRCK_OUT_FRC (1<<24)
#define IOMUX_PAD_I2S1_LRCK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_I2S1_LRCK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_I2S1_LRCK_OUT_REG (1<<20)
#define IOMUX_PAD_I2S1_LRCK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_I2S1_LRCK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_I2S1_LRCK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_I2S1_LRCK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_I2S1_LRCK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_I2S1_LRCK_OEN_REG (1<<17)
#define IOMUX_PAD_I2S1_LRCK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_I2S1_LRCK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_I2S1_LRCK_PULL_FRC (1<<16)
#define IOMUX_PAD_I2S1_LRCK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_I2S1_LRCK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_I2S1_LRCK_PULL_UP (1<<9)
#define IOMUX_PAD_I2S1_LRCK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_I2S1_LRCK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_I2S1_LRCK_PULL_DN (1<<8)
#define IOMUX_PAD_I2S1_LRCK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_I2S1_LRCK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_I2S1_LRCK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_I2S1_LRCK_ANA_MASK (15<<4)
#define IOMUX_PAD_I2S1_LRCK_ANA_SHIFT (4)
#define IOMUX_PAD_I2S1_LRCK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_I2S1_LRCK_SEL_MASK (15<<0)
#define IOMUX_PAD_I2S1_LRCK_SEL_SHIFT (0)

//pad_i2s1_sdat_i_cfg_reg
#define IOMUX_PAD_I2S1_SDAT_I_OEN_FRC (1<<28)
#define IOMUX_PAD_I2S1_SDAT_I_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_I2S1_SDAT_I_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_I2S1_SDAT_I_OUT_FRC (1<<24)
#define IOMUX_PAD_I2S1_SDAT_I_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_I2S1_SDAT_I_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_I2S1_SDAT_I_OUT_REG (1<<20)
#define IOMUX_PAD_I2S1_SDAT_I_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_I2S1_SDAT_I_OUT_REG_SHIFT (20)
#define IOMUX_PAD_I2S1_SDAT_I_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_I2S1_SDAT_I_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_I2S1_SDAT_I_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_I2S1_SDAT_I_OEN_REG (1<<17)
#define IOMUX_PAD_I2S1_SDAT_I_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_I2S1_SDAT_I_OEN_REG_SHIFT (17)
#define IOMUX_PAD_I2S1_SDAT_I_PULL_FRC (1<<16)
#define IOMUX_PAD_I2S1_SDAT_I_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_I2S1_SDAT_I_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_I2S1_SDAT_I_PULL_UP (1<<9)
#define IOMUX_PAD_I2S1_SDAT_I_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_I2S1_SDAT_I_PULL_UP_SHIFT (9)
#define IOMUX_PAD_I2S1_SDAT_I_PULL_DN (1<<8)
#define IOMUX_PAD_I2S1_SDAT_I_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_I2S1_SDAT_I_PULL_DN_SHIFT (8)
#define IOMUX_PAD_I2S1_SDAT_I_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_I2S1_SDAT_I_ANA_MASK (15<<4)
#define IOMUX_PAD_I2S1_SDAT_I_ANA_SHIFT (4)
#define IOMUX_PAD_I2S1_SDAT_I_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_I2S1_SDAT_I_SEL_MASK (15<<0)
#define IOMUX_PAD_I2S1_SDAT_I_SEL_SHIFT (0)

//pad_i2s1_sdat_o_cfg_reg
#define IOMUX_PAD_I2S1_SDAT_O_OEN_FRC (1<<28)
#define IOMUX_PAD_I2S1_SDAT_O_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_I2S1_SDAT_O_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_I2S1_SDAT_O_OUT_FRC (1<<24)
#define IOMUX_PAD_I2S1_SDAT_O_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_I2S1_SDAT_O_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_I2S1_SDAT_O_OUT_REG (1<<20)
#define IOMUX_PAD_I2S1_SDAT_O_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_I2S1_SDAT_O_OUT_REG_SHIFT (20)
#define IOMUX_PAD_I2S1_SDAT_O_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_I2S1_SDAT_O_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_I2S1_SDAT_O_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_I2S1_SDAT_O_OEN_REG (1<<17)
#define IOMUX_PAD_I2S1_SDAT_O_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_I2S1_SDAT_O_OEN_REG_SHIFT (17)
#define IOMUX_PAD_I2S1_SDAT_O_PULL_FRC (1<<16)
#define IOMUX_PAD_I2S1_SDAT_O_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_I2S1_SDAT_O_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_I2S1_SDAT_O_PULL_UP (1<<9)
#define IOMUX_PAD_I2S1_SDAT_O_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_I2S1_SDAT_O_PULL_UP_SHIFT (9)
#define IOMUX_PAD_I2S1_SDAT_O_PULL_DN (1<<8)
#define IOMUX_PAD_I2S1_SDAT_O_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_I2S1_SDAT_O_PULL_DN_SHIFT (8)
#define IOMUX_PAD_I2S1_SDAT_O_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_I2S1_SDAT_O_ANA_MASK (15<<4)
#define IOMUX_PAD_I2S1_SDAT_O_ANA_SHIFT (4)
#define IOMUX_PAD_I2S1_SDAT_O_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_I2S1_SDAT_O_SEL_MASK (15<<0)
#define IOMUX_PAD_I2S1_SDAT_O_SEL_SHIFT (0)

//pad_adi_sda_cfg_reg
#define IOMUX_PAD_ADI_SDA_OEN_FRC  (1<<28)
#define IOMUX_PAD_ADI_SDA_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_ADI_SDA_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_ADI_SDA_OUT_FRC  (1<<24)
#define IOMUX_PAD_ADI_SDA_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_ADI_SDA_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_ADI_SDA_OUT_REG  (1<<20)
#define IOMUX_PAD_ADI_SDA_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_ADI_SDA_OUT_REG_SHIFT (20)
#define IOMUX_PAD_ADI_SDA_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_ADI_SDA_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_ADI_SDA_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_ADI_SDA_OEN_REG  (1<<17)
#define IOMUX_PAD_ADI_SDA_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_ADI_SDA_OEN_REG_SHIFT (17)
#define IOMUX_PAD_ADI_SDA_PULL_FRC (1<<16)
#define IOMUX_PAD_ADI_SDA_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_ADI_SDA_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_ADI_SDA_PULL_UP  (1<<9)
#define IOMUX_PAD_ADI_SDA_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_ADI_SDA_PULL_UP_SHIFT (9)
#define IOMUX_PAD_ADI_SDA_PULL_DN  (1<<8)
#define IOMUX_PAD_ADI_SDA_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_ADI_SDA_PULL_DN_SHIFT (8)
#define IOMUX_PAD_ADI_SDA_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_ADI_SDA_ANA_MASK (15<<4)
#define IOMUX_PAD_ADI_SDA_ANA_SHIFT (4)
#define IOMUX_PAD_ADI_SDA_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_ADI_SDA_SEL_MASK (15<<0)
#define IOMUX_PAD_ADI_SDA_SEL_SHIFT (0)

//pad_adi_sync_cfg_reg
#define IOMUX_PAD_ADI_SYNC_OEN_FRC (1<<28)
#define IOMUX_PAD_ADI_SYNC_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_ADI_SYNC_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_ADI_SYNC_OUT_FRC (1<<24)
#define IOMUX_PAD_ADI_SYNC_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_ADI_SYNC_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_ADI_SYNC_OUT_REG (1<<20)
#define IOMUX_PAD_ADI_SYNC_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_ADI_SYNC_OUT_REG_SHIFT (20)
#define IOMUX_PAD_ADI_SYNC_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_ADI_SYNC_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_ADI_SYNC_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_ADI_SYNC_OEN_REG (1<<17)
#define IOMUX_PAD_ADI_SYNC_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_ADI_SYNC_OEN_REG_SHIFT (17)
#define IOMUX_PAD_ADI_SYNC_PULL_FRC (1<<16)
#define IOMUX_PAD_ADI_SYNC_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_ADI_SYNC_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_ADI_SYNC_PULL_UP (1<<9)
#define IOMUX_PAD_ADI_SYNC_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_ADI_SYNC_PULL_UP_SHIFT (9)
#define IOMUX_PAD_ADI_SYNC_PULL_DN (1<<8)
#define IOMUX_PAD_ADI_SYNC_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_ADI_SYNC_PULL_DN_SHIFT (8)
#define IOMUX_PAD_ADI_SYNC_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_ADI_SYNC_ANA_MASK (15<<4)
#define IOMUX_PAD_ADI_SYNC_ANA_SHIFT (4)
#define IOMUX_PAD_ADI_SYNC_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_ADI_SYNC_SEL_MASK (15<<0)
#define IOMUX_PAD_ADI_SYNC_SEL_SHIFT (0)

//pad_adi_scl_cfg_reg
#define IOMUX_PAD_ADI_SCL_OEN_FRC  (1<<28)
#define IOMUX_PAD_ADI_SCL_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_ADI_SCL_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_ADI_SCL_OUT_FRC  (1<<24)
#define IOMUX_PAD_ADI_SCL_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_ADI_SCL_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_ADI_SCL_OUT_REG  (1<<20)
#define IOMUX_PAD_ADI_SCL_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_ADI_SCL_OUT_REG_SHIFT (20)
#define IOMUX_PAD_ADI_SCL_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_ADI_SCL_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_ADI_SCL_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_ADI_SCL_OEN_REG  (1<<17)
#define IOMUX_PAD_ADI_SCL_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_ADI_SCL_OEN_REG_SHIFT (17)
#define IOMUX_PAD_ADI_SCL_PULL_FRC (1<<16)
#define IOMUX_PAD_ADI_SCL_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_ADI_SCL_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_ADI_SCL_PULL_UP  (1<<9)
#define IOMUX_PAD_ADI_SCL_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_ADI_SCL_PULL_UP_SHIFT (9)
#define IOMUX_PAD_ADI_SCL_PULL_DN  (1<<8)
#define IOMUX_PAD_ADI_SCL_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_ADI_SCL_PULL_DN_SHIFT (8)
#define IOMUX_PAD_ADI_SCL_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_ADI_SCL_ANA_MASK (15<<4)
#define IOMUX_PAD_ADI_SCL_ANA_SHIFT (4)
#define IOMUX_PAD_ADI_SCL_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_ADI_SCL_SEL_MASK (15<<0)
#define IOMUX_PAD_ADI_SCL_SEL_SHIFT (0)

//pad_spi_lcd_sio_cfg_reg
#define IOMUX_PAD_SPI_LCD_SIO_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_LCD_SIO_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_LCD_SIO_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_LCD_SIO_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_LCD_SIO_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_LCD_SIO_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_LCD_SIO_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_LCD_SIO_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_LCD_SIO_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_LCD_SIO_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_LCD_SIO_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_LCD_SIO_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_LCD_SIO_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_LCD_SIO_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_LCD_SIO_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_LCD_SIO_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_LCD_SIO_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_LCD_SIO_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_LCD_SIO_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_LCD_SIO_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_LCD_SIO_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_LCD_SIO_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_LCD_SIO_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_LCD_SIO_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_LCD_SIO_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_LCD_SIO_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_LCD_SIO_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_LCD_SIO_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_LCD_SIO_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_LCD_SIO_SEL_SHIFT (0)

//pad_spi_lcd_sdc_cfg_reg
#define IOMUX_PAD_SPI_LCD_SDC_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_LCD_SDC_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_LCD_SDC_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_LCD_SDC_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_LCD_SDC_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_LCD_SDC_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_LCD_SDC_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_LCD_SDC_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_LCD_SDC_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_LCD_SDC_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_LCD_SDC_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_LCD_SDC_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_LCD_SDC_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_LCD_SDC_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_LCD_SDC_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_LCD_SDC_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_LCD_SDC_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_LCD_SDC_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_LCD_SDC_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_LCD_SDC_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_LCD_SDC_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_LCD_SDC_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_LCD_SDC_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_LCD_SDC_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_LCD_SDC_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_LCD_SDC_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_LCD_SDC_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_LCD_SDC_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_LCD_SDC_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_LCD_SDC_SEL_SHIFT (0)

//pad_spi_lcd_clk_cfg_reg
#define IOMUX_PAD_SPI_LCD_CLK_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_LCD_CLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_LCD_CLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_LCD_CLK_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_LCD_CLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_LCD_CLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_LCD_CLK_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_LCD_CLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_LCD_CLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_LCD_CLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_LCD_CLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_LCD_CLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_LCD_CLK_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_LCD_CLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_LCD_CLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_LCD_CLK_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_LCD_CLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_LCD_CLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_LCD_CLK_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_LCD_CLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_LCD_CLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_LCD_CLK_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_LCD_CLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_LCD_CLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_LCD_CLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_LCD_CLK_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_LCD_CLK_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_LCD_CLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_LCD_CLK_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_LCD_CLK_SEL_SHIFT (0)

//pad_spi_lcd_cs_cfg_reg
#define IOMUX_PAD_SPI_LCD_CS_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_LCD_CS_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_LCD_CS_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_LCD_CS_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_LCD_CS_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_LCD_CS_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_LCD_CS_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_LCD_CS_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_LCD_CS_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_LCD_CS_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_LCD_CS_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_LCD_CS_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_LCD_CS_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_LCD_CS_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_LCD_CS_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_LCD_CS_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_LCD_CS_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_LCD_CS_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_LCD_CS_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_LCD_CS_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_LCD_CS_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_LCD_CS_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_LCD_CS_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_LCD_CS_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_LCD_CS_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_LCD_CS_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_LCD_CS_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_LCD_CS_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_LCD_CS_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_LCD_CS_SEL_SHIFT (0)

//pad_spi_lcd_select_cfg_reg
#define IOMUX_PAD_SPI_LCD_SELECT_OEN_FRC (1<<28)
#define IOMUX_PAD_SPI_LCD_SELECT_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SPI_LCD_SELECT_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SPI_LCD_SELECT_OUT_FRC (1<<24)
#define IOMUX_PAD_SPI_LCD_SELECT_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SPI_LCD_SELECT_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SPI_LCD_SELECT_OUT_REG (1<<20)
#define IOMUX_PAD_SPI_LCD_SELECT_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SPI_LCD_SELECT_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SPI_LCD_SELECT_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SPI_LCD_SELECT_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SPI_LCD_SELECT_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SPI_LCD_SELECT_OEN_REG (1<<17)
#define IOMUX_PAD_SPI_LCD_SELECT_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SPI_LCD_SELECT_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SPI_LCD_SELECT_PULL_FRC (1<<16)
#define IOMUX_PAD_SPI_LCD_SELECT_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SPI_LCD_SELECT_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SPI_LCD_SELECT_PULL_UP (1<<9)
#define IOMUX_PAD_SPI_LCD_SELECT_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SPI_LCD_SELECT_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SPI_LCD_SELECT_PULL_DN (1<<8)
#define IOMUX_PAD_SPI_LCD_SELECT_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SPI_LCD_SELECT_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SPI_LCD_SELECT_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SPI_LCD_SELECT_ANA_MASK (15<<4)
#define IOMUX_PAD_SPI_LCD_SELECT_ANA_SHIFT (4)
#define IOMUX_PAD_SPI_LCD_SELECT_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SPI_LCD_SELECT_SEL_MASK (15<<0)
#define IOMUX_PAD_SPI_LCD_SELECT_SEL_SHIFT (0)

//pad_lcd_rstb_cfg_reg
#define IOMUX_PAD_LCD_RSTB_OEN_FRC (1<<28)
#define IOMUX_PAD_LCD_RSTB_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LCD_RSTB_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LCD_RSTB_OUT_FRC (1<<24)
#define IOMUX_PAD_LCD_RSTB_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LCD_RSTB_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LCD_RSTB_OUT_REG (1<<20)
#define IOMUX_PAD_LCD_RSTB_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LCD_RSTB_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LCD_RSTB_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LCD_RSTB_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LCD_RSTB_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LCD_RSTB_OEN_REG (1<<17)
#define IOMUX_PAD_LCD_RSTB_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LCD_RSTB_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LCD_RSTB_PULL_FRC (1<<16)
#define IOMUX_PAD_LCD_RSTB_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LCD_RSTB_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LCD_RSTB_PULL_UP (1<<9)
#define IOMUX_PAD_LCD_RSTB_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LCD_RSTB_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LCD_RSTB_PULL_DN (1<<8)
#define IOMUX_PAD_LCD_RSTB_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LCD_RSTB_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LCD_RSTB_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_LCD_RSTB_ANA_MASK (15<<4)
#define IOMUX_PAD_LCD_RSTB_ANA_SHIFT (4)
#define IOMUX_PAD_LCD_RSTB_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_LCD_RSTB_SEL_MASK (15<<0)
#define IOMUX_PAD_LCD_RSTB_SEL_SHIFT (0)

//pad_lcd_fmark_cfg_reg
#define IOMUX_PAD_LCD_FMARK_OEN_FRC (1<<28)
#define IOMUX_PAD_LCD_FMARK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LCD_FMARK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LCD_FMARK_OUT_FRC (1<<24)
#define IOMUX_PAD_LCD_FMARK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LCD_FMARK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LCD_FMARK_OUT_REG (1<<20)
#define IOMUX_PAD_LCD_FMARK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LCD_FMARK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LCD_FMARK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LCD_FMARK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LCD_FMARK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LCD_FMARK_OEN_REG (1<<17)
#define IOMUX_PAD_LCD_FMARK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LCD_FMARK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LCD_FMARK_PULL_FRC (1<<16)
#define IOMUX_PAD_LCD_FMARK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LCD_FMARK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LCD_FMARK_PULL_UP (1<<9)
#define IOMUX_PAD_LCD_FMARK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LCD_FMARK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LCD_FMARK_PULL_DN (1<<8)
#define IOMUX_PAD_LCD_FMARK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LCD_FMARK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LCD_FMARK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LCD_FMARK_ANA_MASK (15<<4)
#define IOMUX_PAD_LCD_FMARK_ANA_SHIFT (4)
#define IOMUX_PAD_LCD_FMARK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LCD_FMARK_SEL_MASK (15<<0)
#define IOMUX_PAD_LCD_FMARK_SEL_SHIFT (0)

//pad_camera_rst_l_cfg_reg
#define IOMUX_PAD_CAMERA_RST_L_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_RST_L_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_RST_L_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_RST_L_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_RST_L_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_RST_L_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_RST_L_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_RST_L_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_RST_L_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_RST_L_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_RST_L_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_RST_L_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_RST_L_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_RST_L_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_RST_L_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_RST_L_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_RST_L_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_RST_L_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_RST_L_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_RST_L_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_RST_L_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_RST_L_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_RST_L_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_RST_L_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_RST_L_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_RST_L_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_RST_L_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_RST_L_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_RST_L_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_RST_L_SEL_SHIFT (0)

//pad_camera_pwdn_cfg_reg
#define IOMUX_PAD_CAMERA_PWDN_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_PWDN_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_PWDN_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_PWDN_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_PWDN_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_PWDN_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_PWDN_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_PWDN_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_PWDN_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_PWDN_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_PWDN_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_PWDN_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_PWDN_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_PWDN_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_PWDN_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_PWDN_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_PWDN_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_PWDN_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_PWDN_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_PWDN_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_PWDN_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_PWDN_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_PWDN_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_PWDN_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_PWDN_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_PWDN_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_PWDN_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_PWDN_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_PWDN_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_PWDN_SEL_SHIFT (0)

//pad_camera_ref_clk_cfg_reg
#define IOMUX_PAD_CAMERA_REF_CLK_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_REF_CLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_REF_CLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_REF_CLK_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_REF_CLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_REF_CLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_REF_CLK_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_REF_CLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_REF_CLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_REF_CLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_REF_CLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_REF_CLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_REF_CLK_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_REF_CLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_REF_CLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_REF_CLK_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_REF_CLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_REF_CLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_REF_CLK_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_REF_CLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_REF_CLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_REF_CLK_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_REF_CLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_REF_CLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_REF_CLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_REF_CLK_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_REF_CLK_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_REF_CLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_REF_CLK_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_REF_CLK_SEL_SHIFT (0)

//pad_camera_vsync_cfg_reg
#define IOMUX_PAD_CAMERA_VSYNC_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_VSYNC_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_VSYNC_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_VSYNC_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_VSYNC_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_VSYNC_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_VSYNC_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_VSYNC_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_VSYNC_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_VSYNC_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_VSYNC_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_VSYNC_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_VSYNC_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_VSYNC_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_VSYNC_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_VSYNC_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_VSYNC_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_VSYNC_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_VSYNC_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_VSYNC_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_VSYNC_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_VSYNC_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_VSYNC_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_VSYNC_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_VSYNC_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_VSYNC_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_VSYNC_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_VSYNC_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_VSYNC_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_VSYNC_SEL_SHIFT (0)

//pad_camera_pix_clk_cfg_reg
#define IOMUX_PAD_CAMERA_PIX_CLK_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_PIX_CLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_PIX_CLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_PIX_CLK_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_PIX_CLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_PIX_CLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_PIX_CLK_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_PIX_CLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_PIX_CLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_PIX_CLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_PIX_CLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_PIX_CLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_PIX_CLK_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_PIX_CLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_PIX_CLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_PIX_CLK_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_PIX_CLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_PIX_CLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_PIX_CLK_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_PIX_CLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_PIX_CLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_PIX_CLK_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_PIX_CLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_PIX_CLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_PIX_CLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_PIX_CLK_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_PIX_CLK_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_PIX_CLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_PIX_CLK_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_PIX_CLK_SEL_SHIFT (0)

//pad_camera_href_cfg_reg
#define IOMUX_PAD_CAMERA_HREF_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_HREF_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_HREF_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_HREF_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_HREF_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_HREF_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_HREF_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_HREF_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_HREF_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_HREF_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_HREF_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_HREF_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_HREF_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_HREF_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_HREF_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_HREF_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_HREF_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_HREF_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_HREF_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_HREF_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_HREF_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_HREF_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_HREF_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_HREF_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_HREF_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_HREF_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_HREF_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_HREF_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_HREF_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_HREF_SEL_SHIFT (0)

//pad_camera_data_in_0_cfg_reg
#define IOMUX_PAD_CAMERA_DATA_IN_0_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_DATA_IN_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_DATA_IN_0_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_DATA_IN_0_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_DATA_IN_0_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_DATA_IN_0_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_0_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_0_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_DATA_IN_0_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_0_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_0_SEL_SHIFT (0)

//pad_camera_data_in_1_cfg_reg
#define IOMUX_PAD_CAMERA_DATA_IN_1_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_DATA_IN_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_DATA_IN_1_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_DATA_IN_1_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_DATA_IN_1_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_DATA_IN_1_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_1_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_1_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_DATA_IN_1_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_1_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_1_SEL_SHIFT (0)

//pad_camera_data_in_2_cfg_reg
#define IOMUX_PAD_CAMERA_DATA_IN_2_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_DATA_IN_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_DATA_IN_2_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_DATA_IN_2_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_DATA_IN_2_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_DATA_IN_2_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_2_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_2_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_DATA_IN_2_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_2_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_2_SEL_SHIFT (0)

//pad_camera_data_in_3_cfg_reg
#define IOMUX_PAD_CAMERA_DATA_IN_3_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_DATA_IN_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_DATA_IN_3_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_DATA_IN_3_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_DATA_IN_3_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_DATA_IN_3_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_3_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_3_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_DATA_IN_3_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_3_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_3_SEL_SHIFT (0)

//pad_camera_data_in_4_cfg_reg
#define IOMUX_PAD_CAMERA_DATA_IN_4_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_DATA_IN_4_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_4_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_4_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_4_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_DATA_IN_4_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_4_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_4_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_DATA_IN_4_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_4_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_4_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_DATA_IN_4_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_4_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_4_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_DATA_IN_4_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_4_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_4_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_DATA_IN_4_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_4_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_4_SEL_SHIFT (0)

//pad_camera_data_in_5_cfg_reg
#define IOMUX_PAD_CAMERA_DATA_IN_5_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_DATA_IN_5_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_5_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_5_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_5_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_DATA_IN_5_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_5_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_5_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_DATA_IN_5_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_5_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_5_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_DATA_IN_5_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_5_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_5_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_DATA_IN_5_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_5_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_5_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_DATA_IN_5_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_5_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_5_SEL_SHIFT (0)

//pad_camera_data_in_6_cfg_reg
#define IOMUX_PAD_CAMERA_DATA_IN_6_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_DATA_IN_6_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_6_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_6_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_6_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_DATA_IN_6_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_6_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_6_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_DATA_IN_6_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_6_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_6_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_DATA_IN_6_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_6_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_6_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_DATA_IN_6_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_6_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_6_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_DATA_IN_6_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_6_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_6_SEL_SHIFT (0)

//pad_camera_data_in_7_cfg_reg
#define IOMUX_PAD_CAMERA_DATA_IN_7_OEN_FRC (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OUT_FRC (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OUT_REG (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CAMERA_DATA_IN_7_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_7_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CAMERA_DATA_IN_7_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OEN_REG (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CAMERA_DATA_IN_7_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CAMERA_DATA_IN_7_PULL_FRC (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_7_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CAMERA_DATA_IN_7_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CAMERA_DATA_IN_7_PULL_UP (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_7_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CAMERA_DATA_IN_7_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CAMERA_DATA_IN_7_PULL_DN (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_7_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CAMERA_DATA_IN_7_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CAMERA_DATA_IN_7_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_7_ANA_MASK (15<<4)
#define IOMUX_PAD_CAMERA_DATA_IN_7_ANA_SHIFT (4)
#define IOMUX_PAD_CAMERA_DATA_IN_7_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_7_SEL_MASK (15<<0)
#define IOMUX_PAD_CAMERA_DATA_IN_7_SEL_SHIFT (0)

//pad_ap_jtag_tck_cfg_reg
#define IOMUX_PAD_AP_JTAG_TCK_OEN_FRC (1<<28)
#define IOMUX_PAD_AP_JTAG_TCK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_AP_JTAG_TCK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_AP_JTAG_TCK_OUT_FRC (1<<24)
#define IOMUX_PAD_AP_JTAG_TCK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_AP_JTAG_TCK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_AP_JTAG_TCK_OUT_REG (1<<20)
#define IOMUX_PAD_AP_JTAG_TCK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_AP_JTAG_TCK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_AP_JTAG_TCK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_AP_JTAG_TCK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_AP_JTAG_TCK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_AP_JTAG_TCK_OEN_REG (1<<17)
#define IOMUX_PAD_AP_JTAG_TCK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_AP_JTAG_TCK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_AP_JTAG_TCK_PULL_FRC (1<<16)
#define IOMUX_PAD_AP_JTAG_TCK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_AP_JTAG_TCK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_AP_JTAG_TCK_PULL_UP (1<<9)
#define IOMUX_PAD_AP_JTAG_TCK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_AP_JTAG_TCK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_AP_JTAG_TCK_PULL_DN (1<<8)
#define IOMUX_PAD_AP_JTAG_TCK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_AP_JTAG_TCK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_AP_JTAG_TCK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_AP_JTAG_TCK_ANA_MASK (15<<4)
#define IOMUX_PAD_AP_JTAG_TCK_ANA_SHIFT (4)
#define IOMUX_PAD_AP_JTAG_TCK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_AP_JTAG_TCK_SEL_MASK (15<<0)
#define IOMUX_PAD_AP_JTAG_TCK_SEL_SHIFT (0)

//pad_ap_jtag_trst_cfg_reg
#define IOMUX_PAD_AP_JTAG_TRST_OEN_FRC (1<<28)
#define IOMUX_PAD_AP_JTAG_TRST_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_AP_JTAG_TRST_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_AP_JTAG_TRST_OUT_FRC (1<<24)
#define IOMUX_PAD_AP_JTAG_TRST_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_AP_JTAG_TRST_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_AP_JTAG_TRST_OUT_REG (1<<20)
#define IOMUX_PAD_AP_JTAG_TRST_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_AP_JTAG_TRST_OUT_REG_SHIFT (20)
#define IOMUX_PAD_AP_JTAG_TRST_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_AP_JTAG_TRST_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_AP_JTAG_TRST_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_AP_JTAG_TRST_OEN_REG (1<<17)
#define IOMUX_PAD_AP_JTAG_TRST_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_AP_JTAG_TRST_OEN_REG_SHIFT (17)
#define IOMUX_PAD_AP_JTAG_TRST_PULL_FRC (1<<16)
#define IOMUX_PAD_AP_JTAG_TRST_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_AP_JTAG_TRST_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_AP_JTAG_TRST_PULL_UP (1<<9)
#define IOMUX_PAD_AP_JTAG_TRST_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_AP_JTAG_TRST_PULL_UP_SHIFT (9)
#define IOMUX_PAD_AP_JTAG_TRST_PULL_DN (1<<8)
#define IOMUX_PAD_AP_JTAG_TRST_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_AP_JTAG_TRST_PULL_DN_SHIFT (8)
#define IOMUX_PAD_AP_JTAG_TRST_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_AP_JTAG_TRST_ANA_MASK (15<<4)
#define IOMUX_PAD_AP_JTAG_TRST_ANA_SHIFT (4)
#define IOMUX_PAD_AP_JTAG_TRST_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_AP_JTAG_TRST_SEL_MASK (15<<0)
#define IOMUX_PAD_AP_JTAG_TRST_SEL_SHIFT (0)

//pad_ap_jtag_tms_cfg_reg
#define IOMUX_PAD_AP_JTAG_TMS_OEN_FRC (1<<28)
#define IOMUX_PAD_AP_JTAG_TMS_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_AP_JTAG_TMS_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_AP_JTAG_TMS_OUT_FRC (1<<24)
#define IOMUX_PAD_AP_JTAG_TMS_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_AP_JTAG_TMS_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_AP_JTAG_TMS_OUT_REG (1<<20)
#define IOMUX_PAD_AP_JTAG_TMS_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_AP_JTAG_TMS_OUT_REG_SHIFT (20)
#define IOMUX_PAD_AP_JTAG_TMS_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_AP_JTAG_TMS_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_AP_JTAG_TMS_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_AP_JTAG_TMS_OEN_REG (1<<17)
#define IOMUX_PAD_AP_JTAG_TMS_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_AP_JTAG_TMS_OEN_REG_SHIFT (17)
#define IOMUX_PAD_AP_JTAG_TMS_PULL_FRC (1<<16)
#define IOMUX_PAD_AP_JTAG_TMS_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_AP_JTAG_TMS_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_AP_JTAG_TMS_PULL_UP (1<<9)
#define IOMUX_PAD_AP_JTAG_TMS_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_AP_JTAG_TMS_PULL_UP_SHIFT (9)
#define IOMUX_PAD_AP_JTAG_TMS_PULL_DN (1<<8)
#define IOMUX_PAD_AP_JTAG_TMS_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_AP_JTAG_TMS_PULL_DN_SHIFT (8)
#define IOMUX_PAD_AP_JTAG_TMS_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_AP_JTAG_TMS_ANA_MASK (15<<4)
#define IOMUX_PAD_AP_JTAG_TMS_ANA_SHIFT (4)
#define IOMUX_PAD_AP_JTAG_TMS_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_AP_JTAG_TMS_SEL_MASK (15<<0)
#define IOMUX_PAD_AP_JTAG_TMS_SEL_SHIFT (0)

//pad_ap_jtag_tdi_cfg_reg
#define IOMUX_PAD_AP_JTAG_TDI_OEN_FRC (1<<28)
#define IOMUX_PAD_AP_JTAG_TDI_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_AP_JTAG_TDI_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_AP_JTAG_TDI_OUT_FRC (1<<24)
#define IOMUX_PAD_AP_JTAG_TDI_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_AP_JTAG_TDI_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_AP_JTAG_TDI_OUT_REG (1<<20)
#define IOMUX_PAD_AP_JTAG_TDI_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_AP_JTAG_TDI_OUT_REG_SHIFT (20)
#define IOMUX_PAD_AP_JTAG_TDI_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_AP_JTAG_TDI_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_AP_JTAG_TDI_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_AP_JTAG_TDI_OEN_REG (1<<17)
#define IOMUX_PAD_AP_JTAG_TDI_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_AP_JTAG_TDI_OEN_REG_SHIFT (17)
#define IOMUX_PAD_AP_JTAG_TDI_PULL_FRC (1<<16)
#define IOMUX_PAD_AP_JTAG_TDI_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_AP_JTAG_TDI_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_AP_JTAG_TDI_PULL_UP (1<<9)
#define IOMUX_PAD_AP_JTAG_TDI_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_AP_JTAG_TDI_PULL_UP_SHIFT (9)
#define IOMUX_PAD_AP_JTAG_TDI_PULL_DN (1<<8)
#define IOMUX_PAD_AP_JTAG_TDI_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_AP_JTAG_TDI_PULL_DN_SHIFT (8)
#define IOMUX_PAD_AP_JTAG_TDI_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_AP_JTAG_TDI_ANA_MASK (15<<4)
#define IOMUX_PAD_AP_JTAG_TDI_ANA_SHIFT (4)
#define IOMUX_PAD_AP_JTAG_TDI_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_AP_JTAG_TDI_SEL_MASK (15<<0)
#define IOMUX_PAD_AP_JTAG_TDI_SEL_SHIFT (0)

//pad_ap_jtag_tdo_cfg_reg
#define IOMUX_PAD_AP_JTAG_TDO_OEN_FRC (1<<28)
#define IOMUX_PAD_AP_JTAG_TDO_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_AP_JTAG_TDO_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_AP_JTAG_TDO_OUT_FRC (1<<24)
#define IOMUX_PAD_AP_JTAG_TDO_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_AP_JTAG_TDO_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_AP_JTAG_TDO_OUT_REG (1<<20)
#define IOMUX_PAD_AP_JTAG_TDO_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_AP_JTAG_TDO_OUT_REG_SHIFT (20)
#define IOMUX_PAD_AP_JTAG_TDO_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_AP_JTAG_TDO_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_AP_JTAG_TDO_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_AP_JTAG_TDO_OEN_REG (1<<17)
#define IOMUX_PAD_AP_JTAG_TDO_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_AP_JTAG_TDO_OEN_REG_SHIFT (17)
#define IOMUX_PAD_AP_JTAG_TDO_PULL_FRC (1<<16)
#define IOMUX_PAD_AP_JTAG_TDO_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_AP_JTAG_TDO_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_AP_JTAG_TDO_PULL_UP (1<<9)
#define IOMUX_PAD_AP_JTAG_TDO_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_AP_JTAG_TDO_PULL_UP_SHIFT (9)
#define IOMUX_PAD_AP_JTAG_TDO_PULL_DN (1<<8)
#define IOMUX_PAD_AP_JTAG_TDO_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_AP_JTAG_TDO_PULL_DN_SHIFT (8)
#define IOMUX_PAD_AP_JTAG_TDO_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_AP_JTAG_TDO_ANA_MASK (15<<4)
#define IOMUX_PAD_AP_JTAG_TDO_ANA_SHIFT (4)
#define IOMUX_PAD_AP_JTAG_TDO_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_AP_JTAG_TDO_SEL_MASK (15<<0)
#define IOMUX_PAD_AP_JTAG_TDO_SEL_SHIFT (0)

//pad_pwl_out_1_cfg_reg
#define IOMUX_PAD_PWL_OUT_1_OEN_FRC (1<<28)
#define IOMUX_PAD_PWL_OUT_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_PWL_OUT_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_PWL_OUT_1_OUT_FRC (1<<24)
#define IOMUX_PAD_PWL_OUT_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_PWL_OUT_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_PWL_OUT_1_OUT_REG (1<<20)
#define IOMUX_PAD_PWL_OUT_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_PWL_OUT_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_PWL_OUT_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_PWL_OUT_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_PWL_OUT_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_PWL_OUT_1_OEN_REG (1<<17)
#define IOMUX_PAD_PWL_OUT_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_PWL_OUT_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_PWL_OUT_1_PULL_FRC (1<<16)
#define IOMUX_PAD_PWL_OUT_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_PWL_OUT_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_PWL_OUT_1_PULL_UP (1<<9)
#define IOMUX_PAD_PWL_OUT_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_PWL_OUT_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_PWL_OUT_1_PULL_DN (1<<8)
#define IOMUX_PAD_PWL_OUT_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_PWL_OUT_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_PWL_OUT_1_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_PWL_OUT_1_ANA_MASK (15<<4)
#define IOMUX_PAD_PWL_OUT_1_ANA_SHIFT (4)
#define IOMUX_PAD_PWL_OUT_1_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_PWL_OUT_1_SEL_MASK (15<<0)
#define IOMUX_PAD_PWL_OUT_1_SEL_SHIFT (0)

//pad_pwl_out_0_cfg_reg
#define IOMUX_PAD_PWL_OUT_0_OEN_FRC (1<<28)
#define IOMUX_PAD_PWL_OUT_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_PWL_OUT_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_PWL_OUT_0_OUT_FRC (1<<24)
#define IOMUX_PAD_PWL_OUT_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_PWL_OUT_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_PWL_OUT_0_OUT_REG (1<<20)
#define IOMUX_PAD_PWL_OUT_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_PWL_OUT_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_PWL_OUT_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_PWL_OUT_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_PWL_OUT_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_PWL_OUT_0_OEN_REG (1<<17)
#define IOMUX_PAD_PWL_OUT_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_PWL_OUT_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_PWL_OUT_0_PULL_FRC (1<<16)
#define IOMUX_PAD_PWL_OUT_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_PWL_OUT_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_PWL_OUT_0_PULL_UP (1<<9)
#define IOMUX_PAD_PWL_OUT_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_PWL_OUT_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_PWL_OUT_0_PULL_DN (1<<8)
#define IOMUX_PAD_PWL_OUT_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_PWL_OUT_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_PWL_OUT_0_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_PWL_OUT_0_ANA_MASK (15<<4)
#define IOMUX_PAD_PWL_OUT_0_ANA_SHIFT (4)
#define IOMUX_PAD_PWL_OUT_0_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_PWL_OUT_0_SEL_MASK (15<<0)
#define IOMUX_PAD_PWL_OUT_0_SEL_SHIFT (0)

//pad_pwm_lpg_out_cfg_reg
#define IOMUX_PAD_PWM_LPG_OUT_OEN_FRC (1<<28)
#define IOMUX_PAD_PWM_LPG_OUT_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_PWM_LPG_OUT_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_PWM_LPG_OUT_OUT_FRC (1<<24)
#define IOMUX_PAD_PWM_LPG_OUT_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_PWM_LPG_OUT_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_PWM_LPG_OUT_OUT_REG (1<<20)
#define IOMUX_PAD_PWM_LPG_OUT_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_PWM_LPG_OUT_OUT_REG_SHIFT (20)
#define IOMUX_PAD_PWM_LPG_OUT_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_PWM_LPG_OUT_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_PWM_LPG_OUT_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_PWM_LPG_OUT_OEN_REG (1<<17)
#define IOMUX_PAD_PWM_LPG_OUT_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_PWM_LPG_OUT_OEN_REG_SHIFT (17)
#define IOMUX_PAD_PWM_LPG_OUT_PULL_FRC (1<<16)
#define IOMUX_PAD_PWM_LPG_OUT_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_PWM_LPG_OUT_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_PWM_LPG_OUT_PULL_UP (1<<9)
#define IOMUX_PAD_PWM_LPG_OUT_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_PWM_LPG_OUT_PULL_UP_SHIFT (9)
#define IOMUX_PAD_PWM_LPG_OUT_PULL_DN (1<<8)
#define IOMUX_PAD_PWM_LPG_OUT_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_PWM_LPG_OUT_PULL_DN_SHIFT (8)
#define IOMUX_PAD_PWM_LPG_OUT_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_PWM_LPG_OUT_ANA_MASK (15<<4)
#define IOMUX_PAD_PWM_LPG_OUT_ANA_SHIFT (4)
#define IOMUX_PAD_PWM_LPG_OUT_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_PWM_LPG_OUT_SEL_MASK (15<<0)
#define IOMUX_PAD_PWM_LPG_OUT_SEL_SHIFT (0)

//pad_pwt_out_cfg_reg
#define IOMUX_PAD_PWT_OUT_OEN_FRC  (1<<28)
#define IOMUX_PAD_PWT_OUT_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_PWT_OUT_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_PWT_OUT_OUT_FRC  (1<<24)
#define IOMUX_PAD_PWT_OUT_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_PWT_OUT_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_PWT_OUT_OUT_REG  (1<<20)
#define IOMUX_PAD_PWT_OUT_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_PWT_OUT_OUT_REG_SHIFT (20)
#define IOMUX_PAD_PWT_OUT_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_PWT_OUT_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_PWT_OUT_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_PWT_OUT_OEN_REG  (1<<17)
#define IOMUX_PAD_PWT_OUT_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_PWT_OUT_OEN_REG_SHIFT (17)
#define IOMUX_PAD_PWT_OUT_PULL_FRC (1<<16)
#define IOMUX_PAD_PWT_OUT_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_PWT_OUT_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_PWT_OUT_PULL_UP  (1<<9)
#define IOMUX_PAD_PWT_OUT_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_PWT_OUT_PULL_UP_SHIFT (9)
#define IOMUX_PAD_PWT_OUT_PULL_DN  (1<<8)
#define IOMUX_PAD_PWT_OUT_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_PWT_OUT_PULL_DN_SHIFT (8)
#define IOMUX_PAD_PWT_OUT_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_PWT_OUT_ANA_MASK (15<<4)
#define IOMUX_PAD_PWT_OUT_ANA_SHIFT (4)
#define IOMUX_PAD_PWT_OUT_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_PWT_OUT_SEL_MASK (15<<0)
#define IOMUX_PAD_PWT_OUT_SEL_SHIFT (0)

//pad_gpio_0_cfg_reg
#define IOMUX_PAD_GPIO_0_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_0_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_0_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_0_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_0_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_0_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_0_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_0_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_0_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_0_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_0_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_0_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_0_SEL_SHIFT (0)

//pad_gpio_1_cfg_reg
#define IOMUX_PAD_GPIO_1_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_1_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_1_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_1_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_1_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_1_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_1_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_1_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_1_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_1_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_1_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_1_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_1_SEL_SHIFT (0)

//pad_gpio_2_cfg_reg
#define IOMUX_PAD_GPIO_2_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_2_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_2_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_2_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_2_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_2_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_2_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_2_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_2_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_2_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_2_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_2_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_2_SEL_SHIFT (0)

//pad_gpio_3_cfg_reg
#define IOMUX_PAD_GPIO_3_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_3_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_3_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_3_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_3_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_3_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_3_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_3_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_3_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_3_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_3_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_3_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_3_SEL_SHIFT (0)

//pad_gpio_4_cfg_reg
#define IOMUX_PAD_GPIO_4_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_4_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_4_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_4_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_4_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_4_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_4_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_4_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_4_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_4_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_4_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_4_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_4_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_4_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_4_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_4_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_4_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_4_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_4_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_4_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_4_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_4_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_4_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_4_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_4_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_4_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_4_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_4_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_4_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_4_SEL_SHIFT (0)

//pad_gpio_5_cfg_reg
#define IOMUX_PAD_GPIO_5_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_5_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_5_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_5_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_5_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_5_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_5_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_5_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_5_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_5_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_5_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_5_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_5_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_5_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_5_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_5_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_5_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_5_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_5_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_5_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_5_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_5_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_5_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_5_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_5_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_5_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_5_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_5_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_5_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_5_SEL_SHIFT (0)

//pad_gpio_10_cfg_reg
#define IOMUX_PAD_GPIO_10_OEN_FRC  (1<<28)
#define IOMUX_PAD_GPIO_10_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_10_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_10_OUT_FRC  (1<<24)
#define IOMUX_PAD_GPIO_10_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_10_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_10_OUT_REG  (1<<20)
#define IOMUX_PAD_GPIO_10_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_10_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_10_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_10_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_10_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_10_OEN_REG  (1<<17)
#define IOMUX_PAD_GPIO_10_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_10_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_10_PULL_FRC (1<<16)
#define IOMUX_PAD_GPIO_10_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_10_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_10_PULL_UP  (1<<9)
#define IOMUX_PAD_GPIO_10_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_10_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_10_PULL_DN  (1<<8)
#define IOMUX_PAD_GPIO_10_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_10_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_10_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_GPIO_10_ANA_MASK (15<<4)
#define IOMUX_PAD_GPIO_10_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_10_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_GPIO_10_SEL_MASK (15<<0)
#define IOMUX_PAD_GPIO_10_SEL_SHIFT (0)

//pad_gpio_11_cfg_reg
#define IOMUX_PAD_GPIO_11_OEN_FRC  (1<<28)
#define IOMUX_PAD_GPIO_11_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_11_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_11_OUT_FRC  (1<<24)
#define IOMUX_PAD_GPIO_11_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_11_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_11_OUT_REG  (1<<20)
#define IOMUX_PAD_GPIO_11_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_11_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_11_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_11_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_11_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_11_OEN_REG  (1<<17)
#define IOMUX_PAD_GPIO_11_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_11_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_11_PULL_FRC (1<<16)
#define IOMUX_PAD_GPIO_11_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_11_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_11_PULL_UP  (1<<9)
#define IOMUX_PAD_GPIO_11_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_11_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_11_PULL_DN  (1<<8)
#define IOMUX_PAD_GPIO_11_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_11_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_11_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_GPIO_11_ANA_MASK (15<<4)
#define IOMUX_PAD_GPIO_11_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_11_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_GPIO_11_SEL_MASK (15<<0)
#define IOMUX_PAD_GPIO_11_SEL_SHIFT (0)

//pad_gpio_12_cfg_reg
#define IOMUX_PAD_GPIO_12_OEN_FRC  (1<<28)
#define IOMUX_PAD_GPIO_12_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_12_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_12_OUT_FRC  (1<<24)
#define IOMUX_PAD_GPIO_12_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_12_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_12_OUT_REG  (1<<20)
#define IOMUX_PAD_GPIO_12_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_12_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_12_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_12_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_12_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_12_OEN_REG  (1<<17)
#define IOMUX_PAD_GPIO_12_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_12_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_12_PULL_FRC (1<<16)
#define IOMUX_PAD_GPIO_12_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_12_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_12_PULL_UP  (1<<9)
#define IOMUX_PAD_GPIO_12_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_12_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_12_PULL_DN  (1<<8)
#define IOMUX_PAD_GPIO_12_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_12_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_12_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_GPIO_12_ANA_MASK (15<<4)
#define IOMUX_PAD_GPIO_12_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_12_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_GPIO_12_SEL_MASK (15<<0)
#define IOMUX_PAD_GPIO_12_SEL_SHIFT (0)

//pad_gpio_6_cfg_reg
#define IOMUX_PAD_GPIO_6_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_6_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_6_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_6_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_6_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_6_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_6_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_6_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_6_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_6_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_6_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_6_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_6_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_6_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_6_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_6_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_6_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_6_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_6_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_6_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_6_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_6_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_6_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_6_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_6_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_6_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_6_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_6_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_6_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_6_SEL_SHIFT (0)

//pad_gpio_7_cfg_reg
#define IOMUX_PAD_GPIO_7_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_7_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_7_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_7_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_7_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_7_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_7_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_7_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_7_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_7_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_7_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_7_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_7_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_7_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_7_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_7_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_7_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_7_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_7_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_7_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_7_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_7_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_7_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_7_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_7_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_7_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_7_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_7_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_7_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_7_SEL_SHIFT (0)

//pad_gpio_8_cfg_reg
#define IOMUX_PAD_GPIO_8_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_8_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_8_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_8_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_8_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_8_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_8_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_8_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_8_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_8_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_8_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_8_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_8_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_8_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_8_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_8_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_8_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_8_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_8_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_8_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_8_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_8_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_8_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_8_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_8_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_8_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_8_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_8_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_8_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_8_SEL_SHIFT (0)

//pad_gpio_9_cfg_reg
#define IOMUX_PAD_GPIO_9_OEN_FRC   (1<<28)
#define IOMUX_PAD_GPIO_9_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPIO_9_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPIO_9_OUT_FRC   (1<<24)
#define IOMUX_PAD_GPIO_9_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPIO_9_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPIO_9_OUT_REG   (1<<20)
#define IOMUX_PAD_GPIO_9_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPIO_9_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPIO_9_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPIO_9_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPIO_9_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPIO_9_OEN_REG   (1<<17)
#define IOMUX_PAD_GPIO_9_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPIO_9_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPIO_9_PULL_FRC  (1<<16)
#define IOMUX_PAD_GPIO_9_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPIO_9_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPIO_9_PULL_UP   (1<<9)
#define IOMUX_PAD_GPIO_9_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPIO_9_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPIO_9_PULL_DN   (1<<8)
#define IOMUX_PAD_GPIO_9_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPIO_9_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPIO_9_ANA(n)    (((n)&15)<<4)
#define IOMUX_PAD_GPIO_9_ANA_MASK  (15<<4)
#define IOMUX_PAD_GPIO_9_ANA_SHIFT (4)
#define IOMUX_PAD_GPIO_9_SEL(n)    (((n)&15)<<0)
#define IOMUX_PAD_GPIO_9_SEL_MASK  (15<<0)
#define IOMUX_PAD_GPIO_9_SEL_SHIFT (0)

//pad_i2c_m4_scl_cfg_reg
#define IOMUX_PAD_I2C_M4_SCL_OEN_FRC (1<<28)
#define IOMUX_PAD_I2C_M4_SCL_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_I2C_M4_SCL_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_I2C_M4_SCL_OUT_FRC (1<<24)
#define IOMUX_PAD_I2C_M4_SCL_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_I2C_M4_SCL_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_I2C_M4_SCL_OUT_REG (1<<20)
#define IOMUX_PAD_I2C_M4_SCL_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_I2C_M4_SCL_OUT_REG_SHIFT (20)
#define IOMUX_PAD_I2C_M4_SCL_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_I2C_M4_SCL_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_I2C_M4_SCL_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_I2C_M4_SCL_OEN_REG (1<<17)
#define IOMUX_PAD_I2C_M4_SCL_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_I2C_M4_SCL_OEN_REG_SHIFT (17)
#define IOMUX_PAD_I2C_M4_SCL_PULL_FRC (1<<16)
#define IOMUX_PAD_I2C_M4_SCL_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_I2C_M4_SCL_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_I2C_M4_SCL_PULL_UP (1<<9)
#define IOMUX_PAD_I2C_M4_SCL_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_I2C_M4_SCL_PULL_UP_SHIFT (9)
#define IOMUX_PAD_I2C_M4_SCL_PULL_DN (1<<8)
#define IOMUX_PAD_I2C_M4_SCL_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_I2C_M4_SCL_PULL_DN_SHIFT (8)
#define IOMUX_PAD_I2C_M4_SCL_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_I2C_M4_SCL_ANA_MASK (15<<4)
#define IOMUX_PAD_I2C_M4_SCL_ANA_SHIFT (4)
#define IOMUX_PAD_I2C_M4_SCL_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_I2C_M4_SCL_SEL_MASK (15<<0)
#define IOMUX_PAD_I2C_M4_SCL_SEL_SHIFT (0)

//pad_i2c_m4_sda_cfg_reg
#define IOMUX_PAD_I2C_M4_SDA_OEN_FRC (1<<28)
#define IOMUX_PAD_I2C_M4_SDA_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_I2C_M4_SDA_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_I2C_M4_SDA_OUT_FRC (1<<24)
#define IOMUX_PAD_I2C_M4_SDA_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_I2C_M4_SDA_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_I2C_M4_SDA_OUT_REG (1<<20)
#define IOMUX_PAD_I2C_M4_SDA_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_I2C_M4_SDA_OUT_REG_SHIFT (20)
#define IOMUX_PAD_I2C_M4_SDA_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_I2C_M4_SDA_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_I2C_M4_SDA_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_I2C_M4_SDA_OEN_REG (1<<17)
#define IOMUX_PAD_I2C_M4_SDA_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_I2C_M4_SDA_OEN_REG_SHIFT (17)
#define IOMUX_PAD_I2C_M4_SDA_PULL_FRC (1<<16)
#define IOMUX_PAD_I2C_M4_SDA_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_I2C_M4_SDA_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_I2C_M4_SDA_PULL_UP (1<<9)
#define IOMUX_PAD_I2C_M4_SDA_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_I2C_M4_SDA_PULL_UP_SHIFT (9)
#define IOMUX_PAD_I2C_M4_SDA_PULL_DN (1<<8)
#define IOMUX_PAD_I2C_M4_SDA_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_I2C_M4_SDA_PULL_DN_SHIFT (8)
#define IOMUX_PAD_I2C_M4_SDA_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_I2C_M4_SDA_ANA_MASK (15<<4)
#define IOMUX_PAD_I2C_M4_SDA_ANA_SHIFT (4)
#define IOMUX_PAD_I2C_M4_SDA_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_I2C_M4_SDA_SEL_MASK (15<<0)
#define IOMUX_PAD_I2C_M4_SDA_SEL_SHIFT (0)

//pad_uart_1_rxd_cfg_reg
#define IOMUX_PAD_UART_1_RXD_OEN_FRC (1<<28)
#define IOMUX_PAD_UART_1_RXD_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_UART_1_RXD_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_UART_1_RXD_OUT_FRC (1<<24)
#define IOMUX_PAD_UART_1_RXD_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_UART_1_RXD_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_UART_1_RXD_OUT_REG (1<<20)
#define IOMUX_PAD_UART_1_RXD_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_UART_1_RXD_OUT_REG_SHIFT (20)
#define IOMUX_PAD_UART_1_RXD_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_UART_1_RXD_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_UART_1_RXD_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_UART_1_RXD_OEN_REG (1<<17)
#define IOMUX_PAD_UART_1_RXD_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_UART_1_RXD_OEN_REG_SHIFT (17)
#define IOMUX_PAD_UART_1_RXD_PULL_FRC (1<<16)
#define IOMUX_PAD_UART_1_RXD_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_UART_1_RXD_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_UART_1_RXD_PULL_UP (1<<9)
#define IOMUX_PAD_UART_1_RXD_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_UART_1_RXD_PULL_UP_SHIFT (9)
#define IOMUX_PAD_UART_1_RXD_PULL_DN (1<<8)
#define IOMUX_PAD_UART_1_RXD_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_UART_1_RXD_PULL_DN_SHIFT (8)
#define IOMUX_PAD_UART_1_RXD_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_UART_1_RXD_ANA_MASK (15<<4)
#define IOMUX_PAD_UART_1_RXD_ANA_SHIFT (4)
#define IOMUX_PAD_UART_1_RXD_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_UART_1_RXD_SEL_MASK (15<<0)
#define IOMUX_PAD_UART_1_RXD_SEL_SHIFT (0)

//pad_uart_1_txd_cfg_reg
#define IOMUX_PAD_UART_1_TXD_OEN_FRC (1<<28)
#define IOMUX_PAD_UART_1_TXD_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_UART_1_TXD_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_UART_1_TXD_OUT_FRC (1<<24)
#define IOMUX_PAD_UART_1_TXD_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_UART_1_TXD_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_UART_1_TXD_OUT_REG (1<<20)
#define IOMUX_PAD_UART_1_TXD_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_UART_1_TXD_OUT_REG_SHIFT (20)
#define IOMUX_PAD_UART_1_TXD_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_UART_1_TXD_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_UART_1_TXD_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_UART_1_TXD_OEN_REG (1<<17)
#define IOMUX_PAD_UART_1_TXD_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_UART_1_TXD_OEN_REG_SHIFT (17)
#define IOMUX_PAD_UART_1_TXD_PULL_FRC (1<<16)
#define IOMUX_PAD_UART_1_TXD_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_UART_1_TXD_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_UART_1_TXD_PULL_UP (1<<9)
#define IOMUX_PAD_UART_1_TXD_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_UART_1_TXD_PULL_UP_SHIFT (9)
#define IOMUX_PAD_UART_1_TXD_PULL_DN (1<<8)
#define IOMUX_PAD_UART_1_TXD_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_UART_1_TXD_PULL_DN_SHIFT (8)
#define IOMUX_PAD_UART_1_TXD_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_UART_1_TXD_ANA_MASK (15<<4)
#define IOMUX_PAD_UART_1_TXD_ANA_SHIFT (4)
#define IOMUX_PAD_UART_1_TXD_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_UART_1_TXD_SEL_MASK (15<<0)
#define IOMUX_PAD_UART_1_TXD_SEL_SHIFT (0)

//pad_uart_1_cts_cfg_reg
#define IOMUX_PAD_UART_1_CTS_OEN_FRC (1<<28)
#define IOMUX_PAD_UART_1_CTS_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_UART_1_CTS_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_UART_1_CTS_OUT_FRC (1<<24)
#define IOMUX_PAD_UART_1_CTS_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_UART_1_CTS_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_UART_1_CTS_OUT_REG (1<<20)
#define IOMUX_PAD_UART_1_CTS_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_UART_1_CTS_OUT_REG_SHIFT (20)
#define IOMUX_PAD_UART_1_CTS_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_UART_1_CTS_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_UART_1_CTS_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_UART_1_CTS_OEN_REG (1<<17)
#define IOMUX_PAD_UART_1_CTS_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_UART_1_CTS_OEN_REG_SHIFT (17)
#define IOMUX_PAD_UART_1_CTS_PULL_FRC (1<<16)
#define IOMUX_PAD_UART_1_CTS_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_UART_1_CTS_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_UART_1_CTS_PULL_UP (1<<9)
#define IOMUX_PAD_UART_1_CTS_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_UART_1_CTS_PULL_UP_SHIFT (9)
#define IOMUX_PAD_UART_1_CTS_PULL_DN (1<<8)
#define IOMUX_PAD_UART_1_CTS_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_UART_1_CTS_PULL_DN_SHIFT (8)
#define IOMUX_PAD_UART_1_CTS_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_UART_1_CTS_ANA_MASK (15<<4)
#define IOMUX_PAD_UART_1_CTS_ANA_SHIFT (4)
#define IOMUX_PAD_UART_1_CTS_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_UART_1_CTS_SEL_MASK (15<<0)
#define IOMUX_PAD_UART_1_CTS_SEL_SHIFT (0)

//pad_uart_1_rts_cfg_reg
#define IOMUX_PAD_UART_1_RTS_OEN_FRC (1<<28)
#define IOMUX_PAD_UART_1_RTS_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_UART_1_RTS_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_UART_1_RTS_OUT_FRC (1<<24)
#define IOMUX_PAD_UART_1_RTS_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_UART_1_RTS_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_UART_1_RTS_OUT_REG (1<<20)
#define IOMUX_PAD_UART_1_RTS_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_UART_1_RTS_OUT_REG_SHIFT (20)
#define IOMUX_PAD_UART_1_RTS_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_UART_1_RTS_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_UART_1_RTS_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_UART_1_RTS_OEN_REG (1<<17)
#define IOMUX_PAD_UART_1_RTS_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_UART_1_RTS_OEN_REG_SHIFT (17)
#define IOMUX_PAD_UART_1_RTS_PULL_FRC (1<<16)
#define IOMUX_PAD_UART_1_RTS_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_UART_1_RTS_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_UART_1_RTS_PULL_UP (1<<9)
#define IOMUX_PAD_UART_1_RTS_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_UART_1_RTS_PULL_UP_SHIFT (9)
#define IOMUX_PAD_UART_1_RTS_PULL_DN (1<<8)
#define IOMUX_PAD_UART_1_RTS_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_UART_1_RTS_PULL_DN_SHIFT (8)
#define IOMUX_PAD_UART_1_RTS_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_UART_1_RTS_ANA_MASK (15<<4)
#define IOMUX_PAD_UART_1_RTS_ANA_SHIFT (4)
#define IOMUX_PAD_UART_1_RTS_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_UART_1_RTS_SEL_MASK (15<<0)
#define IOMUX_PAD_UART_1_RTS_SEL_SHIFT (0)

//pad_uart_2_rxd_cfg_reg
#define IOMUX_PAD_UART_2_RXD_OEN_FRC (1<<28)
#define IOMUX_PAD_UART_2_RXD_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_UART_2_RXD_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_UART_2_RXD_OUT_FRC (1<<24)
#define IOMUX_PAD_UART_2_RXD_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_UART_2_RXD_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_UART_2_RXD_OUT_REG (1<<20)
#define IOMUX_PAD_UART_2_RXD_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_UART_2_RXD_OUT_REG_SHIFT (20)
#define IOMUX_PAD_UART_2_RXD_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_UART_2_RXD_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_UART_2_RXD_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_UART_2_RXD_OEN_REG (1<<17)
#define IOMUX_PAD_UART_2_RXD_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_UART_2_RXD_OEN_REG_SHIFT (17)
#define IOMUX_PAD_UART_2_RXD_PULL_FRC (1<<16)
#define IOMUX_PAD_UART_2_RXD_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_UART_2_RXD_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_UART_2_RXD_PULL_UP (1<<9)
#define IOMUX_PAD_UART_2_RXD_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_UART_2_RXD_PULL_UP_SHIFT (9)
#define IOMUX_PAD_UART_2_RXD_PULL_DN (1<<8)
#define IOMUX_PAD_UART_2_RXD_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_UART_2_RXD_PULL_DN_SHIFT (8)
#define IOMUX_PAD_UART_2_RXD_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_UART_2_RXD_ANA_MASK (15<<4)
#define IOMUX_PAD_UART_2_RXD_ANA_SHIFT (4)
#define IOMUX_PAD_UART_2_RXD_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_UART_2_RXD_SEL_MASK (15<<0)
#define IOMUX_PAD_UART_2_RXD_SEL_SHIFT (0)

//pad_uart_2_txd_cfg_reg
#define IOMUX_PAD_UART_2_TXD_OEN_FRC (1<<28)
#define IOMUX_PAD_UART_2_TXD_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_UART_2_TXD_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_UART_2_TXD_OUT_FRC (1<<24)
#define IOMUX_PAD_UART_2_TXD_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_UART_2_TXD_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_UART_2_TXD_OUT_REG (1<<20)
#define IOMUX_PAD_UART_2_TXD_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_UART_2_TXD_OUT_REG_SHIFT (20)
#define IOMUX_PAD_UART_2_TXD_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_UART_2_TXD_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_UART_2_TXD_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_UART_2_TXD_OEN_REG (1<<17)
#define IOMUX_PAD_UART_2_TXD_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_UART_2_TXD_OEN_REG_SHIFT (17)
#define IOMUX_PAD_UART_2_TXD_PULL_FRC (1<<16)
#define IOMUX_PAD_UART_2_TXD_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_UART_2_TXD_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_UART_2_TXD_PULL_UP (1<<9)
#define IOMUX_PAD_UART_2_TXD_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_UART_2_TXD_PULL_UP_SHIFT (9)
#define IOMUX_PAD_UART_2_TXD_PULL_DN (1<<8)
#define IOMUX_PAD_UART_2_TXD_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_UART_2_TXD_PULL_DN_SHIFT (8)
#define IOMUX_PAD_UART_2_TXD_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_UART_2_TXD_ANA_MASK (15<<4)
#define IOMUX_PAD_UART_2_TXD_ANA_SHIFT (4)
#define IOMUX_PAD_UART_2_TXD_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_UART_2_TXD_SEL_MASK (15<<0)
#define IOMUX_PAD_UART_2_TXD_SEL_SHIFT (0)

//pad_uart_2_cts_cfg_reg
#define IOMUX_PAD_UART_2_CTS_OEN_FRC (1<<28)
#define IOMUX_PAD_UART_2_CTS_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_UART_2_CTS_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_UART_2_CTS_OUT_FRC (1<<24)
#define IOMUX_PAD_UART_2_CTS_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_UART_2_CTS_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_UART_2_CTS_OUT_REG (1<<20)
#define IOMUX_PAD_UART_2_CTS_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_UART_2_CTS_OUT_REG_SHIFT (20)
#define IOMUX_PAD_UART_2_CTS_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_UART_2_CTS_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_UART_2_CTS_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_UART_2_CTS_OEN_REG (1<<17)
#define IOMUX_PAD_UART_2_CTS_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_UART_2_CTS_OEN_REG_SHIFT (17)
#define IOMUX_PAD_UART_2_CTS_PULL_FRC (1<<16)
#define IOMUX_PAD_UART_2_CTS_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_UART_2_CTS_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_UART_2_CTS_PULL_UP (1<<9)
#define IOMUX_PAD_UART_2_CTS_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_UART_2_CTS_PULL_UP_SHIFT (9)
#define IOMUX_PAD_UART_2_CTS_PULL_DN (1<<8)
#define IOMUX_PAD_UART_2_CTS_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_UART_2_CTS_PULL_DN_SHIFT (8)
#define IOMUX_PAD_UART_2_CTS_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_UART_2_CTS_ANA_MASK (15<<4)
#define IOMUX_PAD_UART_2_CTS_ANA_SHIFT (4)
#define IOMUX_PAD_UART_2_CTS_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_UART_2_CTS_SEL_MASK (15<<0)
#define IOMUX_PAD_UART_2_CTS_SEL_SHIFT (0)

//pad_uart_2_rts_cfg_reg
#define IOMUX_PAD_UART_2_RTS_OEN_FRC (1<<28)
#define IOMUX_PAD_UART_2_RTS_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_UART_2_RTS_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_UART_2_RTS_OUT_FRC (1<<24)
#define IOMUX_PAD_UART_2_RTS_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_UART_2_RTS_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_UART_2_RTS_OUT_REG (1<<20)
#define IOMUX_PAD_UART_2_RTS_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_UART_2_RTS_OUT_REG_SHIFT (20)
#define IOMUX_PAD_UART_2_RTS_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_UART_2_RTS_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_UART_2_RTS_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_UART_2_RTS_OEN_REG (1<<17)
#define IOMUX_PAD_UART_2_RTS_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_UART_2_RTS_OEN_REG_SHIFT (17)
#define IOMUX_PAD_UART_2_RTS_PULL_FRC (1<<16)
#define IOMUX_PAD_UART_2_RTS_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_UART_2_RTS_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_UART_2_RTS_PULL_UP (1<<9)
#define IOMUX_PAD_UART_2_RTS_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_UART_2_RTS_PULL_UP_SHIFT (9)
#define IOMUX_PAD_UART_2_RTS_PULL_DN (1<<8)
#define IOMUX_PAD_UART_2_RTS_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_UART_2_RTS_PULL_DN_SHIFT (8)
#define IOMUX_PAD_UART_2_RTS_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_UART_2_RTS_ANA_MASK (15<<4)
#define IOMUX_PAD_UART_2_RTS_ANA_SHIFT (4)
#define IOMUX_PAD_UART_2_RTS_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_UART_2_RTS_SEL_MASK (15<<0)
#define IOMUX_PAD_UART_2_RTS_SEL_SHIFT (0)

//pad_gpo_0_cfg_reg
#define IOMUX_PAD_GPO_0_OEN_FRC    (1<<28)
#define IOMUX_PAD_GPO_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPO_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPO_0_OUT_FRC    (1<<24)
#define IOMUX_PAD_GPO_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPO_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPO_0_OUT_REG    (1<<20)
#define IOMUX_PAD_GPO_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPO_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPO_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPO_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPO_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPO_0_OEN_REG    (1<<17)
#define IOMUX_PAD_GPO_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPO_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPO_0_PULL_FRC   (1<<16)
#define IOMUX_PAD_GPO_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPO_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPO_0_PULL_UP    (1<<9)
#define IOMUX_PAD_GPO_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPO_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPO_0_PULL_DN    (1<<8)
#define IOMUX_PAD_GPO_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPO_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPO_0_ANA(n)     (((n)&15)<<4)
#define IOMUX_PAD_GPO_0_ANA_MASK   (15<<4)
#define IOMUX_PAD_GPO_0_ANA_SHIFT  (4)
#define IOMUX_PAD_GPO_0_SEL(n)     (((n)&15)<<0)
#define IOMUX_PAD_GPO_0_SEL_MASK   (15<<0)
#define IOMUX_PAD_GPO_0_SEL_SHIFT  (0)

//pad_gpo_1_cfg_reg
#define IOMUX_PAD_GPO_1_OEN_FRC    (1<<28)
#define IOMUX_PAD_GPO_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPO_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPO_1_OUT_FRC    (1<<24)
#define IOMUX_PAD_GPO_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPO_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPO_1_OUT_REG    (1<<20)
#define IOMUX_PAD_GPO_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPO_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPO_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPO_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPO_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPO_1_OEN_REG    (1<<17)
#define IOMUX_PAD_GPO_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPO_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPO_1_PULL_FRC   (1<<16)
#define IOMUX_PAD_GPO_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPO_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPO_1_PULL_UP    (1<<9)
#define IOMUX_PAD_GPO_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPO_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPO_1_PULL_DN    (1<<8)
#define IOMUX_PAD_GPO_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPO_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPO_1_ANA(n)     (((n)&15)<<4)
#define IOMUX_PAD_GPO_1_ANA_MASK   (15<<4)
#define IOMUX_PAD_GPO_1_ANA_SHIFT  (4)
#define IOMUX_PAD_GPO_1_SEL(n)     (((n)&15)<<0)
#define IOMUX_PAD_GPO_1_SEL_MASK   (15<<0)
#define IOMUX_PAD_GPO_1_SEL_SHIFT  (0)

//pad_gpo_2_cfg_reg
#define IOMUX_PAD_GPO_2_OEN_FRC    (1<<28)
#define IOMUX_PAD_GPO_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPO_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPO_2_OUT_FRC    (1<<24)
#define IOMUX_PAD_GPO_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPO_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPO_2_OUT_REG    (1<<20)
#define IOMUX_PAD_GPO_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPO_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPO_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPO_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPO_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPO_2_OEN_REG    (1<<17)
#define IOMUX_PAD_GPO_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPO_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPO_2_PULL_FRC   (1<<16)
#define IOMUX_PAD_GPO_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPO_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPO_2_PULL_UP    (1<<9)
#define IOMUX_PAD_GPO_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPO_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPO_2_PULL_DN    (1<<8)
#define IOMUX_PAD_GPO_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPO_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPO_2_ANA(n)     (((n)&15)<<4)
#define IOMUX_PAD_GPO_2_ANA_MASK   (15<<4)
#define IOMUX_PAD_GPO_2_ANA_SHIFT  (4)
#define IOMUX_PAD_GPO_2_SEL(n)     (((n)&15)<<0)
#define IOMUX_PAD_GPO_2_SEL_MASK   (15<<0)
#define IOMUX_PAD_GPO_2_SEL_SHIFT  (0)

//pad_gpo_3_cfg_reg
#define IOMUX_PAD_GPO_3_OEN_FRC    (1<<28)
#define IOMUX_PAD_GPO_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPO_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPO_3_OUT_FRC    (1<<24)
#define IOMUX_PAD_GPO_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPO_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPO_3_OUT_REG    (1<<20)
#define IOMUX_PAD_GPO_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPO_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPO_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPO_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPO_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPO_3_OEN_REG    (1<<17)
#define IOMUX_PAD_GPO_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPO_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPO_3_PULL_FRC   (1<<16)
#define IOMUX_PAD_GPO_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPO_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPO_3_PULL_UP    (1<<9)
#define IOMUX_PAD_GPO_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPO_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPO_3_PULL_DN    (1<<8)
#define IOMUX_PAD_GPO_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPO_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPO_3_ANA(n)     (((n)&15)<<4)
#define IOMUX_PAD_GPO_3_ANA_MASK   (15<<4)
#define IOMUX_PAD_GPO_3_ANA_SHIFT  (4)
#define IOMUX_PAD_GPO_3_SEL(n)     (((n)&15)<<0)
#define IOMUX_PAD_GPO_3_SEL_MASK   (15<<0)
#define IOMUX_PAD_GPO_3_SEL_SHIFT  (0)

//pad_gpo_4_cfg_reg
#define IOMUX_PAD_GPO_4_OEN_FRC    (1<<28)
#define IOMUX_PAD_GPO_4_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_GPO_4_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_GPO_4_OUT_FRC    (1<<24)
#define IOMUX_PAD_GPO_4_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_GPO_4_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_GPO_4_OUT_REG    (1<<20)
#define IOMUX_PAD_GPO_4_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_GPO_4_OUT_REG_SHIFT (20)
#define IOMUX_PAD_GPO_4_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_GPO_4_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_GPO_4_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_GPO_4_OEN_REG    (1<<17)
#define IOMUX_PAD_GPO_4_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_GPO_4_OEN_REG_SHIFT (17)
#define IOMUX_PAD_GPO_4_PULL_FRC   (1<<16)
#define IOMUX_PAD_GPO_4_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_GPO_4_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_GPO_4_PULL_UP    (1<<9)
#define IOMUX_PAD_GPO_4_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_GPO_4_PULL_UP_SHIFT (9)
#define IOMUX_PAD_GPO_4_PULL_DN    (1<<8)
#define IOMUX_PAD_GPO_4_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_GPO_4_PULL_DN_SHIFT (8)
#define IOMUX_PAD_GPO_4_ANA(n)     (((n)&15)<<4)
#define IOMUX_PAD_GPO_4_ANA_MASK   (15<<4)
#define IOMUX_PAD_GPO_4_ANA_SHIFT  (4)
#define IOMUX_PAD_GPO_4_SEL(n)     (((n)&15)<<0)
#define IOMUX_PAD_GPO_4_SEL_MASK   (15<<0)
#define IOMUX_PAD_GPO_4_SEL_SHIFT  (0)

//pad_i2c_m1_scl_cfg_reg
#define IOMUX_PAD_I2C_M1_SCL_OEN_FRC (1<<28)
#define IOMUX_PAD_I2C_M1_SCL_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_I2C_M1_SCL_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_I2C_M1_SCL_OUT_FRC (1<<24)
#define IOMUX_PAD_I2C_M1_SCL_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_I2C_M1_SCL_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_I2C_M1_SCL_OUT_REG (1<<20)
#define IOMUX_PAD_I2C_M1_SCL_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_I2C_M1_SCL_OUT_REG_SHIFT (20)
#define IOMUX_PAD_I2C_M1_SCL_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_I2C_M1_SCL_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_I2C_M1_SCL_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_I2C_M1_SCL_OEN_REG (1<<17)
#define IOMUX_PAD_I2C_M1_SCL_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_I2C_M1_SCL_OEN_REG_SHIFT (17)
#define IOMUX_PAD_I2C_M1_SCL_PULL_FRC (1<<16)
#define IOMUX_PAD_I2C_M1_SCL_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_I2C_M1_SCL_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_I2C_M1_SCL_PULL_UP (1<<9)
#define IOMUX_PAD_I2C_M1_SCL_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_I2C_M1_SCL_PULL_UP_SHIFT (9)
#define IOMUX_PAD_I2C_M1_SCL_PULL_DN (1<<8)
#define IOMUX_PAD_I2C_M1_SCL_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_I2C_M1_SCL_PULL_DN_SHIFT (8)
#define IOMUX_PAD_I2C_M1_SCL_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_I2C_M1_SCL_ANA_MASK (15<<4)
#define IOMUX_PAD_I2C_M1_SCL_ANA_SHIFT (4)
#define IOMUX_PAD_I2C_M1_SCL_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_I2C_M1_SCL_SEL_MASK (15<<0)
#define IOMUX_PAD_I2C_M1_SCL_SEL_SHIFT (0)

//pad_i2c_m1_sda_cfg_reg
#define IOMUX_PAD_I2C_M1_SDA_OEN_FRC (1<<28)
#define IOMUX_PAD_I2C_M1_SDA_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_I2C_M1_SDA_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_I2C_M1_SDA_OUT_FRC (1<<24)
#define IOMUX_PAD_I2C_M1_SDA_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_I2C_M1_SDA_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_I2C_M1_SDA_OUT_REG (1<<20)
#define IOMUX_PAD_I2C_M1_SDA_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_I2C_M1_SDA_OUT_REG_SHIFT (20)
#define IOMUX_PAD_I2C_M1_SDA_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_I2C_M1_SDA_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_I2C_M1_SDA_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_I2C_M1_SDA_OEN_REG (1<<17)
#define IOMUX_PAD_I2C_M1_SDA_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_I2C_M1_SDA_OEN_REG_SHIFT (17)
#define IOMUX_PAD_I2C_M1_SDA_PULL_FRC (1<<16)
#define IOMUX_PAD_I2C_M1_SDA_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_I2C_M1_SDA_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_I2C_M1_SDA_PULL_UP (1<<9)
#define IOMUX_PAD_I2C_M1_SDA_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_I2C_M1_SDA_PULL_UP_SHIFT (9)
#define IOMUX_PAD_I2C_M1_SDA_PULL_DN (1<<8)
#define IOMUX_PAD_I2C_M1_SDA_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_I2C_M1_SDA_PULL_DN_SHIFT (8)
#define IOMUX_PAD_I2C_M1_SDA_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_I2C_M1_SDA_ANA_MASK (15<<4)
#define IOMUX_PAD_I2C_M1_SDA_ANA_SHIFT (4)
#define IOMUX_PAD_I2C_M1_SDA_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_I2C_M1_SDA_SEL_MASK (15<<0)
#define IOMUX_PAD_I2C_M1_SDA_SEL_SHIFT (0)

//pad_i_keyon_cfg_reg
#define IOMUX_PAD_I_KEYON_OEN_FRC  (1<<28)
#define IOMUX_PAD_I_KEYON_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_I_KEYON_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_I_KEYON_OUT_FRC  (1<<24)
#define IOMUX_PAD_I_KEYON_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_I_KEYON_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_I_KEYON_OUT_REG  (1<<20)
#define IOMUX_PAD_I_KEYON_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_I_KEYON_OUT_REG_SHIFT (20)
#define IOMUX_PAD_I_KEYON_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_I_KEYON_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_I_KEYON_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_I_KEYON_OEN_REG  (1<<17)
#define IOMUX_PAD_I_KEYON_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_I_KEYON_OEN_REG_SHIFT (17)
#define IOMUX_PAD_I_KEYON_PULL_FRC (1<<16)
#define IOMUX_PAD_I_KEYON_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_I_KEYON_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_I_KEYON_PULL_UP  (1<<9)
#define IOMUX_PAD_I_KEYON_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_I_KEYON_PULL_UP_SHIFT (9)
#define IOMUX_PAD_I_KEYON_PULL_DN  (1<<8)
#define IOMUX_PAD_I_KEYON_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_I_KEYON_PULL_DN_SHIFT (8)
#define IOMUX_PAD_I_KEYON_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_I_KEYON_ANA_MASK (15<<4)
#define IOMUX_PAD_I_KEYON_ANA_SHIFT (4)
#define IOMUX_PAD_I_KEYON_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_I_KEYON_SEL_MASK (15<<0)
#define IOMUX_PAD_I_KEYON_SEL_SHIFT (0)

//pad_keyin_0_cfg_reg
#define IOMUX_PAD_KEYIN_0_OEN_FRC  (1<<28)
#define IOMUX_PAD_KEYIN_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYIN_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYIN_0_OUT_FRC  (1<<24)
#define IOMUX_PAD_KEYIN_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYIN_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYIN_0_OUT_REG  (1<<20)
#define IOMUX_PAD_KEYIN_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYIN_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYIN_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYIN_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYIN_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYIN_0_OEN_REG  (1<<17)
#define IOMUX_PAD_KEYIN_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYIN_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYIN_0_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYIN_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYIN_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYIN_0_PULL_UP  (1<<9)
#define IOMUX_PAD_KEYIN_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYIN_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYIN_0_PULL_DN  (1<<8)
#define IOMUX_PAD_KEYIN_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYIN_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYIN_0_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_KEYIN_0_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYIN_0_ANA_SHIFT (4)
#define IOMUX_PAD_KEYIN_0_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_KEYIN_0_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYIN_0_SEL_SHIFT (0)

//pad_keyin_1_cfg_reg
#define IOMUX_PAD_KEYIN_1_OEN_FRC  (1<<28)
#define IOMUX_PAD_KEYIN_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYIN_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYIN_1_OUT_FRC  (1<<24)
#define IOMUX_PAD_KEYIN_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYIN_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYIN_1_OUT_REG  (1<<20)
#define IOMUX_PAD_KEYIN_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYIN_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYIN_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYIN_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYIN_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYIN_1_OEN_REG  (1<<17)
#define IOMUX_PAD_KEYIN_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYIN_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYIN_1_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYIN_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYIN_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYIN_1_PULL_UP  (1<<9)
#define IOMUX_PAD_KEYIN_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYIN_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYIN_1_PULL_DN  (1<<8)
#define IOMUX_PAD_KEYIN_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYIN_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYIN_1_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_KEYIN_1_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYIN_1_ANA_SHIFT (4)
#define IOMUX_PAD_KEYIN_1_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_KEYIN_1_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYIN_1_SEL_SHIFT (0)

//pad_keyin_2_cfg_reg
#define IOMUX_PAD_KEYIN_2_OEN_FRC  (1<<28)
#define IOMUX_PAD_KEYIN_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYIN_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYIN_2_OUT_FRC  (1<<24)
#define IOMUX_PAD_KEYIN_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYIN_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYIN_2_OUT_REG  (1<<20)
#define IOMUX_PAD_KEYIN_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYIN_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYIN_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYIN_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYIN_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYIN_2_OEN_REG  (1<<17)
#define IOMUX_PAD_KEYIN_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYIN_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYIN_2_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYIN_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYIN_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYIN_2_PULL_UP  (1<<9)
#define IOMUX_PAD_KEYIN_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYIN_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYIN_2_PULL_DN  (1<<8)
#define IOMUX_PAD_KEYIN_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYIN_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYIN_2_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_KEYIN_2_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYIN_2_ANA_SHIFT (4)
#define IOMUX_PAD_KEYIN_2_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_KEYIN_2_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYIN_2_SEL_SHIFT (0)

//pad_keyin_3_cfg_reg
#define IOMUX_PAD_KEYIN_3_OEN_FRC  (1<<28)
#define IOMUX_PAD_KEYIN_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYIN_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYIN_3_OUT_FRC  (1<<24)
#define IOMUX_PAD_KEYIN_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYIN_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYIN_3_OUT_REG  (1<<20)
#define IOMUX_PAD_KEYIN_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYIN_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYIN_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYIN_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYIN_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYIN_3_OEN_REG  (1<<17)
#define IOMUX_PAD_KEYIN_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYIN_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYIN_3_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYIN_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYIN_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYIN_3_PULL_UP  (1<<9)
#define IOMUX_PAD_KEYIN_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYIN_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYIN_3_PULL_DN  (1<<8)
#define IOMUX_PAD_KEYIN_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYIN_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYIN_3_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_KEYIN_3_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYIN_3_ANA_SHIFT (4)
#define IOMUX_PAD_KEYIN_3_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_KEYIN_3_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYIN_3_SEL_SHIFT (0)

//pad_keyin_4_cfg_reg
#define IOMUX_PAD_KEYIN_4_OEN_FRC  (1<<28)
#define IOMUX_PAD_KEYIN_4_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYIN_4_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYIN_4_OUT_FRC  (1<<24)
#define IOMUX_PAD_KEYIN_4_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYIN_4_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYIN_4_OUT_REG  (1<<20)
#define IOMUX_PAD_KEYIN_4_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYIN_4_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYIN_4_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYIN_4_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYIN_4_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYIN_4_OEN_REG  (1<<17)
#define IOMUX_PAD_KEYIN_4_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYIN_4_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYIN_4_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYIN_4_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYIN_4_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYIN_4_PULL_UP  (1<<9)
#define IOMUX_PAD_KEYIN_4_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYIN_4_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYIN_4_PULL_DN  (1<<8)
#define IOMUX_PAD_KEYIN_4_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYIN_4_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYIN_4_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_KEYIN_4_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYIN_4_ANA_SHIFT (4)
#define IOMUX_PAD_KEYIN_4_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_KEYIN_4_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYIN_4_SEL_SHIFT (0)

//pad_keyin_5_cfg_reg
#define IOMUX_PAD_KEYIN_5_OEN_FRC  (1<<28)
#define IOMUX_PAD_KEYIN_5_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYIN_5_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYIN_5_OUT_FRC  (1<<24)
#define IOMUX_PAD_KEYIN_5_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYIN_5_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYIN_5_OUT_REG  (1<<20)
#define IOMUX_PAD_KEYIN_5_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYIN_5_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYIN_5_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYIN_5_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYIN_5_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYIN_5_OEN_REG  (1<<17)
#define IOMUX_PAD_KEYIN_5_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYIN_5_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYIN_5_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYIN_5_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYIN_5_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYIN_5_PULL_UP  (1<<9)
#define IOMUX_PAD_KEYIN_5_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYIN_5_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYIN_5_PULL_DN  (1<<8)
#define IOMUX_PAD_KEYIN_5_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYIN_5_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYIN_5_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_KEYIN_5_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYIN_5_ANA_SHIFT (4)
#define IOMUX_PAD_KEYIN_5_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_KEYIN_5_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYIN_5_SEL_SHIFT (0)

//pad_keyout_0_cfg_reg
#define IOMUX_PAD_KEYOUT_0_OEN_FRC (1<<28)
#define IOMUX_PAD_KEYOUT_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYOUT_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYOUT_0_OUT_FRC (1<<24)
#define IOMUX_PAD_KEYOUT_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYOUT_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYOUT_0_OUT_REG (1<<20)
#define IOMUX_PAD_KEYOUT_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYOUT_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYOUT_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYOUT_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYOUT_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYOUT_0_OEN_REG (1<<17)
#define IOMUX_PAD_KEYOUT_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYOUT_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYOUT_0_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYOUT_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYOUT_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYOUT_0_PULL_UP (1<<9)
#define IOMUX_PAD_KEYOUT_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYOUT_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYOUT_0_PULL_DN (1<<8)
#define IOMUX_PAD_KEYOUT_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYOUT_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYOUT_0_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_KEYOUT_0_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYOUT_0_ANA_SHIFT (4)
#define IOMUX_PAD_KEYOUT_0_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_KEYOUT_0_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYOUT_0_SEL_SHIFT (0)

//pad_keyout_1_cfg_reg
#define IOMUX_PAD_KEYOUT_1_OEN_FRC (1<<28)
#define IOMUX_PAD_KEYOUT_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYOUT_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYOUT_1_OUT_FRC (1<<24)
#define IOMUX_PAD_KEYOUT_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYOUT_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYOUT_1_OUT_REG (1<<20)
#define IOMUX_PAD_KEYOUT_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYOUT_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYOUT_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYOUT_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYOUT_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYOUT_1_OEN_REG (1<<17)
#define IOMUX_PAD_KEYOUT_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYOUT_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYOUT_1_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYOUT_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYOUT_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYOUT_1_PULL_UP (1<<9)
#define IOMUX_PAD_KEYOUT_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYOUT_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYOUT_1_PULL_DN (1<<8)
#define IOMUX_PAD_KEYOUT_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYOUT_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYOUT_1_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_KEYOUT_1_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYOUT_1_ANA_SHIFT (4)
#define IOMUX_PAD_KEYOUT_1_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_KEYOUT_1_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYOUT_1_SEL_SHIFT (0)

//pad_keyout_2_cfg_reg
#define IOMUX_PAD_KEYOUT_2_OEN_FRC (1<<28)
#define IOMUX_PAD_KEYOUT_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYOUT_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYOUT_2_OUT_FRC (1<<24)
#define IOMUX_PAD_KEYOUT_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYOUT_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYOUT_2_OUT_REG (1<<20)
#define IOMUX_PAD_KEYOUT_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYOUT_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYOUT_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYOUT_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYOUT_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYOUT_2_OEN_REG (1<<17)
#define IOMUX_PAD_KEYOUT_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYOUT_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYOUT_2_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYOUT_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYOUT_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYOUT_2_PULL_UP (1<<9)
#define IOMUX_PAD_KEYOUT_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYOUT_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYOUT_2_PULL_DN (1<<8)
#define IOMUX_PAD_KEYOUT_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYOUT_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYOUT_2_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_KEYOUT_2_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYOUT_2_ANA_SHIFT (4)
#define IOMUX_PAD_KEYOUT_2_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_KEYOUT_2_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYOUT_2_SEL_SHIFT (0)

//pad_keyout_3_cfg_reg
#define IOMUX_PAD_KEYOUT_3_OEN_FRC (1<<28)
#define IOMUX_PAD_KEYOUT_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYOUT_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYOUT_3_OUT_FRC (1<<24)
#define IOMUX_PAD_KEYOUT_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYOUT_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYOUT_3_OUT_REG (1<<20)
#define IOMUX_PAD_KEYOUT_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYOUT_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYOUT_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYOUT_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYOUT_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYOUT_3_OEN_REG (1<<17)
#define IOMUX_PAD_KEYOUT_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYOUT_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYOUT_3_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYOUT_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYOUT_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYOUT_3_PULL_UP (1<<9)
#define IOMUX_PAD_KEYOUT_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYOUT_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYOUT_3_PULL_DN (1<<8)
#define IOMUX_PAD_KEYOUT_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYOUT_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYOUT_3_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_KEYOUT_3_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYOUT_3_ANA_SHIFT (4)
#define IOMUX_PAD_KEYOUT_3_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_KEYOUT_3_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYOUT_3_SEL_SHIFT (0)

//pad_keyout_4_cfg_reg
#define IOMUX_PAD_KEYOUT_4_OEN_FRC (1<<28)
#define IOMUX_PAD_KEYOUT_4_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYOUT_4_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYOUT_4_OUT_FRC (1<<24)
#define IOMUX_PAD_KEYOUT_4_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYOUT_4_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYOUT_4_OUT_REG (1<<20)
#define IOMUX_PAD_KEYOUT_4_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYOUT_4_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYOUT_4_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYOUT_4_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYOUT_4_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYOUT_4_OEN_REG (1<<17)
#define IOMUX_PAD_KEYOUT_4_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYOUT_4_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYOUT_4_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYOUT_4_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYOUT_4_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYOUT_4_PULL_UP (1<<9)
#define IOMUX_PAD_KEYOUT_4_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYOUT_4_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYOUT_4_PULL_DN (1<<8)
#define IOMUX_PAD_KEYOUT_4_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYOUT_4_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYOUT_4_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_KEYOUT_4_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYOUT_4_ANA_SHIFT (4)
#define IOMUX_PAD_KEYOUT_4_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_KEYOUT_4_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYOUT_4_SEL_SHIFT (0)

//pad_keyout_5_cfg_reg
#define IOMUX_PAD_KEYOUT_5_OEN_FRC (1<<28)
#define IOMUX_PAD_KEYOUT_5_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_KEYOUT_5_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_KEYOUT_5_OUT_FRC (1<<24)
#define IOMUX_PAD_KEYOUT_5_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_KEYOUT_5_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_KEYOUT_5_OUT_REG (1<<20)
#define IOMUX_PAD_KEYOUT_5_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_KEYOUT_5_OUT_REG_SHIFT (20)
#define IOMUX_PAD_KEYOUT_5_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_KEYOUT_5_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_KEYOUT_5_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_KEYOUT_5_OEN_REG (1<<17)
#define IOMUX_PAD_KEYOUT_5_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_KEYOUT_5_OEN_REG_SHIFT (17)
#define IOMUX_PAD_KEYOUT_5_PULL_FRC (1<<16)
#define IOMUX_PAD_KEYOUT_5_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_KEYOUT_5_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_KEYOUT_5_PULL_UP (1<<9)
#define IOMUX_PAD_KEYOUT_5_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_KEYOUT_5_PULL_UP_SHIFT (9)
#define IOMUX_PAD_KEYOUT_5_PULL_DN (1<<8)
#define IOMUX_PAD_KEYOUT_5_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_KEYOUT_5_PULL_DN_SHIFT (8)
#define IOMUX_PAD_KEYOUT_5_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_KEYOUT_5_ANA_MASK (15<<4)
#define IOMUX_PAD_KEYOUT_5_ANA_SHIFT (4)
#define IOMUX_PAD_KEYOUT_5_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_KEYOUT_5_SEL_MASK (15<<0)
#define IOMUX_PAD_KEYOUT_5_SEL_SHIFT (0)

//pad_debug_host_rx_cfg_reg
#define IOMUX_PAD_DEBUG_HOST_RX_OEN_FRC (1<<28)
#define IOMUX_PAD_DEBUG_HOST_RX_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_DEBUG_HOST_RX_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_DEBUG_HOST_RX_OUT_FRC (1<<24)
#define IOMUX_PAD_DEBUG_HOST_RX_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_DEBUG_HOST_RX_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_DEBUG_HOST_RX_OUT_REG (1<<20)
#define IOMUX_PAD_DEBUG_HOST_RX_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_DEBUG_HOST_RX_OUT_REG_SHIFT (20)
#define IOMUX_PAD_DEBUG_HOST_RX_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_DEBUG_HOST_RX_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_DEBUG_HOST_RX_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_DEBUG_HOST_RX_OEN_REG (1<<17)
#define IOMUX_PAD_DEBUG_HOST_RX_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_DEBUG_HOST_RX_OEN_REG_SHIFT (17)
#define IOMUX_PAD_DEBUG_HOST_RX_PULL_FRC (1<<16)
#define IOMUX_PAD_DEBUG_HOST_RX_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_DEBUG_HOST_RX_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_DEBUG_HOST_RX_PULL_UP (1<<9)
#define IOMUX_PAD_DEBUG_HOST_RX_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_DEBUG_HOST_RX_PULL_UP_SHIFT (9)
#define IOMUX_PAD_DEBUG_HOST_RX_PULL_DN (1<<8)
#define IOMUX_PAD_DEBUG_HOST_RX_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_DEBUG_HOST_RX_PULL_DN_SHIFT (8)
#define IOMUX_PAD_DEBUG_HOST_RX_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_DEBUG_HOST_RX_ANA_MASK (15<<4)
#define IOMUX_PAD_DEBUG_HOST_RX_ANA_SHIFT (4)
#define IOMUX_PAD_DEBUG_HOST_RX_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_DEBUG_HOST_RX_SEL_MASK (15<<0)
#define IOMUX_PAD_DEBUG_HOST_RX_SEL_SHIFT (0)

//pad_debug_host_tx_cfg_reg
#define IOMUX_PAD_DEBUG_HOST_TX_OEN_FRC (1<<28)
#define IOMUX_PAD_DEBUG_HOST_TX_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_DEBUG_HOST_TX_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_DEBUG_HOST_TX_OUT_FRC (1<<24)
#define IOMUX_PAD_DEBUG_HOST_TX_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_DEBUG_HOST_TX_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_DEBUG_HOST_TX_OUT_REG (1<<20)
#define IOMUX_PAD_DEBUG_HOST_TX_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_DEBUG_HOST_TX_OUT_REG_SHIFT (20)
#define IOMUX_PAD_DEBUG_HOST_TX_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_DEBUG_HOST_TX_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_DEBUG_HOST_TX_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_DEBUG_HOST_TX_OEN_REG (1<<17)
#define IOMUX_PAD_DEBUG_HOST_TX_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_DEBUG_HOST_TX_OEN_REG_SHIFT (17)
#define IOMUX_PAD_DEBUG_HOST_TX_PULL_FRC (1<<16)
#define IOMUX_PAD_DEBUG_HOST_TX_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_DEBUG_HOST_TX_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_DEBUG_HOST_TX_PULL_UP (1<<9)
#define IOMUX_PAD_DEBUG_HOST_TX_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_DEBUG_HOST_TX_PULL_UP_SHIFT (9)
#define IOMUX_PAD_DEBUG_HOST_TX_PULL_DN (1<<8)
#define IOMUX_PAD_DEBUG_HOST_TX_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_DEBUG_HOST_TX_PULL_DN_SHIFT (8)
#define IOMUX_PAD_DEBUG_HOST_TX_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_DEBUG_HOST_TX_ANA_MASK (15<<4)
#define IOMUX_PAD_DEBUG_HOST_TX_ANA_SHIFT (4)
#define IOMUX_PAD_DEBUG_HOST_TX_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_DEBUG_HOST_TX_SEL_MASK (15<<0)
#define IOMUX_PAD_DEBUG_HOST_TX_SEL_SHIFT (0)

//pad_debug_host_clk_cfg_reg
#define IOMUX_PAD_DEBUG_HOST_CLK_OEN_FRC (1<<28)
#define IOMUX_PAD_DEBUG_HOST_CLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_DEBUG_HOST_CLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_DEBUG_HOST_CLK_OUT_FRC (1<<24)
#define IOMUX_PAD_DEBUG_HOST_CLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_DEBUG_HOST_CLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_DEBUG_HOST_CLK_OUT_REG (1<<20)
#define IOMUX_PAD_DEBUG_HOST_CLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_DEBUG_HOST_CLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_DEBUG_HOST_CLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_DEBUG_HOST_CLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_DEBUG_HOST_CLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_DEBUG_HOST_CLK_OEN_REG (1<<17)
#define IOMUX_PAD_DEBUG_HOST_CLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_DEBUG_HOST_CLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_DEBUG_HOST_CLK_PULL_FRC (1<<16)
#define IOMUX_PAD_DEBUG_HOST_CLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_DEBUG_HOST_CLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_DEBUG_HOST_CLK_PULL_UP (1<<9)
#define IOMUX_PAD_DEBUG_HOST_CLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_DEBUG_HOST_CLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_DEBUG_HOST_CLK_PULL_DN (1<<8)
#define IOMUX_PAD_DEBUG_HOST_CLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_DEBUG_HOST_CLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_DEBUG_HOST_CLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_DEBUG_HOST_CLK_ANA_MASK (15<<4)
#define IOMUX_PAD_DEBUG_HOST_CLK_ANA_SHIFT (4)
#define IOMUX_PAD_DEBUG_HOST_CLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_DEBUG_HOST_CLK_SEL_MASK (15<<0)
#define IOMUX_PAD_DEBUG_HOST_CLK_SEL_SHIFT (0)

//pad_sim_1_clk_cfg_reg
#define IOMUX_PAD_SIM_1_CLK_OEN_FRC (1<<28)
#define IOMUX_PAD_SIM_1_CLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SIM_1_CLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SIM_1_CLK_OUT_FRC (1<<24)
#define IOMUX_PAD_SIM_1_CLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SIM_1_CLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SIM_1_CLK_OUT_REG (1<<20)
#define IOMUX_PAD_SIM_1_CLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SIM_1_CLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SIM_1_CLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SIM_1_CLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SIM_1_CLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SIM_1_CLK_OEN_REG (1<<17)
#define IOMUX_PAD_SIM_1_CLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SIM_1_CLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SIM_1_CLK_PULL_FRC (1<<16)
#define IOMUX_PAD_SIM_1_CLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SIM_1_CLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SIM_1_CLK_PULL_UP (1<<9)
#define IOMUX_PAD_SIM_1_CLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SIM_1_CLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SIM_1_CLK_PULL_DN (1<<8)
#define IOMUX_PAD_SIM_1_CLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SIM_1_CLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SIM_1_CLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SIM_1_CLK_ANA_MASK (15<<4)
#define IOMUX_PAD_SIM_1_CLK_ANA_SHIFT (4)
#define IOMUX_PAD_SIM_1_CLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SIM_1_CLK_SEL_MASK (15<<0)
#define IOMUX_PAD_SIM_1_CLK_SEL_SHIFT (0)

//pad_sim_1_dio_cfg_reg
#define IOMUX_PAD_SIM_1_DIO_OEN_FRC (1<<28)
#define IOMUX_PAD_SIM_1_DIO_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SIM_1_DIO_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SIM_1_DIO_OUT_FRC (1<<24)
#define IOMUX_PAD_SIM_1_DIO_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SIM_1_DIO_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SIM_1_DIO_OUT_REG (1<<20)
#define IOMUX_PAD_SIM_1_DIO_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SIM_1_DIO_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SIM_1_DIO_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SIM_1_DIO_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SIM_1_DIO_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SIM_1_DIO_OEN_REG (1<<17)
#define IOMUX_PAD_SIM_1_DIO_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SIM_1_DIO_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SIM_1_DIO_PULL_FRC (1<<16)
#define IOMUX_PAD_SIM_1_DIO_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SIM_1_DIO_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SIM_1_DIO_PULL_UP (1<<9)
#define IOMUX_PAD_SIM_1_DIO_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SIM_1_DIO_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SIM_1_DIO_PULL_DN (1<<8)
#define IOMUX_PAD_SIM_1_DIO_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SIM_1_DIO_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SIM_1_DIO_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SIM_1_DIO_ANA_MASK (15<<4)
#define IOMUX_PAD_SIM_1_DIO_ANA_SHIFT (4)
#define IOMUX_PAD_SIM_1_DIO_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SIM_1_DIO_SEL_MASK (15<<0)
#define IOMUX_PAD_SIM_1_DIO_SEL_SHIFT (0)

//pad_sim_1_rst_cfg_reg
#define IOMUX_PAD_SIM_1_RST_OEN_FRC (1<<28)
#define IOMUX_PAD_SIM_1_RST_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SIM_1_RST_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SIM_1_RST_OUT_FRC (1<<24)
#define IOMUX_PAD_SIM_1_RST_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SIM_1_RST_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SIM_1_RST_OUT_REG (1<<20)
#define IOMUX_PAD_SIM_1_RST_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SIM_1_RST_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SIM_1_RST_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SIM_1_RST_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SIM_1_RST_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SIM_1_RST_OEN_REG (1<<17)
#define IOMUX_PAD_SIM_1_RST_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SIM_1_RST_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SIM_1_RST_PULL_FRC (1<<16)
#define IOMUX_PAD_SIM_1_RST_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SIM_1_RST_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SIM_1_RST_PULL_UP (1<<9)
#define IOMUX_PAD_SIM_1_RST_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SIM_1_RST_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SIM_1_RST_PULL_DN (1<<8)
#define IOMUX_PAD_SIM_1_RST_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SIM_1_RST_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SIM_1_RST_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SIM_1_RST_ANA_MASK (15<<4)
#define IOMUX_PAD_SIM_1_RST_ANA_SHIFT (4)
#define IOMUX_PAD_SIM_1_RST_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SIM_1_RST_SEL_MASK (15<<0)
#define IOMUX_PAD_SIM_1_RST_SEL_SHIFT (0)

//pad_sim_2_clk_cfg_reg
#define IOMUX_PAD_SIM_2_CLK_OEN_FRC (1<<28)
#define IOMUX_PAD_SIM_2_CLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SIM_2_CLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SIM_2_CLK_OUT_FRC (1<<24)
#define IOMUX_PAD_SIM_2_CLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SIM_2_CLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SIM_2_CLK_OUT_REG (1<<20)
#define IOMUX_PAD_SIM_2_CLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SIM_2_CLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SIM_2_CLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SIM_2_CLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SIM_2_CLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SIM_2_CLK_OEN_REG (1<<17)
#define IOMUX_PAD_SIM_2_CLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SIM_2_CLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SIM_2_CLK_PULL_FRC (1<<16)
#define IOMUX_PAD_SIM_2_CLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SIM_2_CLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SIM_2_CLK_PULL_UP (1<<9)
#define IOMUX_PAD_SIM_2_CLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SIM_2_CLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SIM_2_CLK_PULL_DN (1<<8)
#define IOMUX_PAD_SIM_2_CLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SIM_2_CLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SIM_2_CLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SIM_2_CLK_ANA_MASK (15<<4)
#define IOMUX_PAD_SIM_2_CLK_ANA_SHIFT (4)
#define IOMUX_PAD_SIM_2_CLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SIM_2_CLK_SEL_MASK (15<<0)
#define IOMUX_PAD_SIM_2_CLK_SEL_SHIFT (0)

//pad_sim_2_dio_cfg_reg
#define IOMUX_PAD_SIM_2_DIO_OEN_FRC (1<<28)
#define IOMUX_PAD_SIM_2_DIO_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SIM_2_DIO_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SIM_2_DIO_OUT_FRC (1<<24)
#define IOMUX_PAD_SIM_2_DIO_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SIM_2_DIO_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SIM_2_DIO_OUT_REG (1<<20)
#define IOMUX_PAD_SIM_2_DIO_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SIM_2_DIO_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SIM_2_DIO_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SIM_2_DIO_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SIM_2_DIO_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SIM_2_DIO_OEN_REG (1<<17)
#define IOMUX_PAD_SIM_2_DIO_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SIM_2_DIO_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SIM_2_DIO_PULL_FRC (1<<16)
#define IOMUX_PAD_SIM_2_DIO_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SIM_2_DIO_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SIM_2_DIO_PULL_UP (1<<9)
#define IOMUX_PAD_SIM_2_DIO_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SIM_2_DIO_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SIM_2_DIO_PULL_DN (1<<8)
#define IOMUX_PAD_SIM_2_DIO_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SIM_2_DIO_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SIM_2_DIO_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SIM_2_DIO_ANA_MASK (15<<4)
#define IOMUX_PAD_SIM_2_DIO_ANA_SHIFT (4)
#define IOMUX_PAD_SIM_2_DIO_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SIM_2_DIO_SEL_MASK (15<<0)
#define IOMUX_PAD_SIM_2_DIO_SEL_SHIFT (0)

//pad_sim_2_rst_cfg_reg
#define IOMUX_PAD_SIM_2_RST_OEN_FRC (1<<28)
#define IOMUX_PAD_SIM_2_RST_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_SIM_2_RST_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_SIM_2_RST_OUT_FRC (1<<24)
#define IOMUX_PAD_SIM_2_RST_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_SIM_2_RST_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_SIM_2_RST_OUT_REG (1<<20)
#define IOMUX_PAD_SIM_2_RST_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_SIM_2_RST_OUT_REG_SHIFT (20)
#define IOMUX_PAD_SIM_2_RST_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_SIM_2_RST_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_SIM_2_RST_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_SIM_2_RST_OEN_REG (1<<17)
#define IOMUX_PAD_SIM_2_RST_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_SIM_2_RST_OEN_REG_SHIFT (17)
#define IOMUX_PAD_SIM_2_RST_PULL_FRC (1<<16)
#define IOMUX_PAD_SIM_2_RST_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_SIM_2_RST_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_SIM_2_RST_PULL_UP (1<<9)
#define IOMUX_PAD_SIM_2_RST_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_SIM_2_RST_PULL_UP_SHIFT (9)
#define IOMUX_PAD_SIM_2_RST_PULL_DN (1<<8)
#define IOMUX_PAD_SIM_2_RST_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_SIM_2_RST_PULL_DN_SHIFT (8)
#define IOMUX_PAD_SIM_2_RST_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_SIM_2_RST_ANA_MASK (15<<4)
#define IOMUX_PAD_SIM_2_RST_ANA_SHIFT (4)
#define IOMUX_PAD_SIM_2_RST_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_SIM_2_RST_SEL_MASK (15<<0)
#define IOMUX_PAD_SIM_2_RST_SEL_SHIFT (0)

//pad_lte_reset_cfg_reg
#define IOMUX_PAD_LTE_RESET_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RESET_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RESET_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RESET_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RESET_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RESET_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RESET_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RESET_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RESET_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RESET_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RESET_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RESET_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RESET_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RESET_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RESET_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RESET_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RESET_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RESET_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RESET_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RESET_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RESET_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RESET_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RESET_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RESET_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RESET_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RESET_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RESET_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RESET_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RESET_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RESET_SEL_SHIFT (0)

//pad_lte_sysclken_cfg_reg
#define IOMUX_PAD_LTE_SYSCLKEN_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_SYSCLKEN_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_SYSCLKEN_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_SYSCLKEN_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_SYSCLKEN_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_SYSCLKEN_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_SYSCLKEN_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_SYSCLKEN_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_SYSCLKEN_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_SYSCLKEN_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_SYSCLKEN_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_SYSCLKEN_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_SYSCLKEN_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_SYSCLKEN_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_SYSCLKEN_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_SYSCLKEN_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_SYSCLKEN_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_SYSCLKEN_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_SYSCLKEN_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_SYSCLKEN_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_SYSCLKEN_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_SYSCLKEN_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_SYSCLKEN_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_SYSCLKEN_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_SYSCLKEN_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_SYSCLKEN_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_SYSCLKEN_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_SYSCLKEN_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_SYSCLKEN_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_SYSCLKEN_SEL_SHIFT (0)

//pad_lte_rfspi_cs_cfg_reg
#define IOMUX_PAD_LTE_RFSPI_CS_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RFSPI_CS_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RFSPI_CS_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RFSPI_CS_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RFSPI_CS_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RFSPI_CS_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RFSPI_CS_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RFSPI_CS_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RFSPI_CS_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RFSPI_CS_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RFSPI_CS_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RFSPI_CS_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RFSPI_CS_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RFSPI_CS_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RFSPI_CS_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RFSPI_CS_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RFSPI_CS_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RFSPI_CS_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RFSPI_CS_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RFSPI_CS_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RFSPI_CS_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RFSPI_CS_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RFSPI_CS_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RFSPI_CS_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RFSPI_CS_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RFSPI_CS_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RFSPI_CS_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RFSPI_CS_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RFSPI_CS_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RFSPI_CS_SEL_SHIFT (0)

//pad_lte_rfspi_clk_cfg_reg
#define IOMUX_PAD_LTE_RFSPI_CLK_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RFSPI_CLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RFSPI_CLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RFSPI_CLK_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RFSPI_CLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RFSPI_CLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RFSPI_CLK_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RFSPI_CLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RFSPI_CLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RFSPI_CLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RFSPI_CLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RFSPI_CLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RFSPI_CLK_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RFSPI_CLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RFSPI_CLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RFSPI_CLK_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RFSPI_CLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RFSPI_CLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RFSPI_CLK_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RFSPI_CLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RFSPI_CLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RFSPI_CLK_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RFSPI_CLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RFSPI_CLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RFSPI_CLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RFSPI_CLK_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RFSPI_CLK_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RFSPI_CLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RFSPI_CLK_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RFSPI_CLK_SEL_SHIFT (0)

//pad_lte_rfspi_dio_cfg_reg
#define IOMUX_PAD_LTE_RFSPI_DIO_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RFSPI_DIO_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RFSPI_DIO_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RFSPI_DIO_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RFSPI_DIO_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RFSPI_DIO_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RFSPI_DIO_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RFSPI_DIO_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RFSPI_DIO_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RFSPI_DIO_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RFSPI_DIO_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RFSPI_DIO_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RFSPI_DIO_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RFSPI_DIO_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RFSPI_DIO_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RFSPI_DIO_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RFSPI_DIO_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RFSPI_DIO_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RFSPI_DIO_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RFSPI_DIO_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RFSPI_DIO_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RFSPI_DIO_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RFSPI_DIO_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RFSPI_DIO_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RFSPI_DIO_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RFSPI_DIO_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RFSPI_DIO_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RFSPI_DIO_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RFSPI_DIO_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RFSPI_DIO_SEL_SHIFT (0)

//pad_lte_rbdp_rx_0_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_0_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_0_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_0_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_0_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_0_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_0_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_0_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_0_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_0_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_0_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_0_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_0_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_0_SEL_SHIFT (0)

//pad_lte_rbdp_rx_1_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_1_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_1_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_1_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_1_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_1_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_1_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_1_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_1_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_1_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_1_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_1_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_1_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_1_SEL_SHIFT (0)

//pad_lte_rbdp_rx_2_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_2_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_2_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_2_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_2_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_2_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_2_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_2_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_2_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_2_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_2_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_2_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_2_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_2_SEL_SHIFT (0)

//pad_lte_rbdp_rx_3_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_3_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_3_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_3_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_3_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_3_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_3_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_3_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_3_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_3_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_3_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_3_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_3_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_3_SEL_SHIFT (0)

//pad_lte_rbdp_rx_4_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_4_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_4_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_4_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_4_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_4_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_4_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_4_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_4_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_4_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_4_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_4_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_4_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_4_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_4_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_4_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_4_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_4_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_4_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_4_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_4_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_4_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_4_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_4_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_4_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_4_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_4_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_4_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_4_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_4_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_4_SEL_SHIFT (0)

//pad_lte_rbdp_rx_5_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_5_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_5_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_5_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_5_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_5_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_5_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_5_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_5_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_5_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_5_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_5_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_5_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_5_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_5_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_5_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_5_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_5_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_5_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_5_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_5_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_5_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_5_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_5_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_5_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_5_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_5_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_5_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_5_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_5_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_5_SEL_SHIFT (0)

//pad_lte_rbdp_rx_6_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_6_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_6_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_6_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_6_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_6_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_6_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_6_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_6_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_6_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_6_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_6_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_6_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_6_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_6_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_6_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_6_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_6_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_6_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_6_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_6_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_6_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_6_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_6_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_6_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_6_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_6_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_6_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_6_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_6_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_6_SEL_SHIFT (0)

//pad_lte_rbdp_rx_7_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_7_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_7_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_7_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_7_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_7_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_7_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_7_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_7_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_7_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_7_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_7_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_7_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_7_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_7_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_7_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_7_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_7_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_7_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_7_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_7_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_7_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_7_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_7_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_7_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_7_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_7_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_7_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_7_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_7_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_7_SEL_SHIFT (0)

//pad_lte_rbdp_rx_8_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_8_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_8_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_8_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_8_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_8_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_8_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_8_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_8_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_8_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_8_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_8_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_8_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_8_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_8_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_8_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_8_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_8_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_8_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_8_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_8_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_8_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_8_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_8_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_8_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_8_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_8_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_8_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_8_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_8_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_8_SEL_SHIFT (0)

//pad_lte_rbdp_rx_9_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_9_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_9_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_9_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_9_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_9_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_9_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_9_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_9_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_9_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_9_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_9_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_9_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_9_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_9_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_9_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_9_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_9_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_9_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_9_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_9_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_9_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_9_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_9_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_9_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_9_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_9_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_9_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_9_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_9_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_9_SEL_SHIFT (0)

//pad_lte_rbdp_rx_10_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_10_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_10_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_10_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_10_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_10_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_10_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_10_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_10_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_10_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_10_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_10_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_10_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_10_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_10_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_10_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_10_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_10_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_10_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_10_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_10_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_10_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_10_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_10_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_10_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_10_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_10_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_10_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_10_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_10_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_10_SEL_SHIFT (0)

//pad_lte_rbdp_rx_11_cfg_reg
#define IOMUX_PAD_LTE_RBDP_RX_11_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_11_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_RX_11_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_RX_11_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_11_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_RX_11_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_RX_11_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_11_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_RX_11_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_RX_11_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_RX_11_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_RX_11_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_RX_11_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_11_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_RX_11_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_RX_11_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_11_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_RX_11_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_RX_11_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_11_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_RX_11_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_RX_11_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_11_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_RX_11_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_RX_11_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_RX_11_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_RX_11_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_RX_11_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_RX_11_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_RX_11_SEL_SHIFT (0)

//pad_lte_rbdp_tx_0_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_0_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_0_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_0_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_0_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_0_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_0_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_0_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_0_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_0_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_0_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_0_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_0_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_0_SEL_SHIFT (0)

//pad_lte_rbdp_tx_1_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_1_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_1_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_1_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_1_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_1_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_1_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_1_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_1_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_1_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_1_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_1_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_1_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_1_SEL_SHIFT (0)

//pad_lte_rbdp_tx_2_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_2_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_2_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_2_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_2_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_2_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_2_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_2_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_2_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_2_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_2_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_2_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_2_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_2_SEL_SHIFT (0)

//pad_lte_rbdp_tx_3_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_3_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_3_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_3_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_3_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_3_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_3_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_3_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_3_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_3_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_3_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_3_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_3_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_3_SEL_SHIFT (0)

//pad_lte_rbdp_tx_4_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_4_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_4_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_4_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_4_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_4_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_4_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_4_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_4_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_4_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_4_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_4_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_4_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_4_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_4_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_4_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_4_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_4_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_4_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_4_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_4_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_4_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_4_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_4_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_4_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_4_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_4_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_4_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_4_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_4_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_4_SEL_SHIFT (0)

//pad_lte_rbdp_tx_5_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_5_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_5_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_5_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_5_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_5_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_5_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_5_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_5_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_5_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_5_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_5_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_5_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_5_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_5_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_5_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_5_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_5_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_5_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_5_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_5_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_5_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_5_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_5_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_5_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_5_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_5_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_5_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_5_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_5_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_5_SEL_SHIFT (0)

//pad_lte_rbdp_tx_6_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_6_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_6_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_6_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_6_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_6_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_6_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_6_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_6_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_6_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_6_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_6_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_6_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_6_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_6_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_6_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_6_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_6_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_6_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_6_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_6_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_6_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_6_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_6_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_6_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_6_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_6_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_6_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_6_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_6_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_6_SEL_SHIFT (0)

//pad_lte_rbdp_tx_7_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_7_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_7_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_7_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_7_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_7_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_7_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_7_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_7_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_7_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_7_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_7_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_7_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_7_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_7_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_7_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_7_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_7_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_7_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_7_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_7_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_7_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_7_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_7_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_7_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_7_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_7_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_7_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_7_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_7_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_7_SEL_SHIFT (0)

//pad_lte_rbdp_tx_8_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_8_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_8_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_8_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_8_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_8_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_8_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_8_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_8_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_8_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_8_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_8_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_8_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_8_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_8_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_8_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_8_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_8_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_8_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_8_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_8_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_8_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_8_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_8_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_8_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_8_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_8_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_8_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_8_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_8_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_8_SEL_SHIFT (0)

//pad_lte_rbdp_tx_9_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_9_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_9_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_9_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_9_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_9_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_9_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_9_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_9_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_9_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_9_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_9_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_9_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_9_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_9_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_9_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_9_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_9_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_9_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_9_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_9_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_9_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_9_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_9_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_9_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_9_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_9_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_9_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_9_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_9_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_9_SEL_SHIFT (0)

//pad_lte_rbdp_tx_10_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_10_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_10_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_10_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_10_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_10_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_10_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_10_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_10_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_10_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_10_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_10_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_10_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_10_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_10_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_10_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_10_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_10_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_10_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_10_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_10_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_10_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_10_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_10_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_10_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_10_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_10_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_10_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_10_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_10_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_10_SEL_SHIFT (0)

//pad_lte_rbdp_tx_11_cfg_reg
#define IOMUX_PAD_LTE_RBDP_TX_11_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_11_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RBDP_TX_11_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RBDP_TX_11_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_11_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RBDP_TX_11_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RBDP_TX_11_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_11_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RBDP_TX_11_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RBDP_TX_11_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RBDP_TX_11_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RBDP_TX_11_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RBDP_TX_11_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_11_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RBDP_TX_11_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RBDP_TX_11_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_11_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RBDP_TX_11_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RBDP_TX_11_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_11_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RBDP_TX_11_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RBDP_TX_11_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_11_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RBDP_TX_11_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RBDP_TX_11_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RBDP_TX_11_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RBDP_TX_11_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RBDP_TX_11_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RBDP_TX_11_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RBDP_TX_11_SEL_SHIFT (0)

//pad_lte_mclk_cfg_reg
#define IOMUX_PAD_LTE_MCLK_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_MCLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_MCLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_MCLK_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_MCLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_MCLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_MCLK_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_MCLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_MCLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_MCLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_MCLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_MCLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_MCLK_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_MCLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_MCLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_MCLK_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_MCLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_MCLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_MCLK_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_MCLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_MCLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_MCLK_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_MCLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_MCLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_MCLK_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_LTE_MCLK_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_MCLK_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_MCLK_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_LTE_MCLK_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_MCLK_SEL_SHIFT (0)

//pad_lte_rxframe_cfg_reg
#define IOMUX_PAD_LTE_RXFRAME_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RXFRAME_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RXFRAME_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RXFRAME_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RXFRAME_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RXFRAME_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RXFRAME_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RXFRAME_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RXFRAME_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RXFRAME_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RXFRAME_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RXFRAME_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RXFRAME_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RXFRAME_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RXFRAME_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RXFRAME_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RXFRAME_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RXFRAME_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RXFRAME_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RXFRAME_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RXFRAME_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RXFRAME_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RXFRAME_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RXFRAME_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RXFRAME_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RXFRAME_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RXFRAME_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RXFRAME_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RXFRAME_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RXFRAME_SEL_SHIFT (0)

//pad_lte_fclk_cfg_reg
#define IOMUX_PAD_LTE_FCLK_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_FCLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_FCLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_FCLK_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_FCLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_FCLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_FCLK_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_FCLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_FCLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_FCLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_FCLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_FCLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_FCLK_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_FCLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_FCLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_FCLK_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_FCLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_FCLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_FCLK_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_FCLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_FCLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_FCLK_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_FCLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_FCLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_FCLK_ANA(n)  (((n)&15)<<4)
#define IOMUX_PAD_LTE_FCLK_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_FCLK_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_FCLK_SEL(n)  (((n)&15)<<0)
#define IOMUX_PAD_LTE_FCLK_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_FCLK_SEL_SHIFT (0)

//pad_lte_txframe_cfg_reg
#define IOMUX_PAD_LTE_TXFRAME_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_TXFRAME_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_TXFRAME_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_TXFRAME_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_TXFRAME_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_TXFRAME_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_TXFRAME_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_TXFRAME_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_TXFRAME_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_TXFRAME_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_TXFRAME_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_TXFRAME_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_TXFRAME_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_TXFRAME_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_TXFRAME_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_TXFRAME_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_TXFRAME_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_TXFRAME_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_TXFRAME_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_TXFRAME_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_TXFRAME_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_TXFRAME_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_TXFRAME_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_TXFRAME_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_TXFRAME_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_TXFRAME_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_TXFRAME_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_TXFRAME_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_TXFRAME_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_TXFRAME_SEL_SHIFT (0)

//pad_lte_rxtxen_cfg_reg
#define IOMUX_PAD_LTE_RXTXEN_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RXTXEN_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RXTXEN_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RXTXEN_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RXTXEN_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RXTXEN_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RXTXEN_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RXTXEN_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RXTXEN_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RXTXEN_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RXTXEN_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RXTXEN_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RXTXEN_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RXTXEN_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RXTXEN_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RXTXEN_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RXTXEN_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RXTXEN_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RXTXEN_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RXTXEN_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RXTXEN_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RXTXEN_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RXTXEN_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RXTXEN_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RXTXEN_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RXTXEN_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RXTXEN_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RXTXEN_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RXTXEN_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RXTXEN_SEL_SHIFT (0)

//pad_lte_rxtxdata_cfg_reg
#define IOMUX_PAD_LTE_RXTXDATA_OEN_FRC (1<<28)
#define IOMUX_PAD_LTE_RXTXDATA_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_LTE_RXTXDATA_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_LTE_RXTXDATA_OUT_FRC (1<<24)
#define IOMUX_PAD_LTE_RXTXDATA_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_LTE_RXTXDATA_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_LTE_RXTXDATA_OUT_REG (1<<20)
#define IOMUX_PAD_LTE_RXTXDATA_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_LTE_RXTXDATA_OUT_REG_SHIFT (20)
#define IOMUX_PAD_LTE_RXTXDATA_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_LTE_RXTXDATA_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_LTE_RXTXDATA_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_LTE_RXTXDATA_OEN_REG (1<<17)
#define IOMUX_PAD_LTE_RXTXDATA_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_LTE_RXTXDATA_OEN_REG_SHIFT (17)
#define IOMUX_PAD_LTE_RXTXDATA_PULL_FRC (1<<16)
#define IOMUX_PAD_LTE_RXTXDATA_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_LTE_RXTXDATA_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_LTE_RXTXDATA_PULL_UP (1<<9)
#define IOMUX_PAD_LTE_RXTXDATA_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_LTE_RXTXDATA_PULL_UP_SHIFT (9)
#define IOMUX_PAD_LTE_RXTXDATA_PULL_DN (1<<8)
#define IOMUX_PAD_LTE_RXTXDATA_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_LTE_RXTXDATA_PULL_DN_SHIFT (8)
#define IOMUX_PAD_LTE_RXTXDATA_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_LTE_RXTXDATA_ANA_MASK (15<<4)
#define IOMUX_PAD_LTE_RXTXDATA_ANA_SHIFT (4)
#define IOMUX_PAD_LTE_RXTXDATA_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_LTE_RXTXDATA_SEL_MASK (15<<0)
#define IOMUX_PAD_LTE_RXTXDATA_SEL_SHIFT (0)

//pad_rfdig_gpio_0_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_0_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_0_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_0_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_0_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_0_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_0_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_0_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_0_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_0_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_0_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_0_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_0_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_0_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_0_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_0_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_0_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_0_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_0_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_0_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_0_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_0_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_0_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_0_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_0_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_0_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_0_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_0_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_0_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_0_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_0_SEL_SHIFT (0)

//pad_rfdig_gpio_1_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_1_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_1_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_1_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_1_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_1_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_1_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_1_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_1_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_1_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_1_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_1_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_1_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_1_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_1_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_1_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_1_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_1_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_1_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_1_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_1_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_1_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_1_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_1_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_1_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_1_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_1_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_1_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_1_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_1_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_1_SEL_SHIFT (0)

//pad_rfdig_gpio_2_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_2_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_2_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_2_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_2_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_2_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_2_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_2_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_2_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_2_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_2_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_2_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_2_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_2_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_2_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_2_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_2_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_2_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_2_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_2_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_2_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_2_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_2_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_2_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_2_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_2_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_2_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_2_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_2_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_2_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_2_SEL_SHIFT (0)

//pad_rfdig_gpio_3_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_3_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_3_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_3_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_3_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_3_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_3_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_3_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_3_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_3_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_3_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_3_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_3_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_3_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_3_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_3_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_3_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_3_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_3_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_3_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_3_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_3_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_3_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_3_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_3_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_3_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_3_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_3_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_3_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_3_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_3_SEL_SHIFT (0)

//pad_rfdig_gpio_4_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_4_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_4_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_4_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_4_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_4_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_4_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_4_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_4_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_4_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_4_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_4_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_4_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_4_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_4_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_4_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_4_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_4_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_4_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_4_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_4_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_4_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_4_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_4_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_4_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_4_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_4_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_4_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_4_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_4_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_4_SEL_SHIFT (0)

//pad_rfdig_gpio_5_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_5_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_5_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_5_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_5_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_5_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_5_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_5_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_5_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_5_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_5_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_5_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_5_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_5_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_5_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_5_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_5_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_5_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_5_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_5_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_5_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_5_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_5_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_5_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_5_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_5_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_5_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_5_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_5_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_5_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_5_SEL_SHIFT (0)

//pad_rfdig_gpio_6_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_6_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_6_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_6_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_6_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_6_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_6_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_6_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_6_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_6_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_6_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_6_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_6_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_6_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_6_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_6_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_6_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_6_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_6_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_6_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_6_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_6_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_6_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_6_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_6_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_6_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_6_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_6_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_6_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_6_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_6_SEL_SHIFT (0)

//pad_rfdig_gpio_7_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_7_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_7_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_7_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_7_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_7_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_7_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_7_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_7_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_7_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_7_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_7_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_7_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_7_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_7_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_7_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_7_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_7_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_7_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_7_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_7_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_7_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_7_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_7_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_7_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_7_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_7_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_7_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_7_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_7_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_7_SEL_SHIFT (0)

//pad_rfdig_gpio_8_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_8_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_8_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_8_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_8_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_8_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_8_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_8_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_8_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_8_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_8_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_8_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_8_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_8_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_8_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_8_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_8_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_8_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_8_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_8_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_8_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_8_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_8_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_8_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_8_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_8_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_8_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_8_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_8_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_8_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_8_SEL_SHIFT (0)

//pad_rfdig_gpio_9_cfg_reg
#define IOMUX_PAD_RFDIG_GPIO_9_OEN_FRC (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_9_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RFDIG_GPIO_9_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RFDIG_GPIO_9_OUT_FRC (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_9_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RFDIG_GPIO_9_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RFDIG_GPIO_9_OUT_REG (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_9_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RFDIG_GPIO_9_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RFDIG_GPIO_9_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RFDIG_GPIO_9_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RFDIG_GPIO_9_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RFDIG_GPIO_9_OEN_REG (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_9_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RFDIG_GPIO_9_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RFDIG_GPIO_9_PULL_FRC (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_9_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RFDIG_GPIO_9_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RFDIG_GPIO_9_PULL_UP (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_9_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RFDIG_GPIO_9_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RFDIG_GPIO_9_PULL_DN (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_9_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RFDIG_GPIO_9_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RFDIG_GPIO_9_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RFDIG_GPIO_9_ANA_MASK (15<<4)
#define IOMUX_PAD_RFDIG_GPIO_9_ANA_SHIFT (4)
#define IOMUX_PAD_RFDIG_GPIO_9_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RFDIG_GPIO_9_SEL_MASK (15<<0)
#define IOMUX_PAD_RFDIG_GPIO_9_SEL_SHIFT (0)

//pad_rf_gge_xen_cfg_reg
#define IOMUX_PAD_RF_GGE_XEN_OEN_FRC (1<<28)
#define IOMUX_PAD_RF_GGE_XEN_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RF_GGE_XEN_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RF_GGE_XEN_OUT_FRC (1<<24)
#define IOMUX_PAD_RF_GGE_XEN_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RF_GGE_XEN_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RF_GGE_XEN_OUT_REG (1<<20)
#define IOMUX_PAD_RF_GGE_XEN_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RF_GGE_XEN_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RF_GGE_XEN_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RF_GGE_XEN_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RF_GGE_XEN_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RF_GGE_XEN_OEN_REG (1<<17)
#define IOMUX_PAD_RF_GGE_XEN_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RF_GGE_XEN_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RF_GGE_XEN_PULL_FRC (1<<16)
#define IOMUX_PAD_RF_GGE_XEN_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RF_GGE_XEN_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RF_GGE_XEN_PULL_UP (1<<9)
#define IOMUX_PAD_RF_GGE_XEN_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RF_GGE_XEN_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RF_GGE_XEN_PULL_DN (1<<8)
#define IOMUX_PAD_RF_GGE_XEN_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RF_GGE_XEN_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RF_GGE_XEN_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RF_GGE_XEN_ANA_MASK (15<<4)
#define IOMUX_PAD_RF_GGE_XEN_ANA_SHIFT (4)
#define IOMUX_PAD_RF_GGE_XEN_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RF_GGE_XEN_SEL_MASK (15<<0)
#define IOMUX_PAD_RF_GGE_XEN_SEL_SHIFT (0)

//pad_rf_gge_pdn_cfg_reg
#define IOMUX_PAD_RF_GGE_PDN_OEN_FRC (1<<28)
#define IOMUX_PAD_RF_GGE_PDN_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RF_GGE_PDN_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RF_GGE_PDN_OUT_FRC (1<<24)
#define IOMUX_PAD_RF_GGE_PDN_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RF_GGE_PDN_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RF_GGE_PDN_OUT_REG (1<<20)
#define IOMUX_PAD_RF_GGE_PDN_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RF_GGE_PDN_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RF_GGE_PDN_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RF_GGE_PDN_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RF_GGE_PDN_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RF_GGE_PDN_OEN_REG (1<<17)
#define IOMUX_PAD_RF_GGE_PDN_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RF_GGE_PDN_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RF_GGE_PDN_PULL_FRC (1<<16)
#define IOMUX_PAD_RF_GGE_PDN_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RF_GGE_PDN_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RF_GGE_PDN_PULL_UP (1<<9)
#define IOMUX_PAD_RF_GGE_PDN_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RF_GGE_PDN_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RF_GGE_PDN_PULL_DN (1<<8)
#define IOMUX_PAD_RF_GGE_PDN_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RF_GGE_PDN_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RF_GGE_PDN_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RF_GGE_PDN_ANA_MASK (15<<4)
#define IOMUX_PAD_RF_GGE_PDN_ANA_SHIFT (4)
#define IOMUX_PAD_RF_GGE_PDN_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RF_GGE_PDN_SEL_MASK (15<<0)
#define IOMUX_PAD_RF_GGE_PDN_SEL_SHIFT (0)

//pad_rf_gge_rfspi_cs_cfg_reg
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OEN_FRC (1<<28)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OUT_FRC (1<<24)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OUT_REG (1<<20)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OEN_REG (1<<17)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_PULL_FRC (1<<16)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_PULL_UP (1<<9)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_PULL_DN (1<<8)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_ANA_MASK (15<<4)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_ANA_SHIFT (4)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_SEL_MASK (15<<0)
#define IOMUX_PAD_RF_GGE_RFSPI_CS_SEL_SHIFT (0)

//pad_rf_gge_rfspi_sclk_cfg_reg
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OEN_FRC (1<<28)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OUT_FRC (1<<24)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OUT_REG (1<<20)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OEN_REG (1<<17)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_PULL_FRC (1<<16)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_PULL_UP (1<<9)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_PULL_DN (1<<8)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_ANA_MASK (15<<4)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_ANA_SHIFT (4)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_SEL_MASK (15<<0)
#define IOMUX_PAD_RF_GGE_RFSPI_SCLK_SEL_SHIFT (0)

//pad_rf_gge_rfspi_sdio_cfg_reg
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OEN_FRC (1<<28)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OUT_FRC (1<<24)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OUT_REG (1<<20)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OEN_REG (1<<17)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_PULL_FRC (1<<16)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_PULL_UP (1<<9)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_PULL_DN (1<<8)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_ANA_MASK (15<<4)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_ANA_SHIFT (4)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_SEL_MASK (15<<0)
#define IOMUX_PAD_RF_GGE_RFSPI_SDIO_SEL_SHIFT (0)

//pad_rf_gge_strobe_cfg_reg
#define IOMUX_PAD_RF_GGE_STROBE_OEN_FRC (1<<28)
#define IOMUX_PAD_RF_GGE_STROBE_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_RF_GGE_STROBE_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_RF_GGE_STROBE_OUT_FRC (1<<24)
#define IOMUX_PAD_RF_GGE_STROBE_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_RF_GGE_STROBE_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_RF_GGE_STROBE_OUT_REG (1<<20)
#define IOMUX_PAD_RF_GGE_STROBE_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_RF_GGE_STROBE_OUT_REG_SHIFT (20)
#define IOMUX_PAD_RF_GGE_STROBE_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_RF_GGE_STROBE_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_RF_GGE_STROBE_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_RF_GGE_STROBE_OEN_REG (1<<17)
#define IOMUX_PAD_RF_GGE_STROBE_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_RF_GGE_STROBE_OEN_REG_SHIFT (17)
#define IOMUX_PAD_RF_GGE_STROBE_PULL_FRC (1<<16)
#define IOMUX_PAD_RF_GGE_STROBE_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_RF_GGE_STROBE_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_RF_GGE_STROBE_PULL_UP (1<<9)
#define IOMUX_PAD_RF_GGE_STROBE_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_RF_GGE_STROBE_PULL_UP_SHIFT (9)
#define IOMUX_PAD_RF_GGE_STROBE_PULL_DN (1<<8)
#define IOMUX_PAD_RF_GGE_STROBE_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_RF_GGE_STROBE_PULL_DN_SHIFT (8)
#define IOMUX_PAD_RF_GGE_STROBE_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_RF_GGE_STROBE_ANA_MASK (15<<4)
#define IOMUX_PAD_RF_GGE_STROBE_ANA_SHIFT (4)
#define IOMUX_PAD_RF_GGE_STROBE_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_RF_GGE_STROBE_SEL_MASK (15<<0)
#define IOMUX_PAD_RF_GGE_STROBE_SEL_SHIFT (0)

//pad_codec_mclk_cfg_reg
#define IOMUX_PAD_CODEC_MCLK_OEN_FRC (1<<28)
#define IOMUX_PAD_CODEC_MCLK_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CODEC_MCLK_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CODEC_MCLK_OUT_FRC (1<<24)
#define IOMUX_PAD_CODEC_MCLK_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CODEC_MCLK_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CODEC_MCLK_OUT_REG (1<<20)
#define IOMUX_PAD_CODEC_MCLK_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CODEC_MCLK_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CODEC_MCLK_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CODEC_MCLK_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CODEC_MCLK_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CODEC_MCLK_OEN_REG (1<<17)
#define IOMUX_PAD_CODEC_MCLK_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CODEC_MCLK_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CODEC_MCLK_PULL_FRC (1<<16)
#define IOMUX_PAD_CODEC_MCLK_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CODEC_MCLK_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CODEC_MCLK_PULL_UP (1<<9)
#define IOMUX_PAD_CODEC_MCLK_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CODEC_MCLK_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CODEC_MCLK_PULL_DN (1<<8)
#define IOMUX_PAD_CODEC_MCLK_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CODEC_MCLK_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CODEC_MCLK_ANA(n) (((n)&15)<<4)
#define IOMUX_PAD_CODEC_MCLK_ANA_MASK (15<<4)
#define IOMUX_PAD_CODEC_MCLK_ANA_SHIFT (4)
#define IOMUX_PAD_CODEC_MCLK_SEL(n) (((n)&15)<<0)
#define IOMUX_PAD_CODEC_MCLK_SEL_MASK (15<<0)
#define IOMUX_PAD_CODEC_MCLK_SEL_SHIFT (0)

//pad_chip_pd_cfg_reg
#define IOMUX_PAD_CHIP_PD_OEN_FRC  (1<<28)
#define IOMUX_PAD_CHIP_PD_OEN_FRC_MASK (1<<28)
#define IOMUX_PAD_CHIP_PD_OEN_FRC_SHIFT (28)
#define IOMUX_PAD_CHIP_PD_OUT_FRC  (1<<24)
#define IOMUX_PAD_CHIP_PD_OUT_FRC_MASK (1<<24)
#define IOMUX_PAD_CHIP_PD_OUT_FRC_SHIFT (24)
#define IOMUX_PAD_CHIP_PD_OUT_REG  (1<<20)
#define IOMUX_PAD_CHIP_PD_OUT_REG_MASK (1<<20)
#define IOMUX_PAD_CHIP_PD_OUT_REG_SHIFT (20)
#define IOMUX_PAD_CHIP_PD_DRV_STRENGTH(n) (((n)&3)<<18)
#define IOMUX_PAD_CHIP_PD_DRV_STRENGTH_MASK (3<<18)
#define IOMUX_PAD_CHIP_PD_DRV_STRENGTH_SHIFT (18)
#define IOMUX_PAD_CHIP_PD_OEN_REG  (1<<17)
#define IOMUX_PAD_CHIP_PD_OEN_REG_MASK (1<<17)
#define IOMUX_PAD_CHIP_PD_OEN_REG_SHIFT (17)
#define IOMUX_PAD_CHIP_PD_PULL_FRC (1<<16)
#define IOMUX_PAD_CHIP_PD_PULL_FRC_MASK (1<<16)
#define IOMUX_PAD_CHIP_PD_PULL_FRC_SHIFT (16)
#define IOMUX_PAD_CHIP_PD_PULL_UP  (1<<9)
#define IOMUX_PAD_CHIP_PD_PULL_UP_MASK (1<<9)
#define IOMUX_PAD_CHIP_PD_PULL_UP_SHIFT (9)
#define IOMUX_PAD_CHIP_PD_PULL_DN  (1<<8)
#define IOMUX_PAD_CHIP_PD_PULL_DN_MASK (1<<8)
#define IOMUX_PAD_CHIP_PD_PULL_DN_SHIFT (8)
#define IOMUX_PAD_CHIP_PD_ANA(n)   (((n)&15)<<4)
#define IOMUX_PAD_CHIP_PD_ANA_MASK (15<<4)
#define IOMUX_PAD_CHIP_PD_ANA_SHIFT (4)
#define IOMUX_PAD_CHIP_PD_SEL(n)   (((n)&15)<<0)
#define IOMUX_PAD_CHIP_PD_SEL_MASK (15<<0)
#define IOMUX_PAD_CHIP_PD_SEL_SHIFT (0)

/*adapt MOCOR IP*/
#define PIN_FPD_EN				BIT_8			// Weak pull down for function mode
#define PIN_FPU_EN				BIT_9			// Weak pull up for function mode

#define PIN_Z_EN                                0x00            // High-Z in sleep mode
#define PIN_O_EN                                0x00           // Output enable in sleep mode
#define PIN_I_EN                                0x00           // Input enable in sleep mode



#endif

