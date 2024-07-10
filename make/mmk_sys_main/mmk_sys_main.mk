
MCFLAG_OPT   	= -DSDL -DEXTERNAL_SDL_DEBUG -DUSE_PM3_RF -D_ENABLE_WATCHDOG -D_NO_MEASUREMENT -D_TEMP_DEBUG \
					-D_WITHOUT_DUMMY_LAYER1 -D_MN_EVENT -D_PUBLIC_TELEPHONE_ -D_PS_BL7 -D_SPREADTRUM_ -D_LOG_SHARED_MEM \
					-DREF_HANDLE_POWER_KEY -D_ENABLE_LOG_TOOL -D_MN_DEBUG -D_DUAL_LCD -DUSE_SOFT_KEY -DFLASH_EMU \
					-DLOG_DEBUG -DTHREADX -DMMI_TPHWICON_ENABLE

ifeq ($(strip $(SPRD_APP_RELEASE)), FALSE)
include make/app_main/app_macro.mk
else
include make/app_main/release_app_macro.mk
endif



MINCPATH =  $(MMI_DIR)/source/mmi_kernel/source/h $(MMI_DIR)/source/mmi_kernel_sys/source/h $(MMI_DIR)/source/mmi_kernel/include \
 $(MMI_DIR)/source/mmi_gui/include \
 $(MMI_DIR)/source/mmi_ctrl/include \
 $(MMI_DIR)/source/mmi_ctrl/source/TypeManager/h \
 $(MMI_DIR)/source/mmi_ctrl/source/basecontrol/h \
 Third-party/ImageDecoder/Include \
 MS_Ref/source/aud_config/inc_export CAF/Template/include $(MMI_DIR)/source/mmi_utility/h \
 $(MMI_DIR)/source/mmi_app/app/fmm/h \
 $(MMI_DIR)/source/mmi_app/custom/h \
 $(MMI_DIR)/source/mmi_app/kernel/h DAPS/export/inc \
 $(MMI_DIR)/source/mmi_service/export/inc
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc

MSRCPATH =  $(MMI_DIR)/source/mmi_kernel_sys/source/c

SOURCES	= mmk_app.c mmk_applet.c mmk_authen.c mmk_handle.c mmk_module.c mmk_shell.c mmk_window.c mmk_modinfo.c mmk_module_manager.c mmk_ctrl.c

ifeq ($(strip $(MCARE_V31_SUPPORT)), TRUE)
MCFLAG_OPT += -DSPREAD_PLAT_SC6530
ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
MCFLAG_OPT += -DSPREAD_PLAT_SC6530_PDA
endif
MINCPATH += $(MMI_DIR)/source/mmi_app/app/TencentMcareV31/inc
MINCPATH += $(MMI_DIR)/MS_Ref/source/display/inc
MINCPATH += MS_Ref/source/display/inc
endif 
