/******************************************************************************
** File Name:      sim_mm_signal.c                                           *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the C file of common sim signal from mm layer     *
**                 process functions                                         *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/
#include "base_trc.h"
#include "sim_include.h"
#include "nvitem.h"                //need by EFS_NvitemRead
#include "nv_item_id.h"          //need by NV_SIM_IMSI
#include "ps_nvitem_id.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

//To indicate where the Kc_gprs stored. In SIM(which support Kc_gprs) or in NV.
//Sometimes the RF-noise will make the kcgprs cannot be written into SIM.
#ifdef MULTI_SIM_SYS_QUAD
#ifdef _SUPPORT_GPRS_
static BOOLEAN s_sim_is_kcgprs_nv_temply[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
static BOOLEAN s_sim_in_gmm_auth_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#endif
static BOOLEAN s_sim_in_auth_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#elif defined (MULTI_SIM_SYS_TRI)
#ifdef _SUPPORT_GPRS_
static BOOLEAN s_sim_is_kcgprs_nv_temply[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
static BOOLEAN s_sim_in_gmm_auth_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#endif
static BOOLEAN s_sim_in_auth_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#else
#ifdef _SUPPORT_GPRS_
static BOOLEAN s_sim_is_kcgprs_nv_temply[SIM_CARD_NUM] = {FALSE, FALSE};
static BOOLEAN s_sim_in_gmm_auth_flag[SIM_CARD_NUM] = {FALSE, FALSE};
#endif
static BOOLEAN s_sim_in_auth_flag[SIM_CARD_NUM] = {FALSE, FALSE};
#endif

/**********************************************************************
//    Description:
//      Compare two IMSI to chech whether the two IMSI is the same
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC BOOLEAN CompareIMSI(//return value: True,the two IMSI is the same
    //              False,the two IMSI is different
    const SIM_EF_MAPPING_IMSI_T *pimsi1, //the first IMSI
    const SIM_EF_MAPPING_IMSI_T *pimsi2  //the second IMSI
);

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
//  Description : Read the KcGPRS information from the NV
//  Global resource dependence : none
//  Author:    Tracy Zhang
//  Note:
/*****************************************************************************/
STATIC void ReadKcGPRSFromNV(
    uint32         cur_sim_task,   //IN:
    GMM_CKSN_E     *cksn_ptr,      //OUT: Ciphering key sequence number
    GPRS_KC_ARR_T  *kc_ptr         //OUT: Ciphering Key Kc
);
#endif

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
//  Description : Read the LociGPRS information from the NV
//  Global resource dependence : none
//  Author:    Tracy Zhang
//  Note:
/*****************************************************************************/
STATIC void ReadLociGPRSFromNV(
    uint32                   cur_sim_task,           //IN:
    RAI_T                    *rai_ptr,               //OUT:
    P_TMSI_T                 *p_tmsi_ptr,            //OUT:
    P_TMSI_SIGNATURE_T       *p_tmsi_signature_ptr,  //OUT:
    SIM_R_UPDATE_STATUS_E    *update_status_ptr      //OUT:
);
#endif

#ifdef _SUPPORT_GPRS_
/**********************************************************************
//    Description:
//      Clear the GPRS information saved in the NV,so these information
//    can be ignored for the new SIM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ClearNVGPRSInfo(uint32 cur_sim_task);
#endif

/**********************************************************************
//    Description:
//      Decode the auth response data,see TS 31.102 7.1.2.1
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
void SIMMMSIG_DecodeAuthResp(uint32 cur_sim_task,
                             uint8 rsp_data_len,
                             uint8 *auth_resp_data, 
                             BOOLEAN is_gsm_context,
                             SIM_AUTH_RESPONSE_T *auth_response);

/**********************************************************************
//    Description:
//      Compate two IMSI to chekc whether the two IMSI is the same
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC BOOLEAN CompareIMSI(//return value: True,the two IMSI is the same
    //              False,the two IMSI is different
    const SIM_EF_MAPPING_IMSI_T *pimsi1, //the first IMSI
    const SIM_EF_MAPPING_IMSI_T *pimsi2  //the second IMSI
)
{
    uint8 i;

    //the length of the imsi is not same
    if(pimsi1->imsi.length != pimsi2->imsi.length)
    {
        return FALSE;
    }

    //check the content of the two imsi
    for(i = 0; i < pimsi1->imsi.length; i++)
    {
        if(pimsi1->imsi.imsi_value[i] != pimsi2->imsi.imsi_value[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : get NV IMSI Id
//  Global resource dependence : none
//  Author:    Tracy Zhang
//  Note:
/*****************************************************************************/
STATIC uint16 GetNvImsiId(
    uint32    cur_sim_task
)
{
    uint16                      nv_item_name = 0;

    switch(cur_sim_task)
    {
        case 0:
            nv_item_name = NV_SIM_IMSI;
            break;

        case 1:
            nv_item_name = NV_SIM_IMSI_1;
            break;

        case 2:
            nv_item_name = NV_SIM_IMSI_2;
            break;

        case 3:
            nv_item_name = NV_SIM_IMSI_3;
            break;

        default:
            break;
    }

    return (nv_item_name);
}

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
//  Description : get NV GPRS kc Id
//  Global resource dependence : none
//  Author:    Tracy Zhang
//  Note:
/*****************************************************************************/
STATIC uint16 GetNvGprsKcId(
    uint32    cur_sim_task
)
{
    uint16                      nv_item_name = 0;

    switch(cur_sim_task)
    {
        case 0:
            nv_item_name = NV_SIM_GPRS_KC;
            break;

        case 1:
            nv_item_name = NV_SIM_GPRS_KC_1;
            break;

        case 2:
            nv_item_name = NV_SIM_GPRS_KC_2;
            break;

        case 3:
            nv_item_name = NV_SIM_GPRS_KC_3;
            break;
        default:
            break;
    }

    return (nv_item_name);
}
#endif

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
//  Description : get NV GPRS Loci Id
//  Global resource dependence : none
//  Author:    Tracy Zhang
//  Note:
/*****************************************************************************/
STATIC uint16 GetNvGprsLociId(
    uint32    cur_sim_task
)
{
    uint16                      nv_item_name = 0;

    switch(cur_sim_task)
    {
        case 0:
            nv_item_name = NV_SIM_GPRS_LOCI;
            break;

        case 1:
            nv_item_name = NV_SIM_GPRS_LOCI_1;
            break;

        case 2:
            nv_item_name = NV_SIM_GPRS_LOCI_2;
            break;

        case 3:
            nv_item_name = NV_SIM_GPRS_LOCI_3;
            break;

        default:
            break;
    }

    return (nv_item_name);
}
#endif

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
//  Description : Read the KcGPRS information from the NV
//  Global resource dependence : none
//  Author:    Tracy Zhang
//  Note:
/*****************************************************************************/
STATIC void ReadKcGPRSFromNV(
    uint32         cur_sim_task,   //IN:
    GMM_CKSN_E     *cksn_ptr,      //OUT: Ciphering key sequence number
    GPRS_KC_ARR_T  *kc_ptr         //OUT: Ciphering Key Kc
)
{
    SIM_EF_MAPPING_KC_GPRS_T    kc_gprs;
    SIM_EF_MAPPING_IMSI_T       nv_imsi;
    NVITEM_ERROR_E              nv_result    = NVERR_NONE;
    uint16                      nv_item_name = 0;

    // init
    // "key not available"
    *cksn_ptr   = 0x07;
    SCI_MEMSET(kc_ptr, 0, sizeof(GPRS_KC_ARR_T));

    //read the IMSI in NV
    nv_item_name = GetNvImsiId(cur_sim_task);
    nv_result = EFS_NvitemRead(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_IMSI_T)), (uint8 *)&nv_imsi);

    if((NVERR_NONE == nv_result)
        && CompareIMSI(&nv_imsi, &(g_sim_card_setting[cur_sim_task].imsi_mapping)))
    {
        nv_item_name    = GetNvGprsKcId(cur_sim_task);
        nv_result = EFS_NvitemRead(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_KC_GPRS_T)), (uint8 *)&kc_gprs);

        if(NVERR_NONE == nv_result)
        {
            SCI_MEMCPY(kc_ptr, &(kc_gprs.kc), sizeof(GPRS_KC_ARR_T));
            *cksn_ptr   = kc_gprs.cksn;
        }
        else
        {
            SCI_MEMSET(&kc_gprs, 0, sizeof(SIM_EF_MAPPING_KC_GPRS_T));
            kc_gprs.cksn    = 0x07;
            EFS_NvitemWrite(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_KC_GPRS_T)), (uint8 *)&kc_gprs, TRUE);
        }
    }
    else
    {
        EFS_NvitemWrite(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_IMSI_T)),
                        (uint8 *)&(g_sim_card_setting[cur_sim_task].imsi_mapping), TRUE);
        ClearNVGPRSInfo(cur_sim_task);
    }
}
#endif

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
//  Description : Read the LociGPRS information from the NV
//  Global resource dependence : none
//  Author:    Tracy Zhang
//  Note:
/*****************************************************************************/
STATIC void ReadLociGPRSFromNV(
    uint32                   cur_sim_task,           //IN:
    RAI_T                    *rai_ptr,               //OUT:
    P_TMSI_T                 *p_tmsi_ptr,            //OUT:
    P_TMSI_SIGNATURE_T       *p_tmsi_signature_ptr,  //OUT:
    SIM_R_UPDATE_STATUS_E    *update_status_ptr      //OUT:
)
{
    SIM_EF_MAPPING_LOCI_GPRS_T  loci_gprs;
    SIM_EF_MAPPING_IMSI_T       nv_imsi;
    NVITEM_ERROR_E              nv_result    = NVERR_NONE;
    uint16                      nv_item_name = 0;

    // init
    *update_status_ptr       = SIM_R_NOT_UPDATED;
    SCI_MEMSET(p_tmsi_ptr, 0, sizeof(P_TMSI_T));

    //read the IMSI in NV
    nv_item_name = GetNvImsiId(cur_sim_task);
    nv_result = EFS_NvitemRead(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_IMSI_T)), (uint8 *)&nv_imsi);

    if((NVERR_NONE == nv_result)
        && CompareIMSI(&nv_imsi, &(g_sim_card_setting[cur_sim_task].imsi_mapping)))
    {
        nv_item_name = GetNvGprsLociId(cur_sim_task);
        nv_result = EFS_NvitemRead(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_LOCI_GPRS_T)), (uint8 *)&loci_gprs);

        if(NVERR_NONE == nv_result)
        {
            *p_tmsi_ptr             = loci_gprs.p_tmsi;
            *p_tmsi_signature_ptr   = loci_gprs.p_tmsi_signature;
            *rai_ptr                = loci_gprs.rai;
            *update_status_ptr      = loci_gprs.update_status;
        }
        else
        {
            SCI_MEMSET(&loci_gprs, 0, sizeof(SIM_EF_MAPPING_LOCI_GPRS_T));
            loci_gprs.update_status  = SIM_R_NOT_UPDATED;
            EFS_NvitemWrite(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_LOCI_GPRS_T)), (uint8 *)&loci_gprs, TRUE);
        }
    }
    else
    {
        EFS_NvitemWrite(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_IMSI_T)),
                        (uint8 *)&(g_sim_card_setting[cur_sim_task].imsi_mapping), TRUE);
        ClearNVGPRSInfo(cur_sim_task);
    }
}
#endif

#ifdef _SUPPORT_GPRS_
/**********************************************************************
//    Description:
//      Clear the GPRS information saved in the NV,so these information
//    can be ignored for the new SIM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ClearNVGPRSInfo(uint32 cur_sim_task)
{
    SIM_EF_MAPPING_KC_GPRS_T    kc_gprs;
    SIM_EF_MAPPING_LOCI_GPRS_T  loci_gprs;
    uint16 nv_item_name = 0;

    SCI_MEMSET(&kc_gprs, 0x0, sizeof(SIM_EF_MAPPING_KC_GPRS_T));
    SCI_MEMSET(&loci_gprs, 0x0, sizeof(SIM_EF_MAPPING_LOCI_GPRS_T));
    //clear the pervious GPRS information
    //the loci information update status is not updated,so this information
    //content can be ignored
    loci_gprs.update_status = SIM_R_NOT_UPDATED;
    nv_item_name            = GetNvGprsLociId(cur_sim_task);
    EFS_NvitemWrite(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_LOCI_GPRS_T)), (uint8 *)&loci_gprs, TRUE);
    //the cksn of the kc file is "key not avaliabled" so the kc information
    //can be ignored
    kc_gprs.cksn = 0x07;

    nv_item_name = GetNvGprsKcId(cur_sim_task);

    EFS_NvitemWrite(nv_item_name, (uint16)(sizeof(SIM_EF_MAPPING_KC_GPRS_T)), (uint8 *)&kc_gprs, TRUE);
}
#endif

/**********************************************************************
//    Description:
//      the MM DATA request signal process function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_MMDataReq(
    uint32              cur_sim_task,
    xSignalHeaderRec    *psig           //the MM_data request signal
)
{
    yPDef_MM_SIM_DATA_CNF       *pcnf           = PNULL;
    SIM_SERVICE_RESULT_E        ser_result;
    SIM_EF_MAPPING_KC_T         kc;
#ifdef _SUPPORT_GPRS_
    SIM_EF_MAPPING_KC_GPRS_T    kc_gprs;
    SIM_EF_MAPPING_LOCI_GPRS_T  loci_gprs;
#endif

    //create the confirm signal of the request signal
    SCI_CREATE_SIGNAL(pcnf, MM_SIM_DATA_CNF, (uint16)(sizeof(yPDef_MM_SIM_DATA_CNF)), g_sim_task_id[cur_sim_task]);

    //read the content of the EF_KC
    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KC, &kc);

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = SIM_MM_DATA_KC_ERROR;
        goto MM_DATA_CONFIRM;
    }

    //CKSN_T
    SCI_MEMCPY(&(pcnf->mm_cksn), &(kc.cksn), sizeof(CKSN_T));
    //KC_T
    SCI_MEMCPY(&(pcnf->mm_kc), &(kc.kc),   sizeof(KC_T));

#ifdef _SUPPORT_GPRS_

    //if service of GPRS of the SIM card been allocated and actived,read the EF_KcGPRS
    //and EF_LOCIGPRS
    if((g_sim_card_setting[cur_sim_task].service_table.service_num > SIMSER_PH2_GPRS)
        && (SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GPRS)))
    {
        //read the content of EF_KcGPRS
        if(s_sim_is_kcgprs_nv_temply[cur_sim_task])
        {
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"SIM%d::Read KcGPRS from NV temply."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_410_112_2_17_22_59_47_4586, (uint8 *)"d", cur_sim_task);
#endif
            ReadKcGPRSFromNV(cur_sim_task, &(pcnf->gmm_cksn), &(pcnf->gmm_kc));
        }
        else
        {
            ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KcGPRS, &kc_gprs);

            if(SIM_SERVICE_OK != ser_result)
            {
                if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
                {
#ifndef _LESSEN_TRACE_SUPPORT
                    //SCI_TRACE_LOW:"SIM::Read KcGPRS from NV."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_422_112_2_17_22_59_47_4587, (uint8 *)"");
#endif
                    ReadKcGPRSFromNV(cur_sim_task, &(pcnf->gmm_cksn), &(pcnf->gmm_kc));
                }
                else
                {
                    pcnf->result = SIM_MM_DATA_KC_GPRS_ERROR;
                    goto MM_DATA_CONFIRM;
                }
            }
            else
            {
                pcnf->gmm_cksn = kc_gprs.cksn;
                SCI_MEMCPY(&(pcnf->gmm_kc), &(kc_gprs.kc), sizeof(GPRS_KC_ARR_T));
            }
        }

        //read the content of the EF_LOCI_GPRS
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LOCIGPRS, &loci_gprs);

        if(SIM_SERVICE_OK != ser_result)
        {
            if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
            {
#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM::Read LociGPRS from NV."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_446_112_2_17_22_59_47_4588, (uint8 *)"");
#endif
                ReadLociGPRSFromNV(cur_sim_task, &(pcnf->cur_rai), &(pcnf->ptimsi), &(pcnf->ptmsi_signature), (SIM_R_UPDATE_STATUS_E *)(&(pcnf->update_state)));
            }
            else
            {
                pcnf->result = SIM_MM_DATA_LOCI_GPRS_ERROR;
                goto MM_DATA_CONFIRM;
            }
        }
        else
        {
            // MM module will check the PLMN is valid
#if 0
            if((2 != loci_gprs.rai.mnc_digit_num)
                && (3 != loci_gprs.rai.mnc_digit_num))
            {
                loci_gprs.rai.mcc           = g_sim_card_setting[cur_sim_task].loci_info.lai.mcc;
                loci_gprs.rai.mnc           = g_sim_card_setting[cur_sim_task].loci_info.lai.mnc;
                loci_gprs.rai.mnc_digit_num = g_sim_card_setting[cur_sim_task].loci_info.lai.mnc_digit_num;
                loci_gprs.rai.lac           = g_sim_card_setting[cur_sim_task].loci_info.lai.lac;
                //SCI_TRACE_LOW:"SIM:no valid rai using lai !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_467_112_2_17_22_59_47_4589, (uint8 *)"");
            }

#endif
            pcnf->cur_rai = loci_gprs.rai;
            pcnf->ptimsi = loci_gprs.p_tmsi;
            pcnf->ptmsi_signature = loci_gprs.p_tmsi_signature;
            pcnf->update_state = loci_gprs.update_status;
        }
    }
    else
    {
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM:read KcGPRS and LociGPRS infor from NV!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_479_112_2_17_22_59_47_4590, (uint8 *)"");
#endif
        ReadKcGPRSFromNV(cur_sim_task, &(pcnf->gmm_cksn), &(pcnf->gmm_kc));
        ReadLociGPRSFromNV(cur_sim_task, &(pcnf->cur_rai), &(pcnf->ptimsi), &(pcnf->ptmsi_signature), (SIM_R_UPDATE_STATUS_E *)(&(pcnf->update_state)));
    }

#endif
    //copy the all data to the confirm signal
    //the MM data request service ok
    pcnf->result = SIM_MM_DATA_OK;

MM_DATA_CONFIRM:
    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, psig->Sender);
}

/**********************************************************************
//    Description:
//      the MM AUTH request signal process function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_AuthReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_AUTH_REQ *psig  //the Auth request signal
)
{
    yPDef_MM_SIM_AUTH_CNF *pcnf;
    SIM_EF_MAPPING_KC_T kc_mapping;
    SIM_FILE_NAME_E     cur_dir;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    SIM_GSM_ALGORITHM_RESULT_T algorithm_result = {0};
    SIM_FILE_NAME_E file_to_select = SIM_FILE_EF_KC;
    BOOLEAN is_need_select = FALSE;
    uint8 rsp_data_len = 0;
    uint8 alg_data[255] = {0};
    uint8 rand_arr[17];
    uint8 autn[17];
    SIM_AUTH_RESPONSE_T auth_response;
    BOOLEAN     is_need_update_kc = FALSE;
    BOOLEAN     is_run_umts_alog = FALSE;
    BOOLEAN     is_2g_sec_context = TRUE;
    BOOLEAN     is_need_update_keys = FALSE;
    SIM_EF_MAPPING_KEYS_T   keys_mapping;

    //create the confirm signal of the request signal
    SCI_CREATE_SIGNAL(pcnf, MM_SIM_AUTH_CNF, (uint16)(sizeof(yPDef_MM_SIM_AUTH_CNF)), g_sim_task_id[cur_sim_task]);

    pcnf->command_ref = psig->command_ref;
    pcnf->kc_len = 0;
    pcnf->sres_ext_len = 0;
    pcnf->auts_len = 0;
    //in the vesion of PS in the really hardware enviroment,run the GSM algorithm
    cur_dir = SIMFS_GetCurDirectoryFile(cur_sim_task);
    s_sim_in_auth_flag[cur_sim_task] = TRUE;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
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
                    //SCI_TRACE_LOW:"SIM::begin to re-select the file"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_564_112_2_17_22_59_48_4591, (uint8 *)"");
                    ser_result = SIMSER_SelectFile(cur_sim_task, file_to_select, &select_data);
                }
                else
                {
                    ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                }
            }

            if(SIM_SERVICE_OK != ser_result)
            {
                //SCI_TRACE_LOW:"SIM::select file failed before mm auth. %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_575_112_2_17_22_59_48_4592, (uint8 *)"d", ser_result);
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
                SIM_PrintRxBuf(cur_sim_task);
#endif

                if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
                {
                    pcnf->result = SIM_MM_AUTH_FAILED;
                }
                else
                {
                    pcnf->result = SIM_MM_AUTH_SELECT_GSM_FAILED;
                }
            }
        }
    }

    //request the GSM algorithm of the SIM card
    if(SIM_SERVICE_OK == ser_result)
    {
#ifdef SIM_3G_USIM_SUPPORT
        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            rand_arr[0] = 16;
            SCI_MEMCPY(rand_arr + 1,psig->rand,16);
            
            if(psig->is_autn_exist)
            {
                is_2g_sec_context = FALSE;
                autn[0] = 16;
                SCI_MEMCPY(autn + 1,psig->autn,16);
            }
            else
            {
                is_2g_sec_context = TRUE;
            }
            
            // RUN UMTS authentication Algorithm
            ser_result = SIMINSTR_RunUMTSAlgorithm(cur_sim_task, \
                                                   is_2g_sec_context, rand_arr, autn, &rsp_data_len, alg_data);
            is_run_umts_alog = TRUE;                                                   
        }
        else
#endif        
        {
            ser_result = SIMINSTR_RunGSMAlgorithm(cur_sim_task, psig->rand, &algorithm_result);
            is_run_umts_alog = FALSE;
        }

        //zyx test
        //ser_result = SIM_SERVICE_RUN_ALGORITHM_MAC_ERROR;
    
        
        if(SIM_SERVICE_RUN_ALGORITHM_MAC_ERROR == ser_result)
        {
            pcnf->result = SIM_MM_AUTH_MAC_INCORRECT;
        }
        else if(SIM_SERVICE_OK != ser_result)
        {
            if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
            {
                //clear the Temp int signal queue
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);

                SIM_DisablePPSFeature(cur_sim_task);//A special Shenzhen CMCC sim

                //if the restroe is successful
                if(DealCardUnResponse(cur_sim_task))
                {
                    //disable the deep sleep,because the dealcardunresponse has open the deep sleep
                    DisableDeepSleep(cur_sim_task);

                    //redo the run gsm algorithm
                    if(is_run_umts_alog)
                    {
                        ser_result = SIMINSTR_RunUMTSAlgorithm(cur_sim_task, \
                                                   is_2g_sec_context, rand_arr, autn, &rsp_data_len, alg_data);
                    }
                    else
                    {
                        ser_result = SIMINSTR_RunGSMAlgorithm(cur_sim_task, psig->rand, &algorithm_result);
                    }
                }
                else
                {
                    ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                }
            }

            if(SIM_SERVICE_OK != ser_result)
            {
                //SCI_TRACE_LOW:"SIM::Run gsm algorithm failed in mm auth. %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_620_112_2_17_22_59_48_4593, (uint8 *)"d", ser_result);
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
                SIM_PrintRxBuf(cur_sim_task);
#endif

                if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
                {
                    pcnf->result = SIM_MM_AUTH_FAILED;
                }
                else
                {
                    pcnf->result = SIM_MM_AUTH_SELECT_GSM_FAILED;

#if 0
#ifdef SIM_DROP_RESTORE_SUPPORT
                    {
                        SIM_SERVICE_RESULT_E temp_ser_result = SIM_SERVICE_OK;

                        SCI_TRACE_LOW("SIM: Run gsm algorithm failed. Try again from MF.");
        		        temp_ser_result = SIMSER_SelectFile(cur_sim_task,SIM_FILE_MF,&select_data);
                        if (SIM_SERVICE_OK == temp_ser_result)
                        {
                            temp_ser_result = SIMSER_SelectFile(cur_sim_task,SIM_FILE_DF_GSM,&select_data);
                            if (SIM_SERVICE_OK == temp_ser_result)
                            {
                                temp_ser_result = SIMINSTR_RunGSMAlgorithm(cur_sim_task,psig->rand,&algorithm_result);
                                if (SIM_SERVICE_OK == temp_ser_result)
                                {
                                    ser_result = SIM_SERVICE_OK;
                                }
                            }
                        }
                    }
#endif
#endif
                }
            }
        }

        if(SIM_SERVICE_OK == ser_result)
        {
            if(is_run_umts_alog)
            {
                SIMMMSIG_DecodeAuthResp(cur_sim_task,
                             rsp_data_len,
                             alg_data, 
                             is_2g_sec_context,
                             &auth_response);
                             
                if(auth_response.auth_result == SIM_MM_AUTH_OK)
                {
                    pcnf->result = SIM_MM_AUTH_OK;
                    
                    pcnf->kc_len = auth_response.kc_len;

                    if(pcnf->kc_len != 0)
                    {
                        SCI_TRACE_LOW("SIM%d::MM auth_response.kc_len=%d",cur_sim_task,auth_response.kc_len);
                        is_need_update_kc = TRUE;
                        //update the EFkc with the new KC and new CKSN
                        SCI_MEMCPY(&(kc_mapping.kc), &(auth_response.kc), sizeof(KC_T));
                        SCI_MEMCPY(&(kc_mapping.cksn), &(psig->cksn), sizeof(CKSN_T));
                
                        SCI_MEMCPY(&(pcnf->kc), &(auth_response.kc), auth_response.kc_len);
                    }
                    
                    SCI_MEMCPY(&(pcnf->sres), &(auth_response.sres), sizeof(SRES_T));

                    if(auth_response.sres_len > 4)
                    {
                        pcnf->sres_ext_len = MIN(auth_response.sres_len - 4,12);
                        SCI_MEMCPY(&(pcnf->sres_ext), &(auth_response.sres[4]), pcnf->sres_ext_len);
                    }

                    if((auth_response.ck_len != 0) || (auth_response.ik_len != 0))
                    {
                        is_need_update_keys = TRUE;
                        SCI_MEMSET(&keys_mapping,0,sizeof(SIM_EF_MAPPING_KEYS_T));
                        //update the EFkeys with the new ck and new ik
                        SCI_MEMCPY(&(keys_mapping.ck), &(auth_response.ck), MIN(auth_response.ck_len,USIM_CK_LEN));
                        SCI_MEMCPY(&(keys_mapping.ik), &(auth_response.ik), MIN(auth_response.ik_len,USIM_IK_LEN));
                        SCI_MEMCPY(&(keys_mapping.ksi), &(psig->cksn), sizeof(CKSN_T));
                    }
                }
                else if(auth_response.auth_result == SIM_MM_AUTH_SYNC_FAILED)
                {
                    pcnf->result = SIM_MM_AUTH_SYNC_FAILED;
                    pcnf->auts_len = MIN(auth_response.sres_len,14);
                    if(pcnf->auts_len != 0)
                    {
                        SCI_MEMCPY(&(pcnf->auts), &(auth_response.sres), pcnf->auts_len);
                    }
                }
                else
                {
                    pcnf->result = SIM_MM_AUTH_FAILED;
                }
            }
            else
            {   
                is_need_update_kc = TRUE;
                pcnf->result = SIM_MM_AUTH_OK;
                SCI_MEMCPY(&(pcnf->kc), &(algorithm_result.kc), sizeof(KC_T));
                SCI_MEMCPY(&(pcnf->sres), &(algorithm_result.sers), sizeof(SRES_T));

                //update the EFkc with the new KC and new CKSN
                SCI_MEMCPY(&(kc_mapping.kc), &(algorithm_result.kc), sizeof(KC_T));
                SCI_MEMCPY(&(kc_mapping.cksn), &(psig->cksn), sizeof(CKSN_T));
            }

            //zyx test
            /*{
                uint8 i=0;
                pcnf->result = SIM_MM_AUTH_SYNC_FAILED;
                pcnf->auts_len = 14;
                
                for(i = 0; i< 14;i++)
                pcnf->auts[i] = i;
            }*/

            if(is_need_update_kc)
            {
                ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KC, &kc_mapping);
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
                SIM_PrintRxBuf(cur_sim_task);
#endif
                
#ifndef _LESSEN_TRACE_SUPPORT

                if(SIM_SERVICE_OK != ser_result)
                {
                    //We regard this as auth OK
                    //SCI_TRACE_LOW:"SIM::update kc failed in mm auth."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_651_112_2_17_22_59_48_4594, (uint8 *)"");
                }

#endif      
				//bug672495, after update the ADFusim/DFgsm-access/EFkc ok, we select 0x7fff failed, so return to MF first
                SCI_TRACE_LOW("SIM: after update ADFusim/DFgsm-access/EFkc,return to MF first.");
        		ser_result = SIMSER_SelectFile(cur_sim_task,SIM_FILE_MF,&select_data);
                if(SIM_SERVICE_OK != ser_result)
                {
                    SCI_TRACE_LOW("SIM: return to MF failed for %d mm auth.",ser_result);
                }        		        
            }

            if(is_need_update_keys)
            {
                ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_KEYS, &keys_mapping);
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
                SIM_PrintRxBuf(cur_sim_task);
#endif                   
                if(SIM_SERVICE_OK != ser_result)
                {
                    //We regard this as auth OK
                    SCI_TRACE_LOW("SIM%d::update keys failed for %d in mm auth.",cur_sim_task,ser_result);
                }
            }
        }
    }

    s_sim_in_auth_flag[cur_sim_task] = FALSE;
#ifdef SIM_DROP_RESTORE_SUPPORT
    if (SIMINSTR_GetAuthInAuthFlag(cur_sim_task))
    {
        SIMINSTR_SetAuthInAuthFlag(cur_sim_task,FALSE);//clear it
        SCI_TRACE_LOW("SIM%d: not auth cnf because of new auth",cur_sim_task);
        SCI_FREE_SIGNAL(pcnf);
    }
    else
#endif
    {
    	//send the confirm signal of the requset signal
    	SCI_SEND_SIGNAL((xSignalHeaderRec*)pcnf,((xSignalHeaderRec*)psig)->Sender);
    }
}

/**********************************************************************
//    Description:
//      the MM FPLMN update request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_FPLMNUpdateReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_FPLMN_UPD_REQ *psig  //the request signal
)
{
    SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_FPLMN, &(psig->fplmn_list));
}

/**********************************************************************
//    Description:
//      the MM Cipher update request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_CipherUpdateReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_CIPHER_UPD_REQ *psig    //the request signal
)
{
    SIM_EF_MAPPING_KC_T kc;

    //add values to the kc mapping structure to update the EF
    SCI_MEMCPY(&(kc.cksn), &(psig->cksn), sizeof(CKSN_T));
    SCI_MEMCPY(&(kc.kc), &(psig->kc), sizeof(KC_T));

    SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KC, &kc);
}

/**********************************************************************
//    Description:
//      the MM SIM status update request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_SIMStatusUpdateReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_STATUS_UPD_REQ *psig //the request signal
)
{
    SIM_EF_MAPPING_LOCI_T loci;
    SIM_SIG_LOCI_UPDATE_IND_T *ind = NULL;

    SCI_MEMSET(&loci, 0x0, sizeof(SIM_EF_MAPPING_LOCI_T));
    SCI_MEMCPY(&(loci.tmsi), &(psig->tmsi), sizeof(TMSI_T));
    SCI_MEMCPY(&(loci.lai), &(psig->lai), sizeof(LAI_T));
    loci.tmsi_time = psig->t3212_val;
    loci.update_status = psig->mm_update_status;

    SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LOCI, &loci);

    //then notify the mn to update the loci
    SCI_CREATE_SIGNAL(ind, SIM_LOCI_UPDATE_IND, (uint16)(sizeof(SIM_SIG_LOCI_UPDATE_IND_T)), g_sim_task_id[cur_sim_task]);
    SCI_MEMCPY(&(ind->loci), &loci, sizeof(SIM_EF_MAPPING_LOCI_T));
    SCI_SEND_SIGNAL(ind, g_sim_info_task_id[cur_sim_task]);
}

/**********************************************************************
//    Description:
//      the MM BAlist update request signal proceess function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMMSIG_BAListUpdateReq(
    uint32 cur_sim_task,
    yPDef_MM_SIM_BALIST_UPD_REQ *psig    //the request signal
)
{
    SIM_EF_MAPPING_BCCH_T bcch;

    SCI_MEMCPY(bcch.ba_list,&(psig->ba_list),sizeof(BA_LIST_ARR_T));
    bcch.is_usim = psig->is_usim;
    bcch.usim_netpar.camponed_arfcn = psig->usim_camponed_arfcn;
    bcch.usim_netpar.arfcn_num = MIN(psig->usim_arfcn_list.num,SIM_MAX_ARFCN_NUM);
    SCI_MEM16CPY(bcch.usim_netpar.arfcn,psig->usim_arfcn_list.arfcn_list_arr,bcch.usim_netpar.arfcn_num*sizeof(uint16));
    
    //There is no EFbcch in 3G USIM. The function will return in 3G mode now.
    SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_BCCH, &bcch);
}

#ifdef _SUPPORT_GPRS_
/**********************************************************************
//    Description:
//      the GMM authorize service request signal process function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMGMMSIG_AuthReq(
    uint32 cur_sim_task,
    yPDef_GMM_SIM_AUTH_REQ *psig    //the request signal
)
{
    yPDef_GMM_SIM_AUTH_CNF *pcnf;
    SIM_EF_MAPPING_KC_GPRS_T kc_mapping = {0};
    SIM_FILE_NAME_E     cur_dir;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    SIM_GSM_ALGORITHM_RESULT_T algorithm_result = {0};
    uint16 nv_item_name = 0;
    SIM_FILE_NAME_E file_to_select = SIM_FILE_EF_KC;
    BOOLEAN is_need_select = FALSE;
    uint8 rsp_data_len = 0;
    uint8 alg_data[255] = {0};
    uint8 rand_arr[17];
    uint8 autn[17];
    SIM_AUTH_RESPONSE_T auth_response;
    BOOLEAN     is_need_update_kcgprs = FALSE;
    BOOLEAN     is_run_umts_alog = FALSE;
    BOOLEAN     is_2g_sec_context = TRUE;
    BOOLEAN     is_need_update_keysps = FALSE;
    SIM_EF_MAPPING_KEYS_PS_T   keysps_mapping = {0};
    
    //create the confirm signal of the request signal
    SCI_CREATE_SIGNAL(pcnf, GMM_SIM_AUTH_CNF, (uint16)(sizeof(yPDef_GMM_SIM_AUTH_CNF)), g_sim_task_id[cur_sim_task]);
    pcnf->sres_ext_len = 0;
    pcnf->auts_len = 0;

    //in the vesion of PS in the really hardware enviroment,run the GSM algorithm
    cur_dir = SIMFS_GetCurDirectoryFile(cur_sim_task);
    s_sim_in_gmm_auth_flag[cur_sim_task] = TRUE;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
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
                    ser_result = SIMSER_SelectFile(cur_sim_task, file_to_select, &select_data);
                }
                else
                {
                    ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                }
            }

            if(SIM_SERVICE_OK != ser_result)
            {
                //SCI_TRACE_LOW:"SIM::select kc file failed in Gmm auth."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_820_112_2_17_22_59_48_4595, (uint8 *)"");
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
                SIM_PrintRxBuf(cur_sim_task);
#endif

                if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
                {
                    pcnf->auth_result = SIM_GMM_AUTH_FAILED;
                }
                else
                {
                    pcnf->auth_result = SIM_GMM_AUTH_SELECT_GSM_FAILED;
                }
            }
        }
    }

    if(SIM_SERVICE_OK == ser_result)
    {
    #ifdef SIM_3G_USIM_SUPPORT
        if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        {
            rand_arr[0] = 16;
            SCI_MEMCPY(rand_arr + 1,psig->rand_arr,16);
            
            if(psig->is_autn_exist)
            {
                is_2g_sec_context = FALSE;
                autn[0] = 16;
                SCI_MEMCPY(autn + 1,psig->autn,16);
               
                //zyx test mac
                //autn[15] = autn[15] + 1;
       
            }
            else
            {
                is_2g_sec_context = TRUE;
            }
            
            // RUN UMTS authentication Algorithm
            ser_result = SIMINSTR_RunUMTSAlgorithm(cur_sim_task, \
                                                   is_2g_sec_context, rand_arr, autn, &rsp_data_len, alg_data);
            is_run_umts_alog = TRUE;                                                   
        }
        else
#endif        
        {
            //request the GSM algorithm of the SIM card
            ser_result = SIMINSTR_RunGSMAlgorithm(cur_sim_task, psig->rand_arr, &algorithm_result);
            is_run_umts_alog = FALSE;
        }

        if(SIM_SERVICE_RUN_ALGORITHM_MAC_ERROR == ser_result)
        {
            pcnf->auth_result = SIM_GMM_AUTH_MAC_INCORRECT;
        }
        else if(SIM_SERVICE_OK != ser_result)
        {
            if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
            {
                //clear the Temp int signal queue
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);

                SIM_DisablePPSFeature(cur_sim_task);//A special Shenzhen CMCC sim

                //try the restore
                //if the restroe is successful
                if(DealCardUnResponse(cur_sim_task))
                {
                    //disable the deep sleep,because the dealcardunresponse has open the deep sleep
                    DisableDeepSleep(cur_sim_task);
                    //redo the run gsm algorithm
                    if(is_run_umts_alog)
                    {
                        ser_result = SIMINSTR_RunUMTSAlgorithm(cur_sim_task, \
                                                   is_2g_sec_context, rand_arr, autn, &rsp_data_len, alg_data);
                    }
                    else
                    {
                        ser_result = SIMINSTR_RunGSMAlgorithm(cur_sim_task, psig->rand_arr, &algorithm_result);
                    }                        
                }
                else
                {
                    ser_result = SIM_SERVICE_CARD_UNRESPONSE;
                }
            }

            if(SIM_SERVICE_OK != ser_result)
            {
                //SCI_TRACE_LOW:"SIM::Run gsm algorithm failed in Gmm auth."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_865_112_2_17_22_59_48_4596, (uint8 *)"");
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
                SIM_PrintRxBuf(cur_sim_task);
#endif

                if(SIM_SERVICE_CARD_UNRESPONSE == ser_result)
                {
                    pcnf->auth_result = SIM_GMM_AUTH_FAILED;
                }
                else
                {
                    pcnf->auth_result = SIM_GMM_AUTH_SELECT_GSM_FAILED;
                }
            }
        }

        if(SIM_SERVICE_OK == ser_result)
        {
            if(is_run_umts_alog)
            {
                SIMMMSIG_DecodeAuthResp(cur_sim_task,
                             rsp_data_len,
                             alg_data, 
                             is_2g_sec_context,
                             &auth_response);

                //zyx test
                /*{
                uint8 i=0;
                auth_response.auth_result = SIM_MM_AUTH_SYNC_FAILED;
                auth_response.sres_len = 14;
                for(i = 0; i< 14;i++)
                auth_response.sres[i] = i;
                }*/
    
                if(auth_response.auth_result == SIM_MM_AUTH_OK)
                {
                    pcnf->auth_result = SIM_GMM_AUTH_OK;
                    SCI_TRACE_LOW("SIM%d::GMM auth_response.kc_len=%d",cur_sim_task,auth_response.kc_len);
                    if(auth_response.kc_len != 0)
                    {
                        is_need_update_kcgprs = TRUE;
                        //update the EFkcgprs with the new KC and new CKSN
                        SCI_MEMCPY(&(kc_mapping.kc), &(auth_response.kc), sizeof(GPRS_KC_ARR_T));
                        kc_mapping.cksn = psig->gmm_cksn;
                        
                        SCI_MEMCPY(&(pcnf->kc_arr), &(auth_response.kc), MIN(auth_response.kc_len,8));
                    }
                    else
                    {
                        SCI_MEMSET(&(pcnf->kc_arr), 0, sizeof(GPRS_KC_ARR_T));
                    }
                    
                    SCI_MEMCPY(&(pcnf->res_num), &(auth_response.sres), sizeof(SRES_T));

                    if(auth_response.sres_len > 4)
                    {
                        pcnf->sres_ext_len = MIN(auth_response.sres_len - 4,12);
                        SCI_MEMCPY(&(pcnf->sres_ext), &(auth_response.sres[4]), pcnf->sres_ext_len);
                    }

                    if((auth_response.ck_len != 0) || (auth_response.ik_len != 0))
                    {
                        is_need_update_keysps = TRUE;
                        SCI_MEMSET(&keysps_mapping,0,sizeof(SIM_EF_MAPPING_KEYS_PS_T));
                        //update the EFkeysPS with the new ck and new ik
                        SCI_MEMCPY(&(keysps_mapping.ckps), &(auth_response.ck), MIN(auth_response.ck_len,USIM_CK_LEN));
                        SCI_MEMCPY(&(keysps_mapping.ikps), &(auth_response.ik), MIN(auth_response.ik_len,USIM_IK_LEN));
                        keysps_mapping.ksips = psig->gmm_cksn;
                    }
                }
                else if(auth_response.auth_result == SIM_MM_AUTH_SYNC_FAILED)
                {
                    pcnf->auth_result = SIM_GMM_AUTH_SYNC_FAILED;
                    pcnf->auts_len = auth_response.sres_len;
                    if(pcnf->auts_len != 0)
                    {
                        SCI_MEMCPY(&(pcnf->auts), &(auth_response.sres), pcnf->auts_len);
                    }
                }
                else
                {
                    pcnf->auth_result = SIM_GMM_AUTH_FAILED;
                }
            }
            else
            {   
                is_need_update_kcgprs = TRUE;
                pcnf->auth_result = SIM_GMM_AUTH_OK;
                SCI_MEMCPY(&(pcnf->kc_arr), &(algorithm_result.kc), sizeof(GPRS_KC_ARR_T));
                SCI_MEMCPY(&(pcnf->res_num), &(algorithm_result.sers), sizeof(SRES_T));

                //update the EFkc with the new KC and new CKSN
                SCI_MEMCPY(&(kc_mapping.kc), &(algorithm_result.kc), sizeof(GPRS_KC_ARR_T));
                kc_mapping.cksn = psig->gmm_cksn;
            }
            
            //to the GPRS non-ware SIM, there is no EFkcgprs file
            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GPRS) && is_need_update_kcgprs)
            {
                ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KcGPRS, &kc_mapping);

                if(SIM_SERVICE_OK != ser_result)
                {
#ifndef _LESSEN_TRACE_SUPPORT
                    //SCI_TRACE_LOW:"SIM::Update Kcgprs of SIM card failed in Gmm auth for result 0x%x."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_898_112_2_17_22_59_48_4597, (uint8 *)"d", ser_result);
#endif

                    //if no kcgprs file in the sim,so update the nv
                    nv_item_name = GetNvGprsKcId(cur_sim_task);

                    if(NVERR_NONE != EFS_NvitemWrite(nv_item_name,
                                                     (uint16)(sizeof(SIM_EF_MAPPING_KC_GPRS_T)), (uint8 *)&kc_mapping, TRUE))
                    {
                        pcnf->auth_result = SIM_GMM_AUTH_FAILED;
                    }
                    else
                    {
                        s_sim_is_kcgprs_nv_temply[cur_sim_task] = TRUE;
                    }
                }
                else
                {
                    s_sim_is_kcgprs_nv_temply[cur_sim_task] = FALSE;
                }

                //bug672495, after update the ADFusim/DFgsm-access/EFkc ok, we select 0x7fff failed, so return to MF first
                SCI_TRACE_LOW("SIM: after update ADFusim/DFgsm-access/EFkcgprs,return to MF first.");
        		ser_result = SIMSER_SelectFile(cur_sim_task,SIM_FILE_MF,&select_data);
                if(SIM_SERVICE_OK != ser_result)
                {
                    SCI_TRACE_LOW("SIM: return to MF failed for %d gmm auth.",ser_result);
                }
            }
            //save the new IMSI and the kc to the NV for the next attach use
            else if(is_need_update_kcgprs)
            {
                nv_item_name = GetNvGprsKcId(cur_sim_task);

                if(NVERR_NONE != EFS_NvitemWrite(nv_item_name,
                                                 (uint16)(sizeof(SIM_EF_MAPPING_KC_GPRS_T)), (uint8 *)&kc_mapping, TRUE))
                {
#ifndef _LESSEN_TRACE_SUPPORT
                    //SCI_TRACE_LOW:"SIM::Update Kcgprs of NV failed in Gmm auth."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_928_112_2_17_22_59_48_4598, (uint8 *)"");
#endif
                    pcnf->auth_result = SIM_GMM_AUTH_FAILED;
                }
            }

            //update EFkeysps
            if(is_need_update_keysps)
            {
                ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_KEYSPS, &keysps_mapping);
#ifndef WIN32
                SIM_PrintTxBuf(cur_sim_task);
                SIM_PrintRxBuf(cur_sim_task);
#endif                
                if(SIM_SERVICE_OK != ser_result)
                {
                    //We regard this as auth OK
                    SCI_TRACE_LOW("SIM%d::update keysPS failed for %d in Gmm auth.",cur_sim_task,ser_result);
                }
            }
        }
    }

    s_sim_in_gmm_auth_flag[cur_sim_task] = FALSE;
#ifdef SIM_DROP_RESTORE_SUPPORT
    if (SIMINSTR_GetGMMAuthInAuthFlag(cur_sim_task))
    {
        SIMINSTR_SetGMMAuthInAuthFlag(cur_sim_task,FALSE);//clear it
        SCI_TRACE_LOW("SIM%d: not GMM auth cnf because of new auth",cur_sim_task);
        SCI_FREE_SIGNAL(pcnf);
    }
    else
#endif
    {
    	//send the confirm signal of the request signal
    	SCI_SEND_SIGNAL((xSignalHeaderRec*)pcnf,((xSignalHeaderRec*)psig)->Sender);
    }
}
#endif

#ifdef _SUPPORT_GPRS_
/**********************************************************************
//    Description:
//      the GMM Save RAI information service request signal process function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMGMMSIG_SaveRAIInfo(
    uint32 cur_sim_task,
    yPDef_GMM_SIM_SAVE_RAI_INFO *psig   //the service request signal
)
{
    NVITEM_ERROR_E nv_result;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_LOCI_GPRS_T loci_gprs_mapping;
    uint16 nv_item_name = 0;

    //composit it to a EFlocigprs mapping
    SCI_MEMCPY(&(loci_gprs_mapping.p_tmsi), &(psig->ptmsi), sizeof(P_TMSI_T));
    SCI_MEMCPY(&(loci_gprs_mapping.p_tmsi_signature), &(psig->ptmsi_signature), sizeof(P_TMSI_SIGNATURE_T));
    SCI_MEMCPY(&(loci_gprs_mapping.rai), &(psig->rai), sizeof(RAI_T));
    SCI_MEMCPY(&(loci_gprs_mapping.update_status), &(psig->update_state), sizeof(GMM_STATE_UPDATED));

    nv_item_name = GetNvGprsLociId(cur_sim_task);

    //to the GPRS non-ware SIM, there is no EFkcgprs file
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GPRS))
    {
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_LOCIGPRS,
                                                     &loci_gprs_mapping);

#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::Save RAI information result is:0x%x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_973_112_2_17_22_59_48_4599, (uint8 *)"d", ser_result);
#endif

        if(SIM_SERVICE_OK != ser_result)
        {
            //if the locigprs file is not in the sim,update the file in nv
            if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
            {
                nv_result = EFS_NvitemWrite(nv_item_name,
                                            (uint16)(sizeof(SIM_EF_MAPPING_LOCI_GPRS_T)), (uint8 *)&loci_gprs_mapping, TRUE);
                SCI_ASSERT(NVERR_NONE == nv_result);/*assert verified */
            }
            else
            {
                if(SIM_SERVICE_CARD_OUT == ser_result)
                {
                    return;
                }
                else
                {
                    //SCI_TRACE_LOW:"SIM::Save RAI failed for result is:0x%x."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_993_112_2_17_22_59_48_4600, (uint8 *)"d", ser_result);
                    return;
                }
            }
        }
    }
    else
    {
        nv_result = EFS_NvitemWrite(nv_item_name,
                                    (uint16)(sizeof(SIM_EF_MAPPING_LOCI_GPRS_T)), (uint8 *)&loci_gprs_mapping, TRUE);
        SCI_ASSERT(NVERR_NONE == nv_result);/*assert verified */
    }
}
#endif

#ifdef _SUPPORT_GPRS_
/**********************************************************************
//    Description:
//      the GMM Save Cipher information service request signal process function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMGMMSIG_SaveCipherInfo(
    uint32 cur_sim_task,
    yPDef_GMM_SIM_SAVE_CIPHER_INFO *psig    //the service request signal
)
{
    NVITEM_ERROR_E nv_result;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_KC_GPRS_T kc_gprs_mapping;
    uint16 nv_item_name = 0;

    //composite a EFkcgprs mapping
    SCI_MEMCPY(&(kc_gprs_mapping.kc), &(psig->kc_arr), sizeof(GPRS_KC_ARR_T));
    SCI_MEMCPY(&(kc_gprs_mapping.cksn), &(psig->gmm_cksn), sizeof(GMM_CKSN_E));

    nv_item_name = GetNvGprsKcId(cur_sim_task);

    //to the GPRS non-ware SIM, there is no EFkcgprs file
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GPRS))
    {
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KcGPRS, &kc_gprs_mapping);

        //SCI_TRACE_LOW:"SIM::Save Cipher information result is:0x%x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MM_SIGNAL_1036_112_2_17_22_59_48_4601, (uint8 *)"d", ser_result);

        if(SIM_SERVICE_OK != ser_result)
        {
            //if the locigprs file is not in the sim,update the file in nv
            nv_result = EFS_NvitemWrite(nv_item_name,
                                        (uint16)(sizeof(SIM_EF_MAPPING_KC_GPRS_T)), (uint8 *)&kc_gprs_mapping, TRUE);
            SCI_ASSERT(NVERR_NONE == nv_result); /*assert verified */
            s_sim_is_kcgprs_nv_temply[cur_sim_task] = TRUE;
        }
        else
        {
            s_sim_is_kcgprs_nv_temply[cur_sim_task] = FALSE;
        }
    }
    else
    {
        nv_result = EFS_NvitemWrite(nv_item_name,
                                    (uint16)(sizeof(SIM_EF_MAPPING_KC_GPRS_T)), (uint8 *)&kc_gprs_mapping, TRUE);
        SCI_ASSERT(NVERR_NONE == nv_result); /*assert verified */
    }
}
#endif

/**********************************************************************
//    Description:
//      clear the variable if the hot swap happen
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMMMSIG_HotSwap_ClearStaticVariable(uint32 cur_sim_task)
{
#ifdef _SUPPORT_GPRS_
    s_sim_is_kcgprs_nv_temply[cur_sim_task] = FALSE;
#endif
}

/**********************************************************************
//    Description:
//      
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMMMSIG_IsInAuthentication(uint32 cur_sim_task)
{
    return s_sim_in_auth_flag[cur_sim_task];
}

/**********************************************************************
//    Description:
//      
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMMMSIG_IsInGMMAuthentication(uint32 cur_sim_task)
{
#ifdef _SUPPORT_GPRS_
    return s_sim_in_gmm_auth_flag[cur_sim_task];
#else
    return FALSE;
#endif
}

/**********************************************************************
//    Description:
//      Decode the authentication response,see TS31.102 7.1.2.1.
//    Global resource dependence :
//    Author:
//    Note:
***********************************************************************/
void SIMMMSIG_DecodeAuthResp(uint32 cur_sim_task,
                             uint8 rsp_data_len,
                             uint8 *auth_resp_data, 
                             BOOLEAN is_gsm_context,
                             SIM_AUTH_RESPONSE_T *auth_response)
{
    uint8   *ptemp = auth_resp_data;
    uint8   temp_len = 0;
    uint8   i = 0;

    SCI_MEMSET(auth_response,0,sizeof(SIM_AUTH_RESPONSE_T));
    
    auth_response->auth_result = SIM_MM_AUTH_OK;
    
    //see TS31.102 7.1.2.1
    if(is_gsm_context)
    //SERS len tag | SERS DATA(4 bytes) |KC len tag | KC DATA (8bytes)
    {

        SCI_TRACE_LOW("SIM GSM Auth rsp_data_len=%d, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", \
                      rsp_data_len, *ptemp, *(ptemp + 1), *(ptemp + 2), *(ptemp + 3), \
                      *(ptemp + 4), *(ptemp + 5), *(ptemp + 6), *(ptemp + 7),*(ptemp + 8),*(ptemp + 9));

        if(rsp_data_len != (SIM_SERS_LEN + SIM_EF_KC_KC_LEN + 2))
        {
            auth_response->auth_result = SIM_MM_AUTH_FAILED;
        }
        else
        {
            auth_response->sres_len = SIM_SERS_LEN;
            SCI_MEMCPY(auth_response->sres, ptemp + 1, SIM_SERS_LEN);
            auth_response->kc_len = SIM_EF_KC_KC_LEN;
            SCI_MEMCPY(auth_response->kc, ptemp + SIM_SERS_LEN + 2, SIM_EF_KC_KC_LEN);
            auth_response->ck_len = 0;
            auth_response->ik_len = 0;
        }
    }
    else   
        //case1 : the AUTH is successful
        //   TAG    | RES LEN tag |    RES DATA  | CK LEN tag|     CKCN     |  IK LEN tag |   IKCN      | KC LEN tag | KC DATA      |
        //   0xDB   |      1byte  | RES LEN bytes|   1 byte  | CK LEN bytes |     1byte   |IK LEN bytes |   1 byte   | KC LEN bytes |

        //case2 : AUTN SYNC Fail
        //  TAG      | AUTS LEN tag  | AUTS DATA       |
        // 0xDC      |     1 byte    |  AUTS LEN bytes |
    {
        
        for(i = 0; i < 5 ;i++)
        {
        /*SCI_TRACE_LOW("SIM Auth rsp_data_len=%d, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", \
                      rsp_data_len, *ptemp, *(ptemp + 1), *(ptemp + 2), *(ptemp + 3), \
                      *(ptemp + 4), *(ptemp + 5), *(ptemp + 6), *(ptemp + 7), *(ptemp + 8),*(ptemp + 9));
        */
        SCI_TRACE_LOW("SIM UMTS Auth rsp_data_len=%d, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", \
                      rsp_data_len, *(ptemp + i*10), *(ptemp + 1 + i*10), *(ptemp + 2 + i*10), *(ptemp + 3 + i*10), \
                      *(ptemp + 4 + i*10), *(ptemp + 5 + i*10), *(ptemp + 6 + i*10), *(ptemp + 7 + i*10), *(ptemp + 8 + i*10),*(ptemp + 9 + i*10));                      
                              
        }
        if(SIM_AUTN_3G_SUCCESS_TAG == *ptemp)
        {
            ptemp += 1; // move to res length tag
            temp_len = *ptemp;
            auth_response->sres_len = temp_len;

            if(auth_response->sres_len > USIM_MAX_RES_LEN)
            {
                auth_response->sres_len = USIM_MAX_RES_LEN;
            }

            ptemp += 1;
            SCI_MEMCPY(auth_response->sres, ptemp, auth_response->sres_len);

            ptemp += temp_len;// move to ck length tag
            temp_len = *ptemp;
            auth_response->ck_len = temp_len;

            if(auth_response->ck_len > USIM_CK_LEN)
            {
                auth_response->ck_len = USIM_CK_LEN;
            }

            ptemp += 1;
            SCI_MEMCPY(auth_response->ck, ptemp, auth_response->ck_len);

            ptemp += temp_len;// move to ik length tag
            temp_len = *ptemp;
            auth_response->ik_len = temp_len;

            if(auth_response->ik_len > USIM_IK_LEN)
            {
                auth_response->ik_len = USIM_IK_LEN;
            }

            ptemp += 1;
            SCI_MEMCPY(auth_response->ik, ptemp, auth_response->ik_len);

            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_GSM_ACCESS))
            {
                ptemp += temp_len;// move to Kc length tag
                auth_response->kc_len = *ptemp;

                if(auth_response->kc_len > 8)
                {
                    auth_response->kc_len = 8;
                }

                ptemp += 1;
                SCI_MEMCPY(auth_response->kc, ptemp, auth_response->kc_len);
            }

            //check the length, should be valid!
            if((auth_response->kc_len > 8)
                || (auth_response->ck_len > USIM_CK_LEN)
                || (auth_response->ik_len > USIM_IK_LEN)
                || (auth_response->sres_len > USIM_MAX_RES_LEN))
            {
                auth_response->auth_result = SIM_MM_AUTH_FAILED;
            }
        }
        else if(SIM_AUTN_SYNC_FAIL_TAG == *ptemp)
        {
            auth_response->auth_result = SIM_MM_AUTH_SYNC_FAILED;
            ptemp += 1; // move to auts length tag
            auth_response->sres_len = *ptemp;

            if(auth_response->sres_len > USIM_AUTS_LEN)
            {
                auth_response->sres_len = 0;
                auth_response->auth_result = SIM_MM_AUTH_FAILED;
            }
            else
            {
                ptemp += 1;
                SCI_MEMCPY(auth_response->sres, ptemp, auth_response->sres_len);
            }
        }
        else
        {
            auth_response->auth_result = SIM_MM_AUTH_FAILED;
        }
    }        
}
#ifdef   __cplusplus
}
#endif
