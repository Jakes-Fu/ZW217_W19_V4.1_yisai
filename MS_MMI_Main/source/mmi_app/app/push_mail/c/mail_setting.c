/****************************************************************************
** File Name:      mail_setting.c                                            *
** Author:                                                                    *
** Date:           11/10/2010                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:                                                               *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2010      Jiaoyou.wu        Create
** 
****************************************************************************/
#define _MAIL_SETTING_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_push_mail_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mail_setting.h"
#include "comm_platform.h"
#include "comm_memory.h"
#include "mail_mem.h"
#include "mail_export.h"
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT //3 in 1 support
#include "mail_fldr.h"
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMAIL_SIZE_LIMIT              (3 * 1024 * 1024)

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
mail_setting_info s_mail_settings = {0};

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*==========================================================
 * Function     : mail_StartOpenFile
 * Author	    : sunguochen  
 * Description : open file
 * Parameter	: type          parameter       Description
                      char* file_name
 * Return	    : TRUE ;success/FALSE:fail
 * Effect	    : NO
 * History	:created  5/12/2010 
 ==========================================================*/
MMIFILE_HANDLE mail_StartOpenFile(char* file_name, comm_boolean is_create)
{
    wchar full_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 path_len = 0;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;

    /*check parameter*/
    if(NULL == file_name) 
    {
        return NULL;
    }

    if(SCI_STRLEN(file_name) > MMIFILE_FULL_PATH_MAX_LEN)
    {
        return NULL;
    }

    MMIAPICOM_StrToWstr((uint8 *)file_name, full_name);
    path_len = (uint16)MMIAPICOM_Wstrlen(full_name);

    if(is_create)
    {
        file_handle = MMIAPIFMM_CreateFile(full_name, SFS_MODE_READ |SFS_MODE_WRITE |SFS_MODE_OPEN_ALWAYS, PNULL, PNULL);
    }
    else
    {
        file_handle = MMIAPIFMM_CreateFile(full_name, SFS_MODE_READ |SFS_MODE_WRITE |SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    }

    return file_handle;
}

/*==========================================================
 * Function     : mail_StartOpenFile
 * Author	    : sunguochen  
 * Description : open file
 * Parameter	: type          parameter       Description
                      char* file_name
 * Return	    : TRUE ;success/FALSE:fail
 * Effect	    : NO
 * History	:created  5/12/2010 
 ==========================================================*/
MMIFILE_HANDLE mail_StartNewFile(char* file_name)
{
    wchar full_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 path_len = 0;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;

    /*check parameter*/
    if(NULL == file_name) 
    {
        return NULL;
    }

    if(SCI_STRLEN(file_name) > MMIFILE_FULL_PATH_MAX_LEN)
    {
        return NULL;
    }

    MMIAPICOM_StrToWstr((uint8 *)file_name, full_name);
    path_len = (uint16)MMIAPICOM_Wstrlen(full_name);

    file_handle = MMIAPIFMM_CreateFile(full_name, SFS_MODE_READ |SFS_MODE_WRITE |SFS_MODE_CREATE_ALWAYS, PNULL, PNULL);

    return file_handle;
}



/*==========================================================
 * Function     : mail_CloseFile
 * Author	    : sunguochen      
 * Description : 关闭文件句柄
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_CloseFile(MMIFILE_HANDLE file_handle)
{
    if(NULL != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }
}



/*==========================================================
 * Function     : mail_SaveFile
 * Author	    : sunguochen  
 * Description : save buffer content in file system
 * Parameter	: type          parameter       Description
                      MMIFILE_HANDLE file_handle, 要存储到的文件句柄
                      char* buffer,      要保存的buffer
                      uint32 buffer_len buffer长度
 * Return	    : TRUE ;success/FALSE:fail
 * Effect	    : NO
 * History	:created  5/12/2010 
 ==========================================================*/
int mail_SaveFile(MMIFILE_HANDLE file_handle, int pos, char* buffer, uint32 buffer_len)
{

    uint32 bytes_written = 0;
    /*check parameter*/
    if (NULL == buffer || NULL == file_handle)
    {
        return 0;
    }

    MMIAPIFMM_SetFilePointer(file_handle, pos, SFS_SEEK_BEGIN);

    MMIAPIFMM_WriteFile(file_handle, buffer, buffer_len, &bytes_written, NULL);

    return (int)bytes_written;
}

/*==========================================================
 * Function     : mail_ReadFile
 * Author	    : sunguochen  
 * Description :  read content from file system
 * Parameter	:  type          parameter       Description
                      MMIFILE_HANDLE file_handle, 要存储到的文件句柄
                      char* buffer,      要保存的buffer
                      uint32 buffer_len buffer长度
 * Return	    : TRUE ;success/FALSE:fail
 * Effect	    : if file not exit, think is ok,and set buffer to zero
 * History	:created  5/12/2010  
 ==========================================================*/
int mail_ReadFile(MMIFILE_HANDLE file_handle, int pos, char* buffer, uint32 buffer_len)
{
    uint32 bytes_read = 0;
    MMIFILE_ERROR_E result = 0;

    /*check parameter*/
    if((NULL == file_handle) || (NULL == buffer) )
    {
        return 0;
    }

    MMIAPIFMM_SetFilePointer(file_handle, pos, SFS_SEEK_BEGIN);

    result = MMIAPIFMM_ReadFile(file_handle, buffer, buffer_len, &bytes_read, NULL);

    if(result != SFS_NO_ERROR)
    {
        return 0;
    }

    return bytes_read;
}

/*==========================================================
 * Function     : mail_RemoveFile
 * Author	    : sunguochen  
 * Description : remove current file
 * Parameter	: type          parameter       Description
                      char* file_name, 存储的文件名
 * Return	    : TRUE ;success/FALSE:fail
 * Effect	    : if file not exit, think is ok,and set buffer to zero
 * History	:created  5/12/2010  
 ==========================================================*/
PUBLIC BOOLEAN mail_RemoveCfgFile(char* file_name)
{
    wchar full_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 path_len = 0;
    MMIFILE_ERROR_E error = SFS_NO_ERROR;

    /*check parameter*/
    if(NULL == file_name)
    {
        return FALSE;
    }

    if(SCI_STRLEN(file_name) > MMIFILE_FULL_PATH_MAX_LEN)
    {
        return FALSE;
    }

    MMIAPICOM_StrToWstr((uint8 *)file_name, full_name);
    path_len = (uint16)MMIAPICOM_Wstrlen(full_name);

    /*file not exit*/
    if(!MMIAPIFMM_IsFileExist(full_name,path_len))
    {
        return TRUE;
    }

    error= MMIAPIFMM_DeleteFile(full_name, NULL);

    if(SFS_ERROR_NONE != error)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*==========================================================
 * Function     : mail_GetNextAccountId
 * Author	    : sunguochen      
 * Description : 获得下一个未用的账户id
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetNextAccountId(mail_setting_info* mail_account_settings)
{
    int i = 0;
    int j = 0;    
    mail_account_info* account_info = NULL;

    if(0 == mail_account_settings->account_num || NULL == mail_account_settings->mail_setting_list)
    {
        //不存在账户，则为第一个
        return MAIL_ACCOUNT_ID_START;
        
    }
    else
    {
        for( i = MAIL_ACCOUNT_ID_START; ; i++)
        {
            for( j = 0; j < mail_account_settings->account_num; j++)
            {
                account_info = &mail_account_settings->mail_setting_list[j];

                if(i == account_info->account_id)
                {
                    break;
                }
            }
            if(j >= mail_account_settings->account_num)
            {
                //列表中不存在i这个账户id
                return i;
            }            
        }
    }
}

/*==========================================================
 * Function     : mail_GetNextAccountId
 * Author	    : sunguochen      
 * Description : 获得相同的账户id 地址
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
mail_account_info* mail_GetEqualAccountId(MMIMAIL_SETTING_INFO_T* account_info, mail_setting_info* mail_account_settings)
{
    int i = 0;
    mail_account_info* account_tmp = NULL;

    if(0 == mail_account_settings->account_num || NULL == mail_account_settings->mail_setting_list)
    {
        //不存在账户，则为第一个
        return NULL;
    }
    else
    {
        for( i = 0; i < mail_account_settings->account_num; i++)
        {
            account_tmp = &mail_account_settings->mail_setting_list[i];

            if(0 == strcmp((char *)account_info->user_name, (char *)account_tmp->mail_account_settings.user_name))
            {
                return account_tmp;
            }
        }
    }

    return NULL;
}

/*==========================================================
 * Function     : mail_WriteMailCfgfile
 * Author	    : sunguochen      
 * Description : 将全部设置信息写入文件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_WriteMailCfgfile(mail_setting_info* mail_cfg)
{
    MMIFILE_HANDLE file_handle = NULL;
    int  len = 0;
    int  write_len = 0;
    int write_pos = 0;

    file_handle = mail_StartNewFile((char *)MAIL_SETTINGS_FILE);

    if(NULL == file_handle)
    {
        //无设置信息-直接退出
        //COMM_TRACE:"Mail:save settings error!!!!!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_352_112_2_18_2_47_49_130,(uint8*)"");
        mail_settingFinal();
        return;
    }

    write_len = sizeof(mail_setting_info) - MAIL_ACCOUNT_PTR_LEN;
    len = mail_SaveFile(file_handle, 0, (char *)mail_cfg, write_len);
    write_pos = write_len;

    if(len != write_len)
    {
        //COMM_ASSERT(0);
        mail_CloseFile(file_handle);
        //COMM_TRACE:"Mail:write settings error!!!!!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_365_112_2_18_2_47_49_131,(uint8*)"");
        mail_settingFinal();
        return;
    }

    if(mail_cfg->account_num)
    {
        write_len = mail_cfg->account_num *((int)sizeof(mail_account_info));
        len = mail_SaveFile(file_handle, write_pos, (char *)(mail_cfg->mail_setting_list), write_len);
        if(len != write_len)
        {
            //COMM_TRACE:"Mail:write settings error len = %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_376_112_2_18_2_47_49_132,(uint8*)"d", len);
            mail_settingFinal();
        }        
    }
    else
    {
        //COMM_TRACE:"Mail:There no account settings to save"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_382_112_2_18_2_47_49_133,(uint8*)"");
    }  
    mail_CloseFile(file_handle);
}

/*==========================================================
 * Function     : mail_WriteMailPartCfgfile
 * Author	    : sunguochen      
 * Description : 将设置信息写入文件
                      而不写账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_WriteMailPartCfgfile(mail_setting_info* mail_cfg)
{

    MMIFILE_HANDLE file_handle = NULL;
    int len = 0;
    int write_len = 0;
    int write_pos = 0;

    file_handle = mail_StartOpenFile((char *)MAIL_SETTINGS_FILE, TRUE);

    if(NULL == file_handle)
    {
        //无设置信息-直接退出
        //COMM_TRACE:"Mail:save settings error!!!!!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_409_112_2_18_2_47_49_134,(uint8*)"");
        mail_settingFinal();
        return;
    }

    write_len = sizeof(mail_setting_info) - MAIL_ACCOUNT_PTR_LEN;
    len = mail_SaveFile(file_handle, 0, (char *)mail_cfg, write_len);
    write_pos = write_len;

    if(len != write_len)
    {
        //COMM_TRACE:"Mail:mail_WriteMailPartCfgfile write settings error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_420_112_2_18_2_47_49_135,(uint8*)"");
        mail_settingFinal();
    }

    mail_CloseFile(file_handle);
}

/*==========================================================
 * Function     : mail_accountIsValid
 * Author	    : sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/14/2010 
 ==========================================================*/
comm_boolean mail_accountIsValid(int accountid)
{

    int i = 0;
    mail_account_info* account_tmp = NULL;
    
    if(accountid == MAIL_INVALD_ACCOUNT_ID)
    {
        return FALSE;
    }

    for(i = 0; i < s_mail_settings.account_num; i++)
    {
        account_tmp = &s_mail_settings.mail_setting_list[i];

        if(account_tmp->account_id == accountid)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*==========================================================
 * Function     : mail_SetSilenceDuration
 * Author	    : sunguochen      
 * Description :计算两个时间之间的差值
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_minuteBetween2Time(SCI_TIME_T* start_time, SCI_TIME_T* end_time)
{
    int minute_value = 0;

    if(!start_time || !end_time)
    {
        //无法比较
        return 0;
    }

    if(end_time->hour > start_time->hour)
    {
        minute_value += (end_time->hour - start_time->hour) * 60;
        minute_value += end_time->min;
        minute_value -= start_time->min;        
    }
    else if(end_time->hour == start_time->hour)
    {
        if(end_time->min >= start_time->min)
        {
            minute_value += end_time->min - start_time->min;
        }
        else
        {
            minute_value += 24*60;
            minute_value += end_time->min;
            minute_value -= start_time->min;            
        }
    }
    else
    {
        minute_value += (24 + end_time->hour - start_time->hour) * 60;
        minute_value += end_time->min;
        minute_value -= start_time->min;   
    }

    return minute_value;
}

//public function list
/*==========================================================
 * Function     : mail_settingParaInit
 * Author	    : sunguochen      
 * Description  : 参数初始化
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  9/2/2010 
 ==========================================================*/
void mail_settingParaInit(void)
{
    memset(&s_mail_settings, 0, sizeof(mail_setting_info));

    s_mail_settings.reply_with_original_cont = TRUE;
}

/*==========================================================
 * Function     : mail_settingParaReinit
 * Author	    : sunguochen      
 * Description  : 参数恢复初始化
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  9/2/2010 
 ==========================================================*/
void mail_settingParaReinit(void)
{
    //comm_boolean     recved_settings = FALSE;
    mail_account_info* tmp_account_info = NULL;

    //recved_settings = s_mail_settings.recved_settings;
    tmp_account_info = s_mail_settings.tmp_account_info;

    memset(&s_mail_settings, 0, sizeof(mail_setting_info));

    s_mail_settings.reply_with_original_cont = TRUE;

    //s_mail_settings.recved_settings = recved_settings;
    s_mail_settings.tmp_account_info = tmp_account_info;
       
}

/*==========================================================
 * Function     : mail_settingInit
 * Author	    : sunguochen      
 * Description : 设置初始化
                 将设置信息从配置文件中读出
                 修改如果配置文件读错，直接删除
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_settingInit(void)
{
    MMIFILE_HANDLE file_handle = NULL;
    int len = 0;
    int read_len = 0;
    int read_pos = 0;    

    file_handle = mail_StartOpenFile((char *)MAIL_SETTINGS_FILE, FALSE);

    if(NULL == file_handle)
    {
        //无设置信息-直接退出
        //COMM_TRACE:"Mail:There no mail settings"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_569_112_2_18_2_47_49_136,(uint8*)"");
        return;
    }

    read_len = sizeof(mail_setting_info) - MAIL_ACCOUNT_PTR_LEN;

    len = mail_ReadFile(file_handle, 0, (char *)(&s_mail_settings), read_len);

    s_mail_settings.recved_settings = FALSE;

    if(len != read_len)
    {
        //配置文件读出错，删除配置文件
        //COMM_TRACE:"Mail:init file read error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_582_112_2_18_2_47_49_137,(uint8*)"");
        mail_settingParaInit();
        mail_CloseFile(file_handle);
        mail_RemoveCfgFile((char *)MAIL_SETTINGS_FILE);
        return;        
    }

    read_pos = read_len;

    //COMM_TRACE:"Mail:mail settings num:%d-default:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_591_112_2_18_2_47_49_138,(uint8*)"dd", s_mail_settings.account_num, s_mail_settings.default_account);

    if((s_mail_settings.account_num > 0) && (s_mail_settings.account_num <= MAIL_MAX_ACCOUNT_NUM))
    {
        read_len = s_mail_settings.account_num *((int)sizeof(mail_account_info));
        s_mail_settings.mail_setting_list = comm_alloc(read_len);

        if(NULL == s_mail_settings.mail_setting_list)
        {
            //COMM_TRACE:"Mail:mail_settingInit:%d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_600_112_2_18_2_47_49_139,(uint8*)"dd", s_mail_settings.account_num, read_len);
            mail_CloseFile(file_handle); 
            comm_free(s_mail_settings.mail_setting_list);
            return;
        }

        len = mail_ReadFile(file_handle, read_pos, (char *)s_mail_settings.mail_setting_list, read_len);

        if(len < read_len)
        {

            //COMM_TRACE:"Mail:file account read:fail %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_611_112_2_18_2_47_49_140,(uint8*)"dd", s_mail_settings.account_num, read_len);
            comm_free(s_mail_settings.mail_setting_list);
            mail_settingParaInit();
            mail_RemoveCfgFile((char *)MAIL_SETTINGS_FILE);
            mail_CloseFile(file_handle); 
            return;

        }

        //COMM_TRACE:"Mail:mail settings id:%d-user:%s-pass:%s-apn:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_624_112_2_18_2_47_49_141,(uint8*)"dsss", s_mail_settings.mail_setting_list->account_id, s_mail_settings.mail_setting_list->mail_account_settings.user_name,s_mail_settings.mail_setting_list->mail_account_settings.user_password,s_mail_settings.mail_setting_list->mail_account_settings.apn);
    }
    else
    {
        //COMM_TRACE:"Mail:There no account settings"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_628_112_2_18_2_47_49_142,(uint8*)"");
        s_mail_settings.mail_setting_list = NULL;
        mail_settingParaInit();
        mail_RemoveCfgFile((char *)MAIL_SETTINGS_FILE);        
    }

    mail_CloseFile(file_handle);    
}

/*==========================================================
 * Function     : mail_settingInit
 * Author	    : sunguochen      
 * Description : 设置终止
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/12/2010 
 ==========================================================*/
void mail_settingFinal(void)
{
    //COMM_TRACE:"Mail:mail_settingFinal"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_647_112_2_18_2_47_49_143,(uint8*)"");
    if(s_mail_settings.account_num && s_mail_settings.mail_setting_list)
    {
        comm_free(s_mail_settings.mail_setting_list);
    }
    mail_settingParaReinit();
}

/*==========================================================
 * Function     : mail_settingRest
 * Author	    : sunguochen      
 * Description : 设置复位
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_settingRest(void)
{
    mail_RemoveCfgFile((char *) MAIL_SETTINGS_FILE);
    mail_settingFinal( );
    mail_WriteAllSettings();
}

/*==========================================================
 * Function     : mail_GetDefaultAccount
 * Author	    : sunguochen      
 * Description : 获取默认账户id
 * Parameter	: type          parameter       Description
 * Return	    : Void
                      如果是0-说明无默认账户-
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetDefaultAccount(void)
{
    if(s_mail_settings.account_num > 0 && NULL != s_mail_settings.default_account)
    {
        //COMM_TRACE:"MMIMAIL==> mail_GetDefaultAccount: default account = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_683_112_2_18_2_47_49_144,(uint8*)"d", s_mail_settings.default_account);
        return s_mail_settings.default_account;
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> mail_GetDefaultAccount: can't get default account!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_688_112_2_18_2_47_49_145,(uint8*)"");
        return MAIL_INVALD_ACCOUNT_ID;
    }
}

/*==========================================================
 * Function     : mail_GetAccountInfoById
 * Author	    : sunguochen      
 * Description : 通过账户id获得账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
mail_account_info* mail_GetAccountInfoById(int accountId)
{
    int i = 0;
    mail_account_info* account_tmp = NULL;
    
    if(accountId == MAIL_INVALD_ACCOUNT_ID || s_mail_settings.account_num == 0 || s_mail_settings.mail_setting_list == NULL)
    {
        return NULL;
    }

    for(i = 0; i < s_mail_settings.account_num; i++)
    {
        account_tmp = &s_mail_settings.mail_setting_list[i];

        if(account_tmp->account_id == accountId)
        {
            return account_tmp;
        }
    }

    return NULL;
}

/*==========================================================
 * Function     : mail_GetAccountInfoById
 * Author	    : sunguochen      
 * Description : 通过账户名称获得id
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAccountIdByUserName(char * user_name)
{
    int i = 0;
    mail_account_info* account_tmp = NULL;

    if(NULL == user_name)
    {
        return MAIL_INVALD_ACCOUNT_ID;
    }

    for(i = 0; i < s_mail_settings.account_num; i++)
    {
        account_tmp = &s_mail_settings.mail_setting_list[i];

        if(!strcmp((char *)account_tmp->mail_account_settings.user_name, user_name))
        {
            return account_tmp->account_id;
        }
    }

    return MAIL_INVALD_ACCOUNT_ID;    
}

/*==========================================================
 * Function     : mail_GetDefaultAccount
 * Author	    : sunguochen      
 * Description : 获得账户个数
 * Parameter	: type          parameter       Description
 * Return	    : Void
                      如果是0-说明无账户-
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAccountNum(void)
{
    return s_mail_settings.account_num;
}

/*==========================================================
 * Function     : mail_ExistLocalAccount
 * Author	    : sunguochen      
 * Description : 是否存在本地账户
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_ExistLocalAccount(void)
{
#ifdef WIN32
    return TRUE;
#else    
    if(s_mail_settings.account_num)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif    
}

/*==========================================================
 * Function     : mail_GetDefAccount
 * Author	    : sunguochen      
 * Description : 获得默认账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetDefAccount( MMIMAIL_ACCOUNT_INFO_T *account_info)
{
    int account_id = 0;
    mail_account_info* account_temp = NULL;

    if(PNULL == account_info)
    {
        //SCI_TRACE_LOW:"mail_GetDefAccount account_info=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_808_112_2_18_2_47_50_146,(uint8*)"d", account_info);
        return FALSE;
    }

    account_id = mail_GetDefaultAccount();

    account_temp = mail_GetAccountInfoById(account_id);

    SCI_MEMSET(account_info, 0, sizeof(MMIMAIL_ACCOUNT_INFO_T));

    if(!account_temp)
    {
        return FALSE;
    }

    account_info->accountid = account_id;
    memcpy(account_info->name, account_temp->account_name, MMIMAIL_MAX_ACC_NAME_LEN);
    comm_str2wstr(account_info->addr, (char *)account_temp->mail_account_settings.user_name);
    comm_str2wstr(account_info->password, (char *)account_temp->mail_account_settings.user_password);    

    return TRUE;
}

/*==========================================================
 * Function     : mail_GetDefAccount
 * Author	    : sunguochen      
 * Description : 根据索引获得账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetAccountByIndex(int index, MMIMAIL_ACCOUNT_INFO_T *account_info)
{
    mail_account_info* account_temp = NULL;

    if(PNULL == account_info)
    {
        //SCI_TRACE_LOW:"mail_GetAccountByIndex account_info=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_845_112_2_18_2_47_50_147,(uint8*)"d", account_info);
        return FALSE;
    }

    SCI_MEMSET(account_info, 0, sizeof(MMIMAIL_ACCOUNT_INFO_T));

    if(index >= s_mail_settings.account_num)
    {
        return FALSE;
    }

    account_temp = &s_mail_settings.mail_setting_list[index];   

    account_info->accountid = account_temp->account_id;
    memcpy(account_info->name, account_temp->account_name, MMIMAIL_MAX_ACC_NAME_LEN);
    comm_str2wstr(account_info->addr, (char *)account_temp->mail_account_settings.user_name);
    comm_str2wstr(account_info->password, (char *)account_temp->mail_account_settings.user_password);    

    return TRUE;
}

/*==========================================================
 * Function     : mail_SetDefAccount
 * Author	    : sunguochen      
 * Description :设置默认账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_SetDefAccount(int accountid)
{
    if(mail_accountIsValid(accountid) == FALSE)
    {
        //COMM_TRACE:"Mail:mail_SetDefAccount invalid account %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_878_112_2_18_2_47_50_148,(uint8*)"d", accountid);
        return FALSE;
    }

    s_mail_settings.default_account = accountid;
    mail_WriteMailPartCfgfile(&s_mail_settings);   
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    //切换账户重新更新信息列表
    mail_switchAccount();    
#endif
    return TRUE;
}

/*==========================================================
 * Function     : mail_SetDefAccountByIndex
 * Author	    : sunguochen      
 * Description :设置默认账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_SetDefAccountByIndex(int index)
{
    mail_account_info* account_temp = NULL;

    if(index >= s_mail_settings.account_num)
    {
        return FALSE;
    }

    account_temp = &s_mail_settings.mail_setting_list[index];   

    s_mail_settings.default_account = account_temp->account_id;

    mail_WriteMailPartCfgfile(&s_mail_settings);   

    return TRUE;
}

/*==========================================================
 * Function     : mail_DeleteAccount
 * Author	    : sunguochen      
 * Description : 删除账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_DeleteAccount(int accountid)
{
    mail_account_info* account_temp = NULL;
    int index = 0;

    for(index = 0; index < s_mail_settings.account_num; index++)
    {
        account_temp = &s_mail_settings.mail_setting_list[index];

        if(account_temp->account_id == accountid)
        {
            /*find delete account in list*/
            break;
        }
    }

    if(index >= s_mail_settings.account_num)
    {
        //COMM_TRACE:"Mail:mail_DeleteAccount account do not exit %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_943_112_2_18_2_47_50_149,(uint8*)"d", accountid);
        return FALSE;
    }

    account_temp = &s_mail_settings.mail_setting_list[index];

    /*not to the last of list and last of program*/
    if(index >= (s_mail_settings.account_num - 1))
    {
        /*clear buffer*/        
        SCI_MEMSET(account_temp, 0x00, sizeof(mail_account_info));        
    }
    else
    {
        /*copy i+1 to i, to clear empty position*/
        SCI_MEMCPY(&(s_mail_settings.mail_setting_list[index]), 
                             &(s_mail_settings.mail_setting_list[index + 1]),
                             (s_mail_settings.account_num - index - 1)*((int)sizeof(mail_account_info)));

        /*clear last buffer*/        
        SCI_MEMSET(&(s_mail_settings.mail_setting_list[s_mail_settings.account_num - 1]), 0x00, sizeof(mail_account_info));  
    }

    if(s_mail_settings.account_num)
    {
        s_mail_settings.account_num --;  
        //set default account
        account_temp = &s_mail_settings.mail_setting_list[index];
        s_mail_settings.default_account = account_temp->account_id;        
    }

    if(s_mail_settings.account_num == 0)
    {
        //no account delete
        memset(s_mail_settings.card_imsi, 0, (MAIL_IMSI_SAVE_LEN + 1));
    }

    mail_WriteMailCfgfile(&s_mail_settings);
        
    return TRUE;
}

/*==========================================================
 * Function     : mail_DeleteAccount
 * Author	    : sunguochen      
 * Description : 删除账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_DeleteAccountByIndex(int index)
{
    mail_account_info* account_temp = NULL;

    if(index >= s_mail_settings.account_num)
    {
        //COMM_TRACE:"Mail:mail_DeleteAccount index do not exit %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_999_112_2_18_2_47_50_150,(uint8*)"d", index);
        return FALSE;
    }

    account_temp = &s_mail_settings.mail_setting_list[index];

    /*not to the last of list and last of program*/
    if(index >= (s_mail_settings.account_num - 1))
    {
        /*clear buffer*/        
        SCI_MEMSET(account_temp, 0x00, sizeof(mail_account_info));        
    }
    else
    {
        /*copy i+1 to i, to clear empty position*/
        SCI_MEMCPY(&(s_mail_settings.mail_setting_list[index]), 
                             &(s_mail_settings.mail_setting_list[index + 1]),
                             (s_mail_settings.account_num - index - 1)*((int)sizeof(mail_account_info)));

        /*clear last buffer*/        
        SCI_MEMSET(&(s_mail_settings.mail_setting_list[s_mail_settings.account_num - 1]), 0x00, sizeof(mail_account_info));  
    }

    if(s_mail_settings.account_num)
    {
        s_mail_settings.account_num --;  
        //set default account
        account_temp = &s_mail_settings.mail_setting_list[index];
        s_mail_settings.default_account = account_temp->account_id;
    }

    if(s_mail_settings.account_num == 0)
    {
        //no account delete
        memset(s_mail_settings.card_imsi, 0, (MAIL_IMSI_SAVE_LEN + 1));
    }

    mail_WriteMailCfgfile(&s_mail_settings);

    return TRUE;
}

/*==========================================================
 * Function     : mail_SetAccountInfo
 * Author	    : sunguochen      
 * Description : 设置账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_SetAccountInfo( MMIMAIL_ACCOUNT_INFO_T *account_info)
{

    mail_account_info* account_temp = NULL;

    if(PNULL == account_info)
    {
        //SCI_TRACE_LOW:"mail_SetAccountInfo account_info=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1056_112_2_18_2_47_50_151,(uint8*)"d", account_info);
        return FALSE;
    }

    account_temp = mail_GetAccountInfoById(account_info->accountid);

//    SCI_MEMSET(account_info, 0, sizeof(MMIMAIL_ACCOUNT_INFO_T));

    if(!account_temp)
    {
        return FALSE;
    }

    memset(account_temp->account_name, 0, (MMIMAIL_MAX_ACC_NAME_LEN + 1)*sizeof(wchar));
    memcpy(account_temp->account_name, account_info->name, MMIMAIL_MAX_ACC_NAME_LEN);

    //企业版可以保存密码
    //个人版不允许保存密码
    if(mail_AccountIsGroupAccount(account_temp->account_id))
    {
        int  password_len = 0;
        char password[MAIL_PASSWORD_LEN + 1] = {0};

        password_len = MMIAPICOM_Wstrlen(account_info->password);

        if(password_len > MAIL_PASSWORD_LEN)
        {
            account_info->password[MAIL_PASSWORD_LEN] = 0;
        }

        MMIAPICOM_WstrToStr(account_info->password, (uint8 *)password);
        
        memset(account_temp->mail_account_settings.user_password, 0, (MAIL_PASSWORD_LEN + 1));
        strcpy((char *)account_temp->mail_account_settings.user_password, password);
        //COMM_TRACE:"Mail:mail_SetAccountInfo password %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1090_112_2_18_2_47_50_152,(uint8*)"s", password);

    }

    mail_WriteMailCfgfile(&s_mail_settings);

    return TRUE;
}

/*==========================================================
 * Function     : mail_SaveAccountName
 * Author	    : sunguochen      
 * Description : 保存密码
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/20/2010 
 ==========================================================*/
comm_boolean mail_SaveAccountName(int accountid, char *password)
{
    mail_account_info* account_temp = NULL;

    account_temp = mail_GetAccountInfoById(accountid);

    if(!account_temp)
    {
        return FALSE;
    }
    //企业版可以保存密码
    //个人版不允许保存密码
    if(mail_AccountIsGroupAccount(account_temp->account_id))
    {        
        memset(account_temp->mail_account_settings.user_password, 0, (MAIL_PASSWORD_LEN + 1));
        strcpy((char *)account_temp->mail_account_settings.user_password, password);
        //COMM_TRACE:"Mail:mail_SetAccountInfo password %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1123_112_2_18_2_47_50_153,(uint8*)"s", password);

    }

    mail_WriteMailCfgfile(&s_mail_settings);

    return TRUE;
}

/*==========================================================
 * Function     : mail_WriteAccountSettings
 * Author	    : sunguochen      
 * Description : 保存接收到的账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_WriteAccountSettings(MMIMAIL_SETTING_INFO_T* account_info)
{
    mail_account_info  *accont_tmp = NULL;
    char imsi_val[(MN_MAX_IMSI_ARR_LEN << 1) + 1] = { 0 };
    MN_IMSI_T imsi = {0};
    int cur_imsi_len = 0;

    imsi = MNSIM_GetImsiEx(MN_DUAL_SYS_1);
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, imsi_val);

    cur_imsi_len = strlen(s_mail_settings.card_imsi);

    if(cur_imsi_len == 0)
    {
        //未保存imsi，可直接保存
        //COMM_TRACE:"Mail:imsi new %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1155_112_2_18_2_47_50_154,(uint8*)"s", imsi_val);
        memset(s_mail_settings.card_imsi, 0, (MAIL_IMSI_SAVE_LEN + 1));
        strcpy(s_mail_settings.card_imsi, imsi_val);
    }
    else
    {
        //COMM_TRACE:"Mail:imsi old %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1161_112_2_18_2_47_50_155,(uint8*)"s", s_mail_settings.card_imsi);
        if(strcmp(imsi_val, s_mail_settings.card_imsi))
        {
            //imsi不一致，覆盖
            //COMM_TRACE:"Mail:imsi recover %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1165_112_2_18_2_47_50_156,(uint8*)"s", imsi_val);
            memset(s_mail_settings.card_imsi, 0, (MAIL_IMSI_SAVE_LEN + 1));
            strcpy(s_mail_settings.card_imsi, imsi_val);            
        }
    }

    if(0 == s_mail_settings.account_num)
    {
        //不存在账户，直接加入即可
        s_mail_settings.mail_setting_list = comm_alloc(sizeof(mail_account_info));

        if(!s_mail_settings.mail_setting_list)
        {
            //COMM_ASSERT(0);
            return;
        }

        s_mail_settings.account_num ++;
        s_mail_settings.default_account = MAIL_ACCOUNT_ID_START;
        s_mail_settings.mail_setting_list->account_id = MAIL_ACCOUNT_ID_START;
        memcpy(&s_mail_settings.mail_setting_list->mail_account_settings, account_info, sizeof(MMIMAIL_SETTING_INFO_T));
    }
    else
    {
        accont_tmp = mail_GetEqualAccountId(account_info, &s_mail_settings);

        if(NULL == accont_tmp)
        {
            //没有相同账户-新建
            s_mail_settings.mail_setting_list = mail_realloc(s_mail_settings.mail_setting_list, 
                                                              s_mail_settings.account_num*((int)sizeof(mail_account_info)),
                                                              (s_mail_settings.account_num + 1)*((int)sizeof(mail_account_info)));
    
            if(!s_mail_settings.mail_setting_list)
            {
                //COMM_ASSERT(0);
                return;
            }
            s_mail_settings.account_num ++;
            accont_tmp = &s_mail_settings.mail_setting_list[s_mail_settings.account_num -1];
            accont_tmp->account_id = mail_GetNextAccountId(&s_mail_settings);            
        }

        memcpy(&accont_tmp->mail_account_settings, account_info, sizeof(MMIMAIL_SETTING_INFO_T));     
    }
    //写文件
    mail_WriteMailCfgfile(&s_mail_settings);
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description : 设置-回复时是否带原邮件正文
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetReplyWithOriginalCont(comm_boolean result)
{
    s_mail_settings.reply_with_original_cont = result;
    
    mail_WriteMailPartCfgfile(&s_mail_settings);   
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description :获取设置-回复时是否带原邮件正文
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetReplyWithOriginalCont(void)
{
    return s_mail_settings.reply_with_original_cont;
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description : 设置-是否添加签名信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAddSignature(comm_boolean result)
{
    s_mail_settings.add_signature = result;

    mail_WriteMailPartCfgfile(&s_mail_settings);   
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description : 获取设置-是否添加签名信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetAddSignature(void)
{
    return s_mail_settings.add_signature ;
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description : 设置-签名信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetSignature_text(wchar* Signature_text)
{
    if(!Signature_text)
    {
        return;
    }

    memset(s_mail_settings.signature_text, 0, (MAIL_SIGNATURE_LEN + 1)*sizeof(wchar));
    memcpy(s_mail_settings.signature_text, Signature_text, MAIL_SIGNATURE_LEN*sizeof(wchar));
    mail_WriteMailPartCfgfile(&s_mail_settings);
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description : 获取设置-签名信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
wchar* mail_GetSignature_text(void)
{
    return s_mail_settings.signature_text;
}

/*==========================================================
 * Function     : mail_SetSilenceDuration
 * Author	    : sunguochen      
 * Description :设置-静音时段
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetSilenceDuration(SCI_TIME_T* silence_start_time, SCI_TIME_T* silence_end_time)
{
    if(!silence_start_time || !silence_end_time)
    {
        return;
    }

    s_mail_settings.silence_start_time.hour = silence_start_time->hour;
    s_mail_settings.silence_start_time.min = silence_start_time->min;
    s_mail_settings.silence_start_time.sec = silence_start_time->sec;    

    s_mail_settings.silence_end_time.hour = silence_end_time->hour;
    s_mail_settings.silence_end_time.min = silence_end_time->min;
    s_mail_settings.silence_end_time.sec = silence_end_time->sec; 

    mail_WriteMailPartCfgfile(&s_mail_settings);
}

/*==========================================================
 * Function     : mail_SetSilenceDuration
 * Author	    : sunguochen      
 * Description :获取设置-静音时段
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_GetSilenceDuration(SCI_TIME_T* silence_start_time, SCI_TIME_T* silence_end_time)
{
    if(!silence_start_time || !silence_end_time)
    {
        return;
    }

    silence_start_time->hour = s_mail_settings.silence_start_time.hour ;
    silence_start_time->min = s_mail_settings.silence_start_time.min ;
    silence_start_time->sec = s_mail_settings.silence_start_time.sec ;    

    silence_end_time->hour = s_mail_settings.silence_end_time.hour ;
    silence_end_time->min = s_mail_settings.silence_end_time.min ;
    silence_end_time->sec = s_mail_settings.silence_end_time.sec ; 
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description :设置-是否自动下载附件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAttachAutoDownload(comm_boolean result)
{
    s_mail_settings.attach_auto_download = result;
    
    mail_WriteMailPartCfgfile(&s_mail_settings);   
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description : 获取设置-是否自动下载附件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
comm_boolean mail_GetAttachAutoDownload(void)
{
    return s_mail_settings.attach_auto_download;   
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description :设置-是否下载附件大小
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAttachDownloadLimit(int limit_size)
{
    s_mail_settings.attach_limit_size = limit_size;

    mail_WriteMailPartCfgfile(&s_mail_settings);   
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description : 获取设置-是否下载附件大小
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAttachDownloadLimit(void)
{
#if 0
    int attch_size = 0;

    attch_size = s_mail_settings.attach_limit_size;

    if(0 == attch_size)
    {
        attch_size = 100*1024;
    }

    return attch_size;
#else
    return MMIMAIL_SIZE_LIMIT;
#endif
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description : 附件格式转换
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_SetAttachDownMode(int result)
{
    s_mail_settings.attach_trans_mode = result;

    mail_WriteMailPartCfgfile(&s_mail_settings);
}

/*==========================================================
 * Function     : mail_SetReplyWithOriginalCont
 * Author	    : sunguochen      
 * Description : 附件格式转换
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
int mail_GetAttachDownMode(void)
{
    return s_mail_settings.attach_trans_mode;   
}

/*==========================================================
 * Function     : mail_IsSilenceDuration
 * Author	    : sunguochen      
 * Description : 判断现在是否是静音时段
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/14/2010 
 ==========================================================*/
comm_boolean mail_IsSilenceDuration(void)
{
    int minute_silence = 0;
    int minute_from_start = 0;    
    SCI_TIME_T  silence_start_time = {0};
    SCI_TIME_T  silence_end_time = {0};
    SCI_TIME_T cur_time = {0};

    TM_GetSysTime(&cur_time);

    mail_GetSilenceDuration(&silence_start_time, &silence_end_time);

    minute_silence = mail_minuteBetween2Time(&silence_start_time, &silence_end_time);
    minute_from_start = mail_minuteBetween2Time(&silence_start_time, &cur_time);

    if(minute_from_start > minute_silence)
    {
        //COMM_TRACE:"Mail:now is not silence time "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1475_112_2_18_2_47_51_157,(uint8*)"");
        return FALSE;
    }
    else
    {
        //COMM_TRACE:"Mail:now is silence time "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1480_112_2_18_2_47_51_158,(uint8*)"");
        return TRUE;
    }
}

/*==========================================================
 * Function     : mail_TlsEnabled
 * Author	    : sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/13/2010 
 ==========================================================*/
comm_boolean mail_TlsEnabled(void)
{
    return FALSE;
}

/*==========================================================
 * Function     : mail_UserName
 * Author	    : sunguochen      
 * Description : 获得账户名
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/13/2010 
 ==========================================================*/
char* mail_UserName(int  accountid)
{
    mail_account_info* account_temp = NULL;    

    account_temp = mail_GetAccountInfoById(accountid);

#ifdef WIN32
    return "sunguochen";
#else
    if(account_temp)
    {
        return (char *)account_temp->mail_account_settings.user_name ;
    }
    else
    {
        return "15865327035@139.com";
    }
#endif
}

/*==========================================================
 * Function     : mail_passWord
 * Author	    : sunguochen      
 * Description : 获得账户密码
 * Parameter	: type          parameter       Description
                      uint16 accountid --账户ID
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/13/2010 
 ==========================================================*/
char* mail_passWord(int  accountid)
{
    mail_account_info* account_temp = NULL;    

    account_temp = mail_GetAccountInfoById(accountid);

#ifdef WIN32    
    return "sunguochen";
#else
    if(account_temp)
    {
        return (char *)account_temp->mail_account_settings.user_password;
    }
    else
    {
        return "1093";
    }    
#endif
}

/*==========================================================
 * Function     : mail_SenderAddr
 * Author	    : sunguochen      
 * Description : 获取邮箱发件人地址
 * Parameter	: type          parameter       Description
                      uint16 accountid --账户ID 
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/14/2010 
 ==========================================================*/
char * mail_SenderAddr(int accountid)
{
    mail_account_info* account_temp = NULL;

    account_temp = mail_GetAccountInfoById(accountid);

#ifdef WIN32
    return "sunguochen@soft.com";
#else
    if(account_temp)
    {
        return (char *)account_temp->mail_account_settings.user_name ;
    }
    else
    {
        return "15865327035@139.com";
    }
#endif
}

/*==========================================================
 * Function     : mail_PopLoginMethod
 * Author	    : sunguochen      
 * Description : 是否POP方式登录邮箱
 * Parameter	: type          parameter       Description
                      uint16 accountid --账户ID 
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/13/2010 
 ==========================================================*/
comm_boolean mail_IsPopLoginMethod(int accountid)
{
    return FALSE;
}

/*==========================================================
 * Function     : mail_protocol_type
 * Author	    : sunguochen      
 * Description : 获取邮箱协议，分为POP3 & IMAP4
 * Parameter	: type          parameter       Description
                      uint16 accountid --账户ID 
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/14/2010 
 ==========================================================*/
MAIL_PROTOCOL_E mail_protocol_type(int accountid)
{
    //目前仅支持pop3
    return MAIL_PROTOCOL_POP3;
}

/*==========================================================
 * Function     : comm_NetGetProxyAddr
 * Author	    : sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/12/2010 
 ==========================================================*/
void comm_NetProxyAddr(int accountid, comm_sockaddr* addr)
{
    //企业版如何?
    addr->family = AF_INET;
    addr->port = htons(COMM_NET_PROXY_PORT);
    addr->ip_addr = htonl((uint32)COMM_NET_PROXY_ADDR);
 //   COMM_TRACE("Mail:mail settings Proxy ip:%s-port:%d", inet_ntoa(addr->ip_addr), addr->port);
}

/*==========================================================
 * Function     : comm_NetSendServerAddr
 * Author	    : sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/12/2010 
 ==========================================================*/
void comm_NetSendServerAddr(int accountid, comm_sockaddr* addr)
{
    mail_account_info* account_temp = NULL;    

    account_temp = mail_GetAccountInfoById(accountid);

    if(account_temp)
    {
        addr->family = AF_INET;
        addr->port = account_temp->mail_account_settings.send_port;
        addr->ip_addr = account_temp->mail_account_settings.send_ip_add;
//        COMM_TRACE("Mail:mail settings send ip:%s-port:%d", inet_ntoa(addr->ip_addr), addr->port);
    }
    else
    {
        addr->family = AF_INET;
        addr->port = htons(COMM_NET_SMTP_SERVER_PORT);
        addr->ip_addr = htonl(COMM_NET_SMTP_SERVER_ADDR);
//        COMM_TRACE("Mail:mail settings send SMTP ip:%s-port:%d", inet_ntoa(addr->ip_addr), addr->port);
    }
}

/*==========================================================
 * Function     : comm_NetRetrServerAddr
 * Author	    : sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/12/2010 
 ==========================================================*/
void comm_NetRetrServerAddr(int accountid, comm_sockaddr* addr)
{
    mail_account_info* account_temp = NULL;    
    account_temp = mail_GetAccountInfoById(accountid);

    if(account_temp)
    {
        addr->family = AF_INET;
        addr->port = account_temp->mail_account_settings.receive_port;
        addr->ip_addr = account_temp->mail_account_settings.receive_ip_add;
//        COMM_TRACE("Mail:mail settings recv ip:%s-port:%d", inet_ntoa(addr->ip_addr), addr->port);
    }
    else
    {
        addr->family = AF_INET;
        addr->port = htons(COMM_NET_POP3_SERVER_PORT);
        addr->ip_addr = htonl(COMM_NET_POP3_SERVER_ADDR);
//        COMM_TRACE("Mail:mail settings recv POP3 ip:%s-port:%d", inet_ntoa(addr->ip_addr), addr->port);
    }
}

/*==========================================================
 * Function     : mail_NetUsedProxy
 * Author	    : sunguochen      
 * Description : 是否使用代理
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/16/2010 
 ==========================================================*/
comm_boolean mail_NetUsedProxy(int accountid)
{
#ifdef WIN32
    return FALSE;
#else
if(mail_AccountIsGroupAccount(accountid))
{
    return FALSE;    
}
else
{
    return TRUE;
}
#endif
}

/*==========================================================
 * Function     : comm_NetGetSetting
 * Author	    : sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/15/2010 
 ==========================================================*/
void comm_NetGetSetting(int  accountid, MMICONNECTION_LINKSETTING_DETAIL_T* settings)
{
    mail_account_info* account_temp = NULL;

    account_temp = mail_GetAccountInfoById(accountid); 

    if(!settings || !account_temp)
    {
        return;
    }    

    strcpy((char *)settings->apn, (char *)account_temp->mail_account_settings.apn);
    settings->apn_len = strlen((char *)account_temp->mail_account_settings.apn);

    settings->username[0] = 0;
    settings->username_len = 0;

    settings->password[0] = 0;
    settings->password_len= 0;    
}

/*==========================================================
 * Function     : mail_AccountName
 * Author	    : sunguochen      
 * Description : 获得账户名称
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/28/2010 
 ==========================================================*/
char* mail_AccountName(uint16 accountid)
{
    mail_account_info* account_temp = NULL;    

    account_temp = mail_GetAccountInfoById(accountid);
    
#ifdef WIN32
    return "td09";
#else
    if(NULL != account_temp)
    {
        return (char *)account_temp->mail_account_settings.user_name ;
    }
    else
    {
        return "15865327035@139.com";
    }
#endif

}

/*==========================================================
 * Function     : mail_AccountIsSSL
 * Author	    : sunguochen      
 * Description : 账户是否采用ssl
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean mail_AccountIsSSL(int accountid)
{
    comm_boolean result  = FALSE;
    mail_account_info *account_info = NULL;  
    account_info = mail_GetAccountInfoById(accountid);
    
    if(NULL != account_info)
    {

        //COMM_TRACE:"Mail:mail_AccountIsSSL ssl = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1788_112_2_18_2_47_52_159,(uint8*)"d", account_info->mail_account_settings.ssl);
        //COMM_TRACE:"Mail:mail_AccountIsSSL is_mas_account = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1789_112_2_18_2_47_52_160,(uint8*)"d", account_info->mail_account_settings.is_mas_account);
        //COMM_TRACE:"Mail:mail_AccountIsSSL is_adc_account = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1790_112_2_18_2_47_52_161,(uint8*)"d", account_info->mail_account_settings.is_adc_account);

        if(account_info->mail_account_settings.ssl)
        {
            result = FALSE;
        }
        else
        {
            result = TRUE;        
        }
    }
    else
    {
        //COMM_TRACE:"Mail:mail_AccountIsSSL  account not exit ssl FALSE\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1803_112_2_18_2_47_52_162,(uint8*)"");
        result = FALSE;    
    }

    return result;
}

/*==========================================================
 * author		: liangshun      
 * function 	: comm_boolean mail_AccountIsGroupAccount(int accountid)
 * decr		: 判断是否是企业账户
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  4/1/2011 
 ==========================================================*/
comm_boolean mail_AccountIsGroupAccount(int accountid)
{

    comm_boolean result  = FALSE;
    mail_account_info *account_info = NULL;  
    
    account_info = mail_GetAccountInfoById(accountid);
    
    if(account_info)
    {
        //COMM_TRACE:"Mail:mail_AccountIsGroupAccount is_mas_account = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1828_112_2_18_2_47_52_163,(uint8*)"d", account_info->mail_account_settings.is_mas_account);
        //COMM_TRACE:"Mail:mail_AccountIsGroupAccount is_adc_account = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1829_112_2_18_2_47_52_164,(uint8*)"d", account_info->mail_account_settings.is_adc_account);

        result = (comm_boolean)(account_info->mail_account_settings.is_mas_account |account_info->mail_account_settings.is_adc_account);
    }
    else
    {
        //COMM_TRACE:"Mail:mail_AccountIsGroupAccount  account not exit!accountid = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1835_112_2_18_2_47_52_165,(uint8*)"d", accountid);
        result = FALSE;    
    }

    return result;

}

/*==========================================================
 * Function     : mail_isCardSwitch
 * Author	    : sunguochen      
 * Description : 卡是否更换
 * Parameter	: type          parameter       Description
 * Return	    : Void
                      TRUE:  已换
                      FALSE: 未换
 * Effect	    : NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean mail_isCardSwitch(void)
{
    char imsi_val[(MN_MAX_IMSI_ARR_LEN << 1) + 1] = { 0 };
    MN_IMSI_T imsi = {0};
    int cur_imsi_len = 0;

    cur_imsi_len = strlen(s_mail_settings.card_imsi);

    if(cur_imsi_len == 0)
    {
        //未保存imsi
        //COMM_TRACE:"Mail:imsi no imsi\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1864_112_2_18_2_47_52_166,(uint8*)"");
        return FALSE;
    }

    imsi = MNSIM_GetImsiEx(MN_DUAL_SYS_1);
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, imsi_val);

    if(strcmp(imsi_val, s_mail_settings.card_imsi))
    {
        //imsi不一致
        //COMM_TRACE:"Mail:imsi change old %s new %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1874_112_2_18_2_47_52_167,(uint8*)"ss", s_mail_settings.card_imsi, imsi_val);
        return TRUE;            
    }
    else
    {
        //imsi一致
        //COMM_TRACE:"Mail:imsi c %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1880_112_2_18_2_47_52_168,(uint8*)"s", imsi_val);
        return FALSE;             
    }
}

/*==========================================================
 * Function     : mail_DelOnServerAfterRetr
 * Author	    : sunguochen      
 * Description : 下载后删除服务器邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/30/2010 
 ==========================================================*/
comm_boolean mail_DelOnServerAfterRetr(void)
{
    return TRUE;
}

/*==========================================================
 * Function     : mail_RetrMailMaxLength
 * Author	    : sunguochen      
 * Description : 下载邮件最大个数
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_RetrMailMaxLength(void)
{
    return MAIL_BODY_MAX_LEN;
}

/*==========================================================
 * Function     : mail_ReceivingNewMsg
 * Author	    : sunguochen      
 * Description : 接收邮件范围:
 * Parameter	: type          parameter       Description
 * Return	    : TRUE : 接收新邮件
                       FALSE : 接收全部
 * Effect	    : NO
 * History	: created  5/5/2010 
 ==========================================================*/
comm_boolean mail_ReceivingNewMsg(uint16 accountid)
{
    //COMM_TRACE:"MAIL:mail_ReceivingNewMsg account: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1922_112_2_18_2_47_52_169,(uint8*)"d", accountid);
    return TRUE;
}

/*==========================================================
 * Function     : mail_SaveSingleAccount
 * Author	    : sunguochen      
 * Description : 保存单个账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/12/2010 
 ==========================================================*/
void mail_SaveSingleAccount(mail_account_info *account_info, MMIMAIL_SETTING_INFO_T   *sigle_account)
{
    mail_account_info *account_cur = NULL;

    account_cur = &account_info[0];

    account_cur->account_id = MAIL_ACCOUNT_ID_START;

    account_cur->mail_account_settings.pro_r = sigle_account->pro_r;
    account_cur->mail_account_settings.ssl = sigle_account->ssl;
    account_cur->mail_account_settings.receive_length= sigle_account->receive_length;  
    account_cur->mail_account_settings.send_ip_add= sigle_account->send_ip_add;  
    account_cur->mail_account_settings.send_port= sigle_account->send_port;    
    account_cur->mail_account_settings.receive_ip_add= sigle_account->receive_ip_add;  
    account_cur->mail_account_settings.receive_port= sigle_account->receive_port;   
    account_cur->mail_account_settings.proxy_ip= 0;  
    account_cur->mail_account_settings.proxy_port= 0;
    strcpy((char *)account_cur->mail_account_settings.user_name, (char *)sigle_account->user_name);
    strcpy((char *)account_cur->mail_account_settings.user_password, (char *)sigle_account->user_password);
    strcpy((char *)account_cur->mail_account_settings.sms_number, (char *)sigle_account->sms_number);
    strcpy((char *)account_cur->mail_account_settings.apn, (char *)sigle_account->apn);    
//    COMM_TRACE("mail_SaveSingleAccount:send_ip_add :%s", inet_ntoa(account_cur->mail_account_settings.send_ip_add));
//    COMM_TRACE("mail_SaveSingleAccount:receive_ip_add:%s", inet_ntoa(account_cur->mail_account_settings.receive_ip_add));
    if(!account_cur->mail_account_settings.ssl)
    {
        account_cur->mail_account_settings.is_mas_account = 1;
        account_cur->mail_account_settings.is_adc_account = 0;
        //COMM_TRACE:"Mail:mail_SaveSingleAccount This is a single mas account!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1961_112_2_18_2_47_52_170,(uint8*)"");
    }
    else
    {
        if(0 == strlen((char *)account_cur->mail_account_settings.user_password))
        {
            account_cur->mail_account_settings.is_mas_account = 0;
            account_cur->mail_account_settings.is_adc_account = 1;
            //COMM_TRACE:"Mail:mail_SaveSingleAccount This is a single adc account!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1969_112_2_18_2_47_52_171,(uint8*)"");
        }
        else
        {
            account_cur->mail_account_settings.is_mas_account = 0;
            account_cur->mail_account_settings.is_adc_account = 0;
            //COMM_TRACE:"Mail:mail_SaveSingleAccount This is a single 139 mail account!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_1975_112_2_18_2_47_52_172,(uint8*)"");
        }
    }
}

/*==========================================================
 * Function     : mail_SaveSingleAccount
 * Author	    : sunguochen      
 * Description : 保存多个账户信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/12/2010 
 ==========================================================*/
void mail_SaveMultiAccount(mail_account_info *account_info, MMIMAIL_MORE_SETTING_INFO_T *multi_account)
{
    int index = 0;
    int account_num = 0;
    mail_account_info *account_cur = NULL;
    MMIMAIL_MORE_SETTING_CONTENT_DATA_INFO_T *setting = NULL;

    account_num = multi_account->account_num;

    if(account_num > MAIL_MAX_ACCOUNT_NUM)
    {
        account_num = MAIL_MAX_ACCOUNT_NUM;
    }

    for(index = 0; index < account_num; index++)
    {
        account_cur = &(account_info[index]);

        setting = &(multi_account->setting_info[index].setting_data);

        account_cur->account_id = MAIL_ACCOUNT_ID_START + index;
        account_cur->mail_account_settings.pro_r = multi_account->pro_r;
        account_cur->mail_account_settings.ssl = setting->ssl;
        account_cur->mail_account_settings.receive_length= 0;  
        account_cur->mail_account_settings.send_ip_add= setting->send_ip_addr;  
        account_cur->mail_account_settings.send_port= setting->send_port;    
        account_cur->mail_account_settings.receive_ip_add= setting->receive_ip_addr;  
        account_cur->mail_account_settings.receive_port= setting->receive_port;   
        account_cur->mail_account_settings.proxy_ip= setting->proxy_ip;  
        account_cur->mail_account_settings.proxy_port= setting->proxy_port;
        strcpy((char *)account_cur->mail_account_settings.user_name, (char *)setting->user_name);
        strcpy((char *)account_cur->mail_account_settings.user_password, (char *)setting->user_password);
        strcpy((char *)account_cur->mail_account_settings.sms_number, (char *)setting->smsnumber);
        strcpy((char *)account_cur->mail_account_settings.apn, (char *)setting->apn);    
        strcpy((char *)account_cur->mail_account_settings.cid, (char *)setting->cid);
//        COMM_TRACE("mail_SaveMultiAccount:send_ip_add :%s", inet_ntoa(account_cur->mail_account_settings.send_ip_add));
//        COMM_TRACE("mail_SaveMultiAccount:receive_ip_add:%s", inet_ntoa(account_cur->mail_account_settings.receive_ip_add));
        if(!account_cur->mail_account_settings.ssl)
        {
            account_cur->mail_account_settings.is_mas_account = 1;
            account_cur->mail_account_settings.is_adc_account = 0;
            //COMM_TRACE:"Mail:mail_SaveMultiAccount This is a mas account!account_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2029_112_2_18_2_47_52_173,(uint8*)"d", account_cur->account_id);
        }
        else
        {
            if(0 == strlen((char *)account_cur->mail_account_settings.user_password))
            {
                account_cur->mail_account_settings.is_mas_account = 0;
                account_cur->mail_account_settings.is_adc_account = 1;
                //COMM_TRACE:"Mail:mail_SaveMultiAccount This is a adc account!account_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2037_112_2_18_2_47_52_174,(uint8*)"d", account_cur->account_id);
            }
            else
            {
                account_cur->mail_account_settings.is_mas_account = 0;
                account_cur->mail_account_settings.is_adc_account = 0;
                //COMM_TRACE:"Mail:mail_SaveMultiAccount This is a 139 mail account!account_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2043_112_2_18_2_47_52_175,(uint8*)"d", account_cur->account_id);
            }
        }
    }
}

/*==========================================================
 * Function     : mail_AccountSettingsArrival
 * Author	    : sunguochen      
 * Description : 账户设置信息处理
 * Parameter	: type          parameter       Description
                       void *accountinfo,           --账户信息
                       comm_boolean is_single  --是否单个
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/12/2010 
 ==========================================================*/
void mail_AccountSettingsArrival(void *accountinfo, comm_boolean is_single)
{        
    mail_account_info *account_info = NULL;

    s_mail_settings.recved_settings = TRUE;

    //COMM_TRACE:"MAIL:mail_AccountSettingsArrival new setting"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2065_112_2_18_2_47_52_176,(uint8*)"");

    if(accountinfo == NULL)
    {
        //COMM_TRACE:"MAIL:mail_AccountSettingsArrival error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2069_112_2_18_2_47_52_177,(uint8*)"");
        //通知账户信息不存在
        MMIAPIMAIL_GetAccountCallback(FALSE, FALSE);
        return ;
    }

    account_info = comm_alloc(sizeof(mail_account_info)*MAIL_MAX_ACCOUNT_NUM);

    //COMM_ASSERT(account_info);
    if(account_info == NULL)
    {
        return;
    }

    memset(account_info, 0, sizeof(mail_account_info)*MAIL_MAX_ACCOUNT_NUM);
    
    if(is_single)
    {
        MMIMAIL_SETTING_INFO_T           *sigle_account = NULL;

        sigle_account = (MMIMAIL_SETTING_INFO_T *)accountinfo;

        mail_SaveSingleAccount(account_info, sigle_account);
    }
    else
    {
        MMIMAIL_MORE_SETTING_INFO_T *multi_account = NULL;

        multi_account = (MMIMAIL_MORE_SETTING_INFO_T *)accountinfo;

        mail_SaveMultiAccount(account_info, multi_account);

    }

    if(PNULL != s_mail_settings.tmp_account_info)
    {
        //收到多次setting消息，删除之前信息
        comm_free(s_mail_settings.tmp_account_info);
    }

    s_mail_settings.tmp_account_info = account_info;

    if(mail_ExistLocalAccount())
    {
        //存在账户
        if(mail_isCardSwitch())
        {
            //更换卡--提示用户是否覆盖
            MMIAPIMAIL_GetAccountCallback(TRUE, FALSE);
            //COMM_TRACE:"MAIL:mail_AccountSettingsArrival change card"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2118_112_2_18_2_47_52_178,(uint8*)"");
        }
        else
        {
            //未更换卡--直接替换
            mail_WriteAllSettings();
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
            //收到账户配置消息后更新信息列表
            mail_switchAccount();    
#endif            
        }
    }
    else
    {
        //不存在账户
        MMIAPIMAIL_GetAccountCallback(TRUE, FALSE);
    }
}

/*==========================================================
 * Function     : mail_tmpAccountNum
 * Author	    : sunguochen      
 * Description : 临时信息的账户个数
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/12/2010 
 ==========================================================*/
int mail_tmpAccountNum(mail_account_info *account_info)
{
    int index = 0;
    int account_num = 0;
    mail_account_info *account_cur = NULL;

    if(account_info == NULL)
    {
        return 0;
    }

    for(index = 0; index < MAIL_MAX_ACCOUNT_NUM; index++)
    {
        account_cur = &(account_info[index]);
        if(account_cur->account_id && account_cur->mail_account_settings.send_ip_add)
        {
            account_num ++;
        }
    }

    return account_num;
}



/*==========================================================
 * Function     : mail_WriteAllSettings
 * Author	    : sunguochen      
 * Description : 保存接收到的账户信息-临时信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/13/2010 
 ==========================================================*/
void mail_WriteAllSettings( void )
{
    char imsi_val[(MN_MAX_IMSI_ARR_LEN << 1) + 1] = { 0 };
    MN_IMSI_T imsi = {0};
    int account_num = 0;

    imsi = MNSIM_GetImsiEx(MN_DUAL_SYS_1);
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, imsi_val);

    account_num = mail_tmpAccountNum(s_mail_settings.tmp_account_info);

    //COMM_TRACE:"MAIL: mail_WriteAllSettings "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2189_112_2_18_2_47_52_179,(uint8*)"");
    if(account_num == 0)
    {
        //COMM_TRACE:"Mail: no account num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2192_112_2_18_2_47_52_180,(uint8*)"");
        return;
    }

    if(0 == s_mail_settings.account_num)
    {
        //不存在账户，直接加入即可
        memset(&s_mail_settings, 0, (sizeof(mail_setting_info) - MAIL_ACCOUNT_PTR_LEN));
        s_mail_settings.reply_with_original_cont = TRUE;
        s_mail_settings.mail_setting_list = s_mail_settings.tmp_account_info;
        s_mail_settings.tmp_account_info = NULL;
        s_mail_settings.account_num = account_num;
        s_mail_settings.default_account = MAIL_ACCOUNT_ID_START;
    }
    else
    {
        if(PNULL != s_mail_settings.mail_setting_list)
        {
            comm_free(s_mail_settings.mail_setting_list);
        }
        memset(&s_mail_settings, 0, (sizeof(mail_setting_info) - MAIL_ACCOUNT_PTR_LEN));        
        s_mail_settings.reply_with_original_cont = TRUE;
        s_mail_settings.mail_setting_list = s_mail_settings.tmp_account_info;
        s_mail_settings.tmp_account_info = NULL;
        s_mail_settings.account_num = account_num;
        s_mail_settings.default_account = MAIL_ACCOUNT_ID_START;
    }

    //保存卡的imsi
    //未保存imsi，可直接保存
    //COMM_TRACE:"Mail:imsi new %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2222_112_2_18_2_47_52_181,(uint8*)"s", imsi_val);
    memset(s_mail_settings.card_imsi, 0, (MAIL_IMSI_SAVE_LEN + 1));
    strcpy(s_mail_settings.card_imsi, imsi_val);
    
    //写文件
    mail_WriteMailCfgfile(&s_mail_settings);
}



/*==========================================================
 * Function     : mail_SettinginfoRecved
 * Author	    : sunguochen      
 * Description : 是否收到过配置信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/12/2010 
 ==========================================================*/
comm_boolean mail_SettinginfoRecved(void)
{
    //COMM_TRACE:"Mail: mail_SettinginfoRecved %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2242_112_2_18_2_47_53_182,(uint8*)"d", s_mail_settings.recved_settings);
    return s_mail_settings.recved_settings;
}



/*==========================================================
 * Function     : mail_RecvedSettinginfoReset
 * Author	    : sunguochen      
 * Description : 清除收到的配置信息标志
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/12/2010 
 ==========================================================*/
void mail_RecvedSettinginfoReset(void)
{
    s_mail_settings.recved_settings = FALSE;

    //COMM_TRACE:"Mail: mail_SettinginfoRecved %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2260_112_2_18_2_47_53_183,(uint8*)"d", s_mail_settings.recved_settings);
}



/*==========================================================
 * Function     : mail_ExistTmpSettinginfo
 * Author	    : sunguochen      
 * Description : 是否存在临时配置信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/12/2010 
 ==========================================================*/
comm_boolean mail_ExistTmpSettinginfo(void)
{
    //COMM_TRACE:"Mail: mail_SettinginfoRecved %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2275_112_2_18_2_47_53_184,(uint8*)"d", s_mail_settings.tmp_account_info);
    if(s_mail_settings.tmp_account_info == NULL)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*==========================================================
 * Function     : mail_AccountNeedPassword
 * Author	    : sunguochen      
 * Description : 企业版用户需要用户提前输入密码
                      否则无法收发邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  6/13/2010 
 ==========================================================*/
comm_boolean mail_AccountNeedPassword(int accountid)
{
    int  password_len = 0;
    mail_account_info *account_info = NULL;  

    account_info = mail_GetAccountInfoById(accountid);

    if(account_info == NULL)
    {
        return FALSE;
    }
    //企业版账户
    if(mail_AccountIsGroupAccount(accountid))
    {
        password_len = strlen((char *)account_info->mail_account_settings.user_password);
        //COMM_TRACE:"Mail:mail_AccountNeedPassword password %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_SETTING_2310_112_2_18_2_47_53_185,(uint8*)"s", account_info->mail_account_settings.user_password);

        if(password_len == 0)
        {
            //无密码
            return TRUE;
        }
    }

    return FALSE;
}

