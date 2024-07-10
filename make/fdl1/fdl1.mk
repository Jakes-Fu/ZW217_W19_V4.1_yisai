include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/fdl1_C_MACRO.txt)
  $(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/fdl1_A_MACRO.txt)
endif

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))
SCF_FILE := $(shell $(PERL) ./make/perl_script/get_filename.pl fdl_bootloader\fdl1 $(strip $(PLATFORM))_$(PROJECT).scf  fdl1_$(strip $(PLATFORM)).scf)

# **************************** COMMON *************************\

ifeq ($(strip $(CHIP_ENDIAN_DEFAULT)),LITTLE)
    ifeq ($(strip $(CHIP_ENDIAN)),BIG)
        MCFLAG_OPT 	  = $(LITTLE_ENDIAN_OPT) -D_LITTLE_ENDIAN -DFDL_ENDIAN_SWITCH
        MASMFLAG_OPT  = $(LITTLE_ENDIAN_OPT)    
        CFLAGS        = $(CPU) ARM926EJ-S -O2 $(APCS) /interwork/ -fy -D_RTOS -D_DEBUG -Ec -W
        AFLAGS        = $(CPU) ARM926EJ-S $(KEEP) $(APCS) /shlclient/interwork/
    endif
else
    ifeq ($(strip $(SC8800S_LITTLE_ENDIAN)),TRUE)
        MCFLAG_OPT    = $(BIG_ENDIAN_OPT)
        MASMFLAG_OPT  = $(BIG_ENDIAN_OPT)
    endif
endif

ifeq ($(strip $(STACK_CHECK_SUPPORT)),TRUE)
    CFLAGS       := $(subst $(SWST_OPT),,$(CFLAGS))
    AFLAGS       := $(subst $(SWST_OPT),,$(AFLAGS))
endif

MINCPATH   		= -Icommon/export/inc \
								-IRTOS/export/inc \
								-IMS_Customize/export/inc \
								-Ichip_drv/export/inc \
								-IThird-party/XSR_STLMP/include \
								-Ifdl_bootloader/fdl1/inc \
								-IMS_Customize/source/product/config/$(strip $(PROD_CONFIG))
MINCPATH += -Ichip_drv/export/inc/outdated

VPATH 			+=  fdl_bootloader/fdl1/src \
								fdl_bootloader/chip_drv/src/mmu\
								MS_Customize/source/product/config/$(strip $(PROD_CONFIG)) \
								MS_Customize/source/product/driver/sdram   

vpath
vpath %.o  $(BUILD_DIR)/obj/fdl1

SOURCES			= 	sdram_prod.c \
                                mmu.c        \
				mmu_asm.s			 

					
# **************************** PLATFORM ************************

SCATFILENAME		= fdl_bootloader/fdl1/$(SCF_FILE)		           	


TARGET      = $(BUILD_DIR)/lib/fdl1.a

ifeq ($(strip $(MAKESHELL)),CMD)
	LIBS        = lib/$(strip $(PRODUCT_BASELIB_DIR))/fdl1_nf.a(init.o)\
                  lib/$(strip $(PRODUCT_BASELIB_DIR))/fdl1_nf.a
else
	LIBS        = lib/$(strip $(PRODUCT_BASELIB_DIR))/fdl1_nf.a\(init.o\)\
                  lib/$(strip $(PRODUCT_BASELIB_DIR))/fdl1_nf.a
endif

OBJECTS1 	:= $(patsubst %.c, %.o, $(SOURCES))
OBJECTS   := $(patsubst %.s, %.o, $(OBJECTS1))


all: builddir $(OBJECTS)
ifeq ($(strip $(COMPILER)),	RVCT)
	$(LINK) --info totals --info unused --entry 0x40000000 --scatter $(SCATFILENAME) --map --symbols --list fdl1.map --info sizes $(LIBS) \
	$(addprefix $(BUILD_DIR)/obj/fdl1/, $(OBJECTS)) --Output $(BUILD_DIR)/fdl_bootloader/fdl1.axf
	$(FROMELF) -c --bin $(BUILD_DIR)/fdl_bootloader/fdl1.axf --output $(BUILD_DIR)/fdl_bootloader/fdl1.bin
else
	$(LINK) -info totals -info unused -entry 0x40000000 -scatter $(SCATFILENAME) -map -symbols -list fdl1.map -info sizes $(LIBS) \
	$(addprefix $(BUILD_DIR)/obj/fdl1/, $(OBJECTS)) -Output $(BUILD_DIR)/fdl_bootloader/fdl1.axf
	$(FROMELF) -c -bin $(BUILD_DIR)/fdl_bootloader/fdl1.axf -output $(BUILD_DIR)/fdl_bootloader/fdl1.bin
endif	
	-$(PERL) make/perl_script/rm_file.pl fdl1.bin $(BUILD_DIR)/img/
	-$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/fdl_bootloader/fdl1.bin) $(call Path_process,$(BUILD_DIR)/img/fdl1.bin)
%.o:%.s
	$(ASM) $(MASMFLAG_OPT) $(AFLAGS) $(VIA) $(BUILD_DIR)/dep/fdl1_A_MACRO.txt $(INCPATH) $< -o $(BUILD_DIR)/obj/fdl1/$@

%.o:%.c
	$(CC) $(MCFLAG_OPT) $(CFLAGS) $(VIA) $(BUILD_DIR)/dep/fdl1_C_MACRO.txt -c $(MINCPATH) $(INCPATH) $< -o $(BUILD_DIR)/obj/fdl1/$@

.PHONY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "fdl_bootloader log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/obj/fdl1" ""

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl fdl1.bin $(BUILD_DIR)/img/ 
	@-$(PERL) make/perl_script/rm_file.pl fdl1.bin $(BUILD_DIR)/fdl_bootloader/
	@-$(PERL) make/perl_script/rm_dir.pl fdl1 $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl fdl1 $(BUILD_DIR)/dep
