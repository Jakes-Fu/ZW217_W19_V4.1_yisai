
MCFLAG_OPT   	= -Otime

ifeq ($(strip $(PLATFORM)), SC8800D)
MCFLAG_OPT 		+= -D_REF_SC8800D_ -DDSP_AUDIO_II
endif

ifeq ($(strip $(PLATFORM)), SC6600R)
MCFLAG_OPT		+= -DDSP_AUDIO_II 
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT		+= -DDSP_AUDIO_III
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
MCFLAG_OPT		+= -DDSP_AUDIO_III
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MCFLAG_OPT		+= -DDSP_AUDIO_III
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT		+= -DDSP_AUDIO_III
endif

ifeq ($(strip $(PLATFORM)), SC8800H)
MCFLAG_OPT		+= -DDSP_AUDIO_III 
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT		+= -DDSP_AUDIO_III 
endif

MINCPATH =  MS_Ref/source/aud_exp/agc/inc MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH += chip_drv/export/inc/outdated

MSRCPATH =  MS_Ref/source/aud_exp/agc/src

SOURCES			= aud_agc.c agc_exp.c aud_proc.s agc_digitalgain.c


ifeq ($(strip $(PLATFORM)), SC6600L)
     MCFLAG_OPT   	+= -D_AGC_FUNC_IN_IRAM_
endif

