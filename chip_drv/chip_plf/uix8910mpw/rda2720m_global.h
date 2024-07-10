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


#ifndef _RDA2720M_GLOBAL_H_
#define _RDA2720M_GLOBAL_H_

typedef volatile unsigned int REG32;

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// RDA2720M_GLOBAL_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_RDA2720M_GLOBAL_BASE   0xC00

typedef volatile struct
{
    REG32                          CHIP_ID_LOW;                  //0x00000000
    REG32                          CHIP_ID_HIGH;                 //0x00000004
    REG32                          MODULE_EN0;                   //0x00000008
    REG32                          ARM_CLK_EN0;                  //0x0000000C
    REG32                          RTC_CLK_EN0;                  //0x00000010
    REG32                          SOFT_RST0;                    //0x00000014
    REG32                          SOFT_RST1;                    //0x00000018
    REG32                          POWER_PD_SW;                  //0x0000001C
    REG32                          POWER_PD_HW;                  //0x00000020
    REG32                          SOFT_RST_HW;                  //0x00000024
    REG32                          RESERVED_REG1;                //0x00000028
    REG32                          RESERVED_REG2;                //0x0000002C
    REG32                          RESERVED_REG3;                //0x00000030
    REG32                          DCDC_CLK_REG0;                //0x00000034
    REG32                          DCDC_CORE_REG0;               //0x00000038
    REG32                          DCDC_CORE_REG1;               //0x0000003C
    REG32                          DCDC_CORE_REG2;               //0x00000040
    REG32                          DCDC_CORE_VOL;                //0x00000044
    REG32                          DCDC_GEN_REG0;                //0x00000048
    REG32                          DCDC_GEN_REG1;                //0x0000004C
    REG32                          DCDC_GEN_REG2;                //0x00000050
    REG32                          DCDC_GEN_VOL;                 //0x00000054
    REG32                          DCDC_WPA_REG0;                //0x00000058
    REG32                          DCDC_WPA_REG1;                //0x0000005C
    REG32                          DCDC_WPA_REG2;                //0x00000060
    REG32                          DCDC_WPA_VOL;                 //0x00000064
    REG32                          DCDC_WPA_DCM_HW;              //0x00000068
    REG32                          DCDC_CH_CTRL;                 //0x0000006C
    REG32                          RESERVED_REG6;                //0x00000070
    REG32                          RESERVED_REG7;                //0x00000074
    REG32                          RESERVED_REG8;                //0x00000078
    REG32                          RESERVED_REG9;                //0x0000007C
    REG32                          RESERVED_REG10;               //0x00000080
    REG32                          LDO_AVDD18_REG0;              //0x00000084
    REG32                          LDO_AVDD18_REG1;              //0x00000088
    REG32                          LDO_CAMIO_REG0;               //0x0000008C
    REG32                          LDO_CAMIO_REG1;               //0x00000090
    REG32                          LDO_RF18A_REG0;               //0x00000094
    REG32                          LDO_RF18A_REG1;               //0x00000098
    REG32                          LDO_RF18B_REG0;               //0x0000009C
    REG32                          LDO_RF18B_REG1;               //0x000000A0
    REG32                          LDO_CAMD_REG0;                //0x000000A4
    REG32                          LDO_CAMD_REG1;                //0x000000A8
    REG32                          LDO_CON_REG0;                 //0x000000AC
    REG32                          LDO_CON_REG1;                 //0x000000B0
    REG32                          LDO_MEM_REG0;                 //0x000000B4
    REG32                          LDO_MEM_REG1;                 //0x000000B8
    REG32                          LDO_SIM0_REG0;                //0x000000BC
    REG32                          LDO_SIM0_PD_REG;              //0x000000C0
    REG32                          LDO_SIM0_REG1;                //0x000000C4
    REG32                          LDO_SIM1_REG0;                //0x000000C8
    REG32                          LDO_SIM1_PD1_REG;              //0x000000CC
    REG32                          LDO_SIM1_REG1;                //0x000000D0
    REG32                          LDO_SIM2_REG0;                //0x000000D4
    REG32                          LDO_SIM2_PD_REG;              //0x000000D8
    REG32                          LDO_SIM2_REG1;                //0x000000DC
    REG32                          LDO_CAMA_REG0;                //0x000000E0
    REG32                          LDO_CAMA_REG1;                //0x000000E4
    REG32                          LDO_CAMMOT_REG0;              //0x000000E8
    REG32                          LDO_CAMMOT_REG1;              //0x000000EC
    REG32                          LDO_EMMCCORE_REG0;            //0x000000F0
    REG32                          LDO_EMMCCORE_PD_REG;          //0x000000F4
    REG32                          LDO_EMMCCORE_REG1;            //0x000000F8
    REG32                          LDO_SD_REG0;                  //0x000000FC
    REG32                          LDO_SD_PD_REG;                //0x00000100
    REG32                          LDO_SD_REG1;                  //0x00000104
    REG32                          LDO_SDIO_REG0;                //0x00000108
    REG32                          LDO_SDIO_PD_REG;              //0x0000010C
    REG32                          LDO_SDIO_REG1;                //0x00000110
    REG32                          LDO_VDD28_REG0;               //0x00000114
    REG32                          LDO_VDD28_REG1;               //0x00000118
    REG32                          LDO_WIFIPA_REG0;              //0x0000011C
    REG32                          LDO_WIFIPA_REG1;              //0x00000120
    REG32                          LDO_DCXO_REG0;                //0x00000124
    REG32                          LDO_DCXO_REG1;                //0x00000128
    REG32                          LDO_USB_REG0;                 //0x0000012C
    REG32                          LDO_USB_PD_REG;               //0x00000130
    REG32                          LDO_USB_REG1;                 //0x00000134
    REG32                          RESERVED_REG17;               //0x00000138
    REG32                          RESERVED_REG18;               //0x0000013C
    REG32                          RESERVED_REG19;               //0x00000140
    REG32                          RESERVED_REG20;               //0x00000144
    REG32                          LDO_TRIM_REG;                 //0x00000148
    REG32                          LDO_RTC_CTRL;                 //0x0000014C
    REG32                          LDO_CH_CTRL;                  //0x00000150
    REG32                          RESERVED_REG23;               //0x00000154
    REG32                          RESERVED_REG24;               //0x00000158
    REG32                          RESERVED_REG25;               //0x0000015C
    REG32                          RESERVED_REG26;               //0x00000160
    REG32                          RESERVED_REG27;               //0x00000164
    REG32                          SLP_CTRL;                     //0x00000168
    REG32                          SLP_DCDC_PD_CTRL;             //0x0000016C
    REG32                          SLP_LDO_PD_CTRL0;             //0x00000170
    REG32                          SLP_LDO_PD_CTRL1;             //0x00000174
    REG32                          SLP_DCDC_LP_CTRL;             //0x00000178
    REG32                          SLP_LDO_LP_CTRL0;             //0x0000017C
    REG32                          SLP_LDO_LP_CTRL1;             //0x00000180
    REG32                          DCDC_CORE_SLP_CTRL0;          //0x00000184
    REG32                          DCDC_CORE_SLP_CTRL1;          //0x00000188
    REG32                          DCDC_XTL_EN0;                 //0x0000018C
    REG32                          RESEVED_DCDC_XTL_EN4;         //0x00000190
    REG32                          LDO_XTL_EN0;                  //0x00000194
    REG32                          LDO_XTL_EN1;                  //0x00000198
    REG32                          LDO_XTL_EN2;                  //0x0000019C
    REG32                          LDO_XTL_EN3;                  //0x000001A0
    REG32                          LDO_XTL_EN4;                  //0x000001A4
    REG32                          LDO_XTL_EN5;                  //0x000001A8
    REG32                          LDO_XTL_EN6;                  //0x000001AC
    REG32                          LDO_XTL_EN7;                  //0x000001B0
    REG32                          LDO_XTL_EN8;                  //0x000001B4
    REG32                          LDO_XTL_EN9;                  //0x000001B8
    REG32                          LDO_XTL_EN10;                 //0x000001BC
    REG32                          RESERVED_REG30;               //0x000001C0
    REG32                          RESERVED_REG31;               //0x000001C4
    REG32                          RESERVED_REG32;               //0x000001C8
    REG32                          RESERVED_REG_CORE;            //0x000001CC
    REG32                          RESERVED_REG_RTC;             //0x000001D0
    REG32                          BG_CTRL;                      //0x000001D4
    REG32                          DCDC_VLG_SEL;                 //0x000001D8
    REG32                          LDO_VLG_SEL0;                 //0x000001DC
    REG32                          CLK32KLESS_CTRL0;             //0x000001E0
    REG32                          CLK32KLESS_CTRL1;             //0x000001E4
    REG32                          XTL_WAIT_CTRL0;               //0x000001E8
    REG32                          RGB_CTRL;                     //0x000001EC
    REG32                          IB_CTRL;                      //0x000001F0
    REG32                          FLASH_CTRL;                   //0x000001F4
    REG32                          KPLED_CTRL0;                  //0x000001F8
    REG32                          KPLED_CTRL1;                  //0x000001FC
    REG32                          VIBR_CTRL0;                   //0x00000200
    REG32                          VIBR_CTRL1;                   //0x00000204
    REG32                          AUDIO_CTRL0;                  //0x00000208
    REG32                          CHGR_CTRL0;                   //0x0000020C
    REG32                          CHGR_CTRL1;                   //0x00000210
    REG32                          CHGR_STATUS;                  //0x00000214
    REG32                          CHGR_DET_FGU_CTRL;            //0x00000218
    REG32                          OVLO_CTRL;                    //0x0000021C
    REG32                          MIXED_CTRL;                   //0x00000220
    REG32                          POR_RST_MONITOR;              //0x00000224
    REG32                          WDG_RST_MONITOR;              //0x00000228
    REG32                          POR_PIN_RST_MONITOR;          //0x0000022C
    REG32                          POR_SRC_FLAG;                 //0x00000230
    REG32                          POR_OFF_FLAG;                 //0x00000234
    REG32                          POR_7S_CTRL;                  //0x00000238
    REG32                          HWRST_RTC;                    //0x0000023C
    REG32                          ARCH_EN;                      //0x00000240
    REG32                          MCU_WR_PROT_VALUE;            //0x00000244
    REG32                          PWR_WR_PROT_VALUE;            //0x00000248
    REG32                          SMPL_CTRL0;                   //0x0000024C
    REG32                          SMPL_CTRL1;                   //0x00000250
    REG32                          RTC_RST0;                     //0x00000254
    REG32                          RTC_RST1;                     //0x00000258
    REG32                          RTC_RST2;                     //0x0000025C
    REG32                          RTC_CLK_STOP;                 //0x00000260
    REG32                          VBAT_DROP_CNT;                //0x00000264
    REG32                          SWRST_CTRL0;                  //0x00000268
    REG32                          SWRST_CTRL1;                  //0x0000026C
    REG32                          OTP_CTRL;                     //0x00000270
    REG32                          FREE_TIMER_LOW;               //0x00000274
    REG32                          FREE_TIMER_HIGH;              //0x00000278
    REG32                          VOL_TUNE_CTRL_CORE;           //0x0000027C
} HWP_RDA2720M_GLOBAL_T;

#define hwp_rda2720mGlobal         ((HWP_RDA2720M_GLOBAL_T*) REG_ACCESS_ADDRESS(REG_RDA2720M_GLOBAL_BASE))


//CHIP_ID_LOW
typedef union {
    REG32 v;
    struct {
        REG32 chip_ip_low : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_CHIP_ID_LOW_T;

//CHIP_ID_HIGH
typedef union {
    REG32 v;
    struct {
        REG32 chip_id_high : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_CHIP_ID_HIGH_T;

//MODULE_EN0
typedef union {
    REG32 v;
    struct {
        REG32 cal_en : 1; // [0]
        REG32 rtc_en : 1; // [1]
        REG32 wdg_en : 1; // [2]
        REG32 eic_en : 1; // [3]
        REG32 aud_en : 1; // [4]
        REG32 adc_en : 1; // [5]
        REG32 efs_en : 1; // [6]
        REG32 fgu_en : 1; // [7]
        REG32 pinreg_en : 1; // [8]
        REG32 bltc_en : 1; // [9]
        REG32 psm_en : 1; // [10], read only
        REG32 __11_11 : 1;
        REG32 tmr_en : 1; // [12]
        REG32 __13_13 : 1;
        REG32 typec_en : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_RDA2720M_GLOBAL_MODULE_EN0_T;

//ARM_CLK_EN0
typedef union {
    REG32 v;
    struct {
        REG32 clk_aud_if_en : 1; // [0]
        REG32 clk_aud_if_6p5m_en : 1; // [1]
        REG32 clk_cal_en : 1; // [2]
        REG32 clk_cal_src_sel : 2; // [4:3]
        REG32 clk_auxadc_en : 1; // [5]
        REG32 clk_auxad_en : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_ARM_CLK_EN0_T;

//RTC_CLK_EN0
typedef union {
    REG32 v;
    struct {
        REG32 rtc_arch_en : 1; // [0]
        REG32 rtc_rtc_en : 1; // [1]
        REG32 rtc_wdg_en : 1; // [2]
        REG32 rtc_eic_en : 1; // [3]
        REG32 __5_4 : 2;
        REG32 rtc_fgu_en : 1; // [6]
        REG32 rtc_bltc_en : 1; // [7]
        REG32 __10_8 : 3;
        REG32 rtc_efs_en : 1; // [11]
        REG32 rtc_flash_en : 1; // [12]
        REG32 rtc_tmr_en : 1; // [13]
        REG32 __14_14 : 1;
        REG32 rtc_typec_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_RTC_CLK_EN0_T;

//SOFT_RST0
typedef union {
    REG32 v;
    struct {
        REG32 cal_soft_rst : 1; // [0]
        REG32 rtc_soft_rst : 1; // [1]
        REG32 wdg_soft_rst : 1; // [2]
        REG32 eic_soft_rst : 1; // [3]
        REG32 fgu_soft_rst : 1; // [4]
        REG32 __5_5 : 1;
        REG32 adc_soft_rst : 1; // [6]
        REG32 efs_soft_rst : 1; // [7]
        REG32 aud_if_soft_rst : 1; // [8]
        REG32 bltc_soft_rst : 1; // [9]
        REG32 __11_10 : 2;
        REG32 audtx_soft_rst : 1; // [12]
        REG32 audrx_soft_rst : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_GLOBAL_SOFT_RST0_T;

//SOFT_RST1
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 tmr_soft_rst : 1; // [1]
        REG32 __2_2 : 1;
        REG32 typec_soft_rst : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_RDA2720M_GLOBAL_SOFT_RST1_T;

//POWER_PD_SW
typedef union {
    REG32 v;
    struct {
        REG32 bg_pd : 1; // [0]
        REG32 ldo_vdd28_pd : 1; // [1]
        REG32 ldo_avdd18_pd : 1; // [2]
        REG32 ldo_mem_pd : 1; // [3]
        REG32 osc3m_en : 1; // [4]
        REG32 dcdc_core_pd : 1; // [5]
        REG32 __6_6 : 1;
        REG32 dcdc_gen_pd : 1; // [7]
        REG32 ldo_cp_pd : 1; // [8]
        REG32 ldo_emm_pd : 1; // [9]
        REG32 ldo_dcxo_pd : 1; // [10]
        REG32 __31_11 : 21;
    } b;
} REG_RDA2720M_GLOBAL_POWER_PD_SW_T;

//POWER_PD_HW
typedef union {
    REG32 v;
    struct {
        REG32 pwr_off_seq_en : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_POWER_PD_HW_T;

//SOFT_RST_HW
typedef union {
    REG32 v;
    struct {
        REG32 reg_soft_rst : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_SOFT_RST_HW_T;

//DCDC_CLK_REG0
typedef union {
    REG32 v;
    struct {
        REG32 osc3m_freq : 4; // [3:0]
        REG32 dcdc_gen_clk_rst : 1; // [4]
        REG32 __6_5 : 2;
        REG32 clkout_sel : 3; // [9:7]
        REG32 clkout_uniphase : 1; // [10]
        REG32 clk3m_out_en : 1; // [11]
        REG32 __31_12 : 20;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_CLK_REG0_T;

//DCDC_CORE_REG0
typedef union {
    REG32 v;
    struct {
        REG32 zx_offset_vcore : 2; // [1:0]
        REG32 sr_ls_vcore : 2; // [3:2]
        REG32 sr_hs_vcore : 2; // [5:4]
        REG32 slope_vcore : 2; // [7:6]
        REG32 rcomp_vcore : 2; // [9:8]
        REG32 pfm_vh_vcore : 2; // [11:10]
        REG32 curses_r_vcore : 2; // [13:12]
        REG32 curlimit_r_vcore : 2; // [15:14]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_CORE_REG0_T;

//DCDC_CORE_REG1
typedef union {
    REG32 v;
    struct {
        REG32 anriting_en_vcore : 1; // [0]
        REG32 force_pwm_vcore : 1; // [1]
        REG32 zx_disable_vcore : 1; // [2]
        REG32 votrim_vcore : 3; // [5:3]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_CORE_REG1_T;

//DCDC_CORE_REG2
typedef union {
    REG32 v;
    struct {
        REG32 div_base_vcore : 6; // [5:0]
        REG32 phase_sel_vcore : 6; // [11:6]
        REG32 div_clk_vcore_en : 1; // [12]
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_CORE_REG2_T;

//DCDC_CORE_VOL
typedef union {
    REG32 v;
    struct {
        REG32 vosel_vcore : 9; // [8:0]
        REG32 __31_9 : 23;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_CORE_VOL_T;

//DCDC_GEN_REG0
typedef union {
    REG32 v;
    struct {
        REG32 zx_offset_vgen : 2; // [1:0]
        REG32 sr_ls_vgen : 2; // [3:2]
        REG32 sr_hs_vgen : 2; // [5:4]
        REG32 slope_vgen : 2; // [7:6]
        REG32 rcomp_vgen : 2; // [9:8]
        REG32 pfm_vh_vgen : 2; // [11:10]
        REG32 curses_r_vgen : 2; // [13:12]
        REG32 curlimit_r_vgen : 2; // [15:14]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_GEN_REG0_T;

//DCDC_GEN_REG1
typedef union {
    REG32 v;
    struct {
        REG32 anriting_en_vgen : 1; // [0]
        REG32 force_pwm_vgen : 1; // [1]
        REG32 maxduty_sel_vgen : 1; // [2]
        REG32 zx_disable_vgen : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_GEN_REG1_T;

//DCDC_GEN_REG2
typedef union {
    REG32 v;
    struct {
        REG32 div_base_vgen : 6; // [5:0]
        REG32 phase_sel_vgen : 6; // [11:6]
        REG32 div_clk_vgen_en : 1; // [12]
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_GEN_REG2_T;

//DCDC_GEN_VOL
typedef union {
    REG32 v;
    struct {
        REG32 vosel_vgen : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_GEN_VOL_T;

//DCDC_WPA_REG0
typedef union {
    REG32 v;
    struct {
        REG32 sr_hs_vpa : 2; // [1:0]
        REG32 sawtooth_slope_vpa : 2; // [3:2]
        REG32 rcomp3_vpa : 2; // [5:4]
        REG32 rcomp2_vpa : 2; // [7:6]
        REG32 pfm_threshold_vpa : 2; // [9:8]
        REG32 ccomp3_vpa : 2; // [11:10]
        REG32 curses_m_vpa : 2; // [13:12]
        REG32 curlimit_r_vpa : 2; // [15:14]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_WPA_REG0_T;

//DCDC_WPA_REG1
typedef union {
    REG32 v;
    struct {
        REG32 maxduty_sel_vpa : 1; // [0]
        REG32 dvs_on_vpa : 1; // [1]
        REG32 bypass_threshold_vpa : 2; // [3:2]
        REG32 bypass_forceon_vpa : 1; // [4]
        REG32 bypass_disable_vpa : 1; // [5]
        REG32 apc_ramp_sel_vpa : 1; // [6]
        REG32 apc_enable_vpa : 1; // [7]
        REG32 anriting_en_vpa : 1; // [8]
        REG32 zx_offset_vpa : 2; // [10:9]
        REG32 votrim_vpa : 3; // [13:11]
        REG32 sr_ls_vpa : 2; // [15:14]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_WPA_REG1_T;

//DCDC_WPA_REG2
typedef union {
    REG32 v;
    struct {
        REG32 pd_buck_vpa : 1; // [0]
        REG32 sawtoothcal_rst_vpa : 1; // [1]
        REG32 zx_disable_vpa : 1; // [2]
        REG32 div_base_vpa : 6; // [8:3]
        REG32 phase_sel_vpa : 6; // [14:9]
        REG32 div_clk_vpa_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_WPA_REG2_T;

//DCDC_WPA_VOL
typedef union {
    REG32 v;
    struct {
        REG32 vosel_vpa : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_WPA_VOL_T;

//DCDC_WPA_DCM_HW
typedef union {
    REG32 v;
    struct {
        REG32 force_pwm_vpa : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_WPA_DCM_HW_T;

//DCDC_CH_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dcdc_auxtrim_sel : 3; // [2:0]
        REG32 __31_3 : 29;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_CH_CTRL_T;

//LDO_AVDD18_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_avdd18_cl_adj : 1; // [1]
        REG32 ldo_avdd18_shpt_adj : 1; // [2]
        REG32 ldo_avdd18_stb : 2; // [4:3]
        REG32 ldo_avdd18_bp : 1; // [5]
        REG32 ldo_avdd18_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_AVDD18_REG0_T;

//LDO_AVDD18_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_avdd18_v : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_LDO_AVDD18_REG1_T;

//LDO_CAMIO_REG0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_camio_pd : 1; // [0]
        REG32 ldo_camio_cl_adj : 1; // [1]
        REG32 ldo_camio_shpt_adj : 1; // [2]
        REG32 ldo_camio_stb : 2; // [4:3]
        REG32 ldo_camio_bp : 1; // [5]
        REG32 ldo_camio_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_CAMIO_REG0_T;

//LDO_CAMIO_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_camio_v : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_LDO_CAMIO_REG1_T;

//LDO_RF18A_REG0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_rf18a_pd : 1; // [0]
        REG32 ldo_rf18a_cl_adj : 1; // [1]
        REG32 ldo_rf18a_shpt_adj : 1; // [2]
        REG32 ldo_rf18a_stb : 2; // [4:3]
        REG32 ldo_rf18a_bp : 1; // [5]
        REG32 ldo_rf18a_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_RF18A_REG0_T;

//LDO_RF18A_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_rf18a_v : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_LDO_RF18A_REG1_T;

//LDO_RF18B_REG0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_rf18b_pd : 1; // [0]
        REG32 ldo_rf18b_cl_adj : 1; // [1]
        REG32 ldo_rf18b_shpt_adj : 1; // [2]
        REG32 ldo_rf18b_stb : 2; // [4:3]
        REG32 ldo_rf18b_bp : 1; // [5]
        REG32 ldo_rf18b_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_RF18B_REG0_T;

//LDO_RF18B_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_rf18b_v : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_LDO_RF18B_REG1_T;

//LDO_CAMD_REG0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_camd_pd : 1; // [0]
        REG32 ldo_camd_cl_adj : 1; // [1]
        REG32 ldo_camd_shpt_adj : 1; // [2]
        REG32 ldo_camd_stb : 2; // [4:3]
        REG32 ldo_camd_bp : 1; // [5]
        REG32 ldo_camd_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_CAMD_REG0_T;

//LDO_CAMD_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_camd_v : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_LDO_CAMD_REG1_T;

//LDO_CON_REG0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_con_pd : 1; // [0]
        REG32 ldo_con_cl_adj : 1; // [1]
        REG32 ldo_con_shpt_adj : 1; // [2]
        REG32 ldo_con_stb : 2; // [4:3]
        REG32 ldo_con_bp : 1; // [5]
        REG32 ldo_con_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_CON_REG0_T;

//LDO_CON_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_con_v : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_LDO_CON_REG1_T;

//LDO_MEM_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_mem_cl_adj : 1; // [1]
        REG32 ldo_mem_shpt_adj : 1; // [2]
        REG32 ldo_mem_stb : 2; // [4:3]
        REG32 ldo_mem_bp : 1; // [5]
        REG32 ldo_mem_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_MEM_REG0_T;

//LDO_MEM_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_mem_v : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_LDO_MEM_REG1_T;

//LDO_SIM0_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_sim0_cl_adj : 1; // [1]
        REG32 ldo_sim0_shpt_adj : 1; // [2]
        REG32 ldo_sim0_stb : 2; // [4:3]
        REG32 ldo_sim0_cap_sel : 1; // [5]
        REG32 ldo_sim0_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SIM0_REG0_T;

//LDO_SIM0_PD_REG
typedef union {
    REG32 v;
    struct {
        REG32 ldo_sim0_pd : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SIM0_PD_REG_T;

//LDO_SIM0_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_sim0_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SIM0_REG1_T;

//LDO_SIM1_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_sim1_cl_adj : 1; // [1]
        REG32 ldo_sim1_shpt_adj : 1; // [2]
        REG32 ldo_sim1_stb : 2; // [4:3]
        REG32 ldo_sim1_cap_sel : 1; // [5]
        REG32 ldo_sim1_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SIM1_REG0_T;

//LDO_SIM1_PD_REG
typedef union {
    REG32 v;
    struct {
        REG32 ldo_sim1_pd : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SIM1_PD_REG_T;

//LDO_SIM1_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_sim1_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SIM1_REG1_T;

//LDO_SIM2_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_sim2_cl_adj : 1; // [1]
        REG32 ldo_sim2_shpt_adj : 1; // [2]
        REG32 ldo_sim2_stb : 2; // [4:3]
        REG32 ldo_sim2_cap_sel : 1; // [5]
        REG32 ldo_sim2_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SIM2_REG0_T;

//LDO_SIM2_PD_REG
typedef union {
    REG32 v;
    struct {
        REG32 ldo_sim2_pd : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SIM2_PD_REG_T;

//LDO_SIM2_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_sim2_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SIM2_REG1_T;

//LDO_CAMA_REG0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_cama_pd : 1; // [0]
        REG32 ldo_cama_cl_adj : 1; // [1]
        REG32 ldo_cama_shpt_adj : 1; // [2]
        REG32 ldo_cama_stb : 2; // [4:3]
        REG32 ldo_cama_cap_sel : 1; // [5]
        REG32 ldo_cama_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_CAMA_REG0_T;

//LDO_CAMA_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_cama_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_CAMA_REG1_T;

//LDO_LCD_REG0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_lcd_pd : 1; // [0]
        REG32 ldo_lcd_cl_adj : 1; // [1]
        REG32 ldo_lcd_shpt_adj : 1; // [2]
        REG32 ldo_lcd_stb : 2; // [4:3]
        REG32 ldo_lcd_cap_sel : 1; // [5]
        REG32 ldo_lcd_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_LCD_REG0_T;

//LDO_LCD_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_lcd_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_LCD_REG1_T;

//LDO_EMMCCORE_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_emmccore_cl_adj : 1; // [1]
        REG32 ldo_emmccore_shpt_adj : 1; // [2]
        REG32 ldo_emmccore_stb : 2; // [4:3]
        REG32 ldo_emmccore_cap_sel : 1; // [5]
        REG32 ldo_emmccore_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_EMMCCORE_REG0_T;

//LDO_EMMCCORE_PD_REG
typedef union {
    REG32 v;
    struct {
        REG32 ldo_emmccore_pd : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_LDO_EMMCCORE_PD_REG_T;

//LDO_EMMCCORE_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_emmccore_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_EMMCCORE_REG1_T;

//LDO_SD_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_vio18_cl_adj : 1; // [1]
        REG32 ldo_vio18_shpt_adj : 1; // [2]
        REG32 ldo_vio18_stb : 2; // [4:3]
        REG32 ldo_vio18_cap_sel : 1; // [5]
        REG32 ldo_vio18_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SD_REG0_T;

//LDO_SD_PD_REG
typedef union {
    REG32 v;
    struct {
        REG32 ldo_vio18_pd : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SD_PD_REG_T;

//LDO_SD_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_vio18_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SD_REG1_T;

//LDO_DDR12_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_ddr12_cl_adj : 1; // [1]
        REG32 ldo_ddr12_shpt_adj : 1; // [2]
        REG32 ldo_ddr12_stb : 2; // [4:3]
        REG32 ldo_ddr12_cap_sel : 1; // [5]
        REG32 ldo_ddr12_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_DDR12_REG0_T;

//LDO_DDR12_PD_REG
typedef union {
    REG32 v;
    struct {
        REG32 ldo_ddr12_pd : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_LDO_DDR12_PD_REG_T;

//LDO_DDR12_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_ddr12_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_DDR12_REG1_T;

//LDO_VDD28_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_vdd28_cl_adj : 1; // [1]
        REG32 ldo_vdd28_shpt_adj : 1; // [2]
        REG32 ldo_vdd28_stb : 2; // [4:3]
        REG32 ldo_vdd28_cap_sel : 1; // [5]
        REG32 ldo_vdd28_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_VDD28_REG0_T;

//LDO_VDD28_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_vdd28_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_VDD28_REG1_T;

//LDO_SPIMEM_REG0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_spimem_pd : 1; // [0]
        REG32 ldo_spimem_cl_adj : 1; // [1]
        REG32 ldo_spimem_shpt_adj : 1; // [2]
        REG32 ldo_spimem_stb : 2; // [4:3]
        REG32 ldo_spimem_cap_sel : 1; // [5]
        REG32 ldo_spimem_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SPIMEM_REG0_T;

//LDO_SPIMEM_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_spimem_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_SPIMEM_REG1_T;

//LDO_DCXO_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_dcxo_cl_adj : 1; // [1]
        REG32 ldo_dcxo_shpt_adj : 1; // [2]
        REG32 ldo_dcxo_stb : 2; // [4:3]
        REG32 ldo_dcxo_cap_sel : 1; // [5]
        REG32 ldo_dcxo_shpt_pd : 1; // [6]
        REG32 ldo_dcxo_trim : 5; // [11:7]
        REG32 __31_12 : 20;
    } b;
} REG_RDA2720M_GLOBAL_LDO_DCXO_REG0_T;

//LDO_DCXO_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_dcxo_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_DCXO_REG1_T;

//LDO_USB_REG0
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 ldo_usb33_cl_adj : 1; // [1]
        REG32 ldo_usb33_shpt_adj : 1; // [2]
        REG32 ldo_usb33_stb : 2; // [4:3]
        REG32 ldo_usb33_cap_sel : 1; // [5]
        REG32 ldo_usb33_shpt_pd : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_USB_REG0_T;

//LDO_USB_PD_REG
typedef union {
    REG32 v;
    struct {
        REG32 ldo_usb33_pd : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_LDO_USB_PD_REG_T;

//LDO_USB_REG1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_usb33_v : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_RDA2720M_GLOBAL_LDO_USB_REG1_T;

//LDO_TRIM_REG
typedef union {
    REG32 v;
    struct {
        REG32 ldo_trim_a : 5; // [4:0]
        REG32 ldo_trim_b : 5; // [9:5]
        REG32 __31_10 : 22;
    } b;
} REG_RDA2720M_GLOBAL_LDO_TRIM_REG_T;

//LDO_RTC_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 vbatbk_v : 2; // [1:0]
        REG32 ldo_rtc_v : 2; // [3:2]
        REG32 ldo_rtc_cal : 5; // [8:4]
        REG32 __31_9 : 23;
    } b;
} REG_RDA2720M_GLOBAL_LDO_RTC_CTRL_T;

//LDO_CH_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 ldo_cal : 3; // [2:0]
        REG32 __4_3 : 2;
        REG32 ldo_cal1 : 3; // [7:5]
        REG32 ldo_cal2 : 3; // [10:8]
        REG32 __31_11 : 21;
    } b;
} REG_RDA2720M_GLOBAL_LDO_CH_CTRL_T;

//SLP_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 slp_ldo_pd_en : 1; // [0]
        REG32 slp_io_en : 1; // [1]
        REG32 ldo_xtl_en : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_RDA2720M_GLOBAL_SLP_CTRL_T;

//SLP_DCDC_PD_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 slp_vddio1v8_pd_en : 1; // [1]
        REG32 slp_dcdcwpa_pd_en : 1; // [2]
        REG32 slp_dcdccore_drop_en : 1; // [3]
        REG32 slp_dcdccore_pd_en : 1; // [4]
        REG32 __5_5 : 1;
        REG32 slp_dcdccore_pu_rstn_th : 6; // [11:6]
        REG32 slp_dcdccore_pd_rstn_th : 4; // [15:12]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_SLP_DCDC_PD_CTRL_T;

//SLP_LDO_PD_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 slp_ldosim1_pd_en : 1; // [0]
        REG32 slp_ldosim2_pd_en : 1; // [1]
        REG32 slp_ldocama_pd_en : 1; // [2]
        REG32 slp_ldocamd_pd_en : 1; // [3]
        REG32 slp_ldocamio_pd_en : 1; // [4]
        REG32 slp_ldolcd_pd_en : 1; // [5]
        REG32 slp_ldousb33_pd_en : 1; // [6]
        REG32 slp_ldoddr12_pd_en : 1; // [7]
        REG32 slp_ldovio18_pd_en : 1; // [8]
        REG32 slp_ldovdd28_pd_en : 1; // [9]
        REG32 slp_ldospimem_pd_en : 1; // [10]
        REG32 slp_ldodcxo_pd_en : 1; // [11]
        REG32 slp_ldoemmccore_pd_en : 1; // [12]
        REG32 slp_ldorf18b_pd_en : 1; // [13]
        REG32 slp_ldorf18a_pd_en : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_RDA2720M_GLOBAL_SLP_LDO_PD_CTRL0_T;

//SLP_LDO_PD_CTRL1
typedef union {
    REG32 v;
    struct {
        REG32 slp_ldomem_pd_en : 1; // [0]
        REG32 slp_ldoavdd18_pd_en : 1; // [1]
        REG32 slp_ldosim0_pd_en : 1; // [2]
        REG32 slp_ldocon_pd_en : 1; // [3]
        REG32 slp_ldocp_pd_en : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_RDA2720M_GLOBAL_SLP_LDO_PD_CTRL1_T;

//SLP_DCDC_LP_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 slp_dcdcwpa_lp_en : 1; // [0]
        REG32 slp_dcdcgen_lp_en : 1; // [1]
        REG32 __3_2 : 2;
        REG32 slp_dcdccore_lp_en : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_RDA2720M_GLOBAL_SLP_DCDC_LP_CTRL_T;

//SLP_LDO_LP_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 slp_ldosim1_lp_en : 1; // [0]
        REG32 slp_ldosim2_lp_en : 1; // [1]
        REG32 slp_ldocama_lp_en : 1; // [2]
        REG32 slp_ldocamd_lp_en : 1; // [3]
        REG32 slp_ldocamio_lp_en : 1; // [4]
        REG32 slp_ldolcd_lp_en : 1; // [5]
        REG32 slp_ldousb33_lp_en : 1; // [6]
        REG32 slp_ldoddr12_lp_en : 1; // [7]
        REG32 slp_ldovio18_lp_en : 1; // [8]
        REG32 slp_ldovdd28_lp_en : 1; // [9]
        REG32 slp_ldospimem_lp_en : 1; // [10]
        REG32 slp_ldodcxo_lp_en : 1; // [11]
        REG32 slp_ldoemmccore_lp_en : 1; // [12]
        REG32 slp_ldorf18b_lp_en : 1; // [13]
        REG32 slp_ldorf18a_lp_en : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_RDA2720M_GLOBAL_SLP_LDO_LP_CTRL0_T;

//SLP_LDO_LP_CTRL1
typedef union {
    REG32 v;
    struct {
        REG32 slp_ldomem_lp_en : 1; // [0]
        REG32 slp_ldoavdd18_lp_en : 1; // [1]
        REG32 slp_ldosim0_lp_en : 1; // [2]
        REG32 slp_ldocon_lp_en : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_RDA2720M_GLOBAL_SLP_LDO_LP_CTRL1_T;

//DCDC_CORE_SLP_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 dcdc_core_slp_step_en : 1; // [0]
        REG32 __2_1 : 2;
        REG32 dcdc_core_slp_step_vol : 5; // [7:3]
        REG32 dcdc_core_slp_step_num : 4; // [11:8]
        REG32 dcdc_core_slp_step_delay : 2; // [13:12]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_CORE_SLP_CTRL0_T;

//DCDC_CORE_SLP_CTRL1
typedef union {
    REG32 v;
    struct {
        REG32 dcdc_core_vosel_ds_sw : 9; // [8:0]
        REG32 __31_9 : 23;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_CORE_SLP_CTRL1_T;

//DCDC_XTL_EN0
typedef union {
    REG32 v;
    struct {
        REG32 dcdc_wpa_ext_xtl3_en : 1; // [0]
        REG32 dcdc_wpa_ext_xtl2_en : 1; // [1]
        REG32 dcdc_wpa_ext_xtl1_en : 1; // [2]
        REG32 dcdc_wpa_ext_xtl0_en : 1; // [3]
        REG32 dcdc_gen_ext_xtl3_en : 1; // [4]
        REG32 dcdc_gen_ext_xtl2_en : 1; // [5]
        REG32 dcdc_gen_ext_xtl1_en : 1; // [6]
        REG32 dcdc_gen_ext_xtl0_en : 1; // [7]
        REG32 __11_8 : 4;
        REG32 dcdc_core_ext_xtl3_en : 1; // [12]
        REG32 dcdc_core_ext_xtl2_en : 1; // [13]
        REG32 dcdc_core_ext_xtl1_en : 1; // [14]
        REG32 dcdc_core_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_XTL_EN0_T;

//LDO_XTL_EN0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_vdd28_ext_xtl3_en : 1; // [0]
        REG32 ldo_vdd28_ext_xtl2_en : 1; // [1]
        REG32 ldo_vdd28_ext_xtl1_en : 1; // [2]
        REG32 ldo_vdd28_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_dcxo_ext_xtl3_en : 1; // [12]
        REG32 ldo_dcxo_ext_xtl2_en : 1; // [13]
        REG32 ldo_dcxo_ext_xtl1_en : 1; // [14]
        REG32 ldo_dcxo_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN0_T;

//LDO_XTL_EN1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_rf18b_ext_xtl3_en : 1; // [0]
        REG32 ldo_rf18b_ext_xtl2_en : 1; // [1]
        REG32 ldo_rf18b_ext_xtl1_en : 1; // [2]
        REG32 ldo_rf18b_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_rf18a_ext_xtl3_en : 1; // [12]
        REG32 ldo_rf18a_ext_xtl2_en : 1; // [13]
        REG32 ldo_rf18a_ext_xtl1_en : 1; // [14]
        REG32 ldo_rf18a_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN1_T;

//LDO_XTL_EN2
typedef union {
    REG32 v;
    struct {
        REG32 ldo_sim1_ext_xtl3_en : 1; // [0]
        REG32 ldo_sim1_ext_xtl2_en : 1; // [1]
        REG32 ldo_sim1_ext_xtl1_en : 1; // [2]
        REG32 ldo_sim1_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_sim0_ext_xtl3_en : 1; // [12]
        REG32 ldo_sim0_ext_xtl2_en : 1; // [13]
        REG32 ldo_sim0_ext_xtl1_en : 1; // [14]
        REG32 ldo_sim0_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN2_T;

//LDO_XTL_EN3
typedef union {
    REG32 v;
    struct {
        REG32 ldo_mem_ext_xtl3_en : 1; // [0]
        REG32 ldo_mem_ext_xtl2_en : 1; // [1]
        REG32 ldo_mem_ext_xtl1_en : 1; // [2]
        REG32 ldo_mem_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_sim2_ext_xtl3_en : 1; // [12]
        REG32 ldo_sim2_ext_xtl2_en : 1; // [13]
        REG32 ldo_sim2_ext_xtl1_en : 1; // [14]
        REG32 ldo_sim2_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN3_T;

//LDO_XTL_EN4
typedef union {
    REG32 v;
    struct {
        REG32 ldo_camio_ext_xtl3_en : 1; // [0]
        REG32 ldo_camio_ext_xtl2_en : 1; // [1]
        REG32 ldo_camio_ext_xtl1_en : 1; // [2]
        REG32 ldo_camio_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_lcd_ext_xtl3_en : 1; // [12]
        REG32 ldo_lcd_ext_xtl2_en : 1; // [13]
        REG32 ldo_lcd_ext_xtl1_en : 1; // [14]
        REG32 ldo_lcd_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN4_T;

//LDO_XTL_EN5
typedef union {
    REG32 v;
    struct {
        REG32 ldo_camd_ext_xtl3_en : 1; // [0]
        REG32 ldo_camd_ext_xtl2_en : 1; // [1]
        REG32 ldo_camd_ext_xtl1_en : 1; // [2]
        REG32 ldo_camd_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_cama_ext_xtl3_en : 1; // [12]
        REG32 ldo_cama_ext_xtl2_en : 1; // [13]
        REG32 ldo_cama_ext_xtl1_en : 1; // [14]
        REG32 ldo_cama_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN5_T;

//LDO_XTL_EN6
typedef union {
    REG32 v;
    struct {
        REG32 ldo_vio18_ext_xtl3_en : 1; // [0]
        REG32 ldo_vio18_ext_xtl2_en : 1; // [1]
        REG32 ldo_vio18_ext_xtl1_en : 1; // [2]
        REG32 ldo_vio18_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_ddr12_ext_xtl3_en : 1; // [12]
        REG32 ldo_ddr12_ext_xtl2_en : 1; // [13]
        REG32 ldo_ddr12_ext_xtl1_en : 1; // [14]
        REG32 ldo_ddr12_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN6_T;

//LDO_XTL_EN7
typedef union {
    REG32 v;
    struct {
        REG32 ldo_usb33_ext_xtl3_en : 1; // [0]
        REG32 ldo_usb33_ext_xtl2_en : 1; // [1]
        REG32 ldo_usb33_ext_xtl1_en : 1; // [2]
        REG32 ldo_usb33_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_emmccore_ext_xtl3_en : 1; // [12]
        REG32 ldo_emmccore_ext_xtl2_en : 1; // [13]
        REG32 ldo_emmccore_ext_xtl1_en : 1; // [14]
        REG32 ldo_emmccore_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN7_T;

//LDO_XTL_EN8
typedef union {
    REG32 v;
    struct {
        REG32 ldo_vibr_ext_xtl3_en : 1; // [0]
        REG32 ldo_vibr_ext_xtl2_en : 1; // [1]
        REG32 ldo_vibr_ext_xtl1_en : 1; // [2]
        REG32 ldo_vibr_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_kpled_ext_xtl3_en : 1; // [12]
        REG32 ldo_kpled_ext_xtl2_en : 1; // [13]
        REG32 ldo_kpled_ext_xtl1_en : 1; // [14]
        REG32 ldo_kpled_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN8_T;

//LDO_XTL_EN9
typedef union {
    REG32 v;
    struct {
        REG32 ldo_avdd18_ext_xtl3_en : 1; // [0]
        REG32 ldo_avdd18_ext_xtl2_en : 1; // [1]
        REG32 ldo_avdd18_ext_xtl1_en : 1; // [2]
        REG32 ldo_avdd18_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_con_ext_xtl3_en : 1; // [12]
        REG32 ldo_con_ext_xtl2_en : 1; // [13]
        REG32 ldo_con_ext_xtl1_en : 1; // [14]
        REG32 ldo_con_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN9_T;

//LDO_XTL_EN10
typedef union {
    REG32 v;
    struct {
        REG32 ldo_spimem_ext_xtl3_en : 1; // [0]
        REG32 ldo_spimem_ext_xtl2_en : 1; // [1]
        REG32 ldo_spimem_ext_xtl1_en : 1; // [2]
        REG32 ldo_spimem_ext_xtl0_en : 1; // [3]
        REG32 __11_4 : 8;
        REG32 ldo_cp_ext_xtl3_en : 1; // [12]
        REG32 ldo_cp_ext_xtl2_en : 1; // [13]
        REG32 ldo_cp_ext_xtl1_en : 1; // [14]
        REG32 ldo_cp_ext_xtl0_en : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_LDO_XTL_EN10_T;

//BG_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 bg_chop_en : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_BG_CTRL_T;

//DCDC_VLG_SEL
typedef union {
    REG32 v;
    struct {
        REG32 dcdc_core_nor_sw_sel : 1; // [0]
        REG32 dcdc_core_slp_sw_sel : 1; // [1]
        REG32 dcdc_core_votrim_sw_sel : 1; // [2]
        REG32 dcdc_gen_sw_sel : 1; // [3]
        REG32 dcdc_wpa_sw_sel : 1; // [4]
        REG32 dcdc_wpa_votrim_sw_sel : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_DCDC_VLG_SEL_T;

//LDO_VLG_SEL0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_trim_b_sw_sel : 1; // [0]
        REG32 ldo_trim_a_sw_sel : 1; // [1]
        REG32 ldo_dcxo_sw_sel : 1; // [2]
        REG32 ldo_rtc_cal_sw_sel : 1; // [3]
        REG32 ldo_vibr_sw_sel : 1; // [4]
        REG32 ldo_kpled_sw_sel : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_LDO_VLG_SEL0_T;

//CLK32KLESS_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 rc_32k_en : 1; // [0]
        REG32 rc_32k_sel : 1; // [1]
        REG32 __3_2 : 2;
        REG32 rtc_mode : 1; // [4], read only
        REG32 __5_5 : 1;
        REG32 ldo_dcxo_lp_en_rtcclr : 1; // [6]
        REG32 ldo_dcxo_lp_en_rtcset : 1; // [7]
        REG32 __9_8 : 2;
        REG32 rc_mode_wr_ack_flag_clr : 1; // [10], write clear
        REG32 __13_11 : 3;
        REG32 rc_mode_wr_ack_flag : 1; // [14], read only
        REG32 __31_15 : 17;
    } b;
} REG_RDA2720M_GLOBAL_CLK32KLESS_CTRL0_T;

//CLK32KLESS_CTRL1
typedef union {
    REG32 v;
    struct {
        REG32 rc_mode : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_CLK32KLESS_CTRL1_T;

//XTL_WAIT_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 xtl_wait : 8; // [7:0]
        REG32 xtl_en : 1; // [8]
        REG32 slp_xtlbuf_pd_en : 1; // [9]
        REG32 ext_xtl0_for_26m_en : 1; // [10]
        REG32 ext_xtl1_for_26m_en : 1; // [11]
        REG32 ext_xtl2_for_26m_en : 1; // [12]
        REG32 ext_xtl3_for_26m_en : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_GLOBAL_XTL_WAIT_CTRL0_T;

//RGB_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 rgb_pd_sw : 1; // [0]
        REG32 rgb_pd_hw_en : 1; // [1]
        REG32 slp_rgb_pd_en : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_RDA2720M_GLOBAL_RGB_CTRL_T;

//IB_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 ib_rex_en : 1; // [0]
        REG32 ib_trim_em_sel : 1; // [1]
        REG32 ib_trim : 7; // [8:2]
        REG32 batdet_cur_i : 3; // [11:9]
        REG32 __12_12 : 1;
        REG32 batdet_cur_en : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_GLOBAL_IB_CTRL_T;

//FLASH_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 flash_v_sw : 4; // [3:0]
        REG32 flash_v_hw_step : 2; // [5:4]
        REG32 flash_v_hw_en : 1; // [6]
        REG32 __14_7 : 8;
        REG32 flash_pon : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_FLASH_CTRL_T;

//KPLED_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_kpled_cl_adj : 1; // [0]
        REG32 ldo_kpled_shpt_adj : 1; // [1]
        REG32 ldo_kpled_stb : 2; // [3:2]
        REG32 ldo_kpled_cap_sel : 1; // [4]
        REG32 __8_5 : 4;
        REG32 slp_ldokpled_pd_en : 1; // [9]
        REG32 kpled_pulldown_en : 1; // [10]
        REG32 kpled_pd : 1; // [11]
        REG32 kpled_v : 4; // [15:12]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_KPLED_CTRL0_T;

//KPLED_CTRL1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_kpled_shpt_pd : 1; // [0]
        REG32 __1_1 : 1;
        REG32 ldo_kpled_reftrim : 5; // [6:2]
        REG32 ldo_kpled_v : 3; // [9:7]
        REG32 __14_10 : 5;
        REG32 ldo_kpled_pd : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_KPLED_CTRL1_T;

//VIBR_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 ldo_vibr_v : 3; // [2:0]
        REG32 __7_3 : 5;
        REG32 ldo_vibr_pd : 1; // [8]
        REG32 slp_ldovibr_pd_en : 1; // [9]
        REG32 ldo_vibr_shpt_pd : 1; // [10]
        REG32 ldo_vibr_cl_adj : 1; // [11]
        REG32 ldo_vibr_shpt_adj : 1; // [12]
        REG32 ldo_vibr_stb : 2; // [14:13]
        REG32 ldo_vibr_cap_sel : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_VIBR_CTRL0_T;

//VIBR_CTRL1
typedef union {
    REG32 v;
    struct {
        REG32 ldo_vibr_eadbias_en : 1; // [0]
        REG32 ldo_vibr_reftrim : 5; // [5:1]
        REG32 __31_6 : 26;
    } b;
} REG_RDA2720M_GLOBAL_VIBR_CTRL1_T;

//AUDIO_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 clk_aud_if_6p5m_rx_inv_en : 1; // [0]
        REG32 clk_aud_if_6p5m_tx_inv_en : 1; // [1]
        REG32 clk_aud_if_rx_inv_en : 1; // [2]
        REG32 clk_aud_if_tx_inv_en : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_RDA2720M_GLOBAL_AUDIO_CTRL0_T;

//CHGR_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 chgr_pd : 1; // [0]
        REG32 chgr_iterm : 2; // [2:1]
        REG32 chgr_end_v : 2; // [4:3]
        REG32 chgr_cv_v : 6; // [10:5]
        REG32 __11_11 : 1;
        REG32 chgr_cc_en : 1; // [12]
        REG32 chgr_dpm : 2; // [14:13]
        REG32 __31_15 : 17;
    } b;
} REG_RDA2720M_GLOBAL_CHGR_CTRL0_T;

//CHGR_CTRL1
typedef union {
    REG32 v;
    struct {
        REG32 vchg_ovp_v : 2; // [1:0]
        REG32 __9_2 : 8;
        REG32 chgr_cc_i : 4; // [13:10]
        REG32 __14_14 : 1;
        REG32 chgr_expower_device : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_CHGR_CTRL1_T;

//CHGR_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 vchg_ovi : 1; // [0], read only
        REG32 dcp_switch_en : 1; // [1]
        REG32 chgr_int : 1; // [2], read only
        REG32 chgr_on : 1; // [3], read only
        REG32 chgr_cv_status : 1; // [4], read only
        REG32 cdp_int : 1; // [5], read only
        REG32 dcp_int : 1; // [6], read only
        REG32 sdp_int : 1; // [7], read only
        REG32 chg_det : 1; // [8], read only
        REG32 dcp_det : 1; // [9], read only
        REG32 dp_low : 1; // [10], read only
        REG32 chg_det_done : 1; // [11], read only
        REG32 non_dcp_int : 1; // [12], read only
        REG32 chgr_int_en : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_GLOBAL_CHGR_STATUS_T;

//CHGR_DET_FGU_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 dp_dm_bc_enb : 1; // [0]
        REG32 dp_dm_aux_en : 1; // [1]
        REG32 sd_dcoffset_a_en : 1; // [2]
        REG32 sd_dcoffset_b_en : 1; // [3]
        REG32 sd_chop_en : 1; // [4]
        REG32 sd_adc0_rc_pd : 1; // [5]
        REG32 sd_clk_p : 2; // [7:6]
        REG32 sd_chop_cap_en : 1; // [8]
        REG32 chg_int_delay : 3; // [11:9]
        REG32 ldo_fgu_pd : 1; // [12]
        REG32 fgua_soft_rst : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_GLOBAL_CHGR_DET_FGU_CTRL_T;

//OVLO_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 ovlo_t : 2; // [1:0]
        REG32 ovlo_v : 2; // [3:2]
        REG32 __8_4 : 5;
        REG32 ovlo_en : 1; // [9]
        REG32 vbat_crash_v : 2; // [11:10]
        REG32 __31_12 : 20;
    } b;
} REG_RDA2720M_GLOBAL_OVLO_CTRL_T;

//MIXED_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 int_debug_en : 1; // [0]
        REG32 all_int_deb : 1; // [1]
        REG32 gpi_debug_en : 1; // [2]
        REG32 all_gpi_deb : 1; // [3]
        REG32 vbat_ok : 1; // [4], read only
        REG32 __7_5 : 3;
        REG32 batdet_ok : 1; // [8], read only
        REG32 vpp_5v_sel : 1; // [9]
        REG32 baton_t : 2; // [11:10]
        REG32 xosc32k_ctl : 1; // [12]
        REG32 __31_13 : 19;
    } b;
} REG_RDA2720M_GLOBAL_MIXED_CTRL_T;

//POR_RST_MONITOR
typedef union {
    REG32 v;
    struct {
        REG32 por_rst_monitor : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_POR_RST_MONITOR_T;

//WDG_RST_MONITOR
typedef union {
    REG32 v;
    struct {
        REG32 wdg_rst_monitor : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_WDG_RST_MONITOR_T;

//POR_PIN_RST_MONITOR
typedef union {
    REG32 v;
    struct {
        REG32 pro_pin_rst_monitor : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_POR_PIN_RST_MONITOR_T;

//POR_SRC_FLAG
typedef union {
    REG32 v;
    struct {
        REG32 por_src_flag : 14; // [13:0], read only
        REG32 reg_soft_rst_flg_clr : 1; // [14], write clear
        REG32 por_sw_force_on : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_POR_SRC_FLAG_T;

//POR_OFF_FLAG
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 otp_chip_pd_flag_clr : 1; // [2], write clear
        REG32 otp_chip_pd_flag : 1; // [3], read only
        REG32 hw_chip_pd_flag_clr : 1; // [4], write clear
        REG32 hw_chip_pd_flag : 1; // [5], read only
        REG32 sw_chip_pd_flag_clr : 1; // [6], write clear
        REG32 sw_chip_pd_flag : 1; // [7], read only
        REG32 hard_7s_chip_pd_flag_clr : 1; // [8], write clear
        REG32 hard_7s_chip_pd_flag : 1; // [9], read only
        REG32 uvlo_chip_pd_flag_clr : 1; // [10], write clear
        REG32 uvlo_chip_pd_flag : 1; // [11], read only
        REG32 por_chip_pd_flag_clr : 1; // [12], write clear
        REG32 por_chip_pd_flag : 1; // [13], read only
        REG32 __31_14 : 18;
    } b;
} REG_RDA2720M_GLOBAL_POR_OFF_FLAG_T;

//POR_7S_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 pbint_7s_rst_mode : 1; // [0]
        REG32 pbint_7s_rst_disable : 1; // [1]
        REG32 pbint_7s_auto_on_en : 1; // [2]
        REG32 ext_rstn_mode : 1; // [3]
        REG32 pbint_7s_rst_threshold : 4; // [7:4]
        REG32 pbint_7s_rst_swmode : 1; // [8]
        REG32 key2_7s_rst_en : 1; // [9]
        REG32 __10_10 : 1;
        REG32 pbint_flag_clr : 1; // [11]
        REG32 pbint2_flag_clr : 1; // [12]
        REG32 chgr_int_flag_clr : 1; // [13]
        REG32 ext_rstn_flag_clr : 1; // [14]
        REG32 pbint_7s_flag_clr : 1; // [15]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_POR_7S_CTRL_T;

//HWRST_RTC
typedef union {
    REG32 v;
    struct {
        REG32 hwrst_rtc_reg_set : 8; // [7:0]
        REG32 hwrst_rtc_reg_sts : 8; // [15:8], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_HWRST_RTC_T;

//ARCH_EN
typedef union {
    REG32 v;
    struct {
        REG32 arch_en : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RDA2720M_GLOBAL_ARCH_EN_T;

//MCU_WR_PROT_VALUE
typedef union {
    REG32 v;
    struct {
        REG32 mcu_wr_prot_value : 15; // [14:0]
        REG32 mcu_wr_prot : 1; // [15], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_MCU_WR_PROT_VALUE_T;

//PWR_WR_PROT_VALUE
typedef union {
    REG32 v;
    struct {
        REG32 pwr_wr_prot_value : 15; // [14:0]
        REG32 pwr_wr_prot : 1; // [15], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_PWR_WR_PROT_VALUE_T;

//SMPL_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 smpl_mode : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_SMPL_CTRL0_T;

//SMPL_CTRL1
typedef union {
    REG32 v;
    struct {
        REG32 smpl_en : 1; // [0], read only
        REG32 __10_1 : 10;
        REG32 smpl_pwr_on_set : 1; // [11], read only
        REG32 smpl_mode_wr_ack_flag_clr : 1; // [12], write clear
        REG32 smpl_pwr_on_flag_clr : 1; // [13], write clear
        REG32 smpl_mode_wr_ack_flag : 1; // [14], read only
        REG32 smpl_pwr_on_flag : 1; // [15], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_SMPL_CTRL1_T;

//RTC_RST0
typedef union {
    REG32 v;
    struct {
        REG32 rtc_clk_flag_set : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_RTC_RST0_T;

//RTC_RST1
typedef union {
    REG32 v;
    struct {
        REG32 rtc_clk_flag_clr : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_RTC_RST1_T;

//RTC_RST2
typedef union {
    REG32 v;
    struct {
        REG32 rtc_clk_flag_rtc : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_RTC_RST2_T;

//RTC_CLK_STOP
typedef union {
    REG32 v;
    struct {
        REG32 rtc_clk_stop_threshold : 7; // [6:0]
        REG32 rtc_clk_stop_flag : 1; // [7], read only
        REG32 __31_8 : 24;
    } b;
} REG_RDA2720M_GLOBAL_RTC_CLK_STOP_T;

//VBAT_DROP_CNT
typedef union {
    REG32 v;
    struct {
        REG32 vbat_drop_cnt : 12; // [11:0], read only
        REG32 __31_12 : 20;
    } b;
} REG_RDA2720M_GLOBAL_VBAT_DROP_CNT_T;

//SWRST_CTRL0
typedef union {
    REG32 v;
    struct {
        REG32 sw_rst_pd_threshold : 4; // [3:0]
        REG32 reg_rst_en : 1; // [4]
        REG32 __6_5 : 2;
        REG32 wdg_rst_pd_en : 1; // [7]
        REG32 reg_rst_pd_en : 1; // [8]
        REG32 pb_7s_rst_pd_en : 1; // [9]
        REG32 ext_rstn_pd_en : 1; // [10]
        REG32 __31_11 : 21;
    } b;
} REG_RDA2720M_GLOBAL_SWRST_CTRL0_T;

//SWRST_CTRL1
typedef union {
    REG32 v;
    struct {
        REG32 sw_rst_vio18_pd_en : 1; // [0]
        REG32 sw_rst_ddr12_pd_en : 1; // [1]
        REG32 sw_rst_emmccore_pd_en : 1; // [2]
        REG32 sw_rst_usb33_pd_en : 1; // [3]
        REG32 sw_rst_rf18a_pd_en : 1; // [4]
        REG32 sw_rst_avdd18_pd_en : 1; // [5]
        REG32 sw_rst_vdd28_pd_en : 1; // [6]
        REG32 sw_rst_dcxo_pd_en : 1; // [7]
        REG32 sw_rst_mem_pd_en : 1; // [8]
        REG32 sw_rst_dcdccore_pd_en : 1; // [9]
        REG32 sw_rst_dcdcgen_pd_en : 1; // [10]
        REG32 __31_11 : 21;
    } b;
} REG_RDA2720M_GLOBAL_SWRST_CTRL1_T;

//OTP_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 otp_en : 1; // [0]
        REG32 otp_op : 2; // [2:1]
        REG32 __31_3 : 29;
    } b;
} REG_RDA2720M_GLOBAL_OTP_CTRL_T;

//FREE_TIMER_LOW
typedef union {
    REG32 v;
    struct {
        REG32 timer_low : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_FREE_TIMER_LOW_T;

//FREE_TIMER_HIGH
typedef union {
    REG32 v;
    struct {
        REG32 timer_high : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RDA2720M_GLOBAL_FREE_TIMER_HIGH_T;

//VOL_TUNE_CTRL_CORE
typedef union {
    REG32 v;
    struct {
        REG32 core_vol_tune_en : 1; // [0]
        REG32 core_vol_tune_flag : 1; // [1], read only
        REG32 core_vol_tune_start : 1; // [2], write clear
        REG32 core_step_vol : 5; // [7:3]
        REG32 core_step_num : 4; // [11:8]
        REG32 core_step_delay : 2; // [13:12]
        REG32 core_clk_sel : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_RDA2720M_GLOBAL_VOL_TUNE_CTRL_CORE_T;


//CHIP_ID_LOW
#define RDA2720M_GLOBAL_CHIP_ID_LOW_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CHIP_IP_LOW(n) (((n)&0xFFFF)<<0)

//CHIP_ID_HIGH
#define RDA2720M_GLOBAL_CHIP_ID_HIGH_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CHIP_ID_HIGH(n) (((n)&0xFFFF)<<0)

//MODULE_EN0
#define RDA2720M_GLOBAL_MODULE_EN0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_MODULE_EN0_RESERVED_1 (1<<15)
#define RDA2720M_GLOBAL_TYPEC_EN   (1<<14)
#define RDA2720M_GLOBAL_MODULE_EN0_RESERVED_2 (1<<13)
#define RDA2720M_GLOBAL_TMR_EN     (1<<12)
#define RDA2720M_GLOBAL_MODULE_EN0_RESERVED_3 (1<<11)
#define RDA2720M_GLOBAL_PSM_EN     (1<<10)
#define RDA2720M_GLOBAL_BLTC_EN    (1<<9)
#define RDA2720M_GLOBAL_PINREG_EN  (1<<8)
#define RDA2720M_GLOBAL_FGU_EN     (1<<7)
#define RDA2720M_GLOBAL_EFS_EN     (1<<6)
#define RDA2720M_GLOBAL_ADC_EN     (1<<5)
#define RDA2720M_GLOBAL_AUD_EN     (1<<4)
#define RDA2720M_GLOBAL_EIC_EN     (1<<3)
#define RDA2720M_GLOBAL_WDG_EN     (1<<2)
#define RDA2720M_GLOBAL_RTC_EN     (1<<1)
#define RDA2720M_GLOBAL_CAL_EN     (1<<0)

//ARM_CLK_EN0
#define RDA2720M_GLOBAL_ARM_CLK_EN0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_ARM_CLK_EN0_RESERVED_1(n) (((n)&0x7F)<<9)
#define RDA2720M_GLOBAL_ARM_CLK_EN0_RESERVED_2 (1<<8)
#define RDA2720M_GLOBAL_ARM_CLK_EN0_RESERVED_3 (1<<7)
#define RDA2720M_GLOBAL_CLK_AUXAD_EN (1<<6)
#define RDA2720M_GLOBAL_CLK_AUXADC_EN (1<<5)
#define RDA2720M_GLOBAL_CLK_CAL_SRC_SEL(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_CLK_CAL_EN (1<<2)
#define RDA2720M_GLOBAL_CLK_AUD_IF_6P5M_EN (1<<1)
#define RDA2720M_GLOBAL_CLK_AUD_IF_EN (1<<0)

//RTC_CLK_EN0
#define RDA2720M_GLOBAL_RTC_CLK_EN0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RTC_TYPEC_EN (1<<15)
#define RDA2720M_GLOBAL_RTC_CLK_EN0_RESERVED_1 (1<<14)
#define RDA2720M_GLOBAL_RTC_TMR_EN (1<<13)
#define RDA2720M_GLOBAL_RTC_FLASH_EN (1<<12)
#define RDA2720M_GLOBAL_RTC_EFS_EN (1<<11)
#define RDA2720M_GLOBAL_RTC_CLK_EN0_RESERVED_2(n) (((n)&7)<<8)
#define RDA2720M_GLOBAL_RTC_BLTC_EN (1<<7)
#define RDA2720M_GLOBAL_RTC_FGU_EN (1<<6)
#define RDA2720M_GLOBAL_RTC_CLK_EN0_RESERVED_3 (1<<5)
#define RDA2720M_GLOBAL_RTC_CLK_EN0_RESERVED_4 (1<<4)
#define RDA2720M_GLOBAL_RTC_EIC_EN (1<<3)
#define RDA2720M_GLOBAL_RTC_WDG_EN (1<<2)
#define RDA2720M_GLOBAL_RTC_RTC_EN (1<<1)
#define RDA2720M_GLOBAL_RTC_ARCH_EN (1<<0)

//SOFT_RST0
#define RDA2720M_GLOBAL_SOFT_RST0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SOFT_RST0_RESERVED_1(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_AUDRX_SOFT_RST (1<<13)
#define RDA2720M_GLOBAL_AUDTX_SOFT_RST (1<<12)
#define RDA2720M_GLOBAL_SOFT_RST0_RESERVED_2(n) (((n)&3)<<10)
#define RDA2720M_GLOBAL_BLTC_SOFT_RST (1<<9)
#define RDA2720M_GLOBAL_AUD_IF_SOFT_RST (1<<8)
#define RDA2720M_GLOBAL_EFS_SOFT_RST (1<<7)
#define RDA2720M_GLOBAL_ADC_SOFT_RST (1<<6)
#define RDA2720M_GLOBAL_SOFT_RST0_RESERVED_3 (1<<5)
#define RDA2720M_GLOBAL_FGU_SOFT_RST (1<<4)
#define RDA2720M_GLOBAL_EIC_SOFT_RST (1<<3)
#define RDA2720M_GLOBAL_WDG_SOFT_RST (1<<2)
#define RDA2720M_GLOBAL_RTC_SOFT_RST (1<<1)
#define RDA2720M_GLOBAL_CAL_SOFT_RST (1<<0)

//SOFT_RST1
#define RDA2720M_GLOBAL_SOFT_RST1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SOFT_RST1_RESERVED_1(n) (((n)&0xFFF)<<4)
#define RDA2720M_GLOBAL_TYPEC_SOFT_RST (1<<3)
#define RDA2720M_GLOBAL_SOFT_RST1_RESERVED_2 (1<<2)
#define RDA2720M_GLOBAL_TMR_SOFT_RST (1<<1)
#define RDA2720M_GLOBAL_SOFT_RST1_RESERVED_3 (1<<0)

//POWER_PD_SW
#define RDA2720M_GLOBAL_POWER_PD_SW_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_POWER_PD_SW_RESERVED_1(n) (((n)&31)<<11)
#define RDA2720M_GLOBAL_LDO_DCXO_PD (1<<10)
#define RDA2720M_GLOBAL_LDO_EMM_PD (1<<9)
#define RDA2720M_GLOBAL_LDO_CP_PD  (1<<8)
#define RDA2720M_GLOBAL_DCDC_GEN_PD (1<<7)
#define RDA2720M_GLOBAL_POWER_PD_SW_RESERVED_2 (1<<6)
#define RDA2720M_GLOBAL_DCDC_CORE_PD (1<<5)
#define RDA2720M_GLOBAL_OSC3M_EN   (1<<4)
#define RDA2720M_GLOBAL_LDO_MEM_PD (1<<3)
#define RDA2720M_GLOBAL_LDO_AVDD18_PD (1<<2)
#define RDA2720M_GLOBAL_LDO_VDD28_PD (1<<1)
#define RDA2720M_GLOBAL_BG_PD      (1<<0)

//POWER_PD_HW
#define RDA2720M_GLOBAL_POWER_PD_HW_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_POWER_PD_HW_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_PWR_OFF_SEQ_EN (1<<0)

//SOFT_RST_HW
#define RDA2720M_GLOBAL_SOFT_RST_HW_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SOFT_RST_HW_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_REG_SOFT_RST (1<<0)

//RESERVED_REG1
#define RDA2720M_GLOBAL_RESERVED_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG1_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG2
#define RDA2720M_GLOBAL_RESERVED_REG2_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG2_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG3
#define RDA2720M_GLOBAL_RESERVED_REG3_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG3_RESERVED_1(n) (((n)&0xFFFF)<<0)

//DCDC_CLK_REG0
#define RDA2720M_GLOBAL_DCDC_CLK_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_CLK_REG0_RESERVED_1(n) (((n)&15)<<12)
#define RDA2720M_GLOBAL_CLK3M_OUT_EN (1<<11)
#define RDA2720M_GLOBAL_CLKOUT_UNIPHASE (1<<10)
#define RDA2720M_GLOBAL_CLKOUT_SEL(n) (((n)&7)<<7)
#define RDA2720M_GLOBAL_DCDC_CLK_REG0_RESERVED_2(n) (((n)&3)<<5)
#define RDA2720M_GLOBAL_DCDC_GEN_CLK_RST (1<<4)
#define RDA2720M_GLOBAL_OSC3M_FREQ(n) (((n)&15)<<0)

//DCDC_CORE_REG0
#define RDA2720M_GLOBAL_DCDC_CORE_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CURLIMIT_R_VCORE(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_CURSES_R_VCORE(n) (((n)&3)<<12)
#define RDA2720M_GLOBAL_PFM_VH_VCORE(n) (((n)&3)<<10)
#define RDA2720M_GLOBAL_RCOMP_VCORE(n) (((n)&3)<<8)
#define RDA2720M_GLOBAL_SLOPE_VCORE(n) (((n)&3)<<6)
#define RDA2720M_GLOBAL_SR_HS_VCORE(n) (((n)&3)<<4)
#define RDA2720M_GLOBAL_SR_LS_VCORE(n) (((n)&3)<<2)
#define RDA2720M_GLOBAL_ZX_OFFSET_VCORE(n) (((n)&3)<<0)

//DCDC_CORE_REG1
#define RDA2720M_GLOBAL_DCDC_CORE_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_CORE_REG1_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_VOTRIM_VCORE(n) (((n)&7)<<3)
#define RDA2720M_GLOBAL_ZX_DISABLE_VCORE (1<<2)
#define RDA2720M_GLOBAL_FORCE_PWM_VCORE (1<<1)
#define RDA2720M_GLOBAL_ANRITING_EN_VCORE (1<<0)

//DCDC_CORE_REG2
#define RDA2720M_GLOBAL_DCDC_CORE_REG2_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_CORE_REG2_RESERVED_1(n) (((n)&7)<<13)
#define RDA2720M_GLOBAL_DIV_CLK_VCORE_EN (1<<12)
#define RDA2720M_GLOBAL_PHASE_SEL_VCORE(n) (((n)&0x3F)<<6)
#define RDA2720M_GLOBAL_DIV_BASE_VCORE(n) (((n)&0x3F)<<0)

//DCDC_CORE_VOL
#define RDA2720M_GLOBAL_DCDC_CORE_VOL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_CORE_VOL_RESERVED_1(n) (((n)&0x7F)<<9)
#define RDA2720M_GLOBAL_VOSEL_VCORE(n) (((n)&0x1FF)<<0)

//DCDC_GEN_REG0
#define RDA2720M_GLOBAL_DCDC_GEN_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CURLIMIT_R_VGEN(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_CURSES_R_VGEN(n) (((n)&3)<<12)
#define RDA2720M_GLOBAL_PFM_VH_VGEN(n) (((n)&3)<<10)
#define RDA2720M_GLOBAL_RCOMP_VGEN(n) (((n)&3)<<8)
#define RDA2720M_GLOBAL_SLOPE_VGEN(n) (((n)&3)<<6)
#define RDA2720M_GLOBAL_SR_HS_VGEN(n) (((n)&3)<<4)
#define RDA2720M_GLOBAL_SR_LS_VGEN(n) (((n)&3)<<2)
#define RDA2720M_GLOBAL_ZX_OFFSET_VGEN(n) (((n)&3)<<0)

//DCDC_GEN_REG1
#define RDA2720M_GLOBAL_DCDC_GEN_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_GEN_REG1_RESERVED_1(n) (((n)&0xFFF)<<4)
#define RDA2720M_GLOBAL_ZX_DISABLE_VGEN (1<<3)
#define RDA2720M_GLOBAL_MAXDUTY_SEL_VGEN (1<<2)
#define RDA2720M_GLOBAL_FORCE_PWM_VGEN (1<<1)
#define RDA2720M_GLOBAL_ANRITING_EN_VGEN (1<<0)

//DCDC_GEN_REG2
#define RDA2720M_GLOBAL_DCDC_GEN_REG2_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_GEN_REG2_RESERVED_1(n) (((n)&7)<<13)
#define RDA2720M_GLOBAL_DIV_CLK_VGEN_EN (1<<12)
#define RDA2720M_GLOBAL_PHASE_SEL_VGEN(n) (((n)&0x3F)<<6)
#define RDA2720M_GLOBAL_DIV_BASE_VGEN(n) (((n)&0x3F)<<0)

//DCDC_GEN_VOL
#define RDA2720M_GLOBAL_DCDC_GEN_VOL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_GEN_VOL_RESERVED_1(n) (((n)&0xFF)<<8)
#define RDA2720M_GLOBAL_VOSEL_VGEN(n) (((n)&0xFF)<<0)

//DCDC_WPA_REG0
#define RDA2720M_GLOBAL_DCDC_WPA_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CURLIMIT_R_VPA(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_CURSES_M_VPA(n) (((n)&3)<<12)
#define RDA2720M_GLOBAL_CCOMP3_VPA(n) (((n)&3)<<10)
#define RDA2720M_GLOBAL_PFM_THRESHOLD_VPA(n) (((n)&3)<<8)
#define RDA2720M_GLOBAL_RCOMP2_VPA(n) (((n)&3)<<6)
#define RDA2720M_GLOBAL_RCOMP3_VPA(n) (((n)&3)<<4)
#define RDA2720M_GLOBAL_SAWTOOTH_SLOPE_VPA(n) (((n)&3)<<2)
#define RDA2720M_GLOBAL_SR_HS_VPA(n) (((n)&3)<<0)

//DCDC_WPA_REG1
#define RDA2720M_GLOBAL_DCDC_WPA_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SR_LS_VPA(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_VOTRIM_VPA(n) (((n)&7)<<11)
#define RDA2720M_GLOBAL_ZX_OFFSET_VPA(n) (((n)&3)<<9)
#define RDA2720M_GLOBAL_ANRITING_EN_VPA (1<<8)
#define RDA2720M_GLOBAL_APC_ENABLE_VPA (1<<7)
#define RDA2720M_GLOBAL_APC_RAMP_SEL_VPA (1<<6)
#define RDA2720M_GLOBAL_BYPASS_DISABLE_VPA (1<<5)
#define RDA2720M_GLOBAL_BYPASS_FORCEON_VPA (1<<4)
#define RDA2720M_GLOBAL_BYPASS_THRESHOLD_VPA(n) (((n)&3)<<2)
#define RDA2720M_GLOBAL_DVS_ON_VPA (1<<1)
#define RDA2720M_GLOBAL_MAXDUTY_SEL_VPA (1<<0)

//DCDC_WPA_REG2
#define RDA2720M_GLOBAL_DCDC_WPA_REG2_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DIV_CLK_VPA_EN (1<<15)
#define RDA2720M_GLOBAL_PHASE_SEL_VPA(n) (((n)&0x3F)<<9)
#define RDA2720M_GLOBAL_DIV_BASE_VPA(n) (((n)&0x3F)<<3)
#define RDA2720M_GLOBAL_ZX_DISABLE_VPA (1<<2)
#define RDA2720M_GLOBAL_SAWTOOTHCAL_RST_VPA (1<<1)
#define RDA2720M_GLOBAL_PD_BUCK_VPA (1<<0)

//DCDC_WPA_VOL
#define RDA2720M_GLOBAL_DCDC_WPA_VOL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_WPA_VOL_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_VOSEL_VPA(n) (((n)&0x7F)<<0)

//DCDC_WPA_DCM_HW
#define RDA2720M_GLOBAL_DCDC_WPA_DCM_HW_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_WPA_DCM_HW_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_FORCE_PWM_VPA (1<<0)

//DCDC_CH_CTRL
#define RDA2720M_GLOBAL_DCDC_CH_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_CH_CTRL_RESERVED_1(n) (((n)&0x1FFF)<<3)
#define RDA2720M_GLOBAL_DCDC_AUXTRIM_SEL(n) (((n)&7)<<0)

//RESERVED_REG6
#define RDA2720M_GLOBAL_RESERVED_REG6_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG6_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG7
#define RDA2720M_GLOBAL_RESERVED_REG7_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG7_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG8
#define RDA2720M_GLOBAL_RESERVED_REG8_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG8_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG9
#define RDA2720M_GLOBAL_RESERVED_REG9_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG9_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG10
#define RDA2720M_GLOBAL_RESERVED_REG10_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG10_RESERVED_1(n) (((n)&0xFFFF)<<0)

//LDO_AVDD18_REG0
#define RDA2720M_GLOBAL_LDO_AVDD18_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_AVDD18_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_AVDD18_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_AVDD18_BP (1<<5)
#define RDA2720M_GLOBAL_LDO_AVDD18_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_AVDD18_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_AVDD18_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_AVDD18_REG0_RESERVED_2 (1<<0)

//LDO_AVDD18_REG1
#define RDA2720M_GLOBAL_LDO_AVDD18_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_AVDD18_REG1_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_LDO_AVDD18_V(n) (((n)&0x3F)<<0)

//LDO_CAMIO_REG0
#define RDA2720M_GLOBAL_LDO_CAMIO_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CAMIO_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_CAMIO_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_CAMIO_BP (1<<5)
#define RDA2720M_GLOBAL_LDO_CAMIO_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_CAMIO_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_CAMIO_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_CAMIO_PD (1<<0)

//LDO_CAMIO_REG1
#define RDA2720M_GLOBAL_LDO_CAMIO_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CAMIO_REG1_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_LDO_CAMIO_V(n) (((n)&0x3F)<<0)

//LDO_RF18A_REG0
#define RDA2720M_GLOBAL_LDO_RF18A_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_RF18A_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_RF18A_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_RF18A_BP (1<<5)
#define RDA2720M_GLOBAL_LDO_RF18A_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_RF18A_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_RF18A_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_RF18A_PD (1<<0)

//LDO_RF18A_REG1
#define RDA2720M_GLOBAL_LDO_RF18A_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_RF18A_REG1_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_LDO_RF18A_V(n) (((n)&0x3F)<<0)

//LDO_RF18B_REG0
#define RDA2720M_GLOBAL_LDO_RF18B_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_RF18B_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_RF18B_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_RF18B_BP (1<<5)
#define RDA2720M_GLOBAL_LDO_RF18B_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_RF18B_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_RF18B_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_RF18B_PD (1<<0)

//LDO_RF18B_REG1
#define RDA2720M_GLOBAL_LDO_RF18B_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_RF18B_REG1_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_LDO_RF18B_V(n) (((n)&0x3F)<<0)

//LDO_CAMD_REG0
#define RDA2720M_GLOBAL_LDO_CAMD_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CAMD_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_CAMD_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_CAMD_BP (1<<5)
#define RDA2720M_GLOBAL_LDO_CAMD_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_CAMD_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_CAMD_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_CAMD_PD (1<<0)

//LDO_CAMD_REG1
#define RDA2720M_GLOBAL_LDO_CAMD_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CAMD_REG1_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_LDO_CAMD_V(n) (((n)&0x3F)<<0)

//LDO_CON_REG0
#define RDA2720M_GLOBAL_LDO_CON_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CON_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_CON_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_CON_BP (1<<5)
#define RDA2720M_GLOBAL_LDO_CON_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_CON_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_CON_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_CON_PD (1<<0)

//LDO_CON_REG1
#define RDA2720M_GLOBAL_LDO_CON_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CON_REG1_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_LDO_CON_V(n) (((n)&0x3F)<<0)

//LDO_MEM_REG0
#define RDA2720M_GLOBAL_LDO_MEM_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_MEM_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_MEM_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_MEM_BP (1<<5)
#define RDA2720M_GLOBAL_LDO_MEM_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_MEM_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_MEM_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_MEM_REG0_RESERVED_2 (1<<0)

//LDO_MEM_REG1
#define RDA2720M_GLOBAL_LDO_MEM_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_MEM_REG1_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_LDO_MEM_V(n) (((n)&0x3F)<<0)

//LDO_SIM0_REG0
#define RDA2720M_GLOBAL_LDO_SIM0_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM0_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_SIM0_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_SIM0_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_SIM0_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_SIM0_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_SIM0_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_SIM0_REG0_RESERVED_2 (1<<0)

//LDO_SIM0_PD_REG
#define RDA2720M_GLOBAL_LDO_SIM0_PD_REG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM0_PD_REG_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_LDO_SIM0_PD (1<<0)

//LDO_SIM0_REG1
#define RDA2720M_GLOBAL_LDO_SIM0_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM0_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_SIM0_V(n) (((n)&0x7F)<<0)

//LDO_SIM1_REG0
#define RDA2720M_GLOBAL_LDO_SIM1_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM1_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_SIM1_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_SIM1_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_SIM1_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_SIM1_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_SIM1_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_SIM1_REG0_RESERVED_2 (1<<0)

//LDO_SIM1_PD_REG
#define RDA2720M_GLOBAL_LDO_SIM1_PD_REG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM1_PD_REG_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_LDO_SIM1_PD (1<<0)

//LDO_SIM1_REG1
#define RDA2720M_GLOBAL_LDO_SIM1_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM1_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_SIM1_V(n) (((n)&0x7F)<<0)

//LDO_SIM2_REG0
#define RDA2720M_GLOBAL_LDO_SIM2_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM2_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_SIM2_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_SIM2_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_SIM2_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_SIM2_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_SIM2_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_SIM2_REG0_RESERVED_2 (1<<0)

//LDO_SIM2_PD_REG
#define RDA2720M_GLOBAL_LDO_SIM2_PD_REG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM2_PD_REG_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_LDO_SIM2_PD (1<<0)

//LDO_SIM2_REG1
#define RDA2720M_GLOBAL_LDO_SIM2_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM2_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_SIM2_V(n) (((n)&0x7F)<<0)

//LDO_CAMA_REG0
#define RDA2720M_GLOBAL_LDO_CAMA_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CAMA_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_CAMA_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_CAMA_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_CAMA_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_CAMA_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_CAMA_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_CAMA_PD (1<<0)

//LDO_CAMA_REG1
#define RDA2720M_GLOBAL_LDO_CAMA_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CAMA_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_CAMA_V(n) (((n)&0x7F)<<0)

//LDO_LCD_REG0
#define RDA2720M_GLOBAL_LDO_LCD_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_LCD_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_LCD_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_LCD_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_LCD_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_LCD_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_LCD_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_LCD_PD (1<<0)

//LDO_LCD_REG1
#define RDA2720M_GLOBAL_LDO_LCD_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_LCD_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_LCD_V(n) (((n)&0x7F)<<0)

//LDO_EMMCCORE_REG0
#define RDA2720M_GLOBAL_LDO_EMMCCORE_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_REG0_RESERVED_2 (1<<0)

//LDO_EMMCCORE_PD_REG
#define RDA2720M_GLOBAL_LDO_EMMCCORE_PD_REG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_PD_REG_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_PD (1<<0)

//LDO_EMMCCORE_REG1
#define RDA2720M_GLOBAL_LDO_EMMCCORE_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_V(n) (((n)&0x7F)<<0)

//LDO_SD_REG0
#define RDA2720M_GLOBAL_LDO_SD_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SD_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_VIO18_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_VIO18_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_VIO18_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_VIO18_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_VIO18_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_SD_REG0_RESERVED_2 (1<<0)

//LDO_SD_PD_REG
#define RDA2720M_GLOBAL_LDO_SD_PD_REG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SD_PD_REG_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_LDO_VIO18_PD (1<<0)

//LDO_SD_REG1
#define RDA2720M_GLOBAL_LDO_SD_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SD_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_VIO18_V(n) (((n)&0x7F)<<0)

//LDO_DDR12_REG0
#define RDA2720M_GLOBAL_LDO_DDR12_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_DDR12_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_DDR12_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_DDR12_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_DDR12_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_DDR12_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_DDR12_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_DDR12_REG0_RESERVED_2 (1<<0)

//LDO_DDR12_PD_REG
#define RDA2720M_GLOBAL_LDO_DDR12_PD_REG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_DDR12_PD_REG_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_LDO_DDR12_PD (1<<0)

//LDO_DDR12_REG1
#define RDA2720M_GLOBAL_LDO_DDR12_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_DDR12_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_DDR12_V(n) (((n)&0x7F)<<0)

//LDO_VDD28_REG0
#define RDA2720M_GLOBAL_LDO_VDD28_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_VDD28_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_VDD28_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_VDD28_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_VDD28_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_VDD28_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_VDD28_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_VDD28_REG0_RESERVED_2 (1<<0)

//LDO_VDD28_REG1
#define RDA2720M_GLOBAL_LDO_VDD28_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_VDD28_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_VDD28_V(n) (((n)&0x7F)<<0)

//LDO_SPIMEM_REG0
#define RDA2720M_GLOBAL_LDO_SPIMEM_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SPIMEM_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_SPIMEM_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_SPIMEM_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_SPIMEM_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_SPIMEM_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_SPIMEM_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_SPIMEM_PD (1<<0)

//LDO_SPIMEM_REG1
#define RDA2720M_GLOBAL_LDO_SPIMEM_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SPIMEM_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_SPIMEM_V(n) (((n)&0x7F)<<0)

//LDO_DCXO_REG0
#define RDA2720M_GLOBAL_LDO_DCXO_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_DCXO_REG0_RESERVED_1(n) (((n)&15)<<12)
#define RDA2720M_GLOBAL_LDO_DCXO_TRIM(n) (((n)&31)<<7)
#define RDA2720M_GLOBAL_LDO_DCXO_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_DCXO_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_DCXO_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_DCXO_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_DCXO_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_DCXO_REG0_RESERVED_2 (1<<0)

//LDO_DCXO_REG1
#define RDA2720M_GLOBAL_LDO_DCXO_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_DCXO_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_DCXO_V(n) (((n)&0x7F)<<0)

//LDO_USB_REG0
#define RDA2720M_GLOBAL_LDO_USB_REG0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_USB_REG0_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_USB33_SHPT_PD (1<<6)
#define RDA2720M_GLOBAL_LDO_USB33_CAP_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_USB33_STB(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_USB33_SHPT_ADJ (1<<2)
#define RDA2720M_GLOBAL_LDO_USB33_CL_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_USB_REG0_RESERVED_2 (1<<0)

//LDO_USB_PD_REG
#define RDA2720M_GLOBAL_LDO_USB_PD_REG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_USB_PD_REG_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_LDO_USB33_PD (1<<0)

//LDO_USB_REG1
#define RDA2720M_GLOBAL_LDO_USB_REG1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_USB_REG1_RESERVED_1(n) (((n)&0x1FF)<<7)
#define RDA2720M_GLOBAL_LDO_USB33_V(n) (((n)&0x7F)<<0)

//RESERVED_REG17
#define RDA2720M_GLOBAL_RESERVED_REG17_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG17_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG18
#define RDA2720M_GLOBAL_RESERVED_REG18_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG18_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG19
#define RDA2720M_GLOBAL_RESERVED_REG19_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG19_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG20
#define RDA2720M_GLOBAL_RESERVED_REG20_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG20_RESERVED_1(n) (((n)&0xFFFF)<<0)

//LDO_TRIM_REG
#define RDA2720M_GLOBAL_LDO_TRIM_REG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_TRIM_REG_RESERVED_1(n) (((n)&0x3F)<<10)
#define RDA2720M_GLOBAL_LDO_TRIM_B(n) (((n)&31)<<5)
#define RDA2720M_GLOBAL_LDO_TRIM_A(n) (((n)&31)<<0)

//LDO_RTC_CTRL
#define RDA2720M_GLOBAL_LDO_RTC_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_RTC_CTRL_RESERVED_1(n) (((n)&0x7F)<<9)
#define RDA2720M_GLOBAL_LDO_RTC_CAL(n) (((n)&31)<<4)
#define RDA2720M_GLOBAL_LDO_RTC_V(n) (((n)&3)<<2)
#define RDA2720M_GLOBAL_VBATBK_V(n) (((n)&3)<<0)

//LDO_CH_CTRL
#define RDA2720M_GLOBAL_LDO_CH_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CH_CTRL_RESERVED_1(n) (((n)&31)<<11)
#define RDA2720M_GLOBAL_LDO_CAL2(n) (((n)&7)<<8)
#define RDA2720M_GLOBAL_LDO_CAL1(n) (((n)&7)<<5)
#define RDA2720M_GLOBAL_LDO_CH_CTRL_RESERVED_2(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_LDO_CAL(n) (((n)&7)<<0)

//RESERVED_REG23
#define RDA2720M_GLOBAL_RESERVED_REG23_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG23_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG24
#define RDA2720M_GLOBAL_RESERVED_REG24_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG24_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG25
#define RDA2720M_GLOBAL_RESERVED_REG25_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG25_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG26
#define RDA2720M_GLOBAL_RESERVED_REG26_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG26_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG27
#define RDA2720M_GLOBAL_RESERVED_REG27_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG27_RESERVED_1(n) (((n)&0xFFFF)<<0)

//SLP_CTRL
#define RDA2720M_GLOBAL_SLP_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SLP_CTRL_RESERVED_1(n) (((n)&0x1FFF)<<3)
#define RDA2720M_GLOBAL_LDO_XTL_EN (1<<2)
#define RDA2720M_GLOBAL_SLP_IO_EN  (1<<1)
#define RDA2720M_GLOBAL_SLP_LDO_PD_EN (1<<0)

//SLP_DCDC_PD_CTRL
#define RDA2720M_GLOBAL_SLP_DCDC_PD_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SLP_DCDCCORE_PD_RSTN_TH(n) (((n)&15)<<12)
#define RDA2720M_GLOBAL_SLP_DCDCCORE_PU_RSTN_TH(n) (((n)&0x3F)<<6)
#define RDA2720M_GLOBAL_SLP_DCDC_PD_CTRL_RESERVED_1 (1<<5)
#define RDA2720M_GLOBAL_SLP_DCDCCORE_PD_EN (1<<4)
#define RDA2720M_GLOBAL_SLP_DCDCCORE_DROP_EN (1<<3)
#define RDA2720M_GLOBAL_SLP_DCDCWPA_PD_EN (1<<2)
#define RDA2720M_GLOBAL_SLP_VDDIO1V8_PD_EN (1<<1)
#define RDA2720M_GLOBAL_SLP_DCDC_PD_CTRL_RESERVED_2 (1<<0)

//SLP_LDO_PD_CTRL0
#define RDA2720M_GLOBAL_SLP_LDO_PD_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SLP_LDO_PD_CTRL0_RESERVED_1 (1<<15)
#define RDA2720M_GLOBAL_SLP_LDORF18A_PD_EN (1<<14)
#define RDA2720M_GLOBAL_SLP_LDORF18B_PD_EN (1<<13)
#define RDA2720M_GLOBAL_SLP_LDOEMMCCORE_PD_EN (1<<12)
#define RDA2720M_GLOBAL_SLP_LDODCXO_PD_EN (1<<11)
#define RDA2720M_GLOBAL_SLP_LDOSPIMEM_PD_EN (1<<10)
#define RDA2720M_GLOBAL_SLP_LDOVDD28_PD_EN (1<<9)
#define RDA2720M_GLOBAL_SLP_LDOVIO18_PD_EN (1<<8)
#define RDA2720M_GLOBAL_SLP_LDODDR12_PD_EN (1<<7)
#define RDA2720M_GLOBAL_SLP_LDOUSB33_PD_EN (1<<6)
#define RDA2720M_GLOBAL_SLP_LDOLCD_PD_EN (1<<5)
#define RDA2720M_GLOBAL_SLP_LDOCAMIO_PD_EN (1<<4)
#define RDA2720M_GLOBAL_SLP_LDOCAMD_PD_EN (1<<3)
#define RDA2720M_GLOBAL_SLP_LDOCAMA_PD_EN (1<<2)
#define RDA2720M_GLOBAL_SLP_LDOSIM2_PD_EN (1<<1)
#define RDA2720M_GLOBAL_SLP_LDOSIM1_PD_EN (1<<0)

//SLP_LDO_PD_CTRL1
#define RDA2720M_GLOBAL_SLP_LDO_PD_CTRL1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SLP_LDO_PD_CTRL1_RESERVED_1(n) (((n)&0x7FF)<<5)
#define RDA2720M_GLOBAL_SLP_LDOCP_PD_EN (1<<4)
#define RDA2720M_GLOBAL_SLP_LDOCON_PD_EN (1<<3)
#define RDA2720M_GLOBAL_SLP_LDOSIM0_PD_EN (1<<2)
#define RDA2720M_GLOBAL_SLP_LDOAVDD18_PD_EN (1<<1)
#define RDA2720M_GLOBAL_SLP_LDOMEM_PD_EN (1<<0)

//SLP_DCDC_LP_CTRL
#define RDA2720M_GLOBAL_SLP_DCDC_LP_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SLP_DCDC_LP_CTRL_RESERVED_1(n) (((n)&0x7FF)<<5)
#define RDA2720M_GLOBAL_SLP_DCDCCORE_LP_EN (1<<4)
#define RDA2720M_GLOBAL_SLP_DCDC_LP_CTRL_RESERVED_2 (1<<3)
#define RDA2720M_GLOBAL_SLP_DCDC_LP_CTRL_RESERVED_3 (1<<2)
#define RDA2720M_GLOBAL_SLP_DCDCGEN_LP_EN (1<<1)
#define RDA2720M_GLOBAL_SLP_DCDCWPA_LP_EN (1<<0)

//SLP_LDO_LP_CTRL0
#define RDA2720M_GLOBAL_SLP_LDO_LP_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SLP_LDO_LP_CTRL0_RESERVED_1 (1<<15)
#define RDA2720M_GLOBAL_SLP_LDORF18A_LP_EN (1<<14)
#define RDA2720M_GLOBAL_SLP_LDORF18B_LP_EN (1<<13)
#define RDA2720M_GLOBAL_SLP_LDOEMMCCORE_LP_EN (1<<12)
#define RDA2720M_GLOBAL_SLP_LDODCXO_LP_EN (1<<11)
#define RDA2720M_GLOBAL_SLP_LDOSPIMEM_LP_EN (1<<10)
#define RDA2720M_GLOBAL_SLP_LDOVDD28_LP_EN (1<<9)
#define RDA2720M_GLOBAL_SLP_LDOVIO18_LP_EN (1<<8)
#define RDA2720M_GLOBAL_SLP_LDODDR12_LP_EN (1<<7)
#define RDA2720M_GLOBAL_SLP_LDOUSB33_LP_EN (1<<6)
#define RDA2720M_GLOBAL_SLP_LDOLCD_LP_EN (1<<5)
#define RDA2720M_GLOBAL_SLP_LDOCAMIO_LP_EN (1<<4)
#define RDA2720M_GLOBAL_SLP_LDOCAMD_LP_EN (1<<3)
#define RDA2720M_GLOBAL_SLP_LDOCAMA_LP_EN (1<<2)
#define RDA2720M_GLOBAL_SLP_LDOSIM2_LP_EN (1<<1)
#define RDA2720M_GLOBAL_SLP_LDOSIM1_LP_EN (1<<0)

//SLP_LDO_LP_CTRL1
#define RDA2720M_GLOBAL_SLP_LDO_LP_CTRL1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SLP_LDO_LP_CTRL1_RESERVED_1(n) (((n)&0xFFF)<<4)
#define RDA2720M_GLOBAL_SLP_LDOCON_LP_EN (1<<3)
#define RDA2720M_GLOBAL_SLP_LDOSIM0_LP_EN (1<<2)
#define RDA2720M_GLOBAL_SLP_LDOAVDD18_LP_EN (1<<1)
#define RDA2720M_GLOBAL_SLP_LDOMEM_LP_EN (1<<0)

//DCDC_CORE_SLP_CTRL0
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_CTRL0_RESERVED_1(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_STEP_DELAY(n) (((n)&3)<<12)
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_STEP_NUM(n) (((n)&15)<<8)
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_STEP_VOL(n) (((n)&31)<<3)
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_CTRL0_RESERVED_2 (1<<2)
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_CTRL0_RESERVED_3 (1<<1)
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_STEP_EN (1<<0)

//DCDC_CORE_SLP_CTRL1
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_CTRL1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_CTRL1_RESERVED_1(n) (((n)&0x7F)<<9)
#define RDA2720M_GLOBAL_DCDC_CORE_VOSEL_DS_SW(n) (((n)&0x1FF)<<0)

//DCDC_XTL_EN0
#define RDA2720M_GLOBAL_DCDC_XTL_EN0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_CORE_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_DCDC_CORE_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_DCDC_CORE_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_DCDC_CORE_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_DCDC_XTL_EN0_RESERVED_1(n) (((n)&15)<<8)
#define RDA2720M_GLOBAL_DCDC_GEN_EXT_XTL0_EN (1<<7)
#define RDA2720M_GLOBAL_DCDC_GEN_EXT_XTL1_EN (1<<6)
#define RDA2720M_GLOBAL_DCDC_GEN_EXT_XTL2_EN (1<<5)
#define RDA2720M_GLOBAL_DCDC_GEN_EXT_XTL3_EN (1<<4)
#define RDA2720M_GLOBAL_DCDC_WPA_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_DCDC_WPA_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_DCDC_WPA_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_DCDC_WPA_EXT_XTL3_EN (1<<0)

//RESEVED_DCDC_XTL_EN4
#define RDA2720M_GLOBAL_RESEVED_DCDC_XTL_EN4_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESEVED_DCDC_XTL_EN4_RESERVED_1(n) (((n)&0xFFFF)<<0)

//LDO_XTL_EN0
#define RDA2720M_GLOBAL_LDO_XTL_EN0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_DCXO_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_DCXO_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_DCXO_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_DCXO_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN0_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_VDD28_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_VDD28_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_VDD28_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_VDD28_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN1
#define RDA2720M_GLOBAL_LDO_XTL_EN1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_RF18A_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_RF18A_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_RF18A_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_RF18A_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN1_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_RF18B_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_RF18B_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_RF18B_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_RF18B_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN2
#define RDA2720M_GLOBAL_LDO_XTL_EN2_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM0_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_SIM0_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_SIM0_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_SIM0_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN2_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_SIM1_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_SIM1_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_SIM1_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_SIM1_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN3
#define RDA2720M_GLOBAL_LDO_XTL_EN3_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_SIM2_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_SIM2_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_SIM2_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_SIM2_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN3_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_MEM_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_MEM_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_MEM_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_MEM_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN4
#define RDA2720M_GLOBAL_LDO_XTL_EN4_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_LCD_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_LCD_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_LCD_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_LCD_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN4_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_CAMIO_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_CAMIO_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_CAMIO_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_CAMIO_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN5
#define RDA2720M_GLOBAL_LDO_XTL_EN5_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CAMA_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_CAMA_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_CAMA_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_CAMA_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN5_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_CAMD_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_CAMD_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_CAMD_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_CAMD_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN6
#define RDA2720M_GLOBAL_LDO_XTL_EN6_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_DDR12_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_DDR12_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_DDR12_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_DDR12_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN6_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_VIO18_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_VIO18_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_VIO18_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_VIO18_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN7
#define RDA2720M_GLOBAL_LDO_XTL_EN7_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_EMMCCORE_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN7_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_USB33_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_USB33_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_USB33_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_USB33_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN8
#define RDA2720M_GLOBAL_LDO_XTL_EN8_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_KPLED_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_KPLED_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_KPLED_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_KPLED_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN8_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_VIBR_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_VIBR_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_VIBR_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_VIBR_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN9
#define RDA2720M_GLOBAL_LDO_XTL_EN9_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CON_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_CON_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_CON_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_CON_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN9_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_AVDD18_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_AVDD18_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_AVDD18_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_AVDD18_EXT_XTL3_EN (1<<0)

//LDO_XTL_EN10
#define RDA2720M_GLOBAL_LDO_XTL_EN10_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_CP_EXT_XTL0_EN (1<<15)
#define RDA2720M_GLOBAL_LDO_CP_EXT_XTL1_EN (1<<14)
#define RDA2720M_GLOBAL_LDO_CP_EXT_XTL2_EN (1<<13)
#define RDA2720M_GLOBAL_LDO_CP_EXT_XTL3_EN (1<<12)
#define RDA2720M_GLOBAL_LDO_XTL_EN10_RESERVED_1(n) (((n)&0xFF)<<4)
#define RDA2720M_GLOBAL_LDO_SPIMEM_EXT_XTL0_EN (1<<3)
#define RDA2720M_GLOBAL_LDO_SPIMEM_EXT_XTL1_EN (1<<2)
#define RDA2720M_GLOBAL_LDO_SPIMEM_EXT_XTL2_EN (1<<1)
#define RDA2720M_GLOBAL_LDO_SPIMEM_EXT_XTL3_EN (1<<0)

//RESERVED_REG30
#define RDA2720M_GLOBAL_RESERVED_REG30_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG30_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG31
#define RDA2720M_GLOBAL_RESERVED_REG31_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG31_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG32
#define RDA2720M_GLOBAL_RESERVED_REG32_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG32_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG_CORE
#define RDA2720M_GLOBAL_RESERVED_REG_CORE_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG_CORE_RESERVED_1(n) (((n)&0xFFFF)<<0)

//RESERVED_REG_RTC
#define RDA2720M_GLOBAL_RESERVED_REG_RTC_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RESERVED_REG_RTC_RESERVED_1(n) (((n)&0xFFFF)<<0)

//BG_CTRL
#define RDA2720M_GLOBAL_BG_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_BG_CTRL_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_BG_CHOP_EN (1<<0)

//DCDC_VLG_SEL
#define RDA2720M_GLOBAL_DCDC_VLG_SEL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_DCDC_VLG_SEL_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_DCDC_WPA_VOTRIM_SW_SEL (1<<5)
#define RDA2720M_GLOBAL_DCDC_WPA_SW_SEL (1<<4)
#define RDA2720M_GLOBAL_DCDC_GEN_SW_SEL (1<<3)
#define RDA2720M_GLOBAL_DCDC_CORE_VOTRIM_SW_SEL (1<<2)
#define RDA2720M_GLOBAL_DCDC_CORE_SLP_SW_SEL (1<<1)
#define RDA2720M_GLOBAL_DCDC_CORE_NOR_SW_SEL (1<<0)

//LDO_VLG_SEL0
#define RDA2720M_GLOBAL_LDO_VLG_SEL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_VLG_SEL0_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_LDO_KPLED_SW_SEL (1<<5)
#define RDA2720M_GLOBAL_LDO_VIBR_SW_SEL (1<<4)
#define RDA2720M_GLOBAL_LDO_RTC_CAL_SW_SEL (1<<3)
#define RDA2720M_GLOBAL_LDO_DCXO_SW_SEL (1<<2)
#define RDA2720M_GLOBAL_LDO_TRIM_A_SW_SEL (1<<1)
#define RDA2720M_GLOBAL_LDO_TRIM_B_SW_SEL (1<<0)

//CLK32KLESS_CTRL0
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_1 (1<<15)
#define RDA2720M_GLOBAL_RC_MODE_WR_ACK_FLAG (1<<14)
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_2 (1<<13)
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_3 (1<<12)
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_4 (1<<11)
#define RDA2720M_GLOBAL_RC_MODE_WR_ACK_FLAG_CLR (1<<10)
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_5 (1<<9)
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_6 (1<<8)
#define RDA2720M_GLOBAL_LDO_DCXO_LP_EN_RTCSET (1<<7)
#define RDA2720M_GLOBAL_LDO_DCXO_LP_EN_RTCCLR (1<<6)
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_7 (1<<5)
#define RDA2720M_GLOBAL_RTC_MODE   (1<<4)
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_8 (1<<3)
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL0_RESERVED_9 (1<<2)
#define RDA2720M_GLOBAL_RC_32K_SEL (1<<1)
#define RDA2720M_GLOBAL_RC_32K_EN  (1<<0)

//CLK32KLESS_CTRL1
#define RDA2720M_GLOBAL_CLK32KLESS_CTRL1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RC_MODE(n) (((n)&0xFFFF)<<0)

//XTL_WAIT_CTRL0
#define RDA2720M_GLOBAL_XTL_WAIT_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_XTL_WAIT_CTRL0_RESERVED_1 (1<<15)
#define RDA2720M_GLOBAL_XTL_WAIT_CTRL0_RESERVED_2 (1<<14)
#define RDA2720M_GLOBAL_EXT_XTL3_FOR_26M_EN (1<<13)
#define RDA2720M_GLOBAL_EXT_XTL2_FOR_26M_EN (1<<12)
#define RDA2720M_GLOBAL_EXT_XTL1_FOR_26M_EN (1<<11)
#define RDA2720M_GLOBAL_EXT_XTL0_FOR_26M_EN (1<<10)
#define RDA2720M_GLOBAL_SLP_XTLBUF_PD_EN (1<<9)
#define RDA2720M_GLOBAL_XTL_EN     (1<<8)
#define RDA2720M_GLOBAL_XTL_WAIT(n) (((n)&0xFF)<<0)

//RGB_CTRL
#define RDA2720M_GLOBAL_RGB_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RGB_CTRL_RESERVED_1(n) (((n)&0x1FFF)<<3)
#define RDA2720M_GLOBAL_SLP_RGB_PD_EN (1<<2)
#define RDA2720M_GLOBAL_RGB_PD_HW_EN (1<<1)
#define RDA2720M_GLOBAL_RGB_PD_SW  (1<<0)

//IB_CTRL
#define RDA2720M_GLOBAL_IB_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_IB_CTRL_RESERVED_1(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_BATDET_CUR_EN (1<<13)
#define RDA2720M_GLOBAL_IB_CTRL_RESERVED_2 (1<<12)
#define RDA2720M_GLOBAL_BATDET_CUR_I(n) (((n)&7)<<9)
#define RDA2720M_GLOBAL_IB_TRIM(n) (((n)&0x7F)<<2)
#define RDA2720M_GLOBAL_IB_TRIM_EM_SEL (1<<1)
#define RDA2720M_GLOBAL_IB_REX_EN  (1<<0)

//FLASH_CTRL
#define RDA2720M_GLOBAL_FLASH_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_FLASH_PON  (1<<15)
#define RDA2720M_GLOBAL_FLASH_CTRL_RESERVED_1(n) (((n)&0xFF)<<7)
#define RDA2720M_GLOBAL_FLASH_V_HW_EN (1<<6)
#define RDA2720M_GLOBAL_FLASH_V_HW_STEP(n) (((n)&3)<<4)
#define RDA2720M_GLOBAL_FLASH_V_SW(n) (((n)&15)<<0)

//KPLED_CTRL0
#define RDA2720M_GLOBAL_KPLED_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_KPLED_V(n) (((n)&15)<<12)
#define RDA2720M_GLOBAL_KPLED_PD   (1<<11)
#define RDA2720M_GLOBAL_KPLED_PULLDOWN_EN (1<<10)
#define RDA2720M_GLOBAL_SLP_LDOKPLED_PD_EN (1<<9)
#define RDA2720M_GLOBAL_KPLED_CTRL0_RESERVED_1(n) (((n)&15)<<5)
#define RDA2720M_GLOBAL_LDO_KPLED_CAP_SEL (1<<4)
#define RDA2720M_GLOBAL_LDO_KPLED_STB(n) (((n)&3)<<2)
#define RDA2720M_GLOBAL_LDO_KPLED_SHPT_ADJ (1<<1)
#define RDA2720M_GLOBAL_LDO_KPLED_CL_ADJ (1<<0)

//KPLED_CTRL1
#define RDA2720M_GLOBAL_KPLED_CTRL1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_KPLED_PD (1<<15)
#define RDA2720M_GLOBAL_KPLED_CTRL1_RESERVED_1(n) (((n)&31)<<10)
#define RDA2720M_GLOBAL_LDO_KPLED_V(n) (((n)&7)<<7)
#define RDA2720M_GLOBAL_LDO_KPLED_REFTRIM(n) (((n)&31)<<2)
#define RDA2720M_GLOBAL_KPLED_CTRL1_RESERVED_2 (1<<1)
#define RDA2720M_GLOBAL_LDO_KPLED_SHPT_PD (1<<0)

//VIBR_CTRL0
#define RDA2720M_GLOBAL_VIBR_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_LDO_VIBR_CAP_SEL (1<<15)
#define RDA2720M_GLOBAL_LDO_VIBR_STB(n) (((n)&3)<<13)
#define RDA2720M_GLOBAL_LDO_VIBR_SHPT_ADJ (1<<12)
#define RDA2720M_GLOBAL_LDO_VIBR_CL_ADJ (1<<11)
#define RDA2720M_GLOBAL_LDO_VIBR_SHPT_PD (1<<10)
#define RDA2720M_GLOBAL_SLP_LDOVIBR_PD_EN (1<<9)
#define RDA2720M_GLOBAL_LDO_VIBR_PD (1<<8)
#define RDA2720M_GLOBAL_VIBR_CTRL0_RESERVED_1(n) (((n)&31)<<3)
#define RDA2720M_GLOBAL_LDO_VIBR_V(n) (((n)&7)<<0)

//VIBR_CTRL1
#define RDA2720M_GLOBAL_VIBR_CTRL1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_VIBR_CTRL1_RESERVED_1(n) (((n)&0x3FF)<<6)
#define RDA2720M_GLOBAL_LDO_VIBR_REFTRIM(n) (((n)&31)<<1)
#define RDA2720M_GLOBAL_LDO_VIBR_EADBIAS_EN (1<<0)

//AUDIO_CTRL0
#define RDA2720M_GLOBAL_AUDIO_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_AUDIO_CTRL0_RESERVED_1(n) (((n)&0xFFF)<<4)
#define RDA2720M_GLOBAL_CLK_AUD_IF_TX_INV_EN (1<<3)
#define RDA2720M_GLOBAL_CLK_AUD_IF_RX_INV_EN (1<<2)
#define RDA2720M_GLOBAL_CLK_AUD_IF_6P5M_TX_INV_EN (1<<1)
#define RDA2720M_GLOBAL_CLK_AUD_IF_6P5M_RX_INV_EN (1<<0)

//CHGR_CTRL0
#define RDA2720M_GLOBAL_CHGR_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CHGR_CTRL0_RESERVED_1 (1<<15)
#define RDA2720M_GLOBAL_CHGR_DPM(n) (((n)&3)<<13)
#define RDA2720M_GLOBAL_CHGR_CC_EN (1<<12)
#define RDA2720M_GLOBAL_CHGR_CTRL0_RESERVED_2 (1<<11)
#define RDA2720M_GLOBAL_CHGR_CV_V(n) (((n)&0x3F)<<5)
#define RDA2720M_GLOBAL_CHGR_END_V(n) (((n)&3)<<3)
#define RDA2720M_GLOBAL_CHGR_ITERM(n) (((n)&3)<<1)
#define RDA2720M_GLOBAL_CHGR_PD    (1<<0)

//CHGR_CTRL1
#define RDA2720M_GLOBAL_CHGR_CTRL1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CHGR_EXPOWER_DEVICE (1<<15)
#define RDA2720M_GLOBAL_CHGR_CTRL1_RESERVED_1 (1<<14)
#define RDA2720M_GLOBAL_CHGR_CC_I(n) (((n)&15)<<10)
#define RDA2720M_GLOBAL_CHGR_CTRL1_RESERVED_2(n) (((n)&0xFF)<<2)
#define RDA2720M_GLOBAL_VCHG_OVP_V(n) (((n)&3)<<0)

//CHGR_STATUS
#define RDA2720M_GLOBAL_CHGR_STATUS_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CHGR_STATUS_RESERVED_1(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_CHGR_INT_EN (1<<13)
#define RDA2720M_GLOBAL_NON_DCP_INT (1<<12)
#define RDA2720M_GLOBAL_CHG_DET_DONE (1<<11)
#define RDA2720M_GLOBAL_DP_LOW     (1<<10)
#define RDA2720M_GLOBAL_DCP_DET    (1<<9)
#define RDA2720M_GLOBAL_CHG_DET    (1<<8)
#define RDA2720M_GLOBAL_SDP_INT    (1<<7)
#define RDA2720M_GLOBAL_DCP_INT    (1<<6)
#define RDA2720M_GLOBAL_CDP_INT    (1<<5)
#define RDA2720M_GLOBAL_CHGR_CV_STATUS (1<<4)
#define RDA2720M_GLOBAL_CHGR_ON    (1<<3)
#define RDA2720M_GLOBAL_CHGR_INT   (1<<2)
#define RDA2720M_GLOBAL_DCP_SWITCH_EN (1<<1)
#define RDA2720M_GLOBAL_VCHG_OVI   (1<<0)

//CHGR_DET_FGU_CTRL
#define RDA2720M_GLOBAL_CHGR_DET_FGU_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_CHGR_DET_FGU_CTRL_RESERVED_1(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_FGUA_SOFT_RST (1<<13)
#define RDA2720M_GLOBAL_LDO_FGU_PD (1<<12)
#define RDA2720M_GLOBAL_CHG_INT_DELAY(n) (((n)&7)<<9)
#define RDA2720M_GLOBAL_SD_CHOP_CAP_EN (1<<8)
#define RDA2720M_GLOBAL_SD_CLK_P(n) (((n)&3)<<6)
#define RDA2720M_GLOBAL_SD_ADC0_RC_PD (1<<5)
#define RDA2720M_GLOBAL_SD_CHOP_EN (1<<4)
#define RDA2720M_GLOBAL_SD_DCOFFSET_B_EN (1<<3)
#define RDA2720M_GLOBAL_SD_DCOFFSET_A_EN (1<<2)
#define RDA2720M_GLOBAL_DP_DM_AUX_EN (1<<1)
#define RDA2720M_GLOBAL_DP_DM_BC_ENB (1<<0)

//OVLO_CTRL
#define RDA2720M_GLOBAL_OVLO_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_OVLO_CTRL_RESERVED_1(n) (((n)&15)<<12)
#define RDA2720M_GLOBAL_VBAT_CRASH_V(n) (((n)&3)<<10)
#define RDA2720M_GLOBAL_OVLO_EN    (1<<9)
#define RDA2720M_GLOBAL_OVLO_CTRL_RESERVED_2(n) (((n)&31)<<4)
#define RDA2720M_GLOBAL_OVLO_V(n)  (((n)&3)<<2)
#define RDA2720M_GLOBAL_OVLO_T(n)  (((n)&3)<<0)

//MIXED_CTRL
#define RDA2720M_GLOBAL_MIXED_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_MIXED_CTRL_RESERVED_1(n) (((n)&7)<<13)
#define RDA2720M_GLOBAL_XOSC32K_CTL (1<<12)
#define RDA2720M_GLOBAL_BATON_T(n) (((n)&3)<<10)
#define RDA2720M_GLOBAL_VPP_5V_SEL (1<<9)
#define RDA2720M_GLOBAL_BATDET_OK  (1<<8)
#define RDA2720M_GLOBAL_MIXED_CTRL_RESERVED_2(n) (((n)&7)<<5)
#define RDA2720M_GLOBAL_VBAT_OK    (1<<4)
#define RDA2720M_GLOBAL_ALL_GPI_DEB (1<<3)
#define RDA2720M_GLOBAL_GPI_DEBUG_EN (1<<2)
#define RDA2720M_GLOBAL_ALL_INT_DEB (1<<1)
#define RDA2720M_GLOBAL_INT_DEBUG_EN (1<<0)

//POR_RST_MONITOR
#define RDA2720M_GLOBAL_POR_RST_MONITOR_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_POR_RST_MONITOR(n) (((n)&0xFFFF)<<0)

//WDG_RST_MONITOR
#define RDA2720M_GLOBAL_WDG_RST_MONITOR_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_WDG_RST_MONITOR(n) (((n)&0xFFFF)<<0)

//POR_PIN_RST_MONITOR
#define RDA2720M_GLOBAL_POR_PIN_RST_MONITOR_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_PRO_PIN_RST_MONITOR(n) (((n)&0xFFFF)<<0)

//POR_SRC_FLAG
#define RDA2720M_GLOBAL_POR_SRC_FLAG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_POR_SW_FORCE_ON (1<<15)
#define RDA2720M_GLOBAL_REG_SOFT_RST_FLG_CLR (1<<14)
#define RDA2720M_GLOBAL_POR_SRC_FLAG(n) (((n)&0x3FFF)<<0)

//POR_OFF_FLAG
#define RDA2720M_GLOBAL_POR_OFF_FLAG_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_POR_OFF_FLAG_RESERVED_1(n) (((n)&3)<<14)
#define RDA2720M_GLOBAL_POR_CHIP_PD_FLAG (1<<13)
#define RDA2720M_GLOBAL_POR_CHIP_PD_FLAG_CLR (1<<12)
#define RDA2720M_GLOBAL_UVLO_CHIP_PD_FLAG (1<<11)
#define RDA2720M_GLOBAL_UVLO_CHIP_PD_FLAG_CLR (1<<10)
#define RDA2720M_GLOBAL_HARD_7S_CHIP_PD_FLAG (1<<9)
#define RDA2720M_GLOBAL_HARD_7S_CHIP_PD_FLAG_CLR (1<<8)
#define RDA2720M_GLOBAL_SW_CHIP_PD_FLAG (1<<7)
#define RDA2720M_GLOBAL_SW_CHIP_PD_FLAG_CLR (1<<6)
#define RDA2720M_GLOBAL_HW_CHIP_PD_FLAG (1<<5)
#define RDA2720M_GLOBAL_HW_CHIP_PD_FLAG_CLR (1<<4)
#define RDA2720M_GLOBAL_OTP_CHIP_PD_FLAG (1<<3)
#define RDA2720M_GLOBAL_OTP_CHIP_PD_FLAG_CLR (1<<2)
#define RDA2720M_GLOBAL_POR_OFF_FLAG_RESERVED_2(n) (((n)&3)<<0)

//POR_7S_CTRL
#define RDA2720M_GLOBAL_POR_7S_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_PBINT_7S_FLAG_CLR (1<<15)
#define RDA2720M_GLOBAL_EXT_RSTN_FLAG_CLR (1<<14)
#define RDA2720M_GLOBAL_CHGR_INT_FLAG_CLR (1<<13)
#define RDA2720M_GLOBAL_PBINT2_FLAG_CLR (1<<12)
#define RDA2720M_GLOBAL_PBINT_FLAG_CLR (1<<11)
#define RDA2720M_GLOBAL_POR_7S_CTRL_RESERVED_1 (1<<10)
#define RDA2720M_GLOBAL_KEY2_7S_RST_EN (1<<9)
#define RDA2720M_GLOBAL_PBINT_7S_RST_SWMODE (1<<8)
#define RDA2720M_GLOBAL_PBINT_7S_RST_THRESHOLD(n) (((n)&15)<<4)
#define RDA2720M_GLOBAL_EXT_RSTN_MODE (1<<3)
#define RDA2720M_GLOBAL_PBINT_7S_AUTO_ON_EN (1<<2)
#define RDA2720M_GLOBAL_PBINT_7S_RST_DISABLE (1<<1)
#define RDA2720M_GLOBAL_PBINT_7S_RST_MODE (1<<0)

//HWRST_RTC
#define RDA2720M_GLOBAL_HWRST_RTC_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_HWRST_RTC_REG_STS(n) (((n)&0xFF)<<8)
#define RDA2720M_GLOBAL_HWRST_RTC_REG_SET(n) (((n)&0xFF)<<0)

//ARCH_EN
#define RDA2720M_GLOBAL_ARCH_EN_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_ARCH_EN_RESERVED_1(n) (((n)&0x7FFF)<<1)
#define RDA2720M_GLOBAL_ARCH_EN    (1<<0)

//MCU_WR_PROT_VALUE
#define RDA2720M_GLOBAL_MCU_WR_PROT_VALUE_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_MCU_WR_PROT (1<<15)
#define RDA2720M_GLOBAL_MCU_WR_PROT_VALUE(n) (((n)&0x7FFF)<<0)

//PWR_WR_PROT_VALUE
#define RDA2720M_GLOBAL_PWR_WR_PROT_VALUE_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_PWR_WR_PROT (1<<15)
#define RDA2720M_GLOBAL_PWR_WR_PROT_VALUE(n) (((n)&0x7FFF)<<0)

//SMPL_CTRL0
#define RDA2720M_GLOBAL_SMPL_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SMPL_MODE(n) (((n)&0xFFFF)<<0)

//SMPL_CTRL1
#define RDA2720M_GLOBAL_SMPL_CTRL1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SMPL_PWR_ON_FLAG (1<<15)
#define RDA2720M_GLOBAL_SMPL_MODE_WR_ACK_FLAG (1<<14)
#define RDA2720M_GLOBAL_SMPL_PWR_ON_FLAG_CLR (1<<13)
#define RDA2720M_GLOBAL_SMPL_MODE_WR_ACK_FLAG_CLR (1<<12)
#define RDA2720M_GLOBAL_SMPL_PWR_ON_SET (1<<11)
#define RDA2720M_GLOBAL_SMPL_CTRL1_RESERVED_1(n) (((n)&0x3FF)<<1)
#define RDA2720M_GLOBAL_SMPL_EN    (1<<0)

//RTC_RST0
#define RDA2720M_GLOBAL_RTC_RST0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RTC_CLK_FLAG_SET(n) (((n)&0xFFFF)<<0)

//RTC_RST1
#define RDA2720M_GLOBAL_RTC_RST1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RTC_CLK_FLAG_CLR(n) (((n)&0xFFFF)<<0)

//RTC_RST2
#define RDA2720M_GLOBAL_RTC_RST2_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RTC_CLK_FLAG_RTC(n) (((n)&0xFFFF)<<0)

//RTC_CLK_STOP
#define RDA2720M_GLOBAL_RTC_CLK_STOP_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_RTC_CLK_STOP_RESERVED_1(n) (((n)&0xFF)<<8)
#define RDA2720M_GLOBAL_RTC_CLK_STOP_FLAG (1<<7)
#define RDA2720M_GLOBAL_RTC_CLK_STOP_THRESHOLD(n) (((n)&0x7F)<<0)

//VBAT_DROP_CNT
#define RDA2720M_GLOBAL_VBAT_DROP_CNT_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_VBAT_DROP_CNT_RESERVED_1(n) (((n)&15)<<12)
#define RDA2720M_GLOBAL_VBAT_DROP_CNT(n) (((n)&0xFFF)<<0)

//SWRST_CTRL0
#define RDA2720M_GLOBAL_SWRST_CTRL0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SWRST_CTRL0_RESERVED_1(n) (((n)&31)<<11)
#define RDA2720M_GLOBAL_EXT_RSTN_PD_EN (1<<10)
#define RDA2720M_GLOBAL_PB_7S_RST_PD_EN (1<<9)
#define RDA2720M_GLOBAL_REG_RST_PD_EN (1<<8)
#define RDA2720M_GLOBAL_WDG_RST_PD_EN (1<<7)
#define RDA2720M_GLOBAL_SWRST_CTRL0_RESERVED_2(n) (((n)&3)<<5)
#define RDA2720M_GLOBAL_REG_RST_EN (1<<4)
#define RDA2720M_GLOBAL_SW_RST_PD_THRESHOLD(n) (((n)&15)<<0)

//SWRST_CTRL1
#define RDA2720M_GLOBAL_SWRST_CTRL1_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_SWRST_CTRL1_RESERVED_1(n) (((n)&15)<<12)
#define RDA2720M_GLOBAL_SWRST_CTRL1_RESERVED_2 (1<<11)
#define RDA2720M_GLOBAL_SW_RST_DCDCGEN_PD_EN (1<<10)
#define RDA2720M_GLOBAL_SW_RST_DCDCCORE_PD_EN (1<<9)
#define RDA2720M_GLOBAL_SW_RST_MEM_PD_EN (1<<8)
#define RDA2720M_GLOBAL_SW_RST_DCXO_PD_EN (1<<7)
#define RDA2720M_GLOBAL_SW_RST_VDD28_PD_EN (1<<6)
#define RDA2720M_GLOBAL_SW_RST_AVDD18_PD_EN (1<<5)
#define RDA2720M_GLOBAL_SW_RST_RF18A_PD_EN (1<<4)
#define RDA2720M_GLOBAL_SW_RST_USB33_PD_EN (1<<3)
#define RDA2720M_GLOBAL_SW_RST_EMMCCORE_PD_EN (1<<2)
#define RDA2720M_GLOBAL_SW_RST_DDR12_PD_EN (1<<1)
#define RDA2720M_GLOBAL_SW_RST_VIO18_PD_EN (1<<0)

//OTP_CTRL
#define RDA2720M_GLOBAL_OTP_CTRL_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_OTP_CTRL_RESERVED_1(n) (((n)&0x1FFF)<<3)
#define RDA2720M_GLOBAL_OTP_OP(n)  (((n)&3)<<1)
#define RDA2720M_GLOBAL_OTP_EN     (1<<0)

//FREE_TIMER_LOW
#define RDA2720M_GLOBAL_FREE_TIMER_LOW_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_TIMER_LOW(n) (((n)&0xFFFF)<<0)

//FREE_TIMER_HIGH
#define RDA2720M_GLOBAL_FREE_TIMER_HIGH_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_TIMER_HIGH(n) (((n)&0xFFFF)<<0)

//VOL_TUNE_CTRL_CORE
#define RDA2720M_GLOBAL_VOL_TUNE_CTRL_CORE_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define RDA2720M_GLOBAL_VOL_TUNE_CTRL_CORE_RESERVED_1 (1<<15)
#define RDA2720M_GLOBAL_CORE_CLK_SEL (1<<14)
#define RDA2720M_GLOBAL_CORE_STEP_DELAY(n) (((n)&3)<<12)
#define RDA2720M_GLOBAL_CORE_STEP_NUM(n) (((n)&15)<<8)
#define RDA2720M_GLOBAL_CORE_STEP_VOL(n) (((n)&31)<<3)
#define RDA2720M_GLOBAL_CORE_VOL_TUNE_START (1<<2)
#define RDA2720M_GLOBAL_CORE_VOL_TUNE_FLAG (1<<1)
#define RDA2720M_GLOBAL_CORE_VOL_TUNE_EN (1<<0)





#endif

