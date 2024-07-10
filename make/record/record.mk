
MCFLAG_OPT   	= -Otime

ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT   	+= -D_RECORD_SC6600H_ -D_RECORD_SC6600L
endif


RECORD_MP3_SUPPORT = TRUE
RECORD_AMR_SUPPORT = TRUE

ifeq ($(strip $(PLATFORM)), SC8800G)
RECORD_MP3_SUPPORT = TRUE
RECORD_AMR_SUPPORT = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
RECORD_MP3_SUPPORT = TRUE
RECORD_AMR_SUPPORT = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC6530)
#RECORD_MP3_SUPPORT = TRUE
#RECORD_AMR_SUPPORT = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
#RECORD_MP3_SUPPORT = TRUE
#RECORD_AMR_SUPPORT = TRUE
endif

ifeq ($(strip $(RECORD_MP3_SUPPORT)), TRUE)
MCFLAG_OPT  += -DREC_MP3_SUPPORT
endif

ifeq ($(strip $(RECORD_AMR_SUPPORT)), TRUE)
MCFLAG_OPT  += -DREC_AMR_SUPPORT
endif

MINCPATH =  MS_Ref/source/codec/record/inc MS_Ref/export/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export \
 MS_Ref/source/audioservice/inc BASE/ps/export/inc
MINCPATH += chip_drv/export/inc/outdated
MINCPATH +=$(L1_INCPATH) 

ifeq ($(strip $(RECORD_MP3_SUPPORT)), TRUE)
MINCPATH  += MS_Ref/source/codec/record/src/mp3_encoder/inc
endif

ifeq ($(strip $(RECORD_AMR_SUPPORT)), TRUE)
MINCPATH  += MS_Ref/source/codec/record/src/amr_encoder/inc
endif
	
MSRCPATH =  MS_Ref/source/codec/record/src

ifeq ($(strip $(RECORD_MP3_SUPPORT)), TRUE)
MSRCPATH  += MS_Ref/source/codec/record/src/mp3_encoder/src
endif

ifeq ($(strip $(RECORD_AMR_SUPPORT)), TRUE)
MSRCPATH  += MS_Ref/source/codec/record/src/amr_encoder/src
endif


ifeq ($(strip $(RECORD_MP3_SUPPORT)), TRUE)
ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MASMFLAG_OPT += -PD "MP3_ENC_ASM_LITTLE_ENDIAN SETA 1"
MCFLAG_OPT   += -DLIT_END 
else
MASMFLAG_OPT += -PD "MP3_ENC_ASM_LITTLE_ENDIAN SETA 0"
endif
endif

SOURCES  = adpcm_encoder.c adpcmrecord.c adpcmrecord_adp.c

ifeq ($(strip $(RECORD_MP3_SUPPORT)), TRUE)
SOURCES  +=  mp3_enc_alloc.c  mp3_enc_analysis_filter.c  mp3_enc_analysis_filter_tab.c \
                        mp3_enc_basic.c  mp3_enc_bit_rate_control.c  mp3_enc_bit_stream.c \
                        mp3_enc_bit_write.c  mp3_enc_block_select.c  mp3_enc_block_switch.c \
                        mp3_enc_encoder.c  mp3_enc_filter_bank.c \
                        mp3_enc_HCB.c  mp3_enc_log2.c  mp3_enc_mdct.c mp3_enc_mdct_table.c \
                        mp3_enc_psy.c  mp3_enc_quantizer.c mp3_enc_RD_loop.c \
                        mp3_enc_RD_optimize.c  mp3_enc_reform.c  mp3_enc_spec_idx.c \
                        mp3_enc_spec_scale.c  mp3_enc_stereo.c  \
                        mp3_arm_tf_converter_asm.s  mp3_arm_window_asm.s  mp3_enc_block_asm.s \
                        mp3_enc_huff_asm.s  mp3_enc_psy_asm.s  mp3_enc_quantize_asm.s                       
endif

ifeq ($(strip $(RECORD_AMR_SUPPORT)), TRUE)
SOURCES  += AMR_ENC_agc.c  AMR_ENC_basicop2.c  AMR_ENC_bits2prm.c  AMR_ENC_c1035pf.c  AMR_ENC_c2_11pf.c  \
			AMR_ENC_c2_9pf.c  AMR_ENC_c8_31pf.c  AMR_ENC_calc_en.c  AMR_ENC_cbsearch.c  \
			AMR_ENC_cl_ltp.c  AMR_ENC_cod_amr.c  AMR_ENC_copy.c  AMR_ENC_count.c  AMR_ENC_dtx_enc.c  \
			AMR_ENC_Encoder.c  AMR_ENC_enc_lag3.c  AMR_ENC_enc_lag6.c  AMR_ENC_e_homing.c  AMR_ENC_gain_q.c  \
			AMR_ENC_gc_pred.c  AMR_ENC_gmed_n.c  AMR_ENC_g_adapt.c  AMR_ENC_g_code.c  AMR_ENC_g_pitch.c  \
			AMR_ENC_hp_max.c  AMR_ENC_inter_36.c  AMR_ENC_int_lpc.c  AMR_ENC_lag_wind.c  AMR_ENC_lflg_upd.c  AMR_ENC_log2.c \
			AMR_ENC_lpc.c  AMR_ENC_lsfwt.c  AMR_ENC_lsp.c  AMR_ENC_lsp_az.c  AMR_ENC_lsp_lsf.c  AMR_ENC_mac_32.c   \
			AMR_ENC_n_proc.c  AMR_ENC_ol_ltp.c  AMR_ENC_oper_32b.c  AMR_ENC_pitch_ol.c  AMR_ENC_post_pro.c  AMR_ENC_pow2.c  \
			AMR_ENC_pre_big.c  AMR_ENC_prm2bits.c AMR_ENC_p_ol_wgh.c  AMR_ENC_qgain475.c AMR_ENC_qgain795.c  AMR_ENC_qua_gain.c  \
			AMR_ENC_q_gain_c.c  AMR_ENC_q_gain_p.c  AMR_ENC_q_plsf.c  AMR_ENC_q_plsf_3.c  AMR_ENC_q_plsf_5.c  AMR_ENC_reorder.c  \
			AMR_ENC_pre_proc.c  AMR_ENC_r_fft.c  AMR_ENC_set_zero.c  AMR_ENC_sid_sync.c  AMR_ENC_spreproc.c  AMR_ENC_spstproc.c  \
			AMR_ENC_sp_enc.c    AMR_ENC_sqrt_l.c  AMR_ENC_strfunc.c AMR_ENC_ton_stab.c  AMR_ENC_vad1.c  AMR_ENC_vad2.c  AMR_ENC_vadname.c  \
			AMR_ENC_weight_a.c  AMR_ENC_vq_subvec.s  AMR_ENC_az_lsp.s  AMR_ENC_c3_14pf.s  AMR_ENC_c4_17pf.s  AMR_ENC_convolve.s   \
			AMR_ENC_inv_sqrt.s  AMR_ENC_levinson.s  AMR_ENC_pred_lt.s  AMR_ENC_residu.s  AMR_ENC_s10_8pf.s  AMR_ENC_set_sign.s
ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
SOURCES  +=  AMR_ENC_autocorr.s  AMR_ENC_calc_cor.s  AMR_ENC_cor_h.s      AMR_ENC_pitch_fr.s    AMR_ENC_syn_filt.s 
else
SOURCES  +=  AMR_ENC_autocorr_bg.s  AMR_ENC_c1035pf_buildcode_bg.s  AMR_ENC_calc_cor_bg.s  AMR_ENC_cor_h_bg.s  AMR_ENC_pitch_fr_bg.s  AMR_ENC_syn_filt_bg.s
endif
endif


