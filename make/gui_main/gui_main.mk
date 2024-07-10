
MCFLAG_OPT = -DSDL -DEXTERNAL_SDL_DEBUG -DUSE_PM3_RF -D_ENABLE_WATCHDOG -D_NO_MEASUREMENT -D_TEMP_DEBUG -D_WITHOUT_DUMMY_LAYER1 \
	-D_MN_EVENT -D_PUBLIC_TELEPHONE_ -D_PS_BL7 -D_SPREADTRUM_ -D_LOG_SHARED_MEM -DREF_HANDLE_POWER_KEY -D_ENABLE_LOG_TOOL \
	-D_MN_DEBUG -DUSE_SOFT_KEY -DFLASH_EMU -DLOG_DEBUG -DTHREADX

include make/gui_main/gui_feature.mk

ifeq ($(strip $(SPRD_APP_RELEASE)), FALSE)
	include make/app_main/app_macro.mk
else
	include make/app_main/release_app_macro.mk
endif

MINCPATH = CAF/Template/include \
	MS_Ref/source/aud_config/inc_export \
	chip_drv/export/inc/outdated BASE/ps/export/inc

MINCPATH += Third-party/S9/include \
	Third-party/dictionary/include Third-party/Java/include \
	Third-party/ocr/include \
	Third-party/t9/incl \
	Third-party/cstar/include Third-party/cstar/lib \
	Third-party/WAP2.0_YR/Include/inc \
	Third-party/WAP2.0_YR/Include/inc_ex_browser \
	Third-party/wintone Third-party/xsr_stlmp/Inc \
	Third-party/xsr/Inc \
	Third-party/dictionary/lib \
	Third-party/Java/lib \
	Third-party/ocr/lib \
	Third-party/streaming/Lib \
	Third-party/t9/lib \
	Third-party/wap/ARM_Lib \
	Third-party/wap/Win_Lib \
	Third-party/WAP2.0_YR/ARM_LIB Third-party/WAP2.0_YR/WIN_LIB \
	Third-party/wintone \
	Third-party/xsr_stlmp/Lib \
	Third-party/xsr/Lib \
	Third-party/sogou/include

ifeq ($(strip $(JUNIVERSALCHARDET_SUPPORT)), TRUE)
	MINCPATH += Third-party/juniversalchardet/c/include
endif

ifeq ($(strip $(PDA_UI_DROPDOWN_WIN)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_app/app/dropdownwin/h
endif


MINCPATH += $(MMI_DIR)/source/mmi_gui/include \
	$(MMI_DIR)/source/mmi_gui/source/block/h \
	$(MMI_DIR)/source/mmi_gui/source/common/h \
	$(MMI_DIR)/source/mmi_gui/source/ctrl/h \
	$(MMI_DIR)/source/mmi_gui/source/font/h \
	$(MMI_DIR)/source/mmi_gui/source/fromref/h \
	$(MMI_DIR)/source/mmi_gui/source/graph/h \
	$(MMI_DIR)/source/mmi_gui/source/img/h \
	$(MMI_DIR)/source/mmi_gui/source/lcd/h \
	$(MMI_DIR)/source/mmi_gui/source/res/h \
	$(MMI_DIR)/source/mmi_gui/source/string/h \
	$(MMI_DIR)/source/mmi_gui/source/t9/h \
	$(MMI_DIR)/source/mmi_gui/source/win/h \
	$(MMI_DIR)/source/mmi_kernel/include \
	$(MMI_DIR)/source/mmi_kernel/source/h \
	$(MMI_DIR)/source/mmi_spml/include \
	$(MMI_DIR)/source/mmi_utility/h \
    $(MMI_DIR)/source/mmi_service/export/inc \
	$(MMI_DIR)/source/mmi_ctrl/include \
	$(MMI_DIR)/source/mmi_ctrl/source/basecontrol/h \
	$(MMI_DIR)/source/mmi_ctrl/source/ListBox/h \
	$(MMI_DIR)/source/mmi_ctrl/source/TypeManager/h \

             
MSRCPATH = $(MMI_DIR)/source/mmi_gui/source/block/c \
	$(MMI_DIR)/source/mmi_gui/source/common/c \
	$(MMI_DIR)/source/mmi_gui/source/ctrl/c \
	$(MMI_DIR)/source/mmi_gui/source/dc/c \
	$(MMI_DIR)/source/mmi_gui/source/font/c \
	$(MMI_DIR)/source/mmi_gui/source/fromref/c \
	$(MMI_DIR)/source/mmi_gui/source/graph/c \
	$(MMI_DIR)/source/mmi_gui/source/img/c \
	$(MMI_DIR)/source/mmi_gui/source/lcd/c \
	$(MMI_DIR)/source/mmi_gui/source/res/c \
	$(MMI_DIR)/source/mmi_gui/source/string/c \
	$(MMI_DIR)/source/mmi_gui/source/win/c
              
SOURCES	= gui_big5.c gui_cp1251.c gui_cp1252.c gui_cp1256.c gui_cp855.c gui_cp866.c gui_cp874.c \
	gui_gb2ucs.c gui_gbk.c gui_koi8r.c gui_mac_cyrillic.c gui_ucs2b_converter.c gui_ucs2gb.c gui_unicode.c \
	guiblock.c \
	guicommon.c \
	guictrl_api.c \
	guidc.c \
	guifont.c \
	guigraph.c \
	guiimg.c \
	guilcd.c \
	guiref_scale.c \
	guires_img.c \
	guistring.c \
	guiwin.c


ifeq ($(strip $(VECTOR_FONT_SUPPORT)), ETRUMP)
	MINCPATH += Third-party/etrump/inc
	MSRCPATH += Third-party/etrump/src
	SOURCES  += etrump_adaptation.c
endif


################################################################################
# 以下，针对每个控件设定编译的路径、文件

## ---- ANIM ----
#ifeq ($(strip $(GUIF_ANIM)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/anim/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/anim/c
#	SOURCES	+= guianim.c guianim_display.c guianim_internal.c guianim_ref.c guianim_rotate.c guianim_zoom.c
#endif


# ---- BUTTON ----
ifeq ($(strip $(GUIF_BUTTON)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/button/h
	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/button/c
	SOURCES	+= guibutton.c
endif


## ---- DROPDOWNLIST ----
#ifeq ($(strip $(GUIF_DROPDOWNLIST)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/dropdownlist/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/dropdownlist/c
#	SOURCES	+= guidropdownlist.c
#endif


# ---- EDITBOX ----
ifeq ($(strip $(GUIF_EDITBOX)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/editbox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/editbox/c
	SOURCES	+= guiedit.c
endif


## ---- FORM ----
#ifeq ($(strip $(GUIF_FORM)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/form/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/form/c
#	SOURCES	+= guiform.c guiform_calc.c guiform_internal.c guiform_layout.c guiform_move.c \
#		guiform_msg.c guiform_parse.c guiform_tp.c
#endif


## ---- ICONFOLDER ----
#ifeq ($(strip $(GUIF_ICONFOLDER)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/iconfolder/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/iconfolder/c
#	SOURCES	+= guiiconfolder.c guiiconfolder_internal.c
#endif


## ---- ICONLIST ----
#ifeq ($(strip $(GUIF_ICONLIST)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/iconlist/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/iconlist/c
#	SOURCES	+= guiiconlist.c guiiconlist_internal.c guiiconlist_stunt.c
#endif


## ---- IM ----
#ifeq ($(strip $(GUIF_IM)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/im/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/im/c
#	SOURCES	+= guiim.c
#endif


## ---- LABEL ----
#ifeq ($(strip $(GUIF_LABEL)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/label/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/label/c
#	SOURCES	+= guilabel.c
#endif


# ---- LIST ----
ifeq ($(strip $(GUIF_LIST)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/listbox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/listbox/c
	SOURCES	+= guilistbox.c
endif


# ---- MENU ----
ifeq ($(strip $(GUIF_MENU)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/menu/h
	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/menu/c
	SOURCES	+= guimenu.c
endif


## ---- MSGBOX ----
#ifeq ($(strip $(GUIF_MSGBOX)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/msgbox/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/msgbox/c
#	SOURCES	+= guimsgbox.c
#endif


## ---- OWNDRAW ----
#ifeq ($(strip $(GUIF_OWNDRAW)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/ownerdraw/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/ownerdraw/c
#	SOURCES	+= guiownerdraw.c
#endif


# ---- PRGBOX ----
ifeq ($(strip $(GUIF_PRGBOX)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/prgbox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/prgbox/c
	SOURCES	+= guiprgbox.c
endif


## ---- RICHTEXT ----
#ifeq ($(strip $(GUIF_RICHTEXT)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/richtext/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/richtext/c
#	SOURCES	+= guirichtext.c
#endif


# ---- SCROLLKEY ----
ifeq ($(strip $(GUIF_SCROLLKEY)), TRUE)
	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/scrollkey/c
	SOURCES += guiscrollkey.c
endif


## ---- SETLIST ----
#ifeq ($(strip $(GUIF_SETLIST)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/settinglist/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/settinglist/c
#	SOURCES	+= guisetlist.c
#endif

ifeq ($(strip $(FONT_TYPE_SUPPORT)), CSTAR_BITMAP)
	MINCPATH += Third-party/cstar/include
	MINCPATH += $(MMI_DIR)/source/mmi_app/app/fmm/h 
	MINCPATH += $(MMI_DIR)/source/mmi_app/custom/h
	MINCPATH +=  build/$(PROJECT)_builddir/tmp  
endif

## ---- SOFTKEY ----
#ifeq ($(strip $(GUIF_SOFTKEY)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/softkey/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/softkey/c
#	SOURCES	+= guisoftkey.c guisoftkey_button.c
#endif


## ---- STATUSBAR ----
#ifeq ($(strip $(GUIF_STATUSBAR)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/statusbar/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/statusbar/c
#	SOURCES	+= guistatusbar.c guistatusbar_data.c guistatusbar_scrollmsg.c
#endif

ifeq ($(strip $(FONT_TYPE_SUPPORT)), CSTAR_BITMAP)
	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/se/c
	SOURCES += gui_scripteasy.c gui_multilanguage_func.c	
endif

# ---- TAB ----
ifeq ($(strip $(GUIF_TAB)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/tab/h
	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/tab/c
	SOURCES	+= guitab.c
endif


## ---- TEXTBOX ----
#ifeq ($(strip $(GUIF_TEXTBOX)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/textbox/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/textbox/c
#	SOURCES	+= guitext.c guitext_clipbrd.c guitext_highlight.c guitext_internal.c
#endif

## ---- TIPS ----
#ifeq ($(strip $(GUIF_TIPS)), TRUE)
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/tips/c
#	SOURCES	+= guitips.c
#endif


# ---- TITLE ----
ifeq ($(strip $(GUIF_TITLE)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/title/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/title/c
#	SOURCES	+= guititle.c
endif


## ---- TOOLBAR ----
#ifeq ($(strip $(GUIF_TOOLBAR)), TRUE)
#	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/toolbar/h
#	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/toolbar/c
#	SOURCES	+= guitoolbar.c
#endif

# ---- COMBOX ----
ifeq ($(strip $(GUIF_COMBOX)), TRUE)
	MINCPATH += $(MMI_DIR)/source/mmi_gui/source/combox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/combox/c
	SOURCES	+= guicombox.c
endif
