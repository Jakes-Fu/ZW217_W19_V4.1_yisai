include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/spload_C_MACRO.txt)
  $(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/spload_A_MACRO.txt)
endif

MCFLAG_OPT   	= 
MASMFLAG_OPT    = 

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))
SCF_FILE := $(shell $(PERL) ./make/perl_script/get_filename.pl MS_Customize $(strip $(PLATFORM))_$(PROJECT).scf $(strip $(PLATFORM))_$(strip $(PRODUCT_CONFIG)).scf)

ifeq ($(strip $(PLATFORM)),	SC6800H)
SCATFILENAME 	= spload/src/spload_scatter.scf
endif

MINCPATH  = -Ispload/inc \
            -Ichip_drv/export/inc \
            -Ichip_drv/export/inc/outdated \
            -IRTOS/export/inc \
            -Icommon/export/inc \
            -IMS_Customize/export/inc \
            -IMS_Ref/export/inc
            
SRCPATH	   = spload/src spload/src/lcm spload/src/sensor

SOURCES	   = init.s \
		     spload_main.c \
    		 lcm_cfg_info.c \
             sensor_cfg.c
#lcd driver             
SOURCES	  += tft_HX8347D.c \
             tft_hx8357.c \
             tft_ILI9328.c 
             
#sensor driver             
SOURCES   += sensor_ov7670.c
		
OBJECTS1 	:= $(patsubst %.c, %.o, $(SOURCES))
OBJECTS     := $(patsubst %.s, %.o, $(OBJECTS1))						

vpath
vpath %.o  $(BUILD_DIR)/obj/spload
vpath %.d  $(BUILD_DIR)/dep/spload
vpath %.h  $(subst \,/,$(MINCPATH))
vpath %.c  $(subst \,/,$(SRCPATH))
vpath %.s  $(subst \,/,$(SRCPATH))
DEPPATH	=  $(BUILD_DIR)/dep/spload

all: builddir $(OBJECTS)
	@-$(PERL) spload/src/scatter_gen.pl MS_Customize/$(SCF_FILE)
#	@-$(PERL) spload/src/scatter_gen.pl MS_Customize/$(strip $(PLATFORM))_$(strip $(SRAM_SIZE)).scf

	@$(LINK) -info totals -scatter $(SCATFILENAME) -map -symbols -list spload_$(PROJECT).map -info unused -info sizes $(LIBSXSR) $(LIBS) \
	$(addprefix $(BUILD_DIR)/obj/spload/, $(OBJECTS)) -Output $(BUILD_DIR)/spload/spload.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/spload/spload.axf -output $(BUILD_DIR)/spload/spload.bin
	@-$(PERL) make/perl_script/rm_file.pl spload.bin $(BUILD_DIR)/img/
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/spload/spload.bin) $(call Path_process,$(BUILD_DIR)/img/spload.bin)

ifneq ($(strip $(MAKE_VAR_DEPEND_D)),0)
-include $(OBJECTS:.o=.d)
endif

%.d %.o:%.s
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(ASM) $(MASMFLAG_OPT) $(AFLAGS) $(VIA) $(BUILD_DIR)/dep/spload_A_MACRO.txt $(INCPATH) $<  $(MDFLAGS) ./$(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/spload/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/spload/spload.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""
%.d %.o:%.c
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(CC) $(MCFLAG_OPT) $(CFLAGS) $(VIA) $(BUILD_DIR)/dep/spload_C_MACRO.txt -c $(MINCPATH) $(INCPATH) $<  $(MDFLAGS) $(DEPPATH)/$*.d.tmp -o $(BUILD_DIR)/obj/spload/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/spload/spload.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""

.PHONY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "spload log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "spload" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "spload" $(BUILD_DIR)/dep

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl spload.bin $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl spload.bin $(BUILD_DIR)/spload/
	@-$(PERL) make/perl_script/rm_dir.pl spload $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl spload $(BUILD_DIR)/dep



