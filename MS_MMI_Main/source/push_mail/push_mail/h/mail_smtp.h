/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_smtp.h
 *	Author		: sunguochen    
 *	Description	: smtp protocol
 *	Vesion		: 1.0        
 *	Date			: 2010-4-27  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-27      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/


#ifndef _MAIL_SMTP_H_
#define _MAIL_SMTP_H_


#include "mail_main.h"
#include "mail_setting.h"
#include "mail_ret.h"
#include "mail_sig.h"



#define SMTP_CRLF               "\r\n"
#define SMTP_HELO               "HELO"
#define SMTP_EHLO               "EHLO"
#define SMTP_AUTH_CRAMMD5       "AUTH CRAM-MD5"
#define SMTP_AUTH_PLAIN         "AUTH PLAIN"
#define SMTP_AUTH_LOGIN         "AUTH LOGIN"
#define SMTP_AUTH               "AUTH"
#define SMTP_STARTTLS           "STARTTLS"
#define SMTP_DSN                "DSN"

#define SMTP_220                "220"
#define SMTP_235                "235"
#define SMTP_250                "250"
#define SMTP_251                "251"
#define SMTP_334                "334"
#define SMTP_354                "354"
#define SMTP_452                "452"
#define SMTP_501                "501"
#define SMTP_504                "504"
#define SMTP_530                "530"
#define SMTP_550                "550"
#define SMTP_551                "551"
#define SMTP_552                "552"
#define SMTP_553                "553"
#define SMTP_554                "554"
#define SMTP_571                "571"
#define SMTP_575                "575"
#define SMTP_QUIT               "QUIT"
#define SMTP_FROM               "MAIL FROM:<"
#define SMTP_RCPT               "RCPT TO:<"
#define SMTP_DATA               "DATA"
#define SMTP_NOTIFY             " NOTIFY=SUCCESS,FAILURE,DELAY"
#define SMTP_ORCPT              " ORCPT=rfc822;"
#define SMTP_ENVID              " ENVID="
#define SMTP_RET_HDRS           " RET=HDRS"

#ifdef MAIL_SUPPORT_CMSMTP//cmsmtp cmd
#define SMTP_EFWD               "EFWD"
#endif

#define SMTP_TERMINATION        "\r\n.\r\n"

#define SMTP_CMD_LEN 128

#define NO_VALUE                -1


typedef enum
{
    
    SMTP_SIG_START_SESSION = MAIL_SMTP_SIGNAL_BASE,
    SMTP_SIG_START_SESSION_RSP,
    SMTP_SIG_SOCKET_EVENT,
    SMTP_SIG_SEND_MAIL,
    SMTP_SIG_SEND_MAIL_RSP,
    SMTP_SIG_SEND_MAIL_NOTIFY_RSP,  
    SMTP_SIG_END_SESSION,
    SMTP_SIG_END_SESSION_RSP,
    SMTP_SIG_RECEIVE_DATA,
    SMTP_SIG_GET_MSG_RSP,
    SMTP_SIG_GET_MSG_INFO_RSP,
    SMTP_SIG_GET_MSG_CHUNK_RSP,
    SMTP_SIG_SEND_MORE_DATA,
    SMTP_SIG_SEND_MORE_CHUNKING_DATA,
    SMTP_SIG_CANCEL,
    SMTP_SIG_CANCEL_RSP,
    SMTP_SIG_TIMER_EXPIRED,     
    SMTP_SIG_NET_NOTIFY_SENT,
    SMTP_SIG_SEND_NEXT_MAIL,             //发送下一封邮件
    
    SMTP_SIG_NET_NOTIFY_CHUNKING_SENT 

} SmtpSignalId;





typedef enum
{
  SMTP_STATE_NOT_INITIALIZED,
  SMTP_STATE_INITIAL,
  SMTP_STATE_STARTING_SESSION,
  SMTP_STATE_SESSION_IN_PROGRESS,
  SMTP_STATE_HELO_SENT,
  SMTP_STATE_EHLO_SENT,
  SMTP_STATE_AUTH_CRAMMD5_SENT,
  SMTP_STATE_CRAMMD5_RESPONSE_SENT,
  SMTP_STATE_AUTH_LOGIN_SENT,
  SMTP_STATE_LOGIN_USERNAME_SENT,
  SMTP_STATE_LOGIN_PASSWORD_SENT,
  SMTP_STATE_AUTH_PLAIN_SENT,
  SMTP_STATE_PLAIN_MESSAGE_SENT,
  SMTP_STATE_FROM_SENT,
  SMTP_STATE_RECIPIENT_SENT,
  SMTP_STATE_SENDING_DATA,
  SMTP_STATE_GET_MSG_RSP,
  SMTP_STATE_DATA_SENT,
  SMTP_STATE_QUIT_SENT,
  SMTP_STATE_ENDING_SESSION,
  SMTP_STATE_SAME_STATE,
  SMTP_STATE_SESSION_IN_PROGRESS_SOCKET_CLOSED

} SmtpStatesId;




struct SmtpSendMailStruct
{
    comm_uint32     msgId;               
    char**          to;               
    comm_uint32     nofRec;
    int             accountid;
    char            *file_name;
    char*           fwd_uidl;

};

typedef struct SmtpSendMailStruct SmtpSendMail;



/*==========================================================
 * Function 	: mail_NotifySendErr
 * Author		: sunguochen      
 * Description : 通知UI 及本地管理发送失败
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_NotifySendErr(mail_retval err);



/*==========================================================
 * Function 	: mail_smtpEndSession
 * Author		: sunguochen      
 * Description : 结束当前SMTP发送
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_smtpEndSession(void);


/*==========================================================
 * Function 	: mail_smtpCancelSending
 * Author		: sunguochen      
 * Description : 取消发送
                      UI调用此接口用于停止发送邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_smtpCancelSending(void);



/*==========================================================
 * Function 	: mail_SmtpInit
 * Author		: sunguochen      
 * Description : smtp init
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_SmtpInit(void);



/*==========================================================
 * Function 	: mail_SmtpTerminate
 * Author		: sunguochen      
 * Description : smtp terminate
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_SmtpTerminate(int termMode);



/*==========================================================
 * Function 	: mail_MailRecpListPtr
 * Author		: sunguochen      
 * Description : 收件人列表
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/31/2010 
 ==========================================================*/
SmtpSendMail *mail_MailRecpListPtr(void);



/*==========================================================
 * Function 	: mail_isWaitingNextSending
 * Author		: sunguochen      
 * Description  : 是否等待发送下一封邮件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	    : created  5/31/2010 
 ==========================================================*/
comm_boolean mail_isWaitingNextSending(void);




#endif 
