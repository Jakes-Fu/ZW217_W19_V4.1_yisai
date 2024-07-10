VDIR := ARMRomCode/seine/arm/drv_sc6600v
include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

.PHONY:all
all:
	$(MAKE) -s -f $(VDIR)\makefile TOPDIR=$(VDIR) PERLDIR=$(MAKE_HOME) new
	echo == copy varm fireware header files ==
	 @-$(PERL) make/perl_script/mk_dir.pl $(PROJECT) $(call Path_process,CMMB\source\mtv_adapter\inc\)
	-@$(DEL_FILE) $(call Path_process,CMMB\source\mtv_adapter\inc\$(PROJECT)\arm_common_code.h)
	-@$(DEL_FILE) $(call Path_process,CMMB\source\mtv_adapter\inc\$(PROJECT)\code_in_sdram.h)
	-@$(DEL_FILE) $(call Path_process,CMMB\source\mtv_adapter\inc\$(PROJECT)\video_decoder_avs.h)
	-@$(DEL_FILE) $(call Path_process,CMMB\source\mtv_adapter\inc\$(PROJECT)\video_decoder_h264.h)

	$(COPY_FILE) $(call Path_process,$(VDIR)\$(PROJECT)\arm_common_code_zip.h) CMMB\source\mtv_adapter\inc\$(PROJECT)\arm_common_code.h
	$(COPY_FILE) $(call Path_process,$(VDIR)\$(PROJECT)\code_in_sdram_zip.h) CMMB\source\mtv_adapter\inc\$(PROJECT)\code_in_sdram.h
	$(COPY_FILE) $(call Path_process,$(VDIR)\$(PROJECT)\video_decoder_avs_zip.h) CMMB\source\mtv_adapter\inc\$(PROJECT)\video_decoder_avs.h
	$(COPY_FILE) $(call Path_process,$(VDIR)\$(PROJECT)\video_decoder_h264_zip.h) CMMB\source\mtv_adapter\inc\$(PROJECT)\video_decoder_h264.h

	@-$(PERL) make/perl_script/rm_dir.pl fw $(BUILD_DIR)/img/
	@-@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)" ""
	@-@$(PERL) make/perl_script/mk_dir.pl "./lib/$(PROJECT)" ""
	@-@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img" ""
	@-@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img/fw" ""
	-$(COPY_DIR)  $(call Path_process,ARMRomCode/seine/arm/drv_sc6600v/build/$(PROJECT)/img/*)  $(call Path_process,$(BUILD_DIR)/img/fw)

.PHONEY:clean
clean:
	-@$(DEL_FILE) $(call Path_process,CMMB\source\mtv_adapter\inc\$(PROJECT)\arm_common_code.h)
	-@$(DEL_FILE) $(call Path_process,CMMB\source\mtv_adapter\inc\$(PROJECT)\code_in_sdram.h)
	-@$(DEL_FILE) $(call Path_process,CMMB\source\mtv_adapter\inc\$(PROJECT)\video_decoder_avs.h)
	-@$(DEL_FILE) $(call Path_process,CMMB\source\mtv_adapter\inc\$(PROJECT)\video_decoder_h264.h)