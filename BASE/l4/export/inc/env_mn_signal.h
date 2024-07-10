/******************************************************************************
 ** File Name:      env_mn_signal.h                                           *
 ** Author:         Fancier.fan                                               *
 ** Date:           05/21/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file define MN received signals                                                       
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/21/2002     Fancier.fan      Create.                                   *
 ******************************************************************************/
#ifndef _ENV_MN_SIGNAL_H
#define _ENV_MN_SIGNAL_H

#include "sig_code.h"
#include "env_ps_signal.h"

#define ENV_MN_SIGNAL_LIST                                           \
    MN_START_UP_REQ = END_MN_IN_PS_SIGNAL + 1, /* signals from sim*/ \
        SIM_READ_T_FILE_CNF,                                         \
    SIM_READ_R_FILE_CNF,                                             \
    SIM_UPDATE_T_FILE_CNF,                                           \
    SIM_UPDATE_R_FILE_CNF,                                           \
    SIM_UPDATE_LANGUAGE_CNF,                                         \
    SIM_INCREASE_FILE_CNF,                                           \
    SIM_INITIALIZE_CNF,                                              \
    SIM_PIN_CHECK_IND,                                               \
    SIM_UPDATE_CCP_CNF,                                              \
    SIM_READ_CCP_CNF,                                                \
    SIM_ERASE_CCP_CNF,                                               \
    SIM_GET_FREE_EXT_INFO_CNF,                                       \
    SIM_UPDATE_DN_CNF,                                               \
    SIM_READ_DN_CNF,                                                 \
    SIM_ERASE_DN_CNF,                                                \
    SIM_ERASE_LND_ALL_CNF,                                           \
    SIM_PURGE_DN_CNF,                                                \
    SIM_FDN_SERVICE_CNF,                                             \
    SIM_BDN_SERVICE_CNF,                                             \
    SIM_SESSION_TERMINATION_CNF,                                     \
    SIM_CHV_CNF,                                                     \
    SIM_GSM_ALGORITHMS_CNF,                                          \
    SIM_CARD_PRESENT_IND,                                            \
    SIM_CARD_REMOVE_IND,                                             \
    SIM_CARD_UNRESPONSE_IND,                                         \
    SIM_CARD_PART_RESPONSE_IND,                                      \
    SIM_GET_RECORD_EF_INFO_CNF,                                      \
    SIM_SMS_GET_SMS_NUM_CNF,                                         \
    SIM_SMS_READ_PARAM_CNF,                                          \
    SIM_SMS_READ_SMS_CNF,                                            \
    SIM_SMS_WRITE_SMS_CNF,                                           \
    SIM_SMS_REPLACE_SMS_CNF,                                         \
    SIM_SMS_EDIT_SMS_CNF,                                            \
    SIM_SMS_UPDATE_SMS_STATUS_CNF,                                   \
    SIM_SMS_READ_STATUS_REPORT_CNF,                                  \
    SIM_SMS_WRITE_STATUS_REPORT_CNF,                                 \
    SIM_SMS_READ_SMSS_CNF,                                           \
    SIM_SMS_WRITE_SMSS_CNF,                                          \
    SIM_SMS_WRITE_PARAM_CNF,                                         \
    SIM_GET_SPN_CNF,                                                 \
    SIM_READ_ECC_CNF,                                                \
    SIM_READ_LP_CNF,                                                 \
    SIM_UPDATE_LP_CNF,                                               \
    SIM_READ_AD_CNF,                                                 \
    SIM_READ_SST_CNF,                                                \
    SIM_READ_PHASE_CNF,                                              \
    SIM_READ_ELP_CNF,                                                \
    SIM_UPDATE_ELP_CNF,                                              \
    SIM_READ_IMSI_CNF,                                               \
    SIM_READ_ACC_CNF,                                                \
    SIM_READ_HPLMN_CNF,                                              \
    SIM_READ_LOCI_CNF,                                               \
    SIM_UPDATE_LOCI_CNF,                                             \
    SIM_READ_KC_CNF,                                                 \
    SIM_UPDATE_KC_CNF,                                               \
    SIM_READ_BCCH_CNF,                                               \
    SIM_UPDATE_BCCH_CNF,                                             \
    SIM_READ_FPLMN_CNF,                                              \
    SIM_UPDATE_FPLMN_CNF,                                            \
    SIM_READ_PLMNSEL_CNF,                                            \
    SIM_UPDATE_PLMNSEL_CNF,                                          \
    SIM_READ_CBMI_CNF,                                               \
    SIM_UPDATE_CBMI_CNF,                                             \
    SIM_READ_GID1_CNF,                                               \
    SIM_READ_GID2_CNF,                                               \
    SIM_READ_VGCS_CNF,                                               \
    SIM_READ_VGCSS_CNF,                                              \
    SIM_READ_VBS_CNF,                                                \
    SIM_READ_VBSS_CNF,                                               \
    SIM_READ_EMLPP_CNF,                                              \
    SIM_READ_AAEM_CNF,                                               \
    SIM_UPDATE_AAEM_CNF,                                             \
    SIM_READ_CBMIR_CNF,                                              \
    SIM_UPDATE_CBMIR_CNF,                                            \
    SIM_READ_DCK_CNF,                                                \
    SIM_READ_NIA_CNF,                                                \
    SIM_AOC_READ_ACM_CNF,                                            \
    SIM_AOC_INIT_ACM_CNF,                                            \
    SIM_AOC_INCREASE_ACM_CNF,                                        \
    SIM_AOC_READ_ACMMAX_CNF,                                         \
    SIM_AOC_UPDATE_ACMMAX_CNF,                                       \
    SIM_AOC_READ_PUCT_CNF,                                           \
    SIM_AOC_UPDATE_PUCT_CNF,                                         \
    SIM_QUERY_CARD_STATUS_CNF,                                       \
    SIM_GET_FILE_INFO_CNF,                                           \
    SIM_GET_ACCESS_INFO_CNF,                                         \
    SIM_SERIOUS_ERROR_IND,                                           \
    SIM_HOT_PLUG_IN_IND,                                             \
    SIM_INIT_ENCRYPT_CNF,                                            \
    SIM_GET_CARD_TYPE_CNF,                                           \
    SIM_GET_CDMA_CARD_IMSI_CNF,                                      \
    SIM_DPHONE_REFRESH_SKIP_CNF,                                     \
    SIM_REFRESH_INI_CNF,                                             \
    SIM_REFRESH_FILE_CHG_CNF,                                        \
    SIM_REFRESH_INI_FILE_CHG_CNF,                                    \
    SIM_REFRESH_INI_FULL_CNF,                                        \
    SIM_REFRESH_READ_FILE_CNF,                                       \
    SIM_REFRESH_RESET_CNF,                                           \
    STK_SMS_PP_CNF,                                                  \
    SIM_READ_FILE_BUF_BY_PATH_CNF,                                   \
    SIM_LOCI_UPDATE_IND,                                             \
    SIM_SIG_CRSM_CNF,                                                \
    SIM_IMG_READ_CNF,                                                \
    SIM_COMMON_AUTH_CNF,                                             \
    SIM_MBBMS_AUTH_CNF,                                              \
    SIM_READ_VOICE_MAIL_CNF,                                         \
    SIM_UPDATE_VOICE_MAIL_CNF,                                       \
    SIM_READ_RAW_T_FILE_CNF,                                         \
    SIM_UPDATE_RAW_T_FILE_CNF,                                       \
    SIM_READ_RAW_R_FILE_CNF,                                         \
    SIM_UPDATE_RAW_R_FILE_CNF,                                       \
    SIM_SEARCH_R_FILE_CNF,                                           \
    SIM_SIG_CSIM_CNF,                                                \
    SIM_POWER_OFF_CNF, /* signals from timeout*/                     \
        MN_ACM_EXP_IND,                                              \
    MN_MPTY_EXP_IND,                                                 \
    MN_ECT_EXP_IND,                                                  \
    MN_AOC_1_EXP_IND,                                                \
    MN_AOC_2_EXP_IND,                                                \
    MN_AOC_3_EXP_IND,                                                \
    MN_AOC_4_EXP_IND,                                                \
    MN_AOC_5_EXP_IND,                                                \
    MN_AOC_6_EXP_IND,                                                \
    MN_AOC_7_EXP_IND,                                                \
    MN_AOC_1_5S_EXP_IND,                                             \
    MN_AOC_2_5S_EXP_IND,                                             \
    MN_AOC_3_5S_EXP_IND,                                             \
    MN_AOC_4_5S_EXP_IND,                                             \
    MN_AOC_5_5S_EXP_IND,                                             \
    MN_AOC_6_5S_EXP_IND,                                             \
    MN_AOC_7_5S_EXP_IND,                                             \
    MN_SS_TIMER_EXP_IND,                                             \
    MN_SS_RETRY_TIMER_EXP_IND, /* signals from mmi*/                 \
        APP_MN_PLMN_LIST_REQ,                                        \
    APP_MN_PLMN_SELECT_REQ,                                          \
    APP_MN_ABORT_PLMN_SEARCH_REQ,                                    \
    APP_MN_ABORT_PLMN_REGN_REQ,                                      \
    APP_MN_SIM_GET_PIN_RSP,                                          \
    APP_MN_SIM_PIN_FUNC_REQ,                                         \
    APP_MN_DEACTIVE_PS_REQ,                                          \
    APP_MN_NCELL_RSSI_REQ,                                           \
    APP_MN_PLMN_RESELECT_REQ,                                        \
    APP_MN_PLMN_READ_REQ,                                            \
    APP_MN_PLMN_WRITE_REQ,                                           \
    APP_MN_BAND_UPDATE_REQ,                                          \
    APP_MN_GPRS_ATTACH_REQ,                                          \
    APP_MN_GPRS_DETACH_REQ,                                          \
    APP_MN_GPRS_FORCE_DETACH_REQ,                                    \
    APP_MN_PS_POWER_ON_REQ,                                          \
    APP_MN_PS_POWER_OFF_REQ,                                         \
    APP_MN_PREFER_PLMN_CHANGE_REQ,                                   \
    APP_MN_SIM_POWER_ON_REQ,                                         \
    APP_MN_SIM_POWER_OFF_REQ, /*    APP_MN_DETACH_GSM_REQ,*/         \
                                                                     \
        APP_MN_CALL_START_REQ,                                       \
    APP_MN_ALERTING_REQ,                                             \
    APP_MN_CALL_DISC_REQ,                                            \
    APP_MN_CONNECT_REQ,                                              \
    APP_MN_START_DTMF_REQ,                                           \
    APP_MN_STOP_DTMF_REQ,                                            \
    APP_MN_CALL_HOLD_REQ,                                            \
    APP_MN_CALL_RETRIEVE_REQ,                                        \
    APP_MN_CALL_SWAP_REQ,                                            \
    APP_MN_CALL_MODIFY_REQ,                                          \
    APP_MN_CALL_MODIFY_RSP,                                          \
    APP_MN_BUILD_MPTY_REQ,                                           \
    APP_MN_SPLIT_MPTY_REQ,                                           \
    APP_MN_ECT_INVOKE_REQ,                                           \
    APP_MN_ACTIVECALL_REL_REQ,                                       \
    APP_MN_RECALL_RSP,                                               \
                                                                     \
    APP_MN_READ_SMS_REQ,                                             \
    APP_MN_DELETE_ALL_SMS_REQ,                                       \
    APP_MN_SET_ASSERT_SMS_DESTADDR_REQ,                              \
    APP_MN_UPDATE_SMS_STATUS_REQ,                                    \
    APP_MN_READ_SMS_STATUS_REPORT_REQ,                               \
    APP_MN_SEND_SMS_REQ,                                             \
    APP_MN_SEND_COMMAND_SMS_REQ,                                     \
    APP_MN_WRITE_SMS_REQ,                                            \
    APP_MN_EDIT_SMS_REQ,                                             \
    APP_MN_READ_SMS_PARAM_REQ,                                       \
    APP_MN_SET_SMS_RECEIVE_FUNC_REQ,                                 \
    APP_MN_WRITE_SMS_PARAM_REQ,                                      \
    APP_MN_CANCEL_SMS_REQ,                                           \
    APP_MN_SEND_SMS_DLVR_RPT_REQ,                                    \
                                                                     \
    /* signal from APP to MNSMSCB*/                                  \
        APP_MN_START_SMSCB_REQ,                                      \
    APP_MN_STOP_SMSCB_REQ,                                           \
    APP_MN_CHANGE_SMSCB_PARAM_REQ,                                   \
    APP_MN_GET_SMSCB_PARAM_REQ, /* signal from SMSCB to MNSMSCB */   \
        MN_SMSCB_MSG_IND,                                            \
    MN_SMSCB_MACRO_MSG_IND,                                          \
                                                                     \
    APP_MN_REGISTER_SS_REQ,                                          \
    APP_MN_ERASE_SS_REQ,                                             \
    APP_MN_ACTIVATE_SS_REQ,                                          \
    APP_MN_DEACTIVATE_SS_REQ,                                        \
    APP_MN_INTERROGATE_SS_REQ,                                       \
    APP_MN_REGISTER_PASSWORD_REQ,                                    \
    APP_MN_REL_ACTIVING_SERVICE_REQ,                                 \
    APP_MN_GET_PASSWORD_RSP,                                         \
    APP_MN_USSD_REQ,                                                 \
    APP_MN_USSD_RSP,                                                 \
    APP_MN_REJ_USSD_SERVICE_REQ,                                     \
    APP_MN_ERASE_CCBS_REQ,                                           \
                                                                     \
    APP_MN_SET_PDP_CONTEXT_REQ,                                      \
    APP_MN_DELETE_PDP_CONTEXT_REQ,                                   \
    APP_MN_SET_QOS_REQ,                                              \
    APP_MN_SET_MIN_QOS_REQ,                                          \
    APP_MN_SET_EXTEND_QOS_REQ,                                       \
    APP_MN_SET_EXTEND_MIN_QOS_REQ,                                   \
    APP_MN_PDP_DEACTIVATE_REQ,                                       \
    APP_MN_ACTIVATE_PDP_CONTEXT_REQ,                                 \
    APP_MN_MTACT_RESPOND_MODE_REQ,                                   \
    APP_MN_SET_LLC_XID_PARAM_REQ,                                    \
    APP_MN_MODIFY_PDP_CONTEXT_REQ,                                   \
                                                                     \
    NVSMS_INIT_CNF,                                                  \
    NVSMS_READ_SMS_CNF,                                              \
    NVSMS_WRITE_SMS_CNF,                                             \
    NVSMS_EDIT_SMS_CNF,                                              \
    NVSMS_READ_STATUS_REPORT_CNF,                                    \
    NVSMS_UPDATE_STATUS_CNF,                                         \
    NVSMS_REFRESH_REPLACE_INFO_CNF,                                  \
    NVSMS_SAVE_MT_SMS_CNF,                                           \
    NVSMS_SAVE_STATUS_REPORT_CNF,                                    \
    NVSMS_REPLACE_ITEM_CNF,                                          \
    MNSMS_DELETE_ALL_SMS_CNF,                                        \
    SMR_MN_DATA_IND,                                                 \
    SMR_MN_REPORT_IND,                                               \
    APP_MN_SWITCH_ARFCN_REQ,                                         \
    MN_MM_SWITCH_ARFCN_CNF,                                          \
    MN_ADP_CFW_ENV,                                                  \
    END_MN_SIGNAL

typedef enum
{
    ENV_MN_SIGNAL_LIST
} mn_signal_id_t;

#endif
