/*
 * Copyright (C) 2017 Rdamicro Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 ************************************************************
 * Auto generated code from asic documentation, DO NOT EDIT *
 ************************************************************
 */
#ifndef _RDA2720_GLB_H
#define _RDA2720_GLB_H

#define CTL_BASE_ANA_GLB 0x0C00
#define ANA_REG_GLB_BASE 0x0C00
#define REG32 volatile unsigned int

typedef struct {
	unsigned int chip_id_low;           /*0x0000*/
	unsigned int chip_id_high;          /*0x0004*/
	unsigned int module_en0;            /*0x0008*/
	unsigned int arm_clk_en0;           /*0x000c*/
	unsigned int rtc_clk_en0;           /*0x0010*/
	unsigned int soft_rst0;             /*0x0014*/
	unsigned int soft_rst1;             /*0x0018*/
	unsigned int power_pd_sw;           /*0x001c*/
	unsigned int power_pd_hw;           /*0x0020*/
	unsigned int soft_rst_hw;           /*0x0024*/
	unsigned int reserved_reg1;         /*0x0028*/
	unsigned int reserved_reg2;         /*0x002c*/
	unsigned int reserved_reg3;         /*0x0030*/
	unsigned int dcdc_clk_reg0;         /*0x0034*/
	unsigned int dcdc_core_reg0;        /*0x0038*/
	unsigned int dcdc_core_reg1;        /*0x003c*/
	unsigned int dcdc_core_reg2;        /*0x0040*/
	unsigned int dcdc_core_vol;         /*0x0044*/
	unsigned int dcdc_gen_reg0;         /*0x0048*/
	unsigned int dcdc_gen_reg1;         /*0x004c*/
	unsigned int dcdc_gen_reg2;         /*0x0050*/
	unsigned int dcdc_gen_vol;          /*0x0054*/
	unsigned int dcdc_wpa_reg0;         /*0x0058*/
	unsigned int dcdc_wpa_reg1;         /*0x005c*/
	unsigned int dcdc_wpa_reg2;         /*0x0060*/
	unsigned int dcdc_wpa_vol;          /*0x0064*/
	unsigned int dcdc_wpa_dcm_hw;       /*0x0068*/
	unsigned int dcdc_ch_ctrl;          /*0x006c*/
	unsigned int reserved_reg6;         /*0x0070*/
	unsigned int reserved_reg7;         /*0x0074*/
	unsigned int reserved_reg8;         /*0x0078*/
	unsigned int reserved_reg9;         /*0x007c*/
	unsigned int reserved_reg10;        /*0x0080*/
	unsigned int ldo_avdd18_reg0;       /*0x0084*/
	unsigned int ldo_avdd18_reg1;       /*0x0088*/
	unsigned int ldo_camio_reg0;        /*0x008c*/
	unsigned int ldo_camio_reg1;        /*0x0090*/
	unsigned int ldo_rf18a_reg0;        /*0x0094*/
	unsigned int ldo_rf18a_reg1;        /*0x0098*/
	unsigned int ldo_rf18b_reg0;        /*0x009c*/
	unsigned int ldo_rf18b_reg1;        /*0x00a0*/
	unsigned int ldo_camd_reg0;         /*0x00a4*/
	unsigned int ldo_camd_reg1;         /*0x00a8*/
	unsigned int ldo_con_reg0;          /*0x00ac*/
	unsigned int ldo_con_reg1;          /*0x00b0*/
	unsigned int ldo_mem_reg0;          /*0x00b4*/
	unsigned int ldo_mem_reg1;          /*0x00b8*/
	unsigned int ldo_sim0_reg0;         /*0x00bc*/
	unsigned int ldo_sim0_pd_reg;       /*0x00c0*/
	unsigned int ldo_sim0_reg1;         /*0x00c4*/
	unsigned int ldo_sim1_reg0;         /*0x00c8*/
	unsigned int ldo_sim1_pd_reg;       /*0x00cc*/
	unsigned int ldo_sim1_reg1;         /*0x00d0*/
	unsigned int ldo_sim2_reg0;         /*0x00d4*/
	unsigned int ldo_sim2_pd_reg;       /*0x00d8*/
	unsigned int ldo_sim2_reg1;         /*0x00dc*/
	unsigned int ldo_cama_reg0;         /*0x00e0*/
	unsigned int ldo_cama_reg1;         /*0x00e4*/
	unsigned int ldo_cammot_reg0;       /*0x00e8*/
	unsigned int ldo_cammot_reg1;       /*0x00ec*/
	unsigned int ldo_emmccore_reg0;     /*0x00f0*/
	unsigned int ldo_emmccore_pd_reg;   /*0x00f4*/
	unsigned int ldo_emmccore_reg1;     /*0x00f8*/
	unsigned int ldo_sd_reg0;           /*0x00fc*/
	unsigned int ldo_sd_pd_reg;         /*0x0100*/
	unsigned int ldo_sd_reg1;           /*0x0104*/
	unsigned int ldo_sdio_reg0;         /*0x0108*/
	unsigned int ldo_sdio_pd_reg;       /*0x010c*/
	unsigned int ldo_sdio_reg1;         /*0x0110*/
	unsigned int ldo_vdd28_reg0;        /*0x0114*/
	unsigned int ldo_vdd28_reg1;        /*0x0118*/
	unsigned int ldo_wifipa_reg0;       /*0x011c*/
	unsigned int ldo_wifipa_reg1;       /*0x0120*/
	unsigned int ldo_dcxo_reg0;         /*0x0124*/
	unsigned int ldo_dcxo_reg1;         /*0x0128*/
	unsigned int ldo_usb_reg0;          /*0x012c*/
	unsigned int ldo_usb_pd_reg;        /*0x0130*/
	unsigned int ldo_usb_reg1;          /*0x0134*/
	unsigned int reserved_reg17;        /*0x0138*/
	unsigned int reserved_reg18;        /*0x013c*/
	unsigned int reserved_reg19;        /*0x0140*/
	unsigned int reserved_reg20;        /*0x0144*/
	unsigned int ldo_trim_reg;          /*0x0148*/
	unsigned int ldo_rtc_ctrl;          /*0x014c*/
	unsigned int ldo_ch_ctrl;           /*0x0150*/
	unsigned int reserved_reg23;        /*0x0154*/
	unsigned int reserved_reg24;        /*0x0158*/
	unsigned int reserved_reg25;        /*0x015c*/
	unsigned int reserved_reg26;        /*0x0160*/
	unsigned int reserved_reg27;        /*0x0164*/
	unsigned int slp_ctrl;              /*0x0168*/
	unsigned int slp_dcdc_pd_ctrl;      /*0x016c*/
	unsigned int slp_ldo_pd_ctrl0;      /*0x0170*/
	unsigned int slp_ldo_pd_ctrl1;      /*0x0174*/
	unsigned int slp_dcdc_lp_ctrl;      /*0x0178*/
	unsigned int slp_ldo_lp_ctrl0;      /*0x017c*/
	unsigned int slp_ldo_lp_ctrl1;      /*0x0180*/
	unsigned int dcdc_core_slp_ctrl0;   /*0x0184*/
	unsigned int dcdc_core_slp_ctrl1;   /*0x0188*/
	unsigned int dcdc_xtl_en0;          /*0x018c*/
	unsigned int reseved_dcdc_xtl_en4;  /*0x0190*/
	unsigned int ldo_xtl_en0;           /*0x0194*/
	unsigned int ldo_xtl_en1;           /*0x0198*/
	unsigned int ldo_xtl_en2;           /*0x019c*/
	unsigned int ldo_xtl_en3;           /*0x01a0*/
	unsigned int ldo_xtl_en4;           /*0x01a4*/
	unsigned int ldo_xtl_en5;           /*0x01a8*/
	unsigned int ldo_xtl_en6;           /*0x01ac*/
	unsigned int ldo_xtl_en7;           /*0x01b0*/
	unsigned int ldo_xtl_en8;           /*0x01b4*/
	unsigned int ldo_xtl_en9;           /*0x01b8*/
	unsigned int ldo_xtl_en10;          /*0x01bc*/
	unsigned int reserved_reg30;        /*0x01c0*/
	unsigned int reserved_reg31;        /*0x01c4*/
	unsigned int reserved_reg32;        /*0x01c8*/
	unsigned int reserved_reg_core;     /*0x01cc*/
	unsigned int reserved_reg_rtc;      /*0x01d0*/
	unsigned int bg_ctrl;               /*0x01d4*/
	unsigned int dcdc_vlg_sel;          /*0x01d8*/
	unsigned int ldo_vlg_sel0;          /*0x01dc*/
	unsigned int clk32kless_ctrl0;      /*0x01e0*/
	unsigned int clk32kless_ctrl1;      /*0x01e4*/
	unsigned int xtl_wait_ctrl0;        /*0x01e8*/
	unsigned int rgb_ctrl;              /*0x01ec*/
	unsigned int ib_ctrl;               /*0x01f0*/
	unsigned int flash_ctrl;            /*0x01f4*/
	unsigned int kpled_ctrl0;           /*0x01f8*/
	unsigned int kpled_ctrl1;           /*0x01fc*/
	unsigned int vibr_ctrl0;            /*0x0200*/
	unsigned int vibr_ctrl1;            /*0x0204*/
	unsigned int audio_ctrl0;           /*0x0208*/
	unsigned int chgr_ctrl0;            /*0x020c*/
	unsigned int chgr_ctrl1;            /*0x0210*/
	unsigned int chgr_status;           /*0x0214*/
	unsigned int chgr_det_fgu_ctrl;     /*0x0218*/
	unsigned int ovlo_ctrl;             /*0x021c*/
	unsigned int mixed_ctrl;            /*0x0220*/
	unsigned int por_rst_monitor;       /*0x0224*/
	unsigned int wdg_rst_monitor;       /*0x0228*/
	unsigned int por_pin_rst_monitor;   /*0x022c*/
	unsigned int por_src_flag;          /*0x0230*/
	unsigned int por_off_flag;          /*0x0234*/
	unsigned int por_7s_ctrl;           /*0x0238*/
	unsigned int hwrst_rtc;             /*0x023c*/
	unsigned int arch_en;               /*0x0240*/
	unsigned int mcu_wr_prot_value;     /*0x0244*/
	unsigned int pwr_wr_prot_value;     /*0x0248*/
	unsigned int smpl_ctrl0;            /*0x024c*/
	unsigned int smpl_ctrl1;            /*0x0250*/
	unsigned int rtc_rst0;              /*0x0254*/
	unsigned int rtc_rst1;              /*0x0258*/
	unsigned int rtc_rst2;              /*0x025c*/
	unsigned int rtc_clk_stop;          /*0x0260*/
	unsigned int vbat_drop_cnt;         /*0x0264*/
	unsigned int swrst_ctrl0;           /*0x0268*/
	unsigned int swrst_ctrl1;           /*0x026c*/
	unsigned int otp_ctrl;              /*0x0270*/
	unsigned int free_timer_low;        /*0x0274*/
	unsigned int free_timer_high;       /*0x0278*/
	unsigned int vol_tune_ctrl_core;    /*0x027c*/
} HWP_ANA_REG_GLB_T;

#define hwp_ana_reg_glb		((HWP_ANA_REG_GLB_T *) ANA_REG_GLB_BASE)
#if 0
#define ANA_REG_GLB_ARM_MODULE_EN	        ANA_REG_GLB_MODULE_EN0
#define ANA_REG_GLB_ARM_CLK_EN		        ANA_REG_GLB_ARM_CLK_EN0
#define ANA_REG_GLB_RTC_CLK_EN		        ANA_REG_GLB_RTC_CLK_EN0
#define ANA_REG_GLB_XTL_WAIT_CTRL		ANA_REG_GLB_XTL_WAIT_CTRL0
#endif

/*-----------------------------------------------------------
 * Register Name          :   ANA_REG_GLB_CHIP_ID_LOW
 * Register Offset        :   0x0000
 * Register Description   :
-----------------------------------------------------------*/

#define BITS_CHIP_ID_LOW(x)                     (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CHIP_ID_HIGH
* Register Offset        :   0x0004
* Register Description   :
-----------------------------------------------------------*/

#define BITS_CHIP_ID_HIGH(x)                    (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_MODULE_EN0
* Register Offset        :   0x0008
* Register Description   :
-----------------------------------------------------------*/

#define BIT_TYPEC_EN                            BIT(14)
#define BIT_CHG_WDG_EN                          BIT(13)
#define BIT_TMR_EN                              BIT(12)
#define BIT_FAST_CHG_EN                         BIT(11)
#define BIT_BLTC_EN                             BIT(9)
#define BIT_PINREG_EN                           BIT(8)
#define BIT_FGU_EN                              BIT(7)
#define BIT_EFS_EN                              BIT(6)
#define BIT_ADC_EN                              BIT(5)
#define BIT_AUD_EN                              BIT(4)
#define BIT_EIC_EN                              BIT(3)
#define BIT_WDG_EN                              BIT(2)
#define BIT_RTC_EN                              BIT(1)
#define BIT_CAL_EN                              BIT(0)

#define BIT_ANA_TYPEC_EN                 BIT_TYPEC_EN
#define BIT_ANA_BLTC_EN                  BIT_BLTC_EN
#define BIT_ANA_PINREG_EN                BIT_PINREG_EN
#define BIT_ANA_FGU_EN                   BIT_FGU_EN
#define BIT_ANA_EFS_EN                   BIT_EFS_EN
#define BIT_ANA_ADC_EN                   BIT_ADC_EN
#define BIT_ANA_AUD_EN                   BIT_AUD_EN
#define BIT_ANA_EIC_EN                   BIT_EIC_EN
#define BIT_ANA_WDG_EN                   BIT_WDG_EN
#define BIT_ANA_RTC_EN                   BIT_RTC_EN
#define BIT_ANA_CAL_EN                   BIT_CAL_EN

/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_ARM_CLK_EN0
* Register Offset        :   0x000C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_CLK_TSEN_ADC_EN                     BIT(8)
#define BIT_CLK_AUXAD_EN                        BIT(6)
#define BIT_CLK_AUXADC_EN                       BIT(5)
#define BITS_CLK_CAL_SRC_SEL(x)                 (((x) & 0x3) << 3)
#define BIT_CLK_CAL_EN                          BIT(2)
#define BIT_CLK_AUD_IF_6P5M_EN                  BIT(1)
#define BIT_CLK_AUD_IF_EN                       BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RTC_CLK_EN0
* Register Offset        :   0x0010
* Register Description   :
-----------------------------------------------------------*/

#define BIT_RTC_TYPEC_EN                        BIT(15)
#define BIT_RTC_CHG_WDG_EN                      BIT(14)
#define BIT_RTC_TMR_EN                          BIT(13)
#define BIT_RTC_FLASH_EN                        BIT(12)
#define BIT_RTC_EFS_EN                          BIT(11)
#define BIT_RTC_BLTC_EN                         BIT(7)
#define BIT_RTC_FGU_EN                          BIT(6)
#define BIT_RTC_FAST_CHG_EN                     BIT(4)
#define BIT_RTC_EIC_EN                          BIT(3)
#define BIT_RTC_WDG_EN                          BIT(2)
#define BIT_RTC_RTC_EN                          BIT(1)
#define BIT_RTC_ARCH_EN                         BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SOFT_RST0
* Register Offset        :   0x0014
* Register Description   :
-----------------------------------------------------------*/

#define BIT_AUDRX_SOFT_RST                      BIT(13)
#define BIT_AUDTX_SOFT_RST                      BIT(12)
#define BIT_BLTC_SOFT_RST                       BIT(9)
#define BIT_AUD_IF_SOFT_RST                     BIT(8)
#define BIT_EFS_SOFT_RST                        BIT(7)
#define BIT_ADC_SOFT_RST                        BIT(6)
#define BIT_FGU_SOFT_RST                        BIT(4)
#define BIT_EIC_SOFT_RST                        BIT(3)
#define BIT_WDG_SOFT_RST                        BIT(2)
#define BIT_RTC_SOFT_RST                        BIT(1)
#define BIT_CAL_SOFT_RST                        BIT(0)

#define BIT_ANA_AUDTX_SOFT_RST           BIT_AUDTX_SOFT_RST
#define BIT_ANA_AUDRX_SOFT_RST           BIT_AUDRX_SOFT_RST
#define BIT_ANA_EFS_SOFT_RST             BIT_EFS_SOFT_RST
#define BIT_ANA_ADC_SOFT_RST             BIT_ADC_SOFT_RST
#define BIT_ANA_FGU_SOFT_RST             BIT_FGU_SOFT_RST
#define BIT_ANA_EIC_SOFT_RST             BIT_EIC_SOFT_RST
#define BIT_ANA_WDG_SOFT_RST             BIT_WDG_SOFT_RST
#define BIT_ANA_RTC_SOFT_RST             BIT_RTC_SOFT_RST
#define BIT_ANA_CAL_SOFT_RST             BIT_CAL_SOFT_RST


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SOFT_RST1
* Register Offset        :   0x0018
* Register Description   :
-----------------------------------------------------------*/

#define BIT_TYPEC_SOFT_RST                      BIT(3)
#define BIT_CHG_WDG_SOFT_RST                    BIT(2)
#define BIT_TMR_SOFT_RST                        BIT(1)
#define BIT_FAST_CHG_SOFT_RST                   BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_POWER_PD_SW
* Register Offset        :   0x001C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_DCXO_PD                         BIT(10)
#define BIT_LDO_EMM_PD                          BIT(9)
#define BIT_DCDC_TOPCLK6M_PD                    BIT(8)
#define BIT_DCDC_GEN_PD                         BIT(7)
#define BIT_DCDC_MEM_PD                         BIT(6)
#define BIT_DCDC_CORE_PD                        BIT(5)
#define BIT_DCDC_CPU_PD                         BIT(4)
#define BIT_LDO_MEM_PD                          BIT(3)
#define BIT_LDO_AVDD18_PD                       BIT(2)
#define BIT_LDO_VDD28_PD                        BIT(1)
#define BIT_BG_PD                               BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_POWER_PD_HW
* Register Offset        :   0x0020
* Register Description   :
-----------------------------------------------------------*/

#define BIT_PWR_OFF_SEQ_EN                      BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SOFT_RST_HW
* Register Offset        :   0x0024
* Register Description   :
-----------------------------------------------------------*/

#define BIT_REG_SOFT_RST                        BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG1
* Register Offset        :   0x0028
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG2
* Register Offset        :   0x002C
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG3
* Register Offset        :   0x0030
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG4
* Register Offset        :   0x0034
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_REG0
* Register Offset        :   0x0038
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CPU_DEADTIME(x)               (((x) & 0x3) << 14)
#define BITS_DCDC_CPU_PDRSLOW(x)                (((x) & 0xF) << 4)
#define BIT_DCDC_CPU_QKRSPS                     BIT(3)
#define BIT_DCDC_CPU_PFMB                       BIT(2)
#define BIT_DCDC_CPU_DCMB                       BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_REG1
* Register Offset        :   0x003C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_CPU_OSCSYCEN_SW                BIT(15)
#define BIT_DCDC_CPU_OSCSYCEN_HW_EN             BIT(14)
#define BIT_DCDC_CPU_OSCSYC_DIV_EN              BIT(13)
#define BITS_DCDC_CPU_OSCSYC_DIV(x)             (((x) & 0x1F) << 8)
#define BITS_DCDC_CPU_DISCHRG(x)                (((x) & 0x3) << 6)
#define BITS_DCDC_CPU_CF(x)                     (((x) & 0x3) << 4)
#define BITS_DCDC_CPU_CL_CTRL(x)                (((x) & 0x3) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_REG2
* Register Offset        :   0x0040
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CPU_STBOP(x)                  (((x) & 0x7F) << 6)
#define BITS_DCDC_CPU_ZCD(x)                    (((x) & 0x3) << 4)
#define BITS_DCDC_CPU_PFMAD(x)                  (((x) & 0xF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_VOL
* Register Offset        :   0x0044
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CPU_CTRL(x)                   (((x) & 0x1F) << 5)
#define BITS_DCDC_CPU_CAL(x)                    (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_REG0
* Register Offset        :   0x0048
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CORE_DEADTIME(x)              (((x) & 0x3) << 14)
#define BITS_DCDC_CORE_PDRSLOW(x)               (((x) & 0xF) << 4)
#define BIT_DCDC_CORE_QKRSPS                    BIT(3)
#define BIT_DCDC_CORE_PFMB                      BIT(2)
#define BIT_DCDC_CORE_DCMB                      BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_REG1
* Register Offset        :   0x004C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_CORE_OSCSYCEN_SW               BIT(15)
#define BIT_DCDC_CORE_OSCSYCEN_HW_EN            BIT(14)
#define BIT_DCDC_CORE_OSCSYC_DIV_EN             BIT(13)
#define BITS_DCDC_CORE_OSCSYC_DIV(x)            (((x) & 0x1F) << 8)
#define BITS_DCDC_CORE_DISCHRG(x)               (((x) & 0x3) << 6)
#define BITS_DCDC_CORE_CF(x)                    (((x) & 0x3) << 4)
#define BITS_DCDC_CORE_CL_CTRL(x)               (((x) & 0x3) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_REG2
* Register Offset        :   0x0050
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CORE_STBOP(x)                 (((x) & 0x7F) << 6)
#define BITS_DCDC_CORE_ZCD(x)                   (((x) & 0x3) << 4)
#define BITS_DCDC_CORE_PFMAD(x)                 (((x) & 0xF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_VOL
* Register Offset        :   0x0054
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CORE_CTRL(x)                  (((x) & 0x1F) << 5)
#define BITS_DCDC_CORE_CAL(x)                   (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_MEM_REG0
* Register Offset        :   0x0058
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_MEM_DEADTIME(x)               (((x) & 0x3) << 14)
#define BITS_DCDC_MEM_PDRSLOW(x)                (((x) & 0xF) << 4)
#define BIT_DCDC_MEM_QKRSPS                     BIT(3)
#define BIT_DCDC_MEM_PFMB                       BIT(2)
#define BIT_DCDC_MEM_DCMB                       BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_MEM_REG1
* Register Offset        :   0x005C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_MEM_DISCHRG(x)                (((x) & 0x3) << 14)
#define BIT_DCDC_MEM_OSCSYCEN_SW                BIT(13)
#define BIT_DCDC_MEM_OSCSYCEN_HW_EN             BIT(12)
#define BIT_DCDC_MEM_OSCSYC_DIV_EN              BIT(11)
#define BITS_DCDC_MEM_OSCSYC_DIV(x)             (((x) & 0x1F) << 6)
#define BITS_DCDC_MEM_CF(x)                     (((x) & 0x3) << 4)
#define BITS_DCDC_MEM_CL_CTRL(x)                (((x) & 0x3) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_MEM_REG2
* Register Offset        :   0x0060
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_MEM_STBOP(x)                  (((x) & 0x7F) << 6)
#define BITS_DCDC_MEM_ZCD(x)                    (((x) & 0x3) << 4)
#define BITS_DCDC_MEM_PFMAD(x)                  (((x) & 0xF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_MEM_VOL
* Register Offset        :   0x0064
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_MEM_CTRL(x)                   (((x) & 0x1F) << 5)
#define BITS_DCDC_MEM_CAL(x)                    (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_GEN_REG0
* Register Offset        :   0x0068
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_GEN_DEADTIME(x)               (((x) & 0x3) << 14)
#define BITS_DCDC_GEN_PDRSLOW(x)                (((x) & 0xF) << 4)
#define BIT_DCDC_GEN_QKRSPS                     BIT(3)
#define BIT_DCDC_GEN_PFMB                       BIT(2)
#define BIT_DCDC_GEN_DCMB                       BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_GEN_REG1
* Register Offset        :   0x006C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_GEN_DISCHRG(x)                (((x) & 0x3) << 14)
#define BIT_DCDC_GEN_OSCSYCEN_SW                BIT(13)
#define BIT_DCDC_GEN_OSCSYCEN_HW_EN             BIT(12)
#define BIT_DCDC_GEN_OSCSYC_DIV_EN              BIT(11)
#define BITS_DCDC_GEN_OSCSYC_DIV(x)             (((x) & 0x1F) << 6)
#define BITS_DCDC_GEN_CF(x)                     (((x) & 0x3) << 4)
#define BITS_DCDC_GEN_CL_CTRL(x)                (((x) & 0x3) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_GEN_REG2
* Register Offset        :   0x0070
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_GEN_STBOP(x)                  (((x) & 0x7F) << 6)
#define BITS_DCDC_GEN_ZCD(x)                    (((x) & 0x3) << 4)
#define BITS_DCDC_GEN_PFMAD(x)                  (((x) & 0xF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_GEN_VOL
* Register Offset        :   0x0074
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_GEN_CTRL(x)                   (((x) & 0x1F) << 5)
#define BITS_DCDC_GEN_CAL(x)                    (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_WPA_REG0
* Register Offset        :   0x0078
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_WPA_DEADTIME(x)               (((x) & 0x3) << 14)
#define BITS_DCDC_WPA_PDRSLOW(x)                (((x) & 0xF) << 4)
#define BIT_DCDC_WPA_QKRSPS                     BIT(3)
#define BIT_DCDC_WPA_PFMB                       BIT(2)
#define BIT_DCDC_WPA_APT2P5XEN                  BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_WPA_REG1
* Register Offset        :   0x007C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_WPA_BPOUT_SOFTW                BIT(15)
#define BITS_DCDC_WPA_VBAT_DIV(x)               (((x) & 0x7) << 12)
#define BIT_DCDC_WPA_BPEN                       BIT(11)
#define BIT_DCDC_WPA_BPMODE                     BIT(10)
#define BIT_DCDC_WPA_DEGEN                      BIT(9)
#define BIT_DCDC_WPA_APTEN                      BIT(8)
#define BITS_DCDC_WPA_DEBC_SEL(x)               (((x) & 0x3) << 6)
#define BITS_DCDC_WPA_CF(x)                     (((x) & 0x3) << 4)
#define BITS_DCDC_WPA_CL_CTRL(x)                (((x) & 0x3) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_WPA_REG2
* Register Offset        :   0x0080
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_WPA_PD                         BIT(13)
#define BIT_DCDC_WPA_OSCSYCEN_SW                BIT(12)
#define BIT_DCDC_WPA_OSCSYCEN_HW_EN             BIT(11)
#define BIT_DCDC_WPA_OSCSYC_DIV_EN              BIT(10)
#define BITS_DCDC_WPA_OSCSYC_DIV(x)             (((x) & 0x1F) << 5)
#define BITS_DCDC_WPA_DISCHRG(x)                (((x) & 0x3) << 3)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_WPA_REG3
* Register Offset        :   0x0084
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_WPA_STBOP(x)                  (((x) & 0x7F) << 6)
#define BITS_DCDC_WPA_ZCD(x)                    (((x) & 0x3) << 4)
#define BITS_DCDC_WPA_PFMAD(x)                  (((x) & 0xF) << 0)

/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_WPA_REG3
* Register Offset        :   0x0084
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_WPA_STBOP(x)                  (((x) & 0x7F) << 6)
#define BITS_DCDC_WPA_ZCD(x)                    (((x) & 0x3) << 4)
#define BITS_DCDC_WPA_PFMAD(x)                  (((x) & 0xF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_WPA_VOL
* Register Offset        :   0x0088
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_WPA_CTRL(x)                   (((x) & 0x1F) << 5)
#define BITS_DCDC_WPA_CAL(x)                    (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_WPA_DCM_HW
* Register Offset        :   0x008C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_WPA_DCMB_HW                    BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CH_CTRL
* Register Offset        :   0x0090
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CAL_SEL(x)                    (((x) & 0xF) << 12)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CTRL
* Register Offset        :   0x0094
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_ILIMITCAL(x)                  (((x) & 0x1F) << 6)
#define BIT_DCDC_CLSEL                          BIT(5)
#define BITS_DCDC_VREF_CAL(x)                   (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CLK_CTRL
* Register Offset        :   0x0098
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_3MCLKCAL_EN                    BIT(13)
#define BIT_DCDC_2MCLKCAL_EN                    BIT(12)
#define BITS_DCDC_6MFRECAL_SW(x)                (((x) & 0x1F) << 7)
#define BITS_DCDC_4MFRECAL_SW(x)                (((x) & 0x1F) << 2)
#define BIT_DCDC_CLK_SP_SEL                     BIT(1)
#define BIT_DCDC_CLK_SP_EN                      BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG6
* Register Offset        :   0x009C
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG7
* Register Offset        :   0x00A0
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG8
* Register Offset        :   0x00A4
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG9
* Register Offset        :   0x00A8
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG10
* Register Offset        :   0x00AC
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CAMA_REG0
* Register Offset        :   0x00B0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CAMA_REFTRIM(x)                (((x) & 0x1F) << 3)
#define BIT_LDO_CAMA_EADBIAS_EN                 BIT(2)
#define BIT_LDO_CAMA_SHPT_PD                    BIT(1)
#define BIT_LDO_CAMA_PD                         BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CAMA_REG1
* Register Offset        :   0x00B4
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CAMA_V(x)                      (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CAMMOT_REG0
* Register Offset        :   0x00B8
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CAMMOT_REFTRIM(x)              (((x) & 0x1F) << 3)
#define BIT_LDO_CAMMOT_EADBIAS_EN               BIT(2)
#define BIT_LDO_CAMMOT_SHPT_PD                  BIT(1)
#define BIT_LDO_CAMMOT_PD                       BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CAMMOT_REG1
* Register Offset        :   0x00BC
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CAMMOT_V(x)                    (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SIM0_REG0
* Register Offset        :   0x00C0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SIM0_REFTRIM(x)                (((x) & 0x1F) << 3)
#define BIT_LDO_SIM0_EADBIAS_EN                 BIT(2)
#define BIT_LDO_SIM0_SHPT_PD                    BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SIM0_PD_REG
* Register Offset        :   0x00C4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_SIM0_PD                         BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SIM0_REG1
* Register Offset        :   0x00C8
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SIM0_V(x)                      (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SIM1_REG0
* Register Offset        :   0x00CC
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SIM1_REFTRIM(x)                (((x) & 0x1F) << 3)
#define BIT_LDO_SIM1_EADBIAS_EN                 BIT(2)
#define BIT_LDO_SIM1_SHPT_PD                    BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SIM1_PD_REG
* Register Offset        :   0x00D0
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_SIM1_PD                         BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SIM1_REG1
* Register Offset        :   0x00D4
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SIM1_V(x)                      (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SIM2_REG0
* Register Offset        :   0x00D8
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SIM2_REFTRIM(x)                (((x) & 0x1F) << 3)
#define BIT_LDO_SIM2_EADBIAS_EN                 BIT(2)
#define BIT_LDO_SIM2_SHPT_PD                    BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SIM2_PD_REG
* Register Offset        :   0x00DC
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_SIM2_PD                         BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SIM2_REG1
* Register Offset        :   0x00E0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SIM2_V(x)                      (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_EMMCCORE_REG0
* Register Offset        :   0x00E4
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_EMMCCORE_REFTRIM(x)            (((x) & 0x1F) << 3)
#define BIT_LDO_EMMCCORE_EADBIAS_EN             BIT(2)
#define BIT_LDO_EMMCCORE_SHPT_PD                BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_EMMCCORE_PD_REG
* Register Offset        :   0x00E8
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_EMMCCORE_PD                     BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_EMMCCORE_REG1
* Register Offset        :   0x00EC
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_EMMCCORE_V(x)                  (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SD_REG0
* Register Offset        :   0x00F0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SDCORE_REFTRIM(x)              (((x) & 0x1F) << 3)
#define BIT_LDO_SDCORE_EADBIAS_EN               BIT(2)
#define BIT_LDO_SDCORE_SHPT_PD                  BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SD_PD_REG
* Register Offset        :   0x00F4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_SDCORE_PD                       BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SD_REG1
* Register Offset        :   0x00F8
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SDCORE_V(x)                    (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SDIO_REG0
* Register Offset        :   0x00FC
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SDIO_REFTRIM(x)                (((x) & 0x1F) << 3)
#define BIT_LDO_SDIO_EADBIAS_EN                 BIT(2)
#define BIT_LDO_SDIO_SHPT_PD                    BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SDIO_PD_REG
* Register Offset        :   0x0100
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_SDIO_PD                         BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_SDIO_REG1
* Register Offset        :   0x0104
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_SDIO_V(x)                      (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_VDD28_REG0
* Register Offset        :   0x0108
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_VDD28_REFTRIM(x)               (((x) & 0x1F) << 3)
#define BIT_LDO_VDD28_EADBIAS_EN                BIT(2)
#define BIT_LDO_VDD28_SHPT_PD                   BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_VDD28_REG1
* Register Offset        :   0x010C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_VDD28_V(x)                     (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_WIFIPA_REG0
* Register Offset        :   0x0110
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_WIFIPA_REFTRIM(x)              (((x) & 0x1F) << 3)
#define BIT_LDO_WIFIPA_EADBIAS_EN               BIT(2)
#define BIT_LDO_WIFIPA_SHPT_PD                  BIT(1)
#define BIT_LDO_WIFIPA_PD                       BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_WIFIPA_REG1
* Register Offset        :   0x0114
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_WIFIPA_V(x)                    (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_DCXO_REG0
* Register Offset        :   0x0118
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_DCXO_REFTRIM(x)                (((x) & 0x1F) << 3)
#define BIT_LDO_DCXO_EADBIAS_EN                 BIT(2)
#define BIT_LDO_DCXO_SHPT_PD                    BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_DCXO_REG1
* Register Offset        :   0x011C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_DCXO_V(x)                      (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_USB33_REG0
* Register Offset        :   0x0120
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_USB33_REFTRIM(x)               (((x) & 0x1F) << 3)
#define BIT_LDO_USB33_EADBIAS_EN                BIT(2)
#define BIT_LDO_USB33_SHPT_PD                   BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_USB33_PD_REG
* Register Offset        :   0x0124
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_USB33_PD                        BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_USB33_REG1
* Register Offset        :   0x0128
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_USB33_V(x)                     (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CAMD_REG0
* Register Offset        :   0x012C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CAMD_REFTRIM(x)                (((x) & 0x1F) << 3)
#define BIT_LDO_CAMD_EADBIAS_EN                 BIT(2)
#define BIT_LDO_CAMD_SHPT_PD                    BIT(1)
#define BIT_LDO_CAMD_PD                         BIT(0)

/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CAMD_REG1
* Register Offset        :   0x0130
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CAMD_V(x)                      (((x) & 0x7F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CON_REG0
* Register Offset        :   0x0134
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CON_REFTRIM(x)                 (((x) & 0x1F) << 3)
#define BIT_LDO_CON_EADBIAS_EN                  BIT(2)
#define BIT_LDO_CON_SHPT_PD                     BIT(1)
#define BIT_LDO_CON_PD                          BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CON_REG1
* Register Offset        :   0x0138
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CON_V(x)                       (((x) & 0x7F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CAMIO_REG0
* Register Offset        :   0x013C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CAMIO_REFTRIM(x)               (((x) & 0x1F) << 3)
#define BIT_LDO_CAMIO_EADBIAS_EN                BIT(2)
#define BIT_LDO_CAMIO_SHPT_PD                   BIT(1)
#define BIT_LDO_CAMIO_PD                        BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CAMIO_REG1
* Register Offset        :   0x0140
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_CAMIO_V(x)                     (((x) & 0x7F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_AVDD18_REG0
* Register Offset        :   0x0144
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_AVDD18_REFTRIM(x)              (((x) & 0x1F) << 3)
#define BIT_LDO_AVDD18_EADBIAS_EN               BIT(2)
#define BIT_LDO_AVDD18_SHPT_PD                  BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_AVDD18_REG1
* Register Offset        :   0x0148
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_AVDD18_V(x)                    (((x) & 0x7F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_VDDRF18_REG0
* Register Offset        :   0x014C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_RF18_REFTRIM(x)                (((x) & 0x1F) << 3)
#define BIT_LDO_RF18_EADBIAS_EN                 BIT(2)
#define BIT_LDO_RF18_SHPT_PD                    BIT(1)
#define BIT_LDO_RF18_PD                         BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_VDDRF18_REG1
* Register Offset        :   0x0150
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_RF18_V(x)                      (((x) & 0x7F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_VDDRF15_REG0
* Register Offset        :   0x0154
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_RF15_REFTRIM(x)                (((x) & 0x1F) << 3)
#define BIT_LDO_RF15_EADBIAS_EN                 BIT(2)
#define BIT_LDO_RF15_SHPT_PD                    BIT(1)
#define BIT_LDO_RF15_PD                         BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_VDDRF15_REG1
* Register Offset        :   0x0158
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_RF15_V(x)                      (((x) & 0x7F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_MEM_REG0
* Register Offset        :   0x015C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_MEM_REFTRIM(x)                 (((x) & 0x1F) << 3)
#define BIT_LDO_MEM_EADBIAS_EN                  BIT(2)
#define BIT_LDO_MEM_SHPT_PD                     BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_MEM_REG1
* Register Offset        :   0x0160
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_MEM_V(x)                       (((x) & 0x7F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG17
* Register Offset        :   0x0164
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG18
* Register Offset        :   0x0168
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG19
* Register Offset        :   0x016C
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG20
* Register Offset        :   0x0170
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG21
* Register Offset        :   0x0174
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_RTC_CTRL
* Register Offset        :   0x0178
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_RTC_CAL(x)                     (((x) & 0x1F) << 4)
#define BITS_LDO_RTC_V(x)                       (((x) & 0x3) << 2)
#define BITS_VBATBK_V(x)                        (((x) & 0x3) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_CH_CTRL
* Register Offset        :   0x017C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDOB_CAL_SEL(x)                    (((x) & 0x7) << 8)
#define BITS_LDOA_CAL_SEL(x)                    (((x) & 0x7) << 5)
#define BITS_LDODCDC_CAL_SEL(x)                 (((x) & 0x7) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG23
* Register Offset        :   0x0180
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG24
* Register Offset        :   0x0184
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG25
* Register Offset        :   0x0188
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG26
* Register Offset        :   0x018C
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG27
* Register Offset        :   0x0190
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SLP_WAIT_DCDCCPU
* Register Offset        :   0x0194
* Register Description   :
-----------------------------------------------------------*/

#define BITS_SLP_IN_WAIT_DCDCCPU(x)             (((x) & 0xFF) << 8)
#define BITS_SLP_OUT_WAIT_DCDCCPU(x)            (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SLP_CTRL
* Register Offset        :   0x0198
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_XTL_EN                          BIT(2)
#define BIT_SLP_IO_EN                           BIT(1)
#define BIT_SLP_LDO_PD_EN                       BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SLP_DCDC_PD_CTRL
* Register Offset        :   0x019C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_SLP_DCDCCORE_DROP_EN                BIT(3)
#define BIT_SLP_DCDCWPA_PD_EN                   BIT(2)
#define BIT_SLP_DCDCCPU_PD_EN                   BIT(1)
#define BIT_SLP_DCDCMEM_PD_EN                   BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SLP_LDO_PD_CTRL0
* Register Offset        :   0x01A0
* Register Description   :
-----------------------------------------------------------*/

#define BIT_SLP_LDORF15_PD_EN                   BIT(14)
#define BIT_SLP_LDORF18_PD_EN                   BIT(13)
#define BIT_SLP_LDOEMMCCORE_PD_EN               BIT(12)
#define BIT_SLP_LDODCXO_PD_EN                   BIT(11)
#define BIT_SLP_LDOWIFIPA_PD_EN                 BIT(10)
#define BIT_SLP_LDOVDD28_PD_EN                  BIT(9)
#define BIT_SLP_LDOSDCORE_PD_EN                 BIT(8)
#define BIT_SLP_LDOSDIO_PD_EN                   BIT(7)
#define BIT_SLP_LDOUSB33_PD_EN                  BIT(6)
#define BIT_SLP_LDOCAMMOT_PD_EN                 BIT(5)
#define BIT_SLP_LDOCAMIO_PD_EN                  BIT(4)
#define BIT_SLP_LDOCAMD_PD_EN                   BIT(3)
#define BIT_SLP_LDOCAMA_PD_EN                   BIT(2)
#define BIT_SLP_LDOSIM2_PD_EN                   BIT(1)
#define BIT_SLP_LDOSIM1_PD_EN                   BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SLP_LDO_PD_CTRL1
* Register Offset        :   0x01A4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_SLP_LDOCON_PD_EN                    BIT(3)
#define BIT_SLP_LDOSIM0_PD_EN                   BIT(2)
#define BIT_SLP_LDOAVDD18_PD_EN                 BIT(1)
#define BIT_SLP_LDOMEM_PD_EN                    BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SLP_DCDC_LP_CTRL
* Register Offset        :   0x01A8
* Register Description   :
-----------------------------------------------------------*/

#define BIT_SLP_DCDCCORE_LP_EN                  BIT(4)
#define BIT_SLP_DCDCMEM_LP_EN                   BIT(3)
#define BIT_SLP_DCDCCPU_LP_EN                   BIT(2)
#define BIT_SLP_DCDCGEN_LP_EN                   BIT(1)
#define BIT_SLP_DCDCWPA_LP_EN                   BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SLP_LDO_LP_CTRL0
* Register Offset        :   0x01AC
* Register Description   :
-----------------------------------------------------------*/

#define BIT_SLP_LDORF15_LP_EN                   BIT(14)
#define BIT_SLP_LDORF18_LP_EN                   BIT(13)
#define BIT_SLP_LDOEMMCCORE_LP_EN               BIT(12)
#define BIT_SLP_LDODCXO_LP_EN                   BIT(11)
#define BIT_SLP_LDOWIFIPA_LP_EN                 BIT(10)
#define BIT_SLP_LDOVDD28_LP_EN                  BIT(9)
#define BIT_SLP_LDOSDCORE_LP_EN                 BIT(8)
#define BIT_SLP_LDOSDIO_LP_EN                   BIT(7)
#define BIT_SLP_LDOUSB33_LP_EN                  BIT(6)
#define BIT_SLP_LDOCAMMOT_LP_EN                 BIT(5)
#define BIT_SLP_LDOCAMIO_LP_EN                  BIT(4)
#define BIT_SLP_LDOCAMD_LP_EN                   BIT(3)
#define BIT_SLP_LDOCAMA_LP_EN                   BIT(2)
#define BIT_SLP_LDOSIM2_LP_EN                   BIT(1)
#define BIT_SLP_LDOSIM1_LP_EN                   BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SLP_LDO_LP_CTRL1
* Register Offset        :   0x01B0
* Register Description   :
-----------------------------------------------------------*/

#define BIT_SLP_LDOCON_LP_EN                    BIT(3)
#define BIT_SLP_LDOSIM0_LP_EN                   BIT(2)
#define BIT_SLP_LDOAVDD18_LP_EN                 BIT(1)
#define BIT_SLP_LDOMEM_LP_EN                    BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_SLP_CTRL0
* Register Offset        :   0x01B4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_CORE_SLP_OUT_STEP_EN           BIT(1)
#define BIT_DCDC_CORE_SLP_IN_STEP_EN            BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_SLP_CTRL1
* Register Offset        :   0x01B8
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CORE_CAL_DS_SW(x)             (((x) & 0x1F) << 5)
#define BITS_DCDC_CORE_CTRL_DS_SW(x)            (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_SLP_CTRL2
* Register Offset        :   0x01BC
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CORE_CTRL_SLP_STEP3(x)        (((x) & 0x1F) << 10)
#define BITS_DCDC_CORE_CTRL_SLP_STEP2(x)        (((x) & 0x1F) << 5)
#define BITS_DCDC_CORE_CTRL_SLP_STEP1(x)        (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_SLP_CTRL3
* Register Offset        :   0x01C0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CORE_CTRL_SLP_STEP5(x)        (((x) & 0x1F) << 5)
#define BITS_DCDC_CORE_CTRL_SLP_STEP4(x)        (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_SLP_CTRL4
* Register Offset        :   0x01C4
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CORE_CAL_SLP_STEP3(x)         (((x) & 0x1F) << 10)
#define BITS_DCDC_CORE_CAL_SLP_STEP2(x)         (((x) & 0x1F) << 5)
#define BITS_DCDC_CORE_CAL_SLP_STEP1(x)         (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CORE_SLP_CTRL5
* Register Offset        :   0x01C8
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CORE_CAL_SLP_STEP5(x)         (((x) & 0x1F) << 5)
#define BITS_DCDC_CORE_CAL_SLP_STEP4(x)         (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_SLP_CTRL0
* Register Offset        :   0x01CC
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_CPU_EN_CTRL                    BIT(2)
#define BIT_DCDC_CPU_SLP_OUT_STEP_EN            BIT(1)
#define BIT_DCDC_CPU_SLP_IN_STEP_EN             BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_SLP_CTRL1
* Register Offset        :   0x01D0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CPU_CAL_DS_SW(x)              (((x) & 0x1F) << 5)
#define BITS_DCDC_CPU_CTRL_DS_SW(x)             (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_SLP_CTRL2
* Register Offset        :   0x01D4
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CPU_CTRL_SLP_STEP3(x)         (((x) & 0x1F) << 10)
#define BITS_DCDC_CPU_CTRL_SLP_STEP2(x)         (((x) & 0x1F) << 5)
#define BITS_DCDC_CPU_CTRL_SLP_STEP1(x)         (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_SLP_CTRL3
* Register Offset        :   0x01D8
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CPU_CTRL_SLP_STEP5(x)         (((x) & 0x1F) << 5)
#define BITS_DCDC_CPU_CTRL_SLP_STEP4(x)         (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_SLP_CTRL4
* Register Offset        :   0x01DC
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CPU_CAL_SLP_STEP3(x)          (((x) & 0x1F) << 10)
#define BITS_DCDC_CPU_CAL_SLP_STEP2(x)          (((x) & 0x1F) << 5)
#define BITS_DCDC_CPU_CAL_SLP_STEP1(x)          (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_CPU_SLP_CTRL5
* Register Offset        :   0x01E0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCDC_CPU_CAL_SLP_STEP5(x)          (((x) & 0x1F) << 5)
#define BITS_DCDC_CPU_CAL_SLP_STEP4(x)          (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_XTL_EN0
* Register Offset        :   0x01E4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_CORE_EXT_XTL0_EN               BIT(15)
#define BIT_DCDC_CORE_EXT_XTL1_EN               BIT(14)
#define BIT_DCDC_CORE_EXT_XTL2_EN               BIT(13)
#define BIT_DCDC_CORE_EXT_XTL3_EN               BIT(12)
#define BIT_DCDC_WPA_EXT_XTL0_EN                BIT(3)
#define BIT_DCDC_WPA_EXT_XTL1_EN                BIT(2)
#define BIT_DCDC_WPA_EXT_XTL2_EN                BIT(1)
#define BIT_DCDC_WPA_EXT_XTL3_EN                BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_XTL_EN1
* Register Offset        :   0x01E8
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_CPU_EXT_XTL0_EN                BIT(15)
#define BIT_DCDC_CPU_EXT_XTL1_EN                BIT(14)
#define BIT_DCDC_CPU_EXT_XTL2_EN                BIT(13)
#define BIT_DCDC_CPU_EXT_XTL3_EN                BIT(12)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_XTL_EN2
* Register Offset        :   0x01EC
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCDC_MEM_EXT_XTL0_EN                BIT(15)
#define BIT_DCDC_MEM_EXT_XTL1_EN                BIT(14)
#define BIT_DCDC_MEM_EXT_XTL2_EN                BIT(13)
#define BIT_DCDC_MEM_EXT_XTL3_EN                BIT(12)
#define BIT_DCDC_GEN_EXT_XTL0_EN                BIT(3)
#define BIT_DCDC_GEN_EXT_XTL1_EN                BIT(2)
#define BIT_DCDC_GEN_EXT_XTL2_EN                BIT(1)
#define BIT_DCDC_GEN_EXT_XTL3_EN                BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESEVED_DCDC_XTL_EN4
* Register Offset        :   0x01F0
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN0
* Register Offset        :   0x01F4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_DCXO_EXT_XTL0_EN                BIT(15)
#define BIT_LDO_DCXO_EXT_XTL1_EN                BIT(14)
#define BIT_LDO_DCXO_EXT_XTL2_EN                BIT(13)
#define BIT_LDO_DCXO_EXT_XTL3_EN                BIT(12)
#define BIT_LDO_VDD28_EXT_XTL0_EN               BIT(3)
#define BIT_LDO_VDD28_EXT_XTL1_EN               BIT(2)
#define BIT_LDO_VDD28_EXT_XTL2_EN               BIT(1)
#define BIT_LDO_VDD28_EXT_XTL3_EN               BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN1
* Register Offset        :   0x01F8
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_RF15_EXT_XTL0_EN                BIT(15)
#define BIT_LDO_RF15_EXT_XTL1_EN                BIT(14)
#define BIT_LDO_RF15_EXT_XTL2_EN                BIT(13)
#define BIT_LDO_RF15_EXT_XTL3_EN                BIT(12)
#define BIT_LDO_RF18_EXT_XTL0_EN                BIT(3)
#define BIT_LDO_RF18_EXT_XTL1_EN                BIT(2)
#define BIT_LDO_RF18_EXT_XTL2_EN                BIT(1)
#define BIT_LDO_RF18_EXT_XTL3_EN                BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN2
* Register Offset        :   0x01FC
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_SIM0_EXT_XTL0_EN                BIT(15)
#define BIT_LDO_SIM0_EXT_XTL1_EN                BIT(14)
#define BIT_LDO_SIM0_EXT_XTL2_EN                BIT(13)
#define BIT_LDO_SIM0_EXT_XTL3_EN                BIT(12)
#define BIT_LDO_SIM1_EXT_XTL0_EN                BIT(3)
#define BIT_LDO_SIM1_EXT_XTL1_EN                BIT(2)
#define BIT_LDO_SIM1_EXT_XTL2_EN                BIT(1)
#define BIT_LDO_SIM1_EXT_XTL3_EN                BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN3
* Register Offset        :   0x0200
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_SIM2_EXT_XTL0_EN                BIT(15)
#define BIT_LDO_SIM2_EXT_XTL1_EN                BIT(14)
#define BIT_LDO_SIM2_EXT_XTL2_EN                BIT(13)
#define BIT_LDO_SIM2_EXT_XTL3_EN                BIT(12)
#define BIT_LDO_MEM_EXT_XTL0_EN                 BIT(3)
#define BIT_LDO_MEM_EXT_XTL1_EN                 BIT(2)
#define BIT_LDO_MEM_EXT_XTL2_EN                 BIT(1)
#define BIT_LDO_MEM_EXT_XTL3_EN                 BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN4
* Register Offset        :   0x0204
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_CAMMOT_EXT_XTL0_EN              BIT(15)
#define BIT_LDO_CAMMOT_EXT_XTL1_EN              BIT(14)
#define BIT_LDO_CAMMOT_EXT_XTL2_EN              BIT(13)
#define BIT_LDO_CAMMOT_EXT_XTL3_EN              BIT(12)
#define BIT_LDO_CAMIO_EXT_XTL0_EN               BIT(3)
#define BIT_LDO_CAMIO_EXT_XTL1_EN               BIT(2)
#define BIT_LDO_CAMIO_EXT_XTL2_EN               BIT(1)
#define BIT_LDO_CAMIO_EXT_XTL3_EN               BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN5
* Register Offset        :   0x0208
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_CAMA_EXT_XTL0_EN                BIT(15)
#define BIT_LDO_CAMA_EXT_XTL1_EN                BIT(14)
#define BIT_LDO_CAMA_EXT_XTL2_EN                BIT(13)
#define BIT_LDO_CAMA_EXT_XTL3_EN                BIT(12)
#define BIT_LDO_CAMD_EXT_XTL0_EN                BIT(3)
#define BIT_LDO_CAMD_EXT_XTL1_EN                BIT(2)
#define BIT_LDO_CAMD_EXT_XTL2_EN                BIT(1)
#define BIT_LDO_CAMD_EXT_XTL3_EN                BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN6
* Register Offset        :   0x020C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_SDIO_EXT_XTL0_EN                BIT(15)
#define BIT_LDO_SDIO_EXT_XTL1_EN                BIT(14)
#define BIT_LDO_SDIO_EXT_XTL2_EN                BIT(13)
#define BIT_LDO_SDIO_EXT_XTL3_EN                BIT(12)
#define BIT_LDO_SDCORE_EXT_XTL0_EN              BIT(3)
#define BIT_LDO_SDCORE_EXT_XTL1_EN              BIT(2)
#define BIT_LDO_SDCORE_EXT_XTL2_EN              BIT(1)
#define BIT_LDO_SDCORE_EXT_XTL3_EN              BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN7
* Register Offset        :   0x0210
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_EMMCCORE_EXT_XTL0_EN            BIT(15)
#define BIT_LDO_EMMCCORE_EXT_XTL1_EN            BIT(14)
#define BIT_LDO_EMMCCORE_EXT_XTL2_EN            BIT(13)
#define BIT_LDO_EMMCCORE_EXT_XTL3_EN            BIT(12)
#define BIT_LDO_USB33_EXT_XTL0_EN               BIT(3)
#define BIT_LDO_USB33_EXT_XTL1_EN               BIT(2)
#define BIT_LDO_USB33_EXT_XTL2_EN               BIT(1)
#define BIT_LDO_USB33_EXT_XTL3_EN               BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN8
* Register Offset        :   0x0214
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_KPLED_EXT_XTL0_EN               BIT(15)
#define BIT_LDO_KPLED_EXT_XTL1_EN               BIT(14)
#define BIT_LDO_KPLED_EXT_XTL2_EN               BIT(13)
#define BIT_LDO_KPLED_EXT_XTL3_EN               BIT(12)
#define BIT_LDO_VIBR_EXT_XTL0_EN                BIT(3)
#define BIT_LDO_VIBR_EXT_XTL1_EN                BIT(2)
#define BIT_LDO_VIBR_EXT_XTL2_EN                BIT(1)
#define BIT_LDO_VIBR_EXT_XTL3_EN                BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN9
* Register Offset        :   0x0218
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_CON_EXT_XTL0_EN                 BIT(15)
#define BIT_LDO_CON_EXT_XTL1_EN                 BIT(14)
#define BIT_LDO_CON_EXT_XTL2_EN                 BIT(13)
#define BIT_LDO_CON_EXT_XTL3_EN                 BIT(12)
#define BIT_LDO_AVDD18_EXT_XTL0_EN              BIT(3)
#define BIT_LDO_AVDD18_EXT_XTL1_EN              BIT(2)
#define BIT_LDO_AVDD18_EXT_XTL2_EN              BIT(1)
#define BIT_LDO_AVDD18_EXT_XTL3_EN              BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_XTL_EN10
* Register Offset        :   0x021C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_WIFIPA_EXT_XTL0_EN              BIT(3)
#define BIT_LDO_WIFIPA_EXT_XTL1_EN              BIT(2)
#define BIT_LDO_WIFIPA_EXT_XTL2_EN              BIT(1)
#define BIT_LDO_WIFIPA_EXT_XTL3_EN              BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCXO_XTL_EN
* Register Offset        :   0x0220
* Register Description   :
-----------------------------------------------------------*/

#define BIT_TSX_XO_EXT_XTL0_EN                  BIT(15)
#define BIT_TSX_XO_EXT_XTL1_EN                  BIT(14)
#define BIT_TSX_XO_EXT_XTL2_EN                  BIT(13)
#define BIT_TSX_XO_EXT_XTL3_EN                  BIT(12)
#define BIT_XO_EXT_XTL0_EN                      BIT(3)
#define BIT_XO_EXT_XTL1_EN                      BIT(2)
#define BIT_XO_EXT_XTL2_EN                      BIT(1)
#define BIT_XO_EXT_XTL3_EN                      BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG30
* Register Offset        :   0x0224
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG31
* Register Offset        :   0x0228
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG32
* Register Offset        :   0x022C
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG33
* Register Offset        :   0x0230
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG34
* Register Offset        :   0x0234
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSX_CTRL0
* Register Offset        :   0x0238
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCXO_26M_REF_OUT3_PIN_EXT_XTL3_EN   BIT(15)
#define BIT_DCXO_26M_REF_OUT2_PIN_EXT_XTL3_EN   BIT(14)
#define BIT_DCXO_26M_REF_OUT1_PIN_EXT_XTL3_EN   BIT(13)
#define BIT_DCXO_26M_REF_OUT0_PIN_EXT_XTL3_EN   BIT(12)
#define BIT_DCXO_26M_REF_OUT3_PIN_EXT_XTL2_EN   BIT(11)
#define BIT_DCXO_26M_REF_OUT2_PIN_EXT_XTL2_EN   BIT(10)
#define BIT_DCXO_26M_REF_OUT1_PIN_EXT_XTL2_EN   BIT(9)
#define BIT_DCXO_26M_REF_OUT0_PIN_EXT_XTL2_EN   BIT(8)
#define BIT_SLP_DCXO_26M_REF_OUT3_EN            BIT(7)
#define BIT_SLP_DCXO_26M_REF_OUT2_EN            BIT(6)
#define BIT_SLP_DCXO_26M_REF_OUT1_EN            BIT(5)
#define BIT_SLP_DCXO_26M_REF_OUT0_EN            BIT(4)
#define BIT_DCXO_26M_REF_OUT3_EN_SW             BIT(3)
#define BIT_DCXO_26M_REF_OUT2_EN_SW             BIT(2)
#define BIT_DCXO_26M_REF_OUT1_EN_SW             BIT(1)
#define BIT_DCXO_26M_REF_OUT0_EN_SW             BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSX_CTRL1
* Register Offset        :   0x023C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCXO_26M_REF_OUT3_EXT_XTL3_EN       BIT(15)
#define BIT_DCXO_26M_REF_OUT2_EXT_XTL3_EN       BIT(14)
#define BIT_DCXO_26M_REF_OUT1_EXT_XTL3_EN       BIT(13)
#define BIT_DCXO_26M_REF_OUT0_EXT_XTL3_EN       BIT(12)
#define BIT_DCXO_26M_REF_OUT3_EXT_XTL2_EN       BIT(11)
#define BIT_DCXO_26M_REF_OUT2_EXT_XTL2_EN       BIT(10)
#define BIT_DCXO_26M_REF_OUT1_EXT_XTL2_EN       BIT(9)
#define BIT_DCXO_26M_REF_OUT0_EXT_XTL2_EN       BIT(8)
#define BIT_DCXO_26M_REF_OUT3_EXT_XTL1_EN       BIT(7)
#define BIT_DCXO_26M_REF_OUT2_EXT_XTL1_EN       BIT(6)
#define BIT_DCXO_26M_REF_OUT1_EXT_XTL1_EN       BIT(5)
#define BIT_DCXO_26M_REF_OUT0_EXT_XTL1_EN       BIT(4)
#define BIT_DCXO_26M_REF_OUT3_EXT_XTL0_EN       BIT(3)
#define BIT_DCXO_26M_REF_OUT2_EXT_XTL0_EN       BIT(2)
#define BIT_DCXO_26M_REF_OUT1_EXT_XTL0_EN       BIT(1)
#define BIT_DCXO_26M_REF_OUT0_EXT_XTL0_EN       BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSX_CTRL2
* Register Offset        :   0x0240
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCXO_LP_EN                          BIT(14)
#define BIT_SLP_DCXO_LP_EN                      BIT(13)
#define BITS_DCXO_LP_REF_CTRL(x)                (((x) & 0x7) << 10)
#define BIT_DCXO_GM_HELPER                      BIT(9)
#define BITS_DCXO_CBANK(x)                      (((x) & 0x7F) << 2)
#define BIT_DCXO_CBANKMSB                       BIT(1)
#define BIT_DCXO_BUF_MODE_SEL                   BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSX_CTRL3
* Register Offset        :   0x0244
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCXO_DIV_RATIO_FRAC_HC(x)          (((x) & 0xFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSX_CTRL4
* Register Offset        :   0x0248
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCXO_DIV_RATIO_FRAC_LC(x)          (((x) & 0xFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSX_CTRL5
* Register Offset        :   0x024C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCXO_REF_BUF3_LEVEL_PUSH(x)        (((x) & 0x7) << 7)
#define BIT_DCXO_REF_BUF3_CCHARGE_2P            BIT(6)
#define BITS_DCXO_REF_BUF3_LEVEL_RSOURCE(x)     (((x) & 0x7) << 3)
#define BITS_DCXO_REF_BUF3_LEVEL_RCHARGE(x)     (((x) & 0x7) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSX_CTRL6
* Register Offset        :   0x0250
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCXO_DIV32K_RES_SW(x)              (((x) & 0x7) << 3)
#define BITS_DCXO_OUT32K_RES_SW(x)              (((x) & 0x7) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSX_CTRL7
* Register Offset        :   0x0254
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DCXO_RESERVED(x)                   (((x) & 0x3F) << 10)
#define BITS_DCXO_CAL(x)                        (((x) & 0xF) << 6)
#define BIT_AML_TUNE_OK_FLAG                    BIT(5)
#define BITS_DCXO_AML_CTRL(x)                   (((x) & 0xF) << 1)
#define BIT_DCXO_AML_EN                         BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG36
* Register Offset        :   0x0258
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG37
* Register Offset        :   0x025C
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG38
* Register Offset        :   0x0260
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSEN_CTRL0
* Register Offset        :   0x0264
* Register Description   :
-----------------------------------------------------------*/

#define BIT_TSEN_ADCLDO_EN                      BIT(15)
#define BITS_TSEN_SDADC_BIAS(x)                 (((x) & 0x3) << 13)
#define BITS_TSEN_CLKSEL(x)                     (((x) & 0x3) << 11)
#define BITS_TSEN_CHOP_CLKSEL(x)                (((x) & 0x3) << 9)
#define BITS_TSEN_ADCLDO_V(x)                   (((x) & 0xF) << 5)
#define BITS_TSEN_ADCLDOREF(x)                  (((x) & 0x1F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSEN_CTRL1
* Register Offset        :   0x0268
* Register Description   :
-----------------------------------------------------------*/

#define BIT_TSEN_DATA_EDGE_SEL                  BIT(15)
#define BIT_TSEN_UGBUF_EN                       BIT(14)
#define BIT_TSEN_INPUT_EN                       BIT(13)
#define BIT_TSEN_SDADC_OFFSET_EN                BIT(12)
#define BIT_TSEN_SDADC_EN                       BIT(11)
#define BIT_TSEN_UGBUF_CHOP_EN                  BIT(10)
#define BIT_TSEN_SDADC_CHOP_EN                  BIT(9)
#define BIT_TSEN_SDADC_CAPCHOP_EN               BIT(8)
#define BITS_UGBUF_CTRL(x)                      (((x) & 0x3) << 6)
#define BITS_TSEN_SDADC_VCMO(x)                 (((x) & 0x3) << 4)
#define BITS_TSEN_SDADC_VCMI(x)                 (((x) & 0x3) << 2)
#define BITS_TSEN_UGBUF_BIAS(x)                 (((x) & 0x3) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_TSEN_CTRL2
* Register Offset        :   0x026C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_TSEN_RESERVED(x)                   (((x) & 0xFF) << 1)
#define BIT_CLK_TSEN_SEL                        BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG40
* Register Offset        :   0x0270
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RESERVED_REG41
* Register Offset        :   0x0274
* Register Description   :
-----------------------------------------------------------*/



/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_BG_CTRL
* Register Offset        :   0x0278
* Register Description   :
-----------------------------------------------------------*/

#define BITS_BG_RESERVED(x)                     (((x) & 0xF) << 1)
#define BIT_BG_CHOP_EN                          BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_DCDC_VLG_SEL
* Register Offset        :   0x027C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_VOL_MODE                            BIT(8)
#define BIT_DCDC_CPU_VOL_SEL                    BIT(7)
#define BIT_DCDC_CORE_VOL_SEL                   BIT(6)
#define BIT_DCDC_WPA_SW_SEL                     BIT(5)
#define BIT_DCDC_GEN_SW_SEL                     BIT(4)
#define BIT_DCDC_MEM_SW_SEL                     BIT(3)
#define BIT_DCDC_CPU_SW_SEL                     BIT(2)
#define BIT_DCDC_CORE_SLP_SW_SEL                BIT(1)
#define BIT_DCDC_CORE_NOR_SW_SEL                BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_VLG_SEL0
* Register Offset        :   0x0280
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_SIM0_SW_SEL                     BIT(15)
#define BIT_LDO_SIM1_SW_SEL                     BIT(14)
#define BIT_LDO_RF15_SW_SEL                     BIT(13)
#define BIT_LDO_USB33_SW_SEL                    BIT(12)
#define BIT_LDO_EMMCCORE_SW_SEL                 BIT(11)
#define BIT_LDO_AVDD18_SW_SEL                   BIT(10)
#define BIT_LDO_VDD28_SW_SEL                    BIT(9)
#define BIT_LDO_DCXO_SW_SEL                     BIT(8)
#define BIT_LDO_SDCORE_SW_SEL                   BIT(7)
#define BIT_LDO_SDIO_SW_SEL                     BIT(6)
#define BIT_LDO_CAMMOT_SW_SEL                   BIT(5)
#define BIT_LDO_CAMIO_SW_SEL                    BIT(4)
#define BIT_LDO_CAMA_SW_SEL                     BIT(3)
#define BIT_LDO_RF18_SW_SEL                     BIT(2)
#define BIT_LDO_CAMD_SW_SEL                     BIT(1)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LDO_VLG_SEL1
* Register Offset        :   0x0284
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_RTC_CAL_SW_SEL                  BIT(11)
#define BIT_DCDC_CPU_PFMAD_SW_SEL               BIT(10)
#define BIT_DCDC_CORE_PFMAD_SW_SEL              BIT(9)
#define BIT_DCDC_GEN_PFMAD_SW_SEL               BIT(8)
#define BIT_DCDC_MEM_PFMAD_SW_SEL               BIT(7)
#define BIT_DCDC_WPA_PFMAD_SW_SEL               BIT(6)
#define BIT_LDO_KPLED_SW_SEL                    BIT(5)
#define BIT_LDO_VIBR_SW_SEL                     BIT(4)
#define BIT_LDO_CON_SW_SEL                      BIT(3)
#define BIT_LDO_WIFIPA_SW_SEL                   BIT(2)
#define BIT_LDO_MEM_SW_SEL                      BIT(1)
#define BIT_LDO_SIM2_SW_SEL                     BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CLK32KLESS_CTRL0
* Register Offset        :   0x0288
* Register Description   :
-----------------------------------------------------------*/

#define BIT_TSX_MODE                            BIT(15)
#define BIT_RC_MODE_WR_ACK_FLAG                 BIT(14)
#define BIT_XO_LOW_CUR_FLAG                     BIT(13)
#define BIT_XO_LOW_CUR_FRC_RTCSET               BIT(12)
#define BIT_XO_LOW_CUR_FRC_RTCCLR               BIT(11)
#define BIT_RC_MODE_WR_ACK_FLAG_CLR             BIT(10)
#define BIT_XO_LOW_CUR_FLAG_CLR                 BIT(9)
#define BIT_XO_LOW_CUR_CNT_CLR                  BIT(8)
#define BIT_LDO_DCXO_LP_EN_RTCSET               BIT(7)
#define BIT_LDO_DCXO_LP_EN_RTCCLR               BIT(6)
#define BIT_SLP_XO_LOW_CUR_EN                   BIT(5)
#define BIT_RTC_MODE                            BIT(4)
#define BIT_RC_32K_SEL                          BIT(1)
#define BIT_RC_32K_EN                           BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CLK32KLESS_CTRL1
* Register Offset        :   0x028C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_RC_MODE(x)                         (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CLK32KLESS_CTRL2
* Register Offset        :   0x0290
* Register Description   :
-----------------------------------------------------------*/

#define BITS_XO_LOW_CUR_CNT_LOW(x)              (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CLK32KLESS_CTRL3
* Register Offset        :   0x0294
* Register Description   :
-----------------------------------------------------------*/

#define BITS_XO_LOW_CUR_CNT_HIGH(x)             (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CLK32KLESS_CTRL4
* Register Offset        :   0x0298
* Register Description   :
-----------------------------------------------------------*/

#define BITS_KO_AML_CAL(x)                      (((x) & 0xF) << 11)
#define BIT_KO_AML_EN                           BIT(10)
#define BITS_OSC_32K_CMP_ICTRL(x)               (((x) & 0x3) << 8)
#define BITS_RC1M_CAL(x)                        (((x) & 0x7F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_XTL_WAIT_CTRL0
* Register Offset        :   0x029C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_RSV                                 BIT(15)
#define BIT_CUR_SEL                             BIT(14)
#define BIT_EXT_XTL3_FOR_26M_EN                 BIT(13)
#define BIT_EXT_XTL2_FOR_26M_EN                 BIT(12)
#define BIT_EXT_XTL1_FOR_26M_EN                 BIT(11)
#define BIT_EXT_XTL0_FOR_26M_EN                 BIT(10)
#define BIT_SLP_XTLBUF_PD_EN                    BIT(9)
#define BIT_XTL_EN                              BIT(8)
#define BITS_XTL_WAIT(x)                        (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RGB_CTRL
* Register Offset        :   0x02A0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_RGB_V(x)                           (((x) & 0x1F) << 4)
#define BIT_SLP_RGB_PD_EN                       BIT(2)
#define BIT_RGB_PD_HW_EN                        BIT(1)
#define BIT_RGB_PD_SW                           BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_IB_CTRL
* Register Offset        :   0x02A4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_BATDET_CUR_EN                       BIT(13)
#define BITS_BATDET_CUR_I(x)                    (((x) & 0xF) << 9)
#define BITS_IB_TRIM(x)                         (((x) & 0x7F) << 2)
#define BIT_IB_TRIM_EM_SEL                      BIT(1)
#define BIT_IB_REX_EN                           BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_FLASH_CTRL
* Register Offset        :   0x02A8
* Register Description   :
-----------------------------------------------------------*/

#define BIT_FLASH_PON                           BIT(15)
#define BIT_FLASH_V_HW_EN                       BIT(6)
#define BITS_FLASH_V_HW_STEP(x)                 (((x) & 0x3) << 4)
#define BITS_FLASH_V_SW(x)                      (((x) & 0xF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_KPLED_CTRL0
* Register Offset        :   0x02AC
* Register Description   :
-----------------------------------------------------------*/

#define BITS_KPLED_V(x)                         (((x) & 0xF) << 12)
#define BIT_KPLED_PD                            BIT(11)
#define BIT_KPLED_PULLDOWN_EN                   BIT(10)
#define BIT_SLP_LDOKPLED_PD_EN                  BIT(9)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_KPLED_CTRL1
* Register Offset        :   0x02B0
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_KPLED_PD                        BIT(15)
#define BITS_LDO_KPLED_V(x)                     (((x) & 0xFF) << 7)
#define BITS_LDO_KPLED_REFTRIM(x)               (((x) & 0x1F) << 2)
#define BIT_LDO_KPLED_EADBIAS_EN                BIT(1)
#define BIT_LDO_KPLED_SHPT_PD                   BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_VIBR_CTRL0
* Register Offset        :   0x02B4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_LDO_VIBR_SHPT_PD                    BIT(10)
#define BIT_SLP_LDOVIBR_PD_EN                   BIT(9)
#define BIT_LDO_VIBR_PD                         BIT(8)
#define BITS_LDO_VIBR_V(x)                      (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_VIBR_CTRL1
* Register Offset        :   0x02B8
* Register Description   :
-----------------------------------------------------------*/

#define BITS_LDO_VIBR_REFTRIM(x)                (((x) & 0x1F) << 1)
#define BIT_LDO_VIBR_EADBIAS_EN                 BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_AUDIO_CTRL0
* Register Offset        :   0x02BC
* Register Description   :
-----------------------------------------------------------*/

#define BIT_CLK_AUD_IF_TX_INV_EN                BIT(3)
#define BIT_CLK_AUD_IF_RX_INV_EN                BIT(2)
#define BIT_CLK_AUD_IF_6P5M_TX_INV_EN           BIT(1)
#define BIT_CLK_AUD_IF_6P5M_RX_INV_EN           BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CHGR_CTRL0
* Register Offset        :   0x02C0
* Register Description   :
-----------------------------------------------------------*/

#define BIT_CHGR_PD_EXT                         BIT(15)
#define BITS_CHGR_DPM(x)                        (((x) & 0x3) << 13)
#define BIT_CHGR_CC_EN                          BIT(12)
#define BIT_RECHG                               BIT(11)
#define BITS_CHGR_CV_V(x)                       (((x) & 0x3F) << 5)
#define BITS_CHGR_END_V(x)                      (((x) & 0x3) << 3)
#define BITS_CHGR_ITERM(x)                      (((x) & 0x3) << 1)
#define BIT_CHGR_PD                             BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CHGR_CTRL1
* Register Offset        :   0x02C4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_CHGR_POWER_DEVICE                   BIT(15)
#define BITS_CHGR_CC_I(x)                       (((x) & 0x1F) << 10)
#define BITS_VBAT_OVP_V(x)                      (((x) & 0xF) << 6)
#define BITS_VCHG_OVP_V(x)                      (((x) & 0x3F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CHGR_STATUS
* Register Offset        :   0x02C8
* Register Description   :
-----------------------------------------------------------*/

#define BIT_CHGR_INT_EN                         BIT(13)
#define BIT_NON_DCP_INT                         BIT(12)
#define BIT_CHG_DET_DONE                        BIT(11)
#define BIT_DP_LOW                              BIT(10)
#define BIT_DCP_DET                             BIT(9)
#define BIT_CHG_DET                             BIT(8)
#define BIT_SDP_INT                             BIT(7)
#define BIT_DCP_INT                             BIT(6)
#define BIT_CDP_INT                             BIT(5)
#define BIT_CHGR_CV_STATUS                      BIT(4)
#define BIT_CHGR_ON                             BIT(3)
#define BIT_CHGR_INT                            BIT(2)
#define BIT_VBAT_OVI                            BIT(1)
#define BIT_VCHG_OVI                            BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_CHGR_DET_FGU_CTRL
* Register Offset        :   0x02CC
* Register Description   :
-----------------------------------------------------------*/

#define BIT_FGUA_SOFT_RST                       BIT(13)
#define BIT_LDO_FGU_PD                          BIT(12)
#define BITS_CHG_INT_DELAY(x)                   (((x) & 0x7) << 9)
#define BIT_SD_CHOP_CAP_EN                      BIT(8)
#define BITS_SD_CLK_P(x)                        (((x) & 0x3) << 6)
#define BIT_SD_DCOFFSET_EN                      BIT(5)
#define BIT_SD_CHOP_EN                          BIT(4)
#define BIT_DP_DM_FC_ENB                        BIT(2)
#define BIT_DP_DM_AUX_EN                        BIT(1)
#define BIT_DP_DM_BC_ENB                        BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_OVLO_CTRL
* Register Offset        :   0x02D0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_VBAT_CRASH_V(x)                    (((x) & 0x3) << 10)
#define BIT_OVLO_EN                             BIT(9)
#define BITS_OVLO_V(x)                          (((x) & 0x3) << 2)
#define BITS_OVLO_T(x)                          (((x) & 0x3) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_MIXED_CTRL
* Register Offset        :   0x02D4
* Register Description   :
-----------------------------------------------------------*/

#define BITS_XOSC32K_CTL(x)                     (((x) & 0xF) << 12)
#define BITS_BATON_T(x)                         (((x) & 0x3) << 10)
#define BIT_BATDET_LDO_SEL                      BIT(9)
#define BIT_BATDET_OK                           BIT(8)
#define BIT_VBAT_OK                             BIT(4)
#define BIT_ALL_GPI_DEB                         BIT(3)
#define BIT_GPI_DEBUG_EN                        BIT(2)
#define BIT_ALL_INT_DEB                         BIT(1)
#define BIT_INT_DEBUG_EN                        BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_POR_RST_MONITOR
* Register Offset        :   0x02D8
* Register Description   :
-----------------------------------------------------------*/

#define BITS_POR_RST_MONITOR(x)                 (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_WDG_RST_MONITOR
* Register Offset        :   0x02DC
* Register Description   :
-----------------------------------------------------------*/

#define BITS_WDG_RST_MONITOR(x)                 (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_POR_PIN_RST_MONITOR
* Register Offset        :   0x02E0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_POR_PIN_RST_MONITOR(x)             (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_POR_SRC_FLAG
* Register Offset        :   0x02E4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_POR_SW_FORCE_ON                     BIT(15)
#define BIT_REG_SOFT_RST_FLG_CLR                BIT(14)
#define BITS_POR_SRC_FLAG(x)                    (((x) & 0x3FFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_POR_OFF_FLAG
* Register Offset        :   0x02E8
* Register Description   :
-----------------------------------------------------------*/

#define BIT_POR_CHIP_PD_FLAG                    BIT(13)
#define BIT_POR_CHIP_PD_FLAG_CLR                BIT(12)
#define BIT_UVLO_CHIP_PD_FLAG                   BIT(11)
#define BIT_UVLO_CHIP_PD_FLAG_CLR               BIT(10)
#define BIT_HARD_7S_CHIP_PD_FLAG                BIT(9)
#define BIT_HARD_7S_CHIP_PD_FLAG_CLR            BIT(8)
#define BIT_SW_CHIP_PD_FLAG                     BIT(7)
#define BIT_SW_CHIP_PD_FLAG_CLR                 BIT(6)
#define BIT_HW_CHIP_PD_FLAG                     BIT(5)
#define BIT_HW_CHIP_PD_FLAG_CLR                 BIT(4)
#define BIT_OTP_CHIP_PD_FLAG                    BIT(3)
#define BIT_OTP_CHIP_PD_FLAG_CLR                BIT(2)

/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_POR_7S_CTRL
* Register Offset        :   0x02EC
* Register Description   :
-----------------------------------------------------------*/

#define BIT_PBINT_7S_FLAG_CLR                   BIT(15)
#define BIT_EXT_RSTN_FLAG_CLR                   BIT(14)
#define BIT_CHGR_INT_FLAG_CLR                   BIT(13)
#define BIT_PBINT2_FLAG_CLR                     BIT(12)
#define BIT_PBINT_FLAG_CLR                      BIT(11)
#define BIT_KEY2_7S_RST_EN                      BIT(9)
#define BIT_PBINT_7S_RST_SWMODE                 BIT(8)
#define BITS_PBINT_7S_RST_THRESHOLD(x)          (((x) & 0xF) << 4)
#define BIT_EXT_RSTN_MODE                       BIT(3)
#define BIT_PBINT_7S_AUTO_ON_EN                 BIT(2)
#define BIT_PBINT_7S_RST_DISABLE                BIT(1)
#define BIT_PBINT_7S_RST_MODE                   BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_HWRST_RTC
* Register Offset        :   0x02F0
* Register Description   :
-----------------------------------------------------------*/

#define BITS_HWRST_RTC_REG_STS(x)               (((x) & 0xFF) << 8)
#define BITS_HWRST_RTC_REG_SET(x)               (((x) & 0xFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_ARCH_EN
* Register Offset        :   0x02F4
* Register Description   :
-----------------------------------------------------------*/

#define BIT_ARCH_EN                             BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_MCU_WR_PROT_VALUE
* Register Offset        :   0x02F8
* Register Description   :
-----------------------------------------------------------*/

#define BIT_MCU_WR_PROT                         BIT(15)
#define BITS_MCU_WR_PROT_VALUE(x)               (((x) & 0x7FFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_PWR_WR_PROT_VALUE
* Register Offset        :   0x02FC
* Register Description   :
-----------------------------------------------------------*/

#define BIT_PWR_WR_PROT                         BIT(15)
#define BITS_PWR_WR_PROT_VALUE(x)               (((x) & 0x7FFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SMPL_CTRL0
* Register Offset        :   0x0300
* Register Description   :
-----------------------------------------------------------*/

#define BITS_SMPL_MODE(x)                       (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SMPL_CTRL1
* Register Offset        :   0x0304
* Register Description   :
-----------------------------------------------------------*/

#define BIT_SMPL_PWR_ON_FLAG                    BIT(15)
#define BIT_SMPL_MODE_WR_ACK_FLAG               BIT(14)
#define BIT_SMPL_PWR_ON_FLAG_CLR                BIT(13)
#define BIT_SMPL_MODE_WR_ACK_FLAG_CLR           BIT(12)
#define BIT_SMPL_PWR_ON_SET                     BIT(11)
#define BIT_SMPL_EN                             BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RTC_RST0
* Register Offset        :   0x0308
* Register Description   :
-----------------------------------------------------------*/

#define BITS_RTC_CLK_FLAG_SET(x)                (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RTC_RST1
* Register Offset        :   0x030C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_RTC_CLK_FLAG_CLR(x)                (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RTC_RST2
* Register Offset        :   0x0310
* Register Description   :
-----------------------------------------------------------*/

#define BITS_RTC_CLK_FLAG_RTC(x)                (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_RTC_CLK_STOP
* Register Offset        :   0x0314
* Register Description   :
-----------------------------------------------------------*/

#define BIT_RTC_CLK_STOP_FLAG                   BIT(7)
#define BITS_RTC_CLK_STOP_THRESHOLD(x)          (((x) & 0x7F) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_VBAT_DROP_CNT
* Register Offset        :   0x0318
* Register Description   :
-----------------------------------------------------------*/

#define BITS_VBAT_DROP_CNT(x)                   (((x) & 0xFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SWRST_CTRL0
* Register Offset        :   0x031C
* Register Description   :
-----------------------------------------------------------*/

#define BIT_EXT_RSTN_PD_EN                      BIT(10)
#define BIT_PB_7S_RST_PD_EN                     BIT(9)
#define BIT_REG_RST_PD_EN                       BIT(8)
#define BIT_WDG_RST_PD_EN                       BIT(7)
#define BIT_REG_RST_EN                          BIT(4)
#define BITS_SW_RST_PD_THRESHOLD(x)             (((x) & 0xF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_SWRST_CTRL1
* Register Offset        :   0x0320
* Register Description   :
-----------------------------------------------------------*/

#define BIT_SW_RST_DCDCMEM_PD_EN                BIT(11)
#define BIT_SW_RST_DCDCGEN_PD_EN                BIT(10)
#define BIT_SW_RST_DCDCCORE_PD_EN               BIT(9)
#define BIT_SW_RST_MEM_PD_EN                    BIT(8)
#define BIT_SW_RST_DCXO_PD_EN                   BIT(7)
#define BIT_SW_RST_VDD28_PD_EN                  BIT(6)
#define BIT_SW_RST_AVDD18_PD_EN                 BIT(5)
#define BIT_SW_RST_RF18_PD_EN                   BIT(4)
#define BIT_SW_RST_USB33_PD_EN                  BIT(3)
#define BIT_SW_RST_EMMCCORE_PD_EN               BIT(2)
#define BIT_SW_RST_SDIO_PD_EN                   BIT(1)
#define BIT_SW_RST_SDCORE_PD_EN                 BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_OTP_CTRL
* Register Offset        :   0x0324
* Register Description   :
-----------------------------------------------------------*/

#define BITS_OTP_RESERVED(x)                    (((x) & 0xF) << 3)
#define BITS_OTP_OP(x)                          (((x) & 0x3) << 1)
#define BIT_OTP_EN                              BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_FREE_TIMER_LOW
* Register Offset        :   0x0328
* Register Description   :
-----------------------------------------------------------*/

#define BITS_TIMER_LOW(x)                       (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_FREE_TIMER_HIGH
* Register Offset        :   0x032C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_TIMER_HIGH(x)                      (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LOW_PWR_CLK32K_CTRL
* Register Offset        :   0x0330
* Register Description   :
-----------------------------------------------------------*/

#define BIT_DCXO_EXTRA_LOW_CUR_MODE_EN          BIT(15)
#define BIT_DCXO_LOW_CUR_MODE_EN                BIT(14)
#define BIT_LDO_DCXO_LP_AUTO_EN                 BIT(13)
#define BITS_TIME_BETWEEN_CALIBRATION(x)        (((x) & 0xF) << 9)
#define BITS_TIME_FOR_CALIBRATION(x)            (((x) & 0x7) << 6)
#define BITS_TIME_FOR_DCXO_STABLE(x)            (((x) & 0xF) << 2)
#define BIT_LOW_PWR_CLK32K_EN                   BIT(0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LOW_PWR_CLK32K_TEST0
* Register Offset        :   0x0334
* Register Description   :
-----------------------------------------------------------*/

#define BITS_CAL_RESULT_1(x)                    (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LOW_PWR_CLK32K_TEST1
* Register Offset        :   0x0338
* Register Description   :
-----------------------------------------------------------*/

#define BITS_CAL_RESULT_0(x)                    (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LOW_PWR_CLK32K_TEST2
* Register Offset        :   0x033C
* Register Description   :
-----------------------------------------------------------*/

#define BITS_DIV_FAC_FRAC(x)                    (((x) & 0xFFFF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_LOW_PWR_CLK32K_TEST3
* Register Offset        :   0x0340
* Register Description   :
-----------------------------------------------------------*/

#define BIT_SWITCH_FOR_CLK                      BIT(5)
#define BIT_LOW_PWR_CLK32K_EN_DBG               BIT(4)
#define BITS_DIV_FAC_INT(x)                     (((x) & 0xF) << 0)


/*-----------------------------------------------------------
* Register Name          :   ANA_REG_GLB_VOL_TUNE_CTRL_CORE
* Register Offset        :   0x0344
* Register Description   :
-----------------------------------------------------------*/

#define BITS_CORE_STEP_DELAY(x)                 (((x) & 0x3) << 12)
#define BITS_CORE_STEP_NUM(x)                   (((x) & 0xF) << 8)
#define BITS_CORE_STEP_VOL(x)                   (((x) & 0x1F) << 3)
#define BIT_CORE_VOL_TUNE_START                 BIT(2)
#define BIT_CORE_VOL_TUNE_FLAG                  BIT(1)
#define BIT_CORE_VOL_TUNE_EN                    BIT(0)


/*-----------------------------------------------------------
 * Register Name          :   ANA_REG_GLB_VOL_TUNE_CTRL_CPU
 * Register Offset        :   0x0348
 * Register Description   :
-----------------------------------------------------------*/

#define BITS_CPU_STEP_DELAY(x)                  (((x) & 0x3) << 12)
#define BITS_CPU_STEP_NUM(x)                    (((x) & 0xF) << 8)
#define BITS_CPU_STEP_VOL(x)                    (((x) & 0x1F) << 3)
#define BIT_CPU_VOL_TUNE_START                  BIT(2)
#define BIT_CPU_VOL_TUNE_FLAG                   BIT(1)
#define BIT_CPU_VOL_TUNE_EN                     BIT(0)

//slp_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 slp_ldo_pd_en : 1; // [0]
        REG32 slp_io_en : 1; // [1]
        REG32 ldo_xtl_en : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_RDA2720_GLOBAL_SLP_CTRL_T;

//slp_dcdc_pd_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 slp_dcdcwpa_pd_en : 1; // [2]
        REG32 slp_dcdccore_drop_en : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_RDA2720_GLOBAL_SLP_DCDC_PD_CTRL_T;

//slp_ldo_pd_ctrl0
typedef union {
    REG32 v;
    struct {
        REG32 slp_ldosim1_pd_en : 1; // [0]
        REG32 slp_ldosim2_pd_en : 1; // [1]
        REG32 slp_ldocama_pd_en : 1; // [2]
        REG32 slp_ldocamd_pd_en : 1; // [3]
        REG32 slp_ldocamio_pd_en : 1; // [4]
        REG32 slp_ldocammot_pd_en : 1; // [5]
        REG32 slp_ldousb33_pd_en : 1; // [6]
        REG32 slp_ldosdio_pd_en : 1; // [7]
        REG32 slp_ldosdcore_pd_en : 1; // [8]
        REG32 slp_ldovdd28_pd_en : 1; // [9]
        REG32 slp_ldowifipa_pd_en : 1; // [10]
        REG32 slp_ldodcxo_pd_en : 1; // [11]
        REG32 slp_ldoemmccore_pd_en : 1; // [12]
        REG32 slp_ldorf18b_pd_en : 1; // [13]
        REG32 slp_ldorf18a_pd_en : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_RDA2720_GLOBAL_SLP_LDO_PD_CTRL0_T;

//slp_ldo_pd_ctrl1
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
} REG_RDA2720_GLOBAL_SLP_LDO_PD_CTRL1_T;

//slp_dcdc_lp_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 slp_dcdcwpa_lp_en : 1; // [0]
        REG32 slp_dcdcgen_lp_en : 1; // [1]
        REG32 __3_2 : 2;
        REG32 slp_dcdccore_lp_en : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_RDA2720_GLOBAL_SLP_DCDC_LP_CTRL_T;

//slp_ldo_lp_ctrl0
typedef union {
    REG32 v;
    struct {
        REG32 slp_ldosim1_lp_en : 1; // [0]
        REG32 slp_ldosim2_lp_en : 1; // [1]
        REG32 slp_ldocama_lp_en : 1; // [2]
        REG32 slp_ldocamd_lp_en : 1; // [3]
        REG32 slp_ldocamio_lp_en : 1; // [4]
        REG32 slp_ldocammot_lp_en : 1; // [5]
        REG32 slp_ldousb33_lp_en : 1; // [6]
        REG32 slp_ldosdio_lp_en : 1; // [7]
        REG32 slp_ldosdcore_lp_en : 1; // [8]
        REG32 slp_ldovdd28_lp_en : 1; // [9]
        REG32 slp_ldowifipa_lp_en : 1; // [10]
        REG32 slp_ldodcxo_lp_en : 1; // [11]
        REG32 slp_ldoemmccore_lp_en : 1; // [12]
        REG32 slp_ldorf18b_lp_en : 1; // [13]
        REG32 slp_ldorf18a_lp_en : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_RDA2720_GLOBAL_SLP_LDO_LP_CTRL0_T;

//slp_ldo_lp_ctrl1
typedef union {
    REG32 v;
    struct {
        REG32 slp_ldomem_lp_en : 1; // [0]
        REG32 slp_ldoavdd18_lp_en : 1; // [1]
        REG32 slp_ldosim0_lp_en : 1; // [2]
        REG32 slp_ldocon_lp_en : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_RDA2720_GLOBAL_SLP_LDO_LP_CTRL1_T;

//dcdc_core_slp_ctrl0
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
} REG_RDA2720_GLOBAL_DCDC_CORE_SLP_CTRL0_T;

//dcdc_core_slp_ctrl1
typedef union {
    REG32 v;
    struct {
        REG32 dcdc_core_vosel_ds_sw : 9; // [8:0]
        REG32 __31_9 : 23;
    } b;
} REG_RDA2720_GLOBAL_DCDC_CORE_SLP_CTRL1_T;

#endif /* _RDA2720_GLB_H */
