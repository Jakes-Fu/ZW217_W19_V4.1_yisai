# the same header as resource.mk

include make/$(RES_MAKE_DIR)/resource_header.mk

.PHONEY:all
all :builddir macro_create $(SOURCE_OBJ) $(PROJECT_OBJ)
ifneq ($(strip $(CHIP_ENDIAN)), LITTLE)
  ifeq ($(strip $(MMI_RES_ORIENT)),LANDSCAPE)
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res_win/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res_win/$(MMI_RES_DIR).bin" "$(PERL)"  -rep $(REP_FLAG)
  else
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res_win/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res_win/$(MMI_RES_DIR).bin" "$(PERL)"  -rep $(REP_FLAG) -resdir "$(ROOT_DIR)/$(MMI_DIR)/source/resource/$(MMI_RES_DIR)"
  endif
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res_win/$(MMI_RES_DIR)_win.bin) $(call Path_process,$(BUILD_DIR)/img/)
else
  ifeq ($(strip $(MMI_RES_ORIENT)),LANDSCAPE)
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res_win/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res_win/$(MMI_RES_DIR).bin" "$(PERL)" -L -rep $(REP_FLAG)
  else
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res_win/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res_win/$(MMI_RES_DIR).bin" "$(PERL)" -L -rep $(REP_FLAG) -resdir "$(ROOT_DIR)/$(MMI_DIR)/source/resource/$(MMI_RES_DIR)"
  endif
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res_win/$(MMI_RES_DIR).bin) $(call Path_process,$(BUILD_DIR)/img/$(MMI_RES_DIR)_win.bin)
endif
	@-$(PERL) make/perl_script/rm_dir.pl $(BUILD_DIR)/res_win ""

.PHONEY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "build" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)" ""
	@$(PERL) make/perl_script/rm_dir.pl res_win $(BUILD_DIR)/
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR)_win.bin $(BUILD_DIR)/img/
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res_win" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img" ""
ifeq ($(strip $(MMI_RES_ORIENT)),LANDSCAPE)
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/FONT" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/RING" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/common" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/common/MMI_RES_DEFAULT" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/common/MMI_RES_DEFAULT/DATA" ""
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/*.xls)  $(call Path_process,$(BUILD_DIR)/res/)
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/FONT/*)  $(call Path_process,$(BUILD_DIR)/res/FONT/)
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/RING/*)  $(call Path_process,$(BUILD_DIR)/res/RING/)
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/DATA/*)  $(call Path_process,$(BUILD_DIR)/res/common/MMI_RES_DEFAULT/DATA/)
	$(COPY_DIR)  $(call Path_process,$(MMI_DIR)/source/resource/$(MMI_RES_DIR)/*)  $(call Path_process,$(BUILD_DIR)/res/)
	$(PERL) make/perl_script/ProcessResDir.pl $(BUILD_DIR)/res
endif
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/tmp" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(SOURCE_DIR)" $(BUILD_DIR)/res_win/

.PHONEY:macro_create
macro_create:
	make PROJECT=$(PROJECT) -f make/$(SIMU_DIR)/simulator.mk -k -r -R COMPONENT=app dsp_mac_res

%_mdu_def.source :%_mdu_def.h
	@$(ECHO) Compiling resource source file $< ...
	@$(ARMCC) $(VIA) $(BUILD_DIR)/win/app/app.dsp_mac_res_final $(INCPATH)  -E $<  -o build/$(PROJECT)_builddir/res_win/$*/$*_mdu_def.h
	@$(ECHO) $*  build finished

%.project :%.h
	@$(ECHO) Compiling resource project file $< ...
	@$(PERL) make/perl_script/rm_dir.pl $*.h build/$(PROJECT)_builddir/res_win/
	@$(ARMCC) $(VIA) $(BUILD_DIR)/win/app/app.dsp_mac_res_final $(INCPATH)  -E $<  -o build/$(PROJECT)_builddir/res_win/$*.h
	@$(ECHO) $*  build finished
	
.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_dir.pl $(BUILD_DIR)/res_win ""
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR)_win.bin $(BUILD_DIR)/img/


	

