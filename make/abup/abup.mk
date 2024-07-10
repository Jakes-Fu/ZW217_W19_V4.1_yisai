ABUPFOTAPATH        =  Third-party/abup
ifeq ($(strip $(FOTA_SUPPORT)), ABUP)
MINCPATH +=	MS_Ref/source/base/inc \
            chip_drv/chip_plf/sc6531efm \
            chip_drv/chip_plf/common\inc \
            chip_drv/export/inc \
            base/l4/export/inc \
            common/export/inc \
            chip_drv/export/inc/outdated \
            DAPS/export/inc/tcpip6 \
            $(ABUPFOTAPATH)/hal/include \
            $(ABUPFOTAPATH)/hal/halcore/include \
            BASE/atc/modem/source/h \
            BASE/atc/feature_phone/source/h \
            BASE/sim/export/inc \
            BASE/ps/export/inc \
            BASE/atc/export/inc \

MINCPATH += $(MMI_DIR)/source/mmi_app/app/connection/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp/h
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += CAF/Interface/source/resource
MINCPATH += CAF/Template/include
MINCPATH += $(MMI_DIR)/source/mmi_app/app/tp/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/udisk/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/vcard/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/vt/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/clipbrd/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/search/h
MINCPATH += $(MMI_DIR)/source/mmi_app/common/h
MINCPATH += $(MMI_DIR)/source/mmi_app/custom/h
MINCPATH += $(MMI_DIR)/source/mmi_app/kernel/h
MINCPATH += $(MMI_DIR)/source/mmi_ctrl/include
MINCPATH += $(MMI_DIR)/source/mmi_gui/include
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
MINCPATH += $(MMI_DIR)/source/mmi_app/kernel/h
MINCPATH += $(MMI_DIR)/source/resource
       

MSRCPATH +=	$(ABUPFOTAPATH) \
						$(ABUPFOTAPATH)/hal/src

SOURCES  += abup_debug.c \
			abup_device.c \
			abup_display.c \
			abup_file.c \
			abup_flash.c \
			abup_md5.c \
			abup_net.c \
			abup_stdlib.c \
			abup_thread.c \
			abup_timer.c

endif	 				 
