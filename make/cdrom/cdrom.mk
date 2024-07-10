
MCFLAG_OPT = -Otime  -DUSB_LOG  

ifeq ($(PLATFORM), SC6800H)
MCFLAG_OPT   	+= -DCHIP_DRV_SC6600L 
endif
ifneq ($(strip $(CHIP_ENDIAN)),	LITTLE)
MCFLAG_OPT		+= -D_BIG_ENDIAN
endif

ifeq ($(strip $(PLATFORM)),	SC8800G)
MCFLAG_OPT 		+= -DCHIP_DRV_SC8800H
endif

MINCPATH =  MS_Ref/source/cdrom/inc MS_Ref/source/mux/inc MS_Ref/source/base/inc
MINCPATH += chip_drv/export/inc/outdated
 
MSRCPATH =  MS_Ref/source/cdrom/src

SOURCES	 = cdrom_config.c cdrom_drv.c cdrom_main.c


SOURCES += cdrom_scsi_sbc.c cdrom_scsi_spc.c cdrom_storage.c    

