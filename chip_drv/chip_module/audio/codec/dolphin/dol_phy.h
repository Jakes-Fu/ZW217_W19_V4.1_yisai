/******************************************************************************
 ** File Name:      dol_phy.h                                                 *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Rigister definition for physical layer.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                   *
 *****************************************************************************/
#ifndef _DOL_PHY_H_
#define _DOL_PHY_H_

#include "audio_hal.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC uint32 __dol_calc_fs(uint32 fs);
PUBLIC void   __dol_s2mono(BOOLEAN is_mono);
PUBLIC void   __dol_mic_sel(uint8 mic_sel);
PUBLIC void   __dol_init (void);
PUBLIC void   __dol_mclk(uint32 en);
PUBLIC void   __dol_open (void);
PUBLIC void   __dol_reset(void);

PUBLIC void   __dol_sb(uint32 is_en);
PUBLIC void   __dol_sb_sleep(uint32 is_en);

PUBLIC void   __dol_sb_adc(uint32 is_en);
PUBLIC void   __dol_linein_rec(uint32 is_en);

PUBLIC void   __dol_sb_dac(uint32 is_en);
PUBLIC void   __dol_sb_lin(uint32 is_en);
PUBLIC void   __dol_sb_mix(uint32 is_en);
PUBLIC void   __dol_sb_lout(uint32 is_en);
PUBLIC uint32 __dol_sb_out(uint32 is_en);
PUBLIC void   __dol_sb_blt(uint32 is_en);

PUBLIC uint32 __dol_dac_mute(uint32 is_mute);
PUBLIC void   __dol_hp_mute(uint32 is_mute);
PUBLIC void   __dol_ep_mute(uint32 is_mute);

PUBLIC uint32 __dol_ramp_done(uint32 up);
PUBLIC void   __dol_clr_ramp(uint32 up);
PUBLIC uint32 __dol_gain_done(uint32 up);
PUBLIC void   __dol_clr_gain(uint32 up);

PUBLIC void   __dol_set_da_samplerate (uint32 sample_rate);
PUBLIC void   __dol_set_ad_samplerate (uint32 sample_rate);

PUBLIC void   __dol_set_pag_gim(uint32 val);
PUBLIC void   __dol_set_pag_gi(uint32 val);
PUBLIC void   __dol_set_pag_golr(uint32 val);
PUBLIC void   __dol_set_pag_gobtl(uint32 val);
PUBLIC void   __dol_set_pag_lrgod(uint32 uiChannel,uint32 lval,uint32 rval);
PUBLIC void   __dol_set_pag_lrgo(uint32 uiChannel,uint32 lval,uint32 rval);
PUBLIC void   __dol_set_pag_lrgob(uint32 uiChannel,uint32 lval,uint32 rval);

PUBLIC void   __dol_reg_dummy(void);
PUBLIC uint32 __dol_reg_rw_test(void);
PUBLIC void   __dol_pga_bug(void);
PUBLIC void   __dol_reg_print(void);
PUBLIC void   __dol_exit_from_dsp(void);
PUBLIC void   __dol_ext_pa_ctl(uint32 en);
PUBLIC uint32 __dol_is_use_ext_hp(void);
PUBLIC uint32 __dol_get_ext_hp_delay_time(void);
PUBLIC void   __dol_ext_hp_pa_ctl_inter(uint32 en);
PUBLIC void   __dol_ext_hp_pa_ctl(uint32 en);
PUBLIC uint32 __dol_is_arm_ctl(void);
PUBLIC uint32 __dol_update_ldo_vol(uint32 cur_vol);
PUBLIC void   __dol_switch_to(uint32 arm);
PUBLIC void   __dol_fs_setting(void);
PUBLIC void   __run_callback(uint32 is_open);
PUBLIC void   __dol_ext_hp_pa_ctl_with_mute(uint32 en);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  //_DOL_PHY_H_


