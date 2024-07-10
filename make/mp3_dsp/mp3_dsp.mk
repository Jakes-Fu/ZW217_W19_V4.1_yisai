MP3_ARM_DECODER_SUPPORT = FALSE
MP3_DSP_6600L_PATCH_ENABLED = FALSE

ifeq ($(strip $(PLATFORM)), SC8800H)
MP3_ARM_DECODER_SUPPORT = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MP3_ARM_DECODER_SUPPORT = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
MP3_DSP_6600L_PATCH_ENABLED = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
MP3_DSP_6600L_PATCH_ENABLED = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MP3_DSP_6600L_PATCH_ENABLED = FALSE
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MP3_DSP_6600L_PATCH_ENABLED = FALSE
endif

ifeq ($(strip $(MP3_ARM_DECODER_SUPPORT)), TRUE)
MCFLAG_OPT   	+=  -Otime -zc -fx -DMP3_ARM_DECODER -DFPM_64BIT -D__ASO__
endif

ifeq ($(strip $(MP3_DSP_6600L_PATCH_ENABLED)), TRUE)
MCFLAG_OPT   	+=  -DMP3_DSP_6600L_PATCH
endif

#MCFLAG_OPT   	+=-DMP3_ASSERT_MODE

MINCPATH   		= MS_Ref/source/codec/mp3_dsp/inc MS_Ref/source/codec/mp3_dsp/src MS_Ref/export/inc \
					MS_Ref/source/aud_exp/eq/inc MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc \
					MS_Ref/source/codec/mp3_dsp/mp3_demux/inc MS_Ref/source/codec/mp3_dsp 
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc
MINCPATH +=$(L1_INCPATH) 	

ifeq ($(strip $(MP3_ARM_DECODER_SUPPORT)),TRUE)
MINCPATH        += MS_Ref/source/codec/mp3_dsp/mp3_arm_decoder/inc
endif


MSRCPATH		= MS_Ref/source/codec/mp3_dsp MS_Ref/source/codec/mp3_dsp/mp3_demux/src


ifeq ($(strip $(MP3_ARM_DECODER_SUPPORT)), TRUE)
MSRCPATH    += MS_Ref/source/codec/mp3_dsp/mp3_arm_decoder/src MS_Ref/source/codec/mp3_dsp/mp3_arm_decoder/asm
endif

SOURCES			= mp3_dsp_adp.c  mp3_demux.c mp3_dmx_bit.c mp3_dmx_data.c \
                                        mp3_dmx_stream.c mp3_dmx_format.c mp3_decoder_proxy.c

ifeq ($(strip $(MP3_ARM_DECODER_SUPPORT)), TRUE)
SOURCES    += mp3_bit.c mp3_dec_api.c mp3_fixed.c mp3_frame.c mp3_huffman.c \
                        mp3_layer3.c mp3_layer12.c mp3_stream.c mp3_synth.c \
                        dctiv.s  dct32.s    III_win_overlap_0.s  mp3_dec_asm_config.s \
                        mp3_dec_quantize.s synth_full.s mp3_dec_bit_operate.s 
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
SOURCES    += mp3_dsp_layer1_cmd_interface.c
endif

ifeq ($(strip $(PLATFORM)), SC6530)
SOURCES    += mp3_dsp_layer1_cmd_interface.c
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES    += mp3_dsp_layer1_cmd_interface.c
endif