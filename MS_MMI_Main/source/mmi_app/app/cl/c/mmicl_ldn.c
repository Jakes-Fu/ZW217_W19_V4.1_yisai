/*****************************************************************************
** File Name:      mmicl_ldn.c                                                   *
** Author:                                                                   *
** Date:           15/04/2011                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe LDN(Last dial number)                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2011       wancan.you       Create
******************************************************************************/
#define _MMICL_LDN_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_cl_trc.h"
#ifdef MMI_LDN_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_nv.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_type.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmicl_export.h"
#include "mmicl_internal.h"
#include "mmicc_export.h"
#include "mmicc_id.h"
#include "mmiwclk_export.h"
#include "mmipub.h"
#include "mmiidle_export.h"
#include "mmi_modu_main.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_call_info_ready[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN s_is_ldn_delete_all = FALSE;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
extern double fabs( double x );
#endif

extern uint16 s_ldn_flag;

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Initial Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void InitCallInfo(PHONEBOOK_STORAGE_E storage, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : ldn正在等待界面不允许返回，等待ldn update confirm消息上来
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void OpenLdnUpdateWaitingWindow (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : ldn正在等待界面不允许返回,等待ldn update confirm消息上来
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLdnUpdateWaitingWindow (
                                           MMI_WIN_ID_T      win_id, 	// 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id, 	// 窗口的内部消息ID
                                           DPARAM 		        param		// 相应消息的参数
                                           );

/*****************************************************************************/
// 	Description : Reset Call Info Record NV
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_ResetCallInfoRecord(MN_DUAL_SYS_E dual_sys)
{
    int32 i = 0;
    MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr = PNULL;
    //int32 j = 0;
    BOOLEAN isChanged=FALSE;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    call_info_ptr = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if (PNULL == call_info_ptr)
    {
        return FALSE;
    }

//     for (j = 0; j < MMICL_CALL_TYPE_MAX; j++)
//     {
//         if (MMICL_CALL_DIALED != j && MMICL_CALL_RECEIVED != j)
//         {
//             continue;
//         }
    
    SCI_MEMSET(call_info_ptr, 0x00, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    MMICL_ReadAllNV(call_info_ptr);

    //clear ldn info
    if (call_info_ptr->record_num > 0)
    {
        for (i = call_info_ptr->record_num - 1; i >= 0; i--)
        {
            if (call_info_ptr->call_bcdinfo[i].is_sim_record
                && dual_sys == call_info_ptr->call_bcdinfo[i].dual_sys)
            {
                if (i != call_info_ptr->record_num - 1)
                {
                    SCI_MEMCPY(&call_info_ptr->call_bcdinfo[i], &call_info_ptr->call_bcdinfo[i + 1], sizeof(MMICL_CALL_BCDINFO_T));
                }

                if (call_info_ptr->record_num > 0)
                {
                    call_info_ptr->record_num--;
                }

                if (call_info_ptr->sim_record_num > 0)
                {
                    call_info_ptr->sim_record_num--;
                }

                SCI_MEMSET(&call_info_ptr->call_bcdinfo[call_info_ptr->record_num], 0x00, sizeof(MMICL_CALL_BCDINFO_T));
                isChanged = TRUE;
            }
        }
    }
    if (isChanged)
    {
        MMICL_WriteNV(MMICL_CALL_ALL, call_info_ptr);
    }    
    /*}*/

    SCI_FREE(call_info_ptr);

    return TRUE;
}

/*****************************************************************************/
// 	Description : Initial Call Info Record NV
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_InitCallInfoRecord(void)
{
    int32 i = 0;
    MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr = PNULL;
    //int32 j = 0;
    BOOLEAN isChanged=FALSE;

    call_info_ptr = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    if (PNULL == call_info_ptr)
    {
        return FALSE;
    }

//     for (j = 0; j < MMICL_CALL_TYPE_MAX; j++)
//     {
//         if (MMICL_CALL_DIALED != j && MMICL_CALL_RECEIVED != j)
//         {
//             continue;
//         }

    SCI_MEMSET(call_info_ptr, 0x00, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

    MMICL_ReadAllNV(call_info_ptr);

    //clear ldn info
    if (call_info_ptr->record_num > 0)
    {
        for (i = call_info_ptr->record_num - 1; i >= 0; i--)
        {
            if (call_info_ptr->call_bcdinfo[i].is_sim_record)
            {
                if (i != call_info_ptr->record_num - 1)
                {
                    SCI_MEMCPY(&call_info_ptr->call_bcdinfo[i], &call_info_ptr->call_bcdinfo[i + 1], sizeof(MMICL_CALL_BCDINFO_T));
                }

                if (call_info_ptr->record_num > 0)
                {
                    call_info_ptr->record_num--;
                }

                SCI_MEMSET(&call_info_ptr->call_bcdinfo[call_info_ptr->record_num], 0x00, sizeof(MMICL_CALL_BCDINFO_T));
                isChanged = TRUE;                    
            }
        }
    }

    call_info_ptr->sim_record_num = 0;
    if (isChanged)
    {
        MMICL_WriteNV(MMICL_CALL_ALL, call_info_ptr);
    }
    /*}*/

    SCI_FREE(call_info_ptr);

    return TRUE;
}

/*****************************************************************************/
// 	Description : Initial Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void InitCallInfo(PHONEBOOK_STORAGE_E storage, MN_DUAL_SYS_E dual_sys)
{
    PHONEBOOK_INFO_T phonebook_info = {0};
    MMICL_CALL_TYPE_E call_type = MMICL_CALL_DIALED;
    BOOLEAN         isChanged= FALSE;

    //SCI_TRACE_LOW:"MMICL: InitCallInfo:storage=%d, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_222_112_2_18_2_11_25_34,(uint8*)"dd", storage, dual_sys);

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    //init s_is_ldn_delete_all as false
    MMIAPICL_SetLdnIsDeleteAll(FALSE);

#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
    if (PHONEBOOK_LND_STORAGE == storage
        || PHONEBOOK_OCI_STORAGE == storage)
    {
        call_type = MMICL_CALL_DIALED;
    }
    else if (PHONEBOOK_ICI_STORAGE == storage)
    {
        call_type = MMICL_CALL_RECEIVED;
    }
    else
    {
        return;
    }
#else
    call_type = MMICL_CALL_DIALED;
#endif

    s_is_call_info_ready[dual_sys] = TRUE;

    MMIAPICL_ResetCallInfoRecord(dual_sys);

    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, storage);

    phonebook_info.max_record_num = MIN(phonebook_info.max_record_num, MMICL_LDN_NUM_MAX);
    phonebook_info.used_record_num = MIN(phonebook_info.used_record_num, phonebook_info.max_record_num);

    if (0 != phonebook_info.used_record_num)
    {
        uint32 i = 0;
        PHONEBOOK_ENTRY_T *ldn_entry_ptr = PNULL;
        MMICL_CALL_ARRAY_BCDINFO_T *call_info_ptr = PNULL;
#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
        CALL_INFO_ENTRY_T call_info_entry = {0};
#endif

        ldn_entry_ptr = SCI_ALLOCAZ(sizeof(PHONEBOOK_ENTRY_T));

        if (PNULL == ldn_entry_ptr)
        {
            //SCI_TRACE_LOW:"MMICL: InitCallInfo ldn_entry_ptr No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_272_112_2_18_2_11_25_35,(uint8*)"");

            return;
        }

        call_info_ptr = SCI_ALLOCAZ(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

        if (PNULL == call_info_ptr)
        {
            //SCI_TRACE_LOW:"MMICL: InitCallInfo call_info_ptr No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_281_112_2_18_2_11_25_36,(uint8*)"");

            SCI_FREE(ldn_entry_ptr);
            ldn_entry_ptr = PNULL;

            return;
        }

        SCI_MEMSET(call_info_ptr, 0x00, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));

        MMICL_ReadAllNV(call_info_ptr);

        for (i = 1; i <= phonebook_info.used_record_num; i++)
        {
            uint32 j = 0;
            BOOLEAN is_same_record = FALSE;
#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
            uint32 call_start_time = 0;
#endif

            SCI_MEMSET(ldn_entry_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));
        
            ldn_entry_ptr->entry_id = i;
#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
            SCI_MEMSET(&call_info_entry, 0x00, sizeof(call_info_entry));

            MMICL_ReadCallInfo(storage, dual_sys, ldn_entry_ptr, &call_info_entry);
#else
            MMICL_ReadCallInfo(storage, dual_sys, ldn_entry_ptr);
#endif
            //check same record in nv
            for (j = 0; j < call_info_ptr->record_num; j++)
            {
                if (call_info_ptr->call_bcdinfo[j].call_type !=call_type)
                {
                    continue;
                }
                if (dual_sys == call_info_ptr->call_bcdinfo[j].dual_sys
                    && call_info_ptr->call_bcdinfo[j].number_len == ldn_entry_ptr->number_len
                    && call_info_ptr->call_bcdinfo[j].number_plan == ldn_entry_ptr->npi
                    && call_info_ptr->call_bcdinfo[j].number_type == ldn_entry_ptr->ton
                    && (0 == memcmp(call_info_ptr->call_bcdinfo[j].number, ldn_entry_ptr->number, call_info_ptr->call_bcdinfo[j].number_len)))
                {
                    is_same_record = TRUE;

                    break;
                }
            }

            if (!is_same_record)
            {
                //add to record nv
                if (call_info_ptr->record_num < MMICL_RECORD_TOTAL_NUM)
                {
                    if (ldn_entry_ptr->number_len > 0)
                    {
                        uint8 tele_number[CC_MAX_TELE_NUM_LEN + 2] = {0};
                        uint8 max_tele_len  = CC_MAX_TELE_NUM_LEN + 2;
                        uint16 tele_len = 0;

                        call_info_ptr->call_bcdinfo[call_info_ptr->record_num].dual_sys = dual_sys;
                        call_info_ptr->call_bcdinfo[call_info_ptr->record_num].is_num_present = TRUE;
                        call_info_ptr->call_bcdinfo[call_info_ptr->record_num].number_len = MIN(ldn_entry_ptr->number_len, MMIPB_BCD_NUMBER_MAX_LEN);
                        SCI_MEMCPY(call_info_ptr->call_bcdinfo[call_info_ptr->record_num].number, ldn_entry_ptr->number, call_info_ptr->call_bcdinfo[call_info_ptr->record_num].number_len);
                        call_info_ptr->call_bcdinfo[call_info_ptr->record_num].number_plan = ldn_entry_ptr->npi;
                        call_info_ptr->call_bcdinfo[call_info_ptr->record_num].number_type = ldn_entry_ptr->ton;

                        tele_len = MMIAPICOM_GenDispNumber(call_info_ptr->call_bcdinfo[call_info_ptr->record_num].number_type,
                                                                                        (uint8)call_info_ptr->call_bcdinfo[call_info_ptr->record_num].number_len, 
                                                                                        call_info_ptr->call_bcdinfo[call_info_ptr->record_num].number, 
                                                                                        tele_number, 
                                                                                        max_tele_len);

                        if (MMIAPICC_IsEmergencyNum(tele_number, tele_len, dual_sys, FALSE))
                        {
                            call_info_ptr->call_bcdinfo[call_info_ptr->record_num].is_emergency_call = TRUE;
                        }
                        else
                        {
                            call_info_ptr->call_bcdinfo[call_info_ptr->record_num].is_emergency_call = FALSE;
                        }
                    }
                    else
                    {
                        call_info_ptr->call_bcdinfo[call_info_ptr->record_num].is_num_present = FALSE;
                    }

#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
                    //set call info
                    if (PHONEBOOK_LND_STORAGE != storage)
                    {
                        call_start_time = MMIAPICOM_Tm2Second(call_info_entry.second,
                                                                                        call_info_entry.minute,
                                                                                        call_info_entry.hour,
                                                                                        call_info_entry.day,
                                                                                        call_info_entry.month,
                                                                                        call_info_entry.year + 2000);

                        call_info_ptr->call_bcdinfo[call_info_ptr->record_num].call_start_time = call_start_time;
                        call_info_ptr->call_bcdinfo[call_info_ptr->record_num].call_duration_time = call_info_entry.call_duration;
                    }
#endif

                    call_info_ptr->call_bcdinfo[call_info_ptr->record_num].isvideocall = CC_CALL_NORMAL;
                    call_info_ptr->call_bcdinfo[call_info_ptr->record_num].is_sim_record = TRUE;
#ifdef CALL_TIMES_SUPPORT
                    call_info_ptr->call_bcdinfo[call_info_ptr->record_num].call_times = 1;
#endif

//#ifdef MMI_PDA_SUPPORT
                    call_info_ptr->call_bcdinfo[call_info_ptr->record_num].call_type = call_type;
//#endif

                    call_info_ptr->record_num++;
                    call_info_ptr->sim_record_num++;

                    isChanged = TRUE;
                }
            }
        }

        if (isChanged)
        {
            MMICL_WriteNV(MMICL_CALL_ALL, call_info_ptr);
        }    

        SCI_FREE(ldn_entry_ptr);
        SCI_FREE(call_info_ptr);
    }

    //SCI_TRACE_LOW:"MMICL: InitCallInfo:is_ldn_valid=%d, max_record_num=%d, used_record_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_407_112_2_18_2_11_25_37,(uint8*)"ddd",phonebook_info.is_phonebook_exist, phonebook_info.max_record_num, phonebook_info.used_record_num);
}

/*****************************************************************************/
// 	Description : Initial Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICL_InitCallInfo(MN_DUAL_SYS_E dual_sys)
{
#if defined(MMI_OCI_SUPPORT)
    InitCallInfo(PHONEBOOK_OCI_STORAGE, dual_sys);
#elif defined(MMI_LDN_SUPPORT)
    InitCallInfo(PHONEBOOK_LND_STORAGE, dual_sys);
#endif

#if defined(MMI_ICI_SUPPORT)
    InitCallInfo(PHONEBOOK_ICI_STORAGE, dual_sys);
#endif
}

/*****************************************************************************/
// 	Description : Read Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
PUBLIC BOOLEAN MMICL_ReadCallInfo(PHONEBOOK_STORAGE_E storage, MN_DUAL_SYS_E dual_sys, PHONEBOOK_ENTRY_T *entry_ptr, CALL_INFO_ENTRY_T *call_info_entry_ptr)
#else
PUBLIC BOOLEAN MMICL_ReadCallInfo(PHONEBOOK_STORAGE_E storage, MN_DUAL_SYS_E dual_sys, PHONEBOOK_ENTRY_T *entry_ptr)
#endif
{
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_IS_NULL;

    if (dual_sys >= MMI_DUAL_SYS_MAX || PNULL == entry_ptr)
    {
        return FALSE;
    }

#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
    if (PHONEBOOK_ICI_STORAGE == storage
        || PHONEBOOK_OCI_STORAGE == storage)
    {
        err_code = PHONEBOOK_ReadCallInfoEntryEx(dual_sys, PHONEBOOK_LND_STORAGE, entry_ptr, call_info_entry_ptr);
    }
    else if (PHONEBOOK_LND_STORAGE == storage)
#endif
    {
        err_code = PHONEBOOK_ReadPhonebookEntryEx(dual_sys, PHONEBOOK_LND_STORAGE, entry_ptr);
    }

    //SCI_TRACE_LOW:"MMICL: MMICL_ReadCallInfo err_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_456_112_2_18_2_11_26_38,(uint8*)"d", err_code);

    if (ERR_MNDATAMAG_NO_ERR != err_code)
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : Update Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_UpdateCallInfo(MMICL_CALL_TYPE_E call_type, MN_DUAL_SYS_E dual_sys, MMICL_CALL_INFO_T *call_info_ptr)
{
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_IS_NULL;
    PHONEBOOK_ENTRY_T *entry_ptr = PNULL;
#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
    CALL_INFO_ENTRY_T call_info_entry = {0};
#endif
    PHONEBOOK_INFO_T phonebook_info = {0};
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_LND_STORAGE;

    if (dual_sys >= MMI_DUAL_SYS_MAX || PNULL == call_info_ptr)
    {
        return FALSE;
    }

    if (MMICL_CALL_DIALED == call_type)
    {
#if defined(MMI_OCI_SUPPORT)
        storage = PHONEBOOK_OCI_STORAGE;
#elif defined(MMI_LDN_SUPPORT)
        storage = PHONEBOOK_LND_STORAGE;
#else
        return FALSE;
#endif
    }
    else if (MMICL_CALL_MISSED == call_type)
    {
#if defined(MMI_ICI_SUPPORT)
        storage = PHONEBOOK_ICI_STORAGE;
#else
        return FALSE;
#endif
    }
    else
    {
        return FALSE;
    }

    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, storage);/*lint !e527*/

    if (!MMIAPIPHONE_GetSimExistedStatus(dual_sys) || !phonebook_info.is_phonebook_exist)
    {
        //SCI_TRACE_LOW:"MMICL: MMICL_UpdateCallInfo ldn file not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_512_112_2_18_2_11_26_39,(uint8*)"");

        return FALSE;
    }

    if (call_info_ptr->call_num.is_num_present)
    {
        entry_ptr = SCI_ALLOCAZ(sizeof(PHONEBOOK_ENTRY_T));

        if (PNULL != entry_ptr)
        {
            MMIPB_BCD_NUMBER_T bcd_num = {0};/*lint !e64*/
            BOOLEAN is_find = FALSE;
            wchar pb_name[MMIPB_MAX_STRING_LEN + 1] = {0};
            MMI_STRING_T num_name = {0};   

            //SCI_MEMSET(entry_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));

            bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(call_info_ptr->call_num.number.number_type, MN_NUM_PLAN_UNKNOW );
            bcd_num.number_len = MIN(MMIPB_BCD_NUMBER_MAX_LEN, call_info_ptr->call_num.number.num_len);
            MMI_MEMCPY(bcd_num.number, MMIPB_BCD_NUMBER_MAX_LEN, call_info_ptr->call_num.number.party_num, MN_MAX_ADDR_BCD_LEN, bcd_num.number_len);

            SCI_MEMSET(pb_name, 0x00, sizeof(pb_name));
            num_name.wstr_len = 0;
            num_name.wstr_ptr = pb_name;

            is_find = MMIAPIPB_GetNameByNumber(&bcd_num, &num_name, MMIPB_MAX_STRING_LEN, FALSE);

            if (is_find && num_name.wstr_len > 0)
            {
                BOOLEAN is_usc2 = FALSE;
                uint8 input_len   = 0;
                uint16 output_len = 0;
                uint8 output_str[MMIPB_NAME_MAX_LEN] = {0};
                uint8 ascii_str[MMIPB_NAME_MAX_LEN] = {0};
                uint8 *in_str_ptr = PNULL;
                uint8 default_data[MMIPB_NAME_MAX_LEN * 2] = {0};
                uint16 data_length = 0;
                PHONEBOOK_ALPHABET_TYPE_E alpha_type = PHONEBOOK_ALPHABET_ASCII;

                is_usc2 = !(MMIAPICOM_IsASCIIString(num_name.wstr_ptr, num_name.wstr_len));

                if(!is_usc2)
                {
                    input_len = (uint8)MIN(num_name.wstr_len, MMIPB_NAME_MAX_LEN);

                    //wstr to str
                    MMI_WSTRNTOSTR(ascii_str, MMIPB_NAME_MAX_LEN, num_name.wstr_ptr, MMIPB_NAME_MAX_LEN, input_len);

                    in_str_ptr = ascii_str;
                }
                else
                {
                    input_len = (uint8)MIN(num_name.wstr_len * sizeof(wchar), MMIPB_NAME_MAX_LEN);
                    in_str_ptr = (uint8 *)num_name.wstr_ptr;
                }

                alpha_type = is_usc2 ? PHONEBOOK_ALPHABET_UCS2 : PHONEBOOK_ALPHABET_ASCII;

                if (PHONEBOOK_ALPHABET_ASCII == alpha_type)
                {
                    // translate the ascii to default according GSM03.38
                    data_length = MMIAPICOM_Ascii2default(in_str_ptr, default_data, input_len);

                    input_len = MIN(data_length, MMIPB_NAME_MAX_LEN);

                    SCI_MEMCPY(in_str_ptr, default_data, input_len);	
                }

                if(!MMIAPICOM_EncodeGSMAlphabet(
                                                                                alpha_type, 
                                                                                input_len, 
                                                                                in_str_ptr, 
                                                                                &output_len, 
                                                                                output_str,
                                                                                (MMIPB_NAME_MAX_LEN -1)
                                                                                ))
                {
                    //SCI_TRACE_LOW:"MMICL:  MMICL_UpdateCallInfo return flase "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_590_112_2_18_2_11_26_40,(uint8*)"");

                }

                entry_ptr->alpha_len = MIN(output_len, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);

                SCI_MEMCPY(entry_ptr->alpha, output_str, entry_ptr->alpha_len);
            }
            else
            {
                entry_ptr->alpha_len = 0;
            }

            entry_ptr->alpha_len = 0;

            entry_ptr->entry_id = PHONEBOOK_DEFAULT_ENTRY_ID;
            entry_ptr->ton = call_info_ptr->call_num.number.number_type;
            entry_ptr->npi = call_info_ptr->call_num.number.number_plan;
            entry_ptr->number_len = MIN(PHONEBOOK_MAX_DN_NUMBER_LEN, call_info_ptr->call_num.number.num_len);
            SCI_MEMSET(entry_ptr->number, 0xFF, PHONEBOOK_MAX_DN_NUMBER_LEN);
            SCI_MEMCPY(entry_ptr->number, call_info_ptr->call_num.number.party_num, entry_ptr->number_len);
            entry_ptr->time = call_info_ptr->call_time.start_time;

#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
            if (PHONEBOOK_OCI_STORAGE == storage
                || PHONEBOOK_ICI_STORAGE == storage)
            {
                MMI_TM_T tm = {0};
                float time_zone = 0;

                tm = MMIAPICOM_Second2Tm(call_info_ptr->call_time.start_time);
                if (tm.tm_year >= 2000)
                {
                    call_info_entry.year = (uint8)(tm.tm_year - 2000);
                }
                else
                {
                    call_info_entry.year = (uint8)(tm.tm_year - 1980);
                }
                call_info_entry.month = tm.tm_mon;
                call_info_entry.day = tm.tm_mday;
                call_info_entry.hour = tm.tm_hour;
                call_info_entry.minute = tm.tm_min;
                call_info_entry.second = tm.tm_sec;

                call_info_entry.is_timezone_support = TRUE;

                time_zone = MMIAPIACC_GetLocalTimeZone();

                if (time_zone > 12.75 || time_zone < -12.0)
                {
                    time_zone = 8.0;
                }

                if (time_zone < -0.25)
                {
                    call_info_entry.is_timezone_nega_or_posi = FALSE;
                }
                else
                {
                    call_info_entry.is_timezone_nega_or_posi = TRUE;
                }
                call_info_entry.timezone_value = (uint8)fabs((double)time_zone);

                if (call_info_ptr->call_time.start_time <= call_info_ptr->call_time.stop_time)
                {
                    call_info_entry.call_duration = call_info_ptr->call_time.stop_time - call_info_ptr->call_time.start_time;
                }
                if (0 != call_info_entry.call_duration)
                {
                    call_info_entry.call_status = TRUE;
                }
                else
                {
                    call_info_entry.call_status = FALSE;
                }

                err_code = PHONEBOOK_UpdateCallInfoEntryEx(dual_sys, storage, entry_ptr, &call_info_entry);
            }
            else
#endif
            {
                err_code = PHONEBOOK_UpdatePhonebookEntryEx(dual_sys, storage, entry_ptr);
            }

            SCI_FREE(entry_ptr);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMICL: MMICL_UpdateCallInfo dial num not present"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_680_112_2_18_2_11_26_41,(uint8*)"");
    }

    //SCI_TRACE_LOW:"MMICL: MMICL_UpdateCallInfo err_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_683_112_2_18_2_11_26_42,(uint8*)"d", err_code);

    if (ERR_MNDATAMAG_NO_ERR != err_code)
    {    
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : Delete Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_DeleteCallInfo(MMICL_CALL_TYPE_E call_type, MN_DUAL_SYS_E dual_sys, MMICL_CALL_INFO_T *call_info_ptr)
{
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_IS_NULL;
    PHONEBOOK_INFO_T phonebook_info = {0};
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_LND_STORAGE;

    SCI_TRACE_LOW("MMICL: Enter MMICL_DeleteCallInfo call_type = %d,dual_sys = %d",call_type,dual_sys);

    if (dual_sys >= MMI_DUAL_SYS_MAX || PNULL == call_info_ptr)
    {
        return FALSE;
    }

    if (MMICL_CALL_DIALED == call_type)
    {
#if defined(MMI_OCI_SUPPORT)
        storage = PHONEBOOK_OCI_STORAGE;
#elif defined(MMI_LDN_SUPPORT)
        storage = PHONEBOOK_LND_STORAGE;
#else
        return FALSE;
#endif
    }
    else if (MMICL_CALL_MISSED == call_type)
    {
#if defined(MMI_ICI_SUPPORT)
        storage = PHONEBOOK_ICI_STORAGE;
#else
        return FALSE;
#endif
    }
    else
    {
        return FALSE;
    }

    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, storage);/*lint !e527*/

    if (!MMIAPIPHONE_GetSimExistedStatus(dual_sys) || !phonebook_info.is_phonebook_exist)
    {
        //SCI_TRACE_LOW:"MMICL: MMICL_DeleteCallInfo ldn file not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_735_112_2_18_2_11_26_43,(uint8*)"");

        return FALSE;
    }

    if (call_info_ptr->call_num.is_num_present)
    {
        uint32 i = 0;
        PHONEBOOK_ENTRY_T *ldn_entry_ptr = PNULL;
#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
        CALL_INFO_ENTRY_T call_info_entry = {0};
#endif

        ldn_entry_ptr = SCI_ALLOCAZ(sizeof(PHONEBOOK_ENTRY_T));

        if (PNULL == ldn_entry_ptr)
        {
            //SCI_TRACE_LOW:"MMICL: MMICL_DeleteCallInfo No Memory"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_752_112_2_18_2_11_26_44,(uint8*)"");

            return FALSE;
        }

        //delete from sim
        for (i = phonebook_info.used_record_num; i >= 1; i--)
        {
            //SCI_MEMSET(ldn_entry_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));
        
            ldn_entry_ptr->entry_id = i;

#if defined(MMI_OCI_SUPPORT) || defined(MMI_ICI_SUPPORT)
            SCI_MEMSET(&call_info_entry, 0x00, sizeof(call_info_entry));

            MMICL_ReadCallInfo(storage, dual_sys, ldn_entry_ptr, &call_info_entry);
#else
            MMICL_ReadCallInfo(storage, dual_sys, ldn_entry_ptr);
#endif

            //check same record
            if (call_info_ptr->call_num.number.num_len == ldn_entry_ptr->number_len
                && call_info_ptr->call_num.number.number_plan == ldn_entry_ptr->npi
                && call_info_ptr->call_num.number.number_type == ldn_entry_ptr->ton
                && (0 == memcmp(call_info_ptr->call_num.number.party_num, ldn_entry_ptr->number, call_info_ptr->call_num.number.num_len)))
            {
                //delete 
                err_code = PHONEBOOK_DeletePhonebookEntryEx(dual_sys, storage, i);

                if (ERR_MNDATAMAG_NO_ERR != err_code)
                {
                    break;
                }
                else
                {
                    if(!MMK_IsOpenWin(MMICC_LDN_UPDATE_WAITING_WIN_ID))
                    {
                        OpenLdnUpdateWaitingWindow(MMICC_LDN_UPDATE_WAITING_WIN_ID);
                    }

                    MMIAPICL_SetLdnIsDeleteAll(FALSE);
                }

                //continue;
            }
        }
        SCI_FREE(ldn_entry_ptr);
    }    

    //SCI_TRACE_LOW:"MMICL: MMICL_DeleteCallInfo err_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_800_112_2_18_2_11_26_45,(uint8*)"d", err_code);

    if (ERR_MNDATAMAG_NO_ERR != err_code)
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : Delete All Call Info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_DeleteAllCallnfo(MMICL_CALL_TYPE_E call_type, MN_DUAL_SYS_E dual_sys)
{
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_IS_NULL;
    PHONEBOOK_INFO_T phonebook_info = {0};
    uint32 i = 0;
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_LND_STORAGE;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    if (MMICL_CALL_DIALED == call_type)
    {
#if defined(MMI_OCI_SUPPORT)
        storage = PHONEBOOK_OCI_STORAGE;
#elif defined(MMI_LDN_SUPPORT)
        storage = PHONEBOOK_LND_STORAGE;
#else
        return FALSE;
#endif
    }
    else if (MMICL_CALL_MISSED == call_type)
    {
#if defined(MMI_ICI_SUPPORT)
        storage = PHONEBOOK_ICI_STORAGE;
#else
        return FALSE;
#endif
    }
    else
    {
        return FALSE;
    }

    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, storage);/*lint !e527*/

    if (!MMIAPIPHONE_GetSimExistedStatus(dual_sys) || !phonebook_info.is_phonebook_exist)
    {
        //SCI_TRACE_LOW:"MMICL: MMICL_DeleteAllCallnfo ldn file not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_853_112_2_18_2_11_26_46,(uint8*)"");

        return FALSE;
    }

    //delete from sim
    for (i = phonebook_info.used_record_num; i >= 1; i--)
    {
        //delete 
        err_code = PHONEBOOK_DeletePhonebookEntryEx(dual_sys, storage, i);

        if (ERR_MNDATAMAG_NO_ERR != err_code)
        {
            break;
        }
        else
        {
            if(!MMK_IsOpenWin(MMICC_LDN_UPDATE_WAITING_WIN_ID))
            {
                OpenLdnUpdateWaitingWindow(MMICC_LDN_UPDATE_WAITING_WIN_ID);
            }

            //set current sim is deleting all
            MMIAPICL_SetLdnIsDeleteAll(TRUE);
        }

        //continue;
    }

    //SCI_TRACE_LOW:"MMICL: MMICL_DeleteAllCallnfo err_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_882_112_2_18_2_11_26_47,(uint8*)"d", err_code);

    if (ERR_MNDATAMAG_NO_ERR != err_code)
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : Is Call Info Ready
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICL_IsCallInfoReady(void)
{
    uint32 i = 0;
    BOOLEAN is_callinfo_ready = TRUE;

    for (i= 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (!s_is_call_info_ready[i])
        {
            is_callinfo_ready = FALSE;

            break;
        }
    }

    //SCI_TRACE_LOW:"MMICL_IsCallInfoReady is_callinfo_ready=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_911_112_2_18_2_11_26_48,(uint8*)"d", is_callinfo_ready);

    return is_callinfo_ready;
}

/*****************************************************************************/
// 	Description : Set Is Call Info Ready
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SetIsCallInfoReady(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ready)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    s_is_call_info_ready[dual_sys] = is_ready;

    return TRUE;
}

/*****************************************************************************/
// 	Description : Delete All LDN Call Info
//	Global resource dependence : none
//  Author:dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_DeleteAllLdnInfo(MMICL_CALL_TYPE_E call_type)
{
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_IS_NULL;
    PHONEBOOK_INFO_T phonebook_info = {0};
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_LND_STORAGE;
    MN_DUAL_SYS_E  dual_sys = MMI_DUAL_SYS_MAX;
    uint32 i = 0;

    if (MMICL_CALL_DIALED == call_type)
    {
#if defined(MMI_OCI_SUPPORT)
        storage = PHONEBOOK_OCI_STORAGE;
#elif defined(MMI_LDN_SUPPORT)
        storage = PHONEBOOK_LND_STORAGE;
#else
        return FALSE;
#endif
    }
    else if (MMICL_CALL_MISSED == call_type)
    {
#if defined(MMI_ICI_SUPPORT)
        storage = PHONEBOOK_ICI_STORAGE;
#else
        return FALSE;
#endif
    }
    else
    {
        return FALSE;
    }
    //获取当前需要删除LDN SIM index
    for(i =0; i < MMI_DUAL_SYS_MAX ;i++)
    {
        phonebook_info = PHONEBOOK_GetFileInfoEx(i, storage);

        /*check if sim is exist and has ldn file*/
        if(MMIAPIPHONE_GetSimExistedStatus(i) && phonebook_info.is_phonebook_exist 
            &&(0 != phonebook_info.used_record_num))
        {
            if(!MMK_IsOpenWin(MMICC_LDN_UPDATE_WAITING_WIN_ID))
            {
                OpenLdnUpdateWaitingWindow(MMICC_LDN_UPDATE_WAITING_WIN_ID);
            }
            /*delete all LDN*/
            err_code = PHONEBOOK_DeleteLndAllEntryEx(i);

            //SCI_TRACE_LOW:"MMICL: MMICL_DeleteAllCallnfo err_code=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_995_112_2_18_2_11_27_50,(uint8*)"d", err_code);

            if (ERR_MNDATAMAG_NO_ERR != err_code)
            {
                return FALSE;
            }
        }
    }

    if(i >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMICL: MMICL_DeleteAllCallnfo dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_983_112_2_18_2_11_27_49,(uint8*)"d", dual_sys);
        return FALSE;
    }

    //set current sim is deleting all
    MMIAPICL_SetLdnIsDeleteAll(TRUE);

    return TRUE;
}

/*****************************************************************************/
// 	Description : ldn正在等待界面不允许返回，等待ldn update confirm消息上来
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void OpenLdnUpdateWaitingWindow (MMI_WIN_ID_T win_id)
{
    MMI_STRING_T    wait_text = {0};
    
    MMK_CloseWin(win_id);
    
    //进入等待界面
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,win_id,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleLdnUpdateWaitingWindow);

    MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, TRUE);
}

/*****************************************************************************/
// 	Description : ldn正在等待界面不允许返回,等待ldn update confirm消息上来
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLdnUpdateWaitingWindow (
                                           MMI_WIN_ID_T      win_id, 	// 窗口的ID
                                           MMI_MESSAGE_ID_E     msg_id, 	// 窗口的内部消息ID
                                           DPARAM 		        param		// 相应消息的参数
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED://puk输入界面存在,wait window不允许返回
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : Is current sim delete all
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_GetLdnIsDeleteAll(void)
{
    //SCI_TRACE_LOW:"MMICL: GetLdnIsDeleteAll s_is_ldn_delete_all =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_LDN_1060_112_2_18_2_11_27_51,(uint8*)"d", s_is_ldn_delete_all);

    return s_is_ldn_delete_all;
}

/*****************************************************************************/
// 	Description : set Is current sim deleting all ldn
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICL_SetLdnIsDeleteAll( BOOLEAN is_delete_all)
{
    s_is_ldn_delete_all = is_delete_all;

    return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPICL_HandleLdnReadyInd(void* param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //APP_MN_LND_READY_IND_T ldn_ready_ind = *(APP_MN_LND_READY_IND_T *)param;
    APP_MN_CALL_READY_IND_T ldn_ready_ind = *(APP_MN_CALL_READY_IND_T *)param; 

    SCI_TRACE_LOW("HandleLdnReadyInd:dual_sys:%d",ldn_ready_ind.dual_sys);

    //Just remember dual_sys,and wait until when...do it
    if (ldn_ready_ind.dual_sys>=MMI_DUAL_SYS_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMIAPIIDLE_IdleWinIsOpen())
    {
        MMIAPICL_InitCallInfo(ldn_ready_ind.dual_sys);
    }
    else
    {
        s_ldn_flag |=0x01<<(ldn_ready_ind.dual_sys);
    }    

    return result;
}

#endif
