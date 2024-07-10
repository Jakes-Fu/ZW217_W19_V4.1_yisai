#--------------------------------------------UAL SRC & INC-----------------------------------------------------------------#
# UAL_TODO

ifeq ($(strip $(PLATFORM)), ANTISW3)
MINCPATH = MSL/l4/inc MSL/sim/inc MSL/gas/inc common/export/inc
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH = BASE/l4/export/inc BASE/sim/export/inc BASE/l4/export/inc BASE/PS/export/inc BASE/atc/export/inc
endif

MINCPATH += $(MMI_DIR)/source/mmi_kernel/include #UAL_TODO
MINCPATH += ual/src/cms
MINCPATH += ual/inc
MINCPATH += ual/inc/tele

MSRCPATH += ual/src/tele
SOURCES  += ual_tele.c

include ./make/ual_tele/ual_tele_sim.mk
include ./make/ual_tele/ual_tele_data.mk
include ./make/ual_tele/ual_tele_call.mk
include ./make/ual_tele/ual_tele_radio.mk
#include ./make/ual_tele/ual_tele_ss.mk
