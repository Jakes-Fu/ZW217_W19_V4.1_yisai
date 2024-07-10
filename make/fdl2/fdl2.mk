include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/fdl2_C_MACRO.txt)
  $(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/fdl2_A_MACRO.txt)
endif

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))
SCF_FILE := $(shell $(PERL) ./make/perl_script/get_filename.pl fdl_bootloader\fdl2 $(strip $(PLATFORM))_$(PROJECT).scf  fdl2_$(strip $(PLATFORM)).scf)

ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MCFLAG_OPT   	= -D_LITTLE_ENDIAN
endif


MINCPATH   	= -Icommon/export/inc \
					 -IRTOS/export/inc \
					 -IMS_Customize/export/inc \
					 -Ichip_drv/export/inc \
           -Ifdl_bootloader/fdl2/inc \
           -IMS_Ref/export/inc
MINCPATH += -Ichip_drv/export/inc/outdated
MINCPATH += -Ifdl_bootloader/fdl2/inc/nandparttable



SRCPATH		=  fdl_bootloader/fdl2/src \
             MS_Customize/source/product/driver/nandflash


SRCPATH +=   fdl_bootloader/fdl2/src/nandparttable/$(strip $(PROD_CONFIG))

SOURCES	 =	fdl2_signature.c \
		nand_spec.c \
		nand_vendor_sw_select.c \
		nand_PartitionCfg.c   \
		Nand_PartTable_16k.c \
		Nand_PartTable_128k.c



ifeq ($(strip $(MAKESHELL)),CMD)
	LIBS        = lib/$(strip $(PRODUCT_BASELIB_DIR))/fdl2_nf.a(init.o)\
								lib/$(strip $(PRODUCT_BASELIB_DIR))/fdl2_nf.a
else
	LIBS        = lib/$(strip $(PRODUCT_BASELIB_DIR))/fdl2_nf.a\(init.o\)
								lib/$(strip $(PRODUCT_BASELIB_DIR))/fdl2_nf.a
endif

LIBSXSR         =	lib/$(strip $(PRODUCT_BASELIB_DIR))/xsr_layer_os_none.a		\
			lib/$(strip $(PRODUCT_BASELIB_DIR))/ftl_os_none.a		\
			lib/$(strip $(PRODUCT_BASELIB_DIR))/sci_ftl_layer_os_none.a

SCATFILENAME	= fdl_bootloader/fdl2/$(SCF_FILE)

ifeq ($(strip $(DEV_MANAGE_SUPPORT)),TRUE)
MCFLAG_OPT		+= -DDEV_MANAGE_SUPPORT
endif


OBJECTS1 	:= $(patsubst %.c, %.o, $(SOURCES))
OBJECTS   := $(patsubst %.s, %.o, $(OBJECTS1))

vpath
vpath %.o  $(BUILD_DIR)/obj/fdl2
vpath %.d  $(BUILD_DIR)/dep/fdl2
vpath %.h  $(subst \,/,$(MINCPATH))
vpath %.c  $(subst \,/,$(SRCPATH))

DEPPATH	= $(BUILD_DIR)/dep/fdl2


all: builddir $(OBJECTS)
ifeq ($(strip $(COMPILER)),	RVCT)
	@$(LINK) --info totals --info unused --entry 0x30000000 --scatter $(SCATFILENAME) --map --symbols --list fdl2.map --info sizes $(LIBSXSR) $(LIBS) \
	$(addprefix $(BUILD_DIR)/obj/fdl2/, $(OBJECTS)) --Output $(BUILD_DIR)/fdl_bootloader/fdl2.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/fdl_bootloader/fdl2.axf --output $(BUILD_DIR)/fdl_bootloader/fdl2.bin
	@-$(PERL) make/perl_script/rm_file.pl fdl2.bin $(BUILD_DIR)/img/
	@$(COPY_FILE)  $(call Path_process,$(BUILD_DIR)/fdl_bootloader/fdl2.bin)  $(call Path_process,$(BUILD_DIR)/img/fdl2.bin)
else
	@$(LINK) -info totals -info unused -entry 0x30000000 -scatter $(SCATFILENAME) -map -symbols -list fdl2.map -info sizes $(LIBSXSR) $(LIBS) \
	$(addprefix $(BUILD_DIR)/obj/fdl2/, $(OBJECTS)) -Output $(BUILD_DIR)/fdl_bootloader/fdl2.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/fdl_bootloader/fdl2.axf -output $(BUILD_DIR)/fdl_bootloader/fdl2.bin
	@-$(PERL) make/perl_script/rm_file.pl fdl2.bin $(BUILD_DIR)/img/
	@$(COPY_FILE)  $(call Path_process,$(BUILD_DIR)/fdl_bootloader/fdl2.bin)  $(call Path_process,$(BUILD_DIR)/img/fdl2.bin)
endif

ifneq ($(strip $(MAKE_VAR_DEPEND_D)),0)
-include $(OBJECTS:.o=.d)
endif

%.d %.o:%.c
	@$(ECHO) 
	@$(ECHO) Compiling Source File $<...
	$(CC) $(MCFLAG_OPT) $(CFLAGS) $(VIA) $(BUILD_DIR)/dep/fdl2_C_MACRO.txt -c $(MINCPATH) $(INCPATH) $<  $(MDFLAGS) $(DEPPATH)/$*.d.tmp -o $(BUILD_DIR)/obj/fdl2/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/fdl2/fdl2.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl $*.d.tmp $(DEPPATH)/

.PHONY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "fdl_bootloader log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "fdl2" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "fdl2" $(BUILD_DIR)/dep

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl fdl2.bin $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl fdl2.bin $(BUILD_DIR)/fdl_bootloader/
	@-$(PERL) make/perl_script/rm_dir.pl fdl2 $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl fdl2 $(BUILD_DIR)/dep
