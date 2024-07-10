
MCFLAG_OPT   	= -Otime -DCHIP_SC6600D 

MINCPATH =  MS_Ref/source/codec/amr_vt/inc MS_Ref/source/codec/amr_vt/inc_export MS_Ref/source/aud_config/inc_export \
 MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc MS_Ref/source/base/inc

MINCPATH += $(L1_INCPATH)
MSRCPATH =  MS_Ref/source/codec/amr_vt/src

SOURCES			= baseband_amrcodec.c
