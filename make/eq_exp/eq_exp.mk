
MCFLAG_OPT   	= -Otime

ifeq ($(strip $(PLATFORM)),	SC6600L)
MCFLAG_OPT	+= -D_EQ_FUNC_IN_IRAM_ -D_EQ_LOUD_PARAM_FROM_NV_
endif

ifeq ($(strip $(PLATFORM)),	SC6600R)
MCFLAG_OPT	+= -D_EQ_FUNC_IN_IRAM_
endif

MCFLAG_OPT += -DEQ_EXP_ASSERT_MODE

MINCPATH =  MS_Ref/source/aud_exp/eq/inc MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc

MSRCPATH =  MS_Ref/source/aud_exp/eq/src

SOURCES			= eq_exp.c
