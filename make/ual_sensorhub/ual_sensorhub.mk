#--------------------------------------------UAL_SENHUB SRC & INC-----------------------------------------------------------------#
ifeq ($(strip $(GOODIX_SUPPORT)), GR5515)
	MINCPATH += MS_Ref/export/inc/sensor
	MINCPATH += Third-party/goodix/inc
endif

MINCPATH += ual/inc
MINCPATH += ual/src/cms

MSRCPATH += ual/src/sensorhub
SOURCES  += ual_sensorhub.c
