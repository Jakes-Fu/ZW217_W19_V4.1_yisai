
MCFLAG_OPT = -DSDL -DEXTERNAL_SDL_DEBUG -DUSE_PM3_RF -D_ENABLE_WATCHDOG -D_NO_MEASUREMENT -D_TEMP_DEBUG -D_WITHOUT_DUMMY_LAYER1 \
	-D_MN_EVENT -D_PUBLIC_TELEPHONE_ -D_PS_BL7 -D_SPREADTRUM_ -D_LOG_SHARED_MEM -DREF_HANDLE_POWER_KEY -D_ENABLE_LOG_TOOL \
	-D_MN_DEBUG -DUSE_SOFT_KEY -DFLASH_EMU -DLOG_DEBUG -DTHREADX

ifeq ($(strip $(MMI_VER)), MAIN)
	GUI_MK_DIR = gui_main
	APP_MK_DIR = app_main
endif

ifeq ($(strip $(MMI_VER)), STABLE)
	GUI_MK_DIR = gui
	APP_MK_DIR = app
endif

include make/$(GUI_MK_DIR)/gui_feature.mk

ifeq ($(strip $(SPRD_APP_RELEASE)), FALSE)
	include make/$(APP_MK_DIR)/app_macro.mk
else
	include make/$(APP_MK_DIR)/release_app_macro.mk
endif

MINCPATH = CAF/Template/include \
	MS_Ref/source/aud_config/inc_export \
	chip_drv/export/inc/outdated BASE/ps/export/inc

MINCPATH += $(MMI_DIR)/source/mmi_ctrl/include \
  $(MMI_DIR)/source/mmi_gui/include \
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

################################################################################
# 以下，针对每个控件设定编译的路径、文件

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/CtrlInfo/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/CtrlInfo/c
	SOURCES	+= ctrlinfo.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/CtrlSys/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/CtrlSys/c
	SOURCES	+= ctrlsys.c
	SOURCES	+= ctrlsys_msg.c
	
	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/TypeManager/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/TypeManager/c
	SOURCES	+= type_manager.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/basecontrol/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/basecontrol/c
	SOURCES	+= ctrlbase.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Button/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Button/c
	SOURCES	+= ctrlbutton.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/CheckBox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/CheckBox/c
	SOURCES	+= ctrlcheckbox.c
	
	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Form/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Form/c
	SOURCES	+= ctrlform.c
	SOURCES	+= ctrlform_calc.c
	SOURCES	+= ctrlform_internal.c
	SOURCES	+= ctrlform_layout.c
	SOURCES	+= ctrlform_move.c
	SOURCES	+= ctrlform_msg.c
	SOURCES	+= ctrlform_parse.c
	SOURCES	+= ctrlform_tp.c
	
	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Menu/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Menu/c
	SOURCES	+= ctrlmenu.c
	SOURCES	+= ctrlmenu_anim.c
	SOURCES	+= ctrlmenu_export.c
	SOURCES	+= ctrlmenu_internal.c
	SOURCES	+= ctrlmenu_sec.c
	SOURCES	+= ctrlmenu_popup.c
	SOURCES	+= ctrlmenu_options.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Tips/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Tips/c
	SOURCES	+= ctrltips.c
	
	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/OwnerDraw/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/OwnerDraw/c
	SOURCES	+= ctrlownerdraw.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Label/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Label/c
	SOURCES	+= ctrllabel.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/editbox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/editbox/c
	SOURCES	+= ctrlbaseedit.c
	SOURCES	+= ctrlbaseedit_cursor.c
	SOURCES	+= ctrlbaseedit_highlight.c
	SOURCES	+= ctrlbaseedit_im.c
	SOURCES	+= ctrlbaseedit_internal.c
	SOURCES	+= ctrlbaseedit_softkey.c
	SOURCES	+= ctrlbasefixed.c
	SOURCES	+= ctrlbaseflex.c
	SOURCES	+= ctrlbaseflex_add_del.c
	SOURCES	+= ctrlbaseflex_clipbrd.c
	SOURCES	+= ctrlbaseflex_common.c
	SOURCES	+= ctrlbaseflex_cursor.c
	SOURCES	+= ctrlbaseflex_display.c
	SOURCES	+= ctrlbaseflex_parse.c
	SOURCES	+= ctrlbaseflex_tp.c
	SOURCES	+= ctrldateedit.c
	SOURCES	+= ctrldigitaledit.c
	SOURCES	+= ctrlipedit.c
	SOURCES	+= ctrllistedit.c
	SOURCES	+= ctrlphonenumedit.c
	SOURCES	+= ctrlpswedit.c
	SOURCES	+= ctrltextedit.c
	SOURCES	+= ctrltextedit_count.c
	SOURCES	+= ctrltimeedit.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/textbox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/textbox/c
	SOURCES	+= ctrltext.c
	SOURCES	+= ctrltext_clipbrd.c
	SOURCES	+= ctrltext_highlight.c
	SOURCES	+= ctrltext_internal.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Progressbar/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Progressbar/c
	SOURCES	+= ctrlprogressbar.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/DropdownList/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/DropdownList/c
	SOURCES	+= ctrldropdownlist.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/ListBox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/ListBox/c
	SOURCES	+= ctrllist_item.c
	SOURCES	+= ctrllistbox.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Slider/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Slider/c
	SOURCES	+= ctrlslider.c
	
	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/progress/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/progress/c
	SOURCES	+= ctrlprogress.c
	
	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/LettersBar/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/LettersBar/c
	SOURCES	+= ctrllettersbar.c
		
	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/msgbox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/msgbox/c
	SOURCES	+= ctrlmsgbox.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/toolbar/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/toolbar/c
	SOURCES	+= ctrltoolbar.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/scroll/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/scroll/c
	SOURCES	+= ctrlscroll.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Combox/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Combox/c
	SOURCES	+= ctrlcombox.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Statusbar/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Statusbar/c
	SOURCES	+= ctrlstatusbar.c
	SOURCES	+= ctrlstatusbar_data.c
	SOURCES	+= ctrlstatusbar_scrollmsg.c	

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Picker/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Picker/c
	SOURCES	+= ctrlbasepicker.c
	SOURCES	+= ctrldatepicker.c
	SOURCES	+= ctrltimepicker.c
	SOURCES	+= ctrlpicker.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/IconFolder/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/IconFolder/c
	SOURCES	+= ctrliconfolder.c
	SOURCES	+= ctrliconfolder_internal.c
	
	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/RichText/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/RichText/c
	SOURCES += ctrlrichtext.c
	SOURCES += guiurl.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/IconList/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/IconList/c
	SOURCES	+= ctrliconlist.c
	SOURCES	+= ctrliconlist_internal.c
	SOURCES	+= ctrliconlist_stunt.c
	
	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Title/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Title/c
	SOURCES	+= ctrltitle.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Mainmenu/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Mainmenu/c
	SOURCES	+= ctrlmainmenu.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Softkey/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Softkey/c
	SOURCES	+= ctrlsoftkey.c
	SOURCES	+= ctrlsoftkey_button.c

	MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Setlist/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Setlist/c
	SOURCES	+= ctrlsetlist.c
	
    MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Im/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Im/c
	SOURCES	+= ctrlim.c

    MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Tab/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/Tab/c
	SOURCES	+= ctrltab.c

    MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/anim/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/anim/c
	SOURCES	+= ctrlanim.c
	SOURCES	+= ctrlanim_display.c
	SOURCES	+= ctrlanim_internal.c
	SOURCES	+= ctrlanim_ref.c
	SOURCES	+= ctrlanim_rotate.c
	SOURCES	+= ctrlanim_zoom.c

ifeq ($(strip $(GUIF_SCROLLKEY)), TRUE)
    MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/scrollkey/h
	MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/scrollkey/c
	SOURCES += ctrlscrollkey.c
endif

    MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/qrcode/h
    MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/qrcode/c
    SOURCES  += bitstream.c mask.c mmask.c mqrspec.c qrencode.c qrinput.c qrspec.c rscode.c split.c

    MINCPATH += $(MMI_DIR)/source/mmi_ctrl/source/watchface/h
    MSRCPATH += $(MMI_DIR)/source/mmi_ctrl/source/watchface/c
    SOURCES += ctrlwfanalogltime.c ctrlwfday.c ctrlwfdigitaltime.c ctrlwfimg.c ctrlwfmonth.c ctrlwfmultinum.c \
               ctrlwfweek.c ctrlwfdigitaldate.c ctrlwfcommon.c ctrlwfbackground.c