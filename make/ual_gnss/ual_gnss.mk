#--------------------------------------------UAL_GNSS SRC & INC-----------------------------------------------------------------#
MINCPATH += MS_Ref/export/inc
MINCPATH += chip_drv/export/inc/outdated
MINCPATH += BASE/l4/export/inc
MINCPATH += BASE/Layer1/export/inc
MINCPATH += BASE/PS/export/inc
MINCPATH += RTOS/export/inc

MINCPATH += ual/inc
MINCPATH += ual/inc/tele
MINCPATH += ual/src/cms
MINCPATH += ual/src/gnss
MSRCPATH += ual/src/gnss
SOURCES  += ual_gnss.c uil_gnss_mgr.c uil_gnss_fsm.c uil_gnss_data.c
