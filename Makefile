#
# Pre-defined Make Rules and Modules Specific Compiler Options
#
include Makefile.rules

PROJECT ?= $(sort $(project) $(P) $(p) $(pro) $(PRO))

MODULES ?= $(sort $(modules) $(M) $(m))

BUILD_RES_FLAG ?= $(sort $(buildresource) $(buildres) $(BUILDRES) $(BR) $(br))

BUILD_RES_FLAG ?= $(sort $(BR) $(br))

IP ?=$(ip)

IB ?=$(ib)

JOB ?=$(job)

STOP ?= $(stop)

ifeq ($(strip $(STOP)),)
      STOP=0
endif

ifneq ($(strip $(JOB)),)
JOB_SUM= -j$(JOB)
endif

ifeq ($(strip $(BUILD_RES_FLAG)),)
BUILDRESOURCE = TRUE
endif

ifeq ($(strip $(BUILD_RES_FLAG)),)
BUILDRESOURCE = TRUE
endif

ifneq ($(findstring "new",$(foreach word,$(MAKECMDGOALS),"$(word)")),)
ifneq ($(strip $(MODULES)),)
$(error Command argument is error!!!)
endif
endif

ifneq ($(strip $(JOB)),)
ifneq ($(strip $(IB)),)
$(error JOB and IB can not be used together !!!)
endif
endif

export STOP IB

include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules
include $(strip $(PLATFORM)).modules
ifneq ($(strip $(PID)),)
include Makefile.pid # Please notice the place!!!!
endif

ifeq  ($(strip $(CHIP_ENDIAN)), LITTLE)
BZP_CMD += -L
endif

ifeq  ($(strip $(STONE_IMAGE_SUPPORT)), TRUE)
BZP_CMD += -cat
endif


XML_FILE = UWS6121E
PAC_FILE = pac_UWS6121E.pl




#
# Build Directories Setting

export BUILD_DIR = build/$(PROJECT)_builddir

vpath	%.a $(SPRDLIB_DIR)

#
# Target file definition
#
ifeq ($(strip $(VM_SUPPORT)), TRUE)
ifneq ($(strip $(MEMORY_CUT)), TRUE)
SCF_FILE := $(shell $(PERL) ./make/perl_script/get_filename.pl MS_Customize $(strip $(PLATFORM))_$(PROJECT)_vm.scf $(strip $(PLATFORM))_$(strip $(PRODUCT_CONFIG))_vm.scf)
else
SCF_FILE := $(shell $(PERL) ./make/perl_script/get_filename.pl MS_Customize $(strip $(PLATFORM))_$(PROJECT)_VM_MC.scf $(strip $(PLATFORM))_$(strip $(PRODUCT_CONFIG))_VM_MC.scf)
endif
else
        SCF_FILE := $(shell $(PERL) ./make/perl_script/get_filename.pl MS_Customize $(strip $(PLATFORM))_$(PROJECT).scf $(strip $(PLATFORM))_$(strip $(PRODUCT_CONFIG)).scf)
endif
SCATFILENAME		= MS_Customize/$(SCF_FILE)
AXFFILENAME			= $(strip $(PLATFORM))_$(PROJECT).axf
MAPFILENAME			= $(strip $(PLATFORM))_$(PROJECT).map

IMGFILENAME		= $(strip $(PLATFORM))_$(PROJECT)

INDEPENDENTLIST	=	$(foreach word,$(INDEPENDENT),"$(word)")

ALL_MODULE_DEP =

RESOURCE_COMP =

# NEWMS00229140:add --keep reset_data
ifeq ($(strip $(COMPILER)),	RVCT)
	LFLAGS    += --keep reset_data --feedback $(BUILD_DIR)/$(PROJECT)_feedback.txt  --xref --remove --pad  0x00  --verbose
	ifeq ($(strip $(PLATFORM)),	UWS6121E)
		LFLAGS    += --diag_suppress 6366
	endif
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
ifeq ($(strip $(UMEM_SUPPORT)), TRUE)
UMEM_ERASE = "UMEM_ERASED"
else
UMEM_ERASE = ""
endif
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
ifeq ($(strip $(FOTA_SUPPORT)), NONE)
	FOTA_FILE_PATH = ""
else
	FOTA_FILE_PATH = "$(BUILD_DIR)/img/fota_bootloader.bin"
endif
endif


ifeq ($(strip $(PRELOAD_SUPPORT)), TRUE)
PRELOAD_UDISK = "$(BUILD_DIR)/img/preload_img.bin"
else
PRELOAD_UDISK = ""
endif

#
# Compiled lib file list
#
ifeq ($(strip $(MODULES)),)
	REMAKE_FLAG		=	TRUE
	MKLIBLISTVAR		= $(foreach comp,$(COMPLIBS),$(if $(findstring "$(subst !,,$(comp))",$(INDEPENDENTLIST)),$(subst !,,$(comp)).indep,$(subst !,,$(comp)).a))
	MKLIBLIST_CLEAN		= $(filter %.del,$(foreach comp,$(COMPLIBS),$(if $(findstring "$(subst !,,$(comp))",$(INDEPENDENTLIST)), $(subst !,,$(comp)).del,$(comp).a)))
	MKDEPSLIST		= $(call sort,$(foreach comp,$(COMPLIBS),$(comp).dep))
	MKCOMPLIST		= $(call sort,$(foreach comp,$(COMPLIBS),$(comp)))
	PCLINTLIST		= $(foreach comp,$(COMPLIBS),$(if $(findstring "$(subst !,,$(comp))",$(INDEPENDENTLIST)),,$(subst !,,$(comp)).pclint))
ifneq ($(strip $(BUILDRESOURCE)),TRUE)
	MKLIBLIST =$(filter-out $(RES_MAKE_DIR).indep,$(MKLIBLISTVAR))
	NEW_INDEP_ADD =
else
	MKLIBLIST = $(MKLIBLISTVAR)
endif
else
	REMAKE_FLAG		=	FALSE
	MKLIBLIST		= $(foreach comp,$(MODULES),$(if $(findstring "$(comp)",$(INDEPENDENTLIST)),$(comp).indep,$(comp).a))
	MKLIBLIST_CLEAN         = $(filter %.del,$(foreach comp,$(MODULES),$(if $(findstring "$(comp)",$(INDEPENDENTLIST)),$(comp).del,$(comp).a)))
	MKDEPSLIST		= $(call sort,$(foreach comp,$(MODULES),$(comp).dep))
	MKCOMPLIST		= $(call sort,$(foreach comp,$(MODULES),$(comp)))
	PCLINTLIST		= $(foreach comp,$(MODULES),$(if $(findstring "$(subst !,,$(comp))",$(INDEPENDENTLIST)),,$(subst !,,$(comp)).pclint))
endif

VARIANT_LP_LINUX:=$(foreach variant_lp,$(LP_LIST),$(variant_lp))

ifeq ($(strip $(IB)),2)
IBLIBLIST =  $(patsubst %.a, %.ib2, $(filter %.a, $(MKLIBLIST)))
endif

export OPEN_MODULES := $(foreach word,$(filter-out !%, $(COMPLIBS_DBG)),"$(word)")

ifeq ($(strip $(MAKESHELL)),CMD)
	LINKLIBLIST := $(shell $(PERL) make/perl_script/liblist_process.pl "$(LINKLIBLIST)")
endif

ifeq ($(strip $(VECTOR_TABLE_ADDR)),	HIGH)
ENTRYPOINT=0x400000
else
ENTRYPOINT=0x0
endif

ROOT_DIR	= $(shell $(PERL) ./make/perl_script/get_root_fullpath.pl)

logel_label := $(shell $(PERL) $(call Path_process,make/perl_script/getlabel.pl)  ./version/version_software_mocor.h)


ALLNV	= $(shell $(CAT) $(call Path_process,$(BUILD_DIR)/tmp/NV_GROUP$(if $(strip $(PID_LIST)),_$(strip $(PID_LIST))).txt))

ifeq  ($(strip $(STONE_IMAGE_SUPPORT)),TRUE)
pac = $(PERL) make/perl_script/pac.pl $(strip $(PLATFORM)) $(MMI_RES_DIR_FINAL) $(IMGFILENAME_FINAL)_stone.bin $(logel_label) "." $(PROJECT) "$(strip $(ALLNV))" "$(strip $(VM_SUPPORT))" "$(strip $(PRELOAD_SUPPORT))" "$(strip $(NANDBOOT_SUPPORT))" "$(strip $(DEV_MANAGE_SUPPORT))" "$(strip $(USB_VCOM_AUTORUN_SUPPORT))" "$(strip $(STONE_IMAGE_SUPPORT))" "$(strip $(SPLOAD_ENABLE))" "$(strip $(MEMORY_CUT))" "$(strip $(DUAL_DSP_ADAPTION))" "$(strip $(SIGNED_NAME_PREFIX))" "$(variant_pid)"
else
pac = $(PERL) make/perl_script/pac.pl $(strip $(PLATFORM)) $(MMI_RES_DIR_FINAL) $(IMGFILENAME) $(logel_label) "." $(PROJECT) "$(strip $(ALLNV))" "$(strip $(VM_SUPPORT))" "$(strip $(PRELOAD_SUPPORT))" "$(strip $(NANDBOOT_SUPPORT))" "FALSE" "$(strip $(USB_VCOM_AUTORUN_SUPPORT))" "$(strip $(STONE_IMAGE_SUPPORT))" "$(strip $(SPLOAD_ENABLE))" "$(strip $(MEMORY_CUT))" "$(strip $(DUAL_DSP_ADAPTION))" "$(strip $(SIGNED_NAME_PREFIX))" "$(variant_pid)"
endif

# Notice blank line in define is needed
# pac_build(variant_pid)
define pac_build
$(eval ALLNV = $(shell $(CAT) $(call Path_process,$(BUILD_DIR)/tmp/NV_GROUP$(if $(1),_$(1)).txt)))
$(eval MMI_RES_DIR_FINAL = $(strip $(MMI_RES_DIR))$(if $(1),_$(word 1,$(subst _, ,$(1)))))
$(eval IMGFILENAME_FINAL = $(strip $(IMGFILENAME))$(if $(1),_$(1)))
$(ECHO) "---------------------------------------------------------------------------"
$(ECHO) "PACING FOR IMGFILENAME_FINAL=$(IMGFILENAME_FINAL)"
$(ECHO) "PACING FOR MMI_RES_DIR_FINAL=$(MMI_RES_DIR_FINAL)"
$(ECHO) "PACING FOR ALLNV=$(ALLNV) "
$(ECHO) "---------------------------------------------------------------------------"
$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/ResearchDownload" ""
$(COPY_DIR) /Q  $(call Path_process,tools/DEBUG_TOOL/ResearchDownload/Bin/*)  $(call Path_process,$(BUILD_DIR)/ResearchDownload) 
attrib -R  $(call Path_process,$(BUILD_DIR)/ResearchDownload/ResearchDownload.ini)
$(if $(1),$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img/$(IMGFILENAME)_$(1)" "")
$(if $(1),$(COPY_DIR) /Q $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/*) $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_$(1)))
$(ECHO) "---------------------------------------------------------------------------"
$(ECHO) $(ALLNV)
$(ECHO) "---------------------------------------------------------------------------"
$(ECHO) $(variant_pid)
$(ECHO) "---------------------------------------------------------------------------"
$(call pac)
$(if $(1),$(PERL) make/perl_script/rm_dir.pl "$(BUILD_DIR)/img/$(IMGFILENAME)_$(1)" "")
$(PERL) make/perl_script/rm_dir.pl  "$(BUILD_DIR)/ResearchDownload" ""

endef

pac_loop ?= $(call pac_build)

.DEFAULT_GOAL := update

#
#	Update here
#
.PHONY:update
ifeq ($(REMAKE_FLAG), TRUE)
update: ziplibbins image

else
update:$(MAKE_VAR_ADD_RESOURCE)  $(MKLIBLIST)
endif
	@-$(PERL) make/perl_script/rm_file.pl error.log   $(BUILD_DIR)/tmp/	
ifeq ($(strip $(MODULES)),)
	@$(PERL) make/perl_script/disp_error.pl "$(COMPLIBS)  link $(strip $(RES_MAKE_DIR))" $(BUILD_DIR)/log  $(PROJECT) 2>&1 | $(TEE) -a $(BUILD_DIR)/tmp/error.log
	@$(PERL) make/perl_script/disp_warning.pl "$(COMPLIBS) link" $(BUILD_DIR)/log 2>&1 > $(BUILD_DIR)/tmp/warning.log
else
	@$(PERL) make/perl_script/disp_error.pl "$(MODULES)" $(BUILD_DIR)/log  $(PROJECT)  2>&1 | $(TEE) -a $(BUILD_DIR)/tmp/error.log
	@$(PERL) make/perl_script/disp_warning.pl "$(MODULES)" $(BUILD_DIR)/log 2>&1 > $(BUILD_DIR)/tmp/warning.log
endif

ifneq ($(strip $(MAKECMDGOALS)),image)
ifneq ($(strip $(MAKECMDGOALS)),pac)
ifeq ($(findstring "new",$(foreach word,$(MAKECMDGOALS),"$(word)")),)
image:$(MAKE_VAR_ADD_RESOURCE) $(MKLIBLIST)
endif
endif
endif

ifeq ($(findstring "new",$(foreach word,$(MAKECMDGOALS),"$(word)")),)
$(MAKE_VAR_ADD_RESOURCE) $(MKLIBLIST):builddirs
endif


complibs:
	@$(ECHO)
	@$(ECHO)	project_$(PROJECT).mk
	@$(ECHO)	$(COMPLIBS)
	@$(ECHO)

cleanlibs:
	@-$(PERL) make/perl_script/rm_file.pl "$(MKLIBLIST)" $(BUILD_DIR)/lib/
	@-$(PERL) make/perl_script/rm_file.pl "$(MKLIBLIST)" ./lib/$(PROJECT)/

cleandeps:
ifeq ($(strip $(MODULES)),)
	@$(PERL) make/perl_script/rm_dir.pl "$(COMPLIBS)" $(BUILD_DIR)/dep
	@$(PERL) make/perl_script/rm_file.pl "$(MKDEPSLIST)" $(BUILD_DIR)/dep/
else
	@$(PERL) make/perl_script/rm_dir.pl "$(MODULES)" $(BUILD_DIR)/dep
	@$(PERL) make/perl_script/rm_file.pl "$(MKDEPSLIST)" $(BUILD_DIR)/dep/
endif

%.indep:
	@$(ECHO)
	@$(ECHO) Building library $* \( refer to $(BUILD_DIR)/log/$*.log for detail \), Please Wait ...
	@-$(PERL) make/perl_script/getsecond.pl >./$(BUILD_DIR)/tmp/$*_start.txt
	@$(PERL) make/perl_script/mk_dir.pl "$*" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "$*" $(BUILD_DIR)/dep
	@$(MAKE) PROJECT=$(PROJECT)  -f $(call GETMAKEPATH,$*) -I ./$(BUILD_DIR)/dep/$*  2>&1 | $(TEE) $(BUILD_DIR)/log/$*.log
	@$(ECHO) \n$* library build finished
	@-$(PERL) make/perl_script/getcompiletime.pl $* ./$(BUILD_DIR)/tmp/$*_start.txt  2>&1 | $(TEE) -a $(BUILD_DIR)/log/time_consuming.log
	@$(PERL) make/perl_script/rm_file.pl ./$(BUILD_DIR)/tmp/$*_start.txt ""
	$(PERL) make/perl_script/del_target.pl "$*" "fdl1 fdl2 bootloader nor_fdl resource nv_parameter dmdsp fota wre_preload  preload_app spload" "$(BUILD_DIR)/img/*.pac"

ifeq ($(strip $(IB)),2)
%.a: compile_all_ib2
	@-$(MAKE)  PROJECT=$(PROJECT)  -f Makefile.modules  -I ./$(BUILD_DIR)/dep/$* -I ./$(BUILD_DIR)/dep/$*/cache -r -R COMPONENT=$*  $*.a
else
%.a:
	@$(ECHO)
	@$(ECHO) Building library $* \( refer to $(BUILD_DIR)/log/$*.log for detail \), Please Wait ...
	@-$(PERL) make/perl_script/getsecond.pl >./$(BUILD_DIR)/tmp/$*_start.txt
	@$(PERL) make/perl_script/mk_dir.pl "$*" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "$*" $(BUILD_DIR)/dep
	@$(MAKE) PROJECT=$(PROJECT) -f make/pclint/pclint.mk -k -r -R COMPONENT=$* moduledep
	@$(PERL) make/perl_script/createfile.pl $(BUILD_DIR)/dep/$*.dep
	@$(PERL) make/perl_script/diff.pl $(BUILD_DIR)/dep/$*.dep.tmp $(BUILD_DIR)/dep/$*.dep $(BUILD_DIR)/dep/$*.cmp
	@$(PERL) ./make/perl_script/module_dep_process.pl  $(BUILD_DIR) $*  $(subst \,/,$(MAKE_HOME))
	@$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/dep/$*.cmp ""

ifeq ($(strip $(STOP)),0)
	@$(MAKE)  -s $(JOB_SUM) PROJECT=$(PROJECT)  -f Makefile.modules  -I ./$(BUILD_DIR)/dep/$* -I ./$(BUILD_DIR)/dep/$*/cache -k -r -R COMPONENT=$* 2>&1 | $(TEE)  $(BUILD_DIR)/log/$*.log
else
	@$(MAKE)  $(JOB_SUM) PROJECT=$(PROJECT)  -f Makefile.modules  -I ./$(BUILD_DIR)/dep/$* -r -R COMPONENT=$*
endif
	@$(ECHO) \n$* library build finished
	@-$(PERL) make/perl_script/getcompiletime.pl $* ./$(BUILD_DIR)/tmp/$*_start.txt  2>&1 | $(TEE) -a $(BUILD_DIR)/log/time_consuming.log
	@$(PERL) make/perl_script/rm_file.pl ./$(BUILD_DIR)/tmp/$*_start.txt ""
#	@-$(PERL) make/perl_script/gen_loadrules.pl p=$(PROJECT) m=$* $(if $(strip $(IB)),ib=1,)
endif

%.ib2:  builddirs
	@$(ECHO)
	@$(ECHO) Building ib library $* \( refer to $(BUILD_DIR)/log/$*.log for detail \), Please Wait ...
	@$(PERL) make/perl_script/mk_dir.pl "$*" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "$*" $(BUILD_DIR)/dep
	@$(MAKE)  PROJECT=$(PROJECT)  -f Makefile.modules  -I ./$(BUILD_DIR)/dep/$* -I ./$(BUILD_DIR)/dep/$*/cache -r -R COMPONENT=$*

compile_all_ib2   : $(IBLIBLIST)
	$(PERL) ./make/perl_script/createxml_ib2.pl $(PROJECT) "$(filter %.a, $(MKLIBLIST))"
	xgConsole.exe build\\$(PROJECT)_builddir\\ALL.xml

%.del:
	@$(ECHO)
	@$(ECHO) Clean library $* , Please Wait ...
	@-$(MAKE) PROJECT=$(PROJECT)  -f $(call GETMAKEPATH,$*)  -I ./$(BUILD_DIR)/dep/$* MAKE_VAR_DEPEND_D=0 clean

#
#	New here
#

.PHONY:new
new: ziplibbins image 
ifeq ($(strip $(MODULES)),)
	@$(PERL) make/perl_script/disp_error.pl "$(COMPLIBS) link  $(strip $(RES_MAKE_DIR))" $(BUILD_DIR)/log   $(PROJECT)  2>&1 | $(TEE) -a $(BUILD_DIR)/tmp/error.log
	@$(PERL) make/perl_script/disp_warning.pl "$(COMPLIBS) link" $(BUILD_DIR)/log 2>&1 > $(BUILD_DIR)/tmp/warning.log
else
	@$(PERL) make/perl_script/disp_error.pl "$(MODULES)" $(BUILD_DIR)/log   $(PROJECT)  2>&1 | $(TEE) -a $(BUILD_DIR)/tmp/error.log
	@$(PERL) make/perl_script/disp_warning.pl "$(MODULES)" $(BUILD_DIR)/log 2>&1 > $(BUILD_DIR)/tmp/warning.log
endif


ifneq ($(strip $(MAKECMDGOALS)),image)
ifneq ($(strip $(MAKECMDGOALS)),pac)
ifneq ($(findstring "new",$(foreach word,$(MAKECMDGOALS),"$(word)")),)
image : $(if $(strip $(NEW_INDEP_ADD)),$(foreach comp,$(strip $(NEW_INDEP_ADD)),$(comp).indep),) $(MKLIBLIST)
endif
endif
endif

ifneq ($(findstring "new",$(foreach word,$(MAKECMDGOALS),"$(word)")),)
$(if $(strip $(NEW_INDEP_ADD)),$(foreach comp,$(strip $(NEW_INDEP_ADD)),$(comp).indep),) $(MKLIBLIST) : builddirs
builddirs:cleanall
endif


ifneq ($(strip $(SPRD_CUSTOMER)), TRUE)
fdl1.indep : fdl1_nf.a
fdl2.indep : fdl2_nf.a xsr_layer_os_none.a ftl_os_none.a sci_ftl_layer_os_none.a
bootloader.indep :  xsr_layer_os_none.a ftl_os_none.a sci_ftl_layer_os_none.a
fota.indep :  xsr_layer_os_none.a ftl_os_none.a sci_ftl_layer_os_none.a chip_drv.a custom_drv.a display.a

ifneq ($(strip $(MTV_MIDDLEWARE_ON_BB)), TRUE)
mtv_adapter.a :varm_fw.indep
endif
release_level_d0.indep : rtos_kal.a  dm_2g_ps.a  tdd_nas.a tdd_bcfe.a
else
fota.indep : chip_drv.a custom_drv.a
endif


ifeq ($(strip $(TF_LOAD_SUPPORT)), TRUE)
custom_drv.a : tf_fdl.indep
ifneq ($(strip $(SPRD_CUSTOMER)), TRUE)
ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
tf_fdl.indep : ftl_os_none.a xsr_layer_os_none.a sci_ftl_layer_os_none.a
endif
endif
endif


ifeq ($(strip $(WRE_VIRTUALDISK_SUPPORT)), TRUE)
app.a : wre_preload.indep
endif


ifneq ($(findstring "clean",$(foreach word,$(MAKECMDGOALS),"$(word)")),)
ifneq ($(findstring "update",$(foreach word,$(MAKECMDGOALS),"$(word)")),)
update : clean
endif
endif

ifeq ($(strip $(MIDI_SUPPORT)), TRUE)
  ifeq ($(strip $(SOUNDBANK_MODE)), COMPACT)
    ifeq ($(strip $(BUILDRESOURCE)),TRUE)
     midisoundbank.a : $(strip $(RES_MAKE_DIR)).indep
     endif
  endif
endif

.PHONY:cmd_arg_check
cmd_arg_check:
ifeq ($(CMD_ARG_VALID), FALSE)
	@$(ECHO) Wrong command!
	@$(ECHO) Example": make PROJECT=projectname"
	@$(ECHO) Typing"   make help  " for more helpful information.
	@exit 1
else
	@$(PERL) make/perl_script/exist_file.pl project_$(PROJECT).mk
endif


.PHONY:buildintro
buildintro:
ifeq ($(strip $(MAKE_OS)), Linux)
	@echo Compiling in Linux
	@chmod -R +x make/make_cmd/
	@chmod -R +x make/perl_script
	@chmod -R +x tools/
endif
	@$(ECHO) Start building project image \(using Makefile project_$(PROJECT).mk\)

.PHONY:cleanall
cleanall: cmd_arg_check $(MKLIBLIST_CLEAN)
	@$(ECHO) Cleanup all librarys ,objects and denpends, please wait...
	@$(PERL) make/perl_script/rm_dir.pl "$(BUILD_DIR)/img $(BUILD_DIR)/obj $(BUILD_DIR)/log $(BUILD_DIR)/dep $(BUILD_DIR)/pclint " ""
	@$(PERL) make/perl_script/rm_dir.pl "$(BUILD_DIR)/tmp $(BUILD_DIR)/res $(BUILD_DIR)/nv  $(BUILD_DIR)/fdl_bootloader $(BUILD_DIR)/lib " ""
ifneq ($(strip $(SPRD_CUSTOMER)),TRUE)
	@$(PERL) make/perl_script/rm_dir.pl "./lib/$(PROJECT)" ""
endif


.PHONY:ziplibbins
ziplibbins:
ifeq ($(strip $(MAKE_OS)), Linux)
	@echo Compiling in Linux
	@chmod -R +x make/make_cmd/
	@chmod -R +x make/perl_script
	@chmod -R +x BASE/
	@chmod -R +x tools/
endif
ifeq ($(REMAKE_FLAG), TRUE)
ifeq ($(strip $(USE_LZMA_UZIP_BIN)),TRUE)
	-$(LZMABIN) lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/cp.bin  65536
	-$(LZMABIN) lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/riscV.bin  65536
else
	@$(ECHO) USE_LZMA_UZIP_BIN set is not TRUE
endif
endif


.PHONY:strongscan
strongscan:
	@ "make/py_script/trace_scan.exe"   $(PROJECT)_builddir strong
.PHONY:tracescan_wt
tracescan_wt:
	@ "make/py_script/trace_scan.exe"   $(PROJECT)_builddir

.PHONY:builddirs
builddirs:

ifeq ($(CMD_ARG_VALID), FALSE)
	@$(ECHO) Wrong command!
	@$(ECHO) Example": make PROJECT=projectname"
	@$(ECHO) Typing"   make help  " for more helpful information.
	@exit 1
else
	@$(PERL) make/perl_script/exist_file.pl project_$(PROJECT).mk
endif
	@$(ECHO)
	@$(ECHO)	project_$(PROJECT).mk
	@$(ECHO)	$(COMPLIBS)
	@$(ECHO)
	@$(ECHO) Setup Directories For LIBs, OBJs LOGs and temp files
	@$(PERL) make/perl_script/mk_dir.pl build ./
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)" ""
ifeq ($(strip $(FOTA_SUPPORT)), ADUPS)
	@$(PERL)  make/perl_script/adups_gettime.pl "$(PROJECT)"
endif
ifneq ($(strip $(SPRD_CUSTOMER)),TRUE)
	@$(PERL) make/perl_script/mk_dir.pl "./lib/$(strip $(PRODUCT_BASELIB_DIR))" ""
endif
ifeq ($(strip $(PLATFORM)),	UWS6121E)
	@$(PERL) make/perl_script/mk_dir.pl "./lib/ap/$(PROJECT)" ""
	@$(PERL) make/perl_script/mk_dir.pl "./lib/modem/$(MODEM_BIN_FILE)/$(strip $(MODEM_PROJECT_FOLDER))" ""
endif
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img $(BUILD_DIR)/lib $(BUILD_DIR)/obj $(BUILD_DIR)/log" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/win $(BUILD_DIR)/dep $(BUILD_DIR)/tmp" ""
ifeq ($(strip $(MODULES)),)
ifneq ($(strip $(MAKECMDGOALS)),image)
ifneq ($(strip $(MAKECMDGOALS)),pac)
	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/tmp/$(PROJECT)_loadrule.txt ./
	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/tmp/$(PROJECT)_filelist.txt ./
endif
endif
endif
	@-$(PERL) make/perl_script/getsecond.pl >./$(BUILD_DIR)/tmp/all_start.txt
	@$(PERL)  make/perl_script/gettime_tofile.pl $(BUILD_DIR)/tmp/buildtime.h
ifeq ($(strip $(MAKE_OS)), Linux)
	@$(ECHO)  '#define _MOCOR_SW_PROJECT_ "$(PROJECT)"' > $(BUILD_DIR)/tmp/version_software_project.h
else
	@$(ECHO)  #define _MOCOR_SW_PROJECT_ \"$(PROJECT)\"> $(BUILD_DIR)/tmp/version_software_project.h
endif

ifneq ($(strip $(MODEM_PLATFORM)), TRUE)
	@$(PERL) make/perl_script/create3dtable.pl $(SPECIAL_EFFECT_ICON_SIZE) $(MAINLCD_SIZE) "$(ROOT_DIR)/build/$(PROJECT)_builddir/tmp"
	@$(PERL) make/perl_script/diff.pl  build/$(PROJECT)_builddir/tmp/ui_special_effect_table.c  build/$(PROJECT)_builddir/tmp/$(PROJECT)_ui_special_effect_table.c build/$(PROJECT)_builddir/tmp/$(PROJECT)_ui_special_effect_table.c.cmp
	@$(PERL) make/perl_script/file_diff_process.pl build/$(PROJECT)_builddir/tmp/$(PROJECT)_ui_special_effect_table.c build/$(PROJECT)_builddir/tmp/ui_special_effect_table.c  build/$(PROJECT)_builddir/tmp/$(PROJECT)_ui_special_effect_table.c.cmp
endif

ifneq ($(strip $(SPRD_CUSTOMER)), TRUE)
	@$(ECHO) $(SPRD_MACRO) > lib/$(PROJECT)/sprd_macro.txt
else
ifneq ($(strip $(MODEM_PLATFORM)), TRUE)
ifneq ($(strip $(RELEASE_LEVEL_D0)),	TRUE)
	@$(ECHO) $(SPRD_MACRO) > $(BUILD_DIR)/tmp/sprd_macro.tmp
	@$(PERL) ./make/perl_script/sprd_macro_check.pl $(BUILD_DIR)/tmp/sprd_macro.tmp lib/$(strip $(PRODUCT_BASELIB_DIR))/sprd_macro.txt
endif
endif
endif

ifeq ($(strip $(GPS_SUPPORT)), TRUE)
ifeq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
	@$(ECHO) Gps bin file handle.....
	@-$(PERL) make/perl_script/ParseGpsBin.pl MS_Customize/source/product/driver/gps/green_eye2/ge2_download build/$(PROJECT)_builddir/tmp
endif
endif

ifeq ($(strip $(MAKECMDGOALS)),pac)
ifeq ($(strip $(PLATFORM)),	UWS6121E)
	@-$(COPY_DIR) /Q  $(call Path_process,lib/ap/$(strip $(PROJECT))/*)  $(call Path_process,$(BUILD_DIR)/img/)
endif
endif

ifeq ($(strip $(MAKECMDGOALS)),image)
ifeq ($(strip $(PLATFORM)),	UWS6121E)
	@-$(COPY_DIR) /Q  $(call Path_process,lib/ap/$(strip $(PROJECT))/*)  $(call Path_process,$(BUILD_DIR)/img/)
endif
endif

#	@-$(PERL) make/perl_script/gen_correct_pathnames.pl $(PROJECT)

ifneq ($(strip $(BUILDRESOURCE)),TRUE)
ifneq ($(strip $(VARIANT_LP_LINUX)),)
	@$(COPY_FILE) $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/$(strip $(MMI_RES_DIR))_$(VARIANT_LP_LINUX).bin) $(call Path_process,$(BUILD_DIR)/img/)
else
	@$(COPY_FILE) $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/$(strip $(MMI_RES_DIR)).bin) $(call Path_process,$(BUILD_DIR)/img/)
endif
ifeq ($(strip $(MIDI_SUPPORT)), TRUE)
   ifeq ($(strip $(SOUNDBANK_MODE)),COMPACT)
	$(COPY_FILE)  $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/midi_soundbank_compact.c)  $(call Path_process,$(BUILD_DIR)/tmp/)
   endif
endif
endif

ifneq ($(strip $(BUILDRESOURCE)),TRUE)
	@$(COPY_FILE) $(call Path_process,$(MMI_DIR)/source/resource/bin/$(strip $(PROJECT))/$(strip $(MMI_RES_DIR)).bin) $(call Path_process,$(BUILD_DIR)/img/)
endif

#	$(DEB)-$(PERL) make/perl_script/gen_correct_pathnames.pl $(PROJECT)
#
# Clean All Libs and Objs
#
.PHONY:clean
clean: cmd_arg_check cleanlibs cleandeps $(MKLIBLIST_CLEAN)
	@$(PERL) make/perl_script/rm_dir.pl "$(MKCOMPLIST)" $(BUILD_DIR)/obj
#
# Link and generate ginal image
#
.PHONY:image pac
image pac :ziplibbins builddirs
	@$(ECHO) $(wordlist 1, 20,$(LINKLIBLIST)) >$(BUILD_DIR)/tmp/link_lib.txt
	@$(ECHO) $(wordlist 21, 40,$(LINKLIBLIST)) >>$(BUILD_DIR)/tmp/link_lib.txt
	@$(ECHO) $(wordlist 41, $(words $(LINKLIBLIST)),$(LINKLIBLIST)) >>$(BUILD_DIR)/tmp/link_lib.txt

	@-$(PERL) make/perl_script/rm_file.pl $(AXFFILENAME) $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl $(MAPFILENAME) $(BUILD_DIR)/img/
ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
	@-$(PERL) make/perl_script/files_proc.pl  $(BUILD_DIR)/img/$(IMGFILENAME)/  del
else
	@-$(PERL) make/perl_script/rm_file.pl $(IMGFILENAME) $(BUILD_DIR)/img/
endif
ifneq ($(strip $(IB)),2)
	@-$(PERL) make/perl_script/rm_file.pl buildtime_ver.h ./version/
	@-$(PERL) make/perl_script/rm_file.pl version.a  $(BUILD_DIR)/lib/
	@-$(PERL) make/perl_script/rm_file.pl *.o  $(BUILD_DIR)/obj/version/
	@-$(PERL) make/perl_script/rm_file.pl *.d  $(BUILD_DIR)/dep/version/
	@$(PERL) make/perl_script/mk_dir.pl "version" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "version" $(BUILD_DIR)/dep
	@$(MAKE) PROJECT=$(PROJECT) -f $(call Path_process,./make/pclint/pclint.mk) -k -r -R COMPONENT=version moduledep
	@$(PERL) make/perl_script/createfile.pl $(BUILD_DIR)/dep/version.dep
	@$(PERL) make/perl_script/diff.pl $(BUILD_DIR)/dep/version.dep.tmp $(BUILD_DIR)/dep/version.dep $(BUILD_DIR)/dep/version.cmp
	@$(PERL) ./make/perl_script/module_dep_process.pl  $(BUILD_DIR) version  $(subst \,/,$(MAKE_HOME))
	@$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/dep/version.cmp ""
	@$(MAKE)  PROJECT=$(PROJECT)  -f Makefile.modules  -I ./$(BUILD_DIR)/dep/version -k -r -R COMPONENT=version 2>&1 | $(TEE) -a $(BUILD_DIR)/log/version.log
endif

	@$(ECHO) Linking Target Image File, Please Wait...
ifeq ($(strip $(COMPILER)),	RVCT)
	-$(COPY_FILE) $(call Path_process,$(SCATFILENAME)  $(BUILD_DIR)/tmp/$(SCF_FILE))

ifeq ($(strip $(PLATFORM)),	UWS6121E)
	-$(COPY_FILE) $(call Path_process, MS_Customize/$(strip $(PLATFORM))_normal.scf  $(BUILD_DIR)/tmp/$(strip $(PLATFORM))_normal.scf)

endif


	$(PERL) make/perl_script/macroconv.pl  $(call Path_process, project_$(PROJECT).mk)   $(BUILD_DIR)/tmp/$(strip $(PLATFORM)).def
	-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/tmp/mem_cfg.h ""
	-$(PERL) make/perl_script/remove_c_comments.pl  MS_Customize/source/product/config/$(strip $(PRODUCT_CONFIG))/mem_cfg.h $(BUILD_DIR)/tmp/mem_cfg.h
	-$(ARMCC)  -E -C  $(BUILD_DIR)/tmp/$(SCF_FILE)  >  $(BUILD_DIR)/tmp/link_$(SCF_FILE)


ifeq  ($(strip $(BZP_SUPPORT)),	TRUE)
	@-$(PERL) make/perl_script/rm_dir.pl $(IMGFILENAME)  $(BUILD_DIR)/img/
	-$(LINK) --entry __vectors --map --symbols --scatter $(BUILD_DIR)/tmp/link_$(SCF_FILE) $(LFLAGS) --list $(MAPFILENAME) --info sizes --via $(BUILD_DIR)/tmp/link_lib.txt -o $(BUILD_DIR)/img/$(AXFFILENAME) 2>&1 | $(TEE) $(BUILD_DIR)/log/link.log && $(FROMELF) -c --bin $(BUILD_DIR)/img/$(AXFFILENAME) --output $(BUILD_DIR)/img/$(IMGFILENAME) 2>&1 | $(TEE) $(BUILD_DIR)/log/fromelf.log
else

	@-$(PERL) make/perl_script/rm_file.pl $(IMGFILENAME)  $(BUILD_DIR)/img/
	-$(LINK) --entry __vectors --map --symbols --scatter $(BUILD_DIR)/tmp/link_$(SCF_FILE) $(LFLAGS) --list $(MAPFILENAME) --info sizes --via $(BUILD_DIR)/tmp/link_lib.txt -o $(BUILD_DIR)/img/$(AXFFILENAME) 2>&1 | $(TEE) $(BUILD_DIR)/log/link.log && $(FROMELF) -c --bin $(BUILD_DIR)/img/$(AXFFILENAME) --output $(BUILD_DIR)/img/$(IMGFILENAME) 2>&1 | $(TEE) $(BUILD_DIR)/log/fromelf.log

endif
else ##ifneq ($(strip $(COMPILER)),	RVCT)
	$(LINK) -entry __vectors -map -symbols -scatter $(SCATFILENAME) $(LFLAGS) -list $(MAPFILENAME) -info sizes -via $(BUILD_DIR)/tmp/link_lib.txt -o $(BUILD_DIR)/img/$(AXFFILENAME) 2>&1 | $(TEE) $(BUILD_DIR)/log/link.log && $(FROMELF) -c -bin $(BUILD_DIR)/img/$(AXFFILENAME) -output $(BUILD_DIR)/img/$(IMGFILENAME) 2>&1 | $(TEE) $(BUILD_DIR)/log/fromelf.log
endif ##ifeq ($(strip $(COMPILER)),	RVCT)


ifeq ($(strip $(BZP_SUPPORT)),	TRUE)
ifeq ($(strip $(MODEM_PLATFORM)), TRUE)
		-$(BZP) -L -cat2 -level 5 -usr $(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_USER_IMAGE  -usrpacsize 4096  -usrcmp b  -out $(BUILD_DIR)/img/resource_bzp.bin
		-$(BZP) $(BZP_CMD) -cat -ps $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_KERNEL_IMAGE) -res  $(call Path_process,$(BUILD_DIR)/img/resource_bzp.bin)  -out  $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME).bin)
		-$(PERL) make/perl_script/rm_dir.pl $(IMGFILENAME)  $(BUILD_DIR)/img/
		-$(PERL) make/perl_script/rm_file.pl  resource_bzp.bin  $(BUILD_DIR)/img/
else
ifeq ($(strip $(BZP_NO_RESOURCE)), TRUE)
	-$(BZP) -L -cat2 -level 5 -usr $(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_USER_IMAGE  -usrpacsize 4096  -usrcmp b  -out $(BUILD_DIR)/img/resource_bzp.bin
else
	$(PERL) make/perl_script/exist_file.pl $(call Path_process,$(BUILD_DIR)/img/$(strip $(MMI_RES_DIR))$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin)  >> $(BUILD_DIR)/tmp/error.log
	-$(BZP) -L -cat2 -level 5 -usr $(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_USER_IMAGE  -usrpacsize 4096  -usrcmp b  -res $(BUILD_DIR)/img/$(strip $(MMI_RES_DIR))$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin -respacsize 4096 -rescmp b -out $(BUILD_DIR)/img/resource_bzp.bin
endif
endif
endif ##ifneq ($(strip $(BZP_SUPPORT)),	TRUE)

ifeq ($(strip $(PLATFORM)),	UWS6121E)
	-${MAKEFDL1IMAGE} mksimage --bin --load 0x50800000 --entry 0x50800100 $(call Path_process,$(BUILD_DIR)/img/nor_fdl1.bin) $(call Path_process,$(BUILD_DIR)/img/nor_fdl1.img)
	-${MAKEFDL1IMAGE} mksimage --bin --load 0x80100500 --entry 0x80100600 $(call Path_process,$(BUILD_DIR)/img/nor_fdl.bin) $(call Path_process,$(BUILD_DIR)/img/nor_fdl.img)
endif

ifeq  ($(strip $(STONE_IMAGE_SUPPORT)), TRUE)
ifeq ($(strip $(BZP_SUPPORT)),	TRUE)
ifeq ($(strip $(MULTI_BIN_SUPPORT)),	TRUE)
	-$(BZP) $(BZP_CMD) -level 5 -cmp b  -ps $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_KERNEL_IMAGE)  -kernz  $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_KERNEL_IMAGE1)  -res  $(call Path_process,$(BUILD_DIR)/img/resource_bzp.bin) -out  $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_stone.bin)
else

	@$(PERL) make/perl_script/exist_file.pl $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_KERNEL_IMAGE)  >> $(BUILD_DIR)/tmp/error.log
	@$(PERL) make/perl_script/exist_file.pl $(call Path_process,$(BUILD_DIR)/img/resource_bzp.bin)  >>  $(BUILD_DIR)/tmp/error.log

	-$(BZP) $(BZP_CMD)  \
	-ps $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_KERNEL_IMAGE) \
	-res  $(call Path_process,$(BUILD_DIR)/img/resource_bzp.bin) \
	-out  $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin)

## -res is used for BZP OK, when unzip isn't used
	-$(BZP) $(BZP_CMD)  \
	-ps $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_CP_IMAGE_DFILE) \
	-res  $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/memory_index_list.bin) \
	-ext0 $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/memory_index_list.bin) \
	-ext1 $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/$(strip $(CPIMG))) \
	-ext2 $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/$(strip $(RISCV))) \
	-out  $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_cp.bin)

endif ##ifeq ($(strip $(MULTI_BIN_SUPPORT)),TRUE)

ifneq ($(strip $(PLATFORM)), UWS6121E)
	@-$(PERL) make/perl_script/rm_dir.pl $(IMGFILENAME)  $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl  resource_bzp.bin  $(BUILD_DIR)/img/
endif

else ## ifneq ($(strip $(BZP_SUPPORT)),	TRUE)

	@$(PERL) make/perl_script/exist_file.pl $(call Path_process,$(BUILD_DIR)/img/$(strip $(MMI_RES_DIR))$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin)   >> $(BUILD_DIR)/tmp/error.log

##	Creating stone bin and cp bin
	@$(PERL) make/perl_script/exist_file.pl $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_KERNEL_IMAGE)   >> $(BUILD_DIR)/tmp/error.log
	-$(BZP) $(BZP_CMD)  \
	-ps $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_KERNEL_IMAGE)  \
	-res  $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/memory_index_list.bin) \
	-out  $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin)

## -res is used for BZP OK, when unzip isn't used
	-$(BZP) $(BZP_CMD)  \
	-ps $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)/EXEC_CP_IMAGE_DFILE) \
	-res  $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/memory_index_list.bin) \
	-ext0 $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/memory_index_list.bin) \
	-ext1 $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/$(strip $(CPIMG))) \
	-ext2 $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/$(strip $(RISCV))) \
	-out  $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_cp.bin)

endif ## ifeq ($(strip $(BZP_SUPPORT)),	TRUE)
endif ## ifeq  ($(strip $(STONE_IMAGE_SUPPORT)), TRUE)

ifneq ($(strip $(IP)),)
	@-$(PERL) make/perl_script/send_message.pl $(BUILD_DIR)/log/link.log $(IP) $(PROJECT)
	@-$(DEL_FILE) $(call Path_process,$(BUILD_DIR)/log/link.error)
endif

	@-$(PERL) make/perl_script/all_loadrule.pl $(PROJECT) $(foreach comp,$(COMPLIBS),$(if $(findstring "$(subst !,,$(comp))",$(INDEPENDENTLIST)),,$(subst !,,$(comp))))

ifeq ($(strip $(PLATFORM)),	UWS6121E)
	-$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/tmp/NV_GROUP$(if $(strip $(PID_LIST)),_$(strip $(PID_LIST))).txt)   $(call Path_process,$(BUILD_DIR)/img/NV_GROUP$(if $(strip $(PID_LIST)),_$(strip $(PID_LIST))).txt)
endif

##apn make
ifeq ($(strip $(FLASH_SIZE)),128MBITX64MBIT_OLD)
	@-$(PERL) make/perl_script/apngen.pl ./MS_MMI_Main/source/resource/Common/DATA/R_NETWORK_ACCOUNT.ntac 12345678  $(call Path_process,$(BUILD_DIR)/img/apn.bin)
endif


    
	@${ECHO} "ALLNV=${ALLNV}"
#	@-$(call pac_loop)


ifeq  ($(strip $(SECURE_BOOT_SUPPORT)), TRUE)
	@-$(PERL) make/perl_script/rm_file.pl *_signed.pac $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl *_signed.img $(BUILD_DIR)/img/
ifeq  ($(strip $(SECURE_BOOT_MAKE_UNSIGNED_PACK)), TRUE)
	@-$(PERL) make/perl_script/rm_file.pl $(PROJECT)$(if $(strip $(PID_LIST)),_$(strip $(PID_LIST))).pac $(BUILD_DIR)/img/
endif
else
	@-$(PERL) make/perl_script/rm_file.pl $(PROJECT)$(if $(strip $(PID_LIST)),_$(strip $(PID_LIST))).pac $(BUILD_DIR)/img/
endif
	@-$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/ResearchDownload" ""
	@-$(COPY_DIR)   $(call Path_process,tools/DEBUG_TOOL/ResearchDownload/Bin/*)  $(call Path_process,$(BUILD_DIR)/ResearchDownload/)
	@-$(ADD_WRITE_PROP)  $(call Path_process,$(BUILD_DIR)/ResearchDownload/ResearchDownload.ini)
    
ifeq  ($(strip $(STONE_IMAGE_SUPPORT)), TRUE)
ifeq  ($(strip $(NOR_BOOTLOADER_SUPPORT)), TRUE)
#	-${MAKEIMAGE} -A arm -T standalone -C none -a 0x800100 -e 0x800100 -n nor_bootloader.img  -d $(call Path_process,$(BUILD_DIR)/img/nor_bootloader.bin) $(call Path_process,$(BUILD_DIR)/img/nor_bootloader.img)
	-${MAKEFDL1IMAGE} mksimage --bin --load 0x50800000 --entry 0x50800100 $(call Path_process,$(BUILD_DIR)/img/nor_bootloader.bin) $(call Path_process,$(BUILD_DIR)/img/nor_bootloader.img)


	@$(PERL) make/perl_script/exist_file.pl $(BUILD_DIR)/img/nor_fdl1.img  >> $(BUILD_DIR)/tmp/error.log
	@$(PERL) make/perl_script/exist_file.pl $(BUILD_DIR)/img/nor_fdl.bin  >> $(BUILD_DIR)/tmp/error.log
	@$(PERL) make/perl_script/exist_file.pl $(BUILD_DIR)/img/nor_bootloader.img  >> $(BUILD_DIR)/tmp/error.log
	@$(PERL) make/perl_script/exist_file.pl $(BUILD_DIR)/img/$(ALLNV).bin  >> $(BUILD_DIR)/tmp/error.log 
	@$(PERL) make/perl_script/exist_file.pl $(BUILD_DIR)/img/$(strip $(MMI_RES_DIR))$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin  >> $(BUILD_DIR)/tmp/error.log

ifeq  ($(strip $(SECURE_BOOT_SUPPORT)), TRUE)
ifneq ($(strip $(SECURE_BOOT_MAKE_UNSIGNED_PACK)), TRUE)
	-${MAKEFDL1IMAGE} mksimage --bin --load 0x60010000 --entry 0x60010100 $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin) $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).img)
	-${MAKEFDL1IMAGE} ssign ./tools/DEBUG_TOOL/key/2048prikey ./tools/DEBUG_TOOL/key/2048pubkey 1 $(call Path_process,$(BUILD_DIR)/img/nor_fdl1.img) $(call Path_process,$(BUILD_DIR)/img/nor_fdl1_signed.img)
	-${MAKEFDL1IMAGE} ssign ./tools/DEBUG_TOOL/key/2048prikey ./tools/DEBUG_TOOL/key/2048pubkey 1 $(call Path_process,$(BUILD_DIR)/img/nor_fdl.img) $(call Path_process,$(BUILD_DIR)/img/nor_fdl_signed.img)
	-${MAKEFDL1IMAGE} ssign ./tools/DEBUG_TOOL/key/2048prikey ./tools/DEBUG_TOOL/key/2048pubkey 1 $(call Path_process,$(BUILD_DIR)/img/nor_bootloader.img) $(call Path_process,$(BUILD_DIR)/img/nor_bootloader_signed.img)
	-${MAKEFDL1IMAGE} ssign ./tools/DEBUG_TOOL/key/2048prikey ./tools/DEBUG_TOOL/key/2048pubkey 1 $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).img) $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP)))_signed.img) 
endif
else
#	-${VLRSIGN} -pw 12345678 -pn testnosign  -ha SHA1-32 -ipbk true -pw2 12345678 -pn2 testnosign -img $(call Path_process,$(BUILD_DIR)/img/nor_fdl.bin) -out $(call Path_process,$(BUILD_DIR)/img/nor_fdl.bin)
	-${MAKEFDL1IMAGE} mksimage --bin --load 0x60010000 --entry 0x60010100 $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin) $(call Path_process,$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).img)
endif #($(strip $(SECURE_BOOT_SUPPORT)), TRUE)

ifneq ($(wildcard lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER))/*.bin),"")
	$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img/$(strip $(MODEM_BIN_FILE))"
	$(COPY_DIR)  $(call Path_process,lib/modem/$(strip $(MODEM_BIN_FILE))/$(strip $(MODEM_PROJECT_FOLDER)))  $(call Path_process,$(BUILD_DIR)/img/$(strip $(MODEM_BIN_FILE)))
	@$(ECHO) COPY $(strip $(MODEM_BIN_FILE)) to $(BUILD_DIR)/img/ done
else
	@$(ECHO)  $(strip $(MODEM_BIN_FILE)) not exist
endif

ifeq  ($(strip $(SECURE_BOOT_SUPPORT)), TRUE)

ifneq ($(strip $(SECURE_BOOT_MAKE_UNSIGNED_PACK)), TRUE)
	@${ECHO} "MULTI_FLASH_SUPPORT=${strip ${MULTI_FLASH_SUPPORT}}-1"
	@-$(PERL) make/perl_script/$(PAC_FILE) \
	$(BUILD_DIR)/img/$(PROJECT)$(if $(strip $(PID_LIST)),_$(strip $(PID_LIST)))_singed.pac \
	NOR_FLASH_$(strip $(PLATFORM)) \
	$(logel_label) \
	make/perl_script/$(strip $(XML_FILE)).xml \
	$(BUILD_DIR)/img/nor_fdl1_signed.img \
	$(BUILD_DIR)/img/nor_fdl_signed.img \
	$(BUILD_DIR)/img/$(ALLNV).bin \
	$(BUILD_DIR)/img/nor_bootloader_signed.img \
	$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP)))_signed.img \
	$(BUILD_DIR)/img/$(strip $(MMI_RES_DIR))$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin \
	$(BUILD_DIR)/img/$(IMGFILENAME)_cp.bin  \
	$(BUILD_DIR)/img/$(strip $(PROJECT))_delta_nv.bin \
	$(PRELOAD_UDISK) \
	$(UMEM_ERASE) \
	$(FOTA_FILE_PATH)
	@$(PERL) make/perl_script/exist_file.pl $(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP)))_signed.bin  >> $(BUILD_DIR)/tmp/error.log
else
	@${ECHO} "MULTI_FLASH_SUPPORT=${strip ${MULTI_FLASH_SUPPORT}}-5"
	@-$(PERL) make/perl_script/$(PAC_FILE) \
	$(BUILD_DIR)/img/$(PROJECT)$(if $(strip $(PID_LIST)),_$(strip $(PID_LIST))).pac \
	NOR_FLASH_$(strip $(PLATFORM)) \
	$(logel_label) \
	make/perl_script/$(strip $(XML_FILE)).xml \
	$(BUILD_DIR)/img/nor_fdl1.img \
	$(BUILD_DIR)/img/nor_fdl.bin \
	$(BUILD_DIR)/img/$(ALLNV).bin \
	$(BUILD_DIR)/img/nor_bootloader.img \
	$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin \
	$(BUILD_DIR)/img/$(strip $(MMI_RES_DIR))$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin \
	$(BUILD_DIR)/img/$(IMGFILENAME)_cp.bin  \
	$(BUILD_DIR)/img/$(strip $(PROJECT))_delta_nv.bin \
	$(PRELOAD_UDISK) \
	$(UMEM_ERASE) \
	$(FOTA_FILE_PATH)
endif

else ##neq  ($(strip $(SECURE_BOOT_SUPPORT)), TRUE)
## Creating pac here
	@${ECHO} "MULTI_FLASH_SUPPORT=${strip ${MULTI_FLASH_SUPPORT}}-3"
	@-$(PERL) make/perl_script/$(PAC_FILE) \
	$(BUILD_DIR)/img/$(PROJECT)$(if $(strip $(PID_LIST)),_$(strip $(PID_LIST))).pac \
	NOR_FLASH_$(strip $(PLATFORM)) \
	$(logel_label) \
	make/perl_script/$(strip $(XML_FILE)).xml \
	$(BUILD_DIR)/img/nor_fdl1.img \
	$(BUILD_DIR)/img/nor_fdl.img \
	$(BUILD_DIR)/img/$(ALLNV).bin \
	$(BUILD_DIR)/img/nor_bootloader.img \
	$(BUILD_DIR)/img/$(IMGFILENAME)_stone$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).img \
	$(BUILD_DIR)/img/$(strip $(MMI_RES_DIR))$(if $(strip $(PID_LP)),_$(strip $(PID_LP))).bin \
	$(BUILD_DIR)/img/$(IMGFILENAME)_cp.bin  \
	$(BUILD_DIR)/img/$(strip $(PROJECT))_delta_nv.bin \
	$(PRELOAD_UDISK) \
	$(UMEM_ERASE) \
	$(FOTA_FILE_PATH)

endif #($(strip $(SECURE_BOOT_SUPPORT)), TRUE)

else
	$(error NOR_BOOTLOADER_SUPPORT can not set to FALSE !!!)
endif


else ## ifneq  ($(strip $(STONE_IMAGE_SUPPORT)), TRUE)
	$(error STONE_IMAGE_SUPPORT can not set to FALSE !!!)
endif ##ifeq  ($(strip $(STONE_IMAGE_SUPPORT)), TRUE)
	@-$(PERL) make/perl_script/rm_dir.pl  "$(BUILD_DIR)/ResearchDownload" ""

ifneq ($(strip $(SPRD_CUSTOMER)), TRUE)
ifeq  ($(strip $(WRE_SUPPORT)), TRUE)
#	@$(ECHO) .
#	@$(ECHO) .
#	@$(ECHO) Generate WRE Kernel...
#	@$(ECHO) .
#	-@cmd /c makewre.bat $(PROJECT)
endif
endif

	@-$(PERL) make/perl_script/getcompiletime.pl "All" ./$(BUILD_DIR)/tmp/all_start.txt  2>&1 | $(TEE) -a $(BUILD_DIR)/log/time_consuming.log

ifeq ($(strip $(FOTA_SUPPORT)), ABUP)
	@$(ECHO) --------ABUP BUILD VERNO START---------

	@$(PERL) make/perl_script/abup.pl "$(PROJECT)_builddir" "" "$(IMGFILENAME).bin" "$(PROJECT).pac" "$(strip $(PROJECT))"

	@$(ECHO) --------ABUP BUILD VERNO END---------
endif


.PHONY:pclint
pclint: $(PCLINTLIST)
	$(PERL) ./make/perl_script/module_pclintresult_merge.pl ./build/$(PROJECT)_builddir/pclint  "$(PCLINT_DIR)"
	@-$(PERL) ./make/perl_script/stat_pclint.pl ./build/$(PROJECT)_builddir/pclint
	@$(ECHO) pclint findshed!!!
$(PCLINTLIST): builddirs
%.pclint:
	$(PERL) make\perl_script\pclint_main_cmd.pl $(subst /,\,$(BUILD_DIR))  "$*" $(MAKE_HOME) $(PROJECT)


.PHONY:options
options:builddirs
	$(PERL) make\perl_script\pclint_main_cmd.pl $(subst /,\,$(BUILD_DIR))  "$(patsubst %.a, %, $(filter-out !% %.indep, $(MKLIBLIST)))" $(MAKE_HOME) $(PROJECT) options

#
# Help infomation
#
.PHONY:help
help:
	@$(ECHO) Typing right command as shown on following:
	@$(ECHO) 1.[make all update] make all targets those have been modified:
	@$(ECHO) "    make PROJECT=projectname"
	@$(ECHO) 2.[make all new] clean all and rebuild all targets:
	@$(ECHO) "    make PROJECT=projectname new"
	@$(ECHO) 3.[make clean all] clean all targets, i.e. all objects and libraries:
	@$(ECHO) "    make PROJECT=projectname cleanall"
	@$(ECHO) 4.[module clean] clean relevant module targets:
	@$(ECHO) "    make PROJECT=projectname MODULES=\"module1 module2 module3 ...\" clean"
	@$(ECHO) 5.[module update] rebuild relevant module targets:
	@$(ECHO) "    make PROJECT=projectname MODULES=\"module1 module2 module3 ...\" update"
	@$(ECHO) 6.[make image] executable binary file generation:
	@$(ECHO) "    make PROJECT=projectname image"
	@$(ECHO) 7.[make bootloader] bootloader generation:
	@$(ECHO) "    make PROJECT=projectname MODULES=bootloader"
	@$(ECHO) 8.[make fdl] fdl generation:
	@$(ECHO) "    make PROJECT=projectname MODULES=fdl1, or make PROJECT=projectname MODULES=fdl2"
	@$(ECHO) 9.[make nor_fdl] nor_fdl generation:
	@$(ECHO) "    make PROJECT=projectname MODULES=nor_fdl"
	@$(ECHO) 10.[make nv_parameter] nv_parameter generation:
	@$(ECHO) "    make PROJECT=projectname MODULES=nv_parameter"
	@$(ECHO) 11.[make resource] resource generation:
	@$(ECHO) "    make PROJECT=projectname MODULES=resource"

	@$(ECHO)
	@$(ECHO) Please refer to directory [./make] for all available modules.
	@$(ECHO) RVCT_VERSION=$(RVCT_VERSION)
	@$(ECHO) CFLAGS=${CFLAGS}
	@$(ECHO) AFLAGS=${AFLAGS}
	@$(ARMCC)
	@exit 0
