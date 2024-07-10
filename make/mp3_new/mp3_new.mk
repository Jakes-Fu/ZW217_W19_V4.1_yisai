


MCFLAG_OPT   	= -Otime


MP3_ARM_DECODER_SUPPORT = FALSE

ifeq ($(strip $(CHIP_ENDIAN)),BIG)
MCFLAG_OPT   	+= -DAMR_DEC_ARM_BE
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MP3_ARM_DECODER_SUPPORT = TRUE
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MP3_ARM_DECODER_SUPPORT = TRUE
endif

ifeq ($(strip $(MP3_ARM_DECODER_SUPPORT)), TRUE)
MCFLAG_OPT   	+=  -Otime -zc -fx -DMP3_ARM_DECODER -DFPM_64BIT -D__ASO__
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
MP3_DSP_6600L_PATCH_ENABLED = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
MP3_DSP_6600L_PATCH_ENABLED = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MP3_DSP_6600L_PATCH_ENABLED = TRUE
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MP3_DSP_6600L_PATCH_ENABLED = TRUE
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MP3_DSP_6600L_PATCH_ENABLED = FALSE
endif


ifeq ($(strip $(MP3_DSP_6600L_PATCH_ENABLED)), TRUE)
MCFLAG_OPT   	+=  -DMP3_DSP_6600L_PATCH
endif

MINCPATH =  Layer1/source/h MS_Ref/source/codec/amr/inc MS_Ref/export/inc MS_Ref/source/codec/record/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH +=$(L1_INCPATH) 	
MINCPATH +=MS_Ref/source/codec/aud_common/stream/inc
MINCPATH +=MS_Ref/source/codec/aud_common/audio_output/inc
MINCPATH +=MS_Ref/source/codec/aud_common/inc
MINCPATH +=MS_Ref/source/codec/aud_common/frame_buffer/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_demux/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_decode/inc

MINCPATH +=MS_Ref/source/codec/aud_dsp_mng/inc
MINCPATH +=MS_Ref/source/codec/decode/mp3/inc
MINCPATH +=MS_Ref/source/codec/demux/mp3/inc
MINCPATH +=MS_Ref/source/codec/adapter/inc

ifeq ($(strip $(MP3_ARM_DECODER_SUPPORT)),TRUE)
MINCPATH        += MS_Ref/source/codec/mp3_dsp/mp3_arm_decoder/inc
endif


MSRCPATH +=MS_Ref/source/codec/audio_output/src
MSRCPATH +=MS_Ref/source/codec/common/src
MSRCPATH +=MS_Ref/source/codec/common/frame_buffer/src
MSRCPATH +=MS_Ref/source/codec/decode/mp3/src
MSRCPATH +=MS_Ref/source/codec/demux/mp3/src
MSRCPATH +=MS_Ref/source/codec/adapter/src


ifeq ($(strip $(MP3_ARM_DECODER_SUPPORT)), TRUE)
MSRCPATH    += MS_Ref/source/codec/mp3_dsp/mp3_arm_decoder/src MS_Ref/source/codec/mp3_dsp/mp3_arm_decoder/asm
endif


SOURCES				=   	mp3_new_adp.c                \
							mp3_dmx_format.c             \
							mp3_demux.c     


ifeq ($(strip $(MP3_ARM_DECODER_SUPPORT)), TRUE)
SOURCES    += mp3_bit.c mp3_dec_api.c mp3_fixed.c mp3_frame.c mp3_huffman.c \
                        mp3_layer3.c mp3_layer12.c mp3_stream.c mp3_synth.c \
                        dctiv.s  dct32.s    III_win_overlap_0.s  mp3_dec_asm_config.s \
                        mp3_dec_quantize.s synth_full.s mp3_dec_bit_operate.s 
endif							
							
ifeq ($(strip $(PLATFORM)), SC6800H)
SOURCES		+= mp3_dsp_decode.c                    
endif

ifeq ($(strip $(PLATFORM)), SC6530)
SOURCES		+= mp3_dsp_decode.c                    
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES		+= mp3_dsp_decode.c                    
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES		+= mp3_arm_decode.c                    
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
SOURCES		+= mp3_arm_decode.c
endif
						    
