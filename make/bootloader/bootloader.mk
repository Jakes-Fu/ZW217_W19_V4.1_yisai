
include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/bootloader_C_MACRO.txt)
  $(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/bootloader_A_MACRO.txt)
endif

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))

MCFLAG_OPT      = -D__RUN_IN_SDRAM -D_NFTL_BOOT_
ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)

MCFLAG_OPT      += -D_LITTLE_ENDIAN
  ifeq ($(strip $(COMPILER)), RVCT)
    MASMFLAG_OPT    += --PD "_LITTLE_ENDIAN SETL {TRUE}"
  else
MASMFLAG_OPT    += -PD "_LITTLE_ENDIAN SETL {TRUE}"
  endif
endif

	
MINCPATH =  	-Icommon/export/inc                \
		-IMS_Customize/export/inc          \
		-Ichip_drv/export/inc              \
		-Ifdl_bootloader/bootloader/inc    \
		-IRTOS/export/inc                  \
		-Ifdl_bootloader/bootloader/src   \
		-IMS_Customize/source/product/config/$(strip $(PROD_CONFIG)) \
		-Ichip_drv/export/inc/outdated \
		-IMS_Ref/export/inc \
		-Ifdl_bootloader/common/inc \
 		-Ifdl_bootloader/chip_drv/inc \
 		-Ichip_drv/chip_plf/common/inc \
 		-Ifdl_bootloader\bootloader\src \
		-IMS_Ref/export/inc \
		-Ichip_drv/export/inc/outdated

MSRCPATH =  	fdl_bootloader/bootloader/src \
 		fdl_bootloader/sio_drv \
 		fdl_bootloader/bootloader/dma_drv \
 		fdl_bootloader/chip_drv/src \
 		fdl_bootloader/common/src \
 		MS_Customize/source/product/driver/sdram \
		fdl_bootloader/chip_drv/src/mmu \
		MS_Customize/source/product/driver/nandflash
		
LIBS1	=	lib/$(strip $(PRODUCT_BASELIB_DIR))/xsr_layer_os_none.a \
		lib/$(strip $(PRODUCT_BASELIB_DIR))/ftl_os_none.a \
		lib/$(strip $(PRODUCT_BASELIB_DIR))/sci_ftl_layer_os_none.a
SOURCES_COMMON = chip_cfg.c  dummy.c mmu.c mmu_asm.s fdl_stdio.c nand_controller.c nand_spec.c
SOURCES0 = boot0_main.c boot0_entry.s  sdram_init.c sdram_prod.c
SOURCES1 = boot1_main.c boot1_entry.s boot1_copy_kernel_img.c nand_vendor_sw_select.c

# **************************** PRODUCT ************************

OBJECTS_COMMON_SUB	:= $(patsubst %.c, %.o, $(SOURCES_COMMON))
OBJECTS_COMMON	 	:= $(patsubst %.s, %.o, $(OBJECTS_COMMON_SUB))

OBJECTS0_SUB 	:= $(patsubst %.c, %.o, $(SOURCES0))
OBJECTS0 	:= $(patsubst %.s, %.o, $(OBJECTS0_SUB))

OBJECTS1_SUB 	:= $(patsubst %.c, %.o, $(SOURCES1))
OBJECTS1 	:= $(patsubst %.s, %.o, $(OBJECTS1_SUB))

vpath
vpath %.o  $(BUILD_DIR)/obj/bootloader
vpath %.d  $(BUILD_DIR)/dep/bootloader
vpath %.c        $(subst \,/,$(MSRCPATH))
vpath %.s        $(subst \,/,$(MSRCPATH))

DEPPATH	=  $(BUILD_DIR)/dep/bootloader

all: builddir $(OBJECTS_COMMON) $(OBJECTS0) $(OBJECTS1)
ifeq ($(strip $(COMPILER)),	RVCT)
	@$(LINK) --info totals --entry 0x40000000 --map --symbols --scatter fdl_bootloader/bootloader/scatter0.scf --list $(BUILD_DIR)/fdl_bootloader/boot0.map --info sizes \
	$(addprefix $(BUILD_DIR)/obj/bootloader/, $(OBJECTS0)) $(addprefix $(BUILD_DIR)/obj/bootloader/, $(OBJECTS_COMMON)) --Output $(BUILD_DIR)/fdl_bootloader/boot0.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/fdl_bootloader/boot0.axf --output $(BUILD_DIR)/fdl_bootloader/boot0.bin

	@$(LINK) --info totals --entry 0x1000000 --map --symbols --scatter fdl_bootloader/bootloader/scatter1.scf --list $(BUILD_DIR)/fdl_bootloader/boot1.map --info sizes $(LIBS1) \
	$(addprefix $(BUILD_DIR)/obj/bootloader/, $(OBJECTS1)) $(addprefix $(BUILD_DIR)/obj/bootloader/, $(OBJECTS_COMMON)) --Output $(BUILD_DIR)/fdl_bootloader/boot1.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/fdl_bootloader/boot1.axf --output $(BUILD_DIR)/fdl_bootloader/boot1.bin

else
	$(LINK) -info totals -entry 0x40000000 -map -symbols -scatter fdl_bootloader/bootloader/scatter0.scf -list $(BUILD_DIR)/fdl_bootloader/boot0.map -info sizes  \
	$(addprefix $(BUILD_DIR)/obj/bootloader/, $(OBJECTS0)) $(addprefix $(BUILD_DIR)/obj/bootloader/, $(OBJECTS_COMMON)) -Output $(BUILD_DIR)/fdl_bootloader/boot0.axf
	$(FROMELF) -c -bin $(BUILD_DIR)/fdl_bootloader/boot0.axf -output $(BUILD_DIR)/fdl_bootloader/boot0.bin

	$(LINK) -info totals -entry 0x1000000 -map -symbols -scatter fdl_bootloader/bootloader/scatter1.scf -list $(BUILD_DIR)/fdl_bootloader/boot1.map -info sizes $(LIBS1) \
	$(addprefix $(BUILD_DIR)/obj/bootloader/, $(OBJECTS1)) $(addprefix $(BUILD_DIR)/obj/bootloader/, $(OBJECTS_COMMON)) -Output $(BUILD_DIR)/fdl_bootloader/boot1.axf
	$(FROMELF) -c -bin $(BUILD_DIR)/fdl_bootloader/boot1.axf -output $(BUILD_DIR)/fdl_bootloader/boot1.bin

endif	
	@-$(PERL) make/perl_script/rm_file.pl bootloader.bin $(BUILD_DIR)/img/
	make\make_cmd\makeBoot.exe $(BUILD_DIR)/fdl_bootloader/boot0.bin $(BUILD_DIR)/fdl_bootloader/boot1.bin $(BUILD_DIR)/fdl_bootloader/bootloader.bin
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/fdl_bootloader/bootloader.bin)  $(call Path_process,$(BUILD_DIR)/img/bootloader.bin)

%.o :%.s
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(ASM) $(MASMFLAG_OPT) $(AFLAGS) $(VIA) $(BUILD_DIR)/dep/bootloader_A_MACRO.txt  $(INCPATH) $< $(MDFLAGS) $(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/bootloader/$*.o

%.o :%.c
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(CC) $(MCFLAG_OPT) $(CFLAGS) -g $(VIA) $(BUILD_DIR)/dep/bootloader_C_MACRO.txt  -c $(MINCPATH) $(INCPATH) $< $(MDFLAGS) $(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/bootloader/$*.o

.PHONY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "fdl_bootloader log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "bootloader" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "bootloader" $(BUILD_DIR)/dep

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl bootloader.bin $(BUILD_DIR)/img/ 
	@-$(PERL) make/perl_script/rm_file.pl boot0.bin $(BUILD_DIR)/fdl_bootloader/
	@-$(PERL) make/perl_script/rm_file.pl boot1.bin $(BUILD_DIR)/fdl_bootloader/
	@-$(PERL) make/perl_script/rm_file.pl bootloader.bin $(BUILD_DIR)/fdl_bootloader/
	@-$(PERL) make/perl_script/rm_dir.pl bootloader $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl bootloader $(BUILD_DIR)/dep
