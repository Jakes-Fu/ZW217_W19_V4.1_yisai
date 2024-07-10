MCFLAG_OPT   	= -DLOG_DEBUG

ifeq ($(strip $(ASSERT_TYPE_SUPPORT)), ASSERT)
MCFLAG_OPT		+=  -DMMI_ASSERT_MODE
endif

ifeq ($(strip $(LOW_MEMORY_SUPPORT)), FALSE)
MCFLAG_OPT		+=  -DMMI_AUTOTEST_SUPPORT
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
$(MMI_DIR)/source/mmi_app/app/fmm/h \
$(MMI_DIR)/source/mmi_app/app/wifi/h \
$(MMI_DIR)/source/mmi_app/app/pb/h   \
$(MMI_DIR)/source/mmi_app/app/pdp/h \
wre/source/include 

MSRCPATH = \
wre\source\config

SOURCES = wre_cfg.c
