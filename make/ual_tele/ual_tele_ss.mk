#--------------------------------------------UAL_SS SRC & INC-----------------------------------------------------------------#
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp/h   //ual_todo:后续使用ual_pdp
MINCPATH += CAF/Template/include
MINCPATH += MSL/l4/inc MSL/sim/inc MSL/gas/inc MSL/atc/inc
MINCPATH += ual/src/timer
MINCPATH += ual/inc
MINCPATH += ual/inc/tele
MSRCPATH += ual/src/tele/ss
SOURCES  += ual_tele_ss.c ss_ctrl.c ss_data.c ss_parse.c
