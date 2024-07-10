/******************************************************************************
** File Name:      sim_signal.c                                              *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the C file of common sim signal process functions *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/
//#include "sig_code.h"
#include "base_trc.h"
#include "sim_include.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

#ifdef SIM_AOC_SUPPORT
STATIC SIM_EF_MAPPING_ACMMAX_T g_sim_acm_max[SIM_CARD_NUM];
#endif

#ifdef SIM_3G_USIM_SUPPORT
uint8 s_sim_usim_ecc_alpha_id_len[SIM_CARD_NUM];
#endif

#ifdef MULTI_SIM_SYS_QUAD
#ifdef GSM_DPHONE_VER2_SUPPORT
STATIC SIM_RESULT_INITIALIZE_SIM_E s_sim_dphone_verify_pin_result[SIM_CARD_NUM] = {SIM_INI_ERROR_UNKNOWN, SIM_INI_ERROR_UNKNOWN, SIM_INI_ERROR_UNKNOWN, SIM_INI_ERROR_UNKNOWN};
//when in dphone bootup, verify pin & terminal profile is done before, so skip these two operations in the initialization
BOOLEAN s_sim_is_dphone_operation_skip[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
STATIC BOOLEAN  s_sim_is_entire_service[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE} ;
#endif
STATIC uint32  s_sim_sender[SIM_CARD_NUM] = {0, 0, 0, 0} ;
STATIC SIM_CHV_T s_sim_chv1[SIM_CARD_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
STATIC SIM_CHV_T s_sim_unblock_chv1[SIM_CARD_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
STATIC BOOLEAN s_sim_need_check_ext1_flag[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
STATIC BOOLEAN s_sim_need_check_ext2_flag[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
STATIC BOOLEAN s_sim_need_check_ext3_flag[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
STATIC BOOLEAN s_sim_get_chv1_original_max_times[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
STATIC uint8 s_sim_chv1_original_max_times[SIM_CARD_NUM] = {SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT};
STATIC BOOLEAN s_sim_get_chv2_original_max_times[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
STATIC uint8 s_sim_chv2_original_max_times[SIM_CARD_NUM] = {SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT};
#ifdef _CPHS_SUPPORT
BOOLEAN s_sim_df_orange_exist[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#ifdef SIM_AOC_SUPPORT
STATIC BOOLEAN s_sim_csp_als_enable[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#endif
#endif
STATIC uint8 s_sim_r4_cfis_line1_rec_id[SIM_CARD_NUM] = {0, 0, 0, 0};
STATIC uint8 s_sim_r4_cfis_line2_rec_id[SIM_CARD_NUM] = {0, 0, 0, 0};
STATIC BOOLEAN s_sim_r4_is_vmw_info_gotten[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#elif defined (MULTI_SIM_SYS_TRI)
#ifdef GSM_DPHONE_VER2_SUPPORT
STATIC SIM_RESULT_INITIALIZE_SIM_E s_sim_dphone_verify_pin_result[SIM_CARD_NUM] = {SIM_INI_ERROR_UNKNOWN, SIM_INI_ERROR_UNKNOWN, SIM_INI_ERROR_UNKNOWN};
//when in dphone bootup, verify pin & terminal profile is done before, so skip these two operations in the initialization
BOOLEAN s_sim_is_dphone_operation_skip[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
STATIC BOOLEAN  s_sim_is_entire_service[SIM_CARD_NUM] = {TRUE, TRUE, TRUE} ;
#endif
STATIC uint32  s_sim_sender[SIM_CARD_NUM] = {0, 0, 0} ;
STATIC SIM_CHV_T s_sim_chv1[SIM_CARD_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
STATIC SIM_CHV_T s_sim_unblock_chv1[SIM_CARD_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
STATIC BOOLEAN s_sim_need_check_ext1_flag[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
STATIC BOOLEAN s_sim_need_check_ext2_flag[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
STATIC BOOLEAN s_sim_need_check_ext3_flag[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
STATIC BOOLEAN s_sim_get_chv1_original_max_times[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
STATIC uint8 s_sim_chv1_original_max_times[SIM_CARD_NUM] = {SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT};
STATIC BOOLEAN s_sim_get_chv2_original_max_times[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
STATIC uint8 s_sim_chv2_original_max_times[SIM_CARD_NUM] = {SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT};
#ifdef _CPHS_SUPPORT
BOOLEAN s_sim_df_orange_exist[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#ifdef SIM_AOC_SUPPORT
STATIC BOOLEAN s_sim_csp_als_enable[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#endif
#endif
STATIC uint8 s_sim_r4_cfis_line1_rec_id[SIM_CARD_NUM] = {0, 0, 0};
STATIC uint8 s_sim_r4_cfis_line2_rec_id[SIM_CARD_NUM] = {0, 0, 0};
STATIC BOOLEAN s_sim_r4_is_vmw_info_gotten[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#else
#ifdef GSM_DPHONE_VER2_SUPPORT
STATIC SIM_RESULT_INITIALIZE_SIM_E s_sim_dphone_verify_pin_result[SIM_CARD_NUM] = {SIM_INI_ERROR_UNKNOWN, SIM_INI_ERROR_UNKNOWN};
//when in dphone bootup, verify pin & terminal profile is done before, so skip these two operations in the initialization
BOOLEAN s_sim_is_dphone_operation_skip[SIM_CARD_NUM] = {FALSE, FALSE};
STATIC BOOLEAN  s_sim_is_entire_service[SIM_CARD_NUM] = {TRUE, TRUE} ;
#endif
STATIC uint32  s_sim_sender[SIM_CARD_NUM] = {0, 0} ;
STATIC SIM_CHV_T s_sim_chv1[SIM_CARD_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
STATIC SIM_CHV_T s_sim_unblock_chv1[SIM_CARD_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
STATIC BOOLEAN s_sim_need_check_ext1_flag[SIM_CARD_NUM] = {TRUE, TRUE};
STATIC BOOLEAN s_sim_need_check_ext2_flag[SIM_CARD_NUM] = {TRUE, TRUE};
STATIC BOOLEAN s_sim_need_check_ext3_flag[SIM_CARD_NUM] = {TRUE, TRUE};
STATIC BOOLEAN s_sim_get_chv1_original_max_times[SIM_CARD_NUM] = {TRUE, TRUE};
STATIC uint8 s_sim_chv1_original_max_times[SIM_CARD_NUM] = {SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT};
STATIC BOOLEAN s_sim_get_chv2_original_max_times[SIM_CARD_NUM] = {TRUE, TRUE};
STATIC uint8 s_sim_chv2_original_max_times[SIM_CARD_NUM] = {SIM_MAX_CHV_ATTEMPT, SIM_MAX_CHV_ATTEMPT};
#ifdef _CPHS_SUPPORT
BOOLEAN s_sim_df_orange_exist[SIM_CARD_NUM] = {FALSE, FALSE};
#ifdef SIM_AOC_SUPPORT
STATIC BOOLEAN s_sim_csp_als_enable[SIM_CARD_NUM] = {FALSE, FALSE};
#endif
#endif
STATIC uint8 s_sim_r4_cfis_line1_rec_id[SIM_CARD_NUM] = {0, 0};
STATIC uint8 s_sim_r4_cfis_line2_rec_id[SIM_CARD_NUM] = {0, 0};
STATIC BOOLEAN s_sim_r4_is_vmw_info_gotten[SIM_CARD_NUM] = {FALSE, FALSE};
#endif

extern uint32 g_sim_simat_task_id[SIM_CARD_NUM];

#ifdef GSM_DPHONE_SUPPORT
#define DPHONE_REFRESH_TIME 5000
extern BLOCK_ID g_P_MN_ID[MULTI_SYS_NUM];
static uint8 s_sim_jiangsu_flag[4] = {0};
#ifdef MULTI_SIM_SYS_QUAD
BOOLEAN s_sim_gsm_dphone_support[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
uint8 s_dphone_reset_times[SIM_CARD_NUM] = {0, 0, 0, 0};
SIMAT_SIG_REFRESH_TYPE_E s_dphone_refresh_type[SIM_CARD_NUM] = {SIMAT_SIG_RF_INITIAL, SIMAT_SIG_RF_INITIAL, SIMAT_SIG_RF_INITIAL, SIMAT_SIG_RF_INITIAL};
BOOLEAN s_sim_use_unblock[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
uint32 s_sim_command_ref[SIM_CARD_NUM] = {0, 0, 0, 0};
//timer, dphone refresh overtime, notify MN Initialization fail
static SCI_TIMER_PTR s_sim_dphone_refresh_timer[SIM_CARD_NUM] = {NULL, NULL, NULL, NULL};
static SIM_EF_MAPPING_ICCID_T s_sim_gsm_dphone_iccid[SIM_CARD_NUM] = {0};
static BOOLEAN s_sim_gsm_dphone_watch_no_refresh[SIM_CARD_NUM] = {FALSE,FALSE,FALSE,FALSE};
#elif defined (MULTI_SIM_SYS_TRI)
BOOLEAN s_sim_gsm_dphone_support[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
uint8 s_dphone_reset_times[SIM_CARD_NUM] = {0, 0, 0};
SIMAT_SIG_REFRESH_TYPE_E s_dphone_refresh_type[SIM_CARD_NUM] = {SIMAT_SIG_RF_INITIAL, SIMAT_SIG_RF_INITIAL, SIMAT_SIG_RF_INITIAL};
BOOLEAN s_sim_use_unblock[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
uint32 s_sim_command_ref[SIM_CARD_NUM] = {0, 0, 0};
//timer, dphone refresh overtime, notify MN Initialization fail
static SCI_TIMER_PTR s_sim_dphone_refresh_timer[SIM_CARD_NUM] = {NULL, NULL, NULL};
static SIM_EF_MAPPING_ICCID_T s_sim_gsm_dphone_iccid[SIM_CARD_NUM] = {0};
static BOOLEAN s_sim_gsm_dphone_watch_no_refresh[SIM_CARD_NUM] = {FALSE,FALSE,FALSE};
#else
BOOLEAN s_sim_gsm_dphone_support[SIM_CARD_NUM] = {FALSE, FALSE};
uint8 s_dphone_reset_times[SIM_CARD_NUM] = {0, 0};
SIMAT_SIG_REFRESH_TYPE_E s_dphone_refresh_type[SIM_CARD_NUM] = {SIMAT_SIG_RF_INITIAL, SIMAT_SIG_RF_INITIAL};
BOOLEAN s_sim_use_unblock[SIM_CARD_NUM] = {TRUE, TRUE};
uint32 s_sim_command_ref[SIM_CARD_NUM] = {0, 0};
//timer, dphone refresh overtime, notify MN Initialization fail
static SCI_TIMER_PTR s_sim_dphone_refresh_timer[SIM_CARD_NUM] = {NULL, NULL};
static SIM_EF_MAPPING_ICCID_T s_sim_gsm_dphone_iccid[SIM_CARD_NUM] = {0};
static BOOLEAN s_sim_gsm_dphone_watch_no_refresh[SIM_CARD_NUM] = {FALSE,FALSE};
#endif
#endif

#ifdef SIM_CDMA_CARD_SUPPORT
static SIM_EF_MAPPING_IMSI_M_T s_sim_cdma_imsi_m[SIM_CARD_NUM] = {0};
#endif

#ifdef GSM_CUSTOMER_AFP_SUPPORT
static SIM_LANGUAGE_REC_LEN_INFO_T s_sim_language_length_info[SIM_CARD_NUM] = {0};
#endif

//STATIC function declare
/*****************************************************************************/
//  Description: save the information of "length of MNC in the IMSI"
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL void SaveMncLenInfo(
    uint32                cur_sim_task,
    SIM_EF_MAPPING_AD_T   *ad_ptr
);

#ifdef GSM_DPHONE_SUPPORT
/*****************************************************************************/
//  Description: For Dphone Refresh
//  Global resource dependence :
//  Author: Jacky.ying
//  Note:
/*****************************************************************************/
LOCAL void SIM_DphoneRefreshReq(uint32   cur_sim_task);
#endif

/**********************************************************************
//    Description:
//      the fucntion process the CHV procedure
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
LOCAL SIM_SERVICE_RESULT_E SIMSIGNAL_CHV_Procedure(uint32 cur_sim_task,
                                                   SIM_CHV_REQ_TYPE_E req_type,
                                                   SIM_CHV_INDICATION_E chv_indication,
                                                   SIM_CHV_T first_chv,
                                                   SIM_CHV_T second_chv);

/**********************************************************************
//    Description:
//      verify the chv1 when the sim card is in intialization.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void VerifyCHV1AtInit(
    uint32 cur_sim_task,
    SIM_CHV_T chv1, //the CHV1 chv
    SIM_CHV_T unblock_chv1, //the Unblock CHV1 chv
    SIM_RESULT_INITIALIZE_SIM_E *result //the verify result
);

#ifdef GSM_DPHONE_SUPPORT
/**********************************************************************
//    Description:
//       Dphone Refresh timeout procedure function
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
STATIC void DphoneRefreshTimeOut(uint32 lparam);
#endif

/*****************************************************************************/
//  Description: read file EF_IMSI
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL SIM_RESULT_INITIALIZE_SIM_E Init_ReadImsiFile(
    const uint32 cur_sim_task,             //IN:
    SIM_EF_MAPPING_IMSI_T   *imsi_mapping_ptr   //OUT:
);

/*****************************************************************************/
//  Description: read file EF_LOCI
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL SIM_RESULT_INITIALIZE_SIM_E Init_ReadLociFile(
    const uint32 cur_sim_task,             //IN:
    SIM_EF_MAPPING_LOCI_T   *loci_mapping_ptr   //OUT:
);

/*****************************************************************************/
//  Description: The function runs the procedures that are supported both by
//  the ME and the SIM when in SIM initialization
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:  the procedures is defined in 3GPP TS 11.11 section 11.2.1
/*****************************************************************************/
LOCAL SIM_RESULT_INITIALIZE_SIM_E Init_GetSimFileTable(                             //RETURN:
    const uint32 cur_sim_task,         //IN:
    SIM_PHASE_E  card_phase,     //IN:
    SIM_INI_INFO *ini_file_ptr,  //IN/OUT:
    BOOLEAN is_in_refresh
);

/**********************************************************************
//    Description:
//      anaylis the failed reason of the increase file operation
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_RESULT_INCREASE_FILE_E   AnalysisIncreaseFileFail(   //return value:the increase file fail reason
    SIM_SERVICE_RESULT_E    ser_result  //the service reasult
);

/**********************************************************************
//    Description:
//      anaylis the failed reason of the read file operation
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC  SIM_RESULT_READ_FILE_E AnalysisReadFileFail(   //return value:the read file result
    SIM_SERVICE_RESULT_E ser_result //the service result
);

/**********************************************************************
//    Description:
//      anaylis the failed reason of the udpate file operation
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_RESULT_UPDATE_FILE_E AnalysisUpdateFileFail( //return value:the udpate file result
    SIM_SERVICE_RESULT_E ser_result //the service result
);
/**********************************************************************
//    Description:
//      anaylis wether the error is fatal error
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
STATIC BOOLEAN IsFatalError(SIM_SERVICE_RESULT_E ser_result //the service result
                           );

/**********************************************************************
//    Description:
//    Get the AOC Access condition(for example:need the CHV1 or CHV2)
//    Global resource dependence :
//    Author:king.li
//    Note:
***********************************************************************/
STATIC BOOLEAN SIM_GetAOCAccessCondition(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E filename
);

#ifdef SIM_AOC_SUPPORT
/**********************************************************************
//    Description:
//      the function that read the EFacm,EFacmmax and EFpuct
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E ReadAocContext(
    uint32 cur_sim_task,
    SIM_AOC_CONTEXT_T *aoc_context,
    SIM_AOC_CHV_SELECT *aoc_chv_select
);
#endif

#ifdef MODEM_PLATFORM
/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_GetResponse(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
);

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_Status(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
);

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_ReadBinary(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
);

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_ReadRecord(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
);

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_UpdateBinary(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
);

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_UpdateRecord(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
);
#endif

/*****************************************************************************/
//  Description: read file EF_EHPLMN and EF_EHPLMNPI
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL SIM_SERVICE_RESULT_E SIMSIGNAL_ReadEhplmnFile(
    const uint32 cur_sim_task, //IN:
    const SIM_PHASE_E           card_phase,       //IN:
    const SIM_EF_MAPPING_SST_T  *saved_sst_ptr,   //IN:
    const PLMN_T                hplmn,            //IN:
    SIM_EF_MAPPING_EHPLMN_T     *ehplmn_list_ptr, //OUT:
    SIM_EF_MAPPING_EHPLMNPI_T   *ehplmn_pi_ptr    //OUT:
);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC SIM_SERVICE_RESULT_E SIMSIGNAL_GetSIMLanguageInfo(
    uint32 cur_sim_task,
    SIM_EF_MAPPING_LP_UNION_T *language_mapping
);

/*****************************************************************************/
//  Description : the function that process the CFFlag request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC BOOLEAN SIMSIGNAL_ReadCallForwardFlag(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    SIM_EF_MAPPING_CFFLAG_T *cfflag);

/*****************************************************************************/
//  Description : the function that process the CFFlag request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC SIM_RESULT_UPDATE_FILE_E SIMSIGNAL_UpdateCallForwardFlag(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    SIM_EF_MAPPING_CFFLAG_T *cfflag);

/*****************************************************************************/
//  Description : the function that process the MWIS request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC BOOLEAN SIMSIGNAL_ReadMWIS(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    SIM_VMW_IND_FLAG_T *vmwflag);

/*****************************************************************************/
//  Description : the function that process the MWIS request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC SIM_RESULT_UPDATE_FILE_E SIMSIGNAL_UpdateMWIS(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    SIM_VMW_IND_FLAG_T *vmwflag);

/*****************************************************************************/
//  Description : the function that change the STK Terminal Profile API
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC void SIMSIGNAL_ChangeSTKTerminalProfile(SIM_TERMINAL_PROFILE_DATA_T* profile_ptr);
/**********************************************************************
//    Description:
//      analysis wether the error is fatal error
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
STATIC BOOLEAN IsFatalError(SIM_SERVICE_RESULT_E ser_result //the service result
                           )
{
    BOOLEAN result = FALSE;

    if((SIM_SERVICE_RX_PARITY_ERROR == ser_result)
        || (SIM_SERVICE_TX_PARITY_ERROR == ser_result)
        || (SIM_SERVICE_CARD_OUT == ser_result)
        || (SIM_SERVICE_CARD_UNRESPONSE == ser_result))
    {
        result = TRUE;
        //SCI_TRACE_LOW:"SIM:IsFatalError TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4884_112_2_17_23_0_2_4665, (uint8 *)"");
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/**********************************************************************
//    Description:
//      anaylis the failed reason of the increase file operation
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_RESULT_INCREASE_FILE_E   AnalysisIncreaseFileFail(   //return value:the increase file fail reason
    SIM_SERVICE_RESULT_E    ser_result  //the service reasult
)
{
    switch(ser_result)
    {
        case SIM_SERVICE_INVALIDATE_FILE_NAME:
            return SIM_INCREASE_INVALIDE_FILE;
        case SIM_SERVICE_INCONSISTENT_FILE:
            return SIM_INCREASE_INCONSISTENT_FILE;
        case SIM_SERVICE_SELECT_NO_SUCH_FILE:
            return SIM_INCREASE_FILE_NOT_EXISTE;
        case SIM_SERVICE_CHV1_NEEDED:
            return SIM_INCREASE_CHV1_NEEDED;
        case SIM_SERVICE_CHV2_NEEDED:
            return SIM_INCREASE_CHV2_NEEDED;
        case SIM_SERVICE_U_CHV1_NEEDED:
            return SIM_INCREASE_U_CHV1_NEEDED;
        case SIM_SERVICE_U_CHV2_NEEDED:
            return SIM_INCREASE_U_CHV2_NEEDED;
        case SIM_SERVICE_CHV_BLOCKED:
            return SIM_INCREASE_CHV_BLOCKED;
        case SIM_SERVICE_ACCESS_NEVER:
            return SIM_INCREASE_ACCESS_NEVER;
        case SIM_SERVICE_INCREASE_MAX_VALUE:
            return SIM_INCREASE_REACH_MAX_VLAUE;
        default:
            return SIM_INCREASE_UNKNOW_ERROR;
    }
}
/**********************************************************************
//    Description:
//      anaylis the failed reason of the read file operation
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC  SIM_RESULT_READ_FILE_E AnalysisReadFileFail(    //return value:the read file result
    SIM_SERVICE_RESULT_E ser_result //the service result
)
{
    switch(ser_result)
    {
        case SIM_SERVICE_INVALIDATE_FILE_NAME:
            return SIM_READ_INVALIDE_FILE;
        case SIM_SERVICE_INCONSISTENT_FILE:
            return SIM_READ_INCONSISTENT_FILE;
        case SIM_SERVICE_SELECT_NO_SUCH_FILE:
            return SIM_READ_NOT_EXSITE;
        case SIM_SERVICE_CHV1_NEEDED:
            return SIM_READ_CHV1_NEEDED;
        case SIM_SERVICE_CHV2_NEEDED:
            return SIM_READ_CHV2_NEEDED;
        case SIM_SERVICE_U_CHV1_NEEDED:
            return SIM_READ_U_CHV1_NEEDED;
        case SIM_SERVICE_U_CHV2_NEEDED:
            return SIM_READ_U_CHV2_NEEDED;
        case SIM_SERVICE_CHV_BLOCKED:
            return SIM_READ_CHV_BLOCKED;
        case SIM_SERVICE_ACCESS_NEVER:
            return SIM_READ_ACCESS_NEVER;
        default:
            return SIM_READ_FAILED_UNKNOW;
    }
}

/**********************************************************************
//    Description:
//      anaylis the failed reason of the udpate file operation
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_RESULT_UPDATE_FILE_E AnalysisUpdateFileFail( //return value:the udpate file result
    SIM_SERVICE_RESULT_E ser_result //the service result
)
{
    switch(ser_result)
    {
        case SIM_SERVICE_INVALIDATE_FILE_NAME:
            return SIM_UPDATE_INVALIDE_FILE;
        case SIM_SERVICE_INCONSISTENT_FILE:
            return  SIM_UPDATE_INCONSISTENT_FILE;
        case SIM_SERVICE_SELECT_NO_SUCH_FILE:
            return SIM_UPDATE_NOT_EXISTE;
        case SIM_SERVICE_CHV1_NEEDED:
            return SIM_UPDATE_CHV1_NEEDED;
        case SIM_SERVICE_CHV2_NEEDED:
            return SIM_UPDATE_CHV2_NEEDED;
        case SIM_SERVICE_U_CHV1_NEEDED:
            return SIM_UPDATE_U_CHV1_NEEDED;
        case SIM_SERVICE_U_CHV2_NEEDED:
            return SIM_UPDATE_U_CHV2_NEEDED;
        case SIM_SERVICE_CHV_BLOCKED:
            return SIM_UPDATE_CHV_BLOCKED;
        case SIM_SERVICE_ACCESS_NEVER:
            return SIM_UPDATE_ACCESS_NEVER;
        default:
            return SIM_UPDATE_FAILED_UNKNOW;
    }
}

/**********************************************************************
//    Description:
//      Check the transparent EF is valid in the SST/UST
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMSIGNAL_CheckTransparentEFInServiceTable(uint32 cur_sim_task, SIM_FILE_NAME_E file_name)
{
    BOOLEAN result = FALSE;

    switch(file_name)
    {
        case SIM_FILE_EF_ACM:
        case SIM_FILE_EF_ACMmax:
        case SIM_FILE_EF_PUCT:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_ACM:
        case SIM_FILE_EF_USIM_ACMMAX:
        case SIM_FILE_EF_USIM_PUCT:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_PLMNsel:

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_PLMN_SEL))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_CBMI:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_CBMI:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CBMI))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_GID1:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_GID1:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GID1))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_GID2:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_GID2:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GID2))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_SPN:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_SPN:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SPN))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_VGCS:
        case SIM_FILE_EF_VGCSS:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_VGCS:
        case SIM_FILE_EF_USIM_VGCSS:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_VGCS))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_VBS:
        case SIM_FILE_EF_VBSS:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_VBS:
        case SIM_FILE_EF_USIM_VBSS:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_VBS))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_eMLPP:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_EMLPP:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EMLPP))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_CBMIR:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_CBMIR:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CBMIR))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_DCK:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_DCK:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_DCK))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_CNL:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_CNL:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CO_NL))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_SAI:

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SOLSA))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_PLMNWACT:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_PLMNWACT:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_PLMN_WACT))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_OPLMNWACT:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_OPLMNWACT:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_OPLMN_WACT))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_HPLMNWACT:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_HPLMNWACT:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_HPLMN_WACT))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_CPBCCH:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_GSM_CPBCCH:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CPBCCH))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_INVSCAN:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_GSM_INVSCAN:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_INV_SCAN))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_MEXE:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_MEXE_SST:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_MEXE))
            {
                result = TRUE;
            }

            break;
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_SPDI:

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SPDI))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_USIM_EHPLMN:

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EHPLMN))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_USIM_EHPLMNPI:

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EHPLMNPI))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_USIM_SPNI:

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SPN_ICON))
            {
                result = TRUE;
            }

            break;
#endif
        default:
            result = TRUE;
            break;
    }

    return result;
}

/**********************************************************************
//    Description:
//      Check the record EF is valid in the SST/UST
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMSIGNAL_CheckRecordEFInServiceTable(uint32 cur_sim_task, SIM_FILE_NAME_E file_name)
{
    BOOLEAN result = FALSE;

    switch(file_name)
    {
        case SIM_FILE_EF_PNN:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_PNN:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_PNN))
            {
                result = TRUE;
            }

            break;
        case SIM_FILE_EF_OPL:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_OPL:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_OPL))
            {
                result = TRUE;
            }

            break;
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_PNNI:

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_PNN_ICON))
            {
                result = TRUE;
            }

            break;
#endif
        case SIM_FILE_EF_CFIS:
#ifdef SIM_3G_USIM_SUPPORT
        case SIM_FILE_EF_USIM_CFIS:
#endif

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CFIS))
            {
                result = TRUE;
            }

            break;
        default:
            result = TRUE;
            break;
    }

    return result;
}

/**********************************************************************
//    Description:
//      the function that process the Transparent file read request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadTransparentEF(
    uint32 cur_sim_task,
    SIM_SIG_READ_T_FILE_REQ_T *psig //in:the transparent fiel read request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_READ_T_FILE_CNF_T *pcnf;
    BOOLEAN is_ef_support = FALSE;

    //create the confirm signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_T_FILE_CNF, (uint16)(sizeof(SIM_SIG_READ_T_FILE_CNF_T)), g_sim_task_id[cur_sim_task]);
    //get info for the confirm signal from request signal
    pcnf->command_ref = psig->command_ref;
    pcnf->file_name = psig->file_name;

    if(SIM_FILE_EF_CFFLAG == psig->file_name) //special operation for CFFLAG
    {
        if(SIMSIGNAL_ReadCallForwardFlag(cur_sim_task, psig->file_name, &(pcnf->buf.cfflag)))
        {
            pcnf->result = SIM_READ_OK;
        }
        else
        {
            pcnf->result = SIM_READ_FAILED_UNKNOW;
        }

        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }
    else if(SIM_FILE_EF_VMWFLAG == psig->file_name) //special operation for VMWFLAG
    {
        if(SIMSIGNAL_ReadMWIS(cur_sim_task, psig->file_name, &(pcnf->buf.vmw_ind_flag)))
        {
            pcnf->result = SIM_READ_OK;
        }
        else
        {
            pcnf->result = SIM_READ_FAILED_UNKNOW;
        }

        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //If the EF is CPHS file, check it before read.
    is_ef_support = SIM_IsCPHSOptionalSupport(cur_sim_task, pcnf->file_name);

    if(is_ef_support)
    {
        is_ef_support = SIMSIGNAL_CheckTransparentEFInServiceTable(cur_sim_task, psig->file_name);
    }

    if(is_ef_support)
    {
        pcnf->result = SIM_READ_OK;
        //read the transparent EF file
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, psig->file_name, &(pcnf->buf.ad));

#ifdef _CPHS_SUPPORT
#ifdef SIM_AOC_SUPPORT
        if(SIM_SERVICE_OK == ser_result)
        {
            if(SIM_FILE_EF_CSP == psig->file_name)
            {
                if(pcnf->buf.csp.cphs_teleserv.is_als)
                {
                    s_sim_csp_als_enable[cur_sim_task] = TRUE;
                }
                else
                {
                    s_sim_csp_als_enable[cur_sim_task] = FALSE;
                }
            }
        }
#endif
#endif

        //get the read error reason
        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->result = SIM_READ_NOT_EXSITE;
    }

    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function that process the recordy file read request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadRecordEF(
    uint32 cur_sim_task,
    SIM_SIG_READ_R_FILE_REQ_T *psig //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_READ_R_FILE_CNF_T *pcnf;
    BOOLEAN is_ef_support = FALSE;

    //create the confirm signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_R_FILE_CNF, (uint16)(sizeof(SIM_SIG_READ_R_FILE_CNF_T)), g_sim_task_id[cur_sim_task]);
    //get info for the confirm signal from request signal
    pcnf->command_ref = psig->command_ref;
    pcnf->file_name = psig->file_name;
    pcnf->record_id = psig->record_num;
    pcnf->record_mode = psig->record_mode;

    if(0 == psig->record_num)
    {
        pcnf->result = SIM_READ_FAILED_UNKNOW;
        //send confirm signal of the request signal
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //If the EF is CPHS file, check it before read.
    is_ef_support = SIM_IsCPHSOptionalSupport(cur_sim_task, pcnf->file_name);

    if(is_ef_support)
    {
        is_ef_support = SIMSIGNAL_CheckRecordEFInServiceTable(cur_sim_task, psig->file_name);
    }

    if(is_ef_support)
    {
        pcnf->result = SIM_READ_OK;
        //read the reacode want to read;
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, psig->file_name,
                                         psig->record_num, psig->record_mode, &(pcnf->buf.ad));

        //process the error,get the error reason
        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->result = SIM_READ_NOT_EXSITE;
    }

    //send confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}



/**********************************************************************
//    Description:
//      the function that process the transparent file update request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateTransparentEF(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_T_FILE_REQ_T *psig   //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_UPDATE_T_FILE_CNF_T *pcnf;
    BOOLEAN is_ef_support = FALSE;

    //create the confirm signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_T_FILE_CNF, sizeof(SIM_SIG_UPDATE_T_FILE_CNF_T), g_sim_task_id[cur_sim_task]);
    //get info for the confirm signal from request signal
    pcnf->command_ref = psig->command_ref;
    pcnf->file_name = psig->file_name;

    if(SIM_FILE_EF_CFFLAG == psig->file_name) //special operation for CFFLAG
    {
        pcnf->result = SIMSIGNAL_UpdateCallForwardFlag(cur_sim_task, psig->file_name, &(psig->buf.cfflag));
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }
    else if(SIM_FILE_EF_VMWFLAG == psig->file_name) //special operation for VWMFLAG
    {
        pcnf->result = SIMSIGNAL_UpdateMWIS(cur_sim_task, psig->file_name, &(psig->buf.vmw_ind_flag));
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //If the EF is  CPHS file, check it before update.
    is_ef_support = SIM_IsCPHSOptionalSupport(cur_sim_task, pcnf->file_name);

    if(is_ef_support)
    {
        is_ef_support = SIMSIGNAL_CheckTransparentEFInServiceTable(cur_sim_task, psig->file_name);
    }

    if(is_ef_support)
    {
        pcnf->result = SIM_UPDATE_OK;
        //update the transparent EF
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, psig->file_name, &(psig->buf.ad));

        //get the fail result of the signal
        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisUpdateFileFail(ser_result);
        }
        else
        {
            //read for L4 update the memory
            ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, psig->file_name, &(pcnf->buf.buf));
        }
    }
    else
    {
        pcnf->result = SIM_UPDATE_NOT_EXISTE;
    }

    //send the condirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

#ifdef GSM_CUSTOMER_AFP_SUPPORT
/**********************************************************************
//    Description:
//      the function that process the sim language update request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateLanguageReq(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_LANGUAGE_REQ_T *psig //in:the request signal
)
{
    SIM_SIG_UPDATE_LANGUAGE_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;
    uint8 temp_max_num = 0;
    uint8 orig_valid_num = 0;

    //create the coonfirm signal for the requst signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_LANGUAGE_CNF, sizeof(SIM_SIG_UPDATE_LANGUAGE_CNF_T), g_sim_task_id[cur_sim_task]);

    //clear all
    pcnf->is_update_ok = FALSE;
    pcnf->language_mapping.is_1st_valid = FALSE;
    pcnf->language_mapping.elp_1st_max_num = 0;
    pcnf->language_mapping.elp_1st_valid_num = 0;
    pcnf->language_mapping.is_lp_valid = FALSE;
    pcnf->language_mapping.lp_1st_max_num = 0;
    pcnf->language_mapping.lp_1st_valid_num = 0;
    pcnf->language_mapping.is_2nd_valid = FALSE;
    pcnf->language_mapping.elp_2nd_max_num = 0;
    pcnf->language_mapping.elp_2nd_valid_num = 0;
    SCI_MEMSET(pcnf->language_mapping.elp_1st_lang_codes, 0xFF, sizeof(SIM_EXT_LANGUAGE_CODE_ARR));
    SCI_MEMSET(pcnf->language_mapping.lp_1st_lang_codes, 0xFF, sizeof(SIM_LANGUAGE_CODE_ARR));
    SCI_MEMSET(pcnf->language_mapping.elp_2nd_lang_codes, 0xFF, sizeof(SIM_EXT_LANGUAGE_CODE_ARR));

#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        temp_max_num = MIN(s_sim_language_length_info[cur_sim_task].elp_1st_max_num, SIM_MAX_VALIDATE_LANGUAGE_NUM);
        psig->elp_mapping.max_num = temp_max_num;

        if(psig->elp_mapping.valid_num > temp_max_num)
        {
            psig->elp_mapping.valid_num = temp_max_num;
        }

        //ELP is more priority
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ELP, &(psig->elp_mapping));

        if(SIM_SERVICE_OK == ser_result)
        {
            pcnf->language_mapping.is_1st_valid = TRUE;
            pcnf->language_mapping.elp_1st_max_num = psig->elp_mapping.max_num;
            pcnf->language_mapping.elp_1st_valid_num = psig->elp_mapping.valid_num;
            SCI_MEMCPY((uint8 *)(&(pcnf->language_mapping.elp_1st_lang_codes)),
                       (uint8 *)(&(psig->elp_mapping.lang_codes)), (psig->elp_mapping.valid_num)*sizeof(SIM_EXT_LANGUAGE_CODE_T));
        }
        else
        {
            //ELP failed, then update LP
            temp_max_num = MIN(s_sim_language_length_info[cur_sim_task].lp_1st_max_num, SIM_MAX_VALIDATE_LANGUAGE_NUM);
            psig->lp_mapping.max_num = temp_max_num;

            if(psig->lp_mapping.valid_num > temp_max_num)
            {
                psig->lp_mapping.valid_num = temp_max_num;
            }

            ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LP, &(psig->lp_mapping));

            if(SIM_SERVICE_OK == ser_result)
            {
                pcnf->language_mapping.is_lp_valid = TRUE;
                pcnf->language_mapping.lp_1st_max_num = psig->lp_mapping.valid_num;
                pcnf->language_mapping.lp_1st_valid_num = psig->lp_mapping.valid_num;
                SCI_MEMCPY((uint8 *)(&(pcnf->language_mapping.lp_1st_lang_codes)),
                           (uint8 *)(&(psig->lp_mapping.lang_codes)), (psig->lp_mapping.valid_num)*sizeof(SIM_LANGUAGE_CODE));
            }
        }
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        temp_max_num = MIN(s_sim_language_length_info[cur_sim_task].elp_1st_max_num, SIM_MAX_VALIDATE_LANGUAGE_NUM);
        psig->elp_mapping.max_num = temp_max_num;
        orig_valid_num = psig->elp_mapping.valid_num; // to be restored in case ADFusim/EFli(6F 05) updation fails

        if(psig->elp_mapping.valid_num > temp_max_num)
        {
            psig->elp_mapping.valid_num = temp_max_num;
        }

        //In 3G USIM, ADFusim/EFli(6F 05) is more priority
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_LI, &(psig->elp_mapping));

        if(SIM_SERVICE_OK == ser_result)
        {
            pcnf->language_mapping.is_1st_valid = TRUE;
            pcnf->language_mapping.elp_1st_max_num = psig->elp_mapping.max_num;
            pcnf->language_mapping.elp_1st_valid_num = psig->elp_mapping.valid_num;
            SCI_MEMCPY((uint8 *)(&(pcnf->language_mapping.elp_1st_lang_codes)),
                       (uint8 *)(&(psig->elp_mapping.lang_codes)), (psig->elp_mapping.valid_num)*sizeof(SIM_EXT_LANGUAGE_CODE_T));
        }
        else
        {
            //EFli update failed, then update EFpl
            // restore the psig->elp_mapping.valid_num 	 
            psig->elp_mapping.valid_num = orig_valid_num;  

            temp_max_num = MIN(s_sim_language_length_info[cur_sim_task].elp_2nd_max_num, SIM_MAX_VALIDATE_LANGUAGE_NUM);
            psig->elp_mapping.max_num = temp_max_num;

            if(psig->elp_mapping.valid_num > temp_max_num)
            {
                psig->elp_mapping.valid_num = temp_max_num;
            }

            ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ELP, &(psig->elp_mapping));

            if(SIM_SERVICE_OK == ser_result)
            {
                pcnf->language_mapping.is_2nd_valid = TRUE;
                pcnf->language_mapping.elp_2nd_max_num = psig->elp_mapping.max_num;
                pcnf->language_mapping.elp_2nd_valid_num = psig->elp_mapping.valid_num;
                SCI_MEMCPY((uint8 *)(&(pcnf->language_mapping.elp_2nd_lang_codes)),
                           (uint8 *)(&(psig->elp_mapping.lang_codes)), (psig->elp_mapping.valid_num)*sizeof(SIM_EXT_LANGUAGE_CODE_T));
            }
        }
    }

#endif

    if(SIM_SERVICE_OK == ser_result)
    {
        pcnf->is_update_ok = TRUE;
    }

    SCI_TRACE_LOW("SIM%d: UPDATE CNF is_1st_valid%d, is_lp_valid%d, is_2nd_valid%d", cur_sim_task,
                  pcnf->language_mapping.is_1st_valid,
                  pcnf->language_mapping.is_lp_valid,
                  pcnf->language_mapping.is_2nd_valid);

    //send the condirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

/**********************************************************************
//    Description:
//      the function that process the recordy file update request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateRecordEF(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_R_FILE_REQ_T *psig //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_UPDATE_R_FILE_CNF_T *pcnf;
    BOOLEAN is_ef_support = FALSE;

    //create the coonfirm signal for the requst signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_R_FILE_CNF, sizeof(SIM_SIG_UPDATE_R_FILE_CNF_T), g_sim_task_id[cur_sim_task]);
    //get info for the confirm signal from request signal
    pcnf->command_ref = psig->command_ref;
    pcnf->file_name = psig->file_name;
    pcnf->record_id = psig->record_num;
    pcnf->record_mode = psig->record_mode;

    //If the EF is optional CPHS file, check it before update.
    is_ef_support = SIM_IsCPHSOptionalSupport(cur_sim_task, pcnf->file_name);

    if(is_ef_support)
    {
        is_ef_support = SIMSIGNAL_CheckRecordEFInServiceTable(cur_sim_task, pcnf->file_name);
    }

    if(is_ef_support)
    {
        pcnf->result = SIM_UPDATE_OK;
        ser_result = SIMSER_UpdateEFRecord(cur_sim_task, psig->file_name, psig->record_num,
                                           psig->record_mode, &(psig->buf.ad), FALSE);

        //get the fail result of the signal
        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisUpdateFileFail(ser_result);
        }
    }
    else
    {
        pcnf->result = SIM_UPDATE_NOT_EXISTE;
    }

    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

#ifdef GSM_DPHONE_SUPPORT
/**********************************************************************
//    Description:
//      the function that send the SIM initialization requset to SIM task
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
void SIMSIGNAL_SIMInitReq(uint32 cur_sim_task)
{

    SIM_SIG_INITIALIZE_REQ_T *req_ptr = PNULL;

    SCI_CREATE_SIGNAL(
        req_ptr,
        SIM_INITIALIZE_REQ,
        sizeof(SIM_SIG_INITIALIZE_REQ_T),
        s_sim_sender[cur_sim_task]);

    req_ptr->chv1 = s_sim_chv1[cur_sim_task];
    req_ptr->unblock_chv1 = s_sim_unblock_chv1[cur_sim_task] ;
    req_ptr->use_unblock = s_sim_use_unblock[cur_sim_task];
    req_ptr->command_ref = s_sim_command_ref[cur_sim_task];

    SCI_SEND_SIGNAL(req_ptr, g_sim_task_id[cur_sim_task]);
}

/**********************************************************************
//    Description:
//      the function that send the SIM initialization not OK signal to MN
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
void SIMSIGNAL_SIMInitNotOK(uint32 cur_sim_task)
{
    SIM_SIG_INITIALIZE_CNF_T *ind;
    BLOCK_ID MN_PID;

    MN_PID = (cur_sim_task == 0) ? P_MN : g_P_MN_ID[cur_sim_task];

    //creat the initialize confirm signal of the request signal
    SCI_CREATE_SIGNAL(ind, SIM_INITIALIZE_CNF, sizeof(SIM_SIG_INITIALIZE_CNF_T), g_sim_task_id[cur_sim_task]);
    ind->result = SIM_INI_ERROR_DPHONE;

    //send confirm signal to the signal sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)ind, MN_PID);
}

/**********************************************************************
//    Description:
//       Dphone Refresh timeout procedure function
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
STATIC void DphoneRefreshTimeOut(uint32 lparam)
{
    SIM_SIG_DPHONE_REFRESH_TIMEOUT_IND_T *psig;

    //SCI_TRACE_LOW:"SIM: Dphone refresh timeout!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_994_112_2_17_22_59_54_4617, (uint8 *)"");

    if(SCI_IsTimerActive(s_sim_dphone_refresh_timer[lparam]))
    {
        SCI_DeactiveTimer(s_sim_dphone_refresh_timer[lparam]);
    }

    SCI_CREATE_SIGNAL(psig, SIM_DPHOEN_REFRESH_TIMEOUT_IND, sizeof(SIM_SIG_DPHONE_REFRESH_TIMEOUT_IND_T), g_sim_task_id[lparam]);

    SCI_SEND_SIGNAL(psig, g_sim_task_id[lparam]);
}

/*****************************************************************************/
//  Description: For Dphone Refresh
//  Global resource dependence :
//  Author: Jacky.ying
//  Note:
/*****************************************************************************/
LOCAL void SIM_DphoneRefreshReq(uint32  cur_sim_task)
{
    SIM_SIG_REFRESH_INI_REQ_T *req_ptr = PNULL;

    SCI_CREATE_SIGNAL(
        req_ptr,
        SIM_REFRESH_INI_REQ,
        sizeof(SIM_SIG_REFRESH_INI_REQ_T),
        s_sim_sender[cur_sim_task]);

    req_ptr->command_ref = 0;
    //New 6530 and main donot send the CHV1 here

    SCI_SEND_SIGNAL(req_ptr, g_sim_task_id[cur_sim_task]);
}
#endif

#ifndef SIM_CODE_COMPACT
/**********************************************************************
//    Description:
//      the function that process the EF file increase request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_IncreaseEF(
    uint32 cur_sim_task,
    SIM_SIG_INCREASE_FILE_REQ_T *psig //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_INCREASE_FILE_CNF_T *pcnf;

    SCI_CREATE_SIGNAL(pcnf, SIM_INCREASE_FILE_CNF, sizeof(SIM_SIG_INCREASE_FILE_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->file_name = psig->file_name;

    pcnf->result = SIM_INCREASE_FILE_OK;


    ser_result = SIMSER_IncreaseEF(cur_sim_task, psig->file_name, psig->data, &(pcnf->increase_result));

    //the service failed,analyze the fail reason
    if(ser_result != SIM_SERVICE_OK)
    {
        pcnf->result = AnalysisIncreaseFileFail(ser_result);
    }

    //send confirm signal to the signal sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      read imsi again,because for the Dphone encrypt card,the IMSI will
//    changed from 0xff... to valid value
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_EncryptReadIMSI(
    uint32 cur_sim_task,
    SIM_SIG_INIT_ENCRYPT_REQ_T *psig  //the service request signal
)
{
    SIM_SIG_INIT_ENCRYPT_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    SCI_CREATE_SIGNAL(pcnf, SIM_INIT_ENCRYPT_CNF, (uint16)(sizeof(SIM_SIG_INIT_ENCRYPT_CNF_T)), g_sim_task_id[cur_sim_task]);

    pcnf->result = SIM_SERVICE_OK;
    //read EF_IMSI
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_IMSI,
                                               &(pcnf->imsi_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        SCI_FREE_SIGNAL(pcnf);
        return FALSE;
    }

    g_sim_card_setting[cur_sim_task].imsi_mapping = pcnf->imsi_mapping;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
    return TRUE;
}

/**********************************************************************
//    Description:
//      process the Get SPN service request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_GetSPN(
    uint32 cur_sim_task,
    SIM_SIG_GET_SPN_REQ_T *psig //the service request signal
)
{
    SIM_SIG_GET_SPN_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal
    SCI_CREATE_SIGNAL(pcnf, SIM_GET_SPN_CNF, sizeof(SIM_SIG_GET_SPN_CNF_T), g_sim_task_id[cur_sim_task]);

    //default set the ok to the result
    pcnf->result = SIM_READ_OK;

    //check the SPN service enable or not in the SIM card
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SPN))
    {
        pcnf->is_service_support = TRUE;

        //read the SPN from EFspn
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_SPN, &(pcnf->spn_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the session termination service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_SessionTerminate(
    uint32 cur_sim_task,
    SIM_SIG_SESSION_TERMINATION_REQ_T *psig //the request signal
)
{
    SIM_SIG_SESSION_TERMINATION_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_INCREASE_RESULT_T increase_result;

    //create the confirm signal fro the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SESSION_TERMINATION_CNF, sizeof(SIM_SIG_SESSION_TERMINATION_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_ST_OK;

    //Location Information update
    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LOCI, &(psig->loci_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = SIM_ST_UPDATE_LOCI_ERROR;
        goto SEND_CONFIRM;
    }

    g_sim_card_setting[cur_sim_task].loci_info = psig->loci_mapping;

    //Cipher Key Update
    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KC, &(psig->kc_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = SIM_ST_UPDATE_KC_ERROR;
        goto SEND_CONFIRM;
    }

    //BCCH information update
    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_BCCH, &psig->bcch_mapping);

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = SIM_ST_UPDATE_BCCH_ERROR;
        goto SEND_CONFIRM;
    }

    //Forbidden PLMN update
    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_FPLMN, &(psig->fplmn_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = SIM_ST_UPDATE_FPLMN_ERROR;
        goto SEND_CONFIRM;
    }

    //Advice of Charge increase
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
    {
        ser_result = SIMSER_IncreaseEF(cur_sim_task, SIM_FILE_EF_ACM, psig->acm_data, &increase_result);

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = SIM_ST_INCREASE_ACM_ERROR;
            goto SEND_CONFIRM;
        }
    }

SEND_CONFIRM:
    //send the confirm of the service request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Emergency Call codes request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadECC(
    uint32 cur_sim_task,
    SIM_SIG_READ_ECC_REQ_T *psig
)
{
    SIM_SIG_READ_ECC_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    SCI_CREATE_SIGNAL(pcnf, SIM_READ_ECC_CNF, sizeof(SIM_SIG_READ_ECC_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;

    pcnf->ecc_exist = TRUE;
    pcnf->result = SIM_READ_OK;

    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ECC, &(pcnf->ecc_mapping));

    if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
    {
        pcnf->ecc_exist = FALSE;
    }
    else if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    //send the confirm signal for the service request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Language preference request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadLP(
    uint32 cur_sim_task,
    SIM_SIG_READ_LP_REQ_T *psig //the service request signal
)
{
    SIM_SIG_READ_LP_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    SCI_CREATE_SIGNAL(pcnf, SIM_READ_LP_CNF, sizeof(SIM_SIG_READ_LP_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LP, &(pcnf->lp_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Language preference update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateLP(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_LP_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_UPDATE_LP_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_LP_CNF, sizeof(SIM_SIG_UPDATE_LP_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LP, &(psig->lp_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisUpdateFileFail(ser_result);
    }

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Administrative information request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadAD(
    uint32 cur_sim_task,
    SIM_SIG_READ_AD_REQ_T *psig //the service request signal
)
{
    SIM_SIG_READ_AD_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    SCI_CREATE_SIGNAL(pcnf, SIM_READ_AD_CNF, sizeof(SIM_SIG_READ_AD_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_AD, &(pcnf->ad_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    //confirm the service request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the SIM service talbe request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadSST(
    uint32 cur_sim_task,
    SIM_SIG_READ_SST_REQ_T *psig    //ther service requst signal
)
{
    SIM_SIG_READ_SST_CNF_T *pcnf = NULL;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_SST_CNF, sizeof(SIM_SIG_READ_SST_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //get the service table the g_card_setting
    SCI_MEMCPY(&(pcnf->sst_mapping), &(g_sim_card_setting[cur_sim_task].service_table), sizeof(SIM_EF_MAPPING_SST_T));

    //confirm the service request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the SIM phase request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadPhase(
    uint32 cur_sim_task,
    SIM_SIG_READ_PHASE_REQ_T *psig  //the service request signal
)
{
    SIM_SIG_READ_PHASE_CNF_T *pcnf = NULL;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_PHASE_CNF, sizeof(SIM_SIG_READ_PHASE_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //get the phase from the g_sim_card_setting[cur_sim_task]
    pcnf->phase_mapping.phase = g_sim_card_setting[cur_sim_task].card_phase;
    //confirm the service request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Extended Language preference request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadELP(
    uint32 cur_sim_task,
    SIM_SIG_READ_ELP_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_READ_ELP_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_ELP_CNF, sizeof(SIM_SIG_READ_ELP_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //read the elp from the EFelp
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ELP, &(pcnf->elp_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    //confirm the service request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Extended Language preference update service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateELP(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_ELP_REQ_T *psig  //the service request signal
)
{
    SIM_SIG_UPDATE_ELP_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_ELP_CNF, sizeof(SIM_SIG_UPDATE_ELP_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->command_ref = SIM_UPDATE_OK;

    //update the EFelp with the elp_mapping in the request signal
    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ELP, &(psig->elp_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        AnalysisUpdateFileFail(ser_result);
    }

    //confirm the service request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the IMSI request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadIMSI(
    uint32 cur_sim_task,
    SIM_SIG_READ_IMSI_REQ_T *psig   //the serivce request signal
)
{
    SIM_SIG_READ_IMSI_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_IMSI_CNF, (uint16)(sizeof(SIM_SIG_READ_IMSI_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //read the IMSI from the EFimsi
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_IMSI, &(pcnf->imsi_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    g_sim_card_setting[cur_sim_task].imsi_mapping = pcnf->imsi_mapping;

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Access control request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadACC(
    uint32 cur_sim_task,
    SIM_SIG_READ_ACC_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_READ_ACC_CNF_T *pcnf;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_ACC_CNF, sizeof(SIM_SIG_READ_ACC_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_SERVICE_OK;

    //read the acc from EFacc
    pcnf->result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ACC, &(pcnf->acc_mapping));

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the HPLMN search period request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadHPLMN(
    uint32 cur_sim_task,
    SIM_SIG_READ_HPLMN_REQ_T *psig  //the service request signal
)
{
    SIM_SIG_READ_HPLMN_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_HPLMN_CNF, sizeof(SIM_SIG_READ_HPLMN_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //read the HPLMN period from the EFhplmn
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_HPLMN, &(pcnf->hplmn_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Location information request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadLOCI(
    uint32 cur_sim_task,
    SIM_SIG_READ_LOCI_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_READ_LOCI_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_LOCI_CNF, (uint16)(sizeof(SIM_SIG_READ_LOCI_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //read the LOCI information from the EFloci
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LOCI, &(pcnf->loci_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    g_sim_card_setting[cur_sim_task].loci_info = pcnf->loci_mapping;

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Location information update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateLOCI(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_LOCI_REQ_T *psig //the service request signal
)
{
    SIM_SIG_UPDATE_LOCI_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_LOCI_CNF, sizeof(SIM_SIG_UPDATE_LOCI_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //udpate the EFloci using the LOCI mapping
    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LOCI, &(psig->loci_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisUpdateFileFail(ser_result);
    }

    g_sim_card_setting[cur_sim_task].loci_info = psig->loci_mapping;

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Cipher Key request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadKC(
    uint32 cur_sim_task,
    SIM_SIG_READ_KC_REQ_T *psig //the service request signal
)
{
    SIM_SIG_READ_KC_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_KC_CNF, (uint16)(sizeof(SIM_SIG_READ_KC_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //read the Cipher Key from the EFkc
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KC, &(pcnf->kc_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Cipher Key update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateKC(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_KC_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_UPDATE_KC_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_KC_CNF, sizeof(SIM_SIG_UPDATE_KC_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //update the EFkc use the kc mapping
    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KC, &(psig->kc_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisUpdateFileFail(ser_result);
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the BCCH information request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadBCCH(
    uint32 cur_sim_task,
    SIM_SIG_READ_BCCH_REQ_T *psig   //the service request sginal
)
{
    SIM_SIG_READ_BCCH_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_BCCH_CNF, (uint16)(sizeof(SIM_SIG_READ_BCCH_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //read the bcch information from the EFbcch
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_BCCH,&(pcnf->bcch_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the BCCH information update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateBCCH(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_BCCH_REQ_T *psig //the service request sginal
)
{
    SIM_SIG_UPDATE_BCCH_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signalf or the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_BCCH_CNF, sizeof(SIM_SIG_UPDATE_BCCH_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //update the EFbcch using th bcch mapping
    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_BCCH, &psig->bcch_mapping);

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisUpdateFileFail(ser_result);
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Forbiden PLMN request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadFPLMN(
    uint32 cur_sim_task,
    SIM_SIG_READ_FPLMN_REQ_T *psig  //the service request signal
)
{
    SIM_SIG_READ_FPLMN_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_FPLMN_CNF, (uint16)(sizeof(SIM_SIG_READ_FPLMN_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //read the Forbiden PLMN from the EFfplmn
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_FPLMN, &(pcnf->fplmn_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Forbiden PLMN update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateFPLMN(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_FPLMN_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_UPDATE_FPLMN_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_FPLMN_CNF, sizeof(SIM_SIG_UPDATE_FPLMN_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //update the EFfplmn using the fplmn mapping
    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_FPLMN, &(psig->flpmn_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisUpdateFileFail(ser_result);
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the PLMN selctor request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadPLMNSEL(
    uint32 cur_sim_task,
    SIM_SIG_READ_PLMNSEL_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_READ_PLMNSEL_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_PLMNSEL_CNF, sizeof(SIM_SIG_READ_PLMNSEL_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check whether the PLMN selector service enable
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_PLMN_SEL))
    {
        pcnf->is_service_support = TRUE;
        //read the PLMN selector from the EFplmnsel
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PLMNsel, &(pcnf->plmn_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the PLMN selctor update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdatePLMNSEL(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_PLMNSEL_REQ_T *psig  //the service request signal
)
{
    SIM_SIG_UPDATE_PLMNSEL_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_PLMNSEL_CNF, sizeof(SIM_SIG_UPDATE_PLMNSEL_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //check whether the PLMN selector service enable
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_PLMN_SEL))
    {
        pcnf->is_service_support = TRUE;
        //update the EFplmnsel using the plmn mapping
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PLMNsel, &(psig->plmn_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisUpdateFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Cell broadcast message identifier request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadCBMI(
    uint32 cur_sim_task,
    SIM_SIG_READ_CBMI_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_READ_CBMI_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_CBMI_CNF, sizeof(SIM_SIG_READ_CBMI_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check whether the Cell broadcast Message identifier service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CBMI))
    {
        pcnf->is_service_support = TRUE;
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CBMI, &(pcnf->cbmi_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Cell broadcast message identifier update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateCBMI(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_CBMI_REQ_T *psig //the service request signal
)
{
    SIM_SIG_UPDATE_CBMI_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_CBMI_CNF, sizeof(SIM_SIG_UPDATE_CBMI_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //check whether the Cell broadcast Message identifier service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CBMI))
    {
        pcnf->is_service_support = TRUE;
        //update the EFcbmi using the CBMI mapping
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CBMI, &(psig->cbmi_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisUpdateFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Voice group call request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadVGCS(
    uint32 cur_sim_task,
    SIM_SIG_READ_VGCS_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_READ_VGCS_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_VGCS_CNF, sizeof(SIM_SIG_READ_VGCS_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enable or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_VGCS))
    {
        pcnf->is_service_support = TRUE;

        //read the VGCS list from the EFvgcs
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_VGCS, &(pcnf->vgcs_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Voice group call service status request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadVGCSS(
    uint32 cur_sim_task,
    SIM_SIG_READ_VGCSS_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_READ_VGCSS_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_VGCSS_CNF, sizeof(SIM_SIG_READ_VGCSS_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_VGCS))
    {
        pcnf->is_service_support = TRUE;

        //get the VGCS status from the EFvgcss
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_VGCSS, &(pcnf->vgcss_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Voice broadcase request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadVBS(
    uint32 cur_sim_task,
    SIM_SIG_READ_VBS_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_READ_VBS_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //creat the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_VBS_CNF, sizeof(SIM_SIG_READ_VBS_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_VBS))
    {
        pcnf->is_service_support = TRUE;

        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_VBS, &(pcnf->vbs_support));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Voice broadcast status request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadVBSS(
    uint32 cur_sim_task,
    SIM_SIG_READ_VBSS_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_READ_VBSS_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_VBSS_CNF, sizeof(SIM_SIG_READ_VBSS_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_VBS))
    {
        pcnf->is_service_support = TRUE;
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_VBSS, &(pcnf->vbss_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Enhanced Multi Level Pre_emption and Priority request service
//      supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadEMLPP(
    uint32 cur_sim_task,
    SIM_SIG_READ_EMLPP_REQ_T *psig  //the service request signal
)
{
    SIM_SIG_READ_EMLPP_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_EMLPP_CNF, sizeof(SIM_SIG_READ_EMLPP_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EMLPP))
    {
        pcnf->is_service_support = TRUE;

        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_eMLPP, &(pcnf->emlpp_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Automatic Answer on eMLPP  request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadAAEM(
    uint32 cur_sim_task,
    SIM_SIG_READ_AAEM_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_READ_AAEM_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_AAEM_CNF, sizeof(SIM_SIG_READ_AAEM_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AAEM))
    {
        pcnf->is_service_support = TRUE;

        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_AAem, &(pcnf->aaem_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Automatic Answer on eMLPP  update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateAAEM(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_AAEM_REQ_T *psig //the service request signal
)
{
    SIM_SIG_UPDATE_AAEM_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_AAEM_CNF, sizeof(SIM_SIG_UPDATE_AAEM_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AAEM))
    {
        pcnf->is_service_support = TRUE;

        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_AAem, &(psig->aaem_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisUpdateFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Cell Broadcase message range identifier request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadCBMIR(
    uint32 cur_sim_task,
    SIM_SIG_READ_CBMIR_REQ_T *psig  //the service request signal
)
{
    SIM_SIG_READ_CBMIR_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_CBMIR_CNF, sizeof(SIM_SIG_READ_CBMIR_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CBMIR))
    {
        pcnf->is_service_support = TRUE;

        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CBMIR, &(pcnf->cbmir_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Cell Broadcase message range identifier update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateCBMIR(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_CBMIR_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_UPDATE_CBMIR_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_CBMIR_CNF, sizeof(SIM_SIG_UPDATE_CBMIR_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CBMIR))
    {
        pcnf->is_service_support = TRUE;

        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CBMIR, &(psig->cbmir_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisUpdateFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Depersonalisation Control Keys request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadDCK(
    uint32 cur_sim_task,
    SIM_SIG_READ_DCK_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_READ_DCK_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_DCK_CNF, sizeof(SIM_SIG_READ_DCK_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_DCK))
    {
        pcnf->is_service_support = TRUE;

        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_DCK, &(pcnf->dck_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Network's indication of alerting request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadNIA(
    uint32 cur_sim_task,
    SIM_SIG_READ_NIA_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_READ_NIA_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_NIA_CNF, sizeof(SIM_SIG_READ_NIA_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_NIA))
    {
        pcnf->is_service_support = TRUE;

        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_NIA, &(pcnf->nia_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

#ifdef SIM_AOC_SUPPORT
/**********************************************************************
//    Description:
//      the Accumulate call meter request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadACM(
    uint32 cur_sim_task,
    SIM_SIG_AOC_READ_ACM_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_AOC_READ_ACM_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_AOC_READ_ACM_CNF, sizeof(SIM_SIG_AOC_READ_ACM_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
    {
        pcnf->is_service_support = TRUE;

        //get the ACM value from the EFacm
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_ACM, 1, SIM_RECORD_ABSOLUTE, &(pcnf->acm_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Accumulate call meter maximum value request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadACMMAX(
    uint32 cur_sim_task,
    SIM_SIG_AOC_READ_ACMMAX_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_AOC_READ_ACMMAX_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_AOC_READ_ACMMAX_CNF, sizeof(SIM_SIG_AOC_READ_ACMMAX_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
    {
        pcnf->is_service_support = TRUE;
        //read the acm max info from the EFacmmax
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ACMmax, &(pcnf->acmmax_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

#ifndef SIM_CODE_COMPACT
/********************************************************************
//    DESCRIPTION
//     Get the imsi of the cdma card
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_GetCDMACardIMSI(
    uint32 cur_sim_task,
    SIM_SIG_GET_CDMA_CARD_IMSI_REQ_T *psig //the service request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SIG_GET_CDMA_CARD_IMSI_CNF_T *pcnf;
    SIM_EF_MAPPING_CDMA_IMSI_M_T cdma_imsi_mapping;

    // 04/24/2008 delete the trace
    //     SCI_TRACE_LOW("SIM::get the cdma card imsi.");

    SCI_CREATE_SIGNAL(pcnf, SIM_GET_CDMA_CARD_IMSI_CNF, sizeof(SIM_SIG_GET_CDMA_CARD_IMSI_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->result = GET_CIMSI_RESULT_FILE_NOT_EXIST;
    SCI_MEMSET(pcnf->imsi_m.imsi_m, 0xff, SIM_EF_CDMA_IMSI_M_LEN);

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM: g_sim_card_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2671_112_2_17_22_59_57_4623, (uint8 *)"d", g_sim_card_state[cur_sim_task]);
#endif

    if(SIM_NULL_STATE == g_sim_card_state[cur_sim_task])
    {
        pcnf->result = GET_CIMSI_RESULT_NO_CARD;
    }
    else if((SIM_NO_POWER_STATE == g_sim_card_state[cur_sim_task])
            || (SIM_ATR_STATE == g_sim_card_state[cur_sim_task]))
    {
        pcnf->result = GET_CIMSI_RESULT_CARD_NOT_READY;
    }
    else
    {
        //select the DF_CDMA to and get the select output data
        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_CDMA, &select_data);
#ifndef WIN32
        SIM_PrintTxBuf(cur_sim_task);
        SIM_PrintRxBuf(cur_sim_task);
#endif

#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM:: select DFcdma ser_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2692_112_2_17_22_59_57_4624, (uint8 *)"d", ser_result);
#endif

        if(ser_result != SIM_SERVICE_OK)
        {
            if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
            {
                pcnf->result = GET_CIMSI_RESULT_FILE_NOT_EXIST;
            }
            else
            {
                pcnf->result = GET_CIMSI_RESULT_FAILED;
            }
        }
        else
        {
            //get information of EF_IMSI
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_CDMA_IMSI,
                                           &select_data);
#ifndef WIN32
            SIM_PrintTxBuf(cur_sim_task);
            SIM_PrintRxBuf(cur_sim_task);
#endif

            if(ser_result != SIM_SERVICE_OK)
            {
                if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
                {
#ifndef _LESSEN_TRACE_SUPPORT
                    //SCI_TRACE_LOW:"SIM::no EFimsi in the DFcdma."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2719_112_2_17_22_59_57_4625, (uint8 *)"");
#endif
                    pcnf->result = GET_CIMSI_RESULT_FILE_NOT_EXIST;
                }
                else
                {
#ifndef _LESSEN_TRACE_SUPPORT
                    //SCI_TRACE_LOW:"SIM::select EFimsi in the DFcdma failed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2726_112_2_17_22_59_57_4626, (uint8 *)"");
#endif
                    pcnf->result = GET_CIMSI_RESULT_FAILED;
                }
            }
            //the file is validate,read the EF_IMSI directly
            else if(select_data.file_status & SIM_FILE_INVALIDATE_MARK)
            {
                //read EF_IMSI
                ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CDMA_IMSI,
                                                           &cdma_imsi_mapping);
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
                SIM_PrintRxBuf(cur_sim_task);
#endif

                if(SIM_SERVICE_OK != ser_result)
                {
#ifndef _LESSEN_TRACE_SUPPORT
                    //SCI_TRACE_LOW:"SIM::read cdma imsi failed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2744_112_2_17_22_59_57_4627, (uint8 *)"");
#endif
                    pcnf->result = GET_CIMSI_RESULT_FAILED;
                }
                else
                {
#ifndef _LESSEN_TRACE_SUPPORT
                    //SCI_TRACE_LOW:"SIM::read cdma imsi OK."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2751_112_2_17_22_59_57_4628, (uint8 *)"");
#endif
                    pcnf->result = GET_CIMSI_RESULT_OK;
                    SCI_MEMCPY(&pcnf->imsi_m, &cdma_imsi_mapping, sizeof(SIM_EF_MAPPING_CDMA_IMSI_M_T));
                }
            }
            else
            {
#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM::the EFimsi of the DFcdma is invalidate."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2760_112_2_17_22_59_57_4629, (uint8 *)"");
#endif
                pcnf->result = GET_CIMSI_RESULT_FILE_NOT_EXIST;
            }
        }
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

/**********************************************************************
//    Description:
//      process the Get file info service that request
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMSIGNAL_GetFileInfo(
    uint32 cur_sim_task,
    SIM_SIG_GET_FILE_INFO_REQ_T *psig   //in:the request signal
)
{
    SIM_SIG_GET_FILE_INFO_CNF_T *pcnf;
    uint8 i, tmp_path_len = 0;
    uint16 path_id[6] = {0};
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_FILE_NAME_E file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_id);

    //creat the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_GET_FILE_INFO_CNF, sizeof(SIM_SIG_GET_FILE_INFO_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->file_id = psig->file_id;

    if(SIM_FILE_TOTAL_NUM == file_name)
    {
        if((0x7000 == (psig->file_id & 0xF000))
            || (0x6000 == (psig->file_id & 0xF000))
            || (0x5000 == (psig->file_id & 0xF000))
            || (0x4000 == (psig->file_id & 0xF000))
            || (0x3F00 == psig->file_id)
            || (0x2000 == (psig->file_id & 0xF000)))
        {
            if(psig->path_len > 10)
            {
                tmp_path_len = 10;
            }
            else
            {
                tmp_path_len = psig->path_len;
            }

            for(i = 0; i < tmp_path_len; i += 2)
            {
                path_id[i/2] = (uint16)((((uint16)(psig->path_id[i])) << 8) | (psig->path_id[i+1]));
            }

            if(SIM_SERVICE_OK == SIMINSTR_SelectFileByPath(cur_sim_task, psig->file_id,
                                                           (psig->path_len) / 2, path_id, &select_data))
            {
                SCI_MEMCPY(&(pcnf->select_data),&select_data,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                pcnf->result = SIM_GET_FILE_INFO_OK;
            }
            else
            {
                pcnf->result = SIM_GET_UNKNOW_ERROR;
            }
        }
        else
        {
            // file ID not fount
            pcnf->result = SIM_GET_UNKNOW_ERROR;
        }
    }
    else
    {
        if(SIM_SERVICE_OK == SIMSER_SelectFile(cur_sim_task, file_name, &select_data))
        {
            SCI_MEMCPY(&(pcnf->select_data),&select_data,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
            pcnf->result = SIM_GET_FILE_INFO_OK;
        }
        else
        {
            pcnf->result = SIM_GET_UNKNOW_ERROR;
        }
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#if !defined (SIM_CODE_COMPACT) || defined (MODEM_PLATFORM)
/********************************************************************
//    DESCRIPTION
//     Get card type,including the get result,wether the card is present,
//    and the card type
//    AT+EUICC?
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_GetCardType(
    uint32 cur_sim_task,
    SIM_SIG_GET_CARD_TYPE_REQ_T *psig //the service request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SIG_GET_CARD_TYPE_CNF_T *pcnf;
    SIM_EF_MAPPING_IMSI_T imsi_mapping;
    BOOLEAN is_gsm_exist = FALSE, is_cdma_exist = FALSE, is_imsi_valid = FALSE;

    SCI_CREATE_SIGNAL(pcnf, SIM_GET_CARD_TYPE_CNF, sizeof(SIM_SIG_GET_CARD_TYPE_CNF_T), g_sim_task_id[cur_sim_task]);

    if(SIM_NULL_STATE == g_sim_card_state[cur_sim_task])
    {
        pcnf->is_have_card = FALSE;
        pcnf->result = GET_RESULT_OK;
    }
    else if((SIM_NO_POWER_STATE == g_sim_card_state[cur_sim_task])
            || (SIM_ATR_STATE == g_sim_card_state[cur_sim_task]))
    {
        pcnf->is_have_card = FALSE;
        pcnf->result = GET_RESULT_CARD_NOT_READY;
    }
    else
    {
#ifdef SIM_3G_USIM_SUPPORT

        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            pcnf->is_have_card = TRUE;
            pcnf->result = GET_RESULT_OK;
            pcnf->card_type = CARD_TYPE_USIM;
        }
        else
#endif
        {
            is_gsm_exist = FALSE;
            is_cdma_exist = FALSE;
            is_imsi_valid = FALSE;

#ifdef SIM_CDMA_CARD_SUPPORT

            if(SIM_ACTIVE_STATE == g_sim_card_state[cur_sim_task])
            {
                pcnf->is_have_card = TRUE;
                pcnf->result = GET_RESULT_OK;

                if(s_sim_cdma_flag[cur_sim_task])
                {
                    if(s_sim_cdma_only_flag[cur_sim_task])
                    {
                        pcnf->card_type = CARD_TYPE_UIM;
                    }
                    else
                    {
                        pcnf->card_type = CARD_TYPE_SIM_UIM_DUAL;
                    }
                }
                else
                {
                    pcnf->card_type = CARD_TYPE_SIM;
                }

                SCI_TRACE_LOW("SIMSIGNAL_GetCardType ActiveState %d %d %d", pcnf->result, pcnf->is_have_card, pcnf->card_type);
            }
            else
#endif
            {
                //select the DF_CDMA to and get the select output data
                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_CDMA, &select_data);

#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM:: select DFcdma ser_result = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2512_112_2_17_22_59_57_4618, (uint8 *)"d", ser_result);
#endif

                if(ser_result != SIM_SERVICE_OK)
                {
                    if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
                    {
                        is_cdma_exist = FALSE;
                    }
                    else
                    {
                        is_cdma_exist = FALSE;
                    }
                }
                else
                {
                    is_cdma_exist = TRUE;
                }

                //select the DF_GSM to and get the select output data
                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM:: select DFgsm ser_result = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2538_112_2_17_22_59_57_4619, (uint8 *)"d", ser_result);
#endif

                if(ser_result != SIM_SERVICE_OK)
                {
                    if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
                    {
                        is_gsm_exist = FALSE;
                    }
                    else
                    {
                        is_gsm_exist = FALSE;
                    }
                }
                else
                {
                    is_gsm_exist = TRUE;
                }

                if(is_gsm_exist && is_cdma_exist)
                {
#ifdef BYD_PRJ//BYD said, donot check the content of imsi.
                    pcnf->is_have_card = TRUE;
                    pcnf->result = GET_RESULT_OK;
#ifdef SIM_CDMA_CARD_SUPPORT
                    pcnf->card_type = CARD_TYPE_SIM_UIM_DUAL;
#else
                    pcnf->card_type = CARD_TYPE_SIM;
#endif
#else
                    //get information of EF_IMSI
                    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_IMSI, &select_data);

                    if(ser_result != SIM_SERVICE_OK)
                    {
#ifndef _LESSEN_TRACE_SUPPORT
                        //SCI_TRACE_LOW:"SIM::select imsi failed."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2568_112_2_17_22_59_57_4620, (uint8 *)"");
#endif
                        is_imsi_valid = FALSE;
                    }
                    //the file is validate,read the EF_IMSI directly
                    else if(select_data.file_status & SIM_FILE_INVALIDATE_MARK)
                    {
                        uint8 empty_str[MAX_MSIN_BYTE_NUM];
                        SCI_MEMSET(empty_str, 0, MAX_MSIN_BYTE_NUM);

                        //read EF_IMSI
                        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_IMSI, &imsi_mapping);

                        //SCI_TRACE_LOW:"SIM:: EFimsi ser_result = %d, msin = %d, %d, %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2586_112_2_17_22_59_57_4621, (uint8 *)"dddd", ser_result, imsi_mapping.msin[0], imsi_mapping.msin[1], imsi_mapping.msin[2]);

                        if(SIM_SERVICE_OK != ser_result)
                        {
                            is_imsi_valid = FALSE;
                        }
                        else if(0 == memcmp(imsi_mapping.msin, empty_str, MAX_MSIN_BYTE_NUM))
                        {
                            is_imsi_valid = FALSE;
                        }
                        else
                        {
                            //Accoding to ChinaTelecom,  case1: DFgsm/EFimsi 46099 is not valid. This also SS case1.
                            if((460 == imsi_mapping.mcc) && (99 == imsi_mapping.mnc))
                            {
                                is_imsi_valid = FALSE;
                            }
#ifdef _SUPPORT_IPC_//SS
                            //SS case2:  special value from SS.
                            else if ((0x08 == imsi_mapping.imsi.imsi_value[0]) 
                                && (0x49 == imsi_mapping.imsi.imsi_value[1]) 
                                && (0x06 == imsi_mapping.imsi.imsi_value[2]) 
                                && (0x10 == imsi_mapping.imsi.imsi_value[3]) 
                                && (0x00 == imsi_mapping.imsi.imsi_value[4]) 
                                && (0x00 == imsi_mapping.imsi.imsi_value[5]) 
                                && (0x00 == imsi_mapping.imsi.imsi_value[6]) 
                                && (0x00 == imsi_mapping.imsi.imsi_value[7]))
                            {
                                is_imsi_valid = FALSE;
                            }
                            //SS case3: All 0xFF in DFgsm/EFimsi
                            else if ((0xFF == imsi_mapping.imsi.imsi_value[0]) 
                                && (0xFF == imsi_mapping.imsi.imsi_value[1]) 
                                && (0xFF == imsi_mapping.imsi.imsi_value[2]) 
                                && (0xFF == imsi_mapping.imsi.imsi_value[3]) 
                                && (0xFF == imsi_mapping.imsi.imsi_value[4]) 
                                && (0xFF == imsi_mapping.imsi.imsi_value[5]) 
                                && (0xFF == imsi_mapping.imsi.imsi_value[6]) 
                                && (0xFF == imsi_mapping.imsi.imsi_value[7]))
                            {
                                is_imsi_valid = FALSE;
                            }
#endif
#ifdef SIM_CDMA_CARD_SUPPORT
                            //ChinaTelecom's spec
                            else if((0 != s_sim_cdma_imsi_m[cur_sim_task].mcc)
                                    && (0xFF != s_sim_cdma_imsi_m[cur_sim_task].mcc)
                                    && (s_sim_cdma_imsi_m[cur_sim_task].mcc == imsi_mapping.mcc)
                                    && (s_sim_cdma_imsi_m[cur_sim_task].mnc == imsi_mapping.mnc))
                            {
                                is_imsi_valid = FALSE;
                            }
#endif
                            else
                            {
                                is_imsi_valid = TRUE;
                            }
                        }
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"SIM::the imsi file is invalidate."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2602_112_2_17_22_59_57_4622, (uint8 *)"");
                        is_imsi_valid = FALSE;
                    }

                    if(is_imsi_valid)
                    {
                        // 04/24/2008 delete the trace
                        //                 SCI_TRACE_LOW("SIM::the card is dual mode card.");
                        pcnf->is_have_card = TRUE;
                        pcnf->result = GET_RESULT_OK;
#ifdef SIM_CDMA_CARD_SUPPORT
                        pcnf->card_type = CARD_TYPE_SIM_UIM_DUAL;
#else
                    pcnf->card_type = CARD_TYPE_SIM;
#endif
                    }
                    else
                    {
                        // 04/24/2008 delete the trace
                        //                 SCI_TRACE_LOW("SIM::the card is uim card.");
                        pcnf->is_have_card = TRUE;
                        pcnf->result = GET_RESULT_OK;
                        pcnf->card_type = CARD_TYPE_UIM;
                    }

#endif
                }
                else if(is_gsm_exist)
                {
                    pcnf->is_have_card = TRUE;
                    pcnf->result = GET_RESULT_OK;
                    pcnf->card_type = CARD_TYPE_SIM;
                }
                else if(is_cdma_exist)
                {
                    pcnf->is_have_card = TRUE;
                    pcnf->result = GET_RESULT_OK;
                    pcnf->card_type = CARD_TYPE_UIM;
                }
                else
                {
                    pcnf->is_have_card = TRUE;
                    pcnf->result = GET_RESULT_FAILED;
                }
            }
        }
    }

#ifdef SIM_CDMA_CARD_SUPPORT

    if((GET_RESULT_OK == pcnf->result) && (pcnf->is_have_card))
    {
        if(CARD_TYPE_UIM == pcnf->card_type)
        {
            s_sim_cdma_flag[cur_sim_task] = TRUE;
            s_sim_cdma_only_flag[cur_sim_task] = TRUE;
        }
        else if(CARD_TYPE_SIM_UIM_DUAL == pcnf->card_type)
        {
            s_sim_cdma_flag[cur_sim_task] = TRUE;
            s_sim_cdma_only_flag[cur_sim_task] = FALSE;
        }
        else
        {
            s_sim_cdma_flag[cur_sim_task] = FALSE;
            s_sim_cdma_only_flag[cur_sim_task] = FALSE;
        }
    }

#endif

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

#ifdef GSM_DPHONE_VER2_SUPPORT
/**********************************************************************
//    Description:
//    verify the chv1 when the Dphone version2 is in intialization.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_SIMDphonePinOperation(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_INITIALIZE_REQ_T *psig  //in:the initialization request signal
)
{
    SIM_SIG_DPHONE_PIN_CHECK_IND_T *ind;
    SIM_SERVICE_RESULT_E    ser_result     = SIM_SERVICE_OK;
    SIM_TERMINAL_PROFILE_DATA_T simat_profile;
    SIMAT_SIG_TERMINAL_PROFILE_IND_T *simat_ind;
#ifdef DPHONE_VER2_TEST
    SIMAT_SIG_PC_IND_T *ppcsig = NULL;
#endif

    s_sim_sender[cur_sim_task] = ((xSignalHeaderRec *)psig)->Sender;

    //creat the initialize confirm signal of the request signal
    SCI_CREATE_SIGNAL(ind, SIM_PIN_CHECK_IND, (uint16)(sizeof(SIM_SIG_DPHONE_PIN_CHECK_IND_T)), g_sim_task_id[cur_sim_task]);
    ind->command_ref = psig->command_ref;

    //verify the chv1
    VerifyCHV1AtInit(cur_sim_task, psig->chv1, psig->unblock_chv1, &s_sim_dphone_verify_pin_result[cur_sim_task]);
    ind->is_chv1_enabled = g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled;
    ind->chv_status = g_sim_card_setting[cur_sim_task].gsm_data.chv1_status;
    ind->unblock_status = g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status;

    SCI_TRACE_LOW("SIM%d: Dphone VerifyCHV1AtInit result %d", cur_sim_task, s_sim_dphone_verify_pin_result[cur_sim_task]);

    //send confirm signal to the signal sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)ind, ((xSignalHeaderRec *)psig)->Sender);

    if(SIM_INI_OK != s_sim_dphone_verify_pin_result[cur_sim_task])
    {
        //Wait for PIN check again
        return FALSE;
    }

    //the SIM Card require the ME to perform the profile download procedure.
    if(SIM_PH_2_PD == g_sim_card_setting[cur_sim_task].card_phase)
    {
        simat_profile.profile_len = SIM_MAX_C_APDU_DATA_LEN;
        SIMAT_GetMeProfile(simat_profile.profile_arr, &(simat_profile.profile_len));
        ser_result = SIMINSTR_TerminalProfile(cur_sim_task, &(simat_profile));

        if(SIM_SERVICE_OK != ser_result)
        {
            //SIM need not pending...
            SCI_TRACE_LOW("SIM%d: Dphone terminal profile fail!", cur_sim_task);
            return TRUE;
        }

        SCI_CREATE_SIGNAL(simat_ind, SIMAT_TERMINAL_PROFILE_IND, sizeof(SIMAT_SIG_TERMINAL_PROFILE_IND_T), g_sim_task_id[cur_sim_task]);
        SCI_SEND_SIGNAL(simat_ind, g_sim_simat_task_id[cur_sim_task]);
    }

#ifdef DPHONE_VER2_TEST

    SCI_TRACE_LOW("SIM: Dphone SIMAT_PC_IND  refresh send!");
    SCI_CREATE_SIGNAL(ppcsig, SIMAT_PC_IND, sizeof(SIMAT_SIG_PC_IND_T), g_sim_task_id[0]);

    ppcsig->pc_data.data_len = 11;
    ppcsig->pc_data.data_arr[0] = 0xD0;
    ppcsig->pc_data.data_arr[1] = 0x9;
    ppcsig->pc_data.data_arr[2] = 0x81;
    ppcsig->pc_data.data_arr[3] = 0x03;
    ppcsig->pc_data.data_arr[4] = 0x01;
    ppcsig->pc_data.data_arr[5] = 0x01;//Refresh
    ppcsig->pc_data.data_arr[6] = 0x00;//ini and full file change
    ppcsig->pc_data.data_arr[7] = 0x82;
    ppcsig->pc_data.data_arr[8] = 0x02;
    ppcsig->pc_data.data_arr[9] = 0x81;
    ppcsig->pc_data.data_arr[10] = 0x82;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)ppcsig, g_sim_simat_task_id[0]);
#endif

    return TRUE;
}

#endif

/**********************************************************************
//    Description:
//      the function that send the SIM initialization requset to SIM task
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
void SIMSIGNAL_SIMDphoneInitReq(uint32 cur_sim_task, BOOLEAN is_entire_service)
{
#ifdef GSM_DPHONE_VER2_SUPPORT
    SIM_SIG_INITIALIZE_REQ_T *req_ptr = PNULL;

    SCI_CREATE_SIGNAL(
        req_ptr,
        SIM_INITIALIZE_REQ,
        sizeof(SIM_SIG_INITIALIZE_REQ_T),
        s_sim_sender[cur_sim_task]);

    SCI_MEMSET(&(req_ptr->chv1), 0x0, sizeof(SIM_CHV_T));
    SCI_MEMSET(&(req_ptr->unblock_chv1), 0x0, sizeof(SIM_CHV_T));
    req_ptr->use_unblock = FALSE;
    req_ptr->command_ref = 0;

    s_sim_is_entire_service[cur_sim_task] = is_entire_service;
    SCI_SEND_SIGNAL(req_ptr, g_sim_task_id[cur_sim_task]);
#endif
}


/**********************************************************************
//    Description:
//      the function that process the SIM initialize signal,reference to the
//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_SIMInitialization(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_INITIALIZE_REQ_T *psig  //in:the initialization request signal
)
{
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_pin_ok_flag = FALSE;
#endif
    BOOLEAN result = FALSE;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_INITIALIZE_CNF_T *ind;
    uint8 imsi_is_ff_buf[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#ifdef GSM_CUSTOMER_AFP_SUPPORT
    uint8 imsi_is_00_buf[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
#endif
    SIM_RESULT_INITIALIZE_SIM_E ini_result = SIM_INI_OK;
#ifdef GSM_CUSTOMER_AFP_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT
    BOOLEAN is_plug_in_flag[SIM_CARD_NUM] = {FALSE};
    xSignalHeaderRec *pintind = NULL;
#endif
#endif

    //creat the initialize confirm signal of the request signal
    SCI_CREATE_SIGNAL(ind, SIM_INITIALIZE_CNF, (uint16)(sizeof(SIM_SIG_INITIALIZE_CNF_T)), g_sim_task_id[cur_sim_task]);
    ind->command_ref = psig->command_ref;
    ind->result = SIM_INI_OK;

#ifdef GSM_DPHONE_VER2_SUPPORT

    if(!s_sim_is_dphone_operation_skip[cur_sim_task])
#endif
    {
#ifdef MODEM_PLATFORM
        SIM_RESPONSE_DATA_OF_SELECT_T temp;
        ser_result = SIMSER_SelectFile(cur_sim_task,SIM_FILE_DF_GSM,&temp);
        if (SIM_SERVICE_OK == ser_result)
#endif
        {
            //verify the chv1
            VerifyCHV1AtInit(cur_sim_task, psig->chv1, psig->unblock_chv1, &(ind->result));
        }
    }

#ifdef GSM_DPHONE_VER2_SUPPORT
    else
    {
        ind->result = s_sim_dphone_verify_pin_result[cur_sim_task];
    }

#endif
    ind->ini_info.is_chv1_enabled = g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled;
    ind->ini_info.chv_status = g_sim_card_setting[cur_sim_task].gsm_data.chv1_status;
    ind->ini_info.unblock_status = g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status;

    if(SIM_INI_OK != ind->result)
    {
#ifdef SIM_3G_USIM_SUPPORT
        is_pin_ok_flag = FALSE;
#endif
        //SCI_TRACE_LOW:"SIM::Verify CHV1 failed in SIM initialization!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2810_112_2_17_22_59_57_4630, (uint8 *)"");
        goto INITIALIZATION_END;
    }
    //save the pin for sim card restore
    else
    {
#ifdef SIM_3G_USIM_SUPPORT
        is_pin_ok_flag = TRUE;
#endif
        s_sim_chv1[cur_sim_task] = psig->chv1;
        s_sim_unblock_chv1[cur_sim_task] = psig->unblock_chv1;
    }

#ifdef GSM_DPHONE_VER2_SUPPORT

    if(!s_sim_is_entire_service[cur_sim_task])
    {
        s_sim_is_entire_service[cur_sim_task] = TRUE;
        ind->result = SIM_INI_ERROR_IMSI_FULL_FF;
        SCI_TRACE_LOW("SIM%d:Dphone limited service return!", cur_sim_task);
        result = FALSE;
        goto INITIALIZATION_END;
    }

#endif

    ini_result = Init_GetSimFileTable(cur_sim_task, g_sim_card_setting[cur_sim_task].card_phase, &(ind->ini_info), FALSE);

#ifdef GSM_DPHONE_SUPPORT

    if(s_sim_gsm_dphone_support[cur_sim_task])
    {
        if(0 == s_dphone_reset_times[cur_sim_task])
        {
            s_sim_chv1[cur_sim_task] = psig->chv1;
            s_sim_unblock_chv1[cur_sim_task] = psig->unblock_chv1;
            s_sim_use_unblock[cur_sim_task] = psig->use_unblock;
            s_sim_command_ref[cur_sim_task] = psig->command_ref;
            s_sim_sender[cur_sim_task] = ((xSignalHeaderRec *)psig)->Sender;

            s_sim_dphone_refresh_timer[cur_sim_task] = SCI_CreateTimer("SIM Dphone refresh Timer",
                                                                       DphoneRefreshTimeOut, cur_sim_task, DPHONE_REFRESH_TIME, SCI_NO_ACTIVATE);

            SCI_ChangeTimer(s_sim_dphone_refresh_timer[cur_sim_task], DphoneRefreshTimeOut, DPHONE_REFRESH_TIME);
            SCI_ActiveTimer(s_sim_dphone_refresh_timer[cur_sim_task]);

            s_dphone_reset_times[cur_sim_task]++ ;
            return FALSE;
        }
        else
        {
        	if(1 == s_dphone_reset_times[cur_sim_task])
        	{
        		s_dphone_reset_times[cur_sim_task]++;
        	}

            if(SCI_IsTimerActive(s_sim_dphone_refresh_timer[cur_sim_task]))
            {
                SCI_DeactiveTimer(s_sim_dphone_refresh_timer[cur_sim_task]);
            }
        }
    }

#endif

    if(SIM_INI_OK == ini_result)
    {
        //Get the CPHS optional feature
#ifdef SIM_3G_USIM_SUPPORT
#ifdef SIM_3G_USIM_SUPPORT_CPHS
        if (1)
#else
        if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
#endif
        {
            ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CPHSINFO, &(g_sim_cphsinfo[cur_sim_task].cphsoptionalinfo));

            if(SIM_SERVICE_OK == ser_result)
            {
                g_sim_cphsinfo[cur_sim_task].cphs_optional_flag = TRUE;
            }
            else
            {
                g_sim_cphsinfo[cur_sim_task].cphs_optional_flag = FALSE;
            }
        }
#ifdef SIM_3G_USIM_SUPPORT
        else
        {
            g_sim_cphsinfo[cur_sim_task].cphs_optional_flag = FALSE;
        }
#endif

        //SCI_TRACE_LOW:"SIM::SIM initialization success!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2876_112_2_17_22_59_57_4631, (uint8 *)"");
        ind->result = SIM_INI_OK;
        result = TRUE;

        //if the imsi is full 0xff
#ifdef GSM_CUSTOMER_AFP_SUPPORT

        if(((memcmp(imsi_is_ff_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0)
            || (memcmp(imsi_is_00_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0))
            && (!(ind->ini_info.imsi_mapping.imsi.valid_flag)))
        {
            //SCI_TRACE_LOW:"SIM::ss imsi is full 0xff during the initialization. len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2886_112_2_17_22_59_57_4632, (uint8 *)"d", ind->ini_info.imsi_mapping.imsi.length);
            ind->result = SIM_INI_OK;
            result = TRUE;
        }

#else

        if((memcmp(imsi_is_ff_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0) &&
            (ind->ini_info.imsi_mapping.imsi.length == 0xff) &&
            (!(ind->ini_info.imsi_mapping.imsi.valid_flag)))
        {
            //SCI_TRACE_LOW:"SIM::imsi is full 0xff during the initialization."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2895_112_2_17_22_59_57_4633, (uint8 *)"");
            ind->result = SIM_INI_ERROR_IMSI_FULL_FF;
            result = FALSE;
        }

#endif
    }

#if defined (_SUPPORT_IPC_) || defined (GSM_CUSTOMER_AFP_SUPPORT)
    else
    {
        if(SIM_INI_ERROR_CARD_OUT == ini_result)
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
        }
        else if(ini_result == SIM_INI_ERROR_READ_IMSI )
        {
            ind->result == ini_result;
        }
        else
        {
            ind->result = SIM_INI_ERROR_UNKNOWN;
        }

        result = FALSE;

        //To check the Int queue, to find out the "PLUG IN"
#ifdef SIM_PLUG_IN_SUPPORT

        if(TB_SIMINT_PLUG_IN == SIMINSTR_LatestHotSwapMessageInTempIntSignalQueue(cur_sim_task))
        {
            is_plug_in_flag[cur_sim_task] = TRUE;
        }

#endif

        PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);

#ifdef SIM_PLUG_IN_SUPPORT

        if(is_plug_in_flag[cur_sim_task])
        {
            SCI_CREATE_SIGNAL(pintind, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[cur_sim_task]);
            ((SIM_SIG_INT_IND_T *)pintind)->int_type = TB_SIMINT_PLUG_IN;
            SIMINSTR_AddThisSIGNALInTempIntSignalQueue(cur_sim_task, pintind);
        }

#endif
    }

#else
    else if(SIM_INI_ERROR_READ_IMSI == ini_result)
    {
        ind->result = SIM_INI_ERROR_READ_IMSI;
        result = TRUE;//EFimsi has problem, but I think we still can enter Active_State...
    }
    else
    {
        ind->result = ini_result;
        result = TRUE;//EFimsi has problem, but I think we still can enter Active_State...
    }
#endif

INITIALIZATION_END:
    //send confirm signal to the signal sender
#ifdef _SUPPORT_IPC_
#else
    SCI_SEND_SIGNAL((xSignalHeaderRec *)ind, ((xSignalHeaderRec *)psig)->Sender);
#endif

#ifdef SIM_3G_USIM_SUPPORT//I think we should check PBR here. wuding.
    if(is_pin_ok_flag
        && ((SIM_INI_ERROR_UNKNOWN != ind->result) && (SIM_INI_ERROR_CARD_OUT != ind->result)))
    {
        SIMDN_GetPBRInfo(cur_sim_task);
    }
#endif

#ifdef GSM_DPHONE_SUPPORT

    if(s_sim_gsm_dphone_support[cur_sim_task])
    {
        if((s_dphone_reset_times[cur_sim_task] != 0) && (s_dphone_refresh_type[cur_sim_task] != SIMAT_SIG_RF_RESET))
        {
            SIM_DphoneRefreshReq(cur_sim_task);
        }
    }

#endif

#ifdef _SUPPORT_IPC_
    ind->ini_info.is_fdn_enabled = SIMDN_FDNCapabilityCheck(cur_sim_task);
    SCI_SEND_SIGNAL((xSignalHeaderRec *)ind, ((xSignalHeaderRec *)psig)->Sender);
#endif

    return result;
}

/**********************************************************************
//    Description:
//    Get the AOC Access condition(for example:need the CHV1 or CHV2)
//    Global resource dependence :
//    Author:king.li
//    Note:
***********************************************************************/
STATIC BOOLEAN SIM_GetAOCAccessCondition(uint32 cur_sim_task, SIM_FILE_NAME_E filename)
{
    SIM_RESPONSE_DATA_OF_SELECT_T resp;
    BOOLEAN CHV2_or_CHV1 = FALSE;
    SIM_SERVICE_RESULT_E result;

    result = SIMSER_SelectFile(cur_sim_task, filename, &resp);

    // 04/24/2008 delete the trace
    //       SCI_TRACE_LOW("the access_condition1 byte 9th is %x", resp.access_condition1&0xff);
    //       SCI_TRACE_LOW("the access_condition1 byte 10h is %x", (resp.access_condition1&0xff00)>>8);

    if(1 == ((resp.access_condition1) & 0x0f00) >> 8)
    {
        CHV2_or_CHV1 = FALSE;
    }
    else if(2 == ((resp.access_condition1) & 0x0f00) >> 8)
    {
        CHV2_or_CHV1 = TRUE;
    }
    else
    {
        CHV2_or_CHV1 = FALSE;
    }

    //SCI_TRACE_LOW:"SIM::GetAOCAccessCondition filename = %d, condition = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_2959_112_2_17_22_59_58_4634, (uint8 *)"dd", filename, resp.access_condition1);
    return CHV2_or_CHV1;
}
/**********************************************************************
//    Description:
//      read information from SIM card that will send to the information task
//      in Card_Present indication
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
LOCAL BOOLEAN SIM_GetCardPresentInfo(    //return value:get the card ind success or fail
    uint32 cur_sim_task,
    SIM_CARD_PRESENT_INFO *ind //out:the card present indication
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RECORD_EF_INFO_T ef_sms_info;
#ifdef SIM_3G_USIM_SUPPORT
    SIM_RESPONSE_DATA_OF_SELECT_T ecc_select_response;
    uint8 ecc_record_num = 0;
    uint8 i = 0;
#endif
#ifdef GSM_CUSTOMER_AFP_SUPPORT
    SIM_RESPONSE_DATA_OF_SELECT_T temp;
#endif

    //Notify the upper layer sim or usim.
#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        ind->is_usim = TRUE;
    }
    else
#endif
    {
        ind->is_usim = FALSE;
    }

    //get the chvs status from gsm specific data
    ind->chv1_enable = g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled;
    //the chv1 status
    SCI_MEMCPY(&(ind->chv1_status), &(g_sim_card_setting[cur_sim_task].gsm_data.chv1_status), sizeof(SIM_CHV_STATUS_T));
    //the chv2 status
    SCI_MEMCPY(&(ind->chv2_status), &(g_sim_card_setting[cur_sim_task].gsm_data.chv2_status), sizeof(SIM_CHV_STATUS_T));
    //the unblock chv1 status
    SCI_MEMCPY(&(ind->unblock_chv1_status), &(g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status), sizeof(SIM_CHV_STATUS_T));
    //the unblock chv2 status
    SCI_MEMCPY(&(ind->unblock_chv2_status), &(g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status), sizeof(SIM_CHV_STATUS_T));

#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_PS_SIM_SAVE_POWER)
	//No EFelp
#else
    //read the EFelp
    ser_result = SIMSIGNAL_GetSIMLanguageInfo(cur_sim_task, &(ind->language_mapping));
#ifndef SIM_TEST_IN_WIN32

    if(IsFatalError(ser_result))
    {
        return FALSE;
    }
#endif
#endif

    //read the EFecc
#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ECC, &(ind->ecc_mapping));
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        // 3G Ecc is  Linear Fixed
        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_USIM_ECC, &ecc_select_response);

        if(SIM_SERVICE_OK == ser_result)
        {
            if(0 != ecc_select_response.dfnumber_recordlen)
            {
                ecc_record_num = (uint8)(ecc_select_response.freespace_filesize / ecc_select_response.dfnumber_recordlen);

                SCI_TRACE_LOW("SIM%d:usim ecc record num is %d",cur_sim_task,ecc_record_num);
                if(ecc_record_num > SIM_MAX_ECC_NUM)
                {
                    ecc_record_num = SIM_MAX_ECC_NUM;
                }

                s_sim_usim_ecc_alpha_id_len[cur_sim_task] = ecc_select_response.dfnumber_recordlen - 4;//

                ind->ecc_mapping.validate_num = 0;

                for(i = 0; i < ecc_record_num; i++)
                {
                    ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_USIM_ECC, (i + 1), SIM_RECORD_ABSOLUTE, &(ind->ecc_mapping.call_codes[i]));

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        if((0xFF == ind->ecc_mapping.call_codes[i].ecc_code[0])
                            && (0xFF == ind->ecc_mapping.call_codes[i].ecc_code[1])
                            && (0xFF == ind->ecc_mapping.call_codes[i].ecc_code[2]))
                        {
                            break;//quit the loop ?
                        }
                        else
                        {
                            SCI_TRACE_LOW("SIM%d:usim ecc record %d,number is 0x%2x,0x%2x,0x%2x",cur_sim_task,i,ind->ecc_mapping.call_codes[i].ecc_code[0],ind->ecc_mapping.call_codes[i].ecc_code[1],ind->ecc_mapping.call_codes[i].ecc_code[2]);
                            ind->ecc_mapping.validate_num++;
                        }
                    }
                    else
                    {
                        break;//quit the loop
                    }
                }
            }
        }
    }

#endif

#ifdef SIM_TEST_IN_WIN32
    ind->ecc_exist = (BOOLEAN)(SIM_SERVICE_OK == ser_result);
#else

    if(IsFatalError(ser_result))
    {
        ind->ecc_exist = FALSE;
        // 04/24/2008 delete the trace
        //      SCI_TRACE_LOW("SIM::Read EFecc failed for fatal error in Card Presented!");
        return FALSE;
    }
    else if(SIM_SERVICE_OK != ser_result)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::Read EFecc failed in Card Presented! %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_3078_112_2_17_22_59_58_4635, (uint8 *)"d", ser_result);
#endif
        ind->ecc_exist = FALSE;
    }
    else
    {
        ind->ecc_exist = TRUE;
    }

#endif

    ind->phase_mapping.phase = SIM_PH_2_PD;//SIM_PH_2 as default. Phase_1 may be too old.
#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        //read the EFphase
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PHASE, &(ind->phase_mapping));

        if(IsFatalError(ser_result))
        {
            // 04/24/2008 delete the trace
            //      SCI_TRACE_LOW("SIM::Read EFphase failed for fatal error in Card Presented!");
            return FALSE;
        }
        else if(ser_result != SIM_SERVICE_OK)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::Read EFphase failed in Card Presented!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_3104_112_2_17_22_59_58_4636, (uint8 *)"");
#endif
            //return FALSE;
        }
    }

    //get the sim card phase
    g_sim_card_setting[cur_sim_task].card_phase = ind->phase_mapping.phase;

    //read the EFad
    ind->ad_mapping.is_ad_exist = FALSE;
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_AD, &(ind->ad_mapping));

    if(IsFatalError(ser_result))
    {
        return FALSE;
    }
    else if(ser_result != SIM_SERVICE_OK)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::Read EFad failed in Card Presented!%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_3123_112_2_17_22_59_58_4637, (uint8 *)"d", ser_result);
#endif
    }
    else
    {
        ind->ad_mapping.is_ad_exist = TRUE;
        // save the mnc length info
        SaveMncLenInfo(cur_sim_task, &(ind->ad_mapping));
    }

#ifdef GSM_CUSTOMER_AFP_SUPPORT
    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_CDMA, &temp);

    if(SIM_SERVICE_OK == ser_result)
    {
        s_sim_cdma_flag[cur_sim_task] = TRUE;
    }
    else
    {
        if(IsFatalError(ser_result))
        {
            return FALSE;
        }
    }

#else
#ifdef SIM_CDMA_CARD_SUPPORT
    {
        SIM_RESPONSE_DATA_OF_SELECT_T temp;
        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_CDMA, &temp);

        if(SIM_SERVICE_OK == ser_result)
        {
            s_sim_cdma_flag[cur_sim_task] = TRUE;
        }
        else
        {
            if(IsFatalError(ser_result))
            {
                return FALSE;
            }
        }
    }
#endif
#endif

    SCI_MEMSET(&ef_sms_info, 0, sizeof(SIM_RECORD_EF_INFO_T));
    ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_SMS, &ef_sms_info);

    if(IsFatalError(ser_result))
    {
        return FALSE;
    }
    else
    {
#ifdef SIM_3G_USIM_SUPPORT

        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            if(ser_result == SIM_SERVICE_SELECT_NO_SUCH_FILE)
            {
                //in DFtelecom, there is no SMS.
                SIMSMS_SetSMSLocationDFtel(cur_sim_task, FALSE);
                //READ again, this time will remap to ADFusim SMS
                ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_SMS, &ef_sms_info);

                if(IsFatalError(ser_result))
                {
                    return FALSE;
                }
            }
            else
            {
                //SMS is in DFtelecom
                SIMSMS_SetSMSLocationDFtel(cur_sim_task, TRUE);
            }
        }

#endif
        ind->sms_num = ef_sms_info.record_num;
    }

#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_PS_SIM_SAVE_POWER)
	//No CPHS
#else
#ifdef _CPHS_SUPPORT
#ifdef SIM_3G_USIM_SUPPORT
    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        SIM_RESPONSE_DATA_OF_SELECT_T temp_orange;
        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_ORANGE, &temp_orange);

        if(SIM_SERVICE_OK == ser_result)
        {
            s_sim_df_orange_exist[cur_sim_task] = TRUE;
        }
        else
        {
            s_sim_df_orange_exist[cur_sim_task] = FALSE;

            if(IsFatalError(ser_result))
            {
                return FALSE;
            }
        }
    }
#endif
#endif

    //Read EF_ICCID for later user
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ICCID, &(ind->iccid_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        //SCI_TRACE_LOW:"SIM::Read EFiccid failed in SIM initialization!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_3197_112_2_17_22_59_58_4638, (uint8 *)"");

        if(IsFatalError(ser_result))
        {
            return FALSE;
        }
    }
#ifdef GSM_DPHONE_SUPPORT
    else
    {
        SCI_MEMCPY(&s_sim_gsm_dphone_iccid[cur_sim_task],&(ind->iccid_mapping),sizeof(SIM_EF_MAPPING_ICCID_T));
    }
#endif

    return TRUE;
}

/**********************************************************************
//    Description:
//      read information from SIM card that will send to the information task
//      in Card_Present indication
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_PrepareCardPresentInd(    //return value:get the card ind success or fail
    uint32 cur_sim_task,
    SIM_SIG_CARD_PRESENT_IND_T *ind //out:the card present indication
)
{
    return SIM_GetCardPresentInfo(cur_sim_task, &ind->present_info);
}
/*****************************************************************************/
//  Description: save the information of "length of MNC in the IMSI"
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL void SaveMncLenInfo(
    uint32                cur_sim_task,
    SIM_EF_MAPPING_AD_T   *ad_ptr
)
{
    SIM_MNC_LEN_INFO_T  *mnc_info_ptr = &(g_sim_card_setting[cur_sim_task].mnc_len_info);

    mnc_info_ptr->mnc_len_exist = ad_ptr->mnc_len_exist;
    mnc_info_ptr->mnc_digit_num = ad_ptr->mnc_digit_num;
}

/**********************************************************************
//    Description:
//      the fucntion process the CHV procedure
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
LOCAL SIM_SERVICE_RESULT_E SIMSIGNAL_CHV_Procedure(uint32 cur_sim_task,
                                                   SIM_CHV_REQ_TYPE_E req_type,
                                                   SIM_CHV_INDICATION_E chv_indication,
                                                   SIM_CHV_T first_chv,
                                                   SIM_CHV_T second_chv)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T temp;

    //the chv req is a verify chv req
    if(SIM_VERIFY_CHV == req_type)
    {
        ser_result = SIMINSTR_VerifyCHV(cur_sim_task, chv_indication, first_chv);
    }
    //the chv req is a change chv req
    else if(SIM_CHANGE_CHV == req_type)
    {
        ser_result = SIMINSTR_ChangeCHV(cur_sim_task, chv_indication, first_chv, second_chv);
    }
    //the chv req is a disable chv req
    else if(SIM_DISABLE_CHV == req_type)
    {
        //before disable the chv,check the chv disable service
        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CHV1_DISABLE))
        {
            ser_result = SIMINSTR_DisableCHV(cur_sim_task, first_chv);
        }
        else
        {
            ser_result = SIM_SERVICE_ERROR_UNKNOW;
        }
    }
    //the chv req is a enable chv req
    else if(SIM_ENABLE_CHV == req_type)
    {
        ser_result = SIMINSTR_EnableCHV(cur_sim_task, first_chv);
    }
    //the chv req is a unblock chv req
    else if(SIM_UNBLOCK_CHV == req_type)
    {
        ser_result = SIMINSTR_UnblockCHV(cur_sim_task, chv_indication, first_chv, second_chv);
    }
    //wrong req type
    else
    {
        ser_result = SIM_SERVICE_ERROR_UNKNOW;
    }

    if(SIM_SERVICE_OK == ser_result)
    {
        //Record the CHV1/2, UNBLOCK CHV1/2
        //the chv req is a verify chv req
        if(SIM_VERIFY_CHV == req_type)
        {
            if(SIM_CHV1 == chv_indication)
            {
                s_sim_chv1[cur_sim_task] = first_chv;
            }
        }
        //the chv req is a change chv req
        else if(SIM_CHANGE_CHV == req_type)
        {
            if(SIM_CHV1 == chv_indication)
            {
                s_sim_chv1[cur_sim_task] = second_chv;
            }
        }
        //the chv req is a disable chv req
        else if(SIM_DISABLE_CHV == req_type)
        {
            if(SIM_CHV1 == chv_indication)
            {
                s_sim_chv1[cur_sim_task] = first_chv;
            }
        }
        //the chv req is a enable chv req
        else if(SIM_ENABLE_CHV == req_type)
        {
            if(SIM_CHV1 == chv_indication)
            {
                s_sim_chv1[cur_sim_task] = first_chv;
            }
        }
        //the chv req is a unblock chv req
        else if(SIM_UNBLOCK_CHV == req_type)
        {
            if(SIM_CHV1 == chv_indication)
            {
                s_sim_chv1[cur_sim_task] = second_chv;
                s_sim_unblock_chv1[cur_sim_task] = first_chv;
            }
        }

        if(SIM_CHV1 == chv_indication)
        {
            g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_verified = TRUE;

            if(s_sim_get_chv1_original_max_times[cur_sim_task])
            {
                s_sim_get_chv1_original_max_times[cur_sim_task] = FALSE;
#ifdef SIM_3G_USIM_SUPPORT

                if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                {
                    ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, FALSE, SIM_CHV1, &s_sim_chv1_original_max_times[cur_sim_task]);

                    if(SIM_SERVICE_OK != ser_result)
                    {
                        s_sim_chv1_original_max_times[cur_sim_task] = SIM_MAX_CHV_ATTEMPT;//as a default
                    }
                }
                else
#endif
                {
                    s_sim_chv1_original_max_times[cur_sim_task] = SIM_MAX_CHV_ATTEMPT;//as a default
                    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &temp); //we shall SELECT DFgsm, but maybe currently we are at DFgsm.

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &temp);

                        if(SIM_SERVICE_OK == ser_result)
                        {
                            s_sim_chv1_original_max_times[cur_sim_task] = temp.chv1_status & SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
                        }
                    }
                }
            }

            g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = s_sim_chv1_original_max_times[cur_sim_task];
        }
        else//CHV2
        {
            g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_verified = TRUE;

            if(s_sim_get_chv2_original_max_times[cur_sim_task])
            {
                s_sim_get_chv2_original_max_times[cur_sim_task] = FALSE;
#ifdef SIM_3G_USIM_SUPPORT

                if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                {
                    ser_result = SIMINSTR_USIMGetCHVRemainTimes(cur_sim_task, FALSE, SIM_CHV2, &s_sim_chv2_original_max_times[cur_sim_task]);

                    if(SIM_SERVICE_OK != ser_result)
                    {
                        s_sim_chv2_original_max_times[cur_sim_task] = SIM_MAX_CHV_ATTEMPT;//as a default
                    }
                }
                else
#endif
                {
                    s_sim_chv2_original_max_times[cur_sim_task] = SIM_MAX_CHV_ATTEMPT;//as a default
                    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &temp); //we shall SELECT DFgsm, but maybe currently we are at DFgsm.

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &temp);

                        if(SIM_SERVICE_OK == ser_result)
                        {
                            s_sim_chv2_original_max_times[cur_sim_task] = temp.chv2_status & SIM_GSM_SPECIFIC_CS_RAMAIN_MARK;
                        }
                    }
                }
            }

            g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain = s_sim_chv2_original_max_times[cur_sim_task];
        }

        ser_result = SIM_SERVICE_OK;//if get remain times FAIL, we shall continue.
    }

    return ser_result;
}

/**********************************************************************
//    Description:
//      the fucntion process the CHV request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_CHVRequest(
    uint32 cur_sim_task,
    SIM_SIG_CHV_REQ_T *psig   //in:the request signal
)
{
#ifdef SIM_3G_USIM_SUPPORT
    SIM_RESPONSE_DATA_OF_SELECT_T temp;
#endif
    BOOLEAN is_need_verify_chv1 = FALSE;
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_SIG_CHV_CNF_T  *ind = NULL;

    //create the confirm signal
    SCI_CREATE_SIGNAL(ind, SIM_CHV_CNF, sizeof(SIM_SIG_CHV_CNF_T), g_sim_task_id[cur_sim_task]);
    //get the chv confirm information from request signal
    ind->chv_indication = psig->chv_indication;
    ind->command_ref = psig->command_ref;
    ind->req_type = psig->req_type;
    ind->result = SIM_CHV_REQ_OK;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        //wuding: 1, USIM cards can verify PIN2 in ADFusim...
        // 2, WATCH test USIM cards must change/disable PIN1 in ADFusim or MF
        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_ADF_USIM, &temp);
        //do not check the result
    }

#endif

    ser_result = SIMSIGNAL_CHV_Procedure(cur_sim_task, psig->req_type, psig->chv_indication, psig->first_chv, psig->second_chv);

    //the chv req is failed
    if(SIM_SERVICE_OK != ser_result)
    {
        //the chv req failed and the chv is blocked
        if(SIM_SERVICE_CHV_FAIL_WITH_BLOCKED == ser_result)
        {
            ind->result = SIM_CHV_ERROR_WITH_BLOCKED;
        }
        //the chv status is contradict to the chv req
        else if(SIM_SERVICE_CHV_CONTRADICT_STATUS == ser_result)
        {
            ind->result = SIM_CHV_ERROR_CONTRADICT_STATUS;
        }
        //the chv is not initialized
        else if(SIM_SERVICE_CHV_NOT_INITIALIZED == ser_result)
        {
            ind->result = SIM_CHV_ERROR_NOT_INITIALIZED;
        }
        //do not fulfill the access condition for the chv req
        else if(SIM_SERVICE_CHV_VER_FAIL == ser_result)
        {
            ind->result = SIM_CHV_ERROR_CHV_FAIL;
        }

        //chv req is failed for unknow reason
        else
        {
            ind->result = SIM_CHV_ERROR_UNKNOW;
        }
    }
    else
    {
        //the chv req is a change chv req
        if(SIM_CHANGE_CHV == psig->req_type)
        {
            if(SIM_CHV1 == psig->chv_indication)
            {
#ifdef SIM_3G_USIM_SUPPORT
                is_need_verify_chv1 = TRUE;//WATCH USIM cards
#endif
            }
        }
        //the chv req is a enable chv req
        else if(SIM_ENABLE_CHV == psig->req_type)
        {
            if(SIM_CHV1 == psig->chv_indication)
            {
#ifdef SIM_3G_USIM_SUPPORT
                is_need_verify_chv1 = TRUE;//WATCH USIM cards
#endif
            }
        }
        //the chv req is a unblock chv req
        else if(SIM_UNBLOCK_CHV == psig->req_type)
        {
            if(SIM_CHV1 == psig->chv_indication)
            {
                is_need_verify_chv1 = TRUE;//X-sim 6 in 1
            }
        }
    }

    //get the chv status
    switch(psig->req_type)
    {
        case SIM_VERIFY_CHV:
        case SIM_CHANGE_CHV:        //they do the seem operation

            if(SIM_CHV1 == psig->chv_indication)
            {
                ind->chv_status = g_sim_card_setting[cur_sim_task].gsm_data.chv1_status;
                ind->unblock_status = g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status;
            }
            else
            {
                ind->chv_status = g_sim_card_setting[cur_sim_task].gsm_data.chv2_status;
                ind->unblock_status = g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status;
            }

            break;
        case SIM_ENABLE_CHV:
        case SIM_DISABLE_CHV:   //they do the seem operation
            ind->chv_status = g_sim_card_setting[cur_sim_task].gsm_data.chv1_status;
            ind->unblock_status = g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status;
            break;
        case SIM_UNBLOCK_CHV:

            if(SIM_CHV1 == psig->chv_indication)
            {
                ind->chv_status = g_sim_card_setting[cur_sim_task].gsm_data.chv1_status;
                ind->unblock_status = g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status;
            }
            else
            {
                ind->chv_status = g_sim_card_setting[cur_sim_task].gsm_data.chv2_status;
                ind->unblock_status = g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status;
            }

            break;
        default:
            //SCI_TRACE_LOW:"SIM::SIMSIGNAL_CHVRequest,ERROR!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_3508_112_2_17_22_59_59_4639, (uint8 *)"");
            break;
    }

    ind->is_chv1_enabled = g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled;

    //send confirm signal to the signal sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)ind, ((xSignalHeaderRec *)psig)->Sender);

    if(is_need_verify_chv1)
    {
        //some SIM/USIM cards must do this.
        ser_result = SIMINSTR_VerifyCHV(cur_sim_task, SIM_CHV1, s_sim_chv1[cur_sim_task]);
    }
}


/**********************************************************************
//    Description:
//      verify the chv1 when the sim card is in intialization.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void VerifyCHV1AtInit(
    uint32 cur_sim_task,
    SIM_CHV_T chv1, //the CHV1 chv
    SIM_CHV_T unblock_chv1, //the Unblock CHV1 chv
    SIM_RESULT_INITIALIZE_SIM_E *result //the verify result
)
{
    SIM_SERVICE_RESULT_E ser_result;

    if(NULL == result)
    {
        //SCI_TRACE_LOW:"SIM::VerifyCHV1AtInit,ERROR."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_3541_112_2_17_22_59_59_4640, (uint8 *)"");
        return;
    }

    *result = SIM_INI_OK;
    //the chv is enable

    //the unblock chv1 also been blocked
    //note: some sim cards' unblock_pin1 is blocked, but the pin1 is still enabled and remain 3 times.
    //NOKIA can enter the pin1 and continue.
    if(g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.chv_blocked
        && g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked)
    {
        *result = SIM_INI_ERROR_UNBLOCK_CHV1_BLOCKED;
        return;
    }

    //the chv1 has been blocked
    if(g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked)
    {
        if((0xFF == unblock_chv1.secret_code[0])
            && (0xFF == unblock_chv1.secret_code[1])
            && (0xFF == unblock_chv1.secret_code[2])
            && (0xFF == unblock_chv1.secret_code[3])
            && (0xFF == unblock_chv1.secret_code[4])
            && (0xFF == unblock_chv1.secret_code[5])
            && (0xFF == unblock_chv1.secret_code[6])
            && (0xFF == unblock_chv1.secret_code[7]))
        {
            //PUK1 is enabled, but the input puk1 is all 0xff.
            *result = SIM_INI_ERROR_CHV_NOT_INPUT;
            return;
        }

        //unblock the chv1
        ser_result = SIMSIGNAL_CHV_Procedure(cur_sim_task, SIM_UNBLOCK_CHV, SIM_CHV1, unblock_chv1, chv1);

        //unblock chv1 fail
        if(SIM_SERVICE_OK != ser_result)
        {
            //the unblock chv1 is blocked for unblock chv1 fail
            if(SIM_SERVICE_CHV_FAIL_WITH_BLOCKED == ser_result)
            {
                *result = SIM_INI_ERROR_UNBLOCK_CHV1_BLOCKED;
                return;
            }
            else if(SIM_SERVICE_CHV_VER_FAIL == ser_result)
            {
                *result = SIM_INI_ERROR_UNBLOCK_CHV1;
                return;
            }
            //unblock chv1 fail
            else
            {
                *result = SIM_INI_ERROR_UNBLOCK_CHV1;
                return;
            }
        }
    }
    else if(g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled)
    {
        if((0xFF == chv1.secret_code[0])
            && (0xFF == chv1.secret_code[1])
            && (0xFF == chv1.secret_code[2])
            && (0xFF == chv1.secret_code[3])
            && (0xFF == chv1.secret_code[4])
            && (0xFF == chv1.secret_code[5])
            && (0xFF == chv1.secret_code[6])
            && (0xFF == chv1.secret_code[7]))
        {
            //CHV1 is enabled, but the input chv1 is all 0xff.
            *result = SIM_INI_ERROR_CHV_NOT_INPUT;
            return;
        }

        //verify chv1
        ser_result = SIMSIGNAL_CHV_Procedure(cur_sim_task, SIM_VERIFY_CHV, SIM_CHV1, chv1, unblock_chv1);

        //verify chv1 fail
        if(SIM_SERVICE_OK != ser_result)
        {
            //the chv1 is blocked for verify chv1 fail
            if(SIM_SERVICE_CHV_FAIL_WITH_BLOCKED == ser_result)
            {
                *result = SIM_INI_ERROR_CHV1_AND_BLOCKED;
                return;
            }
            else if(SIM_SERVICE_CHV_VER_FAIL == ser_result)
            {
                *result = SIM_INI_ERROR_CHV1;
                return;
            }
            else
            {
                *result = SIM_INI_ERROR_CHV1;
                return;
            }
        }
    }

    return;
}

/*****************************************************************************/
//  Description: read file EF_IMSI
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL SIM_RESULT_INITIALIZE_SIM_E Init_ReadImsiFile(
    const uint32 cur_sim_task,             //IN:
    SIM_EF_MAPPING_IMSI_T   *imsi_mapping_ptr   //OUT:
)
{
    SIM_SERVICE_RESULT_E            ser_result = SIM_SERVICE_OK;
    SIM_RESPONSE_DATA_OF_SELECT_T   select_data;
    BOOLEAN is_valid_flag = TRUE;

    select_data.file_status = 0;//clear it
    //get information of EF_IMSI
    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_IMSI, &select_data);

    if(ser_result != SIM_SERVICE_OK)
    {
        if(IsFatalError(ser_result))
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
        else
        {
#ifdef GSM_CUSTOMER_AFP_SUPPORT
            if (SIM_SERVICE_INVALIDATE_EF_ERROR == ser_result)
            {
            }
            else
#endif
            {
                return (SIM_INI_ERROR_READ_IMSI);
            }
        }
    }

    //the file is validate,read the EF_IMSI directly else rehabilitate it.
#ifdef GSM_CUSTOMER_AFP_SUPPORT
    if ((!(select_data.file_status & SIM_FILE_INVALIDATE_MARK)) 
        || (SIM_SERVICE_INVALIDATE_EF_ERROR == ser_result))
    {
        is_valid_flag = FALSE;
    }
#else
    if (select_data.file_status & SIM_FILE_INVALIDATE_MARK)
    {
        is_valid_flag = TRUE;
    }
    else
    {
        is_valid_flag = FALSE;
    }
#endif

    //the file is validate,read the EF_IMSI directly
    if (is_valid_flag)//if(select_data.file_status & SIM_FILE_INVALIDATE_MARK)
    {
        //read EF_IMSI
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_IMSI, imsi_mapping_ptr);

        if(SIM_SERVICE_OK != ser_result)
        {
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                return (SIM_INI_ERROR_READ_IMSI);
            }
        }
    }
    //the file is invalidate,first rehabilitate the EF_IMSI,the read the EF_IMSI
    else
    {
        ser_result = SIMSER_RehabilitateEF(cur_sim_task, SIM_FILE_EF_IMSI);

        if(SIM_SERVICE_OK != ser_result)
        {
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                return (SIM_INI_ERROR_READ_IMSI);
            }
        }

        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_IMSI, imsi_mapping_ptr);

        if(SIM_SERVICE_OK != ser_result)
        {
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                return (SIM_INI_ERROR_READ_IMSI);
            }
        }
    }

    return (SIM_INI_OK);
}

/*****************************************************************************/
//  Description: read file EF_LOCI
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL SIM_RESULT_INITIALIZE_SIM_E Init_ReadLociFile(
    const uint32 cur_sim_task,             //IN:
    SIM_EF_MAPPING_LOCI_T   *loci_mapping_ptr   //OUT:
)
{
    SIM_SERVICE_RESULT_E            ser_result = SIM_SERVICE_OK;
    SIM_RESPONSE_DATA_OF_SELECT_T   select_data;

    //get file information of EF_LOCI
    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_LOCI, &select_data);

    if(ser_result != SIM_SERVICE_OK)
    {
        if(IsFatalError(ser_result))
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
        else
        {
            return (SIM_INI_ERROR_READ_LOCI);
        }
    }

    //the file is validate,read the file EF_LOCI directly
    if(select_data.file_status & SIM_FILE_INVALIDATE_MARK)
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LOCI, loci_mapping_ptr);

        //if the result of read the loci is 98 04,then ignore the error and go on
        if(SIM_SERVICE_ACCESS_NEVER == ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::read loci failed when init."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_3689_112_2_17_22_59_59_4641, (uint8 *)"");
#endif
#ifndef WIN32
            SIM_PrintTxBuf(cur_sim_task);
            SIM_PrintRxBuf(cur_sim_task);
#endif
            loci_mapping_ptr->tmsi.valid_flag = FALSE;
            loci_mapping_ptr->update_status = MM_NOT_UPDATED;
        }
        else if(SIM_SERVICE_OK != ser_result)
        {
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                return (SIM_INI_ERROR_READ_LOCI);
            }
        }
    }
    //the file is invalidate first rehabilitate the EF_LOCI and then read the EF_LOCI
    else
    {
        ser_result = SIMSER_RehabilitateEF(cur_sim_task, SIM_FILE_EF_LOCI);

        if(SIM_SERVICE_OK != ser_result)
        {
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                return (SIM_INI_ERROR_READ_LOCI);
            }
        }

        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LOCI, loci_mapping_ptr);

        if(SIM_SERVICE_OK != ser_result)
        {
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                return (SIM_INI_ERROR_READ_LOCI);
            }
        }
    }

    return (SIM_INI_OK);
}

#ifndef SIM_CODE_COMPACT
/**********************************************************************
//    Description:
//      the Group identifier leve 1 request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadGID1(
    uint32 cur_sim_task,
    SIM_SIG_READ_GID_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_READ_GID_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    // 04/24/2008 delete the trace
    //     SCI_TRACE_LOW("SIM::read gid1");

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_GID1_CNF, sizeof(SIM_SIG_READ_GID_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;
    pcnf->is_service_support = FALSE;

    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GID1))
    {
        pcnf->is_service_support = TRUE;
        //read the GID from the EFgid1
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_GID1, &(pcnf->gid_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::gid1 not support."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_3758_112_2_17_23_0_0_4642, (uint8 *)"");
#endif
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Group identifier leve 2 request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadGID2(
    uint32 cur_sim_task,
    SIM_SIG_READ_GID_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_READ_GID_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    // 04/24/2008 delete the trace
    //     SCI_TRACE_LOW("SIM::read gid2");

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_GID2_CNF, sizeof(SIM_SIG_READ_GID_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;
    pcnf->is_service_support = FALSE;

    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GID2))
    {
        pcnf->is_service_support = TRUE;
        //read the GID from the EFgid2
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_GID2, &(pcnf->gid_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::gid2 not support."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_3803_112_2_17_23_0_0_4643, (uint8 *)"");
#endif
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

#ifdef SIM_AOC_SUPPORT
/**********************************************************************
//    Description:
//      the Accumulate call meter intialize service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_InitACM(
    uint32 cur_sim_task,
    SIM_SIG_AOC_INIT_ACM_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_AOC_INIT_ACM_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;
    SIM_EF_MAPPING_ACM_T    acm_mapping;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_AOC_INIT_ACM_CNF, sizeof(SIM_SIG_AOC_INIT_ACM_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
    {
        pcnf->is_service_support = TRUE;

        acm_mapping.acm_value = 0;
        //update the first request record of the EFacm to 0;
#ifdef _CPHS_SUPPORT

        if(SIM_BEARER_CAPABILITY_LINE_1 == psig->selected_line)
#endif
        {
#ifdef SIM_TEST_IN_WIN32
            ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_ACM, 0, SIM_RECORD_ABSOLUTE, &acm_mapping, FALSE);
#else
            ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_ACM, 0, SIM_RECORD_PREVIOUS, &acm_mapping, FALSE);
#endif

            if(SIM_SERVICE_OK != ser_result)
            {
                pcnf->result = AnalysisUpdateFileFail(ser_result);
            }
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

#ifdef _CPHS_SUPPORT

    if(SIM_BEARER_CAPABILITY_LINE_2 == psig->selected_line)
    {
        if(s_sim_df_orange_exist[cur_sim_task])
        {
            SIM_EF_MAPPING_ACM_T acm_line2;
            acm_line2.acm_value = 0;
            ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ORANGE_ACM2, &acm_line2);

            if(SIM_SERVICE_OK == ser_result)
            {
                pcnf->result = SIM_UPDATE_OK;
            }
            else
            {
                pcnf->result = SIM_UPDATE_FAILED_UNKNOW;
            }
        }
    }

#endif

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Accumulate call meter increase service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_IncreaseACM(
    uint32 cur_sim_task,
    SIM_SIG_AOC_INCREASE_ACM_REQ_T *psig    //the service request signal
)
{
    SIM_SIG_AOC_INCREASE_ACM_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_AOC_INCREASE_ACM_CNF, sizeof(SIM_SIG_AOC_INCREASE_ACM_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_INCREASE_FILE_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
    {
        pcnf->is_service_support = TRUE;
#ifdef _CPHS_SUPPORT

        if(SIM_BEARER_CAPABILITY_LINE_1 == psig->selected_line)
#endif
        {
            //increase the ACM record
            ser_result = SIMSER_IncreaseEF(cur_sim_task, SIM_FILE_EF_ACM, psig->acm_data, &(pcnf->increase_result));

            if(SIM_SERVICE_OK != ser_result)
            {
                pcnf->result = AnalysisIncreaseFileFail(ser_result);
            }
            else
            {
                if((g_sim_acm_max[cur_sim_task].acm_max_value != 0)
                    && (pcnf->increase_result.new_value.data >= g_sim_acm_max[cur_sim_task].acm_max_value))
                {
                    pcnf->result = SIM_INCREASE_REACH_MAX_VLAUE;
                }

#ifdef _CPHS_SUPPORT
                else
                {
                    if(s_sim_csp_als_enable[cur_sim_task])
                    {
                        // 1, read ACM2
                        if(s_sim_df_orange_exist[cur_sim_task])
                        {
                            SIM_EF_MAPPING_ACM_T acm_line2;
                            ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ORANGE_ACM2, &acm_line2);

                            if(SIM_SERVICE_OK == ser_result)
                            {
                                // 2, ACM+ACM2 < ACMmax ?
                                if((g_sim_acm_max[cur_sim_task].acm_max_value != 0)
                                    && ((pcnf->increase_result.new_value.data + acm_line2.acm_value) >= g_sim_acm_max[cur_sim_task].acm_max_value))
                                {
                                    pcnf->result = SIM_INCREASE_REACH_MAX_VLAUE;
                                }
                            }
                        }
                    }
                }

#endif
            }
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

#ifdef _CPHS_SUPPORT

    if(SIM_BEARER_CAPABILITY_LINE_2 == psig->selected_line)
    {
        if(s_sim_df_orange_exist[cur_sim_task])
        {
            ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ORANGE_ACM2, &(psig->acm_data));

            if(SIM_SERVICE_OK == ser_result)
            {
                pcnf->result = SIM_INCREASE_FILE_OK;
                pcnf->increase_result.new_value.data = psig->acm_data.data;//L4 will save the "new_value"
                pcnf->increase_result.added_value.data = psig->acm_data.data;

                if((g_sim_acm_max[cur_sim_task].acm_max_value != 0)
                    && (pcnf->increase_result.new_value.data >= g_sim_acm_max[cur_sim_task].acm_max_value))
                {
                    pcnf->result = SIM_INCREASE_REACH_MAX_VLAUE;
                }
                else
                {
                    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
                    {
                        // 1, read ACM
                        if(s_sim_csp_als_enable[cur_sim_task])
                        {
                            SIM_EF_MAPPING_ACM_T acm;
                            ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_ACM, 1, SIM_RECORD_ABSOLUTE, &acm);

                            if(SIM_SERVICE_OK == ser_result)
                            {
                                // 2, ACM+ACM2 < ACMmax ?
                                if((g_sim_acm_max[cur_sim_task].acm_max_value != 0)
                                    && ((pcnf->increase_result.new_value.data + acm.acm_value) >= g_sim_acm_max[cur_sim_task].acm_max_value))
                                {
                                    pcnf->result = SIM_INCREASE_REACH_MAX_VLAUE;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                pcnf->result = SIM_INCREASE_UNKNOW_ERROR;
            }
        }
    }

#endif

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Accumulate call meter maximum value update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdateACMMAX(
    uint32 cur_sim_task,
    SIM_SIG_AOC_UPDATE_ACMMAX_REQ_T *psig  //the service request signal
)
{
    SIM_SIG_AOC_UPDATE_ACMMAX_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_AOC_UPDATE_ACMMAX_CNF, sizeof(SIM_SIG_AOC_UPDATE_ACMMAX_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;
    pcnf->acm_mapping.acm_max_value = psig->acm_mapping.acm_max_value;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
    {
        pcnf->is_service_support = TRUE;
        //using the acm amx info in the service rquest signal to update the EFacmmax
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ACMmax, &(psig->acm_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisUpdateFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    g_sim_acm_max[cur_sim_task].acm_max_value = psig->acm_mapping.acm_max_value;
    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Price per unit and currency talbe request service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadPUCT(
    uint32 cur_sim_task,
    SIM_SIG_AOC_READ_PUCT_REQ_T *psig   //the service request signal
)
{
    SIM_SIG_AOC_READ_PUCT_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_AOC_READ_PUCT_CNF, sizeof(SIM_SIG_AOC_READ_PUCT_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_READ_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
    {
        pcnf->is_service_support = TRUE;
        //read the puct info from the EFpuct
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PUCT, &(pcnf->puct_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the Price per unit and currency table update service supply fuction
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_UpdatePUCT(
    uint32 cur_sim_task,
    SIM_SIG_AOC_UPDATE_PUCT_REQ_T *psig //the service request signal
)
{
    SIM_SIG_AOC_UPDATE_PUCT_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_AOC_UPDATE_PUCT_CNF, sizeof(SIM_SIG_AOC_UPDATE_PUCT_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_UPDATE_OK;

    //check the service enabled or not
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_AOC))
    {
        pcnf->is_service_support = TRUE;
        //update the EFpuct with the puct info in the service request signal
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PUCT, &(psig->puct_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = AnalysisUpdateFileFail(ser_result);
            //confirm the request signal
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }

        //send the new PUCT value when the udpate operation is succeeded
        SCI_MEMCPY(&(pcnf->puct_mapping), &(psig->puct_mapping), sizeof(SIM_EF_MAPPING_PUCT_T));
    }
    else
    {
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

/**********************************************************************
//    Description:
//      the record EF(including EFadn,EFfdn,EFmsisdn,EFlnd,EFsmsp,EFsdn
//      and EFbdn) information request service supply function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_GetRecordEFInfo(
    uint32 cur_sim_task,
    SIM_SIG_GET_RECORD_EF_INFO_REQ_T *psig  //the service request signal
)
{
    SIM_FILE_NAME_E ext_file_name = SIM_FILE_EF_EXT1;
    SIM_SIG_GET_RECORD_EF_INFO_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E    ser_result = SIM_SERVICE_ERROR_UNKNOW;
    SIM_RECORD_EF_INFO_T   *pinfo;
    SIM_RECORD_EF_INFO_T tmp_info = {0};

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_GET_RECORD_EF_INFO_CNF, sizeof(SIM_SIG_GET_RECORD_EF_INFO_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->file_name = psig->file_name;
    pcnf->result = SIM_GET_INFO_OK;
    pcnf->dn_ext_num = 0;

#ifdef MODEM_PLATFORM
#if defined (_LENOVO_6500_) || defined (HUAWEI_G730) || defined (MODEM_PLATFORM_PS_SIM_SAVE_POWER)//No SIM Phonebook
    if (SIM_FILE_EF_ADN == psig->file_name)//This modem project donnot support LND
    {
        pcnf->is_service_support = FALSE;
        goto SEND_CONFIRM_SIGNAL;
    }
    if (SIM_FILE_EF_LND == psig->file_name)//This modem project donnot support LND
    {
        pcnf->is_service_support = FALSE;
        goto SEND_CONFIRM_SIGNAL;
    }
#ifdef MODEM_PLATFORM_PS_SIM_SAVE_POWER
	if (SIM_FILE_EF_MSISDN == psig->file_name)//This modem project donnot support MSISDN
    {
        pcnf->is_service_support = FALSE;
        goto SEND_CONFIRM_SIGNAL;
    }
	if (SIM_FILE_EF_SDN == psig->file_name)//This modem project donnot support MSISDN
    {
        pcnf->is_service_support = FALSE;
        goto SEND_CONFIRM_SIGNAL;
    }
#endif
#endif
    if (SIM_FILE_EF_IMG == psig->file_name)//There is no modem project support IMG until now
    {
        pcnf->is_service_support = FALSE;
        goto SEND_CONFIRM_SIGNAL;
    }
#endif

    switch(psig->file_name)
    {
        case SIM_FILE_EF_ADN:
#if defined (MODEM_PLATFORM) && defined (_SUPPORT_IPC_)
            pcnf->is_service_support = FALSE;//IPC donot need modem read ADN
            goto SEND_CONFIRM_SIGNAL;
#else
            pinfo = &(g_sim_card_setting[cur_sim_task].adn_info);

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_ADN))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }
#endif
            break;
        case SIM_FILE_EF_FDN:
            pinfo = &(g_sim_card_setting[cur_sim_task].fdn_info);

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_FDN))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }

            break;
        case SIM_FILE_EF_MSISDN:
            pinfo = &(g_sim_card_setting[cur_sim_task].msisdn_info);

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_MSISDN))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }

            break;
        case SIM_FILE_EF_LND:
#ifdef SIM_DROP_RESTORE_SUPPORT
#else
            pinfo = &(g_sim_card_setting[cur_sim_task].lnd_info);

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_LND))
            {
                pcnf->is_service_support = TRUE;
            }
            else
#endif
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }
            break;
        case SIM_FILE_EF_SMSP:
            pinfo = &(g_sim_card_setting[cur_sim_task].smsp_info);

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMSP))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }

            break;
        case SIM_FILE_EF_SDN:
            pinfo = &(g_sim_card_setting[cur_sim_task].sdn_info);

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SDN))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }

            break;
        case SIM_FILE_EF_BDN:
            pinfo = &(g_sim_card_setting[cur_sim_task].bdn_info);

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_BDN))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }

            break;
        case SIM_FILE_EF_IMG:
#ifdef SIM_DROP_RESTORE_SUPPORT
            pcnf->is_service_support = FALSE;
            goto SEND_CONFIRM_SIGNAL;
#else
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:get img record info."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4242_112_2_17_23_0_0_4644, (uint8 *)"");
#endif
            pinfo = &tmp_info;

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_IMAGE))
            {
                //SCI_TRACE_LOW:"SIM: img service support."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4247_112_2_17_23_0_0_4645, (uint8 *)"");
                pcnf->is_service_support = TRUE;
            }
            else
            {
#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM: img service not support."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4253_112_2_17_23_0_0_4646, (uint8 *)"");
#endif
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }
#endif
            break;

        case SIM_FILE_EF_PNN:
            pinfo = &tmp_info;

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_PNN))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }

            break;

        case SIM_FILE_EF_OPL:
            pinfo = &tmp_info;

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_OPL))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }

            break;

        case SIM_FILE_EF_USIM_PNNI:
            pinfo = &tmp_info;

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_PNN_ICON))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }

            break;

        case SIM_FILE_EF_INFONUM:
            pinfo = &tmp_info;

            if(SIM_IsCPHSOptionalSupport(cur_sim_task, SIM_FILE_EF_INFONUM))
            {
                pcnf->is_service_support = TRUE;
            }
            else
            {
                pcnf->is_service_support = FALSE;
                goto SEND_CONFIRM_SIGNAL;
            }

            break;

        default:
            pcnf->result = SIM_GET_INFO_INVALIDE_FILE;
            goto SEND_CONFIRM_SIGNAL;
    }

    //the record ef info still not beeing got
    if((UINT8_INVALIDE == pinfo->alpha_id_len)
        || (SIM_FILE_EF_IMG         == psig->file_name)
        || (SIM_FILE_EF_PNN         == psig->file_name)
        || (SIM_FILE_EF_OPL         == psig->file_name)
        || (SIM_FILE_EF_USIM_PNNI   == psig->file_name)
        || (SIM_FILE_EF_INFONUM     == psig->file_name)
      )
    {
#ifdef SIM_3G_USIM_SUPPORT

        if(SIM_FILE_EF_ADN == psig->file_name)
        {
            ser_result = SIMDN_GetUSIMADNRecordInfomation(cur_sim_task, pinfo);
        }
        else
        {
            ser_result = SIMSER_GetRecordInfo(cur_sim_task, psig->file_name, pinfo);
        }

#else
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, psig->file_name, pinfo);
#endif

        //analysis the fail reason
        if(SIM_SERVICE_OK != ser_result)
        {
            switch(ser_result)
            {
                case SIM_SERVICE_SELECT_FILE_ERROR:
                    pcnf->result = SIM_GET_INFO_SELECT_ERROR;
                    break;
                default:
                    pcnf->result = SIM_GET_INFO_UNKNOW_ERROR;
                    break;
            }

#ifdef SIM_3G_USIM_SUPPORT
            //we set the Lnd service true default when read EFust, and then check it when read Dftelecom/EFlnd, if get the record info ok, then set the service TRUE,
            //else set the service false //bug639449
            if((SIM_FILE_EF_LND == psig->file_name) && (USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]))
            {
                SCI_TRACE_LOW("SIM%d:ldn is not support for get lnd info failed.",cur_sim_task);
                g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_LND].allocated = FALSE;
                g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_LND].activated = FALSE;
                pcnf->is_service_support = FALSE;
            }
#endif            

            goto SEND_CONFIRM_SIGNAL;
        }
    }

    SCI_MEMCPY(&(pcnf->info), pinfo, sizeof(SIM_RECORD_EF_INFO_T));

    if(SIM_SERVICE_OK == ser_result)
    {
        //We should get the EXT file info, for the EXT file num may be smaller than DN num.
        //For example, some sim has 200 ADN records but only 7 EXT1 records.
        if((SIM_FILE_EF_MSISDN == psig->file_name)
            || (SIM_FILE_EF_ADN == psig->file_name)
            || (SIM_FILE_EF_LND == psig->file_name)
            || (SIM_FILE_EF_SDN == psig->file_name)
            || (SIM_FILE_EF_FDN == psig->file_name))
        {
            SIMDN_Init(cur_sim_task, psig->file_name, pcnf->info.record_num);

            if((SIM_FILE_EF_MSISDN == psig->file_name)
                || (SIM_FILE_EF_ADN == psig->file_name)
                || (SIM_FILE_EF_LND == psig->file_name))
            {
                ext_file_name = SIM_FILE_EF_EXT1;
            }
            else if(SIM_FILE_EF_FDN == psig->file_name)
            {
                ext_file_name = SIM_FILE_EF_EXT2;
            }
            else if(SIM_FILE_EF_SDN == psig->file_name)
            {
                ext_file_name = SIM_FILE_EF_EXT3;
            }

            if(s_sim_need_check_ext1_flag[cur_sim_task]
                && ((SIM_FILE_EF_MSISDN == psig->file_name) || (SIM_FILE_EF_ADN == psig->file_name) || (SIM_FILE_EF_LND == psig->file_name)))
            {
                //Here we assume we only support 255 EXT records at most. The DN(ADN) may
                //have a large number(<510 in usim), but one EXT file only have 255 records at most.
                if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EXT1))
                {
                    tmp_info.max_record_num = 255;//init
                    ser_result = SIMSER_GetRecordInfo(cur_sim_task, ext_file_name, &tmp_info);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        s_sim_need_check_ext1_flag[cur_sim_task] = FALSE;
                        s_sim_dn_ext1_num[cur_sim_task] = (uint8)tmp_info.record_num;
                    }
                }
            }
            else if(s_sim_need_check_ext2_flag[cur_sim_task] && (SIM_FILE_EF_FDN == psig->file_name))
            {
                if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EXT2))
                {
                    tmp_info.max_record_num = 255;//init
                    ser_result = SIMSER_GetRecordInfo(cur_sim_task, ext_file_name, &tmp_info);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        s_sim_need_check_ext2_flag[cur_sim_task] = FALSE;
                        s_sim_dn_ext2_num[cur_sim_task] = (uint8)tmp_info.record_num;
                    }
                }
            }
            else if(s_sim_need_check_ext3_flag[cur_sim_task] && (SIM_FILE_EF_SDN == psig->file_name))
            {
                if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EXT3))
                {
                    tmp_info.max_record_num = 255;//init
                    ser_result = SIMSER_GetRecordInfo(cur_sim_task, ext_file_name, &tmp_info);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        s_sim_need_check_ext3_flag[cur_sim_task] = FALSE;
                        s_sim_dn_ext3_num[cur_sim_task] = (uint8)tmp_info.record_num;
                    }
                }
            }
        }
    }

    //这个需要放在外面,因为有前面一些变量的限制
    if((SIM_FILE_EF_MSISDN == psig->file_name)
        || (SIM_FILE_EF_ADN == psig->file_name)
        || (SIM_FILE_EF_LND == psig->file_name))
    {
        pcnf->dn_ext_num = s_sim_dn_ext1_num[cur_sim_task];

        //!!! 我认为LND 的GetRecordInfo是重读sim pb的开始，这里reset ext1 信息
        if(SIM_FILE_EF_LND == psig->file_name)
        {
            SIMDN_ClearEXT1Info(cur_sim_task);
        }
    }
    else if(SIM_FILE_EF_FDN == psig->file_name)
    {
        pcnf->dn_ext_num = s_sim_dn_ext2_num[cur_sim_task];
        SIMDN_ClearEXT2Info(cur_sim_task);
    }

SEND_CONFIRM_SIGNAL:
    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      Polling the SIM card to check the proactive command exist or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_Polling(
    uint32 cur_sim_task,
    SIMAT_SIG_POLLING_IND_T *psig
)
{
    s_sim_stk_poll_time[cur_sim_task] = psig->poll_interval;

    if(s_sim_call_poll_time[cur_sim_task] == 0)

    {
        SIMMAIN_StartCardPresentTimer(cur_sim_task);
    }
}

/**********************************************************************
//    Description:
//      Polling the SIM card to check the proactive command exist or not
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_GetPC(
    uint32 cur_sim_task,
    SIM_SIG_PC_EXIST_IND_T *psig    //the proactive command indicator signal
)
{
    SIMAT_SIG_PC_IND_T *ppcsig = NULL;
#ifdef GSM_DPHONE_SUPPORT
#ifdef GSM_DPHONE_SUPPORT_SELF_TEST
    static uint8 ywd_self_test = 0;
#endif
	static uint32 enter_times[4] = {0};
#endif

    if(g_sim_card_state[cur_sim_task] < SIM_INITIALIZE_STATE)
    {
        //SCI_TRACE_LOW:"SIM::Receive SIM_PC_EXIST_IND signal when card not ready!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4486_112_2_17_23_0_1_4647, (uint8 *)"");
        return;
    }
    else if(SIM_PH_2_PD != g_sim_card_setting[cur_sim_task].card_phase)
    {
        //SCI_TRACE_LOW:"SIM::Receive SIM_PC_EXIST_IND signal when card not support STK!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4491_112_2_17_23_0_1_4648, (uint8 *)"");
        return;
    }

    SCI_CREATE_SIGNAL(ppcsig, SIMAT_PC_IND, sizeof(SIMAT_SIG_PC_IND_T), g_sim_task_id[cur_sim_task]);
    ppcsig->pc_data.data_len = psig->pc_data.data_len;
    SCI_ASSERT(psig->pc_data.data_len <= SIM_MAX_R_APDU_DATA_LEN);/*assert verified:Check_value */
    SCI_MEMCPY(ppcsig->pc_data.data_arr, psig->pc_data.data_arr, psig->pc_data.data_len);

#ifdef GSM_DPHONE_SUPPORT
	enter_times[cur_sim_task]++;
    if ((1 == enter_times[cur_sim_task]) 
		&& (0x23 == ppcsig->pc_data.data_arr[5]))
    {
    	s_sim_jiangsu_flag[cur_sim_task]++;
    }
	if ((2 == enter_times[cur_sim_task])
		&& (0x25 == ppcsig->pc_data.data_arr[6]))
	{
		s_sim_jiangsu_flag[cur_sim_task]++;
	}

	SCI_TRACE_LOW("SIM Dphone Jiangsu %d %d %d",enter_times[cur_sim_task],s_sim_jiangsu_flag[cur_sim_task],s_sim_gsm_dphone_support[cur_sim_task]);

#ifdef GSM_DPHONE_SUPPORT_SELF_TEST

    //This is the self-test code for the developer and debuger.
    if(0x21 == ppcsig->pc_data.data_arr[5])
    {
        //To simulate the SIM card send out the STK refresh.
        if(0 == ywd_self_test)
        {
            ywd_self_test = 1;
            SCI_TRACE_LOW("SIM YWD DPHONE SELFTEST");
            ppcsig->pc_data.data_len = 11;
            ppcsig->pc_data.data_arr[0] = 0xD0;
            ppcsig->pc_data.data_arr[1] = 0x9;
            ppcsig->pc_data.data_arr[2] = 0x81;
            ppcsig->pc_data.data_arr[3] = 0x03;
            ppcsig->pc_data.data_arr[4] = 0x01;
            ppcsig->pc_data.data_arr[5] = 0x01;//Refresh
            ppcsig->pc_data.data_arr[6] = 0x00;//ini and full file change
            ppcsig->pc_data.data_arr[7] = 0x82;
            ppcsig->pc_data.data_arr[8] = 0x02;
            ppcsig->pc_data.data_arr[9] = 0x81;
            ppcsig->pc_data.data_arr[10] = 0x82;
        }
    }
#endif
#endif

    SCI_SEND_SIGNAL((xSignalHeaderRec *)ppcsig, g_sim_simat_task_id[cur_sim_task]);
}

/**********************************************************************
//    Description:
//      Do the Terminal response opreation as service requested
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_TerminalResponse(
    uint32 cur_sim_task,
    SIMAT_SIG_TERMINAL_RESPONSE_REQ_T *psig //the service request signal
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    SIMAT_SIG_SESSION_TERMINATED_IND_T *pcnf = NULL;
    SIMAT_SIG_TERMINAL_RESPONSE_CNF_T *res_pcnf = NULL;
    uint8 sw1 = 0x6f, sw2 = 0;
#ifdef GSM_DPHONE_SUPPORT
    SIMAT_SIG_PC_IND_T *ppcsig = NULL;
#endif

    SCI_CREATE_SIGNAL(res_pcnf, SIMAT_TERMINAL_RESPONSE_CNF, sizeof(SIMAT_SIG_TERMINAL_RESPONSE_CNF_T), g_sim_task_id[cur_sim_task]);

    if(g_sim_card_state[cur_sim_task] < SIM_INITIALIZE_STATE)
    {
        //SCI_TRACE_LOW:"SIM::Receive terminal response request when card not ready!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4518_112_2_17_23_0_1_4649, (uint8 *)"");
#ifdef GSM_DPHONE_SUPPORT

        if(s_sim_gsm_dphone_support[cur_sim_task])
        {
            SCI_CREATE_SIGNAL(pcnf, SIMAT_SESSION_TERMINATED_IND, sizeof(SIMAT_SIG_SESSION_TERMINATED_IND_T), g_sim_task_id[cur_sim_task]);
            pcnf->dual_sys = cur_sim_task;
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, g_sim_simat_task_id[cur_sim_task]);
        }

#elif defined GSM_DPHONE_VER2_SUPPORT
        SCI_CREATE_SIGNAL(pcnf, SIMAT_SESSION_TERMINATED_IND, sizeof(SIMAT_SIG_SESSION_TERMINATED_IND_T), g_sim_task_id[cur_sim_task]);
        pcnf->dual_sys = cur_sim_task;
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, g_sim_simat_task_id[cur_sim_task]);
#endif
    }
    else if(SIM_PH_2_PD != g_sim_card_setting[cur_sim_task].card_phase)
    {
        //SCI_TRACE_LOW:"SIM::Receive terminal response request when card not support STK!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4527_112_2_17_23_0_1_4650, (uint8 *)"");
    }
    else
    {
#ifdef GSM_DPHONE_SUPPORT
        if (s_sim_gsm_dphone_support[cur_sim_task] 
            && s_sim_gsm_dphone_watch_no_refresh[cur_sim_task] 
            && ((0x01 == psig->response.data_arr[3]) && (0x00 == psig->response.data_arr[4])))
        {
            SCI_TRACE_LOW("SIM%d: Dphone Special Watch V4 SIM no TR for Fake Refresh",cur_sim_task);
            s_sim_gsm_dphone_watch_no_refresh[cur_sim_task] = FALSE;
            return;//Fake STK Refresh we build, donnot TR to SIM. 
        }
#endif

        ser_result = SIMINSTR_TerminalResponse(cur_sim_task, &(psig->response), &sw1, &sw2);

        if(SIM_SERVICE_OK != ser_result)
        {
            //SCI_TRACE_LOW:"SIM::Terminal response failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4535_112_2_17_23_0_1_4651, (uint8 *)"");
            SCI_CREATE_SIGNAL(pcnf, SIMAT_SESSION_TERMINATED_IND, sizeof(SIMAT_SIG_SESSION_TERMINATED_IND_T), g_sim_task_id[cur_sim_task]);
            pcnf->dual_sys = cur_sim_task;
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, g_sim_simat_task_id[cur_sim_task]);
        }
#ifdef GSM_DPHONE_SUPPORT
        else
        {
            if (s_sim_gsm_dphone_support[cur_sim_task])
            {
                if ((SIM_INITIALIZE_STATE == g_sim_card_state[cur_sim_task])  
					&& (((0x23 == psig->response.data_arr[3]) && (0x90 == sw1)) 
					|| ((0x25 == psig->response.data_arr[3]) && (2 == s_sim_jiangsu_flag[cur_sim_task]))))//GetInput TeminalResponse, Watch V4 SIM will response with 9000, which means OK.
                {
                    SCI_TRACE_LOW("SIM%d: Dphone Special SIM no Refresh. 0x%x 0x%x",cur_sim_task,\
                        s_sim_gsm_dphone_iccid[cur_sim_task].id_num[4],\
                        s_sim_gsm_dphone_iccid[cur_sim_task].id_num[6]);

                    //The customer told me how to judge the WATCH SIM.
                    //But finally, this code cannot work well because some WATCH card's iccid donnot like this.
                    //So this customer disabled this if (), but I keep it.
                    //If nesscerary you can disable this if ().
#if 0
                    /*Watch V4 SIM card*/
                    if (((0x71 == s_sim_gsm_dphone_iccid[cur_sim_task].id_num[4]) 
                        || (0x03 == s_sim_gsm_dphone_iccid[cur_sim_task].id_num[4]) 
                        || (0x50 == s_sim_gsm_dphone_iccid[cur_sim_task].id_num[4])) 
                        && (0x07 == (s_sim_gsm_dphone_iccid[cur_sim_task].id_num[6]& 0x0F)))
#endif                    
                    {
                        s_sim_gsm_dphone_watch_no_refresh[cur_sim_task] = TRUE;
                        SCI_CREATE_SIGNAL(ppcsig, SIM_PC_EXIST_IND, sizeof(SIM_SIG_PC_EXIST_IND_T), g_sim_task_id[cur_sim_task]);
                        ppcsig->pc_data.data_len = 11;
                        ppcsig->pc_data.data_arr[0] = 0xD0;
                        ppcsig->pc_data.data_arr[1] = 0x9;
                        ppcsig->pc_data.data_arr[2] = 0x81;
                        ppcsig->pc_data.data_arr[3] = 0x03;
                        ppcsig->pc_data.data_arr[4] = 0x01;
                        ppcsig->pc_data.data_arr[5] = 0x01;//Refresh
                        ppcsig->pc_data.data_arr[6] = 0x00;//ini and full file change
                        ppcsig->pc_data.data_arr[7] = 0x82;
                        ppcsig->pc_data.data_arr[8] = 0x02;
                        ppcsig->pc_data.data_arr[9] = 0x81;
                        ppcsig->pc_data.data_arr[10] = 0x82;
                        SCI_SEND_SIGNAL((xSignalHeaderRec *)ppcsig, g_sim_task_id[cur_sim_task]);
                    }
                }
            }
        }
#endif
    }

    res_pcnf->sw1 = sw1;
    res_pcnf->sw2 = sw2;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)res_pcnf, g_sim_simat_task_id[cur_sim_task]);
}

/**********************************************************************
//    Description:
//      Envelope the data from the SIM application toolkit to the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_Envelope(
    uint32 cur_sim_task,
    SIMAT_SIG_ENVELOPE_REQ_T    *psig  //the service request signal
)
{
    SIMAT_SIG_ENVELOPE_CNF_T *pcnf = NULL;

    SCI_CREATE_SIGNAL(pcnf, SIMAT_ENVELOPE_CNF, sizeof(SIMAT_SIG_ENVELOPE_CNF_T), g_sim_task_id[cur_sim_task]);

    if(g_sim_card_state[cur_sim_task] < SIM_INITIALIZE_STATE)
    {
        pcnf->result = SIMAT_EN_CARD_NOT_READY;
    }
    else if(SIM_PH_2_PD != g_sim_card_setting[cur_sim_task].card_phase)
    {
        pcnf->result = SIMAT_EN_CARD_NOT_SUPPORT;
    }
    else
    {
        if(SIMINSTR_Envelope(cur_sim_task, &(psig->envelope_data), &(pcnf->response), &(pcnf->result), psig->delay_fetch,&(pcnf->sim_sw1),&(pcnf->sim_sw2)) != SIM_SERVICE_OK)
        {
            //SCI_TRACE_LOW:"SIM::Envelope failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4570_112_2_17_23_0_1_4652, (uint8 *)"");
        }
    }

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      Notify the STK task that the current stk application session is
//    terminated
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_IndStkSessionTerminate(uint32 cur_sim_task)
{
    SIMAT_SIG_SESSION_TERMINATED_IND_T *psig = NULL;

    SCI_CREATE_SIGNAL(psig, SIMAT_SESSION_TERMINATED_IND, sizeof(SIMAT_SIG_SESSION_TERMINATED_IND_T), g_sim_task_id[cur_sim_task]);
    psig->dual_sys = cur_sim_task;
    SCI_SEND_SIGNAL((xSignalHeaderRec *)psig, g_sim_simat_task_id[cur_sim_task]);
}

/**********************************************************************
//    Description:
//      Notify the STK task that the current stk application session is
//    terminated
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_TerminalProfile(
    uint32 cur_sim_task,
    SIMAT_SIG_TERMINAL_PROFILE_REQ_T *psig    //the service request signal
)
{
    SIMAT_SIG_TERMINAL_PROFILE_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;

    SCI_CREATE_SIGNAL(pcnf, SIMAT_TERMINAL_PROFILE_CNF, sizeof(SIMAT_SIG_TERMINAL_PROFILE_CNF_T), g_sim_task_id[cur_sim_task]);

    if(g_sim_card_state[cur_sim_task] < SIM_INITIALIZE_STATE)
    {
        pcnf->result = SIM_TP_CARD_NOT_READY;
    }
    else if(SIM_PH_2_PD == g_sim_card_setting[cur_sim_task].card_phase)
    {
        pcnf->result = SIM_TP_OK;

        ser_result = SIMINSTR_TerminalProfile(cur_sim_task, &(psig->profile));

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = SIM_TP_ERROR;
        }
    }
    else
    {
        pcnf->result = SIM_TP_CARD_NOT_SUPPORT;
    }

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

#ifdef GSM_DPHONE_VER2_SUPPORT
/**********************************************************************
//    Description:
//      the function that process the read file request signal
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadFileBufByPath(
    uint32 cur_sim_task,
    SIM_SIG_READ_FILE_BUF_BY_PATH_REQ_T *psig //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_READ_FILE_BUF_BY_PATH_CNF_T *pcnf;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    uint16 reback_file_path[MAX_DEEP_FILE_LEVEL];
    uint8 path_tmp[10] = {0};
    uint8 path_len = 0, i;

    // 04/24/2008 delete the trace
    //  SCI_TRACE_LOW("SIM::SIMSIGNAL_ReadFileBufByPath.");

    SCI_MEMSET(reback_file_path, 0x0, (sizeof(uint16)*MAX_DEEP_FILE_LEVEL));

    //create the confirm signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_FILE_BUF_BY_PATH_CNF, sizeof(SIM_SIG_READ_FILE_BUF_BY_PATH_CNF_T), g_sim_task_id[cur_sim_task]);
    //get info for the confirm signal from request signal
    pcnf->command_ref = psig->command_ref;
    pcnf->file_id = psig->file_id;
    pcnf->record_num = psig->record_num;
    pcnf->result = SIM_READ_OK;

    if(psig->file_level > 5)
    {
        psig->file_level = 5;
    }

    path_len = 2 * psig->file_level;

    for(i = 0; i < psig->file_level; i++)
    {
        path_tmp[2 * i] = (uint8)(psig->file_path[i] >> 8);
        path_tmp[2 * i + 1] = (uint8)(psig->file_path[i]);
    }

    //then begin to read the special file
    ser_result =  SIMINSTR_ImiReadEF(cur_sim_task, psig->file_id, path_len,
                                     path_tmp,
                                     0,
                                     psig->record_num,
                                     SIM_RECORD_ABSOLUTE,
                                     0,
                                     &(pcnf->data_len),
                                     pcnf->buf
                                    );
    //process the error,get the error reason
    //SCI_TRACE_LOW:"SIM:: SIMINSTR_ImiReadEF, read 0x%x ser_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4740_112_2_17_23_0_1_4658, (uint8 *)"dd", psig->file_id, ser_result);

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = AnalysisReadFileFail(ser_result);
    }

#ifndef WIN32
    SIM_PrintTxBuf(cur_sim_task);
#endif
    //send confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);

    //reback to the MF
    reback_file_path[0] = 0x3F00;
    ser_result = SIMINSTR_SelectFileByPath(cur_sim_task, 0x3F00,
                                           1,
                                           reback_file_path,
                                           &select_data);
    //SCI_TRACE_LOW:"SIM:: SIMSIGNAL_ReadFileBufByPath, reback to MF ser_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4758_112_2_17_23_0_2_4659, (uint8 *)"d", ser_result);

    if(SIM_SERVICE_OK != ser_result)
    {
        //SCI_TRACE_LOW:"SIM::SIMSIGNAL_ReadFileBufByPath,reback to MF failed2."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4761_112_2_17_23_0_2_4660, (uint8 *)"");
    }

    // 04/24/2008 delete the trace
    //  else
    //  {
    //      SCI_TRACE_LOW("SIM::SIMSIGNAL_ReadFileBufByPath,back select MF ok.");
    //  }

    return;
}
#endif

/**********************************************************************
//    Description:
//      the function that process the SIM re-initialize,reference to the

//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIM_ReInitialization(uint32 cur_sim_task)
{
    BOOLEAN result = FALSE;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_CHV_T chv1;         //the CHV1 chv
    SIM_CHV_T unblock_chv1; //the Unblock CHV1 chv
    SIM_TERMINAL_PROFILE_DATA_T simat_profile;
    SIM_RESULT_INITIALIZE_SIM_E reini_result;
    SIM_EF_MAPPING_SST_T    sst_mapping;
#ifdef SIM_DROP_RESTORE_SUPPORT
	static uint32 enter_times[4] = {0,0,0,0};
#endif
    uint32 exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    SIMFS_SetCurDirectoryFile(cur_sim_task, SIM_FILE_MF);
    SIMINS_ClearCurFileResponse(cur_sim_task);

#ifdef SIM_DROP_RESTORE_SUPPORT
	enter_times[cur_sim_task]++;
	if (enter_times[cur_sim_task] > 2)
	{
	    //ATR happen randomly, it cannot locate directory OK.
		s_sim_is_usim[cur_sim_task] = SIM_2G_APPLICATION;
		SCI_TRACE_LOW("SIM%d:ReInitialization %d",cur_sim_task,enter_times[cur_sim_task]);
	}
#endif

    ser_result = SIMINSTR_GetGSMSpecificData(cur_sim_task, &(g_sim_card_setting[cur_sim_task].gsm_data));

    if(SIM_SERVICE_OK == ser_result)
    {
#ifndef SIM_TEST_IN_WIN32

        if((SIM_CLOCK_S_ALLOWED_2 == g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type)
            || (SIM_CLOCK_S_ALLOWED_4 == g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type))
        {
            SIMHAL_SetClockStopLevel(exchange_card_id, TRUE);
        }
        else if(SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type)
        {
            SIMHAL_SetClockStopLevel(exchange_card_id, FALSE);
        }

#endif
    }
    else
    {
        result = FALSE;
        goto REINIT_END;
    }

    //get the previous chv1 and puk1
    chv1 = s_sim_chv1[cur_sim_task];
    unblock_chv1 = s_sim_unblock_chv1[cur_sim_task];
    VerifyCHV1AtInit(cur_sim_task, chv1, unblock_chv1, &reini_result); //verify the chv1

    if(SIM_INI_OK != reini_result)
    {
        if(SIM_INI_ERROR_CHV_NOT_INPUT == reini_result)
        {
            //This is because an error happened before PIN1/PUK1 verified.
            //We dont't know the correct PIN1/PUK1. Just continue.
            result = TRUE;
        }

        //SCI_TRACE_LOW:"SIM::Verify CHV1 failed in SIM ReInitialization!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4819_112_2_17_23_0_2_4661, (uint8 *)"");
        goto REINIT_END;
    }

    //the Read EF_SST and get service table
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_SST, &sst_mapping);

    if(SIM_SERVICE_OK != ser_result)
    {
        if(IsFatalError(ser_result)) //EFsst may not exist
        {
            //SCI_TRACE_LOW:"SIM::Read EFsst Failed in SIM ReInitialization!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4830_112_2_17_23_0_2_4662, (uint8 *)"");
            goto REINIT_END;
        }
    }
    else
    {
#ifdef SIM_3G_USIM_SUPPORT
        //when USIM, there is very special. Because we will not decide ADN/EXT1 in sim decode EFust.
        //we have to use the old value after RESTORE. Or the old value will be cleared.
        sst_mapping.service_table[SIMSER_PH2_ADN].allocated = g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].allocated;
        sst_mapping.service_table[SIMSER_PH2_ADN].activated = g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].activated;
        sst_mapping.service_table[SIMSER_PH2_EXT1].allocated = g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].allocated;
        sst_mapping.service_table[SIMSER_PH2_EXT1].activated = g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].activated;
#endif
        //save the sim service table for laterly use
        SCI_MEMCPY(&g_sim_card_setting[cur_sim_task].service_table, &sst_mapping, sizeof(SIM_EF_MAPPING_SST_T));
    }

    //the SIM Card require the ME to perform the profile download procedure.
    if(SIM_PH_2_PD == g_sim_card_setting[cur_sim_task].card_phase)
    {
        simat_profile.profile_len = SIM_MAX_C_APDU_DATA_LEN;
        SIMAT_GetMeProfile(simat_profile.profile_arr, &(simat_profile.profile_len));
		SIMSIGNAL_ChangeSTKTerminalProfile(&simat_profile);//You can enabe it if you want to change
        ser_result = SIMINSTR_TerminalProfile(cur_sim_task, &(simat_profile));

        if(SIM_SERVICE_OK != ser_result)
        {
            //SCI_TRACE_LOW:"SIM::STK terminal profile fail %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4856_112_2_17_23_0_2_4663, (uint8 *)"d", ser_result);
        }
    }

    //SCI_TRACE_LOW:"SIM%d::SIM ReInitialization success!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4860_112_2_17_23_0_2_4664, (uint8 *)"d", cur_sim_task);

    result = TRUE;
REINIT_END:
    return result;
}

/*****************************************************************************/
//  Description: The function runs the procedures that are supported both by
//  the ME and the SIM when in SIM initialization
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:  the procedures is defined in 3GPP TS 11.11 section 11.2.1
/*****************************************************************************/
LOCAL SIM_RESULT_INITIALIZE_SIM_E Init_GetSimFileTable(                             //RETURN:
    const uint32 cur_sim_task,         //IN:
    SIM_PHASE_E  card_phase,     //IN:
    SIM_INI_INFO *ini_file_ptr,   //IN/OUT:
    BOOLEAN is_in_refresh
)
{
    SIM_SERVICE_RESULT_E    ser_result     = SIM_SERVICE_OK;
    SIM_EF_MAPPING_SST_T    *saved_sst_ptr = &(g_sim_card_setting[cur_sim_task].service_table);
    SIM_TERMINAL_PROFILE_DATA_T simat_profile;
    SIMAT_SIG_TERMINAL_PROFILE_IND_T *simat_ind;
#ifdef SIM_CDMA_CARD_SUPPORT
    SIM_EF_MAPPING_SST_T cdma_cst;
    SIM_SERVICE_RESULT_E ser_cdma_result = SIM_SERVICE_OK;
#endif
    BOOLEAN is_need_get_plmnsel = TRUE;
    SIM_RESULT_INITIALIZE_SIM_E ini_result = SIM_INI_OK;

    // SIM Service Table request;
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_SST, &(ini_file_ptr->sst_mapping));

    ini_file_ptr->cdma_type = SIM_CDMA_NONE;//init
#ifdef SIM_CDMA_CARD_SUPPORT
#ifndef BYD_PRJ//BYD said, donot check the content of imsi.
    //we should assume a smart card maybe both have DFgsm and DFcdma. But some cards
    //only have an empty DF...
    if(s_sim_cdma_flag[cur_sim_task])
    {
        ser_cdma_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CST, &cdma_cst);

        if(SIM_SERVICE_OK == ser_cdma_result)
        {
            if(SIM_SERVICE_OK == ser_result)
            {
                //the current smart card both have DFgsm/EFsst and DFcdma/EFcst.
                //we will get network parameters from DFgsm.
                //And we will SELECT DFcdma before DFtelecom, I dont know what will happen...
            }
            else
            {
                //use the CST instead of SST
                SCI_MEMCPY(&(ini_file_ptr->sst_mapping), &cdma_cst, sizeof(SIM_EF_MAPPING_SST_T));
                ser_result = SIM_SERVICE_OK;
            }
        }
        else
        {
            //A Shandong Zhenzhouxing GSM SIM both have DFcdma and DFgsm, but no EFcst.
            if(s_sim_cdma_flag[cur_sim_task] && (!s_sim_cdma_only_flag[cur_sim_task]))
            {
                s_sim_cdma_flag[cur_sim_task] = FALSE;
            }
        }

        if(s_sim_cdma_flag[cur_sim_task])
        {
            ser_cdma_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CDMA_IMSI, &s_sim_cdma_imsi_m[cur_sim_task]);

            if(SIM_SERVICE_OK == ser_cdma_result)
            {
            }
            else
            {
                s_sim_cdma_flag[cur_sim_task] = FALSE;
            }
        }
    }

#endif
#endif

    if(SIM_SERVICE_OK == ser_result)
    {
        //save the SIM service table for laterly use
        SCI_MEMCPY(saved_sst_ptr, &(ini_file_ptr->sst_mapping), sizeof(SIM_EF_MAPPING_SST_T));
    }
    else
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFsst Failed %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4960_112_2_17_23_0_2_4666, (uint8 *)"d", ser_result);
#endif
        SCI_MEMSET(saved_sst_ptr, 0, sizeof(SIM_EF_MAPPING_SST_T));

        if(IsFatalError(ser_result))
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
        else
        {
#if defined (_SUPPORT_IPC_) || defined (GSM_CUSTOMER_AFP_SUPPORT)
            //SS need return if no EFsst.
            return (SIM_INI_ERROR_READ_SST);
#endif
        }
    }

#ifdef GSM_DPHONE_SUPPORT
#if 0
    //需要两次TP的握奇卡
    if(s_dphone_reset_times[cur_sim_task] < 2)//Some special WATCH card need ProfileDown again.
#else
    if(s_dphone_reset_times[cur_sim_task] == 0) //dont change here
#endif
#elif defined GSM_DPHONE_VER2_SUPPORT
    if(!s_sim_is_dphone_operation_skip[cur_sim_task])
#endif
    {
#ifdef _SUPPORT_IPC_
        //IPC mode, we will decide STK TP after imsi READ.
#else
        //the SIM Card require the ME to perform the profile download procedure.
        if(SIM_PH_2_PD == g_sim_card_setting[cur_sim_task].card_phase)
        {
#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_PS_SIM_SAVE_POWER)
            //No STK
#else
            simat_profile.profile_len = SIM_MAX_C_APDU_DATA_LEN;
            SIMAT_GetMeProfile(simat_profile.profile_arr, &(simat_profile.profile_len));

            SIMSIGNAL_ChangeSTKTerminalProfile(&simat_profile);

            ser_result = SIMINSTR_TerminalProfile(cur_sim_task, &(simat_profile));

            if(SIM_SERVICE_OK != ser_result)
            {
                //A Guanzhou USIM will response 6D00
                //SCI_TRACE_LOW:"SIM::SIM Application Toolkit terminal profile fail %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4982_112_2_17_23_0_2_4667, (uint8 *)"d", ser_result);

                if(IsFatalError(ser_result))
                {
                    return (SIM_INI_ERROR_CARD_OUT);
                }
            }
#endif

            if(!is_in_refresh)
            {
                SCI_CREATE_SIGNAL(simat_ind, SIMAT_TERMINAL_PROFILE_IND, sizeof(SIMAT_SIG_TERMINAL_PROFILE_IND_T), g_sim_task_id[cur_sim_task]);
                SCI_SEND_SIGNAL(simat_ind, g_sim_simat_task_id[cur_sim_task]);
            }
        }
#endif
    }

    //for dphone, we will warm reset once late, so we do not read this files at the first time
#ifdef GSM_DPHONE_SUPPORT
    if(s_sim_gsm_dphone_support[cur_sim_task])
    {
        if(0 == s_dphone_reset_times[cur_sim_task])
        {
            return (SIM_INI_OK);
        }
    }
#endif

#ifdef SIM_CDMA_CARD_SUPPORT
    if(s_sim_cdma_flag[cur_sim_task] && s_sim_cdma_only_flag[cur_sim_task])
    {
        //if only has DFcdma, we can return now.
        ini_file_ptr->cdma_type = SIM_CDMA_ONLY;
        return SIM_INI_OK;
    }
#endif

    // IMSI request;
    ini_result = Init_ReadImsiFile(cur_sim_task, &(ini_file_ptr->imsi_mapping));

    if(SIM_INI_OK != ini_result)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFimsi failed %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5016_112_2_17_23_0_2_4668, (uint8 *)"d", ini_result);
#endif
#ifdef SIM_CDMA_CARD_SUPPORT

        if(s_sim_cdma_flag[cur_sim_task])
        {
            //maybe a CDMA card have an empty DFgsm.
        }
        else
        {
            return (SIM_INI_ERROR_READ_IMSI);
        }

#else

        if(IsFatalError(ser_result))
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
        else
        {
            return (SIM_INI_ERROR_READ_IMSI);
        }

#endif
    }

    g_sim_card_setting[cur_sim_task].imsi_mapping = ini_file_ptr->imsi_mapping;

#ifdef SIM_CDMA_CARD_SUPPORT
#ifndef BYD_PRJ//BYD said, donot check the content of imsi.
    //Acording to ChinaTelecom:
    //case1: if the DFgsm/EFimsi is 46099, GSM imsi is invalid. 
    if(s_sim_cdma_flag[cur_sim_task])
    {
        //case1: if the DFgsm/EFimsi is 46099, GSM imsi is invalid. This is also the Samasung case1.
        if ((460 == g_sim_card_setting[cur_sim_task].imsi_mapping.mcc) 
            && (99 == g_sim_card_setting[cur_sim_task].imsi_mapping.mnc))
        {
            s_sim_cdma_only_flag[cur_sim_task] = TRUE;
        }

#ifdef _SUPPORT_IPC_
        //Taiwan CDMA/GSM dualcard cannot pbey this rule (DFcdma/EFimsi == DFgsm/EFimsi)
#else
        //case2: if the DFgsm/EFimsi == DFcdma/EFimsi, GSM imsi is invalid.
        if ((s_sim_cdma_imsi_m[cur_sim_task].mcc == g_sim_card_setting[cur_sim_task].imsi_mapping.mcc)
            && (s_sim_cdma_imsi_m[cur_sim_task].mnc == g_sim_card_setting[cur_sim_task].imsi_mapping.mnc))
        {
            s_sim_cdma_only_flag[cur_sim_task] = TRUE;
        }
#endif

#ifdef _SUPPORT_IPC_
        //Samsung case2:  special value from SS.
        if ((0x08 == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[0]) 
            && (0x49 == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[1]) 
            && (0x06 == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[2]) 
            && (0x10 == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[3]) 
            && (0x00 == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[4]) 
            && (0x00 == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[5]) 
            && (0x00 == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[6]) 
            && (0x00 == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[7]))
        {
            s_sim_cdma_only_flag[cur_sim_task] = TRUE;
        }

        //Samsung case3: All 0xFF in DFgsm/EFimsi
        if ((0xFF == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[0]) 
            && (0xFF == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[1]) 
            && (0xFF == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[2]) 
            && (0xFF == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[3]) 
            && (0xFF == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[4]) 
            && (0xFF == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[5]) 
            && (0xFF == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[6]) 
            && (0xFF == g_sim_card_setting[cur_sim_task].imsi_mapping.imsi.imsi_value[7]))
        {
            s_sim_cdma_only_flag[cur_sim_task] = TRUE;
        }
#endif

        SCI_TRACE_LOW("SIM CDMA: GSM imsi mcc=0x%x, mnc=0x%x. CDMA_only=%d", \
                      g_sim_card_setting[cur_sim_task].imsi_mapping.mcc, \
                      g_sim_card_setting[cur_sim_task].imsi_mapping.mnc, \
                      s_sim_cdma_only_flag[cur_sim_task]);

        if(s_sim_cdma_only_flag[cur_sim_task])
        {
            ini_file_ptr->cdma_type = SIM_CDMA_ONLY;
            return SIM_INI_OK;
        }

        ini_file_ptr->cdma_type = SIM_CDMA_GSM_DUAL;
    }
#endif
#endif

#ifdef _SUPPORT_IPC_
    //IPC mode, we will STK TP here only for GSM card.
    if (SIM_CDMA_NONE == ini_file_ptr->cdma_type)
    {
        //the SIM Card require the ME to perform the profile download procedure.
        if(SIM_PH_2_PD == g_sim_card_setting[cur_sim_task].card_phase)
        {
            simat_profile.profile_len = SIM_MAX_C_APDU_DATA_LEN;
            SIMAT_GetMeProfile(simat_profile.profile_arr, &(simat_profile.profile_len));
            ser_result = SIMINSTR_TerminalProfile(cur_sim_task, &(simat_profile));
            if (SIM_SERVICE_OK != ser_result)
            {
                //A Guanzhou USIM will response 6D00
                //SCI_TRACE_LOW:"SIM::SIM Application Toolkit terminal profile fail %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_4982_112_2_17_23_0_2_4667, (uint8 *)"d", ser_result);
                if (IsFatalError(ser_result))
                {
                    return (SIM_INI_ERROR_CARD_OUT);
                }
            }

            if (!is_in_refresh)
            {
                SCI_CREATE_SIGNAL(simat_ind, SIMAT_TERMINAL_PROFILE_IND, sizeof(SIMAT_SIG_TERMINAL_PROFILE_IND_T), g_sim_task_id[cur_sim_task]);
                SCI_SEND_SIGNAL(simat_ind, g_sim_simat_task_id[cur_sim_task]);
            }
        }
    }
#endif

    // Access Control request;
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ACC, &(ini_file_ptr->acc_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        //A very old Shenzhen CMCC has no EFacc. I have to set it as Class0(BIT_0).
        ini_file_ptr->acc_mapping = 1;
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFacc failed %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5041_112_2_17_23_0_2_4669, (uint8 *)"d", ser_result);
#endif

        if(IsFatalError(ser_result))
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
    }

    // Higher Priority PLMN Search Period request;
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_HPLMN, &(ini_file_ptr->hplmn_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFhplmn failed %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5054_112_2_17_23_0_2_4670, (uint8 *)"d", ser_result);
#endif
        SCI_MEMSET(&(ini_file_ptr->hplmn_mapping), 0xff, sizeof(HPLMN_TIMER_VALUE_T));

        if(IsFatalError(ser_result))
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
    }

    // Investigation scan request;

    // User controlled PLMN Selector with Access Technology request;
    if((card_phase >= SIM_PH_2)
        && (saved_sst_ptr->service_table[SIMSER_PH2_PLMN_WACT].allocated)
        && (saved_sst_ptr->service_table[SIMSER_PH2_PLMN_WACT].activated))
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PLMNWACT, &(ini_file_ptr->plmn_wact_t));

        if(SIM_SERVICE_OK != ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFplmnwact failed %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5074_112_2_17_23_0_2_4671, (uint8 *)"d", ser_result);
#endif

            //if read failed for having received POWER OFF request from the upper layer
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                ini_file_ptr->plmn_wact_t.validate_num = 0;
            }
        }
        else
        {
            is_need_get_plmnsel = FALSE;
        }
    }
    else
    {
        ini_file_ptr->plmn_wact_t.validate_num = 0;
    }

    // PLMN selector request; 3G USIM has no PLMNsel
    // If we got the EFplmnwact, then we will not get EFplmnsel.
    if((card_phase >= SIM_PH_2)
        && (saved_sst_ptr->service_table[SIMSER_PH2_PLMN_SEL].allocated)
        && (saved_sst_ptr->service_table[SIMSER_PH2_PLMN_SEL].activated)
        && is_need_get_plmnsel)
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PLMNsel, &(ini_file_ptr->plmn_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFplmnsel failed %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5107_112_2_17_23_0_2_4672, (uint8 *)"d", ser_result);
#endif

            // if read failed for having received POWER OFF request from the upper layer
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                ini_file_ptr->plmn_mapping.validate_num = 0;
                ini_file_ptr->plmn_mapping.max_num = 0;
            }
        }
    }
    else
    {
        ini_file_ptr->plmn_mapping.validate_num = 0;
        ini_file_ptr->plmn_mapping.max_num = 0;
    }

    // Operator controlled PLMN Selector with Access Technology request;
    if((card_phase >= SIM_PH_2)
        && (saved_sst_ptr->service_table[SIMSER_PH2_OPLMN_WACT].allocated)
        && (saved_sst_ptr->service_table[SIMSER_PH2_OPLMN_WACT].activated))
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_OPLMNWACT, &(ini_file_ptr->oplmn_wact_t));

        if(SIM_SERVICE_OK != ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFoplmnwact failed %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5136_112_2_17_23_0_2_4673, (uint8 *)"d", ser_result);
#endif

            //if read failed for having received POWER OFF request from the upper layer
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                ini_file_ptr->oplmn_wact_t.validate_num = 0;
            }
        }
    }
    else
    {
        ini_file_ptr->oplmn_wact_t.validate_num = 0;
    }

    // HPLMN Selector with Access Technology request;
    if((card_phase >= SIM_PH_2)
        && (saved_sst_ptr->service_table[SIMSER_PH2_HPLMN_WACT].allocated)
        && (saved_sst_ptr->service_table[SIMSER_PH2_HPLMN_WACT].activated))
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_HPLMNWACT, &(ini_file_ptr->hplmn_wact_t));

        if(SIM_SERVICE_OK != ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFhplmnwact failed %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5163_112_2_17_23_0_2_4674, (uint8 *)"d", ser_result);
#endif

            //if read failed for having received POWER OFF request from the upper layer
            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
            else
            {
                ini_file_ptr->hplmn_wact_t.validate_num = 0;
            }
        }
    }
    else
    {
        ini_file_ptr->hplmn_wact_t.validate_num = 0;
    }

    // Location Information request;
    ini_file_ptr->gsm_operation_start = TRUE;//I donnot know what's this. we can delete it.
    ini_result = Init_ReadLociFile(cur_sim_task, &(ini_file_ptr->loci_mapping));

    if(SIM_INI_OK != ini_result)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFloci failed %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5186_112_2_17_23_0_2_4675, (uint8 *)"d", ini_result);
#endif

        //A Guangdong Unicom SIM card has no EFloci.
        if(SIM_INI_ERROR_CARD_OUT == ini_result)
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
    }

    g_sim_card_setting[cur_sim_task].loci_info = ini_file_ptr->loci_mapping;

    // GPRS Location Information request;

    // Cipher Key request;
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KC, &(ini_file_ptr->kc_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable Read EFkc failed %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5203_112_2_17_23_0_2_4676, (uint8 *)"d", ser_result);
#endif

        if(IsFatalError(ser_result))
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
    }

    // GPRS Cipher Key request;

    // BCCH information request;
#ifdef SIM_3G_USIM_SUPPORT
    //if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])//Let PS search network with BA even with 3G USIM card.
#endif
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_BCCH, &(ini_file_ptr->bcch_mapping));
        
        if(SIM_SERVICE_OK != ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable EFbcch failed %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5222_112_2_17_23_0_2_4677, (uint8 *)"d", ser_result);
#endif

            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
        }
    }
#if 0//def SIM_3G_USIM_SUPPORT
    else
    {
        //先暂时不处理，让PS  自己找网去
        SCI_MEMSET(&(ini_file_ptr->bcch_mapping), 0, sizeof(BA_LIST_ARR_T));
    }
#endif

    // CPBCCH information request;

    // Forbidden PLMN request;
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_FPLMN, &(ini_file_ptr->fplmn_mapping));

    if(SIM_SERVICE_OK != ser_result)
    {
        ini_file_ptr->fplmn_mapping.length = 0;
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::Init_GetSimFileTable Read EFfplmn failed %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5246_112_2_17_23_0_2_4678, (uint8 *)"d", ser_result);
#endif

        if(IsFatalError(ser_result))
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
    }

    // LSA information request; ---- unsupported item
    ini_file_ptr->solsa_supported = FALSE;

#if defined (MODEM_PLATFORM) && defined (MODEM_PLATFORM_PS_SIM_SAVE_POWER)
	//No CBMID
#else
    // CBMID request;
    if((card_phase >= SIM_PH_2)
        && (saved_sst_ptr->service_table[SIMSER_PH2_SMS_CB_DL].allocated)
        && (saved_sst_ptr->service_table[SIMSER_PH2_SMS_CB_DL].activated))
    {
        ini_file_ptr->cbmid_supported = TRUE;
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CBMID, &(ini_file_ptr->cbmid_mapping));

        if(IsFatalError(ser_result))
        {
            ini_file_ptr->cbmid_supported = FALSE;
            return (SIM_INI_ERROR_CARD_OUT);
        }
        else if(SIM_SERVICE_OK != ser_result)
        {
            ini_file_ptr->cbmid_supported = FALSE;
        }
    }
    else
    {
        ini_file_ptr->cbmid_supported = FALSE;
    }
#endif

    // Depersonalisation Control Keys request; ---- unsupported item
    ini_file_ptr->dck_supported = FALSE;

    // Network's indication of alerting request. ---- unsupported item
    ini_file_ptr->nia_supported = FALSE;

    //GSM11.11 section11.2.1, SIM initialization has been completed.

#ifdef SIM_AOC_SUPPORT
    // the AOC service only when the AOC service is allocated and actived
    if((saved_sst_ptr->service_table[SIMSER_PH2_AOC].allocated)
        && (saved_sst_ptr->service_table[SIMSER_PH2_AOC].activated))
    {
        ser_result = ReadAocContext(cur_sim_task, &(ini_file_ptr->aoc_context), &(ini_file_ptr->aoc_chv_select));

        if(IsFatalError(ser_result))
        {
            return (SIM_INI_ERROR_CARD_OUT);
        }
    }
    else
    {
        ini_file_ptr->aoc_context.AOC_is_support = FALSE;
    }
#else
    ini_file_ptr->aoc_context.AOC_is_support = FALSE;
#endif

    //read SPN request
    ini_file_ptr->spn_supported = TRUE;

    if((saved_sst_ptr->service_table[SIMSER_PH2_SPN].allocated)
        && (saved_sst_ptr->service_table[SIMSER_PH2_SPN].activated))
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_SPN, &(ini_file_ptr->spn_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable EFspn failed %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5311_112_2_17_23_0_3_4679, (uint8 *)"d", ser_result);
#endif
            ini_file_ptr->spn_supported = FALSE;

            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
        }
    }
    else
    {
        ini_file_ptr->spn_supported = FALSE;
    }

    // EHPLMN request
    ser_result = SIMSIGNAL_ReadEhplmnFile(cur_sim_task,
                                          card_phase,
                                          saved_sst_ptr,
                                          ini_file_ptr->imsi_mapping.hplmn,
                                          &(ini_file_ptr->ehplmn_mapping),
                                          &(ini_file_ptr->ehplmn_pi));

    if(IsFatalError(ser_result))
    {
        return (SIM_INI_ERROR_CARD_OUT);
    }

#ifndef MODEM_PLATFORM//I think modem donot need this
    //Group Identifier Level 1 read request
    ini_file_ptr->gid1_supported = TRUE;

    if((saved_sst_ptr->service_table[SIMSER_PH2_GID1].allocated)
        && (saved_sst_ptr->service_table[SIMSER_PH2_GID1].activated))
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_GID1, &(ini_file_ptr->gid1_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable EFgid1 failed %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5349_112_2_17_23_0_3_4680, (uint8 *)"d", ser_result);
#endif
            ini_file_ptr->gid1_supported = FALSE;

            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
        }
    }
    else
    {
        ini_file_ptr->gid1_supported = FALSE;
    }

    //Group Identifier Level 2 read request
    ini_file_ptr->gid2_supported = TRUE;

    if((saved_sst_ptr->service_table[SIMSER_PH2_GID2].allocated)
        && (saved_sst_ptr->service_table[SIMSER_PH2_GID2].activated))
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_GID2, &(ini_file_ptr->gid2_mapping));

        if(SIM_SERVICE_OK != ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:: Init_GetSimFileTable EFgid2 failed %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5375_112_2_17_23_0_3_4681, (uint8 *)"d", ser_result);
#endif
            ini_file_ptr->gid2_supported = FALSE;

            if(IsFatalError(ser_result))
            {
                return (SIM_INI_ERROR_CARD_OUT);
            }
        }
    }
    else
    {
        ini_file_ptr->gid2_supported = FALSE;
    }
#endif

    return (SIM_INI_OK);
}

/*****************************************************************************/
//  Description: read file EF_EHPLMN and EF_EHPLMNPI
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL SIM_SERVICE_RESULT_E SIMSIGNAL_ReadEhplmnFile(
    const uint32 cur_sim_task, //IN:
    const SIM_PHASE_E           card_phase,       //IN:
    const SIM_EF_MAPPING_SST_T  *saved_sst_ptr,   //IN:
    const PLMN_T                hplmn,            //IN:
    SIM_EF_MAPPING_EHPLMN_T     *ehplmn_list_ptr, //OUT:
    SIM_EF_MAPPING_EHPLMNPI_T   *ehplmn_pi_ptr    //OUT:
)
{
    SIM_SERVICE_RESULT_E    ser_result     = SIM_SERVICE_OK;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN                 ehplmn_exist   = FALSE;
#endif

    SCI_MEMSET(ehplmn_list_ptr, 0, sizeof(SIM_EF_MAPPING_EHPLMN_T));
    SCI_MEMSET(ehplmn_pi_ptr,   0, sizeof(SIM_EF_MAPPING_EHPLMNPI_T));

#ifdef SIM_3G_USIM_SUPPORT

    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]) && (card_phase >= SIM_PH_2))
    {
        // Equivalent HPLMN
        if((saved_sst_ptr->service_table[SIMSER_PH2_EHPLMN].allocated)
            && (saved_sst_ptr->service_table[SIMSER_PH2_EHPLMN].activated))
        {
            ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_EHPLMN, ehplmn_list_ptr);
            //SCI_TRACE_LOW:"SIM:: Read EFehplmn cause = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5422_112_2_17_23_0_3_4682, (uint8 *)"d", ser_result);

            if(SIM_SERVICE_OK == ser_result)
            {
                // Equivalent HPLMN Presentation Indication
                if((saved_sst_ptr->service_table[SIMSER_PH2_EHPLMNPI].allocated)
                    && (saved_sst_ptr->service_table[SIMSER_PH2_EHPLMNPI].activated))
                {
                    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_EHPLMNPI, ehplmn_pi_ptr);
                    //SCI_TRACE_LOW:"SIM:: Read EFehplmnpi cause = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5431_112_2_17_23_0_3_4683, (uint8 *)"d", ser_result);
                }
            }

            ehplmn_exist = TRUE;
        }
    }

    if(!ehplmn_exist)
#endif
    {
        // 中国移动终端选网技术要求  section 8
        if((460 == hplmn.mcc)
            && ((00 == hplmn.mnc) || (02 == hplmn.mnc) || (07 == hplmn.mnc)))
        {
            ehplmn_pi_ptr->pi_value                     = 0;
            ehplmn_list_ptr->max_num                    = 3;
            ehplmn_list_ptr->validate_num               = 3;
            ehplmn_list_ptr->plmn_arr[0].mcc            = 460;
            ehplmn_list_ptr->plmn_arr[0].mnc            = 00;
            ehplmn_list_ptr->plmn_arr[0].mnc_digit_num  = 2;
            ehplmn_list_ptr->plmn_arr[1].mcc            = 460;
            ehplmn_list_ptr->plmn_arr[1].mnc            = 02;
            ehplmn_list_ptr->plmn_arr[1].mnc_digit_num  = 2;
            ehplmn_list_ptr->plmn_arr[2].mcc            = 460;
            ehplmn_list_ptr->plmn_arr[2].mnc            = 07;
            ehplmn_list_ptr->plmn_arr[2].mnc_digit_num  = 2;
            //SCI_TRACE_LOW:"SIM:: CMCC special HPLMN"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5458_112_2_17_23_0_3_4684, (uint8 *)"");
        }

#ifdef GSM_CUSTOMER_AFP_SUPPORT

        // [PLM ID  P120807-3833] Eric CIS Free SIM into Orange Network for France region
        if((208 == hplmn.mcc)
            && (15 == hplmn.mnc))
        {
            ehplmn_pi_ptr->pi_value                     = 0;
            ehplmn_list_ptr->max_num                    = 2;
            ehplmn_list_ptr->validate_num               = 2;
            ehplmn_list_ptr->plmn_arr[0].mcc            = 208;
            ehplmn_list_ptr->plmn_arr[0].mnc            = 15;
            ehplmn_list_ptr->plmn_arr[0].mnc_digit_num  = 2;
            ehplmn_list_ptr->plmn_arr[1].mcc            = 208;
            ehplmn_list_ptr->plmn_arr[1].mnc            = 01;
            ehplmn_list_ptr->plmn_arr[1].mnc_digit_num  = 2;
            //SCI_TRACE_LOW:"SIM:: CMCC special HPLMN for Free SIM in Orange NW France "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5458_112_2_17_23_0_3_4684, (uint8 *)"");
        }

        // [PLM ID  P121018-6628] Eton CIS T-Mobile into Orange Network for UK region
        if((234 == hplmn.mcc)
            && ((34 == hplmn.mnc) || (30 == hplmn.mnc) || (33 == hplmn.mnc)))
        {
            ehplmn_pi_ptr->pi_value                     = 0;
            ehplmn_list_ptr->max_num                    = 3;
            ehplmn_list_ptr->validate_num               = 3;
            ehplmn_list_ptr->plmn_arr[0].mcc            = 234;
            ehplmn_list_ptr->plmn_arr[0].mnc            = 34;
            ehplmn_list_ptr->plmn_arr[0].mnc_digit_num  = 2;
            ehplmn_list_ptr->plmn_arr[1].mcc            = 234;
            ehplmn_list_ptr->plmn_arr[1].mnc            = 30;
            ehplmn_list_ptr->plmn_arr[1].mnc_digit_num  = 2;
            ehplmn_list_ptr->plmn_arr[2].mcc            = 234;
            ehplmn_list_ptr->plmn_arr[2].mnc            = 33;
            ehplmn_list_ptr->plmn_arr[2].mnc_digit_num  = 2;
            //SCI_TRACE_LOW:"SIM:: CMCC special HPLMN for T-Mobile in Orange NW UK and vice versa "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5458_112_2_17_23_0_3_4684, (uint8 *)"");
        }

        // [PLM ID 120831-0153] Kiwi TS Vivo NW in Brazil

        if((724 == hplmn.mcc)
            && ((11 == hplmn.mnc) || (06 == hplmn.mnc) || (10 == hplmn.mnc) || (23 == hplmn.mnc)))
        {
            ehplmn_pi_ptr->pi_value                     = 0;
            ehplmn_list_ptr->max_num                    = 4;
            ehplmn_list_ptr->validate_num               = 4;
            ehplmn_list_ptr->plmn_arr[0].mcc            = 724;
            ehplmn_list_ptr->plmn_arr[0].mnc            = 11;
            ehplmn_list_ptr->plmn_arr[0].mnc_digit_num  = 2;
            ehplmn_list_ptr->plmn_arr[1].mcc            = 724;
            ehplmn_list_ptr->plmn_arr[1].mnc            = 06;
            ehplmn_list_ptr->plmn_arr[1].mnc_digit_num  = 2;
            ehplmn_list_ptr->plmn_arr[2].mcc            = 724;
            ehplmn_list_ptr->plmn_arr[2].mnc            = 10;
            ehplmn_list_ptr->plmn_arr[2].mnc_digit_num  = 2;
            ehplmn_list_ptr->plmn_arr[3].mcc            = 724;
            ehplmn_list_ptr->plmn_arr[3].mnc            =  23;
            ehplmn_list_ptr->plmn_arr[3].mnc_digit_num  = 2;
            //SCI_TRACE_LOW:"SIM:: CMCC special HPLMN for Vivo SIM in Brazail "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5458_112_2_17_23_0_3_4684, (uint8 *)"");
        }

#endif

    }

    return (ser_result);
}

/**********************************************************************
//    Description:
//      the function that process the stk refresh signal,the refresh type is sim initialization.
//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_RefreshIni(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_INI_REQ_T *psig  //in:the initialization request signal
)
{
    BOOLEAN result = FALSE;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_REFRESH_INI_CNF_T *ind;
    uint8 imsi_is_ff_buf[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#ifdef GSM_CUSTOMER_AFP_SUPPORT
    uint8 imsi_is_00_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif
    SIM_RESULT_INITIALIZE_SIM_E ini_result = SIM_INI_OK;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_EF_MAPPING_PHASE_T phase_mapping;
    SIM_EF_MAPPING_ICCID_T tmp_iccid;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM::Refresh Ini begin!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5487_112_2_17_23_0_3_4685, (uint8 *)"");
#endif
    //creat the initialize confirm signal of the request signal
    SCI_CREATE_SIGNAL(ind, SIM_REFRESH_INI_CNF, (uint16)(sizeof(SIM_SIG_REFRESH_INI_CNF_T)), g_sim_task_id[cur_sim_task]);
    ind->command_ref = psig->command_ref;

    //Read EF_ICCID for later user
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ICCID, &tmp_iccid);

    if(SIM_SERVICE_OK != ser_result)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIni Read EFiccid failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5498_112_2_17_23_0_3_4686, (uint8 *)"");
#endif

        if(IsFatalError(ser_result))
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
            goto REFRESH_INI_END;
        }
    }

    //then select DFgsm,for profile download
    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIni select DFgsm failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5506_112_2_17_23_0_3_4687, (uint8 *)"");

        if(IsFatalError(ser_result))
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
        }
        else
        {
            ind->result = SIM_INI_ERROR_UNKNOWN;
        }

        goto REFRESH_INI_END;
    }

    //refer to specification, refresh should do initialization precedure after chv verification
#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        //read the EFphase
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PHASE, &phase_mapping);

        if(ser_result != SIM_SERVICE_OK)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::Read EFphase failed in Refresh Ini!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5521_112_2_17_23_0_3_4688, (uint8 *)"");
#endif

            if(IsFatalError(ser_result))
            {
                ind->result = SIM_INI_ERROR_CARD_OUT;
                goto REFRESH_INI_END;
            }
        }
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        // 3G ??óD???t
        phase_mapping.phase = SIM_PH_2_PD;//SIM_PH_2?
    }

#endif
    //get the sim card phase
    g_sim_card_setting[cur_sim_task].card_phase = phase_mapping.phase;

    //read the EFad
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_AD, &(ind->ad_mapping));

    if(ser_result == SIM_SERVICE_OK)
    {
        SaveMncLenInfo(cur_sim_task, &(ind->ad_mapping));
    }
    else
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIni Read EFad failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5544_112_2_17_23_0_3_4689, (uint8 *)"");
#endif

        if(IsFatalError(ser_result))
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
            goto REFRESH_INI_END;
        }
    }

    ini_result = Init_GetSimFileTable(cur_sim_task, g_sim_card_setting[cur_sim_task].card_phase, &(ind->ini_info), TRUE);

    if(SIM_INI_OK == ini_result)
    {
#ifdef SIM_3G_USIM_SUPPORT
        SIMDN_GetPBRInfo(cur_sim_task);
#endif

        //BDN capability check
        ind->is_bdn_enable = SIMDN_BDNCapabilityCheck(cur_sim_task);

        //FDN capability check
        ind->is_fdn_enable = SIMDN_FDNCapabilityCheck(cur_sim_task);

        //SCI_TRACE_LOW:"SIM::SIM Refresh ini success!FDN %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5558_112_2_17_23_0_3_4690, (uint8 *)"d", ind->is_fdn_enable);
        ind->result = SIM_INI_OK;
        result = TRUE;

        //if the imsi is full 0xff
#ifdef GSM_CUSTOMER_AFP_SUPPORT

        if(((memcmp(imsi_is_ff_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0)
            || (memcmp(imsi_is_00_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0))
            && (!ind->ini_info.imsi_mapping.imsi.valid_flag))
#else
        if((memcmp(imsi_is_ff_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0)
            && (ind->ini_info.imsi_mapping.imsi.length == 0xff)
            && (!ind->ini_info.imsi_mapping.imsi.valid_flag))
#endif
        {
            //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIni imsi is full 0xff."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5567_112_2_17_23_0_3_4691, (uint8 *)"");
            ind->result = SIM_INI_ERROR_IMSI_FULL_FF;
            result = FALSE;
        }
    }
    else
    {
        if(SIM_INI_ERROR_CARD_OUT == ini_result)
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
        }
        else
        {
            ind->result = SIM_INI_ERROR_UNKNOWN;
        }
    }

REFRESH_INI_END:
    //send confirm signal to the signal sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)ind, ((xSignalHeaderRec *)psig)->Sender);

    return result;
}

/**********************************************************************
//    Description:
//      the function that process the stk refresh signal,the refresh type is sim file change
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_RefreshFileChg(    //return value:the operation is success or not
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_FILE_CHG_REQ_T *psig  //in:the file change signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_REFRESH_FILE_CHG_CNF_T *cnf;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    uint8 i = 0, file_num = psig->file_num;
    SIM_EF_STRUCTURE_E file_structure;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM::Refresh File change begin!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5597_112_2_17_23_0_3_4692, (uint8 *)"");
#endif

    //creat the file change confirm signal of the request signal
    SCI_CREATE_SIGNAL(cnf, SIM_REFRESH_FILE_CHG_CNF, sizeof(SIM_SIG_REFRESH_FILE_CHG_CNF_T), g_sim_task_id[cur_sim_task]);
    cnf->command_ref = psig->command_ref;
    cnf->file_num = psig->file_num;
    cnf->result = TRUE;

    //first to select the MF
    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &select_data);

    if(SIM_SERVICE_OK == ser_result)
    {
        for(i = 0; i < file_num; i++)
        {
            //the file must be a ef file
            if(SIMFS_GetFileType(psig->file_arr[i]) != SIM_FILE_T_EF)
            {
                cnf->result = FALSE;
                break;
            }

            cnf->file_info[i].file_name = psig->file_arr[i];
            file_structure = SIMFS_GetEFStructure(psig->file_arr[i]);

            //if the file is record file,then to get the record number
            if((SIM_EF_S_LINEAR_FIXED == file_structure) ||
                (SIM_EF_S_CYCLIC == file_structure))
            {
                if (SIMSIGNAL_CheckRecordEFInServiceTable(cur_sim_task,psig->file_arr[i]))
                {
                    ser_result = SIMSER_SelectFile(cur_sim_task, psig->file_arr[i], &select_data);
                }
                else
                {
                    ser_result = SIM_SERVICE_SELECT_NO_SUCH_FILE;
                }

                if(SIM_SERVICE_OK != ser_result)
                {
                    cnf->result = FALSE;
                    break;
                }

                cnf->file_info[i].record_num = (uint8)(select_data.freespace_filesize / select_data.dfnumber_recordlen);
            }
            else
            {
                cnf->file_info[i].record_num = 1;
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshFileChg select MF failed."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5643_112_2_17_23_0_3_4693, (uint8 *)"");
        cnf->result = FALSE;
    }

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM::Refresh File change end!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5648_112_2_17_23_0_3_4694, (uint8 *)"");
#endif
    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)cnf, ((xSignalHeaderRec *)psig)->Sender);

    return TRUE;
}

/**********************************************************************
//    Description:
//      the function that proccess the stk refresh signal,the refresh type is sim initialization and file change
//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_RefreshIniFileChg(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_INI_FILE_CHG_REQ_T *psig  //in:the initialization request signal
)
{
    BOOLEAN result = FALSE;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_REFRESH_INI_FILE_CHG_CNF_T *ind;
    uint8 imsi_is_ff_buf[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#ifdef GSM_CUSTOMER_AFP_SUPPORT
    uint8 imsi_is_00_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    uint8 i = 0, file_num = psig->file_num;
    SIM_EF_STRUCTURE_E file_structure;
    SIM_RESULT_INITIALIZE_SIM_E ini_result = SIM_INI_OK;
    SIM_EF_MAPPING_PHASE_T phase_mapping;
    SIM_EF_MAPPING_ICCID_T tmp_iccid;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM::Refresh Ini File change begin!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5680_112_2_17_23_0_3_4695, (uint8 *)"");
#endif

    //creat the initialize confirm signal of the request signal
    SCI_CREATE_SIGNAL(ind, SIM_REFRESH_INI_FILE_CHG_CNF, (uint16)(sizeof(SIM_SIG_REFRESH_INI_FILE_CHG_CNF_T)), g_sim_task_id[cur_sim_task]);
    ind->command_ref = psig->command_ref;

    //Read EF_ICCID for later user
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ICCID, &tmp_iccid);

    if(SIM_SERVICE_OK != ser_result)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIniFileChg Read EFiccid failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5692_112_2_17_23_0_3_4696, (uint8 *)"");
#endif

        if(IsFatalError(ser_result))
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
            goto REFRESH_INI_FILE_CHG_END;
        }
    }

    //then select DFgsm,for profile download
    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIniFileChg select DFgsm failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5700_112_2_17_23_0_3_4697, (uint8 *)"");

        if(IsFatalError(ser_result))
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
        }
        else
        {
            ind->result = SIM_INI_ERROR_UNKNOWN;
        }

        goto REFRESH_INI_FILE_CHG_END;
    }

    //refer to specification, refresh should do initialization precedure after chv verification
#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        //read the EFphase
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PHASE, &phase_mapping);

        if(ser_result != SIM_SERVICE_OK)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::Read EFphase failed in Refresh Ini!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5715_112_2_17_23_0_3_4698, (uint8 *)"");
#endif

            if(IsFatalError(ser_result))
            {
                ind->result = SIM_INI_ERROR_CARD_OUT;
                goto REFRESH_INI_FILE_CHG_END;
            }
        }
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        // 3G ??óD???t
        phase_mapping.phase = SIM_PH_2_PD;//SIM_PH_2?
    }

#endif
    //get the sim card phase
    g_sim_card_setting[cur_sim_task].card_phase = phase_mapping.phase;

    //read the EFad
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_AD, &(ind->ad_mapping));

    if(ser_result == SIM_SERVICE_OK)
    {
        SaveMncLenInfo(cur_sim_task, &(ind->ad_mapping));
    }
    else
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::Read EFad failed in SIM initialization!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5738_112_2_17_23_0_3_4699, (uint8 *)"");
#endif

        if(IsFatalError(ser_result))
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
            goto REFRESH_INI_FILE_CHG_END;
        }
    }

    ini_result = Init_GetSimFileTable(cur_sim_task, g_sim_card_setting[cur_sim_task].card_phase, &(ind->ini_info), TRUE);

    if(SIM_INI_OK == ini_result)
    {
#ifdef SIM_3G_USIM_SUPPORT
        SIMDN_GetPBRInfo(cur_sim_task);
#endif

        //BDN capability check
        ind->is_bdn_enable = SIMDN_BDNCapabilityCheck(cur_sim_task);

        //FDN capability check
        ind->is_fdn_enable = SIMDN_FDNCapabilityCheck(cur_sim_task);

        //SCI_TRACE_LOW:"SIM::SIM ini file change success!FDN %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5752_112_2_17_23_0_4_4700, (uint8 *)"d", ind->is_fdn_enable);
        ind->result = SIM_INI_OK;
        result = TRUE;

        //if the imsi is full 0xff
#ifdef GSM_CUSTOMER_AFP_SUPPORT

        if(((memcmp(imsi_is_ff_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0)
            || (memcmp(imsi_is_00_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0))
            && (!ind->ini_info.imsi_mapping.imsi.valid_flag))
#else
        if((memcmp(imsi_is_ff_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0)
            && (ind->ini_info.imsi_mapping.imsi.length == 0xff)
            && (!ind->ini_info.imsi_mapping.imsi.valid_flag))
#endif
        {
            //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIniFileChg imsi is full 0xff."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5761_112_2_17_23_0_4_4701, (uint8 *)"");
            ind->result = SIM_INI_ERROR_IMSI_FULL_FF;
            result = FALSE;
        }

        //then to get the refresh file info
        ind->get_file_info_result = TRUE;
        ind->file_num = psig->file_num;

        for(i = 0; i < file_num; i++)
        {
            //the file must be a ef file
            if(SIMFS_GetFileType(psig->file_arr[i]) != SIM_FILE_T_EF)
            {
                ind->get_file_info_result = FALSE;
                break;
            }

            ind->file_info[i].file_name = psig->file_arr[i];
            file_structure = SIMFS_GetEFStructure(psig->file_arr[i]);

            //if the file is record file,then to get the record number
            if((SIM_EF_S_LINEAR_FIXED == file_structure) ||
                (SIM_EF_S_CYCLIC == file_structure))
            {
                ser_result = SIMSER_SelectFile(cur_sim_task, psig->file_arr[i], &select_data);

                if(SIM_SERVICE_OK != ser_result)
                {
                    ind->get_file_info_result = FALSE;
                    break;
                }

                ind->file_info[i].record_num = (uint8)(select_data.freespace_filesize / select_data.dfnumber_recordlen);
            }
            else
            {
                ind->file_info[i].record_num = 1;
            }
        }
    }
    else
    {
        if(SIM_INI_ERROR_CARD_OUT == ini_result)
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
        }
        else
        {
            ind->result = SIM_INI_ERROR_UNKNOWN;
        }
    }

REFRESH_INI_FILE_CHG_END:
    //send confirm signal to the siganl sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)ind, ((xSignalHeaderRec *)psig)->Sender);

    return result;
}

/**********************************************************************
//    Description:
//      the function that proccess the stk refresh signal,the refresh type is sim initialization and full file change
//      GSM 11.11 section 11.2.1 SIM initialization
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN SIMSIGNAL_RefreshIniFull(    //return value:the initialization succeed or not
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_INI_FULL_REQ_T *psig  //in:the initialization request signal
)
{
    BOOLEAN result = FALSE;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_REFRESH_INI_FULL_CNF_T *ind;
    uint8 imsi_is_ff_buf[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#ifdef GSM_CUSTOMER_AFP_SUPPORT
    uint8 imsi_is_00_buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif
    SIM_RESULT_INITIALIZE_SIM_E ini_result = SIM_INI_OK;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_EF_MAPPING_PHASE_T phase_mapping;
    SIM_EF_MAPPING_ICCID_T tmp_iccid;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM::Refresh Ini Full begin!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5831_112_2_17_23_0_4_4702, (uint8 *)"");
#endif
    //creat the initialize confirm signal of the request signal
    SCI_CREATE_SIGNAL(ind, SIM_REFRESH_INI_FULL_CNF, (uint16)(sizeof(SIM_SIG_REFRESH_INI_FULL_CNF_T)), g_sim_task_id[cur_sim_task]);
    ind->command_ref = psig->command_ref;

    //Read EF_ICCID for later user
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ICCID, &tmp_iccid);

    if(SIM_SERVICE_OK != ser_result)
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIniFull Read EFiccid failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5842_112_2_17_23_0_4_4703, (uint8 *)"");
#endif

        if(IsFatalError(ser_result))
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
            goto REFRESH_INI_FULL_END;
        }
    }

    //then select DFgsm,for profile download
    ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIniFull select DFgsm failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5849_112_2_17_23_0_4_4704, (uint8 *)"");

        if(IsFatalError(ser_result))
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
        }
        else
        {
            ind->result = SIM_INI_ERROR_UNKNOWN;
        }

        goto REFRESH_INI_FULL_END;
    }

    //refer to specification, refresh should do initialization precedure after chv verification
#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        //read the EFphase
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PHASE, &phase_mapping);

        if(ser_result != SIM_SERVICE_OK)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM::Read EFphase failed in Refresh Ini!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5864_112_2_17_23_0_4_4705, (uint8 *)"");
#endif

            if(IsFatalError(ser_result))
            {
                ind->result = SIM_INI_ERROR_CARD_OUT;
                goto REFRESH_INI_FULL_END;
            }
        }
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        // 3G ??óD???t
        phase_mapping.phase = SIM_PH_2_PD;//SIM_PH_2?
    }

#endif
    //get the sim card phase
    g_sim_card_setting[cur_sim_task].card_phase = phase_mapping.phase;

    //read the EFad
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_AD, &(ind->ad_mapping));

    if(ser_result == SIM_SERVICE_OK)
    {
        SaveMncLenInfo(cur_sim_task, &(ind->ad_mapping));
    }
    else
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::Read EFad failed in SIM initialization!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5887_112_2_17_23_0_4_4706, (uint8 *)"");
#endif

        if(IsFatalError(ser_result))
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
            goto REFRESH_INI_FULL_END;
        }
    }

    ini_result = Init_GetSimFileTable(cur_sim_task, g_sim_card_setting[cur_sim_task].card_phase, &(ind->ini_info), TRUE);

    if(SIM_INI_OK == ini_result)
    {
#ifdef SIM_3G_USIM_SUPPORT
        SIMDN_GetPBRInfo(cur_sim_task);
#endif

        //BDN capability check
        ind->is_bdn_enable = SIMDN_BDNCapabilityCheck(cur_sim_task);

        //FDN capability check
        ind->is_fdn_enable = SIMDN_FDNCapabilityCheck(cur_sim_task);

        //SCI_TRACE_LOW:"SIM::SIM Refresh Ini Full success!FDN %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5902_112_2_17_23_0_4_4707, (uint8 *)"d", ind->is_fdn_enable);
        ind->result = SIM_INI_OK;
        result = TRUE;

        //if the imsi is full 0xff
#ifdef GSM_CUSTOMER_AFP_SUPPORT

        if(((memcmp(imsi_is_ff_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0)
            || (memcmp(imsi_is_00_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0))
            && (!ind->ini_info.imsi_mapping.imsi.valid_flag))
#else
        if((memcmp(imsi_is_ff_buf, (ind->ini_info.imsi_mapping.imsi.imsi_value), 8) == 0)
            && (ind->ini_info.imsi_mapping.imsi.length == 0xff)
            && (!ind->ini_info.imsi_mapping.imsi.valid_flag))
#endif
        {
            //SCI_TRACE_LOW:"SIM::SIMSIGNAL_RefreshIniFull imsi is full 0xff."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5911_112_2_17_23_0_4_4708, (uint8 *)"");
            ind->result = SIM_INI_ERROR_IMSI_FULL_FF;
            result = FALSE;
        }
    }
    else
    {
        if(SIM_INI_ERROR_CARD_OUT == ini_result)
        {
            ind->result = SIM_INI_ERROR_CARD_OUT;
        }
        else
        {
            ind->result = SIM_INI_ERROR_UNKNOWN;
        }
    }

REFRESH_INI_FULL_END:
    //send confirm signal to the siganl sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)ind, ((xSignalHeaderRec *)psig)->Sender);

    return result;
}

#ifdef GSM_DPHONE_VER2_SUPPORT
/**********************************************************************
//    Description:
//      the function that proccess all the stk refresh signal in the Dphone
//    initialization pending state
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_DphoneRefreshskip(uint32 cur_sim_task, uint32 pid_sender)
{
    SIM_SIG_DPHONE_REFRESH_SKIP_CNF_T *cnf;

    //creat the file change confirm signal of the request signal
    SCI_CREATE_SIGNAL(cnf, SIM_DPHONE_REFRESH_SKIP_CNF, sizeof(SIM_SIG_REFRESH_FILE_CHG_CNF_T), g_sim_task_id[cur_sim_task]);
    cnf->command_ref = 0;

    SCI_TRACE_LOW("SIM%d::Dphone refresh skip.", cur_sim_task);
    //send confirm signal to the siganl sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)cnf, pid_sender);
}
#endif

/**********************************************************************
//    Description:
//      the function that process the refresh command
//      read the Transparent file read request signal
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_RefreshReadFile(
    uint32 cur_sim_task,
    SIM_SIG_REFRESH_READ_FILE_REQ_T *psig //in: the read request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SIG_REFRESH_READ_FILE_CNF_T *pcnf;
    SIM_EF_STRUCTURE_E ef_structure;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM::Refresh read File!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_5941_112_2_17_23_0_4_4709, (uint8 *)"");
#endif

    //create the confirm signal
    SCI_CREATE_SIGNAL(pcnf, SIM_REFRESH_READ_FILE_CNF, (uint16)(sizeof(SIM_SIG_REFRESH_READ_FILE_CNF_T)), g_sim_task_id[cur_sim_task]);
    //get info for the confirm signal from request signal
    pcnf->command_ref = psig->command_ref;
    pcnf->file_name = psig->file_name;
    pcnf->record_id = psig->record_id;

    pcnf->result = SIM_READ_OK;

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(psig->file_name))
    {
        pcnf->result = SIM_READ_INVALIDE_FILE;
    }
    else
    {
        if(SIMFS_GetFileType(psig->file_name) != SIM_FILE_T_EF)
        {
            pcnf->result = SIM_READ_INCONSISTENT_FILE;
        }
        else
        {
            ef_structure = SIMFS_GetEFStructure(psig->file_name);

            //the ef file structure must be transparent file
            if(SIM_EF_S_TRANSPARENT == ef_structure)
            {
                if((SIM_FILE_EF_LP == psig->file_name)
                    || (SIM_FILE_EF_ELP == psig->file_name)
#ifdef SIM_3G_USIM_SUPPORT
                    || (SIM_FILE_EF_USIM_LI == psig->file_name)
#endif
                  )
                {
                    //我将language相关EF打包，所以要特别处理.
                    ser_result = SIMSIGNAL_GetSIMLanguageInfo(cur_sim_task, &(pcnf->buf.language));
                }
                else
                {
                    //read the transparent EF file
                    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, psig->file_name, &(pcnf->buf.ad));

                    if(ser_result == SIM_SERVICE_OK)
                    {
                        if(psig->file_name == SIM_FILE_EF_SST)
                        {
                            SCI_TRACE_LOW("SIM%d:refresh changed file SST",cur_sim_task);
                            //save the sim service table for laterly use
                            SCI_MEMCPY(&g_sim_card_setting[cur_sim_task].service_table, &(pcnf->buf.sst), sizeof(SIM_EF_MAPPING_SST_T));
                        }
                    }
                }
            }
            else
            {
                //read the record file
                ser_result = SIMSER_ReadEFRecord(cur_sim_task, psig->file_name,
                                                 psig->record_id, SIM_RECORD_ABSOLUTE, &(pcnf->buf.ad));
            }

            //get the read error reason
            if(SIM_SERVICE_OK != ser_result)
            {
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
#endif
                pcnf->result = AnalysisReadFileFail(ser_result);
            }
        }
    }

    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

#ifdef SIM_AOC_SUPPORT
/**********************************************************************
//    Description:
//      the function that read the EFacm,EFacmmax and EFpuct
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E ReadAocContext(
    uint32 cur_sim_task,
    SIM_AOC_CONTEXT_T *aoc_context,
    SIM_AOC_CHV_SELECT *aoc_chv_select
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_ACM_T acm;
    SIM_EF_MAPPING_ACMMAX_T acm_max;
    SIM_EF_MAPPING_PUCT_T puct;
    BOOLEAN chvacm = TRUE, chvacmmax = TRUE, chvpuct = TRUE;
#ifdef _CPHS_SUPPORT
    SIM_EF_MAPPING_ACM_T acm_line2;
#endif

    //initialize variable
    SCI_MEMSET(&acm, 0, sizeof(SIM_EF_MAPPING_ACM_T));
    SCI_MEMSET(&acm_max, 0, sizeof(SIM_EF_MAPPING_ACMMAX_T));
    SCI_MEMSET(&puct, 0, sizeof(SIM_EF_MAPPING_PUCT_T));
    aoc_context->AOC_is_support = TRUE;

    //First:read the first record of EF_ACM;
    chvacm = SIM_GetAOCAccessCondition(cur_sim_task, SIM_FILE_EF_ACM);
    ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_ACM, 1, SIM_RECORD_ABSOLUTE, &acm);

    if((SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result) ||
        (SIM_SERVICE_OK != ser_result))
    {
        aoc_context->AOC_is_support = FALSE;

        if(SIM_SERVICE_CARD_OUT == ser_result)
        {
            return ser_result;
        }
    }
    else
    {
        //some sim card set acm_value & acm_max_value with 0xFFFFFF as default value, and AOC_is_support = TRUE
        //this will cause l4 do not allow mo call. Add exception handle here
        //SCI_TRACE_LOW:"SIM%d:acm value = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6051_112_2_17_23_0_4_4710, (uint8 *)"dd", cur_sim_task, acm.acm_value);

        if(acm.acm_value == 0xFFFFFF)
        {
            acm.acm_value = 0;
        }
    }

    //Second:read the EF_ACMmax
    chvacmmax = SIM_GetAOCAccessCondition(cur_sim_task, SIM_FILE_EF_ACMmax);
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ACMmax, &acm_max);
    g_sim_acm_max[cur_sim_task] = acm_max;

    if((SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result) ||
        (SIM_SERVICE_OK != ser_result))
    {
        aoc_context->AOC_is_support = FALSE;

        if(SIM_SERVICE_CARD_OUT == ser_result)
        {
            return ser_result;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"SIM%d::acm max is %x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6073_112_2_17_23_0_4_4711, (uint8 *)"dd", cur_sim_task, acm_max.acm_max_value);
    }

    //Third:read the EF_puct  /*the read EF_puct is added by king.li for cr90738*/
    chvpuct = SIM_GetAOCAccessCondition(cur_sim_task, SIM_FILE_EF_PUCT);
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_PUCT, &puct);

    if((SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result) ||
        (SIM_SERVICE_OK != ser_result))
    {
        aoc_context->AOC_is_support = FALSE;

        if(SIM_SERVICE_CARD_OUT == ser_result)
        {
            return ser_result;
        }
    }

#ifdef _CPHS_SUPPORT

    if(s_sim_df_orange_exist[cur_sim_task])
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ORANGE_ACM2, &acm_line2);

        if(SIM_SERVICE_OK != ser_result)
        {
            acm_line2.acm_value = 0;
        }
    }
    else
    {
        acm_line2.acm_value = 0;
    }

#endif

    aoc_chv_select->chv_acm = chvacm;
    aoc_chv_select->chv_acmmax = chvacmmax;
    aoc_chv_select->chv_puct = chvpuct;
    aoc_context->max_ACM = acm_max.acm_max_value;
    aoc_context->unit_ACM = acm.acm_value;
    SCI_MEMCPY(&(aoc_context->puct), &puct, sizeof(SIM_EF_MAPPING_PUCT_T));
#ifdef _CPHS_SUPPORT
    aoc_context->unit_ACM_line2 = acm_line2.acm_value;
#endif

    return ser_result;
}
#endif

#ifdef MODEM_PLATFORM
/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_CRSM(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T  *psig
)
{
    SIM_SIG_CRSM_CNF_T      *pcnf = PNULL;
    SIM_SW_AND_RESPONSE_T   rsp_ind;
    SIM_EF_MAPPING_DN_T dn_mapping;

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SIG_CRSM_CNF, sizeof(SIM_SIG_CRSM_CNF_T), g_sim_task_id[cur_sim_task]);

    // init
    SCI_MEMSET(&rsp_ind, 0, sizeof(SIM_SW_AND_RESPONSE_T));

    pcnf->file_id = psig->file_id;
    pcnf->result = FALSE;

#ifdef ONTIM_PRJ
#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        //Ontim Project, the AP send CRSM to read EFpbr when sim card.
        uint16 temp_id = psig->file_id;
        temp_id = temp_id & 0xFF00;

        if(0x4F00 == temp_id)
        {
            pcnf->sw1 = 0x94;
            pcnf->sw2 = 0x4;
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return ;
        }
    }

#endif
#endif

    switch(psig->ins)
    {
        case 192:   // GET RESPONSE
            pcnf->result = SIMSIGNAL_CRSM_GetResponse(cur_sim_task, psig, &rsp_ind);
            break;

        case 242:   // STATUS
            pcnf->result = SIMSIGNAL_CRSM_Status(cur_sim_task, psig, &rsp_ind);
            break;

        case 176:   // READ BINARY
            pcnf->result = SIMSIGNAL_CRSM_ReadBinary(cur_sim_task, psig, &rsp_ind);
            break;

        case 178:   // READ RECORD
            pcnf->result = SIMSIGNAL_CRSM_ReadRecord(cur_sim_task, psig, &rsp_ind);
            break;

        case 214:   // UPDATE BINARY
            pcnf->result = SIMSIGNAL_CRSM_UpdateBinary(cur_sim_task, psig, &rsp_ind);
            pcnf->is_update = TRUE;
            break;

        case 220:   // UPDATE RECORD
            pcnf->result = SIMSIGNAL_CRSM_UpdateRecord(cur_sim_task, psig, &rsp_ind);
            pcnf->is_update = TRUE;
            break;

        default:
            break;
    }

    pcnf->sw1 = rsp_ind.sw1;
    pcnf->sw2 = rsp_ind.sw2;
    pcnf->data_len = rsp_ind.data_len;
    SCI_MEMCPY(pcnf->rsp_data, rsp_ind.data, rsp_ind.data_len);

    if(pcnf->result)
    {
        //Some SIM EFs will be changed by AT+CRSM while the L4 donot know...
        //we should notify L4 for some neccesary EFs(FDN, EFfplmn?, EFplmnwact? ...?)
        //to be continue...
        if(214 == psig->ins)
        {
            //some EF just like EFfplmn?
        }
        else if(220 == psig->ins)
        {
            if ((0x6f3a == psig->file_id) || (0x6f3b == psig->file_id))//L4 should update its memory
            {
                SIM_FILE_NAME_E temp_file_name = SIM_FILE_EF_FDN;
                if (0x6f3b == psig->file_id)
                {
                    temp_file_name = SIM_FILE_EF_FDN;
                }
                else// if (0x6f3a == psig->file_id)
                {
                    //But the USIM card's ADN fileid maybe 0x4Fxx...
                    temp_file_name = SIM_FILE_EF_ADN;
                }

                pcnf->dn_record_id = psig->p1;
                if(SIM_SERVICE_OK == SIMSER_ReadEFRecord(cur_sim_task, temp_file_name, psig->p1, SIM_RECORD_ABSOLUTE, &dn_mapping))
                {
                    pcnf->dn_mapping.alpha_id_len = dn_mapping.alpha_id_len;
                    pcnf->dn_mapping.number_len = dn_mapping.dn_len;
                    pcnf->dn_mapping.ccp_id = dn_mapping.ccp_id;
                    pcnf->dn_mapping.is_ccp_exist = FALSE;
                    pcnf->dn_mapping.is_ton_npi_exist = dn_mapping.is_ton_npi_exist;
                    SCI_MEMCPY(pcnf->dn_mapping.alpha_id, dn_mapping.alpha_id, pcnf->dn_mapping.alpha_id_len);
                    SCI_MEMCPY(pcnf->dn_mapping.numbers, dn_mapping.dialling_num, pcnf->dn_mapping.number_len);
                    pcnf->dn_mapping.ton = dn_mapping.ton;
                    pcnf->dn_mapping.npi = dn_mapping.npi;
                }
            }
        }
    }

    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/*****************************************************************************/
//  Description : the function that
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_GetPath(uint32 cur_sim_task, SIM_FILE_NAME_E file_name, uint8 *path_len, uint8 *path_ptr)
{
    SIM_FILE_NAME_E df_name[6] = {SIM_FILE_MF};
    SIM_FILE_NAME_E temp_file_name = file_name;
    BOOLEAN find_flag = FALSE;
    uint16 temp_file_id,i,j = 0;

#ifdef SIM_3G_USIM_SUPPORT
    SIMFS_RemapUSIMFileName(cur_sim_task, file_name, &temp_file_name);
#endif

    for (i=0;i<6;i++)
    {
        df_name[i] = SIMFS_GetParenFileName(temp_file_name);
        if (SIM_FILE_TOTAL_NUM == df_name[i])
        {
            //Invalid DF
            find_flag = FALSE;
            break;//quit the loop.
        }
        else
        {
            if (SIM_FILE_MF == df_name[i])
            {
                find_flag = TRUE;
                break;//end the loop
            }
            temp_file_name = df_name[i];
        }
    }

    //SCI_TRACE_LOW("SIM: %d, i=%d",find_flag,i);

    if (find_flag)
    {
        if (i > 5)
        {
            i = 5;
        }
        *path_len = (i+1)*2;

        for(j=0;j<*path_len;j++)
        {
            temp_file_id = SIMFS_GetFileID(cur_sim_task, df_name[i]);

            //SCI_TRACE_LOW("SIM: xxxx %x %d, i=%d",temp_file_id,df_name[i],i);

            if (0xffff == temp_file_id)
            {
                find_flag = FALSE;
                break;
            }
            else
            {
                path_ptr[j] = (uint8)((temp_file_id & 0xFF00)>>8);
                j++;
                if (j >= SIM_PATH_LEVEL)
                {
                    find_flag = FALSE;
                    break;
                }
                path_ptr[j] = (uint8)(temp_file_id & 0xFF);
            }

            //SCI_TRACE_LOW("SIM: yyyy j=%d, 0x%x 0x%x",j,path_ptr[j-1],path_ptr[j]);
            if (0 == i)
            {
                break;
            }
            else
            {
                i--;
            }
        }
    }

    if (!find_flag)
    {
        *path_len = 0;//0
    }

    return find_flag;
}

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_GetResponse(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
)
{
    BOOLEAN                         result    = FALSE;
    SIM_FILE_NAME_E                 file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_id);
    SIM_RSP_INFO_T                  rsp_info;
    SIM_RESPONSE_DATA_OF_SELECT_T   select_data;
    uint8 i, tmp_path_len = 0;
    uint16 path_id[6] = {0};

    SCI_MEMSET(rsp_ptr, 0, sizeof(SIM_SW_AND_RESPONSE_T));

    if(SIM_FILE_TOTAL_NUM == file_name)
    {
        if((0x7000 == (psig->file_id & 0xF000))
            || (0x6000 == (psig->file_id & 0xF000))
            || (0x5000 == (psig->file_id & 0xF000))
            || (0x4000 == (psig->file_id & 0xF000))
            || (0x3F00 == psig->file_id)
            || (0x2000 == (psig->file_id & 0xF000)))
        {
            if(psig->path_len > 10)
            {
                tmp_path_len = 10;
            }
            else
            {
                tmp_path_len = psig->path_len;
            }

            for(i = 0; i < tmp_path_len; i += 2)
            {
                path_id[i/2] = (uint16)((((uint16)(psig->path_id[i])) << 8) | (psig->path_id[i+1]));
            }

            if(SIM_SERVICE_OK == SIMINSTR_SelectFileByPath(cur_sim_task, psig->file_id,
                                                           (psig->path_len) / 2, path_id, &select_data))
            {
                SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
                SCI_MEMCPY(rsp_ptr, &(rsp_info.sw_rsp), sizeof(SIM_SW_AND_RESPONSE_T));
                result = TRUE;
            }
            else
            {
                rsp_ptr->sw1 = 0x94;
                rsp_ptr->sw2 = 0x04;
            }
        }
        else
        {
            // file ID not fount
            rsp_ptr->sw1 = 0x94;
            rsp_ptr->sw2 = 0x04;
        }
    }
    else
    {
        SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);

        switch(rsp_info.ins_name)
        {
                //AP CRSM do not support commands below, so in my opinion GET RESPONSE should only be related with
                //select or status, so if previous command is select and the name is same, we return response directly.
            case SIMINSTR_SELECT:

                if(rsp_info.file_name == file_name)
                {
                    SCI_MEMCPY(rsp_ptr, &(rsp_info.sw_rsp), sizeof(SIM_SW_AND_RESPONSE_T));
                    result = TRUE;
                }

                break;

            case SIMINSTR_SEEK:
            case SIMINSTR_INCREASE:
            case SIMINSTR_RUN_ALGORITHM:
            case SIMINSTR_ENVELOPE:
            default:
                break;
        }

        if(!result)
        {
            if(SIM_SERVICE_OK == SIMSER_SelectFile(cur_sim_task, file_name, &select_data))
            {
                SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
                SCI_MEMCPY(rsp_ptr, &(rsp_info.sw_rsp), sizeof(SIM_SW_AND_RESPONSE_T));
                result = TRUE;
            }
            else
            {
                rsp_ptr->sw1 = 0x94;
                rsp_ptr->sw2 = 0x04;
            }
        }
    }

#ifdef SIM_3G_USIM_SUPPORT
#ifdef SIM_3G_FORMAT_TRANS

    //convert response data from 3G format to 2G
    if((result) && (USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]))
    {
        if(!SIMINSTR_3GResponseTo2GFormat(cur_sim_task, file_name, rsp_ptr->data, &(rsp_ptr->data_len)))
        {
            rsp_ptr->data_len = 0;
            rsp_ptr->sw1 = 0x94;
            rsp_ptr->sw2 = 0x04;
        }

        SCI_TRACE_LOW("SIM::RSP after 3G->2G transmition: len %d, data: %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,",
                      rsp_ptr->data_len, rsp_ptr->data[0], rsp_ptr->data[1], rsp_ptr->data[2], rsp_ptr->data[3], rsp_ptr->data[4], rsp_ptr->data[5], rsp_ptr->data[6], rsp_ptr->data[7],
                      rsp_ptr->data[8], rsp_ptr->data[9], rsp_ptr->data[10], rsp_ptr->data[11], rsp_ptr->data[12], rsp_ptr->data[13], rsp_ptr->data[14], rsp_ptr->data[15],
                      rsp_ptr->data[16], rsp_ptr->data[17], rsp_ptr->data[18], rsp_ptr->data[19], rsp_ptr->data[20], rsp_ptr->data[21], rsp_ptr->data[22], rsp_ptr->data[23]);
    }

#endif
#endif

    return (result);
}

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_Status(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
)
{
    BOOLEAN                         result    = FALSE;
    SIM_RSP_INFO_T                  rsp_info = {0};
    SIM_RESPONSE_DATA_OF_SELECT_T   select_data;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    BOOLEAN fetch_flag = FALSE;

    SCI_MEMSET(rsp_ptr, 0, sizeof(SIM_SW_AND_RESPONSE_T));

    ser_result = SIMINSTR_Status(cur_sim_task, &select_data, SIM_SELECT_DF_MANDATORY_LEN, &fetch_flag);

    if((SIM_SERVICE_RX_PARITY_ERROR == ser_result)
        || (SIM_SERVICE_TX_PARITY_ERROR == ser_result)
        || (SIM_SERVICE_CARD_OUT == ser_result)
        || (SIM_SERVICE_CARD_UNRESPONSE == ser_result))
    {
        rsp_ptr->data_len = 0;
        rsp_ptr->sw1 = 0x6F;
        rsp_ptr->sw2 = 0x0;
    }
    else
    {
        SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
        SCI_MEMCPY(rsp_ptr, &(rsp_info.sw_rsp), sizeof(SIM_SW_AND_RESPONSE_T));
        result = TRUE;
    }

#ifdef SIM_3G_USIM_SUPPORT
#ifdef SIM_3G_FORMAT_TRANS

    //convert response data from 3G format to 2G
    if((result) && (USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]))
    {
        if(!SIMINSTR_3GResponseTo2GFormat(cur_sim_task, rsp_info.file_name, rsp_ptr->data, &(rsp_ptr->data_len)))
        {
            rsp_ptr->data_len = 0;
            rsp_ptr->sw1 = 0x94;
            rsp_ptr->sw2 = 0x04;
        }
    }

#endif
#endif

    return (result);
}

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_ReadBinary(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    BOOLEAN         result      = FALSE;
    SIM_FILE_NAME_E file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_id);
    SIM_RSP_INFO_T  rsp_info;
    uint16          offset      = 0;
    uint16          data_len    = 0;
    uint8           data_buf[256] = {0};
    uint8 temp_path_len = 0;
    uint8 temp_path_id[SIM_PATH_LEVEL] = {0};

    SCI_MEMSET(rsp_ptr, 0, sizeof(SIM_SW_AND_RESPONSE_T));

    if((0x6000 == (psig->file_id & 0xF000))
        || (0x4000 == (psig->file_id & 0xF000))
        || (0x2000 == (psig->file_id & 0xF000)))
    {
        //must be a EF
        if(SIMFS_GetFileType(file_name) != SIM_FILE_T_EF)
        {
            rsp_ptr->sw1 = 0x94;
            rsp_ptr->sw2 = 0x04;
            return result;
        }

        //the ef file structure must be transparent file
        if(SIMFS_GetEFStructure(file_name) != SIM_EF_S_TRANSPARENT)
        {
            rsp_ptr->sw1 = 0x94;
            rsp_ptr->sw2 = 0x04;
            return result;
        }


        //Valid File id, but no path in CRSM. 
        if ((SIM_FILE_TOTAL_NUM != file_name) && (0 == psig->path_len))
        {
            SIMSIGNAL_CRSM_GetPath(cur_sim_task,file_name,&temp_path_len,temp_path_id);
        }
        else
        {
            temp_path_len = MIN(SIM_PATH_LEVEL,psig->path_len);
            SCI_MEMCPY(temp_path_id,psig->path_id,temp_path_len);
        }

        offset = ((psig->p1) << 8) + (psig->p2);
        ser_result =  SIMINSTR_ImiReadEF(cur_sim_task, psig->file_id,
                                         temp_path_len,
                                         temp_path_id,
                                         offset,
                                         0,
                                         SIM_RECORD_ABSOLUTE,
                                         psig->p3,
                                         &data_len,
                                         data_buf);

        if(SIM_SERVICE_OK == ser_result)
        {
            SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
            rsp_ptr->sw1        = rsp_info.sw_rsp.sw1;
            rsp_ptr->sw2        = rsp_info.sw_rsp.sw2;
            rsp_ptr->data_len   = MIN(data_len, psig->p3);

            if(0 < rsp_ptr->data_len)
            {
                SCI_MEMCPY(rsp_ptr->data, data_buf, rsp_ptr->data_len);
            }

            result = TRUE;
        }
        else
        {
            rsp_ptr->data_len = 0;

            if((SIM_SERVICE_RX_PARITY_ERROR == ser_result)
                || (SIM_SERVICE_TX_PARITY_ERROR == ser_result)
                || (SIM_SERVICE_CARD_OUT == ser_result)
                || (SIM_SERVICE_CARD_UNRESPONSE == ser_result))
            {
                rsp_ptr->sw1 = 0x6F;
                rsp_ptr->sw2 = 0x0;
            }
            else
            {
                SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
                rsp_ptr->sw1 = rsp_info.sw_rsp.sw1;
                rsp_ptr->sw2 = rsp_info.sw_rsp.sw2;
            }
        }
    }
    else
    {
        // file ID not fount
        rsp_ptr->sw1 = 0x94;
        rsp_ptr->sw2 = 0x04;
    }

    return (result);
}

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_ReadRecord(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;
    BOOLEAN         result = FALSE;
    SIM_RSP_INFO_T  rsp_info;
    uint16          data_len    = 0;
    uint8           data_buf[256] = {0};
    SIM_FILE_NAME_E file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_id);
    uint8 temp_path_len = 0;
    uint8 temp_path_id[SIM_PATH_LEVEL] = {0};

    if(!SIMSIGNAL_CheckRecordEFInServiceTable(cur_sim_task, file_name))//Beijing TDUSIM access PNN lead to error.
    {
        rsp_ptr->sw1 = 0x94;
        rsp_ptr->sw2 = 0x04;
        rsp_ptr->data_len = 0;
        return (result);
    }

    SCI_MEMSET(rsp_ptr, 0, sizeof(SIM_SW_AND_RESPONSE_T));

    if((0x6000 == (psig->file_id & 0xF000))
        || (0x4000 == (psig->file_id & 0xF000))
        || (0x2000 == (psig->file_id & 0xF000)))
    {

        //Valid File id, but no path in CRSM. 
        if ((SIM_FILE_TOTAL_NUM != file_name) && (0 == psig->path_len))
        {
            SIMSIGNAL_CRSM_GetPath(cur_sim_task,file_name,&temp_path_len,temp_path_id);
        }
        else
        {
            temp_path_len = MIN(SIM_PATH_LEVEL,psig->path_len);
            SCI_MEMCPY(temp_path_id,psig->path_id,temp_path_len);
        }

        ser_result =  SIMINSTR_ImiReadEF(cur_sim_task, psig->file_id, temp_path_len, temp_path_id, 0,
                                         psig->p1, (SIM_RECORD_ACCESS_MODE_E)psig->p2, psig->p3, &data_len, data_buf);
    }

    if(SIM_SERVICE_OK == ser_result)
    {
        SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
        rsp_ptr->sw1        = rsp_info.sw_rsp.sw1;
        rsp_ptr->sw2        = rsp_info.sw_rsp.sw2;
        rsp_ptr->data_len   = data_len;     // the length of the record

        if(0 < rsp_ptr->data_len)
        {
            SCI_MEMCPY(rsp_ptr->data, data_buf, rsp_ptr->data_len);
        }

        result = TRUE;
    }
    else
    {
        rsp_ptr->data_len = 0;

        if((SIM_SERVICE_RX_PARITY_ERROR == ser_result)
            || (SIM_SERVICE_TX_PARITY_ERROR == ser_result)
            || (SIM_SERVICE_CARD_OUT == ser_result)
            || (SIM_SERVICE_CARD_UNRESPONSE == ser_result))
        {
            rsp_ptr->sw1 = 0x6F;
            rsp_ptr->sw2 = 0x0;
        }
        else
        {
            SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
            rsp_ptr->sw1 = rsp_info.sw_rsp.sw1;
            rsp_ptr->sw2 = rsp_info.sw_rsp.sw2;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_UpdateBinary(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;
    BOOLEAN         result      = FALSE;
    SIM_FILE_NAME_E file_name   = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_id);
    SIM_RSP_INFO_T  rsp_info;
    uint16          offset      = 0;

    SCI_MEMSET(rsp_ptr, 0, sizeof(SIM_SW_AND_RESPONSE_T));

    if(SIM_FILE_TOTAL_NUM == file_name)
    {
        if((0x6000 == (psig->file_id & 0xF000))
            || (0x4000 == (psig->file_id & 0xF000)))
        {
            offset = psig->p1;
            offset <<= 8;
            offset |= psig->p2;
            ser_result =  SIMINSTR_ImiUpdateEF(cur_sim_task, psig->file_id, psig->path_len, psig->path_id,
                                               offset, 0, SIM_RECORD_ABSOLUTE, psig->p3, psig->data_ptr);
        }
        else
        {
            // file ID not fount
            rsp_ptr->sw1 = 0x94;
            rsp_ptr->sw2 = 0x04;
        }
    }
    else
    {
        offset = psig->p1;
        offset <<= 8;
        offset |= psig->p2;
        ser_result = SIMINSTR_UpdateEF(cur_sim_task, file_name, offset, 0, SIM_RECORD_ABSOLUTE, psig->p3, psig->data_ptr);
    }

    if(SIM_SERVICE_OK == ser_result)
    {
        SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
        SCI_MEMCPY(rsp_ptr, &(rsp_info.sw_rsp), sizeof(SIM_SW_AND_RESPONSE_T));
        result = TRUE;
    }
    else
    {
        rsp_ptr->data_len = 0;

        if((SIM_SERVICE_RX_PARITY_ERROR == ser_result)
            || (SIM_SERVICE_TX_PARITY_ERROR == ser_result)
            || (SIM_SERVICE_CARD_OUT == ser_result)
            || (SIM_SERVICE_CARD_UNRESPONSE == ser_result))
        {
            rsp_ptr->sw1 = 0x6F;
            rsp_ptr->sw2 = 0x0;
        }
        else
        {
            SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
            rsp_ptr->sw1 = rsp_info.sw_rsp.sw1;
            rsp_ptr->sw2 = rsp_info.sw_rsp.sw2;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : the function that process the Restricted SIM access
//  Global resource dependence : none
//  Author: Tracy.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SIMSIGNAL_CRSM_UpdateRecord(
    uint32 cur_sim_task,
    SIM_SIG_CRSM_REQ_T       *psig,      //IN:
    SIM_SW_AND_RESPONSE_T    *rsp_ptr    //OUT:
)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;
    BOOLEAN         result      = FALSE;
    SIM_FILE_NAME_E file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_id);
    SIM_RSP_INFO_T  rsp_info;
    SIM_RECORD_ACCESS_MODE_E access_mode = SIM_RECORD_ABSOLUTE;

    if(!SIMSIGNAL_CheckRecordEFInServiceTable(cur_sim_task, file_name))
    {
        rsp_ptr->sw1 = 0x94;
        rsp_ptr->sw2 = 0x04;
        rsp_ptr->data_len = 0;
        return (result);
    }

    SCI_MEMSET(rsp_ptr, 0, sizeof(SIM_SW_AND_RESPONSE_T));

    if(SIM_FILE_TOTAL_NUM == file_name)
    {
        if((0x6000 == (psig->file_id & 0xF000))
            || (0x4000 == (psig->file_id & 0xF000)))
        {
            ser_result =  SIMINSTR_ImiUpdateEF(cur_sim_task, psig->file_id, psig->path_len, psig->path_id,
                                               0, psig->p1, (SIM_RECORD_ACCESS_MODE_E)psig->p2, psig->p3, psig->data_ptr);
        }
        else
        {
            // file ID not fount
            rsp_ptr->sw1 = 0x94;
            rsp_ptr->sw2 = 0x04;
        }
    }
    else
    {
        if((0x2 == psig->p2) || (0x3 == psig->p2) || (0x4 == psig->p2))
        {
            access_mode = (SIM_RECORD_ACCESS_MODE_E)psig->p2;
            ser_result = SIMINSTR_UpdateEF(cur_sim_task, file_name, 0, psig->p1, access_mode, psig->p3, psig->data_ptr);
        }
        else
        {
            rsp_ptr->sw1 = 0x6B;//incorrect p1 or p2
            rsp_ptr->sw2 = 0x00;
            return FALSE;//need to return directly.
        }
    }

    if(SIM_SERVICE_OK == ser_result)
    {
        SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
        SCI_MEMCPY(rsp_ptr, &(rsp_info.sw_rsp), sizeof(SIM_SW_AND_RESPONSE_T));
        result = TRUE;
    }
    else
    {
        rsp_ptr->data_len = 0;

        if((SIM_SERVICE_RX_PARITY_ERROR == ser_result)
            || (SIM_SERVICE_TX_PARITY_ERROR == ser_result)
            || (SIM_SERVICE_CARD_OUT == ser_result)
            || (SIM_SERVICE_CARD_UNRESPONSE == ser_result))
        {
            rsp_ptr->sw1 = 0x6F;
            rsp_ptr->sw2 = 0x0;
        }
        else
        {
            SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);
            rsp_ptr->sw1 = rsp_info.sw_rsp.sw1;
            rsp_ptr->sw2 = rsp_info.sw_rsp.sw2;
        }
    }

    return (result);
}


/**********************************************************************
//    Description:
//      Envelope the data from the SIM application toolkit to the SIM card(for at cmd)
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSIGNAL_ATCpduEnvelope(
    uint32 cur_sim_task,
    SIMAT_SIG_AT_CPDU_ENVELOPE_REQ_T *psig //the service request signal
)
{
    SIMAT_SIG_AT_CPDU_ENVELOPE_CNF_T *pcnf;
    SCI_ASSERT(NULL != psig); /*assert verified check NULL pointer*/

    SCI_CREATE_SIGNAL(pcnf, SIMAT_ENVELOPE_CNF, sizeof(SIMAT_SIG_AT_CPDU_ENVELOPE_CNF_T), g_sim_task_id[cur_sim_task]);

    if(g_sim_card_state[cur_sim_task] < SIM_INITIALIZE_STATE)
    {
        pcnf->result = SIMAT_EN_CARD_NOT_READY;
    }
    else if(SIM_PH_2_PD != g_sim_card_setting[cur_sim_task].card_phase)
    {
        pcnf->result = SIMAT_EN_CARD_NOT_SUPPORT;
    }
    else
    {
        if(SIMINSTR_Envelope(cur_sim_task, &(psig->envelope_data), &(pcnf->response), &(pcnf->result), psig->delay_fetch,&(pcnf->sim_sw1),&(pcnf->sim_sw2)) != SIM_SERVICE_OK)
        {
            //SCI_TRACE_LOW:"SIM::Envelope failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6560_112_2_17_23_0_5_4712, (uint8 *)"");
        }
    }

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

/********************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
void SIMSIGNAL_HotSwap_ClearStaticVariable(uint32 cur_sim_task)
{
#ifdef SIM_AOC_SUPPORT
    g_sim_acm_max[cur_sim_task].acm_max_value = 0;
#endif

#ifdef SIM_3G_USIM_SUPPORT
    s_sim_usim_ecc_alpha_id_len[cur_sim_task] = 0;
#endif

    SCI_MEMSET(&(s_sim_chv1[cur_sim_task]), 0xFF, sizeof(SIM_CHV_T));
    SCI_MEMSET(&(s_sim_unblock_chv1[cur_sim_task]), 0xFF, sizeof(SIM_CHV_T));
    s_sim_need_check_ext1_flag[cur_sim_task] = TRUE;
    s_sim_need_check_ext2_flag[cur_sim_task] = TRUE;
    s_sim_need_check_ext3_flag[cur_sim_task] = TRUE;
    s_sim_get_chv1_original_max_times[cur_sim_task] = TRUE;
    s_sim_chv1_original_max_times[cur_sim_task] = SIM_MAX_CHV_ATTEMPT;
#ifdef _CPHS_SUPPORT
    s_sim_df_orange_exist[cur_sim_task] = FALSE;
#ifdef SIM_AOC_SUPPORT    
    s_sim_csp_als_enable[cur_sim_task] = FALSE;
#endif    
#endif
    s_sim_r4_cfis_line1_rec_id[cur_sim_task] = 0;
    s_sim_r4_cfis_line2_rec_id[cur_sim_task] = 0;
    s_sim_r4_is_vmw_info_gotten[cur_sim_task] = FALSE;
}

/********************************************************************************/
//  Description : The service is allocated and activated, or not
//  Global resource dependence :
//  Author:
//  Note:
/********************************************************************************/
BOOLEAN SIMEF_IsServiceEnabled(                                        //RETURN:
    uint32                  cur_sim_task,   //IN:
    SIM_SERVICE_TYPE_PH_2_E service_type    //IN:
)
{
    return ((g_sim_card_setting[cur_sim_task].service_table.service_table[service_type].allocated)
            && (g_sim_card_setting[cur_sim_task].service_table.service_table[service_type].activated));
}

#ifdef SIMAT_ICON_SUPPORT
/**********************************************************************
//    Description:
//      read img data from the sim card
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
BOOLEAN DecodeIidf(
    SIM_IMG_INSTANCE  img_descriptor,
    uint16 img_instance_data_len,
    uint8 *img_instance_buf,
    SIM_EF_MAPPING_IIDF_T *iidf_info)
{
    uint8 *ptemp = NULL;

    if((img_descriptor.offset_of_data + img_descriptor.len_of_data) > img_instance_data_len)
    {
        //SCI_TRACE_LOW:"SIM::the img len is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6608_112_2_17_23_0_5_4713, (uint8 *)"");
        return FALSE;
    }

    if(img_descriptor.len_of_data < 3)
    {
        return FALSE;
    }

    ptemp = img_instance_buf + img_descriptor.offset_of_data;

    //get the image width
    iidf_info->img_width = *ptemp;
    ptemp++;
    //get the image height
    iidf_info->img_height = *ptemp;
    ptemp++;

    //judge coding scheme
    iidf_info->img_cs = img_descriptor.coding_scheme;

    if(img_descriptor.coding_scheme == 0x11)
    {
        //basic image
        iidf_info->img_len = img_descriptor.len_of_data - 2;

        if(iidf_info->img_len > MN_MAX_IMG_DATA_BODY_NUM)
        {
            iidf_info->img_len = MN_MAX_IMG_DATA_BODY_NUM;
        }

        SCI_MEMCPY(&iidf_info->img_body, ptemp, iidf_info->img_len);
    }
    else if(img_descriptor.coding_scheme == 0x21)
    {
        //color image
        iidf_info->bits_per_img_point = *ptemp;

        if((iidf_info->bits_per_img_point == 0) || (iidf_info->bits_per_img_point > 8))
        {
            //SCI_TRACE_LOW:"SIM::the bits per img point is %d,out of range."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6644_112_2_17_23_0_5_4714, (uint8 *)"d", iidf_info->bits_per_img_point);
            return FALSE;
        }

        ptemp++;

        //number of CLUT entries
        iidf_info->num_of_clut_entry = *ptemp;

        //number of CLUT entries shall have a value between 1 and 2**B
        if(iidf_info->num_of_clut_entry > (2 << iidf_info->bits_per_img_point))
        {
            return FALSE;
        }

        //the value 0 shall be interpreted as 256
        if(iidf_info->num_of_clut_entry == 0)
        {
            iidf_info->num_of_clut_entry = 256;
        }

        ptemp++;

        //location of CLUT(color Look-up Table)
        iidf_info->location_clut = *ptemp << 8;
        ptemp++;
        iidf_info->location_clut |= *ptemp;
        ptemp++;

        if(iidf_info->location_clut < 6)
        {
            return FALSE;
        }
        else if((iidf_info->location_clut + (3 * iidf_info->num_of_clut_entry)) > img_instance_data_len)
        {
            return FALSE;
        }
        else
        {
            iidf_info->location_clut  = iidf_info->location_clut - 6;
        }

        //image body
        iidf_info->img_len = img_descriptor.len_of_data - 6 + (3 * iidf_info->num_of_clut_entry);

        if(iidf_info->img_len > MN_MAX_IMG_DATA_BODY_NUM)
        {
            iidf_info->img_len = MN_MAX_IMG_DATA_BODY_NUM;
        }

        SCI_MEMCPY(&iidf_info->img_body, ptemp, iidf_info->img_len);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/**********************************************************************
//    Description:
//      read img data from the sim card
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIMSIGNAL_ReadImg(    //return value:get the card ind success or fail
    uint32 cur_sim_task,
    SIM_SIG_IMG_READ_REQ_T *psig //in:the card present indication
)
{
    SIM_SIG_IMG_READ_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_IMG_T img = {0};
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    uint8 file_path_len = 8;
    uint8 file_path_id[8] = {0};
    uint16  file_id = 0x3f00;
    uint16 img_instance_data_len = 0;                       //the data len readed
    uint8 img_instance_buf[SIM_MAX_FILE_STRUCTURE_LEN]; //the Read file result buf
    BOOLEAN decode_result = FALSE;
#ifdef WIN32
    uint8 basic_image_instance_data[] = {0x08, 0x08, 0xff, 0x03, 0xa5, 0x99, 0x99, 0xa5, 0xc3, 0xff};
    uint8 color_image_instance_data[] =
    {
        0x08,   0x08,   0x02,   0x03,   0x00,   0x16,   0xAA,   0xAA,   0x80,   0x02,   0x85,   0x42,
        0x81,   0x42,   0x81,   0x42,   0x81,   0x52,   0x80,   0x02,   0xAA,   0xAA,   0xFF,   0x00,
        0x00,   0x00,   0xFF,   0x00,   0x00,   0x00,   0xFF
    };
#endif

    //create the confirm signal for the service request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_IMG_READ_CNF, sizeof(SIM_SIG_IMG_READ_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->record_id = psig->record_id;
    pcnf->result = SIM_READ_OK;
    pcnf->is_service_support = FALSE;

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM:begin to read the %d img record."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6737_112_2_17_23_0_5_4715, (uint8 *)"d", psig->record_id);
#endif

    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_IMAGE))
    {
        pcnf->is_service_support = TRUE;
        //read the image from the EFimg
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_IMG, (uint8)(psig->record_id),
                                         SIM_RECORD_ABSOLUTE, &img);
#ifndef WIN32
        SIM_PrintRxBuf(cur_sim_task);
#endif

        if(SIM_SERVICE_OK != ser_result)
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM:read img failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6751_112_2_17_23_0_5_4716, (uint8 *)"");
#endif
            pcnf->result = AnalysisReadFileFail(ser_result);
        }
        else
        {
            pcnf->img_info.img_instance_num = img.num;

#ifndef WIN32

            //read the image instance data file
            if(img.num != 0)
            {
                file_path_id[0] = 0x3f;
                file_path_id[1] = 0x0;
                file_path_id[2] = 0x7f;
                file_path_id[3] = 0x10;
                file_path_id[4] = 0x5f;
                file_path_id[5] = 0x50;
                file_path_id[6] = (uint8)(img.descriptor[0].fileid_of_data >> 8);
                file_path_id[7] = (uint8)(img.descriptor[0].fileid_of_data);
                file_id = img.descriptor[0].fileid_of_data;

                //then begin to read the special file
                ser_result =  SIMINSTR_ImiReadEF(cur_sim_task, file_id,
                                                 file_path_len,
                                                 file_path_id,
                                                 0,
                                                 psig->record_id,
                                                 SIM_RECORD_ABSOLUTE,
                                                 0,
                                                 &img_instance_data_len,
                                                 img_instance_buf);

                //process the error,get the error reason
                SCI_TRACE_LOW("SIM:: ImiReadEF, read img 0x%x ser_result = %d", file_id, ser_result);

                if(SIM_SERVICE_OK != ser_result)
                {
                    pcnf->result = AnalysisReadFileFail(ser_result);
                }
                else
                {
                    decode_result = DecodeIidf(img.descriptor[0], img_instance_data_len, img_instance_buf, &pcnf->img_info.iidf);

                    if(!decode_result)
                    {
                        SCI_TRACE_LOW("SIM:decode  img instance data file failed.");
                        pcnf->result = SIM_READ_FAILED_UNKNOW;
                    }
                    else
                    {
                        SCI_TRACE_LOW("SIM:decode img instance data file ok, width is %d, height is %d.", pcnf->img_info.iidf.img_width, pcnf->img_info.iidf.img_height);
                    }
                }

                //reback to the MF
                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &select_data);
                SCI_TRACE_LOW("SIM::SIMSIGNAL_ReadImg, reback to MF ser_result = %d", ser_result);
            }

#else

            if(img.descriptor[0].coding_scheme == 0x11)
            {
                img_instance_data_len = sizeof(basic_image_instance_data);
                SCI_MEMCPY(img_instance_buf, basic_image_instance_data, img_instance_data_len);
            }
            else
            {
                img_instance_data_len = sizeof(color_image_instance_data);
                img.descriptor[0].len_of_data = 22;
                SCI_MEMCPY(img_instance_buf, color_image_instance_data, img_instance_data_len);
            }

            decode_result = DecodeIidf(img.descriptor[0], img_instance_data_len, img_instance_buf, &pcnf->img_info.iidf);

            if(!decode_result)
            {
                SCI_TRACE_LOW("SIM:decode  img instance data file failed.");
                pcnf->result = SIM_READ_FAILED_UNKNOW;
            }
            else
            {
#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM:decode  img instance data file ok."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6893_112_2_17_23_0_6_4727, (uint8 *)"");
#endif
                //SCI_TRACE_LOW:"SIM:img width is %d,height is %d."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6895_112_2_17_23_0_6_4728, (uint8 *)"dd", pcnf->img_info.iidf.img_width, pcnf->img_info.iidf.img_height);
            }

#endif
        }
    }
    else
    {
        //SCI_TRACE_LOW:"SIM::img service not support."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SIGNAL_6902_112_2_17_23_0_6_4729, (uint8 *)"");
        pcnf->is_service_support = FALSE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC SIM_SERVICE_RESULT_E SIMSIGNAL_GetSIMLanguageInfo(
    uint32 cur_sim_task,
    SIM_EF_MAPPING_LP_UNION_T *language_mapping
)
{
    SIM_EF_MAPPING_ELP_T mf_elp_mapping;
#ifdef SIM_3G_USIM_SUPPORT
    SIM_EF_MAPPING_ELP_T adf_elp_mapping;
#endif
    SIM_EF_MAPPING_LP_T lp_mapping;
    SIM_SERVICE_RESULT_E elp_result;
    SIM_SERVICE_RESULT_E lp_result;
    uint8 i = 0;

    //clear all
    language_mapping->is_1st_valid = FALSE;
    language_mapping->elp_1st_max_num = 0;
    language_mapping->elp_1st_valid_num = 0;
    language_mapping->is_lp_valid = FALSE;
    language_mapping->lp_1st_max_num = 0;
    language_mapping->lp_1st_valid_num = 0;
    language_mapping->is_2nd_valid = FALSE;
    language_mapping->elp_2nd_max_num = 0;
    language_mapping->elp_2nd_valid_num = 0;
#ifdef GSM_CUSTOMER_AFP_SUPPORT
    s_sim_language_length_info[cur_sim_task].elp_1st_max_num = 0xFF;
    s_sim_language_length_info[cur_sim_task].lp_1st_max_num = 0xFF;
    s_sim_language_length_info[cur_sim_task].elp_2nd_max_num = 0xFF;
#endif

    elp_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ELP, &mf_elp_mapping);

    if(IsFatalError(elp_result))
    {
        return elp_result;
    }

#ifdef SIM_3G_USIM_SUPPORT

    if(SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])
#endif
    {
        if(SIM_SERVICE_OK == elp_result)
        {
            language_mapping->is_1st_valid = TRUE;
            language_mapping->elp_1st_max_num = mf_elp_mapping.max_num;
            language_mapping->elp_1st_valid_num = mf_elp_mapping.valid_num;

            for(i = 0; i < MIN(language_mapping->elp_1st_valid_num, SIM_MAX_VALIDATE_LANGUAGE_NUM); i++)
            {
                language_mapping->elp_1st_lang_codes[i].code[0] = mf_elp_mapping.lang_codes[i].code[0];
                language_mapping->elp_1st_lang_codes[i].code[1] = mf_elp_mapping.lang_codes[i].code[1];
            }
        }

        lp_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LP, &lp_mapping);

        if(SIM_SERVICE_OK == lp_result)
        {
            language_mapping->lp_1st_max_num = lp_mapping.max_num;
            language_mapping->lp_1st_valid_num = lp_mapping.valid_num;

            for(i = 0; i < MIN(language_mapping->lp_1st_valid_num, SIM_MAX_VALIDATE_LANGUAGE_NUM); i++)
            {
                language_mapping->lp_1st_lang_codes[i] = lp_mapping.lang_codes[i];
            }

            language_mapping->is_lp_valid = TRUE;
        }
        else
        {
            if(IsFatalError(lp_result))
            {
                return lp_result;
            }
        }
    }

#ifdef SIM_3G_USIM_SUPPORT
    else
    {
        if(SIM_SERVICE_OK == elp_result)
        {
            //In 3G USIM, EFpl(2F 05) is the 2nd priority language selection.
            language_mapping->is_2nd_valid = TRUE;
            language_mapping->elp_2nd_max_num = mf_elp_mapping.max_num;
            language_mapping->elp_2nd_valid_num = mf_elp_mapping.valid_num;

            for(i = 0; i < MIN(language_mapping->elp_2nd_valid_num, SIM_MAX_VALIDATE_LANGUAGE_NUM); i++)
            {
                language_mapping->elp_2nd_lang_codes[i].code[0] = mf_elp_mapping.lang_codes[i].code[0];
                language_mapping->elp_2nd_lang_codes[i].code[1] = mf_elp_mapping.lang_codes[i].code[1];
            }
        }

        //In 3G USIM, ADFusim/EFli(6F 05) is the 1st priority language selection.
        lp_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_LI, &adf_elp_mapping);

        if(SIM_SERVICE_OK == lp_result)
        {
            language_mapping->is_1st_valid = TRUE;
            language_mapping->elp_1st_max_num = adf_elp_mapping.max_num;
            language_mapping->elp_1st_valid_num = adf_elp_mapping.valid_num;

            for(i = 0; i < MIN(language_mapping->elp_1st_valid_num, SIM_MAX_VALIDATE_LANGUAGE_NUM); i++)
            {
                language_mapping->elp_1st_lang_codes[i].code[0] = adf_elp_mapping.lang_codes[i].code[0];
                language_mapping->elp_1st_lang_codes[i].code[1] = adf_elp_mapping.lang_codes[i].code[1];
            }
        }
        else
        {
            if(IsFatalError(lp_result))
            {
                return lp_result;
            }
        }
    }

#endif

#ifdef GSM_CUSTOMER_AFP_SUPPORT
    s_sim_language_length_info[cur_sim_task].elp_1st_max_num = language_mapping->elp_1st_max_num;
    s_sim_language_length_info[cur_sim_task].lp_1st_max_num = language_mapping->lp_1st_max_num;
    s_sim_language_length_info[cur_sim_task].elp_2nd_max_num = language_mapping->elp_2nd_max_num;
#endif

    if((SIM_SERVICE_OK == elp_result) || (SIM_SERVICE_OK == lp_result))
    {
        return SIM_SERVICE_OK;
    }
    else
    {
        return SIM_SERVICE_ERROR_UNKNOW;
    }
}

#ifndef SIM_CODE_COMPACT
/*****************************************************************************/
//  Description : the function that process the Authentication request from WiFi(or CMMB, etc)
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_CommonAuth(uint32 cur_sim_task, SIM_SIG_COMMON_AUTH_REQ_T *psig)
{
    SIM_SIG_COMMON_ALGO_CNF_T *pcnf;
    SIM_FILE_NAME_E cur_dir;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    SIM_FILE_NAME_E file_to_select = SIM_FILE_EF_KC;
    uint8 alg_data[255] = {0};
    uint8 *ptemp;
    BOOLEAN is_need_select = FALSE;
    BOOLEAN is_gsm_network = FALSE;
    uint8 rsp_data_len;
    BOOLEAN is_need_decode = TRUE;
#ifdef SIM_3G_USIM_SUPPORT
    uint8 temp_len = 0;
#endif

    //create the confirm signal of the request signal
    if(psig->is_mbbms_auth)
    {
        SCI_CREATE_SIGNAL(pcnf, SIM_MBBMS_AUTH_CNF, (uint16)(sizeof(SIM_SIG_COMMON_ALGO_CNF_T)), g_sim_task_id[cur_sim_task]);
    }
    else
    {
        SCI_CREATE_SIGNAL(pcnf, SIM_COMMON_AUTH_CNF, (uint16)(sizeof(SIM_SIG_COMMON_ALGO_CNF_T)), g_sim_task_id[cur_sim_task]);
    }

    pcnf->ret = SIM_MM_AUTH_OK;
    pcnf->command_ref = psig->command_ref;
    pcnf->task_id = psig->task_id;
    pcnf->algo_index = psig->algo_index;
#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        pcnf->sim_or_usim = 1;// 0-sim;1-usim
    }
    else
#endif
    {
        pcnf->sim_or_usim = 0;// 0-sim;1-usim
    }

    if(0 == psig->rand[0]) //double check, rand[0] is rand_length, must not be 0.
    {
        pcnf->ret = SIM_MM_AUTH_FAILED;
    }
    else
    {
        //in the vesion of PS in the really hardware enviroment,run the GSM algorithm
        cur_dir = SIMFS_GetCurDirectoryFile(cur_sim_task);

#ifdef SIM_3G_USIM_SUPPORT

        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            pcnf->sim_or_usim = 1;//usim

            if(SIM_FILE_ADF_USIM != SIMFS_GetCurFileName(cur_sim_task))
            {
                // USIM 3G must be in ADFusim, or a USIM will auth 69 85. wuding
                is_need_select = TRUE;
                file_to_select = SIM_FILE_ADF_USIM;
            }
            else
            {
                is_need_select = FALSE;
            }
        }
        else
#endif
        {
            if((SIM_FILE_DF_GSM != cur_dir)
                && (SIMFS_GetParenFileName(cur_dir) != SIM_FILE_DF_GSM))
            {
                //DFgsm and any sub-directory unser DFgsm is not select as the current Directory
                is_need_select = TRUE;
                file_to_select = SIM_FILE_DF_GSM;//A very old Shenzhen CMCC has no EFkc
            }
            else
            {
                is_need_select = FALSE;
            }
        }

        if(is_need_select)
        {
            ser_result = SIMSER_SelectFile(cur_sim_task, file_to_select, &select_data);

            if(SIM_SERVICE_OK != ser_result)
            {
                if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
                {
                    //clear the Temp int signal queue
                    SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);

                    //try the restore
                    //if the restroe is successful
                    if(DealCardUnResponse(cur_sim_task))
                    {
                        //disable the deep sleep,because the dealcardunresponse has open the deep sleep
                        DisableDeepSleep(cur_sim_task);
                        SCI_TRACE_LOW("SIM::begin to re-select the file");
                        ser_result = SIMSER_SelectFile(cur_sim_task, file_to_select, &select_data);
                    }
                    else
                    {
                        ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                    }
                }

                if(SIM_SERVICE_OK != ser_result)
                {
                    SCI_TRACE_LOW("SIM::select file failed before mm auth. %d", ser_result);
#ifndef WIN32
                    SIM_PrintTxBuf(cur_sim_task);
                    SIM_PrintRxBuf(cur_sim_task);
#endif

                    if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
                    {
                        pcnf->ret = SIM_MM_AUTH_FAILED;
                    }
                    else
                    {
                        pcnf->ret = SIM_MM_AUTH_SELECT_GSM_FAILED;
                    }
                }
            }
        }

        //request the GSM algorithm of the SIM card
        if(SIM_SERVICE_OK == ser_result)
        {
            //autnToken length is zero, so I consider it is in GSM network
            if(psig->autn[0] == 0)
            {
                /*
                //In fact, we should not judge the 2G/3G network by AUTN!
                //The PS should give this indicator directly! wuding.
                //We just judge this in WiFi/MBBMS authentication.
                */
                is_gsm_network = TRUE;
            }

            // RUN 3G authentication Algorithm
            ser_result = SIMINSTR_RunUMTSAlgorithm(cur_sim_task, \
                                                   is_gsm_network, psig->rand, psig->autn, &rsp_data_len, alg_data);

            if(SIM_SERVICE_RUN_ALGORITHM_MAC_ERROR == ser_result)
            {
                is_need_decode = FALSE;
                pcnf->ret = SIM_MM_AUTH_MAC_INCORRECT;
            }
            else if(SIM_SERVICE_OK != ser_result)
            {
                if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
                {
                    //clear the Temp int signal queue
                    SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);

                    //if the restroe is successful
                    if(DealCardUnResponse(cur_sim_task))
                    {
                        //disable the deep sleep,because the dealcardunresponse has open the deep sleep
                        DisableDeepSleep(cur_sim_task);

                        //redo the run 3G algorithm
                        ser_result = SIMINSTR_RunUMTSAlgorithm(cur_sim_task, \
                                                               is_gsm_network, psig->rand, psig->autn, &rsp_data_len, alg_data);
                    }
                    else
                    {
                        ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                    }
                }

                if(SIM_SERVICE_RUN_ALGORITHM_MAC_ERROR == ser_result)
                {
                    is_need_decode = FALSE;
                    pcnf->ret = SIM_MM_AUTH_MAC_INCORRECT;
                }
                else if(SIM_SERVICE_OK != ser_result)
                {
                    SCI_TRACE_LOW("SIM::Run special algorithm failed %d", ser_result);
#ifndef WIN32
                    SIM_PrintTxBuf(cur_sim_task);
                    SIM_PrintRxBuf(cur_sim_task);
#endif

                    if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
                    {
                        pcnf->ret = SIM_MM_AUTH_FAILED;
                    }
                    else
                    {
                        pcnf->ret = SIM_MM_AUTH_SELECT_GSM_FAILED;
                    }
                }
                else
                {
                    is_need_decode = TRUE;
                }
            }
            else
            {
                is_need_decode = TRUE;
            }

            if(is_need_decode)
            {
                ptemp = alg_data;
#ifdef SIM_3G_USIM_SUPPORT

                if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                {
                    if(is_gsm_network)
                        //SERS len tag | SERS DATA(4 bytes) |KC len tag | KC DATA (8bytes)
                    {

                        SCI_TRACE_LOW("SIM Auth rsp_data_len=%d, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x", \
                                      rsp_data_len, *ptemp, *(ptemp + 1), *(ptemp + 2), *(ptemp + 3), \
                                      *(ptemp + 4), *(ptemp + 5), *(ptemp + 6), *(ptemp + 8));

                        if(rsp_data_len != (SIM_SERS_LEN + SIM_EF_KC_KC_LEN + 2))
                        {
                            pcnf->ret = SIM_MM_AUTH_FAILED;
                        }
                        else
                        {
                            pcnf->sres_res_autn[0] = SIM_SERS_LEN;
                            SCI_MEMCPY(&(pcnf->sres_res_autn[1]), ptemp + 1, SIM_SERS_LEN);
                            pcnf->kc[0] = SIM_EF_KC_KC_LEN;
                            SCI_MEMCPY(&(pcnf->kc[1]), ptemp + SIM_SERS_LEN + 2, SIM_EF_KC_KC_LEN);
                            pcnf->ck[0] = 0;
                            pcnf->ik[0] = 0;
                        }
                    }
                    else
                        //case1 : the AUTH is successful
                        //   TAG     | RES LEN tag |    RES DATA   | CK LEN tag|     CKCN        |  IK LEN tag |   IKCN        | KC LEN tag | KC DATA       |
                        //   0xDB   |      1byte      | RES LEN bytes|   1 byte    | CK LEN bytes |     1byte    |IK LEN bytes |      1 byte   | KC LEN bytes |

                        //case2 : AUTN SYNC Fail
                        //  TAG      | AUTS LEN tag  | AUTS DATA        |
                        // 0xDC      |     1 byte       |  AUTS LEN bytes |
                    {

                        SCI_TRACE_LOW("SIM Auth rsp_data_len=%d, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x", \
                                      rsp_data_len, *ptemp, *(ptemp + 1), *(ptemp + 2), *(ptemp + 3), \
                                      *(ptemp + 4), *(ptemp + 5), *(ptemp + 6), *(ptemp + 8));

                        if(SIM_AUTN_3G_SUCCESS_TAG == *ptemp)
                        {
                            ptemp += 1; // move to res length tag
                            temp_len = *ptemp;
                            pcnf->sres_res_autn[0] = temp_len;

                            if(pcnf->sres_res_autn[0] > USIM_MAX_RES_LEN)
                            {
                                pcnf->sres_res_autn[0] = USIM_MAX_RES_LEN;
                            }

                            ptemp += 1;
                            SCI_MEMCPY(&(pcnf->sres_res_autn[1]), ptemp, pcnf->sres_res_autn[0]);

                            ptemp += temp_len;// move to ck length tag
                            temp_len = *ptemp;
                            pcnf->ck[0] = temp_len;

                            if(pcnf->ck[0] > USIM_CK_LEN)
                            {
                                pcnf->ck[0] = USIM_CK_LEN;
                            }

                            ptemp += 1;
                            SCI_MEMCPY(&(pcnf->ck[1]), ptemp, pcnf->ck[0]);

                            ptemp += temp_len;// move to ik length tag
                            temp_len = *ptemp;
                            pcnf->ik[0] = temp_len;

                            if(pcnf->ik[0] > USIM_IK_LEN)
                            {
                                pcnf->ik[0] = USIM_IK_LEN;
                            }

                            ptemp += 1;
                            SCI_MEMCPY(&(pcnf->ik[1]), ptemp, pcnf->ik[0]);

                            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GSM_SEC_CONTEXT))
                            {
                                ptemp += temp_len;// move to Kc length tag
                                pcnf->kc[0] = *ptemp;

                                if(pcnf->kc[0] > 8)
                                {
                                    pcnf->kc[0] = 8;
                                }

                                ptemp += 1;
                                SCI_MEMCPY(&(pcnf->kc[1]), ptemp, pcnf->kc[0]);
                            }

                            //check the length, should be valid!
                            if((pcnf->kc[0] > 8)
                                || (pcnf->ck[0] > USIM_CK_LEN)
                                || (pcnf->ik[0] > USIM_IK_LEN)
                                || (pcnf->sres_res_autn[0] > USIM_MAX_RES_LEN))
                            {
                                pcnf->ret = SIM_MM_AUTH_FAILED;
                            }
                        }
                        else if(SIM_AUTN_SYNC_FAIL_TAG == *ptemp)
                        {
                            pcnf->ret = SIM_MM_AUTH_SYNC_FAILED;
                            ptemp += 1; // move to auts length tag
                            pcnf->sres_res_autn[0] = *ptemp;

                            if(pcnf->sres_res_autn[0] > USIM_AUTS_LEN)
                            {
                                pcnf->sres_res_autn[0] = 0;
                                pcnf->ret = SIM_MM_AUTH_FAILED;
                            }
                            else
                            {
                                ptemp += 1;
                                SCI_MEMCPY(&(pcnf->sres_res_autn[1]), ptemp, pcnf->sres_res_autn[0]);
                            }
                        }
                        else
                        {
                            pcnf->ret = SIM_MM_AUTH_FAILED;
                        }
                    }
                }
                else
#endif
                {
                    //SERS DATA(4 bytes) | KC DATA (8bytes)
                    if(rsp_data_len != (SIM_SERS_LEN + SIM_EF_KC_KC_LEN))
                    {
                        pcnf->ret = SIM_MM_AUTH_FAILED;
                    }
                    else
                    {
                        pcnf->sres_res_autn[0] = SIM_SERS_LEN;
                        SCI_MEMCPY(&(pcnf->sres_res_autn[1]), ptemp, SIM_SERS_LEN);
                        pcnf->kc[0] = SIM_EF_KC_KC_LEN;
                        SCI_MEMCPY(&(pcnf->kc[1]), ptemp + SIM_SERS_LEN, SIM_EF_KC_KC_LEN);
                        pcnf->ck[0] = 0;
                        pcnf->ik[0] = 0;
                    }
                }
            }
        }
    }

    //send the confirm signal of the requset signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

/*****************************************************************************/
//  Description : the function that process the VM request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_ReadVoiceMail(uint32 cur_sim_task, SIM_SIG_READ_VOICE_MAIL_REQ_T *psig)
{
    BOOLEAN is_voicemail_exist = FALSE;
    SIM_SIG_READ_VOICE_MAIL_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E result = SIM_SERVICE_ERROR_UNKNOW;
    SIM_RESPONSE_DATA_OF_SELECT_T temp_response;
    uint8 num_of_record = 0;
    SIM_EF_MAPPING_MBI_T vm_mbi;
    uint8 mbdn_index = 1;
    SIM_EF_MAPPING_MBNUM_T vm_mbdn;

    SCI_CREATE_SIGNAL(pcnf, SIM_READ_VOICE_MAIL_CNF, (uint16)(sizeof(SIM_SIG_READ_VOICE_MAIL_CNF_T)), g_sim_task_id[cur_sim_task]);

    pcnf->line_id = psig->line_id;
    pcnf->vm_type = psig->vm_type;

    SCI_TRACE_LOW("SIMSIGNAL_ReadVoiceMail line=%d, type=%d ", pcnf->line_id, pcnf->vm_type);

    //Rel-4 SIM, firstly check MBI to read MBDN
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_MDN))//For Test
    {
        SCI_TRACE_LOW("SIMSIGNAL_ReadVoiceMail MDN SUPPORT");

        //MBI check
        result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_MBI, &temp_response);

        if(SIM_SERVICE_OK == result)
        {
            if(0 != temp_response.dfnumber_recordlen)
            {
                num_of_record = (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);
            }

            SCI_TRACE_LOW("SIMSIGNAL_ReadVoiceMail MDN_1 %d", num_of_record);

            if(((SIM_MULTI_SUBSCRIBER_ID_1 == psig->line_id) && (num_of_record >= 1))
                || ((SIM_MULTI_SUBSCRIBER_ID_2 == psig->line_id) && (num_of_record >= 2)))
            {
                result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_MBI, (uint8)psig->line_id, SIM_RECORD_ABSOLUTE, &vm_mbi);

                if(SIM_SERVICE_OK == result)
                {
                    if(SIM_VOICE_MAIL == psig->vm_type)
                    {
                        mbdn_index = vm_mbi.voicemail_index;

                        //for a special Claro 3G card in SNMC
                        if((0 == mbdn_index) && (1 == num_of_record))
                        {
                            mbdn_index = 1;
                        }

                        //for a special Claro 3G card in SNMC
                    }
                    else if(SIM_VOICE_MAIL_FAX == psig->vm_type)
                    {
                        mbdn_index = vm_mbi.fax_index;
                    }
                    else if(SIM_VOICE_MAIL_EMAIL == psig->vm_type)
                    {
                        mbdn_index = vm_mbi.email_index;
                    }
                    else
                    {
                        mbdn_index = vm_mbi.other_index;
                    }

                    SCI_TRACE_LOW("SIMSIGNAL_ReadVoiceMail MDN_2 mbdn_index=%d", mbdn_index);

                    if(0 != mbdn_index)
                    {
                        //MBDN check
                        SCI_MEMSET(&temp_response, 0, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                        result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_MBDN, &temp_response);

                        if(SIM_SERVICE_OK == result)
                        {
                            if(0 != temp_response.dfnumber_recordlen)
                            {
                                num_of_record = (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);
                            }

                            if(mbdn_index <= num_of_record)
                            {
                                result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_MBDN, mbdn_index, SIM_RECORD_ABSOLUTE, &vm_mbdn);

                                if(SIM_SERVICE_OK == result)
                                {
                                    SCI_MEMCPY(&(pcnf->voice_mail), &vm_mbdn, sizeof(SIM_EF_MAPPING_MBNUM_T));
                                    is_voicemail_exist = TRUE;
                                }
                            }
                        }
                    }
                }
            }
        }

        if(IsFatalError(result))
        {
            pcnf->is_voicemail_exist = is_voicemail_exist;
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    SCI_TRACE_LOW("SIMSIGNAL_ReadVoiceMail middle %d", is_voicemail_exist);

    //If no Rel-4, or Rel4 content NULL, use CPHS
    if(!is_voicemail_exist)
    {
#ifdef SIM_3G_USIM_SUPPORT
#ifndef SIM_3G_USIM_SUPPORT_CPHS
        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            //In USIM, we donot access CPHS.
        }
        else
#endif 
#endif
        {
            //READ CPHS. 1-Line1,2-Line2,3-Data,4-Fax.
            if(SIM_IsCPHSOptionalSupport(cur_sim_task, SIM_FILE_EF_MBNUM))
            {
                if((SIM_MULTI_SUBSCRIBER_ID_1 == psig->line_id)
                    || (SIM_MULTI_SUBSCRIBER_ID_2 == psig->line_id))
                {
                    if(SIM_VOICE_MAIL == psig->vm_type)
                    {
                        mbdn_index = (uint8)psig->line_id;
                    }
                    else
                    {
                        //......may it be......
                        if(SIM_VOICE_MAIL_FAX == psig->vm_type)
                        {
                            mbdn_index = 4;//Record 4 Fax mailbox
                        }
                        else
                        {
                            mbdn_index = 3;
                        }
                    }
                }
                else
                {
                    if(SIM_VOICE_MAIL_FAX == psig->vm_type)
                    {
                        mbdn_index = 4;//Record 4 Fax mailbox
                    }
                    else
                    {
                        mbdn_index = 3;
                    }
                }

                SCI_MEMSET(&temp_response, 0, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_MBNUM, &temp_response);

                if(SIM_SERVICE_OK == result)
                {
                    if(0 != temp_response.dfnumber_recordlen)
                    {
                        num_of_record = (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);
                    }

                    if(mbdn_index <= num_of_record)
                    {
                        result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_MBNUM, mbdn_index, SIM_RECORD_ABSOLUTE, &vm_mbdn);

                        if(SIM_SERVICE_OK == result)
                        {
                            SCI_MEMCPY(&(pcnf->voice_mail), &vm_mbdn, sizeof(SIM_EF_MAPPING_MBNUM_T));
                            is_voicemail_exist = TRUE;
                        }
                    }
                }
            }
        }
    }

    pcnf->is_voicemail_exist = is_voicemail_exist;
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/*****************************************************************************/
//  Description : the function that process the VM request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_UpdateVoiceMail(uint32 cur_sim_task, SIM_SIG_UPDATE_VOICE_MAIL_REQ_T *psig)
{
    BOOLEAN is_voicemail_exist = FALSE;
    SIM_SIG_UPDATE_VOICE_MAIL_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E result = SIM_SERVICE_ERROR_UNKNOW;
    SIM_RESPONSE_DATA_OF_SELECT_T temp_response;
    uint8 num_of_record = 0;
    SIM_EF_MAPPING_MBI_T vm_mbi;
    SIM_EF_MAPPING_MBI_T temp_vm_mbi;
    SIM_EF_MAPPING_MBNUM_T vm_mbdn;
    uint8 mbdn_index = 1;
    uint8 i = 1;
    uint8 j = 0;
    uint8 valid_index_in_mbi[SIM_EF_MBI_ARR_SIZE *SIM_EF_MBI_ARR_NUM] = {0};
    BOOLEAN mbi_need_update_flag = FALSE;
    BOOLEAN empty_record_flag = FALSE;

    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_VOICE_MAIL_CNF, (uint16)(sizeof(SIM_SIG_UPDATE_VOICE_MAIL_CNF_T)), g_sim_task_id[cur_sim_task]);

    pcnf->line_id = psig->line_id;
    pcnf->vm_type = psig->vm_type;
    pcnf->is_can_be_update = TRUE;
    pcnf->result = FALSE;

    SCI_TRACE_LOW("SIMSIGNAL_UpdateVoiceMail line=%d, type=%d ", pcnf->line_id, pcnf->vm_type);

    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_MDN))//For Test
    {
        SCI_TRACE_LOW("SIMSIGNAL_UpadateVoiceMail MDN SUPPORT");

        //MBI check
        result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_MBI, &temp_response);

        if(SIM_SERVICE_OK == result)
        {
            if(0 != temp_response.dfnumber_recordlen)
            {
                num_of_record = (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);
            }

            SCI_TRACE_LOW("SIMSIGNAL_UpadateVoiceMail MDN_1 %d", num_of_record);

            if(((SIM_MULTI_SUBSCRIBER_ID_1 == psig->line_id) && (num_of_record >= 1))
                || ((SIM_MULTI_SUBSCRIBER_ID_2 == psig->line_id) && (num_of_record >= 2)))
            {
                result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_MBI, (uint8)psig->line_id, SIM_RECORD_ABSOLUTE, &vm_mbi);

                if(SIM_SERVICE_OK == result)
                {
                    if(SIM_VOICE_MAIL == psig->vm_type)
                    {
                        mbdn_index = vm_mbi.voicemail_index;

                        //for a special Claro 3G card in SNMC
                        if((0 == mbdn_index) && (1 == num_of_record))
                        {
                            mbdn_index = 1;
                        }

                        //for a special Claro 3G card in SNMC
                    }
                    else if(SIM_VOICE_MAIL_FAX == psig->vm_type)
                    {
                        mbdn_index = vm_mbi.fax_index;
                    }
                    else if(SIM_VOICE_MAIL_EMAIL == psig->vm_type)
                    {
                        mbdn_index = vm_mbi.email_index;
                    }
                    else
                    {
                        mbdn_index = vm_mbi.other_index;
                    }

                    SCI_TRACE_LOW("SIMSIGNAL_UpdateVoiceMail MDN_2 mbdn_index=%d", mbdn_index);

                    //If EFmbi is empty. We should initialize the EFmbi/EFmbdn.
                    if(0 == mbdn_index)
                    {
                        //check EFmbi content
                        SCI_MEMSET(valid_index_in_mbi, 0, 20); //init

                        if(num_of_record > SIM_EF_MBI_ARR_NUM)
                        {
                            num_of_record = SIM_EF_MBI_ARR_NUM;
                        }

                        for(i = 0; i < num_of_record; i++)
                        {
                            if((i + 1) == psig->line_id) //Do not need read again
                            {
                                temp_vm_mbi = vm_mbi;
                                result = SIM_SERVICE_OK;
                            }
                            else
                            {
                                result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_MBI, (uint8)(i + 1), SIM_RECORD_ABSOLUTE, &temp_vm_mbi);
                            }

                            if(SIM_SERVICE_OK == result)
                            {
                                if(0 != temp_vm_mbi.voicemail_index)
                                {
                                    valid_index_in_mbi[i *SIM_EF_MBI_ARR_SIZE+0] = temp_vm_mbi.voicemail_index;
                                }

                                if(0 != temp_vm_mbi.fax_index)
                                {
                                    valid_index_in_mbi[i *SIM_EF_MBI_ARR_SIZE+1] = temp_vm_mbi.fax_index;
                                }

                                if(0 != temp_vm_mbi.email_index)
                                {
                                    valid_index_in_mbi[i *SIM_EF_MBI_ARR_SIZE+2] = temp_vm_mbi.email_index;
                                }

                                if(0 != temp_vm_mbi.other_index)
                                {
                                    valid_index_in_mbi[i *SIM_EF_MBI_ARR_SIZE+3] = temp_vm_mbi.other_index;
                                }
                            }
                        }

                        SCI_TRACE_LOW("SIM: EFmbi records content %d %d %d %d, %d %d %d %d, %d %d %d %d, %d %d %d %d, %d %d %d %d", \
                                      valid_index_in_mbi[0], valid_index_in_mbi[1], valid_index_in_mbi[2], valid_index_in_mbi[3], \
                                      valid_index_in_mbi[4], valid_index_in_mbi[5], valid_index_in_mbi[6], valid_index_in_mbi[7], \
                                      valid_index_in_mbi[8], valid_index_in_mbi[9], valid_index_in_mbi[10], valid_index_in_mbi[11], \
                                      valid_index_in_mbi[12], valid_index_in_mbi[13], valid_index_in_mbi[14], valid_index_in_mbi[15], \
                                      valid_index_in_mbi[16], valid_index_in_mbi[17], valid_index_in_mbi[18], valid_index_in_mbi[19]);

                        //seek an empty record in EFmbdn
                        SCI_MEMSET(&temp_response, 0, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                        result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_MBDN, &temp_response);

                        if(SIM_SERVICE_OK == result)
                        {
                            if(0 != temp_response.dfnumber_recordlen)
                            {
                                num_of_record = (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);
                            }

                            if(num_of_record > SIM_EF_MBDN_ARR_NUM)
                            {
                                num_of_record = SIM_EF_MBDN_ARR_NUM;
                            }

                            for(i = 0; i < num_of_record; i++)
                            {
                                empty_record_flag = TRUE;

                                for(j = 0; j < (SIM_EF_MBI_ARR_SIZE * SIM_EF_MBI_ARR_NUM); j++)
                                {
                                    if((i + 1) == valid_index_in_mbi[j])
                                    {
                                        empty_record_flag = FALSE;
                                        break;
                                    }
                                }

                                if(empty_record_flag)
                                {
                                    result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_MBDN, (uint8)(i + 1), SIM_RECORD_ABSOLUTE, &vm_mbdn);

                                    if(SIM_SERVICE_OK == result)
                                    {
                                        if(0 == vm_mbdn.mailbox_number_len) //empty record
                                        {
                                            empty_record_flag = TRUE;
                                        }
                                        else
                                        {
                                            empty_record_flag = FALSE;
                                        }
                                    }
                                    else
                                    {
                                        empty_record_flag = FALSE;
                                    }
                                }

                                //find out an empty record in EFmbdn
                                if(empty_record_flag)
                                {
                                    mbi_need_update_flag = TRUE;
                                    mbdn_index = i + 1;
                                    break;
                                }
                            }
                        }
                    }

                    if(0 != mbdn_index)
                    {
                        //MBDN check
                        SCI_MEMSET(&temp_response, 0, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                        result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_MBDN, &temp_response);

                        if(SIM_SERVICE_OK == result)
                        {
                            if((0 == (temp_response.access_condition1 & 0xF))
                                || (1 == (temp_response.access_condition1 & 0xF)))//It can be updated
                            {
                                if(0 != temp_response.dfnumber_recordlen)
                                {
                                    num_of_record = (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);
                                }

                                if(mbdn_index <= num_of_record)
                                {
                                    result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_MBDN, mbdn_index, SIM_RECORD_ABSOLUTE, &(psig->vm_content), FALSE);

                                    if(SIM_SERVICE_OK == result)
                                    {
                                        is_voicemail_exist = TRUE;
                                        pcnf->result = TRUE;

                                        if(mbi_need_update_flag)
                                        {
                                            //Update the EFmbi firstly
                                            if(SIM_VOICE_MAIL == psig->vm_type)
                                            {
                                                vm_mbi.voicemail_index = mbdn_index;
                                            }
                                            else if(SIM_VOICE_MAIL_FAX == psig->vm_type)
                                            {
                                                vm_mbi.fax_index = mbdn_index;
                                            }
                                            else if(SIM_VOICE_MAIL_EMAIL == psig->vm_type)
                                            {
                                                vm_mbi.email_index = mbdn_index;
                                            }
                                            else if(SIM_VOICE_MAIL_OTHER == psig->vm_type)
                                            {
                                                vm_mbi.other_index = mbdn_index;
                                            }

                                            result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_MBI, (uint8)psig->line_id, SIM_RECORD_ABSOLUTE, &vm_mbi, FALSE);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                is_voicemail_exist = TRUE;//Yes, it exist.
                                pcnf->is_can_be_update = FALSE;
                                SCI_TRACE_LOW("SIMSIGNAL_UpdateVoiceMail MBDN cannot update 0x%x", temp_response.access_condition1);
                            }
                        }
                    }
                }
            }
        }

        if(IsFatalError(result))
        {
            pcnf->result = FALSE;
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    SCI_TRACE_LOW("SIMSIGNAL_UpdateVoiceMail middle %d", is_voicemail_exist);

    //If no Rel-4, or Rel4 content NULL, use CPHS
    if(!is_voicemail_exist)
    {
#ifdef SIM_3G_USIM_SUPPORT
#ifndef SIM_3G_USIM_SUPPORT_CPHS
        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            //In USIM, we donot access CPHS.
        }
        else
#endif 
#endif
        {
            //READ CPHS. 1-Line1,2-Line2,3-Data,4-Fax.
            if(SIM_IsCPHSOptionalSupport(cur_sim_task, SIM_FILE_EF_MBNUM))
            {
                if((SIM_MULTI_SUBSCRIBER_ID_1 == psig->line_id)
                    || (SIM_MULTI_SUBSCRIBER_ID_2 == psig->line_id))
                {
                    if(SIM_VOICE_MAIL == psig->vm_type)
                    {
                        mbdn_index = (uint8)psig->line_id;
                    }
                    else
                    {
                        //......may it be......
                        if(SIM_VOICE_MAIL_FAX == psig->vm_type)
                        {
                            mbdn_index = 3;
                        }
                        else
                        {
                            mbdn_index = 4;
                        }
                    }
                }
                else
                {
                    if(SIM_VOICE_MAIL_FAX == psig->vm_type)
                    {
                        mbdn_index = 3;
                    }
                    else
                    {
                        mbdn_index = 4;
                    }
                }

                result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_MBNUM, &temp_response);

                if(SIM_SERVICE_OK == result)
                {
                    if(0 != temp_response.dfnumber_recordlen)
                    {
                        num_of_record = (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);
                    }

                    if(mbdn_index <= num_of_record)
                    {
                        result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_MBNUM, mbdn_index, SIM_RECORD_ABSOLUTE, &(psig->vm_content), FALSE);

                        if(SIM_SERVICE_OK == result)
                        {
                            is_voicemail_exist = TRUE;
                            pcnf->result = TRUE;
                        }
                    }
                }
            }
        }
    }

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/*****************************************************************************/
//  Description : the function that process the CFFlag request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC BOOLEAN SIMSIGNAL_ReadCallForwardFlag(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    SIM_EF_MAPPING_CFFLAG_T *cfflag)
{
    SIM_FILE_NAME_E temp_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T temp_response;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_CFIS_T temp_cfis[4];
    uint8 i, num_of_record = 0;
    uint8 line1_index = 0;
    uint8 line2_index = 0;
    BOOLEAN line1_index_get_flag = FALSE;
    BOOLEAN line2_index_get_flag = FALSE;

    //clear
    SCI_MEMSET(temp_cfis, 0, (sizeof(SIM_EF_MAPPING_CFIS_T) * 4));
    cfflag->is_datafax_flag_present = FALSE;
    cfflag->line1_callfrd_uncond = 0x5;// 5 means no in CPHS CFFLAG.
    cfflag->line2_callfrd_uncond = 0x5;
    cfflag->fax_callfrd_uncond = 0x5;
    cfflag->data_callfrd_uncond = 0x5;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CFIS))
        {
            temp_file_name = SIM_FILE_EF_USIM_CFIS;
        }
        else
        {
            return FALSE;
        }
    }
    else
#endif
    {
        temp_file_name = SIM_FILE_EF_CFFLAG;//init

        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CFIS))
        {
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_CFIS, &temp_response);

            if(SIM_SERVICE_OK == ser_result)
            {
                temp_file_name = SIM_FILE_EF_CFIS;
            }
            else
            {
                if(IsFatalError(ser_result))
                {
                    return FALSE;
                }
            }
        }

        if(SIM_FILE_EF_CFFLAG == temp_file_name)
        {
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_CFFLAG, &temp_response);

            if(SIM_SERVICE_OK == ser_result)
            {
                temp_file_name = SIM_FILE_EF_CFFLAG;
            }
            else
            {
                return FALSE;
            }
        }
    }

    SCI_TRACE_LOW("SIM%d:SIMSIGNAL_ReadCallForwardFlag temp_file_name=%d", cur_sim_task, temp_file_name);

    if(SIM_FILE_EF_CFFLAG == temp_file_name)
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, temp_file_name, cfflag);

        if(SIM_SERVICE_OK != ser_result)
        {
            return FALSE;
        }
    }
    else
    {
        ser_result = SIMSER_SelectFile(cur_sim_task, temp_file_name, &temp_response);

        if(SIM_SERVICE_OK == ser_result)
        {
            if(0 != temp_response.dfnumber_recordlen)
            {
                num_of_record = (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);
            }

            if(num_of_record > 4)
            {
                num_of_record = 4;
            }

            for(i = 0; i < num_of_record; i++)
            {
                ser_result = SIMSER_ReadEFRecord(cur_sim_task, temp_file_name, (i + 1), SIM_RECORD_ABSOLUTE, &temp_cfis[i]);

                if(SIM_SERVICE_OK == ser_result)
                {
                    if(1 == temp_cfis[i].msp_id)
                    {
                        line1_index_get_flag = TRUE;
                        line1_index = i;
                    }
                    else if(2 == temp_cfis[i].msp_id)
                    {
                        line2_index_get_flag = TRUE;
                        line2_index = i;
                    }
                    else
                    {
                        //special case -- without msp, only 1 record, we regard this as line1
                        if ((1 == num_of_record) && (temp_cfis[i].voice_callfrd_uncond))
                        {
                            line1_index_get_flag = TRUE;
                            line1_index = 0;
                            line2_index_get_flag = FALSE;
                            line2_index = 0;
                        }
                    }

                    if(line1_index_get_flag && line2_index_get_flag)
                    {
                        break;//That's enough,quit the loop.
                    }
                }
                else
                {
                    break;//quit the loop
                }
            }

#ifdef GSM_CUSTOMER_AFP_SUPPORT

            //Special USIM use DFgsm/EFcfflag. for Robin Gupta.
            if(
#ifdef SIM_3G_USIM_SUPPORT
                (USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]) &&
#endif
                (!(line1_index_get_flag || line2_index_get_flag)))
            {
                //Not get valid info in ADFusim/EFcfis, so continue to check DFgsm/EFcfis.
                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_CFIS, &temp_response);

                if(SIM_SERVICE_OK == ser_result)
                {
                    if(0 != temp_response.dfnumber_recordlen)
                    {
                        num_of_record = (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);
                    }

                    if(num_of_record > 4)
                    {
                        num_of_record = 4;
                    }

                    for(i = 0; i < num_of_record; i++)
                    {
                        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_CFIS, (i + 1), SIM_RECORD_ABSOLUTE, &temp_cfis[i]);

                        if(SIM_SERVICE_OK == ser_result)
                        {
                            if(1 == temp_cfis[i].msp_id)
                            {
                                line1_index_get_flag = TRUE;
                                line1_index = i;
                            }
                            else if(2 == temp_cfis[i].msp_id)
                            {
                                line2_index_get_flag = TRUE;
                                line2_index = i;
                            }
                            else
                            {
                                //special case -- without msp, only 1 record, we regard this as line1
                                if ((1 == num_of_record) && (temp_cfis[i].voice_callfrd_uncond))
                                {
                                    line1_index_get_flag = TRUE;
                                    line1_index = 0;
                                    line2_index_get_flag = FALSE;
                                    line2_index = 0;
                                }
                            }

                            if(line1_index_get_flag && line2_index_get_flag)
                            {
                                break;//That's enough,quit the loop.
                            }
                        }
                        else
                        {
                            break;//quit the loop
                        }
                    }
                }
                else
                {
                    if(IsFatalError(ser_result))
                    {
                        return FALSE;
                    }
                }

                //Still not get valid info, so continue to check DFgsm/EFcfflag.
                if(!(line1_index_get_flag || line2_index_get_flag))
                {
                    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_CFFLAG, cfflag);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        return TRUE;
                    }
                    else
                    {
                        return FALSE;
                    }
                }
            }

            //Special USIM use DFgsm/EFcfflag. for Robin Gupta.
#endif

            //translate according to the CPHS CFFLAG format for upper layer
            if(!line1_index_get_flag)
            {
                if(line2_index_get_flag)
                {
                    if(0 == line2_index)
                    {
                    }
                    else
                    {
                        line1_index = 0;//set record 0 as line1 if no line1 indicator.
                        line1_index_get_flag = TRUE;
                    }
                }
                else
                {
                    line1_index = 0;//set record 0 as line1 if no line1 indicator.
                    line1_index_get_flag = TRUE;
                }
            }

            if(line1_index_get_flag)
            {
                s_sim_r4_cfis_line1_rec_id[cur_sim_task] = line1_index + 1;

                if(temp_cfis[line1_index].voice_callfrd_uncond)
                {
                    cfflag->line1_callfrd_uncond = 0xA;
                }
                else
                {
                    cfflag->line1_callfrd_uncond = 0x5;
                }

                cfflag->is_datafax_flag_present = TRUE;

                if(temp_cfis[line1_index].fax_callfrd_uncond)
                {
                    cfflag->fax_callfrd_uncond = 0xA;
                }
                else
                {
                    cfflag->fax_callfrd_uncond = 0x5;
                }

                if(temp_cfis[line1_index].data_callfrd_uncond)
                {
                    cfflag->data_callfrd_uncond = 0xA;
                }
                else
                {
                    cfflag->data_callfrd_uncond = 0x5;
                }
            }

            //line2 is less important than line1, so we don't do more.
            if(line2_index_get_flag)
            {
                s_sim_r4_cfis_line2_rec_id[cur_sim_task] = line2_index + 1;

                if(temp_cfis[line2_index].voice_callfrd_uncond)
                {
                    cfflag->line2_callfrd_uncond = 0xA;
                }
                else
                {
                    cfflag->line2_callfrd_uncond = 0x5;
                }
            }
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : the function that process the CFFlag request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC SIM_RESULT_UPDATE_FILE_E SIMSIGNAL_UpdateCallForwardFlag(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    SIM_EF_MAPPING_CFFLAG_T *cfflag)
{
    SIM_FILE_NAME_E temp_file_name = file_name;
    SIM_RESPONSE_DATA_OF_SELECT_T temp_response;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESULT_UPDATE_FILE_E update_result = SIM_UPDATE_FAILED_UNKNOW;
    SIM_EF_MAPPING_CFIS_T temp_cfis;
    uint8 rec_id = 0;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CFIS))
        {
            temp_file_name = SIM_FILE_EF_USIM_CFIS;
        }
        else
        {
            return update_result;
        }
    }
    else
#endif
    {
        temp_file_name = SIM_FILE_EF_CFFLAG;//init

        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CFIS))
        {
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_CFIS, &temp_response);

            if(SIM_SERVICE_OK == ser_result)
            {
                temp_file_name = SIM_FILE_EF_CFIS;
            }
            else
            {
                if(IsFatalError(ser_result))
                {
                    return update_result;
                }
            }
        }

        if(SIM_FILE_EF_CFFLAG == temp_file_name)
        {
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_CFFLAG, &temp_response);

            if(SIM_SERVICE_OK == ser_result)
            {
                temp_file_name = SIM_FILE_EF_CFFLAG;
            }
            else
            {
                return update_result;
            }
        }
    }

    SCI_TRACE_LOW("SIM%d:SIMSIGNAL_UpdateCallForwardFlag temp_file_name=%d", cur_sim_task, temp_file_name);

    if(SIM_FILE_EF_CFFLAG == temp_file_name)
    {
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, temp_file_name, cfflag);

        if(SIM_SERVICE_OK == ser_result)
        {
            update_result = SIM_UPDATE_OK;
        }
        else
        {
            update_result = SIM_UPDATE_FAILED_UNKNOW;
        }
    }
    else
    {
        rec_id = s_sim_r4_cfis_line1_rec_id[cur_sim_task];

        if(0 == rec_id)
        {
            update_result = SIM_UPDATE_FAILED_UNKNOW;//No, no line1.
        }
        else
        {
            temp_cfis.msp_id = 1;

            if(0xA == cfflag->line1_callfrd_uncond)
            {
                temp_cfis.voice_callfrd_uncond = TRUE;
            }
            else
            {
                temp_cfis.voice_callfrd_uncond = FALSE;
            }

            if(0xA == cfflag->fax_callfrd_uncond)
            {
                temp_cfis.fax_callfrd_uncond = TRUE;
            }
            else
            {
                temp_cfis.fax_callfrd_uncond = FALSE;
            }

            if(0xA == cfflag->data_callfrd_uncond)
            {
                temp_cfis.data_callfrd_uncond = TRUE;
            }
            else
            {
                temp_cfis.data_callfrd_uncond = FALSE;
            }

            ser_result = SIMSER_UpdateEFRecord(cur_sim_task, temp_file_name, rec_id, SIM_RECORD_ABSOLUTE, &temp_cfis, FALSE);

            if(SIM_SERVICE_OK == ser_result)
            {
                rec_id = s_sim_r4_cfis_line2_rec_id[cur_sim_task];

                if(0 == rec_id)
                {
                    update_result = SIM_UPDATE_OK;//No line2 in this card
                }
                else
                {
                    temp_cfis.msp_id = 2;

                    if(0xA == cfflag->line2_callfrd_uncond)
                    {
                        temp_cfis.voice_callfrd_uncond = TRUE;
                    }
                    else
                    {
                        temp_cfis.voice_callfrd_uncond = FALSE;
                    }

                    temp_cfis.fax_callfrd_uncond = FALSE;
                    temp_cfis.data_callfrd_uncond = FALSE;
                    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, temp_file_name, rec_id, SIM_RECORD_ABSOLUTE, &temp_cfis, FALSE);
                    update_result = SIM_UPDATE_OK;//Line2 is not important

                    if(SIM_SERVICE_OK != ser_result)
                    {
                        s_sim_r4_cfis_line2_rec_id[cur_sim_task] = 0;//clear. Line2 is not important
                    }
                }
            }
            else
            {
                update_result = SIM_UPDATE_FAILED_UNKNOW;
            }
        }
    }

    return update_result;
}

/*****************************************************************************/
//  Description : the function that process the MWIS request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC BOOLEAN SIMSIGNAL_ReadMWIS(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    SIM_VMW_IND_FLAG_T *vmwflag)
{
    SIM_FILE_NAME_E temp_file_name = file_name;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;
    SIM_EF_MAPPING_MWIS_T temp_mwis[2];//Line1 and Line2 is enough.
    SIM_EF_MAPPING_VMWFLAG_T temp_vmwflag;//cphs vmw flag
    uint8 i = 0;

    if(!s_sim_r4_is_vmw_info_gotten[cur_sim_task])
    {
        SIM_RESPONSE_DATA_OF_SELECT_T temp_response = {0};

        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_MWIS))
        {
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_MWIS, &temp_response);

            if(SIM_SERVICE_OK == ser_result)
            {
                g_sim_r4_vm_mwis_record_len[cur_sim_task] = temp_response.dfnumber_recordlen;

                if(temp_response.dfnumber_recordlen != 0)
                {
                    g_sim_r4_vm_mwis_record_num[cur_sim_task] =
                        (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);

                    if(g_sim_r4_vm_mwis_record_num[cur_sim_task] > 2)
                    {
                        g_sim_r4_vm_mwis_record_num[cur_sim_task] = 2;
                    }
                }
            }
            else
            {
                g_sim_r4_vm_mwis_record_len[cur_sim_task] = 0;
                g_sim_r4_vm_mwis_record_num[cur_sim_task] = 0;
            }
        }
        else
        {
            g_sim_r4_vm_mwis_record_len[cur_sim_task] = 0;
            g_sim_r4_vm_mwis_record_num[cur_sim_task] = 0;
        }

        if(SIM_SERVICE_OK != ser_result)
        {
            SCI_MEMSET(&temp_response, 0, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_VMWFLAG, &temp_response);

            if(SIM_SERVICE_OK == ser_result)
            {
                g_sim_r4_vm_vmwflag_len[cur_sim_task] = temp_response.freespace_filesize;
            }
            else
            {
                g_sim_r4_vm_vmwflag_len[cur_sim_task] = 0;
            }
        }

        s_sim_r4_is_vmw_info_gotten[cur_sim_task] =  TRUE;

        SCI_TRACE_LOW("SIM%d: voice message waiting info: mwis_record_len %d, mwis_record_num %d, vm_vmwflag_len %d",
                      cur_sim_task, g_sim_r4_vm_mwis_record_len[cur_sim_task], g_sim_r4_vm_mwis_record_num[cur_sim_task],
                      g_sim_r4_vm_vmwflag_len[cur_sim_task]);
    }

    //init
    SCI_MEMSET(vmwflag, 0x0, sizeof(SIM_VMW_IND_FLAG_T));
    SCI_MEMSET(temp_mwis, 0x0, (sizeof(SIM_EF_MAPPING_MWIS_T) * 2));
    SCI_MEMSET(&temp_vmwflag, 0x0, sizeof(SIM_EF_MAPPING_VMWFLAG_T));

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_MWIS))
        {
            temp_file_name = SIM_FILE_EF_USIM_MWIS;
        }
        else
        {
            return FALSE;
        }
    }
    else
#endif
    {
        temp_file_name = SIM_FILE_EF_VMWFLAG;//init
        //record information has been gained, indicating that file valid
        if(g_sim_r4_vm_mwis_record_len[cur_sim_task] > 0)
        {
            temp_file_name = SIM_FILE_EF_MWIS;
        }
        else if(g_sim_r4_vm_vmwflag_len[cur_sim_task] > 0)
        {
            temp_file_name = SIM_FILE_EF_VMWFLAG;
        }
        else
        {
            return FALSE;
        }
    }

    SCI_TRACE_LOW("SIM%d:SIMSIGNAL_ReadMWIS temp_file_name=%d", cur_sim_task, temp_file_name);

    if(SIM_FILE_EF_VMWFLAG == temp_file_name)
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, temp_file_name, &temp_vmwflag);

        if(SIM_SERVICE_OK != ser_result)
        {
            return FALSE;
        }

        if(0xA == temp_vmwflag.line1_status)
        {
            vmwflag->line1_waiting_count = 1;
        }

        if(0xA == temp_vmwflag.line2_status)
        {
            vmwflag->line2_waiting_count = 1;
        }

        if(temp_vmwflag.is_datafax_flag_present)
        {
            vmwflag->is_datafax_flag_present = temp_vmwflag.is_datafax_flag_present;

            if(0xA == temp_vmwflag.fax_status)
            {
                vmwflag->fax_waiting_count = 1;
            }

            if(0xA == temp_vmwflag.data_status)
            {
                vmwflag->data_waiting_count = 1;
            }
        }
    }
    else
    {
        for(i = 0; i < g_sim_r4_vm_mwis_record_num[cur_sim_task]; i++)
        {
            ser_result = SIMSER_ReadEFRecord(cur_sim_task, temp_file_name, (i + 1), SIM_RECORD_ABSOLUTE, &temp_mwis[i]);

            if(SIM_SERVICE_OK != ser_result)
            {
                break;//quit
            }
        }

        if(SIM_SERVICE_OK == ser_result)
        {
            vmwflag->is_datafax_flag_present = TRUE;
            vmwflag->line1_waiting_count = temp_mwis[0].vm_waiting_count;
            vmwflag->fax_waiting_count = temp_mwis[0].fax_waiting_count;
            vmwflag->data_waiting_count = temp_mwis[0].data_waiting_count;
            vmwflag->other_waiting_count = temp_mwis[0].other_waiting_count;

            if(g_sim_r4_vm_mwis_record_num[cur_sim_task] > 1)
            {
                vmwflag->line2_waiting_count = temp_mwis[1].vm_waiting_count;
            }
        }
        else
        {
            return FALSE;
        }
    }

    SCI_TRACE_LOW("SIM%d:SIMSIGNAL_ReadMWIS successful, vmwflag info: %d, %d, %d, %d, %d",
                  cur_sim_task, vmwflag->line1_waiting_count, vmwflag->line2_waiting_count,
                  vmwflag->fax_waiting_count, vmwflag->data_waiting_count, vmwflag->other_waiting_count);

    return TRUE;
}

/*****************************************************************************/
//  Description : the function that process the MWIS request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC SIM_RESULT_UPDATE_FILE_E SIMSIGNAL_UpdateMWIS(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,
    SIM_VMW_IND_FLAG_T *vmwflag)
{
    SIM_FILE_NAME_E temp_file_name = file_name;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;
    SIM_EF_MAPPING_MWIS_T mwis_mapping;
    SIM_EF_MAPPING_VMWFLAG_T vmwflag_mapping;
    uint8 i = 0;
    SIM_RESULT_UPDATE_FILE_E update_result = SIM_UPDATE_FAILED_UNKNOW;

    if(!s_sim_r4_is_vmw_info_gotten[cur_sim_task])
    {
        SIM_RESPONSE_DATA_OF_SELECT_T temp_response = {0};

        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_MWIS))
        {
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_MWIS, &temp_response);

            if(SIM_SERVICE_OK == ser_result)
            {
                g_sim_r4_vm_mwis_record_len[cur_sim_task] = temp_response.dfnumber_recordlen;

                if(temp_response.dfnumber_recordlen != 0)
                {
                    g_sim_r4_vm_mwis_record_num[cur_sim_task] =
                        (uint8)(temp_response.freespace_filesize / temp_response.dfnumber_recordlen);

                    if(g_sim_r4_vm_mwis_record_num[cur_sim_task] > 2)
                    {
                        g_sim_r4_vm_mwis_record_num[cur_sim_task] = 2;
                    }
                }
            }
            else
            {
                g_sim_r4_vm_mwis_record_len[cur_sim_task] = 0;
                g_sim_r4_vm_mwis_record_num[cur_sim_task] = 0;
            }
        }
        else
        {
            g_sim_r4_vm_mwis_record_len[cur_sim_task] = 0;
            g_sim_r4_vm_mwis_record_num[cur_sim_task] = 0;
        }

        if(SIM_SERVICE_OK != ser_result)
        {
            SCI_MEMSET(&temp_response, 0, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_VMWFLAG, &temp_response);

            if(SIM_SERVICE_OK == ser_result)
            {
                g_sim_r4_vm_vmwflag_len[cur_sim_task] = temp_response.freespace_filesize;
            }
            else
            {
                g_sim_r4_vm_vmwflag_len[cur_sim_task] = 0;
            }
        }

        s_sim_r4_is_vmw_info_gotten[cur_sim_task] =  TRUE;

        SCI_TRACE_LOW("SIM%d: voice message waiting info: mwis_record_len %d, mwis_record_num %d, vm_vmwflag_len %d",
                      cur_sim_task, g_sim_r4_vm_mwis_record_len[cur_sim_task], g_sim_r4_vm_mwis_record_num[cur_sim_task],
                      g_sim_r4_vm_vmwflag_len[cur_sim_task]);
    }

    //init
    SCI_MEMSET(&mwis_mapping, 0x0, sizeof(SIM_EF_MAPPING_MWIS_T));
    SCI_MEMSET(&vmwflag_mapping, 0x0, sizeof(SIM_EF_MAPPING_VMWFLAG_T));

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_MWIS))
        {
            temp_file_name = SIM_FILE_EF_USIM_MWIS;
        }
        else
        {
            return update_result;
        }
    }
    else
#endif
    {
        temp_file_name = SIM_FILE_EF_VMWFLAG;//init
        //record information has been gained, indicating that file valid
        if(g_sim_r4_vm_mwis_record_len[cur_sim_task] > 0)
        {
            temp_file_name = SIM_FILE_EF_MWIS;
        }
        else if(g_sim_r4_vm_vmwflag_len[cur_sim_task] > 0)
        {
            temp_file_name = SIM_FILE_EF_VMWFLAG;
        }
        else
        {
            return update_result;
        }
    }

    SCI_TRACE_LOW("SIM%d:SIMSIGNAL_UpdateMWIS temp_file_name=%d, VMW_IND_FLAG: %d,%d,%d,%d,%d",
                  cur_sim_task, temp_file_name, vmwflag->line1_waiting_count, vmwflag->line2_waiting_count,
                  vmwflag->fax_waiting_count, vmwflag->data_waiting_count, vmwflag->other_waiting_count);

    update_result = SIM_UPDATE_FAILED_UNKNOW;//init

    if(SIM_FILE_EF_VMWFLAG == temp_file_name)
    {
        if(vmwflag->line1_waiting_count >= 1)
        {
            vmwflag_mapping.line1_status = 0xA;
        }
        else
        {
            vmwflag_mapping.line1_status = 0x5;
        }

        if(vmwflag->line2_waiting_count >= 1)
        {
            vmwflag_mapping.line2_status = 0xA;
        }
        else
        {
            vmwflag_mapping.line2_status = 0x5;
        }

        if(vmwflag->is_datafax_flag_present)
        {
            vmwflag_mapping.is_datafax_flag_present = vmwflag->is_datafax_flag_present;

            if(vmwflag->fax_waiting_count >= 1)
            {
                vmwflag_mapping.fax_status = 0xA;
            }
            else
            {
                vmwflag_mapping.fax_status = 0x5;
            }

            if(vmwflag->data_waiting_count >= 1)
            {
                vmwflag_mapping.data_status = 0xA;
            }
            else
            {
                vmwflag_mapping.data_status = 0x5;
            }
        }

        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, temp_file_name, &vmwflag_mapping);
    }
    else
    {
        for(i = 0; i < g_sim_r4_vm_mwis_record_num[cur_sim_task]; i++)
        {
            if(0 == i) //line1
            {
                mwis_mapping.vm_waiting_count = vmwflag->line1_waiting_count;
                mwis_mapping.fax_waiting_count = vmwflag->fax_waiting_count;
                mwis_mapping.data_waiting_count = vmwflag->data_waiting_count;
                mwis_mapping.other_waiting_count = vmwflag->other_waiting_count;
            }
            else//line2
            {
                mwis_mapping.vm_waiting_count = vmwflag->line2_waiting_count;
                mwis_mapping.fax_waiting_count = 0;
                mwis_mapping.data_waiting_count = 0;
                mwis_mapping.other_waiting_count = 0;
            }

            ser_result = SIMSER_UpdateEFRecord(cur_sim_task, temp_file_name, (i + 1), SIM_RECORD_ABSOLUTE, &mwis_mapping, FALSE);

            if(SIM_SERVICE_OK != ser_result)
            {
                break;//quit
            }
        }
    }

    if(SIM_SERVICE_OK == ser_result)
    {
        update_result = SIM_UPDATE_OK;
    }

    return update_result;
}

/*****************************************************************************/
//  Description : the function that process the READ SIM Transparent EF request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_ReadTransparentEFInRawData(uint32 cur_sim_task, SIM_SIG_READ_RAW_T_FILE_REQ_T *psig)
{
#ifdef MODEM_PLATFORM
#ifdef _SUPPORT_IPC_
    uint16 file_size = 0;
    uint16 data_len = 0;
    uint8 *data_ptr = NULL;
    SIM_SERVICE_RESULT_E result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_READ_RAW_T_FILE_CNF_T *pcnf;
    SIM_FILE_NAME_E file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_ptr);

    SCI_CREATE_SIGNAL(pcnf, SIM_READ_RAW_T_FILE_CNF, (uint16)(sizeof(SIM_READ_RAW_T_FILE_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->file_id = psig->file_id;
    pcnf->result = SIM_READ_FAILED_UNKNOW;
    pcnf->data_len = 0;

    result = SIMSER_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK == result)
    {
        if(0 == select_data.filecharacter_efstructure) //It's not a transparent EF
        {
            file_size = select_data.freespace_filesize;

            if(file_size > 765) //wuding: we support 255*3 now.
            {
                file_size = 765;
            }

            data_ptr = SCI_ALLOC_BASE(file_size);
            //To get the RAW data
            result = SIMINSTR_ReadEF(cur_sim_task, file_name, 0, 0, SIM_RECORD_ABSOLUTE, &file_size, &data_len, data_ptr);

            if(SIM_SERVICE_OK == result)
            {
                pcnf->result = SIM_READ_OK;

                if(data_len > 765) //wuding: we support 255*3 now.
                {
                    data_len = 765;
                }

                if(NULL != data_ptr) //double check
                {
                    pcnf->data_len = data_len;
                    pcnf->data_ptr = data_ptr;
                }
            }
            else
            {
                SCI_FREE(data_ptr);//FAIL, not useful
            }
        }
    }

    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
#endif
#endif
}

/*****************************************************************************/
//  Description : the function that process the UPDATE SIM Transparent EF request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_UpdateTransparentEFInRawData(uint32 cur_sim_task, SIM_SIG_UPDATE_RAW_T_FILE_REQ_T *psig)
{
#ifdef MODEM_PLATFORM
#ifdef _SUPPORT_IPC_
    SIM_SERVICE_RESULT_E result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_UPDATE_RAW_T_FILE_CNF_T *pcnf;
    SIM_FILE_NAME_E file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_ptr);

    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_RAW_T_FILE_CNF, (uint16)(sizeof(SIM_UPDATE_RAW_T_FILE_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->file_id = psig->file_id;
    pcnf->result = SIM_UPDATE_FAILED_UNKNOW;

    result = SIMSER_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK == result)
    {
        if(0 == select_data.filecharacter_efstructure) //It's not a transparent EF
        {
            if((0 != psig->data_len) && (NULL != psig->data_ptr))
            {
                result = SIMINSTR_UpdateEF(cur_sim_task, file_name, 0, 0, SIM_RECORD_ABSOLUTE, psig->data_len, psig->data_ptr);

                if(SIM_SERVICE_OK == result)
                {
                    pcnf->result = SIM_UPDATE_OK;
                }
            }
        }
    }

    //Wuding: Should I FREE the psig->data_ptr and psig->path_ptr here ???

    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
#endif
#endif
}

/*****************************************************************************/
//  Description : the function that process the READ SIM Record EF request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_ReadRecordEFInRawData(uint32 cur_sim_task, SIM_SIG_READ_RAW_R_FILE_REQ_T *psig)
{
#ifdef MODEM_PLATFORM
#ifdef _SUPPORT_IPC_
    uint16 record_len = 0;
    uint16 data_len = 0;
    uint8 *data_ptr = NULL;
    SIM_SERVICE_RESULT_E result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_READ_RAW_R_FILE_CNF_T *pcnf;
    SIM_FILE_NAME_E file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_id);

    SCI_CREATE_SIGNAL(pcnf, SIM_READ_RAW_R_FILE_CNF, (uint16)(sizeof(SIM_READ_RAW_R_FILE_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->file_id = psig->file_id;
    pcnf->result = SIM_READ_FAILED_UNKNOW;
    pcnf->data_len = 0;
    pcnf->rec_num = psig->rec_num;

    result = SIMSER_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK == result)
    {
        if(0 != select_data.filecharacter_efstructure) //It's not a Record/Cyclic EF
        {
            record_len = select_data.dfnumber_recordlen;

            if(record_len > 255) //double check
            {
                record_len = 255;
            }

            data_ptr = SCI_ALLOC_BASE(record_len);
            //To get the RAW data
            result = SIMINSTR_ReadEF(cur_sim_task, file_name, 0, psig->rec_num, SIM_RECORD_ABSOLUTE, &record_len, &data_len, data_ptr);

            if(SIM_SERVICE_OK == result)
            {
                pcnf->result = SIM_READ_OK;

                if(data_len > 255) //double check
                {
                    data_len = 255;
                }

                if(NULL != data_ptr) //double check
                {
                    pcnf->data_len = MIN(psig->rec_len, data_len); //Maybe the called didnot want to get more data
                    pcnf->data_ptr = data_ptr;
                }
            }
            else
            {
                SCI_FREE(data_ptr);//FAIL, not useful
            }
        }
    }

    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
#endif
#endif
}

/*****************************************************************************/
//  Description : the function that process the UPDATE SIM Record EF request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_UpdateRecordEFInRawData(uint32 cur_sim_task, SIM_SIG_UPDATE_RAW_R_FILE_REQ_T *psig)
{
#ifdef MODEM_PLATFORM
#ifdef _SUPPORT_IPC_
    SIM_SERVICE_RESULT_E result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_UPDATE_RAW_R_FILE_CNF_T *pcnf;
    SIM_FILE_NAME_E file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_id);

    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_RAW_R_FILE_CNF, (uint16)(sizeof(SIM_UPDATE_RAW_R_FILE_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->file_id = psig->file_id;
    pcnf->result = SIM_UPDATE_FAILED_UNKNOW;
    pcnf->rec_num = psig->rec_num;

    result = SIMSER_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK == result)
    {
        if(0 != select_data.filecharacter_efstructure) //It's a Record/Cyclic EF
        {
            if((0 != psig->rec_len) && (NULL != psig->data_ptr))
            {
                result = SIMINSTR_UpdateEF(cur_sim_task, file_name, 0, psig->rec_num, SIM_RECORD_ABSOLUTE, psig->rec_len, psig->data_ptr);

                if(SIM_SERVICE_OK == result)
                {
                    pcnf->result = SIM_UPDATE_OK;
                }
            }
        }
    }

    //Wuding: Should I FREE the psig->data_ptr and psig->path_ptr here ???

    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
#endif
#endif
}

/*****************************************************************************/
//  Description : the function that process the SEARCH request
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_Search(uint32 cur_sim_task, SIM_SIG_SEARCH_R_FILE_REQ_T *psig)
{
#ifdef MODEM_PLATFORM
#ifdef _SUPPORT_IPC_
    SIM_SERVICE_RESULT_E result;
    SIM_SEEK_TYPE_E search_type;
    SIM_SEEK_MODE_E search_mode;
    uint8 sim_search_result_len = 0;
    uint8 temp_search_result_buf[255] = {0};
    SIM_SEARCH_RECORD_CNF_T *pcnf;
    SIM_FILE_NAME_E file_name = SIMFS_GetFileName(cur_sim_task, psig->file_id, psig->path_len, psig->path_ptr);

    SCI_CREATE_SIGNAL(pcnf, SIM_SEARCH_R_FILE_CNF, (uint16)(sizeof(SIM_SEARCH_RECORD_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->file_id = psig->file_id;
    pcnf->is_ok = FALSE;
    pcnf->search_result_len = 0;

    search_type = SIM_SEEK_TYPE_2;

    search_mode = SIM_SEEK_BEGINNING_FORWARD;

    result = SIMSER_SeekInFile(cur_sim_task, file_name, search_type, search_mode, psig->search_string_len, psig->search_string_ptr, &sim_search_result_len, temp_search_result_buf);

    if(SIM_SERVICE_OK == result)
    {
        if(0 != sim_search_result_len)
        {
            pcnf->search_result_ptr = SCI_ALLOC_BASE(sim_search_result_len);

            if(NULL != pcnf->search_result_ptr)
            {
                pcnf->is_ok = TRUE;
                pcnf->search_result_len = sim_search_result_len;
                SCI_MEMCPY(pcnf->search_result_ptr, temp_search_result_buf, pcnf->search_result_len);
            }
        }
    }

    //Wuding: Should I FREE the psig->data_ptr and psig->path_ptr here ???

    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
#endif
#endif
}

/*****************************************************************************/
//  Description : the function that change the STK Terminal Profile API
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
STATIC void SIMSIGNAL_ChangeSTKTerminalProfile(SIM_TERMINAL_PROFILE_DATA_T* profile_ptr)
{
	//According to the SPRD's SMALLEST-STK-PICS (remove IMG/ICON/OpenChannel related).
	//The array format is 3GPP spec 11.14 5.2 Structure and coding of TERMINAL PROFILE

#ifdef GSM_DPHONE_SUPPORT
#if 0//Only for Jiangsu CMCC Dphone SIM card
    //special Jiangsu Dphone CMCC card
    // 1 byte manufactory code
    profile_ptr->profile_arr[profile_ptr->profile_len] = 0x0;/**/
	profile_ptr->profile_len++;
	
	// 8 bytes random values
	profile_ptr->profile_arr[profile_ptr->profile_len] = 0x0;/**/
	profile_ptr->profile_len++;
	profile_ptr->profile_arr[profile_ptr->profile_len] = 0x0;/**/
	profile_ptr->profile_len++;
	profile_ptr->profile_arr[profile_ptr->profile_len] = 0x0;/**/
	profile_ptr->profile_len++;
	profile_ptr->profile_arr[profile_ptr->profile_len] = 0x0;/**/
	profile_ptr->profile_len++;
	profile_ptr->profile_arr[profile_ptr->profile_len] = 0x0;/**/
	profile_ptr->profile_len++;
	profile_ptr->profile_arr[profile_ptr->profile_len] = 0x0;/**/
	profile_ptr->profile_len++;
	profile_ptr->profile_arr[profile_ptr->profile_len] = 0x0;/**/
	profile_ptr->profile_len++;
	profile_ptr->profile_arr[profile_ptr->profile_len] = 0x0;/**/
	
	profile_ptr->profile_len++;//array index + 1 = length

	SCI_TRACE_LOW("SIM: SIMSIGNAL_ChangeSTKTerminalProfile len=%d",profile_ptr->profile_len);
#endif
#else
#if 0
	profile_ptr->profile_arr[5] &= ~(BIT_3|BIT_2|BIT_1);//clear BrowserTermination/ChannelStatus/DataAvailable
	
	profile_ptr->profile_arr[8] &= ~BIT_6;//clear LaunchBrowser

	profile_ptr->profile_arr[11] = 0;//clear all in this byte

	profile_ptr->profile_arr[12] = 0;//clear all in this byte

	profile_ptr->profile_arr[16] = 0;//clear all (TCP/UDP)
#endif
#endif

	return;
}

#ifdef MODEM_PLATFORM
/*****************************************************************************/
//  Description : the function that process the CSIM AT command
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
void SIMSIGNAL_CSIM(
    uint32 cur_sim_task,
    SIM_SIG_CSIM_REQ_T  *psig
)
{
    SIM_SIG_CSIM_CNF_T *pcnf = PNULL;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;
    SIM_RSP_INFO_T rsp_info = {0};
	uint8 temp_len = 0;
	//SIM_RESPONSE_DATA_OF_SELECT_T select_response;
	static SIM_FILE_NAME_E s_sim_csim_cur_file = SIM_FILE_MF;

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SIG_CSIM_CNF, sizeof(SIM_SIG_CSIM_CNF_T), g_sim_task_id[cur_sim_task]);

	if (0 != psig->data_len)
	{
#if 0//CMCC delete
		SCI_TRACE_LOW("SIMSIGNAL_CSIM cur_file=%d, %d",\
			s_sim_csim_cur_file,SIMFS_GetCurFileName(cur_sim_task));

		if (s_sim_csim_cur_file == SIMFS_GetCurFileName(cur_sim_task)) 
		{
			ser_result = SIM_SERVICE_OK;
		}
		else
		{
			ser_result = SIMSER_SelectFile(cur_sim_task,s_sim_csim_cur_file,&select_response);
		}

		if (SIM_SERVICE_OK == ser_result)
		{
			if (0xC0 == psig->data_ptr[1])
			{
				//do nothing
			}
			else
			{
#endif

				ser_result = SIMINSTR_CSIM(cur_sim_task,psig->data_len,psig->data_ptr);

#if 0
			}
		}
#endif
	}

    if ((SIM_SERVICE_RX_PARITY_ERROR == ser_result) 
        || (SIM_SERVICE_TX_PARITY_ERROR == ser_result) 
        || (SIM_SERVICE_CARD_OUT == ser_result) 
        || (SIM_SERVICE_CARD_UNRESPONSE == ser_result)
        || (SIM_SERVICE_ERROR_UNKNOW == ser_result))
    {
        pcnf->result = FALSE;
        pcnf->data_len = 0;
    }
    else
    {
    	if ((SIM_SERVICE_OK == ser_result) && (0xA4 == psig->data_ptr[1]))
    	{
			s_sim_csim_cur_file = SIMFS_GetCurFileName(cur_sim_task);
    	}

        SIMINSTR_CRSM_GetResponseInfo(cur_sim_task, &rsp_info);

		SCI_TRACE_LOW("SIM CSIM resposne. cur_file=%d, len=%d, 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x",\
			s_sim_csim_cur_file,\
			rsp_info.sw_rsp.data_len,rsp_info.sw_rsp.sw1,rsp_info.sw_rsp.sw2,\
			rsp_info.sw_rsp.data[0],rsp_info.sw_rsp.data[1],rsp_info.sw_rsp.data[2],rsp_info.sw_rsp.data[3]);

		pcnf->result = TRUE;
		temp_len = rsp_info.sw_rsp.data_len;
		if (temp_len > 253)
	    {
	    	temp_len = 253;
	    }
		SCI_MEMCPY(pcnf->rsp_data,rsp_info.sw_rsp.data,temp_len);
		pcnf->data_len = temp_len + 2;
		pcnf->rsp_data[pcnf->data_len-2] = rsp_info.sw_rsp.sw1;
		pcnf->rsp_data[pcnf->data_len-1] = rsp_info.sw_rsp.sw2;
    }

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);

    return;
}
#endif
#ifdef   __cplusplus
}
#endif
