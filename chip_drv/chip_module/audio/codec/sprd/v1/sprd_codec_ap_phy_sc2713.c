/******************************************************************************
 ** File Name:      sprd_codec_ap_phy.c                                    *
 ** Author:        zhenfang.wang                                             *
 ** DATE:           10/16/2015                                                *
 ** Copyright:      2015 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec ap control                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/03/2016     ye.zhang      Create for sc2713!!!!                        *
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
#include "sprd_codec_ap_phy_sc2713.h"
#include "ldo_drvapi.h"

#ifdef CODEC_DEBUG
#define SPRD_CODEC_AP_PRINT    SCI_TRACE_LOW
#else
#define SPRD_CODEC_AP_PRINT(...)
#endif

#define SPRD_CODEC_AP_REG_BF_SET_VAL(_Reg, _Value, _Lsb, _Mask)	SPRD_CODEC_AP_WRITE_BITS(_Reg,(_Value<<_Lsb),_Mask)

enum
{
	SPRD_CODEC_MAIN_MIC = BIT_0,
	SPRD_CODEC_AUX_MIC  = BIT_1,
	SPRD_CODEC_HP_MIC   = BIT_2,

	SPRD_CODEC_NONE     = 0
};

LOCAL uint32 s_auto_mic_bias_bit = 0;
LOCAL uint32 s_auto_mic_bias_dis = 0;

PUBLIC void __sprd_codec_ap_adc_clk_en(uint32 en);
//PUBLIC void __sprd_codec_ap_drv_clk_en(uint32 en);   //lint Redundantly declared symbol
//PUBLIC void __sprd_codec_ap_mic_bias(uint32 en);
//PUBLIC void __sprd_codec_ap_pa_sw_en(uint32 en);
//PUBLIC void __sprd_codec_ap_hp_pop(uint32 ctl);
//PUBLIC void __sprd_codec_ap_adc_clk_fs_sel(uint32 fs);
//PUBLIC void __sprd_codec_ap_adc_clk_en(uint32 en);
//PUBLIC void __sprd_codec_ap_dac_clk_fs_sel(uint32 fs);
//PUBLIC void __sprd_codec_ap_dac_clk_en(uint32 en);

#if (PGA_SMOOTH_ADJUST)
typedef struct
{
	SPRD_CODEC_PGA_T        pga_setting_0;
	SPRD_CODEC_PGA_T        pga_setting_1;
	SPRD_CODEC_PGA_T        is_mute;
}SPRD_CODEC_PGA_SETTING_T;
LOCAL SPRD_CODEC_PGA_SETTING_T s_sprd_codec_pga_setting;
#endif

LOCAL void _sprd_codec_ap_en_op(uint32 en,void* reg,uint32 bit)
{
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

#if (EAR_NEED_PA_SW)//lint !e553
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

LOCAL void _sprd_codec_ap_auto_mic_bias(uint32 en, uint32 bit)
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
		if( (0 == s_auto_mic_bias_dis) && (0 == s_auto_mic_bias_bit) )
		{
			__sprd_codec_ap_mic_bias(0);
		}
	}
}
PUBLIC void __sprd_codec_ap_clear(uint32 path)
{
	//CLEAR
	//_sprd_codec_ap_path_op(path, &(audif_ptr->audif_clr), 1, (AUDIF_ADC_PATH_CLR), (AUDIF_DAC_PATH_CLR));
}

PUBLIC void __sprd_codec_ap_dac_en(uint32 en, uint32 chan)
{ 
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_DIG_CFG0,AUDIFA_DACL_EN,AUDIFA_DACR_EN);
}

PUBLIC void __sprd_codec_ap_adc_en(uint32 en, uint32 chan)
{
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_DIG_CFG0,AUDIFA_ADCL_EN,AUDIFA_ADCR_EN);
}

// ANALOG PART...

PUBLIC void __sprd_codec_ap_vcm_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG1,VCM_EN);
}

PUBLIC void __sprd_codec_ap_vcm_buf_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG1,VCM_BUF_EN);
}

PUBLIC void __sprd_codec_ap_bg_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG1,BG_EN);
}

PUBLIC void __sprd_codec_ap_vb_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG0,VB_EN);
}

PUBLIC void __sprd_codec_ap_vbo_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG0,VBO_EN);
}

PUBLIC void __sprd_codec_ap_bg_ibias_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG1,BG_IBIAS_EN);
}


PUBLIC void __sprd_codec_ap_en(uint32 en)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec2713_ap_en %d", en);
	if( en )
	{
		_sprd_codec_ap_en_op(1, (void*)ADIE_ARM_MODULE_EN, MODULE_AUD_EN);
		_sprd_codec_ap_en_op(1, (void*)REG_ANA_RTC_CLK_EN,  BIT_4);//(BIT_ANA_RTC_AUD_EN));
		_sprd_codec_ap_en_op(1, (void*)REG_ANA_ARM_CLK_EN, BIT_1|BIT_2);//(BIT_ANA_CLK_AUDIF_EN | BIT_ANA_CLK_AUD_6P5M_EN)); 
		_sprd_codec_ap_en_op(1, (void*)REG_ANA_XTL_WAIT_CTRL, BIT_8);//BIT_ANA_XTL_EN);  //26M

		__sprd_codec_ap_bg_ibias_en(1);
		__sprd_codec_ap_bg_en(1);
		__sprd_codec_ap_vcm_en(1);
		__sprd_codec_ap_vcm_buf_en(1);
		__sprd_codec_ap_vb_en(1);
		__sprd_codec_ap_vbo_en(1);
	}
	else
	{
		__sprd_codec_ap_vcm_en(0);
		__sprd_codec_ap_vcm_buf_en(0);
		__sprd_codec_ap_vb_en(0);
		__sprd_codec_ap_vbo_en(0);
		__sprd_codec_ap_bg_ibias_en(0);
		__sprd_codec_ap_bg_en(0);
		_sprd_codec_ap_en_op(0, (void*)REG_ANA_RTC_CLK_EN,  BIT_4);
		_sprd_codec_ap_en_op(0,(void*)REG_ANA_ARM_CLK_EN, BIT_1|BIT_2);
		_sprd_codec_ap_en_op(0, (void*)REG_ANA_XTL_WAIT_CTRL, BIT_8);
		_sprd_codec_ap_en_op(0, (void*)ADIE_ARM_MODULE_EN, MODULE_AUD_EN);
	}
}

PUBLIC void __sprd_codec_ap_adc_pwr(uint32 en, uint32 chan)
{
	_sprd_codec_ap_chan_en((uint32)(!en),chan,(void*)AUD_AP_ANA_CFG5,ADCL_PD,ADCR_PD);
}

PUBLIC void __sprd_codec_ap_adc_ibuf_pwr(uint32 en, uint32 chan)
{
	_sprd_codec_ap_en_op((uint32)(!en),(void*)AUD_AP_ANA_CFG5,ADC_IBUF_PD);
}

PUBLIC void __sprd_codec_ap_adc_pga_pwr(uint32 en, uint32 chan)
{
	//todo??
	//// _sprd_codec_ap_chan_en((uint32)(!en), chan, &(audif_ptr->ANA_CFG5), ADC_PGAL1_EN, ADC_PGAR1_EN);
	//// _sprd_codec_ap_chan_en((uint32)(!en), chan, &(audif_ptr->ANA_CFG5), ADC_PGAL2_EN, ADC_PGAR2_EN);
	int val = 0;
	if(en)
	{
		val = 0x03;
	}
	_sprd_codec_ap_chan_mask(chan,(void*)AUD_AP_ANA_CFG5,val
			,ADCPGAR_EN_MASK,ADCPGAR_EN_LSB
			,ADCPGAL_EN_MASK,ADCPGAL_EN_LSB
			);
}


PUBLIC void __sprd_codec_ap_adc_pga_gain(uint32 chan, uint32 val)
{
	_sprd_codec_ap_chan_mask(chan,(void*)AUD_AP_ANA_CFG8,val
			,ADPGAR_G_MASK,ADPGAR_G
			,ADPGAL_G_MASK,ADPGAL_G
			);
}

PUBLIC void __sprd_codec_ap_hp_pga_gain(uint32 chan, uint32 val)
{
	_sprd_codec_ap_chan_mask(chan,(void*)AUD_AP_ANA_CFG16,val
			,HPR_G_MASK,HPR_G
			,HPL_G_MASK,HPL_G
			);
}

PUBLIC void __sprd_codec_ap_hp_mute(void)
{
	__sprd_codec_ap_hp_pga_gain(0,DAC_HP_PGA_G_MUTE);
}

PUBLIC void __sprd_codec_ap_ear_pga_gain(uint32 val)
{
	_sprd_codec_ap_chan_mask(AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG16,val
			, 0, 0
			,EAR_G_MASK,EAR_G
			);
}

PUBLIC void __sprd_codec_ap_ear_mute(void)
{
	__sprd_codec_ap_ear_pga_gain(DAC_EAR_PGA_G_MUTE);
}

PUBLIC void __sprd_codec_ap_pal_pga_gain(uint32 val)
{
	_sprd_codec_ap_chan_mask(AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG17,val
			, 0, 0
			,AOL_G_MASK,AOL_G
			);
}

PUBLIC void __sprd_codec_ap_pa_mute(void)
{
	__sprd_codec_ap_pal_pga_gain(DAC_PA_PGA_G_MUTE);
}


PUBLIC void __sprd_codec_ap_par_pga_gain(uint32 val)
{
	_sprd_codec_ap_chan_mask(AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG17,val
			,0,0
			,AOR_G_MASK,AOR_G
			);
}

PUBLIC void __sprd_codec_ap_par_mute(void)
{
	__sprd_codec_ap_par_pga_gain(DAC_PA_PGA_G_MUTE);
}
PUBLIC void __sprd_codec_ap_adc(uint32 en, uint32 chan)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_adc %d", en);

	__sprd_codec_ap_adc_en(en, chan);
	__sprd_codec_ap_adc_pwr(en, chan);
	__sprd_codec_ap_adc_ibuf_pwr(en, chan);
	__sprd_codec_ap_adc_pga_pwr(en, chan);
	__sprd_codec_ap_adc_clk_en(en);
}

PUBLIC void __sprd_codec_ap_linein_rec_mixer(uint32 en, uint32 chan)
{
}

PUBLIC void __sprd_codec_ap_linein_rec(uint32 en, uint32 chan)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_linein_rec %d", en);
	//__sprd_codec_ap_linein_rec_mixer(en,chan);
	// __sprd_codec_adc_en(en, chan);
	__sprd_codec_ap_adc_en(en, chan);
	__sprd_codec_ap_adc_pwr(en, chan);
}

PUBLIC void __sprd_codec_ap_dac_pwr(uint32 en, uint32 chan)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_dac_pwr %d", en);
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG9,DACL_EN,DACR_EN);
}

PUBLIC void __sprd_codec_ap_dac(uint32 en, uint32 chan)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_dac %d", en);
	//__sprd_codec_ap_dac_en(en, chan);//???????  
	__sprd_codec_ap_dac_pwr(en, chan);
	__sprd_codec_ap_dac_clk_en(en);
}
PUBLIC void __sprd_codec_ap_dac_dg_set(uint32 chan,uint32 dg )
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_dac_dg_set %d", dg);

	if(AUDIO_IS_CHAN_L(chan)){
		SPRD_CODEC_AP_REG_BF_SET_VAL(AUD_AP_ANA_CFG9,dg,DACL_G,DACL_G_MASK);
	}
	if(AUDIO_IS_CHAN_R(chan)){
		SPRD_CODEC_AP_REG_BF_SET_VAL(AUD_AP_ANA_CFG9,dg,DACR_G,DACR_G_MASK);
	}
}
PUBLIC void __sprd_codec_ap_dac_vbuf_i_set(uint32 chan,uint32 vbuf_i )
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_dac_vbuf_i_set %d", vbuf_i);

	SPRD_CODEC_AP_REG_BF_SET_VAL(AUD_AP_ANA_CFG9,vbuf_i,DACBUF_I_S_LSB,DACBUF_I_S);
}
//todo ??
LOCAL void _sprd_codec_ap_lin_adc_change(uint32 en, uint32 chan)
{
	uint32 rj217 = (chan >> SPRD_CODEC_RJ217_SHIFT)&SPRD_CODEC_RJ217_MASK;
	uint32 l_val;
	uint32 r_val;

	if( AUDIO_IS_CHAN_L_ONLY(chan) )
	{
		l_val = (AIL_ADCL | AIR_ADCL);
		r_val = 0;
	}
	else if( AUDIO_IS_CHAN_R_ONLY(chan) )
	{
		l_val = 0;
		r_val = (AIL_ADCR | AIR_ADCR);
	}
	else
	{
		if( rj217 )
		{
			l_val = (AIR_ADCL);
			r_val = (AIL_ADCR);
		}
		else
		{
			l_val = (AIL_ADCL);
			r_val = (AIR_ADCR);
		}
	}
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG6,l_val,r_val);
}

PUBLIC void __sprd_codec_ap_lin_adc(uint32 en, uint32 chan)
{
	static uint32 pre_chan = 0;

	if( en )
	{
		if( pre_chan )
		{
			_sprd_codec_ap_lin_adc_change(0, pre_chan);
		}
		pre_chan |= chan;
		chan = pre_chan;
	}
	else
	{
		pre_chan &= ~chan;
		if (AUDIO_ALL_CHAN == chan )
		{
			pre_chan = 0;
		}
	}
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_lin_adc %d,%x\r\n", en, chan);

	_sprd_codec_ap_lin_adc_change(en, chan);
}
#if 0
LOCAL void _sprd_codec_ap_stereo_open_state(uint32 en, uint32 *p_l_val, uint32 *p_r_val)
{
#if 0
	uint32 reg1,reg2 ;
	uint32 l_val = 0;
	uint32 r_val = 0;
	reg1 = SPRD_CODEC_AP_REG_GET((uint32)(&(audif_ptr->ANA_CFG10)));
	reg2 = SPRD_CODEC_AP_REG_GET((uint32)(&(audif_ptr->ANA_CFG11)));

	if( ((reg1 & DAC_ADCPGAR_BUF_AOR) ) )  //DAC_ADCPGAR_BUFN_SPN
	{
		r_val |= ADC_PGAR2_EN; //ADC_PGAR_BUFN_EN
	}
	if( ((reg1 & DAC_ADCPGAR_BUF_AOL) )  //DAC_ADCPGAR_BUFP_SPP
			|| ((reg2 & DAC_ADCPGAR_BUF_HPR) ) //DAC_ADCPGAR_BUFP_HPR
			|| ((reg2 & DAC_ADCPGAR_BUF_HPL) ) //DAC_ADCPGAR_BUFP_HPL
	  )
	{
		r_val |= ADC_PGAR1_EN; //ADC_PGAR_BUFP_EN
	}
	if( ((reg1 & DAC_ADCPGAL_BUF_AOR) ) //DAC_ADCPGAL_BUFN_SPN
			|| ((reg2 & DAC_ADCPGAL_BUF_HPR) ) //DAC_ADCPGAL_BUFN_HPR
	  )
	{
		l_val |= ADC_PGAL2_EN;  //ADC_PGAL_BUFN_EN
	}
	if( ((reg1 & DAC_ADCPGAL_BUF_AOL) ) //DAC_ADCPGAL_BUFP_SPP
			|| ((reg2 & DAC_ADCPGAL_BUF_HPL) ) //DAC_ADCPGAL_BUFP_HPL
	  )
	{
		l_val |= ADC_PGAL1_EN; //ADC_PGAL_BUFP_EN
	}
	if( 0 == en )
	{
		l_val = (~l_val & (ADC_PGAL1_EN | ADC_PGAL2_EN));  //(ADC_PGAL_BUFP_EN | ADC_PGAL_BUFN_EN)
		r_val = (~r_val & (ADC_PGAR1_EN | ADC_PGAR2_EN)); //(ADC_PGAR_BUFP_EN | ADC_PGAR_BUFN_EN)
	}
	*p_l_val = l_val;
	*p_r_val = r_val;
#endif
}
#endif
PUBLIC void __sprd_codec_ap_adc_pga_buf_pwr(uint32 en, uint32 chan)
{
#if 0
	uint32 l_val;
	uint32 r_val;
	_sprd_codec_ap_stereo_open_state(en, &l_val, &r_val);
	_sprd_codec_ap_chan_en(en, AUDIO_ALL_CHAN, &(audif_ptr->ANA_CFG5), l_val, r_val);
#endif
}



PUBLIC void __sprd_codec_ap_lin(uint32 en, uint32 chan)
{
	uint32 l_val;
	uint32 r_val;
	//    SPRD_CODEC_AP_PRINT("__sprd_codec_ap_lin %d", en);
	//   __sprd_codec_ap_lin_adc(en, chan);
	l_val = (AIL_ADCL);
	r_val = (AIR_ADCR);
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG6,l_val,r_val);

}


PUBLIC void __sprd_codec_ap_ear_mixer(uint32 en)
{
	uint32 l_val;
	l_val = (DACL_EAR);
	_sprd_codec_ap_chan_en(en,AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG11,l_val,0);
}


PUBLIC void __sprd_codec_ap_lout_mixer(uint32 en, uint32 chan)
{
	uint32 l_val=0;
	uint32 r_val=0;
	l_val = (DACL_AOL);
	//    r_val = (DACR_AOL);
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG10,l_val,r_val);
}

PUBLIC void __sprd_codec_ap_lout_mixer_adcbuf(uint32 en, uint32 chan)
{
	uint32 l_val=0;
	uint32 r_val=0;
	l_val = (ADCL_AOL);
	//    r_val = (ADCR_AOL);
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG10,l_val,r_val);
	__sprd_codec_ap_adc_pga_buf_pwr(en, chan);
}

PUBLIC void __sprd_codec_ap_aol_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG12,AOL_EN);
}
PUBLIC void __sprd_codec_ap_aor_mixer(uint32 en,uint32 chan)
{
	uint32 l_val=0;
	uint32 r_val=0;
	//    l_val = (DACL_AOR);
	r_val = (DACR_AOR);
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG10,l_val,r_val);
}
PUBLIC void __sprd_codec_ap_aor_mixer_adcbuf(uint32 en,uint32 chan)
{
	uint32 l_val=0;
	uint32 r_val=0;
	//    l_val = (ADCL_AOR);
	r_val = (ADCR_AOR);
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG10,l_val,r_val);
	__sprd_codec_ap_adc_pga_buf_pwr(en,chan);
}
PUBLIC void __sprd_codec_ap_aor_en(uint32 en)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_aor_en %d",en);
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG12,AOR_EN);
}


PUBLIC void __sprd_codec_ap_ear(uint32 en)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_ear %d", en);
	__sprd_codec_ap_ear_en(en);
	//   __sprd_codec_ap_ear_mixer(en);
}


PUBLIC void __sprd_codec_ap_lout(uint32 en)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_lout %d", en);
	__sprd_codec_ap_pa_sw_en(en);
	__sprd_codec_ap_aol_en(en);
}


PUBLIC void __sprd_codec_ap_ear_en(uint32 en)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_ear_en %d", en);
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG12,EAR_EN);
}
PUBLIC void __sprd_codec_ap_hp_en(uint32 en, uint32 chan)
{
	_sprd_codec_ap_chan_en(en,AUDIO_ALL_CHAN,(void*)AUD_AP_ANA_CFG12,HPL_EN,HPR_EN);
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG12,DIFF_EN);
}

PUBLIC void __sprd_codec_ap_hp_mixer(uint32 en, uint32 chan)
{
	uint32 l_val;

	if( AUDIO_IS_CHAN_L_ONLY(chan) )
	{
		l_val = (DACL_P_HPL | DACL_N_HPR);
	}
	else if( AUDIO_IS_CHAN_R_ONLY(chan) )
	{
		l_val = (DACR_P_HPL | DACR_P_HPR);
	}
	else
	{
		l_val = (DACL_P_HPL | DACR_P_HPR);
	}
	_sprd_codec_ap_chan_en(en,AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG11,l_val,0);
}


PUBLIC void __sprd_codec_ap_hp_mixer_adcbuf(uint32 en, uint32 chan)
{
	uint32 rj217 = (chan >> SPRD_CODEC_RJ217_SHIFT)&SPRD_CODEC_RJ217_MASK;
	uint32 l_val;

	if( AUDIO_IS_CHAN_L_ONLY(chan) )
	{
		l_val = (ADCL_P_HPL | ADCL_N_HPR);
	}
	else if( AUDIO_IS_CHAN_R_ONLY(chan) )
	{
		l_val = (ADCR_P_HPL | ADCR_P_HPR);
	}
	else
	{
		if( rj217 )
		{
			l_val = (ADCR_P_HPL | ADCL_N_HPR);
		}
		else
		{
			l_val = (ADCL_P_HPL | ADCR_P_HPR);
		}
	}
	_sprd_codec_ap_chan_en(en,AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG11,l_val,0);
	__sprd_codec_ap_adc_pga_buf_pwr(en, chan);
}


PUBLIC void __sprd_codec_ap_hp(uint32 en, uint32 chan)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_hp %d", en);
	__sprd_codec_ap_hp_en(en, chan);

	__sprd_codec_ap_hp_pop(HP_POP_CTL_DIS);
}

PUBLIC uint32 __sprd_codec_ap_hp_pop_en(uint32 en)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_hp_pop_en %d", en);
	__sprd_codec_ap_drv_clk_en(1);
	if( en )
	{
		__sprd_codec_ap_hp_pop(HP_POP_CTL_UP);
	}
	else
	{
		__sprd_codec_ap_hp_pop(HP_POP_CTL_HOLD);
		__sprd_codec_ap_hp_pop(HP_POP_CTL_DOWN);
	}
	return 1;
}

PUBLIC uint32 __sprd_codec_ap_hp_ramp_flg(void)
{
	uint32 val ;
	val = SPRD_CODEC_AP_REG_GET(AUD_AP_ANA_STS0);
	return ((val&HP_POP_FLG_MASK)>>HP_POP_FLG);
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
		__sprd_codec_ap_drv_clk_en(0);
	}
}

PUBLIC void __sprd_codec_ap_mic_bias(uint32 en)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_mic_bias %d", en);
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG1,MICBIAS_EN);
	s_auto_mic_bias_dis = en;
}


PUBLIC void __sprd_codec_ap_auxmic_bias(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG1,AUXMICBIAS_EN);
}

PUBLIC void __sprd_codec_ap_headmic_bias(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG0,HEADMICBIAS_EN);
}
PUBLIC void __sprd_codec_ap_main_mic(uint32 en, uint32 chan)
{
	uint32 l_val;
	uint32 r_val;
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_main_mic %d", en);
	l_val = (MIC_ADCL);
	r_val = (MIC_ADCR);
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG6,l_val,r_val);
	_sprd_codec_ap_auto_mic_bias(en, SPRD_CODEC_MAIN_MIC);
}


PUBLIC void __sprd_codec_ap_aux_mic(uint32 en, uint32 chan)
{
	uint32 l_val;
	uint32 r_val;
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_aux_mic %d", en);
	l_val = (AUXMIC_ADCL);
	r_val = (AUXMIC_ADCR);
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG6,l_val,r_val);
	__sprd_codec_ap_auxmic_bias(en);
	//_sprd_codec_ap_auto_mic_bias(en,SPRD_CODEC_AUX_MIC);
}


PUBLIC void __sprd_codec_ap_hp_mic(uint32 en, uint32 chan)
{
	uint32 l_val;
	uint32 r_val;
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_hp_mic %d", en);
	l_val = (HEADMIC_ADCL);
	r_val = (HEADMIC_ADCR);
	_sprd_codec_ap_chan_en(en,chan,(void*)AUD_AP_ANA_CFG6,l_val,r_val);
	__sprd_codec_ap_headmic_bias(en);
}


PUBLIC void __sprd_codec_ap_drv_clk_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG4,DRV_CLK_EN);
}

PUBLIC void __sprd_codec_ap_rev_clk_en(uint32 en)
{
	//_sprd_codec_ap_en_op(en, &(audif_ptr->ANA_CFG4), AUD_CLK_REVERSE);
}

PUBLIC void __sprd_codec_ap_dac_clk_en(uint32 en)
{
	SPRD_CODEC_AP_PRINT("__sprd_codec_ap_dac_clk_en %d", en);
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG4,DAC_CLK_EN);
}
PUBLIC void __sprd_codec_ap_dac_clk_fs_sel(uint32 fs)
{
	SPRD_CODEC_AP_PRINT("%s %s %s %d",__FILE__,__FUNCTION__,__LINE__, fs);
	SPRD_CODEC_AP_REG_BF_SET_VAL(AUD_AP_ANA_CFG4, fs, DAC_CLK_F, DAC_CLK_F_MASK);	
}
PUBLIC void __sprd_codec_ap_adc_clk_fs_sel(uint32 fs)
{
	SPRD_CODEC_AP_PRINT("%s %s %s %d",__FILE__,__FUNCTION__,__LINE__, fs);
	SPRD_CODEC_AP_REG_BF_SET_VAL(AUD_AP_ANA_CFG4, fs, ADC_CLK_F, ADC_CLK_F_MASK);
}
PUBLIC void __sprd_codec_ap_adc_clk_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG4,ADC_CLK_EN);
}

PUBLIC void __sprd_codec_ap_adc_clk_rst(void)
{
	_sprd_codec_ap_en_op(1,(void*)AUD_AP_ANA_CFG4,ADC_CLK_RST);

	_sprd_codec_ap_en_op(0,(void*)AUD_AP_ANA_CFG4,ADC_CLK_RST);
}

PUBLIC void __sprd_codec_ap_hp_pop(uint32 ctl)
{
	_sprd_codec_ap_chan_mask(AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG18,ctl
			, 0, 0
			,HP_POP_CTL_MASK,HP_POP_CTL 
			);
}

PUBLIC void __sprd_codec_ap_hp_pop_step(uint32 step)
{
	_sprd_codec_ap_chan_mask(AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG18,step
			, 0, 0
			,HP_POP_STEP_MASK,HP_POP_STEP 
			);
}

PUBLIC void __sprd_codec_ap_pa_sw_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG0,PA_SW_EN);
}

PUBLIC void __sprd_codec_ap_pa_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG0,PA_EN);
}

PUBLIC void __sprd_codec_ap_pa_ldo_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG0,PA_LDO_EN);
	__sprd_codec_ap_pa_sw_en(0);
}


PUBLIC void __sprd_codec_ap_pa_ldo_v_sel(uint32 v_sel)
{
	_sprd_codec_ap_chan_mask(AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG2,v_sel
			, 0, 0
			,PA_LDO_V_MASK,PA_LDO_V
			);
}

PUBLIC void __sprd_codec_ap_pa_d_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_ANA_CFG12,PA_D_EN);
}

PUBLIC void __sprd_codec_ap_pa_dtri_f_sel(uint32 freq)
{
	_sprd_codec_ap_chan_mask(AUDIO_CHAN_L,(void*)AUD_AP_ANA_CFG12,freq
			, 0, 0
			,PA_DTRI_F_MASK,PA_DTRI_F
			);
}


PUBLIC void __sprd_codec_ap_int_set(uint32 en, uint32 mask)
{
	//_sprd_codec_ap_en_op(en,AUD_AP_DIG_CFG4,mask);
}

PUBLIC void __sprd_codec_ap_int_clr(uint32 mask)
{
	_sprd_codec_ap_en_op(1,(void*)AUD_AP_DIG_CFG3,mask);
}

PUBLIC uint32 __sprd_codec_ap_int_raw(void)
{
	return SPRD_CODEC_AP_REG_GET(AUD_AP_DIG_STS0);
}

PUBLIC uint32 __sprd_codec_ap_int_mask(void)
{
	return SPRD_CODEC_AP_REG_GET(AUD_AP_DIG_STS1);
}

PUBLIC uint32 __sprd_codec_ap_ngc_l(void)
{
	return  (SPRD_CODEC_AP_REG_GET(AUD_AP_ANA_CFG22)& AUD_DAC_NG_L);
}

PUBLIC uint32 __sprd_codec_ap_ngc_r(void)
{
	return  (SPRD_CODEC_AP_REG_GET(AUD_AP_ANA_CFG23)& AUD_DAC_NG_R);
}

PUBLIC uint32 __sprd_codec_ap_ngc_en(uint32 en)
{
	_sprd_codec_ap_en_op(en, (void*)AUD_AP_ANA_CFG14, AUD_NG_EN);
	_sprd_codec_ap_en_op(en,(void*) AUD_AP_ANA_CFG14, AUD_NG_PA_EN);
	_sprd_codec_ap_en_op(en,(void*) AUD_AP_ANA_CFG14, AUD_NG_DA_EN);
	return 0;
}

PUBLIC void __sprd_codec_ap_loop_en(uint32 en)
{
	_sprd_codec_ap_en_op(en,(void*)AUD_AP_DIG_CFG0, AUDIO_ANA_LOOP_EN);
#if 0
	if (en)
		ANA_REG_OR(REG_ANA_ARM_CLK_EN,  BIT_ANA_CLK_AUD_LOOP_EN);  //SET  BITS, fix me  ????
	else
		ANA_REG_BIC(REG_ANA_ARM_CLK_EN, BIT_ANA_CLK_AUD_LOOP_EN);//CLEAR BITS  fix me ?????
#endif
}

PUBLIC void __sprd_codec_ap_reg_print(void)
{
	uint32 reg = SPRD_CODEC_AP_REG_BASE;
	uint32 offset = 0;

	for(;offset<0xDC;offset+=0x10)
	{
		SCI_TRACE_LOW("ye.zhang:codec  0x%02x | 0x%02x 0x%02x 0x%02x 0x%02x\n",offset,
				SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x0)),
				SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x4)),
				SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x8)),
				SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0xc)));
	}


	reg = 0x82001800;
	offset=0;
	SCI_TRACE_LOW("ye.zhang:glb codec  0x%02x | 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",offset,
			SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x0)),
			SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x4)),
			SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x8)),
			SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0xc)),
			SPRD_CODEC_AP_REG_GET((void*)(reg+offset+0x8c)));

}

PUBLIC void __sprd_codec_ap_bg_ibais_en(uint32 en)
{
	// _sprd_codec_ap_en_op(en,&(audif_ptr->pmur1),BG_IBIAS_EN);
	//SPRD_CODEC_BG_I_SLP((uint32)(!en));
}

PUBLIC void __sprd_codec_ap_mic_bias_ldo_set(uint32 en)
{
	// _sprd_codec_ap_auto_mic_bias(en,SPRD_CODEC_LDO_MIC);
}

PUBLIC void __sprd_codec_ap_mic_bias_ldo_v_sel(uint32 v_sel)
{
	//SPRD_CODEC_AP_PRINT("__sprd_codec_ap_mic_bias_ldo_v_sel %d",v_sel);
	/*
	   _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->pmur3),v_sel
	   ,0,0
	   ,MICBIAS_V_MASK,MICBIAS_V_SHIFT
	   );
	   */
}

PUBLIC void __sprd_codec_ap_pa_demi_en(uint32 en)
{
	//SPRD_CODEC_AP_PRINT:"__sprd_codec_ap_pa_demi_en %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_AP_PHY_V0_770_112_2_17_23_2_55_220,(uint8*)"d",en);
	//_sprd_codec_ap_en_op(en,&(audif_ptr->dcr2),PA_DEMI_EN);
	//_sprd_codec_ap_en_op(en,&(audif_ptr->dcr3),AUD_DRV_OCP_LO_PD);
	//_sprd_codec_ap_en_op(en,&(audif_ptr->dcr3),AUD_DRV_OCP_PA_PD);
}

PUBLIC void __sprd_codec_ap_pa_pga_gain(uint32 val)
{
#if 0
#if (SPKR_REPLACE_SPKL)
	__sprd_codec_ap_par_pga_gain(val);
#else
	_sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->dcgr3),val
			,0,0
			,DAC_PA_G_MASK,DAC_PA_G_SHIFT
			);
#if (SPKR_OUTPUT_SAME_AS_SPKL)
	__sprd_codec_ap_par_pga_gain(val);
#endif
#endif
#endif
}

PUBLIC uint32 __sprd_codec_update_vb_vbo_vcm_vol(uint32 cur_vol)
{
	return 0;
	/*
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
	   */
}

PUBLIC uint32 __sprd_codec_update_pa_ldo_vol(uint32 cur_vol)
{
	return 0;
	/*
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
	   */
}

PUBLIC void __sprd_codec_ap_adc_pga_gain_set(uint32 chan,uint32 val)
{
	return;
	/*
	   _sprd_codec_ap_chan_mask(chan,&(audif_ptr->acgr),val
	   ,ADC_PGAR_G_MASK,ADC_PGAR_G_SHIFT
	   ,ADC_PGAL_G_MASK,ADC_PGAL_G_SHIFT
	   );
	   */
}

PUBLIC void __sprd_codec_ap_hp_pga_gain_set(uint32 chan,uint32 val)
{
	return;
	/*
	   _sprd_codec_ap_chan_mask(chan,&(audif_ptr->dcgr1),val
	   ,DAC_HPR_G_MASK,DAC_HPR_G_SHIFT
	   ,DAC_HPL_G_MASK,DAC_HPL_G_SHIFT
	   );
	   */
}

PUBLIC void __sprd_codec_ap_ear_pga_gain_set(uint32 val)
{
	return;
	/*
	   _sprd_codec_ap_chan_mask(AUDIO_CHAN_L,&(audif_ptr->dcgr2),val
	   ,0,0
	   ,DAC_EAR_G_MASK,DAC_EAR_G_SHIFT
	   );
	   */
}

PUBLIC void __sprd_codec_ap_pa_pga_gain_set(uint32 chan,uint32 val)
{
	return;
	/*
#if (SPKR_REPLACE_SPKL)
_sprd_codec_ap_chan_mask(chan,&(audif_ptr->dcgr3),val
,DAC_PA_G_MASK,DAC_PA_G_SHIFT
,DAC_PAR_G_MASK,DAC_PAR_G_SHIFT
);
#else
#if (SPKR_OUTPUT_SAME_AS_SPKL)
if (chan == AUDIO_CHAN_R)
return;
chan = AUDIO_ALL_CHAN;
#endif
_sprd_codec_ap_chan_mask(chan,&(audif_ptr->dcgr3),val
,DAC_PAR_G_MASK,DAC_PAR_G_SHIFT
,DAC_PA_G_MASK,DAC_PA_G_SHIFT
);
#endif
*/
}

PUBLIC uint32 __sprd_codec_ap_adc_pga_gain_get(uint32 chan)
{
	return 0;
	/*
	   if(AUDIO_IS_CHAN_L(chan))
	   {
	   return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->acgr)) & ADC_PGAL_G_MASK)>>ADC_PGAL_G_SHIFT);
	   }
	   else if(AUDIO_IS_CHAN_R(chan))
	   {
	   return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->acgr)) & ADC_PGAR_G_MASK)>>ADC_PGAR_G_SHIFT);
	   }
	   return 0;
	   */
}

PUBLIC uint32 __sprd_codec_ap_hp_pga_gain_get(uint32 chan)
{
	/*
	   if(AUDIO_IS_CHAN_L(chan))
	   {
	   return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr1)) & DAC_HPL_G_MASK)>>DAC_HPL_G_SHIFT);
	   }
	   else if(AUDIO_IS_CHAN_R(chan))
	   {
	   return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr1)) & DAC_HPR_G_MASK)>>DAC_HPR_G_SHIFT);
	   }
	   */
	return 0;
}

PUBLIC uint32 __sprd_codec_ap_ear_pga_gain_get(void)
{
	return 0;
	//return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr2)) & DAC_EAR_G_MASK)>>DAC_EAR_G_SHIFT);
}

PUBLIC uint32 __sprd_codec_ap_pa_pga_gain_get(uint32 chan)
{
#if 0
#if (SPKR_REPLACE_SPKL)
	if(AUDIO_IS_CHAN_L(chan))
	{
		return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr3)) & DAC_PAR_G_MASK)>>DAC_PAR_G_SHIFT);
	}
	else if(AUDIO_IS_CHAN_R(chan))
	{
		return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr3)) & DAC_PA_G_MASK)>>DAC_PA_G_SHIFT);
	}
#else
#if (SPKR_OUTPUT_SAME_AS_SPKL)
	chan = AUDIO_CHAN_L;
#endif
	if(AUDIO_IS_CHAN_L(chan))/*lint !e774*/
	{
		return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr3)) & DAC_PA_G_MASK)>>DAC_PA_G_SHIFT);
	}
	else if(AUDIO_IS_CHAN_R(chan))/*lint !e774*/
	{
		return ((SPRD_CODEC_AP_REG_GET(&(audif_ptr->dcgr3)) & DAC_PAR_G_MASK)>>DAC_PAR_G_SHIFT);
	}
#endif
#endif
	return 0;
}

#if 0
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
#if (SPKR_OUTPUT_SAME_AS_SPKL)
	s_sprd_codec_pga_setting.pga_setting_1.pa = val;
#endif
	s_sprd_codec_pga_setting.is_mute.pa = 0;
	__sprd_codec_pga_adj_setting();
}
#endif

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

PUBLIC void __sprd_codec_ap_adc_mute(void)
{
#if (PGA_SMOOTH_ADJUST)
	__sprd_codec_ap_adc_pga_gain(AUDIO_ALL_CHAN,ADC_PGA_G_MUTE_SET);
	s_sprd_codec_pga_setting.is_mute.adcpga= 1;
#else
	__sprd_codec_ap_adc_pga_gain(AUDIO_ALL_CHAN,ADC_PGA_G_MUTE_SET);
#endif
}

