
ifeq ($(strip $(PLATFORM)), SC8800H)
MINCPATH = MS_Ref/export/inc dm_2g_layer1/export/inc

MSRCPATH = MS_Ref/source/codec/vt_adp/src

SOURCES = vt_adp.c
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MINCPATH = MS_Ref/export/inc dm_2g_layer1/export/inc

MSRCPATH = MS_Ref/source/codec/vt_adp/src

SOURCES = vt_adp.c
endif
								
