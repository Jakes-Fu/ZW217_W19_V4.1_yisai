#--------------------------------------------UAL_NETMGR SRC & INC-----------------------------------------------------------------#
#UAL_TODO:
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
MINCPATH += MSL/l4/inc
MINCPATH += MSL/sim/inc
MINCPATH += MSL/gas/inc
MINCPATH += MSL/atc/inc
MINCPATH += DAPS/export/inc
#MINCPATH += DAPS/export/inc/tcpip
MINCPATH += DAPS/export/inc/tcpip6
MINCPATH += CAF/Template/include
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp/h
MINCPATH += $(MMI_DIR)/source/mmi_app/kernel/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/connection/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/phone/h
MINCPATH += $(MMI_DIR)/source/mmi_app/common/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/wlan/h
MINCPATH += $(MMI_DIR)/source/resource
MINCPATH += ual/src/cms
MINCPATH += ual/inc
MINCPATH += ual/inc/tele

#UAL_TODO:之后csm移出后再做调整
MINCPATH += ual/src/audioplayer

#UAL_TODO:ual_wifi接入后使用
ifeq ($(strip $(UAL_WIFI_SUPPORT)), TRUE)
MINCPATH += ual/inc/wifi
endif

MSRCPATH += ual/src/networkmgr
SOURCES += ual_networkmgr_init.c
SOURCES += ual_networkmgr_ctrl.c
SOURCES += ual_networkmgr_data.c