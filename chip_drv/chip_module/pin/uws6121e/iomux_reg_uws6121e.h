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

#define REG_IOMUX_BASE             0x51510000
#define REG_IOMUX_SET_OFFSET (0x1000)
#define REG_IOMUX_CLR_OFFSET (0x2000)

#undef uint32_t
#define uint32_t uint32
//typedef unsigned int uint32_t;
typedef volatile struct
{
    uint32_t pwr_pad_ctl;              // 0x00000000
    uint32_t pin_ctrl_reg0;            // 0x00000004
    uint32_t pin_ctrl_reg1;            // 0x00000008
    uint32_t pin_ctrl_reg2;            // 0x0000000c
    uint32_t pin_ctrl_reg3;            // 0x00000010
    uint32_t pin_ctrl_reg4;            // 0x00000014
    uint32_t pin_ctrl_reg5;            // 0x00000018
    uint32_t rfdig_gpio_7;             // 0x0000001c
    uint32_t rfdig_gpio_6;             // 0x00000020
    uint32_t rfdig_gpio_5;             // 0x00000024
    uint32_t rfdig_gpio_4;             // 0x00000028
    uint32_t rfdig_gpio_3;             // 0x0000002c
    uint32_t rfdig_gpio_2;             // 0x00000030
    uint32_t rfdig_gpio_1;             // 0x00000034
    uint32_t rfdig_gpio_0;             // 0x00000038
    uint32_t keyin_4;                  // 0x0000003c
    uint32_t keyout_5;                 // 0x00000040
    uint32_t keyin_5;                  // 0x00000044
    uint32_t keyout_4;                 // 0x00000048
    uint32_t uart_1_rts;               // 0x0000004c
    uint32_t uart_1_txd;               // 0x00000050
    uint32_t uart_1_rxd;               // 0x00000054
    uint32_t uart_1_cts;               // 0x00000058
    uint32_t gpio_0;                   // 0x0000005c
    uint32_t gpio_3;                   // 0x00000060
    uint32_t gpio_2;                   // 0x00000064
    uint32_t gpio_1;                   // 0x00000068
    uint32_t gpio_7;                   // 0x0000006c
    uint32_t gpio_6;                   // 0x00000070
    uint32_t gpio_5;                   // 0x00000074
    uint32_t gpio_4;                   // 0x00000078
    uint32_t adi_sda;                  // 0x0000007c
    uint32_t adi_scl;                  // 0x00000080
    uint32_t resetb;                   // 0x00000084
    uint32_t osc_32k;                  // 0x00000088
    uint32_t pmic_ext_int;             // 0x0000008c
    uint32_t chip_pd;                  // 0x00000090
    uint32_t ptest;                    // 0x00000094
    uint32_t clk26m_pmic;              // 0x00000098
    uint32_t sim_1_rst;                // 0x0000009c
    uint32_t sim_1_dio;                // 0x000000a0
    uint32_t sim_1_clk;                // 0x000000a4
    uint32_t sim_0_rst;                // 0x000000a8
    uint32_t sim_0_dio;                // 0x000000ac
    uint32_t sim_0_clk;                // 0x000000b0
    uint32_t sw_clk;                   // 0x000000b4
    uint32_t sw_dio;                   // 0x000000b8
    uint32_t debug_host_tx;            // 0x000000bc
    uint32_t debug_host_rx;            // 0x000000c0
    uint32_t debug_host_clk;           // 0x000000c4
    uint32_t camera_rst_l;             // 0x000000c8
    uint32_t spi_camera_sck;           // 0x000000cc
    uint32_t spi_camera_si_1;          // 0x000000d0
    uint32_t spi_camera_si_0;          // 0x000000d4
    uint32_t camera_ref_clk;           // 0x000000d8
    uint32_t camera_pwdn;              // 0x000000dc
    uint32_t i2s_sdat_i;               // 0x000000e0
    uint32_t i2s1_sdat_o;              // 0x000000e4
    uint32_t i2s1_lrck;                // 0x000000e8
    uint32_t i2s1_bck;                 // 0x000000ec
    uint32_t i2s1_mclk;                // 0x000000f0
    uint32_t i2c_m2_scl;               // 0x000000f4
    uint32_t i2c_m2_sda;               // 0x000000f8
    uint32_t nand_sel;                 // 0x000000fc
    uint32_t keyout_3;                 // 0x00000100
    uint32_t keyout_2;                 // 0x00000104
    uint32_t keyout_1;                 // 0x00000108
    uint32_t keyout_0;                 // 0x0000010c
    uint32_t keyin_3;                  // 0x00000110
    uint32_t keyin_2;                  // 0x00000114
    uint32_t keyin_1;                  // 0x00000118
    uint32_t keyin_0;                  // 0x0000011c
    uint32_t lcd_rstb;                 // 0x00000120
    uint32_t lcd_fmark;                // 0x00000124
    uint32_t spi_lcd_select;           // 0x00000128
    uint32_t spi_lcd_cs;               // 0x0000012c
    uint32_t spi_lcd_clk;              // 0x00000130
    uint32_t spi_lcd_sdc;              // 0x00000134
    uint32_t spi_lcd_sio;              // 0x00000138
    uint32_t sdmmc1_rst;               // 0x0000013c
    uint32_t sdmmc1_data_7;            // 0x00000140
    uint32_t sdmmc1_data_6;            // 0x00000144
    uint32_t sdmmc1_data_5;            // 0x00000148
    uint32_t sdmmc1_data_4;            // 0x0000014c
    uint32_t sdmmc1_data_3;            // 0x00000150
    uint32_t sdmmc1_data_2;            // 0x00000154
    uint32_t sdmmc1_data_1;            // 0x00000158
    uint32_t sdmmc1_data_0;            // 0x0000015c
    uint32_t sdmmc1_cmd;               // 0x00000160
    uint32_t sdmmc1_clk;               // 0x00000164
    uint32_t uart_2_rts;               // 0x00000168
    uint32_t uart_2_cts;               // 0x0000016c
    uint32_t uart_2_txd;               // 0x00000170
    uint32_t uart_2_rxd;               // 0x00000174
    uint32_t i2c_m1_sda;               // 0x00000178
    uint32_t i2c_m1_scl;               // 0x0000017c
    uint32_t gpio_23;                  // 0x00000180
    uint32_t gpio_22;                  // 0x00000184
    uint32_t gpio_21;                  // 0x00000188
    uint32_t gpio_20;                  // 0x0000018c
    uint32_t gpio_19;                  // 0x00000190
    uint32_t gpio_18;                  // 0x00000194
    uint32_t gpio_17;                  // 0x00000198
    uint32_t gpio_16;                  // 0x0000019c
    uint32_t m_spi_d_3;                // 0x000001a0
    uint32_t m_spi_d_2;                // 0x000001a4
    uint32_t m_spi_d_1;                // 0x000001a8
    uint32_t m_spi_d_0;                // 0x000001ac
    uint32_t m_spi_cs;                 // 0x000001b0
    uint32_t m_spi_clk;                // 0x000001b4
    uint32_t __440[153];               // 0x000001b8
    uint32_t pad_rfdig_gpio_7;         // 0x0000041c
    uint32_t pad_rfdig_gpio_6;         // 0x00000420
    uint32_t pad_rfdig_gpio_5;         // 0x00000424
    uint32_t pad_rfdig_gpio_4;         // 0x00000428
    uint32_t pad_rfdig_gpio_3;         // 0x0000042c
    uint32_t pad_rfdig_gpio_2;         // 0x00000430
    uint32_t pad_rfdig_gpio_1;         // 0x00000434
    uint32_t pad_rfdig_gpio_0;         // 0x00000438
    uint32_t pad_keyin_4;              // 0x0000043c
    uint32_t pad_keyout_5;             // 0x00000440
    uint32_t pad_keyin_5;              // 0x00000444
    uint32_t pad_keyout_4;             // 0x00000448
    uint32_t pad_uart_1_rts;           // 0x0000044c
    uint32_t pad_uart_1_txd;           // 0x00000450
    uint32_t pad_uart_1_rxd;           // 0x00000454
    uint32_t pad_uart_1_cts;           // 0x00000458
    uint32_t pad_gpio_0;               // 0x0000045c
    uint32_t pad_gpio_3;               // 0x00000460
    uint32_t pad_gpio_2;               // 0x00000464
    uint32_t pad_gpio_1;               // 0x00000468
    uint32_t pad_gpio_7;               // 0x0000046c
    uint32_t pad_gpio_6;               // 0x00000470
    uint32_t pad_gpio_5;               // 0x00000474
    uint32_t pad_gpio_4;               // 0x00000478
    uint32_t pad_adi_sda;              // 0x0000047c
    uint32_t pad_adi_scl;              // 0x00000480
    uint32_t pad_resetb;               // 0x00000484
    uint32_t pad_osc_32k;              // 0x00000488
    uint32_t pad_pmic_ext_int;         // 0x0000048c
    uint32_t pad_chip_pd;              // 0x00000490
    uint32_t pad_ptest;                // 0x00000494
    uint32_t pad_clk26m_pmic;          // 0x00000498
    uint32_t pad_sim_1_rst;            // 0x0000049c
    uint32_t pad_sim_1_dio;            // 0x000004a0
    uint32_t pad_sim_1_clk;            // 0x000004a4
    uint32_t pad_sim_0_rst;            // 0x000004a8
    uint32_t pad_sim_0_dio;            // 0x000004ac
    uint32_t pad_sim_0_clk;            // 0x000004b0
    uint32_t pad_sw_clk;               // 0x000004b4
    uint32_t pad_sw_dio;               // 0x000004b8
    uint32_t pad_debug_host_tx;        // 0x000004bc
    uint32_t pad_debug_host_rx;        // 0x000004c0
    uint32_t pad_debug_host_clk;       // 0x000004c4
    uint32_t pad_camera_rst_l;         // 0x000004c8
    uint32_t pad_spi_camera_sck;       // 0x000004cc
    uint32_t pad_spi_camera_si_1;      // 0x000004d0
    uint32_t pad_spi_camera_si_0;      // 0x000004d4
    uint32_t pad_camera_ref_clk;       // 0x000004d8
    uint32_t pad_camera_pwdn;          // 0x000004dc
    uint32_t pad_i2s_sdat_i;           // 0x000004e0
    uint32_t pad_i2s1_sdat_o;          // 0x000004e4
    uint32_t pad_i2s1_lrck;            // 0x000004e8
    uint32_t pad_i2s1_bck;             // 0x000004ec
    uint32_t pad_i2s1_mclk;            // 0x000004f0
    uint32_t pad_i2c_m2_scl;           // 0x000004f4
    uint32_t pad_i2c_m2_sda;           // 0x000004f8
    uint32_t pad_nand_sel;             // 0x000004fc
    uint32_t pad_keyout_3;             // 0x00000500
    uint32_t pad_keyout_2;             // 0x00000504
    uint32_t pad_keyout_1;             // 0x00000508
    uint32_t pad_keyout_0;             // 0x0000050c
    uint32_t pad_keyin_3;              // 0x00000510
    uint32_t pad_keyin_2;              // 0x00000514
    uint32_t pad_keyin_1;              // 0x00000518
    uint32_t pad_keyin_0;              // 0x0000051c
    uint32_t pad_lcd_rstb;             // 0x00000520
    uint32_t pad_lcd_fmark;            // 0x00000524
    uint32_t pad_spi_lcd_select;       // 0x00000528
    uint32_t pad_spi_lcd_cs;           // 0x0000052c
    uint32_t pad_spi_lcd_clk;          // 0x00000530
    uint32_t pad_spi_lcd_sdc;          // 0x00000534
    uint32_t pad_spi_lcd_sio;          // 0x00000538
    uint32_t pad_sdmmc1_rst;           // 0x0000053c
    uint32_t pad_sdmmc1_data_7;        // 0x00000540
    uint32_t pad_sdmmc1_data_6;        // 0x00000544
    uint32_t pad_sdmmc1_data_5;        // 0x00000548
    uint32_t pad_sdmmc1_data_4;        // 0x0000054c
    uint32_t pad_sdmmc1_data_3;        // 0x00000550
    uint32_t pad_sdmmc1_data_2;        // 0x00000554
    uint32_t pad_sdmmc1_data_1;        // 0x00000558
    uint32_t pad_sdmmc1_data_0;        // 0x0000055c
    uint32_t pad_sdmmc1_cmd;           // 0x00000560
    uint32_t pad_sdmmc1_clk;           // 0x00000564
    uint32_t pad_uart_2_rts;           // 0x00000568
    uint32_t pad_uart_2_cts;           // 0x0000056c
    uint32_t pad_uart_2_txd;           // 0x00000570
    uint32_t pad_uart_2_rxd;           // 0x00000574
    uint32_t pad_i2c_m1_sda;           // 0x00000578
    uint32_t pad_i2c_m1_scl;           // 0x0000057c
    uint32_t pad_gpio_23;              // 0x00000580
    uint32_t pad_gpio_22;              // 0x00000584
    uint32_t pad_gpio_21;              // 0x00000588
    uint32_t pad_gpio_20;              // 0x0000058c
    uint32_t pad_gpio_19;              // 0x00000590
    uint32_t pad_gpio_18;              // 0x00000594
    uint32_t pad_gpio_17;              // 0x00000598
    uint32_t pad_gpio_16;              // 0x0000059c
    uint32_t pad_m_spi_d_3;            // 0x000005a0
    uint32_t pad_m_spi_d_2;            // 0x000005a4
    uint32_t pad_m_spi_d_1;            // 0x000005a8
    uint32_t pad_m_spi_d_0;            // 0x000005ac
    uint32_t pad_m_spi_cs;             // 0x000005b0
    uint32_t pad_m_spi_clk;            // 0x000005b4
    uint32_t __1464[658];              // 0x000005b8
    uint32_t pwr_pad_ctl_reserved_set; // 0x00001000
    uint32_t pin_ctrl_reg0_set;        // 0x00001004
    uint32_t pin_ctrl_reg1_set;        // 0x00001008
    uint32_t pin_ctrl_reg2_set;        // 0x0000100c
    uint32_t pin_ctrl_reg3_set;        // 0x00001010
    uint32_t pin_ctrl_reg4_set;        // 0x00001014
    uint32_t pin_ctrl_reg5_set;        // 0x00001018
    uint32_t rfdig_gpio_7_set;         // 0x0000101c
    uint32_t rfdig_gpio_6_set;         // 0x00001020
    uint32_t rfdig_gpio_5_set;         // 0x00001024
    uint32_t rfdig_gpio_4_set;         // 0x00001028
    uint32_t rfdig_gpio_3_set;         // 0x0000102c
    uint32_t rfdig_gpio_2_set;         // 0x00001030
    uint32_t rfdig_gpio_1_set;         // 0x00001034
    uint32_t rfdig_gpio_0_set;         // 0x00001038
    uint32_t keyin_4_set;              // 0x0000103c
    uint32_t keyout_5_set;             // 0x00001040
    uint32_t keyin_5_set;              // 0x00001044
    uint32_t keyout_4_set;             // 0x00001048
    uint32_t uart_1_rts_set;           // 0x0000104c
    uint32_t uart_1_txd_set;           // 0x00001050
    uint32_t uart_1_rxd_set;           // 0x00001054
    uint32_t uart_1_cts_set;           // 0x00001058
    uint32_t gpio_0_set;               // 0x0000105c
    uint32_t gpio_3_set;               // 0x00001060
    uint32_t gpio_2_set;               // 0x00001064
    uint32_t gpio_1_set;               // 0x00001068
    uint32_t gpio_7_set;               // 0x0000106c
    uint32_t gpio_6_set;               // 0x00001070
    uint32_t gpio_5_set;               // 0x00001074
    uint32_t gpio_4_set;               // 0x00001078
    uint32_t adi_sda_set;              // 0x0000107c
    uint32_t adi_scl_set;              // 0x00001080
    uint32_t resetb_set;               // 0x00001084
    uint32_t osc_32k_set;              // 0x00001088
    uint32_t pmic_ext_int_set;         // 0x0000108c
    uint32_t chip_pd_set;              // 0x00001090
    uint32_t __4244[1];                // 0x00001094
    uint32_t clk26m_pmic_set;          // 0x00001098
    uint32_t sim_1_rst_set;            // 0x0000109c
    uint32_t sim_1_dio_set;            // 0x000010a0
    uint32_t sim_1_clk_set;            // 0x000010a4
    uint32_t sim_0_rst_set;            // 0x000010a8
    uint32_t sim_0_dio_set;            // 0x000010ac
    uint32_t sim_0_clk_set;            // 0x000010b0
    uint32_t sw_clk_set;               // 0x000010b4
    uint32_t sw_dio_set;               // 0x000010b8
    uint32_t debug_host_tx_set;        // 0x000010bc
    uint32_t debug_host_rx_set;        // 0x000010c0
    uint32_t debug_host_clk_set;       // 0x000010c4
    uint32_t camera_rst_l_set;         // 0x000010c8
    uint32_t spi_camera_sck_set;       // 0x000010cc
    uint32_t spi_camera_si_1_set;      // 0x000010d0
    uint32_t spi_camera_si_0_set;      // 0x000010d4
    uint32_t camera_ref_clk_set;       // 0x000010d8
    uint32_t camera_pwdn_set;          // 0x000010dc
    uint32_t i2s_sdat_i_set;           // 0x000010e0
    uint32_t i2s1_sdat_o_set;          // 0x000010e4
    uint32_t i2s1_lrck_set;            // 0x000010e8
    uint32_t i2s1_bck_set;             // 0x000010ec
    uint32_t i2s1_mclk_set;            // 0x000010f0
    uint32_t i2c_m2_scl_set;           // 0x000010f4
    uint32_t i2c_m2_sda_set;           // 0x000010f8
    uint32_t nand_sel_set;             // 0x000010fc
    uint32_t keyout_3_set;             // 0x00001100
    uint32_t keyout_2_set;             // 0x00001104
    uint32_t keyout_1_set;             // 0x00001108
    uint32_t keyout_0_set;             // 0x0000110c
    uint32_t keyin_3_set;              // 0x00001110
    uint32_t keyin_2_set;              // 0x00001114
    uint32_t keyin_1_set;              // 0x00001118
    uint32_t keyin_0_set;              // 0x0000111c
    uint32_t lcd_rstb_set;             // 0x00001120
    uint32_t lcd_fmark_set;            // 0x00001124
    uint32_t spi_lcd_select_set;       // 0x00001128
    uint32_t spi_lcd_cs_set;           // 0x0000112c
    uint32_t spi_lcd_clk_set;          // 0x00001130
    uint32_t spi_lcd_sdc_set;          // 0x00001134
    uint32_t spi_lcd_sio_set;          // 0x00001138
    uint32_t sdmmc1_rst_set;           // 0x0000113c
    uint32_t sdmmc1_data_7_set;        // 0x00001140
    uint32_t sdmmc1_data_6_set;        // 0x00001144
    uint32_t sdmmc1_data_5_set;        // 0x00001148
    uint32_t sdmmc1_data_4_set;        // 0x0000114c
    uint32_t sdmmc1_data_3_set;        // 0x00001150
    uint32_t sdmmc1_data_2_set;        // 0x00001154
    uint32_t sdmmc1_data_1_set;        // 0x00001158
    uint32_t sdmmc1_data_0_set;        // 0x0000115c
    uint32_t sdmmc1_cmd_set;           // 0x00001160
    uint32_t sdmmc1_clk_set;           // 0x00001164
    uint32_t uart_2_rts_set;           // 0x00001168
    uint32_t uart_2_cts_set;           // 0x0000116c
    uint32_t uart_2_txd_set;           // 0x00001170
    uint32_t uart_2_rxd_set;           // 0x00001174
    uint32_t i2c_m1_sda_set;           // 0x00001178
    uint32_t i2c_m1_scl_set;           // 0x0000117c
    uint32_t gpio_23_set;              // 0x00001180
    uint32_t gpio_22_set;              // 0x00001184
    uint32_t gpio_21_set;              // 0x00001188
    uint32_t gpio_20_set;              // 0x0000118c
    uint32_t gpio_19_set;              // 0x00001190
    uint32_t gpio_18_set;              // 0x00001194
    uint32_t gpio_17_set;              // 0x00001198
    uint32_t gpio_16_set;              // 0x0000119c
    uint32_t m_spi_d_3_set;            // 0x000011a0
    uint32_t m_spi_d_2_set;            // 0x000011a4
    uint32_t m_spi_d_1_set;            // 0x000011a8
    uint32_t m_spi_d_0_set;            // 0x000011ac
    uint32_t m_spi_cs_set;             // 0x000011b0
    uint32_t m_spi_clk_set;            // 0x000011b4
    uint32_t __4536[153];              // 0x000011b8
    uint32_t pad_rfdig_gpio_7_set;     // 0x0000141c
    uint32_t pad_rfdig_gpio_6_set;     // 0x00001420
    uint32_t pad_rfdig_gpio_5_set;     // 0x00001424
    uint32_t pad_rfdig_gpio_4_set;     // 0x00001428
    uint32_t pad_rfdig_gpio_3_set;     // 0x0000142c
    uint32_t pad_rfdig_gpio_2_set;     // 0x00001430
    uint32_t pad_rfdig_gpio_1_set;     // 0x00001434
    uint32_t pad_rfdig_gpio_0_set;     // 0x00001438
    uint32_t pad_keyin_4_set;          // 0x0000143c
    uint32_t pad_keyout_5_set;         // 0x00001440
    uint32_t pad_keyin_5_set;          // 0x00001444
    uint32_t pad_keyout_4_set;         // 0x00001448
    uint32_t pad_uart_1_rts_set;       // 0x0000144c
    uint32_t pad_uart_1_txd_set;       // 0x00001450
    uint32_t pad_uart_1_rxd_set;       // 0x00001454
    uint32_t pad_uart_1_cts_set;       // 0x00001458
    uint32_t pad_gpio_0_set;           // 0x0000145c
    uint32_t pad_gpio_3_set;           // 0x00001460
    uint32_t pad_gpio_2_set;           // 0x00001464
    uint32_t pad_gpio_1_set;           // 0x00001468
    uint32_t pad_gpio_7_set;           // 0x0000146c
    uint32_t pad_gpio_6_set;           // 0x00001470
    uint32_t pad_gpio_5_set;           // 0x00001474
    uint32_t pad_gpio_4_set;           // 0x00001478
    uint32_t pad_adi_sda_set;          // 0x0000147c
    uint32_t pad_adi_scl_set;          // 0x00001480
    uint32_t pad_resetb_set;           // 0x00001484
    uint32_t pad_osc_32k_set;          // 0x00001488
    uint32_t pad_pmic_ext_int_set;     // 0x0000148c
    uint32_t pad_chip_pd_set;          // 0x00001490
    uint32_t __5268[1];                // 0x00001494
    uint32_t pad_clk26m_pmic_set;      // 0x00001498
    uint32_t pad_sim_1_rst_set;        // 0x0000149c
    uint32_t pad_sim_1_dio_set;        // 0x000014a0
    uint32_t pad_sim_1_clk_set;        // 0x000014a4
    uint32_t pad_sim_0_rst_set;        // 0x000014a8
    uint32_t pad_sim_0_dio_set;        // 0x000014ac
    uint32_t pad_sim_0_clk_set;        // 0x000014b0
    uint32_t pad_sw_clk_set;           // 0x000014b4
    uint32_t pad_sw_dio_set;           // 0x000014b8
    uint32_t pad_debug_host_tx_set;    // 0x000014bc
    uint32_t pad_debug_host_rx_set;    // 0x000014c0
    uint32_t pad_debug_host_clk_set;   // 0x000014c4
    uint32_t pad_camera_rst_l_set;     // 0x000014c8
    uint32_t pad_spi_camera_sck_set;   // 0x000014cc
    uint32_t pad_spi_camera_si_1_set;  // 0x000014d0
    uint32_t pad_spi_camera_si_0_set;  // 0x000014d4
    uint32_t pad_camera_ref_clk_set;   // 0x000014d8
    uint32_t pad_camera_pwdn_set;      // 0x000014dc
    uint32_t pad_i2s_sdat_i_set;       // 0x000014e0
    uint32_t pad_i2s1_sdat_o_set;      // 0x000014e4
    uint32_t pad_i2s1_lrck_set;        // 0x000014e8
    uint32_t pad_i2s1_bck_set;         // 0x000014ec
    uint32_t pad_i2s1_mclk_set;        // 0x000014f0
    uint32_t pad_i2c_m2_scl_set;       // 0x000014f4
    uint32_t pad_i2c_m2_sda_set;       // 0x000014f8
    uint32_t pad_nand_sel_set;         // 0x000014fc
    uint32_t pad_keyout_3_set;         // 0x00001500
    uint32_t pad_keyout_2_set;         // 0x00001504
    uint32_t pad_keyout_1_set;         // 0x00001508
    uint32_t pad_keyout_0_set;         // 0x0000150c
    uint32_t pad_keyin_3_set;          // 0x00001510
    uint32_t pad_keyin_2_set;          // 0x00001514
    uint32_t pad_keyin_1_set;          // 0x00001518
    uint32_t pad_keyin_0_set;          // 0x0000151c
    uint32_t pad_lcd_rstb_set;         // 0x00001520
    uint32_t pad_lcd_fmark_set;        // 0x00001524
    uint32_t pad_spi_lcd_select_set;   // 0x00001528
    uint32_t pad_spi_lcd_cs_set;       // 0x0000152c
    uint32_t pad_spi_lcd_clk_set;      // 0x00001530
    uint32_t pad_spi_lcd_sdc_set;      // 0x00001534
    uint32_t pad_spi_lcd_sio_set;      // 0x00001538
    uint32_t pad_sdmmc1_rst_set;       // 0x0000153c
    uint32_t pad_sdmmc1_data_7_set;    // 0x00001540
    uint32_t pad_sdmmc1_data_6_set;    // 0x00001544
    uint32_t pad_sdmmc1_data_5_set;    // 0x00001548
    uint32_t pad_sdmmc1_data_4_set;    // 0x0000154c
    uint32_t pad_sdmmc1_data_3_set;    // 0x00001550
    uint32_t pad_sdmmc1_data_2_set;    // 0x00001554
    uint32_t pad_sdmmc1_data_1_set;    // 0x00001558
    uint32_t pad_sdmmc1_data_0_set;    // 0x0000155c
    uint32_t pad_sdmmc1_cmd_set;       // 0x00001560
    uint32_t pad_sdmmc1_clk_set;       // 0x00001564
    uint32_t pad_uart_2_rts_set;       // 0x00001568
    uint32_t pad_uart_2_cts_set;       // 0x0000156c
    uint32_t pad_uart_2_txd_set;       // 0x00001570
    uint32_t pad_uart_2_rxd_set;       // 0x00001574
    uint32_t pad_i2c_m1_sda_set;       // 0x00001578
    uint32_t pad_i2c_m1_scl_set;       // 0x0000157c
    uint32_t pad_gpio_23_set;          // 0x00001580
    uint32_t pad_gpio_22_set;          // 0x00001584
    uint32_t pad_gpio_21_set;          // 0x00001588
    uint32_t pad_gpio_20_set;          // 0x0000158c
    uint32_t pad_gpio_19_set;          // 0x00001590
    uint32_t pad_gpio_18_set;          // 0x00001594
    uint32_t pad_gpio_17_set;          // 0x00001598
    uint32_t pad_gpio_16_set;          // 0x0000159c
    uint32_t pad_m_spi_d_3_set;        // 0x000015a0
    uint32_t pad_m_spi_d_2_set;        // 0x000015a4
    uint32_t pad_m_spi_d_1_set;        // 0x000015a8
    uint32_t pad_m_spi_d_0_set;        // 0x000015ac
    uint32_t pad_m_spi_cs_set;         // 0x000015b0
    uint32_t pad_m_spi_clk_set;        // 0x000015b4
    uint32_t __5560[658];              // 0x000015b8
    uint32_t pwr_pad_ctl_reserved_clr; // 0x00002000
    uint32_t pin_ctrl_reg0_clr;        // 0x00002004
    uint32_t pin_ctrl_reg1_clr;        // 0x00002008
    uint32_t pin_ctrl_reg2_clr;        // 0x0000200c
    uint32_t pin_ctrl_reg3_clr;        // 0x00002010
    uint32_t pin_ctrl_reg4_clr;        // 0x00002014
    uint32_t pin_ctrl_reg5_clr;        // 0x00002018
    uint32_t rfdig_gpio_7_clr;         // 0x0000201c
    uint32_t rfdig_gpio_6_clr;         // 0x00002020
    uint32_t rfdig_gpio_5_clr;         // 0x00002024
    uint32_t rfdig_gpio_4_clr;         // 0x00002028
    uint32_t rfdig_gpio_3_clr;         // 0x0000202c
    uint32_t rfdig_gpio_2_clr;         // 0x00002030
    uint32_t rfdig_gpio_1_clr;         // 0x00002034
    uint32_t rfdig_gpio_0_clr;         // 0x00002038
    uint32_t keyin_4_clr;              // 0x0000203c
    uint32_t keyout_5_clr;             // 0x00002040
    uint32_t keyin_5_clr;              // 0x00002044
    uint32_t keyout_4_clr;             // 0x00002048
    uint32_t uart_1_rts_clr;           // 0x0000204c
    uint32_t uart_1_txd_clr;           // 0x00002050
    uint32_t uart_1_rxd_clr;           // 0x00002054
    uint32_t uart_1_cts_clr;           // 0x00002058
    uint32_t gpio_0_clr;               // 0x0000205c
    uint32_t gpio_3_clr;               // 0x00002060
    uint32_t gpio_2_clr;               // 0x00002064
    uint32_t gpio_1_clr;               // 0x00002068
    uint32_t gpio_7_clr;               // 0x0000206c
    uint32_t gpio_6_clr;               // 0x00002070
    uint32_t gpio_5_clr;               // 0x00002074
    uint32_t gpio_4_clr;               // 0x00002078
    uint32_t adi_sda_clr;              // 0x0000207c
    uint32_t adi_scl_clr;              // 0x00002080
    uint32_t resetb_clr;               // 0x00002084
    uint32_t osc_32k_clr;              // 0x00002088
    uint32_t pmic_ext_int_clr;         // 0x0000208c
    uint32_t chip_pd_clr;              // 0x00002090
    uint32_t __8340[1];                // 0x00002094
    uint32_t clk26m_pmic_clr;          // 0x00002098
    uint32_t sim_1_rst_clr;            // 0x0000209c
    uint32_t sim_1_dio_clr;            // 0x000020a0
    uint32_t sim_1_clk_clr;            // 0x000020a4
    uint32_t sim_0_rst_clr;            // 0x000020a8
    uint32_t sim_0_dio_clr;            // 0x000020ac
    uint32_t sim_0_clk_clr;            // 0x000020b0
    uint32_t sw_clk_clr;               // 0x000020b4
    uint32_t sw_dio_clr;               // 0x000020b8
    uint32_t debug_host_tx_clr;        // 0x000020bc
    uint32_t debug_host_rx_clr;        // 0x000020c0
    uint32_t debug_host_clk_clr;       // 0x000020c4
    uint32_t camera_rst_l_clr;         // 0x000020c8
    uint32_t spi_camera_sck_clr;       // 0x000020cc
    uint32_t spi_camera_si_1_clr;      // 0x000020d0
    uint32_t spi_camera_si_0_clr;      // 0x000020d4
    uint32_t camera_ref_clk_clr;       // 0x000020d8
    uint32_t camera_pwdn_clr;          // 0x000020dc
    uint32_t i2s_sdat_i_clr;           // 0x000020e0
    uint32_t i2s1_sdat_o_clr;          // 0x000020e4
    uint32_t i2s1_lrck_clr;            // 0x000020e8
    uint32_t i2s1_bck_clr;             // 0x000020ec
    uint32_t i2s1_mclk_clr;            // 0x000020f0
    uint32_t i2c_m2_scl_clr;           // 0x000020f4
    uint32_t i2c_m2_sda_clr;           // 0x000020f8
    uint32_t nand_sel_clr;             // 0x000020fc
    uint32_t keyout_3_clr;             // 0x00002100
    uint32_t keyout_2_clr;             // 0x00002104
    uint32_t keyout_1_clr;             // 0x00002108
    uint32_t keyout_0_clr;             // 0x0000210c
    uint32_t keyin_3_clr;              // 0x00002110
    uint32_t keyin_2_clr;              // 0x00002114
    uint32_t keyin_1_clr;              // 0x00002118
    uint32_t keyin_0_clr;              // 0x0000211c
    uint32_t lcd_rstb_clr;             // 0x00002120
    uint32_t lcd_fmark_clr;            // 0x00002124
    uint32_t spi_lcd_select_clr;       // 0x00002128
    uint32_t spi_lcd_cs_clr;           // 0x0000212c
    uint32_t spi_lcd_clk_clr;          // 0x00002130
    uint32_t spi_lcd_sdc_clr;          // 0x00002134
    uint32_t spi_lcd_sio_clr;          // 0x00002138
    uint32_t sdmmc1_rst_clr;           // 0x0000213c
    uint32_t sdmmc1_data_7_clr;        // 0x00002140
    uint32_t sdmmc1_data_6_clr;        // 0x00002144
    uint32_t sdmmc1_data_5_clr;        // 0x00002148
    uint32_t sdmmc1_data_4_clr;        // 0x0000214c
    uint32_t sdmmc1_data_3_clr;        // 0x00002150
    uint32_t sdmmc1_data_2_clr;        // 0x00002154
    uint32_t sdmmc1_data_1_clr;        // 0x00002158
    uint32_t sdmmc1_data_0_clr;        // 0x0000215c
    uint32_t sdmmc1_cmd_clr;           // 0x00002160
    uint32_t sdmmc1_clk_clr;           // 0x00002164
    uint32_t uart_2_rts_clr;           // 0x00002168
    uint32_t uart_2_cts_clr;           // 0x0000216c
    uint32_t uart_2_txd_clr;           // 0x00002170
    uint32_t uart_2_rxd_clr;           // 0x00002174
    uint32_t i2c_m1_sda_clr;           // 0x00002178
    uint32_t i2c_m1_scl_clr;           // 0x0000217c
    uint32_t gpio_23_clr;              // 0x00002180
    uint32_t gpio_22_clr;              // 0x00002184
    uint32_t gpio_21_clr;              // 0x00002188
    uint32_t gpio_20_clr;              // 0x0000218c
    uint32_t gpio_19_clr;              // 0x00002190
    uint32_t gpio_18_clr;              // 0x00002194
    uint32_t gpio_17_clr;              // 0x00002198
    uint32_t gpio_16_clr;              // 0x0000219c
    uint32_t m_spi_d_3_clr;            // 0x000021a0
    uint32_t m_spi_d_2_clr;            // 0x000021a4
    uint32_t m_spi_d_1_clr;            // 0x000021a8
    uint32_t m_spi_d_0_clr;            // 0x000021ac
    uint32_t m_spi_cs_clr;             // 0x000021b0
    uint32_t m_spi_clk_clr;            // 0x000021b4
    uint32_t __8632[153];              // 0x000021b8
    uint32_t pad_rfdig_gpio_7_clr;     // 0x0000241c
    uint32_t pad_rfdig_gpio_6_clr;     // 0x00002420
    uint32_t pad_rfdig_gpio_5_clr;     // 0x00002424
    uint32_t pad_rfdig_gpio_4_clr;     // 0x00002428
    uint32_t pad_rfdig_gpio_3_clr;     // 0x0000242c
    uint32_t pad_rfdig_gpio_2_clr;     // 0x00002430
    uint32_t pad_rfdig_gpio_1_clr;     // 0x00002434
    uint32_t pad_rfdig_gpio_0_clr;     // 0x00002438
    uint32_t pad_keyin_4_clr;          // 0x0000243c
    uint32_t pad_keyout_5_clr;         // 0x00002440
    uint32_t pad_keyin_5_clr;          // 0x00002444
    uint32_t pad_keyout_4_clr;         // 0x00002448
    uint32_t pad_uart_1_rts_clr;       // 0x0000244c
    uint32_t pad_uart_1_txd_clr;       // 0x00002450
    uint32_t pad_uart_1_rxd_clr;       // 0x00002454
    uint32_t pad_uart_1_cts_clr;       // 0x00002458
    uint32_t pad_gpio_0_clr;           // 0x0000245c
    uint32_t pad_gpio_3_clr;           // 0x00002460
    uint32_t pad_gpio_2_clr;           // 0x00002464
    uint32_t pad_gpio_1_clr;           // 0x00002468
    uint32_t pad_gpio_7_clr;           // 0x0000246c
    uint32_t pad_gpio_6_clr;           // 0x00002470
    uint32_t pad_gpio_5_clr;           // 0x00002474
    uint32_t pad_gpio_4_clr;           // 0x00002478
    uint32_t pad_adi_sda_clr;          // 0x0000247c
    uint32_t pad_adi_scl_clr;          // 0x00002480
    uint32_t pad_resetb_clr;           // 0x00002484
    uint32_t pad_osc_32k_clr;          // 0x00002488
    uint32_t pad_pmic_ext_int_clr;     // 0x0000248c
    uint32_t pad_chip_pd_clr;          // 0x00002490
    uint32_t __9364[1];                // 0x00002494
    uint32_t pad_clk26m_pmic_clr;      // 0x00002498
    uint32_t pad_sim_1_rst_clr;        // 0x0000249c
    uint32_t pad_sim_1_dio_clr;        // 0x000024a0
    uint32_t pad_sim_1_clk_clr;        // 0x000024a4
    uint32_t pad_sim_0_rst_clr;        // 0x000024a8
    uint32_t pad_sim_0_dio_clr;        // 0x000024ac
    uint32_t pad_sim_0_clk_clr;        // 0x000024b0
    uint32_t pad_sw_clk_clr;           // 0x000024b4
    uint32_t pad_sw_dio_clr;           // 0x000024b8
    uint32_t pad_debug_host_tx_clr;    // 0x000024bc
    uint32_t pad_debug_host_rx_clr;    // 0x000024c0
    uint32_t pad_debug_host_clk_clr;   // 0x000024c4
    uint32_t pad_camera_rst_l_clr;     // 0x000024c8
    uint32_t pad_spi_camera_sck_clr;   // 0x000024cc
    uint32_t pad_spi_camera_si_1_clr;  // 0x000024d0
    uint32_t pad_spi_camera_si_0_clr;  // 0x000024d4
    uint32_t pad_camera_ref_clk_clr;   // 0x000024d8
    uint32_t pad_camera_pwdn_clr;      // 0x000024dc
    uint32_t pad_i2s_sdat_i_clr;       // 0x000024e0
    uint32_t pad_i2s1_sdat_o_clr;      // 0x000024e4
    uint32_t pad_i2s1_lrck_clr;        // 0x000024e8
    uint32_t pad_i2s1_bck_clr;         // 0x000024ec
    uint32_t pad_i2s1_mclk_clr;        // 0x000024f0
    uint32_t pad_i2c_m2_scl_clr;       // 0x000024f4
    uint32_t pad_i2c_m2_sda_clr;       // 0x000024f8
    uint32_t pad_nand_sel_clr;         // 0x000024fc
    uint32_t pad_keyout_3_clr;         // 0x00002500
    uint32_t pad_keyout_2_clr;         // 0x00002504
    uint32_t pad_keyout_1_clr;         // 0x00002508
    uint32_t pad_keyout_0_clr;         // 0x0000250c
    uint32_t pad_keyin_3_clr;          // 0x00002510
    uint32_t pad_keyin_2_clr;          // 0x00002514
    uint32_t pad_keyin_1_clr;          // 0x00002518
    uint32_t pad_keyin_0_clr;          // 0x0000251c
    uint32_t pad_lcd_rstb_clr;         // 0x00002520
    uint32_t pad_lcd_fmark_clr;        // 0x00002524
    uint32_t pad_spi_lcd_select_clr;   // 0x00002528
    uint32_t pad_spi_lcd_cs_clr;       // 0x0000252c
    uint32_t pad_spi_lcd_clk_clr;      // 0x00002530
    uint32_t pad_spi_lcd_sdc_clr;      // 0x00002534
    uint32_t pad_spi_lcd_sio_clr;      // 0x00002538
    uint32_t pad_sdmmc1_rst_clr;       // 0x0000253c
    uint32_t pad_sdmmc1_data_7_clr;    // 0x00002540
    uint32_t pad_sdmmc1_data_6_clr;    // 0x00002544
    uint32_t pad_sdmmc1_data_5_clr;    // 0x00002548
    uint32_t pad_sdmmc1_data_4_clr;    // 0x0000254c
    uint32_t pad_sdmmc1_data_3_clr;    // 0x00002550
    uint32_t pad_sdmmc1_data_2_clr;    // 0x00002554
    uint32_t pad_sdmmc1_data_1_clr;    // 0x00002558
    uint32_t pad_sdmmc1_data_0_clr;    // 0x0000255c
    uint32_t pad_sdmmc1_cmd_clr;       // 0x00002560
    uint32_t pad_sdmmc1_clk_clr;       // 0x00002564
    uint32_t pad_uart_2_rts_clr;       // 0x00002568
    uint32_t pad_uart_2_cts_clr;       // 0x0000256c
    uint32_t pad_uart_2_txd_clr;       // 0x00002570
    uint32_t pad_uart_2_rxd_clr;       // 0x00002574
    uint32_t pad_i2c_m1_sda_clr;       // 0x00002578
    uint32_t pad_i2c_m1_scl_clr;       // 0x0000257c
    uint32_t pad_gpio_23_clr;          // 0x00002580
    uint32_t pad_gpio_22_clr;          // 0x00002584
    uint32_t pad_gpio_21_clr;          // 0x00002588
    uint32_t pad_gpio_20_clr;          // 0x0000258c
    uint32_t pad_gpio_19_clr;          // 0x00002590
    uint32_t pad_gpio_18_clr;          // 0x00002594
    uint32_t pad_gpio_17_clr;          // 0x00002598
    uint32_t pad_gpio_16_clr;          // 0x0000259c
    uint32_t pad_m_spi_d_3_clr;        // 0x000025a0
    uint32_t pad_m_spi_d_2_clr;        // 0x000025a4
    uint32_t pad_m_spi_d_1_clr;        // 0x000025a8
    uint32_t pad_m_spi_d_0_clr;        // 0x000025ac
    uint32_t pad_m_spi_cs_clr;         // 0x000025b0
    uint32_t pad_m_spi_clk_clr;        // 0x000025b4
} HWP_IOMUX_T_6121E;

#define hwp_iomux ((HWP_IOMUX_T_6121E *)REG_ACCESS_ADDRESS(REG_IOMUX_BASE))
#define hwp_iomux_6121e ((HWP_IOMUX_T_6121E *)REG_ACCESS_ADDRESS(REG_IOMUX_BASE))

// pwr_pad_ctl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t pwrreg_msen_lpvddio_18_33 : 1;   // [0]
        uint32_t pwrreg_msout_lpvddio_18_33 : 1;  // [1], read only
        uint32_t pwrreg_ms_lpvddio_18_33 : 1;     // [2]
        uint32_t pwrreg_msen_vlpvddio1833_1 : 1;  // [3]
        uint32_t pwrreg_msout_vlpvddio1833_1 : 1; // [4], read only
        uint32_t pwrreg_ms_vlpvddio1833_1 : 1;    // [5]
        uint32_t pwrreg_msen_vsim1 : 1;           // [6]
        uint32_t pwrreg_msout_vsim1 : 1;          // [7], read only
        uint32_t pwrreg_ms_vsim1 : 1;             // [8]
        uint32_t pwrreg_msen_vsim0 : 1;           // [9]
        uint32_t pwrreg_msout_vsim0 : 1;          // [10], read only
        uint32_t pwrreg_ms_vsim0 : 1;             // [11]
        uint32_t pwrreg_msen_vddio_18_33 : 1;     // [12]
        uint32_t pwrreg_msout_vddio_18_33 : 1;    // [13], read only
        uint32_t pwrreg_ms_vddio_18_33 : 1;       // [14]
        uint32_t pwrreg_msen_v_lcd_18_33 : 1;     // [15]
        uint32_t pwrreg_msout_v_lcd_18_33 : 1;    // [16], read only
        uint32_t pwrreg_ms_v_lcd_18_33 : 1;       // [17]
        uint32_t pwrreg_msen_v_mmc_18_30 : 1;     // [18]
        uint32_t pwrreg_msout_v_mmc_18_30 : 1;    // [19], read only
        uint32_t pwrreg_ms_v_mmc_18_30 : 1;       // [20]
        uint32_t __31_21 : 11;                    // [31:21]
    } b;
} REG_IOMUX_PWR_PAD_CTL_T;

// rfdig_gpio_7
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_RFDIG_GPIO_7_T;

// rfdig_gpio_6
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_RFDIG_GPIO_6_T;

// rfdig_gpio_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_RFDIG_GPIO_5_T;

// rfdig_gpio_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_RFDIG_GPIO_4_T;

// rfdig_gpio_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_RFDIG_GPIO_3_T;

// rfdig_gpio_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_RFDIG_GPIO_2_T;

// rfdig_gpio_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_RFDIG_GPIO_1_T;

// rfdig_gpio_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_RFDIG_GPIO_0_T;

// keyin_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYIN_4_T;

// keyout_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYOUT_5_T;

// keyin_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYIN_5_T;

// keyout_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYOUT_4_T;

// uart_1_rts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_UART_1_RTS_T;

// uart_1_txd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_UART_1_TXD_T;

// uart_1_rxd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_UART_1_RXD_T;

// uart_1_cts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_UART_1_CTS_T;

// gpio_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_0_T;

// gpio_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_3_T;

// gpio_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_2_T;

// gpio_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_1_T;

// gpio_7
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_7_T;

// gpio_6
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_6_T;

// gpio_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_5_T;

// gpio_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_4_T;

// adi_sda
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_ADI_SDA_T;

// adi_scl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_ADI_SCL_T;

// resetb
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_RESETB_T;

// osc_32k
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_OSC_32K_T;

// pmic_ext_int
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_PMIC_EXT_INT_T;

// chip_pd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_CHIP_PD_T;

// clk26m_pmic
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_CLK26M_PMIC_T;

// sim_1_rst
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SIM_1_RST_T;

// sim_1_dio
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SIM_1_DIO_T;

// sim_1_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SIM_1_CLK_T;

// sim_0_rst
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SIM_0_RST_T;

// sim_0_dio
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SIM_0_DIO_T;

// sim_0_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SIM_0_CLK_T;

// sw_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SW_CLK_T;

// sw_dio
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SW_DIO_T;

// debug_host_tx
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_DEBUG_HOST_TX_T;

// debug_host_rx
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_DEBUG_HOST_RX_T;

// debug_host_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_DEBUG_HOST_CLK_T;

// camera_rst_l
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_CAMERA_RST_L_T;

// spi_camera_sck
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SPI_CAMERA_SCK_T;

// spi_camera_si_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SPI_CAMERA_SI_1_T;

// spi_camera_si_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SPI_CAMERA_SI_0_T;

// camera_ref_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_CAMERA_REF_CLK_T;

// camera_pwdn
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_CAMERA_PWDN_T;

// i2s_sdat_i
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_I2S_SDAT_I_T;

// i2s1_sdat_o
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_I2S1_SDAT_O_T;

// i2s1_lrck
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_I2S1_LRCK_T;

// i2s1_bck
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_I2S1_BCK_T;

// i2s1_mclk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_I2S1_MCLK_T;

// i2c_m2_scl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_I2C_M2_SCL_T;

// i2c_m2_sda
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_I2C_M2_SDA_T;

// nand_sel
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_NAND_SEL_T;

// keyout_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYOUT_3_T;

// keyout_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYOUT_2_T;

// keyout_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYOUT_1_T;

// keyout_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYOUT_0_T;

// keyin_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYIN_3_T;

// keyin_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYIN_2_T;

// keyin_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYIN_1_T;

// keyin_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_KEYIN_0_T;

// lcd_rstb
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_LCD_RSTB_T;

// lcd_fmark
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_LCD_FMARK_T;

// spi_lcd_select
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SPI_LCD_SELECT_T;

// spi_lcd_cs
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SPI_LCD_CS_T;

// spi_lcd_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SPI_LCD_CLK_T;

// spi_lcd_sdc
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SPI_LCD_SDC_T;

// spi_lcd_sio
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SPI_LCD_SIO_T;

// sdmmc1_rst
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_RST_T;

// sdmmc1_data_7
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_DATA_7_T;

// sdmmc1_data_6
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_DATA_6_T;

// sdmmc1_data_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_DATA_5_T;

// sdmmc1_data_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_DATA_4_T;

// sdmmc1_data_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_DATA_3_T;

// sdmmc1_data_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_DATA_2_T;

// sdmmc1_data_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_DATA_1_T;

// sdmmc1_data_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_DATA_0_T;

// sdmmc1_cmd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_CMD_T;

// sdmmc1_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_SDMMC1_CLK_T;

// uart_2_rts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_UART_2_RTS_T;

// uart_2_cts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_UART_2_CTS_T;

// uart_2_txd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_UART_2_TXD_T;

// uart_2_rxd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_UART_2_RXD_T;

// i2c_m1_sda
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_I2C_M1_SDA_T;

// i2c_m1_scl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_I2C_M1_SCL_T;

// gpio_23
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_23_T;

// gpio_22
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_22_T;

// gpio_21
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_21_T;

// gpio_20
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_20_T;

// gpio_19
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_19_T;

// gpio_18
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_18_T;

// gpio_17
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_17_T;

// gpio_16
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_GPIO_16_T;

// m_spi_d_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_M_SPI_D_3_T;

// m_spi_d_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_M_SPI_D_2_T;

// m_spi_d_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_M_SPI_D_1_T;

// m_spi_d_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_M_SPI_D_0_T;

// m_spi_cs
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_M_SPI_CS_T;

// m_spi_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_M_SPI_CLK_T;

// pad_rfdig_gpio_7
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_7_T;

// pad_rfdig_gpio_6
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_6_T;

// pad_rfdig_gpio_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_5_T;

// pad_rfdig_gpio_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_4_T;

// pad_rfdig_gpio_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_3_T;

// pad_rfdig_gpio_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_2_T;

// pad_rfdig_gpio_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_1_T;

// pad_rfdig_gpio_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_RFDIG_GPIO_0_T;

// pad_keyin_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYIN_4_T;

// pad_keyout_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYOUT_5_T;

// pad_keyin_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYIN_5_T;

// pad_keyout_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYOUT_4_T;

// pad_uart_1_rts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_UART_1_RTS_T;

// pad_uart_1_txd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_UART_1_TXD_T;

// pad_uart_1_rxd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_UART_1_RXD_T;

// pad_uart_1_cts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_UART_1_CTS_T;

// pad_gpio_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_GPIO_0_T;

// pad_gpio_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_GPIO_3_T;

// pad_gpio_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_GPIO_2_T;

// pad_gpio_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_GPIO_1_T;

// pad_gpio_7
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_GPIO_7_T;

// pad_gpio_6
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_GPIO_6_T;

// pad_gpio_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_GPIO_5_T;

// pad_gpio_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_GPIO_4_T;

// pad_adi_sda
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_ADI_SDA_T;

// pad_adi_scl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_ADI_SCL_T;

// pad_resetb
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __5_0 : 6;    // [5:0]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t __18_13 : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_RESETB_T;

// pad_osc_32k
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_OSC_32K_T;

// pad_pmic_ext_int
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_PMIC_EXT_INT_T;

// pad_chip_pd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_CHIP_PD_T;

// pad_clk26m_pmic
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_CLK26M_PMIC_T;

// pad_sim_1_rst
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SIM_1_RST_T;

// pad_sim_1_dio
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SIM_1_DIO_T;

// pad_sim_1_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SIM_1_CLK_T;

// pad_sim_0_rst
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SIM_0_RST_T;

// pad_sim_0_dio
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SIM_0_DIO_T;

// pad_sim_0_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SIM_0_CLK_T;

// pad_sw_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_SW_CLK_T;

// pad_sw_dio
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_SW_DIO_T;

// pad_debug_host_tx
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_DEBUG_HOST_TX_T;

// pad_debug_host_rx
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;      // [0]
        uint32_t slp_ie : 1;      // [1]
        uint32_t slp_wpdo : 1;    // [2]
        uint32_t slp_wpu : 1;     // [3]
        uint32_t __5_4 : 2;       // [5:4]
        uint32_t wpdo : 1;        // [6]
        uint32_t wpu : 1;         // [7]
        uint32_t __9_8 : 2;       // [9:8]
        uint32_t padi_switch : 1; // [10]
        uint32_t se : 1;          // [11]
        uint32_t wpus : 1;        // [12]
        uint32_t dslp_en : 6;     // [18:13]
        uint32_t drv : 2;         // [20:19]
        uint32_t __31_21 : 11;    // [31:21]
    } b;
} REG_IOMUX_PAD_DEBUG_HOST_RX_T;

// pad_debug_host_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;      // [0]
        uint32_t slp_ie : 1;      // [1]
        uint32_t slp_wpdo : 1;    // [2]
        uint32_t slp_wpu : 1;     // [3]
        uint32_t __5_4 : 2;       // [5:4]
        uint32_t wpdo : 1;        // [6]
        uint32_t wpu : 1;         // [7]
        uint32_t __9_8 : 2;       // [9:8]
        uint32_t padi_switch : 1; // [10]
        uint32_t se : 1;          // [11]
        uint32_t wpus : 1;        // [12]
        uint32_t dslp_en : 6;     // [18:13]
        uint32_t drv : 2;         // [20:19]
        uint32_t __31_21 : 11;    // [31:21]
    } b;
} REG_IOMUX_PAD_DEBUG_HOST_CLK_T;

// pad_camera_rst_l
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;      // [0]
        uint32_t slp_ie : 1;      // [1]
        uint32_t slp_wpdo : 1;    // [2]
        uint32_t slp_wpu : 1;     // [3]
        uint32_t __5_4 : 2;       // [5:4]
        uint32_t wpdo : 1;        // [6]
        uint32_t wpu : 1;         // [7]
        uint32_t __9_8 : 2;       // [9:8]
        uint32_t padi_switch : 1; // [10]
        uint32_t se : 1;          // [11]
        uint32_t wpus : 1;        // [12]
        uint32_t dslp_en : 6;     // [18:13]
        uint32_t drv : 2;         // [20:19]
        uint32_t __31_21 : 11;    // [31:21]
    } b;
} REG_IOMUX_PAD_CAMERA_RST_L_T;

// pad_spi_camera_sck
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;      // [0]
        uint32_t slp_ie : 1;      // [1]
        uint32_t slp_wpdo : 1;    // [2]
        uint32_t slp_wpu : 1;     // [3]
        uint32_t __5_4 : 2;       // [5:4]
        uint32_t wpdo : 1;        // [6]
        uint32_t wpu : 1;         // [7]
        uint32_t __9_8 : 2;       // [9:8]
        uint32_t padi_switch : 1; // [10]
        uint32_t se : 1;          // [11]
        uint32_t wpus : 1;        // [12]
        uint32_t dslp_en : 6;     // [18:13]
        uint32_t drv : 2;         // [20:19]
        uint32_t __31_21 : 11;    // [31:21]
    } b;
} REG_IOMUX_PAD_SPI_CAMERA_SCK_T;

// pad_spi_camera_si_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;      // [0]
        uint32_t slp_ie : 1;      // [1]
        uint32_t slp_wpdo : 1;    // [2]
        uint32_t slp_wpu : 1;     // [3]
        uint32_t __5_4 : 2;       // [5:4]
        uint32_t wpdo : 1;        // [6]
        uint32_t wpu : 1;         // [7]
        uint32_t __9_8 : 2;       // [9:8]
        uint32_t padi_switch : 1; // [10]
        uint32_t se : 1;          // [11]
        uint32_t wpus : 1;        // [12]
        uint32_t dslp_en : 6;     // [18:13]
        uint32_t drv : 2;         // [20:19]
        uint32_t __31_21 : 11;    // [31:21]
    } b;
} REG_IOMUX_PAD_SPI_CAMERA_SI_1_T;

// pad_spi_camera_si_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;      // [0]
        uint32_t slp_ie : 1;      // [1]
        uint32_t slp_wpdo : 1;    // [2]
        uint32_t slp_wpu : 1;     // [3]
        uint32_t __5_4 : 2;       // [5:4]
        uint32_t wpdo : 1;        // [6]
        uint32_t wpu : 1;         // [7]
        uint32_t __9_8 : 2;       // [9:8]
        uint32_t padi_switch : 1; // [10]
        uint32_t se : 1;          // [11]
        uint32_t wpus : 1;        // [12]
        uint32_t dslp_en : 6;     // [18:13]
        uint32_t drv : 2;         // [20:19]
        uint32_t __31_21 : 11;    // [31:21]
    } b;
} REG_IOMUX_PAD_SPI_CAMERA_SI_0_T;

// pad_camera_ref_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_CAMERA_REF_CLK_T;

// pad_camera_pwdn
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_CAMERA_PWDN_T;

// pad_i2s_sdat_i
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_I2S_SDAT_I_T;

// pad_i2s1_sdat_o
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_I2S1_SDAT_O_T;

// pad_i2s1_lrck
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_I2S1_LRCK_T;

// pad_i2s1_bck
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_I2S1_BCK_T;

// pad_i2s1_mclk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_I2S1_MCLK_T;

// pad_i2c_m2_scl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_I2C_M2_SCL_T;

// pad_i2c_m2_sda
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_I2C_M2_SDA_T;

// pad_nand_sel
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_NAND_SEL_T;

// pad_keyout_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYOUT_3_T;

// pad_keyout_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYOUT_2_T;

// pad_keyout_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYOUT_1_T;

// pad_keyout_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYOUT_0_T;

// pad_keyin_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYIN_3_T;

// pad_keyin_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYIN_2_T;

// pad_keyin_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYIN_1_T;

// pad_keyin_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_KEYIN_0_T;

// pad_lcd_rstb
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_LCD_RSTB_T;

// pad_lcd_fmark
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_LCD_FMARK_T;

// pad_spi_lcd_select
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SPI_LCD_SELECT_T;

// pad_spi_lcd_cs
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SPI_LCD_CS_T;

// pad_spi_lcd_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SPI_LCD_CLK_T;

// pad_spi_lcd_sdc
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SPI_LCD_SDC_T;

// pad_spi_lcd_sio
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SPI_LCD_SIO_T;

// pad_sdmmc1_rst
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_RST_T;

// pad_sdmmc1_data_7
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_7_T;

// pad_sdmmc1_data_6
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_6_T;

// pad_sdmmc1_data_5
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_5_T;

// pad_sdmmc1_data_4
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_4_T;

// pad_sdmmc1_data_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_3_T;

// pad_sdmmc1_data_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_2_T;

// pad_sdmmc1_data_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_1_T;

// pad_sdmmc1_data_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_DATA_0_T;

// pad_sdmmc1_cmd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_CMD_T;

// pad_sdmmc1_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_SDMMC1_CLK_T;

// pad_uart_2_rts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_UART_2_RTS_T;

// pad_uart_2_cts
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_UART_2_CTS_T;

// pad_uart_2_txd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_UART_2_TXD_T;

// pad_uart_2_rxd
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_UART_2_RXD_T;

// pad_i2c_m1_sda
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_I2C_M1_SDA_T;

// pad_i2c_m1_scl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_I2C_M1_SCL_T;

// pad_gpio_23
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_GPIO_23_T;

// pad_gpio_22
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_GPIO_22_T;

// pad_gpio_21
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_GPIO_21_T;

// pad_gpio_20
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_GPIO_20_T;

// pad_gpio_19
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_GPIO_19_T;

// pad_gpio_18
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_GPIO_18_T;

// pad_gpio_17
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_GPIO_17_T;

// pad_gpio_16
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 2;      // [20:19]
        uint32_t __31_21 : 11; // [31:21]
    } b;
} REG_IOMUX_PAD_GPIO_16_T;

// pad_m_spi_d_3
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 3;      // [21:19]
        uint32_t __31_22 : 10; // [31:22]
    } b;
} REG_IOMUX_PAD_M_SPI_D_3_T;

// pad_m_spi_d_2
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 3;      // [21:19]
        uint32_t __31_22 : 10; // [31:22]
    } b;
} REG_IOMUX_PAD_M_SPI_D_2_T;

// pad_m_spi_d_1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 3;      // [21:19]
        uint32_t __31_22 : 10; // [31:22]
    } b;
} REG_IOMUX_PAD_M_SPI_D_1_T;

// pad_m_spi_d_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 3;      // [21:19]
        uint32_t __31_22 : 10; // [31:22]
    } b;
} REG_IOMUX_PAD_M_SPI_D_0_T;

// pad_m_spi_cs
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 3;      // [21:19]
        uint32_t __31_22 : 10; // [31:22]
    } b;
} REG_IOMUX_PAD_M_SPI_CS_T;

// pad_m_spi_clk
typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 3;      // [21:19]
        uint32_t __31_22 : 10; // [31:22]
    } b;
} REG_IOMUX_PAD_M_SPI_CLK_T;

// pwr_pad_ctl
#define IOMUX_PWRREG_MSEN_LPVDDIO_18_33 (1 << 0)
#define IOMUX_PWRREG_MSOUT_LPVDDIO_18_33 (1 << 1)
#define IOMUX_PWRREG_MS_LPVDDIO_18_33 (1 << 2)
#define IOMUX_PWRREG_MSEN_VLPVDDIO1833_1 (1 << 3)
#define IOMUX_PWRREG_MSOUT_VLPVDDIO1833_1 (1 << 4)
#define IOMUX_PWRREG_MS_VLPVDDIO1833_1 (1 << 5)
#define IOMUX_PWRREG_MSEN_VSIM1 (1 << 6)
#define IOMUX_PWRREG_MSOUT_VSIM1 (1 << 7)
#define IOMUX_PWRREG_MS_VSIM1 (1 << 8)
#define IOMUX_PWRREG_MSEN_VSIM0 (1 << 9)
#define IOMUX_PWRREG_MSOUT_VSIM0 (1 << 10)
#define IOMUX_PWRREG_MS_VSIM0 (1 << 11)
#define IOMUX_PWRREG_MSEN_VDDIO_18_33 (1 << 12)
#define IOMUX_PWRREG_MSOUT_VDDIO_18_33 (1 << 13)
#define IOMUX_PWRREG_MS_VDDIO_18_33 (1 << 14)
#define IOMUX_PWRREG_MSEN_V_LCD_18_33 (1 << 15)
#define IOMUX_PWRREG_MSOUT_V_LCD_18_33 (1 << 16)
#define IOMUX_PWRREG_MS_V_LCD_18_33 (1 << 17)
#define IOMUX_PWRREG_MSEN_V_MMC_18_30 (1 << 18)
#define IOMUX_PWRREG_MSOUT_V_MMC_18_30 (1 << 19)
#define IOMUX_PWRREG_MS_V_MMC_18_30 (1 << 20)

// rfdig_gpio_7
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// rfdig_gpio_6
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// rfdig_gpio_5
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// rfdig_gpio_4
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// rfdig_gpio_3
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// rfdig_gpio_2
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// rfdig_gpio_1
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// rfdig_gpio_0
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyin_4
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyout_5
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyin_5
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyout_4
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// uart_1_rts
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// uart_1_txd
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// uart_1_rxd
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// uart_1_cts
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_0
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_3
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_2
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_1
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_7
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_6
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_5
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_4
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// adi_sda
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// adi_scl
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// resetb
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// osc_32k
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// pmic_ext_int
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// chip_pd
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// clk26m_pmic
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sim_1_rst
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sim_1_dio
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sim_1_clk
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sim_0_rst
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sim_0_dio
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sim_0_clk
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sw_clk
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sw_dio
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// debug_host_tx
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// debug_host_rx
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// debug_host_clk
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// camera_rst_l
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// spi_camera_sck
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// spi_camera_si_1
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// spi_camera_si_0
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// camera_ref_clk
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// camera_pwdn
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// i2s_sdat_i
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// i2s1_sdat_o
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// i2s1_lrck
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// i2s1_bck
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// i2s1_mclk
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// i2c_m2_scl
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// i2c_m2_sda
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// nand_sel
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyout_3
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyout_2
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyout_1
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyout_0
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyin_3
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyin_2
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyin_1
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// keyin_0
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// lcd_rstb
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// lcd_fmark
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// spi_lcd_select
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// spi_lcd_cs
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// spi_lcd_clk
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// spi_lcd_sdc
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// spi_lcd_sio
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_rst
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_data_7
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_data_6
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_data_5
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_data_4
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_data_3
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_data_2
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_data_1
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_data_0
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_cmd
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// sdmmc1_clk
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// uart_2_rts
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// uart_2_cts
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// uart_2_txd
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// uart_2_rxd
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// i2c_m1_sda
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// i2c_m1_scl
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_23
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_22
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_21
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_20
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_19
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_18
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_17
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// gpio_16
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// m_spi_d_3
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// m_spi_d_2
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// m_spi_d_1
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// m_spi_d_0
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// m_spi_cs
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// m_spi_clk
#define IOMUX_FUNC_SEL(n) (((n)&0xf) << 2)

// pad_rfdig_gpio_7
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_RFDIG_GPIO_7_DRV(n) (((n)&0x3) << 19)

// pad_rfdig_gpio_6
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_RFDIG_GPIO_6_DRV(n) (((n)&0x3) << 19)

// pad_rfdig_gpio_5
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_RFDIG_GPIO_5_DRV(n) (((n)&0x3) << 19)

// pad_rfdig_gpio_4
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_RFDIG_GPIO_4_DRV(n) (((n)&0x3) << 19)

// pad_rfdig_gpio_3
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_RFDIG_GPIO_3_DRV(n) (((n)&0x3) << 19)

// pad_rfdig_gpio_2
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_RFDIG_GPIO_2_DRV(n) (((n)&0x3) << 19)

// pad_rfdig_gpio_1
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_RFDIG_GPIO_1_DRV(n) (((n)&0x3) << 19)

// pad_rfdig_gpio_0
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_RFDIG_GPIO_0_DRV(n) (((n)&0x3) << 19)

// pad_keyin_4
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYIN_4_DRV(n) (((n)&0xf) << 19)

// pad_keyout_5
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYOUT_5_DRV(n) (((n)&0xf) << 19)

// pad_keyin_5
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYIN_5_DRV(n) (((n)&0xf) << 19)

// pad_keyout_4
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYOUT_4_DRV(n) (((n)&0xf) << 19)

// pad_uart_1_rts
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_UART_1_RTS_DRV(n) (((n)&0xf) << 19)

// pad_uart_1_txd
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_UART_1_TXD_DRV(n) (((n)&0xf) << 19)

// pad_uart_1_rxd
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_UART_1_RXD_DRV(n) (((n)&0xf) << 19)

// pad_uart_1_cts
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_UART_1_CTS_DRV(n) (((n)&0xf) << 19)

// pad_gpio_0
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_0_DRV(n) (((n)&0xf) << 19)

// pad_gpio_3
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_3_DRV(n) (((n)&0xf) << 19)

// pad_gpio_2
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_2_DRV(n) (((n)&0xf) << 19)

// pad_gpio_1
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_1_DRV(n) (((n)&0xf) << 19)

// pad_gpio_7
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_7_DRV(n) (((n)&0xf) << 19)

// pad_gpio_6
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_6_DRV(n) (((n)&0xf) << 19)

// pad_gpio_5
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_5_DRV(n) (((n)&0xf) << 19)

// pad_gpio_4
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_4_DRV(n) (((n)&0xf) << 19)

// pad_adi_sda
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_ADI_SDA_DRV(n) (((n)&0x3) << 19)

// pad_adi_scl
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_ADI_SCL_DRV(n) (((n)&0x3) << 19)

// pad_resetb
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_PAD_RESETB_DRV(n) (((n)&0x3) << 19)

// pad_osc_32k
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_OSC_32K_DRV(n) (((n)&0x3) << 19)

// pad_pmic_ext_int
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_PMIC_EXT_INT_DRV(n) (((n)&0x3) << 19)

// pad_chip_pd
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_CHIP_PD_DRV(n) (((n)&0x3) << 19)

// pad_clk26m_pmic
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_CLK26M_PMIC_DRV(n) (((n)&0x3) << 19)

// pad_sim_1_rst
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SIM_1_RST_DRV(n) (((n)&0xf) << 19)

// pad_sim_1_dio
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SIM_1_DIO_DRV(n) (((n)&0xf) << 19)

// pad_sim_1_clk
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SIM_1_CLK_DRV(n) (((n)&0xf) << 19)

// pad_sim_0_rst
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SIM_0_RST_DRV(n) (((n)&0xf) << 19)

// pad_sim_0_dio
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SIM_0_DIO_DRV(n) (((n)&0xf) << 19)

// pad_sim_0_clk
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SIM_0_CLK_DRV(n) (((n)&0xf) << 19)

// pad_sw_clk
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SW_CLK_DRV(n) (((n)&0x3) << 19)

// pad_sw_dio
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SW_DIO_DRV(n) (((n)&0x3) << 19)

// pad_debug_host_tx
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_DEBUG_HOST_TX_DRV(n) (((n)&0x3) << 19)

// pad_debug_host_rx
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_PADI_SWITCH (1 << 10)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_DEBUG_HOST_RX_DRV(n) (((n)&0x3) << 19)

// pad_debug_host_clk
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_PADI_SWITCH (1 << 10)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_DEBUG_HOST_CLK_DRV(n) (((n)&0x3) << 19)

// pad_camera_rst_l
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_PADI_SWITCH (1 << 10)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_CAMERA_RST_L_DRV(n) (((n)&0x3) << 19)

// pad_spi_camera_sck
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_PADI_SWITCH (1 << 10)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SPI_CAMERA_SCK_DRV(n) (((n)&0x3) << 19)

// pad_spi_camera_si_1
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_PADI_SWITCH (1 << 10)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SPI_CAMERA_SI_1_DRV(n) (((n)&0x3) << 19)

// pad_spi_camera_si_0
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_PADI_SWITCH (1 << 10)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SPI_CAMERA_SI_0_DRV(n) (((n)&0x3) << 19)

// pad_camera_ref_clk
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_CAMERA_REF_CLK_DRV(n) (((n)&0x3) << 19)

// pad_camera_pwdn
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_CAMERA_PWDN_DRV(n) (((n)&0x3) << 19)

// pad_i2s_sdat_i
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_I2S_SDAT_I_DRV(n) (((n)&0x3) << 19)

// pad_i2s1_sdat_o
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_I2S1_SDAT_O_DRV(n) (((n)&0x3) << 19)

// pad_i2s1_lrck
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_I2S1_LRCK_DRV(n) (((n)&0x3) << 19)

// pad_i2s1_bck
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_I2S1_BCK_DRV(n) (((n)&0x3) << 19)

// pad_i2s1_mclk
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_I2S1_MCLK_DRV(n) (((n)&0x3) << 19)

// pad_i2c_m2_scl
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_I2C_M2_SCL_DRV(n) (((n)&0x3) << 19)

// pad_i2c_m2_sda
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_I2C_M2_SDA_DRV(n) (((n)&0x3) << 19)

// pad_nand_sel
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_NAND_SEL_DRV(n) (((n)&0x3) << 19)

// pad_keyout_3
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYOUT_3_DRV(n) (((n)&0xf) << 19)

// pad_keyout_2
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYOUT_2_DRV(n) (((n)&0xf) << 19)

// pad_keyout_1
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYOUT_1_DRV(n) (((n)&0xf) << 19)

// pad_keyout_0
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYOUT_0_DRV(n) (((n)&0xf) << 19)

// pad_keyin_3
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYIN_3_DRV(n) (((n)&0xf) << 19)

// pad_keyin_2
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYIN_2_DRV(n) (((n)&0xf) << 19)

// pad_keyin_1
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYIN_1_DRV(n) (((n)&0xf) << 19)

// pad_keyin_0
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_KEYIN_0_DRV(n) (((n)&0xf) << 19)

// pad_lcd_rstb
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_LCD_RSTB_DRV(n) (((n)&0xf) << 19)

// pad_lcd_fmark
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_LCD_FMARK_DRV(n) (((n)&0xf) << 19)

// pad_spi_lcd_select
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SPI_LCD_SELECT_DRV(n) (((n)&0xf) << 19)

// pad_spi_lcd_cs
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SPI_LCD_CS_DRV(n) (((n)&0xf) << 19)

// pad_spi_lcd_clk
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SPI_LCD_CLK_DRV(n) (((n)&0xf) << 19)

// pad_spi_lcd_sdc
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SPI_LCD_SDC_DRV(n) (((n)&0xf) << 19)

// pad_spi_lcd_sio
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SPI_LCD_SIO_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_rst
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_RST_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_data_7
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_DATA_7_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_data_6
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_DATA_6_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_data_5
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_DATA_5_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_data_4
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_DATA_4_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_data_3
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_DATA_3_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_data_2
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_DATA_2_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_data_1
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_DATA_1_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_data_0
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_DATA_0_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_cmd
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_CMD_DRV(n) (((n)&0xf) << 19)

// pad_sdmmc1_clk
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_SDMMC1_CLK_DRV(n) (((n)&0xf) << 19)

// pad_uart_2_rts
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_UART_2_RTS_DRV(n) (((n)&0x3) << 19)

// pad_uart_2_cts
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_UART_2_CTS_DRV(n) (((n)&0x3) << 19)

// pad_uart_2_txd
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_UART_2_TXD_DRV(n) (((n)&0x3) << 19)

// pad_uart_2_rxd
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_UART_2_RXD_DRV(n) (((n)&0x3) << 19)

// pad_i2c_m1_sda
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_I2C_M1_SDA_DRV(n) (((n)&0x3) << 19)

// pad_i2c_m1_scl
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_I2C_M1_SCL_DRV(n) (((n)&0x3) << 19)

// pad_gpio_23
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_23_DRV(n) (((n)&0x3) << 19)

// pad_gpio_22
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_22_DRV(n) (((n)&0x3) << 19)

// pad_gpio_21
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_21_DRV(n) (((n)&0x3) << 19)

// pad_gpio_20
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_20_DRV(n) (((n)&0x3) << 19)

// pad_gpio_19
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_19_DRV(n) (((n)&0x3) << 19)

// pad_gpio_18
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_18_DRV(n) (((n)&0x3) << 19)

// pad_gpio_17
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_17_DRV(n) (((n)&0x3) << 19)

// pad_gpio_16
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_GPIO_16_DRV(n) (((n)&0x3) << 19)

// pad_m_spi_d_3
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_M_SPI_D_3_DRV(n) (((n)&0x7) << 19)

// pad_m_spi_d_2
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_M_SPI_D_2_DRV(n) (((n)&0x7) << 19)

// pad_m_spi_d_1
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_M_SPI_D_1_DRV(n) (((n)&0x7) << 19)

// pad_m_spi_d_0
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_M_SPI_D_0_DRV(n) (((n)&0x7) << 19)

// pad_m_spi_cs
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_M_SPI_CS_DRV(n) (((n)&0x7) << 19)

// pad_m_spi_clk
#define IOMUX_SLP_OE (1 << 0)
#define IOMUX_SLP_IE (1 << 1)
#define IOMUX_SLP_WPDO (1 << 2)
#define IOMUX_SLP_WPU (1 << 3)
#define IOMUX_WPDO (1 << 6)
#define IOMUX_WPU (1 << 7)
#define IOMUX_SE (1 << 11)
#define IOMUX_WPUS (1 << 12)
#define IOMUX_DSLP_EN(n) (((n)&0x3f) << 13)
#define IOMUX_PAD_M_SPI_CLK_DRV(n) (((n)&0x7) << 19)
#undef uint32_t

#endif

