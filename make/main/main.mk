
MCFLAG_OPT   	=


ifeq ($(strip $(MODEM_PLATFORM)), TRUE)
MCFLAG_OPT		+=-D_ATC_ONLY
endif

MSRCPATH =  common/source

SOURCES			= main.c
