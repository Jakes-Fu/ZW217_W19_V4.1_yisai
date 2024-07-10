
MCFLAG_OPT   	= -Otime
MASMFLAG_OPT	= $(CHECKREGLIST) -g

ifeq ($(strip $(PLATFORM)), 8800H)
MCFLAG_OPT   	+= -DCHIP_DRV_SC8800H
endif

ifeq ($(strip $(PLATFORM)), 6800)
MCFLAG_OPT   	+= -DCHIP_DRV_SC6800D
endif

MINCPATH =  MS_Ref/source/scaling/inc

MSRCPATH =  MS_Ref/source/scaling/src

SOURCES			= isp_scaling_horizontal.s isp_scaling_horizontal2.s isp_scaling_vertical.s isp_scaling_vertical2.s \
					arm_scaledown.c
