/******************************************************************************
 ** File Name:      sci_log_gprs_data.h                                       *
 ** Author:         Ivan.Yin                                                  *
 ** Date:           09/05/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file defines the operation interfaces of repacket    *
 **                 GPRS message.                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 09/05/2002     Ivan.Yin         Create.                                   *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "sci_log.h"
#include "sci_types.h"
//#include "gsm_gprs.h"	
//#include "dynamic_tasks_def.h"
//#include "signals_header.h"
//#include "gprs_log_msg.h"

#ifndef _SCI_LOG_GPRS_DATA_H
#define _SCI_LOG_GPRS_DATA_H

/*****************************************************************************/
//  Description:    Repacket GPRS message which includes pointer., the 
//                  destination packet saved in dest_ptr. If the message with 
//                  no pointer, don't handle it, dest_ptr is still NULL.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void PktGprsMsg(         // None
    uint8     **dest_ptr,       // Destination packet, save packed message.
    uint16    msg_type,         // Message type. Low 8 bit is struct type
    const uint8     *src_ptr,   // Source message
    uint16    *len              // Length of src_ptr when input,
                                // length of dest_ptr when output,
    );
    
#endif