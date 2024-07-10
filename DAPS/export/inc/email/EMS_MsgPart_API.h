/*==============================================================================
    FILE NAME   : EMS_MsgPart_API.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        Define Some MsgOpts for operate multipart-message's part.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-21   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EMS_MSGPART_API_H
#define EMS_MSGPART_API_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "EMS_BaseDef.h"

#ifdef __cplusplus
extern "C"{
#endif

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
typedef void*  HEMSMsgPart;

/*=========================MsgPart Opts=========================*/

#define EMS_PARTOPT_SRCTYPE            (EMSMSGOPT_32BIT+EMS_PARTOPT_BASE+0x01)
#define EMS_PARTOPT_BUFFSZ             (EMSMSGOPT_32BIT+EMS_PARTOPT_BASE+0x02)
#define EMS_PARTOPT_PARTTYPE           (EMSMSGOPT_32BIT+EMS_PARTOPT_BASE+0x03)
#define EMS_PARTOPT_PARTID             (EMSMSGOPT_32BIT+EMS_PARTOPT_BASE+0x04)
/*push mail field*/
#define EMS_PARTOPT_X_ATTACHMENT       (EMSMSGOPT_32BIT+EMS_PARTOPT_BASE+0x05)
#define EMS_PARTOPT_X_ATTACHMENT_SIZE  (EMSMSGOPT_32BIT+EMS_PARTOPT_BASE+0x06)
#define EMS_PARTOPT_CONTENT_SIZE       (EMSMSGOPT_32BIT+EMS_PARTOPT_BASE+0x07)


#define EMS_PARTOPT_CONTENTID          (EMSMSGOPT_SZ+EMS_PARTOPT_BASE+0x01)
#define EMS_PARTOPT_DATABUF            (EMSMSGOPT_SZ+EMS_PARTOPT_BASE+0x02)
#define EMS_PARTOPT_TRANSENC           (EMSMSGOPT_SZ+EMS_PARTOPT_BASE+0x03)
#define EMS_PARTOPT_CONTENTDISP        (EMSMSGOPT_SZ+EMS_PARTOPT_BASE+0x04)
#define EMS_PARTOPT_SECTIONID          (EMSMSGOPT_SZ+EMS_PARTOPT_BASE+0x06)

#define EMS_PARTOPT_ATTACHNAME         (EMSMSGOPT_SZ+EMS_PARAMOPT_BASE+0x01)
#define EMS_PARTOPT_CHARSET            (EMSMSGOPT_SZ+EMS_PARAMOPT_BASE+0x02)

/*=========================Opts Vals=========================*/
#define EMSPART_SRCTYPE_BUFFER         0x00000001  //Source is a string buffer
#define EMSPART_SRCTYPE_FILE           0x00000002  //Source is locate file

typedef unsigned int EMSPART_SRCTYPE;

/* Email message part types */
#define EMSPART_TYPE_TEXT_PLAIN        0x00000001
#define EMSPART_TYPE_TEXT_HTML         0x00000002
#define EMSPART_TYPE_ATTACHMENT        0x00000003

typedef unsigned int EMSPART_TYPE;


/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/
/*==============================================================================
 *    Function:      HEMSMSGPART_AddOpt
 * 
 *    Description:   Adds the list of EMSMsgOpt to the current message part.
 * 
 *    Prototype:
 *       int HEMSMSGPART_AddOpt(HEMSMsgPart hMsgPart, EMSMsgOpt *amo);
 * 
 *    Parameters:
 *       hMsgPart [in]: The handler.
 *       amo      [in]: Array of EMSMsgOpt structures, terminated by 
 *                      a EMSMsgOpt with nId set to EMSMSGOPT_END.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the options could be added successfully
 *       EME_NOMEMORY:  If enough memory couldn't be allocated
 *  	   
 *    Comments:
 * 
*============================================================================*/
int HEMSMSGPART_AddOpt(HEMSMsgPart hMsgPart, EMSMsgOpt *amo);

/*==============================================================================
 *    Function:      HEMSMSGPART_GetOpt
 * 
 *    Description:   Find a EMSMsgOpt in the message part matching nOptId.  
 *                   There may be multiple matches (either in the case of a 
 *                   multi-valued MsgOpt, or in the case of overrides). 
 *                   In this case nIndex specifies how far into the list of 
 *                   message part that match nOptId to look. pmo is filled in 
 *                   with a the matching EMSMsgOpt contents. nIndex is zero-based.
 * 
 *    Prototype:
 *       int HEMSMSGPART_GetOpt(HEMSMsgPart hMsgPart, int nOptId, int nIndex, EMSMsgOpt *pmo);
 * 
 *    Parameters:
 *       hMsgPart [in]: The handler.
 *       nOptId   [in]: Id of the option to find in the options list, EMSMSGOPT_ANY 
 *                      returns the nIndexth option, if there is one...
 *       nIndex   [in]: Which (if there are multiple EMSMsgOpt of the same id) to get
 *                      nIndex counts from most recently AddOpt()ed option, it is
 *                      zero-based.
 *       pmo     [out]: where to stick the found EMSMsgOpt.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the options could be found successfully
 *       EME_FAILED:    If the specified MsgOpt couldn't be found
 *  	   
 *    Comments:
 * 
 *============================================================================*/
int HEMSMSGPART_GetOpt(HEMSMsgPart hMsgPart, int nOptId, int nIndex, EMSMsgOpt *pmo);

/*==============================================================================
 *    Function:      HEMSMSGPART_UpdateOpt
 * 
 *    Description:   Update the nIndex'th EMSMsgOpt matching pmo.
 * 
 *    Prototype:
 *       int HEMSMSGPART_UpdateOpt(HEMSMsgPart hMsgPart, int nIndex, EMSMsgOpt *pmo);
 * 
 *    Parameters:
 *       hMsgPart [in]: The handler.
 *       nIndex   [in]: Which (if there are multiple EMSMsgOpt of the same id) to get
 *                      nIndex counts from most recently AddOpt()ed option, it is
 *                      zero-based.
 *       pmo      [in]: EMSMsgOpt pointer to witch want to update.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the options could be update successfully
 *       EME_NOMEMORY:  If enough memory couldn't be allocated
 *       EME_FAILED:    If the specified MsgOpt couldn't be found
 *  	   
 *    Comments:
 * 
 *============================================================================*/
int HEMSMSGPART_UpdateOpt(HEMSMsgPart hMsgPart, int nIndex, EMSMsgOpt *pmo);

/*==============================================================================
 *    Function:      HEMSMSGPART_RemoveOpt
 * 
 *    Description:   Remove the nIndex'th EMSMsgOpt matching nOptId.
 * 
 *    Prototype:
 *       int HEMSMSGPART_RemoveOpt(HEMSMsgPart hMsgPart, int nOptId, int nIndex);
 * 
 *    Parameters:
 *       hMsgPart [in]: The handler.
 *       nOptId   [in]: Id of the option to remove from the options list
 *       nIndex   [in]: Which (if there are multiple EMSMsgOpt of the same id) to get
 *                      nIndex counts from most recently AddOpt()ed option, it is
 *                      zero-based.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the options could be removed successfully
 *       EME_FAILED:    If the specified MsgOpt couldn't be found
 *  	   
 *    Comments:
 * 
 *============================================================================*/
int HEMSMSGPART_RemoveOpt(HEMSMsgPart hMsgPart, int nOptId, int nIndex);

/*==============================================================================
 *    Function:      HEMSMSGPART_GetOptCount
 * 
 *    Description:   Return the EMSMsgOpt number in the HEMSMsgPart.
 * 
 *    Prototype:
 *       unsigned int HEMSMSGPART_GetOptCount(HEMSMsgPart hMsgPart);
 * 
 *    Parameters:
 *       hMsgPart [in]: The handler.
 * 
 *    Return Value:
 *       The MsgOpts number.
 *  	   
 *    Comments:
 * 
 *============================================================================*/
unsigned int HEMSMSGPART_GetOptCount(HEMSMsgPart hMsgPart);

HEMSMsgPart HEMSMSGPART_New(void *pvMemMgr);
void HEMSMSGPART_Delete(HEMSMsgPart hMsgPart);

#ifdef __cplusplus
}
#endif

#endif /* EMS_MSGPART_API_H */
