/*==============================================================================
    FILE NAME   : EMS_Msg_API.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        A set of interface functions define for email messaging mechanism. 

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-21   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EMS_MSG_API_H
#define EMS_MSG_API_H

#ifdef __cplusplus
extern "C"{
#endif

/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "EMS_BaseDef.h"
#include "EMS_MsgPart_API.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
typedef void*  HEMSMsg;

/*=========================Msg Opts=========================*/

/* Data Type: Integer */
#define EMS_MSGOPT_DATE                   (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x01) 
#define EMS_MSGOPT_TIME_ZONE              (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x02)
#define EMS_MSGOPT_PRIO                   (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x03)
#define EMS_MSGOPT_STATUS                 (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x04)
#define EMS_MSGOPT_SIZE                   (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x05)
#define EMS_MSGOPT_BEDOWN                 (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x07)
#define EMS_MSGOPT_ACCOUNTID              (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x08)
#define EMS_MSGOPT_BOXID                  (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x09)
#define EMS_MSGOPT_MSGID                  (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x0A)
#define EMS_MSGOPT_MSG_TYPE               (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x0B)

/* For EMN */
#define EMS_MSGOPT_EMN_AUTHTYPE           (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x0C)
#define EMS_MSGOPT_EMN_PORT               (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x0D) 
#define EMS_MSGOPT_EMN_URITYPE            (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x0E) 
#define EMS_MSGOPT_EMN_UTCTIME            (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x0F) 

/*hide*/
#define EMS_MSGOPT_MSGHIDE                    (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x10)
#define EMS_MSGOPT_BOXFROM                    (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x11)
#define EMS_MSGOPT_RECEIVE_DATE               (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x12)
#define EMS_MSGOPT_STORESIZE               (EMSMSGOPT_32BIT+EMS_MSGOPT_BASE+0x13)

/* Data Type: string */
#define EMS_MSGOPT_TO                     (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x01)
#define EMS_MSGOPT_CC                     (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x02)
#define EMS_MSGOPT_BCC                    (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x03)
#define EMS_MSGOPT_FROM                   (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x04)
#define EMS_MSGOPT_REPLY_TO               (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x05)
#define EMS_MSGOPT_SUBJECT                (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x06)
#define EMS_MSGOPT_DISP_NOTIFY_TO         (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x07)
#define EMS_MSGOPT_X_MAILER               (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x08)
#define EMS_MSGOPT_CONTENT_TYPE           (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x09)
#define EMS_MSGOPT_UIDL                   (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x0D)
#define EMS_MSGOPT_BOXNAME                (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x0E)
/* For EMN */
#define EMS_MSGOPT_EMN_USERNAME           (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x0F)
#define EMS_MSGOPT_EMN_HOST               (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x10)
#define EMS_MSGOPT_EMN_MAILBOXNAME        (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x11)

/* for forward */
#define EMS_MSGOPT_FWD_UID                (EMSMSGOPT_SZ+EMS_MSGOPT_BASE+0x12)

/*=========================Opts Vals=========================*/
/*
 *	Email message type
 */
#define EMSMSG_TYPE_EMAIL                 0x01
#define EMSMSG_TYPE_EMN                   0x02

typedef unsigned int EMSMSG_TYPE;


/*
 * EMN URI Type
 */
#define EMSMSG_EMN_URITYPE_MAILAT         0x01
#define EMSMSG_EMN_URITYPE_POP            0x02
#define EMSMSG_EMN_URITYPE_IMAP           0x03
#define EMSMSG_EMN_URITYPE_OTHER          0x04

typedef unsigned int EMSMSG_EMN_URITYPE;


/*
 * EMN Authentication Type
 */
#define EMSMSG_EMN_AUTHTYPE_ANY           0x01
#define EMSMSG_EMN_AUTHTYPE_POP_PLAIN     0x02
#define EMSMSG_EMN_AUTHTYPE_POP_APOP      0x03

typedef unsigned int EMSMSG_EMN_AUTHTYPE;


/*
 *	Email message status
 */
#define EMSMSG_STATUS_READ                0x00000001
#define EMSMSG_STATUS_WITH_BODY           0x00000002
#define EMSMSG_STATUS_WITH_ATTACH         0x00000004
#define EMSMSG_STATUS_PROTECTED           0x00000008
#define EMSMSG_STATUS_FORWARDED           0x00000010
#define EMSMSG_STATUS_REPLIED             0x00000020
#define EMSMSG_STATUS_SENT                0x00000040
#define EMSMSG_STATUS_SEND_FAILED         0x00000080
#define EMSMSG_STATUS_DRAFT               0x00000100
#define EMSMSG_STATUS_RETRIEVED           0x00000200
#define EMSMSG_STATUS_IN_SERVER           0x00000400
#define EMSMSG_STATUS_BE_DOWN             0x00000800
#define EMSMSG_STATUS_MAX                 0xffffffff

typedef unsigned int EMSMSG_STATUS;


#define EMS_MSGPART_ID_CONTENTID          0x00000001        //EMS_PARTOPT_PARTID
#define EMS_MSGPART_ID_PART_TYPE          0x00000002        //EMS_PARTOPT_PARTTYPE
#define EMS_MSGPART_ID_ATTACHMENT_NAME    0x00000003        //EMS_PARTOPT_ATTACHNAME
#define EMS_MSGPART_ID_POSITION           0x00000004        //position
#define EMS_MSGPART_ID_SECTIONID          0x00000005        //EMS_PARTOPT_SECTIONID
#define EMS_MSGPART_ID_MAX                0xffffffff

typedef unsigned int EMS_MSGPART_ID;

/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/
int HEMSMSG_AddOpt(HEMSMsg hMsg, EMSMsgOpt *amo);
int HEMSMSG_GetOpt(HEMSMsg hMsg, int nOptId, int nIndex, EMSMsgOpt *pmo);
int HEMSMSG_UpdateOpt(HEMSMsg hMsg, int nIndex, EMSMsgOpt *pmo);
int HEMSMSG_RemoveOpt(HEMSMsg hMsg, int nOptId, int nIndex);
unsigned int HEMSMSG_GetOptCount(HEMSMsg hMsg);

int HEMSMSG_AddMessagePart(HEMSMsg hMsg, HEMSMsgPart hMsgPart, int nPos);
int HEMSMSG_GetMessagePart(HEMSMsg hMsg, EMS_MSGPART_ID nId, void *pVal, HEMSMsgPart *phMsgPart);
int HEMSMSG_RemoveMessagePart(HEMSMsg hMsg, EMS_MSGPART_ID nId, void *pVal);
unsigned int HEMSMSG_GetPartCount(HEMSMsg hMsg);

HEMSMsg HEMSMSG_New(void *pvMemMgr);
void HEMSMSG_Delete(HEMSMsg hMsg);


/*------------------------------------------------------------------------------
 *    Function Documentation
 *----------------------------------------------------------------------------*/
/*=============================================================================
Function: HMSG_AddMessagePart

Description:
Save the provided message part to the store corresponding to the message.

Prototype:
int HMSG_AddMessagePart(HMsg hMsg, HMsgPart hMsgPart, int nPos);

Parameters:

hMsg     [in]: The HMsg handler.
hMsgPart [in]: The HMsgPart handler .
nPos     [in]: The position at which the message part has to added/inserted, 
               -1 means the end position.

Return Value:

EME_SUCCESS : If we are able to save the message part
EME_FAILED  : Otherwise.

Comments:

See Also:
=============================================================================*/
/*=============================================================================
Function: HMSG_GetMessagePart

Description:
Retrieves the corresponding message part as specified by the Message part id
and value.

Prototype:
int HMSG_GetMessagePart(HMsg hMsg, EMS_MSGPART_ID nId, void *pVal, HMsgPart *phMsgPart);

Parameters:
hMsg        [in]: The HMsg handler.
nId         [in]: The message part id.
pVal        [in]: The id value for which we are making a search.
phMsgPart  [out]: Upon return would point to a valid HMsgPart handler. 

Return Value:

EME_SUCCESS : If we are able to retrieve the message part
EME_FAILED  : Otherwise.

Comments:

See Also:
=============================================================================*/
/*=============================================================================
Function: HMSG_RemoveMessagePart

Description:
Removes the corresponding message part, specified by the Id and Value.

Prototype:
int HMSG_RemoveMessagePart(HMsg hMsg, EMS_MSGPART_ID nId, void *pVal);


Parameters:

hMsg        [in]: The HMsg handler.
nId         [in]: The message part id.
pVal        [in]: The id value for which we are trying to delete.


Return Value:

EME_SUCCESS : If we are able to remove the message part
EME_FAILED  : Otherwise.

Comments:

See Also:
=============================================================================*/
/*=============================================================================
Function: HMSG_GetPartCount

Description:
Gets the message parts count.

Prototype:
unsigned int HMSG_GetPartCount(HMsg hMsg);


Parameters:

hMsg        [in]: The HMsg handler.

Return Value:

The part count in the message.

Comments:
The user may want to get the message parts count which can be retrieved 
by using this API. 

See Also:
=============================================================================*/
#ifdef __cplusplus
}
#endif

#endif /* EMS_MSG_API_H */
