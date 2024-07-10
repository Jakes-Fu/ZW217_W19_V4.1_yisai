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
dsp_modules_no_debug= cc voice aac_lc ra8lbr wma dra\
rv_codec video_security video_security_kmv video_security_hmv azan nes aud_dsp_mng chip_drv theme_unzip

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

ifeq ($(strip $(AMR_SUPPORT)),TRUE)
    #dsp_modules += amr_arm
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

#dsp_modules += mpeg4_codec

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
#   dsp_modules += mtv_adapter os_al mtv_api
   dsp_modules += mtv_adapter
   dsp_modules += mtv_service
endif

ifeq ($(strip $(MBBMS_SUPPORT)), TRUE)
   dsp_modules += mbbms_sg
endif

ifeq ($(strip $(MMI_AZAN_SUPPORT)),TRUE)
    dsp_modules += azan                             
endif


ifeq ($(strip $(QBTHEME_SUPPORT)),TRUE)
    dsp_modules += theme_unzip
endif

dsp_modules_cus = chip_drv utility utility_performance app_main gui_main ctrl mmk_main mmk_sys_main mmi_service msdevkernel
ifeq ($(strip $(ENABLE_WEBKIT)), TRUE)
dsp_modules_cus	+=  webkit webcore jscore skia harfbuzz libpng zlib freetype icu4c libxml2 libc
endif

ifeq ($(strip $(DYNAMIC_MODULE_SUPPORT)), TRUE)
	dsp_modules_cus += caf_interface
endif

ifeq ($(strip $(JUNIVERSALCHARDET_SUPPORT)), TRUE)
	dsp_modules_cus += juniversalchardet
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
ifeq ($(strip $(TF_LOAD_SUPPORT)), TRUE)
macro_set += TF_LOAD_SUPPORT=FALSE
endif

ifeq ($(strip $(NEW_MEMORY_MM)), FALSE)
macro_set += NEW_MEMORY_MM=TRUE
endif

ifneq ($(strip $(INTERNAL_LIBS)),)
dsp_modules_cus := $(filter-out devps,$(dsp_modules_cus) $(dsp_modules))
endif

ADD_LINK32  = kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib winmm.lib

ADD_LINK32 +=  ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib LIBCMT.LIB msvcrtd.lib 

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
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/skin/mssim_duallcd_240_320_bar.png)  $(call Path_process,$(BUILD_DIR)/win/)
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/winsimulator_tp/skin/mssim_duallcd_240_320_bar.ini)  $(call Path_process,$(BUILD_DIR)/win/)
	
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
	
	@$(PERL) make/perl_script/create_msdev_ini.pl "$(PROJECT)" "$(strip $(SKIN_FILE_NAME))" "$(strip $(MMI_RES_DIR))" "$(strip $(SUFFIX))" "$(call Path_process,$(strip $(ROOT_DIR)))" "$(strip $(suffix_res_file))" "$(strip $(MMI_DIR))"

	-@$(ARMCC) $(VIA) $(BUILD_DIR)/dep/simulator_C_MACRO.txt $(SPRD_CUSTOMER_MACRO) -E make/$(SIMU_IDH)/simulator.ini  -o build/$(PROJECT)_builddir/tmp/simulator.ini
	@$(PERL) make/perl_script/create_dsp.pl "$(PROJECT)" "$(dsp_modules)" "$(dsp_modules_cus)" "$(SPRD_CUS)" "$(ADD_LINK32)" "$(ACTIVE_PRJ)" "$(strip $(PRODUCT_BASELIB_DIR))" "$(dsp_modules_no_debug)"
ifeq ($(strip $(RUN)),1)
ifneq ($(strip $(IB)),)
	BuildConsole build\$(PROJECT)_builddir\win\simulator.dsw /rebuild /prj=$(strip $(ACTIVE_PRJ))  /cfg="Win32 Debug" /ALL
else
	msdev build\$(PROJECT)_builddir\win\simulator.dsw /MAKE "$(strip $(ACTIVE_PRJ)) - Win32 Debug" /REBUILD
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
	
msdevkernel.dsp:  project_$(PROJECT).mk make/$(SIMU_IDH)/simulator_idh.mk  make/$(SIMU_IDH)/simulator.ini
	-@$(ECHO) "$(subst .dsp,,$@)"
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/$(subst .dsp,,$@)" ""
	-$(PERL) make/perl_script/rm_file.pl $@  "$(BUILD_DIR)/win/$(subst .dsp,,$@)"
	$(MAKE) -s  PROJECT=$(PROJECT)  $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/$(subst .dsp,,$@)  -r -R COMPONENT=$(subst .dsp,,$@)

%.dsp:  %.mk project_$(PROJECT).mk make/$(SIMU_IDH)/simulator_idh.mk  make/$(SIMU_IDH)/simulator.ini
	-@$(ECHO) "$*"
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/$*" ""
	-$(PERL) make/perl_script/rm_file.pl $*.dsp "$(BUILD_DIR)/win/$*"
ifeq ($(strip $(JAVA_SUPPORT)),MYRIAD)
	$(MAKE)  -s PROJECT=$(PROJECT) MODULES=simulator $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/$*  -r -R COMPONENT=$* 
else
	$(MAKE)  -s PROJECT=$(PROJECT) $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/$*  -r -R COMPONENT=$* 
endif

dsp_mac_res:  createdir app_main.mk project_$(PROJECT).mk make/$(SIMU_IDH)/simulator_idh.mk  make/$(SIMU_IDH)/simulator.ini
	-@$(ECHO) "$*"
	$(PERL) make/perl_script/mk_dir.pl "app" $(BUILD_DIR)/obj
	$(PERL) make/perl_script/mk_dir.pl "app" $(BUILD_DIR)/dep
	-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win/app" ""
	-@$(ARMCC) $(VIA) $(BUILD_DIR)/dep/simulator_C_MACRO.txt -E make/$(SIMU_IDH)/simulator.ini  -o $(BUILD_DIR)/tmp/simulator.ini.res
	$(MAKE)   PROJECT=$(PROJECT) $(macro_set) -f ./make/perl_script/modules_dsp.pl  -I ./$(BUILD_DIR)/win/app  -r -R COMPONENT=app_main dsp_mac_res
	$(PERL) make/perl_script/create_dsp_res.pl "$(PROJECT)" "app_main" 

.PHONEY:clean
clean:
	-$(PERL) make/perl_script/rm_dir.pl win "$(BUILD_DIR)/"
	-$(PERL) make/perl_script/rm_dir.pl win "$(BUILD_DIR)/img/"
