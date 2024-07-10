

MCFLAG_OPT   	= -Otime -zc

#MCFLAG_OPT   	+= -DWAV_ASSERT_MODE

ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT   	+= -D_WAV_FUNC_IN_IRAM_ -D_WAV_STATIC_THREAD_
endif

ifeq ($(strip $(PLATFORM)), SC8800H)
MCFLAG_OPT   	+= -D_NOT_CHANGE_FREQ_
endif

MINCPATH =  Layer1/source/h MS_Ref/source/codec/wav/inc MS_Ref/export/inc \
 MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc BASE/ps/export/inc
MINCPATH +=$(L1_INCPATH)
MINCPATH += chip_drv/export/inc/outdated 					
MSRCPATH =  MS_Ref/source/codec/wav/src
SOURCES			= wav.c wav_adp.c wav_ffs.c


