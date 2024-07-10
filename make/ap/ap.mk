
MCFLAG_OPT   	= 

ifeq ($(strip $(PLATFORM)), SC8800D)
MINCPATH +=  MS_Ref/source/ap/inc
MSRCPATH =  MS_Ref/source/ap/src

endif

SOURCES = ap_task.c 
