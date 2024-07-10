
MCFLAG_OPT   	= -Otime
#MCFLAG_OPT   	+= -DARM_AD_DEBUG_MODE

MINCPATH = MS_Ref/source/codec/arm_ad_adp/inc MS_Ref/export/inc MS_Ref/source/aud_config/inc_export \
 MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH +=$(L1_INCPATH) 	
MSRCPATH =  MS_Ref/source/codec/arm_ad_adp/src

SOURCES			= arm_ad_adp.c
