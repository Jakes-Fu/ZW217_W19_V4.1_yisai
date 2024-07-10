ifeq ($(strip $(NOR_BOOTLOADER_SUPPORT)),    TRUE)
MINCPATH      = MS_Ref/source/spzip/inc  RTOS/export/inc  chip_drv/export/inc/outdated
else
MINCPATH      = MS_Ref/source/spzip/inc  RTOS/export/inc  chip_drv/export/inc/outdated
endif
MSRCPATH      = MS_Ref/source/spzip/src 

SOURCES       =   spzip_1.c


