
MCFLAG_OPT   	= -Otime

ifneq ($(strip $(CHIP_ENDIAN)),	LITTLE)
MCFLAG_OPT		+= -D_BIG_ENDIAN
endif

ifeq ($(strip $(PLATFORM)),	SC8800G)
MCFLAG_OPT 		+= -DCHIP_DRV_SC8800H
endif

MINCPATH =  MS_Ref/source/ubot/inc
MINCPATH += chip_drv/export/inc/outdated

MSRCPATH =  MS_Ref/source/ubot/src


SOURCES			= ubot_config.c ubot_drv.c  ubot_main.c ubot_scsi_sbc.c ubot_scsi_spc.c ubot_storage.c

