/****************************************************************************
** File Name:      mmk_applet.c                                            *
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
#define MMK_APPLET_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_kernel_sys_trc.h"
#include "mmk_applet_internal.h"
#include "mmk_handle_internal.h"
#include "mmk_window_internal.h"
#include "cafapplet.h"
#include "cafhelp.h"
#include "mmk_module_manager.h"
#include "mmk_msg_internal.h"
#include "mmk_timer.h"
#include "mmk_gsensor.h"
#include "mmk_modinfo.h"
#include "mmi_link.h"
#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif
/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef struct
{
    MMI_BASE_NODE_T             base_node;
    CAF_GUID_T                  guid;

    IAPPLET_T*                  applet_ptr;
    
    MMI_TREE_NODE_T *           win_tree_root_ptr;

    MMI_TREE_NODE_T*            tree_node_ptr;

    MMI_APPLET_STATE_T          state;
    BOOLEAN                     is_open;
    BOOLEAN                     is_will_close;
    BOOLEAN                     is_return_first_applet;
    uint32                      start_time;//second

}MMI_APPLET_NODE_T;

typedef struct
{	
    MMI_TREE_NODE_T*           first_applet_node_ptr;
    MMI_HANDLE_T               cur_applet_handle;
    
}MMI_APPLET_SYSTEM_T;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
LOCAL MMI_APPLET_SYSTEM_T s_applet_system = {0};

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
extern void MMI_FreeRes(
                        uint32 win_id
                        );
/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get applet node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_APPLET_NODE_T* GetAppletNodePtr(
                                          MMI_HANDLE_T applet_handle
                                          );

/*****************************************************************************/
// 	Description : create a window in parseing window table
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T AppletCreateWindow(
                                      void*   create_ptr,
                                      BOOLEAN is_win_table
                                      );

/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AppletTreeNodeConstruct( 
                                      MMI_TREE_NODE_T* node_ptr,
                                      uint32 param1,
                                      uint32 param2
                                      );

/*****************************************************************************/
// 	Description : applet node destruct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AppletTreeNodeDestruct( 
                                     uint32 handle
                                     );

#if 0
/*****************************************************************************/
// 	Description : applet node handle event
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 AppletTreeNodeHandleEvent( 
                                       MMI_TREE_NODE_T* node_ptr,
                                       uint32           msg_id,
                                       void*            param
                                       );

#endif
/*****************************************************************************/
// 	Description : get the mmi system pointer
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_APPLET_SYSTEM_T* GetAppletSystem(void);

/*****************************************************************************/
// 	Description : applet node destruct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CloseAppletWithWin( 
                                 MMI_APPLET_NODE_T* applet_node_ptr
                                 );

/*****************************************************************************/
// 	Description : stop applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CloseApplet( 
                          MMI_HANDLE_T applet_handle
                          );

/*****************************************************************************/
// 	Description : update applet system
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void UpdateAppletSystem( void );

// extern const MMI_TREE_NODE_VTBL_T window_tree_node_vtbl;
// 
// const MMI_TREE_NODE_VTBL_T applet_tree_node_vtbl = 
// {
//     AppletTreeNodeConstruct,
//     AppletTreeNodeDestruct,
//     AppletTreeNodeHandleEvent,
//     AppletTreeNodeCompare,
// };

/*****************************************************************************/
// 	Description : return the appointed window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ReturnAppointedWin(
                                 MMI_HANDLE_T win_handle // window id
                                 );

/*****************************************************************************/
// 	Description : create a window in parseing window table
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateWinTable(
                                       const MMI_WINDOW_TABLE_CREATE_T* create_ptr
                                       )
{
    MMI_WINDOW_TABLE_CREATE_T create = {0};
    
    if ( PNULL == create_ptr )
    {
        return 0;
    }
    
    create = *create_ptr;
    create.parent_win_handle = MMK_ConvertIdToHandle( create_ptr->parent_win_handle );
    create.applet_handle     = MMK_ConvertIdToHandle( create_ptr->applet_handle );
    
    return AppletCreateWindow( &create, TRUE );
}

/*****************************************************************************/
// 	Description : create a dyanmic window and open it
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateWindow(
                                     const MMI_WINDOW_CREATE_T* create_ptr
                                     )
{
    MMI_WINDOW_CREATE_T create = {0};
    
    if ( PNULL == create_ptr )
    {
        return 0;
    }
    
    create = *create_ptr;
    create.parent_win_handle = MMK_ConvertIdToHandle( create_ptr->parent_win_handle );
    create.applet_handle     = MMK_ConvertIdToHandle( create_ptr->applet_handle );
    
    return AppletCreateWindow( &create, FALSE );
}

/*****************************************************************************/
// 	Description : create a window in parseing window table
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T AppletCreateWindow(
                                      void*   create_ptr,
                                      BOOLEAN is_win_table
                                      )
{
    MMI_HANDLE_T         applet_handle     = 0;
    MMI_HANDLE_T         parent_win_handle = 0;
    MMI_APPLET_NODE_T*   applet_node_ptr   = PNULL;
    MMI_TREE_NODE_T*     new_win_ptr       = PNULL;
    WINDOW_HANDLE_EVENT  func              = PNULL;
    BOOLEAN              is_open_win       = TRUE;

    if ( PNULL == create_ptr )
    {
        return 0;
    }
    
    if( is_win_table )
    {
        applet_handle     = ((MMI_WINDOW_TABLE_CREATE_T*)create_ptr)->applet_handle;
        parent_win_handle = ((MMI_WINDOW_TABLE_CREATE_T*)create_ptr)->parent_win_handle;
        func              = ((MMI_WINDOW_TABLE_CREATE_T*)create_ptr)->func;
    }
    else
    {
        applet_handle     = ((MMI_WINDOW_CREATE_T*)create_ptr)->applet_handle;
        parent_win_handle = ((MMI_WINDOW_CREATE_T*)create_ptr)->parent_win_handle;
        func              = ((MMI_WINDOW_CREATE_T*)create_ptr)->func;

        if ( ((MMI_WINDOW_CREATE_T*)create_ptr)->is_user_open_win )
        {
            is_open_win = FALSE;
        }
        
        // check the win_id, 使用MMK_IsExistWin, 解决关闭窗口消息中打开id相同窗口的问题
        if ( MMK_IsExistWin( applet_handle, ((MMI_WINDOW_CREATE_T*)create_ptr)->win_id ) )
        //if ( MMK_GetWinHandle( applet_handle, ((MMI_WINDOW_CREATE_T*)create_ptr)->win_id ) )
        {
            //SCI_TRACE_LOW:"AppletCreateWindow failed, the same window id!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_227_112_2_18_3_23_5_1,(uint8*)"");

            return 0;
        }
    }
    
    if ( PNULL == ( applet_node_ptr = GetAppletNodePtr( applet_handle ) ) )
    {
        //SCI_TRACE_LOW:"AppletCreateWindow failed, no applet!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_235_112_2_18_3_23_6_2,(uint8*)"");
        //no applet
        return 0; /*lint !e527*/
    }

    //parent win_handle must be valid
    if( parent_win_handle &&
        !MMK_FindAllTreeNode( applet_node_ptr->win_tree_root_ptr, 
        MMK_WindowTreeNodeCompare, MMI_TREE_NODE_FIND_HANDLE, parent_win_handle, MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD ) )
    {
        //SCI_TRACE_LOW:"AppletCreateWindow failed, parent win error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_245_112_2_18_3_23_6_3,(uint8*)"");
        //parent win error
        return 0; /*lint !e527*/
    }

    //MMK_AddTreeNode 如果失败, 需要用户销毁tree node
    if ( PNULL != ( new_win_ptr = MMK_AddTreeNode( MMK_WindowTreeNodeConstruct, 
        is_win_table, (uint32)create_ptr, &applet_node_ptr->win_tree_root_ptr, MMK_GetWinTreeNode( parent_win_handle ) ) ) )
    {
        if ( is_open_win )
        {
            //create win, then open win
            if( MMK_OpenWin( new_win_ptr->data, parent_win_handle ) )
            {
                return new_win_ptr->data;
            }
            else
            {               
                return 0;
            }
        }
        else
        {
            return new_win_ptr->data;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"AppletCreateWindow failed, can't construct!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_273_112_2_18_3_23_6_4,(uint8*)"");

        return 0;
    }
}

/*****************************************************************************/
// 	Description : close all applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ReturnFirstApplet( void )
{
    uint32 i = 0;
    MMI_APPLET_SYSTEM_T* applet_system_ptr = GetAppletSystem();
    MMI_TREE_TRAVEL_INFO_T* travel_ptr     = MMK_CreateTreeTravelInfo();

    SCI_ASSERT(PNULL != applet_system_ptr);/*assert verified*/
    MMK_TravelTree( travel_ptr, applet_system_ptr->first_applet_node_ptr, 0 );

    for ( ; i < travel_ptr->total_num; i++ )
    {
        if( travel_ptr->data_ptr[i] != applet_system_ptr->first_applet_node_ptr->data )
        {
            MMI_APPLET_NODE_T* applet_node_ptr = GetAppletNodePtr( travel_ptr->data_ptr[i] );

            if( PNULL != applet_node_ptr
                && !applet_node_ptr->is_will_close )
            {
                if ( applet_node_ptr->state & MMI_APPLET_STATE_BG )
                {
                    //james,防止在close window消息中关闭bg applet
                    applet_node_ptr->is_return_first_applet = TRUE;
                    MMK_RemoveAllTreeNode( &applet_node_ptr->win_tree_root_ptr, MMK_WindowTreeNodeDestruct );
                    //james,防止在close window消息中关闭bg applet
                    applet_node_ptr->is_return_first_applet = FALSE;
                }
                else if ( applet_node_ptr->state & MMI_APPLET_STATE_BG_HIDE_WIN )
                {
                    //james,防止在close window消息中关闭bg applet
                    applet_node_ptr->is_return_first_applet = TRUE;
                    MMK_HideWinByApplet( applet_node_ptr->base_node.handle );
                    //james,防止在close window消息中关闭bg applet
                    applet_node_ptr->is_return_first_applet = FALSE;
                }
                else if ( applet_node_ptr->state & MMI_APPLET_STATE_DISABLE_RETURN_IDLE )
                {
                    //do nothing
                }
                else
                {
                    applet_node_ptr->is_will_close = TRUE;
                    MMK_RemoveTreeNode( &applet_system_ptr->first_applet_node_ptr, AppletTreeNodeDestruct, applet_node_ptr->tree_node_ptr );
                }
            }
        }
    }

    MMK_DestroyTreeTravelInfo( travel_ptr );

    return TRUE;
}
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : close all applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseAllDynamicApplet( void )
{
    ReturnFirstApplet();

    UpdateAppletSystem();

    return TRUE;
}
#endif
/*****************************************************************************/
// 	Description : stop applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMK_CloseAppletByGuid( 
//                                      CAF_GUID_T guid
//                                      )
// {
//     return MMK_CloseApplet( guid );
// }

/*****************************************************************************/
// 	Description : stop applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CloseApplet( 
                          MMI_HANDLE_T applet_handle
                          )
{
    BOOLEAN            result          = FALSE;
    MMI_APPLET_NODE_T* applet_node_ptr = PNULL;
    
    if( PNULL != ( applet_node_ptr = GetAppletNodePtr( applet_handle ) )
        && !applet_node_ptr->is_will_close 
        && !applet_node_ptr->is_return_first_applet)
    {
        MMI_APPLET_SYSTEM_T* applet_system_ptr = GetAppletSystem();
        SCI_ASSERT(PNULL != applet_system_ptr);/*assert verified*/

        applet_node_ptr->is_will_close = TRUE;
        MMK_RemoveTreeNode( &applet_system_ptr->first_applet_node_ptr, AppletTreeNodeDestruct, applet_node_ptr->tree_node_ptr );
        
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_CloseApplet failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_396_112_2_18_3_23_6_5,(uint8*)"" );
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : update applet system
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void UpdateAppletSystem( void )
{
    //update zorder
    MMK_ZorderSystemUpdate();
    
    //post message to first applet, clear free modules
    MMK_PostMsg( MMK_GetFirstAppletHandle(), MSG_APPLET_CLEAR_FREE_MODULE, PNULL, 0 );
}

/*****************************************************************************/
// 	Description : stop applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseApplet( 
                               MMI_HANDLE_T applet_handle
                               )
{
    BOOLEAN            result          = FALSE;

    applet_handle = MMK_ConvertIdToHandle( applet_handle );

    if ( CloseApplet( applet_handle ) )
    {
        UpdateAppletSystem();
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : start applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_StartApplet( 
                                    const MMI_APPLET_START_T* start_ptr
                                    )
{
    MMI_TREE_NODE_T* tree_node = PNULL;
    MMI_HANDLE_T     applet_handle = 0;
    BOOLEAN          is_first_applet = FALSE;
    MMI_APPLET_SYSTEM_T *applet_system_ptr = GetAppletSystem();
    MMI_APPLET_NODE_T*   applet_node_ptr = PNULL;

    if ( PNULL == start_ptr )
    {
        return 0;
    }

    //SCI_TRACE_LOW:"MMK_StartApplet, guid = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_421_112_2_18_3_23_6_6,(uint8*)"d", start_ptr->guid );

    //first applet
    if( PNULL == applet_system_ptr->first_applet_node_ptr )
    {
        is_first_applet = TRUE;
    }
    else
    {
        //不支持重入, 只支持单实例
        if ( !( start_ptr->state & MMI_APPLET_STATE_DYNAMIC ) )
        {
            if ( 0 != ( applet_handle = MMK_ConvertIdToHandle( start_ptr->guid ) ) )
            {
                //非后台启动applet
                if ( !( start_ptr->state & MMI_APPLET_STATE_START_BG ) )
                {
                    MMK_GrabWinFocusByApplet( applet_handle );

                    MMK_PostMsg( applet_handle, MSG_APPLET_SWITCH, PNULL, 0 );
                }

                return applet_handle;
            }
        }
    }

    //MMK_AddTreeNode 如果失败, 需要用户销毁tree node
    tree_node = MMK_AddTreeNode( AppletTreeNodeConstruct, 
        0, (uint32)start_ptr, &applet_system_ptr->first_applet_node_ptr/*暂时不支持父子应用*/, PNULL );

    if( PNULL != tree_node )
    {
        applet_handle = tree_node->data;

        //first applet, set proc message handle
        if( is_first_applet )
        {
            applet_system_ptr->cur_applet_handle = applet_handle;

            MMK_SetProcMsgHandle( applet_handle );
        }

        MMK_SendMsg( applet_handle, MSG_APPLET_START, start_ptr->param_ptr );

        //start消息中applet可能被关闭
        if ( PNULL != (applet_node_ptr = GetAppletNodePtr( applet_handle )) )
        {
            SCI_TICK_TIME_T tick_time = {0};
            applet_node_ptr->is_open = TRUE;
            SCI_GetTickTime(&tick_time);
            applet_node_ptr->start_time = tick_time.second;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_StartApplet failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_477_112_2_18_3_23_6_7,(uint8*)"" );
    }
    
    return applet_handle;
}

/*****************************************************************************/
// 	Description : get the mmi system pointer
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_APPLET_SYSTEM_T* GetAppletSystem(void)
{
    return &s_applet_system;
}

/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AppletTreeNodeConstruct( 
                                      MMI_TREE_NODE_T* node_ptr,
                                      uint32 param1,
                                      uint32 param2
                                      )
{
    BOOLEAN                   result     = FALSE;
    const MMI_APPLET_START_T* start_ptr  = PNULL;
    void* object_ptr = PNULL;

    start_ptr = (MMI_APPLET_START_T*)param2;

    SCI_ASSERT( PNULL != start_ptr );/*assert verified*/
    
    if( CAF_RESULT_SUCCEEDED == MMKMNG_CreateInstance( start_ptr->guid, &object_ptr ) )
    {
        MMI_APPLET_NODE_T*   applet_node_ptr = (MMI_APPLET_NODE_T*)MMK_CreateBaseNode( MMI_HANDLE_APPLET, sizeof(MMI_APPLET_NODE_T) );
        
        SCI_ASSERT( PNULL != object_ptr );/*assert verified*/
        SCI_ASSERT(PNULL != applet_node_ptr);/*assert verified*/
        
        applet_node_ptr->guid           = start_ptr->guid;
        applet_node_ptr->applet_ptr     = object_ptr;
        applet_node_ptr->tree_node_ptr  = node_ptr;
        applet_node_ptr->state          = start_ptr->state;
        
        IAPPLET_SetAppletHandle( object_ptr, applet_node_ptr->base_node.handle );
        
        node_ptr->data = applet_node_ptr->base_node.handle;
        
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : applet node destruct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AppletTreeNodeDestruct( 
                                     uint32 handle
                                     )

{
    BOOLEAN result = FALSE;
    MMI_APPLET_NODE_T* applet_node_ptr = GetAppletNodePtr( handle );

    if ( PNULL != applet_node_ptr )
    {
        //SCI_ASSERT( PNULL != applet_node_ptr );/*assert verified*/
        //SCI_ASSERT( !applet_node_ptr->is_will_close );/*assert verified*/
        
        applet_node_ptr->is_will_close = TRUE;
        
        MMK_SendMsg( applet_node_ptr->base_node.handle, MSG_APPLET_STOP, PNULL );
        
        MMK_DeleteMSGByHandle( applet_node_ptr->base_node.handle );
        MMK_DeleteTimerByHandle( applet_node_ptr->base_node.handle );
        MMK_CloseGsensor( applet_node_ptr->base_node.handle );
        
        if( 0 != IAPPLET_Release( applet_node_ptr->applet_ptr ) )
        {
            SCI_ASSERT( FALSE );/*assert verified*/
        }
        
        SCI_FREE( applet_node_ptr->tree_node_ptr );
        
        MMI_FreeRes(applet_node_ptr->base_node.handle);
        MMK_DestroyBaseNode( (MMI_BASE_NODE_T*)applet_node_ptr );
        
        result = TRUE;
    }
    else
    {
        SCI_TRACE_LOW("AppletTreeNodeDestruct: this applet has been closed");
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : applet node destruct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CloseAppletWithWin( 
                                 MMI_APPLET_NODE_T* applet_node_ptr
                                 )
{
    BOOLEAN result = FALSE;

    SCI_ASSERT( PNULL != applet_node_ptr );/*assert verified*/

    if ( ( applet_node_ptr->state & MMI_APPLET_STATE_CLOSE_WITH_WIN )
        && PNULL == applet_node_ptr->win_tree_root_ptr )
    {
        result = MMK_CloseApplet( applet_node_ptr->base_node.handle );
    }
    
    //避免在closewin中关闭applet，无法刷新zorder
    if ( !result )
    {
        MMK_ZorderSystemUpdate();
    }

    return result;
}

/*****************************************************************************/
// 	Description : close a opened window (need window id)
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseWin(
					        MMI_HANDLE_T win_handle
                           )
{
    BOOLEAN            result          = FALSE;
    MMI_APPLET_NODE_T* applet_node_ptr = PNULL;
    MMI_HANDLE_T       applet_handle   = 0;
    
    win_handle = MMK_ConvertIdToHandle( win_handle );

    applet_handle = MMK_GetAppletHandleByWin( win_handle );

    if( PNULL != ( applet_node_ptr = GetAppletNodePtr( applet_handle ) )
        && MMK_IsNeedCloseWin( win_handle ) )
    {   
        MMI_TREE_NODE_T* win_node_ptr = MMK_GetWinTreeNode( win_handle );

        SCI_ASSERT( PNULL != win_node_ptr );/*assert verified*/
        
        MMK_SetWinClose( win_handle );
        MMK_RemoveTreeNode( &applet_node_ptr->win_tree_root_ptr, MMK_WindowTreeNodeDestruct, win_node_ptr );

        CloseAppletWithWin( applet_node_ptr );
        
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_CloseWin failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_625_112_2_18_3_23_6_8,(uint8*)"" );
    }

    return result;
}

/*****************************************************************************/
// 	Description : 关闭applet下所有窗口
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseAllWinByApplet(
                                       MMI_HANDLE_T applet_handle
                                       )
{
    BOOLEAN            result          = FALSE;
    MMI_APPLET_NODE_T* applet_node_ptr = PNULL;
    
    applet_handle = MMK_ConvertIdToHandle( applet_handle );

    if( PNULL != ( applet_node_ptr = GetAppletNodePtr( applet_handle ) ) )
    {   
        MMK_RemoveAllTreeNode( &applet_node_ptr->win_tree_root_ptr, MMK_WindowTreeNodeDestruct );

        CloseAppletWithWin( applet_node_ptr );
        
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_CloseAllWinByApplet failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_654_112_2_18_3_23_6_9,(uint8*)"" );
    }

    return result;
}

/*****************************************************************************/
// 	Description : get the window handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandle(
                                     MMI_HANDLE_T applet_handle,
                                     MMI_WIN_ID_T win_id
                                     )
{
    MMI_HANDLE_T win_handle = 0;
    
    applet_handle = MMK_ConvertIdToHandle( applet_handle );

    //wind id为0, 只能使用handle
    if ( 0 != win_id )
    {
        MMI_APPLET_NODE_T* applet_node_ptr = GetAppletNodePtr( applet_handle );
        
        if( PNULL != applet_node_ptr )
        {
            win_handle = MMK_FindAllTreeNode( applet_node_ptr->win_tree_root_ptr, 
                MMK_WindowTreeNodeCompare, MMI_TREE_NODE_FIND_ID, win_id, MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD );
        }
        else
        {
            //SCI_TRACE_LOW:"MMK_GetWinHandle failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_685_112_2_18_3_23_6_10,(uint8*)"" );
        }
    }

    return win_handle;
}

/*****************************************************************************/
// 	Description : get applet instance
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC IAPPLET_T* MMK_GetAppletPtr(
                                   MMI_HANDLE_T applet_handle
                                   )
{
    IAPPLET_T*         applet_ptr      = PNULL;
    MMI_APPLET_NODE_T* applet_node_ptr = PNULL;

    applet_handle = MMK_ConvertIdToHandle( applet_handle );
    
    if( PNULL != ( applet_node_ptr = GetAppletNodePtr( applet_handle ) ) )
    {
        applet_ptr = applet_node_ptr->applet_ptr;
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_GetAppletPtr failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_711_112_2_18_3_23_7_11,(uint8*)"" );
    }

    return applet_ptr;
}

/*****************************************************************************/
// 	Description : get the window list pointer  
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC IAPPLET_T* MMK_GetAppletPtrByWin(
                                        MMI_HANDLE_T win_handle
                                        )
{   
    return MMK_GetAppletPtr( MMK_GetAppletHandleByWin( win_handle ) );
}

/*****************************************************************************/
// 	Description : 查找句柄或id的回调函数
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
// LOCAL BOOLEAN BaseNodeCompare( 
//                               MMI_BASE_NODE_T* node_ptr,
//                               uint32 condition1, 
//                               uint32 condition2
//                               ) 
// {
//     BOOLEAN result = FALSE;
// 
//     if( MMI_HANDLE_APPLET == node_ptr->handle_type )
//     {
//         MMI_APPLET_NODE_T* applet_node_ptr = (MMI_APPLET_NODE_T*)node_ptr;
//         
//         if( applet_node_ptr->guid == condition2 )
//         {
//             result = TRUE;
//         }
//     }
// 
//     return result;
// }

/*****************************************************************************/
// 	Description : 根据guid获得应用的句柄
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
// PUBLIC MMI_HANDLE_T MMK_GetAppletHandleByGuid(
//                                               CAF_GUID_T guid
//                                               )
// {
//     MMI_HANDLE_T applet_handle = 0;
//     IAPPLET_T*   applet_ptr = MMK_GetAppletPtrByGuid( guid, 0, TRUE );
//     
//     if ( PNULL != applet_ptr )
//     {
//         applet_handle = IAPPLET_GetAppletHandle( applet_ptr );
//     }
// 
//     return applet_handle;
// }

/*****************************************************************************/
// 	Description : 根据guid获得应用的指针
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
// PUBLIC IAPPLET_T* MMK_GetAppletPtrByGuid(
//                                          CAF_GUID_T   guid,         //applet的guid
//                                          MMI_HANDLE_T first_handle, //查找的起始handle, 为0则默认从头\尾查找
//                                          BOOLEAN      is_order      //顺序\逆序查找
//                                          )
// {
//     IAPPLET_T*         applet_ptr      = PNULL;
//     MMI_APPLET_NODE_T* applet_node_ptr = PNULL;
// 
//     //first_handle必须是applet的handle, 否则无效
//     if( 0 != first_handle && MMI_HANDLE_APPLET != MMK_GetHandleType( first_handle ) )
//     {
//         return PNULL;
//     }
// 
//     applet_node_ptr = (MMI_APPLET_NODE_T*)MMK_FindInBaseNodeByCondition( BaseNodeCompare, 0, guid, first_handle, is_order );
// 
//     if( PNULL != applet_node_ptr )
//     {
//         applet_ptr = applet_node_ptr->applet_ptr;
//     }
// 
//     return applet_ptr;
// }

/*****************************************************************************/
// 	Description : get the guid by handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_GUID_T MMK_GetGuidByHandle(
                                      MMI_HANDLE_T handle
                                      )
{
    CAF_GUID_T         guid            = 0;
    MMI_HANDLE_T       applet_handle   = 0;
    MMI_APPLET_NODE_T* applet_node_ptr = PNULL;
    MMI_BASE_NODE_T*   base_node_ptr   = MMK_FindBaseNode( handle );
    
    if( PNULL != base_node_ptr )
    {
        switch( base_node_ptr->handle_type )
        {
        case MMI_HANDLE_APPLET:
            applet_handle = base_node_ptr->handle;
            break;
            
        case MMI_HANDLE_WINDOW:
            applet_handle = MMK_GetAppletHandleByWin( base_node_ptr->handle );
            break;

        case MMI_HANDLE_CONTROL:
            applet_handle = MMK_GetAppletHandleByWin( MMK_GetWinHandleByCtrl( base_node_ptr->handle ) );
            break;

        default:
            //SCI_ASSERT( FALSE );
            break;
        }
        
        if( PNULL != ( applet_node_ptr = GetAppletNodePtr( applet_handle ) ) )
        {
            guid = applet_node_ptr->guid;
        }
    }
    
    return guid;
}
/*****************************************************************************/
//     Description : get applet start time
//    Global resource dependence : 
//  Author:fen.xie
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMK_GetAppletStartTime( 
                                  MMI_HANDLE_T       applet_handle
                                  )
{
    MMI_APPLET_NODE_T* applet_node_ptr = PNULL;
    
    applet_handle = MMK_ConvertIdToHandle( applet_handle );

    if( PNULL != ( applet_node_ptr = GetAppletNodePtr( applet_handle ) ) )
    {
        return applet_node_ptr->start_time;
    }

    return 0;
}

/*****************************************************************************/
// 	Description : get the window list pointer  
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_APPLET_NODE_T* GetAppletNodePtr(
                                          MMI_HANDLE_T applet_handle
                                          )
{
    MMI_APPLET_NODE_T* applet_node_ptr = PNULL;
    MMI_BASE_NODE_T*   base_node_ptr   = MMK_FindBaseNode( applet_handle );
    
    if( PNULL != base_node_ptr && MMI_HANDLE_APPLET == base_node_ptr->handle_type )
    {
        applet_node_ptr = (MMI_APPLET_NODE_T*)base_node_ptr;
        
        SCI_ASSERT( PNULL != applet_node_ptr->applet_ptr );/*assert verified*/
    }
    
    return applet_node_ptr;
}

/*****************************************************************************/
// 	Description : remove win node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_RemoveWinNode(
                                 MMI_HANDLE_T    applet_handle,
                                 MMI_HANDLE_T    win_handle
                                 )
{
    BOOLEAN result = FALSE;
    MMI_APPLET_NODE_T*   applet_node_ptr = GetAppletNodePtr( applet_handle );
    MMI_TREE_NODE_T* win_node_ptr = MMK_GetWinTreeNode( win_handle );
    
    if ( PNULL != applet_node_ptr 
        && PNULL != win_node_ptr )
    {
        MMK_SetWinClose( win_handle );
        MMK_RemoveTreeNode( &applet_node_ptr->win_tree_root_ptr, MMK_WindowTreeNodeDestruct, win_node_ptr );
        
        result = TRUE;
    }
    
    return result;
}

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
                                      )
{
	MMI_APPLET_NODE_T*   applet_node_ptr = GetAppletNodePtr( applet_handle );
	MMI_RESULT_E         recode          = MMI_RESULT_FALSE;

	if ( PNULL != applet_node_ptr )
	{
        //SCI_TRACE_LOW:"MMK_RunAppletProc, applet_handle = 0x%x, guid = 0x%x, msg_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_924_112_2_18_3_23_7_12,(uint8*)"ddd", applet_handle, applet_node_ptr->guid, msg_id );

		recode = IAPPLET_HandleEvent( applet_node_ptr->applet_ptr, msg_id, param );
	}
    else
    {
        //SCI_TRACE_LOW:"MMK_RunAppletProc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_930_112_2_18_3_23_7_13,(uint8*)"" );
    }

	return recode;
}

/*****************************************************************************/
// 	Description : get first applet handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 获得idle applet的句柄
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetFirstAppletHandle( void )
{
    MMI_HANDLE_T        applet_handle      = 0;
    MMI_APPLET_SYSTEM_T *applet_system_ptr = GetAppletSystem();

    SCI_ASSERT(PNULL != applet_system_ptr);/*assert verified*/

    if( PNULL != applet_system_ptr->first_applet_node_ptr )
    {
        applet_handle = applet_system_ptr->first_applet_node_ptr->data;
    }

    return applet_handle;
}

/*****************************************************************************/
// 	Description : return to the idle window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ReturnIdleWin(
                                 void
                                 )
{
    extern MMI_WIN_ID_T  g_idle_win;
    MMI_HANDLE_T         idle_handle = MMK_GetWinHandle( MMK_GetFirstAppletHandle(), g_idle_win );
#ifdef MCARE_V31_SUPPORT
    McareV31_RunToIdle();
#endif
    
	//change calling sequence for CR NEWMS00119762
    ReturnAppointedWin( idle_handle );
    ReturnFirstApplet();

    UpdateAppletSystem();

    return TRUE;
}

/*****************************************************************************/
// 	Description : return the appointed window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ReturnAppointedWin(
                                      MMI_HANDLE_T win_handle // window id
                                      )
{
    BOOLEAN result = ReturnAppointedWin( win_handle );
    
    if ( result )
    {
        MMK_ZorderSystemUpdate();
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : return the appointed window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ReturnAppointedWin(
                                 MMI_HANDLE_T win_handle // window id
                                 )
{
    BOOLEAN              result            = FALSE;
    MMI_APPLET_NODE_T*   applet_node_ptr   = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( applet_node_ptr = GetAppletNodePtr( MMK_GetAppletHandleByWin( win_handle ) ) )
        && MMK_IsNeedCloseWin( win_handle ) )
    {
        BOOLEAN is_update = FALSE;
        uint32 i = 0;
        MMI_TREE_TRAVEL_INFO_T* travel_ptr = MMK_CreateTreeTravelInfo();
        
        MMK_TravelTree( travel_ptr, applet_node_ptr->win_tree_root_ptr, 0 );

        for ( ; i < travel_ptr->total_num; i++ )
        {
            if( travel_ptr->data_ptr[i] != win_handle )
            {
                MMI_TREE_NODE_T* win_node_ptr = MMK_GetWinTreeNode( travel_ptr->data_ptr[i] );
                
                if( PNULL != win_node_ptr
                    //窗口没获得过焦点, 不关闭
                    && MMK_IsWinGotFocus( travel_ptr->data_ptr[i] )
                    //窗口不允许被返回关闭, 不关闭
                    && !MMK_GetWinDisplayStyleState( travel_ptr->data_ptr[i], WS_DISABLE_RETURN_WIN )
                    && MMK_IsNeedCloseWin( travel_ptr->data_ptr[i] )
                    && !( applet_node_ptr->state & MMI_APPLET_STATE_DISABLE_RETURN_IDLE ) )
                {
                    MMK_SetWinClose( win_node_ptr->data );
                    MMK_RemoveTreeNode( &applet_node_ptr->win_tree_root_ptr, MMK_WindowTreeNodeDestruct, win_node_ptr );
                    is_update = TRUE;
                }
            }
            else
            {
                break;
            }
        }

        MMK_DestroyTreeTravelInfo( travel_ptr );

        if ( is_update )
        {
            result = TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_ReturnAppointedWin failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_1041_112_2_18_3_23_7_14,(uint8*)"" );
    }

    return result;
}

/*****************************************************************************/
// 	Description : 关闭first applet里的指定id范围内的窗口
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseMiddleWin(
                                  MMI_WIN_ID_T begin_win_id, 
                                  MMI_WIN_ID_T end_win_id    
                                  )
{
    return MMK_CloseMiddleWinByApplet( MMK_GetFirstAppletHandle(), begin_win_id, end_win_id );
}

/*****************************************************************************/
// 	Description : 关闭指定applet里的指定id范围内的窗口
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseMiddleWinByApplet(
                                          MMI_HANDLE_T applet_handle,
                                          MMI_WIN_ID_T begin_win_id, 
                                          MMI_WIN_ID_T end_win_id    
                                          )
{
    BOOLEAN              result            = FALSE;
    MMI_APPLET_NODE_T*   applet_node_ptr   = PNULL;

    if ( begin_win_id == end_win_id )
    {
        //SCI_TRACE_LOW:"MMK_CloseMiddleWin: begin_win_id = end_win_id = %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_1074_112_2_18_3_23_7_15,(uint8*)"d", end_win_id );
        
        return FALSE;
    }

    applet_handle = MMK_ConvertIdToHandle( applet_handle );
    
    if( PNULL != ( applet_node_ptr = GetAppletNodePtr( applet_handle ) ) )
    {
        uint32 i = 0;
        BOOLEAN is_update = FALSE;
        MMI_TREE_TRAVEL_INFO_T* travel_ptr = MMK_CreateTreeTravelInfo();
        
        MMK_TravelTree( travel_ptr, applet_node_ptr->win_tree_root_ptr, 0 );

        for ( ; i < travel_ptr->total_num; i++ )
        {
            MMI_TREE_NODE_T* win_node_ptr = MMK_GetWinTreeNode( travel_ptr->data_ptr[i] );
            
            if( PNULL != win_node_ptr
                && MMK_IsNeedCloseWin( travel_ptr->data_ptr[i] ) )
            {
                MMI_WIN_ID_T win_id = MMK_GetWinId( travel_ptr->data_ptr[i] );

                if ( win_id <= end_win_id && win_id >= begin_win_id )
                {
                    MMK_SetWinClose( win_node_ptr->data );
                    MMK_RemoveTreeNode( &applet_node_ptr->win_tree_root_ptr, MMK_WindowTreeNodeDestruct, win_node_ptr );
                    is_update = TRUE;
                }
            }
        }

        MMK_DestroyTreeTravelInfo( travel_ptr );

        if ( is_update )
        {            
            CloseAppletWithWin( applet_node_ptr );
        }
        
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_CloseMiddleWin failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_1118_112_2_18_3_23_7_16,(uint8*)"" );
    }

    return result;
}

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
                                        )
{
    BOOLEAN            result          = FALSE;
    MMI_APPLET_NODE_T* applet_node_ptr = GetAppletNodePtr( applet_handle );
    
    if( PNULL != applet_node_ptr )
    {
        switch( msg_id )
        {
        case MSG_APPLET_STOP:
            MMK_RemoveAllTreeNode( &applet_node_ptr->win_tree_root_ptr, MMK_WindowTreeNodeDestruct );
            break;

        case MSG_APPLET_CLEAR_FREE_MODULE:
            //clear free module
            MMKMNG_ClearFreeModule();
            break;

        case MSG_UPDATE_SCREEN:
            MMK_UpdateScreen();
            break;
            
        default:
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_ProcSpecialAppletMsg failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_1160_112_2_18_3_23_7_17,(uint8*)"" );
    }
    
    return result;
}

/*****************************************************************************/
//     Description : set applet state
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetAppletState( 
                                  MMI_HANDLE_T       applet_handle,
                                  MMI_APPLET_STATE_T state,
                                  BOOLEAN            is_true
                                  )
{
    BOOLEAN            result          = FALSE;
    MMI_APPLET_NODE_T* applet_node_ptr = PNULL;
    
    applet_handle = MMK_ConvertIdToHandle( applet_handle );

    if( PNULL != ( applet_node_ptr = GetAppletNodePtr( applet_handle ) ) )
    {
        if( is_true )
        {
            applet_node_ptr->state |= state;
        }
        else
        {
            applet_node_ptr->state &= ~state;
        }

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : get applet state
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetAppletState( 
                                  MMI_HANDLE_T       applet_handle,
                                  MMI_APPLET_STATE_T state
                                  )
{
    BOOLEAN            result          = FALSE;
    MMI_APPLET_NODE_T* applet_node_ptr = PNULL;
    
    applet_handle = MMK_ConvertIdToHandle( applet_handle );

    if( PNULL != ( applet_node_ptr = GetAppletNodePtr( applet_handle ) ) )
    {
        result = (BOOLEAN)( ( applet_node_ptr->state & state ) == state );
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : switch applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:会给applet发送suspend\resume消息
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SwitchApplet(
                                MMI_HANDLE_T cur_applet_handle
                                )
{
    BOOLEAN              result            = FALSE;
    MMI_APPLET_SYSTEM_T *applet_system_ptr = GetAppletSystem();

    SCI_ASSERT( PNULL != applet_system_ptr );/*assert verified*/

    if ( cur_applet_handle != applet_system_ptr->cur_applet_handle )
    {
        MMI_APPLET_NODE_T* old_node_ptr = GetAppletNodePtr( applet_system_ptr->cur_applet_handle );
        MMI_APPLET_NODE_T* cur_node_ptr = GetAppletNodePtr( cur_applet_handle );
        
        if ( PNULL != cur_node_ptr )
        {
            if( PNULL != old_node_ptr
                && !old_node_ptr->is_will_close )
            {
                MMK_SendMsg( applet_system_ptr->cur_applet_handle, MSG_APPLET_SUSPEND, PNULL );
            }

            //start applet时打开窗口不需要resume
            if( cur_node_ptr->is_open )
            {
                MMK_SendMsg( cur_applet_handle, MSG_APPLET_RESUME, PNULL );
            }

            applet_system_ptr->cur_applet_handle = cur_applet_handle;
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : create applet info list
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:创建applet信息list
/*****************************************************************************/
PUBLIC MMI_APPLET_INFO_LIST_T* MMK_CreateAppletInfoList(
                                                        MMI_APPLET_INFO_STATE_T state
                                                        )
{
    MMI_APPLET_INFO_LIST_T* list_ptr          = PNULL;
    MMI_APPLET_SYSTEM_T*    applet_system_ptr = GetAppletSystem();
    MMI_TREE_TRAVEL_INFO_T* travel_ptr        = MMK_CreateTreeTravelInfo();
    MMI_TREE_TRAVEL_STATE_T tree_state        = ( state & MMI_APPLET_INFO_BY_ORDER ) ? MMI_TREE_TRAVEL_ORDER : 0;
    
    SCI_ASSERT( PNULL != applet_system_ptr );/*assert verified*/

    MMK_TravelTree( travel_ptr, applet_system_ptr->first_applet_node_ptr, tree_state );

    if ( travel_ptr->total_num > 0 )
    {
        uint32 i = 0;
        uint8* buffer_ptr = SCI_ALLOC_APP( sizeof(MMI_APPLET_INFO_LIST_T) + sizeof(MMI_APPLET_INFO_T) * travel_ptr->total_num );

        SCI_MEMSET( buffer_ptr, 0, sizeof(MMI_APPLET_INFO_LIST_T) + sizeof(MMI_APPLET_INFO_T) * travel_ptr->total_num );

        list_ptr = (MMI_APPLET_INFO_LIST_T*)buffer_ptr;
        list_ptr->num_of_list = travel_ptr->total_num;
        list_ptr->list_ptr = (MMI_APPLET_INFO_T*)( buffer_ptr + sizeof(MMI_APPLET_INFO_LIST_T) );

        for ( i = 0; i < travel_ptr->total_num; i++ )
        {
            list_ptr->list_ptr[i].handle = travel_ptr->data_ptr[i];

            if ( applet_system_ptr->cur_applet_handle == travel_ptr->data_ptr[i] )
            {
                list_ptr->cur_index = i;
            }

            list_ptr->list_ptr[i].guid   = MMK_GetGuidByHandle( travel_ptr->data_ptr[i] );
        }
    }

    MMK_DestroyTreeTravelInfo( travel_ptr );

    return list_ptr;
}

/*****************************************************************************/
// 	Description : destory applet info list
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:销毁applet信息的list
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyAppletInfoList( 
                                         MMI_APPLET_INFO_LIST_T* list_ptr
                                         )
{
    BOOLEAN result = FALSE;

    if( PNULL != list_ptr )
    {
        SCI_FREE( list_ptr );

        result = TRUE;
    }

    return result;
}
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : start DLL
//	Global resource dependence : 
//  Author:andrew.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_StartDLL(
                                 const MMI_APPLET_START_T* start_ptr,
                                 void **object_pptr
                                 )
{
    MMI_TREE_NODE_T* tree_node = PNULL;
    MMI_HANDLE_T     applet_handle = 0;
    BOOLEAN          is_first_applet = FALSE;
    MMI_APPLET_SYSTEM_T *applet_system_ptr = GetAppletSystem();
    MMI_APPLET_NODE_T   *applet_node_ptr = PNULL;
    
    SCI_ASSERT( PNULL != start_ptr );/*assert verified*/
    
    //SCI_TRACE_LOW:"MMK_StartApplet, guid = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_1344_112_2_18_3_23_8_18,(uint8*)"d", start_ptr->guid );
    
    if( PNULL == applet_system_ptr->first_applet_node_ptr )
    {
        is_first_applet = TRUE;
    }
    
    //MMK_AddTreeNode 如果失败, 需要用户销毁tree node
    tree_node = MMK_AddTreeNode( AppletTreeNodeConstruct, 
        0, (uint32)start_ptr, &applet_system_ptr->first_applet_node_ptr/*暂时不支持父子应用*/, PNULL );
    
    if( PNULL != tree_node )
    {
        applet_handle = tree_node->data;
        
        applet_node_ptr = (MMI_APPLET_NODE_T*)GetAppletNodePtr(applet_handle);
        SCI_ASSERT(PNULL != applet_node_ptr);/*assert verified*/
        *object_pptr = (void *)(applet_node_ptr->applet_ptr);
        
        //first applet, set proc message handle
        if( is_first_applet )
        {
            MMK_SetProcMsgHandle( applet_handle );
        }
        
        MMK_SendMsg( applet_handle, MSG_APPLET_START, start_ptr->param_ptr );
    }
    else
    {
        SCI_ASSERT( FALSE );/*assert verified*/
    }
    
    
    return applet_handle;
}

/*****************************************************************************/
// 	Description : stop DLL
//	Global resource dependence : 
//  Author:andrew.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseDLL( 
                            MMI_HANDLE_T applet_handle
                            )
{
    BOOLEAN            result          = FALSE;
    MMI_APPLET_NODE_T* applet_node_ptr = GetAppletNodePtr( applet_handle );
    
    if( PNULL != applet_node_ptr
        && !applet_node_ptr->is_will_close )
    {
        MMI_APPLET_SYSTEM_T* applet_system_ptr = GetAppletSystem();
        SCI_ASSERT(PNULL != applet_system_ptr);/*assert verified*/

        applet_node_ptr->is_will_close = TRUE;
        MMK_RemoveTreeNode( &applet_system_ptr->first_applet_node_ptr, AppletTreeNodeDestruct, applet_node_ptr->tree_node_ptr );

        UpdateAppletSystem();
        
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_CloseApplet failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APPLET_1410_112_2_18_3_23_8_19,(uint8*)"" );
    }
    
    return result;
}
#endif
/*****************************************************************************/
// 	Description : check the applet is active or not
//	Global resource dependence : 
//  Author:aoke.hu
//	Note:判断applet是处于前台还是后台运行
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsActiveApplet(
                                  MMI_HANDLE_T applet_handle
                                  )
{
    MMI_WIN_ID_T win_handle = 0;
    MMI_HANDLE_T cur_applet_handle = 0;

    applet_handle = MMK_ConvertIdToHandle(applet_handle);
    win_handle = MMK_GetFocusWinHandle();
    cur_applet_handle = MMK_GetAppletHandleByWin(win_handle);

    if(cur_applet_handle == applet_handle)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : search static applet guid
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
LOCAL BOOLEAN SearchStaticAppletGuid( 
                                     const MMI_LINK_NODE_T* node_ptr, 
                                     uint32 condition1, 
                                     uint32 condition2
                                     )
{
    if( condition1 == (uint32)((CAF_STATIC_APPLET_T*)node_ptr->data)->guid )
    {
        return TRUE;
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : get static applet ptr
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC const CAF_STATIC_APPLET_T* MMK_GetStaticAppletPtr( 
                                                         CAF_GUID_T guid
                                                         )
{
    MMI_MODINFO_NODE_T* modinfo_ptr = MMK_SearchModInfo( guid, MMI_QUERY_BY_GUID );
    const CAF_STATIC_APPLET_T *static_applet_ptr = PNULL;
    
    if ( PNULL != modinfo_ptr
        && CAF_MOD_STATIC == modinfo_ptr->mod_type )
    {
        MMI_LINK_NODE_T* node_ptr = MMILINK_SearchNode( modinfo_ptr->module_info.class_info_ptr, TRUE,
            SearchStaticAppletGuid, guid, 0 );
        
        if ( PNULL != node_ptr )
        {
            static_applet_ptr = (const CAF_STATIC_APPLET_T*)node_ptr->data;
        }
    }
    
    return static_applet_ptr;
}

/*****************************************************************************/
// 	Description : clear bg apllet
//	Global resource dependence : 
//  Author:aoke.hu
//	Note:低内存时，自动关闭后台大内存应用
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ClearBgApplet( void )
{
    BOOLEAN result = FALSE;
    
    if ( MMK_TestMemory( MMK_TEST_NO_MEM, 0 ) )
    {
        uint32 i = 0;
        MMI_APPLET_SYSTEM_T* applet_system_ptr = GetAppletSystem();
        MMI_TREE_TRAVEL_INFO_T* travel_ptr     = MMK_CreateTreeTravelInfo();
        
        SCI_ASSERT(PNULL != applet_system_ptr);/*assert verified*/
        MMK_TravelTree( travel_ptr, applet_system_ptr->first_applet_node_ptr, 0 );
        
        for ( ; i < travel_ptr->total_num; i++ )
        {
            MMI_HANDLE_T applet_handle = travel_ptr->data_ptr[i];
            
            //关闭后台大内存应用
            if ( MMK_GetAppletState( applet_handle, MMI_APPLET_STATE_MEM_CONFLICT )
                && !MMK_IsActiveApplet( applet_handle ) )
            {
                if ( CloseApplet( applet_handle ) )
                {
                    result = TRUE;
                }
            }
        }
        
        MMK_DestroyTreeTravelInfo( travel_ptr );
        
        if ( result )
        {
            UpdateAppletSystem();
        }
    }
    
    return result;
}