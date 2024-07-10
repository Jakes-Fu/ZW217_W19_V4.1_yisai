
.PHONEY:all
all :builddir macro_create $(SOURCE_OBJ) $(PROJECT_OBJ)

ifeq ($(strip $(MIDI_SUPPORT)), TRUE)
   ifeq ($(strip $(SOUNDBANK_MODE)),COMPACT)
	$(PERL) make/perl_script/midi_proc.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "-resdir" "$(ROOT_DIR)/MS_MMI/source/resource/$(MMI_RES_DIR)" "-tmpdir" "$(ROOT_DIR)/$(BUILD_DIR)/tmp" "-isbf" "$(ROOT_DIR)/$(IN_SOUNDBANK_FILE)"  "-sbps" "$(SOUNTBANK_BYTES_PER_SAMPLE)" "-osbf" "$(ROOT_DIR)/$(BUILD_DIR)/tmp/midi_soundbank_compact.c"
   endif
endif

ifneq ($(strip $(CHIP_ENDIAN)), LITTLE)
ifeq ($(strip $(MMI_RES_ORIENT)),LANDSCAPE)
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res/$(MMI_RES_DIR).bin" "$(PERL)"  -rep $(REP_FLAG) -alu $(ALU_FLAG) -crm $(CRM_FLAG)
else
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res/$(MMI_RES_DIR).bin" "$(PERL)"  -rep $(REP_FLAG) -alu $(ALU_FLAG) -crm $(CRM_FLAG) -resdir "$(ROOT_DIR)/MS_MMI/source/resource/$(MMI_RES_DIR)"
endif
#ifeq ($(strip $(MULTI_THEME_SUPPORT)),TRUE)
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR).bin) $(call Path_process,$(BUILD_DIR)/img/)
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_win.bin) $(call Path_process,$(BUILD_DIR)/img/)
#else
#	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_DEFAULT.bin)  $(call Path_process,$(BUILD_DIR)/img/$(MMI_RES_DIR).bin)
#	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_DEFAULT_win.bin) $(call Path_process,$(BUILD_DIR)/img/$(MMI_RES_DIR)_win.bin)
#endif
else
ifeq ($(strip $(MMI_RES_ORIENT)),LANDSCAPE)
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res/$(MMI_RES_DIR).bin" "$(PERL)" -L -rep $(REP_FLAG) -alu $(ALU_FLAG) -crm $(CRM_FLAG)
else
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res/$(MMI_RES_DIR).bin" "$(PERL)" -L -rep $(REP_FLAG) -alu $(ALU_FLAG) -crm $(CRM_FLAG) -resdir "$(ROOT_DIR)/MS_MMI/source/resource/$(MMI_RES_DIR)"
endif
#ifeq ($(strip $(MULTI_THEME_SUPPORT)),TRUE)
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR).bin) $(call Path_process,$(BUILD_DIR)/img/)
#else
#	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_DEFAULT.bin)  $(call Path_process,$(BUILD_DIR)/img/$(MMI_RES_DIR).bin)
#endif
endif
	@-$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/*.txt) $(call Path_process,$(BUILD_DIR)/tmp/)
	@-$(PERL) make/perl_script/rm_dir.pl $(BUILD_DIR)/res ""



.PHONEY:builddir
builddir:
	@echo $(MMI_RES_DIR)

	@$(PERL) make/perl_script/mk_dir.pl "build" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)" ""
	@$(PERL) make/perl_script/rm_dir.pl res $(BUILD_DIR)/
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR).bin $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR)_win.bin $(BUILD_DIR)/img/
	@$(PERL) make/perl_script/rm_file.pl midi_soundbank_compact.c $(BUILD_DIR)/tmp/
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img" ""	
#	$(COPY_DIR)  $(call Path_process,MS_MMI/source/resource/$(MMI_RES_DIR)/*)  $(call Path_process,$(BUILD_DIR)/res/)
ifeq ($(strip $(MMI_RES_ORIENT)),LANDSCAPE)
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/FONT" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/RING" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/common" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/common/MMI_RES_DEFAULT" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/common/MMI_RES_DEFAULT/DATA" ""
	$(COPY_FILE)  $(call Path_process,MS_MMI/source/resource/Common/*.xls)  $(call Path_process,$(BUILD_DIR)/res/)
	$(COPY_FILE)  $(call Path_process,MS_MMI/source/resource/Common/FONT/*)  $(call Path_process,$(BUILD_DIR)/res/FONT/)
	$(COPY_FILE)  $(call Path_process,MS_MMI/source/resource/Common/RING/*)  $(call Path_process,$(BUILD_DIR)/res/RING/)
	$(COPY_FILE)  $(call Path_process,MS_MMI/source/resource/Common/DATA/*)  $(call Path_process,$(BUILD_DIR)/res/common/MMI_RES_DEFAULT/DATA/)
	$(COPY_DIR)  $(call Path_process,MS_MMI/source/resource/$(MMI_RES_DIR)/*)  $(call Path_process,$(BUILD_DIR)/res/)
	$(PERL) make/perl_script/ProcessResDir.pl $(BUILD_DIR)/res
endif
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/tmp" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(SOURCE_DIR)" $(BUILD_DIR)/res/

.PHONEY:macro_create
macro_create:
	@make PROJECT=$(PROJECT) -f $(call Path_process,make/pclint/pclint.mk) -k -r -R COMPONENT=app resdep

%_mdu_def.source :%_mdu_def.h
	@$(ECHO) Compiling resource source file $< ...
	@$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(BUILD_DIR)/tmp/app.macro -E $<  -o build/$(PROJECT)_builddir/res/$*/$*_mdu_def.h
	@$(ECHO) $*  build finished

%.project :%.h
	@$(ECHO) Compiling resource project file $< ...
	@$(PERL) make/perl_script/rm_dir.pl $*.h build/$(PROJECT)_builddir/res/
	@$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(BUILD_DIR)/tmp/app.macro -E $<  -o build/$(PROJECT)_builddir/res/$*.h
	@$(ECHO) $*  build finished
	
.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_dir.pl $(BUILD_DIR)/res ""
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR).bin $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR)_win.bin $(BUILD_DIR)/img/
