/******************************************************************************
 ** File Name:      env_sim_signal.h                                          *
 ** Author:         Raislin.Kong                                              *
 ** Date:           05/21/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file define SIM received signals              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/21/2002     Raislin.Kong     create                                    *
 ******************************************************************************/
#ifndef _ENV_SIM_SIGNAL_H
#define _ENV_SIM_SIGNAL_H

#include "env_ps_signal.h"


typedef enum
{
    SIM_START_UP_REQ = (( P_SIM << 8 ) | 1 ) ,
    SIM_READ_T_FILE_REQ,
    SIM_READ_R_FILE_REQ,
    SIM_UPDATE_T_FILE_REQ,
    SIM_UPDATE_R_FILE_REQ,
    SIM_UPDATE_LANGUAGE_REQ,
    SIM_INCREASE_FILE_REQ,
    SIM_INITIALIZE_REQ,
    SIM_UPDATE_CCP_REQ,
    SIM_READ_CCP_REQ,
    SIM_ERASE_CCP_REQ,
    SIM_GET_FREE_EXT_INFO_REQ,
    SIM_UPDATE_DN_REQ,
    SIM_READ_DN_REQ,
    SIM_ERASE_DN_REQ,
    SIM_ERASE_LND_ALL_REQ,
    SIM_PURGE_DN_REQ,
    SIM_FDN_SERVICE_REQ,
    SIM_BDN_SERVICE_REQ,
    SIM_SESSION_TERMINATION_REQ,
    SIM_CHV_REQ,
    SIM_GSM_ALGORITHMS_REQ,
    SIM_RESPONSE_IND,
    SIM_INT_IND,
    SIM_CARD_1_8V_DETECT_IND,
    SIM_CARD_INSERTED_IND,
    SIM_GET_RECORD_EF_INFO_REQ,
    SIM_SMS_GET_SMS_NUM_REQ   , 
    SIM_SMS_READ_PARAM_REQ       ,
    SIM_SMS_READ_SMS_REQ         ,
    SIM_SMS_WRITE_SMS_REQ        ,
    SIM_SMS_REPLACE_SMS_REQ      ,
    SIM_SMS_EDIT_SMS_REQ        , 
    SIM_SMS_UPDATE_SMS_STATUS_REQ  ,
    SIM_SMS_READ_STATUS_REPORT_REQ ,
    SIM_SMS_WRITE_STATUS_REPORT_REQ,
    SIM_SMS_READ_SMSS_REQ        ,
    SIM_SMS_WRITE_SMSS_REQ       ,
    SIM_SMS_WRITE_PARAM_REQ       , 
    SIM_GET_SPN_REQ ,
    SIM_READ_ECC_REQ,
    SIM_READ_LP_REQ,
    SIM_UPDATE_LP_REQ,
    SIM_READ_AD_REQ,
    SIM_READ_SST_REQ,
    SIM_READ_PHASE_REQ,
    SIM_READ_ELP_REQ,
    SIM_UPDATE_ELP_REQ,
    SIM_READ_IMSI_REQ,
    SIM_READ_ACC_REQ,
    SIM_READ_HPLMN_REQ,
    SIM_READ_LOCI_REQ,
    SIM_UPDATE_LOCI_REQ,
    SIM_READ_KC_REQ,
    SIM_UPDATE_KC_REQ,
    SIM_READ_BCCH_REQ,
    SIM_UPDATE_BCCH_REQ,
    SIM_READ_FPLMN_REQ,
    SIM_UPDATE_FPLMN_REQ,
    SIM_READ_PLMNSEL_REQ,
    SIM_UPDATE_PLMNSEL_REQ,
    SIM_READ_CBMI_REQ,
    SIM_UPDATE_CBMI_REQ,
    SIM_READ_GID1_REQ,
    SIM_READ_GID2_REQ,
    SIM_READ_VGCS_REQ,
    SIM_READ_VGCSS_REQ,
    SIM_READ_VBS_REQ,
    SIM_READ_VBSS_REQ,
    SIM_READ_EMLPP_REQ,
    SIM_READ_AAEM_REQ,
    SIM_UPDATE_AAEM_REQ,
    SIM_READ_CBMIR_REQ,
    SIM_UPDATE_CBMIR_REQ,
    SIM_READ_DCK_REQ,
    SIM_READ_NIA_REQ,
    SIM_AOC_READ_ACM_REQ,
    SIM_AOC_INIT_ACM_REQ,
    SIM_AOC_INCREASE_ACM_REQ,
    SIM_AOC_READ_ACMMAX_REQ,
    SIM_AOC_UPDATE_ACMMAX_REQ,
    SIM_AOC_READ_PUCT_REQ,
    SIM_AOC_UPDATE_PUCT_REQ,
    SIM_CLOCK_STOP_IND,
    SIM_QUERY_CARD_STATUS_REQ,
    SIM_GET_FILE_INFO_REQ,
    SIM_PC_EXIST_IND,
    SIM_DPHOEN_REFRESH_TIMEOUT_IND,
    SIM_GET_ACCESS_INFO_REQ,
    SIM_POWER_OFF_REQ,
    SIM_DPHONE_REFRESH_REQ,
    SIM_PRESENT_CHECK_IND,
    SIM_SET_ARM_CLK_REQ,
    SIM_REFRESH_INI_REQ,
    SIM_REFRESH_FILE_CHG_REQ,
    SIM_REFRESH_INI_FILE_CHG_REQ,
    SIM_REFRESH_INI_FULL_REQ,
    SIM_REFRESH_RESET_REQ,
    SIM_REFRESH_READ_FILE_REQ,
    SIM_INIT_ENCRYPT_REQ,
    SIM_SWITCH_CHECK_CARD_PRES_REQ,
    SIM_GET_CARD_TYPE_REQ,
    SIM_GET_CDMA_CARD_IMSI_REQ,
    SIMATDUM_ACTIVE_REQ,
    SIMAT_TERMINAL_PROFILE_REQ,
    SIMAT_ENVELOPE_REQ,
    SIMAT_TERMINAL_RESPONSE_REQ,
    SIM_READ_FILE_BUF_BY_PATH_REQ,
    SIM_SIG_CRSM_REQ,   
    SIM_RESTORE_EXP_IND,
    SIM_IMG_READ_REQ,
    SIM_COMMON_AUTH_REQ,
    SIM_READ_VOICE_MAIL_REQ,
    SIM_UPDATE_VOICE_MAIL_REQ,
    MM_SIM_DATA_REQ ,
    MM_SIM_AUTH_REQ ,
    MM_SIM_FPLMN_UPD_REQ ,
    MM_SIM_CIPHER_UPD_REQ ,
    MM_SIM_STATUS_UPD_REQ ,
    MM_SIM_BALIST_UPD_REQ, 
    GMM_SIM_AUTH_REQ ,     
    GMM_SIM_SAVE_CIPHER_INFO ,
    GMM_SIM_SAVE_RAI_INFO , 
    SIMAT_AT_CPDU_ENVELOPE_REQ,
    SIM_READ_RAW_T_FILE_REQ,
    SIM_UPDATE_RAW_T_FILE_REQ,
    SIM_READ_RAW_R_FILE_REQ,
    SIM_UPDATE_RAW_R_FILE_REQ,
    SIM_SEARCH_R_FILE_REQ,
    SIM_SIG_CSIM_REQ,   
    SOFT_SIM_INIT_CNF,
    SOFT_SIM_GET_VERSION_CNF,
    SOFT_SIM_GET_EID_CNF,
    SOFT_SIM_GET_ATR_CNF,
    SOFT_SIM_RESET_CNF,
    SOFT_SIM_APDU_CNF
} sim_signal_id_t;

#endif //_ENV_SIM_SIGNAL_H
