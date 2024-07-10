ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
MCFLAG_OPT   	=  -DSPRD_SUPPORT_MCEX
endif


MINCPATH =  MS_Ref/source/scm/inc BASE/ps/export/inc

MSRCPATH =  MS_Ref/source/scm/src

SOURCES			= scm_api.c scm_devmgr.c scm_func.c scm_irp.c scm_task.c
