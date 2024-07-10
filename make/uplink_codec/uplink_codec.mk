
MCFLAG_OPT   	= -Otime

ifeq ($(strip $(PRODUCT_DM)),TRUE)
     MCFLAG_OPT += -DBT_RECORD_SUPPORT
endif

ifeq ($(strip $(CHIP_VER)), 6531)
MCFLAG_OPT    += -DBLUETOOTH_SHARE_MEMORY
endif 
ifeq ($(strip $(CHIP_VER)), 6531EFM)
MCFLAG_OPT    += -DBLUETOOTH_SHARE_MEMORY
endif

MINCPATH =  MS_Ref/source/codec/uplink_codec/inc \
						MS_Ref/source/codec/uplink_codec/inc_export \
					  MS_Ref/export/inc \
					  MS_Ref/source/aud_config/inc_export \
					  MS_Ref/source/audioservice/inc_export \
					  MS_Ref/source/audioservice/inc \
					  MS_Ref/source/aud_dev/inc

MINCPATH +=$(L1_INCPATH)

MSRCPATH =  MS_Ref/source/codec/uplink_codec/src

ifeq ($(strip $(NXP_LVVE_SUPPORT)),TRUE)
ifeq ($(strip $(BT_ARM_NXP_ENABLE)),TRUE)
    SOURCES         = uplink_nxp_codec_adp.c
else
SOURCES			= uplink_codec_adp.c
endif
else
    SOURCES         = uplink_codec_adp.c
endif
