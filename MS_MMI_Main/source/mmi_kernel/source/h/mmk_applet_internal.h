/****************************************************************************
** File Name:      mmk_applet_internal.h                                   *
** Author:         James.Zhang                                             *
** Date:           05/07/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _MMK_APPLET_INTERNAL_H_    
#define  _MMK_APPLET_INTERNAL_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_app.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : run the applet process 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMK_RunAppletProc(
                                      MMI_HANDLE_T     applet_handle,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM           param
                                      );

/*****************************************************************************/
// 	Description : must process the window message
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ProcSpecialAppletMsg(
                                        MMI_HANDLE_T		applet_handle, 
                                        MMI_MESSAGE_ID_E	msg_id,   
                                        DPARAM			    param    
                                        );

/*****************************************************************************/
// 	Description : remove win node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_RemoveWinNode(
                                 MMI_HANDLE_T    applet_handle,
                                 MMI_HANDLE_T    win_handle
                                 );

/*****************************************************************************/
// 	Description : switch applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:会给applet发送suspend\resume消息
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SwitchApplet(
                                MMI_HANDLE_T cur_applet_handle
                                );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_APPLET_INTERNAL_H_  */
