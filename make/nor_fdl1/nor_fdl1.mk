include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/nor_fdl1_C_MACRO.txt)
  $(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/nor_fdl1_A_MACRO.txt)
endif

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))

MCFLAG_OPT		= -O2 --arm $(ENDIAN_OPT) $(APCS) /interwork
MASMFLAG_OPT	= $(KEEP) --arm $(ENDIAN_OPT) $(APCS) /interwork

ifeq ($(strip $(COMPILER)), RVCT)
MCFLAG_OPT		+= --debug
MASMFLAG_OPT	+= --debug
endif

CONFIGPATH	=  MS_Customize/source/product/config
DRVPATH 	=  MS_Customize/source/product/driver

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT		+= -DNOR_FDL_SC6800H  -D_LITTLE_ENDIAN
SCATFILENAME	= fdl_bootloader/nor_fdl1/nor_fdl1_sc6531efm.scf
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MCFLAG_OPT		+=	-D_LITTLE_ENDIAN
SCATFILENAME	= fdl_bootloader/nor_fdl1/nor_fdl1_uws6121e.scf
endif

MCFLAG_OPT	+= -D_NEW_NORFDL

MINCPATH	=	-Ifdl_bootloader/common/inc \
						-Ifdl_bootloader/nor_fdl1/inc \
						-Ichip_drv/export/inc \
						-Ichip_drv/chip_plf/export/inc \
						-Ifdl_bootloader/chip_drv/inc \
						-Ichip_drv/export/inc/outdated \
						-IRTOS/export/inc \
						-Icommon/export/inc \
						-IMS_Ref/export/inc \
						-Ichip_drv/chip_module/pin/uws6121e \
						-IMS_Customize/export/inc

MSRCPATH	=	fdl_bootloader/common/src \
						fdl_bootloader/nor_fdl1/src \
						fdl_bootloader/chip_drv/src 

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MINCPATH += -Ichip_drv/chip_plf/sc6531efm
endif  

ifeq ($(strip $(PLATFORM)), UWS6121E)
ifeq ($(strip $(CHIP_VER)), UIX8910MPW)
MINCPATH += -Ifdl_bootloader/chip_drv/inc/uix8910mpw   \
			-Ichip_drv/chip_plf/uix8910mpw
else
ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIS8910C UIS8910A UIS8910FF)" ""
MINCPATH += -Ifdl_bootloader/chip_drv/inc/uws6121e	  \
			-Ichip_drv/chip_plf/uws6121e
endif
endif	
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MINCPATH += -Ichip_drv/chip_module/int/r2p0 \
						-Ichip_drv/chip_module/usb/r3p0 
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH += -Ichip_drv/chip_module/int/r2p0 \
			-Ichip_drv/chip_module/usb/r3p0 
endif
 
		
MINCPATH += -I$(CONFIGPATH)/$(strip $(PROD_CONFIG))

SRCPATH =	fdl_bootloader/nor_fdl1/src \
					fdl_bootloader/chip_drv/src \
					fdl_bootloader/common/src \
					fdl_bootloader/chip_drv/src/usb_boot/usb11 \
					fdl_bootloader/chip_drv/src/watchdog/ \
					fdl_bootloader/chip_drv/src/watchdog/v5

ifeq ($(strip $(PLATFORM)), SC6531EFM)
SRCPATH +=	fdl_bootloader/chip_drv/src/smc \
						fdl_bootloader/chip_drv/src/smc/sc6531efm \
						fdl_bootloader/chip_drv/src/chip/sc6531efm \
						fdl_bootloader/chip_drv/src/mmu 
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
SRCPATH +=	fdl_bootloader/chip_drv/src/smc \
			  fdl_bootloader/chip_drv/src/mmu \
			  fdl_bootloader/chip_drv/src/usb_boot/usb11/usb_uws6121e
ifeq ($(strip $(CHIP_VER)), UIX8910MPW)
SRCPATH    += fdl_bootloader/chip_drv/src/chip/uix8910mpw \
			  fdl_bootloader/chip_drv/src/smc/uix8910mpw
endif
ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIS8910C UIS8910A UIS8910FF)" ""
SRCPATH    += fdl_bootloader/chip_drv/src/chip/uws6121e \
			  fdl_bootloader/chip_drv/src/smc/uws6121e
endif
endif

SRCPATH    += fdl_bootloader/chip_drv/src/efuse


SRCPATH 	+=	$(CONFIGPATH)/$(strip $(PROD_CONFIG))

SOURCES 	= fdl_main.c	  \
						fdl_command.c	\
						fdl_stdio.c 	\
						fdl_sys.c	  \
						dl_engine.c 	\
						fdl_crc.c		\
						packet.c			\
						sio_drv.c		\
						hal_rom_api.c	\
						init.s					\
						chip_cfg.c			\
						adi_drv.c			\
						efuse_drv.c 			\
						efuse_api.c 	   \
						fdl_channel.c \
						watchdog_hal.c		\
						watchdog_phy_v5.c

ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES +=	mmu.c \
			mmu_asm.s \
			smc_phy_sc6531efm.c \
			chip_phy_sc6531efm.c \
			#smc_hal.c
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES +=	clk_div.c \
			clock.c \
			ddr.c \
			chip_phy_uws6121e.c \
		boot_userial.c \
			serial.c
else
SOURCES +=	drv_usb.c			\
		usb_boot.c \
		virtual_com.c
ifeq ($(strip $(CHIP_VER)), UIX8910MPW)
SOURCES +=	rda_sys.c
endif
endif

ifeq ($(strip $(MEM_STRESS_TEST)),	PSRAM)
SOURCES += mem_test.c
endif

ifeq ($(strip $(SECURE_BOOT_SUPPORT)),	  TRUE)
SOURCES +=	secure.c
endif

OBJECTS1	:= $(patsubst %.c, %.o, $(SOURCES))
OBJECTS 	:= $(patsubst %.s, %.o, $(OBJECTS1))

vpath
vpath %.o  $(BUILD_DIR)/obj/nor_fdl1
vpath %.d  $(BUILD_DIR)/dep/nor_fdl1
vpath %.h  $(subst \,/,$(MINCPATH))
vpath %.c  $(subst \,/,$(SRCPATH))
vpath %.s  $(subst \,/,$(SRCPATH))
DEPPATH =  $(BUILD_DIR)/dep/nor_fdl1

all: builddir $(OBJECTS)
ifeq ($(strip $(PLATFORM)), SC6531EFM)
ifeq ($(strip $(COMPILER)), RVCT)
	@$(LINK) --info totals --entry 0x40004000 --scatter $(SCATFILENAME) --map --symbols --list fdl_nor1.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_fdl1/, $(OBJECTS)) --Output $(BUILD_DIR)/fdl_bootloader/nor_fdl1.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/fdl_bootloader/nor_fdl1.axf --output $(BUILD_DIR)/fdl_bootloader/nor_fdl1.bin
else
	@$(LINK) -info totals -entry 0x40004000 -scatter $(SCATFILENAME) -map -symbols -list fdl_nor1.map -first init.o -info sizes -xref \
	$(addprefix $(BUILD_DIR)/obj/nor_fdl1/, $(OBJECTS)) -Output $(BUILD_DIR)/fdl_bootloader/nor_fdl1.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/fdl_bootloader/nor_fdl1.axf -output $(BUILD_DIR)/fdl_bootloader/nor_fdl1.bin
endif	
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
ifeq ($(strip $(COMPILER)), RVCT)
	@$(LINK) --info totals --entry 0x50800100 --scatter $(SCATFILENAME) --map --symbols --list fdl_nor1.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_fdl1/, $(OBJECTS)) --Output $(BUILD_DIR)/fdl_bootloader/nor_fdl1.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/fdl_bootloader/nor_fdl1.axf --output $(BUILD_DIR)/fdl_bootloader/nor_fdl1.bin
else
	@$(LINK) -info totals -entry 0x50800100 -scatter $(SCATFILENAME) -map -symbols -list fdl_nor1.map -first init.o -info sizes -xref \
	$(addprefix $(BUILD_DIR)/obj/nor_fdl1/, $(OBJECTS)) -Output $(BUILD_DIR)/fdl_bootloader/nor_fdl1.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/fdl_bootloader/nor_fdl1.axf -output $(BUILD_DIR)/fdl_bootloader/nor_fdl1.bin
endif	
endif

	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/img/nor_fdl1.bin ""
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/fdl_bootloader/nor_fdl1.bin) $(call Path_process,$(BUILD_DIR)/img/nor_fdl1.bin)

ifneq ($(strip $(MAKE_VAR_DEPEND_D)),0)
-include $(OBJECTS:.o=.d)
endif

%.d %.o:%.s
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(ASM) $(MASMFLAG_OPT) $(VIA) $(BUILD_DIR)/dep/nor_fdl1_A_MACRO.txt $(INCPATH) $<  $(MDFLAGS) ./$(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/nor_fdl1/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/nor_fdl1/nor_fdl1.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""
%.d %.o:%.c
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(CC) $(IGNOR_WARNNINGS) $(MCFLAG_OPT) $(VIA) $(BUILD_DIR)/dep/nor_fdl1_C_MACRO.txt -c $(MINCPATH) $(INCPATH) $< $(MDFLAGS) ./$(DEPPATH)/$*.d.tmp -o $(BUILD_DIR)/obj/nor_fdl1/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/nor_fdl1/nor_fdl1.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""

.PHONY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "fdl_bootloader log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "nor_fdl1" $(BUILD_DIR)/obj

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl nor_fdl1.bin $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl nor_fdl1.bin $(BUILD_DIR)/fdl_bootloader/
	@-$(PERL) make/perl_script/rm_dir.pl nor_fdl1 $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl nor_fdl1 $(BUILD_DIR)/dep

	
