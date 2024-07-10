/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_export.h
 *	Author		: sunguochen    
 *	Description	: �ֻ���������ṩ�ӿ�
 *	Vesion		: 1.0        
 *	Date			: 2010-4-13  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-13      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _MAIL_EXPORT_H_
#define _MAIL_EXPORT_H_


#include "comm_platform.h"
#include "mail_setting.h"
#include "mail_network.h"

#ifdef PUSH_EMAIL_SUPPORT//Added by sunlu  2010.5.12
#include "mail_pushparse.h" 
#endif /*PUSH_EMAIL_SUPPORT */


//�ֻ������������ʽ
typedef enum{

    MAIL_START_NONE,
    MAIL_START_MAINMENU,          //���˵�    
    MAIL_START_INBOX,                //�ռ���
    MAIL_START_SEND_ATTACH,    //���͸���
    MAIL_START_SEND_ADDR,       //ͨ����ַ����
    MAIL_START_RETR,                 //�����ʼ�
    MAIL_START_MAX

}MAIL_START_MODE;




/*==========================================================
 * Function 	: pushmail_Start
 * Author		: sunguochen      
 * Description : ����email
 * Parameter	: type          parameter       Description
                       MAIL_START_MODE start_mode    ������ʽ
                       char* para                                 ������Ҫ�Ĳ���
 * Return		: Void
 * Effect		: NO
 * History	: created  4/29/2010 
 ==========================================================*/
void pushmail_Start(MAIL_START_MODE start_mode, char* para);



/*==========================================================
 * Function 	: mail_retrieve_test
 * Author		: sunguochen      
 * Description : ���Դ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/14/2010 
 ==========================================================*/
void mail_retrieve_test(void);



/*==========================================================
 * Function 	: mail_Send_test
 * Author		: sunguochen      
 * Description : ���Դ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/14/2010 
 ==========================================================*/
void mail_Send_test(void);



/*==========================================================
 * Function 	: mail_CoreInit
 * Author		: sunguochen      
 * Description : �ֻ����俪����ʼ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_CoreInit(void);



/*==========================================================
 * Function 	: mail_CoreInit
 * Author		: sunguochen      
 * Description : �ֻ�������ֹ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/15/2010 
 ==========================================================*/
void mail_CoreFinal(void);




/*==========================================================
 * Function 	: mail_DisableAccount
 * Author		: sunguochen      
 * Description : �˻���ͣʹ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_DisableAccount(comm_uint16 accountid);



/*==========================================================
 * Function 	: mail_EnableAccount
 * Author		: sunguochen      
 * Description : �˻��ָ�ʹ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_EnableAccount(comm_uint16 accountid);



/*==========================================================
 * Function 	: mail_retrievePushMsg
 * Author		: sunguochen      
 * Description : ��Ϣ֪ͨ��-�Զ������ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/14/2010 
 ==========================================================*/
void mail_retrievePushMsg(MMIMAIL_EMN_INFO_T* emn_info);



/*==========================================================
 * Function 	: mail_retrieveMsgManual
 * Author		: sunguochen      
 * Description : �ֶ������ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_retrieveMsgManual(int accountid);



/*==========================================================
 * Function 	: mail_retrieve_test
 * Author		: sunguochen      
 * Description : �ֶ����ո���
                      δ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
void mail_retrieveAttachmentManual(int accountid, char* uidl, int attachid);



/*==========================================================
 * Function 	: mail_RetrAttachStart
 * Author		: sunguochen      
 * Description : ���ո���
 * Parameter	: type          parameter       Description
                       int accountid,                                     �˻�id
                       char *uidl,                                         �ʼ�uidl
                       MAIL_MESSAGE_PART_T * attach_list,   ������Ϣ
                       int retr_num                                      ���ո���
 * Return		: Void
 * Effect		: NO
 * History	: created  5/22/2010 
 ==========================================================*/
void mail_RetrAttachStart(int accountid, char *uidl, MAIL_MESSAGE_PART_T * attach_list, int retr_num);



/*==========================================================
 * Function 	: mail_isDisableUdisk
 * Author		: sunguochen      
 * Description : �ж��Ƿ��������u��
                 ���������л����շ��ʼ�ʱ����������u��
                 ����д�ļ������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  7/19/2010 
 ==========================================================*/
BOOLEAN mail_isEnableUdisk(void);


/*==========================================================
 * Function 	: mail_msgIsMail
 * Author		: sunguochen      
 * Description : �Ƿ���push email
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/16/2010 
 ==========================================================*/
BOOLEAN mail_msgIsMail(int record_id);



/*==========================================================
 * Function 	: mail_getMsgIdByRecordId
 * Author		: sunguochen      
 * Description : ��sms��record idתΪmail��msg id
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/16/2010 
 ==========================================================*/
int mail_getMsgIdByRecordId(int record_id);



/*==========================================================
 * Function 	: mail_ReplyMailByRecordId
 * Author		: sunguochen      
 * Description : �ظ��ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/16/2010 
 ==========================================================*/
void mail_ReplyMailByRecordId(int record_id);


/*==========================================================
 * Function 	: mail_ForwardMailByRecordId
 * Author		: sunguochen      
 * Description : ת���ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/16/2010 
 ==========================================================*/
void mail_ForwardMailByRecordId(int record_id);



/*==========================================================
 * Function 	: mail_EditMailByRecordId
 * Author		: sunguochen      
 * Description : �༭�ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/16/2010 
 ==========================================================*/
void mail_EditMailByRecordId(int record_id);



/*==========================================================
 * Function 	: mail_ReadMailByRecordId
 * Author		: sunguochen      
 * Description : �Ķ��ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/18/2010 
 ==========================================================*/
void mail_ReadMailByRecordId(int record_id);


/*==========================================================
 * Function 	: mail_EditNewMail
 * Author		: sunguochen      
 * Description : �½��ʼ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/18/2010 
 ==========================================================*/
void mail_EditNewMail(void);


/*==========================================================
 * Function 	: mail_EnterMailSettings
 * Author		: sunguochen      
 * Description : ������������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/18/2010 
 ==========================================================*/
void mail_EnterMailSettings(void);


//following is #ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support

void MAIL_ListInit(void);
PUBLIC MMISMS_ORDER_INFO_T *Mail_GetListItemByMsgId(int32 msg_id);
BOOLEAN Mail_DelMailByMsgId(int record_id);
void mail_SaveMailListInfo(void);
BOOLEAN mail_SetMailLockFlag(int record_id,BOOLEAN is_locked);
BOOLEAN mail_SetMailSecurityFlag(int record_id,BOOLEAN is_security);
BOOLEAN mail_GetMailLockFlag(uint32 record_id);
BOOLEAN mail_GetMailSendingFlag(uint32 record_id);
BOOLEAN mail_GetMailSecurityFlag(uint32 record_id);
BOOLEAN mail_GetMailReadFlag(uint32 record_id);
void MAIL_InsertMsgToSMS(int msg_id);
void mail_delSMSMessageListById(int msg_id);
PUBLIC BOOLEAN mail_UpdateSMSMsgStateByMsgId(comm_uint16 msg_id);
PUBLIC BOOLEAN mail_MoveMailToSecurityBox(uint32 record_id, BOOLEAN is_to_security);
PUBLIC BOOLEAN mail_MoveMailToSecurityBoxEx(uint32 record_id);//added by liangshun 20110126 for MS00225437
PUBLIC void mail_switchAccount(void);
PUBLIC void  mail_SetFldrMailDelFlag(comm_uint32 msgId, comm_boolean value);
PUBLIC void  mail_SetCurMailLockFlag(BOOLEAN value);
PUBLIC BOOLEAN mail_GetCurMailLockFlag(void);
PUBLIC void mail_SetMailLockFlagEx(int j, BOOLEAN is_locked);

#endif
