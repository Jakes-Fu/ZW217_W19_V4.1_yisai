#--------------------------------------------UAL SRC & INC-----------------------------------------------------------------#
MINCPATH += common/export/inc
MINCPATH += ms_ref/export/inc
MINCPATH += CAF/Template/include
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
MINCPATH += BASE/l4/export/inc
MINCPATH += ual/inc
MINCPATH += ual/src/
MSRCPATH += ual/src/
MINCPATH += ual/src/adap_common
MINCPATH += ual/inc/device
MINCPATH += ual/inc/tele
SOURCES  += ual.c

MINCPATH += ual/src/csm
MSRCPATH += ual/src/csm
SOURCES  += ual_csm.c

MINCPATH += ual/src/cms
MSRCPATH += ual/src/cms
SOURCES  += ual_cms.c

MINCPATH += ual/src/timer
MSRCPATH += ual/src/timer
SOURCES  += ual_timer.c

