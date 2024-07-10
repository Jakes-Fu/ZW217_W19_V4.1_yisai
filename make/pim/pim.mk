ifeq ($(strip $(PIM_SUPPORT)), TRUE)
MCFLAG_OPT   	= -O2 -Otime
ifeq ($(strip $(DEV_MANAGE_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DDM_SUPPORT
endif
ifeq ($(strip $(BROWSER_SUPPORT_NETFRONT)), TRUE)
MCFLAG_OPT		+=-DDL_SUPPORT
endif
MINCPATH =  $(MMI_DIR)/source/mmi_app/app/mms/h $(MMI_DIR)/source/mmi_app/app/audio/h $(MMI_DIR)/source/mmi_app/app/otapush/h  $(MMI_DIR)/source/mmi_app/app/accessory/h \
 $(MMI_DIR)/source/mmi_app/app/cc/h $(MMI_DIR)/source/mmi_app/app/cl/h $(MMI_DIR)/source/mmi_app/common/h \
 $(MMI_DIR)/source/mmi_app/app/mainmenu/h $(MMI_DIR)/source/mmi_app/app/multim/h $(MMI_DIR)/source/mmi_app/app/pb/h $(MMI_DIR)/source/mmi_app/app/fdn/h\
 $(MMI_DIR)/source/mmi_app/app/phone/h $(MMI_DIR)/source/mmi_app/app/setting/h $(MMI_DIR)/source/mmi_app/app/sms/h \
 $(MMI_DIR)/source/mmi_app/app/ss/h $(MMI_DIR)/source/mmi_app/app/stk/h \
 $(MMI_DIR)/source/mmi_app/app/eng/h $(MMI_DIR)/source/mmi_app/app/mb/h \
 $(MMI_DIR)/source/mmi_app/app/envset/h $(MMI_DIR)/source/mmi_app/app/msd/h $(MMI_DIR)/source/mmi_app/app/mpeg4/h \
 $(MMI_DIR)/source/mmi_app/app/udisk/h $(MMI_DIR)/source/mmi_app/app/mp3/h $(MMI_DIR)/source/mmi_app/app/record/h \
 $(MMI_DIR)/source/mmi_app/app/im/h $(MMI_DIR)/source/mmi_app/app/fm/h \
 $(MMI_DIR)/source/mmi_app/app/wap2.0_yr/h \
 $(MMI_DIR)/source/mmi_app/app/ebook/h $(MMI_DIR)/source/mmi_app/app/game/game_boxman/h $(MMI_DIR)/source/mmi_app/app/camera/h \
 $(MMI_DIR)/source/mmi_app/app/game/game_link/h $(MMI_DIR)/source/mmi_app/app/game/game_manager/h \
 $(MMI_DIR)/source/mmi_app/app/game/game_quintet/h $(MMI_DIR)/source/mmi_app/app/game/game_square/h \
 $(MMI_DIR)/source/mmi_app/app/idle/h $(MMI_DIR)/source/mmi_app/app/connection/h $(MMI_DIR)/source/mmi_app/app/pdp/h \
 $(MMI_DIR)/source/mmi_app/app/vt/h $(MMI_DIR)/source/mmi_app/app/engtd/h \
 $(MMI_DIR)/source/mmi_app/app/block/h $(MMI_DIR)/source/mmi_app/app/pic/h \
 $(MMI_DIR)/source/mmi_app/app/pic_viewer/h \
 $(MMI_DIR)/source/mmi_app/app/vcard/h $(MMI_DIR)/source/mmi_app/app/fmm/h $(MMI_DIR)/source/mmi_app/app/streaming/h \
 $(MMI_DIR)/source/mmi_app/app/streaming/h/nex_inc \
 $(MMI_DIR)/source/mmi_app/app/streaming/lib $(MMI_DIR)/source/mmi_app/app/streaming/nxsys \
 $(MMI_DIR)/source/mmi_app/app/streaming/h/nex_inc/nxsys \
 $(MMI_DIR)/source/mmi_app/app/gps/h \
 $(MMI_DIR)/source/mmi_app/app/tp/h $(MMI_DIR)/source/mmi_app/app/ocr/h $(MMI_DIR)/source/mmi_app/app/dynamic/h \
 $(MMI_DIR)/source/mmi_app/app/bt/h \
 $(MMI_DIR)/source/mmi_app/app/theme/h \
 $(MMI_DIR)/source/mmi_app/app/kuro/h \
 $(MMI_DIR)/source/mmi_app/app/dcd/h \
 $(MMI_DIR)/source/mmi_app/app/mmi3d/h \
 $(MMI_DIR)/source/mmi_app/app/mobiletv/h \
 $(MMI_DIR)/source/mmi_app/app/dl/h \
 $(MMI_DIR)/source/mmi_ctrl/include  \
 $(MMI_DIR)/source/mmi_kernel/include $(MMI_DIR)/source/mmi_utility/h $(MMI_DIR)/source/mmi_gui/include \
 $(MMI_DIR)/source/mmi_app/kernel/h \
 $(MMI_DIR)/source/mmi_app/custom/h \
 $(MMI_DIR)/source/mmi_app/app/pdp/h \
 $(MMI_DIR)/source/mmi_app/emcs_app/mMms/export \
 $(MMI_DIR)/source/mmi_app/emcs_app/mMms/inc\
 MS_Ref/source/ap/inc \
 DAPS/source/xml_expat/inc \
 CMMB/export/inc \
 BASE/l4/export/inc BASE/sim/export/inc DAPS/export/inc char_lib/export/inc  l4/source/arm DAPS/export/inc PARSER/export/inc \
 MS_Ref/source/codec/dtmf_tone/inc_export MS_Ref/source/codec/custom_tone/inc_export MS_Ref/source/codec/generic_tone/inc_export MS_Ref/source/codec/record/inc MS_Ref/source/codec/wma/inc_export MS_Ref/source/aud_exp/eq/inc \
 MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc MS_Ref/source/aud_config/inc_export MS_Ref/source/aud_config/inc \
 Third-party/cstar/include Third-party/dictionary/include \
 Third-party/ocr/include Third-party/t9/incl \
 Third-party/wintone \
 Third-party/xsr_stlmp/include Third-party/xsr/include \
 Third-party/ocr/lib Third-party/streaming/Arm_Lib \
 Third-party/Gps \
 Third-party/S9/include \
 Third-party/hw/include \
 Third-party/hw/h CAF/Template/include Third-party/netfront/include Third-party/netfront/include/config \
 $(MMI_DIR)/source/resource \
 DAPS\source\wbxml_parser\inc \
 DAPS\source\wsp_header_decoder\inc\
 $(MMI_DIR)/source/mmi_app/app/pim/h\
 $(MMI_DIR)/source/pim/h
 
ifeq ($(strip $(BROWSER_SUPPORT)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/browser_manager/h
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/browser/h $(MMI_DIR)/source/mmi_app/app/browser/control/inc
endif
MINCPATH += chip_drv/export/inc/outdated
							
ifeq ($(strip $(BROWSER_SUPPORT_NETFRONT)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/www/h
endif

 MINCPATH += $(PS_INCPATH) $(L1_INCPATH) $(ATC_INCPATH)	
 MINCPATH +=  build/$(PROJECT)_builddir/tmp  
 
 ROOT_DIR	= $(shell $(PERL) ./make/perl_script/get_root_fullpath.pl)
CREATEFILE := $(shell $(PERL) ./make/perl_script/gettime_tofile.pl build/$(PROJECT)_builddir/tmp/buildtime.h)

MSRCPATH	=  $(MMI_DIR)/source/pim/c

SOURCES		=  List.c \
                PIMBASE64.c PIMcallback_handler.c PIMcodec.c PIMCommhttp.c PIMCommobex.c PIMCommwsp.c PIMDataSyncTask.c \
                PIMdecodewbxml.c PIMdecodexml.c PIMlibmem.c PIMmd5.c PIMsmlcore.c PIMSyncmlComm.c PIMsyncmlmgr.c PIMwsm.c  

endif

