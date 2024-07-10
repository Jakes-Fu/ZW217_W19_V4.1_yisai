
.SUFFIXES:
.SUFFIXES: .o .c .s .cpp .dat .cmpl .ibd

#
# Vars For Building Target
#
TARGET				= $(COMPONENT).a
INCPATH 			= $(addprefix -I, $(COMMON_INCPATH))
INCPATH             		+=$(addprefix -I, $(MINCPATH))
MCFLAGS				= $(MCFLAG_OPT)
MASMFLAG			= $(MASMFLAG_OPT)
SRCPATH 			= $(MSRCPATH)
MOBJPATH			= $(BUILD_DIR)/obj/$(COMPONENT)
DEPPATH				= $(BUILD_DIR)/dep/$(COMPONENT)

OBJECTS1 			= $(patsubst %.c, %.o, $(SOURCES))
OBJECTS2 			+= $(patsubst %.cpp, %.o, $(OBJECTS1))
OBJECTS3 			+= $(patsubst %.cxx, %.o, $(OBJECTS2))
OBJECTS 			+= $(patsubst %.s, %.o, $(OBJECTS3))

CMDS1 			= $(patsubst %.c, %.cmpl, $(SOURCES))
CMDS2			= $(patsubst %.cpp, %.cmpl, $(CMDS1))
CMDS3			= $(patsubst %.cxx, %.cmpl, $(CMDS2))
CMDS 			+= $(patsubst %.s, %.cmpl, $(CMDS3))

OPEN_MODULE_SUPPORT = $(if $(findstring "$(COMPONENT)",$(OPEN_MODULES)),TRUE,FALSE)

$(call CO_OPTION_TO_FILE,$(OBJECTS),$(MOBJPATH)/objectlist.txt)
$(call CO_OPTION_TO_FILE,$(C_MACRO) $(MCFLAGS),$(BUILD_DIR)/dep/$(COMPONENT)_C_MACRO_INC.txt)
$(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/$(COMPONENT)_A_MACRO_INC.txt)

OPTION_TO_FILE := $(shell $(ECHO) $(INCPATH) >>$(BUILD_DIR)/dep/$(COMPONENT)_C_MACRO_INC.txt)
OPTION_TO_FILE := $(shell $(ECHO) $(INCPATH) >>$(BUILD_DIR)/dep/$(COMPONENT)_A_MACRO_INC.txt)

CreateDir := $(shell $(PERL) make/perl_script/mk_dir.pl "cache" $(BUILD_DIR)/dep/$(COMPONENT))
#CheckDep := $(shell $(PERL) ./make/perl_script/filecheck.pl $(DEPPATH)/cache)

ifeq ($(strip $(OUTPUTPATH)),)
ifeq ($(strip $(OPEN_MODULE_SUPPORT)),TRUE)
TARGET_PATH = $(BUILD_DIR)/lib
else
TARGET_PATH = ./lib/$(strip $(PRODUCT_BASELIB_DIR))
endif
else
TARGET_PATH = $(strip $(OUTPUTPATH))
endif

#
# VPATH Settings For The Specific Modules
#
vpath
vpath %.h        $(subst \,/,$(INCPATH))
vpath %.c        $(subst \,/,$(SRCPATH))
vpath %.cpp      $(subst \,/,$(SRCPATH))
vpath %.cxx      $(subst \,/,$(SRCPATH))
vpath %.dat      $(subst \,/,$(SRCPATH))
vpath %.s        $(subst \,/,$(SRCPATH))
vpath %.o        $(BUILD_DIR)/obj/$(COMPONENT)
vpath %.cmpl     $(BUILD_DIR)/dep/$(COMPONENT)/cache
vpath %.ibd      $(BUILD_DIR)/dep/$(COMPONENT)/cache


vpath %.a        $(TARGET_PATH)


# Common Commands for Building Libs, C and Assemly Language

$(TARGET) : $(CMDS)
	@$(PERL) ./make/perl_script/createxml.pl $(PROJECT) $(COMPONENT)
	@xgConsole.exe build\\$(PROJECT)_builddir\\dep\\$(COMPONENT).xml
	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/dep/$(COMPONENT).xml ./
	@$(ECHO)
	@$(ECHO) creating $(COMPONENT).a ...
	@-$(COPY_FILE)  $(call Path_process,$(BUILD_DIR)/dep/$(COMPONENT)/*.cmpl)  $(call Path_process,$(BUILD_DIR)/dep/$(COMPONENT)/cache/)  2>$(BUILD_DIR)/log/ib_cmpl_$(COMPONENT).log 1>$(BUILD_DIR)/log/ib_cmpl_log_$(COMPONENT).log
	@-$(COPY_FILE)  $(call Path_process,$(BUILD_DIR)/dep/$(COMPONENT)/*.ibd)  $(call Path_process,$(BUILD_DIR)/dep/$(COMPONENT)/cache/)  2>$(BUILD_DIR)/log/ib_ibd_$(COMPONENT).log 1>$(BUILD_DIR)/log/ib_ibd_log_$(COMPONENT).log
	@-$(PERL) make/perl_script/rm_file.pl *.cmpl  $(BUILD_DIR)/dep/$(COMPONENT)/
	@-$(PERL) make/perl_script/rm_file.pl *.ibd  $(BUILD_DIR)/dep/$(COMPONENT)/
	@-$(PERL) make/perl_script/rm_file.pl *.tmp  $(BUILD_DIR)/dep/$(COMPONENT)/

	@$(PERL) make/perl_script/rm_file.pl $(TARGET) $(TARGET_PATH)/
	cd $(MOBJPATH) && $(AR) $(ARFLAGS) ../../../../$(TARGET_PATH)/$(TARGET)  $(VIA) ./objectlist.txt

-include $(OBJECTS:.o=.ibd)

#.PHONEY : builddir
#builddir : 
#	@$(PERL) make/perl_script/mk_dir.pl "cache" $(BUILD_DIR)/dep/$(COMPONENT)

%.cmpl %.ibd:%.cpp $(call GETMAKEPATH,$(COMPONENT)) project_$(PROJECT).mk Makefile Makefile.modules Makefile.rules
	@echo $(CCOMPLAN) $(CFLAGS)   $(VIA) $(BUILD_DIR)/dep/$(COMPONENT)_C_MACRO_INC.txt -c   -I$(ARMINC)  $< $(MDFLAGS) $(DEPPATH)/$*.ibd.tmp -o $(MOBJPATH)/$*.o >$(BUILD_DIR)/dep/$(COMPONENT)/$*.cmpl
	@echo $(PERL) .\make\perl_script\path_process_ib.pl ./$(DEPPATH)/$*.ibd.tmp ^>./$(DEPPATH)/$*.ibd >> ./$(DEPPATH)/$*.cmpl
%.cmpl %.ibd:%.cxx $(call GETMAKEPATH,$(COMPONENT)) project_$(PROJECT).mk Makefile Makefile.modules Makefile.rules
	@echo $(CCOMPLAN) $(CFLAGS)   $(VIA) $(BUILD_DIR)/dep/$(COMPONENT)_C_MACRO_INC.txt -c   -I$(ARMINC)  $< $(MDFLAGS) $(DEPPATH)/$*.ibd.tmp -o $(MOBJPATH)/$*.o >$(BUILD_DIR)/dep/$(COMPONENT)/$*.cmpl
	@echo $(PERL) .\make\perl_script\path_process_ib.pl ./$(DEPPATH)/$*.ibd.tmp ^>./$(DEPPATH)/$*.ibd >> ./$(DEPPATH)/$*.cmpl
%.cmpl %.ibd:%.c $(call GETMAKEPATH,$(COMPONENT)) project_$(PROJECT).mk Makefile Makefile.modules Makefile.rules
	@echo $(CCOMPLAN) $(CFLAGS)   $(VIA) $(BUILD_DIR)/dep/$(COMPONENT)_C_MACRO_INC.txt -c   -I$(ARMINC)  $< $(MDFLAGS) $(DEPPATH)/$*.ibd.tmp -o $(MOBJPATH)/$*.o >$(BUILD_DIR)/dep/$(COMPONENT)/$*.cmpl
	@echo $(PERL) .\make\perl_script\path_process_ib.pl ./$(DEPPATH)/$*.ibd.tmp ^>./$(DEPPATH)/$*.ibd >> ./$(DEPPATH)/$*.cmpl
%.cmpl %.ibd:%.s  $(call GETMAKEPATH,$(COMPONENT)) project_$(PROJECT).mk Makefile Makefile.modules Makefile.rules
	@echo $(ASM) $(AFLAGS) $(MASMFLAG) $(VIA) $(BUILD_DIR)/dep/$(COMPONENT)_A_MACRO_INC.txt -I$(ARMINC) $< $(MDFLAGS) $(DEPPATH)/$*.ibd.tmp -o $(MOBJPATH)/$*.o >$(BUILD_DIR)/dep/$(COMPONENT)/$*.cmpl
	@echo $(PERL) .\make\perl_script\path_process_ib.pl ./$(DEPPATH)/$*.ibd.tmp ^>./$(DEPPATH)/$*.ibd >> ./$(DEPPATH)/$*.cmpl
.PHONEY : $(COMPONENT).indep
$(COMPONENT).indep:
ifeq ($(strip $(OPEN_MODULE_SUPPORT)),TRUE)
	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/lib/$(TARGET) ./
else
	@-$(PERL) make/perl_script/rm_file.pl lib/$(strip $(PRODUCT_BASELIB_DIR))/$(TARGET) ./
endif