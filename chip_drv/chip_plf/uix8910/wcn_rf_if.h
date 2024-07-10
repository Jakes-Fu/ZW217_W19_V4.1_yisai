//------------------------------------------------------------------------
// Copyright (C) 2018 RDA microelectronic Inc.                                           
// All Rights Reserved                                                    
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of RDA microelectronic Inc.                
// The copyright notice above does not evidence any actual or intended    
// publication of such source code.                                       
// No part of this code may be reproduced, stored in a retrieval system,  
// or transmitted, in any form or by any means, electronic, mechanical,   
// photocopying, recording, or otherwise, without the prior written       
// permission of RDA microelectronic Inc.                                                 
//------------------------------------------------------------------------
// File name      : wcn_rf_if                                            
// Author         : liqiu                                                 
// Date of created: 2018-08-30 16:12:02                                                  
// Last modified  : 2018-08-30 16:12:02                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2018-08-30 16:12:02 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __WCN_RF_IF_H__
#define __WCN_RF_IF_H__

typedef unsigned int        U32;

#define REG_WCN_RF_IF_BASE      0x15005000
#define hwp_rf_if           ((T_HWP_WCN_RF_IF_T*) (REG_WCN_RF_IF_BASE))

typedef struct
{
    volatile                             U32                       revision; /*0x0  , RO   , 0x00000003, No description.               */
    volatile                             U32                    sys_control; /*0x4  , RW   , 0x00000061, No description.               */
    volatile                             U32                     bt_control; /*0x8  , RW   , 0x00000027, No description.               */
    volatile                             U32                     wf_control; /*0xc  , RW   , 0x00000002, No description.               */
    volatile                             U32                     fm_control; /*0x10 , RW   , 0x00004808, No description.               */
    volatile                             U32             inter_freq_setting; /*0x14 , RW   , 0x008002F6, No description.               */
    volatile                             U32                bandgap_setting; /*0x18 , RW   , 0x00000001, No description.               */
    volatile                             U32                lna_rmx_setting; /*0x1c , RW   , 0x00810058, No description.               */
    volatile                             U32                   pga_setting0; /*0x20 , RW   , 0x00001800, No description.               */
    volatile                             U32                   pga_setting1; /*0x24 , RW   , 0x0006C222, No description.               */
    volatile                             U32                  rxflt_setting; /*0x28 , RW   , 0x03361112, No description.               */
    volatile                             U32                   adc_setting0; /*0x2c , RW   , 0x00000620, No description.               */
    volatile                             U32                   adc_setting1; /*0x30 , RW   , 0x00000000, No description.               */
    volatile                             U32                 bt_dac_setting; /*0x34 , RW   , 0x0001008A, No description.               */
    volatile                             U32                bt_txrf_setting; /*0x38 , RW   , 0x1E777777, No description.               */
    volatile                             U32                 fm_dsp_setting; /*0x3c , RW   , 0x00000000, No description.               */
    volatile                             U32                    vco_setting; /*0x40 , RW   , 0x0196C838, No description.               */
    volatile                             U32                   pll_setting0; /*0x44 , RW   , 0x2221319C, No description.               */
    volatile                             U32                   pll_setting1; /*0x48 , RW   , 0x221DC8AB, No description.               */
    volatile                             U32                   pll_setting2; /*0x4c , RW   , 0x01000022, No description.               */
    volatile                             U32                     pll_status; /*0x50 , RW   , 0x00000000, No description.               */
    volatile                             U32                 adpll_setting0; /*0x54 , RW   , 0x2B110908, No description.               */
    volatile                             U32                 adpll_setting1; /*0x58 , RW   , 0x00000000, No description.               */
    volatile                             U32                   adpll_status; /*0x5c , RW   , 0x00000000, No description.               */
    volatile                             U32                   mdll_setting; /*0x60 , RW   , 0x000061C5, No description.               */
    volatile                             U32               pll_sdm_setting0; /*0x64 , RW   , 0x00144840, No description.               */
    volatile                             U32               pll_sdm_setting1; /*0x68 , RW   , 0x2EF13B14, No description.               */
    volatile                             U32             adpll_sdm_setting0; /*0x6c , RW   , 0x00000906, No description.               */
    volatile                             U32             adpll_sdm_setting1; /*0x70 , RW   , 0x00000000, No description.               */
    volatile                             U32             adpll_sdm_setting2; /*0x74 , RW   , 0x00000000, No description.               */
    volatile                             U32             rxflt_cal_setting0; /*0x78 , RW   , 0x04B90070, No description.               */
    volatile                             U32             rxflt_cal_setting1; /*0x7c , RW   , 0x02EE8080, No description.               */
    volatile                             U32             rxflt_cal_setting2; /*0x80 , RW   , 0x472B2B27, No description.               */
    volatile                             U32             rxflt_cal_setting3; /*0x84 , RW   , 0x03480348, No description.               */
    volatile                             U32            vco_pkd_cal_setting; /*0x88 , RW   , 0x00010860, No description.               */
    volatile                             U32               pll_cal_setting0; /*0x8c , RW   , 0x00900048, No description.               */
    volatile                             U32               pll_cal_setting1; /*0x90 , RW   , 0x00000000, No description.               */
    volatile                             U32                adc_cal_setting; /*0x94 , RW   , 0x00000000, No description.               */
    volatile                             U32                 cal_dr_setting; /*0x98 , RW   , 0x00000000, No description.               */
    volatile                             U32                     cal_status; /*0x9c , RO   , 0x00000000, No description.               */
    volatile                             U32                   cal_results0; /*0xa0 , RO   , 0x3FC08080, No description.               */
    volatile                             U32                   cal_results1; /*0xa4 , RO   , 0x00000200, No description.               */
    volatile                             U32                       power_dr; /*0xa8 , RW   , 0x00000000, No description.               */
    volatile                             U32                      power_reg; /*0xac , RW   , 0x00000000, No description.               */
    volatile                             U32                   power_status; /*0xb0 , RO   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_0; /*0xb4 , RW   , 0x00000000, No description.               */
    volatile                             U32                bt_gain_table_1; /*0xb8 , RW   , 0x00002000, No description.               */
    volatile                             U32                bt_gain_table_2; /*0xbc , RW   , 0x00002010, No description.               */
    volatile                             U32                bt_gain_table_3; /*0xc0 , RW   , 0x00002020, No description.               */
    volatile                             U32                bt_gain_table_4; /*0xc4 , RW   , 0x00002030, No description.               */
    volatile                             U32                bt_gain_table_5; /*0xc8 , RW   , 0x0000A030, No description.               */
    volatile                             U32                bt_gain_table_6; /*0xcc , RW   , 0x0000A040, No description.               */
    volatile                             U32                bt_gain_table_7; /*0xd0 , RW   , 0x0000A041, No description.               */
    volatile                             U32                bt_gain_table_8; /*0xd4 , RW   , 0x0001A041, No description.               */
    volatile                             U32                bt_gain_table_9; /*0xd8 , RW   , 0x0001A043, No description.               */
    volatile                             U32                bt_gain_table_a; /*0xdc , RW   , 0x0001A843, No description.               */
    volatile                             U32                bt_gain_table_b; /*0xe0 , RW   , 0x0001A847, No description.               */
    volatile                             U32                bt_gain_table_c; /*0xe4 , RW   , 0x0001B047, No description.               */
    volatile                             U32                bt_gain_table_d; /*0xe8 , RW   , 0x0001B04B, No description.               */
    volatile                             U32                bt_gain_table_e; /*0xec , RW   , 0x0001B04F, No description.               */
    volatile                             U32                bt_gain_table_f; /*0xf0 , RW   , 0x0001D04F, No description.               */
    volatile                             U32                wf_gain_table_0; /*0xf4 , RW   , 0x00000000, No description.               */
    volatile                             U32                wf_gain_table_1; /*0xf8 , RW   , 0x00002000, No description.               */
    volatile                             U32                wf_gain_table_2; /*0xfc , RW   , 0x00002010, No description.               */
    volatile                             U32                wf_gain_table_3; /*0x100, RW   , 0x00002020, No description.               */
    volatile                             U32                wf_gain_table_4; /*0x104, RW   , 0x00002030, No description.               */
    volatile                             U32                wf_gain_table_5; /*0x108, RW   , 0x0000A030, No description.               */
    volatile                             U32                wf_gain_table_6; /*0x10c, RW   , 0x0000A040, No description.               */
    volatile                             U32                wf_gain_table_7; /*0x110, RW   , 0x0000A041, No description.               */
    volatile                             U32                wf_gain_table_8; /*0x114, RW   , 0x0001A041, No description.               */
    volatile                             U32                wf_gain_table_9; /*0x118, RW   , 0x0001A043, No description.               */
    volatile                             U32                wf_gain_table_a; /*0x11c, RW   , 0x0001A843, No description.               */
    volatile                             U32                wf_gain_table_b; /*0x120, RW   , 0x0001A847, No description.               */
    volatile                             U32                wf_gain_table_c; /*0x124, RW   , 0x0001B047, No description.               */
    volatile                             U32                wf_gain_table_d; /*0x128, RW   , 0x0001B04B, No description.               */
    volatile                             U32                wf_gain_table_e; /*0x12c, RW   , 0x0001B04F, No description.               */
    volatile                             U32                wf_gain_table_f; /*0x130, RW   , 0x0001D04F, No description.               */
    volatile                             U32             bt_tx_gain_table_0; /*0x134, RW   , 0x00000000, No description.               */
    volatile                             U32             bt_tx_gain_table_1; /*0x138, RW   , 0x01150315, No description.               */
    volatile                             U32             bt_tx_gain_table_2; /*0x13c, RW   , 0x06150715, No description.               */
    volatile                             U32             bt_tx_gain_table_3; /*0x140, RW   , 0x0B150F15, No description.               */
    volatile                             U32                fm_gain_table_0; /*0x144, RW   , 0x10201030, No description.               */
    volatile                             U32                fm_gain_table_1; /*0x148, RW   , 0x104010C0, No description.               */
    volatile                             U32                fm_gain_table_2; /*0x14c, RW   , 0x1AC01B40, No description.               */
    volatile                             U32                fm_gain_table_3; /*0x150, RW   , 0x1D401DC0, No description.               */
    volatile                             U32                fm_gain_table_4; /*0x154, RW   , 0x27C027D0, No description.               */
    volatile                             U32                fm_gain_table_5; /*0x158, RW   , 0x27E027F0, No description.               */
    volatile                             U32                fm_gain_table_6; /*0x15c, RW   , 0x2FF42FF8, No description.               */
    volatile                             U32                fm_gain_table_7; /*0x160, RW   , 0x2FFC2FFE, No description.               */
    volatile                             U32            direct_gain_setting; /*0x164, RW   , 0x000F0F0F, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg0; /*0x168, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg1; /*0x16c, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg2; /*0x170, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg3; /*0x174, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg4; /*0x178, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg5; /*0x17c, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg6; /*0x180, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg7; /*0x184, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg8; /*0x188, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_reg9; /*0x18c, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_rega; /*0x190, RW   , 0x80808080, No description.               */
    volatile                             U32          bt_rxflt_cal_bit_regb; /*0x194, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg0; /*0x198, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg1; /*0x19c, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg2; /*0x1a0, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg3; /*0x1a4, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg4; /*0x1a8, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg5; /*0x1ac, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg6; /*0x1b0, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg7; /*0x1b4, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg8; /*0x1b8, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_reg9; /*0x1bc, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_rega; /*0x1c0, RW   , 0x80808080, No description.               */
    volatile                             U32          wf_rxflt_cal_bit_regb; /*0x1c4, RW   , 0x80808080, No description.               */
    volatile                             U32          fm_rxflt_cal_bit_reg0; /*0x1c8, RW   , 0x80808080, No description.               */
    volatile                             U32          fm_rxflt_cal_bit_reg1; /*0x1cc, RW   , 0x80808080, No description.               */
    volatile                             U32          fm_rxflt_cal_bit_reg2; /*0x1d0, RW   , 0x80808080, No description.               */
    volatile                             U32          fm_rxflt_cal_bit_reg3; /*0x1d4, RW   , 0x80808080, No description.               */
    volatile                             U32          fm_rxflt_cal_bit_reg4; /*0x1d8, RW   , 0x80808080, No description.               */
    volatile                             U32          fm_rxflt_cal_bit_reg5; /*0x1dc, RW   , 0x80808080, No description.               */
    volatile                             U32          fm_rxflt_cal_bit_reg6; /*0x1e0, RW   , 0x80808080, No description.               */
    volatile                             U32          fm_rxflt_cal_bit_reg7; /*0x1e4, RW   , 0x80808080, No description.               */
    volatile                             U32                       test_buf; /*0x1e8, RW   , 0x00000000, No description.               */
    volatile                             U32                   ana_reserved; /*0x1ec, RW   , 0x00F0FF00, No description.               */
    volatile                             U32                   dig_reserved; /*0x1f0, RW   , 0x0000FF00, No description.               */
    volatile                             U32                        new_add; /*0x1f4, RW   , 0x00000020, No description.               */
} T_HWP_WCN_RF_IF_T;

#define WCN_RF_IF_REVID(n)             (((n)&0xF)<<0)      
#define WCN_RF_IF_REVID_MASK           (0xF<<0)            
#define WCN_RF_IF_REVID_SHIFT          (0)                 

#define WCN_RF_IF_SOFT_RESETN          (1<<0)              
#define WCN_RF_IF_CHIP_SELF_CAL_ENABLE (1<<1)              
#define WCN_RF_IF_RF_MODE(n)           (((n)&0x3)<<2)      
#define WCN_RF_IF_RF_MODE_BT           (((0)&0x3)<<2)      
#define WCN_RF_IF_RF_MODE_WIFI         (((1)&0x3)<<2)      
#define WCN_RF_IF_RF_MODE_FM           (((2)&0x3)<<2)      
#define WCN_RF_IF_RF_MODE_MASK         (0x3<<2)            
#define WCN_RF_IF_RF_MODE_SHIFT        (2)                 

#define WCN_RF_IF_CLK_DIGITAL_ENABLE_REG (1<<4)              
#define WCN_RF_IF_BT_TUNE_DIFF_EN      (1<<5)              
#define WCN_RF_IF_BT_HOPPING_EN        (1<<6)              
#define WCN_RF_IF_FM_ADC_CLK_MODE      (1<<7)              
#define WCN_RF_IF_FM_ADC_CLK_MODE_DIVIDER (0<<7)              
#define WCN_RF_IF_FM_ADC_CLK_MODE_ADPLL (1<<7)              
#define WCN_RF_IF_BT_CH_CTRL_SRC_SEL   (1<<8)              
#define WCN_RF_IF_BT_CH_CTRL_SRC_SEL_BT (0<<8)              
#define WCN_RF_IF_BT_CH_CTRL_SRC_SEL_REG (1<<8)              
#define WCN_RF_IF_BT_TX_TYPE           (1<<9)              
#define WCN_RF_IF_BT_ARFCN(n)          (((n)&0x7F)<<0)     
#define WCN_RF_IF_BT_ARFCN_MASK        (0x7F<<0)           
#define WCN_RF_IF_BT_ARFCN_SHIFT       (0)                 

#define WCN_RF_IF_BT_CH_TYPE           (1<<7)              
#define WCN_RF_IF_BT_CH_TYPE_NORMAL    (0<<7)              
#define WCN_RF_IF_BT_CH_TYPE_MULTIPLIER (1<<7)              
#define WCN_RF_IF_WF_TUNE              (1<<0)              
#define WCN_RF_IF_WF_CHN(n)            (((n)&0xF)<<1)      
#define WCN_RF_IF_WF_CHN_MASK          (0xF<<1)            
#define WCN_RF_IF_WF_CHN_SHIFT         (1)                 

#define WCN_RF_IF_WF_FREQ_MODE         (1<<5)              
#define WCN_RF_IF_WF_FREQ_MODE_CHANNEL (0<<5)              
#define WCN_RF_IF_WF_FREQ_MODE_DIRECT  (1<<5)              
#define WCN_RF_IF_WF_FREQ_DIRECT(n)    (((n)&0x1FFFF)<<6)  
#define WCN_RF_IF_WF_FREQ_DIRECT_MASK  (0x1FFFF<<6)        
#define WCN_RF_IF_WF_FREQ_DIRECT_SHIFT (6)                 

#define WCN_RF_IF_FM_TUNE              (1<<0)              
#define WCN_RF_IF_FM_CHAN_REG(n)       (((n)&0x7FF)<<1)    
#define WCN_RF_IF_FM_CHAN_REG_MASK     (0x7FF<<1)          
#define WCN_RF_IF_FM_CHAN_REG_SHIFT    (1)                 

#define WCN_RF_IF_FM_FREQ_MODE         (1<<12)             
#define WCN_RF_IF_FM_FREQ_MODE_CHANNEL (0<<12)             
#define WCN_RF_IF_FM_FREQ_MODE_DIRECT  (1<<12)             
#define WCN_RF_IF_FM_BAND_SEL(n)       (((n)&0x3)<<13)     
#define WCN_RF_IF_FM_BAND_SEL_87_108MHZ (((0)&0x3)<<13)     
#define WCN_RF_IF_FM_BAND_SEL_76_91MHZ (((1)&0x3)<<13)     
#define WCN_RF_IF_FM_BAND_SEL_76_108MHZ (((2)&0x3)<<13)     
#define WCN_RF_IF_FM_BAND_SEL_65_76MHZ (((3)&0x3)<<13)     
#define WCN_RF_IF_FM_BAND_SEL_MASK     (0x3<<13)           
#define WCN_RF_IF_FM_BAND_SEL_SHIFT    (13)                

#define WCN_RF_IF_FM_FREQ_DIRECT(n)    (((n)&0xFFFF)<<16)  
#define WCN_RF_IF_FM_FREQ_DIRECT_MASK  (0xFFFF<<16)        
#define WCN_RF_IF_FM_FREQ_DIRECT_SHIFT (16)                

#define WCN_RF_IF_BT_IF(n)             (((n)&0x7FF)<<0)    
#define WCN_RF_IF_BT_IF_MASK           (0x7FF<<0)          
#define WCN_RF_IF_BT_IF_SHIFT          (0)                 

#define WCN_RF_IF_BT_IMGREJ            (1<<11)             
#define WCN_RF_IF_BT_IMGREJ_POSITIVE   (0<<11)             
#define WCN_RF_IF_BT_IMGREJ_NEGTIVE    (1<<11)             
#define WCN_RF_IF_BT_ZIF               (1<<12)             
#define WCN_RF_IF_BT_ZIF_USE_BT_FREQ   (0<<12)             
#define WCN_RF_IF_BT_ZIF_USE_0HZ       (1<<12)             
#define WCN_RF_IF_FM_IF(n)             (((n)&0x7FF)<<16)   
#define WCN_RF_IF_FM_IF_MASK           (0x7FF<<16)         
#define WCN_RF_IF_FM_IF_SHIFT          (16)                

#define WCN_RF_IF_FM_IMGREJ            (1<<27)             
#define WCN_RF_IF_FM_IMGREJ_POSITIVE   (0<<27)             
#define WCN_RF_IF_FM_IMGREJ_NEGTIVE    (1<<27)             
#define WCN_RF_IF_SEL_BG               (1<<0)              
#define WCN_RF_IF_FM_RMX_RSHORT_EN     (1<<0)              
#define WCN_RF_IF_FM_RMX_REG_BIT(n)    (((n)&0x7)<<1)      
#define WCN_RF_IF_FM_RMX_REG_BIT_MASK  (0x7<<1)            
#define WCN_RF_IF_FM_RMX_REG_BIT_SHIFT (1)                 

#define WCN_RF_IF_FM_RMX_LOBIAS_BIT(n) (((n)&0x3)<<4)      
#define WCN_RF_IF_FM_RMX_LOBIAS_BIT_MASK (0x3<<4)            
#define WCN_RF_IF_FM_RMX_LOBIAS_BIT_SHIFT (4)                 

#define WCN_RF_IF_FM_RMX_HARM_REJ_EN   (1<<6)              
#define WCN_RF_IF_FM_LNA_PORT_SEL(n)   (((n)&0x3)<<7)      
#define WCN_RF_IF_FM_LNA_PORT_SEL_MASK (0x3<<7)            
#define WCN_RF_IF_FM_LNA_PORT_SEL_SHIFT (7)                 

#define WCN_RF_IF_FM_LNA_REG_ICTRL     (1<<12)             
#define WCN_RF_IF_BT_RMX_LO_VCOM_BIT(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_RMX_LO_VCOM_BIT_MASK (0x3<<13)           
#define WCN_RF_IF_BT_RMX_LO_VCOM_BIT_SHIFT (13)                

#define WCN_RF_IF_BT_TIA_RIN_BIT(n)    (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_TIA_RIN_BIT_MASK  (0x3<<15)           
#define WCN_RF_IF_BT_TIA_RIN_BIT_SHIFT (15)                

#define WCN_RF_IF_WF_TIA_RIN_BIT(n)    (((n)&0x3)<<17)     
#define WCN_RF_IF_WF_TIA_RIN_BIT_MASK  (0x3<<17)           
#define WCN_RF_IF_WF_TIA_RIN_BIT_SHIFT (17)                

#define WCN_RF_IF_BT_RMX_DISABLE       (1<<19)             
#define WCN_RF_IF_BT_TIA_BYPASS        (1<<20)             
#define WCN_RF_IF_BT_LNA_VCAS_BIT(n)   (((n)&0x3)<<21)     
#define WCN_RF_IF_BT_LNA_VCAS_BIT_MASK (0x3<<21)           
#define WCN_RF_IF_BT_LNA_VCAS_BIT_SHIFT (21)                

#define WCN_RF_IF_BT_LNA_EN            (1<<23)             
#define WCN_RF_IF_BT_LNA_INPUT_SHORT_TX (1<<24)             
#define WCN_RF_IF_BT_LNA_INPUT_SHORT_RX (1<<25)             
#define WCN_RF_IF_PGA_RS_BIT(n)        (((n)&0x1F)<<0)     
#define WCN_RF_IF_PGA_RS_BIT_MASK      (0x1F<<0)           
#define WCN_RF_IF_PGA_RS_BIT_SHIFT     (0)                 

#define WCN_RF_IF_PGA_IQ_SW            (1<<5)              
#define WCN_RF_IF_PGA_CF_BIT(n)        (((n)&0x1F)<<6)     
#define WCN_RF_IF_PGA_CF_BIT_MASK      (0x1F<<6)           
#define WCN_RF_IF_PGA_CF_BIT_SHIFT     (6)                 

#define WCN_RF_IF_PGA_BW_TUN_BIT(n)    (((n)&0x7)<<11)     
#define WCN_RF_IF_PGA_BW_TUN_BIT_MASK  (0x7<<11)           
#define WCN_RF_IF_PGA_BW_TUN_BIT_SHIFT (11)                

#define WCN_RF_IF_PGA_BLK_MODE         (1<<14)             
#define WCN_RF_IF_PGA_FM_EN_REG        (1<<15)             
#define WCN_RF_IF_PGA_FM_EN_DR         (1<<16)             
#define WCN_RF_IF_FM_PGA_IF_MODE(n)    (((n)&0x3)<<0)      
#define WCN_RF_IF_FM_PGA_IF_MODE_MASK  (0x3<<0)            
#define WCN_RF_IF_FM_PGA_IF_MODE_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_IF_MODE(n)    (((n)&0x3)<<2)      
#define WCN_RF_IF_WF_PGA_IF_MODE_MASK  (0x3<<2)            
#define WCN_RF_IF_WF_PGA_IF_MODE_SHIFT (2)                 

#define WCN_RF_IF_BT_PGA_IF_MODE(n)    (((n)&0x3)<<4)      
#define WCN_RF_IF_BT_PGA_IF_MODE_MASK  (0x3<<4)            
#define WCN_RF_IF_BT_PGA_IF_MODE_SHIFT (4)                 

#define WCN_RF_IF_FM_PGA_IBIT(n)       (((n)&0x3)<<6)      
#define WCN_RF_IF_FM_PGA_IBIT_MASK     (0x3<<6)            
#define WCN_RF_IF_FM_PGA_IBIT_SHIFT    (6)                 

#define WCN_RF_IF_WF_PGA_IBIT(n)       (((n)&0x3)<<8)      
#define WCN_RF_IF_WF_PGA_IBIT_MASK     (0x3<<8)            
#define WCN_RF_IF_WF_PGA_IBIT_SHIFT    (8)                 

#define WCN_RF_IF_BT_PGA_IBIT(n)       (((n)&0x3)<<10)     
#define WCN_RF_IF_BT_PGA_IBIT_MASK     (0x3<<10)           
#define WCN_RF_IF_BT_PGA_IBIT_SHIFT    (10)                

#define WCN_RF_IF_FM_PGA_BW_MODE(n)    (((n)&0x3)<<12)     
#define WCN_RF_IF_FM_PGA_BW_MODE_MASK  (0x3<<12)           
#define WCN_RF_IF_FM_PGA_BW_MODE_SHIFT (12)                

#define WCN_RF_IF_WF_PGA_BW_MODE(n)    (((n)&0x3)<<14)     
#define WCN_RF_IF_WF_PGA_BW_MODE_MASK  (0x3<<14)           
#define WCN_RF_IF_WF_PGA_BW_MODE_SHIFT (14)                

#define WCN_RF_IF_BT_PGA_BW_MODE_2M(n) (((n)&0x3)<<16)     
#define WCN_RF_IF_BT_PGA_BW_MODE_2M_MASK (0x3<<16)           
#define WCN_RF_IF_BT_PGA_BW_MODE_2M_SHIFT (16)                

#define WCN_RF_IF_BT_PGA_BW_MODE_1M(n) (((n)&0x3)<<18)     
#define WCN_RF_IF_BT_PGA_BW_MODE_1M_MASK (0x3<<18)           
#define WCN_RF_IF_BT_PGA_BW_MODE_1M_SHIFT (18)                

#define WCN_RF_IF_FM_PGA_GAIN_BIT(n)   (((n)&0x7)<<20)     
#define WCN_RF_IF_FM_PGA_GAIN_BIT_MASK (0x7<<20)           
#define WCN_RF_IF_FM_PGA_GAIN_BIT_SHIFT (20)                

#define WCN_RF_IF_FM_RXFLT_WIFI_HP     (1<<0)              
#define WCN_RF_IF_WF_RXFLT_WIFI_HP     (1<<1)              
#define WCN_RF_IF_BT_RXFLT_WIFI_HP     (1<<2)              
#define WCN_RF_IF_FM_RXFLT_IF_MODE(n)  (((n)&0x3)<<3)      
#define WCN_RF_IF_FM_RXFLT_IF_MODE_MASK (0x3<<3)            
#define WCN_RF_IF_FM_RXFLT_IF_MODE_SHIFT (3)                 

#define WCN_RF_IF_WF_RXFLT_IF_MODE(n)  (((n)&0x3)<<5)      
#define WCN_RF_IF_WF_RXFLT_IF_MODE_MASK (0x3<<5)            
#define WCN_RF_IF_WF_RXFLT_IF_MODE_SHIFT (5)                 

#define WCN_RF_IF_BT_RXFLT_IF_MODE(n)  (((n)&0x3)<<7)      
#define WCN_RF_IF_BT_RXFLT_IF_MODE_MASK (0x3<<7)            
#define WCN_RF_IF_BT_RXFLT_IF_MODE_SHIFT (7)                 

#define WCN_RF_IF_FM_RXFLT_IBIT(n)     (((n)&0x3)<<9)      
#define WCN_RF_IF_FM_RXFLT_IBIT_MASK   (0x3<<9)            
#define WCN_RF_IF_FM_RXFLT_IBIT_SHIFT  (9)                 

#define WCN_RF_IF_WF_RXFLT_IBIT(n)     (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_RXFLT_IBIT_MASK   (0x3<<11)           
#define WCN_RF_IF_WF_RXFLT_IBIT_SHIFT  (11)                

#define WCN_RF_IF_BT_RXFLT_IBIT(n)     (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_RXFLT_IBIT_MASK   (0x3<<13)           
#define WCN_RF_IF_BT_RXFLT_IBIT_SHIFT  (13)                

#define WCN_RF_IF_FM_RXFLT_MODE_SEL(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_FM_RXFLT_MODE_SEL_MASK (0x3<<15)           
#define WCN_RF_IF_FM_RXFLT_MODE_SEL_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_MODE_SEL(n) (((n)&0x3)<<17)     
#define WCN_RF_IF_WF_RXFLT_MODE_SEL_MASK (0x3<<17)           
#define WCN_RF_IF_WF_RXFLT_MODE_SEL_SHIFT (17)                

#define WCN_RF_IF_BT_RXFLT_MODE_SEL_2M(n) (((n)&0x3)<<19)     
#define WCN_RF_IF_BT_RXFLT_MODE_SEL_2M_MASK (0x3<<19)           
#define WCN_RF_IF_BT_RXFLT_MODE_SEL_2M_SHIFT (19)                

#define WCN_RF_IF_BT_RXFLT_MODE_SEL_1M(n) (((n)&0x3)<<21)     
#define WCN_RF_IF_BT_RXFLT_MODE_SEL_1M_MASK (0x3<<21)           
#define WCN_RF_IF_BT_RXFLT_MODE_SEL_1M_SHIFT (21)                

#define WCN_RF_IF_RXFLT_CAP_BIT(n)     (((n)&0xF)<<23)     
#define WCN_RF_IF_RXFLT_CAP_BIT_MASK   (0xF<<23)           
#define WCN_RF_IF_RXFLT_CAP_BIT_SHIFT  (23)                

#define WCN_RF_IF_RXFLT_IQSWAP         (1<<27)             
#define WCN_RF_IF_RXFLT_AUX_EN         (1<<28)             
#define WCN_RF_IF_RXFLT_RSTN_REG       (1<<29)             
#define WCN_RF_IF_RXFLT_RSTN_DR        (1<<30)             
#define WCN_RF_IF_ADC_CLK_SEL_REG(n)   (((n)&0x7)<<0)      
#define WCN_RF_IF_ADC_CLK_SEL_REG_MASK (0x7<<0)            
#define WCN_RF_IF_ADC_CLK_SEL_REG_SHIFT (0)                 

#define WCN_RF_IF_ADC_CLK_SEL_DR       (1<<3)              
#define WCN_RF_IF_ADC_CLK_DIV2         (1<<4)              
#define WCN_RF_IF_ADC_CLK_EDGE         (1<<5)              
#define WCN_RF_IF_ADC_VCM_SEL(n)       (((n)&0x3)<<6)      
#define WCN_RF_IF_ADC_VCM_SEL_MASK     (0x3<<6)            
#define WCN_RF_IF_ADC_VCM_SEL_SHIFT    (6)                 

#define WCN_RF_IF_ADC_DLY_IN_CTRL      (1<<8)              
#define WCN_RF_IF_ADC_DELAY_BIT(n)     (((n)&0xF)<<9)      
#define WCN_RF_IF_ADC_DELAY_BIT_MASK   (0xF<<9)            
#define WCN_RF_IF_ADC_DELAY_BIT_SHIFT  (9)                 

#define WCN_RF_IF_FM_ADC_REG_IBIT(n)   (((n)&0x7)<<0)      
#define WCN_RF_IF_FM_ADC_REG_IBIT_MASK (0x7<<0)            
#define WCN_RF_IF_FM_ADC_REG_IBIT_SHIFT (0)                 

#define WCN_RF_IF_WF_ADC_REG_IBIT(n)   (((n)&0x7)<<3)      
#define WCN_RF_IF_WF_ADC_REG_IBIT_MASK (0x7<<3)            
#define WCN_RF_IF_WF_ADC_REG_IBIT_SHIFT (3)                 

#define WCN_RF_IF_BT_ADC_REG_IBIT_2M(n) (((n)&0x7)<<6)      
#define WCN_RF_IF_BT_ADC_REG_IBIT_2M_MASK (0x7<<6)            
#define WCN_RF_IF_BT_ADC_REG_IBIT_2M_SHIFT (6)                 

#define WCN_RF_IF_BT_ADC_REG_IBIT_1M(n) (((n)&0x7)<<9)      
#define WCN_RF_IF_BT_ADC_REG_IBIT_1M_MASK (0x7<<9)            
#define WCN_RF_IF_BT_ADC_REG_IBIT_1M_SHIFT (9)                 

#define WCN_RF_IF_FM_ADC_REF_IBIT(n)   (((n)&0x7)<<12)     
#define WCN_RF_IF_FM_ADC_REF_IBIT_MASK (0x7<<12)           
#define WCN_RF_IF_FM_ADC_REF_IBIT_SHIFT (12)                

#define WCN_RF_IF_WF_ADC_REF_IBIT(n)   (((n)&0x7)<<15)     
#define WCN_RF_IF_WF_ADC_REF_IBIT_MASK (0x7<<15)           
#define WCN_RF_IF_WF_ADC_REF_IBIT_SHIFT (15)                

#define WCN_RF_IF_BT_ADC_REF_IBIT_2M(n) (((n)&0x7)<<18)     
#define WCN_RF_IF_BT_ADC_REF_IBIT_2M_MASK (0x7<<18)           
#define WCN_RF_IF_BT_ADC_REF_IBIT_2M_SHIFT (18)                

#define WCN_RF_IF_BT_ADC_REF_IBIT_1M(n) (((n)&0x7)<<21)     
#define WCN_RF_IF_BT_ADC_REF_IBIT_1M_MASK (0x7<<21)           
#define WCN_RF_IF_BT_ADC_REF_IBIT_1M_SHIFT (21)                

#define WCN_RF_IF_BT_DAC_RANGE_BIT(n)  (((n)&0x3)<<0)      
#define WCN_RF_IF_BT_DAC_RANGE_BIT_MASK (0x3<<0)            
#define WCN_RF_IF_BT_DAC_RANGE_BIT_SHIFT (0)                 

#define WCN_RF_IF_BT_DAC_MUX_EN        (1<<2)              
#define WCN_RF_IF_BT_DAC_LPWR_MODE     (1<<3)              
#define WCN_RF_IF_BT_DAC_IOUT_ENABLE   (1<<4)              
#define WCN_RF_IF_BT_DAC_CORE_BIT(n)   (((n)&0x7)<<5)      
#define WCN_RF_IF_BT_DAC_CORE_BIT_MASK (0x7<<5)            
#define WCN_RF_IF_BT_DAC_CORE_BIT_SHIFT (5)                 

#define WCN_RF_IF_BT_DAC_CM_BIT(n)     (((n)&0x3)<<8)      
#define WCN_RF_IF_BT_DAC_CM_BIT_MASK   (0x3<<8)            
#define WCN_RF_IF_BT_DAC_CM_BIT_SHIFT  (8)                 

#define WCN_RF_IF_BT_DAC_AUXOUT_EN(n)  (((n)&0x7)<<10)     
#define WCN_RF_IF_BT_DAC_AUXOUT_EN_MASK (0x7<<10)           
#define WCN_RF_IF_BT_DAC_AUXOUT_EN_SHIFT (10)                

#define WCN_RF_IF_BT_DAC_VTR_SEL       (1<<13)             
#define WCN_RF_IF_BT_DAC_VLOW_CTRL_BIT(n) (((n)&0x7)<<14)     
#define WCN_RF_IF_BT_DAC_VLOW_CTRL_BIT_MASK (0x7<<14)           
#define WCN_RF_IF_BT_DAC_VLOW_CTRL_BIT_SHIFT (14)                

#define WCN_RF_IF_BT_DAC_CLK_EDGE      (1<<17)             
#define WCN_RF_IF_BT_DAC_RSTN_REG      (1<<18)             
#define WCN_RF_IF_BT_DAC_RSTN_DR       (1<<19)             
#define WCN_RF_IF_BT_DAC_LOW_MAG       (1<<20)             
#define WCN_RF_IF_BT_DAC_FILTER_IBIAS_BIT(n) (((n)&0x3)<<21)     
#define WCN_RF_IF_BT_DAC_FILTER_IBIAS_BIT_MASK (0x3<<21)           
#define WCN_RF_IF_BT_DAC_FILTER_IBIAS_BIT_SHIFT (21)                

#define WCN_RF_IF_BT_DAC_BW_TUNE_BIT_2M(n) (((n)&0xF)<<23)     
#define WCN_RF_IF_BT_DAC_BW_TUNE_BIT_2M_MASK (0xF<<23)           
#define WCN_RF_IF_BT_DAC_BW_TUNE_BIT_2M_SHIFT (23)                

#define WCN_RF_IF_BT_DAC_BW_TUNE_BIT_1M(n) (((n)&0xF)<<27)     
#define WCN_RF_IF_BT_DAC_BW_TUNE_BIT_1M_MASK (0xF<<27)           
#define WCN_RF_IF_BT_DAC_BW_TUNE_BIT_1M_SHIFT (27)                

#define WCN_RF_IF_BT_TMX_CAL_CLK_EDGE  (1<<31)             
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_TX_HI(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_TX_HI_MASK (0xF<<0)            
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_TX_HI_SHIFT (0)                 

#define WCN_RF_IF_TXRF_CAPTUNE_BIT_TX_MD(n) (((n)&0xF)<<4)      
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_TX_MD_MASK (0xF<<4)            
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_TX_MD_SHIFT (4)                 

#define WCN_RF_IF_TXRF_CAPTUNE_BIT_TX_LO(n) (((n)&0xF)<<8)      
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_TX_LO_MASK (0xF<<8)            
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_TX_LO_SHIFT (8)                 

#define WCN_RF_IF_TXRF_CAPTUNE_BIT_RX_HI(n) (((n)&0xF)<<12)     
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_RX_HI_MASK (0xF<<12)           
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_RX_HI_SHIFT (12)                

#define WCN_RF_IF_TXRF_CAPTUNE_BIT_RX_MD(n) (((n)&0xF)<<16)     
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_RX_MD_MASK (0xF<<16)           
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_RX_MD_SHIFT (16)                

#define WCN_RF_IF_TXRF_CAPTUNE_BIT_RX_LO(n) (((n)&0xF)<<20)     
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_RX_LO_MASK (0xF<<20)           
#define WCN_RF_IF_TXRF_CAPTUNE_BIT_RX_LO_SHIFT (20)                

#define WCN_RF_IF_BT_DELAY_PADRV(n)    (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_DELAY_PADRV_MASK  (0xFF<<24)          
#define WCN_RF_IF_BT_DELAY_PADRV_SHIFT (24)                

#define WCN_RF_IF_FM_DSP_RESETN_REG    (1<<0)              
#define WCN_RF_IF_FM_DSP_RESETN_DR     (1<<1)              
#define WCN_RF_IF_FM_DSP_RESETN_DELAY(n) (((n)&0x3)<<2)      
#define WCN_RF_IF_FM_DSP_RESETN_DELAY_MASK (0x3<<2)            
#define WCN_RF_IF_FM_DSP_RESETN_DELAY_SHIFT (2)                 

#define WCN_RF_IF_FM_VCO_VMODE         (1<<0)              
#define WCN_RF_IF_WF_VCO_VMODE         (1<<1)              
#define WCN_RF_IF_BT_VCO_VMODE         (1<<2)              
#define WCN_RF_IF_FM_VCO_IMODE         (1<<3)              
#define WCN_RF_IF_WF_VCO_IMODE         (1<<4)              
#define WCN_RF_IF_BT_VCO_IMODE         (1<<5)              
#define WCN_RF_IF_VCO_VMODE_BIT(n)     (((n)&0x7)<<6)      
#define WCN_RF_IF_VCO_VMODE_BIT_MASK   (0x7<<6)            
#define WCN_RF_IF_VCO_VMODE_BIT_SHIFT  (6)                 

#define WCN_RF_IF_VCO_VAR_VCOM(n)      (((n)&0x7)<<9)      
#define WCN_RF_IF_VCO_VAR_VCOM_MASK    (0x7<<9)            
#define WCN_RF_IF_VCO_VAR_VCOM_SHIFT   (9)                 

#define WCN_RF_IF_VCO_VAR_SHORT        (1<<12)             
#define WCN_RF_IF_VCO_VAR_REVERSE      (1<<13)             
#define WCN_RF_IF_VCO_VAR_BIT(n)       (((n)&0x7)<<14)     
#define WCN_RF_IF_VCO_VAR_BIT_MASK     (0x7<<14)           
#define WCN_RF_IF_VCO_VAR_BIT_SHIFT    (14)                

#define WCN_RF_IF_VCO_BUF_BIAS_BIT(n)  (((n)&0x3)<<17)     
#define WCN_RF_IF_VCO_BUF_BIAS_BIT_MASK (0x3<<17)           
#define WCN_RF_IF_VCO_BUF_BIAS_BIT_SHIFT (17)                

#define WCN_RF_IF_VCO_DIV2_BIAS_BIT(n) (((n)&0x7)<<19)     
#define WCN_RF_IF_VCO_DIV2_BIAS_BIT_MASK (0x7<<19)           
#define WCN_RF_IF_VCO_DIV2_BIAS_BIT_SHIFT (19)                

#define WCN_RF_IF_VCO_BUF_IBIT(n)      (((n)&0xF)<<22)     
#define WCN_RF_IF_VCO_BUF_IBIT_MASK    (0xF<<22)           
#define WCN_RF_IF_VCO_BUF_IBIT_SHIFT   (22)                

#define WCN_RF_IF_VCO_REGCAP_SELH      (1<<26)             
#define WCN_RF_IF_PLL_DIV_FM_ADC_CLK_REG(n) (((n)&0x1F)<<0)     
#define WCN_RF_IF_PLL_DIV_FM_ADC_CLK_REG_MASK (0x1F<<0)           
#define WCN_RF_IF_PLL_DIV_FM_ADC_CLK_REG_SHIFT (0)                 

#define WCN_RF_IF_PLL_DIV_FM_ADC_CLK_DR (1<<5)              
#define WCN_RF_IF_PLL_DIV_FM_LO_CLK_REG(n) (((n)&0xF)<<6)      
#define WCN_RF_IF_PLL_DIV_FM_LO_CLK_REG_MASK (0xF<<6)            
#define WCN_RF_IF_PLL_DIV_FM_LO_CLK_REG_SHIFT (6)                 

#define WCN_RF_IF_PLL_DIV_FM_LO_CLK_DR (1<<10)             
#define WCN_RF_IF_PLL_BYPASS_NOTCH     (1<<11)             
#define WCN_RF_IF_PLL_XFER_AUX_EN      (1<<12)             
#define WCN_RF_IF_PLL_SINC_MODE(n)     (((n)&0x7)<<13)     
#define WCN_RF_IF_PLL_SINC_MODE_MASK   (0x7<<13)           
#define WCN_RF_IF_PLL_SINC_MODE_SHIFT  (13)                

#define WCN_RF_IF_PLL_FBC_SEL(n)       (((n)&0x7)<<16)     
#define WCN_RF_IF_PLL_FBC_SEL_MASK     (0x7<<16)           
#define WCN_RF_IF_PLL_FBC_SEL_SHIFT    (16)                

#define WCN_RF_IF_PLL_OPEN_EN          (1<<19)             
#define WCN_RF_IF_PLL_REG_PRESC_RC(n)  (((n)&0x3)<<20)     
#define WCN_RF_IF_PLL_REG_PRESC_RC_MASK (0x3<<20)           
#define WCN_RF_IF_PLL_REG_PRESC_RC_SHIFT (20)                

#define WCN_RF_IF_PLL_REG_PRESC(n)     (((n)&0xF)<<22)     
#define WCN_RF_IF_PLL_REG_PRESC_MASK   (0xF<<22)           
#define WCN_RF_IF_PLL_REG_PRESC_SHIFT  (22)                

#define WCN_RF_IF_PLL_REG_PERI_BIT(n)  (((n)&0xF)<<26)     
#define WCN_RF_IF_PLL_REG_PERI_BIT_MASK (0xF<<26)           
#define WCN_RF_IF_PLL_REG_PERI_BIT_SHIFT (26)                

#define WCN_RF_IF_PLL_TEST_EN          (1<<30)             
#define WCN_RF_IF_PLL_REFMULTI2_EN_TX  (1<<0)              
#define WCN_RF_IF_PLL_REFMULTI2_EN_RX  (1<<1)              
#define WCN_RF_IF_PLL_R_BIT_TX(n)      (((n)&0x3)<<2)      
#define WCN_RF_IF_PLL_R_BIT_TX_MASK    (0x3<<2)            
#define WCN_RF_IF_PLL_R_BIT_TX_SHIFT   (2)                 

#define WCN_RF_IF_PLL_R_BIT_RX(n)      (((n)&0x3)<<4)      
#define WCN_RF_IF_PLL_R_BIT_RX_MASK    (0x3<<4)            
#define WCN_RF_IF_PLL_R_BIT_RX_SHIFT   (4)                 

#define WCN_RF_IF_PLL_LPF_GAIN_TX(n)   (((n)&0xF)<<6)      
#define WCN_RF_IF_PLL_LPF_GAIN_TX_MASK (0xF<<6)            
#define WCN_RF_IF_PLL_LPF_GAIN_TX_SHIFT (6)                 

#define WCN_RF_IF_PLL_LPF_GAIN_RX(n)   (((n)&0xF)<<10)     
#define WCN_RF_IF_PLL_LPF_GAIN_RX_MASK (0xF<<10)           
#define WCN_RF_IF_PLL_LPF_GAIN_RX_SHIFT (10)                

#define WCN_RF_IF_PLL_GAIN_BIT_TX(n)   (((n)&0xF)<<14)     
#define WCN_RF_IF_PLL_GAIN_BIT_TX_MASK (0xF<<14)           
#define WCN_RF_IF_PLL_GAIN_BIT_TX_SHIFT (14)                

#define WCN_RF_IF_PLL_GAIN_BIT_RX(n)   (((n)&0xF)<<18)     
#define WCN_RF_IF_PLL_GAIN_BIT_RX_MASK (0xF<<18)           
#define WCN_RF_IF_PLL_GAIN_BIT_RX_SHIFT (18)                

#define WCN_RF_IF_PLL_CP_R_BIT_TX(n)   (((n)&0xF)<<22)     
#define WCN_RF_IF_PLL_CP_R_BIT_TX_MASK (0xF<<22)           
#define WCN_RF_IF_PLL_CP_R_BIT_TX_SHIFT (22)                

#define WCN_RF_IF_PLL_CP_R_BIT_RX(n)   (((n)&0xF)<<26)     
#define WCN_RF_IF_PLL_CP_R_BIT_RX_MASK (0xF<<26)           
#define WCN_RF_IF_PLL_CP_R_BIT_RX_SHIFT (26)                

#define WCN_RF_IF_PLL_LOWPWR_MODE_TX   (1<<30)             
#define WCN_RF_IF_PLL_LOWPWR_MODE_RX   (1<<31)             
#define WCN_RF_IF_MDLL_DIV_TX(n)       (((n)&0xF)<<0)      
#define WCN_RF_IF_MDLL_DIV_TX_MASK     (0xF<<0)            
#define WCN_RF_IF_MDLL_DIV_TX_SHIFT    (0)                 

#define WCN_RF_IF_MDLL_DIV_RX(n)       (((n)&0xF)<<4)      
#define WCN_RF_IF_MDLL_DIV_RX_MASK     (0xF<<4)            
#define WCN_RF_IF_MDLL_DIV_RX_SHIFT    (4)                 

#define WCN_RF_IF_PLL_PHASE_CTRL_DLY_TX(n) (((n)&0x3)<<8)      
#define WCN_RF_IF_PLL_PHASE_CTRL_DLY_TX_MASK (0x3<<8)            
#define WCN_RF_IF_PLL_PHASE_CTRL_DLY_TX_SHIFT (8)                 

#define WCN_RF_IF_PLL_PHASE_CTRL_DLY_RX(n) (((n)&0x3)<<10)     
#define WCN_RF_IF_PLL_PHASE_CTRL_DLY_RX_MASK (0x3<<10)           
#define WCN_RF_IF_PLL_PHASE_CTRL_DLY_RX_SHIFT (10)                

#define WCN_RF_IF_PLL_PFD_RES_BIT_TX(n) (((n)&0x3F)<<12)    
#define WCN_RF_IF_PLL_PFD_RES_BIT_TX_MASK (0x3F<<12)          
#define WCN_RF_IF_PLL_PFD_RES_BIT_TX_SHIFT (12)                

#define WCN_RF_IF_PLL_PFD_RES_BIT_RX(n) (((n)&0x3F)<<18)    
#define WCN_RF_IF_PLL_PFD_RES_BIT_RX_MASK (0x3F<<18)          
#define WCN_RF_IF_PLL_PFD_RES_BIT_RX_SHIFT (18)                

#define WCN_RF_IF_PLL_REF_MODE_TX      (1<<24)             
#define WCN_RF_IF_PLL_REF_MODE_RX      (1<<25)             
#define WCN_RF_IF_PLL_MDLL_REFCLK_EN_REG (1<<26)             
#define WCN_RF_IF_PLL_MDLL_REFCLK_EN_DR (1<<27)             
#define WCN_RF_IF_PLL_CLK_DIG52M_EN_REG (1<<28)             
#define WCN_RF_IF_PLL_CLK_DIG52M_EN_DR (1<<29)             
#define WCN_RF_IF_PLL_BT_ADC_CLK_EN_REG (1<<30)             
#define WCN_RF_IF_PLL_BT_ADC_CLK_EN_DR (1<<31)             
#define WCN_RF_IF_PLL_LOCK_DET         (1<<0)              
#define WCN_RF_IF_PLL_LOCK_FLAG        (1<<1)              
#define WCN_RF_IF_PLL_LOCK_DET_TIMER_DELAY_SEL(n) (((n)&0x3)<<2)      
#define WCN_RF_IF_PLL_LOCK_DET_TIMER_DELAY_SEL_MASK (0x3<<2)            
#define WCN_RF_IF_PLL_LOCK_DET_TIMER_DELAY_SEL_SHIFT (2)                 

#define WCN_RF_IF_PLL_LOCK_FLAG_TIMER_DELAY_SEL(n) (((n)&0x3)<<4)      
#define WCN_RF_IF_PLL_LOCK_FLAG_TIMER_DELAY_SEL_MASK (0x3<<4)            
#define WCN_RF_IF_PLL_LOCK_FLAG_TIMER_DELAY_SEL_SHIFT (4)                 

#define WCN_RF_IF_ADPLL_SDM_CLK_TEST_EN (1<<0)              
#define WCN_RF_IF_ADPLL_VCO_LOW_TEST   (1<<1)              
#define WCN_RF_IF_ADPLL_VCO_HIGH_TEST  (1<<2)              
#define WCN_RF_IF_ADPLL_REFMULTI2_EN   (1<<3)              
#define WCN_RF_IF_ADPLL_PCON_MODE      (1<<4)              
#define WCN_RF_IF_ADPLL_VREG_BIT(n)    (((n)&0xF)<<5)      
#define WCN_RF_IF_ADPLL_VREG_BIT_MASK  (0xF<<5)            
#define WCN_RF_IF_ADPLL_VREG_BIT_SHIFT (5)                 

#define WCN_RF_IF_ADPLL_TEST_EN        (1<<9)              
#define WCN_RF_IF_ADPLL_RES_BIT(n)     (((n)&0x3)<<10)     
#define WCN_RF_IF_ADPLL_RES_BIT_MASK   (0x3<<10)           
#define WCN_RF_IF_ADPLL_RES_BIT_SHIFT  (10)                

#define WCN_RF_IF_ADPLL_REG_RES_BIT(n) (((n)&0x3)<<12)     
#define WCN_RF_IF_ADPLL_REG_RES_BIT_MASK (0x3<<12)           
#define WCN_RF_IF_ADPLL_REG_RES_BIT_SHIFT (12)                

#define WCN_RF_IF_ADPLL_CP_IBIT(n)     (((n)&0x7)<<14)     
#define WCN_RF_IF_ADPLL_CP_IBIT_MASK   (0x7<<14)           
#define WCN_RF_IF_ADPLL_CP_IBIT_SHIFT  (14)                

#define WCN_RF_IF_ADPLL_RSTN_REG       (1<<17)             
#define WCN_RF_IF_ADPLL_RSTN_DR        (1<<18)             
#define WCN_RF_IF_ADPLL_CLK_EN_TIMER_DELAY (1<<19)             
#define WCN_RF_IF_ADPLL_RSTN_TIMER_DELAY(n) (((n)&0x7F)<<20)    
#define WCN_RF_IF_ADPLL_RSTN_TIMER_DELAY_MASK (0x7F<<20)          
#define WCN_RF_IF_ADPLL_RSTN_TIMER_DELAY_SHIFT (20)                

#define WCN_RF_IF_ADPLL_SDM_CLK_SEL_TIMER_DELAY(n) (((n)&0x7)<<27)     
#define WCN_RF_IF_ADPLL_SDM_CLK_SEL_TIMER_DELAY_MASK (0x7<<27)           
#define WCN_RF_IF_ADPLL_SDM_CLK_SEL_TIMER_DELAY_SHIFT (27)                

#define WCN_RF_IF_ADPLL_CLK2WF_DIG_SEL_REG (1<<0)              
#define WCN_RF_IF_ADPLL_CLK2WF_DIG_SEL_DR (1<<1)              
#define WCN_RF_IF_ADPLL_CLK2WF_DIG_EN_REG (1<<2)              
#define WCN_RF_IF_ADPLL_CLK2WF_DIG_EN_DR (1<<3)              
#define WCN_RF_IF_ADPLL_CLK2FMWF_ADC_SEL_REG (1<<4)              
#define WCN_RF_IF_ADPLL_CLK2FMWF_ADC_SEL_DR (1<<5)              
#define WCN_RF_IF_ADPLL_CLK2FMWF_ADC_EN_REG (1<<6)              
#define WCN_RF_IF_ADPLL_CLK2FMWF_ADC_EN_DR (1<<7)              
#define WCN_RF_IF_ADPLL_CLK2BT_ADC_SEL_REG (1<<8)              
#define WCN_RF_IF_ADPLL_CLK2BT_ADC_SEL_DR (1<<9)              
#define WCN_RF_IF_ADPLL_CLK2BT_ADC_EN_REG (1<<10)             
#define WCN_RF_IF_ADPLL_CLK2BT_ADC_EN_DR (1<<11)             
#define WCN_RF_IF_ADPLL_CLK2BT_DIG_SEL (1<<12)             
#define WCN_RF_IF_ADPLL_CLK2BT_DIG_EN  (1<<13)             
#define WCN_RF_IF_ADPLL_LOCK_DET       (1<<0)              
#define WCN_RF_IF_ADPLL_LOCK_FLAG      (1<<1)              
#define WCN_RF_IF_ADPLL_LOCK_DET_TIMER_DELAY_SEL(n) (((n)&0x3)<<2)      
#define WCN_RF_IF_ADPLL_LOCK_DET_TIMER_DELAY_SEL_MASK (0x3<<2)            
#define WCN_RF_IF_ADPLL_LOCK_DET_TIMER_DELAY_SEL_SHIFT (2)                 

#define WCN_RF_IF_ADPLL_LOCK_FLAG_TIMER_DELAY_SEL(n) (((n)&0x3)<<4)      
#define WCN_RF_IF_ADPLL_LOCK_FLAG_TIMER_DELAY_SEL_MASK (0x3<<4)            
#define WCN_RF_IF_ADPLL_LOCK_FLAG_TIMER_DELAY_SEL_SHIFT (4)                 

#define WCN_RF_IF_MDLL_DIV_BIT(n)      (((n)&0x1F)<<0)     
#define WCN_RF_IF_MDLL_DIV_BIT_MASK    (0x1F<<0)           
#define WCN_RF_IF_MDLL_DIV_BIT_SHIFT   (0)                 

#define WCN_RF_IF_MDLL_BAND_SEL        (1<<5)              
#define WCN_RF_IF_MDLL_BAND_BIT(n)     (((n)&0x7)<<6)      
#define WCN_RF_IF_MDLL_BAND_BIT_MASK   (0x7<<6)            
#define WCN_RF_IF_MDLL_BAND_BIT_SHIFT  (6)                 

#define WCN_RF_IF_MDLL_CP_IBIT(n)      (((n)&0x7)<<9)      
#define WCN_RF_IF_MDLL_CP_IBIT_MASK    (0x7<<9)            
#define WCN_RF_IF_MDLL_CP_IBIT_SHIFT   (9)                 

#define WCN_RF_IF_MDLL_STARTUP_BIT(n)  (((n)&0x7)<<12)     
#define WCN_RF_IF_MDLL_STARTUP_BIT_MASK (0x7<<12)           
#define WCN_RF_IF_MDLL_STARTUP_BIT_SHIFT (12)                

#define WCN_RF_IF_DISABLE_REFCLK_PLL   (1<<15)             
#define WCN_RF_IF_MDLL_RSTN_REG        (1<<16)             
#define WCN_RF_IF_MDLL_RSTN_DR         (1<<17)             
#define WCN_RF_IF_PLL_SDM_DELAY_SEL(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_PLL_SDM_DELAY_SEL_MASK (0xF<<0)            
#define WCN_RF_IF_PLL_SDM_DELAY_SEL_SHIFT (0)                 

#define WCN_RF_IF_PLL_SDM_DITHER_BYPASS (1<<4)              
#define WCN_RF_IF_PLL_SDM_INT_DEC_SEL(n) (((n)&0x3)<<5)      
#define WCN_RF_IF_PLL_SDM_INT_DEC_SEL_MASK (0x3<<5)            
#define WCN_RF_IF_PLL_SDM_INT_DEC_SEL_SHIFT (5)                 

#define WCN_RF_IF_PLL_SDM_CLK_EDGE     (1<<7)              
#define WCN_RF_IF_PLL_SDM_RESETN_REG   (1<<8)              
#define WCN_RF_IF_PLL_SDM_RESETN_DR    (1<<9)              
#define WCN_RF_IF_RESET_PLL_SDM_TIMER_DELAY(n) (((n)&0xF)<<10)     
#define WCN_RF_IF_RESET_PLL_SDM_TIMER_DELAY_MASK (0xF<<10)           
#define WCN_RF_IF_RESET_PLL_SDM_TIMER_DELAY_SHIFT (10)                

#define WCN_RF_IF_PLL_SDM_CLK_SEL_1    (1<<14)             
#define WCN_RF_IF_PLL_SDM_CLK_SEL_0    (1<<15)             
#define WCN_RF_IF_PLL_DIV_DR           (1<<16)             
#define WCN_RF_IF_PLL_FREQ_FORMER_BYPASS (1<<17)             
#define WCN_RF_IF_PLL_FREQ_FORMER_SHIFT_CT(n) (((n)&0x7)<<18)     
#define WCN_RF_IF_PLL_FREQ_FORMER_SHIFT_CT_MASK (0x7<<18)           
#define WCN_RF_IF_PLL_FREQ_FORMER_SHIFT_CT_SHIFT (18)                

#define WCN_RF_IF_PLL_DIV_REG(n)       (((n)&0x7FFFFFFF)<<0)
#define WCN_RF_IF_PLL_DIV_REG_MASK     (0x7FFFFFFF<<0)     
#define WCN_RF_IF_PLL_DIV_REG_SHIFT    (0)                 

#define WCN_RF_IF_ADPLL_SDM_DITHER_BYPASS (1<<0)              
#define WCN_RF_IF_ADPLL_SDM_INT_DEC_SEL(n) (((n)&0x7)<<1)      
#define WCN_RF_IF_ADPLL_SDM_INT_DEC_SEL_MASK (0x7<<1)            
#define WCN_RF_IF_ADPLL_SDM_INT_DEC_SEL_SHIFT (1)                 

#define WCN_RF_IF_ADPLL_SDM_CLK_FBC_INV (1<<4)              
#define WCN_RF_IF_ADPLL_SDM_RESETN_REG (1<<5)              
#define WCN_RF_IF_ADPLL_SDM_RESETN_DR  (1<<6)              
#define WCN_RF_IF_RESET_ADPLL_SDM_TIMER_DELAY(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_RESET_ADPLL_SDM_TIMER_DELAY_MASK (0xF<<7)            
#define WCN_RF_IF_RESET_ADPLL_SDM_TIMER_DELAY_SHIFT (7)                 

#define WCN_RF_IF_ADPLL_SDM_CLK_SEL_1  (1<<11)             
#define WCN_RF_IF_ADPLL_SDM_CLK_SEL_0  (1<<12)             
#define WCN_RF_IF_ADPLL_SDM_FREQ_DR    (1<<13)             
#define WCN_RF_IF_ADPLL_SDM_FREQ_REG(n) (((n)&0xFFFFFFFF)<<0)
#define WCN_RF_IF_ADPLL_SDM_FREQ_REG_MASK (0xFFFFFFFF<<0)     
#define WCN_RF_IF_ADPLL_SDM_FREQ_REG_SHIFT (0)                 

#define WCN_RF_IF_ADPLL_SDM_SS_DEVI_STEP(n) (((n)&0xFFFF)<<0)   
#define WCN_RF_IF_ADPLL_SDM_SS_DEVI_STEP_MASK (0xFFFF<<0)         
#define WCN_RF_IF_ADPLL_SDM_SS_DEVI_STEP_SHIFT (0)                 

#define WCN_RF_IF_ADPLL_SDM_SS_DEVI(n) (((n)&0xFFF)<<16)   
#define WCN_RF_IF_ADPLL_SDM_SS_DEVI_MASK (0xFFF<<16)         
#define WCN_RF_IF_ADPLL_SDM_SS_DEVI_SHIFT (16)                

#define WCN_RF_IF_ADPLL_SDM_SS_EN      (1<<28)             
#define WCN_RF_IF_RXFLT_CAL_INIT_DELAY(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_RXFLT_CAL_INIT_DELAY_MASK (0xF<<0)            
#define WCN_RF_IF_RXFLT_CAL_INIT_DELAY_SHIFT (0)                 

#define WCN_RF_IF_RXFLT_CAL_CNT(n)     (((n)&0xF)<<4)      
#define WCN_RF_IF_RXFLT_CAL_CNT_MASK   (0xF<<4)            
#define WCN_RF_IF_RXFLT_CAL_CNT_SHIFT  (4)                 

#define WCN_RF_IF_RXFLT_CAL_IQSWAP     (1<<8)              
#define WCN_RF_IF_RXFLT_CAL_POLARITY   (1<<9)              
#define WCN_RF_IF_RXFLT_CAL_MODE_SEL(n) (((n)&0x3)<<10)     
#define WCN_RF_IF_RXFLT_CAL_MODE_SEL_MASK (0x3<<10)           
#define WCN_RF_IF_RXFLT_CAL_MODE_SEL_SHIFT (10)                

#define WCN_RF_IF_RXFLT_CAL_CLK_EDGE_SEL (1<<12)             
#define WCN_RF_IF_RXFLT_CAL_CLK_EDGE   (1<<13)             
#define WCN_RF_IF_RXFLT_CAL_CYCLES(n)  (((n)&0x3)<<14)     
#define WCN_RF_IF_RXFLT_CAL_CYCLES_MASK (0x3<<14)           
#define WCN_RF_IF_RXFLT_CAL_CYCLES_SHIFT (14)                

#define WCN_RF_IF_WF_RXFLT_CAL_LOOP_COEF(n) (((n)&0x7FFF)<<16)  
#define WCN_RF_IF_WF_RXFLT_CAL_LOOP_COEF_MASK (0x7FFF<<16)        
#define WCN_RF_IF_WF_RXFLT_CAL_LOOP_COEF_SHIFT (16)                

#define WCN_RF_IF_RXFLT_CAL_Q_BIT_REG(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_RXFLT_CAL_Q_BIT_REG_MASK (0xFF<<0)           
#define WCN_RF_IF_RXFLT_CAL_Q_BIT_REG_SHIFT (0)                 

#define WCN_RF_IF_RXFLT_CAL_I_BIT_REG(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_RXFLT_CAL_I_BIT_REG_MASK (0xFF<<8)           
#define WCN_RF_IF_RXFLT_CAL_I_BIT_REG_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_LOOP_ADC_RNG(n) (((n)&0x3FF)<<16)   
#define WCN_RF_IF_WF_RXFLT_CAL_LOOP_ADC_RNG_MASK (0x3FF<<16)         
#define WCN_RF_IF_WF_RXFLT_CAL_LOOP_ADC_RNG_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_LOOP_POL (1<<26)             
#define WCN_RF_IF_WF_RXFLT_CAL_LOOP_EN (1<<27)             
#define WCN_RF_IF_FM_RXFLT_CAL_RESOLV_ENHANCE (1<<28)             
#define WCN_RF_IF_WF_RXFLT_CAL_RESOLV_ENHANCE (1<<29)             
#define WCN_RF_IF_BT_RXFLT_CAL_RESOLV_ENHANCE (1<<30)             
#define WCN_RF_IF_RXFLT_CAL_BIT_DR     (1<<31)             
#define WCN_RF_IF_BT_CHN_MD(n)         (((n)&0x7F)<<0)     
#define WCN_RF_IF_BT_CHN_MD_MASK       (0x7F<<0)           
#define WCN_RF_IF_BT_CHN_MD_SHIFT      (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_INTER_EN (1<<7)              
#define WCN_RF_IF_WF_RXFLT_CAL_INTER_COEF_HI(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_INTER_COEF_HI_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_INTER_COEF_HI_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_INTER_COEF_LO(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_INTER_COEF_LO_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_INTER_COEF_LO_SHIFT (16)                

#define WCN_RF_IF_WF_CHN_MD(n)         (((n)&0xF)<<24)     
#define WCN_RF_IF_WF_CHN_MD_MASK       (0xF<<24)           
#define WCN_RF_IF_WF_CHN_MD_SHIFT      (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_INTER_EN (1<<28)             
#define WCN_RF_IF_RXFLT_CAL_RANGE_BIT(n) (((n)&0x3)<<29)     
#define WCN_RF_IF_RXFLT_CAL_RANGE_BIT_MASK (0x3<<29)           
#define WCN_RF_IF_RXFLT_CAL_RANGE_BIT_SHIFT (29)                

#define WCN_RF_IF_BT_RXFLT_CAL_INTER_COEF_HI(n) (((n)&0x7FFF)<<0)   
#define WCN_RF_IF_BT_RXFLT_CAL_INTER_COEF_HI_MASK (0x7FFF<<0)         
#define WCN_RF_IF_BT_RXFLT_CAL_INTER_COEF_HI_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_INTER_COEF_LO(n) (((n)&0x7FFF)<<16)  
#define WCN_RF_IF_BT_RXFLT_CAL_INTER_COEF_LO_MASK (0x7FFF<<16)        
#define WCN_RF_IF_BT_RXFLT_CAL_INTER_COEF_LO_SHIFT (16)                

#define WCN_RF_IF_VCO_PKD_CAL_POLARITY (1<<0)              
#define WCN_RF_IF_VCO_PKD_CAL_INIT_DELAY(n) (((n)&0xF)<<1)      
#define WCN_RF_IF_VCO_PKD_CAL_INIT_DELAY_MASK (0xF<<1)            
#define WCN_RF_IF_VCO_PKD_CAL_INIT_DELAY_SHIFT (1)                 

#define WCN_RF_IF_VCO_PKD_REF_BIT(n)   (((n)&0x7)<<5)      
#define WCN_RF_IF_VCO_PKD_REF_BIT_MASK (0x7<<5)            
#define WCN_RF_IF_VCO_PKD_REF_BIT_SHIFT (5)                 

#define WCN_RF_IF_VCO_VBIT_REG(n)      (((n)&0xF)<<8)      
#define WCN_RF_IF_VCO_VBIT_REG_MASK    (0xF<<8)            
#define WCN_RF_IF_VCO_VBIT_REG_SHIFT   (8)                 

#define WCN_RF_IF_VCO_VBIT_DR          (1<<12)             
#define WCN_RF_IF_VCO_IBIT_REG(n)      (((n)&0xF)<<13)     
#define WCN_RF_IF_VCO_IBIT_REG_MASK    (0xF<<13)           
#define WCN_RF_IF_VCO_IBIT_REG_SHIFT   (13)                

#define WCN_RF_IF_VCO_IBIT_DR          (1<<17)             
#define WCN_RF_IF_PLL_INIT_DELAY(n)    (((n)&0x7)<<0)      
#define WCN_RF_IF_PLL_INIT_DELAY_MASK  (0x7<<0)            
#define WCN_RF_IF_PLL_INIT_DELAY_SHIFT (0)                 

#define WCN_RF_IF_PLL_CAL_CNT_SEL(n)   (((n)&0x7)<<3)      
#define WCN_RF_IF_PLL_CAL_CNT_SEL_EACH_CNT_TIME_0 (((0)&0x7)<<3)      
#define WCN_RF_IF_PLL_CAL_CNT_SEL_EACH_CNT_TIME_1 (((1)&0x7)<<3)      
#define WCN_RF_IF_PLL_CAL_CNT_SEL_EACH_CNT_TIME_2 (((2)&0x7)<<3)      
#define WCN_RF_IF_PLL_CAL_CNT_SEL_EACH_CNT_TIME_3 (((3)&0x7)<<3)      
#define WCN_RF_IF_PLL_CAL_CNT_SEL_EACH_CNT_TIME_4 (((4)&0x7)<<3)      
#define WCN_RF_IF_PLL_CAL_CNT_SEL_MASK (0x7<<3)            
#define WCN_RF_IF_PLL_CAL_CNT_SEL_SHIFT (3)                 

#define WCN_RF_IF_PLL_CAL_OPT          (1<<6)              
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME(n) (((n)&0x7)<<7)      
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME_0 (((0)&0x7)<<7)      
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME_1 (((1)&0x7)<<7)      
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME_2 (((2)&0x7)<<7)      
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME_3 (((3)&0x7)<<7)      
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME_4 (((4)&0x7)<<7)      
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME_5 (((5)&0x7)<<7)      
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME_6 (((6)&0x7)<<7)      
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_VCO_BIT_HOLD_TIME_7 (((7)&0x7)<<7)      
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_MASK (0x7<<7)            
#define WCN_RF_IF_PLL_VCO_BIT_HOLD_TIME_SHIFT (7)                 

#define WCN_RF_IF_PLL_CAL_FREQ_DR      (1<<10)             
#define WCN_RF_IF_VCO_BAND_REG(n)      (((n)&0x3FF)<<11)   
#define WCN_RF_IF_VCO_BAND_REG_MASK    (0x3FF<<11)         
#define WCN_RF_IF_VCO_BAND_REG_SHIFT   (11)                

#define WCN_RF_IF_VCO_BAND_DR          (1<<21)             
#define WCN_RF_IF_PLL_CAL_BIT(n)       (((n)&0x3)<<22)     
#define WCN_RF_IF_PLL_CAL_BIT_MASK     (0x3<<22)           
#define WCN_RF_IF_PLL_CAL_BIT_SHIFT    (22)                

#define WCN_RF_IF_PLL_CAL_CLK_SEL      (1<<24)             
#define WCN_RF_IF_PLL_CAL_FREQ_REG(n)  (((n)&0xFFFF)<<0)   
#define WCN_RF_IF_PLL_CAL_FREQ_REG_MASK (0xFFFF<<0)         
#define WCN_RF_IF_PLL_CAL_FREQ_REG_SHIFT (0)                 

#define WCN_RF_IF_ADC_VREG_VBIT_REG(n) (((n)&0x7)<<0)      
#define WCN_RF_IF_ADC_VREG_VBIT_REG_MASK (0x7<<0)            
#define WCN_RF_IF_ADC_VREG_VBIT_REG_SHIFT (0)                 

#define WCN_RF_IF_ADC_VREG_VBIT_DR     (1<<3)              
#define WCN_RF_IF_ADC_REG_CAL_POLARITY (1<<4)              
#define WCN_RF_IF_ADC_REG_CAL_INIT_DELAY(n) (((n)&0xF)<<5)      
#define WCN_RF_IF_ADC_REG_CAL_INIT_DELAY_MASK (0xF<<5)            
#define WCN_RF_IF_ADC_REG_CAL_INIT_DELAY_SHIFT (5)                 

#define WCN_RF_IF_ADC_VREF_VBIT_REG(n) (((n)&0x7)<<9)      
#define WCN_RF_IF_ADC_VREF_VBIT_REG_MASK (0x7<<9)            
#define WCN_RF_IF_ADC_VREF_VBIT_REG_SHIFT (9)                 

#define WCN_RF_IF_ADC_VREF_VBIT_DR     (1<<12)             
#define WCN_RF_IF_ADC_REF_CAL_POLARITY (1<<13)             
#define WCN_RF_IF_ADC_REF_CAL_INIT_DELAY(n) (((n)&0xF)<<14)     
#define WCN_RF_IF_ADC_REF_CAL_INIT_DELAY_MASK (0xF<<14)           
#define WCN_RF_IF_ADC_REF_CAL_INIT_DELAY_SHIFT (14)                

#define WCN_RF_IF_ADC_CAL_RSTN_REG     (1<<18)             
#define WCN_RF_IF_ADC_CAL_RSTN_DR      (1<<19)             
#define WCN_RF_IF_PLL_CAL_EN_REG       (1<<0)              
#define WCN_RF_IF_PLL_CAL_EN_DR        (1<<1)              
#define WCN_RF_IF_RXFLT_CAL_EN_REG     (1<<2)              
#define WCN_RF_IF_RXFLT_CAL_EN_DR      (1<<3)              
#define WCN_RF_IF_ADC_REG_CAL_EN_REG   (1<<4)              
#define WCN_RF_IF_ADC_REG_CAL_EN_DR    (1<<5)              
#define WCN_RF_IF_ADC_REF_CAL_EN_REG   (1<<6)              
#define WCN_RF_IF_ADC_REF_CAL_EN_DR    (1<<7)              
#define WCN_RF_IF_VCO_PKD_CAL_EN_REG   (1<<8)              
#define WCN_RF_IF_VCO_PKD_CAL_EN_DR    (1<<9)              
#define WCN_RF_IF_BT_SELF_CAL_READY    (1<<0)              
#define WCN_RF_IF_WF_SELF_CAL_READY    (1<<1)              
#define WCN_RF_IF_FM_SELF_CAL_READY    (1<<2)              
#define WCN_RF_IF_PLL_CAL_READY        (1<<3)              
#define WCN_RF_IF_RXFLT_CAL_READY      (1<<4)              
#define WCN_RF_IF_ADC_REG_CAL_READY    (1<<5)              
#define WCN_RF_IF_ADC_REF_CAL_READY    (1<<6)              
#define WCN_RF_IF_VCO_PKD_CAL_READY    (1<<7)              
#define WCN_RF_IF_RXFLT_CAL_Q_BIT(n)   (((n)&0xFF)<<0)     
#define WCN_RF_IF_RXFLT_CAL_Q_BIT_MASK (0xFF<<0)           
#define WCN_RF_IF_RXFLT_CAL_Q_BIT_SHIFT (0)                 

#define WCN_RF_IF_RXFLT_CAL_I_BIT(n)   (((n)&0xFF)<<8)     
#define WCN_RF_IF_RXFLT_CAL_I_BIT_MASK (0xFF<<8)           
#define WCN_RF_IF_RXFLT_CAL_I_BIT_SHIFT (8)                 

#define WCN_RF_IF_ADC_VREG_VBIT(n)     (((n)&0x7)<<16)     
#define WCN_RF_IF_ADC_VREG_VBIT_MASK   (0x7<<16)           
#define WCN_RF_IF_ADC_VREG_VBIT_SHIFT  (16)                

#define WCN_RF_IF_ADC_VREF_VBIT(n)     (((n)&0x7)<<19)     
#define WCN_RF_IF_ADC_VREF_VBIT_MASK   (0x7<<19)           
#define WCN_RF_IF_ADC_VREF_VBIT_SHIFT  (19)                

#define WCN_RF_IF_VCO_VBIT(n)          (((n)&0xF)<<22)     
#define WCN_RF_IF_VCO_VBIT_MASK        (0xF<<22)           
#define WCN_RF_IF_VCO_VBIT_SHIFT       (22)                

#define WCN_RF_IF_VCO_IBIT(n)          (((n)&0xF)<<26)     
#define WCN_RF_IF_VCO_IBIT_MASK        (0xF<<26)           
#define WCN_RF_IF_VCO_IBIT_SHIFT       (26)                

#define WCN_RF_IF_VCO_BAND(n)          (((n)&0x3FF)<<0)    
#define WCN_RF_IF_VCO_BAND_MASK        (0x3FF<<0)          
#define WCN_RF_IF_VCO_BAND_SHIFT       (0)                 

#define WCN_RF_IF_RXFLT_CAL_OUT_Q      (1<<10)             
#define WCN_RF_IF_RXFLT_CAL_OUT_I      (1<<11)             
#define WCN_RF_IF_ADC_CAL_REF_OUT      (1<<12)             
#define WCN_RF_IF_ADC_CAL_REG_OUT      (1<<13)             
#define WCN_RF_IF_VCO_PKD_CAL_OUT      (1<<14)             
#define WCN_RF_IF_PU_MDLL_DR           (1<<0)              
#define WCN_RF_IF_PU_ADPLL_DR          (1<<1)              
#define WCN_RF_IF_PU_PLL_FM_ADC_CLK_DR (1<<2)              
#define WCN_RF_IF_PU_PLL_FM_LO_CLK_DR  (1<<3)              
#define WCN_RF_IF_PU_PLL_PRESC_DR      (1<<4)              
#define WCN_RF_IF_PU_PLL_PERI_DR       (1<<5)              
#define WCN_RF_IF_PU_VCO_PKD_DR        (1<<6)              
#define WCN_RF_IF_PU_VCO_RXLO_DR       (1<<7)              
#define WCN_RF_IF_PU_VCO_TXLO_DR       (1<<8)              
#define WCN_RF_IF_PU_VCO_DR            (1<<9)              
#define WCN_RF_IF_PU_BT_TXRF_DR        (1<<10)             
#define WCN_RF_IF_PU_BT_TMX_DR         (1<<11)             
#define WCN_RF_IF_PU_BT_PADRV_DR       (1<<12)             
#define WCN_RF_IF_PU_BT_DAC_DR         (1<<13)             
#define WCN_RF_IF_PU_ADC_DR            (1<<14)             
#define WCN_RF_IF_PU_RXFLT_DR          (1<<15)             
#define WCN_RF_IF_PU_PGA_DR            (1<<16)             
#define WCN_RF_IF_PU_BT_TIA_DR         (1<<17)             
#define WCN_RF_IF_PU_BT_LNA_DR         (1<<18)             
#define WCN_RF_IF_PU_FM_RMX_DR         (1<<19)             
#define WCN_RF_IF_PU_FM_LNA_DR         (1<<20)             
#define WCN_RF_IF_PU_BG_DR             (1<<21)             
#define WCN_RF_IF_PU_MDLL_REG          (1<<0)              
#define WCN_RF_IF_PU_ADPLL_REG         (1<<1)              
#define WCN_RF_IF_PU_PLL_FM_ADC_CLK_REG (1<<2)              
#define WCN_RF_IF_PU_PLL_FM_LO_CLK_REG (1<<3)              
#define WCN_RF_IF_PU_PLL_PRESC_REG     (1<<4)              
#define WCN_RF_IF_PU_PLL_PERI_REG      (1<<5)              
#define WCN_RF_IF_PU_VCO_PKD_REG       (1<<6)              
#define WCN_RF_IF_PU_VCO_RXLO_REG      (1<<7)              
#define WCN_RF_IF_PU_VCO_TXLO_REG      (1<<8)              
#define WCN_RF_IF_PU_VCO_REG           (1<<9)              
#define WCN_RF_IF_PU_BT_TXRF_REG       (1<<10)             
#define WCN_RF_IF_PU_BT_TMX_REG        (1<<11)             
#define WCN_RF_IF_PU_BT_PADRV_REG      (1<<12)             
#define WCN_RF_IF_PU_BT_DAC_REG        (1<<13)             
#define WCN_RF_IF_PU_ADC_REG           (1<<14)             
#define WCN_RF_IF_PU_RXFLT_REG         (1<<15)             
#define WCN_RF_IF_PU_PGA_REG           (1<<16)             
#define WCN_RF_IF_PU_BT_TIA_REG        (1<<17)             
#define WCN_RF_IF_PU_BT_LNA_REG        (1<<18)             
#define WCN_RF_IF_PU_FM_RMX_REG        (1<<19)             
#define WCN_RF_IF_PU_FM_LNA_REG        (1<<20)             
#define WCN_RF_IF_PU_BG_REG            (1<<21)             
#define WCN_RF_IF_PU_MDLL              (1<<0)              
#define WCN_RF_IF_PU_ADPLL             (1<<1)              
#define WCN_RF_IF_PU_PLL_FM_ADC_CLK    (1<<2)              
#define WCN_RF_IF_PU_PLL_FM_LO_CLK     (1<<3)              
#define WCN_RF_IF_PU_PLL_PRESC         (1<<4)              
#define WCN_RF_IF_PU_PLL_PERI          (1<<5)              
#define WCN_RF_IF_PU_VCO_PKD           (1<<6)              
#define WCN_RF_IF_PU_VCO_RXLO          (1<<7)              
#define WCN_RF_IF_PU_VCO_TXLO          (1<<8)              
#define WCN_RF_IF_PU_VCO               (1<<9)              
#define WCN_RF_IF_PU_BT_TXRF           (1<<10)             
#define WCN_RF_IF_PU_BT_TMX            (1<<11)             
#define WCN_RF_IF_PU_BT_PADRV          (1<<12)             
#define WCN_RF_IF_PU_BT_DAC            (1<<13)             
#define WCN_RF_IF_PU_ADC               (1<<14)             
#define WCN_RF_IF_PU_RXFLT             (1<<15)             
#define WCN_RF_IF_PU_PGA               (1<<16)             
#define WCN_RF_IF_PU_BT_TIA            (1<<17)             
#define WCN_RF_IF_PU_BT_LNA            (1<<18)             
#define WCN_RF_IF_PU_FM_RMX            (1<<19)             
#define WCN_RF_IF_PU_FM_LNA            (1<<20)             
#define WCN_RF_IF_PU_BG                (1<<21)             
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_0(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_0_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_0_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_0(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_0_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_0_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_0(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_0_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_0_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_0(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_0_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_0_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_0(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_0_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_0_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_0(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_0_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_0_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_1(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_1_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_1_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_1(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_1_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_1_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_1(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_1_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_1_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_1(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_1_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_1_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_1(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_1_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_1_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_1(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_1_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_1_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_2(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_2_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_2_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_2(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_2_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_2_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_2(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_2_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_2_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_2(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_2_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_2_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_2(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_2_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_2_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_2(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_2_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_2_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_3(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_3_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_3_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_3(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_3_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_3_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_3(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_3_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_3_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_3(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_3_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_3_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_3(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_3_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_3_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_3(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_3_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_3_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_4(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_4_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_4_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_4(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_4_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_4_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_4(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_4_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_4_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_4(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_4_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_4_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_4(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_4_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_4_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_4(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_4_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_4_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_5(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_5_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_5_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_5(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_5_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_5_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_5(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_5_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_5_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_5(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_5_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_5_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_5(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_5_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_5_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_5(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_5_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_5_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_6(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_6_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_6_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_6(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_6_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_6_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_6(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_6_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_6_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_6(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_6_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_6_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_6(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_6_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_6_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_6(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_6_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_6_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_7(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_7_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_7_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_7(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_7_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_7_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_7(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_7_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_7_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_7(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_7_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_7_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_7(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_7_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_7_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_7(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_7_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_7_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_8(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_8_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_8_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_8(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_8_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_8_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_8(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_8_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_8_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_8(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_8_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_8_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_8(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_8_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_8_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_8(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_8_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_8_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_9(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_9_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_9_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_9(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_9_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_9_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_9(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_9_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_9_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_9(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_9_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_9_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_9(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_9_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_9_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_9(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_9_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_9_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_A(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_A_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_A_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_A(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_A_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_A_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_A(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_A_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_A_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_A(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_A_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_A_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_A(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_A_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_A_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_A(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_A_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_A_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_B(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_B_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_B_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_B(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_B_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_B_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_B(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_B_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_B_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_B(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_B_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_B_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_B(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_B_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_B_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_B(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_B_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_B_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_C(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_C_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_C_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_C(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_C_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_C_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_C(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_C_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_C_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_C(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_C_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_C_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_C(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_C_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_C_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_C(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_C_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_C_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_D(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_D_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_D_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_D(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_D_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_D_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_D(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_D_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_D_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_D(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_D_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_D_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_D(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_D_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_D_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_D(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_D_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_D_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_E(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_E_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_E_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_E(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_E_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_E_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_E(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_E_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_E_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_E(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_E_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_E_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_E(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_E_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_E_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_E(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_E_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_E_SHIFT (15)                

#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_F(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_F_MASK (0xF<<0)            
#define WCN_RF_IF_BT_RXFLT_BT_GAIN_BIT_F_SHIFT (0)                 

#define WCN_RF_IF_BT_PGA_GAIN_BIT_F(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_BT_PGA_GAIN_BIT_F_MASK (0x7<<4)            
#define WCN_RF_IF_BT_PGA_GAIN_BIT_F_SHIFT (4)                 

#define WCN_RF_IF_BT_LNA_RESF_BIT_F(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_BT_LNA_RESF_BIT_F_MASK (0xF<<7)            
#define WCN_RF_IF_BT_LNA_RESF_BIT_F_SHIFT (7)                 

#define WCN_RF_IF_BT_LNA_GAIN3_BIT_F(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_F_MASK (0x3<<11)           
#define WCN_RF_IF_BT_LNA_GAIN3_BIT_F_SHIFT (11)                

#define WCN_RF_IF_BT_LNA_GAIN2_BIT_F(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_F_MASK (0x3<<13)           
#define WCN_RF_IF_BT_LNA_GAIN2_BIT_F_SHIFT (13)                

#define WCN_RF_IF_BT_LNA_GAIN1_BIT_F(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_F_MASK (0x3<<15)           
#define WCN_RF_IF_BT_LNA_GAIN1_BIT_F_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_0(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_0_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_0_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_0(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_0_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_0_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_0(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_0_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_0_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_0(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_0_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_0_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_0(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_0_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_0_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_0(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_0_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_0_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_1(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_1_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_1_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_1(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_1_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_1_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_1(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_1_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_1_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_1(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_1_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_1_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_1(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_1_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_1_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_1(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_1_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_1_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_2(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_2_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_2_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_2(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_2_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_2_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_2(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_2_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_2_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_2(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_2_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_2_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_2(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_2_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_2_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_2(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_2_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_2_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_3(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_3_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_3_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_3(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_3_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_3_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_3(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_3_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_3_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_3(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_3_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_3_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_3(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_3_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_3_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_3(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_3_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_3_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_4(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_4_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_4_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_4(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_4_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_4_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_4(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_4_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_4_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_4(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_4_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_4_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_4(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_4_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_4_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_4(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_4_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_4_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_5(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_5_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_5_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_5(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_5_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_5_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_5(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_5_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_5_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_5(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_5_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_5_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_5(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_5_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_5_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_5(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_5_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_5_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_6(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_6_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_6_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_6(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_6_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_6_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_6(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_6_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_6_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_6(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_6_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_6_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_6(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_6_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_6_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_6(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_6_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_6_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_7(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_7_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_7_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_7(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_7_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_7_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_7(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_7_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_7_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_7(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_7_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_7_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_7(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_7_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_7_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_7(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_7_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_7_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_8(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_8_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_8_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_8(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_8_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_8_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_8(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_8_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_8_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_8(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_8_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_8_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_8(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_8_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_8_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_8(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_8_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_8_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_9(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_9_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_9_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_9(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_9_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_9_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_9(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_9_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_9_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_9(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_9_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_9_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_9(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_9_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_9_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_9(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_9_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_9_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_A(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_A_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_A_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_A(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_A_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_A_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_A(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_A_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_A_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_A(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_A_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_A_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_A(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_A_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_A_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_A(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_A_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_A_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_B(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_B_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_B_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_B(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_B_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_B_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_B(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_B_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_B_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_B(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_B_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_B_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_B(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_B_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_B_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_B(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_B_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_B_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_C(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_C_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_C_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_C(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_C_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_C_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_C(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_C_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_C_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_C(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_C_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_C_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_C(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_C_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_C_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_C(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_C_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_C_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_D(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_D_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_D_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_D(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_D_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_D_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_D(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_D_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_D_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_D(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_D_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_D_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_D(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_D_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_D_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_D(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_D_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_D_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_E(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_E_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_E_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_E(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_E_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_E_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_E(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_E_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_E_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_E(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_E_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_E_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_E(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_E_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_E_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_E(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_E_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_E_SHIFT (15)                

#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_F(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_F_MASK (0xF<<0)            
#define WCN_RF_IF_WF_RXFLT_BT_GAIN_BIT_F_SHIFT (0)                 

#define WCN_RF_IF_WF_PGA_GAIN_BIT_F(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_WF_PGA_GAIN_BIT_F_MASK (0x7<<4)            
#define WCN_RF_IF_WF_PGA_GAIN_BIT_F_SHIFT (4)                 

#define WCN_RF_IF_WF_LNA_RESF_BIT_F(n) (((n)&0xF)<<7)      
#define WCN_RF_IF_WF_LNA_RESF_BIT_F_MASK (0xF<<7)            
#define WCN_RF_IF_WF_LNA_RESF_BIT_F_SHIFT (7)                 

#define WCN_RF_IF_WF_LNA_GAIN3_BIT_F(n) (((n)&0x3)<<11)     
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_F_MASK (0x3<<11)           
#define WCN_RF_IF_WF_LNA_GAIN3_BIT_F_SHIFT (11)                

#define WCN_RF_IF_WF_LNA_GAIN2_BIT_F(n) (((n)&0x3)<<13)     
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_F_MASK (0x3<<13)           
#define WCN_RF_IF_WF_LNA_GAIN2_BIT_F_SHIFT (13)                

#define WCN_RF_IF_WF_LNA_GAIN1_BIT_F(n) (((n)&0x3)<<15)     
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_F_MASK (0x3<<15)           
#define WCN_RF_IF_WF_LNA_GAIN1_BIT_F_SHIFT (15)                

#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_1(n) (((n)&0x3)<<0)      
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_1_MASK (0x3<<0)            
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_1_SHIFT (0)                 

#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_1(n) (((n)&0x3)<<2)      
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_1_MASK (0x3<<2)            
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_1_SHIFT (2)                 

#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_1(n) (((n)&0x3)<<4)      
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_1_MASK (0x3<<4)            
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_1_SHIFT (4)                 

#define WCN_RF_IF_TXRF_ATT_EN_1(n)     (((n)&0x3)<<6)      
#define WCN_RF_IF_TXRF_ATT_EN_1_MASK   (0x3<<6)            
#define WCN_RF_IF_TXRF_ATT_EN_1_SHIFT  (6)                 

#define WCN_RF_IF_TXRF_GAIN_BIT_1(n)   (((n)&0xF)<<8)      
#define WCN_RF_IF_TXRF_GAIN_BIT_1_MASK (0xF<<8)            
#define WCN_RF_IF_TXRF_GAIN_BIT_1_SHIFT (8)                 

#define WCN_RF_IF_TXRF_PAD_MODE_1      (1<<12)             
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_0(n) (((n)&0x3)<<16)     
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_0_MASK (0x3<<16)           
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_0_SHIFT (16)                

#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_0(n) (((n)&0x3)<<18)     
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_0_MASK (0x3<<18)           
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_0_SHIFT (18)                

#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_0(n) (((n)&0x3)<<20)     
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_0_MASK (0x3<<20)           
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_0_SHIFT (20)                

#define WCN_RF_IF_TXRF_ATT_EN_0(n)     (((n)&0x3)<<22)     
#define WCN_RF_IF_TXRF_ATT_EN_0_MASK   (0x3<<22)           
#define WCN_RF_IF_TXRF_ATT_EN_0_SHIFT  (22)                

#define WCN_RF_IF_TXRF_GAIN_BIT_0(n)   (((n)&0xF)<<24)     
#define WCN_RF_IF_TXRF_GAIN_BIT_0_MASK (0xF<<24)           
#define WCN_RF_IF_TXRF_GAIN_BIT_0_SHIFT (24)                

#define WCN_RF_IF_TXRF_PAD_MODE_0      (1<<28)             
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_3(n) (((n)&0x3)<<0)      
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_3_MASK (0x3<<0)            
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_3_SHIFT (0)                 

#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_3(n) (((n)&0x3)<<2)      
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_3_MASK (0x3<<2)            
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_3_SHIFT (2)                 

#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_3(n) (((n)&0x3)<<4)      
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_3_MASK (0x3<<4)            
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_3_SHIFT (4)                 

#define WCN_RF_IF_TXRF_ATT_EN_3(n)     (((n)&0x3)<<6)      
#define WCN_RF_IF_TXRF_ATT_EN_3_MASK   (0x3<<6)            
#define WCN_RF_IF_TXRF_ATT_EN_3_SHIFT  (6)                 

#define WCN_RF_IF_TXRF_GAIN_BIT_3(n)   (((n)&0xF)<<8)      
#define WCN_RF_IF_TXRF_GAIN_BIT_3_MASK (0xF<<8)            
#define WCN_RF_IF_TXRF_GAIN_BIT_3_SHIFT (8)                 

#define WCN_RF_IF_TXRF_PAD_MODE_3      (1<<12)             
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_2(n) (((n)&0x3)<<16)     
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_2_MASK (0x3<<16)           
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_2_SHIFT (16)                

#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_2(n) (((n)&0x3)<<18)     
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_2_MASK (0x3<<18)           
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_2_SHIFT (18)                

#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_2(n) (((n)&0x3)<<20)     
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_2_MASK (0x3<<20)           
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_2_SHIFT (20)                

#define WCN_RF_IF_TXRF_ATT_EN_2(n)     (((n)&0x3)<<22)     
#define WCN_RF_IF_TXRF_ATT_EN_2_MASK   (0x3<<22)           
#define WCN_RF_IF_TXRF_ATT_EN_2_SHIFT  (22)                

#define WCN_RF_IF_TXRF_GAIN_BIT_2(n)   (((n)&0xF)<<24)     
#define WCN_RF_IF_TXRF_GAIN_BIT_2_MASK (0xF<<24)           
#define WCN_RF_IF_TXRF_GAIN_BIT_2_SHIFT (24)                

#define WCN_RF_IF_TXRF_PAD_MODE_2      (1<<28)             
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_5(n) (((n)&0x3)<<0)      
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_5_MASK (0x3<<0)            
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_5_SHIFT (0)                 

#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_5(n) (((n)&0x3)<<2)      
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_5_MASK (0x3<<2)            
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_5_SHIFT (2)                 

#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_5(n) (((n)&0x3)<<4)      
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_5_MASK (0x3<<4)            
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_5_SHIFT (4)                 

#define WCN_RF_IF_TXRF_ATT_EN_5(n)     (((n)&0x3)<<6)      
#define WCN_RF_IF_TXRF_ATT_EN_5_MASK   (0x3<<6)            
#define WCN_RF_IF_TXRF_ATT_EN_5_SHIFT  (6)                 

#define WCN_RF_IF_TXRF_GAIN_BIT_5(n)   (((n)&0xF)<<8)      
#define WCN_RF_IF_TXRF_GAIN_BIT_5_MASK (0xF<<8)            
#define WCN_RF_IF_TXRF_GAIN_BIT_5_SHIFT (8)                 

#define WCN_RF_IF_TXRF_PAD_MODE_5      (1<<12)             
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_4(n) (((n)&0x3)<<16)     
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_4_MASK (0x3<<16)           
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_4_SHIFT (16)                

#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_4(n) (((n)&0x3)<<18)     
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_4_MASK (0x3<<18)           
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_4_SHIFT (18)                

#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_4(n) (((n)&0x3)<<20)     
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_4_MASK (0x3<<20)           
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_4_SHIFT (20)                

#define WCN_RF_IF_TXRF_ATT_EN_4(n)     (((n)&0x3)<<22)     
#define WCN_RF_IF_TXRF_ATT_EN_4_MASK   (0x3<<22)           
#define WCN_RF_IF_TXRF_ATT_EN_4_SHIFT  (22)                

#define WCN_RF_IF_TXRF_GAIN_BIT_4(n)   (((n)&0xF)<<24)     
#define WCN_RF_IF_TXRF_GAIN_BIT_4_MASK (0xF<<24)           
#define WCN_RF_IF_TXRF_GAIN_BIT_4_SHIFT (24)                

#define WCN_RF_IF_TXRF_PAD_MODE_4      (1<<28)             
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_7(n) (((n)&0x3)<<0)      
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_7_MASK (0x3<<0)            
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_7_SHIFT (0)                 

#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_7(n) (((n)&0x3)<<2)      
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_7_MASK (0x3<<2)            
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_7_SHIFT (2)                 

#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_7(n) (((n)&0x3)<<4)      
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_7_MASK (0x3<<4)            
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_7_SHIFT (4)                 

#define WCN_RF_IF_TXRF_ATT_EN_7(n)     (((n)&0x3)<<6)      
#define WCN_RF_IF_TXRF_ATT_EN_7_MASK   (0x3<<6)            
#define WCN_RF_IF_TXRF_ATT_EN_7_SHIFT  (6)                 

#define WCN_RF_IF_TXRF_GAIN_BIT_7(n)   (((n)&0xF)<<8)      
#define WCN_RF_IF_TXRF_GAIN_BIT_7_MASK (0xF<<8)            
#define WCN_RF_IF_TXRF_GAIN_BIT_7_SHIFT (8)                 

#define WCN_RF_IF_TXRF_PAD_MODE_7      (1<<12)             
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_6(n) (((n)&0x3)<<16)     
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_6_MASK (0x3<<16)           
#define WCN_RF_IF_TXRF_PAD_CAS_VBIT_6_SHIFT (16)                

#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_6(n) (((n)&0x3)<<18)     
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_6_MASK (0x3<<18)           
#define WCN_RF_IF_TXRF_PAD_AUX_VBIT_6_SHIFT (18)                

#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_6(n) (((n)&0x3)<<20)     
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_6_MASK (0x3<<20)           
#define WCN_RF_IF_TXRF_PAD_BIAS_IBIT_6_SHIFT (20)                

#define WCN_RF_IF_TXRF_ATT_EN_6(n)     (((n)&0x3)<<22)     
#define WCN_RF_IF_TXRF_ATT_EN_6_MASK   (0x3<<22)           
#define WCN_RF_IF_TXRF_ATT_EN_6_SHIFT  (22)                

#define WCN_RF_IF_TXRF_GAIN_BIT_6(n)   (((n)&0xF)<<24)     
#define WCN_RF_IF_TXRF_GAIN_BIT_6_MASK (0xF<<24)           
#define WCN_RF_IF_TXRF_GAIN_BIT_6_SHIFT (24)                

#define WCN_RF_IF_TXRF_PAD_MODE_6      (1<<28)             
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_1(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_1_MASK (0xF<<0)            
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_1_SHIFT (0)                 

#define WCN_RF_IF_FM_RMX_GAIN_BIT_1(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_FM_RMX_GAIN_BIT_1_MASK (0x7<<4)            
#define WCN_RF_IF_FM_RMX_GAIN_BIT_1_SHIFT (4)                 

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_1(n) (((n)&0x3)<<7)      
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_1_MASK (0x3<<7)            
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_1_SHIFT (7)                 

#define WCN_RF_IF_FM_LNA_GAIN_BIT_1(n) (((n)&0x3)<<9)      
#define WCN_RF_IF_FM_LNA_GAIN_BIT_1_MASK (0x3<<9)            
#define WCN_RF_IF_FM_LNA_GAIN_BIT_1_SHIFT (9)                 

#define WCN_RF_IF_FM_LNA_REG_IBIT_1(n) (((n)&0x7)<<11)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_1_MASK (0x7<<11)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_1_SHIFT (11)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_0(n) (((n)&0xF)<<16)     
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_0_MASK (0xF<<16)           
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_0_SHIFT (16)                

#define WCN_RF_IF_FM_RMX_GAIN_BIT_0(n) (((n)&0x7)<<20)     
#define WCN_RF_IF_FM_RMX_GAIN_BIT_0_MASK (0x7<<20)           
#define WCN_RF_IF_FM_RMX_GAIN_BIT_0_SHIFT (20)                

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_0(n) (((n)&0x3)<<23)     
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_0_MASK (0x3<<23)           
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_0_SHIFT (23)                

#define WCN_RF_IF_FM_LNA_GAIN_BIT_0(n) (((n)&0x3)<<25)     
#define WCN_RF_IF_FM_LNA_GAIN_BIT_0_MASK (0x3<<25)           
#define WCN_RF_IF_FM_LNA_GAIN_BIT_0_SHIFT (25)                

#define WCN_RF_IF_FM_LNA_REG_IBIT_0(n) (((n)&0x7)<<27)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_0_MASK (0x7<<27)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_0_SHIFT (27)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_3(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_3_MASK (0xF<<0)            
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_3_SHIFT (0)                 

#define WCN_RF_IF_FM_RMX_GAIN_BIT_3(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_FM_RMX_GAIN_BIT_3_MASK (0x7<<4)            
#define WCN_RF_IF_FM_RMX_GAIN_BIT_3_SHIFT (4)                 

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_3(n) (((n)&0x3)<<7)      
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_3_MASK (0x3<<7)            
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_3_SHIFT (7)                 

#define WCN_RF_IF_FM_LNA_GAIN_BIT_3(n) (((n)&0x3)<<9)      
#define WCN_RF_IF_FM_LNA_GAIN_BIT_3_MASK (0x3<<9)            
#define WCN_RF_IF_FM_LNA_GAIN_BIT_3_SHIFT (9)                 

#define WCN_RF_IF_FM_LNA_REG_IBIT_3(n) (((n)&0x7)<<11)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_3_MASK (0x7<<11)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_3_SHIFT (11)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_2(n) (((n)&0xF)<<16)     
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_2_MASK (0xF<<16)           
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_2_SHIFT (16)                

#define WCN_RF_IF_FM_RMX_GAIN_BIT_2(n) (((n)&0x7)<<20)     
#define WCN_RF_IF_FM_RMX_GAIN_BIT_2_MASK (0x7<<20)           
#define WCN_RF_IF_FM_RMX_GAIN_BIT_2_SHIFT (20)                

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_2(n) (((n)&0x3)<<23)     
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_2_MASK (0x3<<23)           
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_2_SHIFT (23)                

#define WCN_RF_IF_FM_LNA_GAIN_BIT_2(n) (((n)&0x3)<<25)     
#define WCN_RF_IF_FM_LNA_GAIN_BIT_2_MASK (0x3<<25)           
#define WCN_RF_IF_FM_LNA_GAIN_BIT_2_SHIFT (25)                

#define WCN_RF_IF_FM_LNA_REG_IBIT_2(n) (((n)&0x7)<<27)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_2_MASK (0x7<<27)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_2_SHIFT (27)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_5(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_5_MASK (0xF<<0)            
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_5_SHIFT (0)                 

#define WCN_RF_IF_FM_RMX_GAIN_BIT_5(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_FM_RMX_GAIN_BIT_5_MASK (0x7<<4)            
#define WCN_RF_IF_FM_RMX_GAIN_BIT_5_SHIFT (4)                 

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_5(n) (((n)&0x3)<<7)      
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_5_MASK (0x3<<7)            
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_5_SHIFT (7)                 

#define WCN_RF_IF_FM_LNA_GAIN_BIT_5(n) (((n)&0x3)<<9)      
#define WCN_RF_IF_FM_LNA_GAIN_BIT_5_MASK (0x3<<9)            
#define WCN_RF_IF_FM_LNA_GAIN_BIT_5_SHIFT (9)                 

#define WCN_RF_IF_FM_LNA_REG_IBIT_5(n) (((n)&0x7)<<11)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_5_MASK (0x7<<11)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_5_SHIFT (11)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_4(n) (((n)&0xF)<<16)     
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_4_MASK (0xF<<16)           
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_4_SHIFT (16)                

#define WCN_RF_IF_FM_RMX_GAIN_BIT_4(n) (((n)&0x7)<<20)     
#define WCN_RF_IF_FM_RMX_GAIN_BIT_4_MASK (0x7<<20)           
#define WCN_RF_IF_FM_RMX_GAIN_BIT_4_SHIFT (20)                

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_4(n) (((n)&0x3)<<23)     
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_4_MASK (0x3<<23)           
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_4_SHIFT (23)                

#define WCN_RF_IF_FM_LNA_GAIN_BIT_4(n) (((n)&0x3)<<25)     
#define WCN_RF_IF_FM_LNA_GAIN_BIT_4_MASK (0x3<<25)           
#define WCN_RF_IF_FM_LNA_GAIN_BIT_4_SHIFT (25)                

#define WCN_RF_IF_FM_LNA_REG_IBIT_4(n) (((n)&0x7)<<27)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_4_MASK (0x7<<27)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_4_SHIFT (27)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_7(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_7_MASK (0xF<<0)            
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_7_SHIFT (0)                 

#define WCN_RF_IF_FM_RMX_GAIN_BIT_7(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_FM_RMX_GAIN_BIT_7_MASK (0x7<<4)            
#define WCN_RF_IF_FM_RMX_GAIN_BIT_7_SHIFT (4)                 

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_7(n) (((n)&0x3)<<7)      
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_7_MASK (0x3<<7)            
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_7_SHIFT (7)                 

#define WCN_RF_IF_FM_LNA_GAIN_BIT_7(n) (((n)&0x3)<<9)      
#define WCN_RF_IF_FM_LNA_GAIN_BIT_7_MASK (0x3<<9)            
#define WCN_RF_IF_FM_LNA_GAIN_BIT_7_SHIFT (9)                 

#define WCN_RF_IF_FM_LNA_REG_IBIT_7(n) (((n)&0x7)<<11)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_7_MASK (0x7<<11)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_7_SHIFT (11)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_6(n) (((n)&0xF)<<16)     
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_6_MASK (0xF<<16)           
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_6_SHIFT (16)                

#define WCN_RF_IF_FM_RMX_GAIN_BIT_6(n) (((n)&0x7)<<20)     
#define WCN_RF_IF_FM_RMX_GAIN_BIT_6_MASK (0x7<<20)           
#define WCN_RF_IF_FM_RMX_GAIN_BIT_6_SHIFT (20)                

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_6(n) (((n)&0x3)<<23)     
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_6_MASK (0x3<<23)           
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_6_SHIFT (23)                

#define WCN_RF_IF_FM_LNA_GAIN_BIT_6(n) (((n)&0x3)<<25)     
#define WCN_RF_IF_FM_LNA_GAIN_BIT_6_MASK (0x3<<25)           
#define WCN_RF_IF_FM_LNA_GAIN_BIT_6_SHIFT (25)                

#define WCN_RF_IF_FM_LNA_REG_IBIT_6(n) (((n)&0x7)<<27)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_6_MASK (0x7<<27)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_6_SHIFT (27)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_9(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_9_MASK (0xF<<0)            
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_9_SHIFT (0)                 

#define WCN_RF_IF_FM_RMX_GAIN_BIT_9(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_FM_RMX_GAIN_BIT_9_MASK (0x7<<4)            
#define WCN_RF_IF_FM_RMX_GAIN_BIT_9_SHIFT (4)                 

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_9(n) (((n)&0x3)<<7)      
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_9_MASK (0x3<<7)            
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_9_SHIFT (7)                 

#define WCN_RF_IF_FM_LNA_GAIN_BIT_9(n) (((n)&0x3)<<9)      
#define WCN_RF_IF_FM_LNA_GAIN_BIT_9_MASK (0x3<<9)            
#define WCN_RF_IF_FM_LNA_GAIN_BIT_9_SHIFT (9)                 

#define WCN_RF_IF_FM_LNA_REG_IBIT_9(n) (((n)&0x7)<<11)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_9_MASK (0x7<<11)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_9_SHIFT (11)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_8(n) (((n)&0xF)<<16)     
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_8_MASK (0xF<<16)           
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_8_SHIFT (16)                

#define WCN_RF_IF_FM_RMX_GAIN_BIT_8(n) (((n)&0x7)<<20)     
#define WCN_RF_IF_FM_RMX_GAIN_BIT_8_MASK (0x7<<20)           
#define WCN_RF_IF_FM_RMX_GAIN_BIT_8_SHIFT (20)                

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_8(n) (((n)&0x3)<<23)     
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_8_MASK (0x3<<23)           
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_8_SHIFT (23)                

#define WCN_RF_IF_FM_LNA_GAIN_BIT_8(n) (((n)&0x3)<<25)     
#define WCN_RF_IF_FM_LNA_GAIN_BIT_8_MASK (0x3<<25)           
#define WCN_RF_IF_FM_LNA_GAIN_BIT_8_SHIFT (25)                

#define WCN_RF_IF_FM_LNA_REG_IBIT_8(n) (((n)&0x7)<<27)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_8_MASK (0x7<<27)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_8_SHIFT (27)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_B(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_B_MASK (0xF<<0)            
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_B_SHIFT (0)                 

#define WCN_RF_IF_FM_RMX_GAIN_BIT_B(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_FM_RMX_GAIN_BIT_B_MASK (0x7<<4)            
#define WCN_RF_IF_FM_RMX_GAIN_BIT_B_SHIFT (4)                 

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_B(n) (((n)&0x3)<<7)      
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_B_MASK (0x3<<7)            
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_B_SHIFT (7)                 

#define WCN_RF_IF_FM_LNA_GAIN_BIT_B(n) (((n)&0x3)<<9)      
#define WCN_RF_IF_FM_LNA_GAIN_BIT_B_MASK (0x3<<9)            
#define WCN_RF_IF_FM_LNA_GAIN_BIT_B_SHIFT (9)                 

#define WCN_RF_IF_FM_LNA_REG_IBIT_B(n) (((n)&0x7)<<11)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_B_MASK (0x7<<11)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_B_SHIFT (11)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_A(n) (((n)&0xF)<<16)     
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_A_MASK (0xF<<16)           
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_A_SHIFT (16)                

#define WCN_RF_IF_FM_RMX_GAIN_BIT_A(n) (((n)&0x7)<<20)     
#define WCN_RF_IF_FM_RMX_GAIN_BIT_A_MASK (0x7<<20)           
#define WCN_RF_IF_FM_RMX_GAIN_BIT_A_SHIFT (20)                

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_A(n) (((n)&0x3)<<23)     
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_A_MASK (0x3<<23)           
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_A_SHIFT (23)                

#define WCN_RF_IF_FM_LNA_GAIN_BIT_A(n) (((n)&0x3)<<25)     
#define WCN_RF_IF_FM_LNA_GAIN_BIT_A_MASK (0x3<<25)           
#define WCN_RF_IF_FM_LNA_GAIN_BIT_A_SHIFT (25)                

#define WCN_RF_IF_FM_LNA_REG_IBIT_A(n) (((n)&0x7)<<27)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_A_MASK (0x7<<27)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_A_SHIFT (27)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_D(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_D_MASK (0xF<<0)            
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_D_SHIFT (0)                 

#define WCN_RF_IF_FM_RMX_GAIN_BIT_D(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_FM_RMX_GAIN_BIT_D_MASK (0x7<<4)            
#define WCN_RF_IF_FM_RMX_GAIN_BIT_D_SHIFT (4)                 

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_D(n) (((n)&0x3)<<7)      
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_D_MASK (0x3<<7)            
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_D_SHIFT (7)                 

#define WCN_RF_IF_FM_LNA_GAIN_BIT_D(n) (((n)&0x3)<<9)      
#define WCN_RF_IF_FM_LNA_GAIN_BIT_D_MASK (0x3<<9)            
#define WCN_RF_IF_FM_LNA_GAIN_BIT_D_SHIFT (9)                 

#define WCN_RF_IF_FM_LNA_REG_IBIT_D(n) (((n)&0x7)<<11)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_D_MASK (0x7<<11)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_D_SHIFT (11)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_C(n) (((n)&0xF)<<16)     
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_C_MASK (0xF<<16)           
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_C_SHIFT (16)                

#define WCN_RF_IF_FM_RMX_GAIN_BIT_C(n) (((n)&0x7)<<20)     
#define WCN_RF_IF_FM_RMX_GAIN_BIT_C_MASK (0x7<<20)           
#define WCN_RF_IF_FM_RMX_GAIN_BIT_C_SHIFT (20)                

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_C(n) (((n)&0x3)<<23)     
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_C_MASK (0x3<<23)           
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_C_SHIFT (23)                

#define WCN_RF_IF_FM_LNA_GAIN_BIT_C(n) (((n)&0x3)<<25)     
#define WCN_RF_IF_FM_LNA_GAIN_BIT_C_MASK (0x3<<25)           
#define WCN_RF_IF_FM_LNA_GAIN_BIT_C_SHIFT (25)                

#define WCN_RF_IF_FM_LNA_REG_IBIT_C(n) (((n)&0x7)<<27)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_C_MASK (0x7<<27)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_C_SHIFT (27)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_F(n) (((n)&0xF)<<0)      
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_F_MASK (0xF<<0)            
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_F_SHIFT (0)                 

#define WCN_RF_IF_FM_RMX_GAIN_BIT_F(n) (((n)&0x7)<<4)      
#define WCN_RF_IF_FM_RMX_GAIN_BIT_F_MASK (0x7<<4)            
#define WCN_RF_IF_FM_RMX_GAIN_BIT_F_SHIFT (4)                 

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_F(n) (((n)&0x3)<<7)      
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_F_MASK (0x3<<7)            
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_F_SHIFT (7)                 

#define WCN_RF_IF_FM_LNA_GAIN_BIT_F(n) (((n)&0x3)<<9)      
#define WCN_RF_IF_FM_LNA_GAIN_BIT_F_MASK (0x3<<9)            
#define WCN_RF_IF_FM_LNA_GAIN_BIT_F_SHIFT (9)                 

#define WCN_RF_IF_FM_LNA_REG_IBIT_F(n) (((n)&0x7)<<11)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_F_MASK (0x7<<11)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_F_SHIFT (11)                

#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_E(n) (((n)&0xF)<<16)     
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_E_MASK (0xF<<16)           
#define WCN_RF_IF_FM_RXFLT_BT_GAIN_BIT_E_SHIFT (16)                

#define WCN_RF_IF_FM_RMX_GAIN_BIT_E(n) (((n)&0x7)<<20)     
#define WCN_RF_IF_FM_RMX_GAIN_BIT_E_MASK (0x7<<20)           
#define WCN_RF_IF_FM_RMX_GAIN_BIT_E_SHIFT (20)                

#define WCN_RF_IF_FM_LNA_RLOAD_BIT_E(n) (((n)&0x3)<<23)     
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_E_MASK (0x3<<23)           
#define WCN_RF_IF_FM_LNA_RLOAD_BIT_E_SHIFT (23)                

#define WCN_RF_IF_FM_LNA_GAIN_BIT_E(n) (((n)&0x3)<<25)     
#define WCN_RF_IF_FM_LNA_GAIN_BIT_E_MASK (0x3<<25)           
#define WCN_RF_IF_FM_LNA_GAIN_BIT_E_SHIFT (25)                

#define WCN_RF_IF_FM_LNA_REG_IBIT_E(n) (((n)&0x7)<<27)     
#define WCN_RF_IF_FM_LNA_REG_IBIT_E_MASK (0x7<<27)           
#define WCN_RF_IF_FM_LNA_REG_IBIT_E_SHIFT (27)                

#define WCN_RF_IF_FM_AGC_GAIN_REG(n)   (((n)&0xF)<<0)      
#define WCN_RF_IF_FM_AGC_GAIN_REG_MASK (0xF<<0)            
#define WCN_RF_IF_FM_AGC_GAIN_REG_SHIFT (0)                 

#define WCN_RF_IF_FM_AGC_GAIN_DR       (1<<4)              
#define WCN_RF_IF_WF_AGC_GAIN_REG(n)   (((n)&0xF)<<8)      
#define WCN_RF_IF_WF_AGC_GAIN_REG_MASK (0xF<<8)            
#define WCN_RF_IF_WF_AGC_GAIN_REG_SHIFT (8)                 

#define WCN_RF_IF_WF_AGC_GAIN_DR       (1<<12)             
#define WCN_RF_IF_BT_AGC_GAIN_REG(n)   (((n)&0xF)<<16)     
#define WCN_RF_IF_BT_AGC_GAIN_REG_MASK (0xF<<16)           
#define WCN_RF_IF_BT_AGC_GAIN_REG_SHIFT (16)                

#define WCN_RF_IF_BT_AGC_GAIN_DR       (1<<20)             
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_1(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_1_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_1_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_1(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_1_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_1_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_0(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_0_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_0_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_0(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_0_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_0_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_3(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_3_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_3_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_3(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_3_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_3_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_2(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_2_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_2_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_2(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_2_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_2_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_5(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_5_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_5_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_5(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_5_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_5_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_4(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_4_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_4_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_4(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_4_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_4_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_7(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_7_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_7_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_7(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_7_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_7_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_6(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_6_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_6_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_6(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_6_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_6_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_9(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_9_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_9_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_9(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_9_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_9_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_8(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_8_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_8_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_8(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_8_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_8_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_B(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_B_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_B_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_B(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_B_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_B_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_A(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_A_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_A_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_A(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_A_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_A_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_D(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_D_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_D_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_D(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_D_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_D_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_C(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_C_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_C_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_C(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_C_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_C_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_F(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_F_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_F_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_F(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_F_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_F_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_E(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_E_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_E_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_E(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_E_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_E_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_D_LO(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_D_LO_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_D_LO_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_D_LO(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_D_LO_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_D_LO_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_C_LO(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_C_LO_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_C_LO_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_C_LO(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_C_LO_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_C_LO_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_F_LO(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_F_LO_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_F_LO_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_F_LO(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_F_LO_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_F_LO_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_E_LO(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_E_LO_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_E_LO_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_E_LO(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_E_LO_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_E_LO_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_D_HI(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_D_HI_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_D_HI_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_D_HI(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_D_HI_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_D_HI_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_C_HI(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_C_HI_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_C_HI_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_C_HI(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_C_HI_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_C_HI_SHIFT (24)                

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_F_HI(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_F_HI_MASK (0xFF<<0)           
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_F_HI_SHIFT (0)                 

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_F_HI(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_F_HI_MASK (0xFF<<8)           
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_F_HI_SHIFT (8)                 

#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_E_HI(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_E_HI_MASK (0xFF<<16)          
#define WCN_RF_IF_BT_RXFLT_CAL_Q_BIT_E_HI_SHIFT (16)                

#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_E_HI(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_E_HI_MASK (0xFF<<24)          
#define WCN_RF_IF_BT_RXFLT_CAL_I_BIT_E_HI_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_1(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_1_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_1_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_1(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_1_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_1_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_0(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_0_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_0_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_0(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_0_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_0_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_3(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_3_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_3_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_3(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_3_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_3_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_2(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_2_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_2_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_2(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_2_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_2_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_5(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_5_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_5_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_5(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_5_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_5_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_4(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_4_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_4_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_4(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_4_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_4_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_7(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_7_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_7_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_7(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_7_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_7_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_6(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_6_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_6_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_6(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_6_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_6_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_9(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_9_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_9_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_9(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_9_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_9_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_8(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_8_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_8_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_8(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_8_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_8_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_B(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_B_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_B_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_B(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_B_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_B_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_A(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_A_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_A_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_A(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_A_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_A_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_D(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_D_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_D_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_D(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_D_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_D_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_C(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_C_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_C_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_C(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_C_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_C_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_F(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_F_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_F_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_F(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_F_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_F_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_E(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_E_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_E_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_E(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_E_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_E_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_D_LO(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_D_LO_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_D_LO_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_D_LO(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_D_LO_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_D_LO_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_C_LO(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_C_LO_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_C_LO_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_C_LO(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_C_LO_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_C_LO_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_F_LO(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_F_LO_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_F_LO_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_F_LO(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_F_LO_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_F_LO_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_E_LO(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_E_LO_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_E_LO_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_E_LO(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_E_LO_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_E_LO_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_D_HI(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_D_HI_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_D_HI_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_D_HI(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_D_HI_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_D_HI_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_C_HI(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_C_HI_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_C_HI_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_C_HI(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_C_HI_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_C_HI_SHIFT (24)                

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_F_HI(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_F_HI_MASK (0xFF<<0)           
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_F_HI_SHIFT (0)                 

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_F_HI(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_F_HI_MASK (0xFF<<8)           
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_F_HI_SHIFT (8)                 

#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_E_HI(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_E_HI_MASK (0xFF<<16)          
#define WCN_RF_IF_WF_RXFLT_CAL_Q_BIT_E_HI_SHIFT (16)                

#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_E_HI(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_E_HI_MASK (0xFF<<24)          
#define WCN_RF_IF_WF_RXFLT_CAL_I_BIT_E_HI_SHIFT (24)                

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_1(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_1_MASK (0xFF<<0)           
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_1_SHIFT (0)                 

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_1(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_1_MASK (0xFF<<8)           
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_1_SHIFT (8)                 

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_0(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_0_MASK (0xFF<<16)          
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_0_SHIFT (16)                

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_0(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_0_MASK (0xFF<<24)          
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_0_SHIFT (24)                

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_3(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_3_MASK (0xFF<<0)           
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_3_SHIFT (0)                 

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_3(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_3_MASK (0xFF<<8)           
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_3_SHIFT (8)                 

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_2(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_2_MASK (0xFF<<16)          
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_2_SHIFT (16)                

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_2(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_2_MASK (0xFF<<24)          
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_2_SHIFT (24)                

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_5(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_5_MASK (0xFF<<0)           
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_5_SHIFT (0)                 

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_5(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_5_MASK (0xFF<<8)           
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_5_SHIFT (8)                 

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_4(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_4_MASK (0xFF<<16)          
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_4_SHIFT (16)                

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_4(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_4_MASK (0xFF<<24)          
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_4_SHIFT (24)                

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_7(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_7_MASK (0xFF<<0)           
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_7_SHIFT (0)                 

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_7(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_7_MASK (0xFF<<8)           
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_7_SHIFT (8)                 

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_6(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_6_MASK (0xFF<<16)          
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_6_SHIFT (16)                

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_6(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_6_MASK (0xFF<<24)          
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_6_SHIFT (24)                

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_9(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_9_MASK (0xFF<<0)           
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_9_SHIFT (0)                 

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_9(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_9_MASK (0xFF<<8)           
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_9_SHIFT (8)                 

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_8(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_8_MASK (0xFF<<16)          
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_8_SHIFT (16)                

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_8(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_8_MASK (0xFF<<24)          
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_8_SHIFT (24)                

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_B(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_B_MASK (0xFF<<0)           
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_B_SHIFT (0)                 

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_B(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_B_MASK (0xFF<<8)           
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_B_SHIFT (8)                 

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_A(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_A_MASK (0xFF<<16)          
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_A_SHIFT (16)                

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_A(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_A_MASK (0xFF<<24)          
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_A_SHIFT (24)                

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_D(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_D_MASK (0xFF<<0)           
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_D_SHIFT (0)                 

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_D(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_D_MASK (0xFF<<8)           
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_D_SHIFT (8)                 

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_C(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_C_MASK (0xFF<<16)          
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_C_SHIFT (16)                

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_C(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_C_MASK (0xFF<<24)          
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_C_SHIFT (24)                

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_F(n) (((n)&0xFF)<<0)     
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_F_MASK (0xFF<<0)           
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_F_SHIFT (0)                 

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_F(n) (((n)&0xFF)<<8)     
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_F_MASK (0xFF<<8)           
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_F_SHIFT (8)                 

#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_E(n) (((n)&0xFF)<<16)    
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_E_MASK (0xFF<<16)          
#define WCN_RF_IF_FM_RXFLT_CAL_Q_BIT_E_SHIFT (16)                

#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_E(n) (((n)&0xFF)<<24)    
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_E_MASK (0xFF<<24)          
#define WCN_RF_IF_FM_RXFLT_CAL_I_BIT_E_SHIFT (24)                

#define WCN_RF_IF_PLL_TEST_OUT_EN      (1<<0)              
#define WCN_RF_IF_DAC_OUT_EN           (1<<1)              
#define WCN_RF_IF_TX_IF_EN             (1<<2)              
#define WCN_RF_IF_REFCLK_LVDS_EN       (1<<3)              
#define WCN_RF_IF_BW_RSVD_IN(n)        (((n)&0xFFFF)<<0)   
#define WCN_RF_IF_BW_RSVD_IN_MASK      (0xFFFF<<0)         
#define WCN_RF_IF_BW_RSVD_IN_SHIFT     (0)                 

#define WCN_RF_IF_ADPLL_RSVD_IN(n)     (((n)&0xFF)<<16)    
#define WCN_RF_IF_ADPLL_RSVD_IN_MASK   (0xFF<<16)          
#define WCN_RF_IF_ADPLL_RSVD_IN_SHIFT  (16)                

#define WCN_RF_IF_BW_RSVD_OUT(n)       (((n)&0xFF)<<24)    
#define WCN_RF_IF_BW_RSVD_OUT_MASK     (0xFF<<24)          
#define WCN_RF_IF_BW_RSVD_OUT_SHIFT    (24)                

#define WCN_RF_IF_DIG_RSVD(n)          (((n)&0xFFFF)<<0)   
#define WCN_RF_IF_DIG_RSVD_MASK        (0xFFFF<<0)         
#define WCN_RF_IF_DIG_RSVD_SHIFT       (0)                 

#define WCN_RF_IF_TXRF_CAPBANK_EN_REG  (1<<0)              
#define WCN_RF_IF_TXRF_CAPBANK_EN_DR   (1<<1)              
#define WCN_RF_IF_PLL_REG_DIGI_BIT(n)  (((n)&0xF)<<2)      
#define WCN_RF_IF_PLL_REG_DIGI_BIT_MASK (0xF<<2)            
#define WCN_RF_IF_PLL_REG_DIGI_BIT_SHIFT (2)                 

#define WCN_RF_IF_BT_DAC_LP_MODE2(n)   (((n)&0x3)<<6)      
#define WCN_RF_IF_BT_DAC_LP_MODE2_MASK (0x3<<6)            
#define WCN_RF_IF_BT_DAC_LP_MODE2_SHIFT (6)                 


#endif


