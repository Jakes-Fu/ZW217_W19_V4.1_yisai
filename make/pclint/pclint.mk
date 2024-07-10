
#
# Pre-defined Make Rules and Modules Specific Compiler Options
#
include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

COMPONENT_TMP:=$(subst __,$(space),$(COMPONENT))
COMPONENT_SRC:=$(word 1,$(COMPONENT_TMP))
SOURCES_FROM:=$(word 2,$(COMPONENT_TMP))
SOURCES_TO:=$(word 3,$(COMPONENT_TMP))

include $(call GETMAKEPATH,$(COMPONENT_SRC))

#
# Vars For Building Target
#
TARGET              = $(COMPONENT).a
INCPATH             = $(subst \,/,$(addprefix -I, $(COMMON_INCPATH)))
INCPATH             += $(subst \,/,$(addprefix -I, $(MINCPATH)))
MCFLAGS             = $(MCFLAG_OPT)
MASMFLAG            = $(MASMFLAG_OPT)
SRCPATH             = $(subst \,/,$(MSRCPATH))


.PHONEY:pldep
pldep: public_macro
	@$(ECHO) component = $(COMPONENT)
	@$(PERL) make/perl_script/mk_dir.pl $(BUILD_DIR)/pclint
	@$(PERL) make/perl_script/mk_dir.pl $(BUILD_DIR)/pclint/$(COMPONENT)
	@$(ECHO) $(INCPATH) 2>&1 > $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt)
	@$(ECHO) -D_RTOS -D_DEBUG 2>&1 >> $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt)
	
	@$(ECHO) $(filter -D%, $(wordlist 1, 200, $(MCFLAG_OPT))) >> $(BUILD_DIR)/pclint/$(COMPONENT).lnt
	@$(ECHO) $(filter -D%, $(wordlist 201, 400, $(MCFLAG_OPT))) >> $(BUILD_DIR)/pclint/$(COMPONENT).lnt
	@$(ECHO) $(filter -D%, $(wordlist 401, $(words $(MCFLAG_OPT)), $(MCFLAG_OPT))) >> $(BUILD_DIR)/pclint/$(COMPONENT).lnt
	
	@$(ECHO) $(wordlist 1, 200,$(C_MACRO)) >> $(BUILD_DIR)/pclint/$(COMPONENT).lnt
	@$(ECHO) $(wordlist 201, 400,$(C_MACRO)) >> $(BUILD_DIR)/pclint/$(COMPONENT).lnt
	@$(ECHO) $(wordlist 401, $(words $(C_MACRO)),$(C_MACRO)) >> $(BUILD_DIR)/pclint/$(COMPONENT).lnt

	@$(ECHO) $(addprefix -I, $(SRCPATH))  >> $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt)
	@$(ECHO) -I\"$(ARM_HOME)/Include\"  -I\"$(ARM_HOME)/Include/rw\"  >> $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt)

ifneq ($(strip $(SOURCES_FROM)),)
	@-$(call ECHO_Long,$(wordlist $(SOURCES_FROM),$(SOURCES_TO),$(filter-out %.s, $(SOURCES))),>>,$(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt.tmp0))
else
	@-$(call ECHO_Long,$(filter-out %.s, $(SOURCES)),>>,$(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt.tmp0))
endif

	@-$(PERL) make/perl_script/pclint_result_filter.pl $(COMPONENT) $(BUILD_DIR)/pclint/$(COMPONENT).lnt.tmp0 > $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt.tmp1)
	@$(CAT) $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt.tmp1) >> $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt)
	@$(PERL) make/perl_script/rm_file.pl $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt.tmp0) ./
	@$(PERL) make/perl_script/rm_file.pl $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt.tmp1) ./

	@$(ECHO) $(COMPONENT) pclint start ...
	@$(CAT) $(call Path_process,$(BUILD_DIR)/pclint/$(COMPONENT).lnt)
ifeq ($(pclint_tool),PLUS)
	@-"$(PCLINT_DIR)/pclp64.exe"  -b -i"$(PCLINT_DIR)/makelint"  std_arm.lnt  build/$(PROJECT)_builddir/pclint/$(COMPONENT).lnt > build/$(PROJECT)_builddir/pclint/$(COMPONENT)/aloa.xml
	@$(ECHO) $(COMPONENT) pclint-plus finished !
else
	@-"$(PCLINT_DIR)/lint-nt.exe"  -b -i"$(PCLINT_DIR)/makelint" std.lnt  build/$(PROJECT)_builddir/pclint/$(COMPONENT).lnt > build/$(PROJECT)_builddir/pclint/$(COMPONENT)/aloa.xml
endif	
	@-"$(PCLINT_DIR)/aloa.exe" build/$(PROJECT)_builddir/pclint/$(COMPONENT)/aloa.xml > build/$(PROJECT)_builddir/pclint/$(COMPONENT)/Result.txt
	#@-$(PERL) make/perl_script/rm_file.pl $(COMPONENT).lnt build/$(PROJECT)_builddir/pclint/
	@$(ECHO) $(COMPONENT) pclint finished !

.PHONEY:option_create
option_create: public_macro
	@$(ECHO) component = $(COMPONENT)
	@$(PERL) make/perl_script/mk_dir.pl $(BUILD_DIR)/options
	@$(ECHO) $(INCPATH) 2>&1 > $(call Path_process,$(BUILD_DIR)/options/$(COMPONENT).option)
	@$(ECHO) $(filter -D%, $(MCFLAG_OPT))  >> $(call Path_process,$(BUILD_DIR)/options/$(COMPONENT).option)
	@$(ECHO) $(CFLAGS) 2>&1 | $(TEE)  -a $(call Path_process,$(BUILD_DIR)/options/$(COMPONENT).option)
	@$(ECHO) $(C_MACRO) 2>&1 | $(TEE)  -a $(call Path_process,$(BUILD_DIR)/options/$(COMPONENT).option)
	@$(ECHO) $(addprefix -I, $(SRCPATH))  >> $(call Path_process,$(BUILD_DIR)/options/$(COMPONENT).option)
	@$(ECHO) $(filter-out %.s, $(SOURCES)) >> $(call Path_process,$(BUILD_DIR)/options/$(COMPONENT).option)
	@$(ECHO) $(COMPONENT) options start ...
	@$(CAT) $(call Path_process,$(BUILD_DIR)/options/$(COMPONENT).option)
	@$(ECHO) $(COMPONENT) options finished !

.PHONEY:moduledep
moduledep: public_macro
	@$(ECHO) component = $(COMPONENT)
	@$(ECHO) $(INCPATH) 2>&1 > $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp

	@$(ECHO) $(wordlist 1, 200,$(filter -D%, $(MCFLAG_OPT))) >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp
	@$(ECHO) $(wordlist 201, 400,$(filter -D%, $(MCFLAG_OPT))) >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp
	@$(ECHO) $(wordlist 401, $(words $(filter -D%, $(MCFLAG_OPT))),$(filter -D%, $(MCFLAG_OPT))) >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp

	@$(ECHO) $(wordlist 1, 200,$(C_MACRO)) >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp
	@$(ECHO) $(wordlist 201, 400,$(C_MACRO)) >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp
	@$(ECHO) $(wordlist 401, $(words $(C_MACRO)),$(C_MACRO)) >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp

	@$(ECHO) $(wordlist 1, 200,$(MCFLAG_OPT)) >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp
	@$(ECHO) $(wordlist 201, 400,$(MCFLAG_OPT)) >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp
	@$(ECHO) $(wordlist 401, $(words $(MCFLAG_OPT)),$(MCFLAG_OPT)) >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp

	@$(ECHO) $(MASMFLAG_OPT)  >> $(call Path_process,$(BUILD_DIR)/dep/$(COMPONENT).dep.tmp)
	@$(ECHO) $(addprefix -I, $(MSRCPATH)) 2>&1 >> $(BUILD_DIR)/dep/$(COMPONENT).dep.tmp


resdep: public_macro
	@$(ECHO) component = $(COMPONENT)
	@$(ECHO) $(wordlist 1, 200,$(filter -D%, $(MCFLAG_OPT))) > $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro)
	@$(ECHO) $(wordlist 201, 400,$(filter -D%, $(MCFLAG_OPT))) >> $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro)
	@$(ECHO) $(wordlist 401, $(words $(filter -D%, $(MCFLAG_OPT))),$(filter -D%, $(MCFLAG_OPT))) >> $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro)

	@$(ECHO) $(wordlist 1, 200,$(C_MACRO)) >> $(BUILD_DIR)/tmp/$(COMPONENT).macro
	@$(ECHO) $(wordlist 201, 400,$(C_MACRO)) >> $(BUILD_DIR)/tmp/$(COMPONENT).macro
	@$(ECHO) $(wordlist 401, $(words $(C_MACRO)),$(C_MACRO)) >> $(BUILD_DIR)/tmp/$(COMPONENT).macro

nvdep: 
	@$(ECHO) component = $(COMPONENT)

	@$(ECHO) $(CFLAGS)  2>&1 >  $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro.nv)

	@$(ECHO) $(wordlist 1, 200,$(filter -D%, $(MCFLAG_OPT))) >> $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro.nv)
	@$(ECHO) $(wordlist 201, 400,$(filter -D%, $(MCFLAG_OPT))) >> $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro.nv)
	@$(ECHO) $(wordlist 401, $(words $(filter -D%, $(MCFLAG_OPT))),$(filter -D%, $(MCFLAG_OPT))) >> $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro.nv)

	@$(ECHO) $(addprefix -I, $(COMMON_INCPATH)) 2>&1 >>  $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro.nv)
	@$(ECHO) $(addprefix -I, $(MINCPATH)) 2>&1 >>  $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro.nv)

	@$(ECHO) $(wordlist 1, 200,$(C_MACRO))   2>&1  | $(TEE)  -a $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro.nv)
	@$(ECHO) $(wordlist 201, 400,$(C_MACRO)) 2>&1  | $(TEE)  -a $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro.nv)
	@$(ECHO) $(wordlist 401, $(words $(C_MACRO)),$(C_MACRO)) 2>&1  | $(TEE)  -a $(call Path_process,$(BUILD_DIR)/tmp/$(COMPONENT).macro.nv)


.PHONEY:public_macro
public_macro:
	@$(PERL) make/perl_script/mk_dir.pl $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl $(BUILD_DIR)/tmp