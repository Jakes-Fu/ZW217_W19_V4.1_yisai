
include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/fota_C_MACRO.txt)
  $(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/fota_A_MACRO.txt)
endif

MCFLAG_OPT = $(CFLAGS)
MASMFLAG_OPT = $(AFLAGS)

MINCPATH =  -Icommon/export/inc  \
            -IMS_Ref/source/fota/inc \
            -Ichip_drv/export/inc  \
            -IRTOS/export/inc \
            -IMS_Ref/export/inc \
            -IMS_Customize/export/inc \
            -IMS_Ref/source/isp_service/inc \
            -IMS_Ref/source/image_proc/inc \
            -IMS_Ref/source/lcd/inc
MINCPATH += -Ichip_drv/export/inc/outdated

VPATH 		= MS_Ref/source/fota/src \
						MS_Ref/source/lcd/src \
						MS_Customize/source/product/config/$(strip $(PRODUCT_CONFIG))  

LIBS       	=	lib/$(strip $(PRODUCT_BASELIB_DIR))/xsr_layer_os_none.a	\
			lib/$(strip $(PRODUCT_BASELIB_DIR))/ftl_os_none.a	\
			lib/$(strip $(PRODUCT_BASELIB_DIR))/sci_ftl_layer_os_none.a



ifeq ($(strip $(STACK_CHECK_SUPPORT)), TRUE)									
	ifeq ($(strip $(CHIP_ENDIAN)), BIG)
		LIBS			+= Third-party/fota/BigEndian/ImageUpdate.a 
	else
		LIBS			+= Third-party/fota/LittleEndian/ImageUpdate.a 
	endif
else
	ifeq ($(strip $(CHIP_ENDIAN)), BIG)
		LIBS			+= Third-party/fota/BigEndian/ImageUpdate_nostack.a 
	else
		LIBS			+= Third-party/fota/LittleEndian/ImageUpdate_nostack.a 
	endif
endif

ifeq ($(strip $(CMMB_SUPPORT)), TRUE)    
ifeq ($(strip $(MTV_MIDDLEWARE_ON_BB)), FALSE)
LIBS			+= 	lib/$(strip $(PROJECT))/cmmb_fota.a
endif
endif 



################################################################
#
#            chip driver object code
################################################################
CHIPDRV_SOURCES				:=  pinmap.c  gpio_phy_v0.c  gpio_hal.c   nlc_drv.c

ifeq ($(strip $(PLATFORM)),	SC8800H)
CHIPDRV_SOURCES       +=         sc8800h_gpio_cfg.c chip_phy_v2.c pwm_phy_v1.c
endif
ifeq ($(strip $(PLATFORM)),	SC8800G)
CHIPDRV_SOURCES       +=         sc8800g_gpio_cfg.c chip_phy_v3.c pwm_phy_v1.c analog_hal.c  analog_phy_v3.c adi_phy_v3.c
endif
CHIPDRV_OBJECTS1 			:= $(patsubst %.c, %.o, $(CHIPDRV_SOURCES))
CHIPDRV_OBJECTS       :=$(addprefix $(BUILD_DIR)/obj/chip_drv/, $(CHIPDRV_OBJECTS1))

################################################################
#
#            customize object code
#################################################################
CUSTOMDRV_SOURCES				:= lcd_backlight.c Lcm_cfg_info.c lcm_prod.c   gpio_cfg.c pinmap_cfg.c  nand_spec.c \
													 prod_cfg.c  gpio_prod.c nand_vendor_sw_select.c   

####################### LCD DRIVER ##############################                      

                       
CUSTOMDRV_OBJECTS1 			:= $(patsubst %.c, %.o, $(CUSTOMDRV_SOURCES))
CUSTOMDRV_OBJECTS       :=$(addprefix $(BUILD_DIR)/obj/custom_drv/, $(CUSTOMDRV_OBJECTS1))
CUSTOMDRV_OBJECTS       += $(wildcard $(BUILD_DIR)/obj/custom_drv/tft_*.o)                        
################################################################
#                      
#            reference driver object code
################################################################
ifeq ($(strip $(PLATFORM)),	SC8800H)
REFDRV_SOURCES				:= 	lcdc_rgb.c lcdc_mcu.c lcdc.c
endif

ifeq ($(strip $(PLATFORM)),	SC8800G)
REFDRV_SOURCES				:= 	lcdc_drv.c lcdc_app.c 
endif
                       
REFDRV_OBJECTS1 			:= $(patsubst %.c, %.o, $(REFDRV_SOURCES))
REFDRV_OBJECTS       :=$(addprefix $(BUILD_DIR)/obj/lcd/, $(REFDRV_OBJECTS1))
                       
################################################################
#                      
#            FOTA source code
################################################################
SOURCES		=       log.c \
                        dummy.c \
                        entry.s \
                        reboot.s \
                        fota_mmu.c  \
                        fota_mmu_asm.s


SOURCES		+=      sprdftl_ua.c \
                        sprdftl_RB_ImageUpdate.c \
                        sprdftl_nv_update.c \
                        sprdftl_master_img_gen.c


ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
ifeq ($(strip $(MTV_MIDDLEWARE_ON_BB)), FALSE)
MCFLAG_OPT		+=	-DPRODUCT_DRIVER_MTV_ENABLE
endif
endif

ifeq ($(strip $(PLATFORM)),	SC8800H)
SOURCES		        += lcd_for_fota.c
endif	
ifeq ($(strip $(PLATFORM)),	SC8800G)
SOURCES		        += lcd_fota.c
endif	


#**************************** PRODUCT ************************

OBJECTS1 			:= $(patsubst %.c, %.o, $(SOURCES))
OBJECTS 			:= $(patsubst %.s, %.o, $(OBJECTS1))
ALLOBJECTS    :=$(addprefix $(BUILD_DIR)/obj/fota/, $(OBJECTS))

vpath
vpath %.o  $(BUILD_DIR)/obj/fota
vpath %.d  $(BUILD_DIR)/dep/fota
DEPPATH	=  $(BUILD_DIR)/dep/fota


all: builddir $(OBJECTS) 
ifeq ($(strip $(COMPILER)),	RVCT)
	@$(LINK) --info totals --entry __vectors --map --symbols --scatter MS_Ref/source/fota/scatter.scf --list $(BUILD_DIR)/fota/omadm_fota.map --info sizes $(LIBS) \
	$(CHIPDRV_OBJECTS) $(CUSTOMDRV_OBJECTS) $(REFDRV_OBJECTS) $(addprefix $(BUILD_DIR)/obj/fota/, $(OBJECTS)) --output $(BUILD_DIR)/fota/omadm_fota.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/fota/omadm_fota.axf --output $(BUILD_DIR)/fota/omadm_fota.bin
       # @$(PERL) ./make/perl_script/makebloader.pl $(BUILD_DIR)/fota/omadm_fota.bin $(BUILD_DIR)/fota/omadm_fota.bin
	@-$(PERL) make/perl_script/rm_file.pl boot_tmp.bin $(BUILD_DIR)/fota/
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/fota/omadm_fota.bin)  $(call Path_process,$(BUILD_DIR)/img/omadm_fota.bin)

else
	$(LINK) -info totals -entry __vectors -map -symbols -scatter MS_Ref/source/fota/scatter.scf -list $(BUILD_DIR)/fota/omadm_fota.map -info sizes $(LIBS) \
	$(CHIPDRV_OBJECTS) $(CUSTOMDRV_OBJECTS) $(REFDRV_OBJECTS) $(addprefix $(BUILD_DIR)/obj/fota/, $(OBJECTS)) -Output $(BUILD_DIR)/fota/omadm_fota.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/fota/omadm_fota.axf -output $(BUILD_DIR)/fota/omadm_fota.bin
       # @$(PERL) ./make/perl_script/makebloader.pl $(BUILD_DIR)/fota/omadm_fota.bin $(BUILD_DIR)/fota/omadm_fota.bin
	@-$(PERL) make/perl_script/rm_file.pl boot_tmp.bin $(BUILD_DIR)/fota/
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/fota/omadm_fota.bin)  $(call Path_process,$(BUILD_DIR)/img/omadm_fota.bin)
endif

ifneq ($(strip $(MAKE_VAR_DEPEND_D)),0)
-include $(OBJECTS:.o=.d)
endif

%.o %.d:%.s
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	@$(ASM) $(MASMFLAG_OPT) $(VIA) $(BUILD_DIR)/dep/fota_A_MACRO.txt $(INCPATH) $< $(MDFLAGS) $(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/fota/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/fota/fota.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl omadm_fota.bin $(BUILD_DIR)/fota/
	@-$(PERL) make/perl_script/rm_file.pl $*.d.tmp $(DEPPATH)/

%.o %.d :%.c
	@$(ECHO)
	$(ECHO) Compiling Source File $<...
	@$(CC) $(MCFLAG_OPT) $(VIA) $(BUILD_DIR)/dep/fota_C_MACRO.txt  -c $(MINCPATH) $(INCPATH) $< $(MDFLAGS) $(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/fota/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/fota/fota.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl $*.d.tmp $(DEPPATH)/

.PHONY:builddir
builddir:
	@-$(PERL) make/perl_script/rm_dir.pl "$(BUILD_DIR)/fota" ""
	@-$(PERL) make/perl_script/rm_file.pl omadm_fota.bin $(BUILD_DIR)/img/
	@$(PERL) make/perl_script/mk_dir.pl "fota log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "fota" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "fota" $(BUILD_DIR)/dep

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl omadm_fota.bin $(BUILD_DIR)/img/ 
	@-$(PERL) make/perl_script/rm_file.pl omadm_fota.bin $(BUILD_DIR)/fota/
	@-$(PERL) make/perl_script/rm_dir.pl fota $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl fota $(BUILD_DIR)/dep
