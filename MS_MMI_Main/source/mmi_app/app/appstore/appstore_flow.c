/*****************************************************************************
** File Name:      appstore_flow.c                                           *
** Author:         xiuyun.wang                                               *
** Date:           08/12/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define app  store flow function      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2021       xiuyun.wang          Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "appstore_flow.h"
#include "appstore_control.h"
#include "appstore_home_win_view.h"
#include "appstore_operating_note_win_view.h"
#include "appstore_uninstall_query_win_view.h"
#include "appstore_operate_fail_note_win_view.h"
#include "appstore_no_data_note_win_view.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_appstore_vdf_inited = FALSE; //appstore VDF 只注册一次的flag
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : APP Store HomeWin Flow模块Uninstall QueryWin提供的处理左侧button（处理取消）的函数
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Flow_HandleHomeUninstallQueryWinButton1( void )
{
    TRACE_APPSTORE("[APPSTORE_FLOW]: enter");
    if(TRUE == APPSTORE_GetIsProcessing())
    {
        // 设置flag
        APPSTORE_SetIsProcessing(FALSE);
    }
    TRACE_APPSTORE("[APPSTORE_FLOW]: exit");
    return APPSTORE_RES_SUCC;
}

/*****************************************************************************/
//  Description : APP Store HomeWin Flow模块Uninstall QueryWin提供的处理右侧button（处理卸载）的函数
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Flow_HandleHomeUninstallQueryWinButton2( void )
{
    APPSTORE_RES_E       result          = APPSTORE_RES_MAX;
    APPSTORE_APP_NODE_T* p_app_node      = PNULL;

    TRACE_APPSTORE("[APPSTORE_FLOW]: enter");

    if(FALSE == APPSTORE_GetIsProcessing())// 当前不存在正在进行的操作
    {
        // 查询需要进行操作的app node信息
        p_app_node = APPSTORE_GetCurNodeInfo();

        if(PNULL == p_app_node)
        {
            TRACE_APPSTORE("[APPSTORE_FLOW]:p_app_node is PNULL");
            return APPSTORE_RES_FAIL;
        }

        TRACE_APPSTORE("[APPSTORE_FLOW]:p_app_node->app_node_state = %d",p_app_node->app_node_state);
        // 根据不同的需求进行操作
        switch(p_app_node->app_node_state)
        {
            case APPSTORE_APP_STATE_INSTALLED:
            case APPSTORE_APP_STATE_NEED_UPDATE:
            case APPSTORE_APP_STATE_OFFLINE:
            {
                // uninstalling.
                result = APPSTORE_UninstallAPP(p_app_node);

                // Judge return value
                if(APPSTORE_RES_SUCC != result)
                {
                    TRACE_APPSTORE("[APPSTORE_FLOW]:result is %d",result);

                    // display pubwin
                    APPSTORE_Operate_Fail_Note_Win_Enter();
                }
                break;
            }
            default:
            {
                result = APPSTORE_RES_PARAM_ERR;
                break;
            }
        }
    }
    else // 当前存在正在进行的操作,弹出提示框" 当前正在进行app操作"
    {
        TRACE_APPSTORE("[APPSTORE_FLOW]: open operating note win");
        APPSTORE_Operating_Note_Win_Enter();
        result = APPSTORE_RES_FAIL;
    }

    TRACE_APPSTORE("[APPSTORE_FLOW]: exit");
    return result;
}

/*****************************************************************************/
//  Description : APP Store HomeWin Flow模块提供的处理左侧button（处理安装/更新）的函数
//  Parameter: [In] list_item_index : 需要处理的list item的index
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Flow_HandleHomeWinButton1(uint16 list_item_index)
{
    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    APPSTORE_APP_NODE_T* p_app_node = PNULL;
    APPSTORE_APP_LIST_T* p_full_app_list = PNULL;

    TRACE_APPSTORE("[APPSTORE_FLOW]: enter");

    p_full_app_list = APPSTORE_GetFullAppList();
    if(PNULL == p_full_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_FLOW]:the p_full_app_list is PNULL");
        return APPSTORE_RES_FAIL;
    }

    if(FALSE == APPSTORE_GetIsProcessing())// 当前不存在正在进行的操作
    {
        // 查询需要进行操作的app node信息
        APPSTORE_GetAppNodeFromAppList(list_item_index,*p_full_app_list,&p_app_node);
        if(PNULL == p_app_node)
        {
            TRACE_APPSTORE("[APPSTORE_FLOW]:p_app_node is PNULL");
            return result;
        }

        TRACE_APPSTORE("[APPSTORE_FLOW]:p_app_node->app_node_state = %d",p_app_node->app_node_state);
        // 根据不同的需求进行操作
        switch(p_app_node->app_node_state)
        {
            case APPSTORE_APP_STATE_INSTALLED:
            {
                // 设置index
                //APPSTORE_SetCurNodeInfo(p_app_node);
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
                jee_start_app(p_app_node->p_app_id,NULL);
#endif
                break;
            }
            case APPSTORE_APP_STATE_NEED_UPDATE:
            {
                // updating...
                result = APPSTORE_UpdateAPP(p_app_node);

                // Judge return value
                if(APPSTORE_RES_SUCC != result)
                {
                    TRACE_APPSTORE("[APPSTORE_FLOW]:result is %d",result);

                    if(APPSTORE_RES_NO_ENOUGH_SPACE != result)
                    {
                        // display pubwin
                        APPSTORE_Operate_Fail_Note_Win_Enter();
                    }
                    // reset state to installing...
                    p_app_node->app_node_state = APPSTORE_APP_STATE_NEED_UPDATE;
                    APPSTORE_Data_SetAppList(p_full_app_list);
                    MMK_SendMsg(APPSTORE_HOME_WIN_ID, MSG_FULL_PAINT, PNULL);
                }
                break;
            }
            default:
            {
                result = APPSTORE_RES_PARAM_ERR;
                break;
            }
        }
    }
    else // 当前存在正在进行的操作,弹出提示框" 当前正在进行app操作"
    {
        TRACE_APPSTORE("[APPSTORE_FLOW]: open operating note win");
        APPSTORE_Operating_Note_Win_Enter();
        result = APPSTORE_RES_PARAM_ERR;
    }
    TRACE_APPSTORE("[APPSTORE_FLOW]: exit");
    return result;
}

/*****************************************************************************/
//  Description : APP Store HomeWin Flow模块提供的处理右侧button（处理安装/更新）的函数
//  Parameter: [In] list_item_index : 需要处理的list item的index
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Flow_HandleHomeWinButton2(uint16 list_item_index)
{
    APPSTORE_RES_E result = APPSTORE_RES_MAX;
    APPSTORE_APP_NODE_T* p_app_node = PNULL;
    APPSTORE_APP_LIST_T* p_full_app_list = PNULL;

    TRACE_APPSTORE("[APPSTORE_FLOW]: enter");

    p_full_app_list = APPSTORE_GetFullAppList();
    if(PNULL == p_full_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_FLOW]:the p_full_app_list is PNULL");
        return APPSTORE_RES_FAIL;
    }

    if(FALSE == APPSTORE_GetIsProcessing())// 当前不存在正在进行的操作
    {
        // 查询需要进行操作的app node信息
        APPSTORE_GetAppNodeFromAppList(list_item_index,*p_full_app_list,&p_app_node);
        if(PNULL == p_app_node)
        {
            TRACE_APPSTORE("[APPSTORE_FLOW]:p_app_node is PNULL");
            return APPSTORE_RES_FAIL;
        }

        TRACE_APPSTORE("[APPSTORE_FLOW]:p_app_node->app_node_state = %d",p_app_node->app_node_state);
        // 根据不同的需求进行操作
        switch(p_app_node->app_node_state)
        {
            case APPSTORE_APP_STATE_UNINSTALL:
            {
                // installing...
                result = APPSTORE_InstallAPP(p_app_node);

                // Judge return value
                if(APPSTORE_RES_SUCC != result)
                {
                    TRACE_APPSTORE("[APPSTORE_FLOW]:result is %d",result);

                    if(APPSTORE_RES_NO_ENOUGH_SPACE != result)
                    {
                        // display pubwin
                        APPSTORE_Operate_Fail_Note_Win_Enter();
                    }
                    // reset state to installing...
                    p_app_node->app_node_state = APPSTORE_APP_STATE_UNINSTALL;
                    APPSTORE_Data_SetAppList(p_full_app_list);
                    MMK_SendMsg(APPSTORE_HOME_WIN_ID, MSG_FULL_PAINT, PNULL);
                }
                break;
            }
            case APPSTORE_APP_STATE_INSTALLED:
            case APPSTORE_APP_STATE_NEED_UPDATE:
            case APPSTORE_APP_STATE_OFFLINE:
            {
                // 设置flag
                APPSTORE_SetCurNodeInfo(p_app_node);

                // open uninstall query win
                APPSTORE_Uninstall_Query_Win_Enter();
                break;
            }
            default:
            {
                result = APPSTORE_RES_PARAM_ERR;
                break;
            }
        }
    }
    else // 当前存在正在进行的操作,弹出提示框" 当前正在进行app操作"
    {
        TRACE_APPSTORE("[APPSTORE_FLOW]: open operating note win");
        APPSTORE_Operating_Note_Win_Enter();
        result = APPSTORE_RES_PARAM_ERR;
    }

    TRACE_APPSTORE("[APPSTORE_FLOW]: exit");
    return result;
}

LOCAL BOOLEAN AppSoreFlowCallback(MSG_APPSTORE_E msg_id)
{
    APPSTORE_APP_LIST_T* p_full_app_list = PNULL;
    TRACE_APPSTORE("[APPSTORE_FLOW]: msg_id:%d", msg_id);

    switch(msg_id)
    {
        case MSG_APPSTORE_UPDATE_APP:
        {
            p_full_app_list = APPSTORE_GetFullAppList();
            if(PNULL == p_full_app_list)
            {
                TRACE_APPSTORE("[APPSTORE_FLOW]:the p_full_app_list is PNULL");
                return APPSTORE_RES_FAIL;
            }
            APPSTORE_Data_SetAppList(p_full_app_list);
            MMK_SendMsg(APPSTORE_HOME_WIN_ID, MSG_FULL_PAINT, PNULL);
            break;
        }
        default:
        {
            break;
        }
    }
    return APPSTORE_RES_SUCC;
}
/*****************************************************************************/
//  Description : 进入APP Store 应用程序
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Enter( void )
{
    APPSTORE_RES_E  result = APPSTORE_RES_MAX;
    APPSTORE_APP_LIST_T* p_full_app_list = PNULL;

    TRACE_APPSTORE("[APPSTORE_FLOW]: enter");

    if (TRUE == MMIAPIUDISK_UdiskIsRun())
    {
        TRACE_APPSTORE("[APPSTORE_FLOW]: Udisk is run");
        return APPSTORE_RES_FILE_MISSED;
    }

    if(FALSE == s_is_appstore_vdf_inited)
    {
        //register appstore control first
        APPSTORE_AddClient(AppSoreFlowCallback);
        s_is_appstore_vdf_inited = TRUE;
        TRACE_APPSTORE("[APPSTORE_FLOW]: add client to appstore control");
    }

    //get full app list and set to data
    p_full_app_list = APPSTORE_GetFullAppList();
    if(PNULL == p_full_app_list)
    {
        TRACE_APPSTORE("[APPSTORE_FLOW]:the p_full_app_list is PNULL");
        return APPSTORE_RES_FAIL;
    }
    if(p_full_app_list->app_num == 0)
    {
        TRACE_APPSTORE("[APPSTORE_FLOW]:app num is 0, can not enter appstore window");
        APPSTORE_No_Data_Note_Win_Enter();
        return result;
    }
    APPSTORE_Data_SetAppList(p_full_app_list);

    // open enterwin
    APPSTORE_Home_Win_Enter();

    TRACE_APPSTORE("[APPSTORE_FLOW]: exit");
    return result;
}
