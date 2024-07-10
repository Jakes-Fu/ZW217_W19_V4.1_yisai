
MCFLAG_OPT = -Otime  -DUSB_LOG  

ifeq ($(PLATFORM), SC6800H)
MCFLAG_OPT   	+= -DCHIP_DRV_SC6600L 
endif

ifeq ($(PLATFORM), SC6530)
MCFLAG_OPT   	+= -DCHIP_DRV_SC6600L 
endif

ifeq ($(PLATFORM), SC6531EFM)
MCFLAG_OPT   	+= -DCHIP_DRV_SC6600L 
endif

ifneq ($(strip $(CHIP_ENDIAN)),	LITTLE)
MCFLAG_OPT		+= -D_BIG_ENDIAN
endif

ifeq ($(strip $(PLATFORM)),	SC8800G)
MCFLAG_OPT 		+= -DCHIP_DRV_SC8800H
endif

MINCPATH =  MS_Ref/source/ucom/inc MS_Ref/source/mux/inc
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc
 
MSRCPATH =  MS_Ref/source/ucom/src

SOURCES	 = ucom_config.c ucom_drv.c ucom_main.c

