
ifeq ($(strip $(PLATFORM)), ROCKY)

MCFLAG_OPT              = -D_IMG_6600R

endif

ifeq ($(strip $(RELEASE_INFO)),FALSE)
MCFLAG_OPT		+=  -DIMG_ASSERT_MODE
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT              += -D_IMG_6600L
endif


MINCPATH =  MS_Ref/source/lcd/inc \
            MS_Ref/source/isp_service/inc \
            MS_Ref/source/image_proc/inc
           
            
MSRCPATH =  MS_Ref/source/lcd/src

MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc

ifeq ($(strip $(PLATFORM)),	SC8800G)
MINCPATH += chip_drv/chip_plf/export/inc
endif


MCFLAG_OPT              += -O2 -Otime 

SOURCES			= layer_blend.c


ifeq ($(strip $(PLATFORM)),    SC6600L)
SOURCES += lcd_sc6600l.c
endif 

ifeq ($(strip $(PLATFORM)),    SC6800H)
SOURCES += lcd.c  lcdc_app.c lcdc_drv.c 
endif                 

ifeq ($(strip $(PLATFORM)),    SC6530)
SOURCES += lcd.c  lcdc_app.c lcdc_drv_sc6530.c 
endif   

ifeq ($(strip $(PLATFORM)),    SC6531EFM)
SOURCES += lcd.c  lcdc_app.c lcdc_drv_sc6531efm.c 
endif

ifeq ($(strip $(PLATFORM)),    UWS6121E)
MINCPATH += chip_drv/chip_module/lcdc/uws6121e
SOURCES += lcd_uws6121e.c
endif

ifeq ($(strip $(PLATFORM)), SC8800H)        
SOURCES +=      lcd_sc8800h.c lcdc.c lcdc_rgb.c  lcdc_mcu.c        
endif   

ifeq ($(strip $(PLATFORM)),    SC8800G)
SOURCES += lcd.c  lcdc_app.c lcdc_drv.c 
endif            

SOURCES += lcdgui_ctrl.c lcdref_ctrl.c lcd_ctrl_common.c lcd_task.c lcd_interface.c  

MCFLAG_OPT += -DOSD_LAYER_BLEND
SOURCES +=  osd_rgb888_blender.c   