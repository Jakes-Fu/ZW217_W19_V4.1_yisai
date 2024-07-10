include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifeq ($(strip $(CHIP_VER)),8800S2A)
GSM_DSP_NAME = 8800H_Quorum3200_S2A_dsp.bin
endif
ifeq ($(strip $(CHIP_VER)),8800S3B)
GSM_DSP_NAME = 8800H_Quorum3200_S3B_dsp.bin
endif
ifeq ($(strip $(CHIP_VER)),8800H5)
GSM_DSP_NAME = 8800H_Quorum3200_S4_dsp.bin
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
ifeq ($(strip $(MULTI_SIM_SYS)), DUAL)
GSM_DSP_NAME = SC8800G_QS3200_GSM_DSP.bin
else
GSM_DSP_NAME = SC8800G_QS3200_GSM_DSP.bin
endif  
TD_DSP_NAME = SC8800G2_QS3200_TD_DSP.bin
DMDSP_Bin = DSP_DM_G2.bin
endif

ifeq ($(strip $(PLATFORM)), SC8800H)
TD_DSP_NAME = Dsp_qs3200_B0.bin
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
TD_DSP_PATH = 8850_TDBASE/8800G/Code/PRJ/
ifeq ($(strip $(MULTI_SIM_SYS)), DUAL)
GSM_DSP_PATH = DSP_CEVAX/DSP_8800H/GSM/
else
GSM_DSP_PATH = DSP_CEVAX/DSP_8800H/GSM/
endif
endif
ifeq ($(strip $(PLATFORM)), SC8800H)
TD_DSP_PATH = 8850_TDBASE/SW/Code/TDL1_V1.1/PRJ/
GSM_DSP_PATH = DSP_CEVAX/DSP_8800H/GSM/
endif


.PHONEY:all
all:
ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
	make\make_cmd\BinGen.exe -.\$(strip $(GSM_DSP_PATH))$(strip $(GSM_DSP_NAME))  -DSP.SC8800S_GSM -.\$(strip $(TD_DSP_PATH))$(strip $(TD_DSP_NAME)) -DSP.SC8800S_TD -.\build\$(PROJECT)_builddir\img\DSP_DM_BE.bin
ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)		
	$(PERL) make/perl_script/makeledspcode.pl build/$(PROJECT)_builddir/img/DSP_DM_BE.bin  build/$(PROJECT)_builddir/img/DSP_DM.bin
ifeq ($(strip $(PLATFORM)), SC8800G)
	$(COPY_FILE) build\$(PROJECT)_builddir\img\DSP_DM.bin  build\$(PROJECT)_builddir\img\$(DMDSP_Bin)
	$(DEL_FILE) build\$(PROJECT)_builddir\img\DSP_DM.bin			
endif
else
ifeq ($(strip $(PLATFORM)), SC8800G)
	$(COPY_FILE) build\$(PROJECT)_builddir\img\DSP_DM_BE.bin  build\$(PROJECT)_builddir\img\$(DMDSP_Bin)
else
	$(COPY_FILE) build\$(PROJECT)_builddir\img\DSP_DM_BE.bin  build\$(PROJECT)_builddir\img\DSP_DM.bin
endif	
endif	
	$(DEL_FILE) build\$(PROJECT)_builddir\img\DSP_DM_BE.bin	
ifeq ($(strip $(PLATFORM)), SC8800G)
	$(PERL) make/perl_script/rm_file.pl DSP_DM.bin lib/$(strip $(PRODUCT_BASELIB_DIR))/
	$(COPY_FILE) build\$(PROJECT)_builddir\img\$(DMDSP_Bin)  lib\$(strip $(PRODUCT_BASELIB_DIR))\ 
else
	$(PERL) make/perl_script/rm_file.pl DSP_DM.bin lib/$(strip $(PRODUCT_BASELIB_DIR))/
	$(COPY_FILE) build\$(PROJECT)_builddir\img\DSP_DM.bin  lib\$(strip $(PRODUCT_BASELIB_DIR))\ 
endif
	$(PERL) make/perl_script/mk_dir.pl "TD_DSP" "$(BUILD_DIR)/img"
	$(PERL) make/perl_script/mk_dir.pl "GSM_DSP" "$(BUILD_DIR)/img"
	$(PERL) make/perl_script/rm_file.pl $(strip $(GSM_DSP_NAME)) $(BUILD_DIR)/img/GSM_DSP/
	$(PERL) make/perl_script/rm_file.pl $(strip $(TD_DSP_NAME)) $(BUILD_DIR)/img/TD_DSP/
	$(COPY_FILE) $(call Path_process,$(strip $(GSM_DSP_PATH))$(strip $(GSM_DSP_NAME)))  $(call Path_process,$(BUILD_DIR)/img/GSM_DSP/)
	$(COPY_FILE) $(call Path_process,$(strip $(TD_DSP_PATH))$(strip $(TD_DSP_NAME)))  $(call Path_process,$(BUILD_DIR)/img/TD_DSP/)
else
ifeq ($(strip $(PLATFORM)), SC8800G)
	$(PERL) make/perl_script/rm_file.pl $(DMDSP_Bin) build/$(PROJECT)_builddir/img/	
	$(COPY_FILE)  lib\$(strip $(PRODUCT_BASELIB_DIR))\$(DMDSP_Bin)  build\$(PROJECT)_builddir\img\ 
else
	$(COPY_FILE)  lib\$(strip $(PRODUCT_BASELIB_DIR))\DSP_DM.bin  build\$(PROJECT)_builddir\img\ 
endif
endif


#	SET CONFIG=C:\yihui.hu_sp8801h_view\MS_Code\8850_TDBASE\SW\Code\TDL1_V1.1\PRJ;CD %CONFIG%;MD Debug;nmake /a /f TDL1_Debug.bld clean;nmake /a /f TDL1_Debug.bld all;copy debug\tdl10.out tdl10.out;copy debug\tdl11.out tdl11.out;DMHex2bin.exe tdl10.out tdl11.out
.PHONY:clean
clean:
ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
	
ifeq ($(strip $(PLATFORM)), SC8800G)
	$(PERL) make/perl_script/rm_file.pl $(DMDSP_Bin) lib/$(strip $(PRODUCT_BASELIB_DIR))/
	$(PERL) make/perl_script/rm_file.pl $(DMDSP_Bin) build/$(PROJECT)_builddir/img/
else
	$(PERL) make/perl_script/rm_file.pl DSP_DM.bin lib/$(strip $(PRODUCT_BASELIB_DIR))/
endif
	$(PERL) make/perl_script/rm_file.pl $(strip $(GSM_DSP_NAME)) $(BUILD_DIR)/img/GSM_DSP/
	$(PERL) make/perl_script/rm_file.pl $(strip $(TD_DSP_NAME)) $(BUILD_DIR)/img/TD_DSP/
else
	
ifeq ($(strip $(PLATFORM)), SC8800G)
	$(PERL) make/perl_script/rm_file.pl $(DMDSP_Bin) build/$(PROJECT)_builddir/img/
else
	$(PERL) make/perl_script/rm_file.pl DSP_DM.bin build/$(PROJECT)_builddir/img/ 
endif
	
endif

