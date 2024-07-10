


MCFLAG_OPT   	= -Otime


ifeq ($(strip $(CHIP_ENDIAN)),BIG)
MCFLAG_OPT   	+= -DAMR_DEC_ARM_BE
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MCFLAG_OPT		+=-DDECODE_BY_DSP
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT		+=-DDECODE_BY_DSP
endif

#MCFLAG_OPT   	+= -DAMR_ASSERT_MODE


MINCPATH =  Layer1/source/h MS_Ref/source/codec/amr/inc MS_Ref/export/inc MS_Ref/source/codec/record/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH +=$(L1_INCPATH)		
MINCPATH +=MS_Ref/source/codec/aud_common/stream/inc
MINCPATH +=MS_Ref/source/codec/aud_common/audio_output/inc
MINCPATH +=MS_Ref/source/codec/aud_common/audio_input/inc
MINCPATH +=MS_Ref/source/codec/aud_common/inc
MINCPATH +=MS_Ref/source/codec/aud_common/frame_buffer/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_demux/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_decode/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_mux/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_encode/inc



MSRCPATH =  MS_Ref/source/codec/aud_common/stream/src
MSRCPATH +=MS_Ref/source/codec/aud_common/audio_output/src
MSRCPATH +=MS_Ref/source/codec/aud_common/audio_input/src
MSRCPATH +=MS_Ref/source/codec/aud_common/aud_demux/src
MSRCPATH +=MS_Ref/source/codec/aud_common/aud_decode/src
MSRCPATH +=MS_Ref/source/codec/aud_common/frame_buffer/src
MSRCPATH +=MS_Ref/source/codec/aud_common/aud_mux/src
MSRCPATH +=MS_Ref/source/codec/aud_common/aud_encode/src




SOURCES				=    audio_framebuffer.c audio_output.c audio_stream.c  aud_demux.c  aud_decode.c  audio_input.c aud_encode.c aud_mux.c
						    

						    
#ifeq ($(strip $(ARMVB_RECORD_SUPPORT)), TRUE)
#MCFLAG_OPT 		+= -DAUD_ARM_RECORD_EXP
#endif