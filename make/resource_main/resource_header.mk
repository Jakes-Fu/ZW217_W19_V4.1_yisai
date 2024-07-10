include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

#
#  project config
#

ifeq ($(strip $(SOUNDBANK_VER)), ARM_300KB)
  IN_SOUNDBANK_FILE := MS_Ref/source/soundbank/soundbank_22050_alaw8bits_dy_300k.sf2
  SOUNTBANK_BYTES_PER_SAMPLE := 1
endif

ifeq ($(strip $(SOUNDBANK_VER)), ARM_256KB)
  IN_SOUNDBANK_FILE := MS_Ref/source/soundbank/soundbank_dy_alaw8bit_128k_little.sf2
  SOUNTBANK_BYTES_PER_SAMPLE := 1
endif

ifeq ($(strip $(SOUNDBANK_VER)), ARM_700KB)
  IN_SOUNDBANK_FILE := MS_Ref/source/soundbank/soundbank_22050_alaw8bits_700k_little.sf2
  SOUNTBANK_BYTES_PER_SAMPLE := 1
endif

ifeq ($(strip $(SOUNDBANK_VER)), ARM_2MB)
  IN_SOUNDBANK_FILE := MS_Ref/source/soundbank/soundbank_44100_16bit_2M_little.sf2
  SOUNTBANK_BYTES_PER_SAMPLE := 2
endif

MMI_RES_DIR := $(strip $(MMI_RES_DIR))

#define resource replace flag
REP_FLAG := FALSE
ifeq ($(strip $(SPRD_CUSTOMER)), TRUE)
REP_FLAG := FALSE
endif

#resource all use unicode
ifeq ($(strip $(MMI_RES_TEXT_ALL_UNICODE_SUPPORT)), TRUE)
ALU_FLAG := TRUE
else
ALU_FLAG := FALSE
endif

#Remove Cross RepeatString
ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
CRM_FLAG := FALSE
else
CRM_FLAG := TRUE
endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
SOURCES	 += videoplayer_mdu_def.h
#else
#SOURCES	 += mpeg4_mdu_def.h
endif

ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
ifeq ($(strip $(MOBILE_VIDEO_SUPPORT)), TRUE)
SOURCES += mobile_video_mdu_def.h
endif
endif
SOURCES	 += common_mdu_def.h tools_mdu_def.h clock_mdu_def.h bt_mdu_def.h camera_mdu_def.h cc_mdu_def.h ussd_mdu_def.h\
 connection_mdu_def.h dyna_mdu_def.h ebook_mdu_def.h engtd_mdu_def.h eng_mdu_def.h files_manager_mdu_def.h drm_mdu_def.h fm_mdu_def.h \
 im_mdu_def.h \
 mms_mdu_def.h audioplayer_mdu_def.h  multim_mdu_def.h \
 ocr_mdu_def.h pb_mdu_def.h pic_mdu_def.h pic_viewer_mdu_def.h record_mdu_def.h set_mdu_def.h sms_mdu_def.h stk_mdu_def.h \
 tp_mdu_def.h vt_mdu_def.h mobiletv_mdu_def.h browser_mdu_def.h sample_mdu_def.h otapush_mdu_def.h\
 dcd_mdu_def.h cs_mdu_def.h www_mdu_def.h dl_mdu_def.h dm_mdu_def.h pim_mdu_def.h clipbrd_mdu_def.h search_mdu_def.h  asp_mdu_def.h\
 widget_mdu_def.h atv_mdu_def.h memo_mdu_def.h browser_manager_mdu_def.h sns_mdu_def.h

ifeq ($(strip $(DROPDOWN)), NOTIFY_SHORTCUT)
SOURCES	 += notifycenter_mdu_def.h
endif

ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
SOURCES	+= sensor_mdu_def.h
endif

ifneq ($(strip $(QQ_SUPPORT)), NONE)
SOURCES	 += qq_mdu_def.h
endif

ifeq ($(strip $(KURO_SUPPORT)), TRUE)
SOURCES	 += kuro_mdu_def.h
endif

ifeq ($(strip $(GAME_SUPPORT)), TRUE)
SOURCES	 += game_boxman_mdu_def.h game_quintet_mdu_def.h game_link_mdu_def.h game_manager_mdu_def.h game_snake_mdu_def.h 
endif

ifneq ($(strip $(JAVA_SUPPORT)), NONE)
SOURCES	 += java_mdu_def.h 
endif

ifeq ($(strip $(PUSH_EMAIL_SUPPORT)), TRUE)
SOURCES	 += push_mail_mdu_def.h
endif

ifneq ($(strip $(WIFI_SUPPORT)), NONE)
SOURCES	 += wifi_mdu_def.h
endif

ifeq ($(strip $(QBTHEME_SUPPORT)), TRUE)
SOURCES	 += qbtheme_mdu_def.h
endif

ifeq ($(strip $(WRE_SUPPORT)), TRUE)
SOURCES	 += wre_boot_mdu_def.h
endif

ifeq ($(strip $(WLDT_APP_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/wldt/h
SOURCES	 += wldt_mdu_def.h
endif

ifeq ($(strip $(ZDT_ZFB_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/zfb/h
SOURCES	 += zfb_mdu_def.h
endif

ifeq ($(strip $(ZDT_APP_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/zdt/h
SOURCES	 += zdt_mdu_def.h
endif

ifeq ($(strip $(ZDT_HTTP_APP_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/zdthttp/h
SOURCES	 += zdthttp_mdu_def.h
endif

ifeq ($(strip $(ZDT_RTMP_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/rtmp/h
SOURCES	 += rtmp_mdu_def.h
endif

ifeq ($(strip $(TULING_AI_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/tulingai/h
SOURCES	 += tulingai_mdu_def.h
endif

ifeq ($(strip $(LEBAO_MUSIC_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/lebao/h
SOURCES	 += lebao_mdu_def.h
endif

ifeq ($(strip $(BAIRUI_VIDEOCHAT_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/videochat/h
SOURCES	 += videochat_mdu_def.h
endif

ifeq ($(strip $(LOCAL_ALARM_CLOCK_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/alarmClock/h
SOURCES	 += alarmClock_mdu_def.h
endif

ifeq ($(strip $(ZTE_PEDOMETER_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/pedometer/h
SOURCES	+= pedometer_mdu_def.h
endif

ifeq ($(strip $(ZTE_WEATHER_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zteweather/h
SOURCES	+= zteweather_mdu_def.h
endif

ifeq ($(strip $(ZTE_STOPWATCH_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zte_stopwatch/h
SOURCES     += zte_stopwatch_mdu_def.h 
endif

ifeq ($(strip $(ZTE_MENU_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zte_menu/h
SOURCES     += zte_menu_mdu_def.h 
endif

ifeq ($(strip $(BEACON_APP_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/beacon/h
SOURCES	 += beacon_mdu_def.h
endif

ifeq ($(strip $(IKEYBACKUP_SUPPORT)), TRUE)
SOURCES	 += ikeybackup_mdu_def.h
endif

ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
SOURCES	 += weather_mdu_def.h
endif

ifeq ($(strip $(KING_MOVIE_SUPPORT)), ALL)
SOURCES	 += kmovie_mdu_def.h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), EXTEND)
SOURCES	 += kmovie_mdu_def.h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), STAND)
SOURCES	 += kmovie_mdu_def.h
endif
endif
endif

ifeq ($(strip $(PIC_CROP_SUPPORT)), TRUE)
SOURCES	 += pic_crop_mdu_def.h
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/pic_crop/h
endif

ifeq ($(strip $(PDA_UI_DROPDOWN_WIN)), TRUE) 
SOURCES	 += dropdownwin_mdu_def.h
endif

ifeq ($(strip $(CSC_SUPPORT)),TRUE)
SOURCES	 += customer_mdu_def.h
endif
ifeq ($(strip $(MMIEMAIL_SUPPORT)),TRUE)
SOURCES	 += email_mdu_def.h
endif 
ifeq ($(strip $(SXH_APPLE_SUPPORT)),TRUE)
	MCFLAG_OPT		+= -DSXH_APPLE_SUPPORT
#	MCFLAG_OPT		+= -DSXH_APPLE_3GVEDIO_SUPPORT
	SOURCES		+= apple_mdu_def.h
endif
PROJECTS = mmi_res_prj_def.h

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/videoplayer/h
#else
#SRCPATH += $(MMI_DIR)/source/mmi_app/app/mpeg4/h
endif

ifeq ($(strip $(JAVA_SUPPORT)), MYRIAD)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/myriad_java/mmijava/h
endif

ifeq ($(strip $(JAVA_SUPPORT)), IA)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/jas
endif

ifeq ($(strip $(JAVA_SUPPORT)), SUN)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/java/h
endif

ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
ifeq ($(strip $(MOBILE_VIDEO_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/mobile_video/h
endif
endif

ifeq ($(strip $(MCARE_V31_SUPPORT)),TRUE)
SOURCES	 += TencentMcareV31_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/TencentMcareV31/inc
endif

ifeq ($(strip $(ESIM_SUPPORT)), HB)
SOURCES	 += esim_hb_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/esim_hb/h
endif

ifeq ($(strip $(GAME_SUPPORT)), TRUE)
SRCPATH	 += $(MMI_DIR)/source/mmi_app/app/game/game_snake/h \
 $(MMI_DIR)/source/mmi_app/app/game/game_link/h $(MMI_DIR)/source/mmi_app/app/game/game_manager/h \
 $(MMI_DIR)/source/mmi_app/app/game/game_quintet/h $(MMI_DIR)/source/mmi_app/app/game/game_boxman/h
endif

SRCPATH += $(MMI_DIR)/source/mmi_app/app/mms/h $(MMI_DIR)/source/mmi_app/app/audio/h $(MMI_DIR)/source/mmi_app/app/accessory/h \
 $(MMI_DIR)/source/mmi_app/app/cl/h $(MMI_DIR)/source/mmi_app/common/h $(MMI_DIR)/source/mmi_app/app/ussd/h\
 $(MMI_DIR)/source/mmi_app/app/mainmenu/h $(MMI_DIR)/source/mmi_app/app/multim/h $(MMI_DIR)/source/mmi_app/app/pb/h \
 $(MMI_DIR)/source/mmi_app/app/phone/h $(MMI_DIR)/source/mmi_app/app/setting/h $(MMI_DIR)/source/mmi_app/app/sms/h \
 $(MMI_DIR)/source/mmi_app/app/ss/h $(MMI_DIR)/source/mmi_app/app/stk/h \ $(MMI_DIR)/source/mmi_app/app/www/h \
 $(MMI_DIR)/source/mmi_app/app/eng/h $(MMI_DIR)/source/mmi_app/app/mb/h \ $(MMI_DIR)/source/mmi_app/app/dl/h \
 $(MMI_DIR)/source/mmi_app/app/envset/h $(MMI_DIR)/source/mmi_app/app/msd/h  \
 $(MMI_DIR)/source/mmi_app/app/udisk/h $(MMI_DIR)/source/mmi_app/app/audioplayer/h $(MMI_DIR)/source/mmi_app/app/record/h \
 $(MMI_DIR)/source/mmi_app/app/fm/h \
 $(MMI_DIR)/source/mmi_app/app/ebook/h $(MMI_DIR)/source/mmi_app/app/camera/h \
 $(MMI_DIR)/source/mmi_app/app/idle/h $(MMI_DIR)/source/mmi_app/app/connection/h \
 $(MMI_DIR)/source/mmi_app/app/im/h $(MMI_DIR)/source/mmi_app/app/vt/h $(MMI_DIR)/source/mmi_app/app/engtd/h \
 $(MMI_DIR)/source/mmi_app/app/block/h $(MMI_DIR)/source/mmi_app/app/pic/h \
 $(MMI_DIR)/source/mmi_app/app/vcard/h $(MMI_DIR)/source/mmi_app/app/fmm/h \
 $(MMI_DIR)/source/mmi_app/app/drm/h \
 $(MMI_DIR)/source/mmi_app/app/im/im_cstar/h \
 $(MMI_DIR)/source/mmi_app/app/im/im_t9/h \
 $(MMI_DIR)/source/mmi_app/app/tp/h $(MMI_DIR)/source/mmi_app/app/ocr/h $(MMI_DIR)/source/mmi_app/app/dynamic/h \
 $(MMI_DIR)/source/mmi_app/app/bt/h \
 $(MMI_DIR)/source/mmi_app/app/theme/h \
 $(MMI_DIR)/source/mmi_app/app/pic_viewer/h \
 $(MMI_DIR)/source/mmi_app/app/mobiletv/h \
 $(MMI_DIR)/source/mmi_app/app/browser/h \
 $(MMI_DIR)/source/mmi_app/app/dcd/h \
 $(MMI_DIR)/source/mmi_app/app/cs/h \
 $(MMI_DIR)/source/mmi_app/app/sample/h \
 $(MMI_DIR)/source/mmi_app/app/otapush/h \
 $(MMI_DIR)/source/mmi_app/app/dm/h \
 $(MMI_DIR)/source/mmi_app/app/pim/h \
 $(MMI_DIR)/source/mmi_app/app/clipbrd/h \
 $(MMI_DIR)/source/mmi_app/app/asp/h \
 $(MMI_DIR)/source/pim/h \
 $(MMI_DIR)/source/mmi_app/app/search/h \
 $(MMI_DIR)/source/mmi_app/app/atv/h \
 $(MMI_DIR)/source/mmi_app/app/widget/h \
 $(MMI_DIR)/source/mmi_app/app/memo/h\
 $(MMI_DIR)/source/mmi_app/app/browser_manager/h\
 $(MMI_DIR)/source/mmi_app/app/sns/h\
 $(MMI_DIR)/source/mmi_app/app/appsample/h\
 $(MMI_DIR)/source/resource \
 $(MMI_DIR)/source/resource/$(MMI_RES_DIR)

ifeq ($(strip $(DROPDOWN)), NOTIFY_SHORTCUT)
SRCPATH	 += $(MMI_DIR)/source/mmi_app/app/notifycenter/h
endif

ifeq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
SRCPATH	 += $(MMI_DIR)/source/mmi_app/app/sensor/h
endif
 
ifneq ($(strip $(ADULT_WATCH_SUPPORT)), TRUE)
SRCPATH	 += $(MMI_DIR)/source/mmi_app/app/cc/h
else
SRCPATH	 += $(MMI_DIR)/source/mmi_app/app/cc_adult/h
endif

ifneq ($(strip $(QQ_SUPPORT)), NONE)
SOURCES	 += $(MMI_DIR)/source/mmi_app/app/qq/h
endif

ifeq ($(strip $(KURO_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/kuro/h
endif

ifeq ($(strip $(PUSH_EMAIL_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/push_mail/h
endif

ifeq ($(strip $(KING_MOVIE_SUPPORT)), ALL)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), EXTEND)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), STAND)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/h
endif
endif
endif

ifeq ($(strip $(AUTODEMO_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/autodemo/h
SOURCES += autodemo_mdu_def.h
endif

ifeq ($(strip $(BAIDU_DRIVE_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/baidu_drive/h
SOURCES += baidu_drive_mdu_def.h
endif

ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/weather/h
endif

ifeq ($(strip $(PDA_UI_DROPDOWN_WIN)), TRUE) 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/dropdownwin/h
endif
#ifeq ($(strip $(PDA_UI_SUPPORT)), ANDROID)


ifneq ($(strip $(WIFI_SUPPORT)), NONE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/wlan/h
endif

ifeq ($(strip $(QBTHEME_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/qbtheme/h
endif
ifeq ($(strip $(MET_MEX_SUPPORT)), TRUE)
SOURCES	 += met_mex_mdu_def.h
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/met_mex/h
endif

ifeq ($(strip $(WRE_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/wre_boot/h
endif

ifeq ($(strip $(IKEYBACKUP_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/ikeybackup/h
endif

SRCPATH += $(MMI_DIR)\source\mmi_app\app\wallpaper\h
SOURCES	 += wallpaper_mdu_def.h

ifeq ($(strip $(LIVE_WALLPAPER_FRAMEWORK_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)\source\mmi_app\app\livewallpaper\h
SOURCES	 += livewallpaper_mdu_def.h
endif
#add by zack@20110117 for sky start
ifeq ($(strip $(MRAPP_SUPPORT)), TRUE)
SOURCES += mrapp_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/mrapp/h
endif
#add by zack@20110117 for sky end

ifeq ($(strip $(HERO_ENGINE_SUPPORT)), TRUE)
SOURCES += heroengine_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/heroengine/h

SOURCES += heroapp_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/heroapp/h
endif

ifeq ($(strip $(MMIEMAIL_SUPPORT)),TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/email/h
endif

ifeq ($(strip $(OPERA_MINI_SUPPORT)), VER42)
SOURCES	 += operamini_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/operamini4
endif
ifeq ($(strip $(OPERA_MINI_SUPPORT)), VER6)
SOURCES	 += operamini_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/operamini6
endif
ifeq ($(strip $(OPERA_MOBILE_SUPPORT)), V11)
SOURCES	 += operamobile_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/operamobile
endif


ifeq ($(strip $(CSC_SUPPORT)),TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/customer/h
endif 

ifneq ($(strip $(MMIUZONE_SUPPORT)), NONE)
SOURCES	 += uzone_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/uzone/h
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)),TRUE)
	SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/apple/h
endif

#ifeq ($(strip $(SALES_SUPPORT)),TRUE)
SOURCES	 += sales_mdu_def.h
SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/sales
#endif

ifeq ($(strip $(SFR_SUPPORT)), TRUE)
SOURCES	 += selfreg_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/selfreg/h
endif
ifeq ($(strip $(SFR_SUPPORT_CTCC)), TRUE)
SOURCES	 += selfreg_ctcc_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/selfreg_ctcc/h
endif

SOURCES	 += appsample_mdu_def.h

SRCPATH += $(MMI_DIR)/source/mmi_app/app/datashare/h
SOURCES	 += datashare_mdu_def.h 

ifeq ($(strip $(OD_GAME_SUPPORT)),TRUE)
    ifeq ($(findstring 7701,$(strip $(CHIP_VER))),7701)
	    MMI_PATH := MS_MMI_Main
    else
	    MMI_PATH := MS_MMI
    endif
SRCPATH +=	$(MMI_PATH)/source/mmi_app/app/od_game/inc
SOURCES		+= od_game_mdu_def.h
COPY_RES := $(shell $(PERL) $(MMI_PATH)/source/mmi_app/app/od_game/scripts/copy_res.pl $(MMI_RES_DIR) $(MAINLCD_SIZE) $(CHIP_VER))
$(warning $(COPY_RES))
endif

ifeq ($(strip $(ALIIOT_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/aliiot/h
SOURCES	+= aliiot_mdu_def.h
endif

ifeq ($(strip $(DYNAMIC_WATCHFACE_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/watchfacemanage
SOURCES += watchfacemanage_mdu_def.h
endif

ifeq ($(strip $(XYSDK_SUPPORT)),TRUE)
SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/xysdk/h 
SOURCES	 += xysdk_mdu_def.h
endif

SRCPATH += $(MMI_DIR)/source/mmi_app/app/launcher/h
SOURCES += launcher_mdu_def.h

ifeq ($(strip $(VIDEO_CALL_AGORA_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/video_call/h
SOURCES += video_call_mdu_def.h
endif

ifeq ($(strip $(SETUP_WIZARD_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/setupwizard/h
SOURCES += setupwizard_mdu_def.h
endif

ifeq ($(strip $(XYSDK_SUPPORT)),TRUE)
SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/xysdk/h 
SOURCES	 += xysdk_mdu_def.h
endif
##zmt add start
ifeq ($(strip $(MATH_COUNT_SUPPORT)), TRUE)
SOURCES	 += math_count_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/math_count/h
endif
ifeq ($(strip $(FORMULA_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/formula/h
SOURCES	 += formula_mdu_def.h
endif
ifeq ($(strip $(WORD_CARD_SUPPORT)), TRUE)
SOURCES	 += zmt_word_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zmt_word/h
endif
ifeq ($(strip $(HANZI_CARD_SUPPORT)), TRUE)
SOURCES	 += zmt_hanzi_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zmt_hanzi/h
endif
##zmt add end
 
#
#  make process ,don't modify
#

vpath
vpath %.h        $(subst \,/,$(SRCPATH))

SOURCE_OBJ	= $(patsubst %.h, %.source, $(SOURCES))
PROJECT_OBJ	= $(patsubst %.h, %.project, $(PROJECTS))
SOURCE_DIR	= $(patsubst %_mdu_def.h,%, $(SOURCES))
ROOT_DIR	= $(shell $(PERL) ./make/perl_script/get_root_fullpath.pl)


ifeq ($(strip $(VERIFY_ENVIR)),)
    ifneq ($(strip $(verify_envir)),)
		VERIFY_ENVIR = $(strip $(verify_envir))
    endif
else	
		VERIFY_ENVIR = $(strip $(VERIFY_ENVIR))
endif

ifeq ($(strip $(MAKE_OS)), Linux)
	RESGEN_NEW := 1
endif
ifneq ($(strip $(VERIFY_ENVIR)), )
	RESGEN_NEW := 1
endif
