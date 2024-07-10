
MCFLAG_OPT   	= -DSDL -DEXTERNAL_SDL_DEBUG -DUSE_PM3_RF -D_ENABLE_WATCHDOG -D_NO_MEASUREMENT -D_TEMP_DEBUG\
					-D_WITHOUT_DUMMY_LAYER1 -D_MN_EVENT -D_PUBLIC_TELEPHONE_ -D_PS_BL7 -D_SPREADTRUM_ -D_LOG_SHARED_MEM\
					-DREF_HANDLE_POWER_KEY -D_ENABLE_LOG_TOOL -D_MN_DEBUG -D_DUAL_LCD -DUSE_SOFT_KEY -DFLASH_EMU\
					-DLOG_DEBUG -DTHREADX -DMMI_TPHWICON_ENABLE -DZ_PREFIX -D_WRE

ifeq ($(strip $(ASSERT_TYPE_SUPPORT)), ASSERT)
MCFLAG_OPT		+=  -DMMI_ASSERT_MODE
endif

ifeq ($(strip $(LOW_MEMORY_SUPPORT)), FALSE)
MCFLAG_OPT		+=  -DMMI_AUTOTEST_SUPPORT
endif

ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
MCFLAG_OPT		+=  -DTOUCH_PANEL_SUPPORT
endif
ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE) 
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_SUPPORT
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
MCFLAG_OPT		+=  -DGUI_CTRL_SCROLL_KEY
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE7_SUPPORT
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE8_SUPPORT
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_PSW_SUPPORT
MCFLAG_OPT		+=  -DMMI_VIDEOWALLPAPER_SUPPORT
#MCFLAG_OPT		+=  -DEFFECT_LIGHT_GRID_WALLPAPER_SUPPORT
MCFLAG_OPT		+=  -DLIVE_WALLPAPER_SUPPORT
MCFLAG_OPT      += -DLIVE_WALLPAPER_SUPPORT_LINEAR
MCFLAG_OPT      += -DLIVE_WALLPAPER_SUPPORT_POP
MCFLAG_OPT      += -DLIVE_WALLPAPER_SUPPORT_BEE
MCFLAG_OPT		+=  -DMMI_TASK_MANAGER
MCFLAG_OPT		+=  -DMMI_MEMO_SUPPORT
#状态栏滚动显示信息
MCFLAG_OPT		+=  -DMMI_STATUSBAR_SCROLLMSG_SUPPORT
endif

CREATEFILE := $(shell $(PERL) ./make/perl_script/gettime_tofile.pl build/$(PROJECT)_builddir/tmp/buildtime.h)


MINCPATH =  \
$(MMI_DIR)/source/mmi_app/custom/h \
$(MMI_DIR)/source/mmi_ctrl/include  \
$(MMI_DIR)/source/mmi_gui/include  \
CAF/Template/include  \
$(MMI_DIR)/source/mmi_spml/include  \
MS_Ref/export/inc  \
common/export/inc  \
RTOS/export/inc  \
MS_Customize/export/inc  \
chip_drv/export/inc  \
chip_drv/export/inc/outdated \
DAPS/export/inc  \
DAPS/source/common/inc \
char_lib/export/inc  \
MS_Ref/export/inc \
$(MMI_DIR)/source/mmi_app/kernel/h \
$(MMI_DIR)/source/mmi_kernel/include \
$(MMI_DIR)/source/mmi_app/common/h \
$(MMI_DIR)/source/resource \
$(MMI_DIR)/source/dlmem \
$(L4_INCPATH)   
$(SIM_INCPATH)  
$(PS_INCPATH)   
$(MMI_DIR)/source/mmi_utility/h \
$(MMI_DIR)/source/mmi_app/app/audio/h \
$(MMI_DIR)/source/mmi_app/app/fmm/h \
$(MMI_DIR)/source/mmi_app/app/theme/h \
$(MMI_DIR)/source/mmi_app/app/clipbrd/h \
$(MMI_DIR)/source/mmi_app/app/wifi/h \
$(MMI_DIR)/source/mmi_app/app/pdp/h \
$(MMI_DIR)/source/mmi_app/app/phone/h \
$(MMI_DIR)/source/mmi_app/app/setting/h \
$(MMI_DIR)/source/mmi_app/app/cc/h \
$(MMI_DIR)/source/mmi_app/app/idle/h \
$(MMI_DIR)/source/mmi_app/app/pb/h   \
$(MMI_DIR)/source/mmi_app/app/bt/h   \
$(MMI_DIR)/source/mmi_app/app/sms/h   \
$(MMI_DIR)/source/mmi_app/app/ocr/h   \
$(MMI_DIR)/source/mmi_app/app/browser/h   \
$(MMI_DIR)/source/mmi_app/app/vcard/h   \
$(MMI_DIR)/source/mmi_app/app/connection/h   \
$(MMI_DIR)/source/mmi_app/app/wre/h \
$(MMI_DIR)/source/mmi_app/app/browser/control/inc   \
MS_Ref/source/image_proc/src/img_dec/inc \
Parser/export/inc \
Third-party/zlib/include \
wre/source/include

MINCPATH +=  build/$(PROJECT)_builddir/tmp  

MSRCPATH = \
wre/adaptor \

SOURCES  = \
adaptor_charset.c\
adaptor_font.c\
adaptor_gdi.c\
adaptor_image.c\
adaptor_ime.c\
adaptor_media.c\
adaptor_layer.c\
adp_dial.c\
adp_other.c\
adp_thread.c \
wre_file_adapt.c\
wre_sema_adapt.c \
wre_socket_adapt.c \
wre_timer_adapt.c \
adapt_tapi.c
