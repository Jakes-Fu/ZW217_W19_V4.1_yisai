
MCFLAG_OPT   	= 

MINCPATH =  MS_Ref/source/aud_dev/inc  MS_Ref/source/audioservice/inc \
 MS_Ref/source/audioservice/inc_export MS_Ref/source/aud_config/inc_export chip_drv/export/inc 

ifeq ($(strip $(PLATFORM)),    UWS6121E)
MINCPATH += chip_drv/chip_module/chip/uws6121e
endif 
 
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc
MINCPATH +=$(L1_INCPATH) 	
MINCPATH += MS_Ref/source/rpc/inc
MINCPATH += chip_drv/chip_module/audio
MINCPATH += chip_drv/chip_module/audio/inc

MSRCPATH =  MS_Ref/source/aud_dev/src
    
ifeq ($(strip $(PLATFORM)),    UWS6121E)
SOURCES         =armvb_as.c \
				nv_dev_adp.c\
		dspvb_as.c \
		track_mixer.c
else
SOURCES         =l1rec_dev.c\
                pcm_dev.c  \
                armvb_as.c \
                dspvb_as.c \
				nv_dev_adp.c\
                audio_cp.c \
                track_mixer.c
endif

ifeq ($(strip $(BT_MIX_SUPPORT)), TRUE)
SOURCES += btmix_dev.c
endif

ifeq ($(strip $(PLATFORM)), SC8800D)
SOURCES			=l1rec_dev.c\
				armvb_as.c \
				dspvb_as.c

endif

ifeq ($(strip $(BT_A2DP_SUPPORT)), TRUE)
    SOURCES			+= a2dp_as.c
endif

ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
MCFLAG_OPT		+= -DPRODUCT_DRIVER_MTV_ENABLE
endif

ifeq ($(strip $(PLATFORM)), SC8800H)
ifeq ($(strip $(CHIP_VER)), 8800H5)
MCFLAG_OPT		+= -DDEV_MODE_EX_SUPPORT -DDSP_AUDIO_III -DVB_OUTPUT_USE_DMA
else
MCFLAG_OPT		+= -DDSP_AUDIO_III -DDEV_MODE_EX_SUPPORT
endif
endif

ifeq ($(strip $(CHIP_VER)), 6531)
MCFLAG_OPT    += -DBLUETOOTH_SHARE_MEMORY
endif

ifeq ($(strip $(CHIP_VER)), 6531EFM)
MCFLAG_OPT    += -DBLUETOOTH_SHARE_MEMORY
endif



ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT 		+= -DFIFO_WATER_MARK_LOW -DCHIP_DRV_SC6600L -DVB_OUTPUT_USE_DMA -DDSP_AUDIO_III -DARMVB_RECORD_SUPPORT
endif
ifeq ($(strip $(PLATFORM)), SC6800H)
MCFLAG_OPT 		+= -DPLATFORM_SC6800H -DFIFO_WATER_MARK_LOW -DDSP_AUDIO_III

ifneq ($(strip $(BLUETOOTH_SUPPORT)),NONE)
MCFLAG_OPT        += -DDIGITAL_GAIN_SUPPORT
endif

endif

ifeq ($(strip $(PLATFORM)), SC6530)
		MCFLAG_OPT 		+= -DFIFO_WATER_MARK_LOW -DDSP_AUDIO_III -DVB_OUTPUT_USE_DMA -DEQ_HAS_MULTI_BANDS -DBT_SBC_DSP_ENCODE

ifneq ($(strip $(BLUETOOTH_SUPPORT)),NONE)
MCFLAG_OPT        += -DDIGITAL_GAIN_SUPPORT
endif

endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
		MCFLAG_OPT 		+= -DFIFO_WATER_MARK_LOW -DDSP_AUDIO_III -DVB_OUTPUT_USE_DMA -DEQ_HAS_MULTI_BANDS -DBT_SBC_DSP_ENCODE

ifneq ($(strip $(BLUETOOTH_SUPPORT)),NONE)
MCFLAG_OPT        += -DDIGITAL_GAIN_SUPPORT
endif

endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT		+= -DDEV_MODE_EX_SUPPORT -DDSP_AUDIO_III -DVB_OUTPUT_USE_DMA -DEQ_HAS_MULTI_BANDS
endif

ifeq ($(strip $(FM_SUPPORT)), SPRD_V0)
MCFLAG_OPT 		+= -DBUILDIN_FM
endif

#ifeq ($(strip $(ARMVB_RECORD_SUPPORT)), TRUE)
#MCFLAG_OPT 		+= -DAUD_ARM_RECORD_EXP
#endif
