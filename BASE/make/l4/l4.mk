
MCFLAG_OPT   	=-D_CPHS_SUPPORT 

MCFLAG_OPT   	+=-D__MN_ADP_CFW__
MCFLAG_OPT   	+=-D__MN_PHONE_ADP_CFW__
MCFLAG_OPT   	+=-D__MN_SMS_ADP_CFW__
MCFLAG_OPT   	+=-D__MN_CALL_ADP_CFW__
MCFLAG_OPT   	+=-D__MN_GPRS_ADP_CFW__
MCFLAG_OPT   	+=-D__MN_SIM_ADP_CFW__
MCFLAG_OPT   	+=-D__MN_SS_ADP_CFW__
MCFLAG_OPT   	+=-D__VOLTE_HANDSHAKE_SUPPORT__

# MCFLAG_OPT   	+= -DMNPHONE_ADDITIONAL_FEATURE
# MCFLAG_OPT   	+= -DMNSMS_ADDITIONAL_FEATURE
# MCFLAG_OPT   	+= -DMNCALL_ADDITIONAL_FEATURE
# MCFLAG_OPT   	+= -DMNSMS_REPLACE_SMS
# MCFLAG_OPT   	+= -D_DATA_COMPRESS
# ifeq ($(strip $(ECT_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_SUPPORT_ECT_
# endif
# MCFLAG_OPT   	+= -D_SUPPORT_CNAP_

#open this macro will affect the electricity current, so close this macro by default
# MCFLAG_OPT   	+= -DCHECK_DSP_SUPPORT

ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_SUPPORT_GPRS_
# MCFLAG_OPT   	+= -D_GPRS_FULL_FUNCTION_
endif

#dont remove this macro define, because l4 and PS have the common header file "gsm_gprs_type.h",
#and there is a macro _SUPPORT_PBCCH_ included in the struct CELL_INFO_T
ifneq ($(strip $(PBCCH_REMOVE_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_SUPPORT_PBCCH_
endif

ifeq ($(strip $(ATC_SUPPORT)),	COMPACT)
MCFLAG_OPT   	+= -D_BASE_COMPACT_CODE_
endif

#this macro is also defined in ps.mk,you should turn on/off this macro in the l4.mk and ps.mk at the same time
#MCFLAG_OPT   	+= -D_SUPPORT_CCBS_
MCFLAG_OPT   	+= -D_ENH_MEAS_SUPPORT_

# future
# ifeq ($(strip $(TCP_HEADER_COMPRESS_SUPPORT)), TRUE)
# MCFLAG_OPT      += -D_TCP_HEADER_COMPRESS
# endif


ifeq ($(strip $(PCLINK_SUPPORT)), TRUE)
MCFLAG_OPT += -DPCLINK_ENABLE	
endif

ifeq ($(strip $(MUX_SUPPORT)), TRUE)
MCFLAG_OPT   	+= -D_MUX_REDUCED_MODE_
endif

ifeq ($(strip $(MODEM_MODE)), AP_PLUS_BP)
MCFLAG_OPT   	+= -D_ATC_ONLY
endif

ifeq ($(strip $(SIM_LOCK_SUPPORT)), TRUE)
MCFLAG_OPT += -D_SUPPORT_SIM_LOCK_
ifeq ($(strip $(PLATFORM)), UWS6121E)
MCFLAG_OPT += -DDUALSIM_SIMLOCK_SUPPORT
endif
endif


#samsung two macro description:
#GSM_CUSTOMER_AFP_SUPPORT:samsung 6530 customize L4/SIM codes by india engineer
#_SUPPORT_IPC_ :samsung 6500 G+C project IPC support mode codes

ifeq ($(strip $(CUSTOMER_NAME)), A)
MCFLAG_OPT += -DGSM_CUSTOMER_AFP_SUPPORT
endif

#ifeq ($(strip $(SIMAT_BIP_SUPPORT)), TRUE)
MCFLAG_OPT += -DSIMAT_OPEN_CHANNEL_SUPPORT
MCFLAG_OPT += -DSIMAT_BROWSER_SUPPORT
#endif

ifeq ($(strip $(MODEM_PLATFORM)), TRUE)
MCFLAG_OPT += -DMN_AOC_SUPPORT	
endif

ifeq ($(strip $(ORANGE_SUPPORT)), TRUE) 
MCFLAG_OPT += -DMN_AOC_SUPPORT
endif 

ifeq ($(strip $(MODEM_GPRS_FULL)), TRUE)
MCFLAG_OPT += -D_GPRS_FULL_FUNCTION_
endif

ifeq ($(strip $(NETLOCK_SUPPORT)), TRUE)
MCFLAG_OPT += -DNET_LOCK_SUPPORT
endif

ifeq ($(strip $(C_IMEI_SUPPORT)), TRUE)
MCFLAG_OPT += -D_C_IMEI_SUPPORT
endif

ifeq ($(strip $(GPS_SUPPORT)), TRUE)
ifeq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
MCFLAG_OPT += -D_MNGPS_SUPPORT_
endif
endif

ifeq ($(strip $(WIFI_SUPPORT)),	UNISOC_RX)
MCFLAG_OPT   	+= -D_MNWIFI_SUPPORT_
endif

MINCPATH =  BASE/l4/source/arm/call/h BASE/l4/source/arm/datamag/h BASE/l4/source/arm/gprs/h BASE/l4/source/arm/mnsms/h BASE/l4/source/arm/mnsmscb/h \
 BASE/l4/source/arm/mnss/h BASE/l4/source/arm/phone/h BASE/PS/export/inc BASE/Layer1/export/inc BASE/sim/export/inc \
 BASE/l4/export/inc BASE/l4/source/arm/mnstk char_lib/export/inc BASE/l4/source/arm/mnstk/h BASE/l4/source/arm/eng/h BASE/l4/source/arm \
 BASE/PS/source/smc/inc  BASE/PS/source/smr/inc \
 BASE/PS/source/gmm/h BASE/PS/source/llc/h BASE/PS/source/mm/h BASE/PS/source/sm/h BASE/atc/export/inc

ifeq ($(strip $(GPS_SUPPORT)), TRUE)
ifeq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
MINCPATH += BASE/l4/source/arm/mngps/h
endif
endif

ifeq ($(strip $(WIFI_SUPPORT)), UNISOC_RX)
MINCPATH += BASE/l4/source/arm/mnwifi/h
MINCPATH += MS_Ref/export/inc/
endif

MINCPATH += chip_drv/export/inc/outdated

ifeq	($(strip $(CARD_LOG)), TRUE)
MINCPATH +=  BASE/logsave/source/h	 BASE/logsave/export/inc
endif

MSRCPATH =  BASE/l4/source/arm/call BASE/l4/source/arm/datamag BASE/l4/source/arm/gprs BASE/l4/source/arm/mnsms BASE/l4/source/arm/mnsmscb \
 BASE/l4/source/arm/mnss BASE/l4/source/arm/phone BASE/l4/source/arm BASE/l4/source/arm/nvitem_ps/c BASE/l4/source/arm/eng BASE/l4/source/arm/mnstk

ifeq ($(strip $(GPS_SUPPORT)), TRUE)
ifeq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
MSRCPATH         += BASE/l4/source/arm/mngps 
endif
endif

ifeq ($(strip $(WIFI_SUPPORT)), UNISOC_RX)
MSRCPATH += BASE/l4/source/arm/mnwifi 
endif

SOURCES				= mneng_api.c mneng_module.c \
                      mnphone_module.c mnphone_api.c mnphone_signal.c mnphone_event.c \
                      mncall_aoc.c mncall_event.c mncall_api.c mncall_module.c mncall_dtmf.c mncall_signal.c \
					  mnsms_api.c mnsms_event.c mnsms_module.c mnsms_signal.c \
					  mnsmscb_api.c mnsmscb_event.c mnsmscb_module.c \
					  mnss_msg.c ssbasicfunc.c mnss_api.c mnss_module.c sscallrelate.c mnss_event.c mnss_signal.c \
					  simat_envelope.c simat_api.c simat_imple.c simat_main.c simat_confirm.c simat_proactive.c simat_do_decode.c simat_profile.c simat_do_encode.c \
					  app_mnnv_api.c app_phonebook_api.c \
					  mnsim_api.c app_mnsim_api.c mnsim_event.c mnclassmark_api.c mnsim_module.c mnsim_signal_api.c \
					  mnnv_api.c  mn_main.c mn_conv.c nvitem_init.c mnsms_nv_module.c l4_version.c
							 

ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
SOURCES         += mngprs_data.c mngprs_module.c mngprs_signal.c mngprs_api.c mngprs_event.c mngprs_nv.c 
endif

ifeq ($(strip $(SIM_LOCK_SUPPORT)), TRUE)
ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH   += MS_Ref/source/simlock/inc
endif
SOURCES         += mnphone_simlock.c
endif

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif

ifeq ($(strip $(GPS_SUPPORT)), TRUE)
ifeq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
SOURCES         += mngps_module.c mngps_signal.c mngps_api.c mngps_event.c
endif
endif

ifeq ($(strip $(WIFI_SUPPORT)), UNISOC_RX)
SOURCES += mnwifi_module.c mnwifi_signal.c mnwifi_api.c mnwifi_event.c
endif

ifeq ($(strip $(SIMAT_TEST_SUPPORT)), TRUE)
MCFLAG_OPT += -DSTK_DEBUG_SELF_TEST
SOURCES	+= simat_test.c
endif
