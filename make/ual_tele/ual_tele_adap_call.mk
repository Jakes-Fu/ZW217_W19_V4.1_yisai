#--------------------------------------------UAL SRC & INC-----------------------------------------------------------------#
ifeq ($(strip $(MODEM_SUPPORT)), SPRD)
MINCPATH += MSL/l4/inc
endif
ifeq ($(strip $(MODEM_SUPPORT)), RDA)
MINCPATH += BASE/l4/export/inc
endif

MINCPATH += ual/src/tele
MINCPATH += ual/src/tele/call
MSRCPATH += ual/src/tele/call/adap_call
ifeq ($(strip $(MODEM_SUPPORT)), SPRD)
SOURCES  += adap_call_sprd_mn.c
endif
ifeq ($(strip $(MODEM_SUPPORT)), RDA)
SOURCES  += adap_call_rda_mn.c
endif