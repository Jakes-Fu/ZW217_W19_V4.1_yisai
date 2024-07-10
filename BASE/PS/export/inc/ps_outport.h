/******************************************************************************
 ** File Name:      ps_outport.h                                              *
 ** Author:         shijun cui                                                *
 ** Date:           03/11/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *         
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 06/07/2007     Shijun cui       Create.                                   *
 ******************************************************************************/

#if !defined (_PS_OUTPORT_H)
#define _PS_OUTPORT_H
#include "sci_types.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*****************************************************************************/
// Description : Init Protocol Stack, create ps task
// Global resource dependence : none
// Author: 
/*****************************************************************************/
extern uint32 SCI_InitPs(
                 void
                 );

/*****************************************************************************/
//  Description:    Repacket GPRS message which includes pointer., the 
//                  destination packet saved in dest_ptr. If the message with 
//                  no pointer, don't handle it, dest_ptr is still NULL.
//	Global resource dependence: 
//  Author: hanjun.liu
//	Note:
/*****************************************************************************/
extern void REF_PktGprsMsg(            // None
    uint8     **dest_ptr,       // Destination packet, save packed message.
    uint16    msg_type,         // Message type. Low 8 bit is struct type
    const uint8     *src_ptr,   // Source message
    uint16    *len              // Length of src_ptr when input,
                                // length of dest_ptr when output,
    );

/********************************************************************
//    DESCRIPTION:Check if g_sim_card_state is SIM_INITIALIZE_STATE or
//                SIM_ACTIVE_STATE.
//    Global resource dependence :
//    Author:hanjun.liu
//    Note:
***********************************************************************/
extern uint8 REF_CheckSimState(
                     void
                     );

/*****************************************************************************/
// 	Description : L4 init
//	Global resource dependence : none
//  Author: William.Qian
//	Note: 
/*****************************************************************************/
extern uint32 SCI_InitL4(
                     void
                     );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* end of _PS_OUTPORT_H */