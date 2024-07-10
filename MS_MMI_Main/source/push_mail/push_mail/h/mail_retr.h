/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_retr.h
 *	Author		: sunguochen    
 *	Description	: ���մ���
 *	Vesion		: 1.0        
 *	Date			: 2010-4-14  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-14      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/


#ifndef _MAIL_RETR_H_
#define _MAIL_RETR_H_


#include "mail_main.h"
#include "mail_sig.h"
#include "mail_type.h"

#define RETRIEVE_PARTYNO "0"


typedef enum 
{
   MSGBOX_SIG_CREATE_INFO_FILE_AT_RETR_RSP = MAIL_RETRIEVE_SIGNAL_BASE,
   RETRIEVE_SIG_REGISTRY_RESPONSE,
   RETRIEVE_SIG_CANCEL_RETRIEVE,
   RETRIEVE_SIG_CANCEL_SYNC,
   RETR_SIG_GET_SYSTEM_EVENT_RSP,
   RETR_SIG_GET_MESSAGE_INFO_RSP
   
  } RetrieveSignalId;




typedef struct MailRetrImp MailRetr;

typedef void MailRetrRetrieveRsp(MailResult result, comm_uint32 msgSize, void* userData);




/*==========================================================
 * Function 	: mailRetrMgr_init
 * Author		: sunguochen      
 * Description : ���ճ�ʼ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetrMgr_init(void);



/*==========================================================
 * Function 	: mailRetrMgr_terminate
 * Author		: sunguochen      
 * Description : ��ֹ����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetrMgr_terminate(MailTerminateRsp* rspFn, void* userData, MailTerminateMode termMode);



/*==========================================================
 * Function 	: mailRetr_create
 * Author		: sunguochen      
 * Description : ������������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
MailRetr* mailRetr_create(void);



/*==========================================================
 * Function 	: mailRetr_free
 * Author		: sunguochen      
 * Description : �ͷŽ�������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetr_free(MailRetr* retrOp);



/*==========================================================
 * Function 	: mailRetr_cancel
 * Author		: sunguochen      
 * Description : ȡ������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetr_cancel(MailRetr* retrOp);



/*==========================================================
 * Function 	: mailRetr_RetrievingRsp
 * Author		: sunguochen      
 * Description : �����ʼ�-����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetr_RetrievingRsp(mail_retval result, MailServerMsgIDsList* msgIds, void* userData);



/*==========================================================
 * Function 	: mail_RetrieveMsg
 * Author		: sunguochen      
 * Description : ��ʼ��ȡ�ʼ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mail_RetrieveMsg(comm_uint16 accountid, void * prog_func, 
                                        RETRIEVE_MODE_E mode, void*  para);



/*==========================================================
 * Function 	: mail_getRetrMode
 * Author		: sunguochen      
 * Description : ��ý���ģʽ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/6/2010 
 ==========================================================*/
RETRIEVE_MODE_E mail_getRetrMode(void* userData);



/*==========================================================
 * Function 	: mailRetr_closePopSession
 * Author		: sunguochen      
 * Description : �ر����ػỰ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetr_closePopSession(void* retrOp, comm_boolean urgent);


#endif 
