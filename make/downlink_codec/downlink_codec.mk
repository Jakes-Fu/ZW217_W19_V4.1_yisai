
MCFLAG_OPT   	= -Otime 

ifeq ($(strip $(PRODUCT_DM)),TRUE)
     MCFLAG_OPT += -DBT_RECORD_SUPPORT
endif

MINCPATH =  MS_Ref/source/codec/downlink_codec/inc \
						MS_Ref/source/codec/downlink_codec/inc_export\
						MS_Ref/export/inc \
					  MS_Ref/source/aud_config/inc_export \
					  MS_Ref/source/audioservice/inc_export \
					  MS_Ref/source/audioservice/inc \
					  MS_Ref/source/aud_dev/inc

MINCPATH +=$(L1_INCPATH)

MSRCPATH =  MS_Ref/source/codec/downlink_codec/src

ifeq ($(strip $(NXP_LVVE_SUPPORT)),TRUE)
ifeq ($(strip $(BT_ARM_NXP_ENABLE)),TRUE)
    SOURCES         = downlink_nxp_codec_adp.c
else
SOURCES			= downlink_codec_adp.c
endif
else
    SOURCES         = downlink_codec_adp.c
endif
