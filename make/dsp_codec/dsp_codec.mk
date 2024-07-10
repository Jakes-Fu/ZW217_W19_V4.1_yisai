
MCFLAG_OPT   	= -Otime
#MCFLAG_OPT   	+= -DDSP_CODEC_ASSERT_MODE

MINCPATH = MS_Ref/source/codec/dsp_codec/inc MS_Ref/export/inc MS_Ref/source/aud_config/inc_export \
MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH +=$(L1_INCPATH) 	
MINCPATH +=MS_Ref/source/codec/aud_common/frame_buffer/inc
MINCPATH +=MS_Ref/source/codec/aud_common/inc
MINCPATH +=MS_Ref/source/aud_dev/inc
MINCPATH +=MS_Ref/source/rpc/inc
MSRCPATH =  MS_Ref/source/codec/dsp_codec/src

SOURCES			= dsp_codec_adp.c
