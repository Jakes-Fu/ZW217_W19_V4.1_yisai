/******************************************************************************
 ** File Name:      sprd_codec_phy.h                                          *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec control                                             *
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

#ifndef __SPRD_CODEC_PHY_H___
#define __SPRD_CODEC_PHY_H___
#include "chip_plf_export.h"
#include "sprd_codec_ap_phy.h"
#include "sprd_codec_dp_phy.h"


PUBLIC uint32   __sprd_codec_init(void);
PUBLIC void     __sprd_codec_en(uint32 en);
PUBLIC void     __sprd_codec_switch_to(uint32 arm);
PUBLIC void     __sprd_codec_clear(uint32 path);
PUBLIC void     __sprd_codec_dac_en(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_adc_en(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_open(void);
PUBLIC void     __sprd_codec_reset(void);
PUBLIC void     __sprd_codec_mclk(uint32 en);
PUBLIC void     __sprd_codec_register(uint32 en);
PUBLIC void     __sprd_codec_hp_mute(uint32 is_mute);
PUBLIC void     __sprd_codec_ep_mute(uint32 is_mute);
PUBLIC void     __sprd_codec_pa_mute(uint32 is_mute);
PUBLIC void     __sprd_codec_ext_pa_ctl(uint32 en,uint32 id);
PUBLIC uint32   __sprd_codec_is_use_ext_hp(void);
PUBLIC uint32   __sprd_codec_get_ext_hp_delay_time(void);
PUBLIC void     __sprd_codec_ext_hp_pa_ctl(uint32 en);
PUBLIC void     __sprd_codec_ext_hp_pa_ctl_inter(uint32 en);
PUBLIC uint32   __sprd_codec_is_arm_ctl(void);
PUBLIC uint32   __sprd_codec_update_ldo_vol(uint32 cur_vol);
PUBLIC void     __run_callback(uint32 is_open);
PUBLIC void     __sprd_codec_update_vcm(void);
PUBLIC void     __sprd_codec_update_ldo(void);
PUBLIC void     __sprd_codec_fs_setting(void);
PUBLIC uint32   __sprd_codec_ap_mic_bias_ldo(uint32 en,uint32 ldo_val);
PUBLIC void     __sprd_codec_ext_hp_pa_ctl_with_mute(uint32 en);

PUBLIC void     CODEC_SetHeadphoneConnectStatus(BOOLEAN status);

#include "audio_hal.h"

#endif //__SPRD_CODEC_PHY_H___
