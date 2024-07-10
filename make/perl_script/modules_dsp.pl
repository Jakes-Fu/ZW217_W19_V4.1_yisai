
#
# Vars For Building Target
#

include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

AAA:=$(call CO_OPTION_TO_FILE,$(call GET_SIMU_C_MACRO),$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_mac)
AAA:=$(call CO_OPTION_TO_FILE,$(call GET_SIMU_C_MACRO),$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_mac_res)

-include make/$(COMPONENT)/$(COMPONENT).mk



INCPATH       = $(COMMON_INCPATH)
INCPATH       +=$(MINCPATH)
MCFLAGS	      = $(MCFLAG_OPT)
MASMFLAG      = $(MASMFLAG_OPT)
SRCPATH       = $(MSRCPATH)

CMDS1         = $(patsubst %.cpp, %.o, $(SOURCES))
CMDS2         = $(patsubst %.c, %.o, $(CMDS1))
CMDS          += $(patsubst %.s, %.o, $(CMDS2))

#
# VPATH Settings For The Specific Modules
#
vpath
vpath %.h        $(subst \,/,$(INCPATH))
vpath %.c        $(subst \,/,$(SRCPATH))
vpath %.cpp      $(subst \,/,$(SRCPATH))
vpath %.dat      $(subst \,/,$(SRCPATH))
vpath %.s        $(subst \,/,$(SRCPATH))

# Common Commands for Building Libs, C and Assemly Language


.PHONEY : all
all: del_dsp_list $(CMDS)
	$(ECHO) $(wordlist 1, 200,$(MCFLAGS)) >>$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_mac
	$(ECHO) $(wordlist 201, 400,$(MCFLAGS)) >>$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_mac
	$(ECHO) $(wordlist 401, $(words $(MCFLAGS)),$(MCFLAGS)) >>$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_mac
	$(ECHO) $(INCPATH) >$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_inc

del_dsp_list:
	$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_lis ""

%.o:%.cpp
	$(ECHO)  $<  >>$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_lis
%.o:%.c
	$(ECHO)  $<  >>$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_lis
%.o:%.s
	$(ECHO)  $<  >>$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_lis

dsp_mac_res:	
	$(ECHO) $(wordlist 1, 200,$(MCFLAGS))   >>$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_mac_res
	$(ECHO) $(wordlist 201, 400,$(MCFLAGS)) >>$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_mac_res
	$(ECHO) $(wordlist 401, $(words $(MCFLAGS)),$(MCFLAGS)) >>$(BUILD_DIR)/win/$(COMPONENT)/$(COMPONENT).dsp_mac_res	