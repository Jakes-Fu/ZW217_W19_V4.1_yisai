/*****************************************************************************
** File Name:      mail_fldr.h                                           *
** Author:                                                                   *
** Date:           2010-04                                                 *
** Copyright:            *
** Description:    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/04        lixu               Create									 *
*****************************************************************************/
/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#ifndef _FLDRMGR_H
#define _FLDRMGR_H


#include "mail_main.h"
#include "mail_cfg.h"
#include "mail_type.h"
#include "mail_sig.h"
#include "mail_ret.h"
#include "mail_asyn.h"
#include "mail_sub_list.h"


#define MAIL_FILE           "mail"                       
#define MAIL_MMTTMP         "mail.tp"                    
#define MAIL_MMTBACK        "mail.bk"                    
#define MAIL_MMT            "mail.cn" 
#define MAIL_UNSAVED_MESSAGE_SUFFIX "tmp"
#define MAIL_MESSAGE_SUFFIX 'm' 
#define MAIL_SAVE_MESSAGE_SUFFIX 't'
#define MAIL_MMT_BACKUP_FILE MAIL_FM_FOLDER MAIL_MMTBACK   
#define MAIL_MMT_TMP_FILE    MAIL_FM_FOLDER MAIL_MMTTMP    
#define MAIL_MMT_FILE        MAIL_FM_FOLDER MAIL_MMT       
typedef char MailFileType;



#define MAIL_MAX_SERVER_MSG_ID_LENGTH 70
#define MAIL_MAX_HEAD_STRING_LEN 80

#define MAIL_SORT_BY_TIME 4
#define MAIL_SORT_BY_STATE 12

typedef struct
{
    unsigned int readFlag           : 1;  
    unsigned int deleteFlag         : 1;  
    unsigned int sentFlag           : 1;  

    unsigned int haveAttachFlag     :1;
    unsigned int isSendingFlag      :1;
    unsigned int serverRejectFlag   :1;
    unsigned int sendOkFlag         :1;
    unsigned int sendFailFalg       :1;
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//add by lixu 2010.08.19
    unsigned int isLockFlag         :1;
    unsigned int isSecurityFlag     :1;
#endif//PUSH_EMAIL_3_IN_1_SUPPORT
} mail_flags;



typedef struct
{    
    MailFolderType folderId;     

    MailMessageIdType msgId;     
    MailAccountIdType accountID;
    comm_uint32 serverSize;      
    comm_uint32 modifiedTime;    
    comm_uint32 sentTime;        
    comm_uint32 receivedTime;    
    

    char uidl[MAIL_MAX_UIDL_LENGTH + 1]; //用于保存邮件UIDL，删除附件使用
    
    wchar truncSubject[MAIL_MAX_TRUNC_SUBJECT_LENGTH + 1];
    char truncFrom[MAIL_MAX_TRUNC_FROM_LENGTH + 1];               
    char truncTo[MAIL_MAX_TRUNC_TO_LENGTH + 1];     
    
    mail_flags flagValues;
} MailMessageFolderInfo;


typedef enum
{
    
    MAIL_SIG_FMG_MMT_OPERATION_FINISHED = MAIL_FLRD_SIGNAL_BASE,
    MAIL_SIG_FMG_TERMINATE,
    MAIL_SIG_FMG_INT_WRITE_MMT,
    MAIL_SIG_FMG_INT_CREATE_MMT_FILE_DONE,
    MAIL_SIG_FMG_DELETE_NEXT_MSG,//删除下一封邮件，支持取消删除操作
    MAIL_SIG_FMG_ADD_NEXT_FILE, //用于异步保存遍历文件
    MAIL_SIG_FMG_ADD_NEXT_TMP_FILE, //用于异步保存临时文件
    MAIL_SIG_FMG_DEL_ACCOUNT_MSG,//异步删除帐户邮件
    MAIL_SIG_FMG_DEL_ALL_ACCOUNT_MSG,//删除所有帐户邮件
    MAIL_SIG_FMG_APP_INIT,//处理本地管理初始化
    MAIL_SIG_FMG_DEL_SIGNAL_MSG,//删除单封邮件
    
    MAIL_SIG_FMG_MAX

} MailFldrMgrSignalId;


typedef enum
{
    STATE_FOLDER_STOPPED = 0,              
    STATE_FOLDER_IDLE,                     
    STATE_FOLDER_READ_MT_HEADER,           
    STATE_FOLDER_READING, 
    
    STATE_FOLDER_WRITE_MT_HEADER,          
    STATE_FOLDER_WRITING,   
    
    STATE_FOLDER_CREATING_MMT_FILES,       
    STATE_FOLDER_RESTORING,                
    STATE_FOLDER_WRITE_TERMINATE,          

    STATE_FOLDER_TEMINATING                
} Fldr_State;

typedef struct
{
    comm_int32 checkSum;             
    comm_uint32 nextFreeMsgId;       //从1开始标记邮件
    int numOfMsgs;
    MailMessageFolderInfo* msg;    
} MailMmtlist;





struct fsmdata
{
    char fileName[ROUNDUP_TO_WORDS(MAIL_MAX_COMPL_FILE_NAME_LENGTH + 1)];//changed by liangshun for MS00217735 20110105
    eAsyncOperationHandle asyncOperation;           
    eAsyncOperationHandle tmpAsyncOperation;        
    Fldr_State state;                                  
    MclList *filesToRestore; //目录遍历时保存需要添加的邮件信息                            
    MailMmtlist *masterTable;  
    comm_boolean isInitializeAbnormity;//初始化过程出现异常
    comm_boolean isInitialized; //标记应用初始化是否完成
    comm_boolean isInitializing;//标记应用初始化是否正在进行中
    comm_boolean isSysInitialzing;//标记当前是否正在进行系统初始化
    comm_boolean isNeedRestoreAfterSysInit;//标记进入引用初始化时是否需要重建masterTable
    comm_boolean isNeedAppInit;
    const char *masterTableFileName;   //当前存放mTable的文件名称
                                       //mail.cn/mail.tp/mail.bk
    
    MailTerminateMode termMode;                      
    comm_boolean queuedMmtWrite;

    comm_uint32 cur_sendMsgId; //记录当前正在发送的邮件的msgId
    comm_int32 cur_sendAccountId;//记录当前待发送邮件的帐户信息
    comm_uint32 next_saveFileId;//记录下一个需要保存的文件序号
    comm_uint32 cur_needSaveFileNum;//记录当前需要保存的文件个数
    
//用于在初始化未完成时记录请求数据
    void *addrForGetMailList;
    int   accountIdForGetMailList;
//用于在初始化未完成时记录请求数据
    
};

void mail_fldrMgrInit(void);
void mail_fldrEnterAppInit(void);
void mail_fldrMgrQuit(MMIMAIL_BOX_INFO_T *box_list);
void mail_fldrmgrDeleteMsg(comm_uint32 msgId);
mail_retval mail_fldrmgrCreateMsgInTableOnly(MailAccountIdType accountId,
                                           MailFolderType folderType,
                                           comm_uint32 *msgId,
                                           comm_uint32 size);
mail_retval mail_fldrmgrCreateMsgHandle(MailAccountIdType accountId,
                                      MailFolderType folderType,
                                      comm_uint32 size,
                                      comm_boolean leaveFileOpen,
                                      int *newFileHandle,
                                      MailMessageIdType *msgId);





char* mail_fldrmgrMsgId2Name(comm_uint32 msgId, MailFileType suffix);

comm_boolean mail_fldrmgrMoveMsg(MailMessageIdType msgId,
                              MailFolderType dstFolder);


mail_retval mail_fldrmgrSetReadFlag(comm_uint32 msgId, comm_uint32 value);
mail_retval mail_fldrmgrSetAccountId(MailMessageIdType msgId, MailAccountIdType accountId);





mail_retval mail_fldrMgrGetData( comm_uint32 msgId,
                               void *dataRecord,
                               void (*func)(comm_uint8 modId, void*, MailMessageFolderInfo *msgInfo));

void mail_fldrMgrGetAccountId( comm_uint8 modId, void *outData,
                              MailMessageFolderInfo *msgInfo);


void mail_fldrMgrGetServerSize( comm_uint8 modId, void *outData,
                               MailMessageFolderInfo *msgInfo);

comm_boolean mail_GetMailMessageForUi(int  accountId,
                                                                         MMIMAIL_BOX_INFO_T 
                                                                         *box_mail_list);
comm_boolean mail_needNoticeUi(void);
void mail_GetMailMessageFromParser(const wchar* msg_file_name,comm_int32 account_id);
comm_boolean mail_getMailMessageFromMsgId(comm_uint32 msgId,const char* mail_full_name);
comm_int32 mailNumCheck(void);
mail_retval mail_setMailTruncFields(MailMessageIdType msgId,comm_int32 account_id,
                                                                MAIL_MESSAGE_T *message_ptr,int saveMasterTable);
mail_retval mail_setMailTruncFieldsByIndex(int msgIndex,MAIL_MESSAGE_T *message_ptr,
                                                                int saveMasterTable);
comm_uint32 mail_getMsgIdByIndex(int index);
mail_retval mail_deleteMsg(const ESignal *sig);
comm_uint32 mail_getFirstDelMsgId(void);
comm_boolean mail_updateMailListByGetMessage(comm_uint32 msgId);
comm_uint32 mail_getCurMailTotalNum(void);
void mail_deleteMsgForUi(void);
comm_boolean getDelProcessState(void);
void setDelProcessState(comm_boolean state);
comm_uint32 mail_getNextMsgId(void);
mail_retval mail_setCancledMsgFlag(void);
mail_retval mail_fldrmgrSetDeletedFlag(comm_uint32 msgId, comm_boolean value);
void mail_setDelFlag(comm_uint32 record_id,comm_boolean value);
MailMessageFolderInfo* mail_getMailByIndex(int index);
char* mail_getMailUidlByMsgId(comm_uint32 msgId);
char* mail_getMailNameByMsgId(comm_uint32 msgId);
void fileName2Uidl(const char *fName,char* prefix);
int getIndexFromMasterTableByUidl(char* uidl);
mail_retval fldrmgrCleanUpMsgByUidl(char* uidl);
comm_boolean mail_getAppInitState(void);
comm_boolean mail_sendSaveMsg(comm_int32 account_id,MAIL_MESSAGE_T* message_ptr,
                                    char* fwd_uidl);
mail_retval mail_setMailTruncFieldsByAccoutId(comm_int32 account_id,
           MailMessageIdType msgId,MAIL_MESSAGE_T *message_ptr,int saveMasterTable);
void mail_fldrAppTerminate(void * p_param);
comm_uint32 mail_getNextNeedSendMsgId(comm_int32 account_id);
mail_retval mail_fldrmgrSetSendFailFlay(comm_uint32 msgId,comm_boolean value);
mail_retval mail_fldrmgrSetSendServerRejectFalg(comm_uint32 msgId,comm_boolean value);
mail_retval mail_fldrmgrSetIsSendingFalg(comm_uint32 msgId,comm_boolean value);
mail_retval mail_setMailTruncFieldsSaveDraft(comm_int32 account_id,
        MailMessageIdType msgId,MAIL_MESSAGE_T *message_ptr,int saveMasterTable);
void mail_sort(MMIMAIL_MAIL_SUMMERY_T ** mail_list,
                 uint8 var_offset, 
                 BOOLEAN big_to_small,comm_boolean sort_inbox);
void restoreFile(void);
comm_boolean mail_saveTmpFile(void);
mail_retval mail_setMailTruncFieldsTmp(int msgIndex,MAIL_MESSAGE_T *message_ptr,
                                                                int saveMasterTable);
comm_boolean mail_getMailMessageFromTmp(comm_uint32 msgId,const char* mail_full_name);
comm_boolean mail_sendMsgByMsgId(comm_int32 accountId ,comm_uint32 msgId);

comm_boolean mail_MailListSort(int folderId,
                                        uint8 var_offset, 
                                        BOOLEAN big_to_small);
int mail_getMailNum(MMIMAIL_MAIL_SUMMERY_T* mail_list);
MMIMAIL_MAIL_SUMMERY_T* mail_getStartMailByIndex(int index,
                                                MMIMAIL_MAIL_SUMMERY_T* mail_list);
comm_boolean mail_saveMailToDraft(comm_int32 account_id,MAIL_MESSAGE_T* message_ptr);
void mail_sendOkHandle(mail_retr_result send_result);
void mail_BackToFactory(void);
comm_boolean mail_updateMailState(comm_uint32 msgId);
comm_boolean mail_updateMailStateInMailList(MailMessageFolderInfo *  mail_info,
                                                  MMIMAIL_MAIL_SUMMERY_T** mail_list);
void mail_deleteAccountMsgForUi(comm_int32 account_id);
void mail_deleteAllAccountMsgForUi(void);
int mail_delAccountMessageByMsgId(comm_uint32 msgId,comm_int32 account_id);
int mail_delAllAccountMessageByMsgId(comm_uint32 msgId);
comm_boolean mail_freeMailList(MMIMAIL_MAIL_SUMMERY_T** mail_list);
int mail_delMessageByMsgId(comm_uint32 msgId);
int mail_updateOutMailList(comm_uint32 msgId);
void deleteMail(comm_uint32 msg_id);
void finalTerminate(void);
comm_boolean mail_fldrmgrSetModifyTime(comm_uint32 msgId, comm_uint32 value);
void mail_InboxSort(void);
comm_boolean mail_fldrmgrSetSendTime(comm_uint32 msgId, comm_uint32 value);
PUBLIC comm_uint16 mail_GetDefaultAccountUnReadMsgNum(void);
PUBLIC BOOLEAN mail_GetIsWritingMmtFile(void);
PUBLIC void mail_SetIsWritingMmtFile(BOOLEAN value, eAsyncOperationHandle Oper_save);
PUBLIC void mail_CloseWritingMmtFile(void);

void mail_fldrSysInit(void);

int getIndexFromMasterTable(comm_uint32 msgId);
PUBLIC BOOLEAN  mail_IsMsgIntegrated(comm_uint16 msg_id);
#endif 

