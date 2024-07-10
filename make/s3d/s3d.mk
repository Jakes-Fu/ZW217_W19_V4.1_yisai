
ifeq ($(strip $(RELEASE_INFO)),FALSE)
MCFLAG_OPT		+=  -DIMG_ASSERT_MODE
endif

#ifeq ($(strip $(PLATFORM)), SC6600L)
#MCFLAG_OPT              += -D_IMG_6600L
#endif


MINCPATH =  MS_Ref/source/display/inc \
            MS_Ref/source/isp_service/inc \
            MS_Ref/source/image_proc/inc \
            MS_Ref/source/display/src/graphics/inc \
            MS_Ref/source/display/src/special_effect/inc
                        
MINCPATH += chip_drv/export/inc/outdated
            
MSRCPATH =  MS_Ref/source/display/src/img MS_Ref/source/display/src/graphics/src MS_Ref/source/display/src/special_effect/src

MINCPATH += chip_drv/export/inc

ifeq ($(strip $(PLATFORM)),	SC8800G)
MINCPATH += chip_drv/chip_plf/export/inc
endif

ifeq ($(strip $(PLATFORM)),	SC8501C)
MINCPATH += chip_drv/chip_plf/export/inc
endif

MCFLAG_OPT     += -O2 -Otime 

SOURCES			= s3d_rotation_se.c
SOURCES			+= Graphic3D_CalcEngine.c