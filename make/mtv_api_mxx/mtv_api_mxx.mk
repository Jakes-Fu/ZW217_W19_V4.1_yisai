include Makefile.rules
include project_$(PROJECT).mk

PLAT_H_DIR  = CMMB\source\include\mxx
DEF_H_DIR   = CMMB\source\os_al\include\KD
VARM_FW_DIR = ARMRomCode\seine\arm\drv_sc6600v
PACKAGE_DIR = CMMB/build
PACKAGE_DIR_DOS = $(subst /,\,$(PACKAGE_DIR))
BUILD_DIR_DOS = $(subst /,\,$(BUILD_DIR))

CFLAG_OPT    := -littleend -Ec $(CPU) ARM7EJ-S -O1 -zo -fa 
CFLAG_OPT    += -DLCD_CONNECTED_TO_V -DDTL_ERROR_HIDE -DUSING_STATIC_THREAD -DHOST_IS_LITTLEEND
CFLAG_OPT    += -DSPRD_USER_CONFIG

MINCPATH1 = CMMB/source/include \
           CMMB/export/inc CMMB/source/dtl/inc CMMB/source/mtv_api/inc \
           CMMB/source/proxycall/inc CMMB/source/tvfirmware/inc        \
           CMMB/source/mtv_adapter/inc \
           $(COMMON_INCPATH)
MINCPATH = $(addprefix -I, $(MINCPATH1))

VPATH    = CMMB/source/dtl/src
VPATH   += CMMB/source/mtv_api/src
VPATH   += CMMB/source/proxycall/src
VPATH   += CMMB/source/tvfirmware/src
VPATH   += RTOS/source/src/c

vpath
vpath %.o  $(BUILD_DIR)/obj/mtv_api_mxx/

SOURCES  = dtl.c dtl_recovery.c dtl_cmdq.c lcd_tv.c mtv_boot.c scatte_pilot.c \
           seine_sdram_drv.c lcmif_drv.c lcdc_regop.c mtv_api.c               \
           proxycallpeer.c peercustomize.c queue.c mtv_imp_peer.c             \
           mtv_proxy_peer.c dac_peer.c sfs_peer.c mcex_peer.c bitmapfield.c   \
           aac_decoder_code.c dra_decoder_code.c aac_table.c dra_table.c      \
           dsp_ack_track_code.c dsp_common_code_cmmb.c server.c
				 
OBJECTS  = $(patsubst %.c, %.o, $(SOURCES))

OBJECTS_PATHNAME = $(addprefix $(BUILD_DIR)/obj/mtv_api_mxx/, $(OBJECTS))


TARGET_DIR = $(BUILD_DIR)/lib
TARGET_DIR_DOS = $(subst /,\,$(TARGET_DIR))

all: usr_pkg
	@$(ECHO)
	@$(ECHO) 
	@$(ECHO) NOTE: mxx mtv_api user package has been built to:
	@$(ECHO)         $(PACKAGE_DIR)/mtv_api_xx
	@$(ECHO)

.PHONY:mtv_api_lib
mtv_api_lib: builddir platform_fix $(OBJECTS) platform_restore
	$(ECHO) $(CFLAG_OPT)
	$(AR) -create ./$(TARGET_DIR)/mtv_api_mxx.a $(OBJECTS_PATHNAME)
	
.PHONY:platform_fix
platform_fix:
	$(DEL_FILE) CMMB\export\inc\kdplatform.h
	$(COPY_FILE) $(PLAT_H_DIR)\kdplatform.h CMMB\export\inc

.PHONY:platform_restore
platform_restore:
	$(DEL_FILE) CMMB\export\inc\kdplatform.h
	$(COPY_FILE) $(DEF_H_DIR)\kdplatform.h CMMB\export\inc

.PHONY:builddir
builddir:
	@$(ECHO) - build dir...
	@$(PERL) make/perl_script/mk_dir.pl "log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "mtv_api_mxx" $(PACKAGE_DIR)
	
.PHONY:clean
clean:
	$(DEL_FILE) $(TARGET_DIR)/mtv_api_mxx.a

.PHONY:usr_pkg
usr_pkg: mtv_api_lib
	@$(ECHO) - delete previous build...
	@rmdir /S /Q $(PACKAGE_DIR_DOS)\mtv_api_mxx
	@$(ECHO) - create directories...
	@$(PERL) make/perl_script/mk_dir.pl "mtv_api_mxx" $(PACKAGE_DIR_DOS)
	@$(PERL) make/perl_script/mk_dir.pl "C H LIBS fw_pkg" $(PACKAGE_DIR_DOS)/mtv_api_mxx
	@$(PERL) make/perl_script/mk_dir.pl "src build make inc lib" $(PACKAGE_DIR_DOS)/mtv_api_mxx/fw_pkg
	@$(PERL) make/perl_script/mk_dir.pl "customize" $(PACKAGE_DIR_DOS)/mtv_api_mxx/fw_pkg/src
	@$(PERL) make/perl_script/mk_dir.pl "customize" $(PACKAGE_DIR_DOS)/mtv_api_mxx/fw_pkg/make
	@$(PERL) make/perl_script/mk_dir.pl "rtos" $(PACKAGE_DIR_DOS)/mtv_api_mxx/fw_pkg/inc/
	@$(ECHO) - copy files...
	@$(COPY_FILE) CMMB\export\inc\kd.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) $(PLAT_H_DIR)\kdplatform.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\source\dtl\inc\lcm_user.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\source\mtv_api\inc\dac.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\source\tvfirmware\inc\tv_firmware.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\source\proxycall\inc\mtv_fs.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\source\mtv_adapter\inc\lcd_tv.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\source\mtv_adapter\inc\icc_mcex.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\export\inc\mtv_api.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\export\inc\dtl_export.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\export\inc\mtv_common.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\export\inc\mtv_cfg.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) CMMB\export\inc\mtv_ctrl.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	@$(COPY_FILE) $(TARGET_DIR_DOS)\mtv_api_mxx.a $(PACKAGE_DIR_DOS)\mtv_api_mxx\LIBS\mtv_api.a
	@$(COPY_FILE) $(VARM_FW_DIR)\inc\sci_types.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\inc
	@$(COPY_FILE) $(VARM_FW_DIR)\inc\rtos_cfg.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\inc
	@$(COPY_FILE) $(VARM_FW_DIR)\inc\i2c_drv.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\inc
	@$(COPY_FILE) $(VARM_FW_DIR)\inc\arm_reg.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\inc
	@$(COPY_FILE) $(VARM_FW_DIR)\inc\audio_dac_core.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\inc
	@$(COPY_FILE) $(VARM_FW_DIR)\inc\rtos\os_api.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\inc\rtos
	@$(COPY_FILE) $(VARM_FW_DIR)\lib\$(PROJECT)\lib.a $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\lib
	@$(COPY_FILE) $(VARM_FW_DIR)\make\customize\makefile $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\make\customize
	@$(COPY_DIR)  /I $(VARM_FW_DIR)\src\customize $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\src\customize
	@$(COPY_FILE) $(VARM_FW_DIR)\sc6600v_rom_sym.o $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_FILE) $(VARM_FW_DIR)\sc6600v.scf $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_FILE) $(VARM_FW_DIR)\makefile $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_FILE) $(VARM_FW_DIR)\moduledep.mk $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_FILE) $(VARM_FW_DIR)\modulemake.mk $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_FILE) $(VARM_FW_DIR)\gzip.bat $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_FILE) $(VARM_FW_DIR)\firmware.pl $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_FILE) $(VARM_FW_DIR)\bin2txt.exe $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_FILE) $(VARM_FW_DIR)\gzip.exe $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_FILE) Makefile.rules $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg
	@$(COPY_DIR)  /I make\make_cmd $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\make\make_cmd
	@$(COPY_DIR)  /I make\perl_script $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\make\perl_script
	@echo make\make_cmd\make -r -R TOPDIR=. PERLDIR=make\make_cmd image > $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\make.bat
	@echo copy *.h ..\H	 >> $(PACKAGE_DIR_DOS)\mtv_api_mxx\fw_pkg\make.bat
	@$(COPY_FILE) $(VARM_FW_DIR)\$(PROJECT)\*.h $(PACKAGE_DIR_DOS)\mtv_api_mxx\H
	
%.o:%.c
	$(CC) $(CFLAG_OPT) -c $(MINCPATH) $(INCPATH) $< -o $(BUILD_DIR)/obj/mtv_api_mxx/$@
