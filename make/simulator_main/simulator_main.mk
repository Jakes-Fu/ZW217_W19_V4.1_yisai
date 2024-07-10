include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/simulator_C_MACRO.txt)
endif


# *************************************************************************
# Config Start
# *************************************************************************
dsp_modules_no_debug= cc refbase devps aud_algorithm eq_exp dtmf_tone generic_tone voice custom_tone dsp_codec spim transsamplerate s3d atwin daps tcpip\
aud_common isp_service aud_dev record agc_exp audioservice aud_config ratecontrol dv dc aac_lc midi amr_arm wav mp3_new ra8lbr wma dra vsp  \
h264_codec rv_codec video_security video_security_kmv video_security_hmv azan nes aud_dsp_mng chip_drv clone_codec dv_cfg gzip parser theme_unzip
 
dsp_modules = rtos  daps display lcd img_proc img_proc_core refbase sfs devps parser spim ratecontrol
dsp_modules +=aud_config audioservice agc_exp aud_algorithm eq_exp transsamplerate record \
			  dtmf_tone generic_tone voice custom_tone dsp_codec clone_codec dv aud_common s3d dc isp_service aud_dev \
			  dv_cfg

#mpeg4_codec
ifeq ($(strip $(PLD_SRT_SCENARIO_TEST_SUPPORT)), TRUE)
   dsp_modules += pld_srt
endif

ifeq ($(strip $(MMIEMAIL_SUPPORT)), TRUE)
  macro_set += MMIEMAIL_SUPPORT=TRUE
  dsp_modules += email
endif

ifeq ($(strip $(JS_SUPPORT)), TRUE)
  dsp_modules += spidermonkey
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
   dsp_modules += aud_dsp_mng
endif
ifeq ($(strip $(PLATFORM)), SC6530)
   dsp_modules += aud_dsp_mng
endif

ifeq ($(strip $(TCPIP_SUPPORT)), TRUE)
ifeq ($(strip $(TCPIP_EXPORT)), FALSE)
   dsp_modules += tcpip
endif
endif

ifeq ($(strip $(AAC_SUPPORT)),TRUE)
ifeq ($(strip $(PLATFORM)), SC6800H)
   dsp_modules += aac_lc
endif
ifeq ($(strip $(PLATFORM)), SC6530)
   dsp_modules += aac_lc
endif
ifeq ($(strip $(PLATFORM)), SC8800G)
   dsp_modules += aac
endif
ifeq ($(strip $(PLATFORM)), SC8501C)
   dsp_modules += aac
endif
ifeq ($(strip $(PLATFORM)), SC7702)
   dsp_modules += aac
endif
endif

ifeq ($(strip $(MIDI_SUPPORT)),TRUE)
    dsp_modules += midi
endif

ifeq ($(strip $(AMR_SUPPORT)),TRUE)
    dsp_modules += amr_arm
endif

ifneq ($(strip $(FONT_TYPE_SUPPORT)), CSTAR_BITMAP)
ifneq ($(strip $(VECTOR_FONT_SUPPORT)), ETRUMP)
dsp_modules += spml_main
endif
endif

ifeq ($(strip $(IM_ENGINE)), IEKIE)
ifneq ($(strip $(SPRD_CUSTOMER)), TRUE)
dsp_modules += iekie
endif
endif

ifeq ($(strip $(WAV_SUPPORT)),TRUE)
    dsp_modules += wav
endif

ifeq ($(strip $(MP3_SUPPORT)),TRUE)
    dsp_modules += mp3_new
endif

ifeq ($(strip $(RA8LBR_SUPPORT)),TRUE)
    dsp_modules += ra8lbr
endif

ifeq ($(strip $(TA8LBR_SUPPORT)),TRUE)
    dsp_modules += ra8lbr
endif

ifeq ($(strip $(WMA_SUPPORT)),TRUE)
#    dsp_modules += wma
endif

ifeq ($(strip $(DRA_SUPPORT)),TRUE)
	dsp_modules += dra
endif

ifeq ($(strip $(VT_SUPPORT)),TRUE)
    dsp_modules += vt_adp
endif

dsp_modules += vsp

#dsp_modules += mpeg4_codec

ifeq ($(strip $(H264_DEC_SUPPORT)), TRUE)
   dsp_modules += h264_codec
endif

ifeq ($(strip $(RMVB_DEC_SUPPORT)), TRUE)
   dsp_modules += rv_codec
endif

ifeq ($(strip $(TMVB_DEC_SUPPORT)), TRUE)
   dsp_modules += rv_codec
endif

ifeq ($(strip $(KING_MOVIE_SUPPORT)), STAND)
    dsp_modules += video_security
endif
ifeq ($(strip $(KING_MOVIE_SUPPORT)), EXTEND)
    dsp_modules += video_security
    dsp_modules += video_security_kmv
endif

ifeq ($(strip $(KING_MOVIE_SUPPORT)), ALL)
    dsp_modules += video_security
    dsp_modules += video_security_kmv
    dsp_modules += video_security_hmv
endif


ifeq ($(strip $(JAVA_SUPPORT)), SUN)
   dsp_modules += sun_java
endif

ifeq ($(strip $(PIM_SUPPORT)), TRUE)
   dsp_modules +=pim
endif

ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
#   dsp_modules += mtv_adapter os_al gzip mtv_api
   dsp_modules += mtv_adapter
   dsp_modules += mtv_service
endif

ifeq ($(strip $(MBBMS_SUPPORT)), TRUE)
   dsp_modules += mbbms_sg
endif

ifeq ($(strip $(GZIP_DEC_SUPPORT)),GZIP_SW)
	dsp_modules +=gzip
endif

ifeq ($(strip $(MMI_AZAN_SUPPORT)),TRUE)
    dsp_modules += azan                             
endif


ifeq ($(strip $(QBTHEME_SUPPORT)),TRUE)
    dsp_modules += theme_unzip
endif

ifeq ($(strip $(UAL_GNSS_SUPPORT)), TRUE)
dsp_modules += ual_gnss
endif

ifeq ($(strip $(UAL_WIFI_SUPPORT)), TRUE)
dsp_modules += ual_wifi
endif

ifeq ($(strip $(UAL_AUDIOPLAYER_SUPPORT)), TRUE)
dsp_modules += ual_audioplayer
endif

ifeq ($(strip $(UAL_TELE_SUPPORT)), TRUE)
dsp_modules += ual_tele
endif

ifeq ($(strip $(UAL_DEVICE_SUPPORT)), TRUE)
dsp_modules += ual_device
endif

ifeq ($(strip $(UAL_KV_SUPPORT)), TRUE)
dsp_modules += ual_kv
endif

dsp_modules += ual

ifeq ($(strip $(MMI_WATCH_UI)), TRUE)
dsp_modules_cus = chip_drv utility utility_performance atwin app_watch_main gui_main ctrl mmk_main mmk_sys_main mmi_service caf_templete msdevkernel
else
dsp_modules_cus = chip_drv utility utility_performance atwin app_main gui_main ctrl mmk_main mmk_sys_main mmi_service caf_templete msdevkernel
endif
ifeq ($(strip $(ENABLE_WEBKIT)), TRUE)
dsp_modules_cus	+=  webkit webcore jscore skia harfbuzz libpng zlib freetype icu4c libxml2 libc
endif

ifeq ($(strip $(DYNAMIC_MODULE_SUPPORT)), TRUE)
	dsp_modules_cus += caf_interface
endif

ifeq ($(strip $(JUNIVERSALCHARDET_SUPPORT)), TRUE)
	dsp_modules_cus += juniversalchardet
endif

ifeq ($(strip $(TCPIP_SUPPORT)), TRUE)
ifeq ($(strip $(TCPIP_EXPORT)), TRUE)
   dsp_modules_cus += tcpip
endif
endif
ifeq ($(strip $(NES_SUPPORT)), TRUE)
	dsp_modules += nes
endif


ifeq ($(strip $(SQLITE_SUPPORT)), TRUE)
	dsp_modules_cus += libc
	dsp_modules_cus += sqlite
endif
macro_set=
ifeq ($(strip $(JPG_DEC_SUPPORT)), JPG_HW)
macro_set += JPG_DEC_SUPPORT=JPG_SW
endif
ifeq ($(strip $(JPG_ENC_SUPPORT)), JPG_HW)
macro_set += JPG_ENC_SUPPORT=JPG_SW
endif

ifeq ($(strip $(PUSH_EMAIL_SUPPORT)), TRUE)
macro_set += PUSH_EMAIL_SUPPORT=TRUE
#dsp_modules_cus += push_mail
endif

ifeq ($(strip $(FLASH_SUPPORT)), TRUE)
dsp_modules_cus += flash
#macro_set += FLASH_SUPPORT=FALSE
endif
#add by lixd
ifeq ($(strip $(TF_LOAD_SUPPORT)), TRUE)
macro_set += TF_LOAD_SUPPORT=FALSE
endif
ifeq ($(strip $(MET_MEX_SUPPORT)), TRUE)
    macro_set += MET_MEX_SIMULATOR=TRUE
endif
ifeq ($(strip $(NEW_MEMORY_MM)), FALSE)
macro_set += NEW_MEMORY_MM=TRUE
endif

macro_set += MMI_SIMULATOR=TRUE

ifeq ($(strip $(SFR_SUPPORT_CMCC)), TRUE)
macro_set += SFR_SUPPORT_CMCC=FALSE
endif

ifneq ($(strip $(INTERNAL_LIBS)),)
dsp_modules_cus := $(filter-out devps,$(dsp_modules_cus) $(dsp_modules))
dsp_modules     := devps
endif

ADD_LINK32  = kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib winmm.lib

ADD_LINK32 +=  ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib LIBCMT.LIB msvcrtd.lib 

ifeq ($(strip $(MET_MEX_SUPPORT)), TRUE)
ADD_LINK32 +=  ../../../../Third-party/met/lib/win32/met_mex.lib
endif
# *************************************************************************
# Config End
# *************************************************************************

ACTIVE_PRJ = "msdevkernel"

ifeq ($(strip $(lite)), 1)
SPRD_CUSTOMER=TRUE
LITE=1
endif
ifeq ($(strip $(LITE)), 1)
SPRD_CUSTOMER=TRUE
endif

ifeq ($(strip $(lite)), 0)
LITE=0
ACTIVE_PRJ = "MSDev"
endif
ifeq ($(strip $(LITE)), 0)
ACTIVE_PRJ = "MSDev"
endif

ifeq ($(strip $(SPRD_CUSTOMER)),TRUE)
SPRD_CUSTOMER_MACRO = -DSPRD_CUSTOMER
else
SPRD_CUSTOMER_MACRO =
endif

ifeq ($(strip $(SPRD_CUSTOMER)), TRUE)
SPRD_CUS :=1
dsp_modules_obj := $(foreach comp,$(dsp_modules_cus),$(comp).dsp)
else
SPRD_CUS :=0
dsp_modules_obj := $(foreach comp,$(dsp_modules),$(comp).dsp)  $(foreach comp,$(dsp_modules_cus),$(comp).dsp)
endif

ifeq ($(strip $(SUBM)),)
    ifneq ($(strip $(subm)),)
      SUBM=$(subm)
    endif
endif

ifneq ($(strip $(SUBM)),)
SPRD_CUS :=2
dsp_modules_obj := $(foreach comp,$(SUBM),$(comp).dsp)
dsp_modules =
dsp_modules_cus = $(SUBM)
endif

ifeq ($(strip $(RUN)),)
    ifeq ($(strip $(run)),)
	ifneq ($(strip $(R)),)
		RUN = $(strip $(R))
	else
		ifneq ($(strip $(r)),)
			RUN = $(strip $(r))
		endif
	endif
    else
        RUN = $(strip $(run))
    endif
endif

ifneq ($(strip $(CHIP_ENDIAN)), LITTLE)
suffix_res_file=_win
else
suffix_res_file=
endif

ROOT_DIR	= $(shell $(PERL) ./make/perl_script/get_root_fullpath.pl)

ifneq ($(strip $(TOUCH_PANEL_HWICON_SUPPORT)),NONE)
  SUFFIX=_bar
else
  ifeq ($(strip $(KEYPAD_TYPE)),TYPICAL_KEYPAD)
     SUFFIX=_T
  else
     ifeq ($(strip $(KEYPAD_TYPE)),QWERTY_KEYPAD)
        SUFFIX=_Q
     else
        SUFFIX=
     endif
  endif
endif

ifeq ($(strip $(MAINLCD_LOGIC_ANGLE)),90)
  SKIN_FILE_NAME = $(MAINLCD_DEV_SIZE)
else 
  SKIN_FILE_NAME = $(MAINLCD_SIZE)
endif


vpath

vpath %.mk    $(wildcard make/*)
vpath %.dsp   $(wildcard $(BUILD_DIR)/win/*)
vpath %.dsw   $(BUILD_DIR)/win

.PHONEY:all
all: createdir simulator.dsw
	
.PHONEY:createdir
createdir:
	-@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)" ""
	-@$(PERL) make/perl_script/mk_dir.pl "./lib/$(PROJECT)" ""
	-@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/lib" ""
	-@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win" ""
	-@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/bin" ""	
	
ifneq ($(strip $(SPRD_CUSTOMER)), TRUE)
	-$(PERL) make/perl_script/rm_dir.pl win "$(BUILD_DIR)/img/"	
endif
ifeq ($(strip $(LITE)), 0)
	@$(PERL) make/perl_script/rm_file.pl $(MMI_DIR)/source/winsimulator_tp/win_platform/bin/ControlPanel2.dll  ""
	@$(PERL) make/perl_script/rm_file.pl $(MMI_DIR)/source/winsimulator_tp/win_platform/bin/ControlPanel2.lib  ""
	@$(PERL) make/perl_script/rm_file.pl $(MMI_DIR)/source/winsimulator_tp/win_platform/bin/filesys.dll  ""
	@$(PERL) make/perl_script/rm_file.pl $(MMI_DIR)/source/winsimulator_tp/win_platform/bin/filesys.lib  ""
	@$(PERL) make/perl_script/rm_file.pl $(MMI_DIR)/source/winsimulator_tp/win_platform/bin/SIMConPad.dll  ""
	@$(PERL) make/perl_script/rm_file.pl $(MMI_DIR)/source/winsimulator_tp/win_platform/bin/SIMConPad.lib  ""
	@$(PERL) make/perl_script/rm_file.pl $(MMI_DIR)/source/winsimulator_tp/win_platform/bin/MSDev.exe  ""	
endif

simulator.dsw: $(dsp_modules_obj)
	-$(PERL) make/perl_script/rm_file.pl msdevkernel.dll  "$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/"
	-$(PERL) make/perl_script/rm_file.pl flash_sim.dat  "$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/"
	@$(COPY_DIR)  $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/*)  $(call Path_process,$(BUILD_DIR)/win/bin/)
ifneq ($(LP_LIST),)
	@$(PERL) make/perl_script/create_msdev_ini.pl "$(PROJECT)" "$(strip $(SKIN_FILE_NAME))" "$(strip $(MMI_RES_DIR))"_"$(LP_LIST)" "$(strip $(SUFFIX))" "$(call Path_process,$(strip $(ROOT_DIR)))" "$(strip $(suffix_res_file))" "$(strip $(MMI_DIR))"
else
	@$(PERL) make/perl_script/create_msdev_ini.pl "$(PROJECT)" "$(strip $(SKIN_FILE_NAME))" "$(strip $(MMI_RES_DIR))" "$(strip $(SUFFIX))" "$(call Path_process,$(strip $(ROOT_DIR)))" "$(strip $(suffix_res_file))" "$(strip $(MMI_DIR))"
endif
	-@$(ARMCC) $(VIA) $(BUILD_DIR)/dep/simulator_C_MACRO.txt $(SPRD_CUSTOMER_MACRO) -E make/$(SIMU_DIR)/simulator.ini  -o build/$(PROJECT)_builddir/tmp/simulator.ini
ifneq ($(strip $(VS2010)), 1)
	@$(PERL) make/perl_script/create_dsp.pl "$(PROJECT)" "$(dsp_modules)" "$(dsp_modules_cus)" "$(SPRD_CUS)" "$(ADD_LINK32)" "$(ACTIVE_PRJ)" "$(strip $(PRODUCT_BASELIB_DIR))" "$(dsp_modules_no_debug)"
else
	@$(PERL) make/perl_script/create_dsp.pl "$(PROJECT)" "$(dsp_modules)" "$(dsp_modules_cus)" "$(SPRD_CUS)" "$(ADD_LINK32)" "$(ACTIVE_PRJ)" "$(strip $(PRODUCT_BASELIB_DIR))" "$(dsp_modules_no_debug)" "VS2010" "$(ROOT_DIR)"
ifeq ($(strip $(SPRD_CUSTOMER)), TRUE)
	@$(PERL) make/perl_script/set_simulator_properties.pl "$(BUILD_DIR)/win/"
endif
endif
ifeq ($(strip $(RUN)),1)
ifneq ($(strip $(IB)),)
	run_simulator.bat $(PROJECT)
else
	run_simulator.bat $(PROJECT)
endif

ifeq ($(strip $(LITE)), 0)
	@$(COPY_FILE) $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/ControlPanel2.dll) $(call Path_process,$(BUILD_DIR)/win/bin/)
	@$(COPY_FILE) $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/ControlPanel2.lib) $(call Path_process,$(BUILD_DIR)/win/bin/)
	@$(COPY_FILE) $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/filesys.dll) $(call Path_process,$(BUILD_DIR)/win/bin/)
	@$(COPY_FILE) $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/filesys.lib) $(call Path_process,$(BUILD_DIR)/win/bin/)
	@$(COPY_FILE) $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/SIMConPad.dll) $(call Path_process,$(BUILD_DIR)/win/bin/)
	@$(COPY_FILE) $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/SIMConPad.lib) $(call Path_process,$(BUILD_DIR)/win/bin/)
	@$(COPY_FILE) $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/MSDev.exe) $(call Path_process,$(BUILD_DIR)/win/bin/)
	@$(COPY_FILE) $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/win_platform/bin/iHCR.dll) $(call Path_process,$(BUILD_DIR)/win/bin/)
endif
	-@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img/win" ""	
	-@$(COPY_DIR)  $(call Path_process,$(BUILD_DIR)/win/bin/*)  $(call Path_process,$(BUILD_DIR)/img/win/)

endif
	
devps.dsp:  project_$(PROJECT).mk make/$(SIMU_DIR)/simulator_main.mk  make/$(SIMU_DIR)/simulator.ini
	-@$(ECHO) "$(subst .dsp,,$@)"
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/$(subst .dsp,,$@)" ""
	-$(PERL) make/perl_script/rm_file.pl $@  "$(BUILD_DIR)/win/$(subst .dsp,,$@)"
	$(MAKE) -s  PROJECT=$(PROJECT) $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/$(subst .dsp,,$@)  -r -R COMPONENT=$(subst .dsp,,$@)
	
atwin.dsp:  project_$(PROJECT).mk make/$(SIMU_DIR)/simulator_main.mk  make/$(SIMU_DIR)/simulator.ini
	-@$(ECHO) "$(subst .dsp,,$@)"
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/$(subst .dsp,,$@)" ""
	-$(PERL) make/perl_script/rm_file.pl $@  "$(BUILD_DIR)/win/$(subst .dsp,,$@)"
	$(MAKE) -s  PROJECT=$(PROJECT) $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/$(subst .dsp,,$@)  -r -R COMPONENT=$(subst .dsp,,$@)

msdevkernel.dsp:  project_$(PROJECT).mk make/$(SIMU_DIR)/simulator_main.mk  make/$(SIMU_DIR)/simulator.ini
	-@$(ECHO) "$(subst .dsp,,$@)"
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/$(subst .dsp,,$@)" ""
	-$(PERL) make/perl_script/rm_file.pl $@  "$(BUILD_DIR)/win/$(subst .dsp,,$@)"
	$(MAKE) -s  PROJECT=$(PROJECT)  $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/$(subst .dsp,,$@)  -r -R COMPONENT=$(subst .dsp,,$@)

mbbms_sg.dsp:  project_$(PROJECT).mk make/$(SIMU_DIR)/simulator_main.mk  make/$(SIMU_DIR)/simulator.ini
	-@$(ECHO) "$(subst .dsp,,$@)"
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/$(subst .dsp,,$@)" ""
	-$(PERL) make/perl_script/rm_file.pl $@  "$(BUILD_DIR)/win/$(subst .dsp,,$@)"
	$(MAKE) -s  PROJECT=$(PROJECT)  $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/$(subst .dsp,,$@)  -r -R COMPONENT=$(subst .dsp,,$@)

%.dsp:  %.mk project_$(PROJECT).mk make/$(SIMU_DIR)/simulator_main.mk  make/$(SIMU_DIR)/simulator.ini
	-@$(ECHO) "$*"
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/$*" ""
	-$(PERL) make/perl_script/rm_file.pl $*.dsp "$(BUILD_DIR)/win/$*"
ifeq ($(strip $(JAVA_SUPPORT)),MYRIAD)
	$(MAKE)  -s PROJECT=$(PROJECT) MODULES=simulator $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/$*  -r -R COMPONENT=$* 
else
	$(MAKE)  -s PROJECT=$(PROJECT) $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/$*  -r -R COMPONENT=$* 
endif

ifeq ($(strip $(MMI_WATCH_UI)), TRUE)
dsp_mac_res:  createdir app_watch_main.mk project_$(PROJECT).mk make/$(SIMU_DIR)/simulator_main.mk  make/$(SIMU_DIR)/simulator.ini
	-@$(ECHO) "$*"
	$(PERL) make/perl_script/mk_dir.pl "app" $(BUILD_DIR)/obj
	$(PERL) make/perl_script/mk_dir.pl "app" $(BUILD_DIR)/dep
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/app" ""
	-@$(ARMCC) $(VIA) $(BUILD_DIR)/dep/simulator_C_MACRO.txt -E make/$(SIMU_DIR)/simulator.ini  -o $(BUILD_DIR)/tmp/simulator.ini.res
	$(MAKE)   PROJECT=$(PROJECT) $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/app  -r -R COMPONENT=app_watch_main dsp_mac_res
	$(PERL) make/perl_script/create_dsp_res.pl "$(PROJECT)" "app_watch_main" 
else
dsp_mac_res:  createdir app_main.mk project_$(PROJECT).mk make/$(SIMU_DIR)/simulator_main.mk  make/$(SIMU_DIR)/simulator.ini
	-@$(ECHO) "$*"
	$(PERL) make/perl_script/mk_dir.pl "app" $(BUILD_DIR)/obj
	$(PERL) make/perl_script/mk_dir.pl "app" $(BUILD_DIR)/dep
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/app" ""
	-@$(ARMCC) $(VIA) $(BUILD_DIR)/dep/simulator_C_MACRO.txt -E make/$(SIMU_DIR)/simulator.ini  -o $(BUILD_DIR)/tmp/simulator.ini.res
	$(MAKE)   PROJECT=$(PROJECT) $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/app  -r -R COMPONENT=app_main dsp_mac_res
	$(PERL) make/perl_script/create_dsp_res.pl "$(PROJECT)" "app_main" 
endif

.PHONEY:clean
clean:
	-$(PERL) make/perl_script/rm_dir.pl win "$(BUILD_DIR)/"
	-$(PERL) make/perl_script/rm_dir.pl win "$(BUILD_DIR)/img/"
