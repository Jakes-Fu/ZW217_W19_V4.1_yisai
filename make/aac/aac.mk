
MCFLAG_OPT    = -Otime -zc -D_AACARM_ 

#MCFLAG_OPT+=-DAAC_ASSERT_MODE

#MINCPATH =  MS_Ref/source/codec/aac/inc MS_Ref/source/codec/aac/src/decode_inc MS_Ref/source/codec/aac/src/demux_inc MS_Ref/export/inc \
# MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
#MSRCPATH =  MS_Ref/source/codec/aac/src MS_Ref/source/codec/aac/src/decode_src MS_Ref/source/codec/aac/src/demux_src
#
#SOURCES       = aac_adp.c \
#		aac_demux_bits.c aac_demux_interface.c aac_mp4.c aac_mp4atom.c aac_mp4ff.c aac_mp4sample.c aac_mp4tagupdate.c aac_mp4util.c id3_parse.c\
#		aac_bits.c aac_cfft_tab.c aac_common.c aac_decoder.c aac_filtbank.c aac_huffman.c aac_is.c aac_lc_iq_tns_asm.s\
#		aac_ms.c aac_pns.c aac_pulse.c aac_specrec.c aac_syntax.c aac_tns.c AAC_TNS_DecodeCoef.s AAC_IQ_fun.s
PLATFORM_6800_OR_8800H = TRUE
#PLATFORM_6800_OR_8800H = FALSE
#ifeq ($(strip $(PLATFORM)), 6800)
#PLATFORM_6800_OR_8800H = TRUE
#endif
#ifeq ($(strip $(PLATFORM)), SC8800H)
#PLATFORM_6800_OR_8800H = TRUE
#endif
#ifeq ($(strip $(PLATFORM)), SC8800G)
#PLATFORM_6800_OR_8800H = TRUE
#endif
#
#ifeq ($(strip $(PLATFORM)), UWS6121E)
##MINCPATH += chip_drv/export/inc/outdated
#endif

#ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MASMFLAG_OPT +=-PD "AAC_DEC_ASM_LITTLE_ENDIAN SETL {TRUE}"
MCFLAG_OPT   += -DAAC_DEC_LITTLE_ENDIAN -DAAC_DEMUX_LITTLENDIAN
#else
#MASMFLAG_OPT +=-PD "AAC_DEC_ASM_LITTLE_ENDIAN SETL {FALSE}"
#endif

ifeq ($(strip $(PLATFORM_6800_OR_8800H)), TRUE)
MINCPATH =  chip_drv/export/inc MS_Ref/export/inc MS_Ref/source/codec/aac/src_6800/decode_inc MS_Ref/source/codec/aac_lc/inc \
 MS_Ref/source/codec/aac/src_6800/decode_inc/codebook
MINCPATH += chip_drv/export/inc/outdated
MSRCPATH =  MS_Ref/source/codec/aac/src_6800 MS_Ref/source/codec/aac/src_6800/decode_src MS_Ref/source/codec/aac_lc/src

MCFLAG_OPT   +=-D_SC6800_ -D_ARMNINEPLATFORM_

SOURCES      =  aac_adp.c \
		aac_demux_bits.c          aac_interface.c            id3_parse.c          mp4.c                      mp4util.c \
		mp4ff.c                   mp4sample.c                mp4tagupdate.c       mp4atom.c\
		AAC_arm_math.s            aac_asm_filterbank_tab.c   aac_bits.c           aac_cfft_tab.c             aac_common.c               aac_decoder.c \
		aac_huffman.c             aac_ifiltbank.c            aac_is.c             aac_lc_iq_tns_asm.s        aac_ms.c    		aac_pns.c   \
		AAC_ps_dec.c              AAC_ps_decorrelate.c       AAC_ps_init.c        AAC_ps_mix_phase.c         AAC_ps_syntax.c            AAC_ps_table.c \
		aac_pulse.c               aac_sbr_analysis_filter.s  aac_sbr_analysis_post.s   AAC_sbr_common.c      AAC_sbr_dec.c              AAC_sbr_e_nf.c \
		AAC_sbr_fbt.c             AAC_sbr_hfadj.c            AAC_sbr_hfgen.c           AAC_sbr_huff.c        AAC_sbr_qmf.c              AAC_sbr_syntax.c   \
		aac_sbr_synthesis_dct_post.s    aac_sbr_synthesis_dct_pre.s     aac_sbr_synthesis_filter.s   AAC_sbr_table.c               AAC_sbr_tf_grid.c        aac_specrec.c \
		aac_syntax.c                    aac_tns.c                       AAC_TNS_DecodeCoef.s         aacplus_fft.s                 imdct_1024_pre.s         imdct1024.s  \
		imdct1024_post.s                imdct128.s                      long_block_only_window.s     long_block_start_window.s     long_block_window_end.s  short_block_only_window.s
endif
