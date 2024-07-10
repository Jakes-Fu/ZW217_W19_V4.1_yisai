#--------------------------------------------UAL_SS SRC & INC-----------------------------------------------------------------#
ifeq ($(strip $(UAL_SS_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp/h
MINCPATH += CAF/Template/include
MINCPATH += MSL/l4/inc MSL/sim/inc MSL/gas/inc MSL/atc/inc
MINCPATH += ual/inc
MINCPATH += ual/inc/tele
MINCPATH += ual/src/tele
MSRCPATH += ual/src/tele
SOURCES  += ual_tele_ss_ctrl.c ual_tele_ss_data.c ual_tele_ss_parse.c ual_tele.c
endif
