/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_type.h
 *	Author		: sunguochen    
 *	Description	: 手机邮箱类型
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _MAIL_TYPE_H_
#define _MAIL_TYPE_H_


#include "mail_cfg.h"
#include "comm_platform.h"
#include "mail_message.h"






#define MAIL_MSM_REG_MAX_LENGTH_OF_STRING_VALUE      500


#define MAIL_MAX_FILE_NAME_LENGTH              65

//changed by liangshun for MS00217735 20110105
#define MAIL_MAX_WCHAR_FILE_NAME_LENGTH        MAIL_MAX_FILE_PATH_LENGTH//130
//changed by liangshun for MS00217735 20110105
#define MAIL_MAX_COMPL_FILE_NAME_LENGTH  MAIL_MAX_FILE_PATH_LENGTH//65

#define MAIL_MAX_FILE_PATH_LENGTH   255

#define MAIL_TYPE_LENGTH                                64
#define MAIL_SUBTYPE_LENGTH                          64


#define MAIL_MIME_TYPE_MULTIPART      "multipart"
#define MAIL_MIME_TYPE_TEXT                "text"
#define MAIL_MIME_SUBTYPE_MIXED        "mixed"
#define MAIL_MIME_SUBTYPE_ALT            "alternative"
#define MAIL_MIME_SUBTYPE_RELATED    "related"
#define MAIL_MIME_SUBTYPE_PLAIN         "plain"
#define MAIL_MIME_SUBTYPE_REPORT     "report"
#define MAIL_MIME_TYPE_SEPARATOR      "/"
#define MAIL_MIME_BOUNDARY_LENGTH  80 

#define MAIL_MD5_DIGEST_LEN 16


#define MAIL_INBOX        -1      
#define MAIL_DRAFTS       -2      
#define MAIL_OUTBOX       -3      
#define MAIL_SENT         -4      
#define MAIL_BLOCKED      -5     

#define MAIL_SYSTEM_FOLDERS   -6     
#define MAIL_PERSONAL_FOLDERS -7  
#define MAIL_NOTIF            -8  



#define MAIL_DO_NOT_MOVE -15      



#define MAIL_POP3  0
#define MAIL_IMAP4 1


#define MAIL_NO_ACCOUNT       0
#define MAIL_DEFAULT_ACCOUNT -1
#define MAIL_ALL_ACCOUNTS    -2


#define MAIL_PRIORITY_LOW     5 
#define MAIL_PRIORITY_NORMAL  3 
#define MAIL_PRIORITY_HIGH    1 
#define MAIL_PRIORITY_NOT_SET 0


#define MAIL_NO_READ_REPORT_REQUESTED 0
#define MAIL_READ_REPORT_REQUESTED    1
#define MAIL_READ_REPORT_HANDLED      2
#define MAIL_READ_REPORT_WARNING      3

#define MAIL_BPNO_MAX_LEVELS 5    
#define MAIL_BPNO_MAX_STR_LEN 2 * MAIL_BPNO_MAX_LEVELS



#define ROUNDUP_TO_N(x,n)   (((x)+(n)-1)/(n)*(n))



#define ROUNDUP_TO_WORDS(x)   ROUNDUP_TO_N((x),sizeof(int))



#define MAIL_NEW_BLOCK_PATTERN_ID_NOT_STORED_YET -1 


typedef enum
{
    MAIL_TERM_IDLE = 0,  
    MAIL_TERM_SOFT,      
    MAIL_TERM_HARD

}MailTerminateMode;


typedef void MailTerminateRsp(void* userData);


typedef comm_int32 MailFolderType;        




typedef comm_int32 MailAccountIdType;     



typedef comm_int32 MailIncomingProtocol;  




typedef comm_uint32  MailMessageIdType;   

typedef comm_uint16  MailServerPortType;  

typedef unsigned char MailByteType;



typedef comm_int32 MailMessagePriority;    


typedef comm_int32 MailReadReportRequested; 





typedef enum
{
    
    MAIL_RES_OK = 0,

    
    MAIL_RES_ERROR, 

    
    MAIL_RES_BUSY, 

    
    MAIL_RES_COMM_SERVER_ERROR, 

    
    MAIL_RES_COMM_HOST_NOT_FOUND, 

    
    MAIL_RES_NOT_USED_5, 

    
    MAIL_RES_NOT_USED_6, 

    
    MAIL_RES_MSG_TOO_LARGE,  

    
    MAIL_RES_MSG_ERROR,  

    
    MAIL_RES_MSGID_NOT_FOUND,  

    
    MAIL_RES_FOLDER_NOT_FOUND, 

    
    MAIL_RES_FOLDER_ERROR, 

    
    MAIL_RES_NOT_USED_12, 

    
    MAIL_RES_NOT_USED_13, 

    
    MAIL_RES_FILE_ERROR,  

    
    MAIL_RES_FILE_NOT_FOUND, 

    
    MAIL_RES_NOT_USED_16, 

    
    MAIL_RES_NOT_USED_17, 

    
    MAIL_RES_NOT_USED_18, 

    
    MAIL_RES_NOT_USED_19,  

    
    MAIL_RES_MSG_MAX_BODYPARTS_REACHED,  

    

    MAIL_RES_MSG_HAS_NO_BODY,  

    
    MAIL_RES_MSG_BODY_NOT_RETRIEVED,  

    
    MAIL_RES_MSG_BODYPART_NOT_RETRIEVED,  

    
    MAIL_RES_NOT_USED_24,  

    
    MAIL_RES_MEMORY_FAULT,  

    
    MAIL_RES_NO_RECIPIENTS, 

    
    MAIL_RES_SEND_MAIL_FAILED_TOO_MANY_RECIPIENTS, 

    
    MAIL_RES_NOT_USED_28, 

    
    MAIL_RES_ACCOUNT_NOT_FOUND, 

    
    MAIL_RES_NOT_USED_30, 

    
    MAIL_RES_NO_ACCOUNTS, 

    
    MAIL_RES_NO_DEFAULT_ACCOUNT, 

    

    MAIL_RES_LIMIT_OF_MSG_REACHED, 

    
    MAIL_RES_NOT_USED_34, 

    
    MAIL_RES_OPERATION_COULD_NOT_BE_STOPPED, 

    
    MAIL_RES_NOT_USED_36, 

    
    MAIL_RES_CANCELLED, 

    
    MAIL_RES_LOGIN_FAILED, 

    



    MAIL_RES_LOGIN_APOP_FAILED_NO_TIMESTAMP,

    
    MAIL_RES_LOGIN_APOP_FAILED_HMAC,

    
    MAIL_RES_SYNC_HEADER_ONLY_NOT_SUPPORTED,

    

    MAIL_RES_SEND_MAIL_FROM_FAILED,  

    
    MAIL_RES_SYNCH_FAILED, 

    
    MAIL_RES_RETRIEVE_FAILED, 

    
    MAIL_RES_NOT_USED_42, 

    
    MAIL_RES_SOCKET_CREATE_FAILED, 

    
    MAIL_RES_PROTOCOL_ERROR, 

    
    MAIL_RES_NOT_USED_45, 

    
    MAIL_RES_NOT_USED_46, 

    

    MAIL_RES_SOCKET_RESOURCE_LIMIT, 

    
    MAIL_RES_MBX_ACCESS_VIOLATION, 

    
    MAIL_RES_NO_SUCH_BODYPART, 

    
    MAIL_RES_SEND_FAILED, 

    
    MAIL_RES_ACCOUNTS_LIMIT_REACHED, 

    
    MAIL_RES_DEFAULT_ACCOUNT, 

    
    MAIL_RES_ACCOUNT_NAME_EXISTS, 

    
    MAIL_RES_NO_ACCOUNT_NAME, 

    
    MAIL_RES_SEND_MAIL_FAILED_NO_VALID_RECEPIENTS, 

    
    MAIL_RES_SEND_MAIL_FAILED_NON_VALID_RECIPIENT, 

    
    MAIL_RES_COULD_NOT_CONNECT_TO_PORT, 

    
    MAIL_RES_SEND_HELO_FAILED,  

    
    MAIL_RES_FAILED, 

    

    MAIL_RES_PARTLY_SUCCESSFUL, 

    
    MAIL_RES_MSG_ACCOUNT_NOT_EXISTS, 

    

    MAIL_RES_SEND_EHLO_FAILED,  

    
    MAIL_RES_EHLO_CRAMMD5_FAILED,  

    
    MAIL_RES_EHLO_PLAIN_FAILED,  

    
    MAIL_RES_EHLO_LOGIN_FAILED,  

    
    MAIL_RES_EHLO_CRAMMD5_HMAC_FAILED,  

    
    MAIL_RES_EHLO_OUT_OF_MEMORY,  

    
    MAIL_RES_AUTHENTICATION_REQUIRED, 

    
    MAIL_RES_ACCESS_DENIED,    

    









    MAIL_RES_MOVE_MSG_TO_THIS_FOLDER_NOT_ALLOWED, 
    
    
    MAIL_RES_COMM_NO_COVERAGE_ERROR,     

    
    MAIL_RES_COMM_FDN_LIMITATION_ERROR,  

    
    MAIL_RES_COMM_OFFLINE_MODE_ERROR,    

    
    MAIL_RES_COMM_NO_BEARER_ERROR,   

    
    MAIL_RES_COMM_CALL_IN_PROGRESS_ERROR,    

    
    MAIL_RES_MSG_DECODE_ERROR,  

    
    MAIL_RES_FILE_ERROR_FULL,

    
    MAIL_RES_FILE_ERROR_SIZE,

    
    MAIL_RES_FROM_ADDRESS_EXISTS,

    MAIL_RES_TLS_CONNECTION_FAILED,
    MAIL_RES_TLS_TOO_WEAK_CERT,
    MAIL_RES_TLS_SERVER_IDENTITY_NOT_SAFE,
    MAIL_RES_ONLY_LOGIN_WHEN_TLS_ESTAB,
    MAIL_RES_SEND_FAILED_CMP_FROM_ADDRESS,
    MAIL_RES_SEND_EXTENSION_NOT_SUPPORTED


} MailResult;






typedef enum
{
    MAIL_ENC_BASE64 = 0,         
    MAIL_ENC_7BIT,               
    MAIL_ENC_8BIT,               
    MAIL_ENC_QUOTED_PRINTABLE,   
    MAIL_ENC_BINARY,             
    MAIL_ENC_OTHER               


} MailContTransfEnc;





typedef enum
{
    MAIL_ADD = 0,
    MAIL_UPDATE,
    MAIL_DELETE

} MailSetBodyPartAction;







typedef enum
{
    MAIL_NEVER = 0,
    MAIL_NEW_MAILS,
    MAIL_ALL_MAILS

} MailUseSignature;







//pop3 登录机制
typedef enum
{

  MAIL_POP3_LOGIN_MECHANISM_USER = 0,

  MAIL_POP3_LOGIN_MECHANISM_APOP

} MailPop3LoginMechanism;






typedef enum
{
    MAIL_MOVE_TO_BLOCKED_FOLDER = 0,
    MAIL_DO_NOT_SYNC,  
    MAIL_DELETE_ON_SERVER

} MailBlockAction;






typedef enum
{
    MAIL_DOWNLOAD_ALWAYS_ASK = 0,     
    MAIL_DOWNLOAD_AUTOMATIC,          
    MAIL_DOWNLOAD_MANUAL              

} MailDownloadAction;



typedef enum 
{
    MAIL_AUTH_OFF = 0,           
    MAIL_AUTH_SMTP_EXT,          
    MAIL_AUTH_LOGIN_BEFORE_SEND  

} MailAuthMethod;






typedef struct
{
    comm_int32 charset;  
    char  *text;

} MailEncodedText;






typedef struct MailAddressStruct
{
    MailEncodedText name;                    
    char real[MAIL_MAX_REAL_ADDRESS_LENGTH]; 
    struct MailAddressStruct *next;

} MailAddress;






typedef struct
{
    char type[MAIL_TYPE_LENGTH];         
    char subtype[MAIL_SUBTYPE_LENGTH];

} MailContentType;


typedef enum
{
  MAIL_RFC822_ADDRESS_TYPE,
  MAIL_UNKNOWN_ADDRESS_TYPE

}MailAddressType;


typedef struct
{
  MailAddressType addrType;               
  MailEncodedText *text;                  
  comm_uint32 textLength;

} MailOriginalRecipient;








typedef struct
{
    
    MailAddress *from;
    MailAddress *to;
    MailAddress *cc;
    MailAddress *bcc;
    MailAddress *replyTo;
    MailAddress *returnPath;
    MailAddress *dispAddress;
    MailEncodedText subject;
    MailEncodedText originalRecipient;                
    MailEncodedText originalMsgId;                            
    MailMessagePriority priority;
    MailReadReportRequested readReportReq; 



    
    comm_uint32   date;                  


    
    comm_int32    tz_min;                


    comm_uint32   receivedTime;          



    int          receivedTimeZone;      


    comm_uint8    isMDN;                 
    comm_uint8    isDSN;                 
    comm_uint8    protectedFlag;         
    MailContentType contentType;         
    
} MailHeader;






typedef struct
{
    char tempFileName[MAIL_MAX_COMPL_FILE_NAME_LENGTH + 1];//changed by liangshun for MS00217735 20110105
                                    
    MailContentType contentType;     
    MailEncodedText bodypartName;    

    comm_int32 charset;              


    char*         bodyPartNumber;   

    comm_uint32    bodySize;         

    comm_uint16    headerSize;       
    comm_boolean      headerInFile;     
} MailBodypart;


typedef enum
{
    MAIL_HIDDEN = 0,
    MAIL_ATTACHMENT,
    MAIL_MAIN_BODYPART,
    MAIL_MAIN_BODYPART_ALT,
    MAIL_INLINE_CID,
    MAIL_INLINE_CL

} MailVisibleBodyPartType;





typedef struct MailMimeElement
{
    MailContentType   contentType;    
    char             bodyPartNumber[ROUNDUP_TO_WORDS(MAIL_BPNO_MAX_STR_LEN)];
                                     
    comm_int32        charset;        

    MailEncodedText   bodypartName;   

    MailContTransfEnc transfEnc;      

    comm_uint32       size;           

    comm_boolean         isRetrieved;    


    MailVisibleBodyPartType visibleBodyPartType;
    char             *boundary;      
    unsigned char    bodypartId[MAIL_MD5_DIGEST_LEN]; 

} MailMimeElement;







typedef struct
{
    MailMessageIdType  msgId;     
    MailResult           result;

} MailMsgIdWithResult;








typedef struct
{
    char* subjectPrefix;            
    char* originalMessageSeparator; 

    char* fromTitle;                
    char* sentTitle;                
    char* toTitle;                  
    char* subjectTitle;             

    char* ccTitle;

} MailCreateForwardReplyTitles;





typedef struct
{
    MailMessageIdType msgId;
    char* serverId;

}MailMsgAndServerId;






typedef struct MailServerMsgIDsListStruct
{
  struct MailServerMsgIDsListStruct* next;   
  char*            uniqueMsgId;             
  MailMessageIdType sessionMsgId;            
  comm_uint32       messageSize;             
  comm_boolean         synchronizeFullMessage;  

} MailServerMsgIDsList;







typedef enum
{
    MAIL_NO_FILTERING = 0,
    MAIL_EXACT_MATCH     

} mail_FilterSettingType;

typedef struct
{
    mail_FilterSettingType folderId;
    mail_FilterSettingType msgId;
    mail_FilterSettingType accountID;
    mail_FilterSettingType sent;
    mail_FilterSettingType read;
    mail_FilterSettingType answered;
    mail_FilterSettingType protectedflag;
    mail_FilterSettingType forwardLocked;
    mail_FilterSettingType downloaded;
    mail_FilterSettingType mhtml;
    mail_FilterSettingType isIncoming;
    mail_FilterSettingType isMDN;
    mail_FilterSettingType isDSN;

} MailExternalFilterSettingList;




typedef struct
{
    MailFolderType folderId;
    MailMessageIdType msgId;
    MailAccountIdType accountID;
    comm_uint8 sent;
    comm_uint8 read;
    comm_uint8 answered;
    comm_uint8 protectedFlag;
    comm_uint8 forwardLocked;
    comm_uint8 downladed;
    comm_uint8 mhtml;
    comm_uint8 isIncoming;
    comm_uint8 isMDN;
    comm_uint8 isDSN;

} MailExternalFilterValues;





typedef struct
{
    MailExternalFilterSettingList filterSettingList;
    MailExternalFilterValues filterValueList;

} MailExternalFilter;







typedef struct
{
    comm_uint32 totNumOfMsg;
    comm_uint32 numOfUnreadMsg;

} MailFolderInfo;


#define MAIL_MSG_IS_SENT             0x0001   
#define MAIL_MSG_IS_READ             0x0002   
#define MAIL_MSG_IS_ANSWERED         0x0004   
#define MAIL_MSG_IS_PROTECTED        0x0008   
#define MAIL_MSG_IS_FORWARD_LOCKED   0x0010   
#define MAIL_MSG_IS_DOWNLOADED       0x0020   

#define MAIL_MSG_IS_MHTML            0x0040   

#define MAIL_MSG_IS_INCOMING         0x0080   
#define MAIL_MSG_IS_MDN              0x0100   

#define MAIL_MSG_IS_DSN              0x0200   



#define MAIL_MSG_SET_FLAG(flags, s) (flags |= s)
#define MAIL_MSG_CLR_FLAG(flags, s) (flags |= ~s)
#define MAIL_MSG_IS_FLAG_SET(flags, s) (flags & s)





typedef struct
{
    MailFolderType folderId;
    MailMessageIdType msgId;
    MailAccountIdType accountID;
    comm_uint32 messageSize;         
    comm_uint32 serverSize;          
    char *truncSubject;             
    char *truncFrom;                
    char *truncTo;                  
    char *truncRec;          
    char *truncCc;                  
    char *truncBcc;           
    MailMessagePriority priority;    
    MailReadReportRequested rrStatus; 
    comm_uint32 modifiedTime;        

    comm_uint32 sentTime;            


    comm_uint32 receivedTime;        


    comm_int32 numberOfAttachments;  
    comm_uint32 flags;               

} MailMsgListInfo;


typedef enum
{
    MAIL_PROGRESS_CONNECT = 0,             
    MAIL_PROGRESS_GETTING_MSG_IDS_ON_SERVER,  
    MAIL_PROGRESS_SYNCHRONIZING_MESSAGE,      
    MAIL_PROGRESS_RETRIEVING_MESSAGE,         
    MAIL_PROGRESS_SENDING_EMAIL               
                                             
} MailProgressStatus;


#define MAIL_READ_REPORT_ACTION_AUTO     0x01    
#define MAIL_READ_REPORT_SENDING_AUTO    0x02    
#define MAIL_READ_REPORT_DISPLAYED       0x04    

typedef enum
{
  MAIL_DELIVERY_REPORT_FAILED,
  MAIL_DELIVERY_REPORT_DELAYED,
  MAIL_DELIVERY_REPORT_DELIVERED,
  MAIL_DELIVERY_REPORT_RELAYED,
  MAIL_DELIVERY_REPORT_EXPANDED,
  MAIL_DELIVERY_REPORT_UNKNOWN

} MailDeliveryReportStatus;

typedef enum
{
  MAIL_REPORT_MANUAL_ACTION_MODE,
  MAIL_REPORT_AUTOMATIC_ACTION_MODE,
  MAIL_REPORT_UNKNOWN_ACTION_MODE

}MailActionMode;

typedef enum
{
  MAIL_REPORT_MANUALLY_SENT_MODE,
  MAIL_REPORT_AUTOMATICALLY_SENT_MODE,
  MAIL_REPORT_UNKNOWN_SENT_MODE

}MailSendingMode;

typedef enum
{
  MAIL_REPORT_TYPE_DISPLAYED,
  MAIL_REPORT_TYPE_DELETED,
  MAIL_REPORT_TYPE_UNKNOWN

} MailDispositionType;

typedef enum
{
  MAIL_REPORT_ERROR_FAILURE,
  MAIL_REPORT_ERROR_ERROR,
  MAIL_REPORT_ERROR_WARNING,
  MAIL_REPORT_ERROR_NOERROR

} MailDispositionError;

typedef struct
{
  MailActionMode actionMode;
  MailSendingMode sendingMode;

} MailDispositionMode;

typedef struct
{
  MailDispositionMode mode;
  MailDispositionType type;
  MailDispositionError error;

} MailReportStatus;





typedef struct
{

    comm_uint32 totNumOfMsg;      
    comm_uint32 processedBytes;   

    comm_int32  totalByteSize;    
    comm_uint16 msgIndex;         
    comm_uint16 bytePercent;      




    comm_uint16 staticPercent;    


    comm_uint16 lastNotified;     
    int currentMailStatus;

} MailProgressInfo;






typedef struct
{
    MailAccountIdType accountId;
    MailMessageIdType msgId;
    char *uniqueMsgId;

} MailMessageIdData;

typedef enum
{
    MAIL_READ_FLAG,
    MAIL_PROTECT_FLAG,
    MAIL_ANSWERED_FLAG

}MailFlagType;

typedef struct
{
    MailFolderType folder;
    comm_uint32    usedMem;

}MailMemStatus;



#endif
