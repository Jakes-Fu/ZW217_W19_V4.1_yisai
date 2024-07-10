
ifeq ($(strip $(RELEASE_INFO)),FALSE)
MCFLAG_OPT		+=  -DIMG_ASSERT_MODE
endif

MCFLAG_OPT     += -O2 -Otime 


MINCPATH		=  nes/inc
MINCPATH		+=  nes

MSRCPATH		=  nes/infoNES
MSRCPATH		+=  nes

MINCPATH		+=  $(MMI_DIR)/source/mmi_kernel/include
MINCPATH		+=  CAF/Template/include
MINCPATH		+=  $(MMI_DIR)/source/mmi_app/common/h
MINCPATH		+=  $(MMI_DIR)/source/mmi_gui/include
MINCPATH		+=  $(MMI_DIR)/source/mmi_ctrl/include
MINCPATH		+=  l4/export/inc
MINCPATH		+=  $(MMI_DIR)/source/mmi_app/app/setting/h


SOURCES		=  InfoNES.c InfoNES_API.c InfoNES_Mapper.c InfoNES_Palette.c InfoNES_pAPU.c InfoNES_system.c K6502.c xnes_init.c






                
       

         