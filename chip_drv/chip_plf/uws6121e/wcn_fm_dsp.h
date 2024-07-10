/*                                                                        
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.                      
*                                                                         
* Licensed under the Apache License, Version 2.0 (the "License");       
* you may not use this file except in compliance with the License.        
* You may obtain a copy of the License at                                 
*                                                                         
*     http://www.apache.org/licenses/LICENSE-2.0                          
*                                                                         
* Unless required by applicable law or agreed to in writing, software     
* distributed under the License is distributed on an "AS IS" BASIS,     
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and     
* limitations under the License.                                          
*/                                                                        
//------------------------------------------------------------------------
// File name      : fm_dsp                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:43                                                  
// Last modified  : 2016-12-21 13:55:43                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:43 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __FM_DSP_H__
#define __FM_DSP_H__

typedef unsigned int        U32;

#define REG_WCN_FM_DSP_BASE      0x15102000
typedef struct
{
    volatile                             U32                  tx_fifo_rdata; /*0x0  , RO   , 0x00000000, No description.               */
    volatile                             U32                       revision; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                             U32                           ctrl; /*0x8  , RW   , 0x00550003, No description.               */
    volatile                             U32                        tx_ctrl; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                             U32                     seek_ctrl0; /*0x10 , RW   , 0x14C4A91B, No description.               */
    volatile                             U32                     seek_ctrl1; /*0x14 , RW   , 0x32323232, No description.               */
    volatile                             U32                     seek_ctrl2; /*0x18 , RW   , 0x08D18794, No description.               */
    volatile                             U32                     seek_ctrl3; /*0x1c , RW   , 0x0206430A, No description.               */
    volatile                             U32                     seek_ctrl4; /*0x20 , RW   , 0x00281472, No description.               */
    volatile                             U32                       afc_ctrl; /*0x24 , RW   , 0x0002530F, No description.               */
    volatile                             U32                      agc_ctrl0; /*0x28 , RW   , 0x4C9E087C, No description.               */
    volatile                             U32                      agc_ctrl1; /*0x2c , RW   , 0x00003179, No description.               */
    volatile                             U32              ana_gain_rssi_tb0; /*0x30 , RW   , 0xEAF40000, No description.               */
    volatile                             U32              ana_gain_rssi_tb1; /*0x34 , RW   , 0x120C0600, No description.               */
    volatile                             U32              ana_gain_rssi_tb2; /*0x38 , RW   , 0x2A241E18, No description.               */
    volatile                             U32              ana_gain_rssi_tb3; /*0x3c , RW   , 0x423C3630, No description.               */
    volatile                             U32             noise_cancel_ctrl0; /*0x40 , RW   , 0x040C3F10, No description.               */
    volatile                             U32             noise_cancel_ctrl1; /*0x44 , RW   , 0x0004C9E0, No description.               */
    volatile                             U32             noise_cancel_ctrl2; /*0x48 , RW   , 0x2726CE4D, No description.               */
    volatile                             U32                 datapath_ctrl0; /*0x4c , RW   , 0x0043289B, No description.               */
    volatile                             U32                 datapath_ctrl1; /*0x50 , RW   , 0x674A2505, No description.               */
    volatile                             U32                 datapath_ctrl2; /*0x54 , RW   , 0xD1F52283, No description.               */
    volatile                             U32                 datapath_ctrl3; /*0x58 , RW   , 0x3A5E3000, No description.               */
    volatile                             U32                 datapath_ctrl4; /*0x5c , RW   , 0x00700000, No description.               */
    volatile                             U32                 datapath_ctrl5; /*0x60 , RW   , 0x00000000, No description.               */
    volatile                             U32                      log_ctrl0; /*0x64 , RW   , 0x0071C71C, No description.               */
    volatile                             U32                        status0; /*0x68 , RO   , 0x00000000, No description.               */
    volatile                             U32                        status1; /*0x6c , RO   , 0x00000000, No description.               */
    volatile                             U32                        status2; /*0x70 , RO   , 0x00000000, No description.               */
    volatile                             U32                        status3; /*0x74 , RO   , 0x00000000, No description.               */
    volatile                             U32                       rsvd_reg; /*0x78 , RW   , 0x0000FF00, No description.               */
} T_HWP_FM_DSP_T;


#define hwp_fm_dsp             ((T_HWP_FM_DSP_T*) (REG_WCN_FM_DSP_BASE))

#define FM_DSP_TX_DATA_Q(n)            (((n)&0xFFFF)<<0)   
#define FM_DSP_TX_DATA_Q_MASK          (0xFFFF<<0)         
#define FM_DSP_TX_DATA_Q_SHIFT         (0)                 

#define FM_DSP_TX_DATA_I(n)            (((n)&0xFFFF)<<16)  
#define FM_DSP_TX_DATA_I_MASK          (0xFFFF<<16)        
#define FM_DSP_TX_DATA_I_SHIFT         (16)                


#define FM_DSP_REV_ID(n)               (((n)&0xF)<<0)      
#define FM_DSP_REV_ID_MASK             (0xF<<0)            
#define FM_DSP_REV_ID_SHIFT            (0)                 


#define FM_DSP_MUTE                    (1<<0)              
#define FM_DSP_MUTE_NORMAL             (0<<0)              
#define FM_DSP_MUTE_MUTE               (1<<0)              

#define FM_DSP_STEREO_IN               (1<<1)              
#define FM_DSP_STEREO_IN_FORCE         (0<<1)              
#define FM_DSP_STEREO_IN_STEREO        (1<<1)              

#define FM_DSP_DEEMPH_MODE             (1<<2)              
#define FM_DSP_DEEMPH_MODE_75US        (0<<2)              
#define FM_DSP_DEEMPH_MODE_50US        (1<<2)              

#define FM_DSP_SOFTMUTE_EN             (1<<3)              
#define FM_DSP_SOFTMUTE_EN_DISABLE     (0<<3)              
#define FM_DSP_SOFTMUTE_EN_ENABLE      (1<<3)              

#define FM_DSP_SOFTBLEND_OFF           (1<<4)              
#define FM_DSP_SOFTBLEND_OFF_ENABLE    (0<<4)              
#define FM_DSP_SOFTBLEND_OFF_DISABLE   (1<<4)              

#define FM_DSP_AFC_DISABLE             (1<<5)              
#define FM_DSP_AFC_DISABLE_ENABLE      (0<<5)              
#define FM_DSP_AFC_DISABLE_DISABLE     (1<<5)              

#define FM_DSP_IMGREJ                  (1<<6)              
#define FM_DSP_IMGREJ_LOW              (0<<6)              
#define FM_DSP_IMGREJ_HIGH             (1<<6)              

#define FM_DSP_IMGREJ_DSP              (1<<7)              

#define FM_DSP_IQ_SWAP                 (1<<8)              

#define FM_DSP_PLLDEM_SWAP             (1<<9)              

#define FM_DSP_LR_SWAP                 (1<<10)             

#define FM_DSP_FIRCUT_BYPASS           (1<<11)             

#define FM_DSP_FIRCUT_BYPASS_SK        (1<<12)             

#define FM_DSP_LPF_BYPASS              (1<<13)             

#define FM_DSP_DEEMPH_BYPASS           (1<<14)             

#define FM_DSP_PILOT_COSINE            (1<<15)             
#define FM_DSP_PILOT_COSINE_SIN        (0<<15)             
#define FM_DSP_PILOT_COSINE_COS        (1<<15)             

#define FM_DSP_ADC_CLK_INVERT          (1<<16)             

#define FM_DSP_NOISE_SRC_SEL(n)        (((n)&0x3)<<17)     
#define FM_DSP_NOISE_SRC_SEL_DANGLE0   (((0)&0x3)<<17)     
#define FM_DSP_NOISE_SRC_SEL_DANGLE1   (((1)&0x3)<<17)     
#define FM_DSP_NOISE_SRC_SEL_MASK      (0x3<<17)           
#define FM_DSP_NOISE_SRC_SEL_SHIFT     (17)                

#define FM_DSP_NOISE_CANCEL_SRC_SEL    (1<<19)             
#define FM_DSP_NOISE_CANCEL_SRC_SEL_NOISE_DB2 (0<<19)             
#define FM_DSP_NOISE_CANCEL_SRC_SEL_RSSI_DB2 (1<<19)             

#define FM_DSP_SEEK_RSSI_SRC_SEL       (1<<20)             
#define FM_DSP_SEEK_RSSI_SRC_SEL_RSSI_DB1 (0<<20)             
#define FM_DSP_SEEK_RSSI_SRC_SEL_SIGNAL_DB1 (1<<20)             

#define FM_DSP_SEEK_OFFSET_SRC_SEL     (1<<21)             
#define FM_DSP_SEEK_OFFSET_SRC_SEL_OUTPUT_AFC (0<<21)             
#define FM_DSP_SEEK_OFFSET_SRC_SEL_OUTPUT_OFFSET_FILTER (1<<21)             

#define FM_DSP_FMDEM_SRC_SEL           (1<<22)             
#define FM_DSP_FMDEM_SRC_SEL_OUTPUT_LPFIL (0<<22)             
#define FM_DSP_FMDEM_SRC_SEL_OUTPUT_DIG_GAIN (1<<22)             

#define FM_DSP_I2S_EN                  (1<<23)             
#define FM_DSP_I2S_EN_DISABLE          (0<<23)             
#define FM_DSP_I2S_EN_ENABLE           (1<<23)             

#define FM_DSP_I2S_DELAY_1T            (1<<24)             

#define FM_DSP_TX_EN                   (1<<25)             
#define FM_DSP_TX_EN_DISABLE           (0<<25)             
#define FM_DSP_TX_EN_ENABLE            (1<<25)             

#define FM_DSP_FORCE_CLK_ON            (1<<26)             

#define FM_DSP_ADC_DATA_FORMAT         (1<<27)             
#define FM_DSP_ADC_DATA_FORMAT_UNSIGNED (0<<27)             
#define FM_DSP_ADC_DATA_FORMAT_2S_COMPLEMENTARY (1<<27)             

#define FM_DSP_DBG_OUT_EN              (1<<28)             


#define FM_DSP_TX_SEL(n)               (((n)&0x1F)<<0)     
#define FM_DSP_TX_SEL_MASK             (0x1F<<0)           
#define FM_DSP_TX_SEL_SHIFT            (0)                 

#define FM_DSP_TX_FIFO_CLR             (1<<5)              

#define FM_DSP_TX_FIFO_OVERFLOW        (1<<6)              

#define FM_DSP_TX_FIFO_UNDERFLOW       (1<<7)              

#define FM_DSP_TX_FIFO_USEDW(n)        (((n)&0x7)<<8)      
#define FM_DSP_TX_FIFO_USEDW_MASK      (0x7<<8)            
#define FM_DSP_TX_FIFO_USEDW_SHIFT     (8)                 


#define FM_DSP_SK_CMP_GRP_EN(n)        (((n)&0x1F)<<0)     
#define FM_DSP_SK_CMP_GRP_EN_MASK      (0x1F<<0)           
#define FM_DSP_SK_CMP_GRP_EN_SHIFT     (0)                 

#define FM_DSP_SEEK_MODE(n)            (((n)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SEEK_CURRENT_ONLY (((0)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SEEK_CURRENT_OR_ADJACENT (((1)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SEEK_CURRENT_AND_ADJACENT (((2)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SNR_ST        (((3)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_CENTER_ST     (((4)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_UPPER_ST      (((5)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_LOWER_ST      (((6)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_SEEK_BYPASS   (((7)&0x7)<<5)      
#define FM_DSP_SEEK_MODE_MASK          (0x7<<5)            
#define FM_DSP_SEEK_MODE_SHIFT         (5)                 

#define FM_DSP_SEEK_AFC_ON             (1<<8)              
#define FM_DSP_SEEK_AFC_ON_DISABLE     (0<<8)              
#define FM_DSP_SEEK_AFC_ON_ENABLE      (1<<8)              

#define FM_DSP_SEEK_RANGE(n)           (((n)&0x1F)<<9)     
#define FM_DSP_SEEK_RANGE_MASK         (0x1F<<9)           
#define FM_DSP_SEEK_RANGE_SHIFT        (9)                 

#define FM_DSP_SNR_TH(n)               (((n)&0x3F)<<14)    
#define FM_DSP_SNR_TH_MASK             (0x3F<<14)          
#define FM_DSP_SNR_TH_SHIFT            (14)                

#define FM_DSP_DELTA_RSSI_TH(n)        (((n)&0x3F)<<20)    
#define FM_DSP_DELTA_RSSI_TH_MASK      (0x3F<<20)          
#define FM_DSP_DELTA_RSSI_TH_SHIFT     (20)                

#define FM_DSP_SNR_CNT_TH(n)           (((n)&0xF)<<26)     
#define FM_DSP_SNR_CNT_TH_MASK         (0xF<<26)           
#define FM_DSP_SNR_CNT_TH_SHIFT        (26)                


#define FM_DSP_OFFSET_CNT_TH(n)        (((n)&0xFF)<<0)     
#define FM_DSP_OFFSET_CNT_TH_MASK      (0xFF<<0)           
#define FM_DSP_OFFSET_CNT_TH_SHIFT     (0)                 

#define FM_DSP_NOISE_CNT_TH(n)         (((n)&0xFF)<<8)     
#define FM_DSP_NOISE_CNT_TH_MASK       (0xFF<<8)           
#define FM_DSP_NOISE_CNT_TH_SHIFT      (8)                 

#define FM_DSP_RSSI_CNT_TH(n)          (((n)&0xFF)<<16)    
#define FM_DSP_RSSI_CNT_TH_MASK        (0xFF<<16)          
#define FM_DSP_RSSI_CNT_TH_SHIFT       (16)                

#define FM_DSP_PILOT_CNT_TH(n)         (((n)&0xFF)<<24)    
#define FM_DSP_PILOT_CNT_TH_MASK       (0xFF<<24)          
#define FM_DSP_PILOT_CNT_TH_SHIFT      (24)                


#define FM_DSP_RSSI_H_TH(n)            (((n)&0x7F)<<0)     
#define FM_DSP_RSSI_H_TH_MASK          (0x7F<<0)           
#define FM_DSP_RSSI_H_TH_SHIFT         (0)                 

#define FM_DSP_RSSI_L_TH(n)            (((n)&0x7F)<<7)     
#define FM_DSP_RSSI_L_TH_MASK          (0x7F<<7)           
#define FM_DSP_RSSI_L_TH_SHIFT         (7)                 

#define FM_DSP_NOISE_H_TH(n)           (((n)&0x7F)<<14)    
#define FM_DSP_NOISE_H_TH_MASK         (0x7F<<14)          
#define FM_DSP_NOISE_H_TH_SHIFT        (14)                

#define FM_DSP_NOISE_L_TH(n)           (((n)&0x7F)<<21)    
#define FM_DSP_NOISE_L_TH_MASK         (0x7F<<21)          
#define FM_DSP_NOISE_L_TH_SHIFT        (21)                


#define FM_DSP_PILOT_H_TH(n)           (((n)&0x7F)<<0)     
#define FM_DSP_PILOT_H_TH_MASK         (0x7F<<0)           
#define FM_DSP_PILOT_H_TH_SHIFT        (0)                 

#define FM_DSP_PILOT_L_TH(n)           (((n)&0x7F)<<7)     
#define FM_DSP_PILOT_L_TH_MASK         (0x7F<<7)           
#define FM_DSP_PILOT_L_TH_SHIFT        (7)                 

#define FM_DSP_OFFSET_H_TH(n)          (((n)&0x7F)<<14)    
#define FM_DSP_OFFSET_H_TH_MASK        (0x7F<<14)          
#define FM_DSP_OFFSET_H_TH_SHIFT       (14)                

#define FM_DSP_OFFSET_L_TH(n)          (((n)&0x7F)<<21)    
#define FM_DSP_OFFSET_L_TH_MASK        (0x7F<<21)          
#define FM_DSP_OFFSET_L_TH_SHIFT       (21)                


#define FM_DSP_SK_TIMER1(n)            (((n)&0xF)<<0)      
#define FM_DSP_SK_TIMER1_MASK          (0xF<<0)            
#define FM_DSP_SK_TIMER1_SHIFT         (0)                 

#define FM_DSP_SK_TIMER2(n)            (((n)&0x3F)<<4)     
#define FM_DSP_SK_TIMER2_MASK          (0x3F<<4)           
#define FM_DSP_SK_TIMER2_SHIFT         (4)                 

#define FM_DSP_SK_TIMER3(n)            (((n)&0x3F)<<10)    
#define FM_DSP_SK_TIMER3_MASK          (0x3F<<10)          
#define FM_DSP_SK_TIMER3_SHIFT         (10)                

#define FM_DSP_SK_TIMER4(n)            (((n)&0x3F)<<16)    
#define FM_DSP_SK_TIMER4_MASK          (0x3F<<16)          
#define FM_DSP_SK_TIMER4_SHIFT         (16)                


#define FM_DSP_AFC_INV                 (1<<0)              

#define FM_DSP_AFC_RANGE(n)            (((n)&0x3F)<<1)     
#define FM_DSP_AFC_RANGE_MASK          (0x3F<<1)           
#define FM_DSP_AFC_RANGE_SHIFT         (1)                 

#define FM_DSP_AFC_CT(n)               (((n)&0x3F)<<7)     
#define FM_DSP_AFC_CT_MASK             (0x3F<<7)           
#define FM_DSP_AFC_CT_SHIFT            (7)                 

#define FM_DSP_AFC_CT_SK(n)            (((n)&0x3F)<<13)    
#define FM_DSP_AFC_CT_SK_MASK          (0x3F<<13)          
#define FM_DSP_AFC_CT_SK_SHIFT         (13)                


#define FM_DSP_CT_ENDELAY(n)           (((n)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_0            (((0)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_1            (((1)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_2            (((2)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_3            (((3)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_4            (((4)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_5            (((5)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_6            (((6)&0x7)<<0)      
#define FM_DSP_CT_ENDELAY_MASK         (0x7<<0)            
#define FM_DSP_CT_ENDELAY_SHIFT        (0)                 

#define FM_DSP_AGC_INDEX_IN(n)         (((n)&0xF)<<3)      
#define FM_DSP_AGC_INDEX_IN_MASK       (0xF<<3)            
#define FM_DSP_AGC_INDEX_IN_SHIFT      (3)                 

#define FM_DSP_AGC_THD(n)              (((n)&0x1F)<<7)     
#define FM_DSP_AGC_THD_MASK            (0x1F<<7)           
#define FM_DSP_AGC_THD_SHIFT           (7)                 

#define FM_DSP_AGC_UPDATE0(n)          (((n)&0x7)<<12)     
#define FM_DSP_AGC_UPDATE0_MASK        (0x7<<12)           
#define FM_DSP_AGC_UPDATE0_SHIFT       (12)                

#define FM_DSP_AGC_UPDATE1(n)          (((n)&0x7)<<15)     
#define FM_DSP_AGC_UPDATE1_MASK        (0x7<<15)           
#define FM_DSP_AGC_UPDATE1_SHIFT       (15)                

#define FM_DSP_AGC_LOOP_GAIN0(n)       (((n)&0x7)<<18)     
#define FM_DSP_AGC_LOOP_GAIN0_MASK     (0x7<<18)           
#define FM_DSP_AGC_LOOP_GAIN0_SHIFT    (18)                

#define FM_DSP_AGC_LOOP_GAIN1(n)       (((n)&0x7)<<21)     
#define FM_DSP_AGC_LOOP_GAIN1_MASK     (0x7<<21)           
#define FM_DSP_AGC_LOOP_GAIN1_SHIFT    (21)                

#define FM_DSP_AGC_TEST                (1<<24)             

#define FM_DSP_AGC_TARGET_PWR(n)       (((n)&0x3F)<<25)    
#define FM_DSP_AGC_TARGET_PWR_MASK     (0x3F<<25)          
#define FM_DSP_AGC_TARGET_PWR_SHIFT    (25)                


#define FM_DSP_LOG_OVER_SEL            (1<<0)              

#define FM_DSP_LOG_AGC_TH(n)           (((n)&0x3F)<<1)     
#define FM_DSP_LOG_AGC_TH_MASK         (0x3F<<1)           
#define FM_DSP_LOG_AGC_TH_SHIFT        (1)                 

#define FM_DSP_OVER_STEP(n)            (((n)&0x3F)<<7)     
#define FM_DSP_OVER_STEP_MASK          (0x3F<<7)           
#define FM_DSP_OVER_STEP_SHIFT         (7)                 

#define FM_DSP_AGC_CT_U(n)             (((n)&0x3)<<13)     
#define FM_DSP_AGC_CT_U_MASK           (0x3<<13)           
#define FM_DSP_AGC_CT_U_SHIFT          (13)                

#define FM_DSP_DIG_GAIN_IN(n)          (((n)&0x1F)<<15)    
#define FM_DSP_DIG_GAIN_IN_MASK        (0x1F<<15)          
#define FM_DSP_DIG_GAIN_IN_SHIFT       (15)                


#define FM_DSP_ANA_GAIN_RSSI_0(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_0_MASK    (0xFF<<0)           
#define FM_DSP_ANA_GAIN_RSSI_0_SHIFT   (0)                 

#define FM_DSP_ANA_GAIN_RSSI_1(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_1_MASK    (0xFF<<8)           
#define FM_DSP_ANA_GAIN_RSSI_1_SHIFT   (8)                 

#define FM_DSP_ANA_GAIN_RSSI_2(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_2_MASK    (0xFF<<16)          
#define FM_DSP_ANA_GAIN_RSSI_2_SHIFT   (16)                

#define FM_DSP_ANA_GAIN_RSSI_3(n)      (((n)&0xFF)<<24)    
#define FM_DSP_ANA_GAIN_RSSI_3_MASK    (0xFF<<24)          
#define FM_DSP_ANA_GAIN_RSSI_3_SHIFT   (24)                


#define FM_DSP_ANA_GAIN_RSSI_4(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_4_MASK    (0xFF<<0)           
#define FM_DSP_ANA_GAIN_RSSI_4_SHIFT   (0)                 

#define FM_DSP_ANA_GAIN_RSSI_5(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_5_MASK    (0xFF<<8)           
#define FM_DSP_ANA_GAIN_RSSI_5_SHIFT   (8)                 

#define FM_DSP_ANA_GAIN_RSSI_6(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_6_MASK    (0xFF<<16)          
#define FM_DSP_ANA_GAIN_RSSI_6_SHIFT   (16)                

#define FM_DSP_ANA_GAIN_RSSI_7(n)      (((n)&0xFF)<<24)    
#define FM_DSP_ANA_GAIN_RSSI_7_MASK    (0xFF<<24)          
#define FM_DSP_ANA_GAIN_RSSI_7_SHIFT   (24)                


#define FM_DSP_ANA_GAIN_RSSI_8(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_8_MASK    (0xFF<<0)           
#define FM_DSP_ANA_GAIN_RSSI_8_SHIFT   (0)                 

#define FM_DSP_ANA_GAIN_RSSI_9(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_9_MASK    (0xFF<<8)           
#define FM_DSP_ANA_GAIN_RSSI_9_SHIFT   (8)                 

#define FM_DSP_ANA_GAIN_RSSI_A(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_A_MASK    (0xFF<<16)          
#define FM_DSP_ANA_GAIN_RSSI_A_SHIFT   (16)                

#define FM_DSP_ANA_GAIN_RSSI_B(n)      (((n)&0xFF)<<24)    
#define FM_DSP_ANA_GAIN_RSSI_B_MASK    (0xFF<<24)          
#define FM_DSP_ANA_GAIN_RSSI_B_SHIFT   (24)                


#define FM_DSP_ANA_GAIN_RSSI_C(n)      (((n)&0xFF)<<0)     
#define FM_DSP_ANA_GAIN_RSSI_C_MASK    (0xFF<<0)           
#define FM_DSP_ANA_GAIN_RSSI_C_SHIFT   (0)                 

#define FM_DSP_ANA_GAIN_RSSI_D(n)      (((n)&0xFF)<<8)     
#define FM_DSP_ANA_GAIN_RSSI_D_MASK    (0xFF<<8)           
#define FM_DSP_ANA_GAIN_RSSI_D_SHIFT   (8)                 

#define FM_DSP_ANA_GAIN_RSSI_E(n)      (((n)&0xFF)<<16)    
#define FM_DSP_ANA_GAIN_RSSI_E_MASK    (0xFF<<16)          
#define FM_DSP_ANA_GAIN_RSSI_E_SHIFT   (16)                

#define FM_DSP_ANA_GAIN_RSSI_F(n)      (((n)&0xFF)<<24)    
#define FM_DSP_ANA_GAIN_RSSI_F_MASK    (0xFF<<24)          
#define FM_DSP_ANA_GAIN_RSSI_F_SHIFT   (24)                


#define FM_DSP_FMDEM_SEL_GRP(n)        (((n)&0x7)<<0)      
#define FM_DSP_FMDEM_SEL_GRP_MASK      (0x7<<0)            
#define FM_DSP_FMDEM_SEL_GRP_SHIFT     (0)                 

#define FM_DSP_PLLDEM_TH_MAX(n)        (((n)&0x3F)<<3)     
#define FM_DSP_PLLDEM_TH_MAX_MASK      (0x3F<<3)           
#define FM_DSP_PLLDEM_TH_MAX_SHIFT     (3)                 

#define FM_DSP_PLLDEM_TH_MIN(n)        (((n)&0x3F)<<9)     
#define FM_DSP_PLLDEM_TH_MIN_MASK      (0x3F<<9)           
#define FM_DSP_PLLDEM_TH_MIN_SHIFT     (9)                 

#define FM_DSP_TH_MAX_BW(n)            (((n)&0x7F)<<15)    
#define FM_DSP_TH_MAX_BW_MASK          (0x7F<<15)          
#define FM_DSP_TH_MAX_BW_SHIFT         (15)                

#define FM_DSP_TH_MIN_BW(n)            (((n)&0x7F)<<22)    
#define FM_DSP_TH_MIN_BW_MASK          (0x7F<<22)          
#define FM_DSP_TH_MIN_BW_SHIFT         (22)                


#define FM_DSP_TH_MIN_HCC(n)           (((n)&0x3F)<<0)     
#define FM_DSP_TH_MIN_HCC_MASK         (0x3F<<0)           
#define FM_DSP_TH_MIN_HCC_SHIFT        (0)                 

#define FM_DSP_SEL_FIRCUT1(n)          (((n)&0x7)<<6)      
#define FM_DSP_SEL_FIRCUT1_MASK        (0x7<<6)            
#define FM_DSP_SEL_FIRCUT1_SHIFT       (6)                 

#define FM_DSP_SEL_FIRCUT2(n)          (((n)&0x7)<<9)      
#define FM_DSP_SEL_FIRCUT2_MASK        (0x7<<9)            
#define FM_DSP_SEL_FIRCUT2_SHIFT       (9)                 

#define FM_DSP_SEL_FIRCUT3(n)          (((n)&0x7)<<12)     
#define FM_DSP_SEL_FIRCUT3_MASK        (0x7<<12)           
#define FM_DSP_SEL_FIRCUT3_SHIFT       (12)                

#define FM_DSP_SEL_FIRCUT_SK(n)        (((n)&0x3F)<<15)    
#define FM_DSP_SEL_FIRCUT_SK_MASK      (0x3F<<15)          
#define FM_DSP_SEL_FIRCUT_SK_SHIFT     (15)                

#define FM_DSP_FIRCUT_SEL_FORCE_ON     (1<<21)             


#define FM_DSP_GAIN_38K1(n)            (((n)&0x7FFF)<<0)   
#define FM_DSP_GAIN_38K1_MASK          (0x7FFF<<0)         
#define FM_DSP_GAIN_38K1_SHIFT         (0)                 

#define FM_DSP_GAIN_38K2(n)            (((n)&0x7FFF)<<15)  
#define FM_DSP_GAIN_38K2_MASK          (0x7FFF<<15)        
#define FM_DSP_GAIN_38K2_SHIFT         (15)                


#define FM_DSP_DC_CT(n)                (((n)&0xF)<<0)      
#define FM_DSP_DC_CT_MASK              (0xF<<0)            
#define FM_DSP_DC_CT_SHIFT             (0)                 

#define FM_DSP_LIMIT_SEL               (1<<4)              

#define FM_DSP_OVER_TH_SEL(n)          (((n)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_0           (((0)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_1           (((1)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_2           (((2)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_3           (((3)&0x7)<<5)      
#define FM_DSP_OVER_TH_SEL_MASK        (0x7<<5)            
#define FM_DSP_OVER_TH_SEL_SHIFT       (5)                 

#define FM_DSP_SINC_LIMIT_GAIN(n)      (((n)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_0DB     (((0)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_6DB     (((1)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_12DB    (((2)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_18DB    (((3)&0x3)<<8)      
#define FM_DSP_SINC_LIMIT_GAIN_MASK    (0x3<<8)            
#define FM_DSP_SINC_LIMIT_GAIN_SHIFT   (8)                 

#define FM_DSP_DIG_DELAYCT(n)          (((n)&0x1F)<<10)    
#define FM_DSP_DIG_DELAYCT_MASK        (0x1F<<10)          
#define FM_DSP_DIG_DELAYCT_SHIFT       (10)                

#define FM_DSP_GAINCT(n)               (((n)&0x1FF)<<15)   
#define FM_DSP_GAINCT_MASK             (0x1FF<<15)         
#define FM_DSP_GAINCT_SHIFT            (15)                

#define FM_DSP_HBF2_BYPASS             (1<<24)             

#define FM_DSP_HBF1_BYPASS             (1<<25)             


#define FM_DSP_SOFTMUTE_TH3(n)         (((n)&0x3F)<<0)     
#define FM_DSP_SOFTMUTE_TH3_MASK       (0x3F<<0)           
#define FM_DSP_SOFTMUTE_TH3_SHIFT      (0)                 

#define FM_DSP_SOFTMUTE_TH2(n)         (((n)&0x3F)<<6)     
#define FM_DSP_SOFTMUTE_TH2_MASK       (0x3F<<6)           
#define FM_DSP_SOFTMUTE_TH2_SHIFT      (6)                 

#define FM_DSP_SOFTMUTE_TH1(n)         (((n)&0x3F)<<12)    
#define FM_DSP_SOFTMUTE_TH1_MASK       (0x3F<<12)          
#define FM_DSP_SOFTMUTE_TH1_SHIFT      (12)                

#define FM_DSP_NCO_CT_U(n)             (((n)&0x7)<<18)     
#define FM_DSP_NCO_CT_U_MASK           (0x7<<18)           
#define FM_DSP_NCO_CT_U_SHIFT          (18)                

#define FM_DSP_NCO_CT_BW(n)            (((n)&0xF)<<21)     
#define FM_DSP_NCO_CT_BW_MASK          (0xF<<21)           
#define FM_DSP_NCO_CT_BW_SHIFT         (21)                

#define FM_DSP_NCO_CT_U_DT_SK(n)       (((n)&0x7)<<25)     
#define FM_DSP_NCO_CT_U_DT_SK_MASK     (0x7<<25)           
#define FM_DSP_NCO_CT_U_DT_SK_SHIFT    (25)                

#define FM_DSP_NCO_CT_U_DT(n)          (((n)&0x7)<<28)     
#define FM_DSP_NCO_CT_U_DT_MASK        (0x7<<28)           
#define FM_DSP_NCO_CT_U_DT_SHIFT       (28)                


#define FM_DSP_DEEMPH_HCC(n)           (((n)&0x3)<<0)      
#define FM_DSP_DEEMPH_HCC_MASK         (0x3<<0)            
#define FM_DSP_DEEMPH_HCC_SHIFT        (0)                 

#define FM_DSP_LPF_BW_REG(n)           (((n)&0x7)<<2)      
#define FM_DSP_LPF_BW_REG_MASK         (0x7<<2)            
#define FM_DSP_LPF_BW_REG_SHIFT        (2)                 

#define FM_DSP_LPF_BW_DR               (1<<5)              

#define FM_DSP_OFFSET_FLT_CT_U(n)      (((n)&0x3)<<6)      
#define FM_DSP_OFFSET_FLT_CT_U_MASK    (0x3<<6)            
#define FM_DSP_OFFSET_FLT_CT_U_SHIFT   (6)                 

#define FM_DSP_TH_SOFTBLEND2(n)        (((n)&0x3F)<<8)     
#define FM_DSP_TH_SOFTBLEND2_MASK      (0x3F<<8)           
#define FM_DSP_TH_SOFTBLEND2_SHIFT     (8)                 

#define FM_DSP_TH_SOFTBLEND1(n)        (((n)&0x3F)<<14)    
#define FM_DSP_TH_SOFTBLEND1_MASK      (0x3F<<14)          
#define FM_DSP_TH_SOFTBLEND1_SHIFT     (14)                

#define FM_DSP_SM_ATTENU(n)            (((n)&0xF)<<20)     
#define FM_DSP_SM_ATTENU_MASK          (0xF<<20)           
#define FM_DSP_SM_ATTENU_SHIFT         (20)                

#define FM_DSP_SM_RATE(n)              (((n)&0x3)<<24)     
#define FM_DSP_SM_RATE_MASK            (0x3<<24)           
#define FM_DSP_SM_RATE_SHIFT           (24)                

#define FM_DSP_SOFTMUTE_TH4(n)         (((n)&0x3F)<<26)    
#define FM_DSP_SOFTMUTE_TH4_MASK       (0x3F<<26)          
#define FM_DSP_SOFTMUTE_TH4_SHIFT      (26)                


#define FM_DSP_INTERVAL_REG(n)         (((n)&0xFFFF)<<0)   
#define FM_DSP_INTERVAL_REG_MASK       (0xFFFF<<0)         
#define FM_DSP_INTERVAL_REG_SHIFT      (0)                 

#define FM_DSP_STEP19K_REG(n)          (((n)&0x7FFF)<<16)  
#define FM_DSP_STEP19K_REG_MASK        (0x7FFF<<16)        
#define FM_DSP_STEP19K_REG_SHIFT       (16)                


#define FM_DSP_NOTCH_FLT_Z0_I(n)       (((n)&0xFFFFF)<<0)  
#define FM_DSP_NOTCH_FLT_Z0_I_MASK     (0xFFFFF<<0)        
#define FM_DSP_NOTCH_FLT_Z0_I_SHIFT    (0)                 

#define FM_DSP_NOTCH_FLT_KA(n)         (((n)&0x3)<<20)     
#define FM_DSP_NOTCH_FLT_KA_MASK       (0x3<<20)           
#define FM_DSP_NOTCH_FLT_KA_SHIFT      (20)                

#define FM_DSP_NOTCH_FLT_BYPASS        (1<<22)             


#define FM_DSP_NOTCH_FLT_Z0_Q(n)       (((n)&0xFFFFF)<<0)  
#define FM_DSP_NOTCH_FLT_Z0_Q_MASK     (0xFFFFF<<0)        
#define FM_DSP_NOTCH_FLT_Z0_Q_SHIFT    (0)                 


#define FM_DSP_CT_U_PILOT(n)           (((n)&0x3F)<<0)     
#define FM_DSP_CT_U_PILOT_MASK         (0x3F<<0)           
#define FM_DSP_CT_U_PILOT_SHIFT        (0)                 

#define FM_DSP_CT_U_RSSI(n)            (((n)&0x3F)<<6)     
#define FM_DSP_CT_U_RSSI_MASK          (0x3F<<6)           
#define FM_DSP_CT_U_RSSI_SHIFT         (6)                 

#define FM_DSP_CT_U_SIGNAL(n)          (((n)&0x3F)<<12)    
#define FM_DSP_CT_U_SIGNAL_MASK        (0x3F<<12)          
#define FM_DSP_CT_U_SIGNAL_SHIFT       (12)                

#define FM_DSP_CT_U_NOISE(n)           (((n)&0x3F)<<18)    
#define FM_DSP_CT_U_NOISE_MASK         (0x3F<<18)          
#define FM_DSP_CT_U_NOISE_SHIFT        (18)                


#define FM_DSP_OFFSET2(n)              (((n)&0xFF)<<0)     
#define FM_DSP_OFFSET2_MASK            (0xFF<<0)           
#define FM_DSP_OFFSET2_SHIFT           (0)                 

#define FM_DSP_SIGNAL_DB2(n)           (((n)&0x7F)<<8)     
#define FM_DSP_SIGNAL_DB2_MASK         (0x7F<<8)           
#define FM_DSP_SIGNAL_DB2_SHIFT        (8)                 

#define FM_DSP_SNR_OUT(n)              (((n)&0x3F)<<15)    
#define FM_DSP_SNR_OUT_MASK            (0x3F<<15)          
#define FM_DSP_SNR_OUT_SHIFT           (15)                

#define FM_DSP_RSSI_DB2(n)             (((n)&0x7F)<<21)    
#define FM_DSP_RSSI_DB2_MASK           (0x7F<<21)          
#define FM_DSP_RSSI_DB2_SHIFT          (21)                

#define FM_DSP_SEEK_DONE               (1<<28)             

#define FM_DSP_SEEK_READY              (1<<29)             


#define FM_DSP_SNR_CNT(n)              (((n)&0xF)<<0)      
#define FM_DSP_SNR_CNT_MASK            (0xF<<0)            
#define FM_DSP_SNR_CNT_SHIFT           (0)                 

#define FM_DSP_SK_CMP_GRP_FLAG(n)      (((n)&0x1F)<<4)     
#define FM_DSP_SK_CMP_GRP_FLAG_MASK    (0x1F<<4)           
#define FM_DSP_SK_CMP_GRP_FLAG_SHIFT   (4)                 

#define FM_DSP_PILOT_DB2(n)            (((n)&0x7F)<<9)     
#define FM_DSP_PILOT_DB2_MASK          (0x7F<<9)           
#define FM_DSP_PILOT_DB2_SHIFT         (9)                 

#define FM_DSP_SNR19K_OUT(n)           (((n)&0x3F)<<16)    
#define FM_DSP_SNR19K_OUT_MASK         (0x3F<<16)          
#define FM_DSP_SNR19K_OUT_SHIFT        (16)                

#define FM_DSP_NOISE_DB2(n)            (((n)&0x7F)<<22)    
#define FM_DSP_NOISE_DB2_MASK          (0x7F<<22)          
#define FM_DSP_NOISE_DB2_SHIFT         (22)                


#define FM_DSP_OFFSET_CNT(n)           (((n)&0xFF)<<0)     
#define FM_DSP_OFFSET_CNT_MASK         (0xFF<<0)           
#define FM_DSP_OFFSET_CNT_SHIFT        (0)                 

#define FM_DSP_PILOT_CNT(n)            (((n)&0xFF)<<8)     
#define FM_DSP_PILOT_CNT_MASK          (0xFF<<8)           
#define FM_DSP_PILOT_CNT_SHIFT         (8)                 

#define FM_DSP_NOISE_CNT(n)            (((n)&0xFF)<<16)    
#define FM_DSP_NOISE_CNT_MASK          (0xFF<<16)          
#define FM_DSP_NOISE_CNT_SHIFT         (16)                

#define FM_DSP_RSSI_CNT(n)             (((n)&0xFF)<<24)    
#define FM_DSP_RSSI_CNT_MASK           (0xFF<<24)          
#define FM_DSP_RSSI_CNT_SHIFT          (24)                


#define FM_DSP_AGC_INDEX(n)            (((n)&0xF)<<0)      
#define FM_DSP_AGC_INDEX_MASK          (0xF<<0)            
#define FM_DSP_AGC_INDEX_SHIFT         (0)                 

#define FM_DSP_DIG_GAINCT(n)           (((n)&0xFF)<<4)     
#define FM_DSP_DIG_GAINCT_MASK         (0xFF<<4)           
#define FM_DSP_DIG_GAINCT_SHIFT        (4)                 

#define FM_DSP_FMDEM_SEL               (1<<12)             

#define FM_DSP_FIRCUT_BW(n)            (((n)&0x7)<<13)     
#define FM_DSP_FIRCUT_BW_MASK          (0x7<<13)           
#define FM_DSP_FIRCUT_BW_SHIFT         (13)                

#define FM_DSP_CT_HCC(n)               (((n)&0x7)<<16)     
#define FM_DSP_CT_HCC_MASK             (0x7<<16)           
#define FM_DSP_CT_HCC_SHIFT            (16)                


#define FM_DSP_RSVD_OUT(n)             (((n)&0xFFFF)<<0)   
#define FM_DSP_RSVD_OUT_MASK           (0xFFFF<<0)         
#define FM_DSP_RSVD_OUT_SHIFT          (0)                 

#define FM_DSP_RSVD_IN(n)              (((n)&0xFFFF)<<16)  
#define FM_DSP_RSVD_IN_MASK            (0xFFFF<<16)        
#define FM_DSP_RSVD_IN_SHIFT           (16)                


#endif



