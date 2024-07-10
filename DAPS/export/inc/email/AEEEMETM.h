/*=====================================================================================
    FILE NAME   : AEEEMETM.h
    MODULE NAME : Email engine
      
    GENERAL DESCRIPTION
      Supply AEE api for SMTP, POP, IMAP and EMN transaction.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                create AEEEMETM.h

=====================================================================================*/

#ifndef _AEE_EME_TM_H
#define _AEE_EME_TM_H

#include "we_def.h"

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
 * Macro Define Section
==================================================================================================*/
#define EVT_EMAIL (0x9000)

/*========================================Email option start==========================================*/
#define EME_OPTID_END                            0x00
#define EME_OPTID_INT_START                      0x01000000
#define EME_OPTID_STR_START                      0x02000000
#define EME_OPTID_TRANS_PARAM                    0x00001000
#define EME_OPTID_ACCOUNT_INFO                   0x00002000

/************** transaction inform ******************/
/*int*/
#define EME_OPTID_TRANS_TYPE                     (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x01)     /* value type: int32 */
#define EME_OPTID_REPLY_CLSID                    (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x02) 
#define EME_OPTID_REPLY_USERDATA                 (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x03)
#define EME_OPTID_EMN_BODYDATALEN                (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x04) 
#define EME_OPTID_EMN_CONTENTTYPE                (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x05) 
#define EME_OPTID_EMN_CHARSETTYPE                (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x06)
#define EME_OPTID_CHECK_PASSWORD                 (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x07)
#define EME_OPTID_EPOP_SET_OBJ                   (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x08)
#define EME_OPTID_CONN_NETID                     (EME_OPTID_INT_START + EME_OPTID_TRANS_PARAM + 0x09)

/*str*/
#define EME_OPTID_PART_ID                        (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x01)     /* value type: char* , string Length less thans 10 */
#define EME_OPTID_MESSAGE_ID                     (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x02)     /* value type: char*                    */
#define EME_OPTID_LOGIN_MAILBOXNAME              (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x03)     /* value type: char*                    */
#define EME_OPTID_HANDLE_MAILBOXNAME             (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x04)
#define EME_OPTID_EMN_BODYDATA                   (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x05)
#define EME_OPTID_MESSAGE_UID                    (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x06)
#define EME_OPTID_EPOP_SET_STRING                (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x07)   /* value type: char*          */ 
#define EME_OPTID_EPOP_SET_FILTER                (EME_OPTID_STR_START + EME_OPTID_TRANS_PARAM + 0x08)   /* value type: char*          */ 

/************** account inform ***************/
/*int*/
#define EME_OPTID_ACCOUNT_ID                     (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x01)   /* value type: uint32         */    
#define EME_OPTID_DEFAULT_FOLDER_ID              (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x02)   /* value type: uint32         */
#define EME_OPTID_POP_KEEP_MAIL                  (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x03)   /* value type: int32          */
#define EME_OPTID_SMTP_NEED_SSL                  (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x04)   /* value type: int32         */  
#define EME_OPTID_POP_NEED_SSL                   (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x05)   /* value type: int32         */   
#define EME_OPTID_IMAP_NEED_SSL                  (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x06)   /* value type: int32         */    
#define EME_OPTID_SIZE_LIMIT                     (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x07)   /* value type: uint32         */
#define EME_OPTID_NUM_LIMIT                      (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x08)   /* value type: uint32         */
#define EME_OPTID_RECEIVE_SERVER_PORT            (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x09)   /* value type: uint32         */
#define EME_OPTID_SEND_SERVER_PORT               (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x0A)   /* value type: uint32         */
#define EME_OPTID_NETWORK_UMTS_PDP               (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x0B)
#define EME_OPTID_SMTP_AUTHENTICATE              (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x0C)
#define EME_OPTID_RECEIVE_LATEST                 (EME_OPTID_INT_START + EME_OPTID_ACCOUNT_INFO + 0x0D)   /* value type: uint32         */

/*str*/
#define EME_OPTID_RECEIVE_SERVER_ADDR            (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x01)   /* value type: char*          */
#define EME_OPTID_RECEIVE_USERNAME               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x02)   /* value type: char*          */
#define EME_OPTID_RECEIVE_PASSWORD               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x03)   /* value type: char*          */  
#define EME_OPTID_SEND_SERVER_ADDR               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x04)   /* value type: char*          */   
#define EME_OPTID_SEND_EMAIL_ADDR                (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x05)   /* value type: char*          */  
#define EME_OPTID_SEND_USERNAME                  (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x06)   /* value type: char*          */
#define EME_OPTID_SEND_PASSWORD                  (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x07)   /* value type: char*          */  
#define EME_OPTID_UIDL_FILE_POP                  (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x08)   /* value type: char*          */  
#define EME_OPTID_UID_FILE_IMAP                  (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x09)   /* value type: char*          */ 

#define EME_OPTID_EPOP_CONVERTCHARSET            (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x0A)   /* value type: char*          */ 
#define EME_OPTID_EPOP_USECOMPRESS               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x0B)   /* value type: char*          */ 
//#define EME_OPTID_EPOP_CONTENTID                 (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x0C)   /* value type: char*          */ 
#define EME_OPTID_EPOP_CONVERTTYPE               (EME_OPTID_STR_START + EME_OPTID_ACCOUNT_INFO + 0x0D)   /* value type: char*          */ 

/*=========================================Email option end==============================================*/
/*************** TRANS TYPE *********************/
/* SMTP */
#define EME_TRANS_SMTP_SEND_MAIL                         0x01
#define EME_TRANS_SMTP_SEND_RECEIPT                      0x02
#define EME_TRANS_SMTP_FORWARD_WHOLEMAIL                 0x03
#define EME_TRANS_SMTP_FORWARD_ATTACHMENT                0x04
   
/* POP3 */
#define EME_TRANS_POP3_FETCH_NEW_MAILS_HEADER            0x11
#define EME_TRANS_POP3_FETCH_NEW_MAILS                   0x12
#define EME_TRANS_POP3_FETCH_SPEC_MAIL                   0x13
#define EME_TRANS_POP3_DELETE_SPEC_MAIL                  0x14
#define EME_TRANS_POP3_DELETE_ALL_MAIL                   0x15

/*EPOP*/
#define EME_TRANS_EPOP_FETCH_NEW_MAILS_HEADER            0x16
#define EME_TRANS_EPOP_FETCH_NEW_MAILS                   0x17
#define EME_TRANS_EPOP_FETCH_SPEC_MAIL                   0x18
#define EME_TRANS_EPOP_FETCH_SPEC_MAIL_PART              0x19
#define EME_TRANS_EPOP_SET_PUSH_MAIL    	             0x1a
#define EME_TRANS_EPOP_GET_PUSH_MAIL_FILTER	             0x1b
#define EME_TRANS_CHECK_PASSWORD			             0x20

/* IMAP4 */
#define EME_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER           0x21
#define EME_TRANS_IMAP4_FETCH_NEW_MAILS                  0x22
#define EME_TRANS_IMAP4_FETCH_SPEC_MAIL                  0x23
#define EME_TRANS_IMAP4_FETCH_SPEC_MAIL_PART             0x24
#define EME_TRANS_IMAP4_MARK_DEL_ON_SERVER               0x25
#define EME_TRANS_IMAP4_UNMARK_DEL_ON_SERVER             0x26
#define EME_TRANS_IMAP4_MARK_SEEN_ON_SERVER              0x27
#define EME_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER            0x28
#define EME_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX             0x29
#define EME_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER            0x2A
#define EME_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX      0x2B
#define EME_TRANS_IMAP4_CREAT_MAILBOX                    0x2C
#define EME_TRANS_IMAP4_RENAME_MAILBOX                   0x2D
#define EME_TRANS_IMAP4_DELETE_MAILBOX                   0x2E
#define EME_TRANS_IMAP4_SUBSCRIBE_MAILBOX                0x2F
#define EME_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX              0x30
#define EME_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES        0x31
#define EME_TRANS_IMAP4_LIST_ALL_MAILBOXES               0x32
 
/*************** Trans Status TYPE *********************/
#define EME_TRANS_STATUS_PENDING         1     //Transaction was attempted to complete by TM but could not complete because of some reasons.              
#define EME_TRANS_STATUS_CANCELED        2
#define EME_TRANS_STATUS_DONE            3
#define EME_TRANS_STATUS_INPROGRESS      4     //Transaction is in progress
#define EME_TRANS_STATUS_MAX             0x7FFFFFFF

/*************** Trans Id *********************/
#define EME_ALL_TRANS 0xffffffff
#define EME_PROGRESS_INDI_MASK 0x80000001

/*************** EMN Type8*********************/
#define EME_CONTENTTEXT_EMN           0x01
#define EME_CONTENTBINARY_EMN         0x02

/*************** EMN Body charset *************/
#define EME_CHARSETUTF8_EMN           106
#define EME_CHARSETUTF16_EMN          1015

/*************** EMN URI Type *****************/
#define EME_URI_MAILAT                 0x01
#define EME_URI_POP                    0x02
#define EME_URI_IMAP                   0x03
#define EME_URI_OTHER                  0x04

/*************** EMN authentication Type *****************/
#define EME_AUTHTYPE_ANY            0x0100
#define EME_AUTHTYPE_POP_PLAIN      0x0101
#define EME_AUTHTYPE_POP_APOP       0x0102


/*********************NOTIFY TYPE ******************/
#define EVT_EME_TRANS_RESULT             (EVT_EMAIL + 0x01) /*EMETransResult*/
#define EVT_EME_NEW_MESSAGE              (EVT_EMAIL + 0x02) /*EMEMsg*/
#define EVT_EME_MAILBOX_GETED            (EVT_EMAIL + 0x03) /*EMEMailbox*/
#define EVT_EME_MESSAGE_SIZE_NOTIFY      (EVT_EMAIL + 0x04) /*EMESizeNotify*/
#define EVT_EME_MESSAGE_NUMBER_NOTIFY    (EVT_EMAIL + 0x05) /*EMENumberNotify*/
#define EVT_EME_INFO_NOTIFY              (EVT_EMAIL + 0x06) /*ascii-string*/      


/*==================================================================================================
* Type Define Section
==================================================================================================*/
typedef WE_VOID* IEMETMHandle;

typedef WE_UINT32 TransID;

typedef struct _AEEEMEOpt
{
   WE_INT32    iId;
   WE_UINT32 type;
   WE_UINT32 size;
   WE_VOID     *pVal;
}AEEEMEOpt;


typedef struct
{
   WE_UINT32   uiTransId;
   WE_UINT32   uiTransType;
   WE_UINT32   uiMsgId;
   WE_INT32    iResult;
   WE_CHAR     acRespDesc[512];
}AEEEMETransResult;


typedef struct  
{
   WE_UINT32   uiTransId;
   WE_UINT32   uiTransType;
   WE_CHAR     *pcUidl;
   WE_CHAR     *pcMsgId;
}AEEEMEMsg;

typedef struct  
{
   TransID     uiTransId;
   WE_UINT32   uiTransType;
   WE_CHAR     *pcMailboxName;
   WE_CHAR     *pcDelimiter;
}AEEEMEMailbox;


typedef struct
{
   TransID        uiTransId;
   WE_UINT32      uiTransType;
   WE_UINT32      uiTransferedSize;
   WE_UINT32      uiTotalSize;
}AEEEMESizeNotify;

typedef struct
{
   TransID     uiTransId;
   WE_UINT32   uiTransType;
   WE_UINT32   uiCurrMsg;
   WE_UINT32   uiTotalMsg;
}AEEEMENumNotify;


typedef struct
{
   WE_INT32       iAuthType;
   WE_CHAR*       pcUser;
   WE_CHAR*       pcHost;
   WE_UINT16      usPort;
   WE_CHAR        acRes[2];
}AEEEMEServer;


typedef struct
{
   WE_INT32          iURIType;
   WE_UINT32         uiUTCTime;
   WE_CHAR*          pcMailboxName;
   AEEEMEServer      stIServer;
}AEEEMEEmnRet;

/***************************************************************************************************
* Prototype Declare Section
***************************************************************************************************/

WE_INT AEEEMETM_Init(IEMETMHandle * po);
WE_INT AEEEMETM_Delete(IEMETMHandle *po);
WE_INT AEEEMETM_CreateSession(IEMETMHandle po, AEEEMEOpt *pstAccountOpts);
WE_INT AEEEMETM_CloseSession(IEMETMHandle po);
WE_INT AEEEMETM_EnqueueTrans(IEMETMHandle po, AEEEMEOpt* pstTransOpts, TransID *puiTransId);
WE_INT AEEEMETM_DequeueTrans(IEMETMHandle po, TransID uiTransId);
WE_INT AEEEMETM_DecodeEMN(IEMETMHandle po, AEEEMEOpt* pstDecodeOpts, AEEEMEEmnRet** ppRet);
WE_INT AEEEMETM_GetTransNumber(IEMETMHandle po, TransID transId, unsigned int *puiNum);
WE_VOID AEEEMETM_FreeNotifyMemory(WE_VOID* pvMem);

#ifdef __cplusplus
}
#endif

#endif //_AEE_EME_TM_H_
