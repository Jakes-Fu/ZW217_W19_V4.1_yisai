MCFLAG_OPT   	= -Otime

ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT   	+=
endif

ifeq ($(strip $(BT_A2DP_SUPPORT)), TRUE)
ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT   	+= -D_AMR_DSP_ENABLE_SBC
endif
ifeq ($(strip $(PLATFORM)), SC6800H)
MCFLAG_OPT   	+= -D_AMR_DSP_ENABLE_SBC
endif
endif

MINCPATH   		= MS_Ref/export/inc \
								MS_Ref/source/codec/amr_dsp/inc \
								MS_Ref/source/codec/amr_dsp/inc_export \
								MS_Ref/source/audioservice/inc_export \
								MS_Ref/source/audioservice/inc \
								BASE/ps/export/inc
MINCPATH +=$(L1_INCPATH) 	

MSRCPATH		= MS_Ref/source/codec/amr_dsp/src

SOURCES			= amr_dsp_adp.c 
