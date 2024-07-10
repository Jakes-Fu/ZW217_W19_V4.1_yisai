
MINCPATH =  chip_drv/export/inc  	MS_Ref/source/base/inc  \
						chip_drv/export/inc/outdated  \
						chip_drv/chip_module/gpio \
						common/export/inc  \
						RTOS/export/inc  \
						MS_Ref/export/inc  \
						MS_Ref/source/gps/green_eye2/ge2_libgps_mp/gps_lib/inc \
						$(MMI_DIR)/source/mmi_app/app/autocit_lcd/h  \
						$(MMI_DIR)/source/mmi_app/app/theme/h  \
						$(MMI_DIR)/source/mmi_app/app/wlan/h  \
						$(MMI_DIR)/source/mmi_ctrl/include  \
						$(MMI_DIR)/source/mmi_gui/include  \
						$(MMI_DIR)/source/mmi_kernel/include  \
						$(MMI_DIR)/source/mmi_service/export/inc  \
						$(MMI_DIR)/source/mmi_utility/h  \
						CAF/Template/include  \
						MS_Ref/source/autotest_device/inc  \
						chip_drv/chip_plf/export/inc  \
                        chip_drv/chip_plf/common/inc
						
ifeq ($(strip $(CHIP_VER)),    6530)
MINCPATH +=             chip_drv/chip_plf/sc6530
endif

ifeq ($(strip $(CHIP_VER)),    6531)
MINCPATH +=             chip_drv/chip_plf/sc6531
endif

ifeq ($(strip $(CHIP_VER)),    6531EFM)
MINCPATH +=             chip_drv/chip_plf/sc6531efm
endif

ifeq ($(strip $(PLATFORM)),    SC8800G)
MINCPATH +=             chip_drv/chip_plf/sc8800g
endif

ifeq ($(strip $(PLATFORM)),    SC6800H)
MINCPATH +=             chip_drv/chip_plf/sc6800h
endif	

MSRCPATH = MS_Ref/source/autotest_device/src 

SOURCES	= autotest_device.c   autotest_audio.c







