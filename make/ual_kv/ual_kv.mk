#--------------------------------------------UAL_KV_STORE SRC & INC-----------------------------------------------------------------#

MINCPATH += common/export/inc
MINCPATH += ms_ref/export/inc
MINCPATH += chip_drv/export/inc/outdated
MINCPATH += BASE/l4/export/inc

MINCPATH += ual/inc
MINCPATH += ual/src/kv

MSRCPATH += ual/src/kv

SOURCES  += ual_kv.c uil_kv.c kv_common.c kv_nv_store.c kv_adapt.c


