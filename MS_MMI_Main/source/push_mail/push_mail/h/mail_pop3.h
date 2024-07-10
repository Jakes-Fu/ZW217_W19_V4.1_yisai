/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_pop3.h
 *	Author		: sunguochen    
 *	Description	: pop3 协议实现
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/



#ifndef _MAIL_POP3_H_
#define _MAIL_POP3_H_


#include "mail_ret.h"
#include "mail_type.h"
#include "mail_main.h"
#include "mail_sig.h"




typedef enum 
{
    POP3_SIG_TERMINATE = MAIL_POP3_SIGNAL_BASE,
    POP3_SIG_RETR_FILE_WRITE_RSP,
    POP3_SIG_ERET_FILE_WRITE_RSP,
    POP3_SIG_ERET_ATTACH_FILE_WRITE_RSP,
    POP3_SIG_SYNC_FILE_WRITE_RSP

} Pop3SignalId;


//
typedef enum _MAIL_ATT_TYPE_
{
    MAIL_ATT_SUPPORT,    
    MAIL_ATT_DOC,
    MAIL_ATT_PPT,
    MAIL_ATT_XLS,    
    MAIL_ATT_PDF,
    MAIL_ATT_RAR,
    MAIL_ATT_ZIP,
    MAIL_ATT_HTML    

}MAIL_ATT_TYPE;



typedef void* eAsyncOperationHandle;


typedef struct MailPopSessImp MailPopSess;


typedef void MailPopOperRsp(mail_retval result, void* userData);

typedef void MailPopNetworkErrorInd(mail_retval result, void* userData);

typedef void MailPopGetMsgIdsRsp(mail_retval result, MailServerMsgIDsList* msgIds, void* userData);

typedef void MailPopDeleteRsp(mail_retval result, MailMessageIdType sessionMsgId, void* userData);

typedef void MailPopSyncRsp(mail_retval result, MailMessageIdType msgId, comm_uint32 receivedBytes, void* userData);

typedef void MailPopProgressInd(MailProgressInfo  *fsmRetrProgress, void* userData);

typedef void MailPopTerminateRsp(void* userData);



/*==========================================================
 * Function 	: mailPop_init
 * Author		: sunguochen      
 * Description : pop3 初始化
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailPop_init(void);



/*==========================================================
 * Function 	: mailPop_terminate
 * Author		: sunguochen      
 * Description : pop 3 终止
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailPop_terminate(MailPopTerminateRsp* rspFn, void* userData, MailTerminateMode termMode);



/*==========================================================
 * Function 	: mailPop_createSession
 * Author		: sunguochen      
 * Description : 创建pop3 会话
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
MailPopSess* mailPop_createSession(int accountid, mail_retr_method_t *method);



/*==========================================================
 * Function 	: mailPop_freeSession
 * Author		: sunguochen      
 * Description : 释放pop3 会话
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailPop_freeSession(MailPopSess* session);



/*==========================================================
 * Function 	: mailPopSess_startSession
 * Author		: sunguochen      
 * Description : 创建一个pop3 进程
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
void mailPopSess_startSession(MailPopSess* session, 
                                                    MailPopOperRsp* rspFn, 
                                                    MailPopNetworkErrorInd* networkErrorInd,
                                                    void* userData);



/*==========================================================
 * Function 	: mailPopSess_cancel
 * Author		: sunguochen      
 * Description : 取消pop3 会话 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mailPopSess_cancel(MailPopSess* session);



/*==========================================================
 * Function 	: mailPopSess_getMsgIdsOnServ
 * Author		: sunguochen      
 * Description : 获取邮件的UID
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
void mailPopSess_getMsgIdsOnServ(MailPopSess* session, MailPopGetMsgIdsRsp* rspFn, void* userData);



/*==========================================================
 * Function 	: mailPopSess_getAllMsgSizes
 * Author		: sunguochen      
 * Description : 获取全部邮件及大小请求
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
void mailPopSess_getAllMsgSizes(MailPopSess* session, 
                               MailServerMsgIDsList* msgList,
                               MailPopOperRsp* rspFn, void* userData);



/*==========================================================
 * Function 	: mailPopSess_deleteMsg
 * Author		: sunguochen      
 * Description : 删除邮件请求
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
void mailPopSess_deleteMsg(MailPopSess* session, MailMessageIdType sessionMsgId, 
                          MailPopOperRsp* rspFn, void* userData);



#ifdef MAIL_SUPPORT_CMPOP

/*==========================================================
 * Function 	: mailPopSess_retrieveMsgByUidl
 * Author		: sunguochen      
 * Description : 获取邮件内容请求
                     sessionMsgId---邮件的UID
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
void mailPopSess_retrieveMsgByUidl(MailPopSess* session, 
                                                                       MailMessageIdType msgId,
                                                                       MailMessageIdType sessionMsgId,
                                                                       char* sessionMsgUidl,
                                                                       MailProgressInfo  *fsmRetrProgress,
                                                                       MailPopProgressInd* progressIndFn,
                                                                       MailPopOperRsp* rspFn, void* userData);



/*==========================================================
 * Function 	: mailPopSess_discMsg
 * Author		: sunguochen      
 * Description : 标记邮件为已读
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
comm_boolean mailPopSess_discMsg(MailPopSess* session, 
                                                     MailMessageIdType sessionMsgId, 
                                                     MailPopOperRsp* rspFn, 
                                                     void* userData);
#endif



/*==========================================================
 * Function 	: mailPopSess_retrieveMsg
 * Author		: sunguochen      
 * Description : 获取邮件内容请求
                     sessionMsgId---邮件的UID
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/13/2010 
 ==========================================================*/
void mailPopSess_retrieveMsg(MailPopSess* session, MailMessageIdType msgId,
                            MailMessageIdType sessionMsgId,
                            MailProgressInfo  *fsmRetrProgress,
                            MailPopProgressInd* progressIndFn,
                            MailPopOperRsp* rspFn, void* userData);



/*==========================================================
 * Function 	: mailPopSess_sync
 * Author		: sunguochen      
 * Description : 账户同步操作
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void mailPopSess_sync(MailPopSess* session, MailMessageIdType sessionMsgId,
                     comm_uint32 msgSizeOnServer,
                     comm_boolean  fullSync,
                     MailProgressInfo  *fsmRetrProgress,
                     MailPopProgressInd* progressIndFn,
                     MailPopSyncRsp* rspFn, void* userData);



/*==========================================================
 * Function 	: mailPopSess_endSession
 * Author		: sunguochen      
 * Description : 结束与服务器的交互
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/14/2010 
 ==========================================================*/
void mailPopSess_endSession(MailPopSess* session, comm_boolean urgent, MailPopOperRsp* rspFn, void* userData);



/*==========================================================
 * Function 	: findEndSequence
 * Author		: sunguochen      
 * Description : 确定队列是否完整
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
comm_boolean findEndSequence(MailPopSess* session);



/*==========================================================
 * Function 	: mailPopSession_isWritingFile
 * Author		: sunguochen      
 * Description : 是否在写文件操作
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
comm_boolean mailPopSession_isWritingFile(void * session);



/*==========================================================
 * Function 	: mail_NotifyUIError
 * Author		: sunguochen      
 * Description : 通知UI错误
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
void mail_NotifyUIError(void* session, int result);




#endif
