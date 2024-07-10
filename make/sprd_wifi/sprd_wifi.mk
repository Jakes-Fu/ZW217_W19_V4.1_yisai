
#MCFLAG_OPT += -DCONFIG_WIFI_ZERO_COPY

MINCPATH = MS_Ref/export/inc \
			chip_drv/export/inc \
			RTOS/export/inc \
			MS_Ref/source/sprd_wifi/inc/

#MINCPATH += chip_drv/chip_plf/$(strip $(PLATFORM))

MSRCPATH = MS_Ref/source/sprd_wifi/src/
SOURCES	 = wifi_main.c wifi_api.c wifi_npi.c
