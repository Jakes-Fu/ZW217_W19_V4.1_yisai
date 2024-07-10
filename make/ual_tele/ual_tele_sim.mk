include ./make/ual_tele/ual_tele_adap_sim.mk
#--------------------------------------------UAL_TELE_SIM SRC & INC-----------------------------------------------------------------#
MINCPATH += ual/inc
MINCPATH += ual/inc/tele ual/src/tele/sim ual/src/cms

MSRCPATH += ual/src/tele/sim
SOURCES  += ual_tele_sim.c