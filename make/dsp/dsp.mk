include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifeq ($(strip $(PLATFORM)), SC6531EFM)
GSM_DSP_NAME = dsp_dl_nemoG_3593_FDT_zip.bin
GSM_DSP_PATH = DSP_BIN/BASE/
DSP_Bin = DSP.bin

.PHONEY:all
all:
ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
	$(COPY_FILE) $(call Path_process,$(strip $(GSM_DSP_PATH))$(strip $(GSM_DSP_NAME)))  build\$(PROJECT)_builddir\img\$(DSP_Bin)

	$(PERL) make/perl_script/rm_file.pl DSP.bin lib/$(strip $(PRODUCT_BASELIB_DIR))/
	$(COPY_FILE) build\$(PROJECT)_builddir\img\$(DSP_Bin)  lib\$(strip $(PRODUCT_BASELIB_DIR))\ 

	$(PERL) make/perl_script/mk_dir.pl "GSM_DSP" "$(BUILD_DIR)/img"
	$(PERL) make/perl_script/rm_file.pl $(strip $(GSM_DSP_NAME)) $(BUILD_DIR)/img/GSM_DSP/
	$(COPY_FILE) $(call Path_process,$(strip $(GSM_DSP_PATH))$(strip $(GSM_DSP_NAME)))  $(call Path_process,$(BUILD_DIR)/img/GSM_DSP/)
else
	$(PERL) make/perl_script/rm_file.pl $(DSP_Bin) build/$(PROJECT)_builddir/img
	$(COPY_FILE)  lib\$(strip $(PRODUCT_BASELIB_DIR))\$(DSP_Bin)  build\$(PROJECT)_builddir\img
endif


.PHONY:clean
clean:
ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
	$(PERL) make/perl_script/rm_file.pl $(DSP_Bin) lib/$(strip $(PRODUCT_BASELIB_DIR))/
	$(PERL) make/perl_script/rm_file.pl $(DSP_Bin) build/$(PROJECT)_builddir/img/

	$(PERL) make/perl_script/rm_file.pl $(strip $(GSM_DSP_NAME)) $(BUILD_DIR)/img/GSM_DSP/
else
	$(PERL) make/perl_script/rm_file.pl $(DSP_Bin) build/$(PROJECT)_builddir/img/
endif

else ##ifeq ($(strip $(PLATFORM)), SC6531EFM)

ifeq ($(strip $(CHIP_VER)), 6810)
GSM_DSP_NAME = SC6810_QS3200_GSM_DSP.bin
GSM_DSP_PATH = DSP_CEVAX/DSP_8800H/GSM/
DSP_Bin = DSP_6810.bin
endif


.PHONEY:all
all:
ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
	make\make_cmd\BinGen.exe -.\$(strip $(GSM_DSP_PATH))$(strip $(GSM_DSP_NAME))  -DSP.SC6810 - - -.\build\$(PROJECT)_builddir\img\DSP_BE.bin
ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)		
	$(PERL) make/perl_script/makeledspcode.pl build/$(PROJECT)_builddir/img/DSP_BE.bin  build/$(PROJECT)_builddir/img/DSP.bin
ifeq ($(strip $(CHIP_VER)), 6810)
	$(COPY_FILE) build\$(PROJECT)_builddir\img\DSP.bin  build\$(PROJECT)_builddir\img\$(DSP_Bin)
	$(DEL_FILE) build\$(PROJECT)_builddir\img\DSP.bin			
endif
else
ifeq ($(strip $(CHIP_VER)), 6810)
	$(COPY_FILE) build\$(PROJECT)_builddir\img\DSP_BE.bin  build\$(PROJECT)_builddir\img\$(DSP_Bin)
endif	
endif	
	$(DEL_FILE) build\$(PROJECT)_builddir\img\DSP_BE.bin	
ifeq ($(strip $(CHIP_VER)), 6810)
	$(PERL) make/perl_script/rm_file.pl DSP.bin lib/$(strip $(PRODUCT_BASELIB_DIR))/
	$(COPY_FILE) build\$(PROJECT)_builddir\img\$(DSP_Bin)  lib\$(strip $(PRODUCT_BASELIB_DIR))\ 
endif
	$(PERL) make/perl_script/mk_dir.pl "GSM_DSP" "$(BUILD_DIR)/img"
	$(PERL) make/perl_script/rm_file.pl $(strip $(GSM_DSP_NAME)) $(BUILD_DIR)/img/GSM_DSP/
	$(COPY_FILE) $(call Path_process,$(strip $(GSM_DSP_PATH))$(strip $(GSM_DSP_NAME)))  $(call Path_process,$(BUILD_DIR)/img/GSM_DSP/)
else
ifeq ($(strip $(CHIP_VER)), 6810)
	$(PERL) make/perl_script/rm_file.pl $(DSP_Bin) build/$(PROJECT)_builddir/img/	
	$(COPY_FILE)  lib\$(strip $(PRODUCT_BASELIB_DIR))\$(DSP_Bin)  build\$(PROJECT)_builddir\img\ 
endif
endif


.PHONY:clean
clean:
ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
	
ifeq ($(strip $(CHIP_VER)), 6810)
	$(PERL) make/perl_script/rm_file.pl $(DSP_Bin) lib/$(strip $(PRODUCT_BASELIB_DIR))/
	$(PERL) make/perl_script/rm_file.pl $(DSP_Bin) build/$(PROJECT)_builddir/img/
endif
	$(PERL) make/perl_script/rm_file.pl $(strip $(GSM_DSP_NAME)) $(BUILD_DIR)/img/GSM_DSP/
else
	
ifeq ($(strip $(CHIP_VER)), 6810)
	$(PERL) make/perl_script/rm_file.pl $(DSP_Bin) build/$(PROJECT)_builddir/img/
endif
	
endif

endif

