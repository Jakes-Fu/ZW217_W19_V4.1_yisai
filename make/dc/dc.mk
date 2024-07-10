ifeq ($(strip $(PLATFORM)), SC6600L)
MINCPATH = MS_Ref/source/dc/dc_6x00/inc \
           MS_Ref/source/dc/dc_common/inc \
           MS_Include/chip_drv \
           MS_Ref/source/isp_service/inc \
           BASE/ps/export/inc

MSRCPATH = MS_Ref/source/dc/dc_common/src \
           MS_Ref/source/dc/dc_6x00/src
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
MINCPATH = MS_Ref/export/inc \
           MS_Ref/source/dc/dc_6x00/inc \
           MS_Ref/source/dc/dc_common/inc \
           MS_Include/chip_drv \
           MS_Ref/source/isp_service/inc
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc

MSRCPATH = MS_Ref/source/dc/dc_common/src \
           MS_Ref/source/dc/dc_6x00/src
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MINCPATH   		= MS_Ref/source/dc/dc_common/inc MS_Ref/source/image_proc/src/jpeg_codec/inc MS_Ref/source/isp_service/inc
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc
MSRCPATH			= MS_Ref/source/dc/dc_common/src MS_Ref/source/dc/dc_8800h/src 
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MINCPATH   		= MS_Ref/source/dc/dc_common/inc MS_Ref/source/image_proc/src/jpeg_codec/inc MS_Ref/source/isp_service/inc
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc
MSRCPATH			= MS_Ref/source/dc/dc_common/src MS_Ref/source/dc/dc_8800h/src 
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH   		= MS_Ref/source/dc/dc_common/inc MS_Ref/source/image_proc/src/jpeg_codec/inc MS_Ref/source/isp_service/inc
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc
MSRCPATH			= MS_Ref/source/dc/dc_common/src MS_Ref/source/dc/dc_8800h/src 
endif

ifeq ($(strip $(PLATFORM)), SC8800H)
MCFLAG_OPT   	+= -D_VSP_ -DBIG_ENDIAN 

MINCPATH   		= MS_Ref/source/dc/dc_8800h/inc MS_Ref/source/dc/dc_common/inc MS_Ref/source/image_proc/src/jpeg_codec/inc MS_Ref/source/isp_service/inc BASE/ps/export/inc

MSRCPATH			= MS_Ref/source/dc/dc_common/src MS_Ref/source/dc/dc_8800h/src 
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT   	+= -D_VSP_ -DBIG_ENDIAN 

MINCPATH   		= MS_Ref/source/dc/dc_common/inc MS_Ref/source/image_proc/src/jpeg_codec/inc MS_Ref/source/isp_service/inc
MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc
MSRCPATH			= MS_Ref/source/dc/dc_common/src MS_Ref/source/dc/dc_8800h/src 
endif

MINCPATH += MS_Ref/source/lcd/inc \
            MS_Ref/source/image_proc/inc \
            MS_Ref/source/image_proc/src/jpeg/inc \
            MS_Ref/source/dc/include \
            MS_Ref/source/scp/inc \
            MS_Customize/export/inc
            
MSRCPATH += MS_Ref/source/dc/sensor

ifeq ($(strip $(PLATFORM)), SC6600L)
SOURCES = dc_app.c \
          dc_framemgr.c \
          dc_bufmgr.c \
          dc_lcdc.c \
          dc_preview.c \
          dc_misc.c \
          dc_snapshot.c \
          dc_codec.c \
          dc_review.c \
          dc_param_ctrl.c
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
SOURCES = dc_app.c \
          dc_framemgr.c \
          dc_bufmgr.c \
          dc_lcdc.c \
          dc_preview.c \
          dc_misc.c \
          dc_snapshot.c \
          dc_codec.c \
          dc_review.c \
          dc_param_ctrl.c \
          dc_atv.c
endif

ifeq ($(strip $(PLATFORM)), SC6530)
SOURCES				= dcamera_app.c	jpeg.c dcamera_mem.c dcamera_misc.c
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES				= dcamera_app.c	jpeg.c dcamera_mem.c dcamera_misc.c
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES				= dcamera_app.c	jpeg.c dcamera_mem.c dcamera_misc.c dcamera_cfg.c dcamera_30W_128x128.c dcamera_30W_240x240.c dcamera_8W_128x128.c dcamera_8W_240x240.c
endif


ifeq ($(strip $(PLATFORM)), SC8800H)
ifeq ($(strip $(CHIP_VER)), 8800H5)
SOURCES				= dcamera_app.c	jpeg.c
else
SOURCES				= dcamera_app.c	jpeg.c jpg_common.c jpg_dctxidct.c jpg_decode.c jpg_encode.c \
                      jpg_global.c jpg_huff.c jpg_init.c jpg_readjfif.c jpg_stream.c jpg_quant.c \
                      jpg_tabledef.c jpg_writejfif.c 
endif
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
SOURCES				= dcamera_app.c	jpeg.c dcamera_mem.c dcamera_misc.c
#SOURCES = dcamera_app_dummy.c
endif

ifneq ($(strip $(ATV_SUPPORT)), NONE)  
SOURCES += dcamera_atv.c
endif

SOURCES += sensor_drv.c \
           dc_common.c \
           dc_cfg.c
           
ifeq ($(strip $(PLATFORM)), SC6530)
#MCFLAG_OPT   	+= -D_DC_EXIF_        
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
#MCFLAG_OPT   	+= -D_DC_EXIF_        
endif       

ifeq ($(strip $(PLATFORM)), UWS6121E)
#MCFLAG_OPT   	+= -D_DC_EXIF_        
endif     


MCFLAG_OPT   	+= -D_DC_WRONG_SENSOR_COMPATIBLE_        

