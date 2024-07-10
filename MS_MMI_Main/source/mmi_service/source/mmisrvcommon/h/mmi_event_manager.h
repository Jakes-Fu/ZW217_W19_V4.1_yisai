/*****************************************************************************
** File Name:      mmi_event_manager.h                                           *
** Author:                                                                   *
** Date:           10/2009                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMI_EVENT_MANAGER_H_
#define _MMI_EVENT_MANAGER_H_

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

/*****************************************************************************/
//  Description : 该event事件和处理是否已在database中
//  Global resource dependence : 
//  Author: cheng.luo
//  Note: TRUE exsit, FALSE not exsit
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_EventIsRegister( 
										uint32  event_type, 
										void*	 callback
										);

/*****************************************************************************/
//  Description : 注册event事件
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_RegisterEvent(
								   uint32  event_type,
								   void*	call_back
								   );

/*****************************************************************************/
//  Description : 去注册event事件
//  Global resource dependence : 
//  Author: cheng.luo
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_UnregisterEvent(
									 uint32  event_type,
									 void*	call_back
									 );

/*****************************************************************************/
//  Description : 触发event事件
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_PumpEvent( 
							   uint32 event_type,
							   uint32 argv[],
							   uint32 argc
							   );

/*******************************************************************/
//  Interface:		MMIFW_EventCallBack
//  Description : 	MMIFW_EventCallBack
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIFW_EventCallBack(
									uint32 event_type, 
									void * call_back,
									uint32 arvc,
									uint32 argv[]
									);


#ifdef __cplusplus
}
#endif

#endif // _MMI_EVENT_MANAGER_H_


