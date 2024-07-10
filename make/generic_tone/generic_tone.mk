
MCFLAG_OPT   	= -Otime
#MCFLAG_OPT   	+= -DGENERIC_TONE_ASSERT_MODE
MINCPATH =  MS_Ref/source/codec/generic_tone/inc MS_Ref/export/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export \
 MS_Ref/source/audioservice/inc MS_Ref/source/base/inc BASE/ps/export/inc
MINCPATH +=$(L1_INCPATH) 	
MSRCPATH =  MS_Ref/source/codec/generic_tone/src
MINCPATH += MS_Ref/source/rpc/inc

SOURCES			= generic_tone_adp.c
