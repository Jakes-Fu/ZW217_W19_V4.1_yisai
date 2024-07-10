#--------------------------------------------UAL_bt SRC & INC-----------------------------------------------------------------#
ifneq ($(strip $(UAL_BT_BLE_DEV_SUPPORT)), NONE)
MINCPATH += ual/src/cms
MINCPATH += ual/inc
MINCPATH += ual/inc/bt
endif

MINCPATH +=  MS_Ref/source/nvm/inc

MSRCPATH += ual/src/bt/ble_dev
SOURCES  += ual_bt_ble_dev.c

ifeq ($(strip $(UAL_BT_BLE_DEV_SUPPORT)), GOODIX)
SOURCES  += ble_dev_goodix_adapter.c

ifeq ($(strip $(GOODIX_SUPPORT)),GR5515)
MINCPATH += Third-party/goodix/inc
endif

endif

ifeq ($(strip $(UAL_BT_BLE_PRIVATE_SUPPORT)), TRUE)
MSRCPATH += ual/src/bt/ble_private
SOURCES  += ual_bt_ble_private.c

ifeq ($(strip $(UAL_BT_BLE_DEV_SUPPORT)), GOODIX)
SOURCES  += ble_private_goodix_adapter.c
endif

endif

ifneq ($(strip $(UTEST_SUPPORT)), FALSE)
MINCPATH += utest/inc
endif