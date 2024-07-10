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
//	Description : �ʼ��Ƿ�׼������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsReady( void );

/*****************************************************************************/
//	Description : ��ȡ������������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MAIL_ENTER_TYPE_E MMIMAIL_GetOpenType( void );

/*****************************************************************************/
//	Description : ��ȡ������������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC DPARAM MMIMAIL_GetOpenParam( void );

/*****************************************************************************/
// 	Description : ��ȡĬ���ʺ���Ϣ
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_GetDefAccount(MMIMAIL_ACCOUNT_INFO_T *account_info);

/*****************************************************************************/
// 	Description : ��ȡ��ǰĬ���ʺ�ID
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMAIL_GetDefAccountId(void);

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC MMIMAIL_ACCOUNT_INFO_T* MMIMAIL_GetDefAccountInfo(void)
 * decr		: ��ȡָ��ǰĬ���˻���Ϣ�Ľṹָ��
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/16/2011 
 ==========================================================*/
PUBLIC MMIMAIL_ACCOUNT_INFO_T* MMIMAIL_GetDefAccountInfo(void);

/*****************************************************************************/
// 	Description : ��ȡ��ǰĬ���ʺ�������Ϣָ��
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_BOX_INFO_T * MMIMAIL_GetDefAccountMailboxInfo(void);

/*****************************************************************************/
// 	Description : ��ȡ��ǰ�����Ķ����ʼ�
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MAIL_MESSAGE_T * MMIMAIL_GetCurDispMail(void);

/*****************************************************************************/
// 	Description : ���õ�ǰ�����Ķ����ʼ�
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurDispMail(MAIL_MESSAGE_T *mail_ptr);

/*****************************************************************************/
// 	Description : ��ȡ��ǰ��������
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_BOX_TYPE_E MMIMAIL_GetCurBoxType(void);

/*****************************************************************************/
// 	Description : ���õ�ǰ��������
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurBoxType(MMIMAIL_BOX_TYPE_E box_type);

/*****************************************************************************/
// 	Description : ��ȡ��ǰ����ʼ���record id
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMAIL_GetCurDispMailRecordId(void);

/*****************************************************************************/
// 	Description : ���õ�ǰ����ʼ���record id
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurDispMailRecordId(uint32 record_id);

/*****************************************************************************/
// 	Description : ��ȡ��ǰ�򿪱༭���ڵ�����
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_OPEN_EDIT_WIN_TYPE_E MMIMAIL_GetOpenEditwinType(void);

/*****************************************************************************/
// 	Description : ���õ�ǰ�򿪱༭���ڵ�����
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetOpenEditwinType(MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type);

/*****************************************************************************/
//	Description : ��ȡ��ǰ���ڲ������ʼ�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_MAIL_SUMMERY_T *MMIMAIL_GetCurOperationMail(void);

/*****************************************************************************/
//	Description : ���õ�ǰ���ڲ������ʼ�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurOperationMail(MMIMAIL_MAIL_SUMMERY_T * mail_ptr);

/*****************************************************************************/
//	Description : ��ȡ��ǰ�ʼ��Ƿ���ͨ��id���͵�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsSendingById(void);

/*****************************************************************************/
//	Description : ���õ�ǰ�ʼ���ͨ��id���͵�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetSendingById(BOOLEAN is_sendby_id);

/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC void MMIMAIL_SetDelAccountOperFlag(BOOLEAN is_del_account_oper_by_bomb)
 * decr		: ����ͨ��ը����Ϣɾ���˻������ı�־λ
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/18/2011 
 ==========================================================*/
PUBLIC void MMIMAIL_SetDelAccountOperFlag(BOOLEAN is_del_account_oper_by_bomb);

/*****************************************************************************/
//	Description : ��ȡ��ǰ�����ʼ���Ӧ���б��е�index
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMAIL_GetCurOperationMailIndex(MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr);

/*****************************************************************************/
//	Description : ���õ�ǰ���ڲ������ʼ�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetCurOperationMailByIndex(MMIMAIL_BOX_TYPE_E box_type, uint32 index);

/*****************************************************************************/
// 	Description : ��ȡ�ʼ�״̬
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_STATE_E MMIMAIL_GetMailState(MMIMAIL_MAIL_SUMMERY_T *mail_ptr);

/*****************************************************************************/
// 	Description : �����ʼ�״̬
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetMailState(
                                        MMIMAIL_MAIL_SUMMERY_T *mail_ptr, 
                                        MMIMAIL_STATE_E state
                                      );
                                   
/*****************************************************************************/
// 	Description : �Ƿ���Ҫ����
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsNeedSort(void);

/*****************************************************************************/
// 	Description : �����Ƿ���Ҫ����
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SetNeedSort(BOOLEAN is_need_sort);

/*****************************************************************************/
//	Description : ��ѯ���Ƿ�ɾ������
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
//	Description : ���ӿ��б�
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
// 	Description : ����Ĭ���ʺ��ռ���δ���ʼ���Ŀ
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_DecreaseInboxBoxUnreadNum(void);


/*****************************************************************************/
//	Description : ����Ϣ��ʾ��
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_OpenMsgBox( 
                       MMI_TEXT_ID_T     text_id,    //IN:
                       MMI_IMAGE_ID_T    image_id    //IN:
                       );

/*****************************************************************************/
//	Description : ��ȡ��record id��Ӧ���ʼ�ָ��
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_MAIL_SUMMERY_T  *MMIMAIL_GetMailPtrByRecordId(
                                                            uint32 record_id
                                                           );

/*****************************************************************************/
//  Description : �ж϶���ģ���Ƿ�����
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsSmsOpen(void);

/*==========================================================
 * Function 	: MMIAPIMail_HandleTimerForRing
 * Author		: sunguochen      
 * Description : ����mail������ʱʱ�䵽������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/10/2010 
 ==========================================================*/
PUBLIC void MMIAPIMail_HandleTimerForRing(uint8  timer_id, uint32 param);


/*****************************************************************************/
//  Description : ��������������Ŀ¼
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyAccToFolder(
                                  wchar     *acc_full_path, //������ȫ·���������ļ���
                                  uint16    full_path_len   //ȫ·������
                                  );

/*****************************************************************************/
//  Description : �ͷſ�������ʱ�������ʱ�ռ�
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_FreeAccTmpSpace(void);

/*****************************************************************************/
//	Description : ���õ�ǰ��OK���Ĳ�������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void SetHandleOkMenuOperation(
                                          MMIMAIL_OPERATION_T prev_operation
                                          );

/*****************************************************************************/
//	Description : ��ȡ֮ǰ��OK���Ĳ�������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMIMAIL_OPERATION_T GetHandleOkMenuOperation(void);

/*****************************************************************************/
//  Description : ����PDP
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAILPDP_Active(uint32 account_id, MMIPDP_MSG_CALLBACK callback_func);

/*****************************************************************************/
//  Description : ȥ����PDP
//  Global resource dependence :
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAILPDP_Deactive(uint32 app_handler);

#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
/*==========================================================
 * author		: liangshun      
 * function 	: PUBLIC BOOLEAN MMIMAIL_CurMailIsLocked(void )
 * decr		: �жϵ�ǰ�ʼ��Ƿ�����
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


