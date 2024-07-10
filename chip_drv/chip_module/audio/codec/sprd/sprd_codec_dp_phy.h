/******************************************************************************
 ** File Name:      sprd_codec_dp_phy.h                                       *
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

#ifndef __SPRD_CODEC_DP_PHY_H___
#define __SPRD_CODEC_DP_PHY_H___
#include "chip_plf_export.h"

#define DAC_I2S_SEL_PORT0       (0)
#define DAC_I2S_SEL_PORT1       (1)
#define DAC_I2S_SEL_PORT2       (2)
#define DAC_I2S_SEL_PORT3       (3)

#define ADC_I2S_SEL_PORT0       (0)
#define ADC_I2S_SEL_PORT1       (1)
#define ADC_I2S_SEL_PORT2       (2)
#define ADC_I2S_SEL_PORT3       (3)

#define ADC_SDM_SEL_ANA         (0)
#define ADC_SDM_SEL_LOOP        (1)
#define ADC_SDM_SEL_EXT         (2)

#define DAC_I2S_IO_MODE_I2S     (0)
#define DAC_I2S_IO_MODE_DSP     (1)
#define DAC_I2S_IO_MODE_PCM     (2)

#define ADC_I2S_IO_MODE_I2S     (0)
#define ADC_I2S_IO_MODE_DSP     (1)
#define ADC_I2S_IO_MODE_PCM     (2)

#define DAC_I2S_IOWL_16BIT      (0)
#define DAC_I2S_IOWL_18BIT      (1)
#define DAC_I2S_IOWL_20BIT      (2)
#define DAC_I2S_IOWL_24BIT      (3)

#define ADC_I2S_IOWL_16BIT      (0)
#define ADC_I2S_IOWL_18BIT      (1)
#define ADC_I2S_IOWL_20BIT      (2)
#define ADC_I2S_IOWL_24BIT      (3)

#define DAC_FS_MODE_96K         (0)
#define DAC_FS_MODE_48K         (1)
#define DAC_FS_MODE_44K         (2) //44.1K
#define DAC_FS_MODE_32K         (3)
#define DAC_FS_MODE_24K         (4)
#define DAC_FS_MODE_22K         (5) //22.05K
#define DAC_FS_MODE_16K         (6)
#define DAC_FS_MODE_12K         (7)
#define DAC_FS_MODE_11K         (8) //11.025K
#define DAC_FS_MODE_9K          (9) //9.6K
#define DAC_FS_MODE_8K          (10)

#define MUTE_IDLE               (0)
#define MUTE_GNDN               (1)
#define MUTE_WAIT               (2)
#define MUTE_GNUP               (3)


PUBLIC void     __sprd_codec_dp_adc_fs_mode_set(uint32 sample_rate);
PUBLIC uint32   __sprd_codec_dp_dac_fs_mode_calc(uint32 sample_rate,uint32 *fs_mode);
PUBLIC void     __sprd_codec_dp_en(uint32 en);
PUBLIC void     __sprd_codec_dp_clear(uint32 path);
PUBLIC void     __sprd_codec_dp_dac_en(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_dp_adc_en(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_dp_i2s_sel(uint32 path,uint32 port);
PUBLIC void     __sprd_codec_dp_adc_sdm_in_sel(uint32 port);
PUBLIC void     __sprd_codec_dp_i2s_clkdiv_set(uint32 div);
PUBLIC void     __sprd_codec_dp_i2s_lr_sel(uint32 path,uint32 hforl);
PUBLIC void     __sprd_codec_dp_i2s_io_mode(uint32 path,uint32 mode);
PUBLIC void     __sprd_codec_dp_i2s_iowl(uint32 path,uint32 iowl);
PUBLIC void     __sprd_codec_dp_dac_mute_div0(uint32 cnt);
PUBLIC void     __sprd_codec_dp_dac_mute_div1(uint32 cnt);
PUBLIC void     __sprd_codec_dp_dac_mute_en(uint32 en);
PUBLIC uint32   __sprd_codec_dp_dac_mute_sts(void);
PUBLIC uint32   __sprd_codec_dp_dac_mute(uint32 mute);
PUBLIC uint32   __sprd_codec_dp_gain_done(uint32 up);
PUBLIC void     __sprd_codec_dp_clr_gain(uint32 up);
PUBLIC void     __sprd_codec_dp_sdm_set(void);
PUBLIC void     __sprd_codec_dp_sdm_mapping_set(uint32 en);
PUBLIC uint32   __sprd_codec_dp_int_sts_d_raw(void);
PUBLIC uint32   __sprd_codec_dp_int_sts_d(void);
PUBLIC uint32   __sprd_codec_dp_int_sts_u_raw(void);
PUBLIC uint32   __sprd_codec_dp_int_sts_u(void);
PUBLIC void     __sprd_codec_dp_int_clr_d(void);
PUBLIC void     __sprd_codec_dp_int_clr_u(void);
PUBLIC void     __sprd_codec_dp_int_en_d(uint32 en);
PUBLIC void     __sprd_codec_dp_int_en_u(uint32 en);
PUBLIC void     __sprd_codec_dp_loop_en(uint32 en);
PUBLIC void     __sprd_codec_dp_dac_fs_mode_set(uint32 sample_rate);
PUBLIC void     __sprd_codec_dp_reg_print(void);

#endif //__SPRD_CODEC_DP_PHY_H___
