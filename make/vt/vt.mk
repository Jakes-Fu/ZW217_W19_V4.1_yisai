
ifeq ($(strip $(PLATFORM)), SC8800D)
MINCPATH =  RTOS/export/inc \
 MS_Customize/source/product/driver/ap_ext/inc \
 MS_Ref/export/inc \
 MS_Ref/source/ap/inc \
 MS_Ref/source/base/inc

MSRCPATH =  MS_Ref/source/vt/src

SOURCES = 	ap_vt.c \
			ap_vt_codec.c
endif
								
