
MCFLAG_OPT   	= -Otime

ifneq ($(strip $(CHIP_ENDIAN)),	LITTLE)
MCFLAG_OPT		+= -D_BIG_ENDIAN
endif

ifeq ($(strip $(PLATFORM)),	SC8800G)
MCFLAG_OPT 		+= -DCHIP_DRV_SC8800H
endif

MINCPATH =  MS_Ref/source/MTP/inc
MINCPATH += chip_drv/export/inc/outdated

MSRCPATH =  MS_Ref/source/MTP/src


SOURCES			=  MTP_driver.c MTP_Descriptors.c MTP_protocol.c

