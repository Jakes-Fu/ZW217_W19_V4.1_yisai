


MCFLAG_OPT   	= -Otime


#MCFLAG_OPT   	+= -DAMR_ASSERT_MODE



MINCPATH =  BASE/Layer1/source/h MS_Ref/source/codec/amr/inc MS_Ref/export/inc MS_Ref/source/codec/record/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc	
MINCPATH +=MS_Ref/source/codec/aud_common/stream/inc
MINCPATH +=MS_Ref/source/codec/aud_common/audio_output/inc
MINCPATH +=MS_Ref/source/codec/aud_common/inc
MINCPATH +=MS_Ref/source/codec/aud_common/frame_buffer/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_demux/inc
MINCPATH +=MS_Ref/source/codec/aud_common/aud_decode/inc
MINCPATH +=MS_Ref/source/codec/demux/amr/inc
MINCPATH +=MS_Ref/source/codec/aud_dsp_mng/inc
MINCPATH +=MS_Ref/source/codec/decode/amr_arm/inc
MINCPATH +=BASE/Layer1/source/h BASE/ps/export/inc
MINCPATH +=MS_Ref/source/codec/decode/amr_arm/decoder/c_code
MINCPATH +=MS_Ref/source/codec/decode/amr_dsp/inc

MSRCPATH =  MS_Ref/source/codec/adapter/src
MSRCPATH +=MS_Ref/source/codec/stream/src
MSRCPATH +=MS_Ref/source/codec/audio_output/src
MSRCPATH +=MS_Ref/source/codec/common/src
MSRCPATH +=MS_Ref/source/codec/common/frame_buffer/src
MSRCPATH +=MS_Ref/source/codec/demux/amr/src
MSRCPATH +=MS_Ref/source/codec/decode/amr_arm/src
MSRCPATH +=MS_Ref/source/codec/decode/amr_arm/decoder/c_code
MSRCPATH +=MS_Ref/source/codec/decode/amr_dsp/src

SOURCES				= amr_demux.c 

ifeq ($(strip $(AMR_A2DP_SUPPORT)), TRUE)
SOURCES                              +=amr_arm_adp.c amr_dsp_dec.c 
else
SOURCES                              +=amr_dsp_adp.c amr_dsp_decode.c
endif
						    
