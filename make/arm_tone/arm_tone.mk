
MCFLAG_OPT   	= -Otime

MINCPATH =MS_Ref/source/codec/arm_tone/inc MS_Ref/export/inc MS_Ref/source/aud_config/inc_export \
 MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc MS_Ref/source/base/inc
MINCPATH +=$(L1_INCPATH) 	
MSRCPATH =  MS_Ref/source/codec/arm_tone/src

SOURCES			= arm_tone_adp.c
