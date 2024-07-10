/******************************************************************************
 ** File Name:      state_machine.h                                           *
 ** Author:         Karin.li                                                  *
 ** Date:           12/10/2001                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This document contains proprietary information belonging  *
 **                 to SPREADTRUM.Passing on and copying of this document,use *
 **                 and communication of its contents is not permitted without*
 **                 prior written authorization.                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 12/10/2001     Karin.li         Create.                                   *
 ** 05/22.2002     Karin.li         Modification.                             *  
 ******************************************************************************/
#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "sci_types.h"
#include "os_api.h"
//#include "signals_header.h"

typedef enum 
{
	SIGNAL_INPUT,       // signal would be deal
	SIGNAL_DISCARD,     // signal would be discarded
	SIGNAL_SAVE         // signal would be saved in save queue
} SIGNAL_ACTION_E;

// state machine func array define
typedef SIGNAL_ACTION_E (*STATE_MACHINE_FUNC)(xSignalHeaderRec*  sig_ptr, void* argv_ptr);

/*****************************************************************************/
// 	Description : save input signal 
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern SIGNAL_ACTION_E SaveSignal(
         xSignalHeaderRec*  sig_ptr,     // input signal address
         void*              argv_ptr     // input sth else,not used now
         );

/*****************************************************************************/
// 	Description : discard input signal 
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern SIGNAL_ACTION_E DiscardSignal(
         xSignalHeaderRec*  sig_ptr,     // input signal address
         void*              argv_ptr     // input sth else,not used now
         );
         
/*****************************************************************************/
// 	Description : search the msgtable ,and found the msg index
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:         (msg0...msgn)-->(0...m)
/*****************************************************************************/
extern BOOLEAN SearchMsgTable(                         // TRUE means found
             uint32       msg,                     // input msg
             const        uint32* msgtb_ptr,  // msgtable point
             uint32       msg_num,                 // msgtable size
             uint32*      msg_id                   // id founded msg
             );
#endif  /*_STATE_MACHINE_H_*/