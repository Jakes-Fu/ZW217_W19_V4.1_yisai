
MCFLAG_OPT   	= -DSDL -DEXTERNAL_SDL_DEBUG -DUSE_PM3_RF -D_ENABLE_WATCHDOG -D_NO_MEASUREMENT -D_TEMP_DEBUG -D_WITHOUT_DUMMY_LAYER1 \
		-D_MN_EVENT -D_PUBLIC_TELEPHONE_ -D_PS_BL7 -D_SPREADTRUM_ -D_LOG_SHARED_MEM -DREF_HANDLE_POWER_KEY -D_ENABLE_LOG_TOOL \
		-D_MN_DEBUG -DUSE_SOFT_KEY -DFLASH_EMU -DLOG_DEBUG -DTHREADX

ifeq ($(strip $(SPRD_APP_RELEASE)), FALSE)
include make/app/app_macro.mk
else
include make/app/release_app_macro.mk
endif

MINCPATH =  $(MMI_DIR)/source/mmi_gui/source/anim/h $(MMI_DIR)/source/mmi_gui/source/block/h $(MMI_DIR)/source/mmi_gui/source/button/h \
 $(MMI_DIR)/source/mmi_gui/source/common/h \
 $(MMI_DIR)/source/mmi_gui/source/ctrl/h \
 $(MMI_DIR)/source/mmi_gui/source/dropdownlist/h $(MMI_DIR)/source/mmi_gui/source/editbox/h \
 $(MMI_DIR)/source/mmi_gui/source/font/h $(MMI_DIR)/source/mmi_gui/source/fromref/h $(MMI_DIR)/source/mmi_gui/source/graph/h \
 $(MMI_DIR)/source/mmi_gui/source/iconfolder/h $(MMI_DIR)/source/mmi_gui/source/iconlist/h \
 $(MMI_DIR)/source/mmi_gui/source/label/h $(MMI_DIR)/source/mmi_gui/source/lcd/h $(MMI_DIR)/source/mmi_gui/source/listbox/h \
 $(MMI_DIR)/source/mmi_gui/source/menu/h $(MMI_DIR)/source/mmi_gui/source/msgbox/h $(MMI_DIR)/source/mmi_gui/source/prgbox/h \
 $(MMI_DIR)/source/mmi_gui/source/richtext/h $(MMI_DIR)/source/mmi_gui/source/res/h \
 $(MMI_DIR)/source/mmi_gui/source/softkey/h $(MMI_DIR)/source/mmi_gui/source/win/h \
 $(MMI_DIR)/source/mmi_gui/source/statusbar/h $(MMI_DIR)/source/mmi_gui/source/string/h \
 $(MMI_DIR)/source/mmi_gui/source/img/h $(MMI_DIR)/source/mmi_gui/source/settinglist/h \
 $(MMI_DIR)/source/mmi_gui/source/im/h \
 $(MMI_DIR)/source/mmi_gui/source/title/h \
 $(MMI_DIR)/source/mmi_gui/source/form/h \
 $(MMI_DIR)/source/mmi_gui/source/ownerdraw/h $(MMI_DIR)/source/mmi_gui/source/toolbar/h\
 $(MMI_DIR)/source/mmi_gui/source/t9/h $(MMI_DIR)/source/mmi_gui/source/tab/h \
 $(MMI_DIR)/source/mmi_gui/source/textbox/h $(MMI_DIR)/source/mmi_gui/include \
 $(MMI_DIR)/source/mmi_kernel/include $(MMI_DIR)/source/mmi_kernel/source/h \
 $(MMI_DIR)/source/mmi_spml/include \
 $(MMI_DIR)/source/mmi_utility/h \
 Third-party/S9/include \
 Third-party/dictionary/include Third-party/Java/include \
 Third-party/ocr/include \
 Third-party/t9/incl \
 Third-party/cstar/include Third-party/cstar/lib \
 Third-party/WAP2.0_YR/Include/inc Third-party/WAP2.0_YR/Include/inc_ex_browser Third-party/wintone Third-party/xsr_stlmp/Inc \
 Third-party/xsr/Inc  Third-party/dictionary/lib Third-party/Java/lib \
 Third-party/ocr/lib Third-party/streaming/Lib Third-party/t9/lib Third-party/wap/ARM_Lib Third-party/wap/Win_Lib \
 Third-party/WAP2.0_YR/ARM_LIB Third-party/WAP2.0_YR/WIN_LIB Third-party/wintone Third-party/xsr_stlmp/Lib Third-party/xsr/Lib \
 MS_Ref/source/aud_config/inc_export \
 Third-party/sogou/include \
 CAF/Template/include
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc

ifeq ($(strip $(JUNIVERSALCHARDET_SUPPORT)), TRUE)
MINCPATH += external/juniversalchardet/c/include
endif
             
MSRCPATH =  $(MMI_DIR)/source/mmi_gui/source/fromref/c $(MMI_DIR)/source/mmi_gui/source/anim/c $(MMI_DIR)/source/mmi_gui/source/block/c \
 $(MMI_DIR)/source/mmi_gui/source/button/c $(MMI_DIR)/source/mmi_gui/source/common/c \
 $(MMI_DIR)/source/mmi_gui/source/dropdownlist/c $(MMI_DIR)/source/mmi_gui/source/editbox/c \
 $(MMI_DIR)/source/mmi_gui/source/font/c $(MMI_DIR)/source/mmi_gui/source/graph/c \
 $(MMI_DIR)/source/mmi_gui/source/iconfolder/c $(MMI_DIR)/source/mmi_gui/source/iconlist/c $(MMI_DIR)/source/mmi_gui/source/label/c \
 $(MMI_DIR)/source/mmi_gui/source/lcd/c $(MMI_DIR)/source/mmi_gui/source/listbox/c $(MMI_DIR)/source/mmi_gui/source/menu/c \
 $(MMI_DIR)/source/mmi_gui/source/msgbox/c $(MMI_DIR)/source/mmi_gui/source/prgbox/c \
 $(MMI_DIR)/source/mmi_gui/source/richtext/c $(MMI_DIR)/source/mmi_gui/source/res/c \
 $(MMI_DIR)/source/mmi_gui/source/softkey/c $(MMI_DIR)/source/mmi_gui/source/string/c \
 $(MMI_DIR)/source/mmi_gui/source/statusbar/c $(MMI_DIR)/source/mmi_gui/source/tab/c $(MMI_DIR)/source/mmi_gui/source/settinglist/c \
 $(MMI_DIR)/source/mmi_gui/source/textbox/c $(MMI_DIR)/source/mmi_gui/source/ctrl/c \
 $(MMI_DIR)/source/mmi_gui/source/dc/c $(MMI_DIR)/source/mmi_gui/source/win/c $(MMI_DIR)/source/mmi_gui/source/title/c $(MMI_DIR)/source/mmi_gui/source/tips/c \
 $(MMI_DIR)/source/mmi_gui/source/img/c $(MMI_DIR)/source/mmi_gui/source/im/c $(MMI_DIR)/source/mmi_gui/source/form/c \
 $(MMI_DIR)/source/mmi_gui/source/ownerdraw/c $(MMI_DIR)/source/mmi_gui/source/toolbar/c
              
SOURCES	= guianim.c guianim_display.c guianim_internal.c guianim_ref.c guianim_rotate.c guianim_zoom.c \
				guiref_scale.c guiblock.c guibutton.c \
                guicommon.c guidropdownlist.c gui_ucs2gb.c gui_gb2ucs.c gui_gbk.c gui_big5.c guigraph.c gui_cp1252.c gui_cp1251.c gui_cp855.c gui_cp866.c gui_koi8r.c gui_mac_cyrillic.c \
                guiedit.c guiedit_count.c guiedit_cursor.c guiedit_date.c guiedit_highlight.c guiedit_internal.c guiedit_softkey.c \
                guiedit_ip.c guiedit_list.c guiedit_password.c guiedit_text.c guiedit_time.c guiedit_tp.c guiedit_touch.c guiedit_clipbrd.c\
                guifont.c gui_ucs2b_converter.c guiiconfolder.c guiiconfolder_internal.c guiiconlist.c guiiconlist_stunt.c guiiconlist_internal.c guilabel.c guilcd.c \
                guilistbox.c guilist_internal.c guimenu.c guimsgbox.c guiprgbox.c guirichtext.c guiurl.c \
                guisoftkey.c guisoftkey_button.c guistatusbar.c guistatusbar_data.c guitab.c guitext.c guitext_highlight.c guitext_internal.c guitext_clipbrd.c \
                gui_unicode.c guictrl.c guictrl_api.c \
                guidc.c guiwin.c guires_img.c guitips.c guistring.c guisetlist.c \
                guititle.c guiimg.c guiim.c \
		guiform.c guiform_calc.c guiform_internal.c guiform_layout.c guiform_move.c guiform_msg.c guiform_parse.c guiform_tp.c \
		guiownerdraw.c guitoolbar.c guistatusbar_scrollmsg.c

ifeq ($(strip $(VECTOR_FONT_SUPPORT)), ETRUMP)
  MINCPATH += Third-party/etrump/inc
  MSRCPATH += Third-party/etrump/src
  SOURCES  += etrump_adaptation.c
endif

ifneq ($(strip $(MMI_RES_ORIENT)), LANDSCAPE) 
  MSRCPATH += $(MMI_DIR)/source/mmi_gui/source/scrollkey/c
  SOURCES += guiscrollkey.c
endif

ifeq ($(strip $(PDA_UI_DROPDOWN_WIN)), TRUE)
MINCPATH 	+= $(MMI_DIR)/source/mmi_app/app/dropdownwin/h
endif

