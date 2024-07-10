/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_ret.h
 *	Author		: sunguochen    
 *	Description	: 返回值处理
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/



#ifndef _MAIL_RET_H_
#define _MAIL_RET_H_

#include "mail_cfg.h"

typedef enum {
    

    MAIL_OK                      = 0x0000,
    MAIL_SYNCH_CANCELLED         = 0x0001,
    MAIL_RETRIEVE_CANCELLED      = 0x0002,
    MAIL_RESULT_MEMORY_FAULT     = 0x0003,
    MAIL_RESULT_FILE_ERROR       = 0x0004,
    MAIL_DELETE_FAILED           = 0x0005,
    MAIL_RESULT_FILE_NOT_FOUND   = 0x0006,
    MAIL_RESULT_BUSY             = 0x0007,
    MAIL_CANCEL                  = 0x0008,
    MAIL_ERROR                   = 0x0009,
    MAIL_TOO_LATE                = 0x000a,
    MAIL_CALL_IN_PROGRESS        = 0x0010,
    MAIL_FDN_LIMIT               = 0x0011,
    MAIL_OFF_LINE                = 0x0012,
    MAIL_NO_COVERAGE             = 0x0013,
    MAIL_BEARER_NOT_AVAILABLE    = 0x0014,

    MAIL_MSG_MAX_BODYPARTS_REACHED = 0x0100,

    MAIL_POP3_HEADER_TOO_LARGE               = 0x0201,
    MAIL_POP3_INSUFFICIENT_DISK              = 0x0202,
    MAIL_POP3_NO_ACCOUNT                     = 0x0203,
    MAIL_POP3_TIMEOUT                        = 0x0204,
    MAIL_POP3_LOGIN_FAILED                   = 0x0205,
    MAIL_POP3_FAILURE                        = 0x0206,
    MAIL_POP3_NO_SUCH_MSG                    = 0x0207,
    MAIL_POP3_NO_SUCH_MESSAGE                = 0x0208,
    MAIL_POP3_LOGIN_APOP_FAILED_NO_TIMESTAMP = 0x0209,
    MAIL_POP3_LOGIN_APOP_FAILED_HMAC         = 0x020a,
    MAIL_POP3_CANCELLED                      = 0x020b,
    MAIL_POP3_HEADER_ONLY_NOT_SUPPORTED      = 0x020c,


    MAIL_SMTP_OK_QUIT_FAILED           = 0x0300,
    MAIL_SMTP_SEND_MAIL_FAILED         = 0x0302,
    MAIL_SMTP_SEND_MAIL_FROM_FAILED    = 0x0303,
    MAIL_SMTP_SEND_MAIL_FAILED_NO_VALID_RECEPIENTS = 0x0304,
    MAIL_SMTP_SEND_MAIL_FAILED_NON_VALID_RECIPIENT = 0x0305,
    MAIL_SMTP_SEND_MAIL_FAILED_TOO_MANY_RECIPIENTS = 0x0306,
    MAIL_SMTP_SEND_MAIL_ACCESS_DENIED  = 0x0307,
    MAIL_SMTP_RESULT_TIMEOUT           = 0x0308,
    MAIL_SMTP_HELO_FAILED              = 0x0309,
    MAIL_SMTP_EHLO_FAILED              = 0x030a,
    MAIL_SMTP_EHLO_CRAMMD5_FAILED      = 0x030b,
    MAIL_SMTP_EHLO_CRAMMD5_HMAC_FAILED = 0x030c,
    MAIL_SMTP_EHLO_OUT_OF_MEMORY       = 0x030d, 
    MAIL_SMTP_EHLO_LOGIN_FAILED        = 0x030e,
    MAIL_SMTP_EHLO_PLAIN_FAILED        = 0x030f,
    MAIL_SMTP_AUTHENTICATION_REQUIRED  = 0x0310,


    MAIL_IMAP4_CANCELLED                 = 0x0400,
    MAIL_IMAP4_LOGIN_FAILED              = 0x0401,
    MAIL_IMAP4_CONNECTION_DENIED         = 0x0402,
    MAIL_IMAP4_PROTOCOL_ERROR            = 0x0403,
    MAIL_IMAP4_MBX_ACCESS_VIOLATION      = 0x0404,
    MAIL_IMAP4_NO_SUCH_UID               = 0x0405,
    MAIL_IMAP4_NO_SUCH_BODYPART          = 0x0406,
    MAIL_IMAP4_TOO_BIG                   = 0x0407,
    MAIL_IMAP4_INTERNAL_DATA_ERROR       = 0x0408,
    MAIL_IMAP4_TIMEOUT                   = 0x0409,
    MAIL_IMAP4_LOGIN_DISABLED            = 0x040A,


    MAIL_SEND_PARTLY_SUCCESSFUL          = 0x0500,
    MAIL_SEND_FAILED_NO_RECIPIENTS       = 0x0501,
    MAIL_SEND_FAILED_ALL_MAIL_FAILED     = 0x0502,
    MAIL_SEND_FAILED_CMP_FROM_ADDRESS    = 0x0503,
    MAIL_SEND_EXTENSION_NOT_SUPPORTED    = 0x0504,
    

    MAIL_MSGBOX_ERROR                   = 0x0600, 
    MAIL_MSGBOX_BASE64_DECODE_ERROR     = 0x0601,
    
    
     MAIL_NET_SOCKET_CLOSED                       =  0x0700,
     MAIL_NET_ERROR_SOCKET_CREATE                 =  0x0701,
     MAIL_NET_ERROR_SOCKET_COULD_NOT_CONNECT      =  0x0702,
     MAIL_NET_ERROR_SOCKET_FATAL                  =  0x0703,
     MAIL_NET_ERROR_SOCKET_NOT_FOUND              =  0x0704,
     MAIL_NET_ERROR_SOCKET_BAD_ID                 =  0x0705,
     MAIL_NET_ERROR_SOCKET_INVALID_PARAM          =  0x0706,
     MAIL_NET_ERROR_SOCKET_MSG_SIZE               =  0x0707,
     MAIL_NET_ERROR_SOCKET_RESOURCE_LIMIT         =  0x0708,
     MAIL_NET_ERROR_SOCKET_CONNECTION_FAILED      =  0x0709,
     MAIL_NET_ERROR_SOCKET_HOST_NOT_FOUND         =  0x070a,
     MAIL_NET_ERROR_TLS_CONNECTION_FAILED         =  0x070b,
     MAIL_NET_ERROR_TLS_TOO_WEAK_CERT             =  0x070c,
     MAIL_NET_ERROR_TLS_SERVER_IDENTITY_NOT_SAFE  =  0x070d,
     MAIL_NET_ERROR_UNDEF                         =  0x070e,

      
    MAIL_SETTINGS_RESULT_GRP_NOT_FOUND            = 0x0902,
    MAIL_SETTINGS_RESULT_DEF_GRP_NOT_FOUND        = 0x0905,
    MAIL_SETTINGS_RESULT_MAX_NR_OF_BLOCKED_PATTERN = 0x0906, 


    MAIL_ASYNC_EOF                               = 0x0a01,
    MAIL_ASYNC_DELAYED                           = 0x0a02,  
    MAIL_FILE_ERROR_FULL                         = 0x0a03,
    MAIL_FILE_ERROR_SIZE                         = 0x0a04,


     MAIL_FM_FOLDER_NOT_FOUND                    = 0x0b01,
     MAIL_FM_MSGID_NOT_FOUND                     = 0x0b02,
     MAIL_FM_MEMORY_FAULT                        = 0x0b03,
     MAIL_FM_FILE_ERROR                          = 0x0b04,
     MAIL_FM_MSG_BODY_NOT_RETRIEVED              = 0x0b05,
     MAIL_FM_MSG_HAS_NO_BODY                     = 0x0b06,
     MAIL_FM_MSG_BODYPART_NOT_RETRIEVED          = 0x0b07,
     MAIL_FM_MOVE_MSG_TO_THIS_FOLDER_NOT_ALLOWED = 0x0b08,
     MAIL_FM_LIMIT_OF_MSG_REACHED                = 0x0b09,


     MAIL_MIME_MSG_ERROR                         = 0x0c01,  
     MAIL_MIME_MEM_ERROR                         = 0x0c02,  
     MAIL_MIME_FM_ERROR                          = 0x0c03,  
     MAIL_MIME_DEC_ERROR                         = 0x0c04,  
     MAIL_MIME_ENC_ERROR                         = 0x0c05,  
#ifdef MAIL_FLDR_FILE_ERROR   
     MAIL_FILE_OPEN_ERROR                                   =0x0c06,
     MAIL_FILE_WRITE_ERROR                                 =0x0c07,
     MAIL_FILE_READ_ERROR                                   =0x0c08,
     MAIL_FILE_SEEK_ERROR                                   =0x0C09,
     MAIL_FILE_CLOSE_ERROR                                  =0x0d01,
     MAIL_FILE_SET_SIZE_ERROR                             =0x0d02,
     MAIL_FILE_RENAME_ERROR                             =0x0d03
#endif


} mail_retval;





/*==========================================================
 * Function 	: mail_ret2str
 * Author		: sunguochen      
 * Description : 错误值转化为字符串形式
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
const char *mail_ret2str(mail_retval ret);



#endif
