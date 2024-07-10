MINCPATH =  $(MMI_DIR)/source/mmi_app/app/mms/h $(MMI_DIR)/source/mmi_app/app/audio/h $(MMI_DIR)/source/mmi_app/app/otapush/h  $(MMI_DIR)/source/mmi_app/app/accessory/h \
 $(MMI_DIR)/source/mmi_app/app/cc/h $(MMI_DIR)/source/mmi_app/app/cl/h $(MMI_DIR)/source/mmi_app/common/h \
 $(MMI_DIR)/source/mmi_app/app/mainmenu/h $(MMI_DIR)/source/mmi_app/app/multim/h $(MMI_DIR)/source/mmi_app/app/pb/h $(MMI_DIR)/source/mmi_app/app/fdn/h\
 $(MMI_DIR)/source/mmi_app/app/phone/h $(MMI_DIR)/source/mmi_app/app/setting/h $(MMI_DIR)/source/mmi_app/app/sms/h \
 $(MMI_DIR)/source/mmi_app/app/ss/h $(MMI_DIR)/source/mmi_app/app/stk/h \
 $(MMI_DIR)/source/mmi_app/app/eng/h $(MMI_DIR)/source/mmi_app/app/mb/h \
 $(MMI_DIR)/source/mmi_app/app/envset/h $(MMI_DIR)/source/mmi_app/app/msd/h \
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

ifeq ($(strip $(BROWSER_SUPPORT)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/browser_manager/h
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/browser/h $(MMI_DIR)/source/mmi_app/app/browser/control/inc
endif
ifeq ($(strip $(PIM_SUPPORT)), TRUE)
MINCPATH		+=  $(MMI_DIR)/source/mmi_app/app/pim/h
endif
							
ifeq ($(strip $(BROWSER_SUPPORT_NETFRONT)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/www/h
 MCFLAG_OPT		+=-DDL_SUPPORT
endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
 MINCPATH += $(MMI_DIR)/source/mmi_app/app/video_player/h
endif

MINCPATH += $(PS_INCPATH) $(L1_INCPATH) $(ATC_INCPATH)		

MINCPATH +=  build/$(PROJECT)_builddir/tmp 

MINCPATH   +=  $(MMI_DIR)/source/mmi_app/app/push_mail/h \
               $(MMI_DIR)/source/push_mail/comm/h \
               $(MMI_DIR)/source/push_mail/push_mail/h \
               CAF/Interface/source/resource \
               CAF/Template/include \
               l4/export/inc \
               DAPS/source/wbxml_parser/inc \
               DAPS/source/wsp_header_decoder/inc \
               DAPS/source/xml_expat/inc \
               PARSER/export/inc \
               sim\export\inc \
               $(MMI_DIR)/source/mmi_app/common/h \
               $(MMI_DIR)/source/mmi_app/custom/h \
               $(MMI_DIR)/source/mmi_app/kernel/h \
               $(MMI_DIR)/source/mmi_ctrl/include  \
               $(MMI_DIR)/source/mmi_gui/include \
               $(MMI_DIR)/source/mmi_kernel/include \
               $(MMI_DIR)/source/mmi_spml/include \
               $(MMI_DIR)/source/mmi_utility/h \
               $(MMI_DIR)/source/resource \
               $(MMI_DIR)/source/mmi_app/app/audio/h \
               $(MMI_DIR)/source/mmi_app/app/fmm/h \
               $(MMI_DIR)/source/mmi_app/app/mpeg4/h \
               $(MMI_DIR)/source/mmi_app/app/connection/h \
               $(MMI_DIR)/source/mmi_app/app/mms/h \
               $(MMI_DIR)/source/mmi_app/app/sms/h \
               $(MMI_DIR)/source/mmi_app/app/pb/h \
               $(MMI_DIR)/source/mmi_app/app/pdp/h \
               $(MMI_DIR)/source/mmi_app/app/phone/h \
               $(MMI_DIR)/source/mmi_app/app/setting/h \
               $(MMI_DIR)/source/mmi_app/app/cc/h \
               $(MMI_DIR)/source/mmi_app/app/idle/h \
               $(MMI_DIR)/source/mmi_app/app/ocr/h 

ROOT_DIR	= $(shell $(PERL) ./make/perl_script/get_root_fullpath.pl)
CREATEFILE := $(shell $(PERL) ./make/perl_script/gettime_tofile.pl build/$(PROJECT)_builddir/tmp/buildtime.h)
MSRCPATH   +=   $(MMI_DIR)/source/push_mail/comm/c \
               $(MMI_DIR)/source/push_mail/push_mail/c

#SOURCES    =   comm_oma_download.c comm_http.c comm_platform.c comm_file.c comm_memory.c comm_encode.c \
#               comm_debug.c comm_socket.c 
                   
#SOURCES   +=   mail_mem.c mail_conttypes.c mail_prbr.c mail_pop3.c mail_retr.c mail_sig.c mail_smtp.c \
#               mail_send.c mail_time.c mail_smtp_lmnd.c mail_network.c mail_list.c mail_sub_list.c \
#               mail_asyn.c mail_fldr.c mail_pushparse.c mail_message.c mail_utility.c mail_attachment.c
