
MCFLAG_OPT = -DSDL -DEXTERNAL_SDL_DEBUG

include make/mmi_service/mmi_service_feature.mk

ifeq ($(strip $(SPRD_APP_RELEASE)), FALSE)
	include make/app_main/app_macro.mk
else
	include make/app_main/release_app_macro.mk
endif

#--------------------------------------------SRC & INC-----------------------------------------------------------------#

MINCPATH = $(MMI_DIR)/source/mmi_service/export/inc \
  $(MMI_DIR)/source/mmi_service/source/mmifsm/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvapi/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvaud/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvaud_test/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvaudioplayer/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvcc/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvcl/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvfmm/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvmgr/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvcommon/h \
  $(MMI_DIR)/source/mmi_service/source/mmisrvnv/h \


MINCPATH += $(MMI_DIR)/source/mmi_utility/h 
MINCPATH += $(MMI_DIR)/source/mmi_kernel/include
MINCPATH += $(MMI_DIR)/source/mmi_app/app/json_paser/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/connection/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/pdp/h
MINCPATH += $(MMI_DIR)/source/mmi_app/app/phone/h
MINCPATH += $(MMI_DIR)/source/mmi_app/common/h
MINCPATH += $(MMI_DIR)/source/mmi_app/kernel/h
ifeq ($(strip $(MSA_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_app/app/msa/h
endif
MINCPATH += $(MMI_DIR)/source/resource
MINCPATH += CAF/Template/include

MINCPATH += build/$(PROJECT)_builddir/tmp  

MINCPATH += $(ATC_INCPATH)
#MINCPATH += $(L4_INCPATH)
#MINCPATH += $(SIM_INCPATH)
MINCPATH += $(PS_INCPATH)
MINCPATH += $(L1_INCPATH)

MINCPATH += BASE/PS/export/inc
MINCPATH += BASE/Layer1/export/inc
MINCPATH += BASE/l4/export/inc 
MINCPATH += BASE/sim/export/inc 
#MINCPATH += BASE/PS/export/inc 
MINCPATH += DAPS/export/inc 
MINCPATH += chip_drv/export/inc/outdated 


MSRCPATH = $(MMI_DIR)/source/mmi_service/source/mmifsm/c \
  $(MMI_DIR)/source/mmi_service/source/mmisrvapi/c \
  $(MMI_DIR)/source/mmi_service/source/mmisrvaud/c \
  $(MMI_DIR)/source/mmi_service/source/mmisrvmgr/c \
  $(MMI_DIR)/source/mmi_service/source/mmisrvcommon/c
  
ifeq ($(strip $(CAMERAROLL_SUPPORT)), TRUE)
  $(MMI_DIR)/source/mmi_service/source/mmisrv_cameraroll/c
endif

ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
MSRCPATH += $(MMI_DIR)/source/mmi_service/source/mmirecordsrv/c 
endif

SOURCES  = mmisrv_fsm.c Mmisrvaud_api.c mmisrvaud.c Mmisrvaud_drv.c mmisrvaud_util.c Mmisrv_debug.c mmisrvmgr.c \
    mmi_common.c mmi_event_api.c mmi_event_manager.c mmicom_data.c mmicom_string.c mmicom_time.c datasrv_handle.c mmisrv_nv.c mmisrv_module.c

ifeq ($(strip $(CAMERAROLL_SUPPORT)), TRUE)
SOURCES += mmisrv_cameraroll_api.c mmisrv_cameraroll_main.c
endif
    
ifeq ($(strip $(RECORD_SUPPORT)), TRUE)    
SOURCES += mmisrvrecord.c
endif

MINCPATH += $(MMI_DIR)/source/mmi_service/source/mmi_base_data/inc
MSRCPATH += $(MMI_DIR)/source/mmi_service/source/mmi_base_data/src
SOURCES	 += mmi_arraylist.c mmi_dlist.c mmi_queue_new.c mmi_slist.c mmi_stack.c mmi_string.c \
			mmi_timedate_string.c mmi_tree_new.c

#支持Refactor示例代码
ifeq ($(strip $(MMI_REFACTOR_SAMPLE)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_service/source/srvsample/h
MSRCPATH += $(MMI_DIR)/source/mmi_service/source/srvsample/c    
SOURCES	 += srvsample_main.c mmi_slist.c
endif    


#audio play srv
ifeq ($(strip $(MMISRVAP_SUPPORT)), TRUE)
MINCPATH += $(MMI_DIR)/source/mmi_service/source/mmisrvap/h
MSRCPATH += $(MMI_DIR)/source/mmi_service/source/mmisrvap/c

SOURCES += mmisrvap_fsm.c mmisrvap_iup.c mmisrvap_idown.c mmisrvap_init.c
endif

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
else
MINCPATH   += DAPS/export/inc/tcpip
endif