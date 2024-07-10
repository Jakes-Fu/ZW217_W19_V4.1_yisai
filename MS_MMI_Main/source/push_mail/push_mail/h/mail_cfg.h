/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_cfg.h
 *	Author		: sunguochen    
 *	Description	: 手机邮箱相关参数限制
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _MAIL_CFG_H_
#define _MAIL_CFG_H_



#define MAIL_MAX_TRUNC_SUBJECT_LENGTH  65

#define MAIL_MAX_TRUNC_FROM_LENGTH     65

#define MAIL_MAX_TRUNC_TO_LENGTH       65

#define MAIL_MAX_TRUNC_CC_LENGTH       65

#define MAIL_MAX_TRUNC_BCC_LENGTH       65

#define MAIL_MAX_TRUNC_REC_LENGTH      65

#define MAIL_MAX_ATTACHMENT_FILE_PATH  256

#define MAIL_MAX_UIDL_LENGTH 65

#define MAIL_ADDRESS_SEPARATOR         ";"

#define MAIL_CR '\r'
#define MAIL_SPACE  ' '  





#define MAIL_MAIN_FOLDER "D:\\Mail"

#define MAIL_FM_FOLDER  MAIL_MAIN_FOLDER "\\box\\"

#define MAIL_TMP_FOLDER MAIL_MAIN_FOLDER "\\temp\\"

#define MAIL_FM_FOLDER_FOR_GET_FILE_NUM MAIL_MAIN_FOLDER "\\box\\*.*"

#define MAIL_BK_FACTORY_CLR_FOLDER "D:\\Mail\\"
//附件目录
#define MAIL_ATTCH_FOLDER MAIL_MAIN_FOLDER "\\attach\\"

#define MAIL_TEMP_FILE_EXTENSION "tmp"

#define MAIL_TEMP_FILE_WEXT L"tmp"

#define MAIL_MSGBOX_ADD_ATTACHMENT_USE_AFI 0

#define MAIL_FOLDER_SEND_MSG_FAILED  MAIL_OUTBOX

#define MAIL_RFC822_LINE_SIZE 1000

//socket sending buffer size
#define MAIL_SOCK_SEND_BUFF_SIZE  16384 
//socket recving buffer size
#define MAIL_SOCK_RECV_BUFF_SIZE  16384 
//chunking data buffer size
#define MAIL_ASYNC_MAX_CHUNK_SIZE 16384 
//body binary chunk size
#define MAIL_BODYPART_BINARY_CHUNK_SIZE    16386 
//body chunk size
#define MAIL_BODYPART_CHUNK_SIZE ((MAIL_BODYPART_BINARY_CHUNK_SIZE+2)/3*3)

//mail encode line
#define MAIL_ENCODED_LINE_SIZE 72

//mail forward text body
#define MAIL_MAX_SIZE_FORWARD_REPLY_TEXT_BODYPART 5000

#define MAIL_FWD_REPLY_TRUNC_STRING "..."

#define MAIL_MIME_CHUNK_READ_SIZE 16384 

#define MAIL_HEADER_ESTIMATED_SIZE       1200

#define MAIL_MIN_PROGR_INTERVAL      5

#define MAIL_CONNECTED_PROGRESS_PERCENT  10

//network process expire time
#define MAIL_CREATE_SOCKET_TIMEOUT_VALUE 30000
#define MAIL_SOCKET_SELECT_TIMEOUT_VALUE 30000

//发送超时时间 2 分钟
//因为发送大邮件可能很慢
#define MAIL_SMTP_GENERIC_TIMEOUT_VALUE  120000

//接收超时时间设为2 分钟
#define MAIL_POP3_GENERIC_TIMEOUT_VALUE  120000

//接收重试次数设置为3次
#define MAIL_POP3_GENERIC_TIMEOUT_RETRY_TIMES   3
#define MAIL_POP3_IMAP_QUIT_TIMEOUT          5000

#define MAIL_IMAP4_GENERIC_TIMEOUT_VALUE 30000
#define MAIL_IMAP4_IDLE_GENERIC_TIMEOUT_VALUE 300000
#define MAIL_TERMINATE_FILE_TIMEOUT          0

#define MAIL_MAX_HTML_BASE_ADR_LEN      512
#define MAIL_HTML_DEFAULT_BASE_ADDR    "thismessage://"
#define MAIL_HTML_BASE_STR             "<base=20href=3D"

#define MAIL_MAX_NO_OF_MAILS  200
#define MAIL_MAX_NUMBER_OF_BODYPARTS 40
#define MAIL_MAX_REAL_ADDRESS_LENGTH    129
#define MAIL_MAX_ATTACH_NAME_LENGTH   128

#define EMAIL_USE_EMN_TEST_INTEGRATION 0

#define MAIL_IMAP4_USE_FETCH_FOR_MSG_ID 1
#define MAIL_IMAP4_USE_RFC822_HEADER 1

#define MAIL_FLDRMGR_MOVE 1
#define MAIL_FLDRMGR_DELETE 1

#define MAIL_POP3ENDSESSION_SAVE_MT  1
#define MAIL_SMTPENDSESSION_SAVE_MT   1
#define MAIL_IMAPENDSESSION_SAVE_MT   1

#define MAIL_FLDRMGR_SET_HEADER 1
#define MAIL_FLDRMGR_SET_NUMBER_ATTACHMENTS_BODYPART 1
#define MAIL_FLDRMGR_SET_FLAG_VALUE 1

#define MAIL_TLS_MIN_SYMMECTRIC_KEY_LENGTH 40
#define MAIL_TLS_MIN_CERT_KEY_LENGTH 512
#define MAIL_TLS_GUARANTEE_SERVER_IDENTITY 1
#endif 
