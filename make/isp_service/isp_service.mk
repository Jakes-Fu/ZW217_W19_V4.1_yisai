
MCFLAG_OPT   	= 
MINCPATH =  MS_Ref/source/isp_service/inc \
            MS_Ref/source/export/inc \
            MS_Ref/source/lcd/inc \
            MS_Ref/source/image_proc/inc \
            MS_Ref/source/image_proc/src/isp/inc\
			MS_Ref/source/dc/include
			
            
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc                
            
MSRCPATH =  MS_Ref/source/isp_service/src MS_Ref/source/isp/src

ifeq ($(strip $(PLATFORM)), SC8800H)
SOURCES			+= isp_service_sc8800h.c 
endif

ifeq ($(strip $(LCD_FMARK_SUPPORT)), TRUE)
ifeq ($(strip $(PLATFORM)), SC8800H)
MCFLAG_OPT	+=  -DUSE_FMARK
SOURCES += fmark_ctrl.c
endif
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
SOURCES			+= isp_service_sc8800g.c 
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
SOURCES			+= isp_service.c
endif
ifeq ($(strip $(PLATFORM)), SC6800H)
SOURCES			+= isp_service.c
endif
ifeq ($(strip $(PLATFORM)), SC6530)
SOURCES			+= isp_service_sc6530.c
endif
ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES			+= isp_service_sc6530.c
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES			+= isp_service_uws6121e.c
endif