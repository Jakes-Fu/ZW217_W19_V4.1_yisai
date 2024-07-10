
MCFLAG_OPT   	= 

ifeq ($(strip $(PLATFORM)), SC6600R)

MINCPATH =  MS_Ref/source/dc/dc_6600r/inc MS_Ref/source/dc_algorithm/inc MS_Ref/source/dc/dc_common/inc

MSRCPATH =  MS_Ref/source/dc_algorithm/src MS_Ref/source/dc_algorithm/src/digital_zoom/qvga \
 MS_Ref/source/dc_algorithm/src/digital_zoom/vga

SOURCES				= dc_algorithm.c dc_jhuff.c dc_jquant.c dc_jrdfif.c dc_jwrfif.c dc_wqvga_scale.c dc_wqvga_preview_algorithm.c

# ifeq ($(strip $(LOW_MEMORY_SUPPORT)), FALSE)
MCFLAG_OPT	  += -DDC_ZOOM_SUPPORT
MCFLAG_OPT   	+= -DDC_WQVGA_SUPPORT
MCFLAG_OPT   	+= -DDC_INTERPOLATION_SUPPORT
MCFLAG_OPT   	+= -DDC_ABOVE2M_SUPPORT
# endif 

endif




	
