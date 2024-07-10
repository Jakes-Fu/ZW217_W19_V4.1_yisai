ifeq ($(strip $(MMIEMAIL_SUPPORT)),TRUE)
MCFLAG_OPT	+= -DMMIEMAIL_SUPPORT	 

MSRCPATH  += DAPS/source/email/engine/email/impl/common
MSRCPATH  += DAPS/source/email/engine/email/impl/corestore/src
MSRCPATH  += DAPS/source/email/engine/email/impl/emecodec/impl/src
MSRCPATH  += DAPS/source/email/engine/email/impl/emestore/impl/src
MSRCPATH  += DAPS/source/email/engine/email/impl/qeestore/src
MSRCPATH  += DAPS/source/email/engine/email/impl/qeetm/impl/src/aee
MSRCPATH  += DAPS/source/email/engine/email/impl/qeetm/impl/src/lib
MSRCPATH  += DAPS/source/email/engine/email/impl/qeetm/impl/src/oem
MSRCPATH  += DAPS/source/email/engine/email/impl/qeetm/impl/src/stack
MSRCPATH  += DAPS/source/email/engine/qc_inc
MSRCPATH  += DAPS/source/email/engine/wep/comm
MSRCPATH  += DAPS/source/email/engine/wep/csc/wecsc/impl/core
MSRCPATH  += DAPS/source/email/engine/tls/aee
MSRCPATH  += DAPS/source/email/engine/tls/engine/sec/impl/core/alg
MSRCPATH  += DAPS/source/email/engine/tls/engine/sec/impl/core/cert
MSRCPATH  += DAPS/source/email/engine/tls/engine/sec/impl/core/main
MSRCPATH  += DAPS/source/email/engine/tls/engine/sec/impl/core/ue
MSRCPATH  += DAPS/source/email/engine/tls/engine/sec/impl/core/wim
MSRCPATH  += DAPS/source/email/engine/tls/engine/sec/impl/src/isec
MSRCPATH  += DAPS/source/email/engine/tls/engine/sec/impl/src/oem
MSRCPATH  += DAPS/source/email/engine/tls/engine/tls/impl
MSRCPATH  += DAPS/source/email/engine/tls/engine/tls/src


MINCPATH = $(MMI_DIR)/source/mmi_app/app/accessory/h
MINCPATH += $(MMI_DIR)/source/mmi_service/export/inc
MINCPATH += $(MMI_DIR)/source/mmi_app/app/audio/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/bt/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/camera/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/cc/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/cl/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/connection/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/cs/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/dcd/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/dl/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/dm/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/dynamic/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ebook/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/eng/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/engtd/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/envset/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/fdn/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/fm/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/fmm/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_boxman/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_link/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/game/game_manager/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/idle/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/im/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mainmenu/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mmi3d/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mms/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mobiletv/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/atv/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mp3/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/msd/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/multim/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ocr/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/otapush/h  
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pb/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/phone/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pic/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pic_viewer/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pim/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/record/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/setting/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/sms/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/ss/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/stk/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/theme/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tp/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/udisk/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/vcard/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/vt/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/clipbrd/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/search/h
MINCPATH += $(MMI_DIR)/source/mmi_app/common/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/custom/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/kernel/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/parse/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/livewallpaper/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/wallpaper/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/mmise/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/livewallpaper/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/livewallpaper/h/def
MINCPATH += $(MMI_DIR)/source/mmi_gui/include 
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/include
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include 
MINCPATH += $(MMI_DIR)/source/mmi_spml/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h 
MINCPATH += $(MMI_DIR)/source/spim/include
MINCPATH += $(MMI_DIR)/source/pim/h
MINCPATH += $(MMI_DIR)/source/resource
MINCPATH += $(MMI_DIR)/source/mmi_service/source/mmisrvaud/h
MINCPATH += $(MMI_DIR)/source/mmi_service/source/mmisrvmgr/h
MINCPATH += $(MMI_DIR)/source/mmi_service/source/mmisrvapi/h
MINCPATH += $(MMI_DIR)/source/mmi_service/export
MINCPATH += CAF/Interface/source/resource 
MINCPATH += CAF/Template/include 
MINCPATH += DAPS/source/wbxml_parser/inc 
MINCPATH += DAPS/source/wsp_header_decoder/inc
MINCPATH += DAPS/source/xml_expat/inc 
MINCPATH += DAPS/export/inc
ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
else
MINCPATH   += DAPS/export/inc/tcpip
endif
MINCPATH += DAPS/export/inc/email
MINCPATH += $(L4_INCPATH)
MINCPATH += PARSER/export/inc 
MINCPATH += $(SIM_INCPATH)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/kuro/h 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/widget/h 
MINCPATH += CMMB/export/inc 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tts/h
MINCPATH += logsave/export/inc
MINCPATH += DAPS/source/formal/hw/include
MINCPATH += DAPS/source/hw/include 
MINCPATH += DAPS/source/hw/include 
MINCPATH += $(MMI_DIR)/source/mmi_app/app/www/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/vt/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/browser/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/browser/control/inc
MINCPATH += $(MMI_DIR)/source/mmi_app/app/sample/h
MINCPATH += DAPS/source/ocr/include
MINCPATH += $(MMI_DIR)/source/mmi_app/app/gps/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/video_player/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/asp/h
MINCPATH += $(PS_INCPATH) $(L1_INCPATH) $(ATC_INCPATH)						
MINCPATH +=  build/$(PROJECT)_builddir/tmp  
MINCPATH += $(MMI_DIR)/source/mmi_app/app/qq/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/wifi/h \
MINCPATH += MS_Customize/source/product/driver/wifi/ar6102
MINCPATH += MS_Customize/source/product/driver/wifi/ar6302

MINCPATH += chip_drv/export/inc/outdated

MINCPATH 	+= $(MMI_DIR)/source/mmi_app/app/email/h

MINCPATH 	+= DAPS/source/email/engine/email/inc
MINCPATH 	+= DAPS/source/email/engine/email/impl/common
MINCPATH 	+= DAPS/source/email/engine/email/impl/corestore/inc
MINCPATH 	+= DAPS/source/email/engine/email/impl/corestore/src
MINCPATH 	+= DAPS/source/email/engine/email/impl/emecodec/inc
MINCPATH 	+= DAPS/source/email/engine/email/impl/emecodec/impl/inc
MINCPATH 	+= DAPS/source/email/engine/email/impl/emestore/inc
MINCPATH 	+= DAPS/source/email/engine/email/impl/emestore/impl/inc
MINCPATH 	+= DAPS/source/email/engine/email/impl/qeestore/src
MINCPATH 	+= DAPS/source/email/engine/email/impl/qeetm/inc
MINCPATH 	+= DAPS/source/email/engine/email/impl/qeetm/impl/inc
MINCPATH 	+= DAPS/source/email/engine/email/impl/qeetm/impl/src/stack
MINCPATH 	+= DAPS/source/email/engine/wep/inc
MINCPATH 	+= DAPS/source/email/engine/wep/csc/wecsc/impl/comm
MINCPATH 	+= DAPS/source/email/engine/wep/csc/wecsc/impl/core
MINCPATH 	+= DAPS/source/email/engine/qc_inc
MINCPATH 	+= DAPS/source/email/engine/tls/aee
MINCPATH 	+= DAPS/source/email/engine/tls/engine/sec/inc
MINCPATH 	+= DAPS/source/email/engine/tls/engine/sec/impl/inc/sec
MINCPATH 	+= DAPS/source/email/engine/tls/engine/sec/impl/src/oem
MINCPATH 	+= DAPS/source/email/engine/tls/engine/tls/inc
MINCPATH 	+= DAPS/source/email/engine/tls/engine/tls/src
MINCPATH 	+= DAPS/source/email/engine/tls/engine/tls/impl

SOURCES   += eme_callback.c eme_encode_base64.c eme_file.c eme_utils.c oem_asyncproc.c email_store.c EMS_Msg.c EMS_MsgPart.c \
             EMS_Store.c adler_check.c crc_check.c eme_Charset_big5ucs2.c eme_charset_gbkucs2.c eme_codec_api.c eme_codec_binhex.c \
             eme_codec_decode_bodystruct.c eme_codec_decode_quoted.c eme_codec_file.c eme_codec_mailmessage.c eme_codec_utils.c \
             eme_codec_uu.c eme_stringlinereader.c huffmanbuilt.c trees.c eme_dataaccess.c eme_msgindex.c eme_msgopts.c eme_msgstore.c \
             eme_multipartmsg.c aeeemetm.c eme_fsm_imap.c eme_fsm_pop.c eme_fsm_smtp.c eme_list.c eme_sig.c eme_soc.c eme_tm.c \
             eme_tm_utils.c eme_transaction.c eme_socket.c oem_tmlib.c eme_imap.c eme_imap_cmdpool.c eme_imap_parser.c eme_imap_utils.c \
             eme_pop_api.c eme_pop_cmdlist.c eme_pop_util.c eme_smtp.c tf3rd_for_hz_fun.c we_scl.c we_mime.c we_bulk.c we_hash.c \
             we_hdr.c we_libalg.c we_rnd.c we_stream.c brweng_tf2hz_email_fun.c csc.c csc_core_big5.c csc_core_gb18030.c csc_core_gb2312.c csc_core_gbk.c \
             itls.c tlscust.c we_msg.c eme_actindex.c eme_boxindex.c eme_serverconf.c we_mempool.c

endif