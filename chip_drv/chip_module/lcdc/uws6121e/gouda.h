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


#ifndef _GOUDA_H_
#define _GOUDA_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'gouda'."
#endif

#include "global_macros.h"
//#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================
#define GD_MAX_OUT_WIDTH                        (640)
#define GD_NB_BITS_LCDPOS                       (11)
#define GD_FP_FRAC_SIZE                         (8)
#define GD_FIXEDPOINT_SIZE                      (3+GD_FP_FRAC_SIZE)
#define GD_NB_BITS_STRIDE                       (13)
#define GD_MAX_SLCD_READ_LEN                    (4)
#define GD_MAX_SLCD_CLK_DIVIDER                 (255)
#define GD_NB_WORKBUF_WORDS                     (5856)
#define GD_NB_LCD_CMD_WORDS                     (64)
#define GD_SRAM_SIZE                            ((GD_NB_WORKBUF_WORDS+GD_NB_LCD_CMD_WORDS)*2)
#define GD_SRAM_ADDR_WIDTH                      (13)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// GOUDA_T
// -----------------------------------------------------------------------------
/// Number of bits for stride storage
// =============================================================================
#define REG_GOUDA_BASE             0x04804000

typedef volatile unsigned int REG32;

typedef volatile struct
{
    REG32                          gd_command;                   //0x00000000
    REG32                          gd_status;                    //0x00000004
    REG32                          gd_eof_irq;                   //0x00000008
    REG32                          gd_eof_irq_mask;              //0x0000000C
    REG32                          gd_roi_tl_ppos;               //0x00000010
    REG32                          gd_roi_br_ppos;               //0x00000014
    REG32                          gd_roi_bg_color;              //0x00000018
    REG32                          gd_vl_input_fmt;              //0x0000001C
    REG32                          gd_vl_tl_ppos;                //0x00000020
    REG32                          gd_vl_br_ppos;                //0x00000024
    REG32                          gd_vl_extents;                //0x00000028
    REG32                          gd_vl_blend_opt;              //0x0000002C
    REG32                          gd_vl_y_src;                  //0x00000030
    REG32                          gd_vl_u_src;                  //0x00000034
    REG32                          gd_vl_v_src;                  //0x00000038
    REG32                          gd_vl_resc_ratio;             //0x0000003C
    /// The Overlay layers have a fixed depth relative to their index. Overlay
    /// layer 0 is the first to be drawn (thus the deepest), overlay layer 2 is
    /// the last to be drawn.
    struct
    {
        REG32                      gd_ol_input_fmt;              //0x00000040
        REG32                      gd_ol_tl_ppos;                //0x00000044
        REG32                      gd_ol_br_ppos;                //0x00000048
        REG32                      gd_ol_blend_opt;              //0x0000004C
        REG32                      gd_ol_rgb_src;                //0x00000050
    } Overlay_Layer[3];
    REG32                          gd_lcd_ctrl;                  //0x0000007C
    /// All value are in cycle number of system clock
    REG32                          gd_lcd_timing;                //0x00000080
    REG32                          gd_lcd_mem_address;           //0x00000084
    REG32                          gd_lcd_stride_offset;         //0x00000088
    REG32                          gd_lcd_single_access;         //0x0000008C
    REG32                          gd_spilcd_config;             //0x00000090
    REG32                          gd_spilcd_rd;                 //0x00000094
    REG32                          gd_vl_fix_ratio;              //0x00000098
    REG32 Reserved_000000b4[41];               //0x000000A4

    REG32                          TECON;                        //0x0000009C
    REG32                          TECON2;                       //0x000000A0
    REG32 Reserved_000000A4[174];               //0x000000A4
} HWP_GOUDA_T;

#define hwp_gouda                  ((HWP_GOUDA_T*) REG_ACCESS_ADDRESS(REG_GOUDA_BASE))


//gd_command
typedef union {
    REG32 v;
    struct {
        REG32 start : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_GOUDA_GD_COMMAND_T;

//gd_status
typedef union {
    REG32 v;
    struct {
        REG32 ia_busy : 1; // [0], read only
        REG32 __3_1 : 3;
        REG32 lcd_busy : 1; // [4], read only
        REG32 __31_5 : 27;
    } b;
} REG_GOUDA_GD_STATUS_T;

//gd_eof_irq
typedef union {
    REG32 v;
    struct {
        REG32 eof_cause : 1; // [0], write clear
        REG32 __15_1 : 15;
        REG32 eof_status : 1; // [16], write clear
        REG32 __31_17 : 15;
    } b;
} REG_GOUDA_GD_EOF_IRQ_T;

//gd_eof_irq_mask
typedef union {
    REG32 v;
    struct {
        REG32 eof_mask : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_GOUDA_GD_EOF_IRQ_MASK_T;

//gd_roi_tl_ppos
typedef union {
    REG32 v;
    struct {
        REG32 x0 : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 y0 : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_GOUDA_GD_ROI_TL_PPOS_T;

//gd_roi_br_ppos
typedef union {
    REG32 v;
    struct {
        REG32 x1 : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 y1 : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_GOUDA_GD_ROI_BR_PPOS_T;

//gd_roi_bg_color
typedef union {
    REG32 v;
    struct {
        REG32 b : 5; // [4:0]
        REG32 g : 6; // [10:5]
        REG32 r : 5; // [15:11]
        REG32 __31_16 : 16;
    } b;
} REG_GOUDA_GD_ROI_BG_COLOR_T;

//gd_vl_input_fmt
typedef union {
    REG32 v;
    struct {
        REG32 format : 2; // [1:0]
        REG32 stride : 13; // [14:2]
        REG32 __30_15 : 16;
        REG32 active : 1; // [31]
    } b;
} REG_GOUDA_GD_VL_INPUT_FMT_T;

//gd_vl_tl_ppos
typedef union {
    REG32 v;
    struct {
        REG32 x0 : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 y0 : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_GOUDA_GD_VL_TL_PPOS_T;

//gd_vl_br_ppos
typedef union {
    REG32 v;
    struct {
        REG32 x1 : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 y1 : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_GOUDA_GD_VL_BR_PPOS_T;

//gd_vl_extents
typedef union {
    REG32 v;
    struct {
        REG32 max_line : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 max_col : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_GOUDA_GD_VL_EXTENTS_T;

//gd_vl_blend_opt
typedef union {
    REG32 v;
    struct {
        REG32 chroma_key_b : 5; // [4:0]
        REG32 chroma_key_g : 6; // [10:5]
        REG32 chroma_key_r : 5; // [15:11]
        REG32 chroma_key_enable : 1; // [16]
        REG32 chroma_key_mask : 3; // [19:17]
        REG32 alpha : 8; // [27:20]
        REG32 rotation : 2; // [29:28]
        REG32 depth : 2; // [31:30]
    } b;
} REG_GOUDA_GD_VL_BLEND_OPT_T;

//gd_vl_y_src
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 addr : 30; // [31:2]
    } b;
} REG_GOUDA_GD_VL_Y_SRC_T;

//gd_vl_u_src
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 addr : 30; // [31:2]
    } b;
} REG_GOUDA_GD_VL_U_SRC_T;

//gd_vl_v_src
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 addr : 30; // [31:2]
    } b;
} REG_GOUDA_GD_VL_V_SRC_T;

//gd_vl_resc_ratio
typedef union {
    REG32 v;
    struct {
        REG32 xpitch : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 ypitch : 11; // [26:16]
        REG32 __28_27 : 2;
        REG32 pre_fetch_en : 1; // [29]
        REG32 iy_dctenable : 1; // [30]
        REG32 ypitch_scale_enable : 1; // [31]
    } b;
} REG_GOUDA_GD_VL_RESC_RATIO_T;

//gd_ol_input_fmt
typedef union {
    REG32 v;
    struct {
        REG32 format : 2; // [1:0]
        REG32 stride : 13; // [14:2]
        REG32 __17_15 : 3;
        REG32 prefetch : 1; // [18]
        REG32 __30_19 : 12;
        REG32 active : 1; // [31]
    } b;
} REG_GOUDA_GD_OL_INPUT_FMT_T;

//gd_ol_tl_ppos
typedef union {
    REG32 v;
    struct {
        REG32 x0 : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 y0 : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_GOUDA_GD_OL_TL_PPOS_T;

//gd_ol_br_ppos
typedef union {
    REG32 v;
    struct {
        REG32 x1 : 11; // [10:0]
        REG32 __15_11 : 5;
        REG32 y1 : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_GOUDA_GD_OL_BR_PPOS_T;

//gd_ol_blend_opt
typedef union {
    REG32 v;
    struct {
        REG32 chroma_key_b : 5; // [4:0]
        REG32 chroma_key_g : 6; // [10:5]
        REG32 chroma_key_r : 5; // [15:11]
        REG32 chroma_key_enable : 1; // [16]
        REG32 chroma_key_mask : 3; // [19:17]
        REG32 alpha : 8; // [27:20]
        REG32 __31_28 : 4;
    } b;
} REG_GOUDA_GD_OL_BLEND_OPT_T;

//gd_ol_rgb_src
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 addr : 30; // [31:2]
    } b;
} REG_GOUDA_GD_OL_RGB_SRC_T;

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
        REG32 __15_13 : 3;
        REG32 nb_command : 6; // [21:16]
        REG32 __23_22 : 2;
        REG32 start_command : 1; // [24]
        REG32 lcd_resetb : 1; // [25]
        REG32 __31_26 : 6;
    } b;
} REG_GOUDA_GD_LCD_CTRL_T;

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
} REG_GOUDA_GD_LCD_TIMING_T;

//gd_lcd_mem_address
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 addr_dst : 30; // [31:2]
    } b;
} REG_GOUDA_GD_LCD_MEM_ADDRESS_T;

//gd_lcd_stride_offset
typedef union {
    REG32 v;
    struct {
        REG32 stride_offset : 10; // [9:0]
        REG32 __31_10 : 22;
    } b;
} REG_GOUDA_GD_LCD_STRIDE_OFFSET_T;

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
} REG_GOUDA_GD_LCD_SINGLE_ACCESS_T;

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
} REG_GOUDA_GD_SPILCD_CONFIG_T;

//gd_vl_fix_ratio
typedef union {
    REG32 v;
    struct {
        REG32 l_xratio : 8; // [7:0]
        REG32 l_yratio : 8; // [15:8]
        REG32 l_xfixen : 1; // [16]
        REG32 l_yfixen : 1; // [17]
        REG32 mirror : 1; // [18]
        REG32 reg_vl_only_sel : 1; // [19]
        REG32 __31_20 : 12;
    } b;
} REG_GOUDA_GD_VL_FIX_RATIO_T;

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
} REG_GOUDA_TECON_T;

//TECON2
typedef union {
    REG32 v;
    struct {
        REG32 te_count1 : 29; // [28:0]
        REG32 __31_29 : 3;
    } b;
} REG_GOUDA_TECON2_T;


//gd_command
#define GOUDA_START                (1<<0)
#define GOUDA_START_MASK           (1<<0)
#define GOUDA_START_SHIFT          (0)

//gd_status
#define GOUDA_IA_BUSY              (1<<0)
#define GOUDA_IA_BUSY_MASK         (1<<0)
#define GOUDA_IA_BUSY_SHIFT        (0)
#define GOUDA_LCD_BUSY             (1<<4)
#define GOUDA_LCD_BUSY_MASK        (1<<4)
#define GOUDA_LCD_BUSY_SHIFT       (4)

//gd_eof_irq
#define GOUDA_EOF_CAUSE            (1<<0)
#define GOUDA_EOF_CAUSE_MASK       (1<<0)
#define GOUDA_EOF_CAUSE_SHIFT      (0)
#define GOUDA_EOF_STATUS           (1<<16)
#define GOUDA_EOF_STATUS_MASK      (1<<16)
#define GOUDA_EOF_STATUS_SHIFT     (16)

//gd_eof_irq_mask
#define GOUDA_EOF_MASK             (1<<0)
#define GOUDA_EOF_MASK_MASK        (1<<0)
#define GOUDA_EOF_MASK_SHIFT       (0)

//gd_roi_tl_ppos
#define GOUDA_X0(n)                (((n)&0x7FF)<<0)
#define GOUDA_X0_MASK              (0x7FF<<0)
#define GOUDA_X0_SHIFT             (0)
#define GOUDA_Y0(n)                (((n)&0x7FF)<<16)
#define GOUDA_Y0_MASK              (0x7FF<<16)
#define GOUDA_Y0_SHIFT             (16)

//gd_roi_br_ppos
#define GOUDA_X1(n)                (((n)&0x7FF)<<0)
#define GOUDA_X1_MASK              (0x7FF<<0)
#define GOUDA_X1_SHIFT             (0)
#define GOUDA_Y1(n)                (((n)&0x7FF)<<16)
#define GOUDA_Y1_MASK              (0x7FF<<16)
#define GOUDA_Y1_SHIFT             (16)

//gd_roi_bg_color
#define GOUDA_B(n)                 (((n)&31)<<0)
#define GOUDA_B_MASK               (31<<0)
#define GOUDA_B_SHIFT              (0)
#define GOUDA_G(n)                 (((n)&0x3F)<<5)
#define GOUDA_G_MASK               (0x3F<<5)
#define GOUDA_G_SHIFT              (5)
#define GOUDA_R(n)                 (((n)&31)<<11)
#define GOUDA_R_MASK               (31<<11)
#define GOUDA_R_SHIFT              (11)

//gd_vl_input_fmt
#define GOUDA_FORMAT(n)            (((n)&3)<<0)
#define GOUDA_FORMAT_MASK          (3<<0)
#define GOUDA_FORMAT_SHIFT         (0)
#define GOUDA_STRIDE(n)            (((n)&0x1FFF)<<2)
#define GOUDA_STRIDE_MASK          (0x1FFF<<2)
#define GOUDA_STRIDE_SHIFT         (2)
#define GOUDA_ACTIVE               ((uint32)1<<31)
#define GOUDA_ACTIVE_MASK          (1<<31)
#define GOUDA_ACTIVE_SHIFT         (31)

//gd_vl_tl_ppos
#define GOUDA_X0(n)              (((n)&0x7FF)<<0)
//#define GOUDA_X0_MASK            (0x7FF<<0)
//#define GOUDA_X0_SHIFT           (0)
#define GOUDA_Y0(n)              (((n)&0x7FF)<<16)
//#define GOUDA_Y0_MASK            (0x7FF<<16)
//#define GOUDA_Y0_SHIFT           (16)

//gd_vl_br_ppos
#define GOUDA_X1(n)              (((n)&0x7FF)<<0)
//#define GOUDA_X1_MASK            (0x7FF<<0)
//#define GOUDA_X1_SHIFT           (0)
#define GOUDA_Y1(n)              (((n)&0x7FF)<<16)
//#define GOUDA_Y1_MASK            (0x7FF<<16)
//#define GOUDA_Y1_SHIFT           (16)

//gd_vl_extents
#define GOUDA_MAX_LINE(n)          (((n)&0x7FF)<<0)
#define GOUDA_MAX_LINE_MASK        (0x7FF<<0)
#define GOUDA_MAX_LINE_SHIFT       (0)
#define GOUDA_MAX_COL(n)           (((n)&0x7FF)<<16)
#define GOUDA_MAX_COL_MASK         (0x7FF<<16)
#define GOUDA_MAX_COL_SHIFT        (16)

//gd_vl_blend_opt
#define GOUDA_CHROMA_KEY_B(n)      (((n)&31)<<0)
#define GOUDA_CHROMA_KEY_B_MASK    (31<<0)
#define GOUDA_CHROMA_KEY_B_SHIFT   (0)
#define GOUDA_CHROMA_KEY_G(n)      (((n)&0x3F)<<5)
#define GOUDA_CHROMA_KEY_G_MASK    (0x3F<<5)
#define GOUDA_CHROMA_KEY_G_SHIFT   (5)
#define GOUDA_CHROMA_KEY_R(n)      (((n)&31)<<11)
#define GOUDA_CHROMA_KEY_R_MASK    (31<<11)
#define GOUDA_CHROMA_KEY_R_SHIFT   (11)
#define GOUDA_CHROMA_KEY_ENABLE    (1<<16)
#define GOUDA_CHROMA_KEY_ENABLE_MASK (1<<16)
#define GOUDA_CHROMA_KEY_ENABLE_SHIFT (16)
#define GOUDA_CHROMA_KEY_MASK(n)   (((n)&7)<<17)
#define GOUDA_CHROMA_KEY_MASK_MASK (7<<17)
#define GOUDA_CHROMA_KEY_MASK_SHIFT (17)
#define GOUDA_ALPHA(n)             (((n)&0xFF)<<20)
#define GOUDA_ALPHA_MASK           (0xFF<<20)
#define GOUDA_ALPHA_SHIFT          (20)
#define GOUDA_ROTATION(n)          (((n)&3)<<28)
#define GOUDA_ROTATION_MASK        (3<<28)
#define GOUDA_ROTATION_SHIFT       (28)
#define GOUDA_DEPTH(n)             (((n)&3)<<30)
#define GOUDA_DEPTH_MASK           (3<<30)
#define GOUDA_DEPTH_SHIFT          (30)
#define GOUDA_CHROMA_KEY_COLOR(n)  (((n)&0xFFFF)<<0)
#define GOUDA_CHROMA_KEY_COLOR_MASK (0xFFFF<<0)
#define GOUDA_CHROMA_KEY_COLOR_SHIFT (0)

//gd_vl_y_src
#define GOUDA_ADDR(n)              (((n)&0x3FFFFFFF)<<2)
#define GOUDA_ADDR_MASK            (0x3FFFFFFF<<2)
#define GOUDA_ADDR_SHIFT           (2)

//gd_vl_u_src
#define GOUDA_ADDR(n)            (((n)&0x3FFFFFFF)<<2)
//#define GOUDA_ADDR_MASK          (0x3FFFFFFF<<2)
//#define GOUDA_ADDR_SHIFT         (2)

//gd_vl_v_src
#define GOUDA_ADDR(n)            (((n)&0x3FFFFFFF)<<2)
//#define GOUDA_ADDR_MASK          (0x3FFFFFFF<<2)
//#define GOUDA_ADDR_SHIFT         (2)

//gd_vl_resc_ratio
#define GOUDA_XPITCH(n)            (((n)&0x7FF)<<0)
#define GOUDA_XPITCH_MASK          (0x7FF<<0)
#define GOUDA_XPITCH_SHIFT         (0)
#define GOUDA_YPITCH(n)            (((n)&0x7FF)<<16)
#define GOUDA_YPITCH_MASK          (0x7FF<<16)
#define GOUDA_YPITCH_SHIFT         (16)
#define GOUDA_PRE_FETCH_EN         (1<<29)
#define GOUDA_PRE_FETCH_EN_MASK    (1<<29)
#define GOUDA_PRE_FETCH_EN_SHIFT   (29)
#define GOUDA_IY_DCTENABLE         (1<<30)
#define GOUDA_IY_DCTENABLE_MASK    (1<<30)
#define GOUDA_IY_DCTENABLE_SHIFT   (30)
#define GOUDA_YPITCH_SCALE_ENABLE  ((uint32)1<<31)
#define GOUDA_YPITCH_SCALE_ENABLE_MASK (1<<31)
#define GOUDA_YPITCH_SCALE_ENABLE_SHIFT (31)

//gd_ol_input_fmt
#define GOUDA_FORMAT(n)          (((n)&3)<<0)
//#define GOUDA_FORMAT_MASK        (3<<0)
//#define GOUDA_FORMAT_SHIFT       (0)
#define GOUDA_STRIDE(n)          (((n)&0x1FFF)<<2)
//#define GOUDA_STRIDE_MASK        (0x1FFF<<2)
//#define GOUDA_STRIDE_SHIFT       (2)
#define GOUDA_PREFETCH             (1<<18)
#define GOUDA_PREFETCH_MASK        (1<<18)
#define GOUDA_PREFETCH_SHIFT       (18)
#define GOUDA_ACTIVE             (1<<31)
//#define GOUDA_ACTIVE_MASK        (1<<31)
//#define GOUDA_ACTIVE_SHIFT       (31)

//gd_ol_tl_ppos
#define GOUDA_X0(n)              (((n)&0x7FF)<<0)
//#define GOUDA_X0_MASK            (0x7FF<<0)
//#define GOUDA_X0_SHIFT           (0)
#define GOUDA_Y0(n)              (((n)&0x7FF)<<16)
//#define GOUDA_Y0_MASK            (0x7FF<<16)
//#define GOUDA_Y0_SHIFT           (16)

//gd_ol_br_ppos
#define GOUDA_X1(n)              (((n)&0x7FF)<<0)
//#define GOUDA_X1_MASK            (0x7FF<<0)
//#define GOUDA_X1_SHIFT           (0)
#define GOUDA_Y1(n)              (((n)&0x7FF)<<16)
//#define GOUDA_Y1_MASK            (0x7FF<<16)
//#define GOUDA_Y1_SHIFT           (16)

//gd_ol_blend_opt
#define GOUDA_CHROMA_KEY_B(n)    (((n)&31)<<0)
//#define GOUDA_CHROMA_KEY_B_MASK  (31<<0)
//#define GOUDA_CHROMA_KEY_B_SHIFT (0)
#define GOUDA_CHROMA_KEY_G(n)    (((n)&0x3F)<<5)
//#define GOUDA_CHROMA_KEY_G_MASK  (0x3F<<5)
//#define GOUDA_CHROMA_KEY_G_SHIFT (5)
#define GOUDA_CHROMA_KEY_R(n)    (((n)&31)<<11)
//#define GOUDA_CHROMA_KEY_R_MASK  (31<<11)
//#define GOUDA_CHROMA_KEY_R_SHIFT (11)
#define GOUDA_CHROMA_KEY_ENABLE  (1<<16)
//#define GOUDA_CHROMA_KEY_ENABLE_MASK (1<<16)
//#define GOUDA_CHROMA_KEY_ENABLE_SHIFT (16)
#define GOUDA_CHROMA_KEY_MASK(n) (((n)&7)<<17)
//#define GOUDA_CHROMA_KEY_MASK_MASK (7<<17)
//#define GOUDA_CHROMA_KEY_MASK_SHIFT (17)
#define GOUDA_ALPHA(n)           (((n)&0xFF)<<20)
//#define GOUDA_ALPHA_MASK         (0xFF<<20)
//#define GOUDA_ALPHA_SHIFT        (20)
#define GOUDA_CHROMA_KEY_COLOR(n) (((n)&0xFFFF)<<0)
//#define GOUDA_CHROMA_KEY_COLOR_MASK (0xFFFF<<0)
//#define GOUDA_CHROMA_KEY_COLOR_SHIFT (0)

//gd_ol_rgb_src
#define GOUDA_ADDR(n)            (((n)&0x3FFFFFFF)<<2)
//#define GOUDA_ADDR_MASK          (0x3FFFFFFF<<2)
//#define GOUDA_ADDR_SHIFT         (2)

//gd_lcd_ctrl
#define GOUDA_DESTINATION(n)       (((n)&3)<<0)
#define GOUDA_DESTINATION_MASK     (3<<0)
#define GOUDA_DESTINATION_SHIFT    (0)
#define GOUDA_DESTINATION_LCD_CS_0 (0<<0)
#define GOUDA_DESTINATION_LCD_CS_1 (1<<0)
#define GOUDA_DESTINATION_MEMORY_LCD_TYPE (2<<0)
#define GOUDA_DESTINATION_MEMORY_RAM (3<<0)
#define GOUDA_DESTINATION_V_LCD_CS_0 (0)
#define GOUDA_DESTINATION_V_LCD_CS_1 (1)
#define GOUDA_DESTINATION_V_MEMORY_LCD_TYPE (2)
#define GOUDA_DESTINATION_V_MEMORY_RAM (3)
#define GOUDA_OUTPUT_FORMAT(n)     (((n)&7)<<4)
#define GOUDA_OUTPUT_FORMAT_MASK   (7<<4)
#define GOUDA_OUTPUT_FORMAT_SHIFT  (4)
#define GOUDA_OUTPUT_FORMAT_8_BIT_RGB332 (0<<4)
#define GOUDA_OUTPUT_FORMAT_8_BIT_RGB444 (1<<4)
#define GOUDA_OUTPUT_FORMAT_8_BIT_RGB565 (2<<4)
#define GOUDA_OUTPUT_FORMAT_16_BIT_RGB332 (4<<4)
#define GOUDA_OUTPUT_FORMAT_16_BIT_RGB444 (5<<4)
#define GOUDA_OUTPUT_FORMAT_16_BIT_RGB565 (6<<4)
#define GOUDA_OUTPUT_FORMAT_V_8_BIT_RGB332 (0)
#define GOUDA_OUTPUT_FORMAT_V_8_BIT_RGB444 (1)
#define GOUDA_OUTPUT_FORMAT_V_8_BIT_RGB565 (2)
#define GOUDA_OUTPUT_FORMAT_V_16_BIT_RGB332 (4)
#define GOUDA_OUTPUT_FORMAT_V_16_BIT_RGB444 (5)
#define GOUDA_OUTPUT_FORMAT_V_16_BIT_RGB565 (6)
#define GOUDA_HIGH_BYTE            (1<<7)
#define GOUDA_HIGH_BYTE_MASK       (1<<7)
#define GOUDA_HIGH_BYTE_SHIFT      (7)
#define GOUDA_CS0_POLARITY         (1<<8)
#define GOUDA_CS0_POLARITY_MASK    (1<<8)
#define GOUDA_CS0_POLARITY_SHIFT   (8)
#define GOUDA_CS1_POLARITY         (1<<9)
#define GOUDA_CS1_POLARITY_MASK    (1<<9)
#define GOUDA_CS1_POLARITY_SHIFT   (9)
#define GOUDA_RS_POLARITY          (1<<10)
#define GOUDA_RS_POLARITY_MASK     (1<<10)
#define GOUDA_RS_POLARITY_SHIFT    (10)
#define GOUDA_WR_POLARITY          (1<<11)
#define GOUDA_WR_POLARITY_MASK     (1<<11)
#define GOUDA_WR_POLARITY_SHIFT    (11)
#define GOUDA_RD_POLARITY          (1<<12)
#define GOUDA_RD_POLARITY_MASK     (1<<12)
#define GOUDA_RD_POLARITY_SHIFT    (12)
#define GOUDA_NB_COMMAND(n)        (((n)&0x3F)<<16)
#define GOUDA_NB_COMMAND_MASK      (0x3F<<16)
#define GOUDA_NB_COMMAND_SHIFT     (16)
#define GOUDA_START_COMMAND        (1<<24)
#define GOUDA_START_COMMAND_MASK   (1<<24)
#define GOUDA_START_COMMAND_SHIFT  (24)
#define GOUDA_LCD_RESETB           (1<<25)
#define GOUDA_LCD_RESETB_MASK      (1<<25)
#define GOUDA_LCD_RESETB_SHIFT     (25)

//gd_lcd_timing
#define GOUDA_TAS(n)               (((n)&7)<<0)
#define GOUDA_TAS_MASK             (7<<0)
#define GOUDA_TAS_SHIFT            (0)
#define GOUDA_TAH(n)               (((n)&7)<<4)
#define GOUDA_TAH_MASK             (7<<4)
#define GOUDA_TAH_SHIFT            (4)
#define GOUDA_PWL(n)               (((n)&0x3F)<<8)
#define GOUDA_PWL_MASK             (0x3F<<8)
#define GOUDA_PWL_SHIFT            (8)
#define GOUDA_PWH(n)               (((n)&0x3F)<<16)
#define GOUDA_PWH_MASK             (0x3F<<16)
#define GOUDA_PWH_SHIFT            (16)

//gd_lcd_mem_address
#define GOUDA_ADDR_DST(n)          (((n)&0x3FFFFFFF)<<2)
#define GOUDA_ADDR_DST_MASK        (0x3FFFFFFF<<2)
#define GOUDA_ADDR_DST_SHIFT       (2)

//gd_lcd_stride_offset
#define GOUDA_STRIDE_OFFSET(n)     (((n)&0x3FF)<<0)
#define GOUDA_STRIDE_OFFSET_MASK   (0x3FF<<0)
#define GOUDA_STRIDE_OFFSET_SHIFT  (0)

//gd_lcd_single_access
#define GOUDA_LCD_DATA(n)          (((n)&0xFFFF)<<0)
#define GOUDA_LCD_DATA_MASK        (0xFFFF<<0)
#define GOUDA_LCD_DATA_SHIFT       (0)
#define GOUDA_TYPE                 (1<<16)
#define GOUDA_TYPE_MASK            (1<<16)
#define GOUDA_TYPE_SHIFT           (16)
#define GOUDA_START_WRITE          (1<<17)
#define GOUDA_START_WRITE_MASK     (1<<17)
#define GOUDA_START_WRITE_SHIFT    (17)
#define GOUDA_START_READ           (1<<18)
#define GOUDA_START_READ_MASK      (1<<18)
#define GOUDA_START_READ_SHIFT     (18)

//gd_spilcd_config
#define GOUDA_SPI_LCD_SELECT       (1<<0)
#define GOUDA_SPI_LCD_SELECT_MASK  (1<<0)
#define GOUDA_SPI_LCD_SELECT_SHIFT (0)
#define GOUDA_SPI_DEVICE_ID(n)     (((n)&0x3F)<<1)
#define GOUDA_SPI_DEVICE_ID_MASK   (0x3F<<1)
#define GOUDA_SPI_DEVICE_ID_SHIFT  (1)
#define GOUDA_SPI_CLK_DIVIDER(n)   (((n)&0xFF)<<7)
#define GOUDA_SPI_CLK_DIVIDER_MASK (0xFF<<7)
#define GOUDA_SPI_CLK_DIVIDER_SHIFT (7)
#define GOUDA_SPI_DUMMY_CYCLE(n)   (((n)&7)<<15)
#define GOUDA_SPI_DUMMY_CYCLE_MASK (7<<15)
#define GOUDA_SPI_DUMMY_CYCLE_SHIFT (15)
#define GOUDA_SPI_LINE(n)          (((n)&3)<<18)
#define GOUDA_SPI_LINE_MASK        (3<<18)
#define GOUDA_SPI_LINE_SHIFT       (18)
#define GOUDA_SPI_LINE_4           (0<<18)
#define GOUDA_SPI_LINE_3           (1<<18)
#define GOUDA_SPI_LINE_4_START_BYTE (2<<18)
#define GOUDA_SPI_LINE_3_TWO_LANE  (3<<18)
#define GOUDA_SPI_LINE_V_4         (0)
#define GOUDA_SPI_LINE_V_3         (1)
#define GOUDA_SPI_LINE_V_4_START_BYTE (2)
#define GOUDA_SPI_LINE_V_3_TWO_LANE (3)
#define GOUDA_SPI_RX_BYTE(n)       (((n)&7)<<20)
#define GOUDA_SPI_RX_BYTE_MASK     (7<<20)
#define GOUDA_SPI_RX_BYTE_SHIFT    (20)
#define GOUDA_SPI_RW               (1<<23)
#define GOUDA_SPI_RW_MASK          (1<<23)
#define GOUDA_SPI_RW_SHIFT         (23)
#define GOUDA_SPI_RW_WRITE         (0<<23)
#define GOUDA_SPI_RW_READ          (1<<23)
#define GOUDA_SPI_RW_V_WRITE       (0)
#define GOUDA_SPI_RW_V_READ        (1)

//gd_spilcd_rd
#define GOUDA_SPI_LCD_RD(n)        (((n)&0xFFFFFFFF)<<0)
#define GOUDA_SPI_LCD_RD_MASK      (0xFFFFFFFF<<0)
#define GOUDA_SPI_LCD_RD_SHIFT     (0)

//gd_vl_fix_ratio
#define GOUDA_REG_VL_ONLY_SEL      (1<<19)
#define GOUDA_REG_VL_ONLY_SEL_MASK (1<<19)
#define GOUDA_REG_VL_ONLY_SEL_SHIFT (19)
#define GOUDA_MIRROR               (1<<18)
#define GOUDA_MIRROR_MASK          (1<<18)
#define GOUDA_MIRROR_SHIFT         (18)
#define GOUDA_L_YFIXEN             (1<<17)
#define GOUDA_L_YFIXEN_MASK        (1<<17)
#define GOUDA_L_YFIXEN_SHIFT       (17)
#define GOUDA_L_XFIXEN             (1<<16)
#define GOUDA_L_XFIXEN_MASK        (1<<16)
#define GOUDA_L_XFIXEN_SHIFT       (16)
#define GOUDA_L_YRATIO(n)          (((n)&0xFF)<<8)
#define GOUDA_L_YRATIO_MASK        (0xFF<<8)
#define GOUDA_L_YRATIO_SHIFT       (8)
#define GOUDA_L_XRATIO(n)          (((n)&0xFF)<<0)
#define GOUDA_L_XRATIO_MASK        (0xFF<<0)
#define GOUDA_L_XRATIO_SHIFT       (0)

//TECON
#define GOUDA_TE_COUNT2(n)         (((n)&0xFFF)<<16)
#define GOUDA_TE_COUNT2_MASK       (0xFFF<<16)
#define GOUDA_TE_COUNT2_SHIFT      (16)
#define GOUDA_TE_MODE              (1<<2)
#define GOUDA_TE_MODE_MASK         (1<<2)
#define GOUDA_TE_MODE_SHIFT        (2)
#define GOUDA_TE_EDGE_SEL          (1<<1)
#define GOUDA_TE_EDGE_SEL_MASK     (1<<1)
#define GOUDA_TE_EDGE_SEL_SHIFT    (1)
#define GOUDA_TE_EN                (1<<0)
#define GOUDA_TE_EN_MASK           (1<<0)
#define GOUDA_TE_EN_SHIFT          (0)

//TECON2
#define GOUDA_TE_COUNT1(n)         (((n)&0x1FFFFFFF)<<0)
#define GOUDA_TE_COUNT1_MASK       (0x1FFFFFFF<<0)
#define GOUDA_TE_COUNT1_SHIFT      (0)



// ============================================================================
// GOUDA_SRAM_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_GOUDA_SRAM_BASE        0x02080000

typedef volatile struct
{
    unsigned char sram_array[11840];                    //0x00000000
} HWP_GOUDA_SRAM_T;

#define hwp_goudaSram              ((HWP_GOUDA_SRAM_T*) REG_ACCESS_ADDRESS(REG_GOUDA_SRAM_BASE))







#endif

