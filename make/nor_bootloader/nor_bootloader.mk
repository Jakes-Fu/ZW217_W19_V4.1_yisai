include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
$(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/nor_bootloader_C_MACRO.txt)
$(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/nor_bootloader_A_MACRO.txt)
endif

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))

ifeq ($(strip $(PLATFORM)),UWS6121E)
PLATFORM_TEMP = uws6121e
endif

ifeq ($(strip $(FOTA_SUPPORT)),REDSTONE)
3RD_LIBS += Third-party/rsfota/rsupdate/rsua.a
3RD_LIBS += Third-party/rsfota/rsupdate/rsuasdk.a
endif

ifeq ($(strip $(PLATFORM)),UWS6121E)
ifeq ($(strip $(COMPILER)),	RVCT)
MCFLAG_OPT      = $(CPU) cortex-a5  --no_unaligned_access 
MASMFLAG_OPT    = $(CPU) cortex-a5  --no_unaligned_access 
endif
endif

MCFLAG_OPT   	+= $(VFP) -O2  $(ENDIAN_OPT) $(APCS) /interwork -DOS_NONE
MASMFLAG_OPT    += $(VFP) $(KEEP) $(CODE32)  $(ENDIAN_OPT) $(APCS) /interwork

ifeq  ($(strip $(COMPILER)), RVCT)
MCFLAG_OPT  += --debug
MASMFLAG_OPT += --debug
else
MCFLAG_OPT  += -g+
MASMFLAG_OPT += -g
endif

CONFIGPATH      =  MS_Customize/source/product/config
DRVPATH	        =  MS_Customize/source/product/driver

ifeq ($(strip $(PLATFORM)),	SC6530)
MCFLAG_OPT 		+= -DNOR_FDL_SC6800H  -D_LITTLE_ENDIAN
ifeq ($(strip $(CHIP_VER)),	6531)
ifeq ($(strip $(SECURE_BOOT_SUPPORT)),	TRUE)

SCATFILENAME 	= fdl_bootloader/nor_bootloader/src/nor_bootloader_secure_boot_sc6531.scf
else
SCATFILENAME 	= fdl_bootloader/nor_bootloader/src/nor_bootloader_sc6531.scf
endif
else
SCATFILENAME 	= fdl_bootloader/nor_bootloader/src/nor_bootloader.scf
endif
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
ifeq ($(strip $(SECURE_BOOT_SUPPORT)),	TRUE)
MCFLAG_OPT 		+= -D_LITTLE_ENDIAN
ifeq ($(strip $(FOTA_SUPPORT)),NONE)
SCATFILENAME 	= fdl_bootloader/nor_bootloader/src/nor_bootloader_secure_boot_sc6531efm.scf
else
SCATFILENAME 	= fdl_bootloader/nor_bootloader/src/nor_bootloader_secure_boot_fota_sc6531efm.scf
endif
else
MCFLAG_OPT 		+= -D_LITTLE_ENDIAN
ifeq ($(strip $(FOTA_SUPPORT)),NONE)
SCATFILENAME 	= fdl_bootloader/nor_bootloader/src/nor_bootloader_sc6531efm.scf
else
SCATFILENAME 	= fdl_bootloader/nor_bootloader/src/nor_bootloader_fota_sc6531efm.scf
endif
endif
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
MCFLAG_OPT 		+= -D_LITTLE_ENDIAN
# FOTA_SUPPORT_CMIOT #
#ifeq ($(strip $(FOTA_SUPPORT)),NONE)
SCATFILENAME 	= fdl_bootloader/nor_bootloader/src/nor_bootloader_uws6121e.scf
#else
#SCATFILENAME 	= fdl_bootloader/nor_bootloader/src/nor_bootloader_fota_uws6121e.scf
#endif
endif

MCFLAG_OPT 		+= -D_NEW_NORFDL

MINCPATH  = -Ifdl_bootloader/nor_bootloader/inc     \
            -Ifdl_bootloader/nor_fdl/inc            \
            -Ifdl_bootloader/chip_drv/inc           \
            -Ifdl_bootloader/common/inc             \
            -Ifdl_bootloader/chip_drv/src/watchdog  \
            -Ichip_drv/export/inc                   \
            -Ichip_drv/export/inc/outdated          \
            -Ichip_drv/chip_plf/export/inc          \
            -Ichip_drv/chip_module/watchdog/v5      \
            -Ichip_drv/chip_module/sys_timer/v5     \
			-Ichip_drv/chip_module/pin/uws6121e     \
            -Ichip_drv/chip_module/gpio/v5          \
            -I$(strip $(PS_PATH))/Layer1/export/inc \
            -IRTOS/export/inc                       \
            -Icommon/export/inc                     \
            -IMS_Customize/export/inc               \
            -I$(DRVPATH)/spiflash/inc               \
            -I$(DRVPATH)/norflash/normcp_info       \
            -IMS_Ref/export/inc


ifeq ($(strip $(PLATFORM)),	SC6530)
ifeq ($(strip $(CHIP_VER)),	6531)
MINCPATH += -Ichip_drv/chip_plf/sc6531
else
MINCPATH += -Ichip_drv/chip_plf/sc6530
endif
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MINCPATH += -Ichip_drv/chip_plf/sc6531efm
endif	



MINCPATH += -Ichip_drv/export/inc/outdated
MINCPATH += -Ichip_drv/chip_plf
MINCPATH += -Ichip_drv/chip_plf/$(strip $(PLATFORM_TEMP))
MINCPATH += -Ichip_drv/chip_plf/export/inc
MINCPATH += -Ichip_drv/chip_module/adi/$(strip $(PLATFORM_TEMP))
MINCPATH += -Ichip_drv/chip_module/analog/sc2720
MINCPATH += -Ichip_drv/chip_module/mmu
MINCPATH += -Ichip_drv/chip_module/lcdc/$(strip $(PLATFORM))
MINCPATH += -Ichip_drv/chip_module/sdio/$(strip $(PLATFORM_TEMP))
MINCPATH += -Icommon/export/inc/trace_id/
MINCPATH += -IMS_Ref/source/scp/inc

            
ifeq ($(strip $(PLATFORM)),	SC6530)
ifeq ($(strip $(CHIP_VER)),	6531)
MINCPATH += -Ichip_drv/chip_module/int/r2p0 \
	        -Ichip_drv/chip_module/usb/r3p0
else
MINCPATH += -Ichip_drv/chip_module/int/v5 \
	        -Ichip_drv/chip_module/usb/v5
endif
endif	        

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MINCPATH += -Ichip_drv/chip_module/int/r2p0 \
	        -Ichip_drv/chip_module/usb/r3p0
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
MINCPATH += -Ichip_drv/chip_module/int/r2p0 \
	        -Ichip_drv/chip_module/usb/r3p0
endif


MINCPATH += -Ifdl_bootloader/chip_drv/inc/$(strip $(PLATFORM_TEMP))  \
            -Ifdl_bootloader/chip_drv/src/chip/$(strip $(PLATFORM_TEMP))
	        
MINCPATH +=  -I$(CONFIGPATH)/$(strip $(PROD_CONFIG))

ifeq ($(strip $(FOTA_SUPPORT)),REDSTONE)
MINCPATH +=  -IThird-party/rsua
MINCPATH +=  -IMS_Ref/source/rsfota/porting
MINCPATH += -Ifdl_bootloader/nor_bootloader/rsfota
endif

SRCPATH	   =  fdl_bootloader/nor_bootloader/src \
              $(DRVPATH)/norflash \
              fdl_bootloader/chip_drv/src \
              fdl_bootloader/common/src \
              fdl_bootloader/chip_drv/src/usb_boot/usb11

ifeq ($(strip $(FOTA_SUPPORT)),REDSTONE)
SRCPATH += fdl_bootloader/nor_bootloader/rsfota
endif

ifeq ($(strip $(PLATFORM)),	SC6530)
ifeq ($(strip $(CHIP_VER)),	6531)
SRCPATH	   += fdl_bootloader/chip_drv/src/smc \
              fdl_bootloader/chip_drv/src/smc/r1p0 \
              fdl_bootloader/chip_drv/src/chip/v5 
endif
SRCPATH	   += fdl_bootloader/chip_drv/src/mmu 
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
SRCPATH	   += fdl_bootloader/chip_drv/src/smc \
              fdl_bootloader/chip_drv/src/smc/sc6531efm \
              fdl_bootloader/chip_drv/src/chip/sc6531efm \
              fdl_bootloader/chip_drv/src/mmu
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
SRCPATH	   += fdl_bootloader/chip_drv/src/smc \
              fdl_bootloader/chip_drv/src/mmu \
              fdl_bootloader/chip_drv/src/mmu/arm_v7 \
              fdl_bootloader/chip_drv/src/usb_boot/usb11/usb_uws6121e
ifeq ($(strip $(CHIP_VER)),	UIX8910MPW)
SRCPATH	   += fdl_bootloader/chip_drv/src/chip/uix8910mpw \
              fdl_bootloader/chip_drv/src/smc/uix8910mpw
else
ifneq "$(filter $(strip $(CHIP_VER)), UWS6121E UIS8910C UIS8910A UIS8910FF)" ""
SRCPATH	   += fdl_bootloader/chip_drv/src/chip/uws6121e \
              fdl_bootloader/chip_drv/src/smc/uws6121e \
              chip_drv/chip_module/sdio/uws6121e\
              chip_drv/chip_module/sdio/v5\
              chip_drv/chip_module/lcdc/uws6121e\
              chip_drv/chip_module/sfc/uws6121e\
              chip_drv/chip_module/timer/uws6121e\
              chip_drv/chip_module/spinlock/uws6121e\
              chip_drv/chip_module/chip/uws6121e\
              chip_drv/chip_plf/uws6121e\
              chip_drv/chip_module/int/uws6121e\
              chip_drv/chip_module/ifc/apifc\
			  chip_drv/chip_module/ifc/aonifc\
			  chip_drv/chip_module/ifc/lpsifc\
			  chip_drv/chip_module/ifc/ifc_manager\
              chip_drv/chip_module/spinlock/uws6121e\
              chip_drv/chip_module/adi/uws6121e\
              fdl_bootloader/chip_drv/src/mmu\
              fdl_bootloader/tf_fdl/src/nv\
              MS_Ref/source/base/src\
              MS_Ref/source/scp/src
endif
endif
endif

SRCPATH	   +=  $(CONFIGPATH)/$(strip $(PROD_CONFIG))

SRCPATH	   += fdl_bootloader/chip_drv/src/watchdog
SRCPATH	   += chip_drv/chip_module/adi/v5
SRCPATH	   += fdl_bootloader/chip_drv/src/watchdog/v5
SRCPATH    += fdl_bootloader/chip_drv/src/efuse

ifeq ($(strip $(PLATFORM)),	UWS6121E)
SOURCES		=   fdl_main.c\
                init.s\
                boot.s\
                tf_main_nor.c\
                tf_str.c\
                tf_dummy.c\
                cc936.c\
                ff.c\
                diskio_sdio.c\
                card_sdio.c\
                sdio_card_pal.c\
                mcd_sdmmc.c\
                hal_sdmmc.c\
                apifc_phy.c\
				ifc_hal.c\
				ifc_manager_cfg.c\
                spinlock_phy_uws6121e.c \
				efuse_api.c            \
				detect_error.c	\
				boot_userial.c \
				hal_rom_api.c
else
SOURCES		=   fdl_crc.c           \
				fdl_main.c          \
				fdl_stdio.c         \
				fdl_trc.c           \
				fdl_sys.c           \
				bdl_customif.c      \
				bdl_packet.c        \
				drv_usb.c           \
				sio_drv.c           \
				uart_boot.c         \
				usb_boot.c          \
				virtual_com.c       \
				init.s              \
				boot.s              \
				fdl_update_fixnv.c  \
				detect_error.c      \
				dl_engine.c         \
				chip_cfg.c          \
				adi_drv.c           \
				efuse_drv.c           \
				watchdog_hal.c      \
				watchdog_phy_v5.c
endif				

ifeq ($(strip $(PLATFORM)),	SC6530)
    SOURCES +=  vfp.s
endif

MINCPATH   += -Ifdl_bootloader/chip_drv/src/usb_boot/usb11/usb_uws6121e
SRCPATH    += chip_drv/chip_plf/uws6121e
ifeq ($(strip $(RESET_DUMP_SUPPORT)),	TRUE)
SOURCES    += uws6121e_mem_cfg.c
SOURCES    += reset_dump.c
endif

ifeq ($(strip $(SECURE_BOOT_SUPPORT)),    TRUE)
SOURCES += secure.c
endif


ifeq ($(strip $(MODEM_TYPE)), ZBX)
    SOURCES +=  gpio_drv.c
endif

ifneq ($(strip $(PLATFORM)),	UWS6121E)
ifeq ($(strip $(FLASH_TYPE)),	SPI)
SOURCES		+=	sfc_phy_v5.c           \
                spiflash.c    \
                spiflash_cfg.c    
else
SOURCES		+=	flash_drv.c \
                fdl_flash_cfg.c     \
                normcp_spec.c \
                flash_cfg.c					
endif
endif

ifeq ($(strip $(PLATFORM)),	SC6530)
ifeq ($(strip $(CHIP_VER)),	6531)
SOURCES += 	smc_hal.c \
            smc_phy_r1p0.c \
            chip_phy_v5.c
endif
SOURCES +=  mmu.c \
            mmu_asm.s
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
SOURCES +=  smc_hal.c \
            smc_phy_sc6531efm.c \
            chip_phy_sc6531efm.c \
            mmu.c \
            mmu_asm.s
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
SOURCES +=  clk_div.c \
            clock.c \
            ddr.c \
            chip_phy_uws6121e.c \
            serial.c \
            mmu.c \
            v7_mmu_cache.s \
            v7_mmu.c
ifeq ($(strip $(CHIP_VER)),	UIX8910MPW)
SOURCES +=  rda_sys.c
endif






endif

OBJECTS1 	:= $(patsubst %.c, %.o, $(SOURCES))
OBJECTS         := $(patsubst %.s, %.o, $(OBJECTS1))						

vpath
vpath %.o  $(BUILD_DIR)/obj/nor_bootloader
vpath %.d  $(BUILD_DIR)/dep/nor_bootloader
vpath %.h  $(subst \,/,$(MINCPATH))
vpath %.c  $(subst \,/,$(SRCPATH))
vpath %.s  $(subst \,/,$(SRCPATH))
DEPPATH	=  $(BUILD_DIR)/dep/nor_bootloader

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
FLOATLIBS	=	lib/fz_5s.l \
				lib/m_5s.l \
				lib/vfpsupport.l
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
FLOATLIBS	=	lib/fz_5s.l \
				lib/m_5s.l \
				lib/vfpsupport.l
endif

all: builddir $(OBJECTS)
ifeq ($(strip $(PLATFORM)), SC6530)
ifeq  ($(strip $(COMPILER)), RVCT)
ifneq ($(strip $(FOTA_SUPPORT)),NONE)
	@$(LINK) --keep reset_data --info totals --entry __vectors --scatter $(SCATFILENAME) --map --symbols --list nor_bootloader.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) $(3RD_LIBS) --Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
else
	@$(LINK) --keep reset_data --info totals --entry __vectors --scatter $(SCATFILENAME) --map --symbols --list nor_bootloader.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) --Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
endif
	@$(FROMELF) -c --bin $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf --output $(BUILD_DIR)/nor_bootloader/nor_bootloader.bin
else
ifneq ($(strip $(FOTA_SUPPORT)),NONE)
	@$(LINK) -keep reset_data -info totals -entry 0x0 -scatter $(SCATFILENAME) -map -symbols -list nor_bootloader_$(PROJECT).map -first init.o -info sizes -xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) $(3RD_LIBS) -Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
else
	@$(LINK) -keep reset_data -info totals -entry 0x0 -scatter $(SCATFILENAME) -map -symbols -list nor_bootloader_$(PROJECT).map -first init.o -info sizes -xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) -Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
endif
	@$(FROMELF) -c -bin $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf -output $(BUILD_DIR)/nor_bootloader/nor_bootloader.bin
endif
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
ifeq  ($(strip $(COMPILER)), RVCT)
ifneq ($(strip $(FOTA_SUPPORT)),NONE)
	@$(LINK) --keep reset_data --info totals --entry __vectors --scatter $(SCATFILENAME) --verbose --diag_suppress 6366 --map --symbols --list nor_bootloader.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) $(FLOATLIBS) $(3RD_LIBS) --Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
else
	@$(LINK) --keep reset_data --info totals --entry __vectors --scatter $(SCATFILENAME) --verbose --diag_suppress 6366 --map --symbols --list nor_bootloader.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) $(FLOATLIBS) --Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
endif
	@$(FROMELF) -c --bin $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf --output $(BUILD_DIR)/nor_bootloader/nor_bootloader.bin
else
ifneq ($(strip $(FOTA_SUPPORT)),NONE)
	@$(LINK) -keep reset_data -info totals -entry 0x0 -scatter $(SCATFILENAME) -map -symbols -list nor_bootloader_$(PROJECT).map -first init.o -info sizes -xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) $(3RD_LIBS) -Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
else
	@$(LINK) -keep reset_data -info totals -entry 0x0 -scatter $(SCATFILENAME) -map -symbols -list nor_bootloader_$(PROJECT).map -first init.o -info sizes -xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) -Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
endif
	@$(FROMELF) -c -bin $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf -output $(BUILD_DIR)/nor_bootloader/nor_bootloader.bin
endif
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
ifeq  ($(strip $(COMPILER)), RVCT)
ifneq ($(strip $(FOTA_SUPPORT)),NONE)
	@$(LINK) --keep reset_data --info totals --entry __vectors --scatter $(SCATFILENAME) --verbose --diag_suppress 6366 --map --symbols --list nor_bootloader.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) $(FLOATLIBS) $(3RD_LIBS) --Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
else
	@$(LINK) --keep reset_data --info totals --entry __vectors --scatter $(SCATFILENAME) --verbose --diag_suppress 6366 --map --symbols --list nor_bootloader.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) $(FLOATLIBS) --Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
endif
	@$(FROMELF) -c --bin $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf --output $(BUILD_DIR)/nor_bootloader/nor_bootloader.bin
else
ifneq ($(strip $(FOTA_SUPPORT)),NONE)
	@$(LINK) -keep reset_data -info totals -entry 0x800100 -scatter $(SCATFILENAME) -map -symbols -list nor_bootloader_$(PROJECT).map -first init.o -info sizes -xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) $(3RD_LIBS) -Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
else
	@$(LINK) -keep reset_data -info totals -entry 0x800100 -scatter $(SCATFILENAME) -map -symbols -list nor_bootloader_$(PROJECT).map -first init.o -info sizes -xref \
	$(addprefix $(BUILD_DIR)/obj/nor_bootloader/, $(OBJECTS)) -Output $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf
endif
	@$(FROMELF) -c -bin $(BUILD_DIR)/nor_bootloader/nor_bootloader.axf -output $(BUILD_DIR)/nor_bootloader/nor_bootloader.bin
endif
endif

	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/img/nor_bootloader.bin ""
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/nor_bootloader/nor_bootloader.bin) $(call Path_process,$(BUILD_DIR)/img/nor_bootloader.bin)

ifneq ($(strip $(MAKE_VAR_DEPEND_D)),0)
-include $(OBJECTS:.o=.d)
endif

%.d %.o:%.s
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(ASM) $(MASMFLAG_OPT) $(VIA) $(BUILD_DIR)/dep/nor_bootloader_A_MACRO.txt $(INCPATH) $<  $(MDFLAGS) ./$(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/nor_bootloader/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/nor_bootloader/nor_bootloader.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""
%.d %.o:%.c
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(ARMCC) $(IGNOR_WARNNINGS) $(MCFLAG_OPT) $(VIA) $(BUILD_DIR)/dep/nor_bootloader_C_MACRO.txt -c $(MINCPATH) $(INCPATH) $< $(MDFLAGS) ./$(DEPPATH)/$*.d.tmp -o $(BUILD_DIR)/obj/nor_bootloader/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/nor_bootloader/nor_bootloader.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""

.PHONY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "nor_bootloader log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "nor_bootloader" $(BUILD_DIR)/obj

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl nor_bootloader.bin $(BUILD_DIR)/img/ 
	@-$(PERL) make/perl_script/rm_file.pl nor_bootloader.bin $(BUILD_DIR)/nor_bootloader/
	@-$(PERL) make/perl_script/rm_dir.pl  nor_bootloader $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl  nor_bootloader $(BUILD_DIR)/dep
