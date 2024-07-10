/******************************************************************************
 ** File Name:      sprd_codec_ap_phy.h                                       *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec ap control                                          *
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

#ifndef __SPRD_CODEC_AP_PHY_H___
#define __SPRD_CODEC_AP_PHY_H___
#include "chip_plf_export.h"

typedef struct
{
    uint                    adcpga:4;
    uint                    pa:4;
    uint                    ear:4;
    uint                    hp:4;
    uint                    RSV:16;
} SPRD_CODEC_PGA_T;


#if defined (SPRD_AUDIO_USE_V1)
#define HP_OCP_IRQ_MSK          (BIT_0)
#define EAR_OCP_IRQ_MSK         (BIT_1)
#define LOL_OCP_IRQ_MSK         (BIT_2)
#define LOR_OCP_IRQ_MSK         (BIT_3)
#define PA_OCP_IRQ_MSK          (BIT_4)
#define OTP_IRQ_MSK             (BIT_5)
#define OVP_IRQ_MSK             (BIT_6)
#define AUDIO_POP_IRQ_MSK       (BIT_7)
#else
#define HP_OCP_IRQ_MSK          (BIT_0)
#define EAR_OCP_IRQ_MSK         (BIT_1)
#define LO_OCP_IRQ_MSK          (BIT_2)
#define PA_OCP_IRQ_MSK          (BIT_3)
#define OTP_IRQ_MSK             (BIT_4)
#define AUDIO_POP_IRQ_MSK       (BIT_5)
#define OVP_IRQ_MSK             (BIT_6)
#endif



#define MICBIAS_V_21            (0)
#define MICBIAS_V_19            (1)
#define MICBIAS_V_23            (2)
#define MICBIAS_V_25            (3)

#define AUD_BG_I_150            (0)
#define AUD_BG_I_200            (1)
#define AUD_BG_I_250            (2)
#define AUD_BG_I_300            (3)

#define AUD_VCM_V_165           (0)
#define AUD_VCM_V_17            (1)
#define AUD_VCM_V_16            (2)
#define AUD_VCM_V_145           (3)



#define PA_LDO_V_29             (0)
#define PA_LDO_V_30             (1)
#define PA_LDO_V_31             (2)
#define PA_LDO_V_32             (3)
#define PA_LDO_V_33             (4)
#define PA_LDO_V_34             (5)
#define PA_LDO_V_35             (6)
#define PA_LDO_V_36             (7)

#define AUD_OTP_T_104_74        (0)
#define AUD_OTP_T_128_98        (1)
#define AUD_OTP_T_152_122       (2)
#define AUD_OTP_T_175_144       (3)
#define AUD_OTP_T_6_N24         (4)
#define AUD_OTP_T_30_0          (5)
#define AUD_OTP_T_55_25         (6)
#define AUD_OTP_T_79_49         (7)
///..

#define AUD_OVP_T_16            (0)
#define AUD_OVP_T_8             (1)
#define AUD_OVP_T_4             (2)
#define AUD_OVP_T_2             (3)
#define AUD_OVP_T_1             (4)
#define AUD_OVP_T_05            (5)
#define AUD_OVP_T_025           (6)
#define AUD_OVP_T_012           (7)

#define AUD_OVP_V_542_518       (0)
#define AUD_OVP_V_527_503       (1)
#define AUD_OVP_V_512_488       (2)
#define AUD_OVP_V_496_473       (3)
#define AUD_OVP_V_481_459       (4)
#define AUD_OVP_V_465_444       (5)
#define AUD_OVP_V_450_429       (6)
#define AUD_OVP_V_435_415       (7)


#define ADC_PGA_G_N6            (0)
#define ADC_PGA_G_N3            (1)
#define ADC_PGA_G_0             (2)
#define ADC_PGA_G_P3            (3)
#define ADC_PGA_G_P6            (4)
#define ADC_PGA_G_P9            (5)
#define ADC_PGA_G_P12           (6)
#define ADC_PGA_G_P15           (7)
#define ADC_PGA_G_P18           (8)
#define ADC_PGA_G_P21           (9)
#define ADC_PGA_G_P24           (10)
#define ADC_PGA_G_P27           (11)
#define ADC_PGA_G_P30           (12)
#define ADC_PGA_G_P33           (13)
#define ADC_PGA_G_P36           (14)
#define ADC_PGA_G_P39           (15)

#define PA_DTRI_F_300           (0)
#define PA_DTRI_F_590           (1)
#define PA_DTRI_F_870           (2)
#define PA_DTRI_F_1000          (3)

#define PA_SS_T_1               (0)
#define PA_SS_T_1_2             (1)
#define PA_SS_T_1_4             (2)
#define PA_SS_T_1_8             (3)
#define PA_SS_T_1_16            (4)
#define PA_SS_T_1_32            (5)
#define PA_SS_T_1_64            (6)
#define PA_SS_T_1_128           (7)

#define PA_SS_F_3_2             (0)
#define PA_SS_F_9               (1)
#define PA_SS_F_22              (2)
#define PA_SS_F_47              (3)

#define DAC_HP_PGA_G_MUTE       (0)
#define DAC_HP_PGA_G_N33        (1)
#define DAC_HP_PGA_G_N30        (2)
#define DAC_HP_PGA_G_N27        (3)
#define DAC_HP_PGA_G_N24        (4)
#define DAC_HP_PGA_G_N21        (5)
#define DAC_HP_PGA_G_N18        (6)
#define DAC_HP_PGA_G_N15        (7)
#define DAC_HP_PGA_G_N12        (8)
#define DAC_HP_PGA_G_N9         (9)
#define DAC_HP_PGA_G_N6         (10)
#define DAC_HP_PGA_G_N3         (11)
#define DAC_HP_PGA_G_0          (12)
#define DAC_HP_PGA_G_P3         (13)
#define DAC_HP_PGA_G_P6         (14)
#define DAC_HP_PGA_G_P9         (15)

#define DAC_EAR_PGA_G_MUTE      (0)
#define DAC_EAR_PGA_G_N33       (1)
#define DAC_EAR_PGA_G_N30       (2)
#define DAC_EAR_PGA_G_N27       (3)
#define DAC_EAR_PGA_G_N24       (4)
#define DAC_EAR_PGA_G_N21       (5)
#define DAC_EAR_PGA_G_N18       (6)
#define DAC_EAR_PGA_G_N15       (7)
#define DAC_EAR_PGA_G_N12       (8)
#define DAC_EAR_PGA_G_N9        (9)
#define DAC_EAR_PGA_G_N6        (10)
#define DAC_EAR_PGA_G_N3        (11)
#define DAC_EAR_PGA_G_0         (12)
#define DAC_EAR_PGA_G_P3        (13)
#define DAC_EAR_PGA_G_P6        (14)
#define DAC_EAR_PGA_G_P9        (15)

#define DAC_PA_PGA_G_MUTE       (0)
#define DAC_PA_PGA_G_N21        (1)
#define DAC_PA_PGA_G_N18        (2)
#define DAC_PA_PGA_G_N15        (3)
#define DAC_PA_PGA_G_N12        (4)
#define DAC_PA_PGA_G_N9         (5)
#define DAC_PA_PGA_G_N6         (6)
#define DAC_PA_PGA_G_N3         (7)
#define DAC_PA_PGA_G_0          (8)
#define DAC_PA_PGA_G_P3         (9)
#define DAC_PA_PGA_G_P6         (10)
#define DAC_PA_PGA_G_P9         (11)
#define DAC_PA_PGA_G_P12        (12)
#define DAC_PA_PGA_G_P15        (13)
#define DAC_PA_PGA_G_P18        (14)
#define DAC_PA_PGA_G_P21        (15)

#define HP_POP_STEP_012         (0)
#define HP_POP_STEP_025         (1)
#define HP_POP_STEP_05          (2)
#define HP_POP_STEP_1           (3)
#define HP_POP_STEP_2           (4)
#define HP_POP_STEP_4           (5)
#define HP_POP_STEP_8           (6)
#define HP_POP_STEP_16          (7)

#define HP_POP_CTL_DIS          (0)
#define HP_POP_CTL_UP           (1)
#define HP_POP_CTL_DOWN         (2)
#define HP_POP_CTL_HOLD         (3)

#define HP_POP_CHG_V_05         (0)
#define HP_POP_CHG_V_1          (1)
#define HP_POP_CHG_V_2          (2)
#define HP_POP_CHG_V_4          (3)

#define HP_POP_FLG_CHGING       (0)
#define HP_POP_FLG_1_2          (1)
#define HP_POP_FLG_3_4          (2)
#define HP_POP_FLG_NEAR_CMP     (3)

#define ADC_PGA_G_MUTE_SET      ADC_PGA_G_N6
#define DAC_HP_PGA_G_MUTE_SET   DAC_HP_PGA_G_MUTE
#define DAC_EAR_PGA_G_MUTE_SET  DAC_EAR_PGA_G_MUTE
#define DAC_PA_PGA_G_MUTE_SET   DAC_PA_PGA_G_MUTE


PUBLIC void     __sprd_codec_ap_clear(uint32 path);
PUBLIC void     __sprd_codec_ap_dac_en(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_adc_en(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_vcm_en(uint32 en);
PUBLIC void     __sprd_codec_ap_vcm_buf_en(uint32 en);
PUBLIC void     __sprd_codec_ap_bg_en(uint32 en);
PUBLIC void     __sprd_codec_ap_vb_en(uint32 en);
PUBLIC void     __sprd_codec_ap_vbo_en(uint32 en);
PUBLIC void     __sprd_codec_ap_bg_ibais_en(uint32 en);
PUBLIC void     __sprd_codec_ap_en(uint32 en);
PUBLIC void     __sprd_codec_ap_adc_pwr(uint32 chan,uint32 en);
PUBLIC void     __sprd_codec_ap_adc_pga_pwr(uint32 chan,uint32 en);
PUBLIC void     __sprd_codec_ap_adc_pga_gain(uint32 chan,uint32 val);
PUBLIC void     __sprd_codec_ap_hp_pga_gain(uint32 chan,uint32 val);
PUBLIC void     __sprd_codec_ap_adc_mute(void);
PUBLIC void     __sprd_codec_ap_hp_mute(void);
PUBLIC void     __sprd_codec_ap_ear_pga_gain(uint32 val);
PUBLIC void     __sprd_codec_ap_ear_mute(void);
PUBLIC void     __sprd_codec_ap_pa_pga_gain(uint32 val);
PUBLIC void     __sprd_codec_ap_par_pga_gain(uint32 val);
PUBLIC void     __sprd_codec_ap_pa_mute(void);
PUBLIC void     __sprd_codec_ap_adc(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_linein_rec(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_dac_pwr(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_dac(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_lin(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_lout_mixer(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_lout_mixer_adcbuf(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_aol_en(uint32 en);
PUBLIC void     __sprd_codec_ap_aor_mixer(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_aor_mixer_adcbuf(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_aor_en(uint32 en);
PUBLIC void     __sprd_codec_ap_ear(uint32 en);
PUBLIC void     __sprd_codec_ap_lout(uint32 en);
PUBLIC void     __sprd_codec_ap_ear_en(uint32 en);
PUBLIC void     __sprd_codec_ap_hp_en(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_hp_mixer(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_hp_mixer_adcbuf(uint32 en,uint32 chan);
PUBLIC uint32   __sprd_codec_ap_hp_pop_en(uint32 en);
PUBLIC uint32   __sprd_codec_ap_hp_ramp_done(uint32 up);
PUBLIC void     __sprd_codec_ap_clr_ramp(uint32 up);
PUBLIC void     __sprd_codec_ap_hp(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_mic_bias(uint32 en);
PUBLIC void     __sprd_codec_ap_main_mic(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_aux_mic(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_hp_mic(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_drv_clk_en(uint32 en);
PUBLIC void     __sprd_codec_ap_rev_clk_en(uint32 en);
PUBLIC void     __sprd_codec_ap_dac_clk_en(uint32 en);
PUBLIC void     __sprd_codec_ap_adc_clk_pwr(uint32 en);
PUBLIC void     __sprd_codec_ap_adc_clk_rst(void);
PUBLIC void     __sprd_codec_ap_hp_pop(uint32 ctl);
PUBLIC void     __sprd_codec_ap_hp_pop_step(uint32 step);
PUBLIC void     __sprd_codec_ap_lin_adc(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_adc_pga_buf_pwr(uint32 en,uint32 chan);
PUBLIC void     __sprd_codec_ap_ear_mixer(uint32 en);
PUBLIC void     __sprd_codec_ap_pa_sw_en(uint32 en);
PUBLIC void     __sprd_codec_ap_pa_en(uint32 en);
PUBLIC void     __sprd_codec_ap_pa_ldo_en(uint32 en);
PUBLIC void     __sprd_codec_ap_pa_ldo_v_sel(uint32 v_sel);
PUBLIC void     __sprd_codec_ap_pa_d_en(uint32 en);
PUBLIC void     __sprd_codec_ap_pa_dtri_f_sel(uint32 freq);
PUBLIC void     __sprd_codec_ap_pa_demi_en(uint32 en);
PUBLIC void     __sprd_codec_ap_pa_vcm_v_sel(uint32 v_sel);
PUBLIC uint32   __sprd_codec_update_vb_vbo_vcm_vol(uint32 cur_vol);
PUBLIC uint32   __sprd_codec_update_pa_ldo_vol(uint32 cur_vol);
PUBLIC void     __sprd_codec_ap_int_set(uint32 en,uint32 mask);
PUBLIC void     __sprd_codec_ap_int_clr(uint32 mask);
PUBLIC uint32   __sprd_codec_ap_int_raw(void);
PUBLIC uint32   __sprd_codec_ap_int_mask(void);
#if (MIC_BAIS_LDO_USE_FOR_EXT)
PUBLIC uint32   __sprd_codec_ap_is_ldo_mic_bias(void);
#else
#define __sprd_codec_ap_is_ldo_mic_bias() (0)
#endif
PUBLIC void     __sprd_codec_ap_mic_bias_ldo_v_sel(uint32 v_sel);
PUBLIC void     __sprd_codec_ap_mic_bias_ldo_set(uint32 en);



PUBLIC void     __sprd_codec_ap_reg_print(void);
#if (PGA_SMOOTH_ADJUST)
PUBLIC void     __sprd_codec_pga_adj_create_task(void);
PUBLIC void     __sprd_codec_pga_adj_setting(void);
PUBLIC void     __sprd_codec_ap_adc_pga_gain_set(uint32 chan,uint32 val);
PUBLIC void     __sprd_codec_ap_hp_pga_gain_set(uint32 chan,uint32 val);
PUBLIC void     __sprd_codec_ap_ear_pga_gain_set(uint32 val);
PUBLIC void     __sprd_codec_ap_pa_pga_gain_set(uint32 chan,uint32 val);
PUBLIC uint32   __sprd_codec_ap_adc_pga_gain_get(uint32 chan);
PUBLIC uint32   __sprd_codec_ap_hp_pga_gain_get(uint32 chan);
PUBLIC uint32   __sprd_codec_ap_ear_pga_gain_get(void);
PUBLIC uint32   __sprd_codec_ap_pa_pga_gain_get(uint32 chan);
PUBLIC SPRD_CODEC_PGA_T* __sprd_codec_ap_pga_setting(uint32 chan);
PUBLIC uint32   __sprd_codec_ap_adc_pga_gain_is_mute(void);
PUBLIC uint32   __sprd_codec_ap_hp_pga_gain_is_mute(void);
PUBLIC uint32   __sprd_codec_ap_ep_pga_gain_is_mute(void);
PUBLIC uint32   __sprd_codec_ap_pa_pga_gain_is_mute(void);
PUBLIC void     __sprd_codec_ap_adc_pga_gain_clr_mute(void);
PUBLIC void     __sprd_codec_ap_hp_pga_gain_clr_mute(void);
PUBLIC void     __sprd_codec_ap_ep_pga_gain_clr_mute(void);
PUBLIC void     __sprd_codec_ap_pa_pga_gain_clr_mute(void);
PUBLIC void     __sprd_codec_pga_adj_get_mutex(void);
PUBLIC void     __sprd_codec_pga_adj_put_mutex(void);
#endif
PUBLIC void     __sprd_codec_ap_pa_ovp_ldo_en(uint32 en);
PUBLIC void     __sprd_codec_ap_pa_ovp_en(uint32 en);
PUBLIC void     __sprd_codec_ap_pa_ovp_v_sel(uint32 v_sel);


// FOR CHAN MASK
#define SPRD_CODEC_RJ217_SHIFT  4
#define SPRD_CODEC_RJ217_MASK   1


#endif //__SPRD_CODEC_AP_PHY_H___
