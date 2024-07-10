/*==============================================================================
    FILE NAME   : EMS_Store_API.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        A set of interface functions define for email storage mechanism. 

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-21   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EMS_STORE_API_H
#define EMS_STORE_API_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "EMS_Msg_API.h"

#define EVT_EMS_START                    (0x9010)
#define EVT_EMS_DELETEALL_RESULT         (EVT_EMS_START + 1)
#define EVT_EMS_DELETEALL_NOTIFY         (EVT_EMS_START + 2)
#define EVT_EMS_FACTORY_RESET_RESULT     (EVT_EMS_START + 3)
#define EVT_EMS_DELETE_ACCOUNT_RESULT    (EVT_EMS_START + 4)
#define EVT_EMS_UPDATE_STATE_RESULT      (EVT_EMS_START + 5)
#define EVT_EMS_SEARCH_RESULT            (EVT_EMS_START + 6)
#define EVT_EMS_MOVE_RESULT              (EVT_EMS_START + 7)
#define EVT_EMS_RECOVER_MSG_RESULT       (EVT_EMS_START + 8)
#define EVT_EMS_CLEAN_MSG_RESULT         (EVT_EMS_START + 9)

typedef struct 
{
   unsigned int uiTotal;
   unsigned int uiCurr;
   int          iRet;
}StEMSDelAllNotify;

typedef struct
{
	unsigned int msgid;
	unsigned int state;
}StEMSUpdateState;
/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
typedef void* HEMSStore;

/* Message id */
#define EMS_INVALID_MSG_ID          0
typedef unsigned int EMS_MSGID;

/* Account id */
#define EMS_ACCOUNT_MAX_ID          0x0F
#define EMS_ACCOUNT_ID_ALL          0x00
typedef unsigned int EMS_ACTID;

/* Box id */
#define EMS_BOX_MAX_ID              0x0F
#define EMS_BOX_ID_ALL              0x00
typedef unsigned int EMS_BOXID;


#define EMS_ACTINFO_MAX_NUMBER      0x0F
#define EMS_BOXINFO_MAX_NUMBER      0x0F
#define EMS_MSGINFO_MAX_NUMBER      0x0FFF


/*asyn type类型是用来获取通过Store接口异步操作的数据*/
#define EMS_ASYN_TYPE_INVAILED         0x00
#define EMS_ASYN_TYPE_MULTI_MSG_DEL    0x01
#define EMS_ASYN_TYPE_MULTI_ACT_DEL    0x02
#define EMS_ASYN_TYPE_MULTI_MSG_MOVE   0x03
#define EMS_ASYN_TYPE_FACTORY_RESET    0x04
#define EMS_ASYN_TYPE_STATE_UPDATE     0x05
#define EMS_ASYN_TYPE_SEARCH_MSG       0x06
#define EMS_ASYN_TYPE_RECOVER_MSG      0x07
#define EMS_ASYN_TYPE_CLEAN_MSG        0x08
typedef unsigned int EMS_ASYNTYPE;

#define EMS_PRFITEM_ACCOUNT_NAME                (WEMSG_OPT_WSZ + 0x01)  /*wchar*/
#define EMS_PRFITEM_ACCOUNT_EMAIL               (WEMSG_OPT_WSZ + 0x02)  /*wchar*/
#define EMS_PRFITEM_REPLY_TO_EMAIL              (WEMSG_OPT_WSZ + 0x03)  /*wchar*/
#define EMS_PRFITEM_EMAIL_USER_ID               (WEMSG_OPT_WSZ + 0x04)  /*wchar*/
#define EMS_PRFITEM_EMAIL_PASSWORD              (WEMSG_OPT_WSZ + 0x05)  /*wchar*/
#define EMS_PRFITEM_SMTP_USER_ID                (WEMSG_OPT_WSZ + 0x06)  /*wchar*/
#define EMS_PRFITEM_SMTP_PASSWORD               (WEMSG_OPT_WSZ + 0x07)  /*wchar*/
#define EMS_PRFITEM_POP3_SERVER                 (WEMSG_OPT_WSZ + 0x08)  /*wchar*/
#define EMS_PRFITEM_POP3_PORT                   (WEMSG_OPT_WSZ + 0x09)  /*wchar*/
#define EMS_PRFITEM_IMAP4_SERVER                (WEMSG_OPT_WSZ + 0x0A)  /*wchar*/
#define EMS_PRFITEM_IMAP4_SERVER_PORT           (WEMSG_OPT_WSZ + 0x0B)  /*wchar*/
#define EMS_PRFITEM_SMTP_SERVER                 (WEMSG_OPT_WSZ + 0x0C)  /*wchar*/
#define EMS_PRFITEM_SMTP_SERVER_PORT            (WEMSG_OPT_WSZ + 0x0D)  /*wchar*/
#define EMS_PRFITEM_MAX_MSG_SIZE                (WEMSG_OPT_WSZ + 0x0E)  /*wchar*/
#define EMS_PRFITEM_USER_NAME                   (WEMSG_OPT_WSZ + 0x0F)  /*wchar*/
#define EMS_PRFITEM_SENDER_NAME                 (WEMSG_OPT_WSZ + 0x10)  /*wchar*/
#define EMS_PRFITEM_SET_PASSWORD_PROTECTION     (WEMSG_OPT_WSZ + 0x12)  /*wchar*/

#define EMS_PRFITEM_EMAIL_CREATE_SIGNATURE      (WEMSG_OPT_SZ + 0x01)   /*char*/
#define EMS_PRFITEM_EMAIL_UIDLFILEDIR           (WEMSG_OPT_SZ + 0x02)   /*char*/
#define EMS_PRFITEM_EMAIL_UIDFILEDIR            (WEMSG_OPT_SZ + 0x03)   /*char*/
#define EMS_PRFITEM_APN_TYPE                    (WEMSG_OPT_SZ + 0x04)   /*char*/

#define EMS_PRFITEM_DATA_SERVICE_PROVIDER       (WEMSG_OPT_32BIT + 0x01)  /*uint32*/
#define EMS_PRFITEM_RECEIVE_SERVER_TYPE         (WEMSG_OPT_32BIT + 0x02)  /*uint32*/
#define EMS_PRFITEM_EMAIL_DOWNLOAD_MODE         (WEMSG_OPT_32BIT + 0x03)  /*uint32*/
#define EMS_PRFITEM_EMAIL_ADD_SIGNATURE_MODE    (WEMSG_OPT_32BIT + 0x04)  /*uint32*/
#define EMS_PRFITEM_EMAIL_DEFAULTINBOXID        (WEMSG_OPT_32BIT + 0x05)  /*uint32*/
#define EMS_PRFITEM_POP3_SSL_OR_TLS             (WEMSG_OPT_32BIT + 0x06)  /*bool*/
#define EMS_PRFITEM_IMAP4_SSL_OR_TLS            (WEMSG_OPT_32BIT + 0x07)  /*bool*/
#define EMS_PRFITEM_SMTP_SSL_OR_TLS             (WEMSG_OPT_32BIT + 0x08)  /*bool*/
#define EMS_PRFITEM_SMTP_AUTHENTICATE           (WEMSG_OPT_32BIT + 0x09)  /*bool*/
#define EMS_PRFITEM_NEW_MSG_NOTIFICATION        (WEMSG_OPT_32BIT + 0x0A)  /*bool*/
#define EMS_PRFITEM_DELETE_LEAVE_COPY           (WEMSG_OPT_32BIT + 0x0B)  /*bool*/
#define EMS_PRFITEM_SAVE_SENT                   (WEMSG_OPT_32BIT + 0x0C)  /*bool*/
#define EMS_PRFITEM_INCLUDE_ORIGINAL            (WEMSG_OPT_32BIT + 0x0D)  /*bool*/
#define EMS_PRFITEM_READ_RECEIPT                (WEMSG_OPT_32BIT + 0x0E)  /*bool*/
#define EMS_PRFITEM_RECEIVE_UNSEEN              (WEMSG_OPT_32BIT + 0x0F)  /*boolean*/
#define EMS_PRFITEM_DOWNLOAD_ATTACH             (WEMSG_OPT_32BIT + 0x10)  /*boolean*/
#define EMS_PRFITEM_ACCOUNT_ID                  (WEMSG_OPT_32BIT + 0x11)  /*boolean*/
#define EMS_PRFITEM_RECEIVER_LEAVE_COPY         (WEMSG_OPT_32BIT + 0x12)  /*boolean*/
#define EMS_PRFITEM_ACCOUNT_RAND                (WEMSG_OPT_32BIT + 0x13)  /*uint32*/
#define EMS_PRFITEM_SAVE_PLACE                  (WEMSG_OPT_32BIT + 0x14)  /*uint32*/
#define EMS_PRFITEM_USER1_PROFILE               (WEMSG_OPT_32BIT + 0x15)
#define EMS_PRFITEM_USER2_PROFILE               (WEMSG_OPT_32BIT + 0x16)
#define EMS_PRFITEM_USER3_PROFILE               (WEMSG_OPT_32BIT + 0x17)
#define EMS_PRFITEM_NUMBER_LIMIT                (WEMSG_OPT_32BIT + 0x18)  /*uint32*/
#define EMS_PRFITEM_NET_PATH                    (WEMSG_OPT_32BIT + 0x19)  /*boolean*/
#define EMS_PRFITEM_POP3_PORT_DEFAULT           (WEMSG_OPT_32BIT + 0x1A)  /*boolean*/
#define EMS_PRFITEM_IMAP4_PORT_DEFAULT          (WEMSG_OPT_32BIT + 0x1B)  /*boolean*/
#define EMS_PRFITEM_SMTP_PORT_DEFAULT           (WEMSG_OPT_32BIT + 0x1C)  /*boolean*/
#define EMS_PRFITEM_SEND_MAIL                   (WEMSG_OPT_32BIT + 0x1D)  /*boolean*/
#define EMS_PRFITEM_RECEIVE_FREQUENCY           (WEMSG_OPT_32BIT + 0x1E)  /*boolean*/
#define EMS_PRFITEM_RECEIVE_ONLY_NEW_MAIL       (WEMSG_OPT_32BIT + 0x1F)  /*boolean*/
#define EMS_PRFITEM_MAIL_AUTO_DELETE            (WEMSG_OPT_32BIT + 0x20)  /*boolean*/
#define EMS_PRFITEM_MAIL_DELETE                 (WEMSG_OPT_32BIT + 0x21)  /*boolean*/
#define EMS_PRFITEM_NEW_MAIL_TIP                (WEMSG_OPT_32BIT + 0x22)  /*boolean*/
#define EMS_PRFITEM_STARTUP_SCREEN              (WEMSG_OPT_32BIT + 0x23)  /*boolean*/
#define EMS_PRFITEM_ACTIVE_ACCOUNTID            (WEMSG_OPT_32BIT + 0x24)  /*uint32*/
#define EMS_PRFITEM_TONE                        (WEMSG_OPT_32BIT + 0x25)  /*uint32*/
#define EMS_PRFITEM_TONE_VOLUME                 (WEMSG_OPT_32BIT + 0x26)  /*uint32*/
#define EMS_PRFITEM_SLIENT_TIME                 (WEMSG_OPT_32BIT + 0x27)  /*uint32*/
#define EMS_PRFITEM_ACCOUNT_PASSWORD_PROTECTION (WEMSG_OPT_32BIT + 0x28)  /*boolean*/
#define EMS_PRFITEM_DEFAULT_ACC                 (WEMSG_OPT_32BIT + 0x29)  /*wchar*/
#define EMS_PRFITEM_STORAGE_STATISTICS          (WEMSG_OPT_32BIT + 0x2A)  /*uint32*/
#define EMS_PRFITEM_FLOW_STATISTICS             (WEMSG_OPT_32BIT + 0x2B)  /*uint32*/
#define EMS_PRFITEM_MAIL_AUTO_DELETE_TIME       (WEMSG_OPT_32BIT + 0x2C)  /*uint32*/
#define EMS_PRFITEM_SLIENT_TIME_BEGIN           (WEMSG_OPT_32BIT + 0x2D)  /*uint32*/
#define EMS_PRFITEM_SLIENT_TIME_END             (WEMSG_OPT_32BIT + 0x2E)  /*uint32*/
typedef unsigned int EMS_PRFITEM_ID;

/* EMS_PRFITEM_EMAIL_DOWNLOAD_MODE */
#define EMS_DOWNLOAD_HEADER_ONLY             0x00000001
#define EMS_DOWNLOAD_WHOLE_MAIL              0x00000002
#define EMS_DOWNLOAD_ASK_USER                0x00000003
typedef unsigned int EMS_MAIL_DOWNLOAD_MODE;


/* EMS_PRFITEM_EMAIL_ADD_SIGNATURE_MODE */
#define EMS_ADD_SIGNATURE_ALL_MAIL           0x00000001
#define EMS_ADD_SIGNATURE_NEW_MAIL           0x00000002
#define EMS_ADD_SIGNATURE_NO_MAIL            0x00000003
typedef unsigned int EMS_SIG_ADD_MODE;

/* EMS_PRFITEM_RECEIVE_SERVER_TYPE*/
#define EMS_RECEIVE_SERVER_TYPE_POP3         0x00000001
#define EMS_RECEIVE_SERVER_TYPE_IMAP4        0x00000002

/* EMS_PRFITEM_SAVE_PLACE*/
#define EMS_SAVE_PLACE_UDISK                 0x00000001
#define EMS_SAVE_PLACE_SDCARD1               0x00000002
#define EMS_SAVE_PLACE_SDCARD2               0x00000003

/* For account config */
typedef struct EMS_PRFITEM_VAL
{
   unsigned int   size;
   void           *pData;
} EMS_PRFITEM_VAL;


#define EMS_ACCOUNT_NAME_MAX_LEN       255  
typedef struct EMS_ActInfo
{
   EMS_ACTID      uiActId;
   unsigned short wazActName[EMS_ACCOUNT_NAME_MAX_LEN + 1];
   unsigned int   dwActSize;
}EMS_ActInfo;


#define EMS_BOX_NAME_MAX_LEN           255
typedef struct EMS_BoxInfo
{
   EMS_BOXID      uiBoxId;
   unsigned int   uiTotal;    /* All Messages number */
   unsigned int   uiUnRead;   /* UnReaded messages number */
   unsigned short wszFolder[EMS_BOX_NAME_MAX_LEN + 1];
   unsigned char  ucState;
   unsigned int   dwUDSize;   /*udisk size*/
   unsigned int   dwSD1Size;  /*sd card 1 size*/
   unsigned int   dwSD2Size;  /*sd card 2 size*/
}EMS_BoxInfo;


#define EMS_EMAIL_ADDRESS_MAX_LENGTH   255 
#define EMS_SUBJECT_MAX_LENGTH         255
typedef struct EMS_MsgInfo
{
   EMS_ACTID      dwActId;
   EMS_BOXID      dwBoxId;
   EMS_BOXID      dwBoxFrom;
   EMS_MSGID      dwMsgId;
   EMSMSG_TYPE    dwEmlType;
   EMSMSG_STATUS  dwEmlStatus;
   unsigned int   dwDate;
   unsigned int   dwReceDate;
   unsigned short wszSubject[EMS_SUBJECT_MAX_LENGTH + 1]; 
   unsigned short wszFrom[EMS_EMAIL_ADDRESS_MAX_LENGTH + 1];
   unsigned short wszTo[EMS_EMAIL_ADDRESS_MAX_LENGTH + 1];
   unsigned int   dwMsgSize;
   unsigned int   dwMsgPrio;
}EMS_MsgInfo;

#define EMS_SERCONF_STR_MAX_LEN        255
#define EMS_SERCONF_PORT_MAX_LEN       5
typedef struct EMS_ServerConf
{
   unsigned short wszActName[EMS_SERCONF_STR_MAX_LEN + 1];
   unsigned short wszServerName[EMS_SERCONF_STR_MAX_LEN + 1];
   unsigned short wszSmtpServer[EMS_SERCONF_STR_MAX_LEN + 1];
   unsigned short wszSmtpPort[EMS_SERCONF_PORT_MAX_LEN + 1];
   unsigned short wszPop3Server[EMS_SERCONF_STR_MAX_LEN + 1];
   unsigned short wszPop3Port[EMS_SERCONF_PORT_MAX_LEN + 1];
   unsigned short wszImapServer[EMS_SERCONF_STR_MAX_LEN + 1];
   unsigned short wszImapPort[EMS_SERCONF_PORT_MAX_LEN + 1];
   unsigned int   uiServerType;
   unsigned char  bSmtpSSL;
   unsigned char  bPop3SSL;
   unsigned char  bImapSSL;
   unsigned char  bSmtpAuth;
   unsigned char sim1NetType;
   unsigned char sim2NetType;
   unsigned char sim3NetType;
   unsigned char sim4NetType;
}EMS_ServerConf;

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/

HEMSStore HEMSSTORE_New(void *pFileMgr, 
                        void *pvMemMgr);

void HEMSSTORE_Delete(HEMSStore hStore);

int HEMSSTORE_StoreMsg(HEMSStore hStore, 
                       HEMSMsg hMsg);

int HEMSSTORE_GetMsg(HEMSStore hStore, 
                     EMS_MSGID uiMsgId, 
                     HEMSMsg *phMsg);

int HEMSSTORE_UpdateMsg(HEMSStore hStore, 
                        HEMSMsg hMsg);

int HEMSSTORE_RemoveMsg(HEMSStore hStore, 
                        EMS_MSGID uiMsgId);

int HEMSSTORE_RemoveMultiMsg(HEMSStore    hStore, 
                             EMS_MSGID    *puiMsglist, 
                             unsigned int uiNum);

void HEMSSTORE_CancelRemoveMultiMsg(HEMSStore hStore);

int HEMSSTORE_CreateAccount(HEMSStore        hStore, 
                            unsigned short   *pwszActName, 
                            EMS_ACTID        *puiActId);

int HEMSSTORE_GetActInfo(HEMSStore        hStore, 
                         unsigned int     uiStartPos, 
                         unsigned int     *puiNum, 
                         EMS_ActInfo      *aActInfo);

int HEMSSTORE_DeleteAccount(HEMSStore hStore, 
                            EMS_ACTID uiActId);

int HEMSSTORE_DeleteMultiAccount(HEMSStore hStore, 
                                 EMS_ACTID *puiActId, 
                                 WE_UINT32 uiNum);

int HEMSSTORE_SetActCfgItem(HEMSStore        hStore, 
                            EMS_ACTID        uiActId,
                            EMS_PRFITEM_ID   uiItem,
                            EMS_PRFITEM_VAL  *pItemVal);

int HEMSSTORE_SetActCfgItemEx(HEMSStore        hStore, 
                            EMS_ACTID        uiActId,
                            EMSMsgOpt       *pstOpt);

int HEMSSTORE_GetActCfgItem(HEMSStore        hStore, 
                            EMS_ACTID        uiActId, 
                            EMS_PRFITEM_ID   uiItem, 
                            EMS_PRFITEM_VAL  *pItemVal);

int HEMSSTORE_CreateBox(HEMSStore      hStore, 
                        unsigned short *pwszBoxName, 
                        EMS_BOXID      *puiBoxId, 
                        EMS_ACTID      uiActId, 
                        unsigned char  ucState);

int HEMSSTORE_DeleteBox(HEMSStore      hStore, 
                        EMS_BOXID      uiBoxId, 
                        EMS_ACTID      uiActId);

int HEMSSTORE_GetBoxInfo(HEMSStore     hStore, 
                         EMS_ACTID     uiActId, 
                         unsigned int  uiStartPos, 
                         unsigned int  *puiNum, 
                         EMS_BoxInfo   *aBoxInfo);

int HEMSSTORE_MoveMsg(HEMSStore  hStore, 
                      EMS_ACTID  uiActId, 
                      EMS_BOXID  uiSrcBoxId, 
                      EMS_BOXID  uiDestBoxId, 
                      EMS_MSGID  uiMsgId);

int HEMSSTORE_MoveMultiMsg(HEMSStore     hStore, 
                           EMS_ACTID     uiActId,
                           EMS_BOXID     uiSrcBoxId,
                           EMS_BOXID     uiDesBoxId,
                           EMS_MSGID     *puiMsglist,
                           unsigned int  uiNum);

int HEMSSTORE_GetMsgInfo(HEMSStore     hStore, 
                         EMS_ACTID     uiActId, 
                         EMS_BOXID     uiBoxId, 
                         unsigned int  uiStartPos, 
                         unsigned int  *puiNum, 
                         EMS_MsgInfo   *aMsgInfo);

int HEMSSTORE_GetTotalNum(HEMSStore     hStore, 
                          EMS_ACTID     uiActId,
                          unsigned int  *puiTotalNum);
                          
int HEMSSTORE_SearchMsgInfo(HEMSStore hStore, 
                            EMS_ACTID uiActId, 
                            unsigned short *pwcFilte, 
                            EMS_BOXID uiBoxId);

EMS_MSGID HEMSSTORE_CheckMsgId(HEMSStore hStore, 
                               EMS_ACTID uiActId, 
                               EMS_MSGID uiMsgId);

int HEMSSTORE_GetServerConf(HEMSStore hStore, 
                            unsigned int *puiNum, 
                            EMS_ServerConf *pstConInfo);

unsigned char HEMSSTORE_CheckAccountIsOK(HEMSStore hStore, 
                                         EMS_ACTID dwActId);

int HEMSSTORE_SetActiveActId(HEMSStore hStore, 
                             EMS_ACTID dwActId);

int HEMSSTORE_GetActiveActId(HEMSStore hStore, 
                             EMS_ACTID *pdwActId);

void HEMSSTORE_FactoryReset(HEMSStore hStore);

void HEMSSTORE_SyncMsgIndexInfo(void);

int HEMSSTORE_UpdateMsgHeader(HEMSStore hStore, 
                              HEMSMsg hMsg);

int HEMSSTORE_UpdateMsgState(HEMSStore hStore, 
                             StEMSUpdateState *pstMsgState, 
                             unsigned int uiNum);

void HEMSSTORE_GetAsynData(HEMSStore hStore, 
                           EMS_ASYNTYPE asyn, 
                           WE_VOID** ppData, 
                           WE_UINT32 *pdwNum);

int HEMSSTORE_RecoverMsg(HEMSStore hStore, 
                         EMS_ACTID uiActId, 
                         EMS_MSGID *puiMsgList, 
                         unsigned int uiMsgNum);

int HEMSSTORE_CleanMsg(HEMSStore hStore, 
                        EMS_ACTID uiActId, 
                        EMS_BOXID uiBoxId, 
                        unsigned int uiSaveplace);

void HEMSSTORE_ClearPathMgr(void);
/*------------------------------------------------------------------------------
 *    Function Documentation
 *----------------------------------------------------------------------------*/
/*=============================================================================
Function: HSTORE_StoreMsg

Description: Stores the message into the Storage.

Prototype:
int HSTORE_StoreMsg(HStore hStore, HMsg hMsg);


Parameters:

hStore   [in]: The HStore handler.
hMsg [in/out]: The message that has to be stored, upon return the
               message would contain a valid message id.

Return Value:

EME_SUCCESS     : If we are able to save the message.
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_GetMsgs

Description: This would retrieve all the message from the store, for the
             specified property and value.

Prototype:
int HSTORE_GetMsgs(HStore        hStore, 
                   EMSSTORE_PROP uiPropId, 
                   void          *pVal, 
                   EMS_ACTID     uiActId, 
                   HMsg          *ahMsg);

Parameters:

hStore      [in]: The HStore handler.
uiPropId    [in]: The property id for which we are querying for data.
pVal        [in]: The property val.
uiActId     [in]: The account id which we are querying from.
HMsg       [out]: HMsg handler array.

Return Value:
EME_SUCCESS : If we are able to get the messages.
EME_FAILED  : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_UpdateMsg

Description: Updates a message in the store.

Prototype:
int HSTORE_UpdateMsg(HStore hStore, HMsg *phMsg);

Parameters:
hStore      [in]: The HStore handler.
phMsg       [in]: The message that has to be updated.

Return Value:

EME_SUCCESS     : If we are successfully update the message
EME_FAILED      : Otherwise.

Comments:
The HMsg that is passed should have been retrieved previously by using
Get/Store message.
=============================================================================*/
/*=============================================================================
Function: HSTORE_RemoveMsgs

Description: This would remove the messages, specified by the property id and
val specified.

Prototype:
int HSTORE_RemoveMsgs(HStore        hStore, 
                      EMSSTORE_PROP uiPropId, 
                      void          *pVal, 
                      EMS_ACTID     uiActId);

Parameters:
hStore   [in]: The HStore handler.
uiPropId [in]: The property id for which we are trying to delete the data.
pVal     [in]: The property val.
uiActId  [in]: The account id which we are trying to delete from.

Return Value:
EME_SUCCESS     : If we are able to remove the message
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_CreateAccount

Description:
create a new account.

Prototype:
int HSTORE_CreateAccount(HStore           hStore, 
                         unsigned short   *pwszActName, 
                         EMS_ACTID        *puiActId);

Parameters:
hStore      [in]: The HStore handler.
pwszActName [in]: new account name
pdwActId   [out]: Account id

Return Value:
EME_SUCCESS     : If we are successfully create the account
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_GetActInfo

Description:
Get account informations.

Prototype:
int HSTORE_GetActInfo(HStore           hStore, 
                      unsigned int     uiStartPos, 
                      unsigned int     *puiNum, 
                      EMS_ActInfo      *aActInfo);

Parameters:
hStore      [in]: The HStore handler.
uiStartPos  [in]: start position
puiNum  [in/out]: Account number want to get
apActInfo  [out]: Account information structures vector

Return Value:
EME_SUCCESS     : If we are able to get the account info
EME_FAILED      : Otherwise.

Comments: If the param apActInfo == NULL, the puiNum will return the 
          total act number 

=============================================================================*/
/*=============================================================================
Function: HSTORE_DeleteAccount

Description:
Delete the account by account id.

Prototype:
int HSTORE_DeleteAccount(HStore hStore, EMS_ACTID uiActId);

Parameters:

hStore      [in]: The HStore handler.
uiActId     [in]: Account id which want to be deleted

Return Value:
EME_SUCCESS     : If we delete the account successful.
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_SetActCfgItem

Description: This would set the account info specified by the profile item.

Prototype:
int HSTORE_SetActCfgItem(HStore           hStore, 
                         EMS_ACTID        uiActId,
                         EMS_PRFITEM_ID   uiItem,
                         EMS_PRFITEM_VAL  *pItemVal);

Parameters:

hStore      [in]: The HStore handler.
uiActId     [in]: The account id which want to set info.
uiItem      [in]: The profile item.
pItemVal    [in]: The profile item val.

Return Value:
EME_SUCCESS     : If successfully set the config item
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_GetActCfgItem

Description: This would get the account info specified by the profile item.

Prototype:
int HSTORE_GetActCfgItem(HStore           hStore, 
                         EMS_ACTID        uiActId, 
                         EMS_PRFITEM_ID   uiItem, 
                         EMS_PRFITEM_VAL  *pItemVal);

Parameters:

hStore      [in]: The HStore handler.
uiActId     [in]: The account id which want to get the info.
uiItem      [in]: The profile item.
pItemVal   [out]: The profile item val.

Return Value:
EME_SUCCESS     : If we are able to get the config item
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_CreateBox

Description:
Creates a new Box in the MBox table.

Prototype:
int HSTORE_CreateBox(HStore         hStore, 
                     unsigned short *pwszFolderName, 
                     EMS_BOXID      *puiBoxId, 
                     EMS_ACTID      uiActId);


Parameters:

hStore         [in]: The HStore handler.
pwszFolderName [in]: The folder name for which the user wants to create a new folder.
puiBoxId      [out]: The newly created folder id, which can be used to 
                     update the msg and move the msgs to different folders.
uiActId        [in]: The account id in which the folder want to create.

Return Value:
EME_SUCCESS     : If we are able to Create the box
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_DeleteBox

Description:
This would delete the folder identified by the folder id, account id .
It also deletes all the message in that folder (i.e., all the messages with that folder
id attribute).

Prototype:
int HSTORE_DeleteBox(HStore         hStore, 
                     EMS_BOXID      uiBoxId, 
                     EMS_ACTID      uiActId);

Parameters:
hStore      [in]: The HStore handler.
uiBoxId     [in]: The Folder id which has to be deleted.
uiActId     [in]: The account id.

Return Value:
EME_SUCCESS     : If we are able to deltet the box
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_GetBoxInfo

Description:
Get box information from ind file by start position and count.

Prototype:
int HSTORE_GetBoxInfo(HStore        hStore, 
                      EMS_ACTID     uiActId, 
                      unsigned int  uiStartPos, 
                      unsigned int  uiNum, 
                      EMS_BoxInfo   *aBoxInfo);

Parameters:
hStore      [in]: The HStore handler.
uiActId     [in]: account id
uiStartPos  [in]: start position
uiNum       [in]: info number want to get
aBoxInfo   [out]: box information structures array

Return Value:
EME_SUCCESS     : If we are able to get the box info
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_MoveMsg

Description:
Move message from original box to destination box

Prototype:
int HSTORE_MoveMsg(HStore     hStore, 
                   EMS_ACTID  uiActId, 
                   EMS_BOXID  uiSrcBoxId, 
                   EMS_BOXID  uiDestBoxId, 
                   EMS_MSGID  uiMsgId);

Parameters:
hStore      [in]: The HStore handler.
uiActId     [in]: Original account id
uiSrcBoxId  [in]: Original box id.
uiDestBoxId [in]: destination box id.
uiMsgId     [in]: message id.

Return Value:
EME_SUCCESS     : If successful delete this box
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
/*=============================================================================
Function: HSTORE_GetMsgInfo

Description:
Get message's basic information.

Prototype:
int HSTORE_GetMsgInfo(HStore        hStore, 
                      EMS_ACTID     uiActId, 
                      EMS_BOXID     uiBoxId, 
                      unsigned int  uiStartPos, 
                      unsigned int  uiNum, 
                      HMsgInfo      *ahMsgInfo);

Parameters:
hStore      [in]: The HStore handler.
uiActId     [in]: account id
uiBoxId     [in]: box id
uiStartPos  [in]: start position
uiNum       [in]: message number want to get
ahMsgInfo  [out]: HMsgInfo handler array.

Return Value:
EME_SUCCESS     : If we are able to get the box info
EME_FAILED      : Otherwise.

Comments:

=============================================================================*/
#endif /* EMS_STORE_API_H */
