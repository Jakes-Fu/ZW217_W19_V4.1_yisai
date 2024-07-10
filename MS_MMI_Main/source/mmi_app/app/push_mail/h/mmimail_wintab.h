/***************************************************************************
** File Name:      mmimail_wintab.h                                        *
** Author:         liyuanyuan                                              *
** Date:           2010/05/07                                              *
** Copyright:      2010 Hisense, Incorporated. All Rights Reserved.        *
** Description:    This file is used to define module internal relative    *
**                 contents.                                               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2010/05/07     yuanl            Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIMAIL_WINTAB_H_    
#define  _MMIMAIL_WINTAB_H_   

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "mmimail_internal.h"
#include "mmipdp_export.h"

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

/**---------------------------------------------------------------------------*
 **                         FUNCTION DECLARATION                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : 邮件是否准备就绪
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsReady( void );

/*****************************************************************************/
//	Description : 获取邮箱启动类型
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MAIL_ENTER_TYPE_E MMIMAIL_GetOpenType( void );

/*****************************************************************************/
//	Description : 获取邮箱启动参数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC DPARAM MMIMAIL_GetOpenParam( void );

/*****************************************************************************/
// 	Description : 获取默认帐号信息
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_GetDefAccount(MMIMAIL_ACCOUNT_INFO_T *account_info);

/*****************************************************************************/
// 	Description : 获取当前默认帐号ID
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMAIL_GetDefAccountId(void);

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC MMIMAIL_ACCOUNT_INFO_T* MMIMAIL_GetDefAccountInfo(void)
 * decr		: 获取指向当前默认账户信息的结构指针
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/16/2011 
 ==========================================================*/
PUBLIC MMIMAIL_ACCOUNT_INFO_T* MMIMAIL_GetDefAccountInfo(void);

/*****************************************************************************/
// 	Description : 获取当前默认帐号邮箱信息指针
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_BOX_INFO_T * MMIMAIL_GetDefAccountMailboxInfo(void);

/*****************************************************************************/
// 	Description : 获取当前正在阅读的邮件
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MAIL_MESSAGE_T * MMIMAIL_GetCurDispMail(void);

/*****************************************************************************/
// 	Description : 设置当前正在阅读的邮件
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurDispMail(MAIL_MESSAGE_T *mail_ptr);

/*****************************************************************************/
// 	Description : 获取当前所在邮箱
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_BOX_TYPE_E MMIMAIL_GetCurBoxType(void);

/*****************************************************************************/
// 	Description : 设置当前所在邮箱
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurBoxType(MMIMAIL_BOX_TYPE_E box_type);

/*****************************************************************************/
// 	Description : 获取当前浏览邮件的record id
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMAIL_GetCurDispMailRecordId(void);

/*****************************************************************************/
// 	Description : 设置当前浏览邮件的record id
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurDispMailRecordId(uint32 record_id);

/*****************************************************************************/
// 	Description : 获取当前打开编辑窗口的类型
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_OPEN_EDIT_WIN_TYPE_E MMIMAIL_GetOpenEditwinType(void);

/*****************************************************************************/
// 	Description : 设置当前打开编辑窗口的类型
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetOpenEditwinType(MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type);

/*****************************************************************************/
//	Description : 获取当前正在操作的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_MAIL_SUMMERY_T *MMIMAIL_GetCurOperationMail(void);

/*****************************************************************************/
//	Description : 设置当前正在操作的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurOperationMail(MMIMAIL_MAIL_SUMMERY_T * mail_ptr);

/*****************************************************************************/
//	Description : 获取当前邮件是否是通过id发送的
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsSendingById(void);

/*****************************************************************************/
//	Description : 设置当前邮件是通过id发送的
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetSendingById(BOOLEAN is_sendby_id);

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC void MMIMAIL_SetDelAccountOperFlag(BOOLEAN is_del_account_oper_by_bomb)
 * decr		: 设置通过炸弹消息删除账户操作的标志位
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/18/2011 
 ==========================================================*/
PUBLIC void MMIMAIL_SetDelAccountOperFlag(BOOLEAN is_del_account_oper_by_bomb);

/*****************************************************************************/
//	Description : 获取当前操作邮件对应在列表中的index
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMAIL_GetCurOperationMailIndex(MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr);

/*****************************************************************************/
//	Description : 设置当前正在操作的邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurOperationMailByIndex(MMIMAIL_BOX_TYPE_E box_type, uint32 index);

/*****************************************************************************/
// 	Description : 获取邮件状态
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_STATE_E MMIMAIL_GetMailState(MMIMAIL_MAIL_SUMMERY_T *mail_ptr);

/*****************************************************************************/
// 	Description : 设置邮件状态
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetMailState(
                                        MMIMAIL_MAIL_SUMMERY_T *mail_ptr, 
                                        MMIMAIL_STATE_E state
                                      );
                                   
/*****************************************************************************/
// 	Description : 是否需要排序
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsNeedSort(void);

/*****************************************************************************/
// 	Description : 设置是否需要排序
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetNeedSort(BOOLEAN is_need_sort);

/*****************************************************************************/
//	Description : 打开询问是否删除窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_OpenDelteQueryWin(void);

/*****************************************************************************/
//	Description : Query delete all
//	Global resource dependence : none
//	Author:Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_OpenDelteAllQueryWin(void);

/*****************************************************************************/
//	Description : 增加空列表
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_AppendEmptyStr2List(
                             MMI_CTRL_ID_T    ctrl_id,         //IN:
                             MMI_TEXT_ID_T    empty_txt_id,    //IN: 
                             MMI_TEXT_ID_T    left_softkey_id  //IN:
                             );

/*****************************************************************************/
// 	Description : 减少默认帐号收件箱未读邮件数目
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_DecreaseInboxBoxUnreadNum(void);


/*****************************************************************************/
//	Description : 打开消息提示框
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_OpenMsgBox( 
                       MMI_TEXT_ID_T     text_id,    //IN:
                       MMI_IMAGE_ID_T    image_id    //IN:
                       );

/*****************************************************************************/
//	Description : 获取与record id对应的邮件指针
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_MAIL_SUMMERY_T  *MMIMAIL_GetMailPtrByRecordId(
                                                            uint32 record_id
                                                           );

/*****************************************************************************/
//  Description : 判断短信模块是否启动
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsSmsOpen(void);

/*==========================================================
 * Function 	: MMIAPIMail_HandleTimerForRing
 * Author		: sunguochen      
 * Description : 播放mail铃声定时时间到处理函数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/10/2010 
 ==========================================================*/
PUBLIC void MMIAPIMail_HandleTimerForRing(uint8  timer_id, uint32 param);


/*****************************************************************************/
//  Description : 拷贝附件到本地目录
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyAccToFolder(
                                  wchar     *acc_full_path, //附件的全路径，包含文件名
                                  uint16    full_path_len   //全路径长度
                                  );

/*****************************************************************************/
//  Description : 释放拷贝附件时分配的临时空间
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_FreeAccTmpSpace(void);

/*****************************************************************************/
//	Description : 设置当前按OK键的操作类型
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void SetHandleOkMenuOperation(
                                          MMIMAIL_OPERATION_T prev_operation
                                          );

/*****************************************************************************/
//	Description : 获取之前按OK键的操作类型
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_OPERATION_T GetHandleOkMenuOperation(void);

/*****************************************************************************/
//  Description : 激活PDP
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAILPDP_Active(uint32 account_id, MMIPDP_MSG_CALLBACK callback_func);

/*****************************************************************************/
//  Description : 去激活PDP
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAILPDP_Deactive(uint32 app_handler);

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC BOOLEAN MMIMAIL_CurMailIsLocked(void )
 * decr		: 判断当前邮件是否被锁定
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  12/10/2010 
 ==========================================================*/
PUBLIC BOOLEAN MMIMAIL_CurMailIsLocked(void );
PUBLIC BOOLEAN MMIMAIL_IsSendingCurMail(void );
#endif
                             
#ifdef   __cplusplus
    }
#endif
#endif


