
ifeq ($(strip $(SCREEN_TEST_SUPPORT)), TRUE)
MCFLAG_OPT      += -DSCREEN_TEST
endif

ifeq ($(strip $(LCD_CONNECTED_TO_V)), TRUE)
MCFLAG_OPT      += -DLCD_CONNECTED_TO_V
endif

ifeq ($(strip $(USING_STATIC_THREAD)), TRUE)
MCFLAG_OPT      += -DUSING_STATIC_THREAD
endif

ifeq ($(strip $(HOST_IS_LITTLEEND)), TRUE)
MCFLAG_OPT      += -DHOST_IS_LITTLEEND
endif

ifeq ($(strip $(MBBMS_SUPPORT)), TRUE)
MCFLAG_OPT      += -DMBBMS_SUPPORT
endif

MINCPATH = CMMB/export/inc CMMB/source/dtl/inc CMMB/source/mtv_api/inc \
           CMMB/source/proxycall/inc CMMB/source/tvfirmware/inc        \
           CMMB/source/mtv_adapter/inc CMMB/source/mbbms/inc           \
           CMMB/source/misc chip_drv/export/inc/outdated CMMB/source/mtv/h

MSRCPATH = CMMB/source/dtl/src CMMB/source/mtv_api/src CMMB/source/proxycall/src CMMB/source/tvfirmware/src

SOURCES  = mtv_api.c dtl_cmdq.c

ifeq ($(strip $(MTV_MIDDLEWARE_ON_BB)), FALSE)
SOURCES  += dtl.c dtl_recovery.c  lcd_tv.c mtv_boot.c scatte_pilot.c \
           seine_sdram_drv.c lcmif_drv.c lcdc_regop.c                \
           aac_decoder_code.c dra_decoder_code.c aac_table.c dra_table.c      \
           dsp_ack_track_code.c dsp_common_code_cmmb.c mtv_log.c 
else
SOURCES  += demod_dummy.c dtl_local.c
endif

ifeq ($(strip $(MTV_MIDDLEWARE_ON_BB)), TRUE)
	MINCPATH += CMMB/source/demodal/inc CMMB/source/mmal/inc CMMB/source/mmil/inc CMMB/source/cmmbdmx/inc
	MSRCPATH += CMMB/source/mmal/src CMMB/source/mmil/src
	SOURCES += multimedia_al.c multimedia_il.c sequencerwbuffer.c

#	ifeq ($(strip $(DEMOD_HW)), SIANO)
#	MINCPATH += CMMB/source/mbbmsca/h CMMB/source/mtv/h
#	MSRCPATH += CMMB/source/mbbmsca/c 
#	SOURCES += ca_adapter.c ca_key.c ecm.c
#	endif

    
#	ifeq ($(strip $(DEMOD_HW)),INNOFIDEI)
#		MINCPATH += CMMB/source/mbbmsca_inno/h CMMB/source/mtv/h
#		MSRCPATH += CMMB/source/mbbmsca_inno/c 
#		SOURCES += ca_adapter.c
#	endif

	ifeq ($(strip $(MBBMS_SUPPORT)), FALSE)
	
	endif
else
	SOURCES += proxycallpeer.c peercustomize.c queue.c mtv_imp_peer.c         \
	           mtv_proxy_peer.c dac_peer.c sfs_peer.c mcex_peer.c bitmapfield.c

	ifeq ($(strip $(MBBMS_SUPPORT)), TRUE)
	SOURCES += mua_peer.c
	endif
endif
