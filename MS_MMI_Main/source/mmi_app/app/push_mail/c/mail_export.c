/****************************************************************************
** File Name:      mail_export.c                                            *
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
#define _MAIL_EXPORT_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_push_mail_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mail_main.h"
#include "mail_setting.h"
#include "comm_platform.h"
#include "mail_network.h"
#include "comm_memory.h"
#include "mail_export.h"
#include "mail_retr.h"
#include "mail_sig.h"
#include "mail_pop3.h"
#include "mail_smtp.h"
#include "mail_fldr.h"
#include "mail_setting.h"
#include "comm_memory.h"
#ifdef MAIL_PUSH_SETTING_TEST
#include "mmimms_push.h"
#endif
#include "mmiphone_export.h"
#include "mmimail_wintab.h"
#include "mail_attachment.h"
#include "mmisms_app.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef MAIL_PUSH_SETTING_TEST
extern void MMIMAIL_SaveMailDataForSetting(MMIMMS_USER_VALID_DATA_T data);

char s_push1[] = {
  0x01, 0x06, 0x25, 0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 
  0x74, 0x69, 0x6F, 0x6E, 0x2F, 0x76, 0x6E, 0x64, 0x2E, 0x63, 
  0x6D, 0x63, 0x63, 0x2E, 0x73, 0x65, 0x74, 0x74, 0x69, 0x6E, 
  0x67, 0x2B, 0x77, 0x62, 0x78, 0x6D, 0x6C, 0x00, 0xAF, 0x89, 
  0x02, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x50, 
  0x00, 0xDA, 0xC8, 0xF3, 0xEA, 0x46, 0x69, 0xDA, 0xC8, 0xF3, 
  0xEA, 0x46, 0xBE, 0x31, 0x35, 0x38, 0x36, 0x35, 0x33, 0x32, 
  0x37, 0x30, 0x33, 0x35, 0x40, 0x31, 0x33, 0x39, 0x2E, 0x63, 
  0x6F, 0x6D, 0x23, 0x31, 0x30, 0x39, 0x33, 0x00, 0x20, 0x00, 
  0x43, 0x4D, 0x57, 0x41, 0x50, 0x00
};

#define PUSH1_LEN sizeof(s_push1) / sizeof(s_push1[0])

char s_push2[] = {
  0x01, 0x06, 0x25, 0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 
  0x74, 0x69, 0x6F, 0x6E, 0x2F, 0x76, 0x6E, 0x64, 0x2E, 0x63, 
  0x6D, 0x63, 0x63, 0x2E, 0x73, 0x65, 0x74, 0x74, 0x69, 0x6E, 
  0x67, 0x2B, 0x77, 0x62, 0x78, 0x6D, 0x6C, 0x00, 0xAF, 0x89, 
  0x02, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x50, 
  0x00, 0xDA, 0xC8, 0xF3, 0xEA, 0x46, 0x69, 0xDA, 0xC8, 0xF3, 
  0xEA, 0x46, 0xBE, 0x31, 0x35, 0x38, 0x36, 0x35, 0x33, 0x32, 
  0x37, 0x30, 0x33, 0x36, 0x40, 0x31, 0x33, 0x39, 0x2E, 0x63, 
  0x6F, 0x6D, 0x23, 0x31, 0x30, 0x39, 0x33, 0x00, 0x20, 0x00, 
  0x43, 0x4D, 0x57, 0x41, 0x50, 0x00
};

#define PUSH2_LEN sizeof(s_push2) / sizeof(s_push2[0])

char s_push3[] = {
  0x01, 0x06, 0x25, 0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 
  0x74, 0x69, 0x6F, 0x6E, 0x2F, 0x76, 0x6E, 0x64, 0x2E, 0x63, 
  0x6D, 0x63, 0x63, 0x2E, 0x73, 0x65, 0x74, 0x74, 0x69, 0x6E, 
  0x67, 0x2B, 0x77, 0x62, 0x78, 0x6D, 0x6C, 0x00, 0xAF, 0x89, 
  0x02, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x50, 
  0x00, 0xDA, 0xC8, 0xF3, 0xEA, 0x46, 0x69, 0xDA, 0xC8, 0xF3, 
  0xEA, 0x46, 0xBE, 0x31, 0x35, 0x38, 0x36, 0x35, 0x33, 0x32, 
  0x37, 0x30, 0x33, 0x37, 0x40, 0x31, 0x33, 0x39, 0x2E, 0x63, 
  0x6F, 0x6D, 0x23, 0x31, 0x30, 0x39, 0x33, 0x00, 0x20, 0x00, 
  0x43, 0x4D, 0x57, 0x41, 0x50, 0x00
};

#define PUSH3_LEN sizeof(s_push3) / sizeof(s_push3[0])

#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern struct fsmdata fsmFldrData;

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support 
//added by liangshun 20110126 for MS00225437
extern MMISMS_GLOBAL_T g_mmisms_global;      // SMS的全局变量
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
BOOLEAN g_cur_mail_is_locked = FALSE;//记录当前编辑邮件锁定状态的标志位。

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*==========================================================
 * Function     : mail_retrieve_test
 * Author	    : sunguochen      
 * Description : 测试代码
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/14/2010 
 ==========================================================*/
void mail_retrieve_test(void)
{
    int i = 0;

    for(i = 10; i > 0; i--)
    {
        if(mail_accountIsValid(i))
        {
            //COMM_TRACE:"Mail:username id : %d:%s password: %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_134_112_2_18_2_47_37_0,(uint8*)"dss", i, mail_UserName(i), mail_passWord(i));
        }        
    }

    //向mail线程发送请求
    mail_SendRetrMsgReq(mail_GetDefaultAccount());
}


/*==========================================================
 * Function     : mail_Send_test
 * Author	    : sunguochen      
 * Description : 测试代码
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/14/2010 
 ==========================================================*/
void mail_Send_test(void)
{
#ifdef MAIL_PUSH_SETTING_TEST
    {
        MMIMMS_USER_VALID_DATA_T    s_mms_data = {0};

        memset(&s_mms_data, 0, sizeof(MMIMMS_USER_VALID_DATA_T));
        s_mms_data.length = PUSH1_LEN;
        memcpy(s_mms_data.user_valid_data_arr, s_push1, PUSH1_LEN);
        MMIMAIL_IsPushMailForContent(s_mms_data.user_valid_data_arr, s_mms_data.length);

        memset(&s_mms_data, 0, sizeof(MMIMMS_USER_VALID_DATA_T));        
        s_mms_data.length = PUSH2_LEN;
        memcpy(s_mms_data.user_valid_data_arr, s_push2, PUSH2_LEN);
        MMIMAIL_IsPushMailForContent(s_mms_data.user_valid_data_arr, s_mms_data.length);

        memset(&s_mms_data, 0, sizeof(MMIMMS_USER_VALID_DATA_T));
        s_mms_data.length = PUSH3_LEN;
        memcpy(s_mms_data.user_valid_data_arr, s_push3, PUSH3_LEN);
        MMIMAIL_IsPushMailForContent(s_mms_data.user_valid_data_arr, s_mms_data.length);
/**/
        //mail_DeleteAccount(0);
        //mail_DeleteAccount(3);        

    }
#endif

    mail_SendSendMsgReq(mail_GetDefaultAccount());
    //mail_sendTest(1);
}

/*==========================================================
 * Function     : pushmail_Start
 * Author	    : sunguochen      
 * Description : 启动email
 * Parameter	: type          parameter       Description
                       MAIL_START_MODE start_mode    启动方式
                       char* para                                 启动需要的参数
 * Return	    : Void
 * Effect	    : NO
 * History	: created  4/29/2010 
 ==========================================================*/
void pushmail_Start(MAIL_START_MODE start_mode, char* para)
{
    switch(start_mode)
    {
        case MAIL_START_MAINMENU:
            break;

        case MAIL_START_INBOX:
            break;

        case MAIL_START_SEND_ATTACH:
            break;

        case MAIL_START_SEND_ADDR:
            break;            

        case MAIL_START_RETR:
            break;

        default:
            break;
    }
}

/*==========================================================
 * Function     : mail_CoreInit
 * Author	    : sunguochen      
 * Description : 手机邮箱开机初始化
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_CoreInit(void)
{
#ifdef MAIL_MEMORY_DEBUG_MODE
    mail_mem_link_init();
#endif
    //ssl 初始化
    SSL_Init();
    //mail 主线程初始化
    mail_mainInit();
    //创建手机邮箱线程
    mail_CreateThread();
    //邮箱设置初始化
    mail_settingInit();
    //socket初始化
    comm_sock_init();
    //网络架构初始化    
    mail_NetInitial();

    //邮件内部信号管理初始化 
    mail_SignalInit();
    //邮件管理初始化 
    mail_fldrSysInit();

    //接收初始化    
    mailRetrMgr_init();
    //POP3初始化    
    mailPop_init();
    //smtp初始化
    mail_SmtpInit();

#ifdef MAIL_DEBUG_SAVE_FILE
    mail_debug_init();
#endif
    
}

/*==========================================================
 * Function     : mail_CoreInit
 * Author	    : sunguochen      
 * Description : 手机邮箱终止
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_CoreFinal(void)
{
    
}

/*==========================================================
 * Function     : mail_DisableAccount
 * Author	    : sunguochen      
 * Description : 账户暂停使用
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/6/2010 
 ==========================================================*/
void mail_DisableAccount(comm_uint16 accountid)
{
    if(!mail_accountIsValid(accountid))
    {
        //非法id
        //COMM_TRACE:"Mail:invalid account id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_285_112_2_18_2_47_37_1,(uint8*)"d", accountid);
        return;
    }
    
    mail_SendRetrEnableReq(accountid, FALSE);
}

/*==========================================================
 * Function     : mail_EnableAccount
 * Author	    : sunguochen      
 * Description : 账户恢复使用
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/6/2010 
 ==========================================================*/
void mail_EnableAccount(comm_uint16 accountid)
{

    if(!mail_accountIsValid(accountid))
    {
        //非法id
        //COMM_TRACE:"Mail:invalid account id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_306_112_2_18_2_47_37_2,(uint8*)"d", accountid);
        return;
    }
    
    mail_SendRetrEnableReq(accountid, TRUE);
}

/*==========================================================
 * Function     : mail_retrievePushMsg
 * Author	    : sunguochen      
 * Description : 信息通知到-自动接收邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/14/2010 
 ==========================================================*/
void mail_retrievePushMsg(MMIMAIL_EMN_INFO_T* emn_info)
{
    int accountid = 0;

    //COMM_TRACE:"Mail: %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_325_112_2_18_2_47_37_3,(uint8*)"s", MAIL_HS_VERSION);

    if(NULL == emn_info)
    {
        //COMM_TRACE:"Mail:start receive msg-err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_329_112_2_18_2_47_37_4,(uint8*)"");
        return;
    }

    accountid = mail_GetAccountIdByUserName((char *)(emn_info->user_name));

    //COMM_TRACE:"Mail:start receive msg-id:%d, name:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_335_112_2_18_2_47_37_5,(uint8*)"ds", accountid, emn_info->user_name);

    if(!mail_accountIsValid(accountid))
    {
        //非法id
        //COMM_TRACE:"Mail:invalid account id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_340_112_2_18_2_47_37_6,(uint8*)"d", accountid);
        return;
    }

    if(mail_GetRecvingState())
    {
        //COMM_TRACE:"Mail:mail_retrievePushMsg-retring \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_346_112_2_18_2_47_37_7,(uint8*)"");
        return;
    }

    if(mail_GetSendingState())
    {
        //COMM_TRACE:"Mail:mail_retrievePushMsg-sending \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_352_112_2_18_2_47_37_8,(uint8*)"");
        mail_SetNeedRecvingState( TRUE );
        return;
    }

    //自动接收
    mail_SetAutoRecvingState(TRUE);
    //向mail线程发送请求
    mail_SendRetrMsgReq(accountid);
}

/*==========================================================
 * Function     : mail_retrieveMsgManual
 * Author	    : sunguochen      
 * Description : 手动接收邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_retrieveMsgManual(int accountid)
{
    int account = 0;

    if(mail_GetRecvingState())
    {
        mail_retr_result_t     *retr_result = NULL;
        //COMM_TRACE:"Mail:mail_RetrAttachStart-retring"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_378_112_2_18_2_47_37_9,(uint8*)"");

        retr_result = comm_alloc(sizeof(mail_retr_result_t));
        if (NULL == retr_result)
        {
            return;
        }
        retr_result->retr_num = 0;
        retr_result->retr_result = MAIL_RETR_ERR_RETRING ;
        mail_RetrMailNotify(retr_result);        
        return;
    }

    if(mail_GetSendingState())
    {
        mail_retr_result_t     *retr_result = NULL;
        //COMM_TRACE:"Mail:mail_RetrAttachStart-retring"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_394_112_2_18_2_47_38_10,(uint8*)"");

        retr_result = comm_alloc(sizeof(mail_retr_result_t));
        if (NULL == retr_result)
        {
            return;
        }
        retr_result->retr_num = 0;
        retr_result->retr_result = MAIL_RETR_ERR_SENDING ;
        mail_RetrMailNotify(retr_result);        
        return;
    }
    
    if(!mail_accountIsValid(accountid))
    {
        //非法id
        //COMM_TRACE:"Mail:invalid account id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_410_112_2_18_2_47_38_11,(uint8*)"d", accountid);
        account = mail_GetDefaultAccount();

        if(MAIL_INVALD_ACCOUNT_ID == account)
        {
            mail_retr_result_t     *retr_result = NULL;
            //COMM_TRACE:"Mail:mail_RetrAttachStart-retring"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_416_112_2_18_2_47_38_12,(uint8*)"");
    
            retr_result = comm_alloc(sizeof(mail_retr_result_t));
        if (NULL == retr_result)
        {
            return;
        }
            retr_result->retr_num = 0;
            retr_result->retr_result = MAIL_RETR_ERR_PARA ;
            mail_RetrMailNotify(retr_result);                 
            return;
        }
    }
    else
    {
        account = accountid;
    }

    //向mail线程发送请求
    mail_SendRetrMsgReq(account);
}

/*==========================================================
 * Function     : mail_retrieve_test
 * Author	    : sunguochen      
 * Description : 手动接收附件
                      接口改变
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_retrieveAttachmentManual(int accountid, char* uidl, int attachid)
{
    if(!mail_accountIsValid(accountid))
    {
        //非法id
        //COMM_TRACE:"Mail:invalid account id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_452_112_2_18_2_47_38_13,(uint8*)"d", accountid);
        return;
    }

    if(uidl == NULL || *uidl == 0)
    {
        //非法uidl
        //COMM_TRACE:"Mail:invalid uidl"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_459_112_2_18_2_47_38_14,(uint8*)"");
        return;
    }        

    //向mail线程发送请求
    //mail_SendRetrAttachReq(accountid, uidl, attachid);
}

/*==========================================================
 * Function     : mail_isDisableUdisk
 * Author	    : sunguochen      
 * Description : 判断是否可以启动u盘
                 当在邮箱中或者收发邮件时不允许启动u盘
                 否则写文件会出错
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  7/19/2010 
 ==========================================================*/
BOOLEAN mail_isEnableUdisk(void)
{
    //手机邮箱中，不允许启动U盘
    if(MMIMAIL_IsReady())
    {
        //COMM_TRACE:"Mail:is mail, do not allow u disk"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_482_112_2_18_2_47_38_15,(uint8*)"");
        return TRUE;
    }

    //正在收发邮件，不允许启动U盘    
    if(mail_GetRecvingState() || mail_GetSendingState())
    {
        //COMM_TRACE:"Mail:is receiving mail, do not allow u disk"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_489_112_2_18_2_47_38_16,(uint8*)"");
        return TRUE;
    }

    //正在写配置文件
    if(mail_GetIsWritingMmtFile())
    {
        //COMM_TRACE:"Mail:is writing mail, do not allow u disk"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_496_112_2_18_2_47_38_17,(uint8*)"");
        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * Function     : mail_msgIsMail
 * Author	    : sunguochen      
 * Description : 是否是push email
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
BOOLEAN mail_msgIsMail(int record_id)
{
    if((uint32)record_id & 0xFF000000)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*==========================================================
 * Function     : mail_getMsgIdByRecordId
 * Author	    : sunguochen      
 * Description : 从sms的record id转为mail的msg id
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
int mail_getMsgIdByRecordId(int record_id)
{
    return (int)(record_id & 0xFFFFFF);
}

/*==========================================================
 * Function     : mail_ReplyMailByRecordId
 * Author	    : sunguochen      
 * Description : 回复邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
void mail_ReplyMailByRecordId(int record_id)
{
    int msg_id = 0;

    msg_id = mail_getMsgIdByRecordId(record_id);

    //回复邮件
    MMIAPIMAIL_Enter(MAIL_ENTER_REPLYMAIL, (void *)msg_id);
}

/*==========================================================
 * Function     : mail_ForwardMailByRecordId
 * Author	    : sunguochen      
 * Description : 转发邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
void mail_ForwardMailByRecordId(int record_id)
{
    int msg_id = 0;

    msg_id = mail_getMsgIdByRecordId(record_id);

    //转发邮件
    MMIAPIMAIL_Enter(MAIL_ENTER_FORWARDMAIL, (void *)msg_id);
}

/*==========================================================
 * Function     : mail_EditMailByRecordId
 * Author	    : sunguochen      
 * Description : 编辑邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
void mail_EditMailByRecordId(int record_id)
{
    int msg_id = 0;

    msg_id = mail_getMsgIdByRecordId(record_id);

    //编辑邮件
    MMIAPIMAIL_Enter(MAIL_ENTER_EDITMAIL, (void *)msg_id);
}

/*==========================================================
 * Function     : mail_ReadMailByRecordId
 * Author	    : sunguochen      
 * Description : 阅读邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/18/2010 
 ==========================================================*/
void mail_ReadMailByRecordId(int record_id)
{
    int msg_id = 0;
    
    msg_id = mail_getMsgIdByRecordId(record_id);

    //阅读邮件
    MMIAPIMAIL_Enter(MAIL_ENTER_READWIN, (void *)msg_id);

    return;
}

/*==========================================================
 * Function     : mail_EditNewMail
 * Author	    : sunguochen      
 * Description : 新建邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/18/2010 
 ==========================================================*/
void mail_EditNewMail(void)
{
    MMIAPIMAIL_Enter(MAIL_ENTER_NEWMAIL, PNULL);
    return;
}

/*==========================================================
 * Function     : mail_EnterMailSettings
 * Author	    : sunguochen      
 * Description : 进入邮箱设置
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/18/2010 
 ==========================================================*/
void mail_EnterMailSettings(void)
{
    MMIAPIMAIL_Enter(MAIL_ENTER_SETTINGWIN, PNULL);
    return;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//   Author: lixu
//	Note:
/*****************************************************************************/
void MAIL_ListInit(void)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support    
    int i = 0;
    comm_uint16 subject_len = 0;
    comm_uint16 sender_len = 0;
    MMISMS_ORDER_INFO_T order_ptr = {0};

    if ((
            (!MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_1)) 
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            && (!MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_2))
#endif
        )
        || mail_isCardSwitch())

    {
        //COMM_TRACE:"Mail:MAIL_ListInit no sim card or switch sim card"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_659_112_2_18_2_47_38_18,(uint8*)"");
        return;
    }

    mail_fldrMgrInit();
    
    if (PNULL == fsmFldrData.masterTable)
    {
        return;
    }
    for(i = 0; i < fsmFldrData.masterTable->numOfMsgs ; i++)
    {
        if (PNULL != fsmFldrData.masterTable->msg)
        {
            if(mail_GetDefaultAccount() == fsmFldrData.masterTable->msg[i].accountID)
            {
  
              SCI_MEMSET(&order_ptr, 0, sizeof(MMISMS_ORDER_INFO_T));

              order_ptr.msg_type = MMISMS_TYPE_MAIL;

              //subject
              subject_len = MMIAPICOM_Wstrlen(fsmFldrData.masterTable->msg[i].truncSubject);
              subject_len = MIN(subject_len, MMIMESSAGE_DIAPLAY_LEN);

              order_ptr.display_content.content_len = subject_len;
  
              MMIAPICOM_Wstrncpy(order_ptr.display_content.content,
  
                                 fsmFldrData.masterTable->msg[i].truncSubject,
  
                                 subject_len);
  
              //mo_mt_type
              switch(fsmFldrData.masterTable->msg[i].folderId)
              {
                  case MAIL_INBOX:
                      if(fsmFldrData.masterTable->msg[i].flagValues.readFlag)
                      {  
                          order_ptr.mo_mt_type = MMISMS_MT_HAVE_READ;   
                      }
                      else
                      {  
                          order_ptr.mo_mt_type = MMISMS_MT_TO_BE_READ;  
                      }
                      break;
  
                  case MAIL_OUTBOX:
                      order_ptr.mo_mt_type = MMISMS_MO_SEND_FAIL;
                      break;
  
                  case MAIL_SENT:
                      order_ptr.mo_mt_type = MMISMS_MO_SEND_SUCC;
                      break;
  
                  case MAIL_DRAFTS:
                      order_ptr.mo_mt_type = MMISMS_MO_DRAFT;
                      break;
  
                  default:
                      break;
              }
  
              //record_id
              order_ptr.record_id = fsmFldrData.masterTable->msg[i].msgId | MAIL_RECODE_INDEX;

              //sender
              sender_len = strlen(fsmFldrData.masterTable->msg[i].truncFrom);

              sender_len = MIN(sender_len,MMISMS_NUMBER_MAX_LEN);

              strncpy((char*)(order_ptr.sender),fsmFldrData.masterTable->msg[i].truncFrom,sender_len);

              //time
              if(MAIL_INBOX == fsmFldrData.masterTable->msg[i].folderId)
              {
                  order_ptr.time = fsmFldrData.masterTable->msg[i].receivedTime;
              }
              else if(MAIL_DRAFTS == fsmFldrData.masterTable->msg[i].folderId)
              {
                  order_ptr.time = fsmFldrData.masterTable->msg[i].modifiedTime;
              }
              else
              {
                  order_ptr.time = fsmFldrData.masterTable->msg[i].sentTime;
              }
    
              order_ptr.is_lock = fsmFldrData.masterTable->msg[i].flagValues.isLockFlag;
  
              if(fsmFldrData.masterTable->msg[i].flagValues.isSecurityFlag)
              {
                  order_ptr.folder_type = MMISMS_FOLDER_SECURITY;
              }
              else
              {
                  order_ptr.folder_type = MMISMS_FOLDER_NORMAL;
              }
  
              //order_ptr.is_security = FALSE;
              order_ptr.next = NULL;
              MMIAPISMS_InsertOneMsgToOrder(&order_ptr);
            }
        }
  } 
#endif
}

/*==========================================================
 * Function     : MAIL_InsertMsgToSMS
 * Author	    :       
 * Description  : 插入邮件
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/20/2010 
 ==========================================================*/
void MAIL_InsertMsgToSMS(int msg_id)
{

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    int i = 0;
    comm_uint16 subject_len= 0;
    comm_uint16 sender_len = 0;
    MMISMS_ORDER_INFO_T order_ptr = {0};

    i = getIndexFromMasterTable(msg_id);

    if(0 > i)
    {
        return;
    }
    
    if(mail_GetDefaultAccount() == fsmFldrData.masterTable->msg[i].accountID)
    {
      SCI_MEMSET(&order_ptr,0,sizeof(MMISMS_ORDER_INFO_T));

      order_ptr.msg_type = MMISMS_TYPE_MAIL;

      //subject
      subject_len = MMIAPICOM_Wstrlen(fsmFldrData.masterTable->msg[i].truncSubject);
      subject_len = MIN(subject_len, MMIMESSAGE_DIAPLAY_LEN);

      order_ptr.display_content.content_len = subject_len;

      MMIAPICOM_Wstrncpy(order_ptr.display_content.content,

                         fsmFldrData.masterTable->msg[i].truncSubject,

                         subject_len);

      //mo_mt_type
      switch(fsmFldrData.masterTable->msg[i].folderId)
      {
          case MAIL_INBOX:
              if(fsmFldrData.masterTable->msg[i].flagValues.readFlag)
              {
                  order_ptr.mo_mt_type = MMISMS_MT_HAVE_READ; 
              }
              else
              {
                  order_ptr.mo_mt_type = MMISMS_MT_TO_BE_READ;
              }
              break;

          case MAIL_OUTBOX:
              order_ptr.mo_mt_type = MMISMS_MO_SEND_FAIL;
              break;

          case MAIL_SENT:
              order_ptr.mo_mt_type = MMISMS_MO_SEND_SUCC;
              break;

          case MAIL_DRAFTS:
              order_ptr.mo_mt_type = MMISMS_MO_DRAFT;
              break;

          default:
              break;

      }

      //record_id
      order_ptr.record_id = fsmFldrData.masterTable->msg[i].msgId | MAIL_RECODE_INDEX;
      //COMM_TRACE:"MAIL_InsertMsgToSMS:msgId = %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_841_112_2_18_2_47_39_19,(uint8*)"d",fsmFldrData.masterTable->msg[i].msgId);
      //COMM_TRACE:"MAIL_InsertMsgToSMS:record_id = %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_842_112_2_18_2_47_39_20,(uint8*)"d",order_ptr.record_id);

      //sender
      sender_len = strlen(fsmFldrData.masterTable->msg[i].truncFrom);
      sender_len = MIN(sender_len,MMISMS_NUMBER_MAX_LEN);

      strncpy((char*)(order_ptr.sender),fsmFldrData.masterTable->msg[i].truncFrom,sender_len);

      //time
      if(MAIL_INBOX == fsmFldrData.masterTable->msg[i].folderId)
      {
          order_ptr.time = fsmFldrData.masterTable->msg[i].receivedTime;
      }
      else if(MAIL_DRAFTS == fsmFldrData.masterTable->msg[i].folderId)
      {
          order_ptr.time = fsmFldrData.masterTable->msg[i].modifiedTime;
      }
      else

      {
          order_ptr.time = fsmFldrData.masterTable->msg[i].sentTime;
      }

      order_ptr.is_lock = fsmFldrData.masterTable->msg[i].flagValues.isLockFlag;
      if(fsmFldrData.masterTable->msg[i].flagValues.isSecurityFlag)
      {
          order_ptr.folder_type = MMISMS_FOLDER_SECURITY;
      }
      else
      {
          order_ptr.folder_type = MMISMS_FOLDER_NORMAL;
      }

      //order_ptr.is_security = FALSE;
      order_ptr.next = NULL;
      MMIAPISMS_InsertOneMsgToOrder(&order_ptr);
    }
#endif  
}

/*==========================================================
 * Function     : mail_SetMailLockFlag
 * Author	    :       
 * Description : 增加锁定标志位
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
BOOLEAN mail_SetMailLockFlag(int record_id,BOOLEAN is_locked)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    int msg_id = 0;
    int index = 0;

    msg_id = record_id & 0xFFFFFF;

    index = getIndexFromMasterTable(msg_id);
    if(0 > index)
    {
        return FALSE;
    }

    fsmFldrData.masterTable->msg[index].flagValues.isLockFlag = is_locked;
#endif

    return TRUE;
}

/*==========================================================
 * Function     : mail_SetMailSecurityFlag
 * Author	    :       
 * Description  : 设置安全信箱标志位
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
 BOOLEAN mail_SetMailSecurityFlag(int record_id,BOOLEAN is_security)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    int msg_id = 0;
    int index = 0;

    msg_id = record_id & 0xFFFFFF;

    index = getIndexFromMasterTable(msg_id);
    if(0 > index)
    {
        return FALSE;
    }

    fsmFldrData.masterTable->msg[index].flagValues.isSecurityFlag= is_security;
#endif

    return TRUE;
}

/*==========================================================
 * author		: liangshun      
 * function 	: BOOLEAN mail_GetMailLockFlag(int record_id)
 * decr		: 返回当前邮件的锁定标志位
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/10/2010 
 ==========================================================*/
//3record_id是MMIMAIL_MAIL_SUMMERY_T中的record_id，等同于MailMessageFolderInfo中的msgId
BOOLEAN mail_GetMailLockFlag(uint32 record_id)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    uint32 msg_id = 0;
    int index = 0;
    msg_id = record_id;//不需要进行转换。
    index = getIndexFromMasterTable(msg_id);
    if(0 > index)
    {
        //COMM_TRACE:"mail_GetMailLockFlag:index < 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_956_112_2_18_2_47_39_21,(uint8*)"");
        return FALSE;//3NOTICE !
    }
    return (BOOLEAN)fsmFldrData.masterTable->msg[index].flagValues.isLockFlag;
#else
    return FALSE;
#endif
}

/*==========================================================
 * author		: liangshun      
 * function 	: BOOLEAN mail_GetMailSendingFlag(uint32 record_id)
 * decr		: 返回当前邮件发送中标志位
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/15/2011 
 ==========================================================*/
BOOLEAN mail_GetMailSendingFlag(uint32 record_id)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    uint32 msg_id = 0;
    int index = 0;
    msg_id = record_id;//不需要进行转换。
    index = getIndexFromMasterTable(msg_id);
    if(0 > index)
    {
        //COMM_TRACE:"mail_GetMailSendingFlag:index < 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_982_112_2_18_2_47_39_22,(uint8*)"");
        return FALSE;//3NOTICE !
    }
    return (BOOLEAN)fsmFldrData.masterTable->msg[index].flagValues.isSendingFlag;
#else
    return FALSE;
#endif
}

/*==========================================================
 * author		: liangshun      
 * function 	: BOOLEAN mail_GetMailSecurityFlag(uint32 record_id)
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/17/2010 
 ==========================================================*/
BOOLEAN mail_GetMailSecurityFlag(uint32 record_id)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    uint32 msg_id = 0;
    int index = 0;
    msg_id = record_id;//不需要进行转换。
    index = getIndexFromMasterTable(msg_id);
    if(0 > index)
    {
        //COMM_TRACE:"mail_GetMailSecurityFlag:index < 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1008_112_2_18_2_47_39_23,(uint8*)"");
        return FALSE;//3NOTICE !
    }
    return (BOOLEAN)fsmFldrData.masterTable->msg[index].flagValues.isSecurityFlag;
#else
    return FALSE;
#endif
}

/*==========================================================
 * author		: liangshun      
 * function 	: BOOLEAN mail_GetMailReadFlag(uint32 record_id)
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/17/2010 
 ==========================================================*/
BOOLEAN mail_GetMailReadFlag(uint32 record_id)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    uint32 msg_id = 0;
    int index = 0;
    msg_id = record_id;//不需要进行转换。
    index = getIndexFromMasterTable(msg_id);
    if(0 > index)
    {
        //COMM_TRACE:"mail_GetMailSecurityFlag:index < 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1034_112_2_18_2_47_39_24,(uint8*)"");
        return FALSE;//3NOTICE !
    }
    return (BOOLEAN)fsmFldrData.masterTable->msg[index].flagValues.readFlag;
#else
    return FALSE;
#endif
}


/*==========================================================
 * Function     : mail_SaveMailListInfo
 * Author	    :       
 * Description : 保存邮件配置信息
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
void mail_SaveMailListInfo(void)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    E_SIGNAL_SENDTO( E_FSM_FLDR_MANAGER, (int)MAIL_SIG_FMG_INT_WRITE_MMT);
#endif
}

/*****************************************************************************/
// 	Description : 通过msgId，删除指定帐户邮件
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
int mail_delMessageByMsgIdSMS(comm_uint32 msgId,comm_int32 account_id)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    int index = -1;
    MailMessageFolderInfo* mail_info = NULL;
    MMIMAIL_BOX_INFO_T *box_mail_list = NULL;

    if(0 == msgId)
    {
        return 0;
    }

    //COMM_TRACE:"mail_fldr mail_delMessageByMsgIdOtherApp del msg(%d)start! \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1075_112_2_18_2_47_39_25,(uint8*)"d",msgId);
//TODO::调用UI接口获取邮件列表
    box_mail_list = MMIMAIL_GetDefAccountMailboxInfo();
    index = getIndexFromMasterTable(msgId);
    if(0 > index)
    {
        //COMM_TRACE:"mail_fldr mail_delMessageByMsgIdOtherApp:index-msgId not matched"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1081_112_2_18_2_47_39_26,(uint8*)"");
        return -1;
    }
    mail_info = mail_getMailByIndex(index); 

    if((!box_mail_list) ||(!mail_info))
    {
        //COMM_TRACE:"mail_fldr mail_delMessageByMsgIdOtherApp:delete msg not found or msg list not found"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1088_112_2_18_2_47_39_27,(uint8*)"");
        return -1;
    }

    //COMM_TRACE:"mail_fldr mail_delMessageByMsgIdOtherApp delete msg accountId = %d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1092_112_2_18_2_47_39_28,(uint8*)"d",mail_info->accountID);
    //COMM_TRACE:"mail_fldr mail_delMessageByMsgIdOtherApp cur accountId = \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1093_112_2_18_2_47_39_29,(uint8*)"");
    if(account_id == mail_info->accountID)
    {             
        //COMM_TRACE:"mail_fldr mail_delMessageByMsgIdOtherApp mail->uidl=%s \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1096_112_2_18_2_47_39_30,(uint8*)"s",mail_info->uidl);
        mail_DeleteMailAttchment(mail_info->uidl);
        mail_delSMSMessageListById(mail_info->msgId);
        mail_fldrmgrDeleteMsg(mail_info->msgId);
    }
    else
    {
        return -1;
    }
#endif

    return 1;

}

/*==========================================================
 * Function     : Mail_DelMailByMsgId
 * Author	    :       
 * Description : 删除邮件
 * Parameter	: type          parameter       Description
                 cbf--回调函数，删除完毕，调用此函数通知
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
BOOLEAN Mail_DelMailByMsgId(int record_id)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    int msg_id = 0;

    msg_id = record_id & 0xFFFFFF;
    
    mail_delMessageByMsgIdSMS(msg_id, mail_GetDefaultAccount());
    //保存mail配置到File
    E_SIGNAL_SENDTO( E_FSM_FLDR_MANAGER, (int)MAIL_SIG_FMG_INT_WRITE_MMT);
#endif
    return TRUE;
}

/*==========================================================
 * Function     : Mail_GetListItemByMsgId
 * Author	    :   
 * Description : 从id获得信息内容
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
PUBLIC MMISMS_ORDER_INFO_T *Mail_GetListItemByMsgId(int32 msg_id)
{

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    int index = 0;
    comm_uint16 subject_len= 0;
    comm_uint16 sender_len = 0;
    MMISMS_ORDER_INFO_T *order_ptr = NULL;

    index = getIndexFromMasterTable(msg_id);
    if(0 > index)
    {
        return NULL;
    }

    order_ptr = SCI_ALLOC(sizeof(MMISMS_ORDER_INFO_T));
    if(!order_ptr)
    {
        return NULL;
    }
    SCI_MEMSET(order_ptr,0,sizeof(MMISMS_ORDER_INFO_T));

    order_ptr->msg_type = MMISMS_TYPE_MAIL;
    //subject
    subject_len = MMIAPICOM_Wstrlen(fsmFldrData.masterTable->msg[index].truncSubject);
    subject_len = MIN(subject_len, MMIMESSAGE_DIAPLAY_LEN);
    order_ptr->display_content.content_len = subject_len;
    MMIAPICOM_Wstrncpy(order_ptr->display_content.content,
                       fsmFldrData.masterTable->msg[index].truncSubject,
                       subject_len);
    //mo_mt_type
    switch(fsmFldrData.masterTable->msg[index].folderId)
    {
        case MAIL_INBOX:
            if(fsmFldrData.masterTable->msg[index].flagValues.readFlag)
            {
                order_ptr->mo_mt_type = MMISMS_MT_HAVE_READ; 
            }
            else
            {
                order_ptr->mo_mt_type = MMISMS_MT_TO_BE_READ;
            }
            break;
        case MAIL_OUTBOX:
            order_ptr->mo_mt_type = MMISMS_MO_SEND_FAIL;
            break;
        case MAIL_SENT:
            order_ptr->mo_mt_type = MMISMS_MO_SEND_SUCC;
            break;
        case MAIL_DRAFTS:
            order_ptr->mo_mt_type = MMISMS_MO_DRAFT;
            break;
        default:
            break;
    }

    //record_id
    order_ptr->record_id = fsmFldrData.masterTable->msg[index].msgId | MAIL_RECODE_INDEX;

    //sender
    sender_len = strlen(fsmFldrData.masterTable->msg[index].truncFrom);
    sender_len = MIN(sender_len,MMISMS_NUMBER_MAX_LEN);
    strncpy((char*)(order_ptr->sender),fsmFldrData.masterTable->msg[index].truncFrom,sender_len);

    //time
    if(MAIL_INBOX == fsmFldrData.masterTable->msg[index].folderId)
    {
        order_ptr->time = fsmFldrData.masterTable->msg[index].receivedTime;
    }
    else if(MAIL_DRAFTS == fsmFldrData.masterTable->msg[index].folderId)
    {
        order_ptr->time = fsmFldrData.masterTable->msg[index].modifiedTime;
    }
    else
    {
        order_ptr->time = fsmFldrData.masterTable->msg[index].sentTime;
    }

    order_ptr->is_lock = fsmFldrData.masterTable->msg[index].flagValues.isLockFlag;
    if(fsmFldrData.masterTable->msg[index].flagValues.isSecurityFlag)
    {
        order_ptr->folder_type = MMISMS_FOLDER_SECURITY;
    }
    else
    {
        order_ptr->folder_type = MMISMS_FOLDER_NORMAL;
    }
    //order_ptr.is_security = FALSE;
    order_ptr->next = NULL;
    
    return order_ptr;
#else
    return NULL;
#endif
}

/*==========================================================
 * Function     : MMIAPIMMS_MoveSecurityMMS
 * Author	    :       
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
PUBLIC BOOLEAN mail_MoveMailToSecurityBox(uint32 record_id, BOOLEAN is_to_security)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support 
    int msg_id = 0;
    MMISMS_ORDER_INFO_T *mail_list_info_ptr = PNULL;

    msg_id = mail_getMsgIdByRecordId((int)record_id);
    mail_list_info_ptr = Mail_GetListItemByMsgId(msg_id);

    if (PNULL == mail_list_info_ptr)
    {
        return FALSE;
    }

    if (is_to_security)//form box to security box
    {
        if (MMISMS_FOLDER_NORMAL == mail_list_info_ptr->folder_type)
        {
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MAIL, record_id);
            mail_list_info_ptr->folder_type = MMISMS_FOLDER_SECURITY;
            MMIAPISMS_InsertOneMsgToOrder(mail_list_info_ptr);
            mail_SetMailSecurityFlag(msg_id,TRUE);
            mail_SaveMailListInfo();
        }
    }
    else//from security box to box
    {
        if (MMISMS_FOLDER_SECURITY == mail_list_info_ptr->folder_type)
        {
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MAIL, record_id);
            mail_list_info_ptr->folder_type = MMISMS_FOLDER_NORMAL;
            MMIAPISMS_InsertOneMsgToOrder(mail_list_info_ptr);
            mail_SetMailSecurityFlag(msg_id,FALSE);
            mail_SaveMailListInfo();
        }
    }

    SCI_FREE(mail_list_info_ptr);
#endif    
    return TRUE;
}

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC BOOLEAN mail_MoveMailToSecurityBoxEx(uint32 record_id)
 * decr		: Resolve MS00225437
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  1/26/2011 
 ==========================================================*/
PUBLIC BOOLEAN mail_MoveMailToSecurityBoxEx(uint32 record_id)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support 
    if(MMISMS_FOLDER_SECURITY == g_mmisms_global.folder_type)
    {
        int msg_id = 0;
        MMISMS_ORDER_INFO_T *mail_list_info_ptr = PNULL;

        msg_id = mail_getMsgIdByRecordId((int)(record_id | MAIL_RECODE_INDEX));

        mail_list_info_ptr = Mail_GetListItemByMsgId(msg_id);

        if (PNULL == mail_list_info_ptr)
        {
            return FALSE;
        }
        
        if (MMISMS_FOLDER_NORMAL == mail_list_info_ptr->folder_type)
        {
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MAIL, (int)(record_id | MAIL_RECODE_INDEX));
            mail_list_info_ptr->folder_type = MMISMS_FOLDER_SECURITY;
            MMIAPISMS_InsertOneMsgToOrder(mail_list_info_ptr);
            mail_SetMailSecurityFlag(msg_id,TRUE);
            mail_SaveMailListInfo();
            //COMM_TRACE:"mail_MoveMailToSecurityBoxEx!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1320_112_2_18_2_47_40_31,(uint8*)"");
        }

        SCI_FREE(mail_list_info_ptr);
    }
#endif    
    return TRUE;
}

/*==========================================================
 * Function     : mail_delSMSMessageListById
 * Author	    :       
 * Description : 删除三合一信息列表
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/19/2010 
 ==========================================================*/
void mail_delSMSMessageListById(int msg_id)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    int record_id = 0;

    record_id = msg_id | (int)MAIL_RECODE_INDEX;
    //COMM_TRACE:"mail_delSMSMessageListById:msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1343_112_2_18_2_47_40_32,(uint8*)"d",msg_id);
    //COMM_TRACE:"mail_delSMSMessageListById:record_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAIL_EXPORT_1344_112_2_18_2_47_40_33,(uint8*)"d",record_id);
    
    MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MAIL,(int32)record_id);
#endif
}

/*==========================================================
 * Function     : mail_UpdateSMSMsgStateByMsgId
 * Author	    :       
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
PUBLIC BOOLEAN mail_UpdateSMSMsgStateByMsgId(comm_uint16 msg_id)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    MMISMS_ORDER_INFO_T *update_node = NULL;

    update_node = Mail_GetListItemByMsgId(msg_id);

    if(update_node)
    {
        MMIAPISMS_UpdateMsgStatusInOrder(update_node);
    }
    SCI_FREE(update_node);
#endif

    return TRUE;
}

/*==========================================================
 * Function     : mail_UpdateSMSMsgStateByMsgId
 * Author	    :       
 * Description : 
 * Parameter	: type          parameter       Description
 * Return	    : Void
 * Effect	    : NO
 * History	: created  8/16/2010 
 ==========================================================*/
PUBLIC void mail_switchAccount(void)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    int i = 0;
    comm_uint16 subject_len= 0;
    comm_uint16 sender_len = 0;
    MMISMS_ORDER_INFO_T order_ptr = {0};

    MMIAPISMS_DeleteAllMailMsg();

    for(i = 0; i < fsmFldrData.masterTable->numOfMsgs; i++)
    {
        if(mail_GetDefaultAccount() == fsmFldrData.masterTable->msg[i].accountID)
        {
  
          SCI_MEMSET(&order_ptr,0,sizeof(MMISMS_ORDER_INFO_T));

          order_ptr.msg_type = MMISMS_TYPE_MAIL;
  
          //subject
          subject_len = MMIAPICOM_Wstrlen(fsmFldrData.masterTable->msg[i].truncSubject);
          subject_len = MIN(subject_len, MMIMESSAGE_DIAPLAY_LEN);
          order_ptr.display_content.content_len = subject_len;
  
          MMIAPICOM_Wstrncpy(order_ptr.display_content.content,
  
                             fsmFldrData.masterTable->msg[i].truncSubject,
  
                             subject_len);
  
          //mo_mt_type
          switch(fsmFldrData.masterTable->msg[i].folderId)
          {
              case MAIL_INBOX:
                  if(fsmFldrData.masterTable->msg[i].flagValues.readFlag)
                  {  
                      order_ptr.mo_mt_type = MMISMS_MT_HAVE_READ;   
                  }
                  else
                  {  
                      order_ptr.mo_mt_type = MMISMS_MT_TO_BE_READ;  
                  }
                  break;
  
              case MAIL_OUTBOX:
                  order_ptr.mo_mt_type = MMISMS_MO_SEND_FAIL;
                  break;
  
              case MAIL_SENT:
                  order_ptr.mo_mt_type = MMISMS_MO_SEND_SUCC;
                  break;
  
              case MAIL_DRAFTS:
                  order_ptr.mo_mt_type = MMISMS_MO_DRAFT;
                  break;
  
              default:
  
                  break;
  
          }
  
          //record_id
          order_ptr.record_id = fsmFldrData.masterTable->msg[i].msgId | MAIL_RECODE_INDEX;
  
          //sender
          sender_len = strlen(fsmFldrData.masterTable->msg[i].truncFrom);
          sender_len = MIN(sender_len,MMISMS_NUMBER_MAX_LEN);
          strncpy((char*)(order_ptr.sender),fsmFldrData.masterTable->msg[i].truncFrom,sender_len);
  
          //time
          if(MAIL_INBOX == fsmFldrData.masterTable->msg[i].folderId)
          {
              order_ptr.time = fsmFldrData.masterTable->msg[i].receivedTime;
          }
          else if(MAIL_DRAFTS == fsmFldrData.masterTable->msg[i].folderId)
  
          {
              order_ptr.time = fsmFldrData.masterTable->msg[i].modifiedTime;
          }
          else
          {
              order_ptr.time = fsmFldrData.masterTable->msg[i].sentTime;
          }
    
          order_ptr.is_lock = fsmFldrData.masterTable->msg[i].flagValues.isLockFlag;
  
          if(fsmFldrData.masterTable->msg[i].flagValues.isSecurityFlag)
          {
              order_ptr.folder_type = MMISMS_FOLDER_SECURITY;
          }
          else
  
          {
              order_ptr.folder_type = MMISMS_FOLDER_NORMAL;
          }
  
          //order_ptr.is_security = FALSE;
          order_ptr.next = NULL;
          MMIAPISMS_InsertOneMsgToOrder(&order_ptr);
        }
  } 
#endif
}

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC void  mail_SetFldrMailDelFlag(comm_uint32 msgId, comm_boolean value)
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  1/7/2011 
 ==========================================================*/
PUBLIC void  mail_SetFldrMailDelFlag(comm_uint32 msgId, comm_boolean value)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    int i;

    i = getIndexFromMasterTable(msgId);
    if(0 > i)
    {
        return;
    }

    if(value)
    {
        if(mail_GetMailLockFlag(msgId) || mail_GetMailSendingFlag(msgId))
        {
            fsmFldrData.masterTable->msg[i].flagValues.deleteFlag= FALSE;
        }
        else
        {
            fsmFldrData.masterTable->msg[i].flagValues.deleteFlag= TRUE;
        }
    }
    else
    {
        fsmFldrData.masterTable->msg[i].flagValues.deleteFlag= FALSE;
    }
#else
    int i;

    i = getIndexFromMasterTable(msgId);
    if(0 > i)
    {
        return;
    }

    fsmFldrData.masterTable->msg[i].flagValues.deleteFlag= value;
#endif
    return;
}

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC void  mail_SetCurMailLockFlag(BOOLEAN value)
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  1/7/2011 
 ==========================================================*/
PUBLIC void  mail_SetCurMailLockFlag(BOOLEAN value)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    g_cur_mail_is_locked = value;
#endif
    return;
}

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC BOOLEAN mail_GetCurMailLockFlag(void)
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  1/7/2011 
 ==========================================================*/
PUBLIC BOOLEAN mail_GetCurMailLockFlag(void)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    return g_cur_mail_is_locked;
#else
    return FALSE;
#endif
}

/*==========================================================
 * author		: liangshun      
 * function 	: void mail_SetMailLockFlagEx(int j,BOOLEAN is_locked)
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  1/7/2011 
 ==========================================================*/
PUBLIC void mail_SetMailLockFlagEx(int j, BOOLEAN is_locked)
{
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
    fsmFldrData.masterTable->msg[j].flagValues.isLockFlag = is_locked;
#endif
    return;
}
