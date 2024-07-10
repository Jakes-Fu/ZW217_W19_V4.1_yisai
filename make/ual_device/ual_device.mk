#--------------------------------------------UAL_DEVICE SRC & INC-----------------------------------------------------------------#
#MINCPATH += common/export/inc
MINCPATH += chip_drv/chip_module/thermal
MINCPATH += ms_ref/export/inc
MINCPATH += BASE/l4/export/inc
MINCPATH += ual/inc
MINCPATH += ual/src/cms
MINCPATH += ual/adap_common
MINCPATH += ual/inc/device
MINCPATH += ual/src/device
MSRCPATH += ual/src/device
SOURCES  += adap_device.c ual_device.c