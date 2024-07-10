# l1sim_hsdpa.mk
#
#   2010 Spreatrum, Incoporated. All Rights Reserved.
#
# history
#   date       | author      | remarks
#   2010/04/16 | tommy.tang  | created
# 

MINCPATH =
MSRCPATH = 
SOURCES = 
MCFLAG_OPT =
INCLUDE_BASE_DIR =  $(P_TJPATH)
SOURCE_BASE_DIR =  $(P_TJPATH)
  
# ------------------------------------------------------------- #
#                       B a s e  t a b                          #
# ------------------------------------------------------------- #
MINCPATH = \
    $(INCLUDE_BASE_DIR)/../MS_Code/RTOS/export/inc \
    $(INCLUDE_BASE_DIR)/Source/msd/code/l1sim/h \
    $(INCLUDE_BASE_DIR)/Source/msd/build/obj/gen/debug \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/3g/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/3g/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/adaptation/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/adaptation/stack/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/kal_target/include \
    $(INCLUDE_BASE_DIR)/Source/msd/build/obj/gen/production \
    $(INCLUDE_BASE_DIR)/Source/infra/commlib_target/include \
    $(INCLUDE_BASE_DIR)/Source/infra/stacklib/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/test-infra/sme_target/include \
    $(INCLUDE_BASE_DIR)/Source/infra/test-infra/sme_target/asn/include \
    $(INCLUDE_BASE_DIR)/Source/infra/init/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/init/stack/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/common/asn/include \
    $(INCLUDE_BASE_DIR)/Source/infra/test-infra/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/3g/include \
    $(INCLUDE_BASE_DIR)/../MS_Code/common/export/inc \
    $(INCLUDE_BASE_DIR)/../MS_Code/MS_Ref/export/inc \
    $(INCLUDE_BASE_DIR)/../MS_Code/chip_drv/export/inc \
    $(INCLUDE_BASE_DIR)/../MS_Code/PS/export/inc \
    $(INCLUDE_BASE_DIR)/../MS_Code/Layer1/export/inc \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/dm/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/gprsb/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/include \
    $(INCLUDE_BASE_DIR)/../MS_Code/Layer1/source/h \
    $(INCLUDE_BASE_DIR)/../MS_Code/layer1_dsp/Export/inc \
    $(INCLUDE_BASE_DIR)/Source/infra/tools/asn1lib/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/rrc/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/3gl2/mac/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/gprs/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/dm/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/3gl2/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/3gl2/rlc/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/flow_ctrl/3g/include \
    $(PS_INCPATH) \
    $(L1_INCPATH) \
    common/export/inc \
    RTOS/export/inc \
    MS_Ref/export/inc \
    chip_drv/export/inc \
    DAPS/export/inc \
    DAPS/source/tcpip/inc  
   
MSRCPATH = $(SOURCE_BASE_DIR)/Source/msd/code/l1sim/src   
    
SOURCES = \
    L1sim_cell_cfg_sib_construct.c \
    L1sim_cell_cfg_init.c \
    L1sim_cell_cfg_reset.c \
    L1sim_cell_cfg.c \
    L1sim_cell_cfg_sib_init.c \
    L1sim_cell_cfg_sib_modification.c \
    L1sim_cell_cfg_sib_segmnt_concat_schdle.c \
    L1sim_cell_cfg_sib_subinit.c \
    L1sim_cell_cfg_sib_subschedule.c \
    L1sim_cell_cfg_utils.c \
    L1sim_cell_hdlrs.c \
    L1sim_cell_meas_hdlrs.c \
    L1sim_dch_funcs.c \
    L1sim_fach_hdlrs.c \
    L1sim_init.c \
    L1sim_internal_meas_hdlrs.c \
    l1sim_layer1_main.c \
    L1sim_mac_config.c \
    L1sim_mac_hdr.c \
    L1sim_mac_main.c \
    L1sim_mac_prach.c \
    L1sim_mac_tx.c \
    L1sim_main.c \
    L1sim_pccpch.c \
    L1sim_pch_hdlrs.c \
    L1sim_qual_meas_hdlrs.c \
    L1sim_rf_hdlrs.c \
    L1sim_rlc_ciph_config.c \
    L1sim_rlc_configure.c \
    L1sim_rlc_global.c \
    L1sim_rlc_init.c \
    L1sim_rlc_main.c \
    L1sim_rrc_cphy_copy.c \
    L1sim_rrc_prim.c \
    L1sim_timer_hdlrs.c \
    L1sim_utils.c \
    L1sim_dummy_l1.c
    
ifeq ($(strip $(TDPS_UEIT_ARM_L1SIM_HSDPA)), TRUE)
MSRCPATH += $(SOURCE_BASE_DIR)/Source/msd/build/obj/gen/debug
SOURCES +=  tt_umts_perf_tunable_rest.c \
            tt_cell_report_rest.c \
    				tt_classb_perf_tunable_rest.c \
    				tt_gprs_capability_rest.c \
    				tt_l1_if_cfg_rest.c \
    				tt_l1_mac_rest.c \
    				tt_l1_mac_rlc_rest.c \
    				tt_l1_sim_ue_utran_if_rest.c \
    				tt_l12mac_rest.c \
    				tt_l2_rrc_rest.c \
    				tt_l3_inc_rest.c \
    				tt_l4_common_header_rest.c \
    				tt_l4_csm_gen_data_rest.c \
    				tt_l4_phonebook_gen_data_rest.c \
    				tt_l4_smsal_gen_data_rest.c \
    				tt_l4_tcm_gen_data_rest.c \
    				tt_mac2l1_rest.c \
    				tt_ms_identity_rest.c \
    				tt_nvm_easn_structs_rest.c \
    				tt_rrc_peer_asn_rest.c \
    				tt_sat_interface_gen_data_rest.c \
    				tt_sim_cfg_rest.c \
    				tt_ss_errors_rest.c \
    				tt_ss_parameters_rest.c \
    				tt_ss_tcapmessages_rest.c \
    				tt_ssi_interface_gen_data_rest.c \
    				tt_stack_capability_rest.c \
    				tt_stack_cfg_params_rest.c \
    				tt_stack_peer_cfg_rest.c \
    				tt_tc_peer_rest.c \
    				tt_umts_capability_rest.c \
    				tt_umts_peer_cfg_rest.c \
    				tt_ac_interface_gen_data_rest.c 
endif    
    
# ------------------------------------------------------------- #
#                      C / C + +  t a b                         #
# ------------------------------------------------------------- #
MCFLAG_OPT = \
    -DDEBUG_KAL -DSMSAL_PDU -DAC_EXT_IF_STUB -D_L1C_RELEASE -DCHIP_SC6600C \
    -DPROJECT_9100_PATCH -DENTITY_ID=ENTITY_STACK -DKAL_ON_THREADX -D_CALIBRATION_VER_0009_ \
    -DSTATISTICS_ENABLE -DSC6600M -DCLOCK_SYNC_NOT_PRESENT -D_DOWNLOADPARAM_VER_0004_ \
    -D_RTOS -DLOOPBACK2 -D_DEBUG -D_REF_SC8800D_ -D_FILE_CODE_=1 -DRPS_DEBUG \
    -DRRC_DCFE_CONFIG_PARAM_CHECK_ON -DNVM_CHANGES -DTARGET_ARM -DOPT_FOR_REAL_NW \
    -DSTACK_BYPASS -DFLOWCTRL_3G_ON -DTDPS_UEIT_ARM_L1SIM -DTDPS_UEIT_ARM_L1SIM_HSDPA
    
      