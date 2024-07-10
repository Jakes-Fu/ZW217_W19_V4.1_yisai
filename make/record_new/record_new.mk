
MCFLAG_OPT   	= -Otime

RECORD_MP3_SUPPORT = TRUE
#ifeq ($(strip $(AMR_WB_SUPPORT)), TRUE)
#RECORD_AMR_SUPPORT = TRUE
#else
#RECORD_AMR_SUPPORT = FALSE
#endif

ifeq ($(strip $(PLATFORM)),UWS6121E)
RECORD_AMR_SUPPORT = TRUE
endif

ifeq ($(strip $(RECORD_MP3_SUPPORT)), TRUE)
MCFLAG_OPT  += -DREC_MP3_SUPPORT
endif

ifeq ($(strip $(RECORD_AMR_SUPPORT)), TRUE)
MCFLAG_OPT  += -DREC_AMR_SUPPORT
endif


ifeq ($(strip $(RECORD_MP3_SUPPORT)), TRUE)
ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MASMFLAG_OPT += -PD "MP3_ENC_ASM_LITTLE_ENDIAN SETA 1"
MCFLAG_OPT   += -DLIT_END
else
MASMFLAG_OPT += -PD "MP3_ENC_ASM_LITTLE_ENDIAN SETA 0"
endif
endif




MINCPATH =   MS_Ref/export/inc \
 MS_Ref/source/aud_dev/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export \
 MS_Ref/source/audioservice/inc BASE/ps/export/inc
MINCPATH += chip_drv/export/inc/outdated
MINCPATH +=$(L1_INCPATH)
MINCPATH +=MS_Ref/source/codec/aud_common/stream/inc
MINCPATH +=MS_Ref/source/codec/aud_common/audio_input/inc
MINCPATH +=MS_Ref/source/codec/aud_common/inc
MINCPATH +=MS_Ref/source/codec/aud_common/frame_buffer/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_mux/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_encode/inc
MINCPATH +=MS_Ref/source/codec/aud_source/dsp/inc
MINCPATH +=MS_Ref/source/codec/aud_source/arm/inc
MINCPATH +=MS_Ref/source/codec/mux/wave/inc
MINCPATH +=MS_Ref/source/codec/mux/mp3/inc
MINCPATH +=MS_Ref/source/codec/encode/adpcm/inc
MINCPATH +=MS_Ref/source/codec/encode/adpcm/adpcm_encoder/inc
MINCPATH +=BASE/Layer1/source/h BASE/ps/export/inc
MINCPATH +=MS_Ref/source/codec/aud_dsp_mng/inc
MINCPATH +=MS_Ref/source/codec/encode/adpcm/inc
MINCPATH +=MS_Ref/source/codec/encode/mp3/inc
MINCPATH +=MS_Ref/source/codec/encode/mp3/mp3_encoder/inc
MINCPATH +=MS_Ref/source/codec/encode/amr/inc
MINCPATH +=MS_Ref/source/codec/encode/amr/amr_encoder/inc
ifeq ($(strip $(AMR_WB_SUPPORT)), TRUE)
MINCPATH +=MS_Ref/source/codec/encode/amr/amr_wb_encoder/inc
endif




MSRCPATH =  MS_Ref/source/codec/adapter/src
MSRCPATH +=  MS_Ref/source/codec/mux/wave/src
MSRCPATH +=  MS_Ref/source/codec/mux/mp3/src
MSRCPATH +=  MS_Ref/source/codec/encode/adpcm/src
MSRCPATH +=  MS_Ref/source/codec/encode/adpcm/adpcm_encoder/src
MSRCPATH +=  MS_Ref/source/codec/aud_source/dsp/src
MSRCPATH +=  MS_Ref/source/codec/encode/adpcm/src
MSRCPATH +=  MS_Ref/source/codec/encode/mp3/src
MSRCPATH +=  MS_Ref/source/codec/aud_source/arm/src
MSRCPATH +=  MS_Ref/source/codec/encode/amr/src
MSRCPATH +=  MS_Ref/source/codec/encode/amr/amr_encoder/src
MSRCPATH +=  MS_Ref/source/codec/encode/amr/amr_encoder/src/cFile
MSRCPATH +=  MS_Ref/source/codec/encode/amr/inc
MSRCPATH +=  MS_Ref/source/codec/encode/mp3/mp3_encoder/src
MSRCPATH +=  MS_Ref/source/codec/encode/amr/amr_wb_encoder/src


SOURCES  =  record_adp.c wave_mux.c adpcm_encode.c  arm_source.c adpcm_encoder.c

ifeq ($(strip $(NXP_RECORD_SHARE_MEMORY)), TRUE)
SOURCES  +=  dsp_source_ex.c
else
SOURCES  +=  dsp_source.c
endif

ifeq ($(strip $(CVSRCD_ENABLE)), TRUE)
SOURCES  +=  record_process.c
SOURCES  +=  cvs_fifo_recd.c
endif

ifeq ($(strip $(RECORD_MP3_SUPPORT)), TRUE)
SOURCES  +=  mp3_enc_alias.c  \
 mp3_enc_alloc.c 							\
 mp3_enc_bit_rate_control.c 	\
 mp3_enc_bit_stream.c         \
 mp3_enc_bit_write.c          \
 mp3_enc_block_select.c       \
 mp3_enc_encoder.c            \
 mp3_enc_filter_bank.c        \
 mp3_enc_HCB.c                \
 mp3_enc_log2.c               \
 mp3_enc_mdct12.c             \
 mp3_enc_mdct36_LL.c          \
 mp3_enc_mdct36_LS.c          \
 mp3_enc_psy.c                \
 mp3_enc_quantizer.c          \
 mp3_enc_RD_loop.c            \
 mp3_enc_RD_optimize.c        \
 mp3_enc_spec_idx.c           \
 mp3_enc_spec_scale.c         \
 mp3_enc_stereo.c             \
 mp3_enc_subband_filter.c


SOURCES  += mp3_mux.c mp3_encode.c
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
			AMR_ENC_inv_sqrt.s  AMR_ENC_levinson.c  AMR_ENC_pred_lt.s  AMR_ENC_residu.s  AMR_ENC_s10_8pf.s  AMR_ENC_set_sign.s

ifeq ($(strip $(AMR_WB_SUPPORT)), TRUE)
SOURCES  += AMR_ENC_WB_encoder.c  agc2.c  az_isp.c  basicop2.c  bits.c  c2t64fx.c  c4t64fx.c  coder.c  cod_main.c  count.c  d2t64fx.c  d4t64fx.c\
			d_gain2.c  decim54.c  deemph.c  dtx.c  g_pitch.c  gpclip.c  homing.c  hp6k.c  hp7k.c  hp50.c  hp400.c  hp_wsp.c  int_lpc.c\
			isfextrp.c  isp_az.c  isp_isf.c  lag_wind.c  lagconc.c  levinson.c  log2.c  lp_dec2.c  math_op.c  oper_32b.c  p_med_ol.c\
			ph_disp.c  pit_shrp.c  pitch_f4.c  preemph.c  q_gain2.c  q_pulse.c  qisf_ns.c  qpisf_2s.c  random.c  util.c  voicefac.c\
			wb_vad.c  weight_a.c  autocorr.s  C4t64fx_calc.s  convolve.s  copy.s  cor_h_vec.s  cor_h_x.s  dot_product12_calc.s  filt_6k_7k.s  hp50_12k8.s\
			interpol.s  p_med_ol_calc.s  pitch_f4_asm.s  pred_lt4.s  residu.s  scale_sig.s  search_ixiy.s  sub_vq.s  syn_filt_32_asm.s\
			syn_filt_asm.s  updt_tar.s  vq_stage1.s
#			grid100.tab  ham_wind.tab  homing.tab  isp_isf.tab  lag_wind.tab  mime_io.tab  p_med_ol.tab  q_gain2.tab  qisf_ns.tab  qpisf_2s.tab
endif

ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
SOURCES  +=  AMR_ENC_autocorr.s  AMR_ENC_calc_cor.s  AMR_ENC_cor_h.s      AMR_ENC_pitch_fr.s    AMR_ENC_syn_filt.s
else
SOURCES  +=  AMR_ENC_autocorr_bg.s  AMR_ENC_c1035pf_buildcode_bg.s  AMR_ENC_calc_cor_bg.s  AMR_ENC_cor_h_bg.s  AMR_ENC_pitch_fr_bg.s  AMR_ENC_syn_filt_bg.s
endif

SOURCES  += amr_sw_encode.c
endif
