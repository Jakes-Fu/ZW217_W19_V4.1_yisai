/*****************************************************************************
** File Name:      ctrlsys_msg.c                                             *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2011      Jassmine              Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmk_window_internal.h"
#include "ctrlsys_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : control msg is need dispatch to parent control or window
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDispatchToParent(
                                 MMI_MESSAGE_ID_E   msg_id
                                 );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : dispatch msg to control handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSYS_DispatchToHandle(
                                        MMI_HANDLE_T        handle,     //in:
                                        MMI_MESSAGE_ID_E    msg_id,     //in:
                                        DPARAM              param_ptr   //in:
                                        )
{
    BOOLEAN             result = FALSE;
    MMI_HANDLE_T        win_handle = 0;
    MMI_HANDLE_T        old_handle = 0;
    MMI_HANDLE_T        parent_handle = 0;
    MMI_RESULT_E        ctrl_result = MMI_RESULT_FALSE;
    MMI_RESULT_E        win_result = MMI_RESULT_FALSE;
    MMI_HANDLE_TYPE_E   handle_type = MMI_HANDLE_NONE;

    //get handle type
    handle_type = MMK_GetHandleType(handle);
    if (MMI_HANDLE_CONTROL == handle_type)
    {
        //get win handle
        win_handle = MMK_GetWinHandleByCtrl(handle);

        //get process msg old win handle
        old_handle = MMK_GetProcMsgHandle();

        //set process msg win handle
        MMK_SetProcMsgHandle(win_handle);

        //control handle
        ctrl_result = MMK_RunCtrlProc(handle,msg_id,param_ptr);

        switch (ctrl_result)
        {
        case MMI_RESULT_TRUE:
            result = TRUE;
            break;

        case MMI_RESULT_FALSE:
            if (IsDispatchToParent(msg_id))
            {
                //get parent control
                parent_handle = MMK_GetParentCtrlHandle(handle);
                while (0 != parent_handle)
                {
                    ctrl_result = MMK_RunCtrlProc(parent_handle,msg_id,param_ptr);
                    if (ctrl_result)
                    {
                        break;
                    }
                    parent_handle = MMK_GetParentCtrlHandle(parent_handle);
                }

                if (MMI_RESULT_FALSE == ctrl_result)
                {
                    //get parent win
                    parent_handle = MMK_GetWinHandleByCtrl(handle);
                    if (0 != parent_handle)
                    {
                        win_result = MMK_RunWinProc(parent_handle,msg_id,param_ptr);
                    }
                }
            }

            result = (BOOLEAN)(ctrl_result | win_result);
            break;

        default:
            break;
        }

        //reset process msg win handle
        MMK_SetProcMsgHandle(old_handle);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : control msg is need dispatch to parent control or window
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDispatchToParent(
                                 MMI_MESSAGE_ID_E   msg_id
                                 )
{
    BOOLEAN     result = TRUE;

    if ((MSG_NOTIFY_GET_ACTIVE == msg_id) ||
        (MSG_NOTIFY_LOSE_ACTIVE == msg_id))
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get control handle by the position of screen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC BOOLEAN CTRLSYS_GetHandleByPos(
//                                      GUI_POINT_T       *point_ptr,         //in:
//                                      MMI_WINDOW_NODE_T *win_node_ptr,      //in:
//                                      MMI_HANDLE_T      *ctrl_handle_ptr    //in/out:
//                                      )
//{
//    BOOLEAN     result = FALSE;
//
//    if (PNULL != ctrl_handle_ptr)
//    {
//    }
//
//    return (result);
//}


//1.MMK_RunCtrlProc()
//2.TP得确认好ctrl的tree才好移