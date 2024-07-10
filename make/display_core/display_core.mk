MCFLAG_OPT     += -O2 -Otime 

ifeq ($(strip $(RELEASE_INFO)),FALSE)
MCFLAG_OPT		+=  -DIMG_ASSERT_MODE
endif

#ifeq ($(strip $(PLATFORM)), SC6600L)
#MCFLAG_OPT              += -D_IMG_6600L
#endif

MINCPATH =  MS_Ref/source/display/inc   \
						MS_Ref/source/image_proc/inc \
						chip_drv/export/inc/outdated           

MSRCPATH =  MS_Ref/source/display/src/img 

SOURCES	= bmp_decoder.c \
          bmp_encoder.c \
          bmp_io.c \
          img_port.c \
          alpha_res_decoder.c \
          gif_res_decoder.c \
          img_res_decoder.c \
          jpeg_res_table.c jpeg_res_converter.c \
          jpeg_res_decoder.c		                        
