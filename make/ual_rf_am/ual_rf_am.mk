#--------------------------------------------UAL RF_AM SRC & INC----------------------------------------------------------#
MINCPATH += ual/inc
MINCPATH += ual/inc/tele
MINCPATH += ual/src/cms
MINCPATH += ual/src/tele
MINCPATH += ual/src/tele/call
MINCPATH += ual/src/gnss
#MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp_6121E/h
MINCPATH += BASE/l4/export/inc
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += CAF/Template/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
MINCPATH += ual/src/rf_am
MSRCPATH += ual/src/rf_am
SOURCES  += ual_rf_am.c uil_rf_am_data.c uil_rf_am_fsm_background_position.c uil_rf_am_fsm_communication.c \
            uil_rf_am_fsm_fun.c uil_rf_am_fsm_main.c uil_rf_am_fsm_sport_position.c uil_rf_am_mgr.c uil_rf_am_util.c