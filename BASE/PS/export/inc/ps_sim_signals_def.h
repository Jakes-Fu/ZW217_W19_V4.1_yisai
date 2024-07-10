/******************************************************************************
 ** File Name:      ps_sim_signals_def.h                                      *
 ** Author:         Andy Huang                                                *
 ** Date:           2012-08-23                                                *
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
 ** 2012-08-23     Andy Huang      Create.                                   *
 ******************************************************************************/

#ifndef _PS_SIM_SIGNALS_DEF
#define _PS_SIM_SIGNALS_DEF

#include "sci_types.h"

/**************************************************************
*                           MACRO                             *
***************************************************************/
#define AUTH_PARA_RAND_LENGTH (16)
#define AUTH_PARA_AUTN_LENGTH (16)
//#define MAX_FPLMN_SIZE (85)

/**************************************************************
*                           ENUM                              *
***************************************************************/
typedef int32 GMM_STATE_UPDATED;
#define GU1_UPDATED 0
#define GU2_NOT_UPDATED 1
#define GU3_ROAM_NOT_ALLOWED 3

/**************************************************************
*                           STRUCTURE                         *
***************************************************************/
typedef struct {
    int32  value;
    BOOLEAN  valid_flag;
} P_TMSI_SIGNATURE_T;

typedef uint8 AUTH_PARA_RAND_ARR_T[AUTH_PARA_RAND_LENGTH];

typedef int32  GMM_CKSN_E;

typedef uint8 GPRS_KC_ARR_T[8];

typedef uint8 SRES_T[4];

typedef FPLMN_SIM_T FPLMN_ARR_T[MAX_FPLMN_SIZE];

typedef struct {
    uint8  max_num;
    uint8  length;
    FPLMN_ARR_T  fplmn_arr;
} FPLMN_LIST_T;

typedef struct {
    uint8  length;
    AUTN_T autn;
} AUTH_PARA_AUTN_T;

typedef struct {
    uint8  length;
    SRES_EXT_T sres_ext;
}AUTH_SRES_EXT_T ;;

typedef struct {
    uint8  length;
    AUTS_T auts;
}AUTH_AUTS_T ;;

//typedef uint8 RAND_T[16];

typedef struct {
    SIGNAL_VARS
    P_TMSI_T  ptmsi;
    P_TMSI_SIGNATURE_T  ptmsi_signature;
    RAI_T  rai;
    GMM_STATE_UPDATED  update_state;
} yPDef_GMM_SIM_SAVE_RAI_INFO;

typedef struct {
    SIGNAL_VARS
    AUTH_PARA_RAND_ARR_T  rand_arr;
    GMM_CKSN_E  gmm_cksn;
    BOOLEAN     is_autn_exist;
    AUTN_T      autn;
} yPDef_GMM_SIM_AUTH_REQ;

typedef struct {
    SIGNAL_VARS
    uint8  auth_result;
    GPRS_KC_ARR_T  kc_arr;
    SRES_T  res_num;
    uint8   sres_ext_len;
    SRES_EXT_T sres_ext;
    uint8   auts_len;
    AUTS_T  auts;
} yPDef_GMM_SIM_AUTH_CNF;

typedef struct {
    SIGNAL_VARS
    GPRS_KC_ARR_T  kc_arr;
    GMM_CKSN_E  gmm_cksn;
} yPDef_GMM_SIM_SAVE_CIPHER_INFO;

/* signal of MM_SIM_DATA_REQ only has header */

typedef struct {
    SIGNAL_VARS
    uint8  result;
    CKSN_T  mm_cksn;
    KC_T  mm_kc;
    GMM_CKSN_E  gmm_cksn;
    GPRS_KC_ARR_T  gmm_kc;
    RAI_T  cur_rai;
    P_TMSI_T  ptimsi;
    P_TMSI_SIGNATURE_T  ptmsi_signature;
    GMM_STATE_UPDATED  update_state;
} yPDef_MM_SIM_DATA_CNF;

typedef struct {
    SIGNAL_VARS
    RAND_T  rand;
    CKSN_T  cksn;
    BOOLEAN is_autn_exist;
    AUTN_T  autn;
    uint32  command_ref;
} yPDef_MM_SIM_AUTH_REQ;

#define MM_AUTH_OK                  0
#define MM_AUTH_FAILED              1
#define MM_AUTH_SELECT_GSM_FAILED   2
#define MM_AUTH_UPDATE_KC_FAILED    3
#define MM_AUTH_SYNC_FAILED         4
#define MM_AUTH_MAC_INCORRECT       5
#define MM_AUTH_UNACCEPTABLE 6

#define GMM_AUTH_OK   0
#define GMM_AUTH_FAILED 1
#define GMM_AUTH_SELECT_GSM_FAILED 2
#define GMM_AUTH_UPDATE_KC_FAILED 3
#define GMM_AUTH_SYNC_FAILED 4
#define GMM_AUTH_MAC_INCORRECT 5
#define GMM_AUTH_UNACCEPTABLE 6

typedef struct {
    SIGNAL_VARS
    uint8  result;
    KC_T  kc;
    SRES_T  sres;
    uint8   kc_len;
    uint8   sres_ext_len;
    SRES_EXT_T sres_ext;
    uint8   auts_len;
    AUTS_T  auts;
    uint32  command_ref;
} yPDef_MM_SIM_AUTH_CNF;

typedef struct {
    SIGNAL_VARS
    FPLMN_LIST_T  fplmn_list;
} yPDef_MM_SIM_FPLMN_UPD_REQ;

typedef struct {
    SIGNAL_VARS
    KC_T  kc;
    CKSN_T  cksn;
} yPDef_MM_SIM_CIPHER_UPD_REQ;

typedef struct {
    SIGNAL_VARS
    MM_UPDATE_STATUS_E  mm_update_status;
    LAI_T  lai;
    TMSI_T  tmsi;
    T3212_VALUE  t3212_val;
} yPDef_MM_SIM_STATUS_UPD_REQ;

typedef struct {
    SIGNAL_VARS
    BOOLEAN         is_usim;
    BA_LIST_ARR_T   ba_list;
    ARFCN_T         usim_camponed_arfcn;
    ARFCN_LIST_T    usim_arfcn_list;
} yPDef_MM_SIM_BALIST_UPD_REQ;

#endif
