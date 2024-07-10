MCFLAG_OPT   	= -DLOG_DEBUG -DUNICODE -D_UNICODE

ifeq ($(strip $(ASSERT_TYPE_SUPPORT)), ASSERT)
MCFLAG_OPT		+=  -DMMI_ASSERT_MODE
endif

ifeq ($(strip $(LOW_MEMORY_SUPPORT)), FALSE)
MCFLAG_OPT		+=  -DMMI_AUTOTEST_SUPPORT
endif

ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -D_WIFI
endif

MINCPATH =  \
CAF/Template/include  \
chip_drv/export/inc/outdated \
l4/export/inc \
$(MMI_DIR)/source/mmi_ctrl/include  \
$(MMI_DIR)/source/mmi_gui/include  \
$(MMI_DIR)/source/resource \
$(MMI_DIR)/source/mmi_utility/h \
$(MMI_DIR)/source/mmi_kernel/include \
$(MMI_DIR)/source/mmi_app/custom/h \
$(MMI_DIR)/source/mmi_app/kernel/h \
$(MMI_DIR)/source/mmi_app/common/h \
$(MMI_DIR)/source/mmi_app/app/audio/h \
$(MMI_DIR)/source/mmi_app/app/cc/h \
$(MMI_DIR)/source/mmi_app/app/fmm/h \
$(MMI_DIR)/source/mmi_app/app/idle/h \
$(MMI_DIR)/source/mmi_app/app/wifi/h \
$(MMI_DIR)/source/mmi_app/app/pb/h   \
$(MMI_DIR)/source/mmi_app/app/pdp/h \
$(MMI_DIR)/source/mmi_app/app/phone/h   \
$(MMI_DIR)/source/mmi_app/app/setting/h \
$(MMI_DIR)/source/mmi_app/app/theme/h \
$(MMI_DIR)/source/mmi_app/app/wre/h \
wre/source/include \
sim/export/inc \
PS/export/inc

MINCPATH +=  build/$(PROJECT)_builddir/tmp

MSRCPATH = \
wre\source\wrestartup

SOURCES = startup.c adaptor.c boot_stub.c
