/***************************************************************************************
** File Name:      mmiemail_sp_config.c                                                *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    Head file of E-mail config.This File will gather functions that     *
**           special handle email account config. It can create a new account, set the *
**           account config, get the account config and so on. These functions don't   *
**           be changed by project changed.                                            *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create                                              *
**                                                                                     *
***************************************************************************************/
/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "os_api.h"
#include "sfs.h"
#include "guifont.h"
#include "mmifmm_export.h"

#include "mmiemail_sp_config_api.h"
#include "mmiemail_sp_store_api.h"
#include "mmiemail_utils.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_vector.h"
#include "EMS_Store_API.h"
#include "mmi_data.h"
#include "mmi_resource.h"
/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef struct email_cfgmgr_tag
{
   EMAIL_ACT_SETTING_T *act_set_ptr;
   HEmlSPMgr          parent_handle_ptr; 
   EMAIL_MEM_HANDLE_T *mem_mgr_ptr;
   uint32            profile_id;
}EMAIL_CFGMGR_T;

typedef enum
{
    EMA_BOOL = 0, 
    EMA_INT, 
    EMA_MAX_MAIL, 
    EMA_NUM_LIMIT, 
    EMA_MAX_FOURBTYES = 0x7fffffff
}EMAIL_PRFITEM_TYPE_E;
/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
#define EMAIL_SETTING_USERINFOR_NUM 7


/*-------------------------------------------------------------------------------------*
**                             LOCAL FUNCTION DECLARE                                  *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email sp config, init config.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_Init(EMAIL_CFGMGR_T *cfg_mgr_ptr);
/**************************************************************************************/
// Description : email sp config, check account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN SP_CFG_CheckAccount(EMAIL_CFGMGR_T *cfg_mgr_ptr);
/**************************************************************************************/
// Description : email sp config, get active account's info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_GetActiveActInfo(EMAIL_CFGMGR_T *cfg_mgr_ptr);
/**************************************************************************************/
// Description : email sp config, get active account's info by profile item.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_GetActUserInfoCfgItem(
                                                HConfigMgr cfg_handle_ptr,
                                                uint32 act_id,
                                                EMASP_PROFILE_ITEM prf_item_id,
                                                wchar **value_pptr
                                                );
/**************************************************************************************/
// Description : email sp config, get account's msg preference by item.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_GetActEmailPreferenceItem(
                                                 HConfigMgr cfg_handle_ptr,
                                                 uint32 act_id,
                                                 EMASP_PROFILE_ITEM prf_item_id,
                                                 EMAIL_ACT_MSGPREFERENCE_T *email_prf_ptr
                                                 );
/**************************************************************************************/
// Description : email sp config, set is_value to uiProfile item.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL __inline int32 SP_CFG_FillStEmlActMsgPre(
                                         EMAIL_ACT_MSGPREFERENCE_T *email_prf_ptr,
                                         EMASP_PROFILE_ITEM prf_item_id,
                                         BOOLEAN is_value
                                         );
/**************************************************************************************/
// Description : email sp config, free account info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SP_CFG_FreeActInfo(void *data_ptr);
/**************************************************************************************/
// Description : email sp config, get profile.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_GetProfile(EMAIL_CFGMGR_T *cfg_mgr_ptr);

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email sp, to init config handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_InitHandle(
                                        HConfigMgr *cfg_handle_pptr,
                                        void       *parent_handle_ptr
                                        )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = PNULL;
    int ret = EMAIL_SUCCESS;

    if (PNULL == cfg_handle_pptr || PNULL == parent_handle_ptr)
    {
        EMA_UTILS_TRACE("MMIEMAIL_SP_CFG_InitHandle, param error");
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        cfg_mgr_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_CFGMGR_T));
        EMA_UTILS_BREAKNULL(cfg_mgr_ptr, ret, "out of memory");
        EMA_UTILS_MEMSET(cfg_mgr_ptr, 0, sizeof(EMAIL_CFGMGR_T));
        
        cfg_mgr_ptr->parent_handle_ptr = (HEmlSPMgr)parent_handle_ptr;
        cfg_mgr_ptr->act_set_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_ACT_SETTING_T));
        EMA_UTILS_BREAKNULL(cfg_mgr_ptr->act_set_ptr, ret, "out of memory");
        EMA_UTILS_MEMSET(cfg_mgr_ptr->act_set_ptr, 0, sizeof(EMAIL_ACT_SETTING_T));
        
        ret = SP_CFG_Init(cfg_mgr_ptr);
        EMA_UTILS_BREAKIF(ret, "SP_CFG_Init failed");
        
        *cfg_handle_pptr = (HConfigMgr)cfg_mgr_ptr;
        
    } while(0);
    
    if (EMAIL_SUCCESS != ret)
    {
        MMIEMAIL_SP_CFG_DeleteHandle((HConfigMgr*)&cfg_mgr_ptr);
        *cfg_handle_pptr = PNULL;
    }
    return ret;
}

/**************************************************************************************/
// Description : email sp, to delete config handle.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DeleteHandle(HConfigMgr *cfg_handle_pptr)
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = PNULL;
    
    if (PNULL == cfg_handle_pptr || PNULL == *cfg_handle_pptr)
    {
        EMA_UTILS_LOG_INFO(("MMIEMAIL_SP_CFG_DeleteHandle, param error."));
        return EMAIL_BADPARAM;
    }
    
    cfg_mgr_ptr = (EMAIL_CFGMGR_T*)*cfg_handle_pptr;
    
    EMA_UTILS_FREEIF(PNULL, cfg_mgr_ptr->act_set_ptr);
    EMA_UTILS_FREEIF(PNULL, cfg_mgr_ptr);
    
    *cfg_handle_pptr = PNULL;
    
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp config, init config.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_Init(EMAIL_CFGMGR_T *cfg_mgr_ptr)
{
    if (PNULL == cfg_mgr_ptr || PNULL == cfg_mgr_ptr->act_set_ptr)
    {
        EMA_UTILS_LOG_INFO(("SP_CFG_Init, param error"));
        return EMAIL_BADPARAM;
    }
    
    if (!SP_CFG_CheckAccount(cfg_mgr_ptr))
    {
        cfg_mgr_ptr->act_set_ptr->act_state = E_EMAILA_NOT_ACCOUNT;
    }
    else
    {
        cfg_mgr_ptr->act_set_ptr->act_state = E_EMAILA_EXSIT_ACCOUNT;
        if (EMAIL_SUCCESS != SP_CFG_GetActiveActInfo(cfg_mgr_ptr))
        {
            EMA_UTILS_LOG_INFO(("SP_CFG_Init, EmailACfgMgr_GetActiveAct error"));
            return EMAIL_ERROR;
        }
    }
    
    EMA_UTILS_LOG_INFO(("SP_CFG_Init, get the profile info"));
    SP_CFG_GetProfile(cfg_mgr_ptr);
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, Exsit account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint32 MMIEMAIL_SP_CFG_ExsitAct(HConfigMgr cfg_handle_ptr)
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T*)cfg_handle_ptr;
    if (PNULL == cfg_mgr_ptr || PNULL == cfg_mgr_ptr->act_set_ptr)
    {
        EMA_UTILS_LOG_INFO(("MMIEMAIL_SP_CFG_ExsitAct, param error"));
        return EMAIL_BADPARAM;
    }
    
    return cfg_mgr_ptr->act_set_ptr->act_state;
}

/**************************************************************************************/
// Description : email sp, account's receive type.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC uint32 MMIEMAIL_SP_CFG_ActRecType(HConfigMgr cfg_handle_ptr)
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T*)cfg_handle_ptr;
    if (PNULL == cfg_mgr_ptr || PNULL == cfg_mgr_ptr->act_set_ptr)
    {
        EMA_UTILS_LOG_INFO(("MMIEMAIL_SP_CFG_ActRecType, param error"));
        return EMAIL_BADPARAM;
    }
    SP_CFG_GetActiveActInfo(cfg_mgr_ptr);
    
    return cfg_mgr_ptr->act_set_ptr->in_mailserv_type;
}

/**************************************************************************************/
// Description : email sp config, check account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN SP_CFG_CheckAccount(EMAIL_CFGMGR_T *cfg_mgr_ptr)
{
    int result = EMAIL_ERROR;
    HStoreMgr store_mgr_ptr = PNULL;
    EMAIL_VECTOR_T *vetor_model_ptr = PNULL;
    
    if (PNULL == cfg_mgr_ptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_CFG_CheckAccount, param error"));
        return FALSE;
    }
    
    do 
    {
        MMIEMAIL_VectorNew(&vetor_model_ptr);
        
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("SP_CFG_CheckAccount, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        result = MMIEMAIL_SP_Store_GetActInfo(store_mgr_ptr, 0,EMASP_ACTINFO_MAX_NUMBER, vetor_model_ptr);
        EMA_UTILS_BREAKIF(result, "SP_CFG_CheckAccount, MMIEMAIL_SP_Store_GetActInfo failed");
        
        MMIEMAIL_VectorSetPfnFree(vetor_model_ptr, SP_CFG_FreeActInfo);
        
        if (0 == MMIEMAIL_VectorGetVectorNumber(vetor_model_ptr))
        {
            result = EMAIL_ERROR;
        }
        
    } while(0);
    
    MMIEMAIL_VectorDelete(vetor_model_ptr);
    if (EMAIL_SUCCESS != result)
    {
        return FALSE;
    }
    
    return TRUE;
}

/**************************************************************************************/
// Description : email sp config, get active account's info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_GetActiveActInfo(EMAIL_CFGMGR_T *cfg_mgr_ptr)
{
    int result = EMAIL_ERROR;
    uint32 act_id_temp = 0;
    HStoreMgr store_mgr_ptr = PNULL;
    EMASP_PRFITEM_VAL value[2] = {0};
    HEMSStore    *ems_store_ptr = PNULL;
    
    if (PNULL == cfg_mgr_ptr || PNULL == cfg_mgr_ptr->act_set_ptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_CFG_GetActiveActInfo, param error."));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        
        ems_store_ptr = HEMSSTORE_New(PNULL, PNULL);
        HEMSSTORE_GetActiveActId(ems_store_ptr, (unsigned int*)&act_id_temp);
        HEMSSTORE_Delete(ems_store_ptr);
        
        cfg_mgr_ptr->act_set_ptr->active_act_id = act_id_temp;
        
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr,
            (EMASP_ACCOUNT_ID)act_id_temp,
            EMASP_PRFITEM_RECEIVE_SERVER_TYPE,
            (EMASP_PRFITEM_VAL*)&value[0]);
        EMA_UTILS_BREAKIF(result, "SP_CFG_GetActiveActInfo, Get account config item failed!");
        if (EMASP_POP3 == (uint32)value[0].pData)
        {
            cfg_mgr_ptr->act_set_ptr->in_mailserv_type = E_EMAILA_MAILSERV_POP3;
        }
        else if (EMASP_IMAP4 == (uint32)value[0].pData)
        {
            cfg_mgr_ptr->act_set_ptr->in_mailserv_type = E_EMAILA_MAILSERV_IMAP4;
        }
        
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : email sp config, get profile.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_GetProfile(EMAIL_CFGMGR_T *cfg_mgr_ptr)
{
    int result = EMAIL_ERROR;
    SFS_HANDLE piFile = PNULL;
    wchar file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 2] = {0};
    uint16 file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;
    uint32 write_len = 0;
    uint32 read_len = 0;
    
    if (PNULL == cfg_mgr_ptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_CFG_GetProfile, param error."));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
#ifdef EMA_USE_SDCARD
        if (MMI_DEVICE_SDCARD_1 == MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD))
        {
           file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)EMA_PROFILE_ACTIVE_FILE_1, EMA_UTILS_STRLEN(EMA_PROFILE_ACTIVE_FILE_1));
        }
        else
#endif
        {
            file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)EMA_PROFILE_ACTIVE_FILE, EMA_UTILS_STRLEN(EMA_PROFILE_ACTIVE_FILE));
        }
        
        if (0 == file_full_path_len)
        {
            result = EMAIL_ERROR;
            break;
        }
        
        if(!MMIAPIFMM_IsFileExist(file_full_path, file_full_path_len))
        {
            piFile = MMIAPIFMM_CreateFile(file_full_path, SFS_MODE_CREATE_NEW | SFS_MODE_WRITE, PNULL, PNULL);
            EMA_UTILS_BREAKNULL(piFile, result, "SP_CFG_GetProfile Create or open file failed.");
            
            cfg_mgr_ptr->profile_id = 1;
            
            MMIAPIFMM_WriteFile(piFile, &cfg_mgr_ptr->profile_id, sizeof(uint32), &write_len, 0);
        }
        else
        {
            piFile = MMIAPIFMM_CreateFile(file_full_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
            EMA_UTILS_BREAKNULL(piFile, result, "SP_CFG_GetProfile Create or open file failed.");
            
            MMIAPIFMM_ReadFile(piFile, &cfg_mgr_ptr->profile_id, sizeof(uint32), &read_len, 0);
        }
    } while(0);
    
    MMIAPIFMM_CloseFile(piFile);
    return result;
}

/**************************************************************************************/
// Description : email sp, create account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_CreateAccount(
                                           HConfigMgr cfg_handle_ptr, 
                                           wchar *act_name_ptr,
                                           uint32 *act_id_ptr,
                                           EMAIL_ACCOUNT_STATE_E act_state
                                           )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int32 result = EMAIL_ERROR;
    uint32 act_id_temp = 0;
    HStoreMgr  store_mgr_ptr = PNULL;
    
    if (PNULL == cfg_mgr_ptr|| PNULL == act_id_ptr || PNULL == act_name_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_CreateAccount, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_CreateAccount, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        
        result = MMIEMAIL_SP_Store_CreateAccount(store_mgr_ptr, act_name_ptr, &act_id_temp);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_CreateAccount Create account failed.");
        
        *act_id_ptr = act_id_temp;
        
        if (E_EMAILA_NOT_ACCOUNT == act_state)
        {
            MMIEMAIL_SP_CFG_SetActiveAct(cfg_handle_ptr, act_id_temp);
            
            cfg_mgr_ptr->act_set_ptr->act_state = E_EMAILA_EXSIT_ACCOUNT;
            cfg_mgr_ptr->act_set_ptr->active_act_id = act_id_temp;
            cfg_mgr_ptr->act_set_ptr->in_mailserv_type = E_EMAILA_MAILSERV_POP3;
        }
        
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : email sp, set user info date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetUserInfoData(
                                             HConfigMgr cfg_handle_ptr,
                                             uint32 act_id,
                                             uint32 mask,
                                             EMAIL_ACT_USERINFO_T *user_info_ptr
                                             )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    EMASP_PRFITEM_VAL value[2] = {0};
    HStoreMgr store_mgr_ptr = PNULL;

    if (PNULL == cfg_mgr_ptr||PNULL == user_info_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetUserInfoData, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetUserInfoData, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        if (SETTINGMASK_USERINFO_FULL == mask)
        {
            wchar *pwszValue[EMAIL_SETTING_USERINFOR_NUM] = {0};
            int i = 0;
//             pwszValue[0] = user_info_ptr->name_ptr;
//             pwszValue[1] = user_info_ptr->addr_ptr;
//             pwszValue[2] = user_info_ptr->reply_addr_ptr;
//             pwszValue[3] = user_info_ptr->id_ptr;
//             pwszValue[4] = user_info_ptr->password_ptr;
//             pwszValue[5] = user_info_ptr->smtp_id_ptr;
//             pwszValue[6] = user_info_ptr->smtp_password_ptr;
            MMI_MEMCPY((void *)pwszValue, EMAIL_SETTING_USERINFOR_NUM*sizeof(wchar *), (void *)user_info_ptr, sizeof(*user_info_ptr), sizeof(*user_info_ptr));
            
            for (i = EMASP_PRFITEM_ACCOUNT_NAME; i <= EMASP_PRFITEM_SMTP_PASSWORD; i++)
            {
                value[0].pData = (void *)pwszValue[i - 1];
                result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, i, &value[0]);
                EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_SetActCfgItem, error.");
            }
        }
        else if (SETTINGMASK_USERINFO_USERNME == (SETTINGMASK_USERINFO_USERNME & mask))
        {
            value[0].pData = (void *)user_info_ptr->name_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_ACCOUNT_NAME, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_USERINFO_EMLADDR == (SETTINGMASK_USERINFO_EMLADDR & mask))
        {
            value[0].pData = (void *)user_info_ptr->addr_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_ACCOUNT_EMAIL, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_USERINFO_RPLYTOADDR == (SETTINGMASK_USERINFO_RPLYTOADDR & mask))
        {
            value[0].pData = (void *)user_info_ptr->reply_addr_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_REPLY_TO_EMAIL, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_USERINFO_USERID == (SETTINGMASK_USERINFO_USERID & mask))
        {
            value[0].pData = (void *)user_info_ptr->id_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_EMAIL_USER_ID, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_USERINFO_USERPASSWORD == (SETTINGMASK_USERINFO_USERPASSWORD & mask))
        {
            value[0].pData = (void *)user_info_ptr->password_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_EMAIL_PASSWORD, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_USERINFO_SMTPUSERID == (SETTINGMASK_USERINFO_SMTPUSERID & mask))
        {
            value[0].pData = (void *)user_info_ptr->smtp_id_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_USER_ID, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_USERINFO_SMTPUSERPASSWORD == (SETTINGMASK_USERINFO_SMTPUSERPASSWORD & mask))
        {
            value[0].pData = (void *)user_info_ptr->smtp_password_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_PASSWORD, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
    } while(0);
    return result;
}

/**************************************************************************************/
// Description : email sp, set come in email servece date , pop3.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData(
                                                     HConfigMgr cfg_handle_ptr, 
                                                     uint32 act_id, 
                                                     uint32 mask, 
                                                     EMAIL_ACT_POP3MAILSERV_T *pop3_mail_serv_ptr
                                                     )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    EMASP_PRFITEM_VAL value[2] = {0};
    HStoreMgr  store_mgr_ptr = PNULL;
    
    if (PNULL == cfg_mgr_ptr||PNULL == pop3_mail_serv_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        if (SETTINGMASK_INCOMINGMAILSERV_FULL == mask)
        {
            value[0].pData = (void *)EMASP_POP3;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_SERVER_TYPE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)pop3_mail_serv_ptr->serv_name_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_POP3_SERVER, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)pop3_mail_serv_ptr->port_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_POP3_PORT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)pop3_mail_serv_ptr->is_use_ssl;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_POP3_SSL_OR_TLS, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_INCOMINGMAILSERV_TYPE == (SETTINGMASK_INCOMINGMAILSERV_TYPE & mask))
        {
            value[0].pData = (void *)EMASP_POP3;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_SERVER_TYPE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_INCOMINGMAILSERV_NAME == (SETTINGMASK_INCOMINGMAILSERV_NAME & mask))
        {
            value[0].pData = (void *)pop3_mail_serv_ptr->serv_name_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_POP3_SERVER, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_INCOMINGMAILSERV_PORT == (SETTINGMASK_INCOMINGMAILSERV_PORT & mask))
        {
            value[0].pData = (void *)pop3_mail_serv_ptr->port_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_POP3_PORT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_INCOMINGMAILSERV_USESSLORTLS == (SETTINGMASK_INCOMINGMAILSERV_USESSLORTLS & mask))
        {
            value[0].pData = (void *)pop3_mail_serv_ptr->is_use_ssl;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_POP3_SSL_OR_TLS, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmPOP3MaiLServData MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
    } while(0);
    return result;
}
 
/**************************************************************************************/
// Description : email sp, set come in email servece date , imap4.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData(
                                                      HConfigMgr cfg_handle_ptr, 
                                                      uint32 act_id,
                                                      uint32 mask, 
                                                      EMAIL_ACT_IMAP4MAILSERV_T *imap4_mail_serv_ptr
                                                      )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    EMASP_PRFITEM_VAL value[2] = {0};
    HStoreMgr  store_mgr_ptr = PNULL;
    
    if (PNULL == cfg_mgr_ptr|| PNULL == imap4_mail_serv_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {  
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        if (SETTINGMASK_INCOMINGMAILSERV_FULL == mask)
        {
            value[0].pData = (void *)EMASP_IMAP4;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_SERVER_TYPE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)imap4_mail_serv_ptr->serv_name_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_IMAP4_SERVER, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)imap4_mail_serv_ptr->port_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_IMAP4_SERVER_PORT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)imap4_mail_serv_ptr->is_use_ssl;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_IMAP4_SSL_OR_TLS, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_INCOMINGMAILSERV_TYPE == (SETTINGMASK_INCOMINGMAILSERV_TYPE & mask))
        {
            value[0].pData = (void *)EMASP_IMAP4;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_SERVER_TYPE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_INCOMINGMAILSERV_NAME == (SETTINGMASK_INCOMINGMAILSERV_NAME & mask))
        {
            value[0].pData = (void *)imap4_mail_serv_ptr->serv_name_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_IMAP4_SERVER, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_INCOMINGMAILSERV_PORT == (SETTINGMASK_INCOMINGMAILSERV_PORT & mask))
        {
            value[0].pData = (void *)imap4_mail_serv_ptr->port_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_IMAP4_SERVER_PORT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_INCOMINGMAILSERV_USESSLORTLS == (SETTINGMASK_INCOMINGMAILSERV_USESSLORTLS & mask))
        {
            value[0].pData = (void *)imap4_mail_serv_ptr->is_use_ssl;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_IMAP4_SSL_OR_TLS, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetIncmIMAP4MaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        
    } while(0);
    return result;
}
 
/**************************************************************************************/
// Description : email sp, set out email servece date , pop3.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetOutgoMaiLServData(
                                                  HConfigMgr cfg_handle_ptr, 
                                                  uint32 act_id, 
                                                  uint32 mask, 
                                                  EMAIL_ACT_OUT_MAILSERV_T *send_serv_ptr
                                                  )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    EMASP_PRFITEM_VAL value[2] = {0};
    HStoreMgr  store_mgr_ptr = PNULL;
    
    if (PNULL == cfg_mgr_ptr|| PNULL == send_serv_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetOutgoMaiLServData, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetOutgoMaiLServData, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        if (SETTINGMASK_OUTGOINGMAILSERV_FULL == mask)
        {
            value[0].pData = (void *)send_serv_ptr->serv_name_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_SERVER, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetOutgoMaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)send_serv_ptr->port_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_SERVER_PORT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetOutgoMaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)send_serv_ptr->is_use_ssl;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_SSL_OR_TLS, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetOutgoMaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)send_serv_ptr->is_need_authentication;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_AUTHENTICATE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetOutgoMaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_OUTGOINGMAILSERV_NAME == (SETTINGMASK_OUTGOINGMAILSERV_NAME & mask))
        {
            value[0].pData = (void *)send_serv_ptr->serv_name_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_SERVER, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetOutgoMaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_OUTGOINGMAILSERV_PORT == (SETTINGMASK_OUTGOINGMAILSERV_PORT & mask))
        {
            value[0].pData = (void *)send_serv_ptr->port_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_SERVER_PORT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetOutgoMaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_OUTGOINGMAILSERV_USESSLORTLS == (SETTINGMASK_OUTGOINGMAILSERV_USESSLORTLS & mask))
        {
            value[0].pData = (void *)send_serv_ptr->is_use_ssl;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_SSL_OR_TLS, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetOutgoMaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_OUTGOINGMAILSERV_AUTHENTICATION == (SETTINGMASK_OUTGOINGMAILSERV_AUTHENTICATION & mask))
        {
            value[0].pData = (void *)send_serv_ptr->is_need_authentication;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_AUTHENTICATE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetOutgoMaiLServData, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
    } while(0);
    return result;
}

/**************************************************************************************/
// Description : email sp, set set email preference.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetEmailPreference(
                                              HConfigMgr cfg_handle_ptr, 
                                              uint32 act_id, 
                                              uint32 mask, 
                                              EMAIL_ACT_MSGPREFERENCE_T *email_prf_ptr
                                              )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    EMASP_PRFITEM_VAL value[2] = {0};
    HStoreMgr  store_mgr_ptr = PNULL;
    BOOLEAN  is_pop3 = FALSE;
    
    if (PNULL == cfg_mgr_ptr|| PNULL == email_prf_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetEmailPreference, param error"));
        return EMAIL_BADPARAM;
    }
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetEmailPreference, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        if (SETTINGMASK_MSGPREFERENCE_FULL == mask)
        {
            int32 ret =0;
            value[0].pData = (void *)email_prf_ptr->download_mode;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)email_prf_ptr->email_size_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_MAX_MSG_SIZE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            /*POP3*/
            ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_SERVER_TYPE, &value[0]);
	     if (EMAIL_SUCCESS != ret)
	     {
		  EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetActCfgItem failed!~"));
	     }
	     if (EMASP_POP3 == (uint32)value[0].pData)
            {
                is_pop3 = TRUE;
            }
            if (is_pop3)
            {
                value[0].pData = (void *)email_prf_ptr->is_copy_onserv;
                result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVER_LEAVE_COPY, &value[0]);
                EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            }
            
            value[0].pData = (void *)email_prf_ptr->is_del_onserv;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_DELETE_LEAVE_COPY, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)email_prf_ptr->is_save_sentemail;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SAVE_SENT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)email_prf_ptr->is_include_original;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_INCLUDE_ORIGINAL, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            /* Change for download mail's number limit once */
            value[0].pData = (void *)email_prf_ptr->limit_num;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_NUMBER_LIMIT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)email_prf_ptr->is_receive_rr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_READ_RECEIPT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            /*set add sig mode*/
            value[0].pData = (void *)email_prf_ptr->signature_mode;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_EMAIL_ADD_SIGNATURE_MODE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            
            value[0].pData = (void *)email_prf_ptr->is_receive_uuseen;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_UNSEEN, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            value[0].pData = (void *)email_prf_ptr->is_download_attach;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_DOWNLOAD_ATTACH, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_DOWNLOAD == (SETTINGMASK_MSGPREFERENCE_DOWNLOAD & mask))
        {
            value[0].pData = (void *)email_prf_ptr->download_mode;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_MSGSIZE == (SETTINGMASK_MSGPREFERENCE_MSGSIZE & mask))
        {
            value[0].pData = (void *)email_prf_ptr->email_size_ptr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_MAX_MSG_SIZE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_LEAVECOPYONSERV == (SETTINGMASK_MSGPREFERENCE_LEAVECOPYONSERV & mask))
        {
            /*POP3*/
            if (is_pop3)
            {
                value[0].pData = (void *)email_prf_ptr->is_copy_onserv;
                result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVER_LEAVE_COPY, &value[0]);
                EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
            }
        }
        else if (SETTINGMASK_MSGPREFERENCE_DELETION == (SETTINGMASK_MSGPREFERENCE_DELETION & mask))
        {
            value[0].pData = (void *)email_prf_ptr->is_del_onserv;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_DELETE_LEAVE_COPY, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_SAVESENTMSG == (SETTINGMASK_MSGPREFERENCE_SAVESENTMSG & mask))
        {
            value[0].pData = (void *)email_prf_ptr->is_save_sentemail;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SAVE_SENT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_INCLUDEORIGINAL == (SETTINGMASK_MSGPREFERENCE_INCLUDEORIGINAL & mask))
        {
            value[0].pData = (void *)email_prf_ptr->is_include_original;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_INCLUDE_ORIGINAL, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_NUMBERLIMIT == (SETTINGMASK_MSGPREFERENCE_NUMBERLIMIT & mask))
        {
            value[0].pData = (void *)email_prf_ptr->limit_num;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_NUMBER_LIMIT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_REQUSTREADRECPT == (SETTINGMASK_MSGPREFERENCE_REQUSTREADRECPT & mask))
        {
            value[0].pData = (void *)email_prf_ptr->is_receive_rr;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_READ_RECEIPT, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_ADDSIGNMODE == (SETTINGMASK_MSGPREFERENCE_ADDSIGNMODE & mask))
        {
            value[0].pData = (void *)email_prf_ptr->signature_mode;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_EMAIL_ADD_SIGNATURE_MODE, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_RECEIVEUNSEEN == (SETTINGMASK_MSGPREFERENCE_RECEIVEUNSEEN & mask))
        {
            value[0].pData = (void *)email_prf_ptr->is_receive_uuseen;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_UNSEEN, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
        else if (SETTINGMASK_MSGPREFERENCE_DOWNLOADATTACH == (SETTINGMASK_MSGPREFERENCE_DOWNLOADATTACH & mask))
        {
            value[0].pData = (void *)email_prf_ptr->is_download_attach;
            result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_DOWNLOAD_ATTACH, &value[0]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_SetEmailPreference, MMIEMAIL_SP_Store_SetActCfgItem, error.");
        }
   } while(0);
   return result;
}

/**************************************************************************************/
// Description : email sp, get user info date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetUserInfoData(
                                             HConfigMgr cfg_handle_ptr,
                                             uint32 act_id,
                                             EMAIL_ACT_USERINFO_T *user_info_ptr
                                             )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int32 result = EMAIL_SUCCESS;
    uint32 i = 0;
    wchar *value[EMAIL_SETTING_USERINFOR_NUM] = {0};
    
    if (PNULL == cfg_mgr_ptr|| PNULL == user_info_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetUserInfoData, param error."));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        for (i = EMASP_PRFITEM_ACCOUNT_NAME; i <= EMASP_PRFITEM_SMTP_PASSWORD; i++)
        {
            result = SP_CFG_GetActUserInfoCfgItem(cfg_handle_ptr, act_id, i, &value[i - 1]);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_GetUserInfoData, SP_CFG_GetActUserInfoCfgItem, error.");
        }
        if (EMAIL_SUCCESS != result)
        {
            break;
        }
//         user_info_ptr->name_ptr = value[0];
//         user_info_ptr->addr_ptr = value[1];
//         user_info_ptr->reply_addr_ptr = value[2];
//         user_info_ptr->id_ptr = value[3];
//         user_info_ptr->password_ptr = value[4];
//         user_info_ptr->smtp_id_ptr = value[5];
//         user_info_ptr->smtp_password_ptr = value[6];
        
        MMI_MEMCPY((void *)user_info_ptr, sizeof(*user_info_ptr), (void *)value, EMAIL_SETTING_USERINFOR_NUM * sizeof(wchar *), sizeof(*user_info_ptr));
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : email sp config, get active account's info by profile item.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_GetActUserInfoCfgItem(
                                         HConfigMgr cfg_handle_ptr,  
                                         uint32 act_id, 
                                         EMASP_PROFILE_ITEM prf_item_id,
                                         wchar **value_pptr
                                         )
{
    uint32 result = EMAIL_ERROR;
    wchar *value_ptr = PNULL;
    EMASP_PRFITEM_VAL value[2] = {0};
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    HStoreMgr  store_mgr_ptr = PNULL;
    
    if (PNULL == cfg_mgr_ptr || PNULL == value_pptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_CFG_GetActUserInfoCfgItem, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("SP_CFG_GetActUserInfoCfgItem, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, prf_item_id, &value[0]);
        EMA_UTILS_BREAKIF(result, "SP_CFG_GetActUserInfoCfgItem,Get account name failed!");
        if (PNULL != value[0].pData)
        {
            value_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar *)value[0].pData);
            EMA_UTILS_BREAKNULL(value_ptr, result, "email_prf_ptr->email_size_ptr malloc failed");
            *value_pptr = value_ptr;
        }
    } while(0);
    if (EMAIL_SUCCESS != result)
    {
        EMA_UTILS_FREEIF(PNULL, value_ptr);
        *value_pptr = PNULL;
    }
    return result;
}

/**************************************************************************************/
// Description : email sp, get pop3 servece date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetIncmPOP3MaiLServData(
                                                     HConfigMgr cfg_handle_ptr, 
                                                     uint32 act_id, 
                                                     EMAIL_ACT_POP3MAILSERV_T *pop3_mail_serv_ptr
                                                     )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    EMASP_PRFITEM_VAL value[2] = {0};
    HStoreMgr store_mgr_ptr = PNULL;

    if (PNULL == cfg_mgr_ptr ||PNULL == pop3_mail_serv_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetIncmPOP3MaiLServData, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetIncmPOP3MaiLServData, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_SERVER_TYPE, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        pop3_mail_serv_ptr->serv_type = (uint32)value[0].pData;
        
        EMA_UTILS_MEMSET(&value[0],0,sizeof(EMASP_PRFITEM_VAL));
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_POP3_SERVER, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        /*
        len = EMA_UTILS_WSTRLEN((wchar *)value[0].pData) * 2 + 2;
        pop3_mail_serv_ptr->serv_name_ptr = (wchar *)EMA_UTILS_MALLOC(cfg_mgr_ptr->mem_mgr_ptr,len);
        EMA_UTILS_BREAKNULL(pop3_mail_serv_ptr->serv_name_ptr, result, "pop3_mail_serv_ptr->serv_name_ptr malloc failed");
        EMA_UTILS_MEMSET(pop3_mail_serv_ptr->serv_name_ptr, 0, len);
        EMA_UTILS_MEMCPY(pop3_mail_serv_ptr->serv_name_ptr, (wchar *)value[0].pData, len);
        EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr,value[0].pData);
        */
        pop3_mail_serv_ptr->serv_name_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar *)value[0].pData);
        EMA_UTILS_BREAKNULL(pop3_mail_serv_ptr->serv_name_ptr, result, "email_prf_ptr->email_size_ptr malloc failed");
        
        EMA_UTILS_MEMSET(&value[0],0,sizeof(EMASP_PRFITEM_VAL));
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_POP3_PORT, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        /*
        len = EMA_UTILS_WSTRLEN((wchar*)value[0].pData) * 2 + 2;
        pop3_mail_serv_ptr->port_ptr = (wchar *)EMA_UTILS_MALLOC(len);
        EMA_UTILS_BREAKNULL(pop3_mail_serv_ptr->port_ptr, result, "pop3_mail_serv_ptr->port_ptr malloc failed");
        EMA_UTILS_MEMSET(pop3_mail_serv_ptr->port_ptr, 0, len);
        EMA_UTILS_MEMCPY(pop3_mail_serv_ptr->port_ptr, (wchar *)value[0].pData, len);
        EMA_UTILS_FREEIF(PNULL, value[0].pData)
        */
        pop3_mail_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar *)value[0].pData);
        EMA_UTILS_BREAKNULL(pop3_mail_serv_ptr->port_ptr, result, "email_prf_ptr->email_size_ptr malloc failed");
        
        EMA_UTILS_MEMSET(&value[0],0,sizeof(EMASP_PRFITEM_VAL));
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_POP3_SSL_OR_TLS, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        pop3_mail_serv_ptr->is_use_ssl = (BOOLEAN)(unsigned long)value[0].pData;
    } while(0);
    if (EMAIL_SUCCESS != result)
    {
        EMA_UTILS_FREEIF(PNULL, pop3_mail_serv_ptr->serv_name_ptr);
        EMA_UTILS_FREEIF(PNULL, pop3_mail_serv_ptr->port_ptr);
    }
    return result;
}

/**************************************************************************************/
// Description : email sp, get imap4 servece date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetIncmIMAP4MaiLServData(
                                                      HConfigMgr cfg_handle_ptr, 
                                                      uint32 act_id, 
                                                      EMAIL_ACT_IMAP4MAILSERV_T *imap4_mail_serv_ptr
                                                      )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    HStoreMgr store_mgr_ptr = PNULL;
    EMASP_PRFITEM_VAL value[2] = {0};

    if (PNULL == cfg_mgr_ptr ||PNULL == imap4_mail_serv_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetIncmIMAP4MaiLServData, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetIncmIMAP4MaiLServData, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_SERVER_TYPE, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        imap4_mail_serv_ptr->serv_type = (uint32)value[0].pData;
        
        EMA_UTILS_MEMSET(&value[0],0,sizeof(EMASP_PRFITEM_VAL));
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_IMAP4_SERVER, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        
        imap4_mail_serv_ptr->serv_name_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar *)value[0].pData);
        EMA_UTILS_BREAKNULL(imap4_mail_serv_ptr->serv_name_ptr, result, "email_prf_ptr->email_size_ptr malloc failed");
        
        EMA_UTILS_MEMSET(&value[0],0,sizeof(EMASP_PRFITEM_VAL));
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_IMAP4_SERVER_PORT, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        
        imap4_mail_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar *)value[0].pData);
        EMA_UTILS_BREAKNULL(imap4_mail_serv_ptr->port_ptr, result, "email_prf_ptr->email_size_ptr malloc failed");          
        
        EMA_UTILS_MEMSET(&value[0],0,sizeof(EMASP_PRFITEM_VAL));
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_IMAP4_SSL_OR_TLS, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        imap4_mail_serv_ptr->is_use_ssl = (BOOLEAN)(uint32)value[0].pData;
        
    } while(0);
    if (EMAIL_SUCCESS != result)
    {
        EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr,imap4_mail_serv_ptr->serv_name_ptr);
        EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr,imap4_mail_serv_ptr->port_ptr);
    }
    return result;
}

/**************************************************************************************/
// Description : email sp config, get sign dir.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetSignDir(
                                        HConfigMgr cfg_handle_ptr, 
                                        uint32 act_id, 
                                        char** sig_dir_pptr
                                        )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    HStoreMgr store_mgr_ptr = PNULL;
    EMASP_PRFITEM_VAL value = {0};

    if (PNULL == cfg_mgr_ptr || 0 == act_id||PNULL == sig_dir_pptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetSignDir, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetSignDir, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_EMAIL_CREATE_SIGNATURE, &value);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        *sig_dir_pptr = EMA_UTILS_STRDUP(cfg_mgr_ptr->mem_mgr_ptr,(char*)value.pData);
        EMA_UTILS_BREAKNULL(*sig_dir_pptr, result, "imap4_mail_serv_ptr->serv_name_ptr malloc failed");
        
    } while(0);
    if (EMAIL_SUCCESS != result)
    {
        EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr,*sig_dir_pptr);
    }
    return result;
}

/**************************************************************************************/
// Description : email sp, get out mail servece date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetOutgoMaiLServData(
                                                  HConfigMgr cfg_handle_ptr, 
                                                  uint32 act_id,  
                                                  EMAIL_ACT_OUT_MAILSERV_T *send_serv_ptr
                                                  )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    EMASP_PRFITEM_VAL value[2] = {0};
    HStoreMgr store_mgr_ptr = PNULL;

    if (PNULL == cfg_mgr_ptr ||PNULL == send_serv_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetOutgoMaiLServData, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetOutgoMaiLServData, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_SERVER, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        
        send_serv_ptr->serv_name_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar *)value[0].pData);
        EMA_UTILS_BREAKNULL(send_serv_ptr->serv_name_ptr, result, "email_prf_ptr->email_size_ptr malloc failed");
        
        EMA_UTILS_MEMSET(&value[0],0,sizeof(EMASP_PRFITEM_VAL));
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_SERVER_PORT, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        
        send_serv_ptr->port_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar *)value[0].pData);
        EMA_UTILS_BREAKNULL(send_serv_ptr->port_ptr, result, "email_prf_ptr->email_size_ptr malloc failed");
        
        EMA_UTILS_MEMSET(&value[0],0,sizeof(EMASP_PRFITEM_VAL));
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_SSL_OR_TLS, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        send_serv_ptr->is_use_ssl = (BOOLEAN)(uint32)value[0].pData;
        
        EMA_UTILS_MEMSET(&value[0],0,sizeof(EMASP_PRFITEM_VAL));
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_SMTP_AUTHENTICATE, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        send_serv_ptr->is_need_authentication = (BOOLEAN)(uint32)value[0].pData;
        
    } while(0);
    if (EMAIL_SUCCESS != result)
    {
        EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr,send_serv_ptr->serv_name_ptr);
        EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr,send_serv_ptr->port_ptr);
    }
    return result;
}

/**************************************************************************************/
// Description : email sp, get msg preference.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetEmailPreference(
                                              HConfigMgr cfg_handle_ptr, 
                                              uint32 act_id, 
                                              EMAIL_ACT_MSGPREFERENCE_T *email_prf_ptr
                                              )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    int i = 0;
    EMASP_PRFITEM_VAL value[2] = {0};
    BOOLEAN  bIMAP4 = FALSE;
    HStoreMgr store_mgr_ptr = PNULL;

    if (PNULL == cfg_mgr_ptr ||PNULL == email_prf_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetEmailPreference, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        int32 ret =0;
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetEmailPreference, param error"));
            result = EMAIL_ERROR;
            break;
        }
        ret = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, EMASP_PRFITEM_RECEIVE_SERVER_TYPE, &value[0]);
	 if (EMAIL_SUCCESS != ret)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetActCfgItem failed!~"));
        }
	 
	 if (EMASP_IMAP4 == (uint32)value[0].pData)
        {
            bIMAP4 = TRUE;
        }
        
        for (i = EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE; i <= EMASP_PRFITEM_EMAIL_ADD_SIGNATURE_MODE; i++)
        {
            if (bIMAP4 && EMASP_PRFITEM_RECEIVER_LEAVE_COPY == i)
            {
                continue;
            }
            result = SP_CFG_GetActEmailPreferenceItem(cfg_handle_ptr, act_id, i, email_prf_ptr);
            EMA_UTILS_BREAKIF(result, "SP_CFG_GetActEmailPreferenceItem error.");
        }
        
    } while(0);
    return result;
}

/**************************************************************************************/
// Description : email sp config, get account's msg preference by item.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_GetActEmailPreferenceItem(
                                           HConfigMgr cfg_handle_ptr, 
                                           uint32 act_id, 
                                           EMASP_PROFILE_ITEM prf_item_id,
                                           EMAIL_ACT_MSGPREFERENCE_T *email_prf_ptr
                                           )
{
    int result = EMAIL_ERROR;
    EMAIL_PRFITEM_TYPE_E prfitem_type = EMA_BOOL;
    EMASP_PRFITEM_VAL value[2] = {0};
    HStoreMgr store_mgr_ptr = PNULL;
    EMAIL_CFGMGR_T* cfg_mgr_ptr = (EMAIL_CFGMGR_T*)cfg_handle_ptr;
    
    if (PNULL == cfg_mgr_ptr ||PNULL == email_prf_ptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_CFG_GetActEmailPreferenceItem, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        switch(prf_item_id)
        {
        case  EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE:
        case  EMASP_PRFITEM_EMAIL_ADD_SIGNATURE_MODE:
            prfitem_type = EMA_INT;
            break;
        case EMASP_PRFITEM_NUMBER_LIMIT:
            prfitem_type = EMA_NUM_LIMIT;
            break;
        case EMASP_PRFITEM_MAX_MSG_SIZE:
            prfitem_type = EMA_MAX_MAIL;
            break;
        case EMASP_PRFITEM_NEW_MSG_NOTIFICATION:
        case EMASP_PRFITEM_RECEIVER_LEAVE_COPY:
        case EMASP_PRFITEM_DELETE_LEAVE_COPY:
        case EMASP_PRFITEM_SAVE_SENT:
        case EMASP_PRFITEM_INCLUDE_ORIGINAL:
        case EMASP_PRFITEM_READ_RECEIPT:
        case EMASP_PRFITEM_RECEIVE_UNSEEN:
        case EMASP_PRFITEM_DOWNLOAD_ATTACH:
            prfitem_type = EMA_BOOL;
            break;
        default:
            EMA_UTILS_LOG_ERROR(("Item is not found"));
            return EMAIL_ERROR;
        }
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("SP_CFG_GetActEmailPreferenceItem, param error"));
            result = EMAIL_ERROR;
            break;
        }
        
        result = MMIEMAIL_SP_Store_GetActCfgItem(store_mgr_ptr, act_id, prf_item_id, &value[0]);
        EMA_UTILS_BREAKIF(result, "Get account config item failed!");
        if (EMA_INT == prfitem_type)
        {
            if (EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE == prf_item_id)
            {
                email_prf_ptr->download_mode = (uint32)value[0].pData;
            }
            else if (EMASP_PRFITEM_EMAIL_ADD_SIGNATURE_MODE == prf_item_id)
            {
                email_prf_ptr->signature_mode = (uint32)value[0].pData;
            }
        }
        else if (EMA_BOOL == prfitem_type)
        {
            BOOLEAN is_value = (BOOLEAN)(uint32)value[0].pData;
            result = SP_CFG_FillStEmlActMsgPre(email_prf_ptr, prf_item_id, is_value);
            EMA_UTILS_BREAKIF(result, "SP_CFG_FillStEmlActMsgPre error");
        }
        else if (EMA_MAX_MAIL == prfitem_type)
        {
            if (PNULL != value[0].pData)
            {
                email_prf_ptr->email_size_ptr = EMA_UTILS_WSTRDUP(PNULL, value[0].pData);
                EMA_UTILS_BREAKNULL(email_prf_ptr->email_size_ptr, result, "email_prf_ptr->email_size_ptr malloc failed");
            }
        }
        else if (EMA_NUM_LIMIT == prfitem_type)
        {
            if (PNULL != value[0].pData)
            {
                email_prf_ptr->limit_num = (uint32)value[0].pData;
            }
        }
    } while(0);
    
    if (EMAIL_SUCCESS != result)
    {
        EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr,email_prf_ptr->email_size_ptr);
    }
    return result;
}
 
/**************************************************************************************/
// Description : email sp config, set bValue to uiProfile item.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL __inline int32 SP_CFG_FillStEmlActMsgPre(
                                         EMAIL_ACT_MSGPREFERENCE_T *email_prf_ptr,
                                         EMASP_PROFILE_ITEM prf_item_id, 
                                         BOOLEAN is_value
                                         )
{
    if (PNULL == email_prf_ptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_CFG_FillStEmlActMsgPre, param error"));
        return EMAIL_BADPARAM;
    }
    
    switch(prf_item_id)
    {
    case EMASP_PRFITEM_NEW_MSG_NOTIFICATION:
        break;
    case EMASP_PRFITEM_RECEIVER_LEAVE_COPY:
        email_prf_ptr->is_copy_onserv = is_value;
        break;
    case EMASP_PRFITEM_DELETE_LEAVE_COPY:
        email_prf_ptr->is_del_onserv = is_value;
        break;
    case EMASP_PRFITEM_SAVE_SENT:
        email_prf_ptr->is_save_sentemail = is_value;
        break;
    case EMASP_PRFITEM_INCLUDE_ORIGINAL:
        email_prf_ptr->is_include_original = is_value;
        break;
        //case EMASP_PRFITEM_NUMBER_LIMIT:
        /* Change for download mails' number limit once */ 
        //   msg_prf_ptr->pwszNumLimit = is_value;
        //   break;
    case EMASP_PRFITEM_READ_RECEIPT:
        email_prf_ptr->is_receive_rr = is_value;
        break;
    case EMASP_PRFITEM_RECEIVE_UNSEEN:
        email_prf_ptr->is_receive_uuseen = is_value;
        break;
    case EMASP_PRFITEM_DOWNLOAD_ATTACH:
        email_prf_ptr->is_download_attach = is_value;
        break;
    default:
        EMA_UTILS_LOG_ERROR(("Item is not found"));
        return EMAIL_ERROR;
    }
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, delete account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DeleteAccount(
                                           HConfigMgr cfg_handle_ptr,
                                           uint32 act_id,
                                           BOOLEAN is_del_all_act)
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int result = EMAIL_ERROR;
    HStoreMgr store_mgr_ptr = PNULL;

    if (PNULL == cfg_mgr_ptr|| 0 == act_id)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_DeleteAccount, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        if (PNULL == store_mgr_ptr)
        {
            EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_DeleteAccount, store_mgr_ptr is PNULL."));
            result = EMAIL_ERROR;
            break;
        }
        result = MMIEMAIL_SP_Store_DeleteAccount(store_mgr_ptr, act_id, is_del_all_act);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_DeleteAccount, delete account failed.");
    } while(0);

    return result;
}

/**************************************************************************************/
// Description : email sp, get active account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetActiveAct(
                                          HConfigMgr cfg_handle_ptr,
                                          uint32 *act_id_ptr
                                          )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    
    if (PNULL == cfg_mgr_ptr|| PNULL == act_id_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetActiveAct, param error"));
        return EMAIL_ERROR;
    }
    *act_id_ptr = cfg_mgr_ptr->act_set_ptr->active_act_id;
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, Set active account in file.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetActiveAct(
                                          HConfigMgr cfg_handle_ptr,
                                          uint32 act_id
                                          )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    int32 result = EMAIL_ERROR;
    HEMSStore *ems_store_ptr = PNULL;
    
    if (PNULL == cfg_mgr_ptr|| 0 == act_id)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetActiveAct, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        ems_store_ptr = HEMSSTORE_New(PNULL, PNULL);
        EMA_UTILS_BREAKNULL(ems_store_ptr, result, "create store handle failed!!");
        
        result = HEMSSTORE_SetActiveActId(ems_store_ptr, act_id);
        EMA_UTILS_BREAKIF(result, "set active actid failed!!");
        
        cfg_mgr_ptr->act_set_ptr->active_act_id = act_id;
        
    } while(0);
    
    if(PNULL != ems_store_ptr)
        HEMSSTORE_Delete(ems_store_ptr);
    
    return result;
}

/**************************************************************************************/
// Description : email sp, Set default configure for account.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetActDefaultCfg(
                                              HConfigMgr cfg_handle_ptr,
                                              uint32 act_id
                                              )
{
    HStoreMgr store_mgr_ptr = PNULL;
    int32 result = EMAIL_ERROR;
    EMASP_PRFITEM_VAL val = {0};
    wchar *text_ptr = PNULL;
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    
    if (PNULL == cfg_mgr_ptr|| 0 == act_id)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetActDefaultCfg, param error"));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        store_mgr_ptr = MMIEMAIL_SP_GetStoreHandle(cfg_mgr_ptr->parent_handle_ptr);
        
        /*connect info*/
        //EMASP_PRFITEM_DATA_SERVICE_PROVIDER       0x08 /*uint32*/
        val.pData = (void*)0;
        val.size = sizeof(uint32);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_DATA_SERVICE_PROVIDER,
            &val);
        
        //EMASP_PRFITEM_RECEIVE_SERVER_TYPE         0x09 /*uint32*/
        val.pData = (void*)EMASP_POP3;
        val.size = sizeof(uint32);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_RECEIVE_SERVER_TYPE,
            &val);
        
        //EMASP_PRFITEM_POP3_SSL_OR_TLS             0x0C /*BOOLEAN*/
        val.pData = (void*)FALSE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_POP3_SSL_OR_TLS,
            &val);
        
        //EMASP_PRFITEM_IMAP4_SSL_OR_TLS            0x0F /*BOOLEAN*/
        val.pData = (void*)FALSE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_IMAP4_SSL_OR_TLS,
            &val);
        
        //EMASP_PRFITEM_SMTP_SSL_OR_TLS             0x12 /*BOOLEAN*/
        val.pData = (void*)FALSE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_SMTP_SSL_OR_TLS,
            &val);
        //EMASP_PRFITEM_SMTP_AUTHENTICATE           0x13 /*BOOLEAN*/
        val.pData = (void*)TRUE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_SMTP_AUTHENTICATE,
            &val);
        
        /*message preferences*/
        //EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE         0x14 /*uint32*/
        val.pData = (void*)EMA_DOWNLOAD_ASK_USER;
        val.size = sizeof(uint32);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_EMAIL_DOWNLOAD_MODE,
            &val);
        
        //EMASP_PRFITEM_RECEIVER_LEAVE_COPY         0x17 /*BOOLEAN*/
        val.pData = (void*)TRUE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_RECEIVER_LEAVE_COPY,
            &val);
        
        //EMASP_PRFITEM_DELETE_LEAVE_COPY           0x18 /*BOOLEAN*/
        val.pData = (void*)FALSE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_DELETE_LEAVE_COPY,
            &val);
        
        //EMASP_PRFITEM_SAVE_SENT                   0x19 /*BOOLEAN*/
        val.pData = (void*)TRUE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_SAVE_SENT,
            &val);
        
        //EMASP_PRFITEM_INCLUDE_ORIGINAL            0x1A /*BOOLEAN*/
        val.pData = (void*)FALSE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_INCLUDE_ORIGINAL,
            &val);
        
        //EMASP_PRFITEM_DELIVERY_RECEIPT            0x1B /*BOOLEAN*/
        /* Change for Download mails' number limit once */
        text_ptr = (wchar*)L"10";
        val.pData = (void*)text_ptr;
        val.size = EMA_UTILS_WSTRLEN(text_ptr);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_NUMBER_LIMIT,
            &val);
        
        //EMASP_PRFITEM_READ_RECEIPT                0x1C /*BOOLEAN*/
        val.pData = (void*)FALSE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_READ_RECEIPT,
            &val);
        
        /* Add EMASP_PRFITEM_EMAIL_DEFAULTINBOXID (EM_DEFAULT_FOLDER_ID)*/
        val.pData = (void*)EMASP_INBOX_INDEX;
        val.size = sizeof(uint32);
        result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_EMAIL_DEFAULTINBOXID,
            &val);
        
        
        /* Add EMASP_PRFITEM_MAX_MSG_SIZE (EM_SIZE_LIMIT)*/
        text_ptr = (wchar*)L"3000";
        val.pData = (void*)text_ptr;
        val.size = EMA_UTILS_WSTRLEN(text_ptr);
        result = MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_MAX_MSG_SIZE,
            &val);
        
        /* Add EMASP_PRFITEM_POP3_SERVER (EM_RECEIVE_SERVER_ADDR)*/
        //       text_ptr = L"mailhz.techfaith.cn";
        //       val.pData = (void*)text_ptr;
        //       val.size = WSTRLEN(text_ptr);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_POP3_SERVER,
        //                              &val);
        
        /* Add EMASP_PRFITEM_POP3_PORT (EM_RECEIVE_SERVER_PORT) */
        text_ptr = (wchar*)L"110";
        val.pData = (void*)text_ptr;
        val.size = EMA_UTILS_WSTRLEN(text_ptr);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_POP3_PORT,
            &val);
        
        /* Add EMASP_PRFITEM_POP3_SERVER (EM_RECEIVE_SERVER_ADDR)*/
        //       text_ptr = L"mailhz.techfaith.cn";
        //       val.pData = (void*)text_ptr;
        //       val.size = WSTRLEN(text_ptr);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_IMAP4_SERVER,
        //                              &val);
        
        /* Add EMASP_PRFITEM_POP3_PORT (EM_RECEIVE_SERVER_PORT) */
        text_ptr = (wchar*)L"143";
        val.pData = (void*)text_ptr;
        val.size = EMA_UTILS_WSTRLEN(text_ptr);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_IMAP4_SERVER_PORT,
            &val);      
        
        /* Add EMASP_PRFITEM_EMAIL_USER_ID (EM_RECEIVE_USERNAME)*/
        //       text_ptr = L"ut_smtp_sender";
        //       val.pData = (void*)text_ptr;
        //       val.size = WSTRLEN(text_ptr);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_EMAIL_USER_ID,
        //                             &val);
        
        /* Add EMASP_PRFITEM_EMAIL_PASSWORD (EM_RECEIVE_PASSWORD)*/
        //       text_ptr = L"1234";
        //       val.pData = (void*)text_ptr;
        //       val.size = WSTRLEN(text_ptr);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_EMAIL_PASSWORD,
        //                              &val);
        
        /* Add EMASP_PRFITEM_SMTP_SERVER (EM_SEND_SERVER_ADDR)*/
        //       text_ptr = L"mailhz.techfaith.cn";
        //       val.pData = (void*)text_ptr;
        //       val.size = WSTRLEN(text_ptr);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_SMTP_SERVER,
        //                              &val);
        
        /* Add EMASP_PRFITEM_SMTP_SERVER_PORT (EM_SEND_SERVER_PORT)*/
        text_ptr = (wchar*)L"25";
        val.pData = (void*)text_ptr;
        val.size = EMA_UTILS_WSTRLEN(text_ptr);;
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_SMTP_SERVER_PORT,
            &val);
        
        /* Add EMASP_PRFITEM_SMTP_USER_ID (EM_SEND_USERNAME)*/
        //       text_ptr = L"ut_smtp_sender";
        //       val.pData = (void*)text_ptr;
        //       val.size = WSTRLEN(text_ptr);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_SMTP_USER_ID,
        //                             &val);
        
        /* Add EMASP_PRFITEM_SMTP_PASSWORD (EM_SEND_PASSWORD)*/
        //       text_ptr = L"1234";
        //       val.pData = (void*)text_ptr;
        //       val.size = WSTRLEN(text_ptr);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_SMTP_PASSWORD,
        //                             &val);
        //EMASP_PRFITEM_SAVE_SENT                   0x19 /*BOOLEAN*/
        val.pData = (void*)TRUE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_SAVE_SENT,
            &val);
        
        //EMASP_PRFITEM_ACCOUNT_EMAIL                   0x02 /*BOOLEAN*/
        //       text_ptr = L"ut_smtp_sender@mailhz.techfaith.cn";
        //       val.pData = (void*)text_ptr;
        //       val.size = WSTRLEN(text_ptr);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_ACCOUNT_EMAIL,
        //                               &val);
        //    
        //       //EMASP_PRFITEM_ACCOUNT_EMAIL                   0x03 /*BOOLEAN*/
        //       text_ptr = L"ut_smtp_sender@mailhz.techfaith.cn";
        //       val.pData = (void*)text_ptr;
        //       val.size = WSTRLEN(text_ptr);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_REPLY_TO_EMAIL,
        //                               &val);
        
        //EMASP_PRFITEM_ACCOUNT_EMAIL                   0x03 /*BOOLEAN*/
        //       val.pData = (void*)0;    //Data Service Provider :default
        //       val.size = sizeof(uint32);
        //       MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
        //                               act_id,
        //                               EMASP_PRFITEM_DATA_SERVICE_PROVIDER,
        //                               &val);
        /* receive unseen email */
        val.pData = (void*)TRUE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_RECEIVE_UNSEEN,
            &val);
        val.pData = (void*)TRUE;
        val.size = sizeof(BOOLEAN);
        MMIEMAIL_SP_Store_SetActCfgItem(store_mgr_ptr,
            act_id,
            EMASP_PRFITEM_DOWNLOAD_ATTACH,
            &val);
   } while(0);
   return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_USERINFO_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_ActUserInfo(
                                             HConfigMgr cfg_handle_ptr,
                                             EMAIL_ACT_USERINFO_T *user_into_ptr
                                             )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;

    if (PNULL == cfg_mgr_ptr|| PNULL == user_into_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_DEL_ActUserInfo, param error"));
        return EMAIL_BADPARAM;
    }
    
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, user_into_ptr->addr_ptr);
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, user_into_ptr->id_ptr);
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, user_into_ptr->name_ptr);
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, user_into_ptr->password_ptr);
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, user_into_ptr->reply_addr_ptr);
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, user_into_ptr->smtp_id_ptr);
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, user_into_ptr->smtp_password_ptr); 
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_POP3MAILSERV_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_IncmPOP3MailServ(
                                                  HConfigMgr cfg_handle_ptr,
                                                  EMAIL_ACT_POP3MAILSERV_T *pop3_mail_serv_ptr
                                                  )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    
    if (PNULL == cfg_mgr_ptr|| PNULL == pop3_mail_serv_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_DEL_IncmPOP3MailServ, param error"));
        return EMAIL_ERROR;
    }
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, pop3_mail_serv_ptr->port_ptr);
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, pop3_mail_serv_ptr->serv_name_ptr);
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_IMAP4MAILSERV_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_IncmIMAP4MailServ(
                                                   HConfigMgr cfg_handle_ptr,
                                                   EMAIL_ACT_IMAP4MAILSERV_T *imap4_mail_serv_ptr
                                                   )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    
    if (PNULL == cfg_mgr_ptr|| PNULL == imap4_mail_serv_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_DEL_IncmIMAP4MailServ, param error"));
        return EMAIL_BADPARAM;
    }
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, imap4_mail_serv_ptr->port_ptr);
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, imap4_mail_serv_ptr->serv_name_ptr);
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_OUT_MAILSERV_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_OutgoMailServ(
                                               HConfigMgr cfg_handle_ptr,
                                               EMAIL_ACT_OUT_MAILSERV_T *mail_serv_ptr
                                               )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;

    if (PNULL == cfg_mgr_ptr|| PNULL == mail_serv_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_DEL_OutgoMailServ, param error"));
        return EMAIL_BADPARAM;
    }
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, mail_serv_ptr->port_ptr);
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, mail_serv_ptr->serv_name_ptr);
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, Release struct EMAIL_ACT_MSGPREFERENCE_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_DEL_EmailPreference(
                                               HConfigMgr cfg_handle_ptr,
                                               EMAIL_ACT_MSGPREFERENCE_T *email_pref_ptr
                                               )
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    if (PNULL == cfg_mgr_ptr|| PNULL == email_pref_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_DEL_EmailPreference, param error"));
        return EMAIL_BADPARAM;
    }
    
    EMA_UTILS_FREEIF(cfg_mgr_ptr->mem_mgr_ptr, email_pref_ptr->email_size_ptr);
    
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp config, free account info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SP_CFG_FreeActInfo(void *data_ptr)
{
    EMASP_ACTINFO_T *act_info_ptr = (EMASP_ACTINFO_T *)data_ptr;
    EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE, act_info_ptr);
}

/**************************************************************************************/
// Description : email sp, get active profile.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_GetActiveProfile(
                                              HConfigMgr cfg_handle_ptr,
                                              uint32* profile_id_ptr)
{
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    
    if (PNULL == cfg_mgr_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_GetActiveProfile, param error"));
        return EMAIL_BADPARAM;
    }
    
    *profile_id_ptr = cfg_mgr_ptr->profile_id;
    
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email sp, set active profile.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_SetActiveProfile(
                                              HConfigMgr cfg_handle_ptr,
                                              uint32 profile_id)
{
    int result = EMAIL_ERROR;
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    SFS_HANDLE piFile = PNULL;
    wchar file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 2] = {0};
    uint32 write_len = 0;
    
    if (PNULL == cfg_mgr_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_SetActiveProfile, param error."));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
        cfg_mgr_ptr->profile_id = profile_id;
        
        piFile = MMIAPIFMM_CreateFile(file_full_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_WRITE, PNULL, PNULL);
        EMA_UTILS_BREAKNULL(piFile, result, "MMIEMAIL_SP_CFG_SetActiveProfile Create or open file failed.");
        
        MMIAPIFMM_WriteFile(piFile, &cfg_mgr_ptr->profile_id, sizeof(uint32), &write_len, 0);
    } while(0);
    
    MMIAPIFMM_CloseFile(piFile);
    return result;
}

/**************************************************************************************/
// Description : email sp, check profile ext.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_SP_CFG_CheckProfileExt(HConfigMgr cfg_handle_ptr)
{
    int result = EMAIL_ERROR;
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    wchar file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 2] = {0};
    uint32 file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;
    
    if (PNULL == cfg_mgr_ptr)
    {
        EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_CheckProfileExt, param error."));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
#ifdef EMA_USE_SDCARD
        if (MMI_DEVICE_SDCARD_1 == MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD))
        {
            file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)EMA_PROFILE_ACTIVE_FILE_1, EMA_UTILS_STRLEN(EMA_PROFILE_ACTIVE_FILE_1));
        }
        else
#endif
        {
            file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)EMA_PROFILE_ACTIVE_FILE, EMA_UTILS_STRLEN(EMA_PROFILE_ACTIVE_FILE));
        }
        
        if (MMIAPIFMM_IsFileExist(file_full_path, file_full_path_len))
        {
            result = EMAIL_SUCCESS;
        }
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_SP_CFG_CheckProfileExt custom file is  not exist");
        
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : email sp, write profile.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_WriteProFile(HConfigMgr cfg_handle_ptr,char * data_ptr)
{
    int result = EMAIL_ERROR;
    EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
    SFS_HANDLE piFile = PNULL;
    wchar file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 2] = {0};
    uint32 file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;
    uint32 write_len = 0;
    
    if (PNULL == cfg_mgr_ptr ||PNULL == data_ptr)
    {
        EMA_UTILS_LOG_ERROR(("SP_CFG_WriteProFile, param error."));
        return EMAIL_BADPARAM;
    }
    
    do 
    {
#ifdef EMA_USE_SDCARD
        if (MMI_DEVICE_SDCARD_1 == MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD))
        {
            file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)EMA_PROFILE_CUSTOM_FILE_1, EMA_UTILS_STRLEN(EMA_PROFILE_CUSTOM_FILE_1));
        }
        else
#endif
        {
            file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)EMA_PROFILE_CUSTOM_FILE, EMA_UTILS_STRLEN(EMA_PROFILE_CUSTOM_FILE));
        }
        
        if(MMIAPIFMM_IsFileExist(file_full_path, file_full_path_len))
        {
            MMIAPIFMM_DeleteFile(file_full_path, 0);
        }
        
        piFile = MMIAPIFMM_CreateFile(file_full_path, SFS_MODE_CREATE_NEW, PNULL, PNULL);
        EMA_UTILS_BREAKNULL(piFile, result, "SP_CFG_WriteProFile Create or open file failed.");
        
        MMIAPIFMM_WriteFile(piFile, data_ptr, EMA_UTILS_STRLEN(data_ptr), &write_len, 0);
    } while(0);
    MMIAPIFMM_CloseFile(piFile);
    
    return result;
}

/**************************************************************************************/
// Description : email sp, read profile.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 SP_CFG_ReadProFile(HConfigMgr cfg_handle_ptr,char** data_pptr)
{
   int result = EMAIL_ERROR;
   EMAIL_CFGMGR_T *cfg_mgr_ptr = (EMAIL_CFGMGR_T *)cfg_handle_ptr;
   SFS_HANDLE piFile = PNULL;
   char      *pcSource  = PNULL;
   int       iReadSize = 0;
   wchar file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 2] = {0};
   uint32 file_full_path_len = MMIFILE_FULL_PATH_MAX_LEN + 2;
   uint32 file_size = 0;
   
   if (PNULL == cfg_mgr_ptr ||PNULL == data_pptr)
   {
      EMA_UTILS_LOG_ERROR(("SP_CFG_ReadProFile, param error."));
      return EMAIL_BADPARAM;
   }
   
   do 
   {
#ifdef EMA_USE_SDCARD
        if (MMI_DEVICE_SDCARD_1 == MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD))
        {
            file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)EMA_PROFILE_CUSTOM_FILE_1, EMA_UTILS_STRLEN(EMA_PROFILE_CUSTOM_FILE_1));
        }
        else
#endif
        {
            file_full_path_len = GUI_GBToWstr(file_full_path, (uint8 *)EMA_PROFILE_CUSTOM_FILE, EMA_UTILS_STRLEN(EMA_PROFILE_CUSTOM_FILE));
        }
        
        if(MMIAPIFMM_IsFileExist(file_full_path, file_full_path_len))
        {       
         piFile = MMIAPIFMM_CreateFile(file_full_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ | SFS_MODE_WRITE, PNULL, PNULL);
         EMA_UTILS_BREAKNULL(piFile, result, "SP_CFG_ReadProFile Create or open file failed.");
        }
        file_size = MMIAPIFMM_GetFileSize(piFile);

        pcSource =EMA_UTILS_MALLOC(PNULL,file_size + 1);
        EMA_UTILS_BREAKNULL(pcSource, result,"SP_CFG_ReadProFile No memory");

        EMA_UTILS_MEMSET(pcSource, 0, file_size + 1);

        MMIAPIFMM_ReadFile(piFile, pcSource, file_size, (uint32*)&iReadSize, PNULL);
        if (iReadSize != (int)file_size)
        {
         result = EMAIL_ERROR;
         break;
        }
        * data_pptr = pcSource;
   } while(0);
   
   MMIAPIFMM_CloseFile(piFile);
   return result;
}

/*-------------------------------------------------------------------
Add by jade 2012-03-21 10:16:22
---------------------------------------------------------------------*/
PUBLIC void SP_CFG_GetSerConfigData(char* data_ptr, uint32* data_len)
{ 
    MMIRES_DATA_INFO_T  file_info = {0}; /*lint !e64*/

    if(PNULL == data_len)
    {
      return;
    }

    /*从系统中获取serverconf.txt的内容，存放到data_ptr内存中

    注意: data_ptr中所存的内容前三个字节必须为0xEF,0xBB,0xBF,标明buffer中的内容以utf8编码

    data_ptr: serverconf.txt的内容
    data_len: serverconf.txt的内容长度
    */
    MMI_GetDataInfo(R_EMAIL_CONFIG_DATA, &file_info);
    if(PNULL == data_ptr)
    {
      *data_len = file_info.data_len;
      return;
    }
	if(PNULL!=file_info.data_ptr)
	{
		SCI_MEMCPY(data_ptr,file_info.data_ptr,*data_len);
	}
 }

PUBLIC void SP_CFG_GetRootPath(char** udisk_path, char** sd1_path, char **sd2_path)
{
   /*获取email邮件的根目录路径,根路径的最大长度不能超过32字节*/
#ifdef EMA_USE_SDCARD
   *udisk_path  = "E:\\Email";
   *sd1_path    = "E:\\Email";
   *sd2_path    = "F:\\Email";
#else
   *udisk_path  = "D:\\Email";
   *sd1_path    = "D:\\Email";
   *sd2_path    = "D:\\Email";
#endif
}