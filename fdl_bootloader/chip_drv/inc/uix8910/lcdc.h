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


#ifndef _LCDC_H_
#define _LCDC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'lcdc'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// LCDC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_LCDC_BASE              0x08807000

typedef volatile struct
{
    REG32                          gd_command;                   //0x00000000
    REG32                          gd_status;                    //0x00000004
    REG32                          gd_eof_irq;                   //0x00000008
    REG32                          gd_eof_irq_mask;              //0x0000000C
    REG32                          gd_lcd_ctrl;                  //0x00000010
    /// All value are in cycle number of system clock
    REG32                          gd_lcd_timing;                //0x00000014
    REG32                          gd_lcd_mem_address;           //0x00000018
    REG32                          gd_lcd_stride_offset;         //0x0000001C
    REG32                          gd_lcd_single_access;         //0x00000020
    REG32                          gd_spilcd_config;             //0x00000024
    REG32 Reserved_00000028;                    //0x00000028
    REG32                          DCT_SHIFTR_UV_reg1;           //0x0000002C
    REG32                          DPI_CONFIG;                   //0x00000030
    REG32                          DPI_FRAM0_ADDR;               //0x00000034
    REG32                          DPI_FRAM0_CON;                //0x00000038
    REG32                          DPI_FRAM1_ADDR;               //0x0000003C
    REG32                          DPI_FRAM1_CON;                //0x00000040
    REG32                          DPI_FRAM2_ADDR;               //0x00000044
    REG32                          DPI_FRAM2_CON;                //0x00000048
    REG32                          DPI_SIZE;                     //0x0000004C
    REG32                          DPI_FIFO_CTRL;                //0x00000050
    REG32                          DPI_THROT;                    //0x00000054
    REG32                          DPI_POL;                      //0x00000058
    REG32                          DPI_TIME0;                    //0x0000005C
    REG32                          DPI_TIME1;                    //0x00000060
    REG32                          DPI_TIME2;                    //0x00000064
    REG32                          DPI_TIME3;                    //0x00000068
    REG32                          DPI_STATUS;                   //0x0000006C
    REG32                          DITHER_CTRL;                  //0x00000070
    REG32                          DITHER_MATRIX0_0;             //0x00000074
    REG32                          DITHER_MATRIX0_1;             //0x00000078
    REG32                          DITHER_MATRIX1;               //0x0000007C
    REG32                          TECON;                        //0x00000080
    REG32                          TECON2;                       //0x00000084
    REG32 Reserved_00000088[2];                 //0x00000088
    REG32                          DCT_SHIFTR_Y_reg0;            //0x00000090
    REG32                          GAMMA_R_COEF;                 //0x00000094
    REG32                          GAMMA_G_COEF;                 //0x00000098
    REG32                          GAMMA_B_COEF;                 //0x0000009C
    REG32 Reserved_000000A0[216];               //0x000000A0
    REG32                          dsi_power_up;                 //0x00000400
    REG32                          dsi_enable;                   //0x00000404
    REG32                          dsi_lane_config;              //0x00000408
    REG32                          dsi_pixel_num;                //0x0000040C
    REG32                          dsi_pixel_type;               //0x00000410
    REG32                          dsi_tx_mode;                  //0x00000414
    REG32                          dsi_vcid_bllp;                //0x00000418
    REG32                          dsi_line_byte_num;            //0x0000041C
    REG32                          dsi_hsa_num;                  //0x00000420
    REG32                          dsi_hbp_num;                  //0x00000424
    REG32                          dsi_hfp_num;                  //0x00000428
    REG32                          dsi_hact_num;                 //0x0000042C
    REG32                          dsi_vsa_line_num;             //0x00000430
    REG32                          dsi_vbp_line_num;             //0x00000434
    REG32                          dsi_vfp_line_num;             //0x00000438
    REG32                          dsi_vact_line_num;            //0x0000043C
    REG32                          dsi_cmd_num;                  //0x00000440
    REG32                          dsi_cq_ctrl;                  //0x00000444
    REG32                          dsi_int_clear;                //0x00000448
    REG32                          dsi_int_mask;                 //0x0000044C
    REG32                          dsi_fifo_set;                 //0x00000450
    REG32                          dsi_irq_status;               //0x00000454
    REG32                          dsi_irq_cause;                //0x00000458
    REG32                          dsi_rx_payload;               //0x0000045C
    REG32                          dsi_esc_pause_num;            //0x00000460
    REG32                          dsi_lptx_ratio;               //0x00000464
    REG32                          dsi_dsi_sel;                  //0x00000468
    REG32                          dsi_bllp_num;                 //0x0000046C
    REG32                          dsi_hsa_hd_num;               //0x00000470
    REG32                          dsi_hbp_hd_num;               //0x00000474
    REG32                          dsi_hfp_hd_num;               //0x00000478
    REG32                          dsi_hact_hd_num;              //0x0000047C
    REG32                          dsi_clk_state_ad_num;         //0x00000480
    REG32                          dsi_t_clk_zero;               //0x00000484
    REG32                          dsi_rx_fifo_cnt;              //0x00000488
    REG32 Reserved_0000048C[29];                //0x0000048C
    REG32                          dsi_hs_mode_ctrl;             //0x00000500
    REG32                          dsi_swap_control;             //0x00000504
    REG32                          dsi_t_lp00;                   //0x00000508
    REG32                          dsi_t_lp01;                   //0x0000050C
    REG32                          dsi_t_lp10;                   //0x00000510
    REG32                          dsi_t_lp11;                   //0x00000514
    REG32                          dsi_t_hs_zero;                //0x00000518
    REG32                          dsi_t_hs_sync;                //0x0000051C
    REG32                          dsi_t_hs_trail;               //0x00000520
    REG32                          dsi_t_clk_lp00;               //0x00000524
    REG32                          dsi_t_clk_lp01;               //0x00000528
    REG32                          dsi_ulps_wakeup;              //0x0000052C
    REG32                          dsi_t_clk_eot;                //0x00000530
    REG32                          dsi_t_bta_lpx;                //0x00000534
    REG32                          dsi_t_bta_go;                 //0x00000538
    REG32                          dsi_t_bta_00;                 //0x0000053C
    REG32                          dsi_t_bta_timeout;            //0x00000540
    REG32                          dsi_t_phy_ctrl;               //0x00000544
    REG32                          dsi_t_ulps_ana_ctrl;          //0x00000548
    REG32                          dsi_t_bta_sure;               //0x0000054C
    REG32                          dsi_t_clk_post;               //0x00000550
    REG32                          dsi_cd_err_num;               //0x00000554
    REG32 Reserved_00000558[2];                 //0x00000558
    REG32                          dsi_phy_reg;                  //0x00000560
    REG32                          dsi_pll_reg;                  //0x00000564
    REG32 Reserved_00000568[2];                 //0x00000568
    REG32                          dsi_misc_ctrl;                //0x00000570
    REG32 Reserved_00000574[3];                 //0x00000574
    REG32                          dsi_cmd_1_0_reg;              //0x00000580
    REG32                          dsi_cmd_1_1_reg;              //0x00000584
    REG32                          dsi_cmd_1_2_reg;              //0x00000588
    REG32                          dsi_cmd_1_3_reg;              //0x0000058C
    REG32                          dsi_cmd_1_4_reg;              //0x00000590
    REG32                          dsi_cmd_1_5_reg;              //0x00000594
    REG32                          dsi_cmd_1_6_reg;              //0x00000598
    REG32                          dsi_cmd_1_7_reg;              //0x0000059C
    REG32                          dsi_cmd_1_8_reg;              //0x000005A0
    REG32                          dsi_cmd_1_9_reg;              //0x000005A4
    REG32                          dsi_cmd_1_a_reg;              //0x000005A8
    REG32                          dsi_cmd_1_b_reg;              //0x000005AC
    REG32                          dsi_cmd_1_c_reg;              //0x000005B0
    REG32                          dsi_cmd_1_d_reg;              //0x000005B4
    REG32                          dsi_cmd_1_e_reg;              //0x000005B8
    REG32                          dsi_cmd_1_f_reg;              //0x000005BC
    REG32                          dsi_cmd_2_0_reg;              //0x000005C0
    REG32                          dsi_cmd_2_1_reg;              //0x000005C4
    REG32                          dsi_cmd_2_2_reg;              //0x000005C8
    REG32                          dsi_cmd_2_3_reg;              //0x000005CC
    REG32                          dsi_cmd_2_4_reg;              //0x000005D0
    REG32                          dsi_cmd_2_5_reg;              //0x000005D4
    REG32                          dsi_cmd_2_6_reg;              //0x000005D8
    REG32                          dsi_cmd_2_7_reg;              //0x000005DC
    REG32                          dsi_cmd_2_8_reg;              //0x000005E0
    REG32                          dsi_cmd_2_9_reg;              //0x000005E4
    REG32                          dsi_cmd_2_a_reg;              //0x000005E8
    REG32                          dsi_cmd_2_b_reg;              //0x000005EC
    REG32                          dsi_cmd_2_c_reg;              //0x000005F0
    REG32                          dsi_cmd_2_d_reg;              //0x000005F4
    REG32                          dsi_cmd_2_e_reg;              //0x000005F8
    REG32                          dsi_cmd_2_f_reg;              //0x000005FC
    REG32                          dsi_read_reg0;                //0x00000600
    REG32                          dsi_read_reg1;                //0x00000604
    REG32                          dsi_read_reg2;                //0x00000608
    REG32                          dsi_read_reg3;                //0x0000060C
    REG32                          dsi_read_reg4;                //0x00000610
    REG32                          dsi_read_reg5;                //0x00000614
    REG32                          dsi_read_reg6;                //0x00000618
    REG32                          dsi_read_reg7;                //0x0000061C
    REG32                          dsi_read_reg8;                //0x00000620
    REG32                          dsi_read_reg9;                //0x00000624
    REG32                          dsi_read_reg10;               //0x00000628
    REG32                          dsi_read_reg11;               //0x0000062C
    REG32                          dsi_read_reg12;               //0x00000630
    REG32                          dsi_read_reg13;               //0x00000634
    REG32                          dsi_read_reg14;               //0x00000638
    REG32                          dsi_read_reg15;               //0x0000063C
    REG32                          dsi_read_reg16;               //0x00000640
    REG32                          dsi_read_reg17;               //0x00000644
    REG32                          dsi_read_reg18;               //0x00000648
    REG32                          dsi_read_reg19;               //0x0000064C
    REG32                          dsi_read_reg20;               //0x00000650
    REG32                          dsi_read_reg21;               //0x00000654
    REG32                          dsi_read_reg22;               //0x00000658
    REG32                          dsi_read_reg23;               //0x0000065C
    REG32                          dsi_read_reg24;               //0x00000660
    REG32                          dsi_read_reg25;               //0x00000664
    REG32                          dsi_read_reg26;               //0x00000668
    REG32                          dsi_read_reg27;               //0x0000066C
    REG32                          dsi_read_reg28;               //0x00000670
    REG32                          dsi_read_reg29;               //0x00000674
    REG32                          dsi_read_reg30;               //0x00000678
    REG32                          dsi_read_reg31;               //0x0000067C
} HWP_LCDC_T;

#define hwp_lcdc                   ((HWP_LCDC_T*) REG_ACCESS_ADDRESS(REG_LCDC_BASE))


//gd_command
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_LCDC_GD_COMMAND_T;

//gd_status
typedef union {
    REG32 v;
    struct {
        REG32 ia_busy : 1; // [0], read only
        REG32 __3_1 : 3;
        REG32 lcd_busy : 1; // [4], read only
        REG32 __31_5 : 27;
    } b;
} REG_LCDC_GD_STATUS_T;

//gd_eof_irq
typedef union {
    REG32 v;
    struct {
        REG32 eof_cause : 1; // [0], write clear
        REG32 __1_1 : 1;
        REG32 vsync_rise : 1; // [2]
        REG32 vsync_fall : 1; // [3]
        REG32 dpi_overflow : 1; // [4]
        REG32 dpi_frameover : 1; // [5]
        REG32 mipi_int : 1; // [6]
        REG32 __15_7 : 9;
        REG32 eof_status : 1; // [16], write clear
        REG32 __31_17 : 15;
    } b;
} REG_LCDC_GD_EOF_IRQ_T;

//gd_eof_irq_mask
typedef union {
    REG32 v;
    struct {
        REG32 eof_mask : 1; // [0]
        REG32 vsync_rise_mask : 1; // [1]
        REG32 vsync_fall_mask : 1; // [2]
        REG32 dpi_overflow_mask : 1; // [3]
        REG32 dpi_frameover_mask : 1; // [4]
        REG32 mipi_int_mask : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_LCDC_GD_EOF_IRQ_MASK_T;

//gd_lcd_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 destination : 2; // [1:0]
        REG32 __3_2 : 2;
        REG32 output_format : 3; // [6:4]
        REG32 high_byte : 1; // [7]
        REG32 cs0_polarity : 1; // [8]
        REG32 cs1_polarity : 1; // [9]
        REG32 rs_polarity : 1; // [10]
        REG32 wr_polarity : 1; // [11]
        REG32 rd_polarity : 1; // [12]
        REG32 gamma_en : 1; // [13]
        REG32 __24_14 : 11;
        REG32 bus_sel : 2; // [26:25]
        REG32 __31_27 : 5;
    } b;
} REG_LCDC_GD_LCD_CTRL_T;

//gd_lcd_timing
typedef union {
    REG32 v;
    struct {
        REG32 tas : 3; // [2:0]
        REG32 __3_3 : 1;
        REG32 tah : 3; // [6:4]
        REG32 __7_7 : 1;
        REG32 pwl : 6; // [13:8]
        REG32 __15_14 : 2;
        REG32 pwh : 6; // [21:16]
        REG32 __31_22 : 10;
    } b;
} REG_LCDC_GD_LCD_TIMING_T;

//gd_lcd_mem_address
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 addr_dst : 30; // [31:2]
    } b;
} REG_LCDC_GD_LCD_MEM_ADDRESS_T;

//gd_lcd_stride_offset
typedef union {
    REG32 v;
    struct {
        REG32 stride_offset : 11; // [10:0]
        REG32 __31_11 : 21;
    } b;
} REG_LCDC_GD_LCD_STRIDE_OFFSET_T;

//gd_lcd_single_access
typedef union {
    REG32 v;
    struct {
        REG32 lcd_data : 16; // [15:0]
        REG32 type : 1; // [16]
        REG32 start_write : 1; // [17]
        REG32 start_read : 1; // [18]
        REG32 __31_19 : 13;
    } b;
} REG_LCDC_GD_LCD_SINGLE_ACCESS_T;

//gd_spilcd_config
typedef union {
    REG32 v;
    struct {
        REG32 spi_lcd_select : 1; // [0]
        REG32 spi_device_id : 6; // [6:1]
        REG32 spi_clk_divider : 8; // [14:7]
        REG32 spi_dummy_cycle : 3; // [17:15]
        REG32 spi_line : 2; // [19:18]
        REG32 spi_rx_byte : 3; // [22:20]
        REG32 spi_rw : 1; // [23]
        REG32 __31_24 : 8;
    } b;
} REG_LCDC_GD_SPILCD_CONFIG_T;

//DCT_SHIFTR_UV_reg1
typedef union {
    REG32 v;
    struct {
        REG32 vsync_toggle_hsync_cnt : 11; // [10:0]
        REG32 __29_11 : 19;
        REG32 reg_rgb_wait : 1; // [30]
        REG32 __31_31 : 1;
    } b;
} REG_LCDC_DCT_SHIFTR_UV_REG1_T;

//DPI_CONFIG
typedef union {
    REG32 v;
    struct {
        REG32 r_rgb_enable : 1; // [0]
        REG32 r_frame1_enable : 1; // [1]
        REG32 r_frame2_enable : 1; // [2]
        REG32 r_rgb_order : 1; // [3]
        REG32 r_pix_fmt : 2; // [5:4]
        REG32 r_outoff_all : 1; // [6]
        REG32 r_outoff_clk : 1; // [7]
        REG32 r_outoff_data : 1; // [8]
        REG32 r_dsi_enable : 1; // [9]
        REG32 __11_10 : 2;
        REG32 r_rgb_format : 2; // [13:12]
        REG32 reg_empty_ctrl : 2; // [15:14]
        REG32 __19_16 : 4;
        REG32 mipi_cmd_sel : 1; // [20]
        REG32 reg_pend_req : 5; // [25:21]
        REG32 __31_26 : 6;
    } b;
} REG_LCDC_DPI_CONFIG_T;

//DPI_FRAM0_CON
typedef union {
    REG32 v;
    struct {
        REG32 r_frame0_valid : 1; // [0]
        REG32 __15_1 : 15;
        REG32 r_frame0_line_step : 13; // [28:16]
        REG32 __31_29 : 3;
    } b;
} REG_LCDC_DPI_FRAM0_CON_T;

//DPI_FRAM1_CON
typedef union {
    REG32 v;
    struct {
        REG32 r_frame1_valid : 1; // [0]
        REG32 __15_1 : 15;
        REG32 r_frame1_line_step : 13; // [28:16]
        REG32 __31_29 : 3;
    } b;
} REG_LCDC_DPI_FRAM1_CON_T;

//DPI_FRAM2_CON
typedef union {
    REG32 v;
    struct {
        REG32 r_frame2_valid : 1; // [0]
        REG32 __15_1 : 15;
        REG32 r_frame2_line_step : 13; // [28:16]
        REG32 __31_29 : 3;
    } b;
} REG_LCDC_DPI_FRAM2_CON_T;

//DPI_SIZE
typedef union {
    REG32 v;
    struct {
        REG32 r_h_size : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 r_v_size : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_LCDC_DPI_SIZE_T;

//DPI_FIFO_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 r_fifo_rst : 1; // [0]
        REG32 r_fifo_rst_auto : 1; // [1]
        REG32 __15_2 : 14;
        REG32 r_data_fifo_lowthres : 10; // [25:16]
        REG32 __31_26 : 6;
    } b;
} REG_LCDC_DPI_FIFO_CTRL_T;

//DPI_THROT
typedef union {
    REG32 v;
    struct {
        REG32 r_throttle_en : 1; // [0]
        REG32 __15_1 : 15;
        REG32 r_throttle_period : 10; // [25:16]
        REG32 __31_26 : 6;
    } b;
} REG_LCDC_DPI_THROT_T;

//DPI_POL
typedef union {
    REG32 v;
    struct {
        REG32 r_dot_clk_div : 8; // [7:0]
        REG32 r_dot_clk_pol : 1; // [8]
        REG32 r_hsync_pol : 1; // [9]
        REG32 r_vsync_pol : 1; // [10]
        REG32 r_de_pol : 1; // [11]
        REG32 __12_12 : 1;
        REG32 r_reg_clk_adj : 4; // [16:13]
        REG32 __31_17 : 15;
    } b;
} REG_LCDC_DPI_POL_T;

//DPI_TIME0
typedef union {
    REG32 v;
    struct {
        REG32 r_back_porch_end_vsync_timer : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 r_front_porch_start_vsync_timer : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_LCDC_DPI_TIME0_T;

//DPI_TIME1
typedef union {
    REG32 v;
    struct {
        REG32 r_vsync_include_hsync_th_high : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 r_vsync_include_hsync_th_low : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_LCDC_DPI_TIME1_T;

//DPI_TIME2
typedef union {
    REG32 v;
    struct {
        REG32 r_hsync_include_dotclk_th_high : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 r_hsync_include_dotclk_th_low : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_LCDC_DPI_TIME2_T;

//DPI_TIME3
typedef union {
    REG32 v;
    struct {
        REG32 r_rgb_data_enable_start_timer : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 r_rgb_data_enable_end_timer : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_LCDC_DPI_TIME3_T;

//DPI_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 frame0_over : 1; // [0], read only
        REG32 frame1_over : 1; // [1], read only
        REG32 frame2_over : 1; // [2], read only
        REG32 frame_runing : 1; // [3], read only
        REG32 current_frame : 2; // [5:4], read only
        REG32 __31_6 : 26;
    } b;
} REG_LCDC_DPI_STATUS_T;

//DITHER_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 r_dither_en : 1; // [0]
        REG32 dither_mode_r : 1; // [1]
        REG32 dither_mode_g : 1; // [2]
        REG32 dither_mode_b : 1; // [3]
        REG32 dither_ctrl_r : 2; // [5:4]
        REG32 dither_ctrl_g : 2; // [7:6]
        REG32 dither_ctrl_b : 2; // [9:8]
        REG32 __31_10 : 22;
    } b;
} REG_LCDC_DITHER_CTRL_T;

//DITHER_MATRIX1
typedef union {
    REG32 v;
    struct {
        REG32 dither_matrix1 : 8; // [7:0]
        REG32 reg_lfsr_initialdata : 12; // [19:8]
        REG32 __31_20 : 12;
    } b;
} REG_LCDC_DITHER_MATRIX1_T;

//TECON
typedef union {
    REG32 v;
    struct {
        REG32 te_en : 1; // [0]
        REG32 te_edge_sel : 1; // [1]
        REG32 te_mode : 1; // [2]
        REG32 __15_3 : 13;
        REG32 te_count2 : 12; // [27:16]
        REG32 __31_28 : 4;
    } b;
} REG_LCDC_TECON_T;

//TECON2
typedef union {
    REG32 v;
    struct {
        REG32 te_count1 : 29; // [28:0]
        REG32 __31_29 : 3;
    } b;
} REG_LCDC_TECON2_T;

//DCT_SHIFTR_Y_reg0
typedef union {
    REG32 v;
    struct {
        REG32 delay_counter : 24; // [23:0]
        REG32 __30_24 : 7;
        REG32 counter_en : 1; // [31]
    } b;
} REG_LCDC_DCT_SHIFTR_Y_REG0_T;

//GAMMA_R_COEF
typedef union {
    REG32 v;
    struct {
        REG32 gamma_r_a : 8; // [7:0]
        REG32 gamma_r_d : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_GAMMA_R_COEF_T;

//GAMMA_G_COEF
typedef union {
    REG32 v;
    struct {
        REG32 gamma_g_a : 8; // [7:0]
        REG32 gamma_g_d : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_GAMMA_G_COEF_T;

//GAMMA_B_COEF
typedef union {
    REG32 v;
    struct {
        REG32 gamma_b_a : 8; // [7:0]
        REG32 gamma_b_d : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_GAMMA_B_COEF_T;

//dsi_power_up
typedef union {
    REG32 v;
    struct {
        REG32 pu_dsi : 1; // [0]
        REG32 dsipll_pu : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_LCDC_DSI_POWER_UP_T;

//dsi_enable
typedef union {
    REG32 v;
    struct {
        REG32 dsi_enable_reg : 1; // [0]
        REG32 dsi_lprx_lpcd_enable : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_LCDC_DSI_ENABLE_T;

//dsi_lane_config
typedef union {
    REG32 v;
    struct {
        REG32 dsi_config_reg : 2; // [1:0]
        REG32 video_packet_hd_dr : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_LCDC_DSI_LANE_CONFIG_T;

//dsi_pixel_num
typedef union {
    REG32 v;
    struct {
        REG32 pixel_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_PIXEL_NUM_T;

//dsi_pixel_type
typedef union {
    REG32 v;
    struct {
        REG32 pixel_type_reg : 3; // [2:0]
        REG32 rgb_quad_en_reg : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_LCDC_DSI_PIXEL_TYPE_T;

//dsi_tx_mode
typedef union {
    REG32 v;
    struct {
        REG32 dsi_tx_mode_reg : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_LCDC_DSI_TX_MODE_T;

//dsi_vcid_bllp
typedef union {
    REG32 v;
    struct {
        REG32 vci_reg : 2; // [1:0]
        REG32 bllp_enable_per_l_reg : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_LCDC_DSI_VCID_BLLP_T;

//dsi_line_byte_num
typedef union {
    REG32 v;
    struct {
        REG32 frame_idle_en : 1; // [0]
        REG32 v2c_switch_start : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_LCDC_DSI_LINE_BYTE_NUM_T;

//dsi_hsa_num
typedef union {
    REG32 v;
    struct {
        REG32 hsa_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_HSA_NUM_T;

//dsi_hbp_num
typedef union {
    REG32 v;
    struct {
        REG32 hbp_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_HBP_NUM_T;

//dsi_hfp_num
typedef union {
    REG32 v;
    struct {
        REG32 hfp_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_HFP_NUM_T;

//dsi_hact_num
typedef union {
    REG32 v;
    struct {
        REG32 rgb_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_HACT_NUM_T;

//dsi_vsa_line_num
typedef union {
    REG32 v;
    struct {
        REG32 vsa_line_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_VSA_LINE_NUM_T;

//dsi_vbp_line_num
typedef union {
    REG32 v;
    struct {
        REG32 vbp_line_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_VBP_LINE_NUM_T;

//dsi_vfp_line_num
typedef union {
    REG32 v;
    struct {
        REG32 vfp_line_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_VFP_LINE_NUM_T;

//dsi_vact_line_num
typedef union {
    REG32 v;
    struct {
        REG32 vat_line_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_VACT_LINE_NUM_T;

//dsi_cmd_num
typedef union {
    REG32 v;
    struct {
        REG32 dsi_cmd_num_reg : 5; // [4:0]
        REG32 __31_5 : 27;
    } b;
} REG_LCDC_DSI_CMD_NUM_T;

//dsi_cq_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 dsi_cmd_start : 1; // [0]
        REG32 dsi_cmd_merge_en_reg : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_LCDC_DSI_CQ_CTRL_T;

//dsi_int_clear
typedef union {
    REG32 v;
    struct {
        REG32 cd_err : 1; // [0]
        REG32 rx_bta_timeout : 1; // [1]
        REG32 rx_ecc_err : 1; // [2]
        REG32 rx_crc_err : 1; // [3]
        REG32 rx_data_end : 1; // [4]
        REG32 cmd_q_end : 1; // [5]
        REG32 fifo_ovfl : 1; // [6]
        REG32 fifo_half : 1; // [7]
        REG32 rx_te_ready : 1; // [8]
        REG32 frame_done : 1; // [9]
        REG32 sleepout_done : 1; // [10]
        REG32 __31_11 : 21;
    } b;
} REG_LCDC_DSI_INT_CLEAR_T;

//dsi_int_mask
typedef union {
    REG32 v;
    struct {
        REG32 cd_err : 1; // [0]
        REG32 rx_bta_timeout : 1; // [1]
        REG32 rx_ecc_err : 1; // [2]
        REG32 rx_crc_err : 1; // [3]
        REG32 rx_data_end : 1; // [4]
        REG32 cmd_q_end : 1; // [5]
        REG32 fifo_ovfl : 1; // [6]
        REG32 fifo_half : 1; // [7]
        REG32 rx_te_ready : 1; // [8]
        REG32 frame_done : 1; // [9]
        REG32 sleepout_done : 1; // [10]
        REG32 __31_11 : 21;
    } b;
} REG_LCDC_DSI_INT_MASK_T;

//dsi_fifo_set
typedef union {
    REG32 v;
    struct {
        REG32 fifo_read_set_reg : 1; // [0]
        REG32 fifo_set_reg : 1; // [1]
        REG32 lpk_mem_rst_reg : 1; // [2]
        REG32 rx_ph_clr_reg : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_LCDC_DSI_FIFO_SET_T;

//dsi_irq_status
typedef union {
    REG32 v;
    struct {
        REG32 cd_err : 1; // [0], read only
        REG32 rx_bta_timeout : 1; // [1], read only
        REG32 rx_ecc_err : 1; // [2], read only
        REG32 rx_crc_err : 1; // [3], read only
        REG32 rx_data_end : 1; // [4], read only
        REG32 cmd_q_end : 1; // [5], read only
        REG32 fifo_ovfl : 1; // [6], read only
        REG32 fifo_half : 1; // [7], read only
        REG32 rx_te_ready : 1; // [8], read only
        REG32 frame_done : 1; // [9], read only
        REG32 sleepout_done : 1; // [10], read only
        REG32 fifo_empty : 1; // [11], read only
        REG32 __31_12 : 20;
    } b;
} REG_LCDC_DSI_IRQ_STATUS_T;

//dsi_irq_cause
typedef union {
    REG32 v;
    struct {
        REG32 cd_err : 1; // [0], read only
        REG32 rx_bta_timeout : 1; // [1], read only
        REG32 rx_ecc_err : 1; // [2], read only
        REG32 rx_crc_err : 1; // [3], read only
        REG32 rx_data_end : 1; // [4], read only
        REG32 cmd_q_end : 1; // [5], read only
        REG32 fifo_ovfl : 1; // [6], read only
        REG32 fifo_half : 1; // [7], read only
        REG32 rx_te_ready : 1; // [8], read only
        REG32 frame_done : 1; // [9], read only
        REG32 sleepout_done : 1; // [10], read only
        REG32 __31_11 : 21;
    } b;
} REG_LCDC_DSI_IRQ_CAUSE_T;

//dsi_esc_pause_num
typedef union {
    REG32 v;
    struct {
        REG32 esc_pause_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_LCDC_DSI_ESC_PAUSE_NUM_T;

//dsi_lptx_ratio
typedef union {
    REG32 v;
    struct {
        REG32 phy_lp_tx_rate_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_LPTX_RATIO_T;

//dsi_dsi_sel
typedef union {
    REG32 v;
    struct {
        REG32 mipi_dsi_sel_reg : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_LCDC_DSI_DSI_SEL_T;

//dsi_bllp_num
typedef union {
    REG32 v;
    struct {
        REG32 blp_num_reg : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_LCDC_DSI_BLLP_NUM_T;

//dsi_hsa_hd_num
typedef union {
    REG32 v;
    struct {
        REG32 hsa_hd_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_HSA_HD_NUM_T;

//dsi_hbp_hd_num
typedef union {
    REG32 v;
    struct {
        REG32 hbp_hd_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_HBP_HD_NUM_T;

//dsi_hfp_hd_num
typedef union {
    REG32 v;
    struct {
        REG32 hfp_hd_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_HFP_HD_NUM_T;

//dsi_hact_hd_num
typedef union {
    REG32 v;
    struct {
        REG32 rgb_hd_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_HACT_HD_NUM_T;

//dsi_clk_state_ad_num
typedef union {
    REG32 v;
    struct {
        REG32 clk_state_ad_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_CLK_STATE_AD_NUM_T;

//dsi_t_clk_zero
typedef union {
    REG32 v;
    struct {
        REG32 clk_hd_zero_num_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_CLK_ZERO_T;

//dsi_rx_fifo_cnt
typedef union {
    REG32 v;
    struct {
        REG32 rx_fifo_cnt : 9; // [8:0], read only
        REG32 __31_9 : 23;
    } b;
} REG_LCDC_DSI_RX_FIFO_CNT_T;

//dsi_hs_mode_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 hs_mode_reg : 1; // [0]
        REG32 hs_clk_always_reg : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_LCDC_DSI_HS_MODE_CTRL_T;

//dsi_swap_control
typedef union {
    REG32 v;
    struct {
        REG32 data_swap_reg : 1; // [0]
        REG32 sync_word_swap_reg : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_LCDC_DSI_SWAP_CONTROL_T;

//dsi_t_lp00
typedef union {
    REG32 v;
    struct {
        REG32 t_lp_00_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_LP00_T;

//dsi_t_lp01
typedef union {
    REG32 v;
    struct {
        REG32 t_lp_01_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_LP01_T;

//dsi_t_lp10
typedef union {
    REG32 v;
    struct {
        REG32 t_lp_10_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_LP10_T;

//dsi_t_lp11
typedef union {
    REG32 v;
    struct {
        REG32 t_lp_11_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_LP11_T;

//dsi_t_hs_zero
typedef union {
    REG32 v;
    struct {
        REG32 t_zero_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_HS_ZERO_T;

//dsi_t_hs_sync
typedef union {
    REG32 v;
    struct {
        REG32 t_sync_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_HS_SYNC_T;

//dsi_t_hs_trail
typedef union {
    REG32 v;
    struct {
        REG32 t_eot_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_HS_TRAIL_T;

//dsi_t_clk_lp00
typedef union {
    REG32 v;
    struct {
        REG32 clk_lp_00_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_CLK_LP00_T;

//dsi_t_clk_lp01
typedef union {
    REG32 v;
    struct {
        REG32 clk_lp_01_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_CLK_LP01_T;

//dsi_ulps_wakeup
typedef union {
    REG32 v;
    struct {
        REG32 t_ulps_wakeup : 20; // [19:0]
        REG32 __31_20 : 12;
    } b;
} REG_LCDC_DSI_ULPS_WAKEUP_T;

//dsi_t_clk_eot
typedef union {
    REG32 v;
    struct {
        REG32 clk_eot_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_CLK_EOT_T;

//dsi_t_bta_lpx
typedef union {
    REG32 v;
    struct {
        REG32 t_bta_lpx_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_BTA_LPX_T;

//dsi_t_bta_go
typedef union {
    REG32 v;
    struct {
        REG32 t_bta_cd_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_BTA_GO_T;

//dsi_t_bta_00
typedef union {
    REG32 v;
    struct {
        REG32 t_bta_00_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_BTA_00_T;

//dsi_t_bta_timeout
typedef union {
    REG32 v;
    struct {
        REG32 t_bta_timeout_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_BTA_TIMEOUT_T;

//dsi_t_phy_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 phase_selg : 3; // [2:0]
        REG32 vreg_bit : 2; // [4:3]
        REG32 hz_bit_reg : 5; // [9:5]
        REG32 hz_bit_dr : 5; // [14:10]
        REG32 __31_15 : 17;
    } b;
} REG_LCDC_DSI_T_PHY_CTRL_T;

//dsi_t_ulps_ana_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 ulps_enable : 1; // [0]
        REG32 wakeup_en : 1; // [1]
        REG32 trig_en : 1; // [2]
        REG32 __3_3 : 1;
        REG32 rx_bit : 2; // [5:4]
        REG32 cd_bit : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_LCDC_DSI_T_ULPS_ANA_CTRL_T;

//dsi_t_bta_sure
typedef union {
    REG32 v;
    struct {
        REG32 t_rx_bta_cd_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_BTA_SURE_T;

//dsi_t_clk_post
typedef union {
    REG32 v;
    struct {
        REG32 clk_post_reg : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_LCDC_DSI_T_CLK_POST_T;

//dsi_cd_err_num
typedef union {
    REG32 v;
    struct {
        REG32 cd_err_num : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_LCDC_DSI_CD_ERR_NUM_T;

//dsi_phy_reg
typedef union {
    REG32 v;
    struct {
        REG32 clk_band : 4; // [3:0]
        REG32 phase_sel : 3; // [6:4]
        REG32 __7_7 : 1;
        REG32 rx_bit : 2; // [9:8]
        REG32 cd_bit : 2; // [11:10]
        REG32 clk_enable : 1; // [12]
        REG32 ck2lane_edge_sel : 1; // [13]
        REG32 ck2dig_edge_sel : 1; // [14]
        REG32 __15_15 : 1;
        REG32 clk_dly_sel : 4; // [19:16]
        REG32 drv_sel : 2; // [21:20]
        REG32 lp_ldo_bit : 2; // [23:22]
        REG32 hs_ldo_bit : 2; // [25:24]
        REG32 clk_pu_hsldo : 1; // [26]
        REG32 d1_pu_hsldo : 1; // [27]
        REG32 d0_pu_hsldo : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_LCDC_DSI_PHY_REG_T;

//dsi_pll_reg
typedef union {
    REG32 v;
    struct {
        REG32 pll_cpr2_bit : 3; // [2:0]
        REG32 __3_3 : 1;
        REG32 pll_cpc2_ibit : 3; // [6:4]
        REG32 __7_7 : 1;
        REG32 pll_cpbias_ibit : 3; // [10:8]
        REG32 __11_11 : 1;
        REG32 reg_res_bit : 2; // [13:12]
        REG32 __15_14 : 2;
        REG32 pll_vreg_bit : 4; // [19:16]
        REG32 vco_low_test : 1; // [20]
        REG32 vco_high_test : 1; // [21]
        REG32 pll_test_en : 1; // [22]
        REG32 pll_refmulti2_en : 1; // [23]
        REG32 sdm_clk_test_en : 1; // [24]
        REG32 pcon_mode : 1; // [25]
        REG32 __31_26 : 6;
    } b;
} REG_LCDC_DSI_PLL_REG_T;

//dsi_misc_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 eot_tx_disable_reg : 1; // [0]
        REG32 frz_disable_reg : 1; // [1]
        REG32 rx_lp_lp11_reg : 1; // [2]
        REG32 tx_lp_swap_reg : 1; // [3]
        REG32 tx_lp_cmd_reg : 8; // [11:4]
        REG32 tx_lp_cmd_dr : 1; // [12]
        REG32 __31_13 : 19;
    } b;
} REG_LCDC_DSI_MISC_CTRL_T;


//gd_command
#define LCDC_START                 (1<<0)

//gd_status
#define LCDC_IA_BUSY               (1<<0)
#define LCDC_LCD_BUSY              (1<<4)

//gd_eof_irq
#define LCDC_EOF_CAUSE             (1<<0)
#define LCDC_VSYNC_RISE            (1<<2)
#define LCDC_VSYNC_FALL            (1<<3)
#define LCDC_DPI_OVERFLOW          (1<<4)
#define LCDC_DPI_FRAMEOVER         (1<<5)
#define LCDC_MIPI_INT              (1<<6)
#define LCDC_EOF_STATUS            (1<<16)

//gd_eof_irq_mask
#define LCDC_EOF_MASK              (1<<0)
#define LCDC_VSYNC_RISE_MASK       (1<<1)
#define LCDC_VSYNC_FALL_MASK       (1<<2)
#define LCDC_DPI_OVERFLOW_MASK     (1<<3)
#define LCDC_DPI_FRAMEOVER_MASK    (1<<4)
#define LCDC_MIPI_INT_MASK         (1<<5)

//gd_lcd_ctrl
#define LCDC_DESTINATION_LCD_CS_0  (0<<0)
#define LCDC_DESTINATION_LCD_CS_1  (1<<0)
#define LCDC_DESTINATION_MEMORY_RAM (3<<0)
#define LCDC_DESTINATION_V_LCD_CS_0 (0)
#define LCDC_DESTINATION_V_LCD_CS_1 (1)
#define LCDC_DESTINATION_V_MEMORY_RAM (3)
#define LCDC_OUTPUT_FORMAT_16_BIT_RGB565 (6<<4)
#define LCDC_OUTPUT_FORMAT_32_BIT_ARGB8888 (7<<4)
#define LCDC_OUTPUT_FORMAT_V_16_BIT_RGB565 (6)
#define LCDC_OUTPUT_FORMAT_V_32_BIT_ARGB8888 (7)
#define LCDC_HIGH_BYTE             (1<<7)
#define LCDC_CS0_POLARITY          (1<<8)
#define LCDC_CS1_POLARITY          (1<<9)
#define LCDC_RS_POLARITY           (1<<10)
#define LCDC_WR_POLARITY           (1<<11)
#define LCDC_RD_POLARITY           (1<<12)
#define LCDC_GAMMA_EN              (1<<13)
#define LCDC_BUS_SEL(n)            (((n)&3)<<25)

//gd_lcd_timing
#define LCDC_TAS(n)                (((n)&7)<<0)
#define LCDC_TAH(n)                (((n)&7)<<4)
#define LCDC_PWL(n)                (((n)&0x3F)<<8)
#define LCDC_PWH(n)                (((n)&0x3F)<<16)

//gd_lcd_mem_address
#define LCDC_ADDR_DST(n)           (((n)&0x3FFFFFFF)<<2)

//gd_lcd_stride_offset
#define LCDC_STRIDE_OFFSET(n)      (((n)&0x7FF)<<0)

//gd_lcd_single_access
#define LCDC_LCD_DATA(n)           (((n)&0xFFFF)<<0)
#define LCDC_TYPE                  (1<<16)
#define LCDC_START_WRITE           (1<<17)
#define LCDC_START_READ            (1<<18)

//gd_spilcd_config
#define LCDC_SPI_LCD_SELECT        (1<<0)
#define LCDC_SPI_DEVICE_ID(n)      (((n)&0x3F)<<1)
#define LCDC_SPI_DEVICE_ID_MASK    (0x3F<<1)
#define LCDC_SPI_DEVICE_ID_SHIFT   (1)
#define LCDC_SPI_CLK_DIVIDER(n)    (((n)&0xFF)<<7)
#define LCDC_SPI_CLK_DIVIDER_MASK  (0xFF<<7)
#define LCDC_SPI_CLK_DIVIDER_SHIFT (7)
#define LCDC_SPI_DUMMY_CYCLE(n)    (((n)&7)<<15)
#define LCDC_SPI_DUMMY_CYCLE_MASK  (7<<15)
#define LCDC_SPI_DUMMY_CYCLE_SHIFT (15)
#define LCDC_SPI_LINE_MASK         (3<<18)
#define LCDC_SPI_LINE_4            (0<<18)
#define LCDC_SPI_LINE_3            (1<<18)
#define LCDC_SPI_LINE_4_START_BYTE (2<<18)
#define LCDC_SPI_LINE_3_TWO_LANE   (3<<18)
#define LCDC_SPI_LINE_V_4          (0)
#define LCDC_SPI_LINE_V_3          (1)
#define LCDC_SPI_LINE_V_4_START_BYTE (2)
#define LCDC_SPI_LINE_V_3_TWO_LANE (3)
#define LCDC_SPI_RX_BYTE(n)        (((n)&7)<<20)
#define LCDC_SPI_RX_BYTE_MASK      (7<<20)
#define LCDC_SPI_RX_BYTE_SHIFT     (20)
#define LCDC_SPI_RW_WRITE          (0<<23)
#define LCDC_SPI_RW_READ           (1<<23)
#define LCDC_SPI_RW_V_WRITE        (0)
#define LCDC_SPI_RW_V_READ         (1)

//DCT_SHIFTR_UV_reg1
#define LCDC_VSYNC_TOGGLE_HSYNC_CNT(n) (((n)&0x7FF)<<0)
#define LCDC_REG_RGB_WAIT          (1<<30)

//DPI_CONFIG
#define LCDC_REG_PEND_REQ(n)       (((n)&31)<<21)
#define LCDC_MIPI_CMD_SEL          (1<<20)
#define LCDC_REG_EMPTY_CTRL(n)     (((n)&3)<<14)
#define LCDC_R_RGB_FORMAT(n)       (((n)&3)<<12)
#define LCDC_R_DSI_ENABLE          (1<<9)
#define LCDC_R_OUTOFF_DATA         (1<<8)
#define LCDC_R_OUTOFF_CLK          (1<<7)
#define LCDC_R_OUTOFF_ALL          (1<<6)
#define LCDC_R_PIX_FMT(n)          (((n)&3)<<4)
#define LCDC_R_RGB_ORDER           (1<<3)
#define LCDC_R_FRAME2_ENABLE       (1<<2)
#define LCDC_R_FRAME1_ENABLE       (1<<1)
#define LCDC_R_RGB_ENABLE          (1<<0)

//DPI_FRAM0_CON
#define LCDC_R_FRAME0_LINE_STEP(n) (((n)&0x1FFF)<<16)
#define LCDC_R_FRAME0_VALID        (1<<0)

//DPI_FRAM1_CON
#define LCDC_R_FRAME1_LINE_STEP(n) (((n)&0x1FFF)<<16)
#define LCDC_R_FRAME1_VALID        (1<<0)

//DPI_FRAM2_CON
#define LCDC_R_FRAME2_LINE_STEP(n) (((n)&0x1FFF)<<16)
#define LCDC_R_FRAME2_VALID        (1<<0)

//DPI_SIZE
#define LCDC_R_V_SIZE(n)           (((n)&0x7FF)<<16)
#define LCDC_R_H_SIZE(n)           (((n)&0x7FF)<<0)

//DPI_FIFO_CTRL
#define LCDC_R_DATA_FIFO_LOWTHRES(n) (((n)&0x3FF)<<16)
#define LCDC_R_FIFO_RST_AUTO       (1<<1)
#define LCDC_R_FIFO_RST            (1<<0)

//DPI_THROT
#define LCDC_R_THROTTLE_PERIOD(n)  (((n)&0x3FF)<<16)
#define LCDC_R_THROTTLE_EN         (1<<0)

//DPI_POL
#define LCDC_R_REG_CLK_ADJ(n)      (((n)&15)<<13)
#define LCDC_R_DE_POL              (1<<11)
#define LCDC_R_VSYNC_POL           (1<<10)
#define LCDC_R_HSYNC_POL           (1<<9)
#define LCDC_R_DOT_CLK_POL         (1<<8)
#define LCDC_R_DOT_CLK_DIV(n)      (((n)&0xFF)<<0)

//DPI_TIME0
#define LCDC_R_FRONT_PORCH_START_VSYNC_TIMER(n) (((n)&0x7FF)<<16)
#define LCDC_R_BACK_PORCH_END_VSYNC_TIMER(n) (((n)&0x7FF)<<0)

//DPI_TIME1
#define LCDC_R_VSYNC_INCLUDE_HSYNC_TH_LOW(n) (((n)&0x7FF)<<16)
#define LCDC_R_VSYNC_INCLUDE_HSYNC_TH_HIGH(n) (((n)&0x7FF)<<0)

//DPI_TIME2
#define LCDC_R_HSYNC_INCLUDE_DOTCLK_TH_LOW(n) (((n)&0x7FF)<<16)
#define LCDC_R_HSYNC_INCLUDE_DOTCLK_TH_HIGH(n) (((n)&0x7FF)<<0)

//DPI_TIME3
#define LCDC_R_RGB_DATA_ENABLE_END_TIMER(n) (((n)&0x7FF)<<16)
#define LCDC_R_RGB_DATA_ENABLE_START_TIMER(n) (((n)&0x7FF)<<0)

//DPI_STATUS
#define LCDC_CURRENT_FRAME(n)      (((n)&3)<<4)
#define LCDC_FRAME_RUNING          (1<<3)
#define LCDC_FRAME2_OVER           (1<<2)
#define LCDC_FRAME1_OVER           (1<<1)
#define LCDC_FRAME0_OVER           (1<<0)

//DITHER_CTRL
#define LCDC_R_DITHER_EN           (1<<0)
#define LCDC_DITHER_MODE_R         (1<<1)
#define LCDC_DITHER_MODE_G         (1<<2)
#define LCDC_DITHER_MODE_B         (1<<3)
#define LCDC_DITHER_CTRL_R(n)      (((n)&3)<<4)
#define LCDC_DITHER_CTRL_G(n)      (((n)&3)<<6)
#define LCDC_DITHER_CTRL_B(n)      (((n)&3)<<8)

//DITHER_MATRIX0_0
#define LCDC_DITHER_MATRIX0_0(n)   (((n)&0xFFFFFFFF)<<0)

//DITHER_MATRIX0_1
#define LCDC_DITHER_MATRIX0_1(n)   (((n)&0xFFFFFFFF)<<0)

//DITHER_MATRIX1
#define LCDC_DITHER_MATRIX1(n)     (((n)&0xFF)<<0)
#define LCDC_REG_LFSR_INITIALDATA(n) (((n)&0xFFF)<<8)

//TECON
#define LCDC_TE_COUNT2(n)          (((n)&0xFFF)<<16)
#define LCDC_TE_MODE               (1<<2)
#define LCDC_TE_EDGE_SEL           (1<<1)
#define LCDC_TE_EN                 (1<<0)

//TECON2
#define LCDC_TE_COUNT1(n)          (((n)&0x1FFFFFFF)<<0)

//DCT_SHIFTR_Y_reg0
#define LCDC_COUNTER_EN            (1<<31)
#define LCDC_DELAY_COUNTER(n)      (((n)&0xFFFFFF)<<0)

//GAMMA_R_COEF
#define LCDC_GAMMA_R_A(n)          (((n)&0xFF)<<0)
#define LCDC_GAMMA_R_D(n)          (((n)&0xFF)<<8)

//GAMMA_G_COEF
#define LCDC_GAMMA_G_A(n)          (((n)&0xFF)<<0)
#define LCDC_GAMMA_G_D(n)          (((n)&0xFF)<<8)

//GAMMA_B_COEF
#define LCDC_GAMMA_B_A(n)          (((n)&0xFF)<<0)
#define LCDC_GAMMA_B_D(n)          (((n)&0xFF)<<8)

//dsi_power_up
#define LCDC_DSIPLL_PU             (1<<1)
#define LCDC_PU_DSI                (1<<0)

//dsi_enable
#define LCDC_DSI_ENABLE_REG        (1<<0)
#define LCDC_DSI_LPRX_LPCD_ENABLE  (1<<1)

//dsi_lane_config
#define LCDC_VIDEO_PACKET_HD_DR    (1<<2)
#define LCDC_DSI_CONFIG_REG(n)     (((n)&3)<<0)

//dsi_pixel_num
#define LCDC_PIXEL_NUM_REG(n)      (((n)&0xFFFF)<<0)

//dsi_pixel_type
#define LCDC_RGB_QUAD_EN_REG       (1<<3)
#define LCDC_PIXEL_TYPE_REG(n)     (((n)&7)<<0)

//dsi_tx_mode
#define LCDC_DSI_TX_MODE_REG(n)    (((n)&3)<<0)

//dsi_vcid_bllp
#define LCDC_BLLP_ENABLE_PER_L_REG (1<<2)
#define LCDC_VCI_REG(n)            (((n)&3)<<0)

//dsi_line_byte_num
#define LCDC_V2C_SWITCH_START      (1<<1)
#define LCDC_FRAME_IDLE_EN         (1<<0)

//dsi_hsa_num
#define LCDC_HSA_NUM_REG(n)        (((n)&0xFFFF)<<0)

//dsi_hbp_num
#define LCDC_HBP_NUM_REG(n)        (((n)&0xFFFF)<<0)

//dsi_hfp_num
#define LCDC_HFP_NUM_REG(n)        (((n)&0xFFFF)<<0)

//dsi_hact_num
#define LCDC_RGB_NUM_REG(n)        (((n)&0xFFFF)<<0)

//dsi_vsa_line_num
#define LCDC_VSA_LINE_REG(n)       (((n)&0xFFFF)<<0)

//dsi_vbp_line_num
#define LCDC_VBP_LINE_REG(n)       (((n)&0xFFFF)<<0)

//dsi_vfp_line_num
#define LCDC_VFP_LINE_REG(n)       (((n)&0xFFFF)<<0)

//dsi_vact_line_num
#define LCDC_VAT_LINE_REG(n)       (((n)&0xFFFF)<<0)

//dsi_cmd_num
#define LCDC_DSI_CMD_NUM_REG(n)    (((n)&31)<<0)

//dsi_cq_ctrl
#define LCDC_DSI_CMD_MERGE_EN_REG  (1<<1)
#define LCDC_DSI_CMD_START         (1<<0)

//dsi_int_clear
#define LCDC_SLEEPOUT_DONE         (1<<10)
#define LCDC_FRAME_DONE            (1<<9)
#define LCDC_RX_TE_READY           (1<<8)
#define LCDC_FIFO_HALF             (1<<7)
#define LCDC_FIFO_OVFL             (1<<6)
#define LCDC_CMD_Q_END             (1<<5)
#define LCDC_RX_DATA_END           (1<<4)
#define LCDC_RX_CRC_ERR            (1<<3)
#define LCDC_RX_ECC_ERR            (1<<2)
#define LCDC_RX_BTA_TIMEOUT        (1<<1)
#define LCDC_CD_ERR                (1<<0)

//dsi_int_mask
//#define LCDC_SLEEPOUT_DONE       (1<<10)
//#define LCDC_FRAME_DONE          (1<<9)
//#define LCDC_RX_TE_READY         (1<<8)
//#define LCDC_FIFO_HALF           (1<<7)
//#define LCDC_FIFO_OVFL           (1<<6)
//#define LCDC_CMD_Q_END           (1<<5)
//#define LCDC_RX_DATA_END         (1<<4)
//#define LCDC_RX_CRC_ERR          (1<<3)
//#define LCDC_RX_ECC_ERR          (1<<2)
//#define LCDC_RX_BTA_TIMEOUT      (1<<1)
//#define LCDC_CD_ERR              (1<<0)

//dsi_fifo_set
#define LCDC_RX_PH_CLR_REG         (1<<3)
#define LCDC_LPK_MEM_RST_REG       (1<<2)
#define LCDC_FIFO_SET_REG          (1<<1)
#define LCDC_FIFO_READ_SET_REG     (1<<0)

//dsi_irq_status
#define LCDC_FIFO_EMPTY            (1<<11)
//#define LCDC_SLEEPOUT_DONE       (1<<10)
//#define LCDC_FRAME_DONE          (1<<9)
//#define LCDC_RX_TE_READY         (1<<8)
//#define LCDC_FIFO_HALF           (1<<7)
//#define LCDC_FIFO_OVFL           (1<<6)
//#define LCDC_CMD_Q_END           (1<<5)
//#define LCDC_RX_DATA_END         (1<<4)
//#define LCDC_RX_CRC_ERR          (1<<3)
//#define LCDC_RX_ECC_ERR          (1<<2)
//#define LCDC_RX_BTA_TIMEOUT      (1<<1)
//#define LCDC_CD_ERR              (1<<0)

//dsi_irq_cause
//#define LCDC_SLEEPOUT_DONE       (1<<10)
//#define LCDC_FRAME_DONE          (1<<9)
//#define LCDC_RX_TE_READY         (1<<8)
//#define LCDC_FIFO_HALF           (1<<7)
//#define LCDC_FIFO_OVFL           (1<<6)
//#define LCDC_CMD_Q_END           (1<<5)
//#define LCDC_RX_DATA_END         (1<<4)
//#define LCDC_RX_CRC_ERR          (1<<3)
//#define LCDC_RX_ECC_ERR          (1<<2)
//#define LCDC_RX_BTA_TIMEOUT      (1<<1)
//#define LCDC_CD_ERR              (1<<0)

//dsi_rx_payload
#define LCDC_RX_PAYLOAD_03(n)      (((n)&0xFFFFFFFF)<<0)

//dsi_esc_pause_num
#define LCDC_ESC_PAUSE_NUM_REG(n)  (((n)&15)<<0)

//dsi_lptx_ratio
#define LCDC_PHY_LP_TX_RATE_REG(n) (((n)&0xFFFF)<<0)

//dsi_dsi_sel
#define LCDC_MIPI_DSI_SEL_REG(n)   (((n)&0x3F)<<0)

//dsi_bllp_num
#define LCDC_BLP_NUM_REG(n)        (((n)&0x3F)<<0)

//dsi_hsa_hd_num
#define LCDC_HSA_HD_NUM_REG(n)     (((n)&0xFFFF)<<0)

//dsi_hbp_hd_num
#define LCDC_HBP_HD_NUM_REG(n)     (((n)&0xFFFF)<<0)

//dsi_hfp_hd_num
#define LCDC_HFP_HD_NUM_REG(n)     (((n)&0xFFFF)<<0)

//dsi_hact_hd_num
#define LCDC_RGB_HD_NUM_REG(n)     (((n)&0xFFFF)<<0)

//dsi_clk_state_ad_num
#define LCDC_CLK_STATE_AD_NUM_REG(n) (((n)&0xFFFF)<<0)

//dsi_t_clk_zero
#define LCDC_CLK_HD_ZERO_NUM_REG(n) (((n)&0xFFFF)<<0)

//dsi_rx_fifo_cnt
#define LCDC_RX_FIFO_CNT(n)        (((n)&0x1FF)<<0)

//dsi_hs_mode_ctrl
#define LCDC_HS_CLK_ALWAYS_REG     (1<<1)
#define LCDC_HS_MODE_REG           (1<<0)

//dsi_swap_control
#define LCDC_SYNC_WORD_SWAP_REG    (1<<1)
#define LCDC_DATA_SWAP_REG         (1<<0)

//dsi_t_lp00
#define LCDC_T_LP_00_REG(n)        (((n)&0xFFFF)<<0)

//dsi_t_lp01
#define LCDC_T_LP_01_REG(n)        (((n)&0xFFFF)<<0)

//dsi_t_lp10
#define LCDC_T_LP_10_REG(n)        (((n)&0xFFFF)<<0)

//dsi_t_lp11
#define LCDC_T_LP_11_REG(n)        (((n)&0xFFFF)<<0)

//dsi_t_hs_zero
#define LCDC_T_ZERO_REG(n)         (((n)&0xFFFF)<<0)

//dsi_t_hs_sync
#define LCDC_T_SYNC_REG(n)         (((n)&0xFFFF)<<0)

//dsi_t_hs_trail
#define LCDC_T_EOT_REG(n)          (((n)&0xFFFF)<<0)

//dsi_t_clk_lp00
#define LCDC_CLK_LP_00_REG(n)      (((n)&0xFFFF)<<0)

//dsi_t_clk_lp01
#define LCDC_CLK_LP_01_REG(n)      (((n)&0xFFFF)<<0)

//dsi_ulps_wakeup
#define LCDC_T_ULPS_WAKEUP(n)      (((n)&0xFFFFF)<<0)

//dsi_t_clk_eot
#define LCDC_CLK_EOT_REG(n)        (((n)&0xFFFF)<<0)

//dsi_t_bta_lpx
#define LCDC_T_BTA_LPX_REG(n)      (((n)&0xFFFF)<<0)

//dsi_t_bta_go
#define LCDC_T_BTA_CD_REG(n)       (((n)&0xFFFF)<<0)

//dsi_t_bta_00
#define LCDC_T_BTA_00_REG(n)       (((n)&0xFFFF)<<0)

//dsi_t_bta_timeout
#define LCDC_T_BTA_TIMEOUT_REG(n)  (((n)&0xFFFF)<<0)

//dsi_t_phy_ctrl
#define LCDC_HZ_BIT_DR(n)          (((n)&31)<<10)
#define LCDC_HZ_BIT_REG(n)         (((n)&31)<<5)
#define LCDC_VREG_BIT(n)           (((n)&3)<<3)
#define LCDC_PHASE_SELG(n)         (((n)&7)<<0)

//dsi_t_ulps_ana_ctrl
#define LCDC_CD_BIT(n)             (((n)&3)<<6)
#define LCDC_RX_BIT(n)             (((n)&3)<<4)
#define LCDC_TRIG_EN               (1<<2)
#define LCDC_WAKEUP_EN             (1<<1)
#define LCDC_ULPS_ENABLE           (1<<0)

//dsi_t_bta_sure
#define LCDC_T_RX_BTA_CD_REG(n)    (((n)&0xFFFF)<<0)

//dsi_t_clk_post
#define LCDC_CLK_POST_REG(n)       (((n)&0xFFFF)<<0)

//dsi_cd_err_num
#define LCDC_CD_ERR_NUM(n)         (((n)&15)<<0)

//dsi_phy_reg
#define LCDC_CLK_BAND(n)           (((n)&15)<<0)
#define LCDC_PHASE_SEL(n)          (((n)&7)<<4)
#define LCDC_RX_BIT_X1(n)          (((n)&3)<<8)
#define LCDC_CD_BIT_X1(n)          (((n)&3)<<10)
#define LCDC_CLK_ENABLE            (1<<12)
#define LCDC_CK2LANE_EDGE_SEL      (1<<13)
#define LCDC_CK2DIG_EDGE_SEL       (1<<14)
#define LCDC_CLK_DLY_SEL(n)        (((n)&15)<<16)
#define LCDC_DRV_SEL(n)            (((n)&3)<<20)
#define LCDC_LP_LDO_BIT(n)         (((n)&3)<<22)
#define LCDC_HS_LDO_BIT(n)         (((n)&3)<<24)
#define LCDC_CLK_PU_HSLDO          (1<<26)
#define LCDC_D1_PU_HSLDO           (1<<27)
#define LCDC_D0_PU_HSLDO           (1<<28)

//dsi_pll_reg
#define LCDC_PLL_CPR2_BIT(n)       (((n)&7)<<0)
#define LCDC_PLL_CPC2_IBIT(n)      (((n)&7)<<4)
#define LCDC_PLL_CPBIAS_IBIT(n)    (((n)&7)<<8)
#define LCDC_REG_RES_BIT(n)        (((n)&3)<<12)
#define LCDC_PLL_VREG_BIT(n)       (((n)&15)<<16)
#define LCDC_VCO_LOW_TEST          (1<<20)
#define LCDC_VCO_HIGH_TEST         (1<<21)
#define LCDC_PLL_TEST_EN           (1<<22)
#define LCDC_PLL_REFMULTI2_EN      (1<<23)
#define LCDC_SDM_CLK_TEST_EN       (1<<24)
#define LCDC_PCON_MODE             (1<<25)

//dsi_misc_ctrl
#define LCDC_TX_LP_CMD_DR          (1<<12)
#define LCDC_TX_LP_CMD_REG(n)      (((n)&0xFF)<<4)
#define LCDC_TX_LP_SWAP_REG        (1<<3)
#define LCDC_RX_LP_LP11_REG        (1<<2)
#define LCDC_FRZ_DISABLE_REG       (1<<1)
#define LCDC_EOT_TX_DISABLE_REG    (1<<0)

//dsi_cmd_1_0_reg
#define LCDC_DSI_CMD_1_0_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_1_reg
#define LCDC_DSI_CMD_1_1_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_2_reg
#define LCDC_DSI_CMD_1_2_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_3_reg
#define LCDC_DSI_CMD_1_3_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_4_reg
#define LCDC_DSI_CMD_1_4_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_5_reg
#define LCDC_DSI_CMD_1_5_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_6_reg
#define LCDC_DSI_CMD_1_6_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_7_reg
#define LCDC_DSI_CMD_1_7_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_8_reg
#define LCDC_DSI_CMD_1_8_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_9_reg
#define LCDC_DSI_CMD_1_9_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_a_reg
#define LCDC_DSI_CMD_1_A_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_b_reg
#define LCDC_DSI_CMD_1_B_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_c_reg
#define LCDC_DSI_CMD_1_C_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_d_reg
#define LCDC_DSI_CMD_1_D_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_e_reg
#define LCDC_DSI_CMD_1_E_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_1_f_reg
#define LCDC_DSI_CMD_1_F_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_0_reg
#define LCDC_DSI_CMD_2_0_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_1_reg
#define LCDC_DSI_CMD_2_1_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_2_reg
#define LCDC_DSI_CMD_2_2_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_3_reg
#define LCDC_DSI_CMD_2_3_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_4_reg
#define LCDC_DSI_CMD_2_4_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_5_reg
#define LCDC_DSI_CMD_2_5_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_6_reg
#define LCDC_DSI_CMD_2_6_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_7_reg
#define LCDC_DSI_CMD_2_7_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_8_reg
#define LCDC_DSI_CMD_2_8_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_9_reg
#define LCDC_DSI_CMD_2_9_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_a_reg
#define LCDC_DSI_CMD_2_A_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_b_reg
#define LCDC_DSI_CMD_2_B_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_c_reg
#define LCDC_DSI_CMD_2_C_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_d_reg
#define LCDC_DSI_CMD_2_D_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_e_reg
#define LCDC_DSI_CMD_2_E_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_cmd_2_f_reg
#define LCDC_DSI_CMD_2_F_REG(n)    (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg0
#define LCDC_READ_LCD0(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg1
#define LCDC_READ_LCD1(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg2
#define LCDC_READ_LCD2(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg3
#define LCDC_READ_LCD3(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg4
#define LCDC_READ_LCD4(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg5
#define LCDC_READ_LCD5(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg6
#define LCDC_READ_LCD6(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg7
#define LCDC_READ_LCD7(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg8
#define LCDC_READ_LCD8(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg9
#define LCDC_READ_LCD9(n)          (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg10
#define LCDC_READ_LCD10(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg11
#define LCDC_READ_LCD11(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg12
#define LCDC_READ_LCD12(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg13
#define LCDC_READ_LCD13(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg14
#define LCDC_READ_LCD14(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg15
#define LCDC_READ_LCD15(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg16
#define LCDC_READ_LCD16(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg17
#define LCDC_READ_LCD17(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg18
#define LCDC_READ_LCD18(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg19
#define LCDC_READ_LCD19(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg20
#define LCDC_READ_LCD20(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg21
#define LCDC_READ_LCD21(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg22
#define LCDC_READ_LCD22(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg23
#define LCDC_READ_LCD23(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg24
#define LCDC_READ_LCD24(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg25
#define LCDC_READ_LCD25(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg26
#define LCDC_READ_LCD26(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg27
#define LCDC_READ_LCD27(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg28
#define LCDC_READ_LCD28(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg29
#define LCDC_READ_LCD29(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg30
#define LCDC_READ_LCD30(n)         (((n)&0xFFFFFFFF)<<0)

//dsi_read_reg31
#define LCDC_READ_LCD31(n)         (((n)&0xFFFFFFFF)<<0)





#endif

