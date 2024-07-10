include Makefile.rules

.PHONEY:all
all:
	@$(PERL) make/perl_script/rm_file.pl romdisk.bin $(BUILD_DIR)/img/ 
	@$(COPY_FILE) MS_Ref\source\ucom\src\romdisk.bin $(call Path_process,$(BUILD_DIR)/img/)
	
	