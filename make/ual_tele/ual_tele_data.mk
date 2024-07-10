#--------------------------------------------UAL_TELE_DATA SRC & INC-----------------------------------------------------------------#
MINCPATH += MSL/atc/inc
MINCPATH += CAF/Template/include
ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp_6121E/h
endif
MINCPATH += ual/src/cms
MINCPATH += ual/inc
MINCPATH += ual/inc/tele
MSRCPATH += ual/src/tele/data
SOURCES  += ual_tele_data.c
