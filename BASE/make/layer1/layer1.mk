
MCFLAG_OPT   	= -DCHIP_SC6600C -D_AUDIO_ENHANCEMENT_PHASE1_ -D_PM3_RF -D_CALIBRATION_VER_0009_ -DSC6600M \
                  -D_DOWNLOADPARAM_VER_0004_ -D_MONITOR_ARFCN_NUM_EXTEND -D_LAYER1_MODEM_RDNV 


ifeq ($(strip $(CHIP_VER)),6610) 
    ifeq ($(strip $(DSPZIP_COMPRESS_SUPPORT)), TRUE)
        MCFLAG_OPT   	+= -DBZP_DSP_CODE
    endif
endif

ifeq ($(strip $(GPRS_SUPPORT)), FALSE)
MCFLAG_OPT   	+= -D_GSM_ONLY_ -D_BASE_COMPACT_CODE_
else
MCFLAG_OPT   	+= -D_SUPPORT_GPRS_
endif

ifeq ($(strip $(SMART_PAGING_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_SUPPORT_SMART_PAGE
endif


MCFLAG_OPT   	+= -DLAYER1_EMR_SUPPORT

ifeq ($(strip $(PBCCH_REMOVE_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_BCCH_MODE_ONLY_
endif

ifeq ($(strip $(HW_ALPHA_CUSTOMER)), TRUE)
MCFLAG_OPT   	+= -D_FIXPOSITION_SEARCH_OPTIMIZE_
endif


ifeq ($(strip $(AAC_SUPPORT)), TRUE)
MCFLAG_OPT      += -DAAC_SUPPORT
endif

ifeq ($(strip $(CARD_LOG)), TRUE)
MCFLAG_OPT      += -DCARD_LOG
endif

ifeq ($(strip $(PRODUCT_DM)), TRUE)
MCFLAG_OPT      += -DPRODUCT_DM
else
MCFLAG_OPT      += -D_RXLEVEL_PRECISION_
endif


ifeq ($(strip $(PLATFORM)), SC6530)
MCFLAG_OPT   	+= -DSC6600MPLUS   -D_AUDIO_SC6800H_
MCFLAG_OPT   	+= -D_SHAREMEMORY_CLK_OPERATION_
endif
ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT   	+= -DSC6600MPLUS   -D_AUDIO_SC6800H_
MCFLAG_OPT   	+= -D_SHAREMEMORY_CLK_OPERATION_ -DLAYER1_APLLCLK_SUPPORT
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
MCFLAG_OPT   	+= -DSC6600MPLUS   -D_AUDIO_SC6800H_
MCFLAG_OPT   	+= -D_SHAREMEMORY_CLK_OPERATION_ -DLAYER1_APLLCLK_SUPPORT
endif
ifeq ($(strip $(SBC_DSP_SUPPORT)), VERSION1)
MCFLAG_OPT   	+= -D_ENABLE_LAYER1_SBC
endif

ifeq ($(strip $(BT_A2DP_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_ENABLE_LAYER1_SBC
endif

ifeq ($(strip $(SBC_DSP_SUPPORT)), VERSION2)
MCFLAG_OPT   	+= -D_L1_MP3SYNTH_SBC_SUPPORTED_ 
endif

ifeq ($(strip $(SBC_OVERLAY)), TRUE)
MCFLAG_OPT   	+= -DSBC_OVERLAY_SUPPORT
endif

ifeq ($(strip $(HALFRATE_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_LAYER1_HALFRATE_SUPPORTED 
endif



ifeq ($(strip $(DSP_HW_COMPRESS)), TRUE)
MCFLAG_OPT   	+= -DHW_DSP_COMPRESS 
endif

ifeq ($(strip $(VOLUME_LEVEL)), 15)
MCFLAG_OPT   	+= -DLAYER1_VOLUME_15 
endif

ifeq ($(strip $(CHIP_DSP)), 6500)
MCFLAG_OPT   	+= -DLAYER1_SC6530C
endif

ifeq ($(strip $(CHIP_DSP)), 6531)
MCFLAG_OPT   	+= -DLAYER1_SC6530C
endif

ifeq ($(strip $(CHIP_DSP)), 6531EFM)
MCFLAG_OPT   	+= -DLAYER1_SC6530C
MCFLAG_OPT   	+= -DLAYER1_BLUETOOTH_SUPPORT 
MCFLAG_OPT   	+= -DGL1_BT_INTERFERENCE_SUPPORT 
MCFLAG_OPT   	+= -DLAYER1_FM_SUPPORT 
endif


ifeq ($(strip $(BLUETOOTH_SUPPORT)), SPRD_BT)
ifeq ($(strip $(CHIP_DSP)), 6531)
MCFLAG_OPT   	+= -DLAYER1_BLUETOOTH_SUPPORT 
endif
ifeq ($(strip $(CHIP_DSP)), 6531EFM)

endif

endif
ifeq ($(strip $(CHIP_DSP)), 6530)
MCFLAG_OPT   	+= -DLAYER1_OSC32K_SUPPORT 
endif

ifeq ($(strip $(CHIP_DSP)), 6500)
ifeq ($(strip $(OSC32K_CLOCK_SELECT)), EXTE)
MCFLAG_OPT   	+= -DLAYER1_OSC32K_SUPPORT 
endif
endif

ifeq ($(strip $(CHIP_DSP)), 6531)
ifeq ($(strip $(OSC32K_CLOCK_SELECT)), EXTE)
MCFLAG_OPT   	+= -DLAYER1_OSC32K_SUPPORT 
endif
endif
ifeq ($(strip $(CHIP_DSP)), 6531EFM)
ifeq ($(strip $(OSC32K_CLOCK_SELECT)), EXTE)
MCFLAG_OPT   	+= -DLAYER1_OSC32K_SUPPORT 
endif
endif


MINCPATH =  BASE/PS/export/inc BASE/Layer1/export/inc BASE/Layer1/source/h BASE/layer1_dsp/export/inc BASE/l4/export/inc BASE/sim/export/inc	
MINCPATH += chip_drv/export/inc/outdated chip_drv/export/inc connectivity/BT/export/inc MS_Customize/export/inc

ifeq ($(strip $(CARD_LOG)), TRUE)
MINCPATH      += BASE/logsave/export/inc
endif

MSRCPATH =  BASE/Layer1/source/c

ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES			=  layer1_diag.c layer1_outport.c   

else

SOURCES			= calibration.c cellsyncinfo.c cmdq.c datacnfproc.c \
								dataproc.c downloadparam.c dspintisr.c dspproc.c \
								frameinstrutil.c gsmframegen.c \
								layer1_audio.c layer1_diag.c layer1_freq.c layer1_mac_proc.c \
								layer1_main.c layer1_midi.c layer1_outport.c  \
								mphhandle.c phhandle.c resq.c rxlevmon.c sharedmem_proc.c \
								systeminfo.c tapwr.c layer1_audio_codec.c


ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
SOURCES         += gprsframegen.c gprssysinfo.c 
endif

ifeq ($(strip $(PRODUCT_DM)), FALSE)
SOURCES         += layer1_aac.c
SOURCES         += layer1_sbc.c
endif

endif