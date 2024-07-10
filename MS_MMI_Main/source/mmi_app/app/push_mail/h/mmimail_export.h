/***************************************************************************
** File Name:      mmimail_export.h                                               *
** Author:         liyuanyuan                                              *
** Date:           2010/05/07                                              *
** Copyright:      2010 Hisense, Incorporated. All Rights Reserved.        *
** Description:    This file is used to declare common variables and       *
**                 functions.                                              *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2010/05/07     yuanl            Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIMAIL_H_    
#define  _MMIMAIL_H_   

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
#include "mail_message.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**----------------------------------------------------------------------*
 **                         MACRO DEFINITION                             *
 **----------------------------------------------------------------------*/


/**----------------------------------------------------------------------*
 **                         TYPE AND STRUCT                              *
 **----------------------------------------------------------------------*/
//邮箱启动方式
typedef enum
{
    MAIL_ENTER_NULL,
    MAIL_ENTER_MAINMENU,    //主菜单启动
    MAIL_ENTER_SMS,         //短信中启动
    MAIL_ENTER_INBOX,       //进入收件箱界面
    MAIL_ENTER_NEWMAIL,     //进入新建界面
    MAIL_ENTER_REPLYMAIL,   //进入回复界面
    MAIL_ENTER_FORWARDMAIL, //进入转发编辑界面
    MAIL_ENTER_EDITMAIL,    //进入编辑原始邮件界面
    MAIL_ENTER_SETTINGWIN,  //进入设置界面
    MAIL_ENTER_READWIN,     //进入邮件阅读界面
    MAIL_ENTER_MAX
}MAIL_ENTER_TYPE_E;

//邮箱类型
typedef enum
{
    MMIMAIL_BOX_NONE,
    MMIMAIL_INBOX,      //收件箱
    MMIMAIL_OUTBOX,     //发件箱
    MMIMAIL_SENTBOX,    //已发件箱
    MMIMAIL_DRAFTBOX,   //草稿箱
    MMIMAIL_BOX_MAX
}MMIMAIL_BOX_TYPE_E;

//编辑邮件窗口打开类型
typedef enum
{
    MMIMAIL_OPEN_TYPE_DEFAULT,
    MMIMAIL_OPEN_TYPE_NEW_MAIL,         //新建邮件
    MMIMAIL_OPEN_TYPE_ORIGINAL,         //编辑原始邮件
    MMIMAIL_OPEN_TYPE_REPLY,            //回复邮件
    MMIMAIL_OPEN_TYPE_REPLY_ALL,        //全部回复
    MMIMAIL_OPEN_TYPE_FORWARD_CONTENT,  //转发正文
    MMIMAIL_OPEN_TYPE_FORWARD_ALL,      //转发全部
    MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ACC,  //服务器转发附件
    MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ALL   //服务器转发全部
}MMIMAIL_OPEN_EDIT_WIN_TYPE_E;

//邮件操作相关错误码
typedef enum
{
    MMIMAIL_NO_ERROR = 0,     
    MMIMAIL_NO_OPERATION,
    MMIMAIL_ERROR_FATAL,
    MMIMAIL_ERROR_FAIL,
    MMIMAIL_ERROR_NOT_INIT,
    MMIMAIL_ERROR_UDISK_USING, 
    MMIMAIL_ERROR_CC_IN_CALL,
    MMIMAIL_ERROR_CMMB_RUNNING,
    MMIMAIL_ERROR_EDITING,
    MMIMAIL_ERROR_SENDRECVING,
    MMIMAIL_ERROR_NO_CONTENT, 
    MMIMAIL_ERROR_NO_MEMORY, 
    MMIMAIL_ERROR_NO_SPACE, 
    MMIMAIL_ERROR_INVALID_DATA,
    MMIMAIL_ERROR_INVALID_SIZE,
    MMIMAIL_ERROR_CONFLICT,    
    MMIMAIL_ERROR_MAX_SLIDE,
    MMIMAIL_ERROR_NOT_SUPPORT,
    MMIMAIL_ERROR_SFS_ERROR, 
    MMIMAIL_ERROR_SIM_ERROR,     
    MMIMAIL_ERROR_ENCODE_ERROR,     
    MMIMAIL_ERROR_SETTING_ERROR,
    MMIMAIL_ERROR_INVALID_URL,     
    MMIMAIL_ERROR_PUSH_NOT_EXIST,    
    MMIMAIL_ERROR_CANCEL, 
    MMIMAIL_ERROR_NETLINK_FAILED,   
    MMIMAIL_ERROR_NETLINK_DISCONNECT,  
    MMIMAIL_ERROR_GATEWAY_CONNECT_FAILED, 
    MMIMAIL_ERROR_NETCONNECT_FAILED,   
    MMIMAIL_ERROR_INVALIDPERIOD,   
    MMIMAIL_ERROR_SERVICE_DENIED,  
    MMIMAIL_ERROR_FORMAT_CORRUPT,   
    MMIMAIL_ERROR_ADDRESS_UNRESOLVED,   
    MMIMAIL_ERROR_MESSAGE_NOT_FOUND, 
    MMIMAIL_ERROR_SERVER_BUSY,
    MMIMAIL_ERROR_NOT_ACCEPTED,
    MMIMAIL_ERROR_UNSUPORTED_MESSAGE, 
    MMIMAIL_ERROR_RECV_NEED_AUTH,
    MMIMAIL_ERROR_RECV_ERROR_DATA,
    MMIMAIL_ERROR_MAX  
}MMIMAIL_ERROR_E;

/**---------------------------------------------------------------------------*
 **                         FUNCTION DECLARATION                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : Pushmail初始化
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_Init(void);

/*****************************************************************************/
//	Description : 模块初始化
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_InitModule(void);

/*****************************************************************************/
// 	Description : Pushmail 入口
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_Enter(MAIL_ENTER_TYPE_E   enter_type, DPARAM param);

/*****************************************************************************/
//	Description : 退出手机邮箱
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_Exit(void);

/*****************************************************************************/
// 	Description : 设置资费信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_SetChargeInfo(MMI_STRING_T *charge_info);

/*****************************************************************************/
// 	Description : 获取帐号信息回调
//	Global resource dependence : none
//  Author:yuanl
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_GetAccountCallback(
                                                    BOOLEAN is_open, //当前卡是否开通了139邮箱
                                                    BOOLEAN is_white_sim //当前卡是否是白卡(即欠费卡)
                                                    );

/*****************************************************************************/
//  Description : 如果没有新邮件了则关闭新邮件提示窗口
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_CloseNewMailPromptWin(void);

/*****************************************************************************/
//  Description : 是否有未读邮件
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_IsHaveUnreadMail(void);

/*****************************************************************************/
//  Description : 邮件是否满
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_IsMailFull(void);

/*****************************************************************************/
//  Description : 设置短信模块准备就绪
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_SetSmsOk(BOOLEAN is_sms_ok);

/*****************************************************************************/
//	Description : 打开邮件编辑窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_OpenEditWin(
                                                MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type, 
                                                MAIL_MESSAGE_T *whole_mail_ptr
                                               );

/*****************************************************************************/
//	Description : 发送邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_SendMail(MAIL_MESSAGE_T *whole_mail_ptr);

/*****************************************************************************/
//	Description : 发送指定id的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_SendMailById(uint32 record_id);

/*****************************************************************************/
//	Description : 取消发送指定id的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_CancelSendById(uint32 record_id);

/*****************************************************************************/
//  Description : 从文件调用邮件编辑窗口
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_EditMailFromFile(
                                        wchar   *full_path_name, //全路径
                                        uint16  full_path_len    //全路径长度
                                        );

/*****************************************************************************/
//	Description : 读取指定邮件，开始解码过程
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_ReadMail(uint32 record_id);

/*****************************************************************************/
//	Description : 打开邮件阅读窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_OpenReadWin(uint32 record_id);

/*****************************************************************************/
//	Description : 邮件应用是否处于最上层
//	Global resource dependence : none
//	Author:jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_IsActive(void);

/*****************************************************************************/
//  Description : insert string to sms edit
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_InsertMessage(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             );

#ifdef   __cplusplus
    }
#endif
#endif
