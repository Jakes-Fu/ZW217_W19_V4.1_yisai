/*************************************************************************
** File Name:      mmisms_nv.c                                          *
** Author:         Tracy Zhang                                          *
** Date:           2006/10/24                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function about NV             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2006/10/24     Tracy Zhang      Create.                              *
*************************************************************************/


#define _MMISMS_NV_C_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_nv.h"
#include "mmi_modu_main.h"
#include "mmisms_app.h"
#include "nvitem.h"
#include "mn_api.h"
#include "mmi_nv.h"
#include "mmi_signal_ext.h"
#include "mmisms_filecontrol.h"
#ifdef HERO_ENGINE_TTS_SUPPORT 
#include "hero_tts_ui.h"
#endif
/*the length of set NV*/
const uint16 sms_nv_len[] =
{
    // SMS NV item
    sizeof( MMINV_SMS_REFERENCE_NUM_T ),    // MMINV_SMS_REFERENCE_NUM
    sizeof( MMINV_SMS_STATUS_REPORT_T ),    // MMINV_SMS_STATUS_REPORT_FLAG
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    sizeof( MMINV_SMS_REPLY_PATH_T ),
#endif
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
    sizeof( MMINV_SMS_AUTOSAVE_T ),            // MMINV_SMS_AUTOSAVE_FLAG
#endif    
    sizeof( MMINV_SMS_VALID_PERIOD_T ),        // MMINV_SMS_VALID_PERIOD
    sizeof( MMISMS_SMS_SAVEPOS_TYPE_E ),        // MMINV_SMS_SAVE_POS
    sizeof( MMISMS_PREFERRED_PATH_E ),        // MMINV_SMS_PREFERRED_CONNECTION
    sizeof( MMINV_SMS_FREQUENT_WORD_T ),    // MMINV_SMS_FREQUENT_WORD
#ifdef TTS_SUPPORT
    sizeof( BOOLEAN ),
    sizeof( uint8 ),
#endif
#ifdef HERO_ENGINE_TTS_SUPPORT
    sizeof(hero_tts_setting_struct),
#endif
    sizeof( MMINV_SMS_LOCK_STATE_T ),
    sizeof( MMINV_SMS_BROWSER_INFO_T ),
    sizeof( MMISMS_SL_OPERATION_E ),    
    sizeof( MMINV_SMS_IS_EXTRACT_T ),
    
#if defined(MMI_SMSCB_SUPPORT)
    // SMSCB NV item
    sizeof( MMINV_SMSCB_ENABLE_T ) * MMI_DUAL_SYS_MAX,
    sizeof( MMINV_SMSCB_CHANNEL_T ) * MMI_DUAL_SYS_MAX,
    sizeof( MMINV_SMSCB_LANG_T ) * MMI_DUAL_SYS_MAX,
#ifdef MMI_ETWS_SUPPORT
    sizeof(BOOLEAN) ,
#endif
#endif

    sizeof( MMISMS_ORDERTYPE_E ),
    sizeof( MMINV_SMS_FOLDER_TYPE_T ),
    sizeof( MMISMS_SECURITYBOX_PWD_T ),
#if defined(MMI_SMS_EXPORT_SUPPORT)
    sizeof( MMISMS_SETTING_PATH_TYPE ),
    sizeof( MMINV_SMS_EXPORT_FILENAME_T ),
#endif
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    sizeof( MMISMS_FONT_E ),
#endif
#ifdef MMI_TIMERMSG_SUPPORT
    sizeof( MMINV_SMS_TIMERMSG_T) * MMISMS_TIMERMSG_MAX_ITEM,
#endif	
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    //auto signature flag
    sizeof( BOOLEAN ),//MMINV_SMS_AUTO_SIGNATURE_FLAG
    (sizeof(wchar) * MMISMS_MAX_SIGNATURE_NUMBER_LEN),//MMINV_SMS_AUTO_SIGNATURE_CONTENT
#endif
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    sizeof( MMINV_SMS_SIM_SETMODE_T ) * MMI_DUAL_SYS_MAX,
#endif
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
    sizeof( BOOLEAN ),
#endif
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    sizeof( MMISMS_ALPHABET_MANAGEMENT_E ),
#endif

#ifdef MMI_SMS_FIX_SEND_SIM
        sizeof(BOOLEAN),
        sizeof(uint32),
#endif    
#ifdef MMI_MSG_RESEND
    sizeof( BOOLEAN ),
#endif
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
    sizeof( MN_SMS_VMW_IND_FLAG_T ) * MMI_DUAL_SYS_MAX,
#endif
    sizeof( BOOLEAN ),
    sizeof( BOOLEAN ),
    sizeof(MN_CB_MSG_ID_T)*5,
    sizeof( uint8 ),   //MMIUSERNV_SMSCB_MESSAGE_LIST_NUM ,
    sizeof( uint8 ),
};

/*****************************************************************************/
//     Description : register set module NV len and max item
//    Global resource dependence : none
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_RegNv(void)
{
#ifdef MMI_ETWS_SUPPORT
    MMI_RegModuleNv(MMI_MODULE_SMS, sms_nv_len, ARR_SIZE(sms_nv_len) + MMINV_MAX_SMS_RECORDS+MMINV_MAX_SMSCB_RECORDS);
#else
    MMI_RegModuleNv(MMI_MODULE_SMS, sms_nv_len, ARR_SIZE(sms_nv_len) + MMINV_MAX_SMS_RECORDS);
#endif
}

/*****************************************************************************/
//     Description :read SMS in NV by MMI
//    Global resource dependence :  
//  Author:alex.ye
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_NV_ReadSmsItem(                                    //RETURN:
                              uint16                    rid,        //IN:
                              MN_SMS_MESSAGE_ITEM_T    *item_ptr    //OUT:
                              )
{
    BOOLEAN ret_value = TRUE;
    uint16 true_id = 0;
    uint32 m_item_id = MMINV_SMS_NV_FIRST_ENTRY + rid;
    NVITEM_ERROR_E nv_result = NVERR_NONE;

    SCI_TraceLow("MMISMS:MMISMS_NV_ReadSmsItem start tick = %d",SCI_GetTickCount());
    if (PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_NV_ReadSmsItem item_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_NV_113_112_2_18_2_52_49_88,(uint8*)"");
        return FALSE;
    }
    
    SCI_MEMSET(item_ptr, 0x00, sizeof (MN_SMS_MESSAGE_ITEM_T));   
    
    true_id   = MMI_GetTrueNVItemId(m_item_id);
    nv_result = EFS_NvitemRead(
        true_id, 
        sizeof (MN_SMS_MESSAGE_ITEM_T),
        (void *)item_ptr
        ); 
    
    if (NVERR_NONE == nv_result)
    {
        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }
    SCI_TraceLow("MMISMS:MMISMS_NV_ReadSmsItem end tick = %d",SCI_GetTickCount());
    return (ret_value);
}    

/*****************************************************************************/
//     Description :write SMS in NV by MMI
//    Global resource dependence :  
//  Author:alex.ye
//    Note:
/*****************************************************************************/    
BOOLEAN MMIAPISMS_NV_WriteSmsItem(                                            //RETURN:
                               BOOLEAN                    is_need_sync,    //IN:
                               uint16                     rid,            //IN:
                               MN_SMS_MESSAGE_ITEM_T        *item_ptr        //IN:
                               )
{
    BOOLEAN ret_value = TRUE;
    BOOLEAN is_async  = !is_need_sync;
    uint16 true_id   = 0;
    uint32 m_item_id = MMINV_SMS_NV_FIRST_ENTRY + rid;
    NVITEM_ERROR_E nv_result = NVERR_NONE;
    SCI_TraceLow("MMISMS:MMIAPISMS_NV_WriteSmsItem start tick = %d",SCI_GetTickCount());
    true_id = MMI_GetTrueNVItemId(m_item_id);
    
    // call EFS API
    nv_result = EFS_NvitemWrite(
        true_id,
        sizeof (MN_SMS_MESSAGE_ITEM_T),
        (void *)item_ptr,
        is_async
        ); 
    
    if (NVERR_NONE == nv_result)
    {
        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }
    SCI_TraceLow("MMISMS:MMIAPISMS_NV_WriteSmsItem end tick = %d",SCI_GetTickCount());
    return (ret_value);
}    

/*****************************************************************************/
//     Description :set max SMS num in NV by MMI
//    Global resource dependence :  
//  Author:jian.ma
//    Note:
/*****************************************************************************/      
BOOLEAN MMISMS_NV_SetMaxSmsNum(                                            //RETURN:
                               MN_SMS_RECORD_ID_T        *max_sms_num_ptr    //OUT:
                               )
{
    *max_sms_num_ptr = MMINV_MAX_SMS_RECORDS;

    //SCI_TRACE_LOW:"SMS: MMISMS_NV_SetMaxSmsNum max num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_NV_186_112_2_18_2_52_49_89,(uint8*)"d", *max_sms_num_ptr);

    return TRUE;
}

/*****************************************************************************/
//     Description :delete all SMS item in NV by MMI
//    Global resource dependence :  
//  Author:jian.ma
//    Note:
/*****************************************************************************/    
BOOLEAN MMISMS_NV_DelAllSmsItem(                                            //RETURN:
                                MN_SMS_RECORD_ID_T    sms_in_me_valid_num    //IN:
                                )
{
    
    BOOLEAN ret_value = TRUE;
    uint16 begin_true_id = 0;
    uint32 begin_m_item_id = MMINV_SMS_NV_FIRST_ENTRY;
    NVITEM_ERROR_E nv_result = NVERR_NONE;

    SCI_TraceLow("MMISMS:MMISMS_NV_DelAllSmsItem start tick = %d",SCI_GetTickCount());
    begin_true_id = MMI_GetTrueNVItemId(begin_m_item_id);
    nv_result = EFS_NvitemDelete(
        begin_true_id,
        (uint16)(begin_true_id + sms_in_me_valid_num - 1)
        );
    
    if (NVERR_NONE == nv_result)
    {
        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }
    //SCI_TRACE_LOW:"SMS: MMISMS_NV_DelAllSmsItem nv_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_NV_219_112_2_18_2_52_49_90,(uint8*)"d", nv_result);
    SCI_TraceLow("MMISMS:MMISMS_NV_DelAllSmsItem end tick = %d",SCI_GetTickCount());
    return (ret_value);        
}

/*****************************************************************************/
//     Description : to init the flag of status report
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_NV_GetSmsIsLocked(MN_SMS_RECORD_ID_T record_id)
{
    MMINV_SMS_LOCK_STATE_T lock_state = {0};
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
    
    MMINV_READ(MMINV_SMS_LOCK_STATE, &lock_state, nv_value);
    
    if (MN_RETURN_SUCCESS !=nv_value)
    {
        MMINV_WRITE(MMINV_SMS_LOCK_STATE, &lock_state);
    }

    return lock_state.is_locked[record_id];
}

/*****************************************************************************/
//     Description : to Set the status report flag
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
void MMISMS_NV_SetSmsLockState(MN_SMS_RECORD_ID_T record_id, BOOLEAN is_locked)
{
    MMINV_SMS_LOCK_STATE_T lock_state = {0};
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
    
    MMINV_READ(MMINV_SMS_LOCK_STATE, &lock_state, nv_value);
    
    if (MN_RETURN_SUCCESS !=nv_value)
    {
        MMINV_WRITE(MMINV_SMS_LOCK_STATE, &lock_state);
    }
    lock_state.is_locked[record_id] = is_locked;

    MMINV_WRITE(MMINV_SMS_LOCK_STATE, &lock_state);
}

/*****************************************************************************/
//     Description : get sum size of all sms nv itme
//    Global resource dependence : 
//  Author: gang.tong
//    Note:
/*****************************************************************************/
uint32 MMIAPISMS_GetSumNvSize(void)
{
    uint16 count = 0;
    uint32 sms_nv_sum_size = 0;
    /* get sms settings nv size */
    for(count = 0; count < ARR_SIZE(sms_nv_len); count ++)
    {
        sms_nv_sum_size += sms_nv_len[count];
    }

    /* get sms all items nv size */
    sms_nv_sum_size += MMINV_MAX_SMS_RECORDS*(sizeof(MN_SMS_MESSAGE_ITEM_T));

#ifdef MMI_ETWS_SUPPORT
    /* get smscb all items nv size */
    sms_nv_sum_size += MMINV_MAX_SMSCB_RECORDS*(sizeof(MMINV_SMSCB_MESSAGE_ARRAY_T));
#endif

    return sms_nv_sum_size;
}

/*****************************************************************************/
// 	Description : Read Folder Type Manager
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_NV_ReadFolderTypeManager(MMINV_SMS_FOLDER_TYPE_T *folder_type_ptr)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == folder_type_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_NV_ReadFolderTypeManager folder_type_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_NV_293_112_2_18_2_52_49_91,(uint8*)"");
        return;
    }
    MMINV_READ(MMINV_SMS_FOLDER_TYPE, folder_type_ptr, nv_value);
    if (MN_RETURN_SUCCESS != nv_value)
    {
        SCI_MEMSET(folder_type_ptr, 0x00, sizeof(MMINV_SMS_FOLDER_TYPE_T));

        MMISMS_WriteFolderTypeManager(folder_type_ptr);
    }
}

/*****************************************************************************/
// 	Description :  Write Folder Type Manager
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_WriteFolderTypeManager(MMINV_SMS_FOLDER_TYPE_T *folder_type_ptr)
{
    if (PNULL == folder_type_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_WriteFolderTypeManager folder_type_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_NV_313_112_2_18_2_52_49_92,(uint8*)"");
        return;
    }
    MMINV_WRITE(MMINV_SMS_FOLDER_TYPE, folder_type_ptr);
}

/*****************************************************************************/
// 	Description : 设置安全信箱密码
//	Global resource dependence : 
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_NV_SetSecurityBoxPwd(
                                 const wchar *pwd_ptr,
                                 uint16     pwd_len
                                 )
{
    MMISMS_SECURITYBOX_PWD_T   phone_code = {0};
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMISMS_SECURITYBOX_PWD_T security_default_pwd ={{0},MMISMS_SECURITY_PWD_DEFAULT_LEN};
    
    if (PNULL == pwd_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_NV_SetSecurityBoxPwd pwd_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_NV_334_112_2_18_2_52_49_93,(uint8*)"");
        return;
    }

    //read power on password from
    MMINV_READ(MMINV_SMS_SECURITY_PWD,&phone_code,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        uint16 default_pwd_len =0;
        default_pwd_len = SCI_STRLEN(MMISMS_GetSecurityDefaultPwd());
        SCI_MEMCPY(security_default_pwd.pwd,MMISMS_GetSecurityDefaultPwd(),default_pwd_len);

        SCI_MEMCPY(&phone_code,&security_default_pwd,sizeof(MMISMS_SECURITYBOX_PWD_T));
        MMINV_WRITE(MMINV_SMS_SECURITY_PWD,&phone_code);
    }
    
    phone_code.pwd_len = (uint8)(pwd_len & 0x00FF);
    MMI_WSTRNTOSTR((uint8 *)phone_code.pwd,MMISMS_SECURITY_PWD_MAX_LEN,
        pwd_ptr,pwd_len,pwd_len);

    //write power on password into NV
    MMINV_WRITE(MMINV_SMS_SECURITY_PWD,&phone_code);
}

#if defined(MMI_SMS_EXPORT_SUPPORT)
/*****************************************************************************/
// 	Description : Read Export path
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReadExportPath(wchar *path_ptr)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_ReadExportPath path_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_NV_369_112_2_18_2_52_49_94,(uint8*)"");
        return;
    }
    MMINV_READ(MMINV_SMS_EXPORT_PATH, path_ptr, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;

        if(MMISMS_GetDefaultExportPath(path_ptr, &path_len))
        {
            MMISMS_WriteExportPath(path_ptr);
        }
    }
}

/*****************************************************************************/
// 	Description : Write Export path
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_WriteExportPath(wchar *path_ptr)
{
    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS: MMISMS_WriteExportPath path_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_NV_393_112_2_18_2_52_49_95,(uint8*)"");
        return;
    }
    MMINV_WRITE(MMINV_SMS_EXPORT_PATH, path_ptr);
}

/*****************************************************************************/
// 	Description : Read Export Method type
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReadExportMethodType(MMISMS_SETTING_PATH_TYPE *method_type)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    if (PNULL == method_type)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_ReadExportMethodType method_type = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_NV_409_112_2_18_2_52_49_96,(uint8*)"");
        return;
    }
    MMINV_READ(MMINV_SMS_EXPORT_METHOD, method_type, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        *method_type = MMISMS_SETTING_PATH_DEFAULT;
        MMISMS_WriteExportMethodType(MMISMS_SETTING_PATH_DEFAULT);
    }
}

/*****************************************************************************/
// 	Description : Write Export Method type
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_WriteExportMethodType(MMISMS_SETTING_PATH_TYPE method_type)
{
    MMINV_WRITE(MMINV_SMS_EXPORT_METHOD, &method_type);
}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: get securitybox default key
//  return: 
/*****************************************************************************/
PUBLIC char* MMISMS_GetSecurityDefaultPwd(void)
{
	return (char*)MMISMS_SECURITYBOX_DEFAULT_PWD;
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description :read SMSCB in NV by MMI
//    Global resource dependence :
//  Author:Candice
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_NV_ReadSmsCBItem(                                    //RETURN:
                              uint16                    rid,        //IN:
                              MMINV_SMSCB_MESSAGE_ARRAY_T    *item_ptr    //OUT:
                              )
{
    BOOLEAN ret_value = TRUE;
    uint16 true_id = 0;
    uint32 m_item_id = MMIUSERNV_SMSCB_MESSAGE_LIST_FIRST + rid;
    NVITEM_ERROR_E nv_result = NVERR_NONE;

    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem");

    if (PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_NV_ReadSmsCBItem item_ptr = PNULL"
        return FALSE;
    }

    SCI_MEMSET(item_ptr, 0x00, sizeof (MMINV_SMSCB_MESSAGE_ARRAY_T));
    //nv_result = EFS_NvitemRead(m_item_id,sizeof (MMINV_SMSCB_MESSAGE_ARRAY_T),(void *)item_ptr);

    true_id   = MMI_GetTrueNVItemId(m_item_id);
    nv_result = EFS_NvitemRead(
        true_id,
        sizeof (MMINV_SMSCB_MESSAGE_ARRAY_T),
        (void *)item_ptr
        );

    if (NVERR_NONE == nv_result)
    {
        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }

    return (ret_value);
}

/*****************************************************************************/
//     Description :write SMSCB in NV by MMI
//    Global resource dependence :
//  Author:Candice
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_NV_WriteSmsCBItem(                                            //RETURN:
                               BOOLEAN                    is_need_sync,    //IN:
                               uint16                     rid,            //IN:
                               MMINV_SMSCB_MESSAGE_ARRAY_T        *item_ptr        //IN:
                               )
{
    BOOLEAN ret_value = TRUE;
    BOOLEAN is_async  = !is_need_sync;
    uint16 true_id   = 0;
    uint32 m_item_id = MMIUSERNV_SMSCB_MESSAGE_LIST_FIRST + rid;
    NVITEM_ERROR_E nv_result = NVERR_NONE;

    SCI_TRACE_LOW("MMISMSCB: MMIAPISMS_NV_WriteSmsCBItem, rid=%d", rid);
    nv_result = EFS_NvitemWrite(m_item_id,sizeof(MMINV_SMSCB_MESSAGE_ARRAY_T),(void *)item_ptr,is_async);

    if (NVERR_NONE == nv_result)
    {
        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }

    SCI_TRACE_LOW("MMISMSCB: MMIAPISMS_NV_WriteSmsCBItem, ret_value=%d", ret_value);

    return (ret_value);
}

/*****************************************************************************/
//     Description :delete all SMSCB item in NV by MMI
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_NV_DelSmsCBItem(                                            //RETURN:
                                uint16    start_index, uint16 end_index
                                )
{

    BOOLEAN ret_value = TRUE;
    uint16 begin_true_id = 0;
    uint32 begin_m_item_id = MMIUSERNV_SMSCB_MESSAGE_LIST_FIRST ;
    NVITEM_ERROR_E nv_result = NVERR_NONE;

    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_DelSmsCBItem");

    if( !( (start_index <  end_index)
    && (start_index < MMINV_MAX_SMSCB_RECORDS)
    && (end_index < MMINV_MAX_SMSCB_RECORDS ) ) )
    {
        SCI_TRACE_LOW("MMISMSCB: invalid index");
        return FALSE;
    }

    nv_result = EFS_NvitemDelete(begin_m_item_id + start_index,
                                (uint16)(begin_m_item_id + end_index )
                                );

    if (NVERR_NONE == nv_result)
    {
        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }

    return (ret_value);
}


/*****************************************************************************/
//     Description :delete all SMSCB item in NV by MMI
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_NV_DelAllSmsCBItem(                                            //RETURN:
                                uint16    smscb_in_me_valid_num    //IN:
                                )
{

    BOOLEAN ret_value = TRUE;
    uint16 begin_true_id = 0;
    uint32 begin_m_item_id = MMIUSERNV_SMSCB_MESSAGE_LIST_FIRST;
    NVITEM_ERROR_E nv_result = NVERR_NONE;

    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_DelAllSmsCBItem");

    nv_result = EFS_NvitemDelete(begin_m_item_id,
        (uint16)(begin_m_item_id + smscb_in_me_valid_num - 1));

    if (NVERR_NONE == nv_result)
    {
        ret_value = TRUE;
    }
    else
    {
        ret_value = FALSE;
    }

    return (ret_value);
}

#endif
