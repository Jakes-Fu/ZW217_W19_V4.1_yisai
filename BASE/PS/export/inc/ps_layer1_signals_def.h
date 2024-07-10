/******************************************************************************
 ** File Name:      ps_layer1_signals_def.h                                   *
 ** Author:         Tracy Zhang                                               *
 ** Date:           2012-09-29                                                *
 ** Copyright:      Copyright 2001-2012 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2012-09-29     Tracy Zhang      Create.                                   *
 ******************************************************************************/

#ifndef _PS_LAYER1_SIGNALS_DEF
#define _PS_LAYER1_SIGNALS_DEF

/////////////////////signallist signals_rra_l1 begin////////////////////

typedef struct {
    SIGNAL_VARS
    uint16  invalid_param;
	BOOLEAN AFC_ON;
} yPDef_RRA_MPH_ACTIVE_REQ;

typedef struct {
    SIGNAL_VARS
    MS_MULTISLOT_CAPABILITY_T  gprs_multislot_class;
    MS_TXPWR_CAPABILITY_T  ms_gsm_txpwr;
    MS_TXPWR_CAPABILITY_T  ms_dcs_txpwr;
    MS_TXPWR_CAPABILITY_T  ms_pcs_txpwr;
    MS_TXPWR_CAPABILITY_T  ms_gsm850_txpwr;
    MS_EGPRS_CAPABILITY_T  egprs_supported;
} yPDef_RRA_MPH_CLASSMARK_REQ;

typedef struct {
    SIGNAL_VARS
    MS_BAND_E               band;   
    BA_IS_VALID_T           ph_ba_valid;
    ARFCN_LIST_T            ph_ba_list;
    CARD_MASK_T             card_mask;
    ARFCN_BAND_LIST_ARR_T   band_list;
} yPDef_RRA_MPH_RXPOWER_SWEEP_REQ;

typedef struct {
    SIGNAL_VARS
    CAMP_PARAM_T  bcch_camp_param;
    CARD_MASK_T  card_mask;
    BOOLEAN  is_master;
    RR_SERVICE_STATE_E  rr_service_state;
    MS_BAND_E  ms_band;
} yPDef_RRA_MPH_BCCH_CAMP_REQ;

typedef struct {
    SIGNAL_VARS
    ARFCN_LIST_T  ba_list;
    ARFCN_BAND_LIST_ARR_T  band_list;
    CARD_MASK_T  card_mask;
} yPDef_RRA_MPH_IDLE_BA_UPDATE_REQ;

typedef struct {
    SIGNAL_VARS
    SYSINFO_READ_MODE_E  read_mode;
    int16  bcch_decode_num;
    BCCH_DECODE_LIST_T  bcch_list;
    PBCCH_DESCRIPTION_T  pbcch_desc;
} yPDef_RRA_MPH_BCCH_LIST_DECODE_REQ;

typedef struct {
    SIGNAL_VARS
    ARFCN_T  ph_arfcn;
    MS_BAND_E  decode_band;
} yPDef_RRA_MPH_BCCH_STOP_REQ;

typedef struct {
    SIGNAL_VARS
    int16  bsic_decode_num;
    BSIC_DECODE_LIST_T  bsic_list;
} yPDef_RRA_MPH_BSIC_LIST_DECODE_REQ;

typedef struct {
    SIGNAL_VARS
    PAGING_MODE_E  page_mode;
    BOOLEAN  is_ccch;
    CARD_MASK_T  card_mask;
} yPDef_RRA_MPH_PAGE_MODE_REQ;

typedef struct {
    SIGNAL_VARS
    RACH_AB_T  rach_ab;
    DELAY_SLOTS_T  delay_slots;
    EGPRS_RACH_AB_T  erach_ab;
    BOOLEAN  is_erach;
    GSM_EST_TYPE_E  est_type;
} yPDef_RRA_MPH_RANDOM_ACCESS_REQ;

typedef struct {
    SIGNAL_VARS
    CHANNEL_DESCRIPTION_T  chan_desc;
    MOBILE_ALLOCATION_T  mobile_alloc;
    ABSOLUTE_STARTING_TIME_T  starting_time;
    FREQ_PARAM_BEFORE_T  freq_param_before;
    TIMING_ADVANCE_T  timing_advance;
    TRANS_PARAM_T  trans_param;
    MS_BAND_E  ms_band;
} yPDef_RRA_MPH_IMMED_ASSIGN_REQ;

typedef struct {
    SIGNAL_VARS
    CHANNEL_ASSIGN_T  chan_assign;
    MS_BAND_E  ms_band;
} yPDef_RRA_MPH_CHAN_ASSIGN_REQ;

typedef struct {
    SIGNAL_VARS
    CIPHER_PARAM_T  cipher_param;
    KC_T  kc;
} yPDef_RRA_MPH_CIPHER_MODE_REQ;

typedef struct {
    SIGNAL_VARS
    CHANNEL_DESCRIPTION_T  chan_desc;
    ARFCN_LIST_T  ma_list;
    ABSOLUTE_FRAME_NUMBER_T  starting_time;
    MS_BAND_E  ms_band;
} yPDef_RRA_MPH_FREQUENCY_CHANGE_REQ;

typedef struct {
    SIGNAL_VARS
    CHANNEL_MODE_E  chan_mode;
    MULTI_RATE_CFG_T  multi_rate_cfg;
} yPDef_RRA_MPH_CHANNEL_MODE_REQ;

typedef struct {
    SIGNAL_VARS
    HANDOVER_PARAM_T  ho_param;
    CHANNEL_ASSIGN_T  chan_assign;
    MS_BAND_E  ms_band;
} yPDef_RRA_MPH_HANDOVER_REQ;

typedef struct {
    SIGNAL_VARS
    TIMING_ADVANCE_T  ta;
} yPDef_RRA_MPH_TIMING_ADV_REQ;

typedef struct {
    SIGNAL_VARS
    NCC_PERMITTED_T  ncc_permitted;
    RADIO_LINK_TIMEOUT_T  rlt;
    PWRC_T  pwrc;
    DTX_ALLOWED_E  dtx_allowed_sacch;
} yPDef_RRA_MPH_RADIO_LINK_TIMEOUT_REQ;

typedef struct {
    SIGNAL_VARS
    RR_CELLS_INFO_T  rr_cells_info;
} yPDef_RRA_MPH_RR_CELLS_INFO_REQ;

typedef struct {
    SIGNAL_VARS
    ARFCN_LIST_T  ba_list;
    MULTIBAND_REP_T  ph_multiband_rep;
    ARFCN_BAND_LIST_ARR_T  band_list;
    CARD_MASK_T  card_mask;
} yPDef_RRA_MPH_BA_UPDATE_REQ;

typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  card_mask;
} yPDef_RRA_MPH_DEACTIVATE_REQ;

typedef struct {
    SIGNAL_VARS
    ARFCN_T  arfcn;
} yPDef_RRA_MPH_CHAN_REL_REQ;

typedef struct {
    SIGNAL_VARS
    CAMP_PARAM_T  bcch_camp_param;
    PBCCH_MODE_PARAM_T  pbcch_mode_param;
    CARD_MASK_T  card_mask;
    BOOLEAN  valid_flag;
    RR_SERVICE_STATE_E  rr_service_state;
    MS_BAND_E  ms_band;
} yPDef_RRA_MPH_PBCCH_CAMP_REQ;

typedef struct {
    SIGNAL_VARS
    NC_NON_DRX_PERIOD_T  nc_non_drx_period;
    NC_REPORT_PERIOD_I_T  nc_reporting_period_i;
    NC_REPORT_PERIOD_T_T  nc_reporting_period_t;
    ARFCN_LIST_T  nc_meas_freq_list;
    ARFCN_BAND_LIST_ARR_T  band_list;
    CARD_MASK_T  card_mask;
} yPDef_RRA_MPH_START_NC_MEAS_REQ;

typedef struct {
    SIGNAL_VARS
    EXT_MEAS_MODE_T  ext_meas_mode;
    EXT_REPORT_PERIOD_T  ext_rep_period_t;
    NCC_PERMITTED_T  ncc_permitted;
    INT_FREQ_VALID_T  int_freq_valid;
    INT_FREQ_INDEX_T  int_freq_index;
    EXT_FREQUENCY_LIST_T  ext_meas_freq_list;
} yPDef_RRA_MPH_START_EXT_NC_MEAS_REQ;

typedef struct {
    SIGNAL_VARS
    ARFCN_T  arfcn;
    MS_BAND_E  cell_band;
} yPDef_RRA_MPH_STOP_PERIODICALLY_BCCH_REQ;

typedef struct {
    SIGNAL_VARS
    BOOLEAN  on_off_enh_meas;
} yPDef_RRA_MPH_ON_OFF_ENH_MEAS_REQ;

typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  card_mask;
} yPDef_MPH_SMSCB_START_REQ;

typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  card_mask;
} yPDef_MPH_SMSCB_STOP_REQ;

typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  card_mask;
    SMSCB_SCHEDULE_T smscb_schedule;
} yPDef_MPH_SMSCB_SCHEDULE_REQ;

typedef struct {
    SIGNAL_VARS
    CARD_MASK_T  card_mask;
} yPDef_MPH_SMSCB_USELESS_MSG_REQ;

/////////////////////signallist signals_rra_l1 end////////////////////

/////////////////////signallist signals_l1_rra begin////////////////////
typedef struct {
    SIGNAL_VARS
    RXLEV_ARR_T  ph_rxlev_arr;
    MS_BAND_E  band;
    BOOLEAN     ph_ba_valid;
} yPDef_RRA_MPH_RXPOWER_SWEEP_CNF;
typedef struct {
    SIGNAL_VARS
    BOOLEAN  valid_flag;
    ARFCN_T  ph_arfcn;
    CAMP_RESULT_E  camp_reslt;
} yPDef_RRA_MPH_BCCH_CAMP_CNF;
typedef struct {
    SIGNAL_VARS
    CHANNEL_TYPE_E  chan_type;
    ARFCN_T  ph_arfcn;
    PHYSICAL_BLOCK_ARR_T  ph_block;
    BCCH_DECODE_INDICATOR_T  decode_indicator;
    ABSOLUTE_FRAME_NUMBER_T  ph_abs_fn;
    MS_BAND_E  decode_band;
} yPDef_RRA_MPH_BCCH_INFO_IND;
typedef struct {
    SIGNAL_VARS
    NCELL_NUM_T  ph_ncell_meas_num;
    DUAL_NCELL_MEAS_REPORT_T  ph_ncell_meas_list;
} yPDef_RRA_MPH_IDLE_NCELL_MEAS_IND;
typedef struct {
    SIGNAL_VARS
    ARFCN_T  ph_arfcn;
    BSIC_T  bsic;
    MS_BAND_E  decode_band;
} yPDef_RRA_MPH_BSIC_DECODE_CNF;
typedef struct {
    SIGNAL_VARS
    ARFCN_T  ph_arfcn;
    MS_BAND_E  decode_band;
    BSIC_FAIL_CAUSE_E  cause;
} yPDef_RRA_MPH_BSIC_DECODE_FAIL;
typedef struct {
    SIGNAL_VARS
    RACH_AB_T  rach_ab;
    REDUCED_FRAME_NUMBER_T  rach_fn;
    BOOLEAN  is_another_rr_traffic;
    EGPRS_RACH_AB_T  erach_ab;
    BOOLEAN  is_erach;
} yPDef_RRA_MPH_RANDOM_ACCESS_CNF;
typedef struct {
    SIGNAL_VARS
    BOOLEAN  valid_flag;
    MOBILE_OTD_T  ph_otd;
} yPDef_RRA_MPH_HANDOVER_CNF;
typedef struct {
    SIGNAL_VARS
    CELL_MEAS_T  ph_idle_scell_meas;
    RRA_COMMAND_E  rra_command;
} yPDef_RRA_MPH_IDLE_SCELL_MEAS_IND;
typedef struct {
    SIGNAL_VARS
    DTX_USED_T  ph_dtx_used;
    NCELL_NUM_T  ph_ncell_meas_num;
    DUAL_NCELL_MEAS_REPORT_T  ph_ncell_meas_list;
    SCELL_MEAS_REPORT_T  ph_scell_meas;
    IS_MEAS_VALID_T  is_meas_valid;
    SCELL_ENH_MEAS_T  ph_enh_meas;
} yPDef_RRA_MPH_CELL_MEAS_IND;
typedef struct {
    SIGNAL_VARS
    ARFCN_T  ph_arfcn;
    L1_ERROR_CODE_E  ph_error;
    MS_BAND_E  decode_band;
    CARD_MASK_T  card_id;
} yPDef_RRA_MPH_ERROR_IND;
typedef struct {
    SIGNAL_VARS
    ARFCN_T  ph_arfcn;
    CHANNEL_TYPE_E  ph_channel_type;
    SUB_CHANNEL_T  ph_sub_channel;
    ABSOLUTE_FRAME_NUMBER_T  ph_abs_fn;
    PHYSICAL_BLOCK_ARR_T  ph_block;
    MS_BAND_E  decode_band;
    uint32 cdac;
} yPDef_RRA_MPH_DATA_IND;
typedef struct {
    SIGNAL_VARS
    CELL_MEAS_T  ph_idle_scell_meas;
    NCELL_NUM_T  ph_ncell_meas_num;
    DUAL_NCELL_MEAS_REPORT_T  ph_ncell_meas_list;
    BCCH_IF_LEVEL_T  ph_pcch_if_level;
    BOOLEAN  send_to_network;
} yPDef_RRA_MPH_NC_MEAS_REPORT_IND;
typedef struct {
    SIGNAL_VARS
    NCELL_NUM_T  ext_meas_num;
    NC_EM_REPORT_PTR_T  ext_meas_rep_ptr;
    INT_FREQ_VALID_T  ext_int_freq_valid;
    IF_LEVEL_LIST_T  ext_if_level_list;
} yPDef_RRA_MPH_NC_EXT_MEAS_REPORT_IND;
typedef struct {
    SIGNAL_VARS
    uint32  scan_meas_num;
    RF_SCAN_MEAS_LIST_T  scan_meas_info;
} yPDef_RRA_MPH_RF_SCAN_IND;

typedef struct {
    SIGNAL_VARS
    SMSCB_BLOCK_ARR_T smscb_data;
    CARD_MASK_T  card_mask;
} yPDef_MPH_SMSCB_DATA_IND;
/////////////////////signallist signals_l1_rra end////////////////////




typedef struct {
    SIGNAL_VARS
    CHANNEL_TYPE_E  chan_type;
    SUB_CHANNEL_T  sub_channel;
    L2_SIZE_T  l2_size;
    L2_MESSAGE_ARR_T  l2_msg;
    ABSOLUTE_FRAME_NUMBER_T  ph_abs_fn;
} yPDef_PH_DATA_IND;

typedef struct {
    SIGNAL_VARS
    SAPI_NUM_T  sapi;
    CHANNEL_TYPE_E  chan_type;
} yPDef_PH_READY_TO_SEND_IND;

typedef struct {
    SIGNAL_VARS
    CHANNEL_TYPE_E  chan_type;
} yPDef_PH_CONNECT_IND;

typedef struct {
    SIGNAL_VARS
    SAPI_NUM_T  sapi;
    CHANNEL_TYPE_E  chan_type;
    L2_SIZE_T  l2_size;
    L2_MESSAGE_ARR_T  l2_msg;
} yPDef_PH_DATA_REQ;

/////////////////////signallist signals_RLC to Layer1 start////////////////////

typedef struct {
    SIGNAL_VARS
    CHAN_DESCR_T  chan_descr;
} yPDef_PH_RLC_REALLOCATION_REQ;


/* this signal is only used to log the uplink block */
typedef struct {
    SIGNAL_VARS
    ABSOLUTE_FRAME_NUMBER_T  ph_abs_fn;
    uint8  timeslot_index;
    BLOCK_STRUCT_T  ph_data;
} yPDef_PH_MAC_DATA_REQ;

/////////////////////signallist signals_RLC to Layer1 end////////////////////

/////////////////////signallist signals_MAC to Layer1 start////////////////////
typedef struct {
    SIGNAL_VARS
    TBF_DIRECTION_T  tbf_direction;
    PACKET_CHANNEL_T  chann_cfg;
    TBF_STARTING_TIME_T  starting_time;
    PH_MAC_MODE_T  ph_mac_mode;
    MEAS_PARAM_T  meas_param;
    MS_BAND_E  ms_band;
    TFI_T  tfi;
    TBF_MODE_T  tbf_mode;
    EGPRS_TBF_PARAM_T  egprs_tbf_param;
    RLC_MODE_T  rlc_mode;
} yPDef_PH_MAC_TBF_CONNECT_REQ;

typedef struct {
    SIGNAL_VARS
    TBF_DIRECTION_T  tbf_direction;
} yPDef_PH_MAC_TBF_RELEASE_REQ;

typedef struct {
    SIGNAL_VARS
    AB_FORMAT_E  ab_format;
    PRACH_AB_T  prach_ab;
    DELAY_SLOTS_T  delay_slot;
    IS_EGPRS_PRACH_T  is_eprach;
} yPDef_PH_MAC_PRACH_REQ;

typedef struct {
    SIGNAL_VARS
    GAMMA_ARR_T  gamma_arr;
    ALPHA_T  alpha;
    T_AVG_W_T  t_avg_w;
    T_AVG_T_T  t_avg_t;
    BOOLEAN  pb_present;
    PB_T  pb;
    USE_BCCH_T  use_bcch;
    N_AVG_T  n_avg_i;
    GPRS_TIMING_ADVANCE_T  ul_ph_ta;
    GPRS_TIMING_ADVANCE_T  dl_ph_ta;
    PMAX_T  pmax;
} yPDef_PH_MAC_TA_PWR_REQ;

typedef struct {
    SIGNAL_VARS
    uint8  pdch_timeslot;
} yPDef_PH_MAC_PDCH_REL_REQ;

typedef struct {
    SIGNAL_VARS
    BOOLEAN  is_egprs_sct_loopback_on;
    SCT_EGPRS_MODE_T  sct_egprs_mode;
} yPDef_PH_MAC_SCT_EGPRS_PARAM;

/////////////////////signallist signals_MAC to Layer1  end////////////////////


/////////////////////signallist signals_Layer1 to MAC start////////////////////
typedef struct {
    SIGNAL_VARS
    TBF_DIRECTION_T  tbf_direction;
} yPDef_PH_MAC_TBF_CONNECT_CNF;

typedef struct {
    SIGNAL_VARS
    TBF_DIRECTION_T  tbf_direction;
} yPDef_PH_MAC_TBF_RELEASE_CNF;

typedef struct {
    SIGNAL_VARS
    BLOCK_STRUCT_PTR_ARR_T  ph_block;
    ABSOLUTE_FRAME_NUMBER_T  ph_abs_fn;
} yPDef_PH_MAC_DATA_IND;

typedef struct {
    SIGNAL_VARS
    PRACH_AB_T  prach_ab;
    REDUCED_FRAME_NUMBER_T  ph_reduce_fn;
} yPDef_PH_MAC_PRACH_CNF;
/////////////////////signallist signals_Layer1 to MAC  end////////////////////

/////////////////////signallist signals_SCT to Layer1 start////////////////////
typedef struct {
    SIGNAL_VARS
    BOOLEAN  loop_switch;
    LOOP_LIST_E  loop_mode;
} yPDef_SCT_PH_TCH_LOOP_CMD;

typedef struct {
    SIGNAL_VARS
    TEST_INTERFACE_E  test_mode;
} yPDef_SCT_PH_TEST_INTERFACE;
/////////////////////signallist signals_SCT to Layer1  end////////////////////


/////////////////////signallist signals_Layer1 to SCT start////////////////////
typedef struct {
    SIGNAL_VARS
    BOOLEAN  loop_switch;
} yPDef_PH_SCT_TCH_LOOP_ACK;
/////////////////////signallist signals_Layer1 to SCT  end////////////////////

typedef struct
{
    SIGNAL_VARS
    SMART_PAGING_MODE_E  smart_paging_mode;
    CARD_MASK_T          card_mask;
} yPDef_RRA_MPH_SMART_PAGING_REQ;

#endif
