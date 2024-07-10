#--------------------------------------------UAL_TELE_SIM SRC & INC-----------------------------------------------------------------#
MINCPATH += ual/src/tele ual/src/tele/sim ual/src/adap_common

ifeq ($(strip $(PLATFORM)), ANTISW3)
MINCPATH += MSL/l4/inc MSL/gas/inc common/export/inc PS/stack/sim/inc
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH += BASE/l4/export/inc common/export/inc BASE/PS/export/inc CAF/Template/include
endif

MSRCPATH += ual/src/tele/sim/adap_sim
MSRCPATH += ual/src/adap_common
SOURCES  += adap_common_string.c

ifeq ($(strip $(PLATFORM)), ANTISW3)
SOURCES  += adap_sim_sprd_mn.c
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES  += adap_sim_rda_mn.c
endif