/******************************************************************************
 ** File Name:      sprd_codec_dp_phy_v0.h                                    *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec dp control                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifndef __SPRD_CODEC_DP_PHY_V0_H___
#define __SPRD_CODEC_DP_PHY_V0_H___
#include "Chip_plf_export.h"



//top_ctl
#define DAC_EN_L                (BIT_0)
#define ADC_EN_L                (BIT_1)
#define DAC_EN_R                (BIT_2)
#define ADC_EN_R                (BIT_3)

#define DAC_I2S_SEL_SHIFT       (4)
#define DAC_I2S_SEL_MASK        (0x3<<DAC_I2S_SEL_SHIFT)

#define ADC_I2S_SEL_SHIFT       (6)
#define ADC_I2S_SEL_MASK        (0x3<<ADC_I2S_SEL_SHIFT)

#define ADC_SDM_SEL_SHIFT       (8)
#define ADC_SDM_SEL_MASK        (0x3<<ADC_SDM_SEL_SHIFT)

//aud_clr
#define DAC_PATH_CLR            (BIT_0)
#define ADC_PATH_CLR            (BIT_1)

//i2s_ctl
#define I2S_CLK_DIV_2           (BIT_0)
#define DAC_LR_HFORL            (BIT_1)
#define ADC_LR_HFORL            (BIT_2)

#define DAC_I2S_IO_MODE_SHIFT   (3)
#define DAC_I2S_IO_MODE_MASK    (0x3<<DAC_I2S_IO_MODE_SHIFT)


#define ADC_I2S_IO_MODE_SHIFT   (5)
#define ADC_I2S_IO_MODE_MASK    (0x3<<ADC_I2S_IO_MODE_SHIFT)

#define DAC_I2S_IOWL_SHIFT      (7)
#define DAC_I2S_IOWL_MASK       (0x3<<DAC_I2S_IOWL_SHIFT)


#define ADC_I2S_IOWL_SHIFT      (9)
#define ADC_I2S_IOWL_MASK       (0x3<<ADC_I2S_IOWL_SHIFT)

//dac_ctl
#define DAC_FS_MODE_SHIFT       (0)
#define DAC_FS_MODE_MASK        (0xF<<DAC_FS_MODE_SHIFT)

#define DAC_MUTE_DIV0_SHIFT     (4)
#define DAC_MUTE_DIV0_MASK      (0x3F<<DAC_MUTE_DIV0_SHIFT)

#define DAC_MUTE_DIV1_SHIFT     (10)
#define DAC_MUTE_DIV1_MASK      (0xF<<DAC_MUTE_DIV1_SHIFT)

#define DAC_MUTE_START          (BIT_14)
#define DAC_MUTE_EN             (BIT_15)

//sdm_ctl0
#define DAC_SDM_DI_SHIFT        (0)
#define DAC_SDM_DI_MASK         (0x3<<DAC_SDM_DI_SHIFT)

#define DAC_SDM_DO_SHIFT        (2)
#define DAC_SDM_DO_MASK         (0x3<<DAC_SDM_DO_SHIFT)

#define DAC_SDM_DILVL_SHIFT     (4)
#define DAC_SDM_DILVL_MASK      (0xF<<DAC_SDM_DILVL_SHIFT)

#define DAC_SDM_DOLVL_SHIFT     (8)
#define DAC_SDM_DOLVL_MASK      (0xF<<DAC_SDM_DOLVL_SHIFT)

//sdm_ctl1
#define DAC_SDM_TEST_SHIFT      (0)
#define DAC_SDM_TEST_MASK       (0xFF<<DAC_SDM_TEST_SHIFT)


//adc_ctl
#define ADC_SRC_N_SHIFT         (0)
#define ADC_SRC_N_MASK          (0xF<<ADC_SRC_N_SHIFT)

//loop_ctl
#define AUD_LOOP_TEST           (BIT_0)
#define LOOP_R_PATH_2R          (BIT_1)
#define LOOP_L_PATH_2R          (BIT_2)

#define LOOP_FIFO_AF_LVL_SHIFT  (3)
#define LOOP_FIFO_AF_LVL_MASK   (0x7<<LOOP_FIFO_AF_LVL_SHIFT)

#define LOOP_FIFO_AE_LVL_SHIFT  (6)
#define LOOP_FIFO_AE_LVL_MASK   (0x7<<LOOP_FIFO_AE_LVL_SHIFT)

//aud_sts0 RO
#define MUTE_ST_SHIFT           (0)
#define MUTE_ST_MASK            (0x3<<MUTE_ST_SHIFT)

#define MUTE_GAIN_D_DONE_INT_RAW    (BIT_2)
#define MUTE_GAIN_U_DONE_INT_RAW    (BIT_3)

#define MUTE_GAIN_D_DONE_INT_MASK   (BIT_4)
#define MUTE_GAIN_U_DONE_INT_MASK   (BIT_5)

//int_clr
#define MUTE_GAIN_D_DONE_INT_CLR    (BIT_0)
#define MUTE_GAIN_U_DONE_INT_CLR    (BIT_1)

//int_en
#define MUTE_GAIN_D_DONE_INT_EN     (BIT_0)
#define MUTE_GAIN_U_DONE_INT_EN     (BIT_1)



typedef struct{
    uint32  top_ctl;    //0000
    uint32  aud_clr;    //0004
    uint32  i2s_ctl;    //0008
    uint32  dac_ctl;    //000c
    uint32  sdm_ctl0;   //0010
    uint32  sdm_ctl1;   //0014
    uint32  adc_ctl;    //0018
    uint32  loop_ctl;   //001c
    uint32  aud_sts0;   //0020
    uint32  int_clr;    //0024
    uint32  int_en;     //0028
}SPRD_CODEC_DP_CTL_T;

#endif //__SPRD_CODEC_DP_PHY_V0_H___
