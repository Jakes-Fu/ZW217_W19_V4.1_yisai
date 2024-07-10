#--------------------------------------------UAL_TELE_CALL SRC & INC-----------------------------------------------------------------#
include ./make/ual_tele/ual_tele_adap_call.mk
MINCPATH += $(MMI_DIR)/source/mmi_service/export/inc        #UAL_TODO:后续修改为ual路径
MINCPATH += $(MMI_DIR)/source/mmi_utility/h                 #UAL_TODO:后续修改为ual路径
MINCPATH += MS_Ref/export/inc
MINCPATH += RTOS/export/inc

MINCPATH += ual/inc
MINCPATH += ual/inc/tele
MSRCPATH += ual/src/tele/call
SOURCES  += ual_tele_call_control.c ual_tele_call_audio.c ual_tele_call_dtmf.c ual_tele_call_ecc.c call_data.c \
            call_state_manager.c call_service.c