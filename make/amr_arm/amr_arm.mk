


MCFLAG_OPT   	= -Otime


ifeq ($(strip $(CHIP_ENDIAN)),BIG)
MCFLAG_OPT   	+= -DAMR_DEC_ARM_BE
endif

#MCFLAG_OPT   	+= -DAMR_ASSERT_MODE


ifeq ($(strip $(PLATFORM)), SC6800H)
MCFLAG_OPT		+=-DPRODUCT_SC6800H
endif
MINCPATH =  BASE/Layer1/source/h MS_Ref/source/codec/amr/inc MS_Ref/export/inc MS_Ref/source/codec/record/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc	
MINCPATH +=MS_Ref/source/codec/aud_common/stream/inc
MINCPATH +=MS_Ref/source/codec/aud_common/audio_output/inc
MINCPATH +=MS_Ref/source/codec/aud_common/inc
MINCPATH +=MS_Ref/source/codec/aud_common/frame_buffer/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_demux/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_decode/inc
MINCPATH +=MS_Ref/source/codec/demux/amr/inc
MINCPATH +=MS_Ref/source/codec/decode/amr_arm/inc
MINCPATH +=BASE/Layer1/source/h BASE/ps/export/inc
MINCPATH +=MS_Ref/source/codec/decode/amr_arm/decoder/c_code

MSRCPATH =  MS_Ref/source/codec/adapter/src
MSRCPATH +=MS_Ref/source/codec/stream/src
MSRCPATH +=MS_Ref/source/codec/audio_output/src
MSRCPATH +=MS_Ref/source/codec/common/src
MSRCPATH +=MS_Ref/source/codec/common/frame_buffer/src
MSRCPATH +=MS_Ref/source/codec/demux/amr/src
MSRCPATH +=MS_Ref/source/codec/decode/amr_arm/src
MSRCPATH +=MS_Ref/source/codec/decode/amr_arm/decoder/c_code

SOURCES				=    AMR_DEC_agc.c          AMR_DEC_b_cn_cod.c   AMR_DEC_basicop2.c \
						    AMR_DEC_bgnscd.c    AMR_DEC_bits2prm.c	 AMR_DEC_c_g_aver.c   AMR_DEC_copy.c \
						    AMR_DEC_count.c      AMR_DEC_d_gain_c.c    AMR_DEC_d_gain_p.c   AMR_DEC_d_homing.c \
						    AMR_DEC_d_plsf.c     AMR_DEC_d_plsf_3.c     AMR_DEC_d_plsf_5.c    AMR_DEC_d1035pf.c \
						    AMR_DEC_d2_11pf.c   AMR_DEC_d2_9pf.c          AMR_DEC_d3_14pf.c \
						    AMR_DEC_d4_17pf.c   AMR_DEC_d8_31pf.c    AMR_DEC_dec_amr.c    AMR_DEC_dec_gain.c \
						    AMR_DEC_dec_lag3.c  AMR_DEC_dec_lag6.c   AMR_DEC_Decoder.c     AMR_DEC_dtx_dec.c \
						    AMR_DEC_ec_gains.c  AMR_DEC_ex_ctrl.c      AMR_DEC_gc_pred.c     AMR_DEC_gmed_n.c \
						    AMR_DEC_int_lpc.c     AMR_DEC_int_lsf.c        AMR_DEC_inv_sqrt.c     AMR_DEC_log2.c \
						    AMR_DEC_lsp_avg.c    AMR_DEC_lsp_avg.c           AMR_DEC_lsp_lsf.c \
						    AMR_DEC_mac_32.c   AMR_DEC_oper_32b.c   AMR_DEC_ph_disp.c      AMR_DEC_post_pro.c \
						    AMR_DEC_pow2.c             AMR_DEC_preemph.c   AMR_DEC_prm2bits.c \
						    AMR_DEC_pstfilt.c       AMR_DEC_r_fft.c           AMR_DEC_reorder.c    \
						    AMR_DEC_set_zero.c   AMR_DEC_sp_dec.c      AMR_DEC_sqrt_l.c        AMR_DEC_strfunc.c \
						    AMR_DEC_strfunc.c          AMR_DEC_ton_stab.c    AMR_DEC_vad1.c \
						    AMR_DEC_vad2.c        AMR_DEC_vadname.c   AMR_DEC_a_refl.c        AMR_DEC_weight_a.s \
						    AMR_DEC_bgnscd_calc.s  AMR_DEC_lsp_az.s    AMR_DEC_norm_s.s    AMR_DEC_ph_disp_calc.s \
						    AMR_DEC_post_process.s AMR_DEC_pred_lt.s   AMR_DEC_preemph_calc.s AMR_DEC_residu.s \
						    AMR_DEC_syn_filt.s     AMR_DEC_agc_calc.s
ifeq ($(strip $(AMR_WB_SUPPORT)), TRUE) 
SOURCES				+= awb_dec_tab.c					awb_dec_qpisf_2s.c			awb_dec_qisf_ns.c		\
								awb_dec_q_pulse.c			awb_dec_p_med_ol.c			awb_dec_math_op.c		\
								awb_dec_lagconc.c			awb_dec_isp_az.c				awb_dec_int_lpc.c		\
								awb_dec_homing.c			awb_dec_dtx.c						awb_dec_d_gain2.c		\
								awb_dec_d4t64fx.c			awb_dec_asm_config.s		awb_dec_arm_stream_parse.s \
								awb_dec_arm_fun.s			awb_dec_arm_filter_f.s	awb_dec_arm_filter.s	\
								awb_dec_arm_basic_op.s awb_dec_decoder.c
endif
SOURCES                              +=amr_arm_adp.c amr_decode.c amr_demux.c

						    
