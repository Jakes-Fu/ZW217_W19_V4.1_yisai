
MCFLAG_OPT   	= -Otime
#MCFLAG_OPT   	+= -DDTMF_TONE_ASSERT_MODE

MINCPATH =MS_Ref/source/codec/dtmf_tone/inc MS_Ref/export/inc MS_Ref/source/aud_config/inc_export \
 MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc MS_Ref/source/base/inc
MINCPATH +=$(L1_INCPATH) 	
MSRCPATH =  MS_Ref/source/codec/dtmf_tone/src

SOURCES			= dtmf_tone_adp.c
