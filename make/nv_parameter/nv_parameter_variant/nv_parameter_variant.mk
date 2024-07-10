#
# nv_parameter_variant.mk - NV makefile for customer
# 2015	Candle Sun <candle.sun@spreadtrum.com>
#

# Removes macro file first, or name will be lowercased if already existing
ifneq ($(VARIANT_PID),)
$(shell $(PERL) make/perl_script/rm_file.pl $(patsubst %.txt,%$(if $(VARIANT_PID),_$(VARIANT_PID)).txt,$(NV_C_MACRO_FILE)) "")
endif
$(shell $(MOVE) $(NV_C_MACRO_FILE) $(patsubst %.txt,%$(if $(VARIANT_PID),_$(VARIANT_PID)).txt,$(NV_C_MACRO_FILE)))
NV_C_MACRO_FILE := $(patsubst %.txt,%$(if $(VARIANT_PID),_$(VARIANT_PID)).txt,$(NV_C_MACRO_FILE))
Config_Seg := build/$(PROJECT)_builddir/tmp/config_info$(if $(VARIANT_PID),_$(VARIANT_PID)).nv
nvitem += $(wildcard common/nv_parameters/nv_variant/*.nvm)
ifneq ($(VARIANT_PID),)
nvitem$(if $(VARIANT_PID),_$(VARIANT_PID)) := $(strip $(nvitem))
endif
$(strip $(NV_GROUP_NAME))$(if $(VARIANT_PID),_$(VARIANT_PID)) := $(strip $($(strip $(NV_GROUP_NAME))))
NV_GROUP_NAME := $(strip $(NV_GROUP_NAME))$(if $(VARIANT_PID),_$(VARIANT_PID))
$(NV_GROUP_NAME) := $(strip $(foreach item,$($(NV_GROUP_NAME)),$(item)$(if $(VARIANT_PID),_$(VARIANT_PID))))
VARIANT_LP := $(word 1,$(subst _,$(SPACE),$(strip $(VARIANT_PID))))
VARIANT_PC := $(word 2,$(subst _,$(SPACE),$(strip $(VARIANT_PID))))
VARIANT_CONFIG_INFO := make/nv_parameter/nv_parameter_variant/config_info$(if $(VARIANT_LP),_$(VARIANT_LP)).nv
VARIANT_CONFIG_INFO_SW_VERSION := make/nv_parameter/nv_parameter_variant/config_info_sw_version.nv

# For define, final blank line is needed
define variant_prj_file_copy
$(COPY_FILE) $(call Path_process,common/nv_parameters/nv_variant/nvitem_VARIANT.prj)  $(call Path_process,$(BUILD_DIR)/nv/nvitem$(if $(VARIANT_PID),_$(VARIANT_PID)))

$(shell $(PERL) make/perl_script/gen_software_version_to_fixnv.pl)

endef
define variant_config_add
$(ECHO) >> $(call Path_process,$(Config_Seg).tmp)
$(CAT) $(call Path_process,$(VARIANT_CONFIG_INFO)) >> $(call Path_process,$(Config_Seg).tmp) 
$(CAT) $(call Path_process,$(VARIANT_CONFIG_INFO_SW_VERSION)) >> $(call Path_process,$(Config_Seg).tmp) 
$(ECHO) >> $(call Path_process,$(Config_Seg).tmp)

endef
