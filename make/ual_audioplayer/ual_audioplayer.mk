#--------------------------------------------UAL_AUDIOPLAYER SRC & INC-----------------------------------------------------------------#

MINCPATH += ual/inc
MINCPATH += ual/src/cms
MINCPATH += ual/src/networkmgr
MINCPATH += ual/inc/tele
# UAL_TODO:
MINCPATH += $(MMI_DIR)/source/mmi_service/export/inc
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += $(MMI_DIR)/source/mmi_utility/h
MINCPATH += CAF/Template/include
MINCPATH += DAPS/export/inc
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp_6121E/h
MINCPATH += $(MMI_DIR)/source/mmi_app/kernel/h
MINCPATH += BASE/PS/export/inc
MINCPATH += BASE/l4/export/inc
MINCPATH += BASE/sim/export/inc
MINCPATH += BASE/atc/export/inc

MINCPATH += ual/src/audioplayer
MSRCPATH += ual/src/audioplayer
SOURCES  += ual_audioplayer.c uil_audioplayer_manager_data.c uil_audioplayer_manager.c uil_audiosource_streamsave.c uil_audioplayer_function.c uil_audioplayer_fsm.c uil_audiosource_stream_download.c uil_audiosource_control.c


ifeq ($(strip $(AUDIOPLAYER_IT_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/cc/h
MINCPATH += $(MMI_DIR)/source/mmi_app/common/h
MINCPATH += $(MMI_DIR)/source/mmi_app/kernel/h
MINCPATH += msl/l4/inc
MINCPATH += msl/sim/inc
MINCPATH += msl/gas/inc
MINCPATH += msl/atc/inc
MINCPATH += ual/src/audioplayer_cases/audioplayer_it_cases
MSRCPATH += ual/src/audioplayer_cases/audioplayer_it_cases
SOURCES  += ual_audioplayer_it_test_main.c  ual_audioplayer_it_api.c
SOURCES  += ual_audioplayer_it_cases.c
endif

