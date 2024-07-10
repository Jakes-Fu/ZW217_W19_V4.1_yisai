
ifeq ($(strip $(MODEM_TYPE)), ZBX)
	MCFLAG_OPT += -DMODEM_ZBX_MUX -DMUX_ATCIPSEND -D_MUX_DEBUG_
endif

MCFLAG_OPT   	+= -DMUX_ENABLE -D_RETARGET_HEAP_ -D_REF_SC8800H_ -D_RTOS -D_MUX_ENABLE_ -D_DEBUG -D_MUX_MODEM_


MINCPATH =  MS_Ref/source/mux/inc  chip_drv/source/h  l4/export/inc $(strip $(ATC_PATH))/source/h
MINCPATH += $(strip $(ATC_INCPATH))
MINCPATH += chip_drv/export/inc/outdated		
MSRCPATH =  MS_Ref/source/mux/src

SOURCES			= mux_api.c mux_common.c mux_ctl.c mux_driver.c mux_recv.c mux_sleep.c