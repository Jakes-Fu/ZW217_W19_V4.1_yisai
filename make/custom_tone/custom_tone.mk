
MCFLAG_OPT   	= -Otime
#MCFLAG_OPT   	+= -DCUSTOM_TONE_ASSERT_MODE

MINCPATH =  MS_Ref/source/codec/custom_tone/inc MS_Ref/export/inc MS_Ref/source/aud_config/inc_export \
 MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc MS_Ref/source/base/inc
MINCPATH +=$(L1_INCPATH) 	
MSRCPATH =  MS_Ref/source/codec/custom_tone/src

SOURCES			= custom_tone_adp.c
