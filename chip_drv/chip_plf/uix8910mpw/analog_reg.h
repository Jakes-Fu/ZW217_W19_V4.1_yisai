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


#ifndef _ANALOG_REG_H_
#define _ANALOG_REG_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'analog_reg'."
#endif

#include "global_macros.h"
#include "uix8910_module_ver_extern.h"

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// ANALOG_REG_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_ANALOG_REG_BASE        0x05109000
#else
#define REG_ANALOG_REG_BASE        0x50109000
#endif

typedef volatile struct
{
    REG32                          APLL_reg0;                    //0x00000000
    REG32                          APLL_reg1;                    //0x00000004
    REG32                          APLL_reg2;                    //0x00000008
    REG32                          SDM_APLL_reg0;                //0x0000000C
    REG32                          SDM_APLL_reg1;                //0x00000010
    REG32                          SDM_APLL_reg2;                //0x00000014
    REG32                          SDM_APLL_reg3;                //0x00000018
    REG32 Reserved_0000001C[9];                 //0x0000001C
    REG32                          MEMPLL_reg0;                  //0x00000040
    REG32                          MEMPLL_reg1;                  //0x00000044
    REG32                          MEMPLL_reg2;                  //0x00000048
    REG32                          SDM_MEMPLL_reg0;              //0x0000004C
    REG32                          SDM_MEMPLL_reg1;              //0x00000050
    REG32                          SDM_MEMPLL_reg2;              //0x00000054
    REG32                          SDM_MEMPLL_reg3;              //0x00000058
    REG32 Reserved_0000005C[9];                 //0x0000005C
    REG32                          usb_reg0;                     //0x00000080
    REG32                          usb_reg1;                     //0x00000084
    REG32                          usb_reg2;                     //0x00000088
    REG32                          usb_reg3;                     //0x0000008C
    REG32                          usb_reg4;                     //0x00000090
    REG32                          usb_pll1;                     //0x00000094
    REG32                          usb_pll2;                     //0x00000098
    REG32                          usb_suspend;                  //0x0000009C
    REG32                          usb11_reg;                    //0x000000A0
    REG32                          usb_mon;                      //0x000000A4
    REG32                          usb_reserved;                 //0x000000A8
    REG32                          SDM_USBPLL_reg0;              //0x000000AC
    REG32                          SDM_USBPLL_reg1;              //0x000000B0
    REG32                          SDM_USBPLL_reg2;              //0x000000B4
    REG32                          SDM_USBPLL_reg3;              //0x000000B8
    REG32                          USB_DIGPHY_ANA1;              //0x000000BC
    REG32                          USB_DIGPHY_ANA2;              //0x000000C0
    REG32                          USB_DIGPHY_ANA3;              //0x000000C4
    REG32                          USB_DIGPHY_ANA4;              //0x000000C8
    REG32 Reserved_000000CC[13];                //0x000000CC
    REG32                          ddr_rpull_cfg;                //0x00000100
    REG32                          ddr_drv_cfg;                  //0x00000104
    REG32                          ddr_pad_cfg;                  //0x00000108
    REG32                          ddr_vref_cfg;                 //0x0000010C
    REG32                          ddr_zq_cfg;                   //0x00000110
    REG32                          ddr_zq_mon;                   //0x00000114
    REG32                          ddr_reserved;                 //0x00000118
    REG32 Reserved_0000011C[9];                 //0x0000011C
    REG32                          psram_drv_cfg;                //0x00000140
    REG32                          psram_pad_en_cfg;             //0x00000144
    REG32                          psram_pull_cfg;               //0x00000148
    REG32                          psram_reserved;               //0x0000014C
    REG32 Reserved_00000150[12];                //0x00000150
    REG32                          SDM_DSIPLL_reg0;              //0x00000180
    REG32                          SDM_DSIPLL_reg1;              //0x00000184
    REG32                          SDM_DSIPLL_reg2;              //0x00000188
    REG32                          SDM_DSIPLL_reg3;              //0x0000018C
    REG32 Reserved_00000190[12];                //0x00000190
    REG32                          pad_ctrl_resv;                //0x000001C0
    REG32                          pad_OSC_32K_cfg;              //0x000001C4
    REG32                          pad_RESETB_cfg;               //0x000001C8
    REG32                          pad_adi_cfg;                  //0x000001CC
    REG32                          pad_ap_jtag_cfg;              //0x000001D0
    REG32                          pad_camera_data_cfg1;         //0x000001D4
    REG32                          pad_camera_data_cfg2;         //0x000001D8
    REG32                          pad_camera_sync_cfg;          //0x000001DC
    REG32                          pad_camera_others_cfg;        //0x000001E0
    REG32                          pad_chip_pd_cfg;              //0x000001E4
    REG32                          pad_codec_mclk_cfg;           //0x000001E8
    REG32                          pad_debug_host_cfg;           //0x000001EC
    REG32                          pad_gpio_cfg1;                //0x000001F0
    REG32                          pad_gpio_cfg2;                //0x000001F4
    REG32                          pad_gpio_cfg3;                //0x000001F8
    REG32                          pad_gpo_cfg;                  //0x000001FC
    REG32                          pad_i2c_m1_cfg;               //0x00000200
    REG32                          pad_i2s1_cfg;                 //0x00000204
    REG32                          pad_i_keyon_cfg;              //0x00000208
    REG32                          pad_keyin_cfg;                //0x0000020C
    REG32                          pad_keyout_cfg;               //0x00000210
    REG32                          pad_lcd_cfg;                  //0x00000214
    REG32                          pad_lte_clk_cfg;              //0x00000218
    REG32                          pad_lte_rbdp_rx_cfg1;         //0x0000021C
    REG32                          pad_lte_rbdp_rx_cfg2;         //0x00000220
    REG32                          pad_lte_rbdp_tx_cfg1;         //0x00000224
    REG32                          pad_lte_rbdp_tx_cfg2;         //0x00000228
    REG32                          pad_lte_reset_cfg;            //0x0000022C
    REG32                          pad_lte_rfspi_cfg;            //0x00000230
    REG32                          pad_lte_frame_cfg;            //0x00000234
    REG32                          pad_lte_data_cfg;             //0x00000238
    REG32                          pad_lte_sysclken_cfg;         //0x0000023C
    REG32                          pad_pwm_cfg;                  //0x00000240
    REG32                          pad_rf_gge_cfg;               //0x00000244
    REG32                          pad_rfdig_gpio_cfg1;          //0x00000248
    REG32                          pad_rfdig_gpio_cfg2;          //0x0000024C
    REG32                          pad_sdmmc1_others_cfg;        //0x00000250
    REG32                          pad_sdmmc1_data_cfg;          //0x00000254
    REG32                          pad_sim_1_cfg;                //0x00000258
    REG32                          pad_sim_2_cfg;                //0x0000025C
    REG32                          pad_spi_flash_cfg1;           //0x00000260
    REG32                          pad_spi_flash_cfg2;           //0x00000264
    REG32                          pad_spi_lcd_cfg1;             //0x00000268
    REG32                          pad_spi_lcd_cfg2;             //0x0000026C
    REG32                          pad_tst_cfg;                  //0x00000270
    REG32                          pad_uart_1_cfg;               //0x00000274
    REG32                          pad_uart_2_cfg;               //0x00000278
    REG32 Reserved_0000027C;                    //0x0000027C
    REG32                          resv0;                        //0x00000280
    REG32                          resv1;                        //0x00000284
    REG32                          resv2;                        //0x00000288
    REG32                          resv3;                        //0x0000028C
    REG32                          resv4;                        //0x00000290
    REG32                          resv5;                        //0x00000294
    REG32                          resv6;                        //0x00000298
    REG32                          resv7;                        //0x0000029C
    REG32                          resv8;                        //0x000002A0
    REG32                          resv9;                        //0x000002A4
    REG32                          resv10;                       //0x000002A8
    REG32                          resv11;                       //0x000002AC
    REG32                          resv12;                       //0x000002B0
    REG32                          resv13;                       //0x000002B4
    REG32                          resv14;                       //0x000002B8
    REG32                          resv15;                       //0x000002BC
} HWP_ANALOG_REG_T;

#define hwp_analogReg              ((HWP_ANALOG_REG_T*) REG_ACCESS_ADDRESS(REG_ANALOG_REG_BASE))


//APLL_reg0
#define ANALOG_REG_APLL_REG0_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define ANALOG_REG_APLL_LOCK       (1<<2)
#define ANALOG_REG_APLL_LP_MODE_EN (1<<1)
#define ANALOG_REG_APLL_CLK_AP_EN  (1<<0)

//APLL_reg1
#define ANALOG_REG_APLL_REG1_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_APLL_PFD_DLY_NUM(n) (((n)&7)<<17)
#define ANALOG_REG_APLL_VREG_BIT(n) (((n)&15)<<13)
#define ANALOG_REG_APLL_REG_RES_BIT(n) (((n)&3)<<11)
#define ANALOG_REG_APLL_CPBIAS_IBIT(n) (((n)&7)<<8)
#define ANALOG_REG_APLL_CPC2_IBIT(n) (((n)&7)<<5)
#define ANALOG_REG_APLL_CPR2_IBIT(n) (((n)&7)<<2)
#define ANALOG_REG_APLL_PCON_MODE  (1<<1)
#define ANALOG_REG_APLL_REFMULTI2_EN (1<<0)

//APLL_reg2
#define ANALOG_REG_APLL_REG2_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define ANALOG_REG_APLL_TEST_EN    (1<<3)
#define ANALOG_REG_APLL_SDM_CLK_TEST_EN (1<<2)
#define ANALOG_REG_APLL_VCO_HIGH_TEST (1<<1)
#define ANALOG_REG_APLL_VCO_LOW_TEST (1<<0)

//SDM_APLL_reg0
#define ANALOG_REG_SDM_APLL_REG0_RESERVED_0(n) (((n)&0x7FFFFF)<<9)
#define ANALOG_REG_SDM_APLL_FBC_INV (1<<8)
#define ANALOG_REG_SDM_APLL_PU     (1<<7)
#define ANALOG_REG_SDM_APLL_SDM_CLK_SEL_RST (1<<6)
#define ANALOG_REG_SDM_APLL_SDM_CLK_SEL_NOR (1<<5)
#define ANALOG_REG_SDM_APLL_PU_PLL_DR (1<<4)
#define ANALOG_REG_SDM_APLL_PU_PLL_REG (1<<3)
#define ANALOG_REG_SDM_APLL_INT_DEC_SEL(n) (((n)&7)<<0)

//SDM_APLL_reg1
#define ANALOG_REG_SDM_APLL_SDM_FREQ(n) (((n)&0xFFFFFFFF)<<0)

//SDM_APLL_reg2
#define ANALOG_REG_SDM_APLL_REG2_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define ANALOG_REG_SDM_APLL_DITHER_BYPASS (1<<11)
#define ANALOG_REG_SDM_APLL_SDM_RESETN_DR (1<<10)
#define ANALOG_REG_SDM_APLL_SDM_RESETN_REG (1<<9)
#define ANALOG_REG_SDM_APLL_SDM_RESET_TIME_SEL(n) (((n)&3)<<7)
#define ANALOG_REG_SDM_APLL_SDMCLK_SEL_TIME_SEL(n) (((n)&3)<<5)
#define ANALOG_REG_SDM_APLL_CLK_GEN_EN_REG (1<<4)
#define ANALOG_REG_SDM_APLL_CLKOUT_EN_COUNTER_SEL(n) (((n)&3)<<2)
#define ANALOG_REG_SDM_APLL_LOCK_COUNTER_SEL(n) (((n)&3)<<0)

//SDM_APLL_reg3
#define ANALOG_REG_SDM_APLL_REG3_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define ANALOG_REG_SDM_APLL_SDM_RESETN (1<<2)
#define ANALOG_REG_SDM_APLL_LOCK_STEADY (1<<1)
#define ANALOG_REG_SDM_APLL_CLK_READY (1<<0)

//MEMPLL_reg0
#define ANALOG_REG_MEMPLL_REG0_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define ANALOG_REG_MEMPLL_LOCK     (1<<2)
#define ANALOG_REG_MEMPLL_LP_MODE_EN (1<<1)
#define ANALOG_REG_MEMPLL_CLK_MEM_EN (1<<0)

//MEMPLL_reg1
#define ANALOG_REG_MEMPLL_REG1_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_MEMPLL_PFD_DLY_NUM(n) (((n)&7)<<17)
#define ANALOG_REG_MEMPLL_VREG_BIT(n) (((n)&15)<<13)
#define ANALOG_REG_MEMPLL_REG_RES_BIT(n) (((n)&3)<<11)
#define ANALOG_REG_MEMPLL_CPBIAS_IBIT(n) (((n)&7)<<8)
#define ANALOG_REG_MEMPLL_CPC2_IBIT(n) (((n)&7)<<5)
#define ANALOG_REG_MEMPLL_CPR2_IBIT(n) (((n)&7)<<2)
#define ANALOG_REG_MEMPLL_PCON_MODE (1<<1)
#define ANALOG_REG_MEMPLL_REFMULTI2_EN (1<<0)

//MEMPLL_reg2
#define ANALOG_REG_MEMPLL_REG2_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define ANALOG_REG_MEMPLL_TEST_EN  (1<<3)
#define ANALOG_REG_MEMPLL_SDM_CLK_TEST_EN (1<<2)
#define ANALOG_REG_MEMPLL_VCO_HIGH_TEST (1<<1)
#define ANALOG_REG_MEMPLL_VCO_LOW_TEST (1<<0)

//SDM_MEMPLL_reg0
#define ANALOG_REG_SDM_MEMPLL_REG0_RESERVED_0(n) (((n)&0x7FFFFF)<<9)
#define ANALOG_REG_SDM_MEMPLL_FBC_INV (1<<8)
#define ANALOG_REG_SDM_MEMPLL_PU   (1<<7)
#define ANALOG_REG_SDM_MEMPLL_SDM_CLK_SEL_RST (1<<6)
#define ANALOG_REG_SDM_MEMPLL_SDM_CLK_SEL_NOR (1<<5)
#define ANALOG_REG_SDM_MEMPLL_PU_PLL_DR (1<<4)
#define ANALOG_REG_SDM_MEMPLL_PU_PLL_REG (1<<3)
#define ANALOG_REG_SDM_MEMPLL_INT_DEC_SEL(n) (((n)&7)<<0)

//SDM_MEMPLL_reg1
#define ANALOG_REG_SDM_MEMPLL_SDM_FREQ(n) (((n)&0xFFFFFFFF)<<0)

//SDM_MEMPLL_reg2
#define ANALOG_REG_SDM_MEMPLL_REG2_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define ANALOG_REG_SDM_MEMPLL_DITHER_BYPASS (1<<11)
#define ANALOG_REG_SDM_MEMPLL_SDM_RESETN_DR (1<<10)
#define ANALOG_REG_SDM_MEMPLL_SDM_RESETN_REG (1<<9)
#define ANALOG_REG_SDM_MEMPLL_SDM_RESET_TIME_SEL(n) (((n)&3)<<7)
#define ANALOG_REG_SDM_MEMPLL_SDMCLK_SEL_TIME_SEL(n) (((n)&3)<<5)
#define ANALOG_REG_SDM_MEMPLL_CLK_GEN_EN_REG (1<<4)
#define ANALOG_REG_SDM_MEMPLL_CLKOUT_EN_COUNTER_SEL(n) (((n)&3)<<2)
#define ANALOG_REG_SDM_MEMPLL_LOCK_COUNTER_SEL(n) (((n)&3)<<0)

//SDM_MEMPLL_reg3
#define ANALOG_REG_SDM_MEMPLL_REG3_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define ANALOG_REG_SDM_MEMPLL_SDM_RESETN (1<<2)
#define ANALOG_REG_SDM_MEMPLL_LOCK_STEADY (1<<1)
#define ANALOG_REG_SDM_MEMPLL_CLK_READY (1<<0)

//usb_reg0
#define ANALOG_REG_USB_REG0_RESERVED_0(n) (((n)&0x7FFFFF)<<9)
#define ANALOG_REG_USB_CDR_CLK_EDGE_BIT (1<<8)
#define ANALOG_REG_USB_RST_INTP_ENABLE (1<<7)
#define ANALOG_REG_USB_UPDN_MODE   (1<<6)
#define ANALOG_REG_USB_PHASE_SEL(n) (((n)&3)<<4)
#define ANALOG_REG_USB_CDR_GAIN(n) (((n)&15)<<0)

//usb_reg1
#define ANALOG_REG_USB_REG1_RESERVED_0(n) (((n)&0x1FFFF)<<15)
#define ANALOG_REG_USB_VREF_IBIT(n) (((n)&3)<<13)
#define ANALOG_REG_USB_VREF_VBIT(n) (((n)&7)<<10)
#define ANALOG_REG_USB_V575M_SEL_BIT(n) (((n)&7)<<7)
#define ANALOG_REG_USB_V125M_SEL_BIT(n) (((n)&7)<<4)
#define ANALOG_REG_USB_HS_LVLOUT_BIT(n) (((n)&15)<<0)

//usb_reg2
#define ANALOG_REG_USB_REG2_RESERVED_0(n) (((n)&0x1FFFF)<<15)
#define ANALOG_REG_USB_SQUELCH_AUX (1<<14)
#define ANALOG_REG_USB_DISCNNXT_MODE_SEL (1<<13)
#define ANALOG_REG_USB_SQUELCH_MODE_SEL (1<<12)
#define ANALOG_REG_USB_VBG_SEL     (1<<11)
#define ANALOG_REG_USB_PU_OTG      (1<<10)
#define ANALOG_REG_USB_VBUSVLD_BIT (1<<9)
#define ANALOG_REG_USB_PU_USB_DEV  (1<<8)
#define ANALOG_REG_USB_PWR_ON      (1<<7)
#define ANALOG_REG_USB_PU_PKD      (1<<6)
#define ANALOG_REG_USB_PU_1V8      (1<<5)
#define ANALOG_REG_USB_PU_1V2      (1<<4)
#define ANALOG_REG_USB_PU_IREF     (1<<3)
#define ANALOG_REG_USB_PU_HSRX     (1<<2)
#define ANALOG_REG_USB_PU_HSTX     (1<<1)
#define ANALOG_REG_USB_PU_LPTX     (1<<0)

//usb_reg3
#define ANALOG_REG_USB_REG3_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define ANALOG_REG_USB_DET_EN      (1<<12)
#define ANALOG_REG_USB_LOOPBACK    (1<<11)
#define ANALOG_REG_USB_EN_PATTERN  (1<<10)
#define ANALOG_REG_USB_PATTERN(n)  (((n)&0x3FF)<<0)

//usb_reg4
#define ANALOG_REG_USB_REG4_RESERVED_0(n) (((n)&0x1FFFFF)<<11)
#define ANALOG_REG_USB_IO33_ENABLE (1<<10)
#define ANALOG_REG_USB_RES_TERM_BIT(n) (((n)&3)<<8)
#define ANALOG_REG_USB_LPTX_DRV_SEL(n) (((n)&0xFF)<<0)

//usb_pll1
#define ANALOG_REG_USB_PLL1_RESERVED_0(n) (((n)&0x1FFFF)<<15)
#define ANALOG_REG_USB_PLL_VREG_BIT(n) (((n)&15)<<11)
#define ANALOG_REG_USB_REG_RES_BIT(n) (((n)&3)<<9)
#define ANALOG_REG_USB_PLL_CPBIAS_IBIT(n) (((n)&7)<<6)
#define ANALOG_REG_USB_PLL_CPC2_IBIT(n) (((n)&7)<<3)
#define ANALOG_REG_USB_PLL_CPR2_IBIT(n) (((n)&7)<<0)

//usb_pll2
#define ANALOG_REG_USB_PLL2_RESERVED_0(n) (((n)&0x1FFFFFF)<<7)
#define ANALOG_REG_USB_PCON_MODE   (1<<6)
#define ANALOG_REG_USB_REFMULTI2_EN (1<<5)
#define ANALOG_REG_USB_PLL_TEST_EN (1<<4)
#define ANALOG_REG_USB_SDM_CLK_TEST_EN (1<<3)
#define ANALOG_REG_USB_VCO_HIGH_TEST (1<<2)
#define ANALOG_REG_USB_VCO_LOW_TEST (1<<1)
#define ANALOG_REG_USB_PLL_CLK_960M_EN (1<<0)

//usb_suspend
#define ANALOG_REG_USB_SUSPEND_RESERVED_0(n) (((n)&0x3FFFFFF)<<6)
#define ANALOG_REG_USB_PU_SUSPEND_BYPASS (1<<5)
#define ANALOG_REG_USB_PU_OTG_SUSPEND (1<<4)
#define ANALOG_REG_USB_PU_PKD_SUSPEND (1<<3)
#define ANALOG_REG_USB_PU_IREF_SUSPEND (1<<2)
#define ANALOG_REG_USB_PU_HSRX_SUSPEND (1<<1)
#define ANALOG_REG_USB_PU_HSTX_SUSPEND (1<<0)

//usb11_reg
#define ANALOG_REG_USB11_REG_RESERVED_0(n) (((n)&0x3FFFF)<<14)
#define ANALOG_REG_USB11_USB_STANDBY (1<<13)
#define ANALOG_REG_USB11_USB_CTRL(n) (((n)&7)<<10)
#define ANALOG_REG_USB11_PU_USB    (1<<9)
#define ANALOG_REG_USB11_IO_BIAS_EN (1<<8)
#define ANALOG_REG_USB11_LPTX_DRVSEL(n) (((n)&0xFF)<<0)

//usb_mon
#define ANALOG_REG_USB_MON_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define ANALOG_REG_USB_DP_CHR      (1<<4)
#define ANALOG_REG_USB_DM_CHR      (1<<3)
#define ANALOG_REG_USB_CORRECT     (1<<2)
#define ANALOG_REG_USB_LOCK        (1<<1)
#define ANALOG_REG_USB_PLL_LOCK    (1<<0)

//usb_reserved
#define ANALOG_REG_USB_RESERVED_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define ANALOG_REG_USB_REG_RESV(n) (((n)&0xFFFF)<<0)

//SDM_USBPLL_reg0
#define ANALOG_REG_SDM_USBPLL_REG0_RESERVED_0(n) (((n)&0x7FFFFF)<<9)
#define ANALOG_REG_SDM_USBPLL_FBC_INV (1<<8)
#define ANALOG_REG_SDM_USBPLL_PU   (1<<7)
#define ANALOG_REG_SDM_USBPLL_SDM_CLK_SEL_RST (1<<6)
#define ANALOG_REG_SDM_USBPLL_SDM_CLK_SEL_NOR (1<<5)
#define ANALOG_REG_SDM_USBPLL_PU_PLL_DR (1<<4)
#define ANALOG_REG_SDM_USBPLL_PU_PLL_REG (1<<3)
#define ANALOG_REG_SDM_USBPLL_INT_DEC_SEL(n) (((n)&7)<<0)

//SDM_USBPLL_reg1
#define ANALOG_REG_SDM_USBPLL_SDM_FREQ(n) (((n)&0xFFFFFFFF)<<0)

//SDM_USBPLL_reg2
#define ANALOG_REG_SDM_USBPLL_REG2_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define ANALOG_REG_SDM_USBPLL_DITHER_BYPASS (1<<11)
#define ANALOG_REG_SDM_USBPLL_SDM_RESETN_DR (1<<10)
#define ANALOG_REG_SDM_USBPLL_SDM_RESETN_REG (1<<9)
#define ANALOG_REG_SDM_USBPLL_SDM_RESET_TIME_SEL(n) (((n)&3)<<7)
#define ANALOG_REG_SDM_USBPLL_SDMCLK_SEL_TIME_SEL(n) (((n)&3)<<5)
#define ANALOG_REG_SDM_USBPLL_CLK_GEN_EN_REG (1<<4)
#define ANALOG_REG_SDM_USBPLL_CLKOUT_EN_COUNTER_SEL(n) (((n)&3)<<2)
#define ANALOG_REG_SDM_USBPLL_LOCK_COUNTER_SEL(n) (((n)&3)<<0)

//SDM_USBPLL_reg3
#define ANALOG_REG_SDM_USBPLL_REG3_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define ANALOG_REG_SDM_USBPLL_SDM_RESETN (1<<2)
#define ANALOG_REG_SDM_USBPLL_LOCK_STEADY (1<<1)
#define ANALOG_REG_SDM_USBPLL_CLK_READY (1<<0)

//USB_DIGPHY_ANA1
#define ANALOG_REG_USB_DIGPHY_ANA_1(n) (((n)&0xFFFFFFFF)<<0)

//USB_DIGPHY_ANA2
#define ANALOG_REG_USB_DIGPHY_ANA_2(n) (((n)&0xFFFFFFFF)<<0)

//USB_DIGPHY_ANA3
#define ANALOG_REG_USB_DIGPHY_ANA_3(n) (((n)&0xFFFFFFFF)<<0)

//USB_DIGPHY_ANA4
#define ANALOG_REG_USB_DIGPHY_ANA_4(n) (((n)&0xFFFFFFFF)<<0)

//ddr_rpull_cfg
#define ANALOG_REG_DDR_RPULL_CFG_RESERVED_0(n) (((n)&0xFFFFFFF)<<4)
#define ANALOG_REG_DDR_CK_RPULL(n) (((n)&3)<<2)
#define ANALOG_REG_DDR_DQS_RPULL(n) (((n)&3)<<0)

//ddr_drv_cfg
#define ANALOG_REG_DDR_DRV_CFG_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_DDR_DRVNC(n)    (((n)&31)<<15)
#define ANALOG_REG_DDR_DRVND(n)    (((n)&31)<<10)
#define ANALOG_REG_DDR_DRVPC(n)    (((n)&31)<<5)
#define ANALOG_REG_DDR_DRVPD(n)    (((n)&31)<<0)

//ddr_pad_cfg
#define ANALOG_REG_DDR_PAD_CFG_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define ANALOG_REG_DDR_LATCH       (1<<2)
#define ANALOG_REG_DDR_LP2C        (1<<1)
#define ANALOG_REG_DDR_LP2D        (1<<0)

//ddr_vref_cfg
#define ANALOG_REG_DDR_VREF_CFG_RESERVED_0(n) (((n)&0x3FFFF)<<14)
#define ANALOG_REG_DDR_SEL_INT(n)  (((n)&0x3F)<<8)
#define ANALOG_REG_DDR_SEL_PAD(n)  (((n)&0x3F)<<2)
#define ANALOG_REG_DDR_VREF_EN_INT (1<<1)
#define ANALOG_REG_DDR_VREF_EN_PAD (1<<0)

//ddr_zq_cfg
#define ANALOG_REG_DDR_ZQ_CFG_RESERVED_0(n) (((n)&0x1FFFFFF)<<7)
#define ANALOG_REG_DDR_CAL_ZQ_CAL  (1<<6)
#define ANALOG_REG_DDR_CAL_ZQ_PD   (1<<5)
#define ANALOG_REG_DDR_ZPROG(n)    (((n)&31)<<0)

//ddr_zq_mon
#define ANALOG_REG_DDR_ZQ_MON_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define ANALOG_REG_DDR_CALOVER     (1<<0)

//ddr_reserved
#define ANALOG_REG_DDR_REG_RESV(n) (((n)&0xFFFFFFFF)<<0)

//psram_drv_cfg
#define ANALOG_REG_PSRAM_DRV_CFG_RESERVED_0(n) (((n)&0x7FFFF)<<13)
#define ANALOG_REG_PSRAM_DRVN(n)   (((n)&31)<<8)
#define ANALOG_REG_PSRAM_DRVP(n)   (((n)&31)<<3)
#define ANALOG_REG_PSRAM_SLEWRATE(n) (((n)&3)<<1)
#define ANALOG_REG_PSRAM_FIX_READ0 (1<<0)

//psram_pad_en_cfg
#define ANALOG_REG_PSRAM_PAD_EN_CFG_RESERVED_0(n) (((n)&0x3FFFFFFF)<<2)
#define ANALOG_REG_PSRAM_LATCH     (1<<1)
#define ANALOG_REG_PSRAM_PAD_CLKN_EN (1<<0)

//psram_pull_cfg
#define ANALOG_REG_PSRAM_PULL_CFG_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define ANALOG_REG_PSRAM_CEN_PULL1_BIT(n) (((n)&3)<<10)
#define ANALOG_REG_PSRAM_CLK_PULL0_BIT(n) (((n)&3)<<8)
#define ANALOG_REG_PSRAM_CLKN_PULL1_BIT(n) (((n)&3)<<6)
#define ANALOG_REG_PSRAM_DM_PULL1_BIT(n) (((n)&3)<<4)
#define ANALOG_REG_PSRAM_DQ_PULL0_BIT(n) (((n)&3)<<2)
#define ANALOG_REG_PSRAM_DQS_PULL0_BIT(n) (((n)&3)<<0)

//psram_reserved
#define ANALOG_REG_PSRAM_REG_RESV(n) (((n)&0xFFFFFFFF)<<0)

//SDM_DSIPLL_reg0
#define ANALOG_REG_SDM_DSIPLL_REG0_RESERVED_0(n) (((n)&0x7FFFFF)<<9)
#define ANALOG_REG_SDM_DSIPLL_FBC_INV (1<<8)
#define ANALOG_REG_SDM_DSIPLL_PU   (1<<7)
#define ANALOG_REG_SDM_DSIPLL_SDM_CLK_SEL_RST (1<<6)
#define ANALOG_REG_SDM_DSIPLL_SDM_CLK_SEL_NOR (1<<5)
#define ANALOG_REG_SDM_DSIPLL_PU_PLL_DR (1<<4)
#define ANALOG_REG_SDM_DSIPLL_PU_PLL_REG (1<<3)
#define ANALOG_REG_SDM_DSIPLL_INT_DEC_SEL(n) (((n)&7)<<0)

//SDM_DSIPLL_reg1
#define ANALOG_REG_SDM_DSIPLL_SDM_FREQ(n) (((n)&0xFFFFFFFF)<<0)

//SDM_DSIPLL_reg2
#define ANALOG_REG_SDM_DSIPLL_REG2_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define ANALOG_REG_SDM_DSIPLL_DITHER_BYPASS (1<<11)
#define ANALOG_REG_SDM_DSIPLL_SDM_RESETN_DR (1<<10)
#define ANALOG_REG_SDM_DSIPLL_SDM_RESETN_REG (1<<9)
#define ANALOG_REG_SDM_DSIPLL_SDM_RESET_TIME_SEL(n) (((n)&3)<<7)
#define ANALOG_REG_SDM_DSIPLL_SDMCLK_SEL_TIME_SEL(n) (((n)&3)<<5)
#define ANALOG_REG_SDM_DSIPLL_CLK_GEN_EN_REG (1<<4)
#define ANALOG_REG_SDM_DSIPLL_CLKOUT_EN_COUNTER_SEL(n) (((n)&3)<<2)
#define ANALOG_REG_SDM_DSIPLL_LOCK_COUNTER_SEL(n) (((n)&3)<<0)

//SDM_DSIPLL_reg3
#define ANALOG_REG_SDM_DSIPLL_REG3_RESERVED_0(n) (((n)&0x1FFFFFFF)<<3)
#define ANALOG_REG_SDM_DSIPLL_SDM_RESETN (1<<2)
#define ANALOG_REG_SDM_DSIPLL_LOCK_STEADY (1<<1)
#define ANALOG_REG_SDM_DSIPLL_CLK_READY (1<<0)

//pad_ctrl_resv
#define ANALOG_REG_REG_PAD_CTRL_RESV(n) (((n)&0xFFFFFFFF)<<0)

//pad_OSC_32K_cfg
#define ANALOG_REG_PAD_OSC_32K_CFG_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define ANALOG_REG_PAD_OSC_32K_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_OSC_32K_IE  (1<<2)
#define ANALOG_REG_PAD_OSC_32K_SE  (1<<1)
#define ANALOG_REG_PAD_OSC_32K_WPUS (1<<0)

//pad_RESETB_cfg
#define ANALOG_REG_PAD_RESETB_CFG_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define ANALOG_REG_PAD_RESETB_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_RESETB_IE   (1<<2)
#define ANALOG_REG_PAD_RESETB_SE   (1<<1)
#define ANALOG_REG_PAD_RESETB_WPUS (1<<0)

//pad_adi_cfg
#define ANALOG_REG_PAD_ADI_CFG_RESERVED_0(n) (((n)&0x1FFFF)<<15)
#define ANALOG_REG_PAD_ADI_SCL_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_ADI_SCL_IE  (1<<12)
#define ANALOG_REG_PAD_ADI_SCL_SE  (1<<11)
#define ANALOG_REG_PAD_ADI_SCL_WPUS (1<<10)
#define ANALOG_REG_PAD_ADI_SDA_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_ADI_SDA_IE  (1<<7)
#define ANALOG_REG_PAD_ADI_SDA_SE  (1<<6)
#define ANALOG_REG_PAD_ADI_SDA_WPUS (1<<5)
#define ANALOG_REG_PAD_ADI_SYNC_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_ADI_SYNC_IE (1<<2)
#define ANALOG_REG_PAD_ADI_SYNC_SE (1<<1)
#define ANALOG_REG_PAD_ADI_SYNC_WPUS (1<<0)

//pad_ap_jtag_cfg
#define ANALOG_REG_PAD_AP_JTAG_CFG_RESERVED_0(n) (((n)&0x7F)<<25)
#define ANALOG_REG_PAD_AP_JTAG_TCK_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_AP_JTAG_TCK_IE (1<<22)
#define ANALOG_REG_PAD_AP_JTAG_TCK_SE (1<<21)
#define ANALOG_REG_PAD_AP_JTAG_TCK_WPUS (1<<20)
#define ANALOG_REG_PAD_AP_JTAG_TDI_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_AP_JTAG_TDI_IE (1<<17)
#define ANALOG_REG_PAD_AP_JTAG_TDI_SE (1<<16)
#define ANALOG_REG_PAD_AP_JTAG_TDI_WPUS (1<<15)
#define ANALOG_REG_PAD_AP_JTAG_TDO_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_AP_JTAG_TDO_IE (1<<12)
#define ANALOG_REG_PAD_AP_JTAG_TDO_SE (1<<11)
#define ANALOG_REG_PAD_AP_JTAG_TDO_WPUS (1<<10)
#define ANALOG_REG_PAD_AP_JTAG_TMS_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_AP_JTAG_TMS_IE (1<<7)
#define ANALOG_REG_PAD_AP_JTAG_TMS_SE (1<<6)
#define ANALOG_REG_PAD_AP_JTAG_TMS_WPUS (1<<5)
#define ANALOG_REG_PAD_AP_JTAG_TRST_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_AP_JTAG_TRST_IE (1<<2)
#define ANALOG_REG_PAD_AP_JTAG_TRST_SE (1<<1)
#define ANALOG_REG_PAD_AP_JTAG_TRST_WPUS (1<<0)

//pad_camera_data_cfg1
#define ANALOG_REG_PAD_CAMERA_DATA_CFG1_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_0_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_0_IE (1<<17)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_0_SE (1<<16)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_0_SPU (1<<15)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_1_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_1_IE (1<<12)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_1_SE (1<<11)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_1_SPU (1<<10)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_2_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_2_IE (1<<7)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_2_SE (1<<6)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_2_SPU (1<<5)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_3_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_3_IE (1<<2)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_3_SE (1<<1)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_3_SPU (1<<0)

//pad_camera_data_cfg2
#define ANALOG_REG_PAD_CAMERA_DATA_CFG2_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_4_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_4_IE (1<<17)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_4_SE (1<<16)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_4_SPU (1<<15)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_5_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_5_IE (1<<12)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_5_SE (1<<11)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_5_SPU (1<<10)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_6_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_6_IE (1<<7)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_6_SE (1<<6)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_6_SPU (1<<5)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_7_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_7_IE (1<<2)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_7_SE (1<<1)
#define ANALOG_REG_PAD_CAMERA_DATA_IN_7_SPU (1<<0)

//pad_camera_sync_cfg
#define ANALOG_REG_PAD_CAMERA_SYNC_CFG_RESERVED_0(n) (((n)&0x3FFFFF)<<10)
#define ANALOG_REG_PAD_CAMERA_HREF_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_CAMERA_HREF_IE (1<<7)
#define ANALOG_REG_PAD_CAMERA_HREF_SE (1<<6)
#define ANALOG_REG_PAD_CAMERA_HREF_SPU (1<<5)
#define ANALOG_REG_PAD_CAMERA_VSYNC_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_CAMERA_VSYNC_IE (1<<2)
#define ANALOG_REG_PAD_CAMERA_VSYNC_SE (1<<1)
#define ANALOG_REG_PAD_CAMERA_VSYNC_SPU (1<<0)

//pad_camera_others_cfg
#define ANALOG_REG_PAD_CAMERA_OTHERS_CFG_RESERVED_0(n) (((n)&0x7FF)<<21)
#define ANALOG_REG_PAD_CAMERA_WPDI (1<<20)
#define ANALOG_REG_PAD_CAMERA_PIX_CLK_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_CAMERA_PIX_CLK_IE (1<<17)
#define ANALOG_REG_PAD_CAMERA_PIX_CLK_SE (1<<16)
#define ANALOG_REG_PAD_CAMERA_PIX_CLK_SPU (1<<15)
#define ANALOG_REG_PAD_CAMERA_PWDN_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_CAMERA_PWDN_IE (1<<12)
#define ANALOG_REG_PAD_CAMERA_PWDN_SE (1<<11)
#define ANALOG_REG_PAD_CAMERA_PWDN_SPU (1<<10)
#define ANALOG_REG_PAD_CAMERA_REF_CLK_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_CAMERA_REF_CLK_IE (1<<7)
#define ANALOG_REG_PAD_CAMERA_REF_CLK_SE (1<<6)
#define ANALOG_REG_PAD_CAMERA_REF_CLK_SPU (1<<5)
#define ANALOG_REG_PAD_CAMERA_RST_L_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_CAMERA_RST_L_IE (1<<2)
#define ANALOG_REG_PAD_CAMERA_RST_L_SE (1<<1)
#define ANALOG_REG_PAD_CAMERA_RST_L_SPU (1<<0)

//pad_chip_pd_cfg
#define ANALOG_REG_PAD_CHIP_PD_CFG_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define ANALOG_REG_PAD_CHIP_PD_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_CHIP_PD_IE  (1<<2)
#define ANALOG_REG_PAD_CHIP_PD_SE  (1<<1)
#define ANALOG_REG_PAD_CHIP_PD_WPUS (1<<0)

//pad_codec_mclk_cfg
#define ANALOG_REG_PAD_CODEC_MCLK_CFG_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define ANALOG_REG_PAD_CODEC_MCLK_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_CODEC_MCLK_IE (1<<2)
#define ANALOG_REG_PAD_CODEC_MCLK_SE (1<<1)
#define ANALOG_REG_PAD_CODEC_MCLK_WPUS (1<<0)

//pad_debug_host_cfg
#define ANALOG_REG_PAD_DEBUG_HOST_CFG_RESERVED_0(n) (((n)&0x1FFFF)<<15)
#define ANALOG_REG_PAD_DEBUG_HOST_CLK_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_DEBUG_HOST_CLK_IE (1<<12)
#define ANALOG_REG_PAD_DEBUG_HOST_CLK_SE (1<<11)
#define ANALOG_REG_PAD_DEBUG_HOST_CLK_WPUS (1<<10)
#define ANALOG_REG_PAD_DEBUG_HOST_RX_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_DEBUG_HOST_RX_IE (1<<7)
#define ANALOG_REG_PAD_DEBUG_HOST_RX_SE (1<<6)
#define ANALOG_REG_PAD_DEBUG_HOST_RX_WPUS (1<<5)
#define ANALOG_REG_PAD_DEBUG_HOST_TX_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_DEBUG_HOST_TX_IE (1<<2)
#define ANALOG_REG_PAD_DEBUG_HOST_TX_SE (1<<1)
#define ANALOG_REG_PAD_DEBUG_HOST_TX_WPUS (1<<0)

//pad_gpio_cfg1
#define ANALOG_REG_PAD_GPIO_CFG1_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_PAD_GPIO_0_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_GPIO_0_IE   (1<<17)
#define ANALOG_REG_PAD_GPIO_0_SE   (1<<16)
#define ANALOG_REG_PAD_GPIO_0_SPU  (1<<15)
#define ANALOG_REG_PAD_GPIO_1_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_GPIO_1_IE   (1<<12)
#define ANALOG_REG_PAD_GPIO_1_SE   (1<<11)
#define ANALOG_REG_PAD_GPIO_1_SPU  (1<<10)
#define ANALOG_REG_PAD_GPIO_2_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_GPIO_2_IE   (1<<7)
#define ANALOG_REG_PAD_GPIO_2_SE   (1<<6)
#define ANALOG_REG_PAD_GPIO_2_SPU  (1<<5)
#define ANALOG_REG_PAD_GPIO_3_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_GPIO_3_IE   (1<<2)
#define ANALOG_REG_PAD_GPIO_3_SE   (1<<1)
#define ANALOG_REG_PAD_GPIO_3_SPU  (1<<0)

//pad_gpio_cfg2
#define ANALOG_REG_PAD_GPIO_CFG2_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_PAD_GPIO_4_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_GPIO_4_IE   (1<<17)
#define ANALOG_REG_PAD_GPIO_4_SE   (1<<16)
#define ANALOG_REG_PAD_GPIO_4_SPU  (1<<15)
#define ANALOG_REG_PAD_GPIO_5_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_GPIO_5_IE   (1<<12)
#define ANALOG_REG_PAD_GPIO_5_SE   (1<<11)
#define ANALOG_REG_PAD_GPIO_5_SPU  (1<<10)
#define ANALOG_REG_PAD_GPIO_6_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_GPIO_6_IE   (1<<7)
#define ANALOG_REG_PAD_GPIO_6_SE   (1<<6)
#define ANALOG_REG_PAD_GPIO_6_WPUS (1<<5)
#define ANALOG_REG_PAD_GPIO_7_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_GPIO_7_IE   (1<<2)
#define ANALOG_REG_PAD_GPIO_7_SE   (1<<1)
#define ANALOG_REG_PAD_GPIO_7_WPUS (1<<0)

//pad_gpio_cfg3
#define ANALOG_REG_PAD_GPIO_CFG3_RESERVED_0(n) (((n)&0x7F)<<25)
#define ANALOG_REG_PAD_GPIO_8_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_GPIO_8_IE   (1<<22)
#define ANALOG_REG_PAD_GPIO_8_SE   (1<<21)
#define ANALOG_REG_PAD_GPIO_8_WPUS (1<<20)
#define ANALOG_REG_PAD_GPIO_9_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_GPIO_9_IE   (1<<17)
#define ANALOG_REG_PAD_GPIO_9_SE   (1<<16)
#define ANALOG_REG_PAD_GPIO_9_WPUS (1<<15)
#define ANALOG_REG_PAD_GPIO_10_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_GPIO_10_IE  (1<<12)
#define ANALOG_REG_PAD_GPIO_10_SE  (1<<11)
#define ANALOG_REG_PAD_GPIO_10_WPUS (1<<10)
#define ANALOG_REG_PAD_GPIO_11_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_GPIO_11_IE  (1<<7)
#define ANALOG_REG_PAD_GPIO_11_SE  (1<<6)
#define ANALOG_REG_PAD_GPIO_11_WPUS (1<<5)
#define ANALOG_REG_PAD_GPIO_12_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_GPIO_12_IE  (1<<2)
#define ANALOG_REG_PAD_GPIO_12_SE  (1<<1)
#define ANALOG_REG_PAD_GPIO_12_WPUS (1<<0)

//pad_gpo_cfg
#define ANALOG_REG_PAD_GPO_CFG_RESERVED_0(n) (((n)&0x7F)<<25)
#define ANALOG_REG_PAD_GPO_0_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_GPO_0_IE    (1<<22)
#define ANALOG_REG_PAD_GPO_0_SE    (1<<21)
#define ANALOG_REG_PAD_GPO_0_WPUS  (1<<20)
#define ANALOG_REG_PAD_GPO_1_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_GPO_1_IE    (1<<17)
#define ANALOG_REG_PAD_GPO_1_SE    (1<<16)
#define ANALOG_REG_PAD_GPO_1_WPUS  (1<<15)
#define ANALOG_REG_PAD_GPO_2_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_GPO_2_IE    (1<<12)
#define ANALOG_REG_PAD_GPO_2_SE    (1<<11)
#define ANALOG_REG_PAD_GPO_2_WPUS  (1<<10)
#define ANALOG_REG_PAD_GPO_3_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_GPO_3_IE    (1<<7)
#define ANALOG_REG_PAD_GPO_3_SE    (1<<6)
#define ANALOG_REG_PAD_GPO_3_SPU   (1<<5)
#define ANALOG_REG_PAD_GPO_4_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_GPO_4_IE    (1<<2)
#define ANALOG_REG_PAD_GPO_4_SE    (1<<1)
#define ANALOG_REG_PAD_GPO_4_SPU   (1<<0)

//pad_i2c_m1_cfg
#define ANALOG_REG_PAD_I2C_M1_CFG_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_PAD_I2C_M1_SCL_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_I2C_M1_SCL_IE (1<<17)
#define ANALOG_REG_PAD_I2C_M1_SCL_SE (1<<16)
#define ANALOG_REG_PAD_I2C_M1_SCL_SPU (1<<15)
#define ANALOG_REG_PAD_I2C_M1_SDA_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_I2C_M1_SDA_IE (1<<12)
#define ANALOG_REG_PAD_I2C_M1_SDA_SE (1<<11)
#define ANALOG_REG_PAD_I2C_M1_SDA_SPU (1<<10)
#define ANALOG_REG_PAD_I2C_M4_SCL_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_I2C_M4_SCL_IE (1<<7)
#define ANALOG_REG_PAD_I2C_M4_SCL_SE (1<<6)
#define ANALOG_REG_PAD_I2C_M4_SCL_WPUS (1<<5)
#define ANALOG_REG_PAD_I2C_M4_SDA_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_I2C_M4_SDA_IE (1<<2)
#define ANALOG_REG_PAD_I2C_M4_SDA_SE (1<<1)
#define ANALOG_REG_PAD_I2C_M4_SDA_WPUS (1<<0)

//pad_i2s1_cfg
#define ANALOG_REG_PAD_I2S1_CFG_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_PAD_I2S1_BCK_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_I2S1_BCK_IE (1<<17)
#define ANALOG_REG_PAD_I2S1_BCK_SE (1<<16)
#define ANALOG_REG_PAD_I2S1_BCK_WPUS (1<<15)
#define ANALOG_REG_PAD_I2S1_LRCK_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_I2S1_LRCK_IE (1<<12)
#define ANALOG_REG_PAD_I2S1_LRCK_SE (1<<11)
#define ANALOG_REG_PAD_I2S1_LRCK_WPUS (1<<10)
#define ANALOG_REG_PAD_I2S1_SDAT_I_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_I2S1_SDAT_I_IE (1<<7)
#define ANALOG_REG_PAD_I2S1_SDAT_I_SE (1<<6)
#define ANALOG_REG_PAD_I2S1_SDAT_I_WPUS (1<<5)
#define ANALOG_REG_PAD_I2S1_SDAT_O_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_I2S1_SDAT_O_IE (1<<2)
#define ANALOG_REG_PAD_I2S1_SDAT_O_SE (1<<1)
#define ANALOG_REG_PAD_I2S1_SDAT_O_WPUS (1<<0)

//pad_i_keyon_cfg
#define ANALOG_REG_PAD_I_KEYON_CFG_RESERVED_0(n) (((n)&0x3FFFFFF)<<6)
#define ANALOG_REG_PAD_KEY_WPDI    (1<<5)
#define ANALOG_REG_PAD_I_KEYON_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_I_KEYON_IE  (1<<2)
#define ANALOG_REG_PAD_I_KEYON_SE  (1<<1)
#define ANALOG_REG_PAD_I_KEYON_WPUS (1<<0)

//pad_keyin_cfg
#define ANALOG_REG_PAD_KEYIN_CFG_RESERVED_0(n) (((n)&3)<<30)
#define ANALOG_REG_PAD_KEYIN_0_DRV(n) (((n)&3)<<28)
#define ANALOG_REG_PAD_KEYIN_0_IE  (1<<27)
#define ANALOG_REG_PAD_KEYIN_0_SE  (1<<26)
#define ANALOG_REG_PAD_KEYIN_0_WPUS (1<<25)
#define ANALOG_REG_PAD_KEYIN_1_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_KEYIN_1_IE  (1<<22)
#define ANALOG_REG_PAD_KEYIN_1_SE  (1<<21)
#define ANALOG_REG_PAD_KEYIN_1_WPUS (1<<20)
#define ANALOG_REG_PAD_KEYIN_2_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_KEYIN_2_IE  (1<<17)
#define ANALOG_REG_PAD_KEYIN_2_SE  (1<<16)
#define ANALOG_REG_PAD_KEYIN_2_WPUS (1<<15)
#define ANALOG_REG_PAD_KEYIN_3_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_KEYIN_3_IE  (1<<12)
#define ANALOG_REG_PAD_KEYIN_3_SE  (1<<11)
#define ANALOG_REG_PAD_KEYIN_3_WPUS (1<<10)
#define ANALOG_REG_PAD_KEYIN_4_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_KEYIN_4_IE  (1<<7)
#define ANALOG_REG_PAD_KEYIN_4_SE  (1<<6)
#define ANALOG_REG_PAD_KEYIN_4_WPUS (1<<5)
#define ANALOG_REG_PAD_KEYIN_5_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_KEYIN_5_IE  (1<<2)
#define ANALOG_REG_PAD_KEYIN_5_SE  (1<<1)
#define ANALOG_REG_PAD_KEYIN_5_WPUS (1<<0)

//pad_keyout_cfg
#define ANALOG_REG_PAD_KEYOUT_CFG_RESERVED_0(n) (((n)&3)<<30)
#define ANALOG_REG_PAD_KEYOUT_0_DRV(n) (((n)&3)<<28)
#define ANALOG_REG_PAD_KEYOUT_0_IE (1<<27)
#define ANALOG_REG_PAD_KEYOUT_0_SE (1<<26)
#define ANALOG_REG_PAD_KEYOUT_0_WPUS (1<<25)
#define ANALOG_REG_PAD_KEYOUT_1_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_KEYOUT_1_IE (1<<22)
#define ANALOG_REG_PAD_KEYOUT_1_SE (1<<21)
#define ANALOG_REG_PAD_KEYOUT_1_WPUS (1<<20)
#define ANALOG_REG_PAD_KEYOUT_2_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_KEYOUT_2_IE (1<<17)
#define ANALOG_REG_PAD_KEYOUT_2_SE (1<<16)
#define ANALOG_REG_PAD_KEYOUT_2_WPUS (1<<15)
#define ANALOG_REG_PAD_KEYOUT_3_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_KEYOUT_3_IE (1<<12)
#define ANALOG_REG_PAD_KEYOUT_3_SE (1<<11)
#define ANALOG_REG_PAD_KEYOUT_3_WPUS (1<<10)
#define ANALOG_REG_PAD_KEYOUT_4_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_KEYOUT_4_IE (1<<7)
#define ANALOG_REG_PAD_KEYOUT_4_SE (1<<6)
#define ANALOG_REG_PAD_KEYOUT_4_WPUS (1<<5)
#define ANALOG_REG_PAD_KEYOUT_5_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_KEYOUT_5_IE (1<<2)
#define ANALOG_REG_PAD_KEYOUT_5_SE (1<<1)
#define ANALOG_REG_PAD_KEYOUT_5_WPUS (1<<0)

//pad_lcd_cfg
#define ANALOG_REG_PAD_LCD_CFG_RESERVED_0(n) (((n)&0x1FFFFF)<<11)
#define ANALOG_REG_PAD_LCD_WPDI    (1<<10)
#define ANALOG_REG_PAD_LCD_FMARK_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_LCD_FMARK_IE (1<<7)
#define ANALOG_REG_PAD_LCD_FMARK_SE (1<<6)
#define ANALOG_REG_PAD_LCD_FMARK_SPU (1<<5)
#define ANALOG_REG_PAD_LCD_RSTB_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LCD_RSTB_IE (1<<2)
#define ANALOG_REG_PAD_LCD_RSTB_SE (1<<1)
#define ANALOG_REG_PAD_LCD_RSTB_SPU (1<<0)

//pad_lte_clk_cfg
#define ANALOG_REG_PAD_LTE_CLK_CFG_RESERVED_0(n) (((n)&0x3FFFFF)<<10)
#define ANALOG_REG_PAD_LTE_FCLK_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_LTE_FCLK_IE (1<<7)
#define ANALOG_REG_PAD_LTE_FCLK_SE (1<<6)
#define ANALOG_REG_PAD_LTE_FCLK_WPUS (1<<5)
#define ANALOG_REG_PAD_LTE_MCLK_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_MCLK_IE (1<<2)
#define ANALOG_REG_PAD_LTE_MCLK_SE (1<<1)
#define ANALOG_REG_PAD_LTE_MCLK_WPUS (1<<0)

//pad_lte_rbdp_rx_cfg1
#define ANALOG_REG_PAD_LTE_RBDP_RX_CFG1_RESERVED_0(n) (((n)&3)<<30)
#define ANALOG_REG_PAD_LTE_RBDP_RX_0_DRV(n) (((n)&3)<<28)
#define ANALOG_REG_PAD_LTE_RBDP_RX_0_IE (1<<27)
#define ANALOG_REG_PAD_LTE_RBDP_RX_0_SE (1<<26)
#define ANALOG_REG_PAD_LTE_RBDP_RX_0_WPUS (1<<25)
#define ANALOG_REG_PAD_LTE_RBDP_RX_1_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_LTE_RBDP_RX_1_IE (1<<22)
#define ANALOG_REG_PAD_LTE_RBDP_RX_1_SE (1<<21)
#define ANALOG_REG_PAD_LTE_RBDP_RX_1_WPUS (1<<20)
#define ANALOG_REG_PAD_LTE_RBDP_RX_2_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_LTE_RBDP_RX_2_IE (1<<17)
#define ANALOG_REG_PAD_LTE_RBDP_RX_2_SE (1<<16)
#define ANALOG_REG_PAD_LTE_RBDP_RX_2_WPUS (1<<15)
#define ANALOG_REG_PAD_LTE_RBDP_RX_3_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_LTE_RBDP_RX_3_IE (1<<12)
#define ANALOG_REG_PAD_LTE_RBDP_RX_3_SE (1<<11)
#define ANALOG_REG_PAD_LTE_RBDP_RX_3_WPUS (1<<10)
#define ANALOG_REG_PAD_LTE_RBDP_RX_4_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_LTE_RBDP_RX_4_IE (1<<7)
#define ANALOG_REG_PAD_LTE_RBDP_RX_4_SE (1<<6)
#define ANALOG_REG_PAD_LTE_RBDP_RX_4_WPUS (1<<5)
#define ANALOG_REG_PAD_LTE_RBDP_RX_5_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_RBDP_RX_5_IE (1<<2)
#define ANALOG_REG_PAD_LTE_RBDP_RX_5_SE (1<<1)
#define ANALOG_REG_PAD_LTE_RBDP_RX_5_WPUS (1<<0)

//pad_lte_rbdp_rx_cfg2
#define ANALOG_REG_PAD_LTE_RBDP_RX_CFG2_RESERVED_0(n) (((n)&3)<<30)
#define ANALOG_REG_PAD_LTE_RBDP_RX_6_DRV(n) (((n)&3)<<28)
#define ANALOG_REG_PAD_LTE_RBDP_RX_6_IE (1<<27)
#define ANALOG_REG_PAD_LTE_RBDP_RX_6_SE (1<<26)
#define ANALOG_REG_PAD_LTE_RBDP_RX_6_WPUS (1<<25)
#define ANALOG_REG_PAD_LTE_RBDP_RX_7_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_LTE_RBDP_RX_7_IE (1<<22)
#define ANALOG_REG_PAD_LTE_RBDP_RX_7_SE (1<<21)
#define ANALOG_REG_PAD_LTE_RBDP_RX_7_WPUS (1<<20)
#define ANALOG_REG_PAD_LTE_RBDP_RX_8_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_LTE_RBDP_RX_8_IE (1<<17)
#define ANALOG_REG_PAD_LTE_RBDP_RX_8_SE (1<<16)
#define ANALOG_REG_PAD_LTE_RBDP_RX_8_WPUS (1<<15)
#define ANALOG_REG_PAD_LTE_RBDP_RX_9_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_LTE_RBDP_RX_9_IE (1<<12)
#define ANALOG_REG_PAD_LTE_RBDP_RX_9_SE (1<<11)
#define ANALOG_REG_PAD_LTE_RBDP_RX_9_WPUS (1<<10)
#define ANALOG_REG_PAD_LTE_RBDP_RX_10_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_LTE_RBDP_RX_10_IE (1<<7)
#define ANALOG_REG_PAD_LTE_RBDP_RX_10_SE (1<<6)
#define ANALOG_REG_PAD_LTE_RBDP_RX_10_WPUS (1<<5)
#define ANALOG_REG_PAD_LTE_RBDP_RX_11_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_RBDP_RX_11_IE (1<<2)
#define ANALOG_REG_PAD_LTE_RBDP_RX_11_SE (1<<1)
#define ANALOG_REG_PAD_LTE_RBDP_RX_11_WPUS (1<<0)

//pad_lte_rbdp_tx_cfg1
#define ANALOG_REG_PAD_LTE_RBDP_TX_CFG1_RESERVED_0(n) (((n)&3)<<30)
#define ANALOG_REG_PAD_LTE_RBDP_TX_0_DRV(n) (((n)&3)<<28)
#define ANALOG_REG_PAD_LTE_RBDP_TX_0_IE (1<<27)
#define ANALOG_REG_PAD_LTE_RBDP_TX_0_SE (1<<26)
#define ANALOG_REG_PAD_LTE_RBDP_TX_0_WPUS (1<<25)
#define ANALOG_REG_PAD_LTE_RBDP_TX_1_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_LTE_RBDP_TX_1_IE (1<<22)
#define ANALOG_REG_PAD_LTE_RBDP_TX_1_SE (1<<21)
#define ANALOG_REG_PAD_LTE_RBDP_TX_1_WPUS (1<<20)
#define ANALOG_REG_PAD_LTE_RBDP_TX_2_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_LTE_RBDP_TX_2_IE (1<<17)
#define ANALOG_REG_PAD_LTE_RBDP_TX_2_SE (1<<16)
#define ANALOG_REG_PAD_LTE_RBDP_TX_2_WPUS (1<<15)
#define ANALOG_REG_PAD_LTE_RBDP_TX_3_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_LTE_RBDP_TX_3_IE (1<<12)
#define ANALOG_REG_PAD_LTE_RBDP_TX_3_SE (1<<11)
#define ANALOG_REG_PAD_LTE_RBDP_TX_3_WPUS (1<<10)
#define ANALOG_REG_PAD_LTE_RBDP_TX_4_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_LTE_RBDP_TX_4_IE (1<<7)
#define ANALOG_REG_PAD_LTE_RBDP_TX_4_SE (1<<6)
#define ANALOG_REG_PAD_LTE_RBDP_TX_4_WPUS (1<<5)
#define ANALOG_REG_PAD_LTE_RBDP_TX_5_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_RBDP_TX_5_IE (1<<2)
#define ANALOG_REG_PAD_LTE_RBDP_TX_5_SE (1<<1)
#define ANALOG_REG_PAD_LTE_RBDP_TX_5_WPUS (1<<0)

//pad_lte_rbdp_tx_cfg2
#define ANALOG_REG_PAD_LTE_RBDP_TX_CFG2_RESERVED_0(n) (((n)&3)<<30)
#define ANALOG_REG_PAD_LTE_RBDP_TX_6_DRV(n) (((n)&3)<<28)
#define ANALOG_REG_PAD_LTE_RBDP_TX_6_IE (1<<27)
#define ANALOG_REG_PAD_LTE_RBDP_TX_6_SE (1<<26)
#define ANALOG_REG_PAD_LTE_RBDP_TX_6_WPUS (1<<25)
#define ANALOG_REG_PAD_LTE_RBDP_TX_7_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_LTE_RBDP_TX_7_IE (1<<22)
#define ANALOG_REG_PAD_LTE_RBDP_TX_7_SE (1<<21)
#define ANALOG_REG_PAD_LTE_RBDP_TX_7_WPUS (1<<20)
#define ANALOG_REG_PAD_LTE_RBDP_TX_8_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_LTE_RBDP_TX_8_IE (1<<17)
#define ANALOG_REG_PAD_LTE_RBDP_TX_8_SE (1<<16)
#define ANALOG_REG_PAD_LTE_RBDP_TX_8_WPUS (1<<15)
#define ANALOG_REG_PAD_LTE_RBDP_TX_9_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_LTE_RBDP_TX_9_IE (1<<12)
#define ANALOG_REG_PAD_LTE_RBDP_TX_9_SE (1<<11)
#define ANALOG_REG_PAD_LTE_RBDP_TX_9_WPUS (1<<10)
#define ANALOG_REG_PAD_LTE_RBDP_TX_10_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_LTE_RBDP_TX_10_IE (1<<7)
#define ANALOG_REG_PAD_LTE_RBDP_TX_10_SE (1<<6)
#define ANALOG_REG_PAD_LTE_RBDP_TX_10_WPUS (1<<5)
#define ANALOG_REG_PAD_LTE_RBDP_TX_11_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_RBDP_TX_11_IE (1<<2)
#define ANALOG_REG_PAD_LTE_RBDP_TX_11_SE (1<<1)
#define ANALOG_REG_PAD_LTE_RBDP_TX_11_WPUS (1<<0)

//pad_lte_reset_cfg
#define ANALOG_REG_PAD_LTE_RESET_CFG_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define ANALOG_REG_PAD_LTE_RESET_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_RESET_IE (1<<2)
#define ANALOG_REG_PAD_LTE_RESET_SE (1<<1)
#define ANALOG_REG_PAD_LTE_RESET_WPUS (1<<0)

//pad_lte_rfspi_cfg
#define ANALOG_REG_PAD_LTE_RFSPI_CFG_RESERVED_0(n) (((n)&0x1FFFF)<<15)
#define ANALOG_REG_PAD_LTE_RFSPI_CLK_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_LTE_RFSPI_CLK_IE (1<<12)
#define ANALOG_REG_PAD_LTE_RFSPI_CLK_SE (1<<11)
#define ANALOG_REG_PAD_LTE_RFSPI_CLK_WPUS (1<<10)
#define ANALOG_REG_PAD_LTE_RFSPI_CS_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_LTE_RFSPI_CS_IE (1<<7)
#define ANALOG_REG_PAD_LTE_RFSPI_CS_SE (1<<6)
#define ANALOG_REG_PAD_LTE_RFSPI_CS_WPUS (1<<5)
#define ANALOG_REG_PAD_LTE_RFSPI_DIO_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_RFSPI_DIO_IE (1<<2)
#define ANALOG_REG_PAD_LTE_RFSPI_DIO_SE (1<<1)
#define ANALOG_REG_PAD_LTE_RFSPI_DIO_WPUS (1<<0)

//pad_lte_frame_cfg
#define ANALOG_REG_PAD_LTE_FRAME_CFG_RESERVED_0(n) (((n)&0x3FFFFF)<<10)
#define ANALOG_REG_PAD_LTE_RXFRAME_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_LTE_RXFRAME_IE (1<<7)
#define ANALOG_REG_PAD_LTE_RXFRAME_SE (1<<6)
#define ANALOG_REG_PAD_LTE_RXFRAME_WPUS (1<<5)
#define ANALOG_REG_PAD_LTE_TXFRAME_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_TXFRAME_IE (1<<2)
#define ANALOG_REG_PAD_LTE_TXFRAME_SE (1<<1)
#define ANALOG_REG_PAD_LTE_TXFRAME_WPUS (1<<0)

//pad_lte_data_cfg
#define ANALOG_REG_PAD_LTE_DATA_CFG_RESERVED_0(n) (((n)&0x3FFFFF)<<10)
#define ANALOG_REG_PAD_LTE_RXTXDATA_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_LTE_RXTXDATA_IE (1<<7)
#define ANALOG_REG_PAD_LTE_RXTXDATA_SE (1<<6)
#define ANALOG_REG_PAD_LTE_RXTXDATA_WPUS (1<<5)
#define ANALOG_REG_PAD_LTE_RXTXEN_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_RXTXEN_IE (1<<2)
#define ANALOG_REG_PAD_LTE_RXTXEN_SE (1<<1)
#define ANALOG_REG_PAD_LTE_RXTXEN_WPUS (1<<0)

//pad_lte_sysclken_cfg
#define ANALOG_REG_PAD_LTE_SYSCLKEN_CFG_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define ANALOG_REG_PAD_LTE_SYSCLKEN_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_LTE_SYSCLKEN_IE (1<<2)
#define ANALOG_REG_PAD_LTE_SYSCLKEN_SE (1<<1)
#define ANALOG_REG_PAD_LTE_SYSCLKEN_WPUS (1<<0)

//pad_pwm_cfg
#define ANALOG_REG_PAD_PWM_CFG_RESERVED_0(n) (((n)&0x7FF)<<21)
#define ANALOG_REG_PAD_MISC_WPDI   (1<<20)
#define ANALOG_REG_PAD_PWL_OUT_0_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_PWL_OUT_0_IE (1<<17)
#define ANALOG_REG_PAD_PWL_OUT_0_SE (1<<16)
#define ANALOG_REG_PAD_PWL_OUT_0_WPUS (1<<15)
#define ANALOG_REG_PAD_PWL_OUT_1_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_PWL_OUT_1_IE (1<<12)
#define ANALOG_REG_PAD_PWL_OUT_1_SE (1<<11)
#define ANALOG_REG_PAD_PWL_OUT_1_WPUS (1<<10)
#define ANALOG_REG_PAD_PWM_LPG_OUT_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_PWM_LPG_OUT_IE (1<<7)
#define ANALOG_REG_PAD_PWM_LPG_OUT_SE (1<<6)
#define ANALOG_REG_PAD_PWM_LPG_OUT_WPUS (1<<5)
#define ANALOG_REG_PAD_PWT_OUT_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_PWT_OUT_IE  (1<<2)
#define ANALOG_REG_PAD_PWT_OUT_SE  (1<<1)
#define ANALOG_REG_PAD_PWT_OUT_WPUS (1<<0)

//pad_rf_gge_cfg
#define ANALOG_REG_PAD_RF_GGE_CFG_RESERVED_0(n) (((n)&3)<<30)
#define ANALOG_REG_PAD_RF_GGE_PDN_DRV(n) (((n)&3)<<28)
#define ANALOG_REG_PAD_RF_GGE_PDN_IE (1<<27)
#define ANALOG_REG_PAD_RF_GGE_PDN_SE (1<<26)
#define ANALOG_REG_PAD_RF_GGE_PDN_WPUS (1<<25)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_CS_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_CS_IE (1<<22)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_CS_SE (1<<21)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_CS_WPUS (1<<20)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_SCLK_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_SCLK_IE (1<<17)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_SCLK_SE (1<<16)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_SCLK_WPUS (1<<15)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_SDIO_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_SDIO_IE (1<<12)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_SDIO_SE (1<<11)
#define ANALOG_REG_PAD_RF_GGE_RFSPI_SDIO_WPUS (1<<10)
#define ANALOG_REG_PAD_RF_GGE_STROBE_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_RF_GGE_STROBE_IE (1<<7)
#define ANALOG_REG_PAD_RF_GGE_STROBE_SE (1<<6)
#define ANALOG_REG_PAD_RF_GGE_STROBE_WPUS (1<<5)
#define ANALOG_REG_PAD_RF_GGE_XEN_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_RF_GGE_XEN_IE (1<<2)
#define ANALOG_REG_PAD_RF_GGE_XEN_SE (1<<1)
#define ANALOG_REG_PAD_RF_GGE_XEN_WPUS (1<<0)

//pad_rfdig_gpio_cfg1
#define ANALOG_REG_PAD_RFDIG_GPIO_CFG1_RESERVED_0(n) (((n)&0x3F)<<26)
#define ANALOG_REG_PAD_RF_WPDI     (1<<25)
#define ANALOG_REG_PAD_RFDIG_GPIO_0_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_RFDIG_GPIO_0_IE (1<<22)
#define ANALOG_REG_PAD_RFDIG_GPIO_0_SE (1<<21)
#define ANALOG_REG_PAD_RFDIG_GPIO_0_WPUS (1<<20)
#define ANALOG_REG_PAD_RFDIG_GPIO_1_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_RFDIG_GPIO_1_IE (1<<17)
#define ANALOG_REG_PAD_RFDIG_GPIO_1_SE (1<<16)
#define ANALOG_REG_PAD_RFDIG_GPIO_1_WPUS (1<<15)
#define ANALOG_REG_PAD_RFDIG_GPIO_2_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_RFDIG_GPIO_2_IE (1<<12)
#define ANALOG_REG_PAD_RFDIG_GPIO_2_SE (1<<11)
#define ANALOG_REG_PAD_RFDIG_GPIO_2_WPUS (1<<10)
#define ANALOG_REG_PAD_RFDIG_GPIO_3_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_RFDIG_GPIO_3_IE (1<<7)
#define ANALOG_REG_PAD_RFDIG_GPIO_3_SE (1<<6)
#define ANALOG_REG_PAD_RFDIG_GPIO_3_WPUS (1<<5)
#define ANALOG_REG_PAD_RFDIG_GPIO_4_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_RFDIG_GPIO_4_IE (1<<2)
#define ANALOG_REG_PAD_RFDIG_GPIO_4_SE (1<<1)
#define ANALOG_REG_PAD_RFDIG_GPIO_4_WPUS (1<<0)

//pad_rfdig_gpio_cfg2
#define ANALOG_REG_PAD_RFDIG_GPIO_CFG2_RESERVED_0(n) (((n)&0x7F)<<25)
#define ANALOG_REG_PAD_RFDIG_GPIO_5_DRV(n) (((n)&3)<<23)
#define ANALOG_REG_PAD_RFDIG_GPIO_5_IE (1<<22)
#define ANALOG_REG_PAD_RFDIG_GPIO_5_SE (1<<21)
#define ANALOG_REG_PAD_RFDIG_GPIO_5_WPUS (1<<20)
#define ANALOG_REG_PAD_RFDIG_GPIO_6_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_RFDIG_GPIO_6_IE (1<<17)
#define ANALOG_REG_PAD_RFDIG_GPIO_6_SE (1<<16)
#define ANALOG_REG_PAD_RFDIG_GPIO_6_WPUS (1<<15)
#define ANALOG_REG_PAD_RFDIG_GPIO_7_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_RFDIG_GPIO_7_IE (1<<12)
#define ANALOG_REG_PAD_RFDIG_GPIO_7_SE (1<<11)
#define ANALOG_REG_PAD_RFDIG_GPIO_7_WPUS (1<<10)
#define ANALOG_REG_PAD_RFDIG_GPIO_8_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_RFDIG_GPIO_8_IE (1<<7)
#define ANALOG_REG_PAD_RFDIG_GPIO_8_SE (1<<6)
#define ANALOG_REG_PAD_RFDIG_GPIO_8_WPUS (1<<5)
#define ANALOG_REG_PAD_RFDIG_GPIO_9_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_RFDIG_GPIO_9_IE (1<<2)
#define ANALOG_REG_PAD_RFDIG_GPIO_9_SE (1<<1)
#define ANALOG_REG_PAD_RFDIG_GPIO_9_WPUS (1<<0)

//pad_sdmmc1_others_cfg
#define ANALOG_REG_PAD_SDMMC1_OTHERS_CFG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define ANALOG_REG_PAD_SDMMC1_MS   (1<<15)
#define ANALOG_REG_PAD_SDMMC1_MSEN (1<<14)
#define ANALOG_REG_PAD_SDMMC1_CLK_DRV(n) (((n)&15)<<10)
#define ANALOG_REG_PAD_SDMMC1_CLK_IE (1<<9)
#define ANALOG_REG_PAD_SDMMC1_CLK_SE (1<<8)
#define ANALOG_REG_PAD_SDMMC1_CLK_SPU (1<<7)
#define ANALOG_REG_PAD_SDMMC1_CMD_DRV(n) (((n)&15)<<3)
#define ANALOG_REG_PAD_SDMMC1_CMD_IE (1<<2)
#define ANALOG_REG_PAD_SDMMC1_CMD_SE (1<<1)
#define ANALOG_REG_PAD_SDMMC1_CMD_SPU (1<<0)

//pad_sdmmc1_data_cfg
#define ANALOG_REG_PAD_SDMMC1_DATA_CFG_RESERVED_0(n) (((n)&15)<<28)
#define ANALOG_REG_PAD_SDMMC1_DATA_0_DRV(n) (((n)&15)<<24)
#define ANALOG_REG_PAD_SDMMC1_DATA_0_IE (1<<23)
#define ANALOG_REG_PAD_SDMMC1_DATA_0_SE (1<<22)
#define ANALOG_REG_PAD_SDMMC1_DATA_0_SPU (1<<21)
#define ANALOG_REG_PAD_SDMMC1_DATA_1_DRV(n) (((n)&15)<<17)
#define ANALOG_REG_PAD_SDMMC1_DATA_1_IE (1<<16)
#define ANALOG_REG_PAD_SDMMC1_DATA_1_SE (1<<15)
#define ANALOG_REG_PAD_SDMMC1_DATA_1_SPU (1<<14)
#define ANALOG_REG_PAD_SDMMC1_DATA_2_DRV(n) (((n)&15)<<10)
#define ANALOG_REG_PAD_SDMMC1_DATA_2_IE (1<<9)
#define ANALOG_REG_PAD_SDMMC1_DATA_2_SE (1<<8)
#define ANALOG_REG_PAD_SDMMC1_DATA_2_SPU (1<<7)
#define ANALOG_REG_PAD_SDMMC1_DATA_3_DRV(n) (((n)&15)<<3)
#define ANALOG_REG_PAD_SDMMC1_DATA_3_IE (1<<2)
#define ANALOG_REG_PAD_SDMMC1_DATA_3_SE (1<<1)
#define ANALOG_REG_PAD_SDMMC1_DATA_3_SPU (1<<0)

//pad_sim_1_cfg
#define ANALOG_REG_PAD_SIM_1_CFG_RESERVED_0(n) (((n)&0x1FF)<<23)
#define ANALOG_REG_PAD_SIM_1_MS    (1<<22)
#define ANALOG_REG_PAD_SIM_1_MSEN  (1<<21)
#define ANALOG_REG_PAD_SIM_1_CLK_DRV(n) (((n)&15)<<17)
#define ANALOG_REG_PAD_SIM_1_CLK_IE (1<<16)
#define ANALOG_REG_PAD_SIM_1_CLK_SE (1<<15)
#define ANALOG_REG_PAD_SIM_1_CLK_SPU (1<<14)
#define ANALOG_REG_PAD_SIM_1_DIO_DRV(n) (((n)&15)<<10)
#define ANALOG_REG_PAD_SIM_1_DIO_IE (1<<9)
#define ANALOG_REG_PAD_SIM_1_DIO_SE (1<<8)
#define ANALOG_REG_PAD_SIM_1_DIO_SPU (1<<7)
#define ANALOG_REG_PAD_SIM_1_RST_DRV(n) (((n)&15)<<3)
#define ANALOG_REG_PAD_SIM_1_RST_IE (1<<2)
#define ANALOG_REG_PAD_SIM_1_RST_SE (1<<1)
#define ANALOG_REG_PAD_SIM_1_RST_SPU (1<<0)

//pad_sim_2_cfg
#define ANALOG_REG_PAD_SIM_2_CFG_RESERVED_0(n) (((n)&0x1FF)<<23)
#define ANALOG_REG_PAD_SIM_2_MS    (1<<22)
#define ANALOG_REG_PAD_SIM_2_MSEN  (1<<21)
#define ANALOG_REG_PAD_SIM_2_CLK_DRV(n) (((n)&15)<<17)
#define ANALOG_REG_PAD_SIM_2_CLK_IE (1<<16)
#define ANALOG_REG_PAD_SIM_2_CLK_SE (1<<15)
#define ANALOG_REG_PAD_SIM_2_CLK_SPU (1<<14)
#define ANALOG_REG_PAD_SIM_2_DIO_DRV(n) (((n)&15)<<10)
#define ANALOG_REG_PAD_SIM_2_DIO_IE (1<<9)
#define ANALOG_REG_PAD_SIM_2_DIO_SE (1<<8)
#define ANALOG_REG_PAD_SIM_2_DIO_SPU (1<<7)
#define ANALOG_REG_PAD_SIM_2_RST_DRV(n) (((n)&15)<<3)
#define ANALOG_REG_PAD_SIM_2_RST_IE (1<<2)
#define ANALOG_REG_PAD_SIM_2_RST_SE (1<<1)
#define ANALOG_REG_PAD_SIM_2_RST_SPU (1<<0)

//pad_spi_flash_cfg1
#define ANALOG_REG_PAD_SPI_FLASH_CFG1_RESERVED_0(n) (((n)&0x7FF)<<21)
#define ANALOG_REG_PAD_SPI_FLASH_CLK_DRV(n) (((n)&15)<<17)
#define ANALOG_REG_PAD_SPI_FLASH_CLK_IE (1<<16)
#define ANALOG_REG_PAD_SPI_FLASH_CLK_SE (1<<15)
#define ANALOG_REG_PAD_SPI_FLASH_CLK_SPU (1<<14)
#define ANALOG_REG_PAD_SPI_FLASH_CS0_DRV(n) (((n)&15)<<10)
#define ANALOG_REG_PAD_SPI_FLASH_CS0_IE (1<<9)
#define ANALOG_REG_PAD_SPI_FLASH_CS0_SE (1<<8)
#define ANALOG_REG_PAD_SPI_FLASH_CS0_SPU (1<<7)
#define ANALOG_REG_PAD_SPI_FLASH_CS1_DRV(n) (((n)&15)<<3)
#define ANALOG_REG_PAD_SPI_FLASH_CS1_IE (1<<2)
#define ANALOG_REG_PAD_SPI_FLASH_CS1_SE (1<<1)
#define ANALOG_REG_PAD_SPI_FLASH_CS1_SPU (1<<0)

//pad_spi_flash_cfg2
#define ANALOG_REG_PAD_SPI_FLASH_CFG2_RESERVED_0(n) (((n)&3)<<30)
#define ANALOG_REG_PAD_SPI_FLASH_MS (1<<29)
#define ANALOG_REG_PAD_SPI_FLASH_MSEN (1<<28)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_0_DRV(n) (((n)&15)<<24)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_0_IE (1<<23)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_0_SE (1<<22)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_0_SPU (1<<21)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_1_DRV(n) (((n)&15)<<17)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_1_IE (1<<16)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_1_SE (1<<15)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_1_SPU (1<<14)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_2_DRV(n) (((n)&15)<<10)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_2_IE (1<<9)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_2_SE (1<<8)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_2_SPU (1<<7)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_3_DRV(n) (((n)&15)<<3)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_3_IE (1<<2)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_3_SE (1<<1)
#define ANALOG_REG_PAD_SPI_FLASH_SIO_3_SPU (1<<0)

//pad_spi_lcd_cfg1
#define ANALOG_REG_PAD_SPI_LCD_CFG1_RESERVED_0(n) (((n)&0x3FFFFF)<<10)
#define ANALOG_REG_PAD_SPI_LCD_CLK_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_SPI_LCD_CLK_IE (1<<7)
#define ANALOG_REG_PAD_SPI_LCD_CLK_SE (1<<6)
#define ANALOG_REG_PAD_SPI_LCD_CLK_SPU (1<<5)
#define ANALOG_REG_PAD_SPI_LCD_CS_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_SPI_LCD_CS_IE (1<<2)
#define ANALOG_REG_PAD_SPI_LCD_CS_SE (1<<1)
#define ANALOG_REG_PAD_SPI_LCD_CS_SPU (1<<0)

//pad_spi_lcd_cfg2
#define ANALOG_REG_PAD_SPI_LCD_CFG2_RESERVED_0(n) (((n)&0x1FFFF)<<15)
#define ANALOG_REG_PAD_SPI_LCD_SDC_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_SPI_LCD_SDC_IE (1<<12)
#define ANALOG_REG_PAD_SPI_LCD_SDC_SE (1<<11)
#define ANALOG_REG_PAD_SPI_LCD_SDC_SPU (1<<10)
#define ANALOG_REG_PAD_SPI_LCD_SELECT_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_SPI_LCD_SELECT_IE (1<<7)
#define ANALOG_REG_PAD_SPI_LCD_SELECT_SE (1<<6)
#define ANALOG_REG_PAD_SPI_LCD_SELECT_SPU (1<<5)
#define ANALOG_REG_PAD_SPI_LCD_SIO_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_SPI_LCD_SIO_IE (1<<2)
#define ANALOG_REG_PAD_SPI_LCD_SIO_SE (1<<1)
#define ANALOG_REG_PAD_SPI_LCD_SIO_SPU (1<<0)

//pad_tst_cfg
#define ANALOG_REG_PAD_TST_CFG_RESERVED_0(n) (((n)&0x7FFFFFF)<<5)
#define ANALOG_REG_PAD_TST_H_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_TST_H_IE    (1<<2)
#define ANALOG_REG_PAD_TST_H_SE    (1<<1)
#define ANALOG_REG_PAD_TST_H_WPUS  (1<<0)

//pad_uart_1_cfg
#define ANALOG_REG_PAD_UART_1_CFG_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_PAD_UART_1_CTS_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_UART_1_CTS_IE (1<<17)
#define ANALOG_REG_PAD_UART_1_CTS_SE (1<<16)
#define ANALOG_REG_PAD_UART_1_CTS_WPUS (1<<15)
#define ANALOG_REG_PAD_UART_1_RTS_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_UART_1_RTS_IE (1<<12)
#define ANALOG_REG_PAD_UART_1_RTS_SE (1<<11)
#define ANALOG_REG_PAD_UART_1_RTS_WPUS (1<<10)
#define ANALOG_REG_PAD_UART_1_RXD_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_UART_1_RXD_IE (1<<7)
#define ANALOG_REG_PAD_UART_1_RXD_SE (1<<6)
#define ANALOG_REG_PAD_UART_1_RXD_WPUS (1<<5)
#define ANALOG_REG_PAD_UART_1_TXD_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_UART_1_TXD_IE (1<<2)
#define ANALOG_REG_PAD_UART_1_TXD_SE (1<<1)
#define ANALOG_REG_PAD_UART_1_TXD_WPUS (1<<0)

//pad_uart_2_cfg
#define ANALOG_REG_PAD_UART_2_CFG_RESERVED_0(n) (((n)&0xFFF)<<20)
#define ANALOG_REG_PAD_UART_2_CTS_DRV(n) (((n)&3)<<18)
#define ANALOG_REG_PAD_UART_2_CTS_IE (1<<17)
#define ANALOG_REG_PAD_UART_2_CTS_SE (1<<16)
#define ANALOG_REG_PAD_UART_2_CTS_WPUS (1<<15)
#define ANALOG_REG_PAD_UART_2_RTS_DRV(n) (((n)&3)<<13)
#define ANALOG_REG_PAD_UART_2_RTS_IE (1<<12)
#define ANALOG_REG_PAD_UART_2_RTS_SE (1<<11)
#define ANALOG_REG_PAD_UART_2_RTS_WPUS (1<<10)
#define ANALOG_REG_PAD_UART_2_RXD_DRV(n) (((n)&3)<<8)
#define ANALOG_REG_PAD_UART_2_RXD_IE (1<<7)
#define ANALOG_REG_PAD_UART_2_RXD_SE (1<<6)
#define ANALOG_REG_PAD_UART_2_RXD_WPUS (1<<5)
#define ANALOG_REG_PAD_UART_2_TXD_DRV(n) (((n)&3)<<3)
#define ANALOG_REG_PAD_UART_2_TXD_IE (1<<2)
#define ANALOG_REG_PAD_UART_2_TXD_SE (1<<1)
#define ANALOG_REG_PAD_UART_2_TXD_WPUS (1<<0)

//resv0
#define ANALOG_REG_REG_RESV0(n)    (((n)&0xFFFFFFFF)<<0)

//resv1
#define ANALOG_REG_REG_RESV1(n)    (((n)&0xFFFFFFFF)<<0)

//resv2
#define ANALOG_REG_REG_RESV2(n)    (((n)&0xFFFFFFFF)<<0)

//resv3
#define ANALOG_REG_REG_RESV3(n)    (((n)&0xFFFFFFFF)<<0)

//resv4
#define ANALOG_REG_REG_RESV4(n)    (((n)&0xFFFFFFFF)<<0)

//resv5
#define ANALOG_REG_REG_RESV5(n)    (((n)&0xFFFFFFFF)<<0)

//resv6
#define ANALOG_REG_REG_RESV6(n)    (((n)&0xFFFFFFFF)<<0)

//resv7
#define ANALOG_REG_REG_RESV7(n)    (((n)&0xFFFFFFFF)<<0)

//resv8
#define ANALOG_REG_REG_RESV8(n)    (((n)&0xFFFFFFFF)<<0)

//resv9
#define ANALOG_REG_REG_RESV9(n)    (((n)&0xFFFFFFFF)<<0)

//resv10
#define ANALOG_REG_REG_RESV10(n)   (((n)&0xFFFFFFFF)<<0)

//resv11
#define ANALOG_REG_REG_RESV11(n)   (((n)&0xFFFFFFFF)<<0)

//resv12
#define ANALOG_REG_REG_RESV12(n)   (((n)&0xFFFFFFFF)<<0)

//resv13
#define ANALOG_REG_REG_RESV13(n)   (((n)&0xFFFFFFFF)<<0)

//resv14
#define ANALOG_REG_REG_RESV14(n)   (((n)&0xFFFFFFFF)<<0)

//resv15
#define ANALOG_REG_REG_RESV15(n)   (((n)&0xFFFFFFFF)<<0)





#endif

