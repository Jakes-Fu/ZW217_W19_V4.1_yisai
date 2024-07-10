

MCFLAG_OPT   	= -Otime

ifeq ($(strip $(PLATFORM)),	SC6600R)
MCFLAG_OPT		+= -DCHIP_DRV_SC6600R 
endif

ifeq ($(strip $(PLATFORM)),	6800)
MCFLAG_OPT		+= -DCHIP_DRV_SC6800D -D_UPCC_SC6800D_
endif

ifeq ($(strip $(PLATFORM)),	SC6600L)
MCFLAG_OPT 		+= -DCHIP_DRV_SC6600L 
endif
ifeq ($(strip $(PLATFORM)), SC8800H)
MCFLAG_OPT		+= -DCHIP_DRV_SC8800H
endif

MINCPATH =  MS_Ref/source/usbservice/inc
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc

MSRCPATH =  MS_Ref/source/usbservice/src

SOURCES	= usbservice_isr.c usbservice_main.c usbservice_interface.c
