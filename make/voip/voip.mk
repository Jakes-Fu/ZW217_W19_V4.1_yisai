
MCFLAG_OPT = -Otime

#MCFLAG_OPT +=-DVOICE_ASSERT_MODE

ifeq ($(strip $(MODEM_PLATFORM)), TRUE)
MCFLAG_OPT +=-DMODEM_RECORD_DEBUG_DATA_PARA
endif
MINCPATH = MS_Ref/source/codec/voip/inc MS_Ref/export/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH +=$(L1_INCPATH)

MINCPATH += chip_drv/export/inc
MSRCPATH = MS_Ref/source/codec/voip/src

SOURCES = voip_adp.c