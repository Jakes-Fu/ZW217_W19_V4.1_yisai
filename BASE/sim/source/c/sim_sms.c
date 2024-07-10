/******************************************************************************
** File Name:      sim_sms.c                                                 *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the C file of short message sim signal process    *
**                 functions                                                 *
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


#ifdef   __cplusplus
extern   "C"
{
#endif

//Flag to show wether the SMS EFs is in DFtelecom or not.
#ifdef MULTI_SIM_SYS_QUAD
static BOOLEAN s_sim_is_sms_in_dftel[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
#elif defined (MULTI_SIM_SYS_TRI)
static BOOLEAN s_sim_is_sms_in_dftel[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
#else
static BOOLEAN s_sim_is_sms_in_dftel[SIM_CARD_NUM] = {TRUE, TRUE};
#endif

#ifdef GSM_CUSTOMER_AFP_SUPPORT
typedef struct 
{
    uint8 record_id;
    BOOLEAN used_flag;
    SIM_SMS_T *sim_sms_ptr;
}SIM_SMS_STORE_T;
static uint8 s_sim_sms_temp_store_index[SIM_CARD_NUM] = {0}; 
static SIM_SMS_STORE_T s_sim_sms_temp_store_ptr[SIM_CARD_NUM][50] = {NULL};
#endif

//the Local function declare
/**********************************************************************
//    Description:
//      the function that process the Read SMS parameter request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_ReadSMSParameter(
    uint32 cur_sim_task,
    SIM_SMS_READ_PARAM_REQ_T *psig   //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_READ_PARAM_CNF_T *pcnf;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

    //create the confirm signal of the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_READ_PARAM_CNF,
                      (uint16)(sizeof(SIM_SMS_READ_PARAM_CNF_T)), g_sim_task_id[cur_sim_task]);
    pcnf->record_id = psig->record_id;

    if(psig->record_id == 0)
    {
        pcnf->cause = MN_SMS_SIM_READ_FAILED;
        //send the confirm to the signal sender
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //read the destination reocrd from EF_SMSP
    ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMSP, (uint8)(psig->record_id),
                                     SIM_RECORD_ABSOLUTE, &(pcnf->param_t));

#ifdef SIM_3G_USIM_SUPPORT

    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
    {
        if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
        {
            is_in_adf_usim = FALSE;
        }
        else
        {
            is_in_adf_usim = TRUE;
        }

        SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMSP, (uint8)(psig->record_id),
                                         SIM_RECORD_ABSOLUTE, &(pcnf->param_t));
    }

#endif

    //read operation failed
    if(SIM_SERVICE_OK != ser_result)
    {
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;
        }
        else
        {
            pcnf->cause = MN_SMS_SIM_READ_FAILED;
        }
    }
    //read operation succeeded
    else
    {
        pcnf->cause = MN_SMS_OPERATE_SUCCESS;
    }

    //send the confirm to the signal sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      Get the SMS record num in the EFsms
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_GetSMSNum(
    uint32 cur_sim_task,
    SIM_SMS_GET_SMS_NUM_REQ_T *psig //in:the service request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_GET_SMS_NUM_CNF_T *pcnf;
    SIM_RECORD_EF_INFO_T ef_sms_info;

    //create the confirm signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_GET_SMS_NUM_CNF,
                      sizeof(SIM_SMS_GET_SMS_NUM_CNF_T), g_sim_task_id[cur_sim_task]);

    pcnf->cause = MN_SMS_OPERATE_SUCCESS;

    //check whether the SMS enabled
    if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS))
    {
        //the service failed for the SMS disabled in the SIM card
        pcnf->cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the conform signal of the request signal
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    SCI_MEMSET(&ef_sms_info, 0, sizeof(SIM_RECORD_EF_INFO_T));
    ser_result = SIMSER_GetRecordInfo(cur_sim_task, SIM_FILE_EF_SMS, &ef_sms_info);

    if(SIM_SERVICE_OK == ser_result)
    {
        pcnf->sms_in_sim_valid_num = ef_sms_info.record_num;
        g_sim_card_setting[cur_sim_task].sms_info.valide_sms_num = ef_sms_info.record_num;

        if(PNULL == g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr)
        {
            g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr  = (BOOLEAN *)SCI_ALLOC_BASEZ(g_sim_card_setting[cur_sim_task].sms_info.valide_sms_num * sizeof(BOOLEAN));
        }

        if(PNULL == g_sim_card_setting[cur_sim_task].sms_info.states_ptr)
        {
            g_sim_card_setting[cur_sim_task].sms_info.states_ptr  = (MN_SMS_STATUS_E *)SCI_ALLOC_BASEZ(g_sim_card_setting[cur_sim_task].sms_info.valide_sms_num * sizeof(MN_SMS_STATUS_E));
        }
    }
    else
    {
        g_sim_card_setting[cur_sim_task].sms_info.valide_sms_num = 0;
        pcnf->sms_in_sim_valid_num = 0;
        pcnf->cause = MN_SMS_SIM_UNKNOW_FAILED;
    }

    //SCI_TRACE_LOW:"SIMSMS: g_sim_card_setting.sms_info.valide_sms_num is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_SMS_174_112_2_17_23_0_6_4730, (uint8 *)"d", g_sim_card_setting[cur_sim_task].sms_info.valide_sms_num);
    //send the confirm signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function that process the Read short message request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_ReadSM(
    uint32 cur_sim_task,
    SIM_SMS_READ_SMS_REQ_T *psig //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_READ_SMS_CNF_T *pcnf;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif
#ifdef GSM_CUSTOMER_AFP_SUPPORT
    uint8 i = 0;
#endif

    //creat the confim signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_READ_SMS_CNF,
                      sizeof(SIM_SMS_READ_SMS_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->record_id = psig->record_id;

    if(psig->record_id == 0)
    {
        pcnf->cause = MN_SMS_SIM_READ_FAILED;
        //send the confirm to the signal sender
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //check whether the SMS enabled
    if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS))
    {
        //the service failed for the SMS disabled in the SIM card
        pcnf->cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the confirm signal of the requset signa;
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

#ifdef GSM_CUSTOMER_AFP_SUPPORT
    if (0 != s_sim_sms_temp_store_index[cur_sim_task])
    {
        for (i=0;i<50;i++)
        {
            SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_ReadSM %d",s_sim_sms_temp_store_ptr[cur_sim_task][i].record_id);
            if (psig->record_id == s_sim_sms_temp_store_ptr[cur_sim_task][i].record_id)
            {
                break;
            }
        }
    }
    if ((0 != s_sim_sms_temp_store_index[cur_sim_task])
        && s_sim_sms_temp_store_ptr[cur_sim_task][i].used_flag 
        && (s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr))
    {
        SCI_MEMCPY(&(pcnf->sim_sms_t),s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr,sizeof(SIM_SMS_T));
        ser_result = SIM_SERVICE_OK;

        SCI_TRACE_LOW("SIM SMSSTORE ReadSMS from mem 0x%x, 0x%x 0x%x 0x%x 0x%x",\
            s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr->sms_status,\
            s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr->sms_sim_sm_arr[0],\
            s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr->sms_sim_sm_arr[1],\
            s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr->sms_sim_sm_arr[2],\
            s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr->sms_sim_sm_arr[3]);
    }
    else
#endif
    {
        //read the short message
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->record_id),
                                    SIM_RECORD_ABSOLUTE, &(pcnf->sim_sms_t));
    }

#ifdef SIM_3G_USIM_SUPPORT

    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
    {
        if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
        {
            is_in_adf_usim = FALSE;
        }
        else
        {
            is_in_adf_usim = TRUE;
        }

        SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->record_id),
                                         SIM_RECORD_ABSOLUTE, &(pcnf->sim_sms_t));
    }

#endif

    //read short message failed
    if(SIM_SERVICE_OK != ser_result)
    {
        //failed for no right to read the EF_SMS
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;
        }
        //failed to read EF_SMS for other reason
        else
        {
            pcnf->cause = MN_SMS_SIM_READ_FAILED;
        }

        //send the confirm signal to the request signal sender
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }
    //read short message succeeded
    else
    {
        pcnf->cause = MN_SMS_OPERATE_SUCCESS;
    }

    //save the status of the short message into the status array
    *(g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr + psig->record_id - 1) = TRUE;
    *(g_sim_card_setting[cur_sim_task].sms_info.states_ptr + psig->record_id - 1) = pcnf->sim_sms_t.sms_status;
    //send the confirm signal to the request signal sender
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function that process the save short message request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_SaveSM(
    uint32 cur_sim_task,
    SIM_SMS_WRITE_SMS_REQ_T *psig //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_WRITE_SMS_CNF_T *pcnf;
    SIM_SMS_T sms;
    BOOLEAN free_space = FALSE;
    int32 i;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_WRITE_SMS_CNF,
                      sizeof(SIM_SMS_WRITE_SMS_CNF_T), g_sim_task_id[cur_sim_task]);

    pcnf->sim_sms_t = psig->sim_sms_t;

    //check whether the SMS enabled
    if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS))
    {
        //the service failed for the SMS disabled in the SIM card
        pcnf->cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the confirm signal of the requset signa;
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //looking for a free SMS record to save the new SM
    for(i = 0; i < g_sim_card_setting[cur_sim_task].sms_info.valide_sms_num; i++)
    {
        //if the state of the short message still not get
        if(!(*(g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr + i)))
        {
            //read the short message
            ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(i + 1),
                                             SIM_RECORD_ABSOLUTE, &sms);

#ifdef SIM_3G_USIM_SUPPORT

            if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]) &&
                (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
            {
                if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
                {
                    is_in_adf_usim = FALSE;
                }
                else
                {
                    is_in_adf_usim = TRUE;
                }

                SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
                ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(i + 1),
                                                 SIM_RECORD_ABSOLUTE, &sms);
            }

#endif

            if(SIM_SERVICE_OK != ser_result)
            {
                pcnf->cause = MN_SMS_SIM_UNKNOW_FAILED;

                //send the confirm signal
                SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
                return;
            }

            *(g_sim_card_setting[cur_sim_task].sms_info.states_ptr + i) = sms.sms_status;
            *(g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr + i) = TRUE;
        }

        if(MN_SMS_FREE_SPACE == *(g_sim_card_setting[cur_sim_task].sms_info.states_ptr + i))
        {
            free_space = TRUE;
            pcnf->record_id = i + 1;
            break;
        }
    }

    //there are no free space left to save the SMS
    if(!free_space)
    {
        pcnf->cause = MN_SMS_SIM_MEM_FULL;

        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //save the new SM to a empty SMS Record
    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(pcnf->record_id),
                                       SIM_RECORD_ABSOLUTE, &(psig->sim_sms_t), FALSE);

    //the update operatio is failed
    if(SIM_SERVICE_OK == ser_result)
    {
#ifdef GSM_CUSTOMER_AFP_SUPPORT
        if (psig->is_long_sms)//only work when long SMS
        {
            ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(pcnf->record_id),SIM_RECORD_ABSOLUTE, &sms);
            if (SIM_SERVICE_OK == ser_result)
            {
                if (0 != memcmp(&sms,&(psig->sim_sms_t),sizeof(SIM_SMS_T)))
                {
                    uint8 find_flag = FALSE;
                    uint8 j = 0;

                    SCI_TRACE_LOW("SIM%d:SMSSTORE Read it back data changed. %d",cur_sim_task,s_sim_sms_temp_store_index[cur_sim_task]);

                    if (0 != s_sim_sms_temp_store_index[cur_sim_task])
                    {
                        for (j=0;j<50;j++)
                        {
                            SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_SaveSM To find %d",s_sim_sms_temp_store_ptr[cur_sim_task][j].record_id);

                            if ((pcnf->record_id == s_sim_sms_temp_store_ptr[cur_sim_task][j].record_id) 
                                && (s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr))
                            {
                                SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_SaveSM find %d",pcnf->record_id);
                                find_flag = TRUE;
                                break;
                            }
                        }
                    }

                    if (find_flag)
                    {
                        SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_SaveSM Middle1");

                        //s_sim_sms_temp_store_ptr[cur_sim_task][s_sim_sms_temp_store_index[cur_sim_task]].sim_sms_ptr = s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr;
                    }
                    else
                    {
                        if (s_sim_sms_temp_store_index[cur_sim_task] < 50)
                        {
                            for(j=0;j<50;j++)
                            {
                                if (!s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr)
                                {
                                    break;
                                }
                            }
                            s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr = (SIM_SMS_T*)SCI_ALLOC_BASEZ(sizeof(SIM_SMS_T));
                        }
                        SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_SaveSM Middle2");
                    }

                    if (s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr)
                    {
                        SCI_MEMCPY(s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr,&(psig->sim_sms_t),sizeof(SIM_SMS_T));
                        s_sim_sms_temp_store_ptr[cur_sim_task][j].used_flag = TRUE;
                        s_sim_sms_temp_store_ptr[cur_sim_task][j].record_id = pcnf->record_id;

                        SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_SaveSM Save %d. 0x%x, 0x%x 0x%x 0x%x 0x%x",\
                            pcnf->record_id,\
                            s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr->sms_status,\
                            s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr->sms_sim_sm_arr[0],\
                            s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr->sms_sim_sm_arr[1],\
                            s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr->sms_sim_sm_arr[2],\
                            s_sim_sms_temp_store_ptr[cur_sim_task][j].sim_sms_ptr->sms_sim_sm_arr[3]);

                        if (!find_flag)//a new one
                        {
                            s_sim_sms_temp_store_index[cur_sim_task]++;
                        }
                        SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_SaveSM Save Finish %d",s_sim_sms_temp_store_index[cur_sim_task]);
                    }
                }
            }
        }
#endif
    }
    else
    {
        //failed for no right to update th EF_SMS
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        //other fail reason
#ifdef GSM_CUSTOMER_AFP_SUPPORT
        else if (SIM_SERVICE_INSTR_TECHNICAL_PROBLEM == ser_result)
        {
            SIM_SMS_T temp_sms;

            //to regard as OK, and continue
            SCI_TRACE_LOW("SIM SMS 6F 02");

            SCI_MEMCPY(&temp_sms.sms_sim_sm_arr[0],&(psig->sim_sms_t.sms_sim_sm_arr[0]),MN_SMS_SIM_SMS_LEN);
            temp_sms.sms_status = MN_SMS_MT_READED;

            //update again.
            ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(pcnf->record_id),
            SIM_RECORD_ABSOLUTE, &temp_sms, FALSE);
            if (SIM_SERVICE_OK != ser_result)
            {
                SCI_TRACE_LOW("SIM SMS 6F 02 fail again ser_result=%d",ser_result);
                pcnf->cause = MN_SMS_SIM_UPDATE_FAILED;
                SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
                return;
            }
        }
#endif
        else
        {
            pcnf->cause = MN_SMS_SIM_UPDATE_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    //set the SMS record to not free
    *(g_sim_card_setting[cur_sim_task].sms_info.states_ptr + i) = psig->sim_sms_t.sms_status;

    //save the new SM correct
    pcnf->cause = MN_SMS_OPERATE_SUCCESS;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

#ifdef MNSMS_REPLACE_SMS
/**********************************************************************
//    Description:
//      the function that process the replace short message request signal
//    Global resource dependence :
//    Author: William.Qian 2005_12_20
//    Note:
***********************************************************************/
void SIMSMS_ReplaceSM(                                          // for replace sms sim
    uint32 cur_sim_task,
    SIM_SMS_REPLACE_SMS_REQ_T *psig //in:the request signal
)
{
    SIM_SERVICE_RESULT_E        ser_result;
    SIM_SMS_REPLACE_SMS_CNF_T   *pcnf       = PNULL;
    uint8                       i           = 0;
    BOOLEAN                     clear_smsr  = FALSE;
    uint8                       record_num  = 0;
    uint8                       record[255];
    SIM_SMS_T                   sms;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_REPLACE_SMS_CNF,
                      sizeof(SIM_SMS_REPLACE_SMS_CNF_T), g_sim_task_id[cur_sim_task]);

    pcnf->replace_cnf.sim_sms_t = psig->replace_info.sim_sms_t;
    pcnf->replace_cnf.record_id = psig->replace_info.record_id;
    pcnf->replace_cnf.cause     = MN_SMS_OPERATE_SUCCESS;

    if(psig->replace_info.record_id == 0)
    {
        pcnf->replace_cnf.cause = MN_SMS_SIM_UPDATE_FAILED;
        //send the confirm to the signal sender
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //check whether the SMS enabled
    if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS))
    {
        //the service failed for the SMS disabled in the SIM card
        pcnf->replace_cnf.cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the confirm signal of the requset signa;
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //replace the new SM to SMS Record
    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->replace_info.record_id),
                                       SIM_RECORD_ABSOLUTE, &(psig->replace_info.sim_sms_t), FALSE);

#ifdef SIM_3G_USIM_SUPPORT

    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
    {
        if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
        {
            is_in_adf_usim = FALSE;
        }
        else
        {
            is_in_adf_usim = TRUE;
        }

        SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
        ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->replace_info.record_id),
                                           SIM_RECORD_ABSOLUTE, &(psig->replace_info.sim_sms_t), FALSE);
    }

#endif

    //the update operation is failed
    if(SIM_SERVICE_OK != ser_result)
    {
        //failed for no right to access EF_SMS
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->replace_cnf.cause = MN_SMS_SIM_ACCESS_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        //other failed reason of update operation
        else
        {
            pcnf->replace_cnf.cause = MN_SMS_SIM_UPDATE_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    // delete
    pcnf->delete_num = psig->delete_info.num;

    for(i = 0; i < psig->delete_info.num; i++)
    {
        pcnf->delete_cnf[i].delete_cause = MN_SMS_OPERATE_SUCCESS;
        pcnf->delete_cnf[i].delete_id    = psig->delete_info.delete_list[i];

        if(psig->delete_info.delete_list[i] == 0)
        {
            pcnf->delete_cnf[i].delete_cause = MN_SMS_SIM_READ_FAILED;

            continue;
        }

        //check whether the SMS enabled
        if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS))
        {
            //the service failed for the SMS disabled in the SIM card
            pcnf->delete_cnf[i].delete_cause = MN_SMS_SIM_SERVICE_DISABLED;

            continue;
        }

        //first read the SM from EF_SMS
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->delete_info.delete_list[i]),
                                         SIM_RECORD_ABSOLUTE, &sms);

#ifdef SIM_3G_USIM_SUPPORT

        if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
            && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
        {
            if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
            {
                is_in_adf_usim = FALSE;
            }
            else
            {
                is_in_adf_usim = TRUE;
            }

            SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
            ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->delete_info.delete_list[i]), SIM_RECORD_ABSOLUTE, &sms);
        }

#endif

        if(SIM_SERVICE_OK != ser_result)
        {
            if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
                (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
                (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
                (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
                (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
                (SIM_SERVICE_ACCESS_NEVER == ser_result))
            {
                pcnf->delete_cnf[i].delete_cause = MN_SMS_SIM_ACCESS_FAILED;
            }
            else
            {
                pcnf->delete_cnf[i].delete_cause = MN_SMS_SIM_READ_FAILED;
            }

            continue;
        }

        //the SM have a status report saved in EF_SMSR
        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS_STATUS_REPORT)
            && (MN_SMS_MO_SR_RECEIVED_AND_STORED == sms.sms_status))
        {
            clear_smsr = TRUE;
        }

        //change the SM state
        sms.sms_status = MN_SMS_FREE_SPACE;

        //update the SM
        ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->delete_info.delete_list[i]),
                                           SIM_RECORD_ABSOLUTE, &sms, FALSE);

        //the update operation is failed
        if(SIM_SERVICE_OK != ser_result)
        {
            //failed for no right to access EF_SMS
            if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
                (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
                (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
                (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
                (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
                (SIM_SERVICE_ACCESS_NEVER == ser_result))
            {
                pcnf->delete_cnf[i].delete_cause = MN_SMS_SIM_ACCESS_FAILED;
            }
            //other failed reason of update operation
            else
            {
                pcnf->delete_cnf[i].delete_cause = MN_SMS_SIM_UPDATE_FAILED;
            }

            continue;
        }

        //if there status report
        if(clear_smsr)
        {
            //find the status report
            ser_result = SIMSER_SeekInFile(cur_sim_task, SIM_FILE_EF_SMSR, SIM_SEEK_TYPE_1,
                                           SIM_SEEK_BEGINNING_FORWARD, 1, (uint8 *) & (psig->delete_info.delete_list[i]), &record_num, record);

#ifdef SIM_3G_USIM_SUPPORT

            if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
            {
                if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
                {
                    is_in_adf_usim = FALSE;
                }
                else
                {
                    is_in_adf_usim = TRUE;
                }

                SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
                ser_result = SIMSER_SeekInFile(cur_sim_task, SIM_FILE_EF_SMSR, SIM_SEEK_TYPE_1,
                                               SIM_SEEK_BEGINNING_FORWARD, 1, (uint8 *) & (psig->delete_info.delete_list[i]), &record_num, record);
            }

#endif

            if(SIM_SERVICE_OK == ser_result)
            {
                //clear the status report
                SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMSR, record[0],
                                      SIM_RECORD_ABSOLUTE, NULL, TRUE);
            }
        }

        //change the SM state if it has got
        *(g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr + psig->delete_info.delete_list[i] - 1)   = TRUE;
        *(g_sim_card_setting[cur_sim_task].sms_info.states_ptr + psig->delete_info.delete_list[i] - 1)       = MN_SMS_FREE_SPACE;
    }

    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif
/**********************************************************************
//    Description:
//      the function that process the edit short message request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_EditSM(
    uint32 cur_sim_task,
    SIM_SMS_EDIT_SMS_REQ_T *psig //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_EDIT_SMS_CNF_T *pcnf;
    SIM_SMS_T sms;
    BOOLEAN clear_smsr = FALSE;
    uint8 record_num;
    uint8 record[255];
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_EDIT_SMS_CNF,
                      sizeof(SIM_SMS_EDIT_SMS_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->status = psig->sim_sms_t.sms_status;
    pcnf->record_id = psig->record_id;

    //check whether the SMS enabled
    if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS))
    {
        //the service failed for the SMS disabled in the SIM card
        pcnf->cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the confirm signal of the requset signa;
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    if(psig->record_id == 0)
    {
        pcnf->cause = MN_SMS_SIM_UPDATE_FAILED;
        //send the confirm to the signal sender
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //if the state of the short message still not get
    if(!(*(g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr + psig->record_id - 1)))
    {
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->record_id),
                                         SIM_RECORD_ABSOLUTE, &sms);

#ifdef SIM_3G_USIM_SUPPORT

        if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
            && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
        {
            if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
            {
                is_in_adf_usim = FALSE;
            }
            else
            {
                is_in_adf_usim = TRUE;
            }

            SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
            ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->record_id), SIM_RECORD_ABSOLUTE, &sms);
        }

#endif

        if(SIM_SERVICE_OK != ser_result)
        {
            if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
                (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
                (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
                (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
                (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
                (SIM_SERVICE_ACCESS_NEVER == ser_result))
            {
                pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;

                SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
                return;
            }
            else
            {
                pcnf->cause = MN_SMS_SIM_READ_FAILED;

                SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
                return;
            }
        }

        *(g_sim_card_setting[cur_sim_task].sms_info.states_ptr + psig->record_id - 1) = sms.sms_status;
        *(g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr + psig->record_id - 1) = TRUE;
    }


    //the SM have a status report saved in EF_SMSR
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS_STATUS_REPORT)
        && (MN_SMS_MO_SR_RECEIVED_AND_STORED == *(g_sim_card_setting[cur_sim_task].sms_info.states_ptr + psig->record_id - 1))
        && (MN_SMS_FREE_SPACE == psig->sim_sms_t.sms_status))
    {
        clear_smsr = TRUE;
    }

    //save the new SM to a empty SMS Record
    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->record_id),
                                       SIM_RECORD_ABSOLUTE, &(psig->sim_sms_t), FALSE);

    //the update operation is failed
    if(SIM_SERVICE_OK != ser_result)
    {
        //failed for no right to access EF_SMS
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        //other failed reason of update operation
        else
        {
            pcnf->cause = MN_SMS_SIM_UPDATE_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    //if there status report
    if(clear_smsr)
    {
        //find the status report
        ser_result = SIMSER_SeekInFile(cur_sim_task, SIM_FILE_EF_SMSR, SIM_SEEK_TYPE_1,
                                       SIM_SEEK_BEGINNING_FORWARD, 1, (uint8 *) & (psig->record_id), &record_num, record);

#ifdef SIM_3G_USIM_SUPPORT

        if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
            && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
        {
            if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
            {
                is_in_adf_usim = FALSE;
            }
            else
            {
                is_in_adf_usim = TRUE;
            }

            SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
            ser_result = SIMSER_SeekInFile(cur_sim_task, SIM_FILE_EF_SMSR, SIM_SEEK_TYPE_1,
                                           SIM_SEEK_BEGINNING_FORWARD, 1, (uint8 *) & (psig->record_id), &record_num, record);
        }

#endif

        if(SIM_SERVICE_OK == ser_result)
        {
            //clear the status report
            SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMSR, record[0],
                                  SIM_RECORD_ABSOLUTE, NULL, TRUE);
        }
    }

    //change the SM state if it has got
    *(g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr + psig->record_id - 1) = TRUE;
    *(g_sim_card_setting[cur_sim_task].sms_info.states_ptr + psig->record_id - 1) = psig->sim_sms_t.sms_status;

    pcnf->cause = MN_SMS_OPERATE_SUCCESS;
    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function that process the update short message state
//    request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_UpdateSMState(
    uint32 cur_sim_task,
    SIM_SMS_UPDATE_SMS_STATUS_REQ_T *psig    //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_UPDATE_SMS_STATUS_CNF_T *pcnf;
    SIM_SMS_T sms;
    BOOLEAN clear_smsr = FALSE;
    uint8 record_num;
    uint8 record[255];
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif
#ifdef GSM_CUSTOMER_AFP_SUPPORT
    uint8 i = 0;
#endif

    //create the confirm signal of the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_UPDATE_SMS_STATUS_CNF,
                      sizeof(SIM_SMS_UPDATE_SMS_STATUS_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->record_id = psig->record_id;
    pcnf->status = psig->status;

    if(psig->record_id == 0)
    {
        pcnf->cause = MN_SMS_SIM_READ_FAILED;
        //send the confirm to the signal sender
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //check whether the SMS enabled
    if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS))
    {
        //the service failed for the SMS disabled in the SIM card
        pcnf->cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the confirm signal of the requset signa;
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //first read the SM from EF_SMS
    ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->record_id),
                                     SIM_RECORD_ABSOLUTE, &sms);

#ifdef SIM_3G_USIM_SUPPORT

    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
    {
        if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
        {
            is_in_adf_usim = FALSE;
        }
        else
        {
            is_in_adf_usim = TRUE;
        }

        SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->record_id), SIM_RECORD_ABSOLUTE, &sms);
    }

#endif

    if(SIM_SERVICE_OK != ser_result)
    {
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        else
        {
            pcnf->cause = MN_SMS_SIM_READ_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    //the SM have a status report saved in EF_SMSR
    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS_STATUS_REPORT)
        && (MN_SMS_MO_SR_RECEIVED_AND_STORED == sms.sms_status)
        && (MN_SMS_FREE_SPACE == psig->status))
    {
        clear_smsr = TRUE;
    }

    //change the SM state
    sms.sms_status = psig->status;

    //update the SM
    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMS, (uint8)(psig->record_id),
                                       SIM_RECORD_ABSOLUTE, &sms, FALSE);

    //the update operation is failed
    if(SIM_SERVICE_OK != ser_result)
    {
        //failed for no right to access EF_SMS
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        //other failed reason of update operation
        else
        {
            pcnf->cause = MN_SMS_SIM_UPDATE_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

#ifdef GSM_CUSTOMER_AFP_SUPPORT
    if (0 != s_sim_sms_temp_store_index[cur_sim_task])
    {
        for (i=0;i<50;i++)
        {
            SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_UpdateSMState To find. %d",s_sim_sms_temp_store_ptr[cur_sim_task][i].record_id);

            if (psig->record_id == s_sim_sms_temp_store_ptr[cur_sim_task][i].record_id)
            {
                SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_UpdateSMState find. %d",psig->record_id);
                break;
            }
        }
        if (s_sim_sms_temp_store_ptr[cur_sim_task][i].used_flag 
            && (s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr))
        {
            if (MN_SMS_FREE_SPACE == psig->status)
            {
                s_sim_sms_temp_store_ptr[cur_sim_task][i].record_id = 0;
                s_sim_sms_temp_store_ptr[cur_sim_task][i].used_flag = FALSE;
                SCI_FREE(s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr);
                if (s_sim_sms_temp_store_index[cur_sim_task] > 0)
                {
                    s_sim_sms_temp_store_index[cur_sim_task]--;
                }
                SCI_TRACE_LOW("SIM SMSSTORE SIMSMS_UpdateSMState Delete. %d",s_sim_sms_temp_store_index[cur_sim_task]);
            }
            else
            {
                s_sim_sms_temp_store_ptr[cur_sim_task][i].sim_sms_ptr->sms_status = psig->status;
            }
        }
    }
#endif

    //if there status report
    if(clear_smsr)
    {
        //find the status report
        ser_result = SIMSER_SeekInFile(cur_sim_task, SIM_FILE_EF_SMSR, SIM_SEEK_TYPE_1,
                                       SIM_SEEK_BEGINNING_FORWARD, 1, (uint8 *) & (psig->record_id), &record_num, record);

#ifdef SIM_3G_USIM_SUPPORT

        if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
            && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
        {
            if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
            {
                is_in_adf_usim = FALSE;
            }
            else
            {
                is_in_adf_usim = TRUE;
            }

            SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
            ser_result = SIMSER_SeekInFile(cur_sim_task, SIM_FILE_EF_SMSR, SIM_SEEK_TYPE_1,
                                           SIM_SEEK_BEGINNING_FORWARD, 1, (uint8 *) & (psig->record_id), &record_num, record);
        }

#endif

        if(SIM_SERVICE_OK == ser_result)
        {
            //clear the status report
            SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMSR, record[0],
                                  SIM_RECORD_ABSOLUTE, NULL, TRUE);
        }
    }

    //change the SM state if it has got
    *(g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr + psig->record_id - 1) = TRUE;
    *(g_sim_card_setting[cur_sim_task].sms_info.states_ptr + psig->record_id - 1) = psig->status;

    pcnf->cause = MN_SMS_OPERATE_SUCCESS;
    //confirm the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function that process the read short message status report
//    request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_ReadSMStatusReport(
    uint32 cur_sim_task,
    SIM_SMS_READ_STATUS_REPORT_REQ_T *psig    //in:the reauest signal
)
{
#ifdef MNSMS_ADDITIONAL_FEATURE
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_READ_STATUS_REPORT_CNF_T *pcnf;
    uint8 record_num;
    uint8 record[255];
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

    //creat the confirm signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_READ_STATUS_REPORT_CNF,
                      sizeof(SIM_SMS_READ_STATUS_REPORT_CNF_T), g_sim_task_id[cur_sim_task]);

    //check whether the SMS status report enabled
    if((g_sim_card_setting[cur_sim_task].service_table.service_num <= SIMSER_PH2_SMS_STATUS_REPORT) ||
        (!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS_STATUS_REPORT)))
    {
        //the service failed for the SMS status report disabled in the SIM card
        pcnf->cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the confirm signal of the requset signa;
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //seek the SM status report that is belong to the record id point SMS in EF_SMS
    ser_result = SIMSER_SeekInFile(cur_sim_task, SIM_FILE_EF_SMSR, SIM_SEEK_TYPE_1,
                                   SIM_SEEK_BEGINNING_FORWARD, 1, (uint8 *) & (psig->record_id), &record_num, record);

#ifdef SIM_3G_USIM_SUPPORT

    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
    {
        if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
        {
            is_in_adf_usim = FALSE;
        }
        else
        {
            is_in_adf_usim = TRUE;
        }

        SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
        ser_result = SIMSER_SeekInFile(cur_sim_task, SIM_FILE_EF_SMSR, SIM_SEEK_TYPE_1,
                                       SIM_SEEK_BEGINNING_FORWARD, 1, (uint8 *) & (psig->record_id), &record_num, record);
    }

#endif

    if(SIM_SERVICE_OK != ser_result)
    {
        if(SIM_SERVICE_SEEK_NOT_FIND_PATTERN == ser_result)
        {
            pcnf->cause = MN_SMS_SIM_PATTERN_NOT_FIND;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        //failed for no right to access EF_SMS
        else if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
                (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
                (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
                (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
                (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
                (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        //other failed reason of read operation
        else
        {
            pcnf->cause = MN_SMS_SIM_READ_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    //read the Status report from the EF_SMSR
    ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_SMSR, record[0],
                                     SIM_RECORD_ABSOLUTE, &(pcnf->status_report_arr));

    if(SIM_SERVICE_OK != ser_result)
    {
        //failed for no right to access EF_SMS
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        //other failed reason of read operation
        else
        {
            pcnf->cause = MN_SMS_SIM_READ_FAILED;

            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    //read the status report ok
    pcnf->cause = MN_SMS_OPERATE_SUCCESS;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
#endif
}

/**********************************************************************
//    Description:
//      the function that process the get TP message reference request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_GetTPMessageReference(
    uint32 cur_sim_task,
    SIM_SMS_READ_SMSS_REQ_T *psig  //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_READ_SMSS_CNF_T *pcnf;
    SIM_EF_MAPPING_SMSS_T smss;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

    //create the confirm signal of the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_READ_SMSS_CNF,
                      sizeof(SIM_SMS_READ_SMSS_CNF_T), g_sim_task_id[cur_sim_task]);

    //check the whether SMS service enabled in the SIM card.
    if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS))
    {
        //the request failed for the SMS service disabled.
        pcnf->cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the confirm to the signal sender
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_SMSS, &smss);
#ifdef SIM_3G_USIM_SUPPORT

    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
    {
        if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
        {
            is_in_adf_usim = FALSE;
        }
        else
        {
            is_in_adf_usim = TRUE;
        }

        SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_SMSS, &smss);
    }

#endif

    if(SIM_SERVICE_OK != ser_result)
    {
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;
            //send the confirm signal of the request signal
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        else
        {
            pcnf->cause = MN_SMS_SIM_READ_FAILED;
            //send the confirm signal of the request signal
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    pcnf->tp_mr = smss.tp_mr;
    pcnf->mem_is_full = smss.mem_exeed;

    pcnf->cause = MN_SMS_OPERATE_SUCCESS;
    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function that process the save TP message reference request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_SaveTPMessageReference(
    uint32 cur_sim_task,
    SIM_SMS_WRITE_SMSS_REQ_T *psig  //in:the requst signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_WRITE_SMSS_CNF_T *pcnf;
    SIM_EF_MAPPING_SMSS_T smss_mapping;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

    //creat the confirm signal of the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_WRITE_SMSS_CNF,
                      sizeof(SIM_SMS_WRITE_SMSS_CNF_T), g_sim_task_id[cur_sim_task]);

    //check the whether SMS service enabled in the SIM card.
    if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMS))
    {
        //the request failed for the SMS service disabled.
        pcnf->cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the confirm to the signal sender
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    smss_mapping.tp_mr = psig->tp_mr;
    smss_mapping.mem_exeed = psig->mem_is_full;

    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_SMSS, &(smss_mapping));
#ifdef SIM_3G_USIM_SUPPORT

    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
    {
        if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
        {
            is_in_adf_usim = FALSE;
        }
        else
        {
            is_in_adf_usim = TRUE;
        }

        SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
        ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_SMSS, &(smss_mapping));
    }

#endif

    if(SIM_SERVICE_OK != ser_result)
    {
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;
            //send the confirm signal of the request signal
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        else
        {
            pcnf->cause = MN_SMS_SIM_UPDATE_FAILED;
            //send the confirm signal of the request signal
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    pcnf->cause = MN_SMS_OPERATE_SUCCESS;
    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function that process the save SMS parameter request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMSMS_UpdateSMSParameter(
    uint32 cur_sim_task,
    SIM_SMS_WRITE_PARAM_REQ_T *psig  //in:the request signal
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_SMS_WRITE_PARAM_CNF_T *pcnf;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

    //create confirm signal of the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_SMS_WRITE_PARAM_CNF,
                      sizeof(SIM_SMS_WRITE_PARAM_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->record_id = psig->record_id;

    //check the whether SMS service enabled in the SIM card.
    if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_SMSP))
    {
        //the request failed for the SMS service disabled.
        pcnf->cause = MN_SMS_SIM_SERVICE_DISABLED;

        //send the confirm to the signal sender
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMSP, (uint8)(psig->record_id),
                                       SIM_RECORD_ABSOLUTE, &(psig->param_t), FALSE);
#ifdef SIM_3G_USIM_SUPPORT

    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
        && (SIM_SEC_STATUS_NOT_SATISFIED == ser_result))
    {
        if(SIMSMS_GetSMSLocationDFtel(cur_sim_task))
        {
            is_in_adf_usim = FALSE;
        }
        else
        {
            is_in_adf_usim = TRUE;
        }

        SIMSMS_SetSMSLocationDFtel(cur_sim_task, is_in_adf_usim);
        ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_SMSP, (uint8)(psig->record_id),
                                           SIM_RECORD_ABSOLUTE, &(psig->param_t), FALSE);
    }

#endif

    if(SIM_SERVICE_OK != ser_result)
    {
        if((SIM_SERVICE_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV1_NEEDED == ser_result) ||
            (SIM_SERVICE_U_CHV2_NEEDED == ser_result) ||
            (SIM_SERVICE_CHV_BLOCKED == ser_result) ||
            (SIM_SERVICE_ACCESS_NEVER == ser_result))
        {
            pcnf->cause = MN_SMS_SIM_ACCESS_FAILED;
            //send the confirm signal of the request signal
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
        else
        {
            pcnf->cause = MN_SMS_SIM_UPDATE_FAILED;
            //send the confirm signal of the request signal
            SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
            return;
        }
    }

    pcnf->cause = MN_SMS_OPERATE_SUCCESS;
    //send the confirm signal of the request signal
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function set the SMS files is in DFtelecom or not.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMSMS_SetSMSLocationDFtel(uint32 cur_sim_task, BOOLEAN is_sms_in_dftel)
{
    s_sim_is_sms_in_dftel[cur_sim_task] = is_sms_in_dftel;
}

/**********************************************************************
//    Description:
//      the function return the SMS files is in DFtelecom or not.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMSMS_GetSMSLocationDFtel(uint32 cur_sim_task)
{
    return s_sim_is_sms_in_dftel[cur_sim_task];
}

#ifdef   __cplusplus
}
#endif
