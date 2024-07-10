
include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifeq ($(strip $(SOUNDBANK_VER)), NEWMIDI_75KB)
MIDIVOLUME=_loud
endif

ifeq ($(strip $(SOUNDBANK_VER)), NEWMIDI_170KB)
MIDIVOLUME=_loud
endif

ifeq ($(strip $(SOUNDBANK_VER)), NEWMIDI_400KB)
MIDIVOLUME=_loud
endif

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/nv_C_MACRO.txt)
endif

Config_Seg=build/$(PROJECT)_builddir/tmp/config_info.nv

ifeq ($(strip $(RELEASE_INFO)),TRUE)
RELEASE_DEBUG = 1
else
RELEASE_DEBUG = 0
endif
ifeq ($(strip $(MAKE_OS)), Linux)
CMD_OS = 0
else
CMD_OS = 1
endif

ifeq ($(strip $(SPRD_CUSTOMER)),TRUE)
SPRD_CUSTOMER_MACRO = -DSPRD_CUSTOMER
else
SPRD_CUSTOMER_MACRO =
endif


ifneq ($(findstring $(strip $(PRODUCT_CONFIG)),uix8910_fpga uix8910 uis8910c_demophone uis8910c_mpphone uis8910ff_mpphone),)
NV_GROUP  = nvitem
ifeq ($(strip $(VIRTUAL_VIBRATE_FUNC)), TRUE)
nvitem         = common/nv_parameters/audio/audio_eq.nvm
ifeq ($(strip $(BT_DIALER_SUPPORT)), TRUE)
nvitem         += common/nv_parameters/audio/audio_btdialer_sc6531efm.nvm
else
nvitem         += common/nv_parameters/audio/audio_sc6531efm.nvm
endif
nvitem         += common/nv_parameters/audio/audio_arm_6531efm.nvm 
else
nvitem         = common/nv_parameters/audio/audio_eq.nvm 
ifeq ($(strip $(BT_DIALER_SUPPORT)), TRUE)
nvitem         += common/nv_parameters/audio/audio_btdialer_sc6531efm.nvm
else
ifeq ($(strip $(NXP_LVVE_SUPPORT)), TRUE) 
nvitem         += common/nv_parameters/audio/audio_sc6531efm.nvm
else
nvitem         += common/nv_parameters/audio/audio_sc6531efm_AEC.nvm
endif
endif
nvitem         += common/nv_parameters/audio/audio_arm_6531efm.nvm   
endif  
ifeq ($(strip $(NXP_LVVE_SUPPORT)), TRUE)     	 
nvitem         += common/nv_parameters/audio/audio_lvve.nvm   
endif
ifeq ($(strip $(BT_DIALER_SUPPORT)), TRUE)
nvitem         += common/nv_parameters/audio/audio_btdialer_dsp_codec_6531efm.nvm
else
ifeq ($(strip $(NXP_LVVE_SUPPORT)), TRUE)     	 
nvitem         += common/nv_parameters/audio/audio_dsp_codec_6531efm.nvm
else   
nvitem         += common/nv_parameters/audio/audio_dsp_codec_6531efm_AEC.nvm
endif
endif


ifeq ($(strip $(CHIP_SUBVER)), AA)
nvitem         += BASE/base_nv/CalibrationParam/Calibrationparam_sc6531efm_AA_7182.nvm \
            	 BASE/base_nv/DownloadParam/DownloadParam_sc6531efm_AA_7182.nvm
else
nvitem         += BASE/base_nv/CalibrationParam/Calibrationparam_sc6531efm_AB_7182.nvm \
            	 BASE/base_nv/DownloadParam/DownloadParam_sc6531efm_AB_7182.nvm
endif
nvitem         += BASE/base_nv/Classmark/nv_classmark.nvm  \
				 BASE/base_nv/BaseParam/BaseParam.nvm  \
            	 common/nv_parameters/nv_type/nv_type_uix8910.nvm  \
            	 common/nv_parameters/ProductionParam/ProductionParam_uix8910.nvm
 
nvitem         += common/nv_parameters/ProductionParam/license.nvm
nvitem         += common/nv_parameters/modem_nv/modem_nv_cat1bis_uix8910_static.nvm
nvitem         += common/nv_parameters/modem_nv/modem_nv_cat1bis_uix8910_phy.nvm
nvitem         += common/nv_parameters/modem_nv/modem_nv_cat1bis_uix8910_dynamic.nvm
nvitem         += common/nv_parameters/modem_nv/modem_nv_cat1bis_uix8910_card2_static.nvm
nvitem         += common/nv_parameters/modem_nv/modem_nv_cat1bis_uix8910_card2_phy.nvm
nvitem         += common/nv_parameters/modem_nv/modem_nv_cat1bis_uix8910_card2_dynamic.nvm
nvitem         += common/nv_parameters/modem_nv/RF_calib.nvm
nvitem         += common/nv_parameters/modem_nv/Cfw_nv.nvm
nvitem         += common/nv_parameters/modem_nv/RF_nv.nvm
nvitem         += common/nv_parameters/modem_nv/golden_board_param.nvm
ifeq ($(strip $(NXP_AUDIO_PARAM)), TRUE)     	 
nvitem         += common/nv_parameters/audio/audio_calib_NXP.nvm
else   
nvitem         += common/nv_parameters/audio/audio_calib_VQE.nvm
endif
#nvitem				 += common/nv_parameters/modem_nv/modem_nv_card2_cat1bis_uix8910.nvm
nvitem				 += common/nv_parameters/modem_nv/ims_nv.nvm
endif ##($(findstring $(strip $(PRODUCT_CONFIG)),uix8910_fpga uix8910 uis8910c_demophone uis8910c_mpphone uis8910ff_mpphone ),)


ifeq ($(strip $(PRODUCT_CONFIG)), uis8910ff_zxf)
NV_GROUP  = nvitem
ifeq ($(strip $(VIRTUAL_VIBRATE_FUNC)), TRUE)
nvitem         = common/nv_parameters/audio/uis8910ff_zxf/audio_eq.nvm
ifeq ($(strip $(BT_DIALER_SUPPORT)), TRUE)
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_btdialer_sc6531efm.nvm
else
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_sc6531efm.nvm
endif
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_arm_6531efm.nvm 
else
nvitem         = common/nv_parameters/audio/uis8910ff_zxf/audio_eq.nvm 
ifeq ($(strip $(BT_DIALER_SUPPORT)), TRUE)
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_btdialer_sc6531efm.nvm
else
ifeq ($(strip $(NXP_LVVE_SUPPORT)), TRUE) 
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_sc6531efm.nvm
else
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_sc6531efm_AEC.nvm
endif
endif
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_arm_6531efm.nvm   
endif  
ifeq ($(strip $(NXP_LVVE_SUPPORT)), TRUE)     	 
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_lvve.nvm   
endif

ifeq ($(strip $(BT_DIALER_SUPPORT)), TRUE)
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_btdialer_dsp_codec_6531efm.nvm
else
ifeq ($(strip $(NXP_LVVE_SUPPORT)), TRUE)     	 
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_dsp_codec_6531efm.nvm
else   
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_dsp_codec_6531efm_AEC.nvm
endif
endif

ifeq ($(strip $(NXP_AUDIO_PARAM)), TRUE)     	 
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_calib_NXP.nvm
else   
ifeq ($(strip $(ZXF_NV)), Z606)     
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_calib_VQE_z606.nvm
else ifeq ($(strip $(ZXF_NV)), Z685)     
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_calib_VQE_z685.nvm
else
nvitem         += common/nv_parameters/audio/uis8910ff_zxf/audio_calib_VQE.nvm
endif
endif


ifeq ($(strip $(CHIP_SUBVER)), AA)
nvitem         += BASE/base_nv/CalibrationParam/Calibrationparam_sc6531efm_AA_7182.nvm \
            	 BASE/base_nv/DownloadParam/DownloadParam_sc6531efm_AA_7182.nvm
else
nvitem         += BASE/base_nv/CalibrationParam/Calibrationparam_sc6531efm_AB_7182.nvm \
            	 BASE/base_nv/DownloadParam/DownloadParam_sc6531efm_AB_7182.nvm
endif
nvitem         += BASE/base_nv/Classmark/nv_classmark.nvm  \
				 BASE/base_nv/BaseParam/BaseParam.nvm  \
            	 common/nv_parameters/nv_type/nv_type_uix8910.nvm  \
            	 common/nv_parameters/ProductionParam/ProductionParam_uix8910.nvm
 
nvitem         += common/nv_parameters/ProductionParam/license.nvm

nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/modem_nv_cat1bis_uix8910_static.nvm
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/modem_nv_cat1bis_uix8910_phy.nvm
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/modem_nv_cat1bis_uix8910_dynamic.nvm
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/modem_nv_cat1bis_uix8910_card2_static.nvm
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/modem_nv_cat1bis_uix8910_card2_phy.nvm
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/modem_nv_cat1bis_uix8910_card2_dynamic.nvm
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/RF_calib.nvm
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/Cfw_nv.nvm
ifeq ($(strip $(ZXF_NV)), Z606)     
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/RF_nv_z606.nvm
else ifeq ($(strip $(ZXF_NV)), Z685)     
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/RF_nv_z685.nvm
else
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/RF_nv.nvm
endif
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/golden_board_param.nvm
nvitem         += common/nv_parameters/modem_nv/uis8910ff_zxf/ims_nv.nvm


endif ##ifeq ($(strip $(PRODUCT_CONFIG)), uis8910ff_zxf)



ifeq ($(strip $(PRODUCT_CONFIG)), uws6121e)
NV_GROUP  = nvitem
CP_DELTANV := lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/deltanv/modem_config.nv
nvitem = common/nv_parameters/audio/AudioCvsParam.nvm

ifeq ($(strip $(CHIP_SUBVER)), AA)
nvitem         +=BASE/base_nv/DownloadParam/DownloadParam_sc6531efm_AA_7182.nvm
else
nvitem         +=BASE/base_nv/DownloadParam/DownloadParam_sc6531efm_AB_7182.nvm
endif
nvitem         += BASE/base_nv/Classmark/nv_classmark.nvm  \
            	 common/nv_parameters/nv_type/nv_type.nvm  \
            	 common/nv_parameters/ProductionParam/ProductionParam_uws6121e.nvm	\
				 common/nv_parameters/nv_type/GPS_nv.nvm	\
				 common/nv_parameters/nv_type/gps_tsx_data.nvm

nvitem         += common/nv_parameters/ProductionParam/license.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/calibrationparam.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/modem_nv_UWS6121E_dynamic.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/modem_nv_UWS6121E_phy.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/modem_nv_UWS6121E_static.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/nv_type_UWS6121E.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/RF_calib_modem.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/Cfw_nv_volte.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/RF_nv.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/golden_board_param.nvm
nvitem	       += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/ims_nv.nvm
nvitem	       += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/SimlockParam.nvm
endif ##($(findstring $(strip $(PRODUCT_CONFIG)),uws6121e),)

ifeq ($(strip $(PRODUCT_CONFIG)), uws6121e_openphone)
NV_GROUP  = nvitem
CP_DELTANV := lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/deltanv/modem_config.nv
nvitem = common/nv_parameters/audio/AudioCvsParam.nvm

ifeq ($(strip $(CHIP_SUBVER)), AA)
nvitem         +=BASE/base_nv/DownloadParam/DownloadParam_sc6531efm_AA_7182.nvm
else
nvitem         +=BASE/base_nv/DownloadParam/DownloadParam_sc6531efm_AB_7182.nvm
endif
nvitem         += BASE/base_nv/Classmark/nv_classmark.nvm  \
                 common/nv_parameters/nv_type/nv_type.nvm  \
                 common/nv_parameters/ProductionParam/ProductionParam_uws6121e.nvm	\
                 common/nv_parameters/nv_type/GPS_nv.nvm	\
                 common/nv_parameters/nv_type/gps_tsx_data.nvm

nvitem         += common/nv_parameters/ProductionParam/license.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/calibrationparam.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/modem_nv_UWS6121E_dynamic.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/modem_nv_UWS6121E_phy.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/modem_nv_UWS6121E_static.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/nv_type_UWS6121E.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/RF_calib_modem.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/Cfw_nv_volte.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/RF_nv.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/golden_board_param.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/ims_nv.nvm
nvitem         += lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/nvitem/SimlockParam.nvm
endif ##($(findstring $(strip $(PRODUCT_CONFIG)),uws6121e_openphone),)

NV_GROUP_NAME := NV_GROUP
NV_C_MACRO_FILE := $(BUILD_DIR)/dep/nv_C_MACRO.txt

include make/nv_parameter/nv_parameter_variant/nv_parameter_variant.mk # Please notice the place!!!


NV_GROUP_OBJ := $(foreach word,$($(strip $(NV_GROUP_NAME))),$(word)=$($(word));)
NV_GROUP_PATH := $(foreach word,$($(strip $(NV_GROUP_NAME))),$(BUILD_DIR)/nv/$(word)/*.bin)
ifeq ($(strip $(RELEASE_INFO)),TRUE)
  OLD_NV_GROUP_PATH := $(foreach word,$($(strip $(NV_GROUP_NAME))),$(word)_release.bin)
  NV_GROUP_PAC := $(foreach word,$($(strip $(NV_GROUP_NAME))),$(word)_release)
else
  OLD_NV_GROUP_PATH := $(foreach word,$($(strip $(NV_GROUP_NAME))),$(word).bin)
  NV_GROUP_PAC := $(foreach word,$($(strip $(NV_GROUP_NAME))),$(word))
endif

DELTA_NV_PATH := ./lib/modem/$(strip $(DELTA_NV_CONFIG_PATH))/$(strip $(MODEM_PROJECT_FOLDER))
DELTA_NV_PROJECT := $(strip $(DELTA_NV_CONFIG_PATH))

HW_NV_PATH := ./lib/modem/$(strip $(DELTA_NV_CONFIG_PATH))/$(strip $(MODEM_PROJECT_FOLDER))/indeltanv
HW_NV_LIST = $(notdir $(wildcard $(HW_NV_PATH)/*.nv))

ROOT_DIR	= $(shell $(PERL) ./make/perl_script/get_root_fullpath.pl)

ifneq ($(strip $(MAKECMDGOALS)),clean)
#  ifneq ($(strip $(MULTI_LAN_SUPPORT)),NONE)
    NVDEP := $(shell $(MAKE) PROJECT=$(PROJECT) -f $(call Path_process,make/pclint/pclint.mk) -k -r -R COMPONENT=$(APP_MK) nvdep)
#  endif
endif

.PHONEY:all
all : builddir
	$(ECHO) $(NV_GROUP_PAC) > $(BUILD_DIR)/tmp/$(NV_GROUP_NAME).txt
	@$(PERL) make/perl_script/rm_dir.pl $(NV_GROUP_PAC) $(BUILD_DIR)/img 
	@-$(PERL) make/perl_script/rm_file.pl "$(OLD_NV_GROUP_PATH)" $(BUILD_DIR)/img/

ifeq ($(strip $(CHIP_ENDIAN)),LITTLE)
ifeq ($(strip $(RF_HW_SELFADAPTIVE_ENABLE)),TRUE)
	@$(ECHO) ./make/perl_script/nv_parameter_le.pl "$(NV_GROUP_OBJ)" $(BUILD_DIR)/nv $(CMD_OS) "$(ROOT_DIR)" "$(Config_Seg)"  $(RELEASE_DEBUG) $(strip $(RF_HW_SELFADAPTIVE_CONFIG_NV_DIR))
	@$(PERL) ./make/perl_script/nv_parameter_le.pl "$(NV_GROUP_OBJ)" $(BUILD_DIR)/nv $(CMD_OS) "$(ROOT_DIR)" "$(Config_Seg)"  $(RELEASE_DEBUG) $(strip $(RF_HW_SELFADAPTIVE_CONFIG_NV_DIR))
else
	@$(ECHO) ./make/perl_script/nv_parameter_le.pl "$(NV_GROUP_OBJ)" $(BUILD_DIR)/nv $(CMD_OS) "$(ROOT_DIR)" "$(Config_Seg)"  $(RELEASE_DEBUG)
	@$(PERL) ./make/perl_script/nv_parameter_le.pl "$(NV_GROUP_OBJ)" $(BUILD_DIR)/nv $(CMD_OS) "$(ROOT_DIR)" "$(Config_Seg)"  $(RELEASE_DEBUG)
endif

else
	@$(PERL) ./make/perl_script/nv_parameter.pl "$(NV_GROUP_OBJ)" $(BUILD_DIR)/nv $(CMD_OS) "$(ROOT_DIR)" "$(Config_Seg)"  $(RELEASE_DEBUG)
endif
ifneq ($(VARIANT_PID),)
	@$(call variant_prj_file_copy)
endif
	@$(COPY_DIR) $(call Path_process,$(BUILD_DIR)/nv/*)  $(call Path_process,$(BUILD_DIR)/img/)
	
ifeq ($(strip $(MAKE_OS)), Linux)
	@$(COPY_FILE) `find $(BUILD_DIR)/nv/ -iname "*.bin"` $(call Path_process,$(BUILD_DIR)/img/)
else
	@$(CP_FILE) $(call Path_process,$(NV_GROUP_PATH))  $(call Path_process,$(BUILD_DIR)/img/)
endif
	
ifeq ($(strip $(DELTA_NV_BIN_SUPPORT)),TRUE)
	@$(ECHO) "make delta nv "
	@$(COPY_DIR) $(call Path_process,$(strip $(DELTA_NV_PATH))/ims_delta_nv/*.nv)  $(call Path_process,$(BUILD_DIR)/tmp/delta_nv)
	@$(COPY_DIR) $(call Path_process,$(strip $(DELTA_NV_PATH))/ims_delta_nv/*.xml)  $(call Path_process,$(BUILD_DIR)/tmp/delta_nv)
	$(MAKE) $(jobopt) -f make/nv_parameter/nv_parameter.mk -k -r -R PROJECT=$(PROJECT) $(HW_NV_LIST)
	$(PERL) ./make/perl_script/nv_parameter_le.pl "$(NV_GROUP_OBJ)" "$(BUILD_DIR)/nv" $(CMD_OS) "$(ROOT_DIR)" "$(Config_Seg)"  $(RELEASE_DEBUG)  $(strip $(DELTA_NV_PROJECT)) "$(BUILD_DIR)/tmp/delta_nv"

ifeq ($(strip $(MAKE_OS)), Linux)
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/tmp/delta_nv/delta_nv.bin)  $(call Path_process,$(BUILD_DIR)/img/$(PROJECT)_delta_nv.bin)
else
	@$(CP_FILE) $(call Path_process,$(BUILD_DIR)/tmp/delta_nv/delta_nv.bin)  $(call Path_process,$(BUILD_DIR)/img/$(PROJECT)_delta_nv.bin)
endif
	$(PERL) make/perl_script/rm_file.pl delta_nv.bin $(DELTA_NV_PATH)/
endif
	
#	@$(PERL) make/perl_script/rm_dir.pl nv $(BUILD_DIR)

.PHONEY:builddir
builddir:
	@$(PERL) make/perl_script/rm_dir.pl nv $(BUILD_DIR)/
	@$(PERL) make/perl_script/rm_dir.pl "delta_nv" $(BUILD_DIR)/tmp/ 
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)" ""
	@$(PERL) make/perl_script/mk_dir.pl "nv" $(BUILD_DIR)/
	@$(PERL) make/perl_script/mk_dir.pl "delta_nv" $(BUILD_DIR)/tmp/ 
	@$(PERL) make/perl_script/mk_dir.pl "$($(strip $(NV_GROUP_NAME)))" $(BUILD_DIR)/nv/
ifeq ($(strip $(MMI_VER)), STABLE)
	$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(BUILD_DIR)/tmp/app.macro.nv -E $(MMI_DIR)/source/mmi_app/app/setting/h/mmiset_export.h -o $(BUILD_DIR)/tmp/mmiset_export.h.tmp
	$(COPY_FILE) $(call Path_process,make/nv_parameter/config_info.nv) $(call Path_process,$(Config_Seg).tmp)
	$(COPY_FILE) $(call Path_process,tools/DEBUG_TOOL/ResearchDownload/Bin/ResearchDownload.ini) $(call Path_process,$(BUILD_DIR)/tmp/ResearchDownload.ini)
	$(ADD_WRITE_PROP)  $(call Path_process,$(Config_Seg).tmp)
	$(ADD_WRITE_PROP)  $(call Path_process,tools/DEBUG_TOOL/ResearchDownload/Bin/ResearchDownload.ini)
	$(ADD_WRITE_PROP)  $(call Path_process,$(BUILD_DIR)/tmp/ResearchDownload.ini)

	$(call variant_config_add)


ifneq ($(VARIANT_PID),)
	$(PERL) make/perl_script/get_language_index_lp.pl $(BUILD_DIR)/tmp/mmiset_export.h.tmp $(BUILD_DIR)/tmp/ResearchDownload.ini >> $(call Path_process,$(Config_Seg).tmp)
else
	$(PERL) make/perl_script/get_language_index.pl $(BUILD_DIR)/tmp/mmiset_export.h.tmp $(BUILD_DIR)/tmp/ResearchDownload.ini >> $(call Path_process,$(BUILD_DIR)/tmp/config_info.nv.tmp)
endif

	$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(NV_C_MACRO_FILE) -D$(PROJECT) -E $(Config_Seg).tmp -o "$(Config_Seg)"
	$(PERL) ./make/perl_script/diff.pl  $(BUILD_DIR)/tmp/ResearchDownload.ini  tools/DEBUG_TOOL/ResearchDownload/Bin/ResearchDownload.ini $(BUILD_DIR)/tmp/ResearchDownload.ini.cmp
	$(PERL) ./make/perl_script/file_diff_process.pl tools/DEBUG_TOOL/ResearchDownload/Bin/ResearchDownload.ini build/$(PROJECT)/tmp/ResearchDownload.ini  build/$(PROJECT)/tmp/ResearchDownload.ini.cmp
endif
ifeq ($(strip $(MMI_VER)), MAIN)
#ifneq ($(strip $(MULTI_LAN_SUPPORT)),NONE)
	$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(BUILD_DIR)/tmp/app_main.macro.nv -E $(MMI_DIR)/source/mmi_service/export/inc/mmi_base_common.h -o $(BUILD_DIR)/tmp/mmi_base_common.h.tmp
	$(COPY_FILE) $(call Path_process,make/nv_parameter/config_info.nv) $(call Path_process,$(Config_Seg).tmp)
	$(COPY_FILE) $(call Path_process,tools/DEBUG_TOOL/ResearchDownload/Bin/BMFileType.ini) $(call Path_process,$(BUILD_DIR)/tmp/BMFileType.ini)

	$(ADD_WRITE_PROP)  $(call Path_process,$(Config_Seg).tmp)
	$(ADD_WRITE_PROP)  $(call Path_process,tools/DEBUG_TOOL/ResearchDownload/Bin/BMFileType.ini)
	$(ADD_WRITE_PROP)  $(call Path_process,$(BUILD_DIR)/tmp/BMFileType.ini)        

	$(call variant_config_add)
	
ifeq ($(strip $(PRODUCT_CONFIG)), uws6121e)
	@$(COPY_DIR) $(call Path_process,$(strip $(DELTA_NV_PATH))/deltanv/*.nv)  $(call Path_process,$(BUILD_DIR)/tmp/)
	$(CAT) $(call Path_process,$(CP_DELTANV)) >> $(call Path_process,$(Config_Seg).tmp) 
endif

ifeq ($(strip $(PRODUCT_CONFIG)), uws6121e_openphone)
	@$(COPY_DIR) $(call Path_process,$(strip $(DELTA_NV_PATH))/deltanv/*.nv)  $(call Path_process,$(BUILD_DIR)/tmp/)
	$(CAT) $(call Path_process,$(CP_DELTANV)) >> $(call Path_process,$(Config_Seg).tmp) 
endif

ifneq ($(VARIANT_PID),)
	$(PERL) make/perl_script/get_language_index_lp.pl $(BUILD_DIR)/tmp/mmi_base_common.h.tmp $(BUILD_DIR)/tmp/BMFileType.ini >> $(call Path_process,$(Config_Seg).tmp)
	$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(NV_C_MACRO_FILE)  -D$(PROJECT) -E $(Config_Seg).tmp -o "$(Config_Seg)"
else
	$(PERL) make/perl_script/get_language_index.pl $(BUILD_DIR)/tmp/mmi_base_common.h.tmp $(BUILD_DIR)/tmp/BMFileType.ini >> $(call Path_process,$(BUILD_DIR)/tmp/config_info.nv.tmp)
	$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(NV_C_MACRO_FILE)  -D$(PROJECT) -E $(BUILD_DIR)/tmp/config_info.nv.tmp -o "$(Config_Seg)"

endif

#   $(PERL) ./make/perl_script/diff.pl  $(BUILD_DIR)/tmp/BMFileType.ini  tools/DEBUG_TOOL/ResearchDownload/Bin/BMFileType.ini $(BUILD_DIR)/tmp/BMFileType.ini.cmp
#   $(PERL) ./make/perl_script/file_diff_process.pl tools/DEBUG_TOOL/ResearchDownload/Bin/BMFileType.ini build/$(PROJECT)/tmp/BMFileType.ini  build/$(PROJECT)/tmp/BMFileType.ini.cmp
	$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/tmp/BMFileType.ini) $(call Path_process,tools/DEBUG_TOOL/ResearchDownload/Bin/BMFileType.ini)
#else
#	$(ARMCC) $(VIA)  $(BUILD_DIR)/dep/nv_C_MACRO.txt $(SPRD_CUSTOMER_MACRO) -E make/nv_parameter/config_info.nv  -o "$(Config_Seg)"
#endif
endif

.PHONEY:$(HW_NV_LIST)
$(HW_NV_LIST):
	$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(NV_C_MACRO_FILE)  -D$(PROJECT) -E "$(HW_NV_PATH)/$@"  -o "$(BUILD_DIR)/tmp/delta_nv/$@"

.PHONEY:clean
clean:
	@$(PERL) make/perl_script/rm_dir.pl nv $(BUILD_DIR)/ 
