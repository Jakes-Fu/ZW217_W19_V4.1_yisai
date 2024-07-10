
MCFLAG_OPT   	= -DSDL -DEXTERNAL_SDL_DEBUG -DUSE_PM3_RF -D_ENABLE_WATCHDOG -D_NO_MEASUREMENT -D_TEMP_DEBUG \
					-D_WITHOUT_DUMMY_LAYER1 -D_MN_EVENT -D_PUBLIC_TELEPHONE_ -D_PS_BL7 -D_SPREADTRUM_ -D_LOG_SHARED_MEM \
					-DREF_HANDLE_POWER_KEY -D_ENABLE_LOG_TOOL -D_MN_DEBUG -D_DUAL_LCD -DUSE_SOFT_KEY -DFLASH_EMU \
					-DLOG_DEBUG -DTHREADX -DMMI_TPHWICON_ENABLE

ifeq ($(strip $(SPRD_APP_RELEASE)), FALSE)
include make/app/app_macro.mk
else
include make/app/release_app_macro.mk
endif



MINCPATH =  $(MMI_DIR)/source/mmi_kernel/source/h $(MMI_DIR)/source/mmi_kernel_sys/source/h $(MMI_DIR)/source/mmi_kernel/include \
$(MMI_DIR)/source/mmi_gui/include Third-party/ImageDecoder/Include \
MS_Ref/source/aud_config/inc_export CAF/Template/include $(MMI_DIR)/source/mmi_utility/h
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc


ifeq ($(strip $(WRE_SUPPORT)),TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/wre_boot/h
endif  
ifeq ($(strip $(MET_MEX_SUPPORT)), TRUE)
MINCPATH   += $(MMI_DIR)/source/mmi_app/app/met_mex/h
MINCPATH   += $(MMI_DIR)/source/mmi_app/app/met_mex/lib
ifeq ($(strip $(MET_MEX_SIMULATOR)), TRUE)
MCFLAG_OPT += -DMEX_PLATFORM_SIMULATOR -DMEX_TARGET_SERVER
else
MCFLAG_OPT += -DMEX_PLATFORM_SPREAD -DMEX_TARGET_SERVER
endif
endif

MSRCPATH =  $(MMI_DIR)/source/mmi_kernel/source/c

SOURCES			= mmk_kbd.c  mmk_msg.c mmk_timer.c mmk_tp.c ui_layer.c mmk_gsensor.c

ifeq ($(strip $(MMI_SOFTBLTLAYER_ENABLE)), TRUE)
SOURCES	 += ui_layer_softblt.c
endif  

ifeq ($(strip $(MCARE_V31_SUPPORT)), TRUE)
MCFLAG_OPT += -DSPREAD_PLAT_SC6530
ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
MCFLAG_OPT += -DSPREAD_PLAT_SC6530_PDA
endif
MINCPATH += $(MMI_DIR)/source/mmi_app/app/TencentMcareV31/inc
MINCPATH += $(MMI_DIR)/MS_Ref/source/display/inc
MINCPATH += MS_Ref/source/display/inc
endif 

