/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_retr.h
 *	Author		: sunguochen    
 *	Description	: 接收处理
 *	Vesion		: 1.0        
 *	Date			: 2010-4-14  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-14      1.0           初步编写完成
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
 * Description : 接收初始化
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetrMgr_init(void);



/*==========================================================
 * Function 	: mailRetrMgr_terminate
 * Author		: sunguochen      
 * Description : 终止接收
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetrMgr_terminate(MailTerminateRsp* rspFn, void* userData, MailTerminateMode termMode);



/*==========================================================
 * Function 	: mailRetr_create
 * Author		: sunguochen      
 * Description : 创建接收数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
MailRetr* mailRetr_create(void);



/*==========================================================
 * Function 	: mailRetr_free
 * Author		: sunguochen      
 * Description : 释放接收数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetr_free(MailRetr* retrOp);



/*==========================================================
 * Function 	: mailRetr_cancel
 * Author		: sunguochen      
 * Description : 取消接收
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetr_cancel(MailRetr* retrOp);



/*==========================================================
 * Function 	: mailRetr_RetrievingRsp
 * Author		: sunguochen      
 * Description : 接收邮件-继续
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetr_RetrievingRsp(mail_retval result, MailServerMsgIDsList* msgIds, void* userData);



/*==========================================================
 * Function 	: mail_RetrieveMsg
 * Author		: sunguochen      
 * Description : 开始获取邮件正文
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
 * Description : 获得接收模式
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/6/2010 
 ==========================================================*/
RETRIEVE_MODE_E mail_getRetrMode(void* userData);



/*==========================================================
 * Function 	: mailRetr_closePopSession
 * Author		: sunguochen      
 * Description : 关闭下载会话
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailRetr_closePopSession(void* retrOp, comm_boolean urgent);


#endif 
