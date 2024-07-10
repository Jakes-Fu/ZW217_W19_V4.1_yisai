ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_SUPPORT_GPRS_
else
MCFLAG_OPT   	+= -DSIM_CODE_COMPACT
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT   	+= -DSIM_ENHANCED_PPS_SUPPORT -DSIM_3G_USIM_SUPPORT -DSIM_CDMA_CARD_SUPPORT -DSIM_CODE_COMPACT
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
MCFLAG_OPT   	+=-DPLATFORM_SC6800H  -DSIM_ENHANCED_PPS_SUPPORT  -DSIM_3G_USIM_SUPPORT
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MCFLAG_OPT   	+= -DSIM_ENHANCED_PPS_SUPPORT -DSIM_CODE_COMPACT -D_CPHS_SUPPORT
ifeq ($(strip $(ORANGE_SUPPORT)), TRUE)
else
MCFLAG_OPT   	+= -DSIM_3G_USIM_SUPPORT
endif
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT   	+= -DSIM_ENHANCED_PPS_SUPPORT -DSIM_CODE_COMPACT -D_CPHS_SUPPORT
ifeq ($(strip $(ORANGE_SUPPORT)), TRUE)
else
MCFLAG_OPT   	+= -DSIM_3G_USIM_SUPPORT
endif
endif

ifeq ($(strip $(MODEM_PLATFORM)), TRUE)
MCFLAG_OPT += -DSIM_AOC_SUPPORT	
endif

ifeq ($(strip $(ORANGE_SUPPORT)), TRUE) 
MCFLAG_OPT += -DSIM_AOC_SUPPORT
endif

ifeq ($(strip $(MULTI_SIM_SYS)), QUAD) 
MCFLAG_OPT += -DSIM_6302_SWITCH
endif

ifeq ($(strip $(MULTI_SIM_SYS)), TRI)
ifeq ($(strip $(PLATFORM)), SC6531EFM) 
MCFLAG_OPT += -DSIM_6302_SWITCH
endif
endif
 
MINCPATH =  BASE/sim/source/h BASE/sim/export/inc BASE/PS/export/inc BASE/Layer1/export/inc char_lib/export/inc BASE/l4/export/inc
MINCPATH += chip_drv/export/inc/outdated
MSRCPATH =  BASE/sim/source/c

SOURCES			= sim_instruction.c sim_mm_signal.c  sim_ser_procedure.c    \
					sim_sms.c sim_ef_encode.c sim_file_system.c sim_signal.c sim_ef_decode.c \
					sim_dialing_number.c sim_main.c sim_hal.c
