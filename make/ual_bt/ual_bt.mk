#--------------------------------------------UAL_bt SRC & INC-----------------------------------------------------------------#
MINCPATH += common/export/inc
MINCPATH += ms_ref/export/inc
# UAL_TODO
MINCPATH += $(MMI_DIR)/source/mmi_service/export/inc
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += CAF/Template/include
MINCPATH += ual/inc
MINCPATH += ual/src/
MINCPATH += ual/src/cms
ifneq ($(strip $(BLUETOOTH_SUPPORT)), NONE)
MINCPATH += ual/inc/bt
MINCPATH += ual/src/bt
MINCPATH += ual/src/bt/dev
MSRCPATH += ual/src/bt
SOURCES  += bt_unisoc_adapter.c ual_bt_internal.c
ifeq ($(strip $(UAL_BT_CLASSIC_DEV_SUPPORT)), TRUE)
MSRCPATH += ual/src/bt/dev
SOURCES  += ual_bt_dev.c
endif
ifeq ($(strip $(UAL_BT_HFU_SUPPORT)), TRUE)
MINCPATH += ual/src/bt/hfu
MSRCPATH += ual/src/bt/hfu
SOURCES  += ual_bt_hfu.c
endif
endif

ifneq ($(strip $(UTEST_SUPPORT)), FALSE)
MINCPATH += utest/inc
endif