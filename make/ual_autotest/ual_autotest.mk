#--------------------------------------------UAL_autotest SRC & INC-----------------------------------------------------------------#
MINCPATH += ual/inc
MINCPATH += ual/src/cms

ifeq ($(strip $(UAL_FACTORY_TEST_SUPPORT)), TRUE)
MINCPATH += MS_Ref/source/engpc/sprd_fts_inc
ifneq ($(strip $(UAL_BT_BLE_DEV_SUPPORT)), NONE)
MINCPATH += ual/inc/bt
MSRCPATH += ual/src/autotest/factory_test
SOURCES  += ual_autotest_factory_test_ble.c
endif
endif
ifeq ($(strip $(UAL_ATC_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += CAF/Template/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
ifneq ($(strip $(UAL_BT_BLE_DEV_SUPPORT)), NONE)
MINCPATH += ual/inc/bt
MSRCPATH += ual/src/autotest/atc
SOURCES  += ual_autotest_atc_bt_ble.c
endif
endif


