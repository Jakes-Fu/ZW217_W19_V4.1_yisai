/******************************************************************************
 ** File Name:      sprd_codec_ap_phy_v0.c                                    *
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
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"

#include "chip_plf_export.h"

#include "audio_drvapi.h"
#include "../sprd_codec_phy.h"
#include "sprd_codec_ap_phy_v0.h"
#include "ldo_drvapi.h"

/* somo customer use ear&spk device, use the option
    for reduce TDD NOISE.
*/
#define EAR_NEED_PA_SW 0

#ifdef CODEC_DEBUG
#define SPRD_CODEC_AP_PRINT    SCI_TRACE_LOW
#else
#define SPRD_CODEC_AP_PRINT(...)
#endif

#if (PGA_SMOOTH_ADJUST)
typedef struct
{
    SPRD_CODEC_PGA_T        pga_setting_0;
    SPRD_CODEC_PGA_T        pga_setting_1;
    SPRD_CODEC_PGA_T        is_mute;
}SPRD_CODEC_PGA_SETTING_T;
LOCAL SPRD_CODEC_PGA_SETTING_T s_sprd_codec_pga_setting;
#endif

typedef struct
{
    uint32 cur_vol;
    uint32 ldo_level;
} SPRD_CODEC_LDO_VOL_MAP_T;



LOCAL SPRD_CODEC_AP_REG_CTL_T * audif_ptr = (SPRD_CODEC_AP_REG_CTL_T*)SPRD_CODEC_AP_REG_BASE;

LOCAL SPRD_CODEC_LDO_VOL_MAP_T _vb_vbo_vcm_vol_map_tab[] =
{
    {3650,  AUD_VCM_V_145   },
    {3750,  AUD_VCM_V_16    },
    {3850,  AUD_VCM_V_165   },
    {4300,  AUD_VCM_V_17    },
};

LOCAL SPRD_CODEC_LDO_VOL_MAP_T _pa_ldo_vol_map_tab[] =
{
    {3500,  PA_LDO_V_29  },
    {3600,  PA_LDO_V_30  },
    {3700,  PA_LDO_V_31  },
    {3800,  PA_LDO_V_32  },
    {3900,  PA_LDO_V_33  },
    {4000,  PA_LDO_V_34  },
    {4100,  PA_LDO_V_35  },
    {4300,  PA_LDO_V_36  },
};

LOCAL void _sprd_codec_ap_en_op(uint32 en,void* reg,uint32 bit)
{
    //SPRD_CODEC_AP_PRINT("_sprd_codec_ap_en_op 0x%x 0x%x",reg,bit);
    if( en )
    {
        SPRD_CODEC_AP_WRITE_BITS((uint32)reg,(bit),(bit));
    }
    else
    {
        SPRD_CODEC_AP_WRITE_BITS((uint32)reg,~(bit),(bit));
    }
}

LOCAL void _sprd_codec_ap_chan_en(uint32 en,uint32 chan,void* reg,uint32 l_val,uint32 r_val)
{
    if( en )
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            SPRD_CODEC_AP_WRITE_BITS((uint32)reg,(l_val),(l_val));
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            SPRD_CODEC_AP_WRITE_BITS((uint32)reg,(r_val),(r_val));
        }
    }
    else
    {
        if( AUDIO_IS_CHAN_L(chan) )
        {
            SPRD_CODEC_AP_WRITE_BITS((uint32)reg,~(l_val),(l_val));
        }
        if( AUDIO_IS_CHAN_R(chan) )
        {
            SPRD_CODEC_AP_WRITE_BITS((uint32)reg,~(r_val),(r_val));
        }
    }
}

LOCAL void _sprd_codec_ap_path_op(uint32 path,void* reg,uint32 en,
    uint32 adc_bit,uint32 dac_bit)
{
    switch(path)
    {
        case AUDIO_PLAYBACK:
        {
            _sprd_codec_ap_en_op(en,reg,adc_bit);
        }break;
        case AUDIO_CAPTRUE:
        {
            _sprd_codec_ap_en_op(en,reg,dac_bit);
        }break;
        default:break;
    }
}

LOCAL void _sprd_codec_ap_path_mask(uint32 path,void* reg,uint32 val
    ,uint32 adc_mask,uint32 adc_shif
    ,uint32 dac_mask,uint32 dac_shif
)
{
    switch(path)
    {
        case AUDIO_PLAYBACK:
        {
            SPRD_CODEC_AP_WRITE_BITS(reg,(val<<dac_shif),dac_mask);
        }break;
        case AUDIO_CAPTRUE:
        {
            SPRD_CODEC_AP_WRITE_BITS(reg,(val<<adc_shif),adc_mask);
        }break;
        default:break;
    }
}

LOCAL void _sprd_codec_ap_chan_mask(uint32 chan,void* reg,uint32 val
    ,uint32 r_mask,uint32 r_shif
    ,uint32 l_mask,uint32 l_shif
)
{
    if( AUDIO_IS_CHAN_L(chan) )
    {
        SPRD_CODEC_AP_WRITE_BITS(reg,(val<<l_shif),l_mask);
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        SPRD_CODEC_AP_WRITE_BITS(reg,(val<<r_shif),r_mask);
    }
}

enum{
    SPRD_CODEC_MAIN_MIC =BIT_0,
    SPRD_CODEC_AUX_MIC  =BIT_1,
    SPRD_CODEC_HP_MIC   =BIT_2,
    SPRD_CODEC_LDO_MIC  =BIT_3,
    
    SPRD_CODEC_NONE     =0
};

LOCAL uint32 s_auto_mic_bias_bit = 0;
LOCAL uint32 s_auto_mic_bias_dis = 0;

LOCAL void _sprd_codec_ap_auto_mic_bias(uint32 en,uint32 bit)
{
    if( en )
    {
        s_auto_mic_bias_bit |= bit;
        __sprd_codec_ap_mic_bias(1);
        s_auto_mic_bias_dis = 0;
    }
    else
    {
        s_auto_mic_bias_bit &= ~bit;
        if( (0==s_auto_mic_bias_dis) && (0==s_auto_mic_bias_bit) )
        {
            __sprd_codec_ap_mic_bias(0);
        }
    }
}

#if (EAR_NEED_PA_SW)
enum{
    SPRD_CODEC_PA_SW        =BIT_0,
    SPRD_CODEC_EAR_PA_SW    =BIT_1,
};

LOCAL uint32 s_auto_pa_sw_bit = 0;
LOCAL uint32 s_auto_pa_sw_dis = 0;

LOCAL void _sprd_codec_ap_auto_pa_sw_en(uint32 en,uint32 bit)
{
    if( en )
    {
        s_auto_pa_sw_bit |= bit;
        __sprd_codec_ap_pa_sw_en(1);
        s_auto_pa_sw_dis = 0;
    }
    else
    {
        s_auto_pa_sw_bit &= ~bit;
        if( (0==s_auto_pa_sw_dis) && (0==s_auto_pa_sw_bit) )
        {
            __sprd_codec_ap_pa_sw_en(0);
        }
    }
}
#endif

#if (MIC_BAIS_LDO_USE_FOR_EXT)
PUBLIC uint32 __sprd_codec_ap_is_ldo_mic_bias(void)
{
    return ((s_auto_mic_bias_bit&SPRD_CODEC_LDO_MIC)==SPRD_CODEC_LDO_MIC);
}
#endif


PUBLIC void __sprd_codec_ap_clear(uint32 path)
{
    //CLEAR
    _sprd_codec_ap_path_op(path,&(audif_ptr->audif_clr),1,(AUDIF_ADC_PATH_CLR),(AUDIF_DAC_PATH_CLR));
}

PUBLIC void __sprd_codec_ap_dac_en(uint32 en,uint32 chan)
{
    _sprd_codec_ap_chan_en(en,chan,&(audif_ptr->audif_enb),AUDIF_DAC_EN_L,AUDIF_DAC_EN_R);
}

PUBLIC void __sprd_codec_ap_adc_en(uint32 en,uint32 chan)
{
    _sprd_codec_ap_chan_en(en,chan,&(audif_ptr->audif_enb),AUDIF_ADC_EN_L,AUDIF_ADC_EN_R);
}

// ANALOG PART...

PUBLIC void __sprd_codec_ap_vcm_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur1),AUD_VCM_EN);
    SPRD_CODEC_VCM_SLP((uint32)(!en));
}

PUBLIC void __sprd_codec_ap_vcm_buf_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur1),VCM_BUF_EN);
    SPRD_CODEC_VCMBUF_SLP((uint32)(!en));
}

PUBLIC void __sprd_codec_ap_bg_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur1),AUD_BG_EN);
    SPRD_CODEC_BG_SLP((uint32)(!en));
}

PUBLIC void __sprd_codec_ap_vb_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur1),VB_EN);
    SPRD_CODEC_VB_SLP((uint32)(!en));
}

PUBLIC void __sprd_codec_ap_vbo_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur1),VBO_EN);
    SPRD_CODEC_VBO_SLP((uint32)(!en));
}

PUBLIC void __sprd_codec_ap_bg_ibais_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur1),BG_IBIAS_EN);
}


PUBLIC void __sprd_codec_ap_en(uint32 en)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_en %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_234_112_2_17_23_2_54_202,(uint8*)"d",en);
    if( en )
    {
        __sprd_codec_ap_bg_ibais_en(1);
        __sprd_codec_ap_bg_en(1);
        __sprd_codec_ap_vcm_en(1);
        __sprd_codec_ap_vcm_buf_en(1);
        __sprd_codec_ap_vb_en(1);
        __sprd_codec_ap_vbo_en(1);
        SPRD_CODEC_AP_REG_SET(&(audif_ptr->audif_shutdown_ctl),0x00); //Trun OFF all protect.
        //_sprd_codec_ap_en_op(0,&(audif_ptr->audif_shutdown_ctl),PA_SHUTDOWN_EN1);

        //for pa ovp.
        __sprd_codec_ap_pa_ovp_en(1);
        __sprd_codec_ap_pa_ovp_ldo_en(1);
        __sprd_codec_ap_pa_ovp_v_sel(AUD_OVP_V_465_444);
    }
    else
    {
        if( !__sprd_codec_ap_is_ldo_mic_bias() ) //lint !e506 !e774
        {
            __sprd_codec_ap_vcm_en(0);
            SCI_Sleep(2);
            __sprd_codec_ap_vcm_buf_en(0);
            __sprd_codec_ap_vb_en(0);
            __sprd_codec_ap_vbo_en(0);
            __sprd_codec_ap_bg_ibais_en(0);
            __sprd_codec_ap_bg_en(0);
        }
    }
}

PUBLIC void __sprd_codec_ap_adc_pwr(uint32 en,uint32 chan)
{
    _sprd_codec_ap_chan_en((uint32)(!en),chan,&(audif_ptr->aacr1),AUD_ADCL_PD,AUD_ADCR_PD);
}

PUBLIC void __sprd_codec_ap_adc_ibuf_pwr(uint32 en,uint32 chan)
{
    _sprd_codec_ap_en_op((uint32)(!en),&(audif_ptr->aacr1),AUD_ADC_IBUF_PD);
}


PUBLIC void __sprd_codec_ap_adc_pga_pwr(uint32 en,uint32 chan)
{
    _sprd_codec_ap_chan_en((uint32)(!en),chan,&(audif_ptr->aacr2),ADC_PGAL_PD,ADC_PGAR_PD);
}


#if (PGA_SMOOTH_ADJUST)
PUBLIC void __sprd_codec_ap_adc_pga_gain_set(uint32 chan,uint32 val)
{
    _sprd_codec_ap_chan_mask(chan,&(audif_ptr->acgr),val
        ,ADC_PGAR_G_MASK,ADC_PGAR_G_SHIFT
        ,ADC_PGAL_G_MASK,ADC_PGAL_G_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_hp_pga_gain_set(uint32 chan,uint32 val)
{
    _sprd_codec_ap_chan_mask(chan,&(audif_ptr->dcgr1),val
        ,DAC_HPR_G_MASK,DAC_HPR_G_SHIFT
        ,DAC_HPL_G_MASK,DAC_HPL_G_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_ear_pga_gain_set(uint32 val)
{
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->dcgr2),val
        ,0,0
        ,DAC_EAR_G_MASK,DAC_EAR_G_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_pa_pga_gain_set(uint32 chan,uint32 val)
{
    _sprd_codec_ap_chan_mask(chan,&(audif_ptr->dcgr3),val
        ,DAC_PAR_G_MASK,DAC_PAR_G_SHIFT
        ,DAC_PA_G_MASK,DAC_PA_G_SHIFT
    );
}

PUBLIC uint32 __sprd_codec_ap_adc_pga_gain_get(uint32 chan)
{
    if(AUDIO_IS_CHAN_L(chan))
    {
        return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->acgr)) & ADC_PGAL_G_MASK)>>ADC_PGAL_G_SHIFT);
    }
    else if(AUDIO_IS_CHAN_R(chan))
    {
        return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->acgr)) & ADC_PGAR_G_MASK)>>ADC_PGAR_G_SHIFT);
    }
    return 0;
}

PUBLIC uint32 __sprd_codec_ap_hp_pga_gain_get(uint32 chan)
{
    if(AUDIO_IS_CHAN_L(chan))
    {
        return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr1)) & DAC_HPL_G_MASK)>>DAC_HPL_G_SHIFT);
    }
    else if(AUDIO_IS_CHAN_R(chan))
    {
        return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr1)) & DAC_HPR_G_MASK)>>DAC_HPR_G_SHIFT);
    }
    return 0;
}

PUBLIC uint32 __sprd_codec_ap_ear_pga_gain_get(void)
{
    return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr2)) & DAC_EAR_G_MASK)>>DAC_EAR_G_SHIFT);
}

PUBLIC uint32 __sprd_codec_ap_pa_pga_gain_get(uint32 chan)
{
    if(AUDIO_IS_CHAN_L(chan))
    {
        return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr3)) & DAC_PA_G_MASK)>>DAC_PA_G_SHIFT);
    }
    else if(AUDIO_IS_CHAN_R(chan))
    {
        return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr3)) & DAC_PAR_G_MASK)>>DAC_PAR_G_SHIFT);
    }
    return 0;
}


PUBLIC void __sprd_codec_ap_adc_pga_gain(uint32 chan,uint32 val)
{
    if(AUDIO_IS_CHAN_L(chan))
    {
        s_sprd_codec_pga_setting.pga_setting_0.adcpga = val;
    }
    if(AUDIO_IS_CHAN_R(chan))
    {
        s_sprd_codec_pga_setting.pga_setting_1.adcpga = val;
    }
    s_sprd_codec_pga_setting.is_mute.adcpga = 0;
    __sprd_codec_pga_adj_setting();
}

PUBLIC void __sprd_codec_ap_hp_pga_gain(uint32 chan,uint32 val)
{
    if(AUDIO_IS_CHAN_L(chan))
    {
        s_sprd_codec_pga_setting.pga_setting_0.hp = val;
    }
    if(AUDIO_IS_CHAN_R(chan))
    {
        s_sprd_codec_pga_setting.pga_setting_1.hp = val;
    }
    s_sprd_codec_pga_setting.is_mute.hp = 0;
    __sprd_codec_pga_adj_setting();
}

PUBLIC void __sprd_codec_ap_ear_pga_gain(uint32 val)
{
    s_sprd_codec_pga_setting.pga_setting_0.ear = val;
    s_sprd_codec_pga_setting.is_mute.ear = 0;
    __sprd_codec_pga_adj_setting();
}

PUBLIC void __sprd_codec_ap_pa_pga_gain(uint32 val)
{
    s_sprd_codec_pga_setting.pga_setting_0.pa = val;
    s_sprd_codec_pga_setting.is_mute.pa = 0;
    __sprd_codec_pga_adj_setting();
}

PUBLIC SPRD_CODEC_PGA_T* __sprd_codec_ap_pga_setting(uint32 chan)
{
    if(AUDIO_IS_CHAN_L(chan))
    {
        return &(s_sprd_codec_pga_setting.pga_setting_0);
    }
    if(AUDIO_IS_CHAN_R(chan))
    {
        return &(s_sprd_codec_pga_setting.pga_setting_1);
    }
    return &(s_sprd_codec_pga_setting.pga_setting_0);
}

PUBLIC uint32 __sprd_codec_ap_adc_pga_gain_is_mute(void)
{
    return s_sprd_codec_pga_setting.is_mute.adcpga;
}

PUBLIC uint32 __sprd_codec_ap_hp_pga_gain_is_mute(void)
{
    return s_sprd_codec_pga_setting.is_mute.hp;
}

PUBLIC uint32 __sprd_codec_ap_ep_pga_gain_is_mute(void)
{
    return s_sprd_codec_pga_setting.is_mute.ear;
}

PUBLIC uint32 __sprd_codec_ap_pa_pga_gain_is_mute(void)
{
    return s_sprd_codec_pga_setting.is_mute.pa;
}

PUBLIC void __sprd_codec_ap_adc_pga_gain_clr_mute(void)
{
    s_sprd_codec_pga_setting.is_mute.adcpga=0;
}

PUBLIC void __sprd_codec_ap_hp_pga_gain_clr_mute(void)
{
    s_sprd_codec_pga_setting.is_mute.hp=0;
}

PUBLIC void __sprd_codec_ap_ep_pga_gain_clr_mute(void)
{
    s_sprd_codec_pga_setting.is_mute.ear=0;
}

PUBLIC void __sprd_codec_ap_pa_pga_gain_clr_mute(void)
{
    s_sprd_codec_pga_setting.is_mute.pa=0;
}

#else

PUBLIC void __sprd_codec_ap_adc_pga_gain(uint32 chan,uint32 val)
{
    _sprd_codec_ap_chan_mask(chan,&(audif_ptr->acgr),val
        ,ADC_PGAR_G_MASK,ADC_PGAR_G_SHIFT
        ,ADC_PGAL_G_MASK,ADC_PGAL_G_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_hp_pga_gain(uint32 chan,uint32 val)
{
    _sprd_codec_ap_chan_mask(chan,&(audif_ptr->dcgr1),val
        ,DAC_HPR_G_MASK,DAC_HPR_G_SHIFT
        ,DAC_HPL_G_MASK,DAC_HPL_G_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_ear_pga_gain(uint32 val)
{
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->dcgr2),val
        ,0,0
        ,DAC_EAR_G_MASK,DAC_EAR_G_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_pa_pga_gain(uint32 val)
{
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->dcgr3),val
        ,0,0
        ,DAC_PA_G_MASK,DAC_PA_G_SHIFT
    );
}
#endif

PUBLIC void __sprd_codec_ap_adc_mute(void)
{
#if (PGA_SMOOTH_ADJUST)
    __sprd_codec_ap_adc_pga_gain(AUDIO_ALL_CHAN,ADC_PGA_G_MUTE_SET);
    s_sprd_codec_pga_setting.is_mute.adcpga= 1;
#else
    __sprd_codec_ap_adc_pga_gain(AUDIO_ALL_CHAN,ADC_PGA_G_MUTE_SET);
#endif
}

PUBLIC void __sprd_codec_ap_hp_mute(void)
{
#if (PGA_SMOOTH_ADJUST)
    __sprd_codec_ap_hp_pga_gain(AUDIO_ALL_CHAN,DAC_HP_PGA_G_MUTE_SET);
    s_sprd_codec_pga_setting.is_mute.hp = 1;
#else
    __sprd_codec_ap_hp_pga_gain(AUDIO_ALL_CHAN,DAC_HP_PGA_G_MUTE_SET);
#endif
}

PUBLIC void __sprd_codec_ap_ear_mute(void)
{
#if (PGA_SMOOTH_ADJUST)
    __sprd_codec_ap_ear_pga_gain(DAC_EAR_PGA_G_MUTE_SET);
    s_sprd_codec_pga_setting.is_mute.ear = 1;
#else
    __sprd_codec_ap_ear_pga_gain(DAC_EAR_PGA_G_MUTE_SET);
#endif
}

PUBLIC void __sprd_codec_ap_pa_mute(void)
{
#if (PGA_SMOOTH_ADJUST)
    __sprd_codec_ap_pa_pga_gain(DAC_PA_PGA_G_MUTE_SET);
    s_sprd_codec_pga_setting.is_mute.pa = 1;
#else
    __sprd_codec_ap_pa_pga_gain(DAC_PA_PGA_G_MUTE_SET);
#endif
}



PUBLIC void __sprd_codec_ap_adc(uint32 en,uint32 chan)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_adc %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_327_112_2_17_23_2_54_203,(uint8*)"d",en);
    __sprd_codec_ap_adc_ibuf_pwr(en,chan);
    __sprd_codec_ap_adc_pga_pwr(en,chan);
}

PUBLIC void __sprd_codec_ap_linein_rec_mixer(uint32 en,uint32 chan)
{
    if( (en) && (AUDIO_IS_CHAN_R(chan)) )
    {
        _sprd_codec_ap_en_op(1,&(audif_ptr->aacr1),FM_REC_EN);
    }
    else
    {
        _sprd_codec_ap_en_op(0,&(audif_ptr->aacr1),FM_REC_EN);
    }
}

PUBLIC void __sprd_codec_ap_linein_rec(uint32 en,uint32 chan)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_linein_rec %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_347_112_2_17_23_2_54_204,(uint8*)"d",en);
    __sprd_codec_ap_adc_clk_pwr(en);
    __sprd_codec_ap_linein_rec_mixer(en,chan);
    __sprd_codec_adc_en(en,chan);
    __sprd_codec_ap_adc_pwr(en,chan);
}

PUBLIC void __sprd_codec_ap_dac_pwr(uint32 en,uint32 chan)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_dac_pwr %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_355_112_2_17_23_2_54_205,(uint8*)"d",en);
    _sprd_codec_ap_chan_en(en,chan,&(audif_ptr->dacr),AUD_DACL_EN,AUD_DACR_EN);
}

PUBLIC void __sprd_codec_ap_dac(uint32 en,uint32 chan)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_dac %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_361_112_2_17_23_2_54_206,(uint8*)"d",en);
    __sprd_codec_dac_en(en,chan);
    __sprd_codec_ap_dac_pwr(en,chan);
}

LOCAL void _sprd_codec_ap_lin_adc_change(uint32 en,uint32 chan)
{
    uint32 rj217 = (chan >>SPRD_CODEC_RJ217_SHIFT)&SPRD_CODEC_RJ217_MASK;
    uint32 l_val;
    uint32 r_val;
    
    if( AUDIO_IS_CHAN_L_ONLY(chan) )
    {
        l_val = (ADC_AIL_ADCL | ADC_AIR_ADCL | ADC_VCM_ADCL);
        r_val = 0;
    }
    else if( AUDIO_IS_CHAN_R_ONLY(chan) )
    {
        l_val = 0;
        r_val = (ADC_AIL_ADCR | ADC_AIR_ADCR | ADC_VCM_ADCR);
    }
    else
    {
        if( rj217 )
        {
            l_val = (ADC_AIR_ADCL | ADC_VCM_ADCL);
            r_val = (ADC_AIL_ADCR | ADC_VCM_ADCR);
        }
        else
        {
            l_val = (ADC_AIL_ADCL | ADC_VCM_ADCL);
            r_val = (ADC_AIR_ADCR | ADC_VCM_ADCR);
        }
    }
    _sprd_codec_ap_chan_en(en,chan,&(audif_ptr->aaicr3),l_val,r_val);
}

PUBLIC void __sprd_codec_ap_lin_adc(uint32 en,uint32 chan)
{
    static uint32 pre_chan = 0;

    if( en )
    {
        if( pre_chan )
        {
            _sprd_codec_ap_lin_adc_change(0,pre_chan);
        }
        pre_chan |= chan;
        chan = pre_chan;
    }
    else
    {
        pre_chan &= ~chan;
    }
    
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_lin_adc %d,%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_416_112_2_17_23_2_54_207,(uint8*)"dd",en,chan);
    _sprd_codec_ap_lin_adc_change(en,chan);
}

LOCAL void _sprd_codec_ap_stereo_open_state(uint32 en,uint32* p_l_val,uint32* p_r_val)
{
    uint32 reg ;
    uint32 l_val = 0;
    uint32 r_val = 0;
    reg = SPRD_CODEC_AP_REG_GET((uint32)(&(audif_ptr->daocr1)));
    if( ((reg & DAC_ADCPGAR_BUFN_SPN) ) )
    {
        r_val |= ADC_PGAR_BUFN_EN;
    }
    if( ((reg & DAC_ADCPGAR_BUFP_SPP) )
     || ((reg & DAC_ADCPGAR_BUFP_HPR) )
     || ((reg & DAC_ADCPGAR_BUFP_HPL) )
    )
    {
        r_val |= ADC_PGAR_BUFP_EN;
    }
    if( ((reg & DAC_ADCPGAL_BUFN_SPN) )
     || ((reg & DAC_ADCPGAL_BUFN_HPR) )
    )
    {
        l_val |= ADC_PGAL_BUFN_EN;
    }
    if( ((reg & DAC_ADCPGAL_BUFP_SPP) )
     || ((reg & DAC_ADCPGAL_BUFP_HPL) )
    )
    {
        l_val |= ADC_PGAL_BUFP_EN;
    }
    if( 0==en )
    {
        l_val = (~l_val&(ADC_PGAL_BUFP_EN|ADC_PGAL_BUFN_EN));
        r_val = (~r_val&(ADC_PGAR_BUFP_EN|ADC_PGAR_BUFN_EN));
    }
    *p_l_val = l_val;
    *p_r_val = r_val;
}

PUBLIC void __sprd_codec_ap_adc_pga_buf_pwr(uint32 en,uint32 chan)
{
    uint32 l_val;
    uint32 r_val;
    _sprd_codec_ap_stereo_open_state(en,&l_val,&r_val);
    _sprd_codec_ap_chan_en(en,AUDIO_ALL_CHAN,&(audif_ptr->aacr2),l_val,r_val);
}



PUBLIC void __sprd_codec_ap_lin(uint32 en,uint32 chan)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_lin %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_470_112_2_17_23_2_54_208,(uint8*)"d",en);
    __sprd_codec_ap_lin_adc(en,chan);
}


PUBLIC void __sprd_codec_ap_ear_mixer(uint32 en)
{
    uint32 l_val;
    l_val = (DAC_DACL_EARN | DAC_DACL_EARP);
    _sprd_codec_ap_chan_en(en,AUDIO_CHAN_L,&(audif_ptr->daocr3),l_val,0);
}


PUBLIC void __sprd_codec_ap_lout_mixer(uint32 en,uint32 chan)
{
    uint32 l_val;
    uint32 r_val;
    l_val = (DAC_DACL_SPN | DAC_DACL_SPP);
    r_val = (DAC_DACR_SPN | DAC_DACR_SPP);
    _sprd_codec_ap_chan_en(en,chan,&(audif_ptr->daocr2),l_val,r_val);
}

PUBLIC void __sprd_codec_ap_lout_mixer_adcbuf(uint32 en,uint32 chan)
{
    uint32 l_val;
    uint32 r_val;
    l_val = (DAC_ADCPGAL_BUFN_SPN | DAC_ADCPGAL_BUFP_SPP);
    r_val = (DAC_ADCPGAR_BUFN_SPN | DAC_ADCPGAR_BUFP_SPP);
    _sprd_codec_ap_chan_en(en,chan,&(audif_ptr->daocr1),l_val,r_val);
    __sprd_codec_ap_adc_pga_buf_pwr(en,chan);
}

PUBLIC void __sprd_codec_ap_aol_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->dcr1),AUD_AOL_EN);
}


PUBLIC void __sprd_codec_ap_ear(uint32 en)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_ear %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_510_112_2_17_23_2_55_209,(uint8*)"d",en);
    __sprd_codec_ap_ear_en(en);
#if (EAR_NEED_PA_SW)
    _sprd_codec_ap_auto_pa_sw_en(en,SPRD_CODEC_EAR_PA_SW);
#endif
    __sprd_codec_ap_ear_mixer(en);
}


PUBLIC void __sprd_codec_ap_lout(uint32 en)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_lout %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_518_112_2_17_23_2_55_210,(uint8*)"d",en);
    if( en )
    {
#if (EAR_NEED_PA_SW)
        _sprd_codec_ap_auto_pa_sw_en(1,SPRD_CODEC_PA_SW);
#else
        __sprd_codec_ap_pa_sw_en(1);
#endif
    }
    __sprd_codec_ap_aol_en(en);
}


PUBLIC void __sprd_codec_ap_ear_en(uint32 en)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_ear_en %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_526_112_2_17_23_2_55_211,(uint8*)"d",en);
    _sprd_codec_ap_en_op(en,&(audif_ptr->dcr1),AUD_EAR_EN);
}

PUBLIC void __sprd_codec_ap_hp_en(uint32 en,uint32 chan)
{
    _sprd_codec_ap_chan_en(en,AUDIO_ALL_CHAN,&(audif_ptr->dcr1),AUD_HPL_EN,AUD_HPR_EN);
    _sprd_codec_ap_en_op(en,&(audif_ptr->dcr1),AUD_DIFF_EN);
}

PUBLIC void __sprd_codec_ap_hp_mixer(uint32 en,uint32 chan)
{
    uint32 l_val;

    if( AUDIO_IS_CHAN_L_ONLY(chan) )
    {
        l_val = (DAC_DACL_HPL | DAC_DACL_HPR);
    }
    else if( AUDIO_IS_CHAN_R_ONLY(chan) )
    {
        l_val = (DAC_DACR_HPL | DAC_DACR_HPR);
    }
    else
    {
        l_val = (DAC_DACL_HPL | DAC_DACR_HPR);
    }
    _sprd_codec_ap_chan_en(en,AUDIO_CHAN_L,&(audif_ptr->daocr2),l_val,0);
}


PUBLIC void __sprd_codec_ap_hp_mixer_adcbuf(uint32 en,uint32 chan)
{
    uint32 rj217 = (chan >>SPRD_CODEC_RJ217_SHIFT)&SPRD_CODEC_RJ217_MASK;
    uint32 l_val;

    if( AUDIO_IS_CHAN_L_ONLY(chan) )
    {
        l_val = (DAC_ADCPGAL_BUFP_HPL | DAC_ADCPGAL_BUFN_HPR);
    }
    else if( AUDIO_IS_CHAN_R_ONLY(chan) )
    {
        l_val = (DAC_ADCPGAR_BUFP_HPL | DAC_ADCPGAR_BUFP_HPR);
    }
    else
    {
        if( rj217 )
        {
            l_val = (DAC_ADCPGAR_BUFP_HPL | DAC_ADCPGAL_BUFN_HPR);
        }
        else
        {
            l_val = (DAC_ADCPGAL_BUFP_HPL | DAC_ADCPGAR_BUFP_HPR);
        }
    }
    _sprd_codec_ap_chan_en(en,AUDIO_CHAN_L,&(audif_ptr->daocr1),l_val,0);
    __sprd_codec_ap_adc_pga_buf_pwr(en,chan);
}


PUBLIC void __sprd_codec_ap_hp(uint32 en,uint32 chan)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_hp %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_587_112_2_17_23_2_55_212,(uint8*)"d",en);
    __sprd_codec_ap_hp_en(en,chan);
}

PUBLIC uint32 __sprd_codec_ap_hp_pop_en(uint32 en)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_hp_pop_en %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_593_112_2_17_23_2_55_213,(uint8*)"d",en);
    if( en )
    {
        __sprd_codec_ap_hp_pop_step(HP_POP_STEP_2);
        __sprd_codec_ap_hp_pop(HP_POP_CTL_UP);
    }
    else
    {
        __sprd_codec_ap_hp_pop_step(HP_POP_STEP_1);
        __sprd_codec_ap_hp_pop(HP_POP_CTL_DOWN);
    }
    return 1;
}

PUBLIC uint32 __sprd_codec_ap_hp_ramp_flg(void)
{
    uint32 val ;
    val = SPRD_CODEC_AP_REG_GET((uint32)(&(audif_ptr->ifr1)));
    return ((val&HP_POP_FLG_MASK)>>HP_POP_FLG_SHIFT);
}

PUBLIC uint32 __sprd_codec_ap_hp_ramp_done(uint32 up)
{
    if( HP_POP_FLG_NEAR_CMP == __sprd_codec_ap_hp_ramp_flg() )
    {
        return 1;
    }
    return 0;
}

PUBLIC void __sprd_codec_ap_clr_ramp(uint32 up)
{
    if( up )
    {
        __sprd_codec_ap_hp_pop(HP_POP_CTL_HOLD);
    }
    else
    {
        __sprd_codec_ap_hp_pop(HP_POP_CTL_DIS);
    }
}

PUBLIC void __sprd_codec_ap_mic_bias(uint32 en)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_mic_bias %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_637_112_2_17_23_2_55_214,(uint8*)"d",en);
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur1),MICBIAS_EN);
    SPRD_CODEC_MICBIAS_SLP((uint32)(!en));
    s_auto_mic_bias_dis = en;
}


PUBLIC void __sprd_codec_ap_auxmic_bias(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur1),AUXMICBIAS_EN);
}

PUBLIC void __sprd_codec_ap_main_mic(uint32 en,uint32 chan)
{
    uint32 l_val;
    uint32 r_val;
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_main_mic %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_652_112_2_17_23_2_55_215,(uint8*)"d",en);
    l_val = (ADC_MICP_ADCL | ADC_MICN_ADCL);
    r_val = (ADC_MICP_ADCR | ADC_MICN_ADCR);
    _sprd_codec_ap_chan_en(en,chan,&(audif_ptr->aaicr1),l_val,r_val);
    _sprd_codec_ap_auto_mic_bias(en,SPRD_CODEC_MAIN_MIC);
}


PUBLIC void __sprd_codec_ap_aux_mic(uint32 en,uint32 chan)
{
    uint32 l_val;
    uint32 r_val;
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_aux_mic %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_664_112_2_17_23_2_55_216,(uint8*)"d",en);
    l_val = (ADC_AUXMICP_ADCL | ADC_AUXMICN_ADCL);
    r_val = (ADC_AUXMICP_ADCR | ADC_AUXMICN_ADCR);
    _sprd_codec_ap_chan_en(en,chan,&(audif_ptr->aaicr2),l_val,r_val);
    _sprd_codec_ap_auto_mic_bias(en,SPRD_CODEC_AUX_MIC);
}


PUBLIC void __sprd_codec_ap_hp_mic(uint32 en,uint32 chan)
{
    uint32 l_val;
    uint32 r_val;
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_hp_mic %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_676_112_2_17_23_2_55_217,(uint8*)"d",en);
    l_val = (ADC_HEADMICP_ADCL | ADC_HEADMICN_ADCL);
    r_val = (ADC_HEADMICP_ADCR | ADC_HEADMICN_ADCR);
    _sprd_codec_ap_chan_en(en,chan,&(audif_ptr->aaicr1),l_val,r_val);
    _sprd_codec_ap_auto_mic_bias(en,SPRD_CODEC_HP_MIC);
}


PUBLIC void __sprd_codec_ap_drv_clk_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->ccr),AUD_DRV_CLK_EN);
}

PUBLIC void __sprd_codec_ap_rev_clk_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->ccr),AUD_CLK_REVERSE);
}

PUBLIC void __sprd_codec_ap_dac_clk_en(uint32 en)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_dac_clk_en %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_696_112_2_17_23_2_55_218,(uint8*)"d",en);
    _sprd_codec_ap_en_op(en,&(audif_ptr->ccr),AUD_DAC_CLK_EN);
}

PUBLIC void __sprd_codec_ap_adc_clk_pwr(uint32 en)
{
    _sprd_codec_ap_en_op((uint32)(!en),&(audif_ptr->ccr),AUD_ADC_CLK_PD);
}

PUBLIC void __sprd_codec_ap_adc_clk_rst(void)
{
    _sprd_codec_ap_en_op(1,&(audif_ptr->ccr),AUD_ADC_CLK_RST);
    
    _sprd_codec_ap_en_op(0,&(audif_ptr->ccr),AUD_ADC_CLK_RST);
}

PUBLIC void __sprd_codec_ap_hp_pop(uint32 ctl)
{
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->pnrcr1),ctl
        ,0,0
        ,HP_POP_CTL_MASK,HP_POP_CTL_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_hp_pop_step(uint32 step)
{
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->pnrcr1),step
        ,0,0
        ,HP_POP_STEP_MASK,HP_POP_STEP_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_pa_sw_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur2),PA_SW_EN);
}

PUBLIC void __sprd_codec_ap_pa_en(uint32 en)
{
    if( en )
    {
        _sprd_codec_ap_en_op(en,&(audif_ptr->pmur2),PA_EN);
    }
    else
    {
#if (EAR_NEED_PA_SW)
        if( (0==s_auto_pa_sw_dis) && (0==s_auto_pa_sw_bit) )
        {
            _sprd_codec_ap_en_op(en,&(audif_ptr->pmur2),(PA_EN|PA_SW_EN|PA_LDO_EN));
        }
        else
        {
            _sprd_codec_ap_en_op(en,&(audif_ptr->pmur2),(PA_EN|PA_LDO_EN));
        }
#else
        _sprd_codec_ap_en_op(en,&(audif_ptr->pmur2),(PA_EN|PA_SW_EN|PA_LDO_EN));
#endif
        SCI_Sleep(CLOSE_DELAY_TIME);
    }
}

PUBLIC void __sprd_codec_ap_pa_ldo_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur2),PA_LDO_EN);
#if (EAR_NEED_PA_SW)
    _sprd_codec_ap_auto_pa_sw_en(0,SPRD_CODEC_PA_SW);
#else
    __sprd_codec_ap_pa_sw_en(0);
#endif
}


PUBLIC void __sprd_codec_ap_pa_ldo_v_sel(uint32 v_sel)
{
    //SPRD_CODEC_AP_PRINT("__sprd_codec_ap_pa_ldo_v_sel %d",v_sel);
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->pmur4),v_sel
        ,0,0
        ,PA_LDO_V_MASK,PA_LDO_V_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_pa_d_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->dcr2),PA_D_EN);
}

PUBLIC void __sprd_codec_ap_pa_dtri_f_sel(uint32 freq)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_pa_dtri_f_sel %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_761_112_2_17_23_2_55_219,(uint8*)"d",freq);
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->dcr2),freq
        ,0,0
        ,PA_DTRI_F_MASK,PA_DTRI_F_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_pa_demi_en(uint32 en)
{
    //SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_pa_demi_en %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_770_112_2_17_23_2_55_220,(uint8*)"d",en);
    _sprd_codec_ap_en_op(en,&(audif_ptr->dcr2),PA_DEMI_EN);
    _sprd_codec_ap_en_op(en,&(audif_ptr->dcr3),AUD_DRV_OCP_PA_PD);
}

PUBLIC void __sprd_codec_ap_pa_vcm_v_sel(uint32 v_sel)
{
    //SPRD_CODEC_AP_PRINT("__sprd_codec_ap_pa_vcm_v_sel %d",v_sel);
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->pmur3),v_sel
        ,0,0
        ,AUD_VCM_V_MASK,AUD_VCM_V_SHIFT
    );
}

PUBLIC uint32 __sprd_codec_update_vb_vbo_vcm_vol(uint32 cur_vol)
{
    uint32 i = 0;
    uint32 len = sizeof(_vb_vbo_vcm_vol_map_tab)/sizeof(_vb_vbo_vcm_vol_map_tab[0]);
    for(i=0;i<len;i++)
    {
        if(cur_vol <= _vb_vbo_vcm_vol_map_tab[i].cur_vol)
        {
            __sprd_codec_ap_pa_vcm_v_sel(_vb_vbo_vcm_vol_map_tab[i].ldo_level);
            return 0;
        }
    }
    return (uint32)(-1);
}

PUBLIC uint32 __sprd_codec_update_pa_ldo_vol(uint32 cur_vol)
{
    uint32 i = 0;
    uint32 len = sizeof(_pa_ldo_vol_map_tab)/sizeof(_pa_ldo_vol_map_tab[0]);
    for(i=0;i<len;i++)
    {
        if(cur_vol <= _pa_ldo_vol_map_tab[i].cur_vol)
        {
            __sprd_codec_ap_pa_ldo_v_sel(_pa_ldo_vol_map_tab[i].ldo_level);
            return 0;
        }
    }
    return (uint32)(-1);
}


PUBLIC void __sprd_codec_ap_int_set(uint32 en,uint32 mask)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->audif_int_en),mask);
}

PUBLIC void __sprd_codec_ap_int_clr(uint32 mask)
{
    _sprd_codec_ap_en_op(1,&(audif_ptr->audif_int_clr),mask);
}

PUBLIC uint32 __sprd_codec_ap_int_raw(void)
{
    return SPRD_CODEC_AP_REG_GET(&(audif_ptr->audif_int_raw));
}

PUBLIC uint32 __sprd_codec_ap_int_mask(void)
{
    return SPRD_CODEC_AP_REG_GET(&(audif_ptr->audif_int_mask));
}

PUBLIC void __sprd_codec_ap_mic_bias_ldo_v_sel(uint32 v_sel)
{
    //SPRD_CODEC_AP_PRINT("__sprd_codec_ap_mic_bias_ldo_v_sel %d",v_sel);
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->pmur3),v_sel
        ,0,0
        ,MICBIAS_V_MASK,MICBIAS_V_SHIFT
    );
}

PUBLIC void __sprd_codec_ap_mic_bias_ldo_set(uint32 en)
{
    _sprd_codec_ap_auto_mic_bias(en,SPRD_CODEC_LDO_MIC);
}


PUBLIC void __sprd_codec_ap_pa_ovp_ldo_en(uint32 en)
{
    _sprd_codec_ap_en_op(en,&(audif_ptr->pmur2),OVP_LDO_EN);
}

PUBLIC void __sprd_codec_ap_pa_ovp_en(uint32 en)
{
    _sprd_codec_ap_en_op((uint32)(!en),&(audif_ptr->pmur2),OVP_PD);
}

PUBLIC void __sprd_codec_ap_pa_ovp_v_sel(uint32 v_sel)
{
    _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->pmur6),v_sel
        ,0,0
        ,AUD_OVP_V_MASK,AUD_OVP_V_SHIFT
    );
}


PUBLIC void __sprd_codec_ap_reg_print(void)
{
    uint32 reg = SPRD_CODEC_AP_REG_BASE;
    uint32 offset = 0;
    for(;offset<0xB8;offset+=0x10)
    {
        //SPRD_CODEC_AP_PRINT:"0x%02x | 0x%02x 0x%02x 0x%02x 0x%02x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_850_112_2_17_23_2_55_221,(uint8*)"ddddd",offset,SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x0)),SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x4)),SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x8)),SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0xc)));
    }
}

