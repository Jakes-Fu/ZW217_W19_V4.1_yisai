/******************************************************************************
 ** File Name:      sci_stack_layer1_types.h                                  *
 ** Author:         Feng.shi                                                  *
 ** Date:           2012-10-04                                                *
 ** Copyright:      Copyright 2001-2012 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2012-10-04     Feng.shi      Create.                                      *
 ******************************************************************************/
#ifndef _SCI_STACK_LAYER1_TYPES_H
#define _SCI_STACK_LAYER1_TYPES_H

#include "sci_types.h"

/**************************************************************
*                           MACRO                             *
***************************************************************/
#define MAX_L2_MESSAGE_SIZE 23
#define MAX_BA_SIZE 64
#define MAX_USIM_BA_SIZE 32
#define CARD_NUM 4
#define MAX_BA_SIZE_THREE_CARD 256
#define MAX_CELL_DECODED_SIZE 32
#define SEARCH_DECODE_SIZE 6
#define MAX_TX_SLOT_NUM 8
#define MAX_ARFCN_SIZE 971
#define INVALID_LINK_TBF 0
#define DOWNLINK_TBF 1
#define UPLINK_TBF 2
#define BOTH_LINK_TBF 3
#define GPRS_TBF 0
#define EGPRS_TBF 1
#define MAC_INVALID_BEP_PERIOD2 255
#define MAX_TX_SLOT_NUM 8
#define MAX_RX_SLOT_NUM 8
#define MAX_TX_DATA_NUM_CLASS12 4
#define BLOCK_SIZE 54
#define EGPRS_LOOPBACK_ON 14
#define EGPRS_LOOPBACK_OFF 15
#define BCCH_HIGH_PRIORITY 160
#define FAST_PLMN_PRIORITY 176
#define BCCH_HIGHEST_PRIORITY_NORMAL 255
#define MAX_BSIC_PRIORITY 255
#define MAC_NR_OF_TIMESLOTS  8

#define CS1  0
#define CS2  1
#define CS3  2
#define CS4  3
#define BURST_8_BIT  4
#define BURST_11_BIT  5
#define BURST_8_BIT_ON_PCCCH  6
#define BURST_11_BIT_ON_PCCCH  7
#define CS1_ON_PCCCH  8
#define MCS1  9
#define MCS2  10
#define MCS3  11
#define MCS4  12
#define MCS5  13
#define MCS6  14
#define MCS7  15
#define MCS8  16
#define MCS9  17
#define MCS5_7  18
#define MCS6_9  19
#define MCSERROR  20

#define RLC_MODE_ACK  0
#define RLC_MODE_UNACK  1

/**************************************************************
*                           ENUM                              *
***************************************************************/
typedef enum
{
	EGSM900 = 0,
	DCS1800 = 1,
	EGSM_DCS_DUALBAND =2,
	PCS1900 =3,
	GSM850 =4,
	EGSM_PCS_DUALBAND = 5,
	GSM850_DCS_DUALBAND = 6,
	GSM850_PCS_DUALBAND = 7,
	GSM850_EGSM_DUALBAND = 8,
	GSM850_EGSM_PCS_TRIBAND = 9,
	GSM850_EGSM_DCS_TRIBAND = 10,
	EGSM_DCS_PCS_TRIBAND = 11,
	GSM850_EGSM_DCS_PCS_QUALBAND = 12,
	DCS_PCS_DUALBAND =13,
	GSM850_DCS_PCS_TRIBAND = 14,
	INVALID_BAND = 15
} MS_BAND_E;

typedef enum
{
    NO_COMMAND,
    BCCH_FIRST_COMMAND,
    FORCED_CELL_RESEL_COMMAND
} RRA_COMMAND_E;

typedef enum
{
    CAMP_OK,
    CAMP_FAIL_NONSYNC
} CAMP_RESULT_E;

typedef enum
{
    SACCH,
    SDCCH,
    FACCH_H,
    FACCH_F,
    BCCH,
    PCH_CHANNEL,   
    AGCH,
    PBCCH,
    PPCH,  
    PAGCH,
    PDTCH,
	PACCH,
	NO_CHANNEL
} CHANNEL_TYPE_E;
 
typedef enum
{
    SIGNAL_ONLY ,
    F_H_RATE_VER_1,
    F_H_RATE_VER_2,
    F_H_RATE_VER_3,
    DATA_43D_14U,
    DATA_29D_14U,
    DATA_43D_29U,
    DATA_14D_43U,
    DATA_14D_29U,
    DATA_29D_43U,
    DATA_43_RATE,
    DATA_32_RATE,
    DATA_29_RATE,
    DATA_14_RATE,
    DATA_12_RATE,
    DATA_6_RATE,
    DATA_3_RATE
} CHANNEL_MODE_E;

typedef enum
{
    TCH_H,
    TCH_F,
    SDCCH_4,
    SDCCH_8
} DCH_TYPE_E;

typedef enum
{
    PAGING_NORMAL,
    PAGING_EXTENDED,
    PAGING_REORGANISATION,
    PAGING_SAME_AS_BEFORE
} PAGING_MODE_E;

typedef enum
{
    SI13_NOT_PRESENT,
    SI13_NORM,
    SI13_EXT
} SI13_INDICATOR_E;

typedef enum
{
    CS_ONLY_ON_PCH,
    CS_PS_PAGING_ON_PPCH,
    CS_PS_PAGING_ON_PCH,
    CS_PCH_AND_PS_PPCH_OR_PCH
} PAGING_COORDINATION_E;

typedef enum
{
    CON_BCCH,
    CON_BCCH_A_EXT,
    SINGLE_BCCH,
    SINGLE_EXT_BCCH,
    PACKET_SYSINFO_1,
    CON_PBCCH,
    CON_PBCCH_WITH_LOW_REP_RATE,
    CON_PBCCH_WITH_HIGH_REP_RATE,
    MULTIPLE_SINGLE_BCCH,
    NO_BCCH_DEC  // layer1 use
} SYSINFO_READ_MODE_E;

typedef enum
{
    SYSINFO_ALL,
    SYSINFO_1,
    SYSINFO_2TER,
    SYSINFO_2BIS,
    SYSINFO_2QUATER,
    SYSINFO_3,
    SYSINFO_4,
    SYSINFO_7,
    SYSINFO_8,
    SYSINFO_9,
    SYSINFO_13,
    SYSINFO_2
} SYSINFO_IND_E;

typedef enum
{
    CIPHER_WITH_A5_1,
    CIPHER_WITH_A5_2,
    CIPHER_WITH_A5_3,
    CIPHER_WITH_A5_4,
    CIPHER_WITH_A5_5,
    CIPHER_WITH_A5_6,
    CIPHER_WITH_A5_7
} CIPHER_ALGORITHM_E;

typedef enum
{
    NON_SYNCHRONIZED,
    SYNCHRONIZED,
    PRE_SYNCHRONIZED,
    PSEUDO_SYNCHRONIZED
} SYNCHRON_TYPE_E;

typedef enum
{
    AB_8,
    AB_11
} AB_FORMAT_E;

typedef enum
{
    LOOP_A,
    LOOP_B,
    LOOP_C,
    LOOP_D,
    LOOP_E,
    LOOP_F,
    LOOP_G,
    LOOP_I,
    LOOP_MAX
} LOOP_LIST_E;

typedef enum
{
    NORMAL_OPERATION,
    SPEECH_DECODER_DTX,
    SPEECH_ENCODER_DTX,
    RESERVED_VALUE,
    ACOUSTIC_DEVICE_AD_DA
} TEST_INTERFACE_E;

typedef enum
{
    NEITHER_USE_DTX,
    ONLY_FULL_RATE_USE_DTX,
    ONLY_HALF_RATE_USE_DTX,
    BOTH_USE_DTX
} DTX_ALLOWED_E;

typedef enum
{
    DOWN_LINK_FAIL,
    RADIO_LINK_FAIL,
    RR_MT_COLLISION,
    RR_CS_OVERRIDE_PS
}L1_ERROR_CODE_E;

typedef enum
{
	NO_SERVICE,
	LIMITED_SERVICE,
	ONLY_GSM_SERVICE,
	GSM_GPRS_SERVICE
}RR_SERVICE_STATE_E;

typedef enum
{ 
	DSP_DECODE_FAIL,
	FORBIDDEN_PLMN,
	LAYER1_BUSY,
	LAYER1_PGREORG
}BSIC_FAIL_CAUSE_E;

typedef enum
{
    SMART_PAGING_OFF,        //关闭smart paging   
    SMART_PAGING_2,          //开启smart paging，周期为2倍bs_pamfrms
    SMART_PAGING_3,          //开启smart paging，周期为2倍bs_pamfrms
    SMART_PAGING_4           //开启smart paging，周期为4倍bs_pamfrms
} SMART_PAGING_MODE_E ;

typedef int16  ARQ_MODE_T;
#define ARQ1 0
#define ARQ2 1

/**************************************************************
*                           STRUCTURE                         *
***************************************************************/
typedef uint8  TXPWR_T;
typedef uint16 ARFCN_T;
typedef uint8  BSIC_T;
typedef uint16 ARFCN_LIST_ARR_T[MAX_BA_SIZE];
typedef uint8  MS_MULTISLOT_CAPABILITY_T;
typedef uint8  SUB_CHANNEL_T;
typedef uint8  MS_TXPWR_CAPABILITY_T;
typedef uint8  MS_EGPRS_CAPABILITY_T; 
typedef BOOLEAN  BA_IS_VALID_T;
typedef BOOLEAN  DTX_USED_T;
typedef int16  CCCH_GROUP_T; 
typedef int32  L2_SIZE_T;
typedef uint8 L2_MESSAGE_ARR_T[MAX_L2_MESSAGE_SIZE];
typedef int16  SAPI_NUM_T;
typedef uint32  TLLI_T;
typedef uint8 POLLING_TS_INDEX_ARR_T[MAX_TX_SLOT_NUM];
typedef uint8 RXLEV_ARR_T[MAX_ARFCN_SIZE];
typedef BOOLEAN  BCCH_DECODE_INDICATOR_T;
typedef uint8  RACH_AB_T;
typedef uint16  DELAY_SLOTS_T;
typedef uint8  PMAX_T;
typedef uint8  BA_IND_T;
typedef uint32  ABSOLUTE_FRAME_NUMBER_T;
typedef uint8  TIMING_ADVANCE_T;
typedef uint8  RADIO_LINK_TIMEOUT_T;
typedef BOOLEAN  PWRC_T;
typedef BOOLEAN  APPLY_DTX_T;
typedef uint8  NCC_PERMITTED_T;
typedef uint8  NC_NON_DRX_PERIOD_T;
typedef uint8  NC_REPORT_PERIOD_I_T;
typedef uint8  NC_REPORT_PERIOD_T_T;
typedef uint8  BCCH_INTERF_LEVEL_T;
typedef uint8  EXT_MEAS_MODE_T;
typedef uint8  EXT_REPORT_PERIOD_T;
typedef BOOLEAN  INT_FREQ_VALID_T;
typedef uint8  INT_FREQ_INDEX_T;
typedef uint8 IF_LEVEL_LIST_T[8];
typedef int16  TBF_DIRECTION_T;
typedef int16  TBF_MODE_T;
typedef uint8 USF_ARR_T[8];
typedef uint8  ALPHA_T;
typedef uint8  T_AVG_W_T;
typedef uint8  BA_VALID_T;
typedef uint8  T_AVG_T_T;
typedef uint8  PB_T;
typedef BOOLEAN  USE_BCCH_T;
typedef uint8  N_AVG_T;
typedef BOOLEAN  CBCH_ACT_T;
typedef BOOLEAN  INPUT_SWITCH_T;
typedef BOOLEAN  OUTPUT_SWITCH_T;
typedef BOOLEAN  TONE_SWITCH_T;
typedef uint16  TONE_DURATION_T;
typedef uint16  INPUT_LEVEL_T;
typedef uint16  OUTPUT_LEVEL_T;
typedef int16  MAC_MODE_T;
typedef int16  LLC_PDU_TYPE_T;
typedef int16  RADIO_PRIORITY_T;
typedef int16  RLC_MODE_T;
typedef uint8 EGPRS_RACH_AB_T[2];
typedef uint8 PRACH_AB_T[2];
typedef BOOLEAN  IS_EGPRS_PRACH_T;
typedef uint8 GAMMA_ARR_T[8];
typedef BOOLEAN SMSCB_SCHEDULE_TABLE_T[48];
typedef uint8 SMSCB_BLOCK_ARR_T[23];
typedef uint16  BCCH_TC_MASK_T;
typedef uint8  CELL_PARAM_ID_T;
typedef uint16  CARD_MASK_T;
typedef BOOLEAN  IS_MEAS_VALID_T;
typedef int16  BEP_PERIOD2;
typedef uint8  TFI_T;
typedef int16  SCT_GPRS_MODE_T;
typedef int16  SCT_EGPRS_MODE_T;

typedef struct{
    DCH_TYPE_E  dch_type;
    uint8  sub_channel;
    uint8  tn;
    uint8  tsc;
    BOOLEAN  hopping_ind;
    uint8  maio;
    uint8  hsn;
    ARFCN_T  arfcn;
} CHANNEL_DESCRIPTION_T;

typedef struct{
    int16  num;
    ARFCN_LIST_ARR_T  arfcn_list_arr;
} ARFCN_LIST_T;

typedef struct{
    int16  num;
    int16  start_point;
    ARFCN_LIST_ARR_T  arfcn_list_arr;
    MS_BAND_E  the_band;
} ARFCN_ROUND_LIST_T;

typedef struct{
    BSIC_T  bsic;
    ARFCN_T  arfcn;
} ARFCN_BSIC_T;

typedef ARFCN_BSIC_T ARFCN_BSIC_LIST_ARR_T[MAX_CELL_DECODED_SIZE];

typedef MS_BAND_E ARFCN_BAND_LIST_ARR_T[MAX_BA_SIZE];

typedef struct{
    int16  num;
    ARFCN_BSIC_LIST_ARR_T  arfcn_bsic_list_arr;
} ARFCN_BSIC_LIST_T;
 
typedef struct{
    BOOLEAN  MR_cfg_present;
    uint8  MR_ver;
    BOOLEAN  icmi;
    uint8  start_mode;
    uint8  acs;
    uint8  thr_hyst_num;
    uint8  thresh1;
    uint8  hyst1;
    uint8  thresh2;
    uint8  hyst2;
    uint8  thresh3;
    uint8  hyst3;
} MULTI_RATE_CFG_T;

typedef struct{
    ALPHA_T  alpha;
    uint8  t_avg_w;
    uint8  t_avg_t;
    BOOLEAN  pb_present;
    uint8  pb;
    BOOLEAN  pc_meas_chan;
    uint8  n_avg_i;
} GLOBAL_PWRC_PARAM_T;

typedef struct{
    ARFCN_T  arfcn;
    SI13_INDICATOR_E  si13_ind;
    BOOLEAN  combined_ccch;
    CCCH_GROUP_T  ccch_group;
    uint16  paging_group;
    uint8  bs_agblks_res;
    uint8  bs_pamfrms;
    TXPWR_T  txpwr;
    PAGING_MODE_E  paging_mode;
    ARFCN_LIST_T  ba_list;
    BOOLEAN  pwrc;
    DTX_ALLOWED_E  dtx_allowed;
    RADIO_LINK_TIMEOUT_T  radio_link_timeout;
    BOOLEAN  cbch_present;
    CHANNEL_DESCRIPTION_T  cbch_desc;
    ARFCN_LIST_T  cbch_ma_list;
    BOOLEAN  gprs_requested;
    GLOBAL_PWRC_PARAM_T  global_pwrc_param;
    AB_FORMAT_E  access_burst_type;
    BOOLEAN  short_vice_dsc_used;
    uint8  bep_period;
    BOOLEAN  pch_tbf_nv_allowed;
    BOOLEAN  pch_tbf_net_allowed;
} CAMP_PARAM_T;

typedef struct{
    uint8  pb;
    uint8  tsc;
    uint8  tn;
    BOOLEAN  pbcch_present;
    BOOLEAN  pbcch_hopping;
    uint16  arfcn;
    uint16  bcch_arfcn;
    uint8  hsn;
    uint8  maio;
    ARFCN_LIST_T  ma_list;
    MS_BAND_E  band;
} PBCCH_DESCRIPTION_T;

typedef struct{
    uint8  tsc;
    uint8  tn;
    BOOLEAN  hopping_ind;
    ARFCN_T  arfcn;
    uint8  hsn;
    uint8  maio;
    ARFCN_LIST_T  ma_list;
} PCCCH_DESC_T;

typedef struct{
    PAGING_MODE_E  page_mode;
    uint8  bs_prach_blks;
    uint8  bs_pbcch_blks;
    uint8  bs_pag_blks_res;
    uint8  split_pg_cycle;
    uint8  psi1_repeat_period;
    uint8  psi_count_high_rate;
    uint8  psi_count_low_rate;
    uint16  paging_group;
    TXPWR_T  txpwr;
    PBCCH_DESCRIPTION_T  pbcch_desc;
    PCCCH_DESC_T  pccch_desc;
    PAGING_COORDINATION_E  paging_ordination;
} PBCCH_MODE_PARAM_T;

typedef L2_MESSAGE_ARR_T  PHYSICAL_BLOCK_ARR_T;

typedef struct{
    uint8  fn_m32;
    uint8  fn_m51;
    uint8  fn_m26;
} REDUCED_FRAME_NUMBER_T;

typedef struct{
    BOOLEAN  starting_time_present;
    REDUCED_FRAME_NUMBER_T  starting_time;
} STARTING_TIME_T;

typedef struct{
    BOOLEAN  starting_time_present;
    ABSOLUTE_FRAME_NUMBER_T  starting_time;
} ABSOLUTE_STARTING_TIME_T;

typedef ARFCN_LIST_T  MOBILE_ALLOCATION_T;

typedef struct{
    BOOLEAN  freq_param_present;
    uint8  maio;
    ARFCN_LIST_T  ma_list;
} FREQ_PARAM_BEFORE_T;

typedef struct{
    BOOLEAN  pwrc;
    DTX_ALLOWED_E  dtx_allowed;
    uint8  ncc_permitted;
    RADIO_LINK_TIMEOUT_T  radio_link_timeout;
    BOOLEAN  multiband_report_present;
    uint8  multiband_report;
} TRANS_PARAM_T;

typedef struct{
    BOOLEAN  cipher_mode;
    CIPHER_ALGORITHM_E  cipher_algorithm;
} CIPHER_PARAM_T;

typedef struct{
    uint8  tn;
    BOOLEAN  uplink;
    BOOLEAN  downlink;
    uint8  channel_set;
    CHANNEL_MODE_E  channel_mode;
} SLOT_DESCRIPTION_T;

typedef SLOT_DESCRIPTION_T MULTI_SLOTS_T[8];

typedef struct{
    CHANNEL_DESCRIPTION_T  channel_desc;
    CHANNEL_MODE_E  channel_mode;
    MOBILE_ALLOCATION_T  ma_list;
    ABSOLUTE_STARTING_TIME_T  starting_time;
    BOOLEAN  param_before_present;
    CHANNEL_DESCRIPTION_T  channel_before;
    MOBILE_ALLOCATION_T  ma_list_before;
    TXPWR_T  txpwr;
    BOOLEAN  cipher_set;
    CIPHER_PARAM_T  cipher_param;
    MULTI_SLOTS_T  multi_slots;
    MULTI_RATE_CFG_T  multi_rate_cfg;
} CHANNEL_ASSIGN_T;

typedef struct{
    uint8  bsic;
    ARFCN_T  arfcn;
} CELL_DESCRIPTION_T;

typedef struct{
    SYNCHRON_TYPE_E  synchron_type;
    BOOLEAN  report_otd;
    BOOLEAN  normal_cell_ind;
    uint8  rtd;
    TIMING_ADVANCE_T  timing_advance;
} SYNCHRON_PARAM_T;

typedef struct{
    CELL_DESCRIPTION_T  cell_description;
    SYNCHRON_PARAM_T  synchron_param;
    uint8  handover_ref;
    BOOLEAN  access_type;
} HANDOVER_PARAM_T;

typedef struct{
    BOOLEAN  otd_present;
    uint32  otd;
} MOBILE_OTD_T;

typedef struct{
    BOOLEAN  multiband_report_present;
    uint8  multiband_report;
} MULTIBAND_REP_T;

typedef struct{
    ARFCN_T  arfcn;
    BSIC_T  bsic;
    uint8  rxlev;
    uint32  delta_fn;
    int16  otd;
    uint8  dsc_cur;
} CELL_MEAS_T;

typedef struct SCELL_ENH_MEAS_T_s {
    BOOLEAN  is_valid;
    uint8    rxlev_val;
    uint8    mean_bep;
    uint8    cv_bep;
    uint8    nbr_rcvd_blocks;
} SCELL_ENH_MEAS_T;

typedef uint16  NCELL_NUM_T;
typedef CELL_MEAS_T NCELL_MEAS_REPORT_T[6];
typedef CELL_MEAS_T DUAL_NCELL_MEAS_REPORT_T[28];

typedef struct{
    uint8  rxlev_full;
    uint8  rxlev_sub;
    uint8  rxqual_full;
    uint8  rxqual_sub;
    TXPWR_T  txpwr;
    RADIO_LINK_TIMEOUT_T  rlt_cur;
} SCELL_MEAS_REPORT_T;

typedef struct{
    BOOLEAN  valid_flag;
    uint8  i_level;
} BCCH_IF_LEVEL_T;

typedef ARFCN_LIST_T  EXT_FREQUENCY_LIST_T;
typedef ARFCN_LIST_T  NC_FREQUENCY_LIST_T;
typedef CELL_MEAS_T *NC_EM_REPORT_PTR_T;

typedef struct{
    uint8  tsc;
    uint8  ul_timeslot_masks;
    uint8  dl_timeslot_masks;
    uint8  ptcch_timeslot;
    ARFCN_T  arfcn;
    uint8  hsn;
    BOOLEAN  channel_hopping;
    uint8  maio;
    ARFCN_LIST_T  ma_list;
} PACKET_CHANNEL_T;

typedef struct{
    uint8  time_advance_value;
    BOOLEAN  time_advance_index_present;
    uint8  time_advance_index;
    uint8  time_advance_timeslotnumber;
} GPRS_TIMING_ADVANCE_T;

typedef struct{
    BOOLEAN  starting_time_present;
    BOOLEAN  absolute_or_reduced;
    ABSOLUTE_FRAME_NUMBER_T  absolute_frame_number;
    REDUCED_FRAME_NUMBER_T  reduced_frame_number;
} TBF_STARTING_TIME_T;

typedef struct{
    MAC_MODE_T  mac_mode;
    uint8  usf_granularity;
    USF_ARR_T  usf_arr;
} PH_MAC_MODE_T;

typedef struct{
    BOOLEAN  meas_param_present;
    TBF_STARTING_TIME_T  meas_starting_time;
    uint8  meas_bitmap;
    uint8  meas_interval;
} MEAS_PARAM_T;

typedef struct{
    BEP_PERIOD2  bep_period2;
    ARQ_MODE_T  arq;
    uint8  multiblock_num;
} EGPRS_TBF_PARAM_T;

typedef uint8 BLOCK_ARR_T[BLOCK_SIZE];

typedef struct{
    int16  data_flag;
    CHANNEL_TYPE_E  channel_type;
    int16  send_type;
    int16  puncture_type;
    BLOCK_ARR_T  array;
} BLOCK_STRUCT_T;

typedef BLOCK_STRUCT_T *BLOCK_STRUCT_PTR_T;
typedef BLOCK_STRUCT_PTR_T BLOCK_STRUCT_PTR_ARR_T[MAX_TX_SLOT_NUM];
typedef BLOCK_STRUCT_PTR_ARR_T *BLOCK_STRUCT_PTR_ARR_PTR_T;
typedef PRACH_AB_T *PRACH_AB_PTR_T;
typedef EGPRS_RACH_AB_T *EGPRS_RACH_AB_PTR_T;

typedef struct{
    uint16  begin_slot_num;
    uint16  end_slot_num;
    SMSCB_SCHEDULE_TABLE_T  schedule_table;
} SMSCB_SCHEDULE_T;

typedef struct{
    ARFCN_T  arfcn;
    MS_BAND_E  cell_band;
    SYSINFO_READ_MODE_E  si_read_mode;
    SYSINFO_IND_E  si_ind;
    uint8  tc;
} SINGLE_SI_REQ_T;

typedef SINGLE_SI_REQ_T SINGLE_BCCH_DECODE_LIST_ARR_T[4];

typedef struct{
    int16  num;
    SINGLE_BCCH_DECODE_LIST_ARR_T  si_req_list;
} MULTIPLE_SINGLE_BCCH_DECODE_T;

typedef struct{
    ARFCN_T  arfcn;
    BCCH_TC_MASK_T  tc_mask;
    MS_BAND_E  band;
    BOOLEAN  bcch_first;
    int16  bcch_priority;
    BOOLEAN  force_decode;
} BCCH_DECODE_T;

typedef BCCH_DECODE_T BCCH_DECODE_LIST_T[SEARCH_DECODE_SIZE];

typedef struct{
    ARFCN_T  arfcn;
    MS_BAND_E  band;
    int16  bsic_priority;
} BSIC_DECODE_T;

typedef BSIC_DECODE_T BSIC_DECODE_LIST_T[SEARCH_DECODE_SIZE];

typedef struct{
    uint16  arfcn_index;
    uint8  rxlev;
} RF_SCAN_MEAS_T;

typedef RF_SCAN_MEAS_T RF_SCAN_MEAS_LIST_T[MAX_BA_SIZE_THREE_CARD];

typedef struct{
    uint8  present;
    ARFCN_T  arfcn;
    BSIC_T  bsic;
    uint8  rxlev;
    int16  C1;
    int16  C2;
    int16  C31;
    int16  C32;
} RR_CELL_INFO_T;

typedef struct{
    uint8  present;
    ARFCN_T  arfcn;
    BSIC_T  bsic;
    uint8  rxlev;
    int16  rxlev_full;
    int16  rxlev_sub;
    int16  rxqual_full;
    int16  rxqual_sub;
} RR_CELL_INFO_TRAFFIC_T;

typedef RR_CELL_INFO_T RR_NCELLS_INFO_T[6];

typedef struct{
    uint8  rr_task_no;
    RR_CELL_INFO_T  scell;
    RR_NCELLS_INFO_T  ncells;
} RR_CELLS_INFO_T;

typedef struct{
    uint8  ms_txpwr_max_ccch;
    uint8  rxlev_access_min;
    uint8  CBA;
    uint8  CBQ;
    uint8  CRO;
    uint8  TempOffset;
    uint8  PT;
    uint8  CRH;
} RR_CELL_INFO_EX_T;

typedef RR_CELL_INFO_EX_T RR_NCELLS_INFO_EX_T[6];

typedef struct{
    RR_CELL_INFO_EX_T  scell;
    RR_NCELLS_INFO_EX_T  ncells;
} RR_CELLS_INFO_EX_T;

typedef struct{
    uint8  rr_task_no;
    RR_CELL_INFO_TRAFFIC_T  scell;
    RR_NCELLS_INFO_T  ncells;
} RR_CELLS_INFO_TRAFFIC_T;

typedef struct TIMESLOT_VALUES_T_s {
    BOOLEAN  value_present;
    uint8  value;
} TIMESLOT_VALUES_T;

typedef TIMESLOT_VALUES_T TIMESLOT_VALUES_ARR_T[MAC_NR_OF_TIMESLOTS];

typedef struct TS_BEP_T_s {
    BOOLEAN  value_present;
    BOOLEAN  is_psk8;
    uint8  gmsk_mean_bep_tn;
    uint8  psk8_mean_bep_tn;
} TS_BEP_T;

typedef TS_BEP_T TS_BEP_ARR_T[MAC_NR_OF_TIMESLOTS];

typedef struct CHANNEL_QUALITY_REP_T_s {
    uint8  c_value;
    uint8  rxqual;
    uint8  sign_var;
    uint8  gmsk_mean_bep;
    uint8  gmsk_cv_bep;
    uint8  psk8_mean_bep;
    uint8  psk8_cv_bep;
    BOOLEAN  mean_bep_tn_present;
    TS_BEP_ARR_T  ts_bep_val;
    TIMESLOT_VALUES_ARR_T  ts_values;
} CHANNEL_QUALITY_REP_T;

#endif
