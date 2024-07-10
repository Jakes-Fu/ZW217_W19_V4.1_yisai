/******************************************************************************
 ** File Name:      atc_mbbms.c                                               *
 ** Author:         Tracy Zhang                                               *
 ** Date:           2011-03-28                                                *
 ** Copyright:      Copyright 2001-2011 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains functions definitions about MBBMS ATC. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 2011-03-28     Tracy Zhang      Create.                                   *
 ******************************************************************************/
#include "atc.h"
#include "atc_mbbms.h"
#include "atc_common.h"
#include "atc_stk_common_func.h"

/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
/*lint -save -e752 */
extern MN_DUAL_SYS_E       g_current_card_id;//lint !e752
extern RSP_RESULT_STR      g_rsp_str;
extern const ATC_INFO_T    g_atc_info_table[];
BOOLEAN             is_xor_mbau_cmd = FALSE;

LOCAL uint8 s_mbbms_rand[MN_MBBMS_ALGO_REQ_LEN];
LOCAL uint8 s_mbbms_autn[MN_MBBMS_ALGO_REQ_LEN];
LOCAL char  s_mbbms_ck_kc[APP_MN_KC_CK_LEN*2];
LOCAL char  s_mbbms_ik[APP_MN_IK_LEN*2];
LOCAL char  s_mbbms_sres_auts[APP_MN_SRES_AUTN_LEN*2];
LOCAL int   s_mbbms_context = 0;

/*****************************************************************************/
//  Description : This function handles the AT+ERGA command
//  Global resource dependence :
//  Author:    minqian.qian
//  Note:   This command is used to implement "RUN GSM ALGORITHM" in SIM
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessERGA)
{
    uint32  status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_TEST:
            sprintf((char*)g_rsp_str, "%s:<rand> (16 bytes, string type values)",
                g_atc_info_table[AT_CMD_ERGA].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    
        case ATC_CMD_TYPE_SET:
            if (s_mbbms_context != 0)
            {
                SCI_TRACE_LOW("ATC: ATC_ProcessERGA, busy");
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            else if (PARAM1_FLAG && PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING)
            {
                SCI_MEMSET(s_mbbms_rand,0,MN_MBBMS_ALGO_REQ_LEN);
                s_mbbms_rand[0] = PARA1.str_ptr->str_len/2;
                ConvertHexToBin(PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len, (s_mbbms_rand+1));
                //Run GSM Algorithm:    PARA1 (<RAND> :1~16 bytes)
                s_mbbms_autn[0] = 0;//Set the AUTN len as 0.
                MNSIM_RunSIMCommonAlgoEx(dual_sys,P_ATC,0,s_mbbms_rand,s_mbbms_autn);
                status = S_ATC_DEFAULT_HANDLE;
                s_mbbms_context = 1;
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_SIM_MBBMS_ALGO_CNF, 0xFF, ATC_NO_DOMAIN, dual_sys);
            }
            else
            {
                SCI_TRACE_LOW("ATC: ATC_ProcessERGA, param1 is invalid!");
                status = ERR_INVALID_INDEX;
            }
            break;

        default:
            SCI_ASSERT(FALSE);
            break;
    }
    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+ERTCA command
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:       This command is used to implement 3G Context authentication mechanism in USIM
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessERTCA )
{
    uint32  status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_TEST:
        sprintf((char*)g_rsp_str, "%s:<rand>,<autn> (<rand>: 16 bytes, string type values, <autn>: string type values)",
            g_atc_info_table[AT_CMD_ERTCA].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
    case ATC_CMD_TYPE_SET:
        if (s_mbbms_context != 0)
        {
            SCI_TRACE_LOW("ATC: ATC_ProcessERTCA, busy");
            status = ERR_OPERATION_NOT_ALLOWED;
        }
        else if (PARAM1_FLAG && PARAM2_FLAG)
        {
            if ((PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING) ||
                (PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING))
            {
                SCI_TRACE_LOW("ATC: ATC_ProcessERTCA, param1 or param2 is not string");
                status = ERR_INVALID_INDEX;
            }
            else
            {
                SCI_MEMSET(s_mbbms_rand,0,MN_MBBMS_ALGO_REQ_LEN);
                SCI_MEMSET(s_mbbms_autn,0,MN_MBBMS_ALGO_REQ_LEN);
                s_mbbms_rand[0] = PARA1.str_ptr->str_len/2;
                s_mbbms_autn[0] = PARA2.str_ptr->str_len/2;
                ConvertHexToBin(PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len, (s_mbbms_rand + 1));
                ConvertHexToBin(PARA2.str_ptr->str_ptr, PARA2.str_ptr->str_len, (s_mbbms_autn + 1));
                //run 3G Context authentication
                MNSIM_RunSIMCommonAlgoEx(dual_sys,P_ATC,0,s_mbbms_rand,s_mbbms_autn);
                status = S_ATC_DEFAULT_HANDLE;
                s_mbbms_context = 2;
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_SIM_MBBMS_ALGO_CNF, 0xFF, ATC_NO_DOMAIN, dual_sys);
            }
        }
        else
        {
            SCI_TRACE_LOW("ATC: ATC_ProcessERTCA, param1 or param2 is invalid");
            status = ERR_INVALID_INDEX;
        }
        break;

    default:
        SCI_ASSERT(FALSE);
        break;
    }
    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT^MBCELLID command
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_MBCELLID)
{
    ATC_STATUS                      at_status   = S_ATC_SUCCESS;
    int32                           str_len     = 0;
    uint8                           bds_type    = 0;
    uint8                           cgi_arr[7];
    char                            hex_arr[15];
    MN_PHONE_CURRENT_PLMN_INFO_T    plmn_info;

    ATC_TRACE("ATENG: ATC_Processxor_MBCELLID");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                bds_type = PARA1.num;

                switch(bds_type)
                {
                    case 1:     // CGI
                        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(ATC_CURRENT_SIM_ID_MUX);

                        EncodeMNCMCCToThreeByte(plmn_info.mcc, plmn_info.mnc, plmn_info.mnc_digit_num, cgi_arr);

                        //set the location area code
                        cgi_arr[3] = (uint8)(plmn_info.lac >> 8);
                        cgi_arr[4] = (uint8)(plmn_info.lac & 0xff);

                        //set the cell identity value
                        cgi_arr[5] = (uint8)(plmn_info.cell_id >> 8);
                        cgi_arr[6] = (uint8)(plmn_info.cell_id & 0xff);

                        ConvertBinToHex(cgi_arr, 7, (uint8 *)hex_arr);
                        str_len = sprintf((char *)g_rsp_str, "^MBCELLID: %s", hex_arr);
                        g_rsp_str[str_len] = '\0';
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        break;

                    default:
                        at_status = ERR_INVALID_INDEX;
                        break;
                }
            }
            else
            {
                at_status = ERR_OPERATION_NOT_SUPPORTED;
            }

            break;

        default:
            at_status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return (at_status);
}

/*****************************************************************************/
//  Description : This function handles the AT^MBAU command
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_MBAU)
{
    uint32  status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_TEST:
            sprintf((char*)g_rsp_str, "^MBAU:<rand>,<autn> (<rand>: 16 bytes, string type values, <autn>: string type values)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
    
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG && PARAM2_FLAG)
            {
                if ((PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING) ||
                    (PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING))
                {
                    SCI_TRACE_LOW("ATC: ATC_Processxor_MBAU, param1 or param2 is not string");
                    status = ERR_INVALID_INDEX;
                }
                else
                {
                    SCI_MEMSET(s_mbbms_rand,0,MN_MBBMS_ALGO_REQ_LEN);
                    SCI_MEMSET(s_mbbms_autn,0,MN_MBBMS_ALGO_REQ_LEN);
                    s_mbbms_rand[0] = PARA1.str_ptr->str_len/2;
                    s_mbbms_autn[0] = PARA2.str_ptr->str_len/2;
                    ConvertHexToBin(PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len,(s_mbbms_rand + 1));
                    ConvertHexToBin(PARA2.str_ptr->str_ptr, PARA2.str_ptr->str_len,(s_mbbms_autn + 1));
                    MNSIM_RunSIMCommonAlgoEx(dual_sys,P_ATC,0,s_mbbms_rand,s_mbbms_autn);
                    is_xor_mbau_cmd = TRUE;
                    status = S_ATC_DEFAULT_HANDLE;
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_SIM_MBBMS_ALGO_CNF, 0xFF, ATC_NO_DOMAIN, dual_sys);
                }
            }
            else if (PARAM1_FLAG && !PARAM2_FLAG)
            {
                if (PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING)
                {
                    SCI_MEMSET(s_mbbms_rand,0,MN_MBBMS_ALGO_REQ_LEN);
                    s_mbbms_rand[0] = PARA1.str_ptr->str_len/2;
                    s_mbbms_autn[0] = 0;
                    ConvertHexToBin(PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len, (s_mbbms_rand+1));
                    MNSIM_RunSIMCommonAlgoEx(dual_sys,P_ATC,0,s_mbbms_rand,s_mbbms_autn);
                    status = S_ATC_DEFAULT_HANDLE;
                    is_xor_mbau_cmd = TRUE;
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_SIM_MBBMS_ALGO_CNF, 0xFF, ATC_NO_DOMAIN, dual_sys);
                }
            }
            else
            {
                SCI_TRACE_LOW("ATC: ATC_Processxor_MBAU, param1 or param2 is invalid");
                status = ERR_INVALID_INDEX;
            }
            break;
    
        default:
            SCI_ASSERT(FALSE);
            break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function porcess APP_MN_SIM_MBBMS_ALGO_CNF at AT^MBAU
//  Global resource dependence :
//  Author:     chong.liu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSIMmbbmsAlgoCnf_Mbau(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                APP_MN_SIM_MBBMS_ALGO_CNF_T  *sig_ptr
                )
{
    uint32  retCode = S_ATC_RESULT_CODE_OK;
    uint32  errCode = 0;

    SCI_TRACE_LOW("ATC: ATC_ProcessSIMmbbmsAlgoCnf_Mbau, mbbms context:%d, Rsp:%d. %d %d %d ",\
        s_mbbms_context,sig_ptr->ret,sig_ptr->kc_ck[0],sig_ptr->ik[0],sig_ptr->sres_res_autn[0]);
    SCI_MEMSET(s_mbbms_ck_kc,0,(APP_MN_KC_CK_LEN*2));
    SCI_MEMSET(s_mbbms_sres_auts,0,(APP_MN_SRES_AUTN_LEN*2));
    SCI_MEMSET(s_mbbms_ik,0,(APP_MN_IK_LEN*2));

    g_rsp_str[0] = '\0';

    if (ATC_CARD_TYPE_SIM == sig_ptr->sec_context)
    {
        /* GSM Algorithm (SIM) Rsp Cnf */
        if ( APP_MN_SIM_MBBMS_RESPONSE_SUCCESS == sig_ptr->ret)
        {
            ConvertBinToHex((sig_ptr->sres_res_autn + 1), sig_ptr->sres_res_autn[0], (uint8 *)s_mbbms_sres_auts);
            ConvertBinToHex((sig_ptr->kc_ck + 1), sig_ptr->kc_ck[0], (uint8 *)s_mbbms_ck_kc);

            /* send (^MABU:<ret>,<kc>,<sres>): SRES: 1~4bytes, Cipher Key Kc:5~12bytes */
            sprintf((char*)g_rsp_str, "^MBAU: %d,\"%s\",\"%s\"",
                    sig_ptr->ret,s_mbbms_ck_kc, s_mbbms_sres_auts);
        }
        else
        {
            sprintf((char*)g_rsp_str, "^MBAU: %d", sig_ptr->ret);
            retCode = S_ATC_RESULT_CODE_ERROR;
            errCode = ERR_UNKNOWN;
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else if (ATC_CARD_TYPE_USIM == sig_ptr->sec_context)
    {
        /* 3G Context Authentication (USIM) Rsp Cnf */
        ConvertBinToHex((sig_ptr->sres_res_autn + 1), sig_ptr->sres_res_autn[0], (uint8 *)s_mbbms_sres_auts);
        ConvertBinToHex((sig_ptr->kc_ck + 1), sig_ptr->kc_ck[0], (uint8 *)s_mbbms_ck_kc);
        ConvertBinToHex((sig_ptr->ik + 1), sig_ptr->ik[0], (uint8 *)s_mbbms_ik);

        switch (sig_ptr->ret )
        {
            case APP_MN_SIM_MBBMS_RESPONSE_SUCCESS:
                /* send (^MABU:0,res,ck,ik) */
                sprintf((char*)g_rsp_str, "^MBAU: 0,\"%s\",\"%s\",\"%s\"",
                        s_mbbms_sres_auts, s_mbbms_ck_kc, s_mbbms_ik);
                break;

            case APP_MN_SIM_MBBMS_RESPONSE_SYNC_FAILURE:
                /* send (^MABU:1,<auts>) */
                sprintf((char*)g_rsp_str, "^MBAU: 1,\"%s\"", s_mbbms_sres_auts);
                break;

            case APP_MN_SIM_MBBMS_RESPONSE_MAC_FAILURE:
                /* send (^MABU:2) */
                sprintf((char*)g_rsp_str, "^MBAU: 2");
                break;
  
            case APP_MN_SIM_MBBMS_RESPONSE_CONTEXT_NOT_SUPPORT:
            case APP_MN_SIM_MBBMS_RESPONSE_OTHER_FAILURE:
                /* send (^MABU:3) */
                sprintf((char*)g_rsp_str, "^MBAU: 3");
                break;

            default:
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_UNKNOWN);
                is_xor_mbau_cmd = FALSE;
                return S_ATC_SUCCESS;
        }

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else
    {
        retCode = S_ATC_RESULT_CODE_ERROR;
        errCode = ERR_UNKNOWN;
    }

    is_xor_mbau_cmd = FALSE;
    ATC_BuildResultCodeRsp(atc_config_ptr, retCode, errCode);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function porcess APP_MN_SIM_MBBMS_ALGO_CNF at AT+ERGA or AT+ERTCA
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSIMmbbmsAlgoCnf(
                ATC_CONFIG_T        *atc_config_ptr,    // point to the ATC_CONFIG_T struct.
                APP_MN_SIM_MBBMS_ALGO_CNF_T  *sig_ptr
                )
{
    uint32  retCode = S_ATC_RESULT_CODE_OK;
    uint32  errCode = 0;

    SCI_TRACE_LOW("ATC: ATC_ProcessSIMmbbmsAlgoCnf, mbbms context:%d, Rsp:%d ",s_mbbms_context,sig_ptr->ret);
    SCI_MEMSET(s_mbbms_ck_kc,0,(APP_MN_KC_CK_LEN*2));
    SCI_MEMSET(s_mbbms_sres_auts,0,(APP_MN_SRES_AUTN_LEN*2));
    SCI_MEMSET(s_mbbms_ik,0,(APP_MN_IK_LEN*2));

    if (1 == s_mbbms_context)
    {
        /*GSM Algorithm (SIM) Rsp Cnf*/
        s_mbbms_context = 0;
        if (sig_ptr->ret == APP_MN_SIM_MBBMS_RESPONSE_SUCCESS)
        {
            ConvertBinToHex((sig_ptr->sres_res_autn + 1), sig_ptr->sres_res_autn[0], (uint8 *)s_mbbms_sres_auts);
            ConvertBinToHex((sig_ptr->kc_ck + 1), sig_ptr->kc_ck[0], (uint8 *)s_mbbms_ck_kc);

            //send (+ERGA:<sres>,<kc>): SRES: 1~4bytes, Cipher Key Kc:5~12bytes
            sprintf((char*)g_rsp_str, "%s:%s,%s",
                    g_atc_info_table[AT_CMD_ERGA].cmd_name->str_ptr,
                    s_mbbms_sres_auts, s_mbbms_ck_kc);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            retCode = S_ATC_RESULT_CODE_ERROR;
            errCode = ERR_UNKNOWN;
        }
    }
    else if (2 == s_mbbms_context)
    {
        /*3G Context Authentication (USIM) Rsp Cnf*/
        s_mbbms_context = 0;
        ConvertBinToHex((sig_ptr->sres_res_autn + 1), sig_ptr->sres_res_autn[0], (uint8 *)s_mbbms_sres_auts);
        ConvertBinToHex((sig_ptr->kc_ck + 1), sig_ptr->kc_ck[0], (uint8 *)s_mbbms_ck_kc);
        ConvertBinToHex((sig_ptr->ik + 1), sig_ptr->ik[0], (uint8 *)s_mbbms_ik);

        switch (sig_ptr->ret )
        {
            case APP_MN_SIM_MBBMS_RESPONSE_SUCCESS:
                //send (+ERTCA:0,res,ck,ik)
                sprintf((char*)g_rsp_str, "%s:0,%s,%s,%s",
                        g_atc_info_table[AT_CMD_ERTCA].cmd_name->str_ptr,s_mbbms_sres_auts, s_mbbms_ck_kc, s_mbbms_ik);
                break;

            case APP_MN_SIM_MBBMS_RESPONSE_SYNC_FAILURE:
                //send (+ERTCA:1,<auts>)
                sprintf((char*)g_rsp_str, "%s:1,%s",
                        g_atc_info_table[AT_CMD_ERTCA].cmd_name->str_ptr, s_mbbms_sres_auts);
                break;

            case APP_MN_SIM_MBBMS_RESPONSE_MAC_FAILURE:
                //send (+ERTCA:2)
                sprintf((char*)g_rsp_str, "%s:2", g_atc_info_table[AT_CMD_ERTCA].cmd_name->str_ptr);
                break;

            case APP_MN_SIM_MBBMS_RESPONSE_CONTEXT_NOT_SUPPORT:
            case APP_MN_SIM_MBBMS_RESPONSE_OTHER_FAILURE:
                //send (+ERTCA:3)
                sprintf((char*)g_rsp_str, "%s:3", g_atc_info_table[AT_CMD_ERTCA].cmd_name->str_ptr);
                break;

            default:
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_UNKNOWN);
                return S_ATC_SUCCESS;
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else
    {
        retCode = S_ATC_RESULT_CODE_ERROR;
        errCode = ERR_UNKNOWN;
    }

    ATC_BuildResultCodeRsp(atc_config_ptr, retCode, errCode);
    return S_ATC_SUCCESS;
}

