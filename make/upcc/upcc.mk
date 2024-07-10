
MCFLAG_OPT   	= 

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

ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT		+= -DCHIP_DRV_SC8800H
endif

MINCPATH =  MS_Ref/source/upcc/inc MS_Ref/source/isp_service/inc MS_Ref/source/img/inc MS_Ref/source/image_proc/inc 
MINCPATH += chip_drv/export/inc/outdated

MSRCPATH =  MS_Ref/source/upcc/src

SOURCES			= upcc_camera.c upcc_config.c upcc_drv.c  upcc_main.c upcc_request.c
