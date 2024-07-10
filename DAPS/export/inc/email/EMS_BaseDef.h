/*==============================================================================
    FILE NAME   : EMS_BaseDef.h
    MODULE NAME : 


    GENERAL DESCRIPTION:
        Define option structure and the base ids for EMSMsg and EMSMsgPart.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-20   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EMS_BASEDEF_H
#define EMS_BASEDEF_H
#include "we_msg.h"

/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
/*
 * EMSMsgOpt
 *	Holds an id, value pair for HEMSMsg or HEMSMsgPart
 */
typedef struct EMSMsgOpt
{
   int nId;  /* option id */
   int type;
   int size;
   void  *pVal;/* option value, depends on nId */
} EMSMsgOpt;


/*
 * EMSMsgOpt option ids
 *
 * EMSMsgOpt ids fall in ranges that are designed to keep the pVal data opaque 
 * while helping users deal with copying the data if necessary.
 */
/*
 * pVals are reserved, as are ids, up to 0x0ffff
 */
#define EMSMSGOPT_RESERVED       WEMSG_OPT_RESERVED
#define EMSMSGOPT_RESERVED_LAST  WEMSG_OPT_RESERVED_LAST
#define EMSMSGOPT_END            WEMSG_OPT_END /* no pVal, ignore rest, used for terminating a vector of MsgOpts */
#define EMSMSGOPT_ANY            WEMSG_OPT_ANY /* no pVal, matches any opt, used for enumeration */


/*
 * pVals are null-terminated strings, valid ids up to 0x01ffff
 */
#define EMSMSGOPT_SZ             WEMSG_OPT_SZ
#define EMSMSGOPT_SZ_LAST        WEMSG_OPT_SZ_LAST


/*
 * pVals are 32-bit quantities, no need to copy, valid ids up to 0x02ffff
 */
#define EMSMSGOPT_32BIT          WEMSG_OPT_32BIT
#define EMSMSGOPT_32BIT_LAST     WEMSG_OPT_32BIT_LAST

/*
 *	Define the options base id
 */
#define EMS_MSGOPT_BASE          0x00001000
#define EMS_PARTOPT_BASE         0x00002000
#define EMS_PARAMOPT_BASE        0x00004000 

/*------------------------------------------------------------------------------
 *    Data Structure Documentation
 *----------------------------------------------------------------------------*/
/*==============================================================================
 *    Data Structure:      EMSMsgOpt
 * 
 *    Description:   Holds an id, value pair for HEMSMsg or HEMSMsgPart.
 * 
 *    Definition:
 *       typedef struct EMSMsgOpt
 *       {
 *          int   nId;
 *          void  *pVal;
 *       } EMSMsgOpt; 
 * 
 *    Members:
 *    nId: option id
 *
 *    pVal: option value, depends on nId
 *
 *    Comments:
 *    EMSMsgOpt structures are the basic communication building block for
 *    representing email message.  The structures, when passed to AddOpt, 
 *    are not themselves copied, the contents are copied out
 * 
 *============================================================================*/

#endif /* EMS_BASEDEF_H */