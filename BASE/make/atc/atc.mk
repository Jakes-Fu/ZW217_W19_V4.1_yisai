ifeq ($(strip $(CUSTOMER_NAME)), SS)
    MCFLAG_OPT	=	-D_PS_BL7

    ifeq ($(strip $(PRODUCT_CONFIG)),	sc6530_samsung_Kiwi)
        MCFLAG_OPT	+=	-D_ATC_KIWI
    endif

    ifeq ($(strip $(GPRSMPDP_SUPPORT)), TRUE)
        MCFLAG_OPT += -DGPRSMPDP_ENABLE	
    endif
    
    ifeq ($(strip $(_LESSEN_TRACE_SUPPORT)), TRUE)
        MCFLAG_OPT   	+= -DATC_LESSEN_TRACE
    endif   
else
    MCFLAG_OPT	= -D_AUDIO_ENHANCE_

    ifeq ($(strip $(DUALSIM_SUPPORT)),	TRUE)
        MCFLAG_OPT	+=	-D_DUAL_SIM
    endif
    
    ifeq ($(strip $(MODEM_PLATFORM)), TRUE)
        MCFLAG_OPT += -D_ATC_ONLY
        MCFLAG_OPT += -DMN_AOC_SUPPORT	
    endif
    
    ifeq ($(strip $(MODEM_TYPE)), ZBX)
        MCFLAG_OPT		+= -DATC_DISABLE_SPEQPARA
        MCFLAG_OPT		+= -DATC_SAT_ENABLE
    endif

    ifeq ($(strip $(ORANGE_SUPPORT)), TRUE) 
        MCFLAG_OPT		+= -DATC_ORANGE_ENABLE
    endif 
endif

ifeq ($(strip $(CUS_ATC_DEV)), TRUE)
    MCFLAG_OPT   	+= -D_SCM21_ATC_DEV
endif
    
ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
    MCFLAG_OPT   	+= -D_SUPPORT_GPRS_
else
    MCFLAG_OPT   	+= -D_GSM_ONLY_ -D_BASE_COMPACT_CODE_
endif

ifeq ($(strip $(ATC_SUPPORT)),	COMPACT)
    MCFLAG_OPT	+=	-D_ULTRA_LOW_CODE_ -D_GSM_ONLY_
endif

MCFLAG_OPT	+=	-D_ATC_UIX8910_ENABLE_


ifeq ($(strip $(SYNCTOOL_SUPPORT)),	TRUE)
    MCFLAG_OPT	+=	-D_ATC_SYNC_TOOL_
endif

ifeq ($(strip $(PCLINK_SUPPORT)), TRUE)
    MCFLAG_OPT += -DPCLINK_ENABLE	
endif

ifeq ($(strip $(MUX_SUPPORT)), TRUE)
    MCFLAG_OPT   	+= -D_MUX_ENABLE_ -D_MUX_REDUCED_MODE_
    ifeq ($(strip $(DUAL_MUX_SUPPORT)), TRUE)
        MCFLAG_OPT   	+= -D_DUAL_MUX_ENABLE_
    endif
endif

ifeq ($(strip $(MODEM_MODE)), AP_PLUS_BP)
    MCFLAG_OPT   	+= -D_ATC_ONLY
endif

ifeq ($(strip $(CUSTOMER_NAME)), A)
MCFLAG_OPT += -DGSM_CUSTOMER_AFP_SUPPORT
endif

ifeq ($(strip $(ATC_AUTO_TEST)), TRUE)
MCFLAG_OPT += -DAUTO_TEST_SUPPORT
endif

ifeq ($(strip $(SIM_LOCK_SUPPORT)), TRUE)
ifeq ($(strip $(PLATFORM)), UWS6121E)
MCFLAG_OPT += -DDUALSIM_SIMLOCK_SUPPORT
endif
endif

ifneq ($(strip $(FM_SUPPORT)), NONE)
ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT += -DFM_EX_SUPPORT
endif
endif

ifeq ($(strip $(MODEM_GPRS_FULL)), TRUE)
MCFLAG_OPT += -D_GPRS_FULL_FUNCTION_
endif

ifeq ($(strip $(C_IMEI_SUPPORT)), TRUE)
MCFLAG_OPT += -D_C_IMEI_SUPPORT
endif

ifeq ($(strip $(GPS_SUPPORT)), TRUE)
ifeq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
MCFLAG_OPT += -D_MNGPS_SUPPORT_
endif
endif

MINCPATH  = BASE/PS/export/inc BASE/atc/export/inc BASE/Layer1/export/inc 
MINCPATH += char_lib/export/inc BASE/l4/export/inc BASE/sim/export/inc
MINCPATH += chip_drv/export/inc/outdated ms_ref/export/inc
ifeq ($(strip $(CUSTOMER_NAME)), SS)
    MINCPATH += BASE/atc/samsung_phone/source/c/aud_file base/l4/source/arm/datamag/h base/l4/source/arm \
                ARMRomCode/seine/arm/drv_sc6600v/inc
endif

ifeq ($(strip $(DUALMIC)), TRUE)
    MINCPATH += MS_Ref/export/inc/dualmic
else
    MINCPATH += MS_Ref/export/inc/singlemic
endif

ifeq ($(strip $(SIM_LOCK_SUPPORT)), TRUE)
ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH   += MS_Ref/source/simlock/inc
MINCPATH   += MS_Ref/source/simlock/library/include
endif
endif

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif


ifeq ($(strip $(MODEM_PLATFORM)),TRUE)
    ATC_SRC_PATH = BASE/atc/modem/
else
    ifeq ($(strip $(CUSTOMER_NAME)), SS)
        ATC_SRC_PATH = BASE/atc/samsung_phone/
    else
        ATC_SRC_PATH = BASE/atc/feature_phone/
    endif
endif

ifeq ($(strip $(WIFI_SUPPORT)),TROUT)
    MINCPATH += connectivity/WIFI/export/inc
endif

ifeq ($(strip $(GPS_SUPPORT)), TRUE)
ifeq ($(strip $(GPS_CHIP_VER)), GREENEYE2)
    MINCPATH += MS_Customize/export/inc
endif
endif

MINCPATH += MS_Customize/export/inc
MINCPATH += MS_Customize/source/common
MINCPATH += MS_Customize/source/product/driver/spiflash
MINCPATH += MS_Customize/source/product/driver/spiflash/inc

MINCPATH += $(strip $(ATC_SRC_PATH))source/h
MSRCPATH =  $(strip $(ATC_SRC_PATH))source/c

#MINCPATH   += Third-party/at_op

#MQTT
ifeq ($(strip $(MQTT_SUPPORT)), TRUE)
MINCPATH +=  Third-party/mqtt/src/unisoc/include \
Third-party/mqtt/src/paho.mqtt.c-master/src \
Third-party/mqtt/src/unisoc/time \
Third-party/mqtt/src/unisoc/thread \
Third-party/mqtt/src/unisoc/tls 


MINCPATH += Third-party/mbedtls/V206/include
MINCPATH += Third-party/mbedtls/V206/include/mbedtls
MINCPATH += Third-party/mbedtls/V224/include
MINCPATH += Third-party/mbedtls/V224/include/mbedtls

MINCPATH  += external/libc/h
MINCPATH  += external/libc/h/sys
MINCPATH  += external/libc
endif

#coap
ifeq ($(strip $(COAP_SUPPORT)), TRUE)
MINCPATH +=  Third-party/libcoap/include \
Third-party/libcoap/src/obgm_libcoap \
Third-party/libcoap/src/obgm_libcoap/examples \
Third-party/libcoap/src/obgm_libcoap/examples/contiki \
Third-party/libcoap/src/obgm_libcoap/examples/lwip \
Third-party/libcoap/src/obgm_libcoap/include/coap \
Third-party/libcoap/src/obgm_libcoap/src \
Third-party/libcoap/src/obgm_libcoap/src/platform/posix 
endif

# modem : direct satellite & common modem
ifeq ($(strip $(ATC_SRC_PATH)), BASE/atc/modem/)
    # direct satellite
    ifeq ($(strip $(MODEM_TYPE)), ZBX)
        SOURCES    =    at_common.c atc_gsm.c atc_main.c atc.c atc_gsm_sms.c atc_malloc.c atc_version.c atc_gsm_ss.c \
                        atc_phonebook.c atc_yacc.c atc_basic_cmd.c atc_plus_gprs.c atc_common.c atc_sat_info.c atc_gprs_modem.c \
                        atc_sat_lex.c atc_sleep.c atc_plmntable.c
    # common modem
    else
        SOURCES    =    at_common.c atc_gsm.c atc_main.c atc.c atc_gsm_sms.c atc_malloc.c atc_version.c atc_gsm_ss.c \
	                atc_phonebook.c atc_yacc.c atc_basic_cmd.c atc_plus_gprs.c atc_common.c atc_info.c atc_gprs_modem.c \
	                atc_lex.c atc_sleep.c atc_eng.c atc_stk.c atc_stk_adapt.c atc_stk_func.c atc_stk_decode.c \
	                atc_stk_encode.c atc_mbbms.c atc_plmntable.c
    endif
# feature phone & orange	& ss
else
    ifeq ($(strip $(ATC_SRC_PATH)), BASE/atc/samsung_phone/)
        #SS
        ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
        SOURCES			= 	at_common.c atc_gsm.c atc_main.c atc.c atc_gsm_sms.c atc_malloc.c atc_version.c atc_gsm_ss.c \
                  	 		atc_phonebook.c atc_yacc.c atc_basic_cmd.c atc_gsm_stk.c atc_plus_gprs.c atc_common.c atc_info.c atc_gprs_modem.c \
	        							atc_lex.c atc_sleep.c atc_samsung_prod.c atc_sha1.c
        else
        SOURCES			= 	at_common.c atc_gsm.c atc_main.c atc.c atc_gsm_sms.c atc_malloc.c atc_version.c atc_gsm_ss.c \
                  	 		atc_phonebook.c atc_yacc.c atc_basic_cmd.c atc_gsm_stk.c atc_common.c atc_gsmonly_info.c \
	        							atc_gsmonly_lex.c atc_sleep.c atc_samsung_prod.c atc_sha1.c
        endif
    else
        # orange
        ifeq ($(strip $(ORANGE_SUPPORT)), TRUE)
            ifeq ($(strip $(GPRS_SUPPORT)), TRUE)	
                SOURCES    =    atc_orange_prod.c at_common.c atc.c atc_basic_cmd.c atc_common.c atc_gprs_modem.c atc_gsm.c atc_gsm_sms.c atc_gsm_ss.c \
	                        atc_gsm_stk.c atc_org_info.c atc_org_lex.c atc_main.c atc_malloc.c atc_phonebook.c \
	                        atc_plus_gprs.c atc_sleep.c atc_version.c atc_yacc.c			               
            else
                SOURCES    =    atc_orange_prod.c at_common.c atc_gsm.c atc_main.c atc.c atc_gsm_sms.c atc_malloc.c atc_version.c atc_gsm_ss.c \
                                atc_phonebook.c atc_yacc.c atc_basic_cmd.c atc_gsm_stk.c atc_common.c atc_gsmonly_info.c \
                                atc_gsmonly_lex.c atc_sleep.c
            endif
        # feature phone
        else
            ifeq ($(strip $(GPRS_SUPPORT)), TRUE)	
                SOURCES    =    at_common.c atc.c atc_basic_cmd.c atc_common.c atc_gprs_modem.c atc_gsm.c atc_gsm_sms.c atc_gsm_ss.c \
                                atc_gsm_stk.c atc_info.c atc_lex.c atc_main.c atc_malloc.c atc_phonebook.c \
                                atc_plus_gprs.c atc_sleep.c atc_version.c atc_yacc.c atc_custom_cmd.c
            else
                SOURCES    =    at_common.c atc_gsm.c atc_main.c atc.c atc_gsm_sms.c atc_malloc.c atc_version.c atc_gsm_ss.c \
                                atc_phonebook.c atc_yacc.c atc_basic_cmd.c atc_gsm_stk.c atc_common.c atc_gsmonly_info.c \
                                atc_gsmonly_lex.c atc_sleep.c atc_custom_cmd.c
            endif
        endif
    endif
endif


SOURCES         += atc_gps.c
ifeq ($(strip $(MAKE_OS)), Windows)
ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
cmd:= $(shell cd $(ATC_SRC_PATH)tools&atc_gen.bat)
else
cmd:= $(shell cd $(ATC_SRC_PATH)tools&atc_gsmonly_gen.bat)
endif
else
ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
cmd:= $(shell cd $(ATC_SRC_PATH)tools;./atc_gen.bash)
else
cmd:= $(shell cd $(ATC_SRC_PATH)tools;./atc_gsmonly_gen.bash)
endif
endif
