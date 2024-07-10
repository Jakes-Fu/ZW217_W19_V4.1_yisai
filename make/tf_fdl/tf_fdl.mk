include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/tf_fdl_C_MACRO.txt)
  $(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/tf_fdl_A_MACRO.txt)
endif

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))
VERSION_FILE := $(shell $(PERL) ./make/perl_script/get_filename.pl version $(strip $(PLATFORM))_$(PROJECT)_version.c $(strip $(PLATFORM))_$(strip $(PRODUCT_CONFIG))_version.c)

CONFIGPATH   =  MS_Customize/source/product/config
DRVPATH      =  MS_Customize/source/product/driver

MCFLAG_OPT      += -DOS_NONE -DTF_FDL_BUILD -g --debug
MASMFLAG_OPT	+= -g --debug

MCFLAG_OPT      += -DTF_UPDATE_SC6530

ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MCFLAG_OPT      += -DTF_LITTLE_ENDIAN
MASMFLAG_OPT    += -PD "TF_LITTLE_ENDIAN SETL {TRUE}"
endif

MINCPATH  = -Ifdl_bootloader/tf_fdl/inc\
            -Ifdl_bootloader/fdl2/inc\
            -Ifdl_bootloader/common/inc\
            -Ichip_drv/export/inc \
            -Ichip_drv/source/h \
            -IRTOS/export/inc \
            -Icommon/export/inc \
            -Icommon/export/inc/trace_id \
            -IMS_Customize/export/inc \
            -I$(CONFIGPATH)/$(PRODUCT_CONFIG) \
            -I$(DRVPATH)/norflash/normcp_info\
            -IMS_Ref/export/inc \
            -IMS_Ref/source/scp/inc \
            -I$(DRVPATH)/tf_load/inc\
            -Ifdl_bootloader/nor_fdl/inc\
            -IMS_Ref/source/lcd/inc\
            -Ichip_drv/chip_module/gpio\
            -Ibuild/$(PROJECT)_builddir/tmp\
            -Ichip_drv/export/inc/outdated \
            -IMS_Customize/source/product/driver/spiflash/inc\
            -Ibase/ps/export/inc   \
			-Ifdl_bootloader/chip_drv/inc/uws6121e \
            -Ifdl_bootloader/chip_drv/inc    

ifeq ($(strip $(MULTI_SIM_SYS)), TRI)
  MINCPATH  += -Itrisim/Layer1/export/inc
else
  MINCPATH  += -ILayer1/export/inc
endif

MINCPATH  += -Ifdl_bootloader/tf_fdl/src/tf_lcd/inc
MINCPATH +=  -I$(CONFIGPATH)/$(strip $(PROJECT))

SRCPATH    =  fdl_bootloader/tf_fdl/src \
              fdl_bootloader/tf_fdl/src/nv \
              $(CONFIGPATH)/$(PRODUCT_CONFIG) \
              MS_Customize/source/product/driver/lcd\
              chip_drv/source/c \
              chip_drv/chip_module/ldo \
              chip_drv/chip_module/clock \
              chip_drv/chip_module/chip/uws6121e \
              chip_drv/chip_module/adi/uws6121e \
              $(DRVPATH)/nandflash\
              MS_Ref/source/scp/src\
              chip_drv/chip_module/gpio\
              chip_drv/chip_module/gpio/uws6121e\
              chip_drv/chip_module/mmu/uws6121e\
              chip_drv/chip_module/lcdc/v5\
              fdl_bootloader/chip_drv/src/watchdog\
              fdl_bootloader/chip_drv/src\
              version

SRCPATH   +=  fdl_bootloader/tf_fdl/src/tf_lcd/src
SRCPATH   +=  MS_Ref/source/lcd/src
SRCPATH   +=  $(CONFIGPATH)/$(strip $(PROJECT))
SRCPATH   +=  fdl_bootloader/nor_fdl/src
SRCPATH   +=  fdl_bootloader/tf_fdl/src/fs

SOURCES     =   tf_init.s \
                tf_main.c \
                tf_display.c \
                tf_dummy.c \
                tf_str.c \
                tf_lcmprod.c \
                tf_lcmcfg.c \
                cc936.c \
                crc16.c \
                card_sdio.c \
                sdio_card_pal.c \
                serial.c

SOURCES   +=  ff.c diskio_sdio.c
 
ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
  SCATFILENAME    = fdl_bootloader/tf_fdl/src/tf_scatter_nand.scf
  SOURCES   += 	tf_main_nand.c\
				tf_nv_nand.c\
				tf_drv_nand.c\
				nand_spec.c				
else
  SCATFILENAME    = fdl_bootloader/tf_fdl/src/tf_scatter_nor.scf
  SOURCES   += 	tf_main_nor.c\
				tf_parse_nor.c\
				tf_nv_nor.c
		
  ifeq ($(strip $(FLASH_TYPE)), SPI)
  ifeq ($(strip $(PLATFORM)),	UWS6121E)
    SOURCES	+=spiflash_v2.c spiflash_cfg.c
  else
    SOURCES	+=spiflash.c spiflash_cfg.c
  endif
  else
    SOURCES += flash_drv.c fdl_flash_cfg.c
  endif
endif


ifeq ($(strip $(PLATFORM)),	UWS6121E)
  MINCPATH += -Ichip_drv/export/inc/outdated
  MINCPATH += -Ichip_drv/chip_plf
  MINCPATH += -Ichip_drv/chip_plf/uws6121e
  MINCPATH += -Ichip_drv/chip_plf/export/inc
  MINCPATH += -Ichip_drv/chip_module/adi/uws6121e
  MINCPATH += -Ichip_drv/chip_module/analog/sc2720
  MINCPATH += -Ichip_drv/chip_module/mmu
  MINCPATH += -Ichip_drv/chip_module/lcdc/uws6121e
  MINCPATH += -Ichip_drv/chip_module/sdio/uws6121e
  MINCPATH += -Ichip_drv/chip_module/pin/uws6121e
  MINCPATH += -Icommon/export/inc/trace_id/
  MINCPATH += -IMS_Ref/source/scp/inc

  SRCPATH   +=  chip_drv/chip_module/sdio/uws6121e \
                chip_drv/chip_module/sdio/v5 \
                chip_drv/chip_module/lcdc/uws6121e \
                chip_drv/chip_module/sfc/uws6121e \
                chip_drv/chip_module/timer/uws6121e \
                chip_drv/chip_module/spinlock/uws6121e \
                chip_drv/chip_module/chip/uws6121e \
                chip_drv/chip_plf/uws6121e \
                chip_drv/chip_module/int/uws6121e \
                chip_drv/chip_module/ifc/apifc \
				chip_drv/chip_module/ifc/aonifc \
				chip_drv/chip_module/ifc/lpsifc \
				chip_drv/chip_module/ifc/ifc_manager \
                chip_drv/chip_module/analog/sc2720 \
                chip_drv/chip_module/adi/uws6121e \
                chip_drv/chip_module/spinlock/uws6121e \
                chip_drv/chip_module/lcdc_v5 \
                chip_drv/chip_module/spi \
                chip_drv/chip_module/spi/v5 \
                chip_drv/chip_module/dma \
                chip_drv/chip_module/dma/uws6121e \
                chip_drv/chip_module/gpio \
                chip_drv/chip_module/gpio/uws6121e \
                fdl_bootloader/chip_drv/src/mmu \
                fdl_bootloader/chip_drv/src/usb_boot/usb11/usb_uws6121e \
                MS_Ref/source/base/src

  SOURCES   +=  mcd_sdmmc.c \
              hal_sdmmc.c \
              hal_gouda.c \
              apifc_phy.c \
			  ifc_hal.c\
			  ifc_manager_cfg.c\
              analog_phy_w217.c \
              adi_phy_uws6121e.c \
              spinlock_phy_uws6121e.c \
              sfc_phy_uws6121e_v2.c \
              tf_lcd_uws6121e.c \
              ldo_cfg.c \
              ldo_hal.c \


  SOURCES   +=  tft_GC9305.c \
                tft_GC9306.c \
                tft_GC9307.c \
                tft_st7789v2.c \
                tft_JD9851.c
else #else of 8910

ifeq ($(strip $(PLATFORM)),	SC8800G)
  MINCPATH += -Ichip_drv/export/inc/outdated
  MINCPATH += -Ichip_drv/chip_plf
  MINCPATH += -Ichip_drv/chip_plf/sc8800g
  MINCPATH += -Ichip_drv/chip_plf/export/inc
  MINCPATH += -Ichip_drv/chip_module/adi/v3
  MINCPATH += -Ichip_drv/chip_module/analog/v3
endif

ifeq ($(strip $(PLATFORM)),    SC6800H)
  MINCPATH += -Ichip_drv/export/inc/outdated
  MINCPATH += -Ichip_drv/chip_plf/export/inc
  MINCPATH += -Ichip_drv/chip_plf
  MINCPATH += -Ichip_drv/chip_plf/sc6800h
  MINCPATH += -Ichip_drv/chip_module/gpio/v0
endif

ifeq ($(strip $(PLATFORM)),    SC6530)
  MINCPATH += -Ichip_drv/export/inc/outdated
  MINCPATH += -Ichip_drv/chip_plf/export/inc
  MINCPATH += -Ichip_drv/chip_plf
  MINCPATH += -Ichip_drv/chip_module/gpio/v5
  ifeq ($(strip $(CHIP_VER)),    6531)
    MINCPATH += -Ichip_drv/chip_plf/sc6531
  else
    MINCPATH += -Ichip_drv/chip_plf/sc6530
  endif
endif

ifeq ($(strip $(PLATFORM)),    SC6530)
  SRCPATH   +=   chip_drv/chip_module/sdio/v5\
                chip_drv/chip_module/lcdc/v5

  ifeq ($(strip $(CHIP_VER)),	6531)
    SRCPATH	 += fdl_bootloader/chip_drv/src/smc \
                  fdl_bootloader/chip_drv/src/smc/r1p0 \
                  fdl_bootloader/chip_drv/src/chip/v5 \
                  chip_drv/chip_module/int
  else
    SRCPATH  +=   chip_drv/chip_module/sdio/v0\
                   chip_drv/chip_module/lcdc/v0
  endif
endif

               
ifeq ($(strip $(PLATFORM)),	SC6530)
  SOURCES   +=  sdhost_phy_v5.c
  ifeq ($(strip $(CHIP_VER)),	6531)
    SOURCES +=  smc_phy_r1p0.c \
              smc_hal.c
  endif 
endif

ifneq ($(strip $(PLATFORM)),SC6530)
  SOURCES   +=  sdhost_phy.c
endif

ifeq ($(strip $(PLATFORM)),	SC6800H)
  SRCPATH += chip_drv/chip_plf/sc6800h \
             fdl_bootloader/chip_drv/src/watchdog/v0
  SOURCES   += 	lcm_phy_v0.c \
                sc6800h_gpio_cfg.c \
                watchdog_phy_v0.c
endif

ifeq ($(strip $(PLATFORM)),	SC8800G)
  SRCPATH += chip_drv/chip_plf/sc8800g \
             fdl_bootloader/chip_drv/src/watchdog/v3\
             chip_drv/chip_module/analog/v5

  SOURCES   += 	sc8800g_gpio_cfg.c \
				watchdog_phy_v3.c\
				analog_phy_v3.c
endif

ifeq ($(strip $(PLATFORM)),	SC6530)
  ifeq ($(strip $(CHIP_VER)),	6531)
    SRCPATH += chip_drv/chip_plf/sc6531 \
               chip_drv\chip_module\lcdc\v5\
               chip_drv\chip_module\adi\v5\
               chip_drv\chip_module\watchdog\v5\
               chip_drv\chip_module\dma\
               chip_drv\chip_module\dma\r3p0\
               chip_drv\chip_module\analog\sr1131
  else                     
    SRCPATH += chip_drv/chip_plf/sc6530 \
               chip_drv\chip_module\lcdc\v5\
               chip_drv\chip_module\adi\v5\
               chip_drv\chip_module\dma\
               chip_drv\chip_module\dma\v5\
               chip_drv\chip_module\watchdog\v5\
               chip_drv\chip_module\analog\v5
  endif
		       
  SOURCES   += 	lcd_if_hal.c\
                tf_lcd.c \
                watchdog_phy_v5.c \
                adi_phy_v5.c \
                dma_hal.c \
                gpio_phy_v0.c \
                sfc_phy_v5.c

  ifeq ($(strip $(CHIP_VER)),    6531)
    SOURCES   += analog_phy_sr1131.c\
                 dma_phy_r3p0.c\
                 sc6531_gpio_cfg.c\
                 sc6531_dma_cfg.c
  else
    SOURCES   += analog_phy_v5.c\
                 dma_phy_v5.c\
                 sc6530_gpio_cfg.c
  endif

  ifeq ($(strip $(MAINLCM_INTERFACE)), LCM)
    SOURCES   += 	lcm_phy_v5.c
  endif

  SOURCES   += 	lcd_if_spi.c
endif

ifeq ($(strip $(PLATFORM)),	SC6530)
  SOURCES   += 	lcdc_drv_sc6530.c
else
  SOURCES   += 	lcdc_drv.c
endif


ifeq ($(strip $(MAINLCD_SIZE)),128X64)            
  SOURCES += fstn_nt7538.c
endif

endif #end of else 8910

ifeq ($(strip $(RELEASE_INFO)), TRUE)
MCFLAG_OPT   	+= -D_VERSION_RELEASE_
else
MCFLAG_OPT   	+= -D_VERSION_DEBUG_
endif

SOURCES			+= $(VERSION_FILE)
             	
ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
LIBSXSR         =	lib/$(strip $(PRODUCT_BASELIB_DIR))/xsr_layer_os_none.a		\
			lib/$(strip $(PRODUCT_BASELIB_DIR))/ftl_os_none.a		\
			lib/$(strip $(PRODUCT_BASELIB_DIR))/sci_ftl_layer_os_none.a
endif
            
OBJECTS1    := $(patsubst %.c, %.o, $(SOURCES))
OBJECTS     := $(patsubst %.s, %.o, $(OBJECTS1))                        

vpath
vpath %.o  $(BUILD_DIR)/obj/tf_fdl
vpath %.d  $(BUILD_DIR)/dep/tf_fdl
vpath %.h  $(subst \,/,$(MINCPATH))
vpath %.c  $(subst \,/,$(SRCPATH))
vpath %.s  $(subst \,/,$(SRCPATH))
DEPPATH =  $(BUILD_DIR)/dep/tf_fdl

all: builddir $(OBJECTS)
ifeq ($(strip $(COMPILER)),	RVCT)
ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
	@$(LINK) --info totals --map --symbols --scatter $(SCATFILENAME) --list tfboot_$(PROJECT).map --first init.o --info sizes $(LIBSXSR) \
	$(addprefix $(BUILD_DIR)/obj/tf_fdl/, $(OBJECTS)) --Output $(BUILD_DIR)/tf_fdl/tfboot.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/tf_fdl/tfboot.axf --output $(BUILD_DIR)/tf_fdl/tfboot.bin	
	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/img/tfboot.bin ""
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/tf_fdl/tfboot.bin) $(call Path_process,$(BUILD_DIR)/img/tfboot.bin)
	@"$(MAKE_HOME)/BinToTxt.exe" "$(BUILD_DIR)/tf_fdl/tfboot.bin" "MS_Customize/export/inc/tfboot.h"
else
	@$(LINK) --info totals --map --symbols --scatter $(SCATFILENAME) --list tfboot_$(PROJECT).map --first init.o --info sizes \
	$(addprefix $(BUILD_DIR)/obj/tf_fdl/, $(OBJECTS)) --Output $(BUILD_DIR)/tf_fdl/tfboot.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/tf_fdl/tfboot.axf --output $(BUILD_DIR)/tf_fdl/tfboot.bin	
	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/img/tfboot.bin ""
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/tf_fdl/tfboot.bin) $(call Path_process,$(BUILD_DIR)/img/tfboot.bin)
	@"$(MAKE_HOME)/BinToTxt.exe" "$(BUILD_DIR)/tf_fdl/tfboot.bin" "MS_Customize/export/inc/tfboot.h"
endif
else
ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
	@$(LINK) -info totals -map -symbols -scatter $(SCATFILENAME) -list tfboot_$(PROJECT).map -first init.o -info sizes $(LIBSXSR) \
	$(addprefix $(BUILD_DIR)/obj/tf_fdl/, $(OBJECTS)) -Output $(BUILD_DIR)/tf_fdl/tfboot.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/tf_fdl/tfboot.axf -output $(BUILD_DIR)/tf_fdl/tfboot.bin	
	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/img/tfboot.bin ""
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/tf_fdl/tfboot.bin) $(call Path_process,$(BUILD_DIR)/img/tfboot.bin)
	@"$(MAKE_HOME)/BinToTxt.exe" "$(BUILD_DIR)/tf_fdl/tfboot.bin" "MS_Customize/export/inc/tfboot.h"
else
	@$(LINK) -info totals -map -symbols -scatter $(SCATFILENAME) -list tfboot_$(PROJECT).map -first init.o -info sizes \
	$(addprefix $(BUILD_DIR)/obj/tf_fdl/, $(OBJECTS)) -Output $(BUILD_DIR)/tf_fdl/tfboot.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/tf_fdl/tfboot.axf -output $(BUILD_DIR)/tf_fdl/tfboot.bin	
	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/img/tfboot.bin ""
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/tf_fdl/tfboot.bin) $(call Path_process,$(BUILD_DIR)/img/tfboot.bin)
	@"$(MAKE_HOME)/BinToTxt.exe" "$(BUILD_DIR)/tf_fdl/tfboot.bin" "MS_Customize/export/inc/tfboot.h"
endif
endif

ifneq ($(strip $(MAKECMDGOALS)),clean)
-include $(OBJECTS:.o=.d)
endif
%.d %.o:%.s
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(ASM) $(MASMFLAG_OPT) $(AFLAGS) $(VIA) $(BUILD_DIR)/dep/tf_fdl_A_MACRO.txt $(INCPATH) $<  $(MDFLAGS) $(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/tf_fdl/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/tf_fdl/tf_fdl.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""
%.d %.o:%.c
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(CC) $(MCFLAG_OPT) $(CFLAGS) $(VIA) $(BUILD_DIR)/dep/tf_fdl_C_MACRO.txt -c $(MINCPATH) $(INCPATH) $< $(MDFLAGS) $(DEPPATH)/$*.d.tmp -o $(BUILD_DIR)/obj/tf_fdl/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/tf_fdl/tf_fdl.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl $*.d.tmp $(DEPPATH)/

.PHONY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "tf_fdl log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "tf_fdl" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "tf_fdl" $(BUILD_DIR)/dep

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl tf_fdl.bin $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl tf_fdl.bin $(BUILD_DIR)/fdl_bootloader/tf_fdl/
	@-$(PERL) make/perl_script/rm_dir.pl tf_fdl $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl tf_fdl $(BUILD_DIR)/dep
