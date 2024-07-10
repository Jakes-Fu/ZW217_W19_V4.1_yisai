/****************************************************************************
** File Name:      mmk_kdb.h                                               *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message id of key     *
**                  and define the function of key                         *                                                *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003       Louis.wei         Create
** 
****************************************************************************/
#ifndef  _MMK_KBD_INTERNAL_H_    
#define  _MMK_KBD_INTERNAL_H_   

/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_signal.h"
#include "mmi_osbridge.h"

/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**-----------------------------------------------------------------------------*
 **                         MACRO Declaration                                   *
 **----------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//  Global resource dependence : 
//  Author:Robert Lu
//  Note:
/*****************************************************************************/
void MMK_DispatchMSGTp(
                       MmiTPPress* signal_ptr // the pointer of message which need dispatch
                       );

/*****************************************************************************/
// 	Description : set key down ctrl
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMK_SetKeyDownCtrl(
                           MMI_HANDLE_T      ctrl_handle,
                           MMI_MESSAGE_ID_E  msg_id
                           );

/*****************************************************************************/
// 	Description : get key down ctrl
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
MMI_HANDLE_T MMK_GetKeyDownCtrl( 
                                MMI_MESSAGE_ID_E msg_id
                                );

/*****************************************************************************/
// 	Description : clear key down ctrl
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
BOOLEAN MMK_ClearKeyDownCtrl(
                             MMI_HANDLE_T        ctrl_handle
                             );


/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author: Cheng.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMK_ReInitKbdDblclkState(
                                     BOOLEAN is_notify
                                     );

/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_KBD_H_  */
