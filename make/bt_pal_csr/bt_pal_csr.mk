
#MOCRO DEFINE
#NO_MOCOR_MEM_OPTION :   os old mem alloc interface
#BT_AUD_VER_1_ENABLE :   audio service 1.0 interface surpport
#BT_UARTAPI_IF_OLD   :   uart api moved to com_drv in latest mocor version ,to comply with other ads version, should add this mocor

MCFLAG_OPT   	+= -D__ARM__ -D_BT_USE_SFS -D_BT_ENABLE_FOLDER_OPERATION \
-DEXCLUDE_LAPC_MODULE -DEXCLUDE_EXCEPTION_HANDLER_MODULE\
-DEXCLUDE_OCS_MODULE -DCUSTOMER_SPREADTRUM_OPTIMIZATION\
-DEXCLUDE_FAX_MODULE -DNO_DISCONNECT_IN_LP -DHIDE_EXTENDED_BLUESTACK_PRIMS\
-DEXCLUDE_TCS_MODULE -DSTACK_ON_HOST -DINSTALL_RFCOMM_MODULE \
-D_RTOS -DACCEPT_SNIFF_REQUEST -DEXCLUDE_BIPS_MODULE -DEXCLUDE_BIPC_MODULE\
-DEXCLUDE_SYNCS_MODULE -DEXCLUDE_BNEP_MODULE -DEXCLUDE_AG_MODULE \
-D_BT_SBC_ENCODER_ARM

 ### bt support for Ultra Low Cost
ifeq ($(strip $(BLUETOOTH_SUPPORT)), CSR_BC4_6_LOW_MEM)
    MCFLAG_OPT += -DEXCLUDE_BPP_MODULE -DEXCLUDE_SAPS_MODULE -DEXCLUDE_SAPC_MODULE -DEXCLUDE_OSS_MODULE \
    -DINSTALL_PAN_MODULE -DROM_BUILD_ENABLE -DEXCLUDE_IWU_MODULE -DEXCLUDE_SCO_MODULE \
    -DCUSTOMER_SPREADTRUM -DCSR_USE_FAST_SEARCH -DCSR_BCHS_EXT_FEATURES_INQ_CLIENT \
    -DCSR_BCHS_EXT_FEATURES_INQ_SERVER \
    -D_BT_ULC_SUPPORT_ \
    -DCSR_BCHS_MEM_REDUCTION_SD -DCSR_BCHS_MEM_REDUCTION_CM -DCSR_BCHS_MEM_REDUCTION_HFG\
    -DCSR_BCHS_MEM_REDUCTION_SD -DCSR_BCHS_MEM_REDUCTION_CM -DCSR_BCHS_MEM_REDUCTION_VM \
    -DCSR_BCHS_MEM_REDUCTION_HQ -DCSR_BCHS_MEM_REDUCTION_ABCSP -DCSR_BCHS_MEM_REDUCTION_SDP \
    -DCSR_BCHS_MEM_REDUCTION_SC -DCSR_BCHS_MEM_REDUCTION_HFG -DCSR_BCHS_MEM_REDUCTION_OPP \
    -DCSR_BCHS_MEM_REDUCTION_CM_PRIV -DCSR_BCHS_MEM_REDUCTION_CM_CACHE_PARAM -DCSR_BCHS_MEM_REDUCTION_CM_DISCOVERY \
    -DCSR_BCHS_MEM_REDUCTION_RFCOMM -DCSR_BCHS_MEM_REDUCTION_L2CAP -DCSR_BCHS_MEM_REDUCTION_FCON_FCOFF \
    -DCSR_BCHS_MEM_REDUCTION_DM -DCSR_BCHS_MEM_REDUCTION_UNUSED -DCSR_BCHS_MEM_REDUCTION_SCO \
    -DCSR_BCHS_MEM_REDUCTION_BDADDR_MACRO -DEXCLUDE_ETHER_CONVERTER_FUNCTIONS -DCSR_BCHS_MEM_REDUCTION_CM_PRIV_LIB \
    -DCSR_BCHS_MEM_REDUCTION_HFG_UPSTREAM -UDCSR_BCHS_MEM_REDUCTION_HFG_VOL_AT -DCSR_BCHS_MEM_REDUCTION_INQ \
    -DAV_STREAM_INACTIVITY_LP_ENABLE \
    -DCUSTOMER_SPREADTRUM_OPTIMIZATION_DEV \
    -DEXCLUDE_AT_MODULE \
    -DEXCLUDE_BPPS_MODULE \
    -DEXCLUDE_CM_BNEP \
    -DEXCLUDE_DG_MODULE \
    -DEXCLUDE_DUNC_MODULE \
    -DEXCLUDE_FM_MODULE \
    -DEXCLUDE_HCRP_MODULE \
    -DEXCLUDE_HF_MODULE \
    -DEXCLUDE_HIDD_MODULE \
    -DEXCLUDE_HIDH_MODULE \
    -DEXCLUDE_JSR82_MODULE \
    -DEXCLUDE_PAC_MODULE \
    -DEXCLUDE_PAS_MODULE \
    -DEXCLUDE_SMLC_MODULE \
    -DEXCLUDE_SMLS_MODULE \
    -DEXCLUDE_TEST_MODULE \
    -DCSR_BCHS_MEM_REDUCTION_HCI \
    -DEXCLUDE_BCHS_L2CA_MODULE \
    -DCSR_BCHS_MEM_REDUCTION_CM_EPR \
    -DCSR_BCHS_MEM_REDUCTION_CM_PARK_MODE \
    -DCSR_BCHS_EXT_FEATURES_INQ_CLIENT \
    -DCSR_BCHS_EXT_FEATURES_INQ_SERVER 
    
else
    MCFLAG_OPT += -DENABLE_SHUTDOWN \
    -DEXCLUDE_SAPS_MODULE -DEXCLUDE_SAPC_MODULE -DEXCLUDE_OSS_MODULE\
    -DINSTALL_PAN_MODULE -DROM_BUILD_ENABLE -DEXCLUDE_IWU_MODULE -DEXCLUDE_SCO_MODULE 
endif

ifneq ($(strip $(DRM_SUPPORT)),)  
   ifneq ($(strip $(DRM_SUPPORT)), NONE)    
        MCFLAG_OPT		+=  -DDRM_SUPPORT_$(strip $(DRM_SUPPORT))  -DDRM_SUPPORT
   else
        MCFLAG_OPT		+=  -DDRM_SUPPORT_NONE
   endif
endif

### Switch Of General Object Exchange Profile Support
# It is the base function of OPP/FTS
# Set TURE before using OPP/FTS
ifeq ($(strip $(BT_OPP_SUPPORT)), TRUE)
    MCFLAG_OPT += -DBT_GOEP_SUPPORT
    MCFLAG_OPT += -DBT_OPP_SUPPORT
else
    MCFLAG_OPT += -DEXCLUDE_OPS_MODULE -DEXCLUDE_OPC_MODULE ### disable  BT_OPP_SUPPORT
endif

ifeq ($(strip $(BT_FTS_SUPPORT)), TRUE)
    MCFLAG_OPT += -DBT_GOEP_SUPPORT	
    MCFLAG_OPT += -DBT_FTP_SUPPORT
else
    MCFLAG_OPT += -DEXCLUDE_FTS_MODULE -DEXCLUDE_FTC_MODULE ### disable  BT_FTP_SUPPORT
endif

ifeq ($(strip $(BT_HFG_SUPPORT)), TRUE)
    MCFLAG_OPT += -DBT_HFG_SUPPORT
else
    MCFLAG_OPT += -DEXCLUDE_HFG_MODULE ### disable  BT_HFG_SUPPORT
endif

ifeq ($(strip $(BT_A2DP_SUPPORT)), TRUE)
 
else
    MCFLAG_OPT += -DEXCLUDE_AV_MODULE -DEXCLUDE_AVRCP_MODULE ### disable  BT_A2DP_SUPPORT
endif

ifeq ($(strip $(BT_SPP_SUPPORT)), TRUE)

else
    MCFLAG_OPT += -DEXCLUDE_SPP_MODULE 
endif




ifeq ($(strip $(BLUETOOTH_SUPPORT)), CSR_BC4_6)
    BLUETOOTH_PATH := bt_csr_15_3
    BLUETOOTH_CHIP := CSR_CMP
endif

ifeq ($(strip $(BLUETOOTH_SUPPORT)), CSR_BC4_6_LOW_MEM)
    BLUETOOTH_PATH := bt_csr_15_3
    BLUETOOTH_CHIP := CSR_CMP
endif

ifeq ($(strip $(BLUETOOTH_SUPPORT)), ATHR_AR3002)
    BLUETOOTH_PATH := bt_atheros
    BLUETOOTH_CHIP :=ATHEROS_CMP
endif

ifneq ($(strip $(BLUETOOTH_SUPPORT)), NONE)
  ifeq ($(strip $(SPRD_CUSTOMER)), FALSE)
    ifeq  ($(strip $(BLUETOOTH_CHIP)), CSR_CMP)
        ifeq ($(strip $(BLUETOOTH_SUPPORT)), CSR_BC4_6_LOW_MEM)
            MINCPATH =  Third-party/formal/bt_csr/$(strip $(BLUETOOTH_PATH))/inc_reduce Third-party/bt_csr/$(strip $(BLUETOOTH_PATH))/inc_reduce/sched
        else
            MINCPATH =  Third-party/formal/bt_csr/$(strip $(BLUETOOTH_PATH))/include Third-party/bt_csr/$(strip $(BLUETOOTH_PATH))/include/sched
        endif
    else
      MINCPATH =  Third-party/formal/$(strip $(BLUETOOTH_PATH))/inc
    endif
else
    MINCPATH =  Third-party/bt_csr/$(strip $(BLUETOOTH_PATH))/include Third-party/bt_csr/$(strip $(BLUETOOTH_PATH))/include/sched
endif
endif



MINCPATH +=  MS_Ref/source/bt_pal_csr/inc MS_Ref/source/bt_pal_csr/pf/inc \
 MS_Ref/source/bt_pal_csr/kernel/inc  \
 MS_Ref/source/bt_pal_csr/pf/a2dp MS_Ref/source/bt_pal_csr/kernel/low_level  MS_Ref/source/aud_config/inc_export \
 MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc MS_Ref/source/bt_pal_csr/pf/dun
MINCPATH += $(ATC_INCPATH) $(L1_INCPATH) 	
MINCPATH += chip_drv/export/inc/outdated
MSRCPATH =  MS_Ref/source/bt_pal_csr MS_Ref/source/bt_pal_csr/kernel/os MS_Ref/source/bt_pal_csr/kernel/low_level \
 MS_Ref/source/bt_pal_csr/pf/a2dp MS_Ref/source/bt_pal_csr/pf/common MS_Ref/source/bt_pal_csr/pf/sc_db MS_Ref/source/bt_pal_csr/pf/spp \
 MS_Ref/source/bt_pal_csr/pf/dun  MS_Ref/source/bt_pal_csr/kernel/sbc \
 MS_Ref/source/bt_pal_csr/pf/bpp MS_Ref/source/bt_pal_csr/reduce

#add for DRM 
MINCPATH += BASE/l4/export/inc BASE/sim/export/inc $(PS_INCPATH)

                

SOURCES			=  bt_abs.c pf_app.c bccmd_boot_strap.c task.c  bt_dut.c  os_pin.c  panic.c os_int.c os_msg.c \
				  os_pmalloc.c os_sched.c os_task.c os_timer.c  \
				  pf_common.c pf_file.c \
				  pf_sdp.c db.c sc_db.c  \
				  os_uart_dma.c  timer.c pmalloc.c uart.c 


ifeq ($(strip $(BLUETOOTH_CHIP)),CSR_CMP)
     MCFLAG_OPT  += -DBLUETOOTH_CHIP_CSR_CMP
     SOURCES     += os_debug.c  sched.c interrupt.c  message.c  \
                    sbc_enc.c 
     ifeq ($(strip $(BLUETOOTH_SUPPORT)), CSR_BC4_6_LOW_MEM)
          SOURCES += pf_cm_reduce.c  pf_sd_reduce.c 
     else
          SOURCES += pf_cm.c pf_sd.c 
     endif
                    
else
     SOURCES +=  pf_cm.c pf_sd.c 
     MCFLAG_OPT  += -DBLUETOOTH_CHIP_ATHEROS_CMP
endif

ifeq ($(strip $(BT_FTS_SUPPORT)), TRUE)
    SOURCES += pf_ftp_server.c
endif

ifeq ($(strip $(BT_FTC_SUPPORT)), TRUE)
    SOURCES += pf_ftp_client.c
endif

ifeq ($(strip $(BT_OPP_SUPPORT)), TRUE)
    SOURCES += pf_opp_client.c pf_opp_server.c
endif

ifeq ($(strip $(BT_HFG_SUPPORT)), TRUE)
    SOURCES += pf_hfg.c
endif

ifeq ($(strip $(BT_DUN_SUPPORT)),TRUE)
     SOURCES += pf_dun_gw.c dun_gw_api.c
endif
					
ifeq ($(strip $(BT_SPP_SUPPORT)),TRUE)
     SOURCES += pf_spp.c
endif

ifeq ($(strip $(BT_BPPC_SUPPORT)),TRUE)
     SOURCES += pf_bpp_client.c
endif

ifeq ($(strip $(BT_A2DP_SUPPORT)), TRUE)
    SOURCES += a2dp_api.c pf_a2dp.c sbc_pal.c \
    polyphase_analysis.c polyphase_synthesis.c sbc.c \
    sbc_dec.c polyphase.c
    
endif

ifeq ($(strip $(BT_HS_HW_INT_TYPE)), UART0)
    SOURCES += bluecall_uart.c						
endif


ifeq ($(strip $(PLATFORM)), SC6530)
   MCFLAG_OPT += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE
endif					

ifeq ($(strip $(PLATFORM)), SC6531EFM)
   MCFLAG_OPT += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
   MCFLAG_OPT += -DBT_SBC_DSP_ENCODE
endif	

ifeq ($(strip $(PLATFORM)), SC8800G)
   MCFLAG_OPT += -DBT_SBC_ARM_ENCODE
endif	

# only 6600 DO NOT support GAIN, 6800,6530,8800 support it.
MCFLAG_OPT +=  -DDIGITAL_GAIN_SUPPORT

ifeq ($(strip $(BLUETOOTH_SUPPORT)), CSR_BC4_6)
    # BQB supported as default at BC4_6..
    MCFLAG_OPT +=  -DBT_BQB_SUPPORT
endif


