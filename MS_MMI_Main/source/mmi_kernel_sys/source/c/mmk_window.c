/****************************************************************************
** File Name:      mmk_window.c                                            *
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
#define MMK_WINDOW_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_kernel_sys_trc.h"
#include "mmk_window_internal.h"
#include "mmk_handle_internal.h"
#include "mmk_applet_internal.h"
#include "mmk_msg_internal.h"
#include "mmk_kbd_internal.h"
#include "mmk_module_manager.h"
#include "mmk_timer.h"
#include "cafcontrol.h"
#include "cafdc.h"
#include "mmi_theme.h"
#include "mmitheme_pubwin.h"
#include "mmitheme_tab.h"
#include "ui_layer.h"
#include "mmi_autotest.h"
#include "mmk_gsensor.h"
#include "mmi_default.h"
#include "mmitheme_special.h"
#include "mmitheme_coverbar.h"
#include "mmi_list.h"
#include "os_import.h"
/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
//#define CHILD_WIN_RECT_TOP          39
#define WIN_SOFTKEY_BUTTON_NUM      3
#define ZORDER_UPDATE_TEMP_NUM      32
#define MMI_WIN_ID_NAME_MAX_LENGTH  50

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef struct mmi_window_node_tag MMI_WINDOW_NODE_T;
typedef struct mmk_zorder_node_tag
{
    MMI_WINDOW_NODE_T*          win_node_ptr;
    //MMI_HANDLE_T                win_handle;
    BOOLEAN                     is_hide;
    BOOLEAN                     is_got_topmost; //获得过焦点
    BOOLEAN                     is_painted;     //画过
    struct mmk_zorder_node_tag* prev_ptr;
    struct mmk_zorder_node_tag* next_ptr;
}MMI_ZORDER_NODE_T;

typedef enum
{
    MMI_ZORDER_SYSTEM_UPDATE_NONE,     //不刷新
    MMI_ZORDER_SYSTEM_UPDATE_CURRENT,  //刷新当前
    MMI_ZORDER_SYSTEM_UPDATE_ALL       //从最顶层的全屏非透明窗口开始刷新
}MMI_ZORDER_UPDATE_TYPE_E;

typedef enum
{
    MMI_ZORDER_SYSTEM_FOCUS_NONE,     //need not send focus
    MMI_ZORDER_SYSTEM_FOCUS_CHILD,    //child win
    MMI_ZORDER_SYSTEM_FOCUS_NORMAL   //noraml win
}MMI_ZORDER_FOCUS_TYPE_E;

typedef struct
{	
    MMI_ZORDER_NODE_T*       top_most_ptr;
    MMI_ZORDER_NODE_T*       first_paint_ptr;

    MMI_ZORDER_NODE_T*       top_hide_ptr;

    MMI_ZORDER_UPDATE_TYPE_E update_type;
    MMI_ZORDER_FOCUS_TYPE_E  need_send_focus;
    BOOLEAN                  is_on_paint;
    BOOLEAN                  is_need_update;

}MMI_ZORDER_SYSTEM_T;

typedef struct window_tag
{	
    WINDOW_HANDLE_EVENT         ProcessMsg;     /*!< the process message function of the window	 */
    WINDOW_HANDLE_EVENT         ProcessHookMsg; /*!< hook process message function of the window */
	ADD_DATA					adddata_ptr;	/*!< the additional data point of the window */
    ADD_DATA					adddata_ex_ptr;
    ADD_DATA                    user_data_ptr;
    MMI_WIN_PRIORITY_E			win_priority;  	/*!< the priority of the window */
    MMI_HANDLE_T                win_handle;
    MMI_SOFT_ICON           	softicon_state;   /*!< the state of the soft icon */

    BOOLEAN                     is_open;        //防止窗口打开过程中被关闭
    BOOLEAN                     is_handle_open; //防止窗口处理open消息之前收到其他消息
    BOOLEAN                 	is_will_close; 	/*!< mark the window is cloing .as to say that is process the MSG_CLOSE_WINDOW */
    BOOLEAN                     is_on_paint;
    // has status bar
    BOOLEAN                     is_has_statusbar;
    BOOLEAN                     is_has_tips;
    BOOLEAN                     is_not_need_copylcd;//当时去焦点时候不拷贝LCD，如果出现重叠的窗体可以考虑使用
    //BOOLEAN                     is_horizon;
    WINDOW_SUPPORT_ANGLE        support_angle;
    LCD_ANGLE_E                 cur_angle;
    
    //not use
    //BOOLEAN         			full_paint_state; 	/*!< TRUE: Kernel will firstly process FullPaint message，FALSE: window will process FullPaint message  */
	MMI_IMAGE_ID_T				background_image_id;

    WINDOW_DISPLAY_STYLE        window_style;
	WINDOW_MOVE_STYLE		    window_move_style;
    GUI_RECT_T                  window_rect;  
    GUI_RECT_T                  client_rect;       

    GUI_BOTH_RECT_T             both_rect;

	GUI_LCD_DEV_INFO			lcd_dev_info;

	//SOFTKEY
	MMI_CTRL_ID_T				softkey_ctrl_id;

	//title
	MMI_CTRL_ID_T				title_ctrl_id;

//#ifdef GUI_CTRL_SCROLL_KEY
    //SCROLLKEY
    MMI_CTRL_ID_T				scrollkey_ctrl_id;
//#endif
}MMI_WINDOW_T;

struct mmi_window_node_tag
{
    MMI_BASE_NODE_T             base_node;
    MMI_HANDLE_T                parent_applet_handle;
    MMI_WINDOW_T*               win_ptr;
    MMI_WIN_ID_T         		win_id;

    MMI_WINDOW_NODE_T*          focus_child_win_ptr;
    MMI_BASE_NODE_T*            active_control_ptr;

    MMI_TREE_NODE_T*            ctrl_tree_root_ptr;

    MMI_ZORDER_NODE_T           zorder_node;
    MMI_TREE_NODE_T*            tree_node_ptr;
};

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

LOCAL MMI_ZORDER_SYSTEM_T s_zorder_system = {0};
LOCAL MMI_HANDLE_T        s_proc_msg_handle = 0; //note!!! zorder topmost is not focus window, like windows desktop...
LOCAL MMI_HANDLE_T        s_focus_win_handle = 0; //note!!! zorder topmost is not focus window, like windows desktop...
//LOCAL MMI_HANDLE_T        s_capture_handle = 0; //每一次触笔TP_Down、TP_Up应作用于同一个控件或窗口, 
                                 //s_capture_handle记录获得TP_Down的控件或窗口

//默认0度, 如果是横屏, 开机需要设置
LOCAL LCD_ANGLE_E s_screen_angle = LCD_ANGLE_0;

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
extern BOOLEAN MMK_ParseWinTab(    
                               MMI_HANDLE_T  win_handle,
                               uint32       *win_tab_ptr
                               );

extern void MMI_FreeRes(
                        uint32 win_id
                        );

/*****************************************************************************/
//     Description : is window disable common bg
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsWindowDisableCommonBg( 
                                      MMI_WINDOW_T *win_ptr
                                      );

/*****************************************************************************/
// 	Description : 恢复默认的layer_blt顺序，窗口切换的时候调用
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RestoreDefaultLayerBlt(
                                  MMI_HANDLE_T		win_handle
                                  );

/*****************************************************************************/
// 	Description : get window system pointer
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_ZORDER_SYSTEM_T* GetZorderSystem( void );
     
/*****************************************************************************/
// 	Description : get top most window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/                                      
LOCAL MMI_HANDLE_T GetTopmostWindow(
                                    void
                                    );

/*****************************************************************************/
// 	Description : get first paint window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetFirstPaintWindow(
                                       void
                                       );

/*****************************************************************************/
//     Description : Get current support angle
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL WINDOW_SUPPORT_ANGLE GetCurSupportAngle(
                                              void
                                              );

/*****************************************************************************/
// 	Description : create window node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_NODE_T* CreateWindowNode( 
                                          MMI_TREE_NODE_T* node_ptr,
                                          MMI_HANDLE_T     applet_handle,
                                          MMI_WINDOW_T*    win_ptr
                                          );

/*****************************************************************************/
// 	Description : destroy window node
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyWindowNode(
                                MMI_WINDOW_NODE_T* win_node_ptr				    
                                );

/*****************************************************************************/
// 	Description : destroy window resource
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyWindowResource(
                                    MMI_HANDLE_T win_handle 
                                    );

/*****************************************************************************/
// 	Description : get the window node
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_NODE_T* GetWindowNodePtr(
                                          MMI_HANDLE_T win_handle
                                          );

/*****************************************************************************/
// 	Description : get the window pointer base on window_id
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_T* GetWindowPtr(
                                 MMI_HANDLE_T win_handle
                                 );

/*****************************************************************************/
// 	Description : create a dyanmic window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_T* CreateWin(
                              const MMI_WINDOW_CREATE_T* create_ptr
                              );

/*****************************************************************************/
// 	Description : get foucs window pointer
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_NODE_T* GetFocusWinNodePtr( void );

/*****************************************************************************/
// 	Description : set the focus win
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFocusWin(
                          MMI_HANDLE_T win_handle
                          );

/*****************************************************************************/
// 	Description : 仅仅用于设置子窗口focus
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFocusChildWin(
                               MMI_HANDLE_T win_handle
                               );

/*****************************************************************************/
// 	Description : add to zorder system
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 新建窗口和切换焦点使用该函数, 关闭窗口使用zorder update
/*****************************************************************************/
LOCAL BOOLEAN AddToZorderSystem(
                                MMI_ZORDER_NODE_T* node_ptr,
                                BOOLEAN            is_hide
                                );

/*****************************************************************************/
// 	Description : remove from zorder system
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN RemoveFromZorderSystem(
                                     MMI_ZORDER_NODE_T* node_ptr
                                     );

/*****************************************************************************/
//     Description : initialise the window rectangle
//    Global resource dependence : 
//  Author: robert.lu
//    Note:
/*****************************************************************************/
LOCAL void AdjustWinRect(
                         MMI_WINDOW_T* new_win_ptr         // window pointer
                         );


/*****************************************************************************/
// 	Description : get parent win handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetParentHandle( 
                                   MMI_TREE_NODE_T* node_ptr
                                   );

/*****************************************************************************/
// 	Description : get ancestor handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:获得祖先的句柄
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetAncestorHandle( 
                                     MMI_TREE_NODE_T* node_ptr
                                     );

/*****************************************************************************/
// 	Description : create a window in parseing window table
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN WinTableCreate(
                             MMI_TREE_NODE_T* node_ptr,
                             const MMI_WINDOW_TABLE_CREATE_T* create_ptr
                             );

/*****************************************************************************/
// 	Description : create a dyanmic window and open it
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN WindowCreate(
                           MMI_TREE_NODE_T* node_ptr,
                           const MMI_WINDOW_CREATE_T* create_ptr
                           );

/*****************************************************************************/
// 	Description : the window grabs focus
//    Global resource dependence : 
//  Author:Great.Tian
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN WinGrabFocus(
                           MMI_HANDLE_T win_handle,
                           BOOLEAN      is_need_send_msg
                       );



/*****************************************************************************/
// 	Description : is need paint prev window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedPaintPrevWin( 
                                 MMI_WINDOW_NODE_T* win_node_ptr
                                 );

/*****************************************************************************/
// 	Description : post update screen message
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN PostUpdateScreenMsg( 
                                  MMI_ZORDER_UPDATE_TYPE_E type
                                  );

/*****************************************************************************/
// 	Description : set zorder update type
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetZorderSystemUpdateType(
                                        MMI_ZORDER_UPDATE_TYPE_E type
                                        );

/*****************************************************************************/
// 	Description : reset zorder update type
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void ResetZorderSystemUpdateType( void );

/*****************************************************************************/
// 	Description : set zorder focus type
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetZorderSystemFocusType(
                                       MMI_ZORDER_FOCUS_TYPE_E type
                                       );

/*****************************************************************************/
// 	Description : reset zorder focus type
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void ResetZorderSystemFocusType( void );

/*****************************************************************************/
// 	Description : find first paint zorder node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void SetZorderSystemFirstPaintNode(
                                         void
                                         );

/*****************************************************************************/
// Description : 根据类型判断屏窗口是否处于focus状态
// Global resource dependence : 
// Author:gang.tong
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsWinOnFocusByStyle( 
                                  MMI_HANDLE_T      win_handle,
                                  WINDOW_MOVE_STYLE style
                                  );

/*****************************************************************************/
// 	Description : hide win
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:隐藏窗口
/*****************************************************************************/
LOCAL BOOLEAN HideWindow(
                         MMI_ZORDER_NODE_T* node_ptr,
                         BOOLEAN            is_hide
                         );

/*****************************************************************************/
//     Description : Get angel
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:根据支持的角度和平台角度获得合适的角度
/*****************************************************************************/
LOCAL LCD_ANGLE_E GetCurAngle(
                              LCD_ANGLE_E angle,
                              WINDOW_SUPPORT_ANGLE support_angle
                              );

/*****************************************************************************/
// 	Description : is window landscape
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:用于取代MMITHEME_IsMainScreenLandscape接口, 全局变量可能和win中angle不一致
/*****************************************************************************/
LOCAL BOOLEAN IsWindowLandscape( 
                                MMI_HANDLE_T win_handle
                                );

/*****************************************************************************/
// 	Description : 是否处理过open消息
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:防止窗口收到open消息前收到其他消息
/*****************************************************************************/
LOCAL BOOLEAN IsHandleOpenMsg( 
                              MMI_HANDLE_T win_handle
                              );

/*****************************************************************************/
// 	Description : FindExistWindow
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:查找句柄池中是否存在窗口, 解决关闭窗口消息中打开id相同窗口的问题
/*****************************************************************************/
LOCAL BOOLEAN FindExistWindow( 
                              MMI_BASE_NODE_T* node_ptr,
                              uint32 condition1, 
                              uint32 condition2
                              );

/*****************************************************************************/
// 	Description : set the window pointer that is processing the message 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseNodeCompare( 
                              MMI_BASE_NODE_T* node_ptr,
                              uint32 condition1, 
                              uint32 condition2
                              );

/*****************************************************************************/
// 	Description : hide applet window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:将applet所有窗口放到zorder system的隐藏窗口队列
/*****************************************************************************/
LOCAL BOOLEAN HideAppletWindow(
                               MMI_HANDLE_T        applet_handle,
                               MMI_ZORDER_NODE_T*  top_node_ptr,
                               BOOLEAN             is_hide
                               );



/*****************************************************************************/
// 	Description : create a window in parseing window table
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  )
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = MMK_GetFirstAppletHandle();
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = win_table_ptr;
    win_table_create.add_data_ptr = add_data_ptr;

    return MMK_CreateWinTable( &win_table_create );
}


/*****************************************************************************/
// 	Description : create win by applet guid
//	Global resource dependence : 
//  Author James.Zhang
//	Note: 仅限静态applet创建窗口使用
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateWinByApplet(
                                          MMI_HANDLE_T  applet_handle,
                                          uint32*       win_table_ptr,
                                          ADD_DATA      add_data_ptr
                                          )
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
    
    win_table_create.applet_handle = applet_handle;
    win_table_create.parent_win_handle = 0;
    win_table_create.win_table_ptr = win_table_ptr;
    win_table_create.add_data_ptr = add_data_ptr;
    
    return MMK_CreateWinTable( &win_table_create );
}

/*****************************************************************************/
//     Description : switch to focus tab win
//    Global resource dependence : 
//  Author:jibin
//    Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T  MMK_CreateChildWin(
                                   MMI_WIN_ID_T	parent_win_id,
                                   uint32*		win_table_ptr,
                                   ADD_DATA		add_data_ptr		// the additional data
                                   )

{
    MMI_WINDOW_TABLE_CREATE_T create = {0};
    MMI_HANDLE_T win_handle = 0;

    create.applet_handle = MMK_GetAppletHandleByWin(parent_win_id);//MMK_GetFirstAppletHandle();
    create.parent_win_handle = parent_win_id;
    create.win_table_ptr = win_table_ptr;
    create.add_data_ptr = add_data_ptr;

    win_handle = MMK_CreateWinTable( &create );
    
    if( 0 != win_handle)
    {
#ifdef MMI_PDA_SUPPORT
        if (MMITHEME_IsIstyle())//Istyle Tab在下，也可以在每个APP的Open窗口中调用
        {
            GUI_BOTH_RECT_T win_both_rect = MMITHEME_GetWinClientBothRect(win_handle);

            if (MMK_GetWinDisplayStyleState(win_handle, WS_HAS_BUTTON_SOFTKEY))
            {
                if (MMK_IsWinIncludeSoftkey(win_handle))
                {
                    win_both_rect.h_rect.bottom += (int16)MMITHEME_GetWinSoftkeyHeight(win_handle);
                    win_both_rect.v_rect.bottom += (int16)MMITHEME_GetWinSoftkeyHeight(win_handle);
                }
            }

            MMK_SetBothRect(win_handle, &win_both_rect);
        }
#endif

        return win_handle;
    }
    else
    {
        return win_handle;
    }
}

/*****************************************************************************/
//     Description : create tab win
//    Global resource dependence : 
//  Author:jibin
//    Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateParentWin(
								 uint32*            win_table_ptr,
								 ADD_DATA           add_data_ptr		// the additional data
                                )
{
    return MMK_CreateWin( win_table_ptr, add_data_ptr );
}

/*****************************************************************************/
//     Description : switch to focus tab win
//    Global resource dependence : 
//  Author:jibin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseParentWin(
                                  MMI_HANDLE_T parent_win_handle
                                  )
{
    return MMK_CloseWin( parent_win_handle );
}

/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_WindowTreeNodeConstruct( 
                                           MMI_TREE_NODE_T* node_ptr,
                                           uint32 param1,
                                           uint32 param2
                                           )
{
#ifdef JAMES_TEST
    //SCI_TRACE_LOW:"james test, MMK_WindowTreeNodeConstruct start = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_804_112_2_18_3_23_23_34,(uint8*)"d", SCI_GetTickCount() );
#endif

    if( param1 )
    {
        return WinTableCreate( node_ptr, (const MMI_WINDOW_TABLE_CREATE_T*)param2 );
    }
    else
    {
        return WindowCreate( node_ptr, (const MMI_WINDOW_CREATE_T*)param2 );
    }
}

/*****************************************************************************/
// 	Description : window node destruct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_WindowTreeNodeDestruct( 
                                          uint32 handle
                                          )
                                          
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    if( PNULL != ( win_node_ptr = GetWindowNodePtr( handle ) ) )
    {
        MMI_WINDOW_T*      win_ptr       = PNULL;
        MMI_HANDLE_T       parent_win_handle = 0;
        
        win_ptr = win_node_ptr->win_ptr;
        
        win_ptr->is_will_close = TRUE;
        
        //有open失败的情况, 不需要发送close消息
        if ( win_ptr->is_open )
        {
            MMK_SendMsg( win_ptr->win_handle, MSG_CLOSE_WINDOW, PNULL );
        }

        if( 0 == ( parent_win_handle = GetParentHandle( win_node_ptr->tree_node_ptr ) ) )
        {
            RemoveFromZorderSystem( &win_node_ptr->zorder_node );
        }
        else
        {
            MMI_WINDOW_NODE_T* parent_win_node_ptr = GetWindowNodePtr( parent_win_handle );

            if ( PNULL != parent_win_node_ptr )
            {
                if( win_node_ptr == parent_win_node_ptr->focus_child_win_ptr )
                {
                    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();
                    
                    parent_win_node_ptr->focus_child_win_ptr = 0;
                    
                    if ( !win_node_ptr->zorder_node.is_hide )
                    {
                        SCI_ASSERT( PNULL != zorder_system_ptr );/*assert verified*/
                        SetZorderSystemFocusType(MMI_ZORDER_SYSTEM_FOCUS_CHILD);
                    }
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMK_WindowTreeNodeDestruct: parent window closed in child win MSG_CLOSE_WINDOW msg"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_869_112_2_18_3_23_24_35,(uint8*)"");
            }
        }
        
        MMI_TraceAutoTestOfWinMessage(win_node_ptr->win_id, MSG_CLOSE_WINDOW);

        DestroyWindowResource( win_ptr->win_handle );
        
        DestroyWindowNode( win_node_ptr );
        
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_WindowTreeNodeDestruct: this window has been closed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_883_112_2_18_3_23_24_36,(uint8*)"");
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : get all child win handle
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
// return actual child win number
/*****************************************************************************/
PUBLIC uint16 MMK_GetAllChildWinHandle(
                                       MMI_HANDLE_T   parent_handle,
                                       MMI_HANDLE_T   *child_win_arr_ptr,
                                       uint16         child_win_arr_size
                                       )
{
    uint16              child_num = 0;
    MMI_WINDOW_NODE_T   *win_node_ptr = PNULL;
    MMI_TREE_NODE_T     *cur_child_node_ptr = PNULL;

    parent_handle = MMK_ConvertIdToHandle( parent_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( parent_handle ) ) )
    {
        if (PNULL != win_node_ptr->tree_node_ptr->child_ptr)
        {
            cur_child_node_ptr = win_node_ptr->tree_node_ptr->child_ptr;

            do
            {
                if (child_num < child_win_arr_size && PNULL != child_win_arr_ptr)
                {
                    child_win_arr_ptr[child_num] = cur_child_node_ptr->data;
                }


                child_num++;

                cur_child_node_ptr = cur_child_node_ptr->next_ptr;
            } while (cur_child_node_ptr != win_node_ptr->tree_node_ptr->child_ptr);
        }
    }

    return child_num;
}

/*****************************************************************************/
// 	Description : 是否需要关闭窗口
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:防止窗口被重复关闭导致暴栈
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinClose( 
                               MMI_HANDLE_T win_handle
                               )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        win_node_ptr->win_ptr->is_will_close = TRUE;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : is exist window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:判断窗口是否存在, 解决关闭窗口消息中打开id相同窗口的问题
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsExistWin(
                              MMI_HANDLE_T applet_handle,
                              MMI_WIN_ID_T win_id
                              )
{
    BOOLEAN  result = FALSE;
    
    applet_handle = MMK_ConvertIdToHandle( applet_handle );
    
    //pubwin支持重入
    if( 0 != win_id 
        && PNULL != MMK_FindInBaseNodeByCondition( FindExistWindow, applet_handle, win_id, 0, FALSE ) )
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : open a window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_OpenWin(
                           MMI_HANDLE_T win_handle,
                           MMI_HANDLE_T parent_win_handle
                           )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );
    
    SCI_ASSERT( PNULL != win_node_ptr );/*assert verified*/
    
    if( parent_win_handle )
    {
        MMI_WINDOW_NODE_T* parent_node_ptr = GetWindowNodePtr( parent_win_handle );

        SCI_ASSERT( PNULL != parent_node_ptr && win_handle != parent_win_handle );/*assert verified*/
        
        if ( PNULL != parent_node_ptr->focus_child_win_ptr
            && parent_node_ptr->focus_child_win_ptr != win_node_ptr )
        {
            BOOLEAN is_tool_win = MMK_GetWinDisplayStyleState( win_handle, WS_TOOL_WIN );
            
            if ( is_tool_win )
            {
                MMK_SetWinDisplayStyleState( parent_win_handle, WS_TOOL_WIN, TRUE );
            }

            //idle下的子窗口处理比较特殊，需要允许多个子窗口处于焦点状态            
            if(!MMK_GetWinDisplayStyleState( win_handle, WS_WIDGET_WIN ) )
            {
                if(!MMK_GetWinDisplayStyleState(parent_node_ptr->focus_child_win_ptr->base_node.handle, WS_WIDGET_WIN ))
                {
                    MMK_SendMsg( parent_node_ptr->focus_child_win_ptr->base_node.handle,
                                MSG_LOSE_FOCUS, PNULL );
                }
            }
            
            if ( is_tool_win )
            {
                MMK_SetWinDisplayStyleState( parent_win_handle, WS_TOOL_WIN, FALSE );
            }
        }

        parent_node_ptr->focus_child_win_ptr = win_node_ptr;

        SetFocusChildWin( win_node_ptr->base_node.handle );
        
        MMK_SendMsg(win_node_ptr->base_node.handle, MSG_OPEN_WINDOW, PNULL);

        //加is open标志量, 否则在发送lose focus窗口可能被关闭
        win_node_ptr->win_ptr->is_open = TRUE;
        
        result = TRUE;
    }
    else
    {
        MMI_HANDLE_T new_applet_handle = win_node_ptr->parent_applet_handle;

        AddToZorderSystem( &win_node_ptr->zorder_node, FALSE );

        if ( win_node_ptr->zorder_node.is_got_topmost )
        {
            MMI_WINDOW_NODE_T* old_node_ptr = GetFocusWinNodePtr();
			
			//将来要考虑topmost和focus解藕
            SetFocusWin( win_node_ptr->base_node.handle );
			
            if( PNULL != old_node_ptr
                && PNULL != old_node_ptr->win_ptr
                && !old_node_ptr->win_ptr->is_will_close )
            {
                //此处最好改为异步消息, 防止消息的时序问题
				//cr133889, create win->lose focus->close win->create win 导致问题
                MMK_SendMsg( old_node_ptr->base_node.handle, MSG_LOSE_FOCUS, PNULL );
            }
        }
        
        if ( MMI_RESULT_TRUE == MMK_PostMsg( win_node_ptr->win_ptr->win_handle, MSG_OPEN_WINDOW, PNULL, PNULL ) )
        {  
            MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

            SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

			if( PNULL != zorder_system_ptr->top_most_ptr )
            {
//                 MMI_WINDOW_T* first_paint_win_ptr = PNULL;
//                 
//                 SCI_ASSERT( PNULL != zorder_system_ptr->first_paint_ptr->win_node_ptr );/*assert verified*/
// 
//                 first_paint_win_ptr = zorder_system_ptr->first_paint_ptr->win_node_ptr->win_ptr;
// 
//                 SCI_ASSERT( PNULL != first_paint_win_ptr );/*assert verified*/
                
					PostUpdateScreenMsg( MMI_ZORDER_SYSTEM_UPDATE_CURRENT );
            }

            //加is open标志量, 否则在发送lose focus窗口可能被关闭
            win_node_ptr->win_ptr->is_open = TRUE;

            result = TRUE;

            MMK_SwitchApplet( new_applet_handle );
            
            MMI_TraceAutoTestOfWinMessage(win_node_ptr->win_id, MSG_OPEN_WINDOW);

        }
    }

    if ( !result )
    {
        MMK_RemoveWinNode( win_node_ptr->parent_applet_handle, win_handle );
        
        MMK_ZorderSystemUpdate();

        //SCI_TRACE_LOW:"MMK_OpenWin failed, window is not open !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_1897_112_2_18_3_23_26_41,(uint8*)"");
    }

	return result;
}

/******************************************************************************/
/*! \brief check the window is open 
 *  \author Great.Tian 
 *  \param[in]	win_id			the window id 
 *  \return 
 *		- <i><c>TRUE</c></i>: 	the window is open 	
 * 		- <i><c>FALSE</c></i>:	the window is not open
 */ 
/******************************************************************************/
PUBLIC BOOLEAN MMK_IsOpenWin(
                             MMI_HANDLE_T win_handle
                             )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        result = win_node_ptr->win_ptr->is_open;
    }

    return result;
}


/*****************************************************************************/
// 	Description : 是否需要关闭窗口
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:防止窗口被重复关闭导致暴栈
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsNeedCloseWin( 
                                  MMI_HANDLE_T win_handle )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        result = (BOOLEAN)( win_node_ptr->win_ptr->is_open 
            && !win_node_ptr->win_ptr->is_will_close );
    }

    return result;
}

/*****************************************************************************/
// 	Description : is child window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsChildWin(
                              MMI_HANDLE_T win_handle
                              )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        if( PNULL != win_node_ptr->tree_node_ptr->parent_ptr )
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : update screen
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_UpdateScreen( void )
{
    BOOLEAN              result            = FALSE;
    MMI_ZORDER_NODE_T*   zorder_node_ptr   = PNULL;
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();
    MMI_HANDLE_T         handle_array[ZORDER_UPDATE_TEMP_NUM]  = {0};
    uint32               i = 0;
    uint32               num = 0;

#ifdef JAMES_TEST
    uint32 tick = SCI_GetTickCount();
#endif

    if (!MMITHEME_IsBacklightOn())
    {
        return FALSE;
    }

    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    //画屏保护, 防止在full paint时开关窗口导致的update screen
    zorder_system_ptr->is_on_paint = TRUE;

    if( PNULL != zorder_system_ptr->top_most_ptr )
    {
        //每次刷新时都需要获得first paint node
        SetZorderSystemFirstPaintNode();

        if ( PNULL != zorder_system_ptr->first_paint_ptr )
        {
            //优化仅仅打开一个窗口的情况
            if( MMI_ZORDER_SYSTEM_UPDATE_CURRENT == zorder_system_ptr->update_type
                && PNULL != zorder_system_ptr->top_most_ptr->prev_ptr
                //WS_NEED_UPDATE_PRE_WIN，解决来短信时数字提醒无法更新的问题
                && !MMK_GetWinDisplayStyleState( zorder_system_ptr->top_most_ptr->win_node_ptr->base_node.handle, WS_NEED_PAINT_PRE_WIN )
                && !IsNeedPaintPrevWin( zorder_system_ptr->top_most_ptr->prev_ptr->win_node_ptr ) )
            {
                zorder_node_ptr = zorder_system_ptr->top_most_ptr;
            }
            else//MMI_ZORDER_SYSTEM_UPDATE_NONE可能是应用主动发起的刷屏消息, 也需要刷全屏
            {
                zorder_node_ptr = zorder_system_ptr->first_paint_ptr;
            }
            //zorder_node_ptr = zorder_system_ptr->first_paint_ptr;

        }
        else//无全屏非透明窗口, 从第一个窗口开始画
        {
            zorder_node_ptr = zorder_system_ptr->top_most_ptr->next_ptr;
        }
        
        //为防止在full paint时开窗口导致刷新有问题, 用临时变量先保存遍历的handle, 然后再绘制
        do 
        {
            SCI_ASSERT( PNULL != zorder_node_ptr->win_node_ptr );/*assert verified*/

            //MMK_SendMsg( zorder_node_ptr->win_handle, MSG_FULL_PAINT, PNULL );
            handle_array[num] = zorder_node_ptr->win_node_ptr->base_node.handle;
            num++;
            
            zorder_node_ptr = zorder_node_ptr->next_ptr;
            
        } while( zorder_node_ptr != zorder_system_ptr->top_most_ptr->next_ptr
            && num < ZORDER_UPDATE_TEMP_NUM );

        //开始绘制窗口
        if( num < ZORDER_UPDATE_TEMP_NUM )
        {
            //第一个窗口为全屏窗口需要擦背景, MS00242844
            if ( handle_array[0] == zorder_system_ptr->top_most_ptr->next_ptr->win_node_ptr->base_node.handle
                && IsNeedPaintPrevWin( zorder_system_ptr->top_most_ptr->prev_ptr->win_node_ptr ) )
            {
                GUI_LCD_DEV_INFO lcd_dev_info = {0};
                GUI_RECT_T		 rect         = MMITHEME_GetFullScreenRect();

                MMITHEME_DisplayCommonBg( &rect, handle_array[0], &lcd_dev_info );
            }
            
            for( ; i < num; i++ )
            {
                if ( IsHandleOpenMsg( handle_array[i] ) )
                {
                    MMK_SendMsg( handle_array[i], MSG_FULL_PAINT, PNULL );
                }
            }
        }
        //错误处理, 需要刷新窗口超过最大限制, 只画最顶层窗口
        else
        {
            SCI_ASSERT( PNULL != zorder_system_ptr->top_most_ptr->win_node_ptr );/*assert verified*/
            
            if ( IsHandleOpenMsg( zorder_system_ptr->top_most_ptr->win_node_ptr->base_node.handle ) )
            {
                MMK_SendMsg( zorder_system_ptr->top_most_ptr->win_node_ptr->base_node.handle, MSG_FULL_PAINT, PNULL );
            }
        }

        result = TRUE;
    }

    zorder_system_ptr->is_on_paint = FALSE;
    zorder_system_ptr->is_need_update = FALSE;

    ResetZorderSystemUpdateType();

    MMITHEME_SetUpdateDelayCount( 0 );
    
#ifdef JAMES_TEST
    tick = SCI_GetTickCount() - tick;
    
    //SCI_TRACE_LOW:"james test, MMK_UpdateScreen = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_1177_112_2_18_3_23_24_38,(uint8*)"d", tick );
    
    //SCI_TRACE_LOW:"james test, MMK_UpdateScreen end = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_1179_112_2_18_3_23_24_39,(uint8*)"d", SCI_GetTickCount() );
#endif

    return result;
}

/*****************************************************************************/
// 	Description : delay update screen
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 为减少刷新, 延迟刷新屏幕
/*****************************************************************************/
PUBLIC void MMK_DelayUpdateScreen( void )
{
    //延迟刷屏, 防止冗余刷新
    MMK_DeleteMSGByHwndAndMsg( MMK_GetFirstAppletHandle(), MSG_UPDATE_SCREEN );
    
    if ( MMK_PostMsg( MMK_GetFirstAppletHandle(), MSG_UPDATE_SCREEN, PNULL, 0 ) )
    {
        //update screen前禁止任何刷屏动作, 避免横竖屏切换时的花屏
        MMITHEME_SetUpdateDelayCount( 0xFF );
    }

}

/*****************************************************************************/
// 	Description : is update screen
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:区分应用发full paint刷新和系统刷新
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsUpdateScreen( void )
{
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    return zorder_system_ptr->is_on_paint;
}

/*****************************************************************************/
// 	Description : hide applet window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:将applet所有窗口放到zorder system的隐藏窗口队列
/*****************************************************************************/
PUBLIC BOOLEAN MMK_HideAppletWindow(
                                    MMI_HANDLE_T applet_handle,
                                    BOOLEAN      is_hide
                                    )
{
    
    BOOLEAN result = FALSE;
    
    applet_handle = MMK_ConvertIdToHandle( applet_handle );

    if ( MMI_HANDLE_APPLET == MMK_GetHandleType( applet_handle )
        && MMK_GetFirstAppletHandle() != applet_handle )
    {
        MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();
        MMI_HANDLE_T         focus_handle      = GetTopmostWindow();

        SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

        //如果使窗口可见, 需要先处理隐藏的窗口
        if ( !is_hide )
        {
            result |= HideAppletWindow( applet_handle, zorder_system_ptr->top_hide_ptr, is_hide );
        }
        
        //显示或隐藏topmost的窗口
        result |= HideAppletWindow( applet_handle, zorder_system_ptr->top_most_ptr, is_hide );

        if ( result )
        {
            if ( is_hide )
            {
                if ( MMK_GetAppletHandleByWin( focus_handle ) == applet_handle )
                {
                    SetZorderSystemFocusType(MMI_ZORDER_SYSTEM_FOCUS_NORMAL);
                    
                    MMK_SendMsg( focus_handle, MSG_LOSE_FOCUS, PNULL );
                }
            }
            else
            {
                if ( MMK_GetAppletHandleByWin( focus_handle ) != applet_handle )
                {
                    SetZorderSystemFocusType(MMI_ZORDER_SYSTEM_FOCUS_NORMAL);
                    
                    MMK_SendMsg( focus_handle, MSG_LOSE_FOCUS, PNULL );
                }
            }
            
            MMK_ZorderSystemUpdate();
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : hide win by appelt
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:将applet所有窗口放到zorder system的隐藏窗口队列
/*****************************************************************************/
PUBLIC BOOLEAN MMK_HideWinByApplet(
                                   MMI_HANDLE_T applet_handle
                                   )
{
    return MMK_HideAppletWindow( applet_handle, TRUE );
}

/*****************************************************************************/
// 	Description : hide window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:隐藏\显示窗口
/*****************************************************************************/
PUBLIC BOOLEAN MMK_HideWindow(
                              MMI_HANDLE_T win_handle,
                              BOOLEAN      is_hide //是否隐藏
                              )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) )
        && 0 == GetParentHandle( win_node_ptr->tree_node_ptr ) )
    {   
        MMI_HANDLE_T focus_handle = GetTopmostWindow();

        result = HideWindow( &win_node_ptr->zorder_node, is_hide );

        if ( result )
        {
//            MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

            if ( is_hide )
            {
                if ( focus_handle == win_handle )
                {
                    SetZorderSystemFocusType(MMI_ZORDER_SYSTEM_FOCUS_NORMAL);
                    
                    MMK_SendMsg( focus_handle, MSG_LOSE_FOCUS, PNULL );
                }
            }
            else
            {
                if ( GetTopmostWindow() == win_handle )
                {
                    SetZorderSystemFocusType(MMI_ZORDER_SYSTEM_FOCUS_NORMAL);
                    
                    MMK_SendMsg( focus_handle, MSG_LOSE_FOCUS, PNULL );
                }
            }
            
            MMK_ZorderSystemUpdate();
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : zorder system update, for close window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:  关闭窗口使用该函数, zorder关系目前与focus关系绑定的太紧, 将来要考虑解耦
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ZorderSystemUpdate( void )
{
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();
	MMI_HANDLE_T focus_handle       = GetTopmostWindow();
	MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( focus_handle );

    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    if( MMI_ZORDER_SYSTEM_FOCUS_NONE != zorder_system_ptr->need_send_focus )
    {
        MMI_HANDLE_T new_applet_handle  = 0;
        BOOLEAN is_need_send_focus = (BOOLEAN)( MMI_ZORDER_SYSTEM_FOCUS_NORMAL == zorder_system_ptr->need_send_focus );
        
        if( PNULL != win_node_ptr )
        {
            //for child window
            if( PNULL != win_node_ptr->focus_child_win_ptr )
            {
                focus_handle = win_node_ptr->focus_child_win_ptr->base_node.handle;
            }
            else
            {
                MMI_TREE_NODE_T* child_node_ptr = win_node_ptr->tree_node_ptr->child_ptr;
                
                if( PNULL != child_node_ptr )
                {
                    win_node_ptr->focus_child_win_ptr = GetWindowNodePtr( child_node_ptr->data );
                    
                    focus_handle = child_node_ptr->data;
                }
            }
            
            SetFocusWin( focus_handle );
    
            ResetZorderSystemFocusType();
            //zorder_system_ptr->need_send_focus = FALSE;

            new_applet_handle = win_node_ptr->parent_applet_handle;

            if( PNULL != win_node_ptr->win_ptr
                && !win_node_ptr->win_ptr->is_will_close
                && IsHandleOpenMsg( win_node_ptr->base_node.handle )//防止窗口处理open消息之前收到其他消息
                && is_need_send_focus )
            {
                //message must send to parent
                MMK_SendMsg( win_node_ptr->base_node.handle, MSG_GET_FOCUS, PNULL );
            }

            MMK_SwitchApplet( new_applet_handle );
        }
    }
	//避免topmost为半屏窗口花屏问题
	else if( PNULL != win_node_ptr
		&& IsNeedPaintPrevWin( win_node_ptr ) )
	{
		//设置角度
		LCD_ANGLE_E angle = GetCurAngle( s_screen_angle, GetCurSupportAngle() );
		
		GUILCD_SetLogicAngle( GUI_MAIN_LCD_ID, angle );
	}

    PostUpdateScreenMsg( MMI_ZORDER_SYSTEM_UPDATE_ALL );

    return TRUE;
}

/*****************************************************************************/
// 	Description : the window grabs focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_WinGrabFocus(
	   					        MMI_HANDLE_T win_handle
					            )
{
    win_handle = MMK_ConvertIdToHandle( win_handle );

    return WinGrabFocus( win_handle, TRUE );
}

/*****************************************************************************/
// 	Description : get the focus window id,if have parent win,return parent win id
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMK_GetFocusWinId(void)
{
    uint32              win_id = 0;
    MMI_HANDLE_T        win_handle = 0;
    MMI_WINDOW_NODE_T   *win_node_ptr = PNULL;

    //get parent win handle
    win_handle = MMK_GetFocusParentWinHandle();
    if (0 == win_handle)
    {
        //no parent win,get focus child win handle
        win_handle = MMK_GetFocusWinHandle();
    }

    //get window node pointer
    win_node_ptr = GetWindowNodePtr(win_handle);
    if (PNULL != win_node_ptr)
    {
        //静态应用返回win id, 动态应用返回win hanlde
        if( !MMK_GetAppletState( win_node_ptr->parent_applet_handle, MMI_APPLET_STATE_DYNAMIC ) )
        {
            win_id = win_node_ptr->win_id;
        }
        else
        {
            win_id = win_handle;
        }
    }

    return (win_id);
}

/*****************************************************************************/
// 	Description : whether the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsFocusWin(
                              MMI_HANDLE_T win_handle  //[IN] the window id
					          )
{
    BOOLEAN result = FALSE;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    if ( 0 != win_handle  && 
        ( win_handle == MMK_GetFocusWinHandle() || win_handle == MMK_GetFocusParentWinHandle() ) )
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : is win go focus
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 窗口是否曾经获得焦点
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinGotFocus(
                                 MMI_HANDLE_T win_handle
                                 )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );

    if( PNULL != win_node_ptr )
    {
        //对低优先级窗口做特殊处理
        if ( WIN_LOWEST_LEVEL == win_node_ptr->win_ptr->win_priority )
        {
            result = ( win_node_ptr->zorder_node.is_got_topmost 
                && win_node_ptr->zorder_node.is_painted );
        }
        else
        {
            result = win_node_ptr->zorder_node.is_got_topmost;
        }

    }

    return result;
}

/*****************************************************************************/
// 	Description : get focus window id
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetFocusWinHandle(
                                          void
                                          )
{
    return s_focus_win_handle;
}

/*****************************************************************************/
//     Description : switch to focus child win
//    Global resource dependence : 
//  Author:jibin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetChildWinFocus(
									 MMI_WIN_ID_T parent_win_id,
									 MMI_WIN_ID_T win_id
									 )

{
    win_id = MMK_ConvertIdToHandle( win_id );

    return WinGrabFocus( win_id, FALSE );
}


/*****************************************************************************/
//     Description : set child win lost focus
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetChildWinLostFocus(
                                        MMI_HANDLE_T win_handle,
                                        BOOLEAN is_update
                                        ) 
{
    BOOLEAN result = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
  
    win_handle = MMK_ConvertIdToHandle( win_handle );
    
    if (!MMK_IsFocusWin(win_handle))
    {
        return result;
    }
    // get the window pointer
    win_node_ptr = GetWindowNodePtr( win_handle );
    
    if ( PNULL != win_node_ptr
        && PNULL != win_node_ptr->focus_child_win_ptr )
    {       
        MMK_SendMsg( win_node_ptr->focus_child_win_ptr->base_node.handle,
            MSG_LOSE_FOCUS, PNULL );
        
        win_node_ptr->focus_child_win_ptr = PNULL;

        SetFocusWin( win_handle );

        if ( is_update )
        {
            MMK_PostMsg( win_handle, MSG_FULL_PAINT, PNULL, 0 );
        }

        result = TRUE;
    }
    else
    {
        SCI_TRACE_LOW("MMK_SetChildWinLostFocus FALSE, win_handle = 0x%08x", win_handle);
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : get the focus child window id
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMK_GetFocusChildWinId(void)
{
    uint32              win_id = 0;
    MMI_HANDLE_T        win_handle = 0;
    MMI_WINDOW_NODE_T   *win_node_ptr = PNULL;

    //get focus child win handle
    win_handle = MMK_GetFocusWinHandle();

    //get window node pointer
    win_node_ptr = GetWindowNodePtr(win_handle);
    if (PNULL != win_node_ptr)
    {
        //静态应用返回win id, 动态应用返回win hanlde
        if( !MMK_GetAppletState( win_node_ptr->parent_applet_handle, MMI_APPLET_STATE_DYNAMIC ) )
        {
            win_id = win_node_ptr->win_id;
        }
        else
        {
            win_id = win_handle;
        }
    }

    return (win_id);
}

/*****************************************************************************/
// 	Description : get focus child win handle
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetFocusChildWinHandle(
                                       MMI_HANDLE_T parent_handle
                                       )
{
    MMI_WINDOW_NODE_T   *win_node_ptr = PNULL;

    parent_handle = MMK_ConvertIdToHandle( parent_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( parent_handle ) ) )
    {
        if (PNULL != win_node_ptr->focus_child_win_ptr)
        {
            return win_node_ptr->focus_child_win_ptr->base_node.handle;
        }
    }

    return 0;
}

/*****************************************************************************/
// 	Description : get focus window id
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetFocusParentWinHandle(
                                                void
                                                )
{
    MMI_HANDLE_T       parent_win_handle = 0;
    MMI_WINDOW_NODE_T* win_node_ptr      = GetWindowNodePtr( MMK_GetFocusWinHandle() );

    if( PNULL != win_node_ptr )
    {
        parent_win_handle = GetParentHandle( win_node_ptr->tree_node_ptr );
    }

    return parent_win_handle;
}

/*******************************************************************/
//  Interface:		MMK_GerFocusMainWinHandle
//  Description : 	MMK_GerFocusMainWinHandle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC MMI_HANDLE_T MMK_GerFocusMainWinHandle(
                                              void
                                              )
{
    MMI_HANDLE_T main_win_handle = MMK_GetFocusParentWinHandle();

    if (0 == main_win_handle)
    {
        main_win_handle = MMK_GetFocusWinHandle();
    }
    
    return main_win_handle;
}

/*****************************************************************************/
// 	Description : get focus window id
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetParentWinHandle(
                                           MMI_HANDLE_T win_handle
                                           )
{
    MMI_HANDLE_T       parent_win_handle = 0;
    MMI_WINDOW_NODE_T* win_node_ptr      = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );
    
    if( PNULL != (win_node_ptr = GetWindowNodePtr( win_handle )) )
    {
        parent_win_handle = GetParentHandle( win_node_ptr->tree_node_ptr );
    }

    return parent_win_handle;
}

/*****************************************************************************/
// 	Description : grab window foucus by applet 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:将applet所有窗口提到zorder system的topmost
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GrabWinFocusByApplet(
                                        MMI_HANDLE_T applet_handle
                                        )
{
    return MMK_HideAppletWindow( applet_handle, FALSE );
}

/*****************************************************************************/
// 	Description : get the add data pointer of the window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC ADD_DATA MMK_GetWinAddDataPtr(
                                     MMI_HANDLE_T win_handle
                                     )
{
    MMI_WINDOW_T* win_ptr    = PNULL;
    ADD_DATA      recode_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );

    if (PNULL != win_ptr)
    {
        recode_ptr = win_ptr->adddata_ptr;
    }

    return recode_ptr;
}

/*****************************************************************************/
// 	Description : free the window addtional data
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_FreeWinAddData(
                                  MMI_HANDLE_T win_handle
					              )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = PNULL;
    BOOLEAN              result       = FALSE;

    //SCI_TRACE_LOW:"MMK_FreeWinAddData: the win_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4496_112_2_18_3_23_31_60,(uint8*)"d", win_handle );

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_node_ptr = GetWindowNodePtr( win_handle );

    if (  PNULL != win_node_ptr
        && PNULL != win_node_ptr->win_ptr->adddata_ptr )
    {
        SCI_FREE( win_node_ptr->win_ptr->adddata_ptr );

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get the add data pointer of the window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:仅提供给pub win使用
/*****************************************************************************/
PUBLIC ADD_DATA MMK_GetWinAddDataEx(
                                    MMI_HANDLE_T win_handle
                                    )
{
    MMI_WINDOW_T* win_ptr    = PNULL;
    ADD_DATA      recode_ptr = PNULL;
    
    win_handle = MMK_ConvertIdToHandle( win_handle );
    
    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );
    
    if (PNULL != win_ptr)
    {
        recode_ptr = win_ptr->adddata_ex_ptr;
    }
    
    return recode_ptr;
}

/*****************************************************************************/
// 	Description : free the window addtional data
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:仅提供给pub win使用
/*****************************************************************************/
PUBLIC BOOLEAN MMK_FreeWinAddDataEx(
                                    MMI_HANDLE_T win_handle
                                    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = PNULL;
    BOOLEAN              result       = FALSE;
    
    //SCI_TRACE_LOW:"MMK_FreeWinAddDataEx: the win_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4549_112_2_18_3_23_31_61,(uint8*)"d", win_handle );
    
    win_handle = MMK_ConvertIdToHandle( win_handle );
    
    // get the window pointer
    win_node_ptr = GetWindowNodePtr( win_handle );
    
    if (  PNULL != win_node_ptr
        && PNULL != win_node_ptr->win_ptr->adddata_ex_ptr )
    {
        SCI_FREE( win_node_ptr->win_ptr->adddata_ex_ptr );
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : create window user data
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:设置窗口数据，会保存在窗口里
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinUserData(
                                  MMI_HANDLE_T win_handle,
                                  void*        data_ptr
                                  )
{
    MMI_WINDOW_T* win_ptr = PNULL;
    BOOLEAN       result  = FALSE;
    
    win_handle = MMK_ConvertIdToHandle( win_handle );
    
    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );
    
    if ( PNULL != win_ptr )
    {
        win_ptr->user_data_ptr = data_ptr;
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : get window user data
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void* MMK_GetWinUserData(
                                MMI_HANDLE_T win_handle
                                )
{
    MMI_WINDOW_T* win_ptr    = PNULL;
    void*         recode_ptr = PNULL;
    
    win_handle = MMK_ConvertIdToHandle( win_handle );
    
    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );
    
    if ( PNULL != win_ptr )
    {
        recode_ptr = win_ptr->user_data_ptr;
    }
    
    return recode_ptr;
}


/*****************************************************************************/
// 	Description : set window dirty
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinDirty(
                               MMI_HANDLE_T win_handle,
                               BOOLEAN is_dirty
                               )
{
    return MMK_SetWinDisplayStyleState( win_handle, WS_WIN_DIRTY, is_dirty );
}

/*****************************************************************************/
// 	Description : get window dirty
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinDirty(
                               MMI_HANDLE_T win_handle
                               )
{
    return MMK_GetWinDisplayStyleState( win_handle, WS_WIN_DIRTY );
}

/*****************************************************************************/
// 	Description : the window get the active, all controls of the window lose active
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_WinGetActive(
                                MMI_HANDLE_T win_handle
                                )
{
    BOOLEAN             result   = FALSE;
    MMI_WINDOW_NODE_T*  win_node_ptr  = PNULL;
    MMI_BASE_NODE_T*    ctrl_node_ptr = PNULL;

    //SCI_TRACE_LOW:"MMK_WinGetActive: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4579_112_2_18_3_23_31_62,(uint8*)"d", win_handle);

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_node_ptr = GetWindowNodePtr( win_handle );

    if ( PNULL != win_node_ptr )
    {
        ctrl_node_ptr = win_node_ptr->active_control_ptr;
        
        if (PNULL != ctrl_node_ptr)
        {
            MMK_RunCtrlProc( MMK_GetHandleByCtrlNode(ctrl_node_ptr), MSG_CTL_LOSE_ACTIVE, PNULL );
            win_node_ptr->active_control_ptr = PNULL;
        }

        MMK_RunWinProc( win_handle, MSG_FULL_PAINT, PNULL);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : all controls of the window are inactive controls
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_WinInactiveCtrl(
                                   MMI_HANDLE_T win_handle,
                                   BOOLEAN is_update
                                   )
{
    BOOLEAN        result   = TRUE;
    MMI_WINDOW_NODE_T*  win_node_ptr  = PNULL;
    MMI_BASE_NODE_T* ctrl_node_ptr = PNULL;

    //SCI_TRACE_LOW:"MMK_WinInactiveCtrl: the win_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4617_112_2_18_3_23_32_63,(uint8*)"d", win_handle );

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_node_ptr = GetWindowNodePtr( win_handle );

    if ( PNULL != win_node_ptr )
    {
        ctrl_node_ptr = win_node_ptr->active_control_ptr;
        
        if (PNULL != ctrl_node_ptr)
        {
            MMK_ClearKeyDownCtrl( MMK_GetHandleByCtrlNode(ctrl_node_ptr) );

            MMK_RunCtrlProc( MMK_GetHandleByCtrlNode(ctrl_node_ptr), MSG_CTL_LOSE_ACTIVE, PNULL );
            if (is_update)
            {
                MMK_RunCtrlProc( MMK_GetHandleByCtrlNode(ctrl_node_ptr), MSG_CTL_PAINT, PNULL );
            }
            win_node_ptr->active_control_ptr = PNULL;
        }
    }
    else
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : Get the prev window id
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMK_GetPrevWinId(
                                     MMI_WIN_ID_T win_id // the window id
                                     )
{
    uint32             id = 0;
    MMI_HANDLE_T       win_handle   = MMK_GetPrevWinHandle( MMK_ConvertIdToHandle( win_id ) );
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );
    
    if( PNULL != win_node_ptr )
    {
        //静态应用返回win id, 动态应用返回win hanlde
        if( !MMK_GetAppletState( win_node_ptr->parent_applet_handle, MMI_APPLET_STATE_DYNAMIC ) )
        {
            id = win_node_ptr->win_id;
        }
        else
        {
            id = win_handle;
        }
    }
    
    return id;
}

/*****************************************************************************/
// 	Description : Get the prev window handle
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetPrevWinHandle(
                                         MMI_HANDLE_T win_handle
                                         )
{    
    MMI_HANDLE_T         prev_handle   = 0;
    MMI_WINDOW_NODE_T*   win_node_ptr = GetWindowNodePtr( win_handle );
    
    if( PNULL != win_node_ptr )
    {
        //只有祖先窗口才有zorder, 解决获得子窗口的prev win exception的问题
        win_node_ptr = GetWindowNodePtr( GetAncestorHandle( win_node_ptr->tree_node_ptr ) );

        if( PNULL != win_node_ptr )
        {
            MMI_ZORDER_SYSTEM_T* zoder_system   = GetZorderSystem();
            MMI_ZORDER_NODE_T*   zoder_node_ptr = &win_node_ptr->zorder_node;
            
            SCI_ASSERT( PNULL != zoder_system && PNULL != zoder_node_ptr );/*assert verified*/
            
            if( zoder_node_ptr->prev_ptr != zoder_system->top_most_ptr )
            {
                if( PNULL != zoder_node_ptr->prev_ptr )
                {
                    SCI_ASSERT( PNULL != zoder_node_ptr->prev_ptr->win_node_ptr );/*assert verified*/

                    prev_handle = zoder_node_ptr->prev_ptr->win_node_ptr->base_node.handle;
                }
                else
                {
                    //SCI_TRACE_LOW:"MMI: MMK_GetPrevWinHandle failed!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4681_112_2_18_3_23_32_64,(uint8*)"");
                }
            }
        }
    }
    
    return prev_handle;
}

/*****************************************************************************/
// 	Description : whether the anim ctrl is need paint
//	Global resource dependence : 
//  Author:cheng.luo
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinNeedPaint(
                                  MMI_HANDLE_T win_handle     //[IN] the window id
                                  )
{
    BOOLEAN result = FALSE;
    
    win_handle = MMK_ConvertIdToHandle( win_handle );

    if (MMK_IsFocusWin(win_handle))
    {
        result = TRUE;
    }
    else
    {
        MMI_WINDOW_NODE_T* win_node_ptr       = PNULL;
        MMI_HANDLE_T       parent_win_handle  = 0;
        
        if ( PNULL != (win_node_ptr = GetWindowNodePtr(win_handle)))
        {
            if( 0 != (parent_win_handle = GetParentHandle(win_node_ptr->tree_node_ptr)))
            {
                if (MMK_IsFocusWin(parent_win_handle)
                    && !MMK_GetWinDisplayStyleState(parent_win_handle, WS_DISABLE_REDRAW_CHILDWIN))
                {
                    result = TRUE;
                }
            }
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : whether the win handle full paint
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinOnPaint(
                                MMI_HANDLE_T win_handle     //[IN] the window id
                                )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        result = win_node_ptr->win_ptr->is_on_paint;
    }
    
    return result;
}

/*****************************************************************************/
//    Description : set the pen capture 
//    Global resource dependence : 
//    Author:Robert Lu
//    Note:
/*****************************************************************************/
// PUBLIC void MMK_SetCapture(
//                            MMI_HANDLE_T cap_handle
//                            )
// {
//     s_capture_handle = cap_handle;
// }

/*****************************************************************************/
//    Description : get the pen capture 
//    Global resource dependence : 
//    Author:Robert Lu
//    Note:
/*****************************************************************************/
// PUBLIC MMI_HANDLE_T MMK_GetCapture(
//                                    void        
//                                    )
// {
//     return s_capture_handle;
// }

/*****************************************************************************/
//    Description : release the pen capture 
//    Global resource dependence : 
//    Author:Robert Lu
//    Note:
/*****************************************************************************/
// PUBLIC void MMK_ReleaseCapture( 
//                                void        
//                                )
// {
//     s_capture_handle = 0;
// }

/*****************************************************************************/
//     Description : set window priority
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinPriority( 
                                  MMI_HANDLE_T win_handle,
                                  MMI_WIN_PRIORITY_E win_priority
                                  )
{
    BOOLEAN            result       = FALSE;
	MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );

    if( PNULL != win_node_ptr )
    {
        win_node_ptr->win_ptr->win_priority = win_priority;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//     Description : set window priority
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC MMI_WIN_PRIORITY_E MMK_GetWinPriority( 
                                             MMI_HANDLE_T win_handle
                                             )
{
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
	MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );

    if( PNULL != win_node_ptr )
    {
        win_priority = win_node_ptr->win_ptr->win_priority;
    }

    return win_priority;
}

/*****************************************************************************/
//     Description : set window id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinId( 
                              MMI_HANDLE_T win_handle,
                              MMI_WIN_ID_T win_id
                              )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );
    
    if( PNULL != win_node_ptr )
    {
        win_node_ptr->win_id = win_id;
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : get window id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMK_GetWinId( 
                                 MMI_HANDLE_T win_handle
                                 )
{
    MMI_WIN_ID_T       win_id       = 0;
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );
    
    if( PNULL != win_node_ptr )
    {
        win_id = win_node_ptr->win_id;
    }
    
    return win_id;
}

/*****************************************************************************/
//     Description : set window id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinFunc( 
                              MMI_HANDLE_T      win_handle,
                              PROCESSMSG_FUNC   func
                              )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );
    
    if( PNULL != win_node_ptr )
    {
        win_node_ptr->win_ptr->ProcessMsg = func;
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set window hook function
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// this method is used to send message to user first.
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinHookFunc(
                                  MMI_HANDLE_T      win_handle,
                                  PROCESSMSG_FUNC   func
                                  )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );
    win_node_ptr = GetWindowNodePtr( win_handle );

    if( PNULL != win_node_ptr )
    {
        win_node_ptr->win_ptr->ProcessHookMsg = func;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//     Description : set window display style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinDisplayStyleState( 
                                           MMI_HANDLE_T         win_handle,
                                           WINDOW_DISPLAY_STYLE style,
                                           BOOLEAN              is_true
                                           )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        if( is_true )
        {
            win_node_ptr->win_ptr->window_style |= style;
        }
        else
        {
            win_node_ptr->win_ptr->window_style &= ~style;
        }
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : get window display style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinDisplayStyleState( 
                                           MMI_HANDLE_T         win_handle,
                                           WINDOW_DISPLAY_STYLE style
                                           )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        result = (BOOLEAN)( ( win_node_ptr->win_ptr->window_style & style ) == style );
    }
    
    return result;
}

/*****************************************************************************/
//     Description : get window display style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinDisplayStyle( 
                                      MMI_HANDLE_T          win_handle,
                                      WINDOW_DISPLAY_STYLE* style_ptr
                                      )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    if ( PNULL == style_ptr )
    {
        return FALSE;
    }

    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        *style_ptr = win_node_ptr->win_ptr->window_style;
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : set window move style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinMoveStyle( 
                                   MMI_HANDLE_T      win_handle,
                                   WINDOW_MOVE_STYLE style
                                   )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        win_node_ptr->win_ptr->window_move_style = style;
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : Get window move style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC WINDOW_MOVE_STYLE MMK_GetWinMoveStyle(MMI_HANDLE_T win_handle)
{
    
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        return win_node_ptr->win_ptr->window_move_style;
    }
    else
    {
        return MOVE_BY_SETTING;
    }
}

/*****************************************************************************/
//     Description : set window bg image id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinBackgroundImageId( 
                                           MMI_HANDLE_T   win_handle,
                                           MMI_IMAGE_ID_T image_id
                                           )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        win_node_ptr->win_ptr->background_image_id = image_id;
        
        if ( image_id != MMITHEME_GetBgImgId() )
        {
            win_node_ptr->win_ptr->window_style |= WS_DISABLE_COMMON_BG;
        }

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : get window bg image id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMK_GetWinBackgroundImageId( 
                                                  MMI_HANDLE_T   win_handle
                                                  )
{
    MMI_IMAGE_ID_T image_id = MMITHEME_GetInvalidImageID();
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        image_id = win_node_ptr->win_ptr->background_image_id;
    }
    
    return image_id;
}

/*****************************************************************************/
//     Description : get window title id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinRect( 
                              MMI_HANDLE_T   win_handle,
                              const GUI_RECT_T*    rect_ptr
                              )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;

    if ( PNULL == rect_ptr )
    {
        return FALSE;
    }
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        win_node_ptr->win_ptr->window_rect = *rect_ptr;

        MMK_PostMsg(win_handle, MSG_USER_CHANGE_WINRECT, NULL, NULL);
        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
//     Description : get window title id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinRect( 
                              MMI_HANDLE_T   win_handle,
                              GUI_RECT_T*    rect_ptr
                              )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;

    if ( PNULL == rect_ptr )
    {
        return FALSE;
    }
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        *rect_ptr = win_node_ptr->win_ptr->window_rect;

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : set win both rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinBothRect( 
                                  MMI_HANDLE_T           handle,
                                  const GUI_BOTH_RECT_T* both_rect_ptr
                                  )
{
    BOOLEAN             result = FALSE;
    MMI_WINDOW_NODE_T   *win_node_ptr = PNULL;

    if ( PNULL == both_rect_ptr )
    {
        return FALSE;
    }
    
    handle = MMK_ConvertIdToHandle( handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( handle ) ) )
    {
        win_node_ptr->win_ptr->both_rect = *both_rect_ptr;

        win_node_ptr->win_ptr->window_rect = IsWindowLandscape( handle ) ? both_rect_ptr->h_rect : both_rect_ptr->v_rect;

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : get win both rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinBothRect( 
                                  MMI_HANDLE_T     handle,
                                  GUI_BOTH_RECT_T* both_rect_ptr
                                  )
{
    BOOLEAN             result = FALSE;
    MMI_WINDOW_NODE_T   *win_node_ptr = PNULL;

    if ( PNULL == both_rect_ptr )
    {
        return FALSE;
    }
    
    handle = MMK_ConvertIdToHandle( handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( handle ) ) )
    {
        *both_rect_ptr = win_node_ptr->win_ptr->both_rect;

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : get the window pointer that is processing the message 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetProcMsgHandle(
                                         void
                                         )
{
    return s_proc_msg_handle;
}

/*****************************************************************************/
// 	Description : set the window pointer that is processing the message 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetProcMsgHandle(
                                    MMI_HANDLE_T handle
                                    )
{
    s_proc_msg_handle = handle;

    return TRUE;
}

/*****************************************************************************/
//     Description : get window lcd dev info
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinLcdDevInfo( 
                                    MMI_HANDLE_T win_handle,
                                    GUI_LCD_DEV_INFO *dev_info_ptr  //out
                                    )
{
    MMI_WINDOW_NODE_T  *win_node_ptr = PNULL;
    BOOLEAN result = FALSE;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        if (PNULL != win_node_ptr->win_ptr)
        {
            if (PNULL != dev_info_ptr)
            {
                *dev_info_ptr = win_node_ptr->win_ptr->lcd_dev_info;
                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//     Description : set window lcd dev info
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinLcdDevInfo( 
                                    MMI_HANDLE_T win_handle,
                                    const GUI_LCD_DEV_INFO *dev_info_ptr  //in
                                    )
{
    MMI_WINDOW_NODE_T  *win_node_ptr = PNULL;
    BOOLEAN result = FALSE;
    
    if (PNULL == dev_info_ptr)
    {
        return result;
    }

    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        if (PNULL != win_node_ptr->win_ptr)
        {
            win_node_ptr->win_ptr->lcd_dev_info = *dev_info_ptr;
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : MMK_GetScreenAngle
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 获得平台全局角度
/*****************************************************************************/
PUBLIC LCD_ANGLE_E MMK_GetScreenAngle( void )
{
    return s_screen_angle;
}

/*****************************************************************************/
//  Description : MMK_SetScreenAngle
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 设置平台全局角度,只有gsensor\推盖\工程模式中使用,普通app禁止调用
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetScreenAngle( 
                                  LCD_ANGLE_E angle
                                  )
{
    BOOLEAN result = FALSE;

#ifdef UI_P3D_SUPPORT
#ifdef EFFECT_LCD_ROTATE_SUPPORT
    LCD_ANGLE_E old_angle = s_screen_angle;
#endif
#endif    

    //先设置全局角度
    s_screen_angle = angle;

    if ( GUILCD_GetLogicAngle( GUI_MAIN_LCD_ID ) != angle
        && GetCurAngle( s_screen_angle, GetCurSupportAngle() ) == angle )
    {
#ifdef UI_P3D_SUPPORT    
#ifdef EFFECT_LCD_ROTATE_SUPPORT
        MMIDEFAULT_HandlePreLCDRotate();
#endif
#endif        
        GUILCD_SetLogicAngle( GUI_MAIN_LCD_ID, angle );

        MMK_RunWinProc(MMK_GetFocusWinHandle(),MSG_PRE_LCD_SWITCH,PNULL);

        MMK_UpdateScreen();

#ifdef UI_P3D_SUPPORT            
#ifdef EFFECT_LCD_ROTATE_SUPPORT
        MMITHEME_PlayWinRotate(s_screen_angle, old_angle);
#endif
#endif        
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get window angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinAngle(
                               MMI_HANDLE_T     win_handle, //in:
                               LCD_ANGLE_E      *angle_ptr  //in/out:
                               )
{
    BOOLEAN             result = FALSE;
    MMI_WINDOW_NODE_T   *win_node_ptr = PNULL;

    //convert win handle
    win_handle = MMK_ConvertIdToHandle(win_handle);

    //get win node
    win_node_ptr = GetWindowNodePtr(win_handle);
    
    if ((PNULL != angle_ptr) &&
        (PNULL != win_node_ptr))
    {
        *angle_ptr = win_node_ptr->win_ptr->cur_angle;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : MMK_SetScreenAngle
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 设置当前窗口角度,不会改变平台全局角度
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinAngle( 
                               MMI_HANDLE_T win_handle,
                               LCD_ANGLE_E angle,
                               BOOLEAN is_update
                               )
{
    BOOLEAN result = FALSE;

    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if ( GetTopmostWindow() == win_handle
		|| GetFirstPaintWindow() == win_handle )
    {
        if ( GUILCD_GetLogicAngle( GUI_MAIN_LCD_ID ) != angle
            && GetCurAngle( angle, GetCurSupportAngle() ) == angle )
        {
            GUILCD_SetLogicAngle( GUI_MAIN_LCD_ID, angle );
            
            if ( is_update )
            {
                MMK_UpdateScreen();
            }

            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//     Description : set window support angle
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinSupportAngle( 
                                      MMI_HANDLE_T         win_handle,
                                      WINDOW_SUPPORT_ANGLE support_angle
                                      )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );
    
    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        if ( WIN_SUPPORT_ANGLE_CUR == support_angle
            || WIN_SUPPORT_ANGLE_INIT == support_angle )
        {
            LCD_ANGLE_E angle = ( WIN_SUPPORT_ANGLE_INIT == support_angle )
                ? GUILCD_GetInitAngle( MAIN_LCD_ID ) : GUILCD_GetLogicAngle( MAIN_LCD_ID );
            uint8 bit = 0;

            switch( angle )
            {
            case LCD_ANGLE_90:
                bit = 1;
            	break;
            case LCD_ANGLE_180:
                bit = 2;
                break;
            case LCD_ANGLE_270:
                bit = 3;
                break;
            default:
                break;
            }

            win_node_ptr->win_ptr->support_angle = 1 << bit;
        }
        else
        {
            win_node_ptr->win_ptr->support_angle = support_angle;
        }
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//     Description : Get window support angel
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC WINDOW_SUPPORT_ANGLE MMK_GetWinSupportAngle(
                                                   MMI_HANDLE_T win_handle
                                                   )
{
    
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        return win_node_ptr->win_ptr->support_angle;
    }
    else
    {
        return WIN_SUPPORT_ANGLE_ALL;
    }
}

/*****************************************************************************/
// 	Description : is win need copy lcd when lose focus
//	Global resource dependence : 
//  Author:Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetWinNeedCopyLcdInLoseFocus(
                                                                                                MMI_HANDLE_T		win_handle,
                                                                                                BOOLEAN is_need_copy
                                                                                                )
{

    MMI_WINDOW_NODE_T   *win_node_ptr = PNULL;
    
    win_handle = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        if(win_node_ptr->win_ptr)
        {
            win_node_ptr->win_ptr->is_not_need_copylcd = !is_need_copy;
        }
    }
}
/*****************************************************************************/
// 	Description : is win need copy lcd when lose focus
//	Global resource dependence : 
//  Author:Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinNeedCopyLcdInLoseFocus(MMI_HANDLE_T	win_handle)
{

    MMI_WINDOW_NODE_T   *win_node_ptr = PNULL;
    
    win_handle = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        if(win_node_ptr->win_ptr)
        {
            return (!win_node_ptr->win_ptr->is_not_need_copylcd);
        }
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : is window landscape
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:用于取代MMITHEME_IsMainScreenLandscape接口, 全局变量可能和win中angle不一致
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWindowLandscape( 
                                     MMI_HANDLE_T win_handle
                                     )
{
    win_handle = MMK_ConvertIdToHandle( win_handle );
    
    return IsWindowLandscape( win_handle );
}

/*****************************************************************************/
// 	Description : 是否响应硬图标
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:临时方案, 避免模块在硬图标退出时未释放完整, 增加禁止响应硬图标的功能
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsResponseHWIcon( void )
{
    BOOLEAN              result            = TRUE;
    MMI_ZORDER_NODE_T*   zorder_node_ptr   = PNULL;
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    zorder_node_ptr = zorder_system_ptr->top_most_ptr;

    //遍历zorder中的所有窗口, 查找是否有禁止响应硬图标的窗口
    if( PNULL != zorder_node_ptr )
    {
        do 
        {
            SCI_ASSERT( PNULL != zorder_node_ptr->win_node_ptr );/*assert verified*/

            if( MMK_GetWinDisplayStyleState( zorder_node_ptr->win_node_ptr->base_node.handle, WS_DISABLE_HWICON ))
            {
                result = FALSE;

                break;
            }
            else
            {
                // 对于半屏窗口，例如pubwin。当它在焦点窗口的时候，不响应硬图标。不过，这里最好定义pubwin的属性，而不用半屏这个类型。
                if (IsWinOnFocusByStyle( zorder_node_ptr->win_node_ptr->base_node.handle, MOVE_NOT_FULL_SCREEN_WINDOW ))
                {
                    result = FALSE;
                    break;
                }
            }
            
            zorder_node_ptr = zorder_node_ptr->prev_ptr;
            
        } while( zorder_node_ptr != zorder_system_ptr->top_most_ptr );
    }

    return result;
}

/*****************************************************************************/
// 	Description : compare
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_WindowTreeNodeCompare( 
                                         uint32 handle,
                                         uint32 condition1, 
                                         uint32 condition2
                                         )
{
    BOOLEAN result = FALSE;

    //SCI_ASSERT( condition1 < MMI_TREE_NODE_FIND_TYPE_MAX );
    
    switch( condition1 )
    {
    case MMI_TREE_NODE_FIND_HANDLE:
        {
            if( condition2 == handle )
            {
                result = TRUE;
            }
        }
    	break;
        
    case MMI_TREE_NODE_FIND_ID:
        {          
            if( condition2 == GetWindowNodePtr( handle )->win_id )
            {
                result = TRUE;
            }
        }
    	break;

    default:
        //SCI_ASSERT( FALSE );
        break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get the window handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetAppletHandleByWin(
                                             MMI_HANDLE_T win_handle
                                             )
{
    MMI_HANDLE_T       applet_handle = 0;
    MMI_WINDOW_NODE_T* win_node_ptr  = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr  = GetWindowNodePtr( win_handle ) ) )
    {
        applet_handle = win_node_ptr->parent_applet_handle;
    }

    return applet_handle;
}

/*****************************************************************************/
// 	Description : get the window handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_GetWinTreeNode(
                                           MMI_HANDLE_T win_handle
                                           )
{
    MMI_TREE_NODE_T*   tree_node_ptr = PNULL;
    MMI_WINDOW_NODE_T* win_node_ptr  = GetWindowNodePtr( win_handle );

    if( PNULL != win_node_ptr )
    {
        tree_node_ptr = win_node_ptr->tree_node_ptr;
    }

    return tree_node_ptr;
}

/*****************************************************************************/
// 	Description : run the control process 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
MMI_RESULT_E MMK_RunWinProc(
                            MMI_HANDLE_T     win_handle,
                            MMI_MESSAGE_ID_E msg_id,
                            DPARAM           param
                            )
{
    MMI_RESULT_E        result       = MMI_RESULT_FALSE;
    MMI_WINDOW_NODE_T*  win_node_ptr = GetWindowNodePtr( win_handle );
    uint16              win_name_len = 0;
    if ( PNULL != win_node_ptr )
    {
        uint8 *win_id_name = PNULL;
        uint8 winIdName[MMI_WIN_ID_NAME_MAX_LENGTH+2] = {0};
        uint8 strNull[] = "NULL";
#ifdef JAMES_TEST
        uint32 tick = SCI_GetTickCount();

        //SCI_TRACE_LOW:"james test, MMK_RunWinProc start %d, win_id = 0x%08x, msg_id = 0x%08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4248_112_2_18_3_23_31_53,(uint8*)"ddd", tick, win_node_ptr->win_id, msg_id );
#else
#ifdef MMI_WINIDNAME_SUPPORT
        MMITHEME_GetWinIdNameArr(win_node_ptr->win_id, &win_id_name);
        win_name_len = MIN(strlen(win_id_name),(MMI_WIN_ID_NAME_MAX_LENGTH+2));
        SCI_MEMCPY(winIdName,win_id_name,(sizeof(uint8)*win_name_len));
        SCI_TRACE_LOW("MMK_RunWinProc, win_handle = 0x%08x, win_id = 0x%08x, msg_id = 0x%08x", win_handle, win_node_ptr->win_id, msg_id );
        SCI_TRACE_LOW("MMK_RunWinProc, win_name = %s", win_id_name?winIdName:strNull);
#endif
        //SCI_TRACE_LOW:"MMK_RunWinProc, win_handle = 0x%08x, win_id = 0x%08x, win_name = %s, msg_id = 0x%08x"
        // temp_for_IDLE        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4253_112_2_18_3_23_31_54,(uint8*)"ddsd", win_handle, win_node_ptr->win_id, win_id_name?win_id_name:"NULL", msg_id );
#endif

        //静态应用传入win id, 动态应用传入win handle
        if( !MMK_GetAppletState( win_node_ptr->parent_applet_handle, MMI_APPLET_STATE_DYNAMIC )
            && 0 != win_node_ptr->win_id )
        {
            result = win_node_ptr->win_ptr->ProcessMsg( win_node_ptr->win_id, msg_id, param ); 
        }
        else
        {
            result = win_node_ptr->win_ptr->ProcessMsg( win_node_ptr->win_ptr->win_handle, msg_id, param ); 
        } 

#ifdef JAMES_TEST
        tick = SCI_GetTickCount() - tick;
        
        //SCI_TRACE_LOW:"james ticks, MMK_RunWinProc end ticks = %d, win_id = 0x%08x, msg_id = 0x%08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4270_112_2_18_3_23_31_55,(uint8*)"ddd", tick, win_node_ptr->win_id, msg_id );
#endif
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_RunWinProc failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4275_112_2_18_3_23_31_56,(uint8*)"" );
    }
    
    MMI_CheckAllocatedMemInfo();
    
    return result;
}

/*****************************************************************************/
// 	Description : prcoess window hook function
//	Global resource dependence : 
//  Author: hua.fang
//	Note:处理勾子函数
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMK_RunWinHookProc(
                                       MMI_HANDLE_T      win_handle, // the window pointer
                                       MMI_MESSAGE_ID_E  msg_id,     // the message id
                                       DPARAM            param       // the param
                                       )
{
    MMI_RESULT_E       result       = MMI_RESULT_FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );

    if (PNULL != win_node_ptr && PNULL != win_node_ptr->win_ptr->ProcessHookMsg)
    {
#ifdef JAMES_TEST
        uint32 tick = SCI_GetTickCount();

        //SCI_TRACE_LOW:"james test, MMK_RunWinHookProc start %d, win_id = 0x%08x, msg_id = 0x%08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4299_112_2_18_3_23_31_57,(uint8*)"ddd", tick, win_node_ptr->win_id, msg_id );
#else
        //SCI_TRACE_LOW:"MMK_RunWinHookProc, win_handle = 0x%08x, win_id = 0x%08x, msg_id = 0x%08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4301_112_2_18_3_23_31_58,(uint8*)"ddd", win_handle, win_node_ptr->win_id, msg_id );
#endif

        //静态应用传入win id, 动态应用传入win handle
        if( !MMK_GetAppletState( win_node_ptr->parent_applet_handle, MMI_APPLET_STATE_DYNAMIC )
            && 0 != win_node_ptr->win_id )
        {
            result = win_node_ptr->win_ptr->ProcessHookMsg( win_node_ptr->win_id, msg_id, param ); 
        }
        else
        {
            result = win_node_ptr->win_ptr->ProcessHookMsg( win_node_ptr->win_ptr->win_handle, msg_id, param ); 
        } 

#ifdef JAMES_TEST
        tick = SCI_GetTickCount() - tick;
        
        //SCI_TRACE_LOW:"james ticks, MMK_RunWinHookProc end ticks = %d, win_id = 0x%08x, msg_id = 0x%08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_4318_112_2_18_3_23_31_59,(uint8*)"ddd", tick, win_node_ptr->win_id, msg_id );
#endif
    }

    return result;
}

/*****************************************************************************/
// 	Description : check the control id 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
// BOOLEAN MMK_CheckCtrlId(
//                         MMI_HANDLE_T  win_handle,
// 					    MMI_CTRL_ID_T ctrl_id // the control id
// 					   )
// {
//     BOOLEAN        result   = FALSE;
//     MMI_WINDOW_T*  win_ptr  = PNULL;
//     MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;
// 
//     win_handle = MMK_ConvertIdToHandle( win_handle );
// 
//     if ( PNULL != ( win_ptr = GetWindowPtr( win_handle ) ) )
//     {
//         // get the control pointer
//         ctrl_node_ptr = GetCtrlNodeByWinHandle( win_handle, ctrl_id, MMI_TREE_NODE_FIND_ID );
//         
//         if (PNULL != ctrl_node_ptr)
//         {
//             // set the result to TRUE
//             result = TRUE;
//         }
//     }
// 
//     return result;
// }

/*****************************************************************************/
//     Description : the controls notify the parent  window 
//    Global resource dependence : 
//  Author:Robert.Lu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DefNotifyProc(                               
                                 MMI_HANDLE_T       win_handle,
                                 MMI_HANDLE_T       ctrl_handle,       //Control Id
                                 MMI_MESSAGE_ID_E   notify_code,  //Notification Code
                                 ADD_DATA           data          //Added data
                                 )
{
    BOOLEAN           result         = FALSE;
    MMI_WINDOW_NODE_T *win_node_ptr  = PNULL;
    MMI_BASE_NODE_T   *ctrl_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );
    ctrl_handle = MMK_ConvertIdToHandle( ctrl_handle );

    if ( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) 
        && PNULL != ( ctrl_node_ptr = MMK_GetCtrlNodeByHandle( ctrl_handle ) ) )
    {       
        switch( notify_code )
        {
        case MSG_NOTIFY_SET_SOFTKEY:
            break;
            
        case MSG_NOTIFY_RESIZE:
            {
                GUI_RECT_T    *new_region = PNULL; 
                
                SCI_ASSERT(PNULL != data);/*assert verified*/
                
                new_region = (GUI_RECT_T *) data;
                result  =  MMK_ProcOpenWinMsg( win_handle, MSG_FULL_PAINT, PNULL);
                result |=  MMK_RunWinProc( win_handle, MSG_FULL_PAINT, PNULL);
                result |= MMK_ProcSpecialWinMsg( win_handle, MSG_FULL_PAINT, PNULL);
            }
            break;
            
        case MSG_NOTIFY_UPDATE:
            result  =  MMK_ProcOpenWinMsg( win_handle, MSG_FULL_PAINT, PNULL);
            result |=  MMK_RunWinProc( win_handle, MSG_FULL_PAINT, PNULL );
            result |= MMK_ProcSpecialWinMsg( win_handle, MSG_FULL_PAINT, PNULL);
            break;
            
        default:
            //SCI_PASSERT(FALSE,("MMK_DefNotifyProc:notify_code %d is error!",notify_code));
            break;
        }
    }
    else
    {
        //SCI_PASSERT(FALSE,("MMK_DefNotifyProc:parent_win_ptr is %d!",win_node_ptr));
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : default process the window message
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:按键消息的处理函数
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMK_DefaultProcessWinMsg(
                                             MMI_HANDLE_T       win_handle, // the window pointer
                                             MMI_MESSAGE_ID_E	msg_id,   // the message id
                                             DPARAM				param     // the param
                                             )
{
	MMI_RESULT_E        result = MMI_RESULT_FALSE;
    MMI_HANDLE_T        ctrl_handle = 0;
	MMI_BASE_NODE_T     *ctrl_node_ptr = PNULL;
    MMI_WINDOW_NODE_T   *win_node_ptr  = GetWindowNodePtr( win_handle );
    MMI_CheckAllocatedMemInfo();
    if( PNULL != win_node_ptr )
    {
        // 处理应用的勾子函数
        result = MMK_RunWinHookProc(win_handle, msg_id, param);

        if (MMI_RESULT_FALSE != result)     // 被应用的勾子函数处理了，那系统不用再处理
        {
            // 清除按键控件，如果下次应用的勾子函数不处理，就有焦点控件处理
            MMK_SetKeyDownCtrl(0, msg_id);
        }
        else
        {
            //如果是down按键消息，则丢给Active的控件的处理
            if ( MSG_KEYDOWN <= msg_id && MSG_KEYDOWN_MAX_NUM >= msg_id )
            {
                ctrl_node_ptr = win_node_ptr->active_control_ptr;
                
                if ( PNULL != ctrl_node_ptr )
                {
                    ctrl_handle = MMK_GetHandleByCtrlNode(ctrl_node_ptr);
                    result = MMK_RunCtrlProc(ctrl_handle,msg_id,param);
                    if (!result)
                    {
                        //get parent handle
                        ctrl_handle = MMK_GetParentCtrlHandle(ctrl_handle);
                        while (0 != ctrl_handle)
                        {
                            result = MMK_RunCtrlProc(ctrl_handle,msg_id,param);
                            if (result)
                            {
                                break;
                            }
                            ctrl_handle = MMK_GetParentCtrlHandle(ctrl_handle);
                        }
                    }

                    //记录key down是发给哪个控件的
                    MMK_SetKeyDownCtrl(ctrl_handle,msg_id);
                }
                else
                {
                    MMK_SetKeyDownCtrl( 0, msg_id );
                }
            }
            //其他按键
            else if ( ( MSG_KEYUP <= msg_id && MSG_KEYUP_MAX_NUM >= msg_id )
                || ( MSG_KEYLONG <= msg_id && MSG_KEYLONG_MAX_NUM >= msg_id )
                || ( MSG_KEYREPEAT <= msg_id && MSG_KEYREPEAT_MAX_NUM >= msg_id )
                || ( MSG_KEYPRESSUP <= msg_id && MSG_KEYPRESSUP_MAX_NUM >= msg_id ) )
            {
                ctrl_node_ptr = MMK_GetCtrlNodeByHandle( MMK_GetKeyDownCtrl( msg_id ) );
                
                if( PNULL != ctrl_node_ptr )
                {
                    MMI_HANDLE_T keydown_win_handle = MMK_GetWinHandleByCtrl( MMK_GetHandleByCtrlNode(ctrl_node_ptr) );

                    //按住kbd切换窗口, 再松开kbd时, 丢弃该消息, 避免花屏.
                    if ( !MMK_IsFocusWin( keydown_win_handle ) )                    
                    {                   
                        if( MSG_KEYREPEAT <= msg_id && MSG_KEYREPEAT_MAX_NUM >= msg_id )
                        {
                            MMK_StopKeyRepeatPressTimer();
                        }

                        MMK_SetKeyDownCtrl( 0, msg_id );
                        
                        result = MMI_RESULT_TRUE;

                        //SCI_TRACE_LOW:"MMK_DefaultProcessWinMsg, down ctrl is not focus!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3154_112_2_18_3_23_29_44,(uint8*)"");
                    }
                    else
                    {
                        //解决父子窗口重复发送两次消息给key donw控件的问题
                        if ( win_handle == keydown_win_handle )
                        {
                            result = MMK_RunCtrlProc( MMK_GetHandleByCtrlNode(ctrl_node_ptr), msg_id, param );
                        }
                        else
                        {
                            ctrl_node_ptr = win_node_ptr->active_control_ptr;
                            
                            if ( PNULL != ctrl_node_ptr )
                            {
                                result = MMK_RunCtrlProc( MMK_GetHandleByCtrlNode(ctrl_node_ptr), msg_id, param );
                            }  
                        }
                    }
                }
                else
                {
                    //存在发假按键消息的问题, 需要处理下
                    ctrl_node_ptr = win_node_ptr->active_control_ptr;
                    
                    if ( PNULL != ctrl_node_ptr )
                    {
                        result = MMK_RunCtrlProc( MMK_GetHandleByCtrlNode(ctrl_node_ptr), msg_id, param );
                    }
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_DefaultProcessWinMsg failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3189_112_2_18_3_23_29_45,(uint8*)"" );
    }
    MMI_CheckAllocatedMemInfo(); 
	return result;
}

/*****************************************************************************/
// 	Description : must process the window message
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
BOOLEAN MMK_ProcSpecialWinMsg(
                              MMI_HANDLE_T		win_handle, 
                              MMI_MESSAGE_ID_E	msg_id,   
                              DPARAM			param    
                              )
{
    BOOLEAN             result       = TRUE;
    MMI_WINDOW_NODE_T   *win_node_ptr = GetWindowNodePtr( win_handle );

    if( PNULL != win_node_ptr )
    {
        GUI_LCD_DEV_INFO	 lcd_dev_info = {0};
        
        MMITHEME_GetDefaultLcdBlock(&lcd_dev_info.lcd_id, &lcd_dev_info.block_id);
        
        switch(msg_id)
        {
        case MSG_CLOSE_WINDOW:
            MMK_DestroyAllControlByNode(win_node_ptr);
            break;
            
        case MSG_BACKLIGHT_TURN_ON:
        case MSG_GET_FOCUS:	  // notify all controls of the window to come back	
            if ( MMK_IsFocusWin( win_handle )
                ||( MMK_IsFocusWin(MMK_GetParentWinHandle(win_handle)) 
                    &&  MMK_GetWinDisplayStyleState(win_handle, WS_WIDGET_WIN )) 
                    )
            {
                MMK_DispatchToAllControl(
                    win_node_ptr, 
                    MSG_CTL_GET_FOCUS, param, 
                    MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD );
                
                //同步调用增加保护
                if ( PNULL != GetWindowNodePtr( win_handle ) )
                {
                    if( PNULL != win_node_ptr->focus_child_win_ptr )
                    {
                        if(!MMK_GetWinDisplayStyleState( win_node_ptr->focus_child_win_ptr->base_node.handle, WS_WIDGET_WIN ) )
                        {
                            MMK_SendMsg( win_node_ptr->focus_child_win_ptr->base_node.handle, MSG_GET_FOCUS, param );
                        }
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"MMK_ProcSpecialWinMsg failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3295_112_2_18_3_23_29_47,(uint8*)"" );
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMK_ProcSpecialWinMsg MSG_GET_FOCUS failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3244_112_2_18_3_23_29_46,(uint8*)"" );
            }
            break;
            
        case MSG_BACKLIGHT_TURN_OFF:
        case MSG_LOSE_FOCUS:  // notify all controls of the window to save secne
            MMK_DispatchToAllControl(
                win_node_ptr, 
                MSG_CTL_LOSE_FOCUS, param,
                MMI_TREE_TRAVEL_CHILD );
            
            MMI_FreeRes( win_handle );
            
            //同步调用增加保护
            if ( PNULL != GetWindowNodePtr( win_handle ) )
            {
                if( PNULL != win_node_ptr->focus_child_win_ptr )
                {
                    if(!MMK_GetWinDisplayStyleState(win_node_ptr->focus_child_win_ptr->base_node.handle, WS_WIDGET_WIN ))
                    {
                        MMK_SendMsg( win_node_ptr->focus_child_win_ptr->base_node.handle, MSG_LOSE_FOCUS, param );
                    }
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMK_ProcSpecialWinMsg failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3295_112_2_18_3_23_29_47,(uint8*)"" );
            }
            break;
            
        case MSG_FULL_PAINT:  // display all control of the window
#ifdef UI_MULTILAYER_SUPPORT
            if (UILAYER_IsMultiLayerEnable()
                && !IsWindowDisableCommonBg(win_node_ptr->win_ptr))
            {
                MMITHEME_RedrawBgLayer();
            }
            else
#endif
            {
                // add the background picture for the window
                MMITHEME_DisplayWinBackground(win_node_ptr->win_ptr->background_image_id,win_node_ptr->win_ptr->win_handle);
            }

            win_node_ptr->zorder_node.is_painted = TRUE;

            MMK_DispatchToAllControl(
                win_node_ptr,
                MSG_CTL_PAINT, param,
                MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD );
            
            //同步调用增加保护
            if ( PNULL != GetWindowNodePtr( win_handle ) )
            {
                if( PNULL != win_node_ptr->focus_child_win_ptr )
                {
                    if ((win_node_ptr->win_ptr->window_style & WS_DISABLE_REDRAW_CHILDWIN ) != WS_DISABLE_REDRAW_CHILDWIN )
                    {
                        MMK_SendMsg( win_node_ptr->focus_child_win_ptr->base_node.handle, MSG_FULL_PAINT, param );
                    }
                }           
                win_node_ptr->win_ptr->is_on_paint = FALSE;
                MMK_RunWinProc( win_handle,MSG_END_FULL_PAINT,PNULL);
            }
            else
            {
                //SCI_TRACE_LOW:"MMK_ProcSpecialWinMsg failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3295_112_2_18_3_23_29_47,(uint8*)"" );
            }
            break;
            
        default:
            result = FALSE;
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_ProcSpecialWinMsg failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3295_112_2_18_3_23_29_47,(uint8*)"" );

        result = FALSE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : handle the MSG_OPEN_WINDOW message
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ProcOpenWinMsg(
                                  MMI_HANDLE_T		win_handle,
                                  MMI_MESSAGE_ID_E	msg_id,   // the message id
                                  DPARAM			param     // the param
                                  )
{
    BOOLEAN            result       = TRUE;
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );
    
    if( PNULL != win_node_ptr )
    {
        GUI_LCD_DEV_INFO	 lcd_dev_info   = {0};
        
        MMITHEME_GetDefaultLcdBlock(&lcd_dev_info.lcd_id, &lcd_dev_info.block_id);
        
        switch(msg_id)
        {
        case MSG_OPEN_WINDOW:
            win_node_ptr->win_ptr->is_handle_open = TRUE;//防止窗口处理open消息之前收到其他消息
            //MMK_DispatchInBrotherNode( win_node_ptr->ctrl_tree_root_ptr, MSG_CTL_OPEN, param );
            MMK_DispatchToAllControl(
                win_node_ptr, 
                MSG_CTL_OPEN, param,
                MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD );
            
            //同步调用增加保护
            if ( PNULL != GetWindowNodePtr( win_handle ) )
            {
                //send active msg
                if ( PNULL != win_node_ptr->active_control_ptr )
                {
                    MMK_RunCtrlProc( MMK_GetHandleByCtrlNode(win_node_ptr->active_control_ptr), MSG_CTL_GET_ACTIVE, param );
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMK_ProcOpenWinMsg failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3631_112_2_18_3_23_30_52,(uint8*)"" );
            }

            if (MMK_IsFocusWin(win_handle))
            {
#ifdef UI_MULTILAYER_SUPPORT
                UILAYER_ActiveAllLayerOfHandle(win_handle);
#endif
                RestoreDefaultLayerBlt(win_handle);                
            }
            break;
            
        case MSG_CLOSE_WINDOW:
            // UILAYER_ReleaseAllLayerOfHandle(win_handle);
            break;
          
        case MSG_GET_FOCUS:
        	MMI_TraceAutoTestOfWinMessage(win_node_ptr->win_id, MSG_GET_FOCUS);
#ifdef UI_MULTILAYER_SUPPORT
            UILAYER_ActiveAllLayerOfHandle(win_handle);
#endif
            RestoreDefaultLayerBlt(win_handle);
            break;
            
        case MSG_LOSE_FOCUS:
            // 需要清理一下是否有该窗口的MSG_FULL_PAINT消息
            MMK_DeleteMSGByHwndAndMsg( win_node_ptr->win_ptr->win_handle, MSG_FULL_PAINT );
            break;

        case MSG_FULL_PAINT:
            {
                LCD_ANGLE_E cur_angle = GUILCD_GetLogicAngle( GUI_MAIN_LCD_ID );
  
                MMK_RunWinProc( win_handle, MSG_PRE_FULL_PAINT, PNULL );

                //MMK_RunWinProc中可能会closewin，需要重新get win_node_ptr
                win_node_ptr = GetWindowNodePtr( win_handle );
                if ((PNULL != win_node_ptr) && (PNULL != win_node_ptr->win_ptr))
                {
                    win_node_ptr->win_ptr->is_on_paint = TRUE;
                    
                    cur_angle = GetCurAngle(cur_angle,win_node_ptr->win_ptr->support_angle);
                    
                    if ( win_node_ptr->win_ptr->cur_angle != cur_angle )
                    {
                        win_node_ptr->win_ptr->cur_angle = cur_angle;

                        // 先让勾子函数处理，不需要判断返回值，后续的窗口必须要处理此消息
                        MMK_RunWinHookProc(win_handle, MSG_LCD_SWITCH, (DPARAM)cur_angle);

                        //同步调用增加保护
                        if ( PNULL != GetWindowNodePtr( win_handle ) )
                        {
                            win_node_ptr->win_ptr->window_rect = ( LCD_ANGLE_90 == cur_angle || LCD_ANGLE_270 == cur_angle )//is_horizon
                                ? win_node_ptr->win_ptr->both_rect.h_rect : win_node_ptr->win_ptr->both_rect.v_rect;
                            
                            MMK_DispatchToAllControl(
                                win_node_ptr, 
                                MSG_LCD_SWITCH, (DPARAM)cur_angle,
                                MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD );
                            
                            MMK_RunWinProc( win_handle, MSG_LCD_SWITCH, (DPARAM)cur_angle );
                        }
                        else
                        {
                            //SCI_TRACE_LOW:"MMK_ProcOpenWinMsg failed"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3631_112_2_18_3_23_30_52,(uint8*)"" );
                        }
                    }
                }

                //UILAYER_SetDirectDraw( FALSE );
            }
            break;
            
        default:
            result = FALSE; 
            // SCI_TRACE_LOW("MMI: the message needn't to process!");
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_ProcOpenWinMsg failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_3631_112_2_18_3_23_30_52,(uint8*)"" );

        result = FALSE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : set the window pointer that is processing the message 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_ConvertIdToHandle(
                                          uint32 id
                                          )
{
    MMI_HANDLE_T handle = 0;
    
    //convert static id to handle
    if( !MMK_IsHandleValid( id ) )
    {
        MMI_BASE_NODE_T* node_ptr = MMK_FindInBaseNodeByCondition( BaseNodeCompare, 0, id, 0, FALSE );
        
        if( PNULL != node_ptr )
        {
            handle = node_ptr->handle;
        }
    }
    else
    {
        handle = id;
    }

    return handle;
}

/*****************************************************************************/
// Description : if window include statusbar control
// Global resource dependence : 
// Author:Ming.Song
// Note:2007-8-1
/*****************************************************************************/
BOOLEAN MMK_IsIncludeStatusBar( MMI_HANDLE_T win_handle )
{
    BOOLEAN       result  = FALSE;
    MMI_WINDOW_T* win_ptr = PNULL;

    win_handle   = MMK_ConvertIdToHandle( win_handle );
    // get the window pointer base on win_id
    win_ptr = GetWindowPtr( win_handle );

    // if the window is exist and including the statusbar, set the result
    if ((PNULL != win_ptr))
    {
        result = win_ptr->is_has_statusbar;
    }   

    return result;
}

/*****************************************************************************/
// Description : if parent window include statusbar control
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
BOOLEAN MMK_IsParentWinIncStb( MMI_HANDLE_T win_handle )
{
    BOOLEAN       result  = FALSE;
    MMI_WINDOW_T* win_ptr = PNULL;
    MMI_HANDLE_T parent_win_handle = 0;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle   = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        parent_win_handle = GetParentHandle( win_node_ptr->tree_node_ptr );
    }

    // win_handle   = MMK_ConvertIdToHandle( win_handle );
    // get the window pointer base on win_id
    win_ptr = GetWindowPtr( parent_win_handle );

    // if the window is exist and including the statusbar, set the result
    if ((PNULL != win_ptr))
    {
        result = win_ptr->is_has_statusbar;
    }   

    return result;
}

/*****************************************************************************/
// Description : if window include tips control
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
BOOLEAN MMK_IsIncludeTips( MMI_HANDLE_T win_handle )
{
    BOOLEAN       result  = FALSE;
    MMI_WINDOW_T* win_ptr = PNULL;

    win_handle   = MMK_ConvertIdToHandle( win_handle );
    // get the window pointer base on win_id
    win_ptr = GetWindowPtr( win_handle );

    // if the window is exist and including the statusbar, set the result
    if ((PNULL != win_ptr))
    {
        result = win_ptr->is_has_tips;
    }   

    return result;
}

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetWinSoftkeyCtrlId(
									   MMI_HANDLE_T		win_handle
									   )
{
    MMI_WINDOW_T*		win_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );

	if (NULL != win_ptr)
	{
		return win_ptr->softkey_ctrl_id;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinSoftkeyCtrlId(
									   MMI_HANDLE_T		win_handle,
									   MMI_CTRL_ID_T	softkey_ctrl_id
									   )
{
    MMI_WINDOW_T*		win_ptr = PNULL;
    BOOLEAN				result  = FALSE;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );

	if (NULL != win_ptr)
	{
		win_ptr->softkey_ctrl_id = softkey_ctrl_id;
	}

	return result;
}

//#ifdef GUI_CTRL_SCROLL_KEY
/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetWinScrollkeyCtrlId(
									   MMI_HANDLE_T		win_handle
									   )
{
    MMI_WINDOW_T*		win_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );

	if (NULL != win_ptr)
	{
		return win_ptr->scrollkey_ctrl_id;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMK_SetWinScrollkeyCtrlId(
									   MMI_HANDLE_T		win_handle,
									   MMI_CTRL_ID_T	scrollkey_ctrl_id
									   )
{
    MMI_WINDOW_T*		win_ptr = PNULL;
    BOOLEAN				result  = FALSE;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );

	if (NULL != win_ptr)
	{
		win_ptr->scrollkey_ctrl_id = scrollkey_ctrl_id;
	}

	return result;
}
//#endif

/*****************************************************************************/
// Description : get win statusbar handle
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinStatusBarHandle(
                                              MMI_HANDLE_T win_handle 
                                              )
{
    return MMK_GetCtrlHandleByGuid(win_handle, SPRD_GUI_STATUSBAR_ID);
}

/*****************************************************************************/
// Description : get win title handle
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinTitleHandle(
                                          MMI_HANDLE_T win_handle 
                                          )
{
    return MMK_GetCtrlHandleByGuid(win_handle, SPRD_GUI_TITLE_ID);
}

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetWinTitleCtrlId(
                                           MMI_HANDLE_T		win_handle
                                           )
{
    MMI_WINDOW_T*		win_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );

	if (NULL != win_ptr)
	{
		return win_ptr->title_ctrl_id;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinTitleCtrlId(
                                     MMI_HANDLE_T		win_handle,
                                     MMI_CTRL_ID_T	title_ctrl_id
                                     )
{
    MMI_WINDOW_T*		win_ptr = PNULL;
    BOOLEAN				result  = FALSE;

    win_handle = MMK_ConvertIdToHandle( win_handle );

    // get the window pointer
    win_ptr = GetWindowPtr( win_handle );

	if (NULL != win_ptr)
	{
		win_ptr->title_ctrl_id = title_ctrl_id;
	}

	return result;
}

/*****************************************************************************/
// 	Description : get win handle by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandleByWinNode(
    MMI_BASE_NODE_T *node_ptr
    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;
    return win_node_ptr->base_node.handle;
}

/*****************************************************************************/
// 	Description : set active ctrl by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetActiveCtrlByWinNode(
    MMI_BASE_NODE_T *node_ptr,
    MMI_BASE_NODE_T *ctrl_node_ptr
    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;
    win_node_ptr->active_control_ptr = ctrl_node_ptr;
}

/*****************************************************************************/
// 	Description : get active ctrl by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_GetActiveCtrlByWinNode(
    MMI_BASE_NODE_T *node_ptr
    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;

    //active_control_ptr is invalid
    if ( PNULL != win_node_ptr->active_control_ptr
        && PNULL == MMK_GetCtrlNodeByHandle( win_node_ptr->active_control_ptr->handle ))
    {
        SCI_TRACE_LOW("active_control_ptr is invalid, win=0x%x, ctrl=0x%x", win_node_ptr, win_node_ptr->active_control_ptr);
        win_node_ptr->active_control_ptr = NULL;
    }

    return win_node_ptr->active_control_ptr;
}

/*****************************************************************************/
// 	Description : get ctrl tree by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_GetCtrlTreeByWinNode(
    MMI_BASE_NODE_T *node_ptr
    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;
    return win_node_ptr->ctrl_tree_root_ptr;
}

/*****************************************************************************/
// 	Description : get ctrl tree ptr by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T** MMK_GetCtrlTreePtrByWinNode(
    MMI_BASE_NODE_T *node_ptr
    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;
    return &win_node_ptr->ctrl_tree_root_ptr;
}

/*****************************************************************************/
// 	Description : get win tree ptr by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T** MMK_GetWinTreeByWinNode(
    MMI_BASE_NODE_T *node_ptr
    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;
    return win_node_ptr->tree_node_ptr;
}

/*****************************************************************************/
// 	Description : get win node ptr by win handle
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_GetWinNodeByHandle(
    MMI_HANDLE_T    win_handle
    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = GetWindowNodePtr(win_handle);
    return (MMI_BASE_NODE_T*)win_node_ptr;
}

/*****************************************************************************/
// 	Description : set has statusbar tag
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetHasStb(
    MMI_BASE_NODE_T *node_ptr,
    BOOLEAN         has_stb
    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;
    win_node_ptr->win_ptr->is_has_statusbar = has_stb;
}

/*****************************************************************************/
// 	Description : set has tips tag
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetHasTips(
    MMI_BASE_NODE_T *node_ptr,
    BOOLEAN         has_tips
    )
{
    MMI_WINDOW_NODE_T*   win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;
    win_node_ptr->win_ptr->is_has_tips = has_tips;
}


/*****************************************************************************/
// 	Description : set the window pointer that is processing the message 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BaseNodeCompare( 
                              MMI_BASE_NODE_T* node_ptr,
                              uint32 condition1, 
                              uint32 condition2
                              ) 
{
    BOOLEAN result = FALSE;

    if( MMI_HANDLE_WINDOW == node_ptr->handle_type )
    {
        MMI_WINDOW_NODE_T* win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;
        
        if( win_node_ptr->win_id == condition2 
            //必须是平台内的窗口
            && !MMK_GetAppletState( win_node_ptr->parent_applet_handle, MMI_APPLET_STATE_DYNAMIC ) )
        {
            result = TRUE;
        }
    }
    else if( MMI_HANDLE_CONTROL == node_ptr->handle_type )
    {
        if( MMK_GetCtrlIdByCtrlNode(node_ptr) == condition2 )
        {
            MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( MMK_GetWinHandleByCtrlNode(node_ptr));
            
            //必须是平台内的控件
            if( win_node_ptr != PNULL 
                && !MMK_GetAppletState( win_node_ptr->parent_applet_handle, MMI_APPLET_STATE_DYNAMIC ) )
            {
                result = TRUE;
            }
        }
    }
    else if ( MMI_HANDLE_APPLET == node_ptr->handle_type )
    {
        if ( MMK_GetGuidByHandle( node_ptr->handle ) == condition2
            && !MMK_GetAppletState( node_ptr->handle, MMI_APPLET_STATE_DYNAMIC ) )
        {
            result = TRUE;
        }
    }

    return result;
}

#ifdef ATEST_SUPPORT
/*****************************************************************************/
//  Description : 获取窗体控件树根节点
//  Global resource dependence :
//  Author:xiaolei.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_GetCtrlTreeRootByWin(
                                            MMI_HANDLE_T win_handle
                                            )
{
    MMI_TREE_NODE_T* root_ptr = PNULL;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;

    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        root_ptr = win_node_ptr->ctrl_tree_root_ptr;
    }

    return root_ptr;
}

/*****************************************************************************/
//  Description : 获取顶层窗体
//  Global resource dependence :
//  Author:xiaolei.zheng
//  Note:
/*****************************************************************************/    
PUBLIC MMI_HANDLE_T MMK_GetTopmostWindow(
                                    void
                                    )
{
    return GetTopmostWindow();
}
#endif // ATEST_SUPPORT

/*****************************************************************************/
// 	Description : get parent handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetParentHandle( 
                                   MMI_TREE_NODE_T* node_ptr
                                   )
{
    MMI_HANDLE_T parent_handle = 0;

    SCI_ASSERT( PNULL != node_ptr );/*assert verified*/

    if( PNULL != node_ptr->parent_ptr )
    {
        parent_handle = node_ptr->parent_ptr->data;
    }

    return parent_handle;
}

/*****************************************************************************/
// 	Description : get ancestor handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:获得祖先的句柄
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetAncestorHandle( 
                                     MMI_TREE_NODE_T* node_ptr
                                     )
{
    MMI_HANDLE_T     ancestro_handle  = 0;
    MMI_TREE_NODE_T* temp_node_ptr    = node_ptr;

    SCI_ASSERT( PNULL != node_ptr );/*assert verified*/

    do
    {
        ancestro_handle = temp_node_ptr->data;
        
        //解决父窗口已经被销毁时，exception的问题
        if ( PNULL == GetWindowNodePtr( ancestro_handle ) )
        {
            ancestro_handle = 0;
            break;
        }
    }while( PNULL != ( temp_node_ptr = temp_node_ptr->parent_ptr ) );

    return ancestro_handle;
}

/*****************************************************************************/
// 	Description : is need paint prev window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedPaintPrevWin( 
                                 MMI_WINDOW_NODE_T* win_node_ptr
                                 )
{
    BOOLEAN result = FALSE;

    SCI_ASSERT( PNULL != win_node_ptr );/*assert verified*/
    
    //非全屏窗口或透明窗口, 需要绘制前一个窗口
    if(  !MMITHEME_IsFullScreenRect( win_node_ptr->win_ptr->window_rect )
        || ( win_node_ptr->win_ptr->window_style & WS_HAS_TRANSPARENT ) == WS_HAS_TRANSPARENT )
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : post update screen message
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN PostUpdateScreenMsg( 
                                  MMI_ZORDER_UPDATE_TYPE_E type
                                  )
{
    BOOLEAN result = TRUE;
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    if ( zorder_system_ptr->is_need_update )
    {
        //画屏保护, 防止在full paint时开关窗口导致的消息死循环
        if( !zorder_system_ptr->is_on_paint )
        {
            //延迟刷屏, 防止冗余刷新
            MMK_DelayUpdateScreen();
            
            SetZorderSystemUpdateType( type );
        }
        else
        {
            //向top most的窗口发送full paint, 防止在full paint时打开的窗口无法刷新
            MMK_PostMsg( GetTopmostWindow(), MSG_FULL_PAINT, PNULL, 0 );
            
            result = FALSE;
            
            //SCI_TRACE_LOW:"PostUpdateScreenMsg: don't open or close window when full paint!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_1069_112_2_18_3_23_24_37,(uint8*)"");
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : 是否处理过open消息
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:防止窗口收到open消息前收到其他消息
/*****************************************************************************/
LOCAL BOOLEAN IsHandleOpenMsg( 
                              MMI_HANDLE_T win_handle
                              )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        result = win_node_ptr->win_ptr->is_handle_open;
    }
    
    if ( !result )
    {
        SCI_TRACE_LOW("IsHandleOpenMsg FALSE, win_handle = 0x%08x", win_handle);
    }
    
    return result;
}

#if 0
/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 WindowTreeNodeHandleEvent( 
                                       MMI_TREE_NODE_T* node_ptr,
                                       uint32           msg_id,
                                       void*            param
                                       )
{
    //use send message here, because can dispatch event to control
    return MMK_SendMsg( node_ptr->data, msg_id, param );
}
#endif

/*****************************************************************************/
// 	Description : create a window in parseing window table
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN WinTableCreate(
                             MMI_TREE_NODE_T* node_ptr,
                             const MMI_WINDOW_TABLE_CREATE_T* create_ptr
                             )
{
    MMI_HANDLE_T         applet_handle     = create_ptr->applet_handle;
    const uint32*        win_table_ptr     = create_ptr->win_table_ptr;
    WINDOW_HANDLE_EVENT  func              = create_ptr->func;
    MMI_WINDOW_T*        new_win_ptr       = PNULL;
    MMI_WINDOW_NODE_T*   new_win_node_ptr  = PNULL;
    BOOLEAN       result = FALSE;

    SCI_ASSERT( PNULL != create_ptr );/*assert verified*/
    /*SCI_ASSERT( PNULL != func );*/
    
    new_win_ptr = (MMI_WINDOW_T *)SCI_ALLOC_APP( sizeof(MMI_WINDOW_T) );
    SCI_MEMSET((void*)new_win_ptr, 0, sizeof(MMI_WINDOW_T) );
    
    new_win_ptr->win_priority = WIN_ONE_LEVEL;//default priority

    new_win_ptr->background_image_id = MMITHEME_GetInvalidImageID();
    new_win_ptr->window_style      = MMITHEME_GetDefaultWinStyle();
    new_win_ptr->window_move_style = MOVE_BY_SETTING;
    new_win_ptr->window_rect.left = 0;
    //child win top is CHILD_WIN_RECT_TOP
    new_win_ptr->window_rect.top = ( create_ptr->parent_win_handle ) ?  MMITHEME_GetTabChildWinTop(create_ptr->parent_win_handle) : 0;
    new_win_ptr->window_rect.right= MMITHEME_GetMainScreenRightPixel();  
    new_win_ptr->window_rect.bottom= MMITHEME_GetMainScreenBottomPixel();

    new_win_ptr->ProcessMsg = func;
    new_win_ptr->adddata_ptr = create_ptr->add_data_ptr;
    new_win_ptr->adddata_ex_ptr = create_ptr->add_data_ex_ptr;
    
    new_win_node_ptr = CreateWindowNode( node_ptr, applet_handle, new_win_ptr );

    SCI_ASSERT( PNULL != new_win_ptr );/*assert verified*/
    SCI_ASSERT(PNULL != new_win_node_ptr);/*assert verified*/

    //adjust default rect
    AdjustWinRect( new_win_ptr );

    //must parse success
    if( !MMK_ParseWinTab( new_win_ptr->win_handle, (uint32*)win_table_ptr ) )
    {
        //must remove control
        MMK_DestroyAllControlByNode(new_win_node_ptr);

        DestroyWindowResource( new_win_ptr->win_handle );

        DestroyWindowNode( new_win_node_ptr );

        //SCI_TRACE_LOW:"WinTableCreate: parse failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_1343_112_2_18_3_23_25_40,(uint8*)"");
    }
    else
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : create a dyanmic window and open it
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN WindowCreate(
                           MMI_TREE_NODE_T*           node_ptr,
                           const MMI_WINDOW_CREATE_T* create_ptr
                           )
{
    MMI_WINDOW_T*      new_win_ptr      = PNULL;
    MMI_WINDOW_NODE_T* new_win_node_ptr = PNULL;
    MMK_DEFAULT_CTRL_PARAM_T    default_ctrl_param = {0};

    SCI_ASSERT( PNULL != create_ptr );/*assert verified*/
    SCI_ASSERT( create_ptr->win_id < MMITHEME_GetInvalidWinId() );/*assert verified*/

    new_win_ptr = CreateWin( create_ptr );

    new_win_node_ptr = CreateWindowNode( node_ptr, create_ptr->applet_handle, new_win_ptr );
    SCI_ASSERT( PNULL != new_win_node_ptr );/*assert verified*/

    new_win_node_ptr->win_id = create_ptr->win_id;

    MMK_CreateDefaultControl(new_win_ptr->win_handle, &default_ctrl_param);
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : get window system pointer
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_ZORDER_SYSTEM_T* GetZorderSystem( void )
{
    return &s_zorder_system;
}

/*****************************************************************************/
// 	Description : get top most window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetTopmostWindow(
                                    void
                                    )
{
    if( PNULL != s_zorder_system.top_most_ptr )
    {
        SCI_ASSERT( PNULL != s_zorder_system.top_most_ptr->win_node_ptr );/*assert verified*/

        return s_zorder_system.top_most_ptr->win_node_ptr->base_node.handle;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
// 	Description : get first paint window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetFirstPaintWindow(
                                       void
                                       )
{
    if( PNULL != s_zorder_system.first_paint_ptr )
    {
        SCI_ASSERT( PNULL != s_zorder_system.first_paint_ptr->win_node_ptr );/*assert verified*/

        return s_zorder_system.first_paint_ptr->win_node_ptr->base_node.handle;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//     Description : Get current support angle
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL WINDOW_SUPPORT_ANGLE GetCurSupportAngle(
                                              void
                                              )
{
    WINDOW_SUPPORT_ANGLE topmost_angle = MMK_GetWinSupportAngle( GetTopmostWindow() );
    WINDOW_SUPPORT_ANGLE firstpaint_angle = MMK_GetWinSupportAngle( GetFirstPaintWindow() );

    return ( WIN_SUPPORT_ANGLE_ALL == topmost_angle ) ? firstpaint_angle : topmost_angle;
}

/*****************************************************************************/
// 	Description : is window landscape
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:用于取代MMITHEME_IsMainScreenLandscape接口, 全局变量可能和win中angle不一致
/*****************************************************************************/
LOCAL BOOLEAN IsWindowLandscape( 
                                MMI_HANDLE_T win_handle
                                )
{
    BOOLEAN result = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );
    
    if( PNULL != win_node_ptr )
    {
        if ( LCD_ANGLE_90 == win_node_ptr->win_ptr->cur_angle
            || LCD_ANGLE_270 == win_node_ptr->win_ptr->cur_angle )
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : create window node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_NODE_T* CreateWindowNode( 
                                          MMI_TREE_NODE_T* node_ptr,
                                          MMI_HANDLE_T     applet_handle,
                                          MMI_WINDOW_T*    win_ptr
                                          )
{
    MMI_WINDOW_NODE_T* win_node_ptr = (MMI_WINDOW_NODE_T*)MMK_CreateBaseNode( MMI_HANDLE_WINDOW, sizeof(MMI_WINDOW_NODE_T) );

    SCI_ASSERT( PNULL != win_ptr );/*assert verified*/
    SCI_ASSERT( PNULL != win_node_ptr );/*assert verified*/

    win_node_ptr->parent_applet_handle   = applet_handle;
    win_node_ptr->win_ptr                = win_ptr;
    win_node_ptr->tree_node_ptr          = node_ptr;
    
    node_ptr->data = win_ptr->win_handle = win_node_ptr->base_node.handle;

    win_node_ptr->zorder_node.win_node_ptr = win_node_ptr;

    win_ptr->cur_angle = GUILCD_GetLogicAngle( GUI_MAIN_LCD_ID );
    win_ptr->both_rect.v_rect = win_ptr->both_rect.h_rect = win_ptr->window_rect;
    
    if ( IsWindowLandscape( win_node_ptr->base_node.handle ) )
    {
        win_ptr->both_rect.v_rect.right = win_ptr->both_rect.h_rect.bottom;
        win_ptr->both_rect.v_rect.bottom = win_ptr->both_rect.h_rect.right;
        
    }
    else
    {
        win_ptr->both_rect.h_rect.right = win_ptr->both_rect.v_rect.bottom;
        win_ptr->both_rect.h_rect.bottom = win_ptr->both_rect.v_rect.right;
    }

    return win_node_ptr;
}

/*****************************************************************************/
// 	Description : destroy window resource
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyWindowResource(
                                    MMI_HANDLE_T win_handle 
                                    )
{
    MMK_DeleteMSGByHandle( win_handle );
    MMK_DeleteTimerByHandle( win_handle );
    MMK_CloseGsensor( win_handle );
    MMI_FreeRes( win_handle );
    MMITHEME_FreeMenuGrayed( win_handle );
    
    return TRUE;
}


/*****************************************************************************/
// 	Description : destroy window node
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyWindowNode(
                                MMI_WINDOW_NODE_T* win_node_ptr				    
                                )
{
	SCI_ASSERT( PNULL != win_node_ptr && PNULL != win_node_ptr->win_ptr );/*assert verified*/

    SCI_FREE( win_node_ptr->win_ptr );
    SCI_FREE( win_node_ptr->tree_node_ptr );
    //MMI_FreeRes(win_node_ptr->base_node.handle);
    MMK_DestroyBaseNode( (MMI_BASE_NODE_T*)win_node_ptr );

    return TRUE;
}

/*****************************************************************************/
// 	Description : get the window node
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_NODE_T* GetWindowNodePtr(
                                          MMI_HANDLE_T win_handle
                                          )
{
    MMI_WINDOW_NODE_T* win_node_ptr  = PNULL;
    MMI_BASE_NODE_T*   base_node_ptr = MMK_FindBaseNode( win_handle );

    if( PNULL != base_node_ptr && MMI_HANDLE_WINDOW == base_node_ptr->handle_type )
    {
        win_node_ptr = (MMI_WINDOW_NODE_T*)base_node_ptr;

        SCI_ASSERT( PNULL != win_node_ptr->win_ptr );/*assert verified*/
    }
    
    return win_node_ptr;
}

/*****************************************************************************/
// 	Description : get the window pointer base on window_id
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_T* GetWindowPtr(
                                 MMI_HANDLE_T win_handle
                                 )
{
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );
    
    if( NULL != win_node_ptr )	
    {
        return win_node_ptr->win_ptr;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
// 	Description : create a dyanmic window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_T* CreateWin(
                              const MMI_WINDOW_CREATE_T* create_ptr
                              )
{
	MMI_WINDOW_T *new_win_ptr = PNULL;
    //MMI_SOFT_ICON soft_icon_state = 0; 

	// check the param
	if ( PNULL == create_ptr->func )
	{
		//SCI_TRACE_LOW:"MMI: CreateWin, create window is fail!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_1917_112_2_18_3_23_26_42,(uint8*)"");
		return PNULL;
	}

    // alloc the MMI_WINDOW_T space
    new_win_ptr = (MMI_WINDOW_T *)SCI_ALLOC_APP(sizeof(MMI_WINDOW_T) );
    SCI_MEMSET( (void*)new_win_ptr, 0, sizeof(MMI_WINDOW_T) );
  
    // init the member of the window
    new_win_ptr->win_priority      = create_ptr->win_priority;
    new_win_ptr->window_move_style = create_ptr->window_move_style;	 
    new_win_ptr->ProcessMsg        = create_ptr->func;
    new_win_ptr->adddata_ptr       = create_ptr->add_data_ptr;
    new_win_ptr->adddata_ex_ptr    = create_ptr->add_data_ex_ptr;
    new_win_ptr->is_not_need_copylcd = FALSE;//default need to copylcd when losefocus
    //soft_icon_state = create_ptr->softicon_state; // to remove warning
    if (0 == (create_ptr->window_style&WS_NO_DEFAULT_STYLE))
    {
        new_win_ptr->window_style = MMITHEME_GetDefaultWinStyle();
    }
    new_win_ptr->window_style |= create_ptr->window_style;

//     if ((new_win_ptr->window_style & WS_HAS_PROMPT_WIN) > 0)
//     {
//         new_win_ptr->window_rect.left = MMITHEME_GetFullScreenRect().left;
//         new_win_ptr->window_rect.right = MMITHEME_GetFullScreenRect().right;
//         new_win_ptr->window_rect.top = MMITHEME_GetFullScreenRect().top;
//         new_win_ptr->window_rect.bottom = MMITHEME_GetFullScreenRect().bottom;
//     }
//     else
    {
        new_win_ptr->window_rect.left = 0;
        //child win top is CHILD_WIN_RECT_TOP
        new_win_ptr->window_rect.top = ( create_ptr->parent_win_handle ) ? MMITHEME_GetTabChildWinTop( create_ptr->parent_win_handle ) : 0;
        new_win_ptr->window_rect.right = MMITHEME_GetMainScreenRightPixel();  
        new_win_ptr->window_rect.bottom = MMITHEME_GetMainScreenBottomPixel(); 
    }
    
    new_win_ptr->background_image_id = MMITHEME_GetInvalidImageID();

	return new_win_ptr;
}

/*****************************************************************************/
// 	Description : get foucs wind
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_WINDOW_NODE_T* GetFocusWinNodePtr( void )
{
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;

    if( PNULL == ( win_node_ptr = GetWindowNodePtr( MMK_GetFocusParentWinHandle() ) ) )
    {
        win_node_ptr = GetWindowNodePtr( MMK_GetFocusWinHandle() );
    }

    return win_node_ptr;
}

/*****************************************************************************/
// 	Description : set the focus win
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFocusWin(
                          MMI_HANDLE_T win_handle
                          )
{
    LCD_ANGLE_E angle = LCD_ANGLE_0;

    s_focus_win_handle = win_handle;

    //设置角度
    angle = GetCurAngle( s_screen_angle, GetCurSupportAngle() );
    
    GUILCD_SetLogicAngle( GUI_MAIN_LCD_ID, angle );
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : 仅仅用于设置子窗口focus
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 设置子窗口focus时, 判断其父亲是否为focus
/*****************************************************************************/
LOCAL BOOLEAN SetFocusChildWin(
                               MMI_HANDLE_T win_handle
                               )
{
    BOOLEAN            result            = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr      = PNULL;
    MMI_HANDLE_T       parent_win_handle = 0;
    
    if ( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) 
        && 0 != ( parent_win_handle = GetParentHandle( win_node_ptr->tree_node_ptr ) ) )
    {
        if ( GetTopmostWindow() == parent_win_handle )
        {
            SetFocusWin( win_handle );

            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : set zorder update type
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetZorderSystemUpdateType(
                                        MMI_ZORDER_UPDATE_TYPE_E type
                                        )
{
    BOOLEAN result = FALSE;
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    switch( type )
    {
    case MMI_ZORDER_SYSTEM_UPDATE_ALL:
        zorder_system_ptr->update_type = MMI_ZORDER_SYSTEM_UPDATE_ALL;
    	break;
    case MMI_ZORDER_SYSTEM_UPDATE_CURRENT:
        if( zorder_system_ptr->update_type == MMI_ZORDER_SYSTEM_UPDATE_NONE )
        {
            zorder_system_ptr->update_type = MMI_ZORDER_SYSTEM_UPDATE_CURRENT;
        }
        else if( zorder_system_ptr->update_type == MMI_ZORDER_SYSTEM_UPDATE_CURRENT )
        {
            zorder_system_ptr->update_type = MMI_ZORDER_SYSTEM_UPDATE_ALL;
        }
    	break;
    default:
        break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : reset zorder update type
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void ResetZorderSystemUpdateType( void )
{
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    zorder_system_ptr->update_type = MMI_ZORDER_SYSTEM_UPDATE_NONE;
}

/*****************************************************************************/
// 	Description : set zorder focus type
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetZorderSystemFocusType(
                                       MMI_ZORDER_FOCUS_TYPE_E type
                                       )
{
    BOOLEAN result = FALSE;
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    switch( type )
    {
    case MMI_ZORDER_SYSTEM_FOCUS_NORMAL:
        zorder_system_ptr->need_send_focus = MMI_ZORDER_SYSTEM_FOCUS_NORMAL;
    	break;
    case MMI_ZORDER_SYSTEM_FOCUS_CHILD:
        if( zorder_system_ptr->need_send_focus == MMI_ZORDER_SYSTEM_FOCUS_NONE )
        {
            zorder_system_ptr->need_send_focus = MMI_ZORDER_SYSTEM_FOCUS_CHILD;
        }
    	break;
    default:
        break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : reset zorder focus type
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void ResetZorderSystemFocusType( void )
{
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    zorder_system_ptr->need_send_focus = MMI_ZORDER_SYSTEM_FOCUS_NONE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:设置zorder system是否需要刷新
/*****************************************************************************/
LOCAL void SetZorderSystemNeedPaint(
                                    MMI_WIN_PRIORITY_E win_priority
                                    )
{
    MMI_WIN_PRIORITY_E   old_priority = WIN_LOWEST_LEVEL;
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();
    
    if ( PNULL != zorder_system_ptr->first_paint_ptr )
    {
        SCI_ASSERT( PNULL != zorder_system_ptr->first_paint_ptr->win_node_ptr );/*assert verified*/
        
        old_priority = zorder_system_ptr->first_paint_ptr->win_node_ptr->win_ptr->win_priority;
    }
    
    if ( win_priority >= old_priority )
    {
        zorder_system_ptr->is_need_update = TRUE;
    }

    SetZorderSystemFirstPaintNode();
}

/*****************************************************************************/
// 	Description : find first paint zorder node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void SetZorderSystemFirstPaintNode(
                                         void
                                         )
{
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();
    MMI_ZORDER_NODE_T*   find_node_ptr     = PNULL;
    MMI_ZORDER_NODE_T*   result_ptr        = PNULL;
    
    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/

    //SetZorderSystemNeedPaint( win_priority );

    find_node_ptr = zorder_system_ptr->top_most_ptr;

    if( PNULL != find_node_ptr )
    {
        do 
        {
            MMI_WINDOW_NODE_T* win_node_ptr = find_node_ptr->win_node_ptr;
            
            SCI_ASSERT( PNULL != win_node_ptr );/*assert verified*/
            
            if( !IsNeedPaintPrevWin( win_node_ptr ) )
            {      
                result_ptr = find_node_ptr;
                break;
            }
            
            find_node_ptr = find_node_ptr->prev_ptr;
            
        } while( find_node_ptr != zorder_system_ptr->top_most_ptr );
    }

    zorder_system_ptr->first_paint_ptr = result_ptr;
}

/*****************************************************************************/
// 	Description : add to zorder system
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:新建窗口和切换焦点使用该函数, 关闭窗口使用zorder update
/*****************************************************************************/
LOCAL BOOLEAN AddToZorderSystem(
                                MMI_ZORDER_NODE_T* node_ptr,
                                BOOLEAN            is_hide
                                )
{
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();
    MMI_WIN_PRIORITY_E   win_priority = WIN_LOWEST_LEVEL;
    MMI_ZORDER_NODE_T**  root_pptr = PNULL;
    
    SCI_ASSERT( PNULL != node_ptr && PNULL != zorder_system_ptr && PNULL != node_ptr->win_node_ptr );/*assert verified*/

    if( GetParentHandle( node_ptr->win_node_ptr->tree_node_ptr ) )
    {
        //只有父窗口才能加入zorder
        //SCI_TRACE_LOW:"MMI: AddToZorderSystem failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_WINDOW_2198_112_2_18_3_23_27_43,(uint8*)"");

        return FALSE;
    }

    root_pptr = ( is_hide ) ? &zorder_system_ptr->top_hide_ptr : &zorder_system_ptr->top_most_ptr;

    win_priority = node_ptr->win_node_ptr->win_ptr->win_priority;

    if( PNULL != (*root_pptr) )
    {
        MMI_ZORDER_NODE_T* find_node_ptr       = (*root_pptr);
        BOOLEAN            is_find             = FALSE;
        MMI_WINDOW_T*      win_ptr             = PNULL;
        
        do 
        {
            SCI_ASSERT( PNULL != find_node_ptr->win_node_ptr );/*assert verified*/
            
            win_ptr = find_node_ptr->win_node_ptr->win_ptr;
            
            SCI_ASSERT( PNULL != win_ptr );/*assert verified*/
            
            if( win_priority >= win_ptr->win_priority )
            {
                is_find = TRUE;
                
                break;
            }
            
            find_node_ptr = find_node_ptr->prev_ptr;
            
        } while( find_node_ptr != (*root_pptr) );
        
        if( is_find )
        {
            node_ptr->prev_ptr = find_node_ptr;
            node_ptr->next_ptr = find_node_ptr->next_ptr;
            
            find_node_ptr->next_ptr->prev_ptr = node_ptr;
            find_node_ptr->next_ptr           = node_ptr;
            
            //cover top most
            if( (*root_pptr) == find_node_ptr )
            {
                (*root_pptr) = node_ptr;
            }
        }
        else
        {
            node_ptr->prev_ptr = (*root_pptr);
            node_ptr->next_ptr = (*root_pptr)->next_ptr;
            
            (*root_pptr)->next_ptr->prev_ptr = node_ptr;
            (*root_pptr)->next_ptr           = node_ptr;
        }
    }
    else
    {
        (*root_pptr) = node_ptr;
        node_ptr->prev_ptr  = node_ptr;
        node_ptr->next_ptr  = node_ptr;
    }

    node_ptr->is_hide = is_hide;

    if ( !is_hide )
    {
        //set got topmost state
        if( PNULL != zorder_system_ptr->top_most_ptr )
        {
            zorder_system_ptr->top_most_ptr->is_got_topmost = TRUE;
        }

        SetZorderSystemNeedPaint( win_priority );
        //SetZorderSystemFirstPaintNode( win_priority );
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : remove zorder node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void RemoveZorderNode(
                            MMI_ZORDER_NODE_T* node_ptr
                            )
{
    //防止无效node传入引起exception
    if ( PNULL != node_ptr 
        && PNULL != node_ptr->next_ptr
        && PNULL != node_ptr->prev_ptr )
    {   
        node_ptr->next_ptr->prev_ptr = node_ptr->prev_ptr;
        node_ptr->prev_ptr->next_ptr = node_ptr->next_ptr;
    }
    else
    {
        SCI_TRACE_LOW("MMI: RemoveZorderNode failed!");
    }
}

/*****************************************************************************/
// 	Description : remove from zorder system
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN RemoveFromZorderSystem(
                                     MMI_ZORDER_NODE_T* node_ptr
                                     )
{
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();
    MMI_WIN_PRIORITY_E   win_priority = WIN_LOWEST_LEVEL;
    MMI_ZORDER_NODE_T**  root_pptr = PNULL;

    SCI_ASSERT( PNULL != node_ptr );/*assert verified*/
    SCI_ASSERT( PNULL != zorder_system_ptr );/*assert verified*/
    SCI_ASSERT( PNULL != node_ptr->win_node_ptr );/*assert verified*/
    SCI_ASSERT( PNULL != node_ptr->win_node_ptr->win_ptr );/*assert verified*/

    root_pptr = ( node_ptr->is_hide ) ? &zorder_system_ptr->top_hide_ptr : &zorder_system_ptr->top_most_ptr;
    win_priority = node_ptr->win_node_ptr->win_ptr->win_priority;
    
    if ( (*root_pptr) == node_ptr )
    {
        //only one node
        if( node_ptr->next_ptr == node_ptr )
        {
            (*root_pptr) = PNULL;
        }
        else
        {
            RemoveZorderNode( node_ptr );

            (*root_pptr) = node_ptr->prev_ptr;
            
            if ( !node_ptr->is_hide )
            {
                SetZorderSystemFocusType(MMI_ZORDER_SYSTEM_FOCUS_NORMAL);
            }
        }
    }
    else
    {
        RemoveZorderNode( node_ptr );
    }

    if ( !node_ptr->is_hide )
    {
        //set got topmost state
        if ( PNULL != zorder_system_ptr->top_most_ptr )
        {
            zorder_system_ptr->top_most_ptr->is_got_topmost = TRUE;
        }
        
        SetZorderSystemNeedPaint( win_priority );
        //SetZorderSystemFirstPaintNode( win_priority );
    }

    return TRUE;
}

/*****************************************************************************/
//     Description : initialise the window rectangle
//    Global resource dependence : 
//  Author: robert.lu
//    Note:
/*****************************************************************************/
LOCAL void AdjustWinRect(
                         MMI_WINDOW_T* new_win_ptr         // window pointer
                         )
{  
    SCI_ASSERT(PNULL != new_win_ptr);/*assert verified*/

    if (new_win_ptr->window_rect.left < 0)
    {
        new_win_ptr->window_rect.left = 0;
    }
    if (new_win_ptr->window_rect.top < 0)
    {
        new_win_ptr->window_rect.top = 0;
    }
    if (new_win_ptr->window_rect.right < new_win_ptr->window_rect.left)
    {
        new_win_ptr->window_rect.right = new_win_ptr->window_rect.left;
    }
    if (new_win_ptr->window_rect.bottom < new_win_ptr->window_rect.top)
    {
        new_win_ptr->window_rect.bottom = new_win_ptr->window_rect.top;
    }

    new_win_ptr->client_rect  = new_win_ptr->window_rect;
}

/*****************************************************************************/
//     Description : get the window pointer on the point
//    Global resource dependence : 
//  Author:Robert.Lu
//    Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_GetTPPressWin(
                                       GUI_POINT_T point,
                                       BOOLEAN     is_find_parent
                                       )
{
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    /* 在此假设所有的window都是全屏的, 将来扩展需要找到于full screen的applet */
    if( PNULL != ( win_node_ptr = GetWindowNodePtr( MMK_GetFocusWinHandle() ) ) )
    {
        if ( !GUI_PointIsInRect( point, win_node_ptr->win_ptr->window_rect )
            && is_find_parent )
        {
            MMI_HANDLE_T parent_win_handle = GetParentHandle( win_node_ptr->tree_node_ptr );
            
            if( 0 != parent_win_handle )
            {
                win_node_ptr = GetWindowNodePtr( parent_win_handle );
            }
            else
            {
                MMI_ZORDER_NODE_T* top_most_ptr = GetZorderSystem()->top_most_ptr;

                SCI_ASSERT( PNULL != top_most_ptr->win_node_ptr );/*assert verified*/

                win_node_ptr = top_most_ptr->win_node_ptr;
            }
        }
    }

    return win_node_ptr;
}

/*****************************************************************************/
// 	Description : hide win
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:隐藏窗口
/*****************************************************************************/
LOCAL BOOLEAN HideWindow(
                         MMI_ZORDER_NODE_T* node_ptr,
                         BOOLEAN            is_hide
                         )
{
    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();
    BOOLEAN              result            = FALSE;

    SCI_ASSERT( PNULL != node_ptr );/*assert verified*/

    if( PNULL != zorder_system_ptr->top_most_ptr )
    {
        RemoveFromZorderSystem( node_ptr );

        AddToZorderSystem( node_ptr, is_hide );
        
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : create zorder list
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_ZORDER_NODE_T** CreateZorderList(
                                            MMI_HANDLE_T        applet_handle,
                                            MMI_ZORDER_NODE_T*  first_node_ptr,
                                            uint32*             num_ptr
                                            )
{
    MMI_ZORDER_NODE_T**  zorder_list_pptr = PNULL;
    uint32               total_num        = 0;

    SCI_ASSERT( PNULL != num_ptr );/*assert verified*/

    if ( PNULL != first_node_ptr )
    {
        uint16               max_num          = ZORDER_UPDATE_TEMP_NUM;
        MMI_ZORDER_NODE_T*   zorder_node_ptr  = first_node_ptr;
        
        zorder_list_pptr = SCI_ALLOC_APP( max_num * sizeof(MMI_ZORDER_NODE_T*) );
        //从第一个窗口开始向后循环
        //记录属于applet的所有父窗口zorder
        do 
        {
            SCI_ASSERT( PNULL != zorder_node_ptr->win_node_ptr );/*assert verified*/
            
            if ( zorder_node_ptr->win_node_ptr->parent_applet_handle == applet_handle )
            {
                SCI_ASSERT( total_num <= max_num );/*assert verified*/
                
                if( total_num >= max_num )
                {
                    MMI_ZORDER_NODE_T** new_list_pptr = PNULL;
                    
                    max_num *= 2;
                    
                    new_list_pptr = SCI_ALLOC_APP( max_num * sizeof(MMI_ZORDER_NODE_T*) );
                    
                    SCI_MEMCPY( new_list_pptr, zorder_list_pptr, total_num * sizeof(MMI_ZORDER_NODE_T*) );
                    
                    SCI_FREE( zorder_list_pptr );
                    
                    zorder_list_pptr = new_list_pptr;
                }
                
                zorder_list_pptr[total_num] = zorder_node_ptr;
                total_num++;
            }
            
            zorder_node_ptr = zorder_node_ptr->next_ptr;
            
        } while( zorder_node_ptr != first_node_ptr );
    }
    
    *num_ptr = total_num;

    return zorder_list_pptr;
}

/*****************************************************************************/
// 	Description : destroy
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyZorderList(
                                MMI_ZORDER_NODE_T** zorder_list_pptr
                                )
{
    BOOLEAN result = FALSE;

    if ( PNULL != zorder_list_pptr )
    {
        SCI_FREE( zorder_list_pptr );

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : hide applet window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:将applet所有窗口放到zorder system的隐藏窗口队列
/*****************************************************************************/
LOCAL BOOLEAN HideAppletWindow(
                               MMI_HANDLE_T        applet_handle,
                               MMI_ZORDER_NODE_T*  top_node_ptr,
                               BOOLEAN             is_hide
                               )
{
    MMI_ZORDER_NODE_T**  zorder_list_pptr  = PNULL;
    uint32               i                 = 0;
    uint32               total_num         = 0;
    BOOLEAN              result            = FALSE;

    if ( PNULL != top_node_ptr )
    {
        zorder_list_pptr = CreateZorderList( applet_handle, top_node_ptr->next_ptr, &total_num );
        
        //显示applet对应的所有窗口
        for ( i = 0; i < total_num; i++ )
        {
            HideWindow( zorder_list_pptr[i], is_hide );
        }
        
        DestroyZorderList( zorder_list_pptr );
        
        if ( total_num > 0 )
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : the window grabs focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN WinGrabFocus(
                           MMI_HANDLE_T win_handle,
                           BOOLEAN      is_need_send_msg
                           )
{
    BOOLEAN            result             = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr       = PNULL;
    MMI_HANDLE_T       parent_win_handle  = 0;

    if ( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) )
        && win_node_ptr->win_ptr->is_open )
    {
        //child win
        if( 0 != ( parent_win_handle = GetParentHandle( win_node_ptr->tree_node_ptr ) ) )
        {
            BOOLEAN is_get_focus = FALSE;
            MMI_WINDOW_NODE_T* parent_node_ptr = GetWindowNodePtr( parent_win_handle );
            
            SCI_ASSERT(parent_node_ptr != PNULL);/*assert verified*/
            
            if(is_need_send_msg
                && parent_node_ptr->focus_child_win_ptr )
            {
                MMK_SendMsg( parent_node_ptr->focus_child_win_ptr->base_node.handle,
                    MSG_LOSE_FOCUS, PNULL );
            }

            parent_node_ptr->focus_child_win_ptr = win_node_ptr;
            
            is_get_focus = SetFocusChildWin( win_handle );

            if ( is_get_focus )
            {
                MMK_SendMsg( win_handle, MSG_GET_FOCUS, PNULL );
            }

            if( is_need_send_msg )
            {
                if ( is_get_focus )
                {
                    MMK_PostMsg( parent_win_handle, MSG_FULL_PAINT, PNULL, 0 );
                }
            }
        }
        else
        {
            MMI_WINDOW_NODE_T* focus_win_node_ptr = GetFocusWinNodePtr();
            
            if( PNULL != focus_win_node_ptr )
            {
                MMI_HANDLE_T focus_win_handle = focus_win_node_ptr->base_node.handle;
                
                if ( win_handle != focus_win_handle
                    && win_node_ptr->win_ptr->win_priority >= focus_win_node_ptr->win_ptr->win_priority )
                {
                    MMI_HANDLE_T         new_applet_handle = win_node_ptr->parent_applet_handle;
                    MMI_ZORDER_SYSTEM_T* zorder_system_ptr = GetZorderSystem();

                    SCI_ASSERT(PNULL != zorder_system_ptr);/*assert verified*/
           
                    //更新zorder
                    RemoveFromZorderSystem( &win_node_ptr->zorder_node );
                    AddToZorderSystem( &win_node_ptr->zorder_node, FALSE );
                    
                    // 支持应用在收到MSG_GET_FOCUS时，关闭相关窗口以及其本身，但是不允许打开窗口
                    if( PNULL != win_node_ptr->focus_child_win_ptr )
                    {
                        //have child focus win
                        SetFocusWin( win_node_ptr->focus_child_win_ptr->base_node.handle );
                    }
                    else
                    {
                        SetFocusWin( win_handle );
                    }
                    
                    MMK_SendMsg( focus_win_handle, MSG_LOSE_FOCUS, PNULL );
                    MMK_SendMsg( win_handle, MSG_GET_FOCUS, PNULL );

                    MMK_SwitchApplet( new_applet_handle );

                    PostUpdateScreenMsg( MMI_ZORDER_SYSTEM_UPDATE_ALL );

                    result = TRUE;
                }
            }
        }
    }

    return result;
}

/*****************************************************************************/
// Description : 根据类型判断窗口是否处于focus状态
// Global resource dependence : 
// Author:gang.tong
// Note:
/*****************************************************************************/
LOCAL BOOLEAN IsWinOnFocusByStyle( 
                                  MMI_HANDLE_T      win_handle,
                                  WINDOW_MOVE_STYLE style
                                  )
{
    BOOLEAN            result       = FALSE;
    MMI_WINDOW_NODE_T* win_node_ptr = PNULL;
    
    win_handle  = MMK_ConvertIdToHandle( win_handle );

    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
    {
        if ((style == win_node_ptr->win_ptr->window_move_style)
            && MMK_IsFocusWin(win_handle))
        {
            result = TRUE;
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : FindExistWindow
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:查找句柄池中是否存在窗口, 解决关闭窗口消息中打开id相同窗口的问题
/*****************************************************************************/
LOCAL BOOLEAN FindExistWindow( 
                              MMI_BASE_NODE_T* node_ptr,
                              uint32 condition1, 
                              uint32 condition2
                              ) 
{
    BOOLEAN result = FALSE;

    if( MMI_HANDLE_WINDOW == node_ptr->handle_type )
    {
        MMI_WINDOW_NODE_T* win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;
        
        if( condition2 == win_node_ptr->win_id 
            && condition1 == win_node_ptr->parent_applet_handle )
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : 恢复默认的layer_blt顺序，窗口切换的时候调用
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RestoreDefaultLayerBlt(
                                  MMI_HANDLE_T		win_handle
                                  )
{
    MMI_WINDOW_NODE_T* win_node_ptr = GetWindowNodePtr( win_handle );

    if (!MMK_IsFocusWin(win_handle))
    {
        return;
    }
    if (PNULL == win_node_ptr)
    {
        return;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RestoreMainLayer();
        /*
        UILAYER_APPEND_BLT_T append_layer = {0};
        GUI_LCD_DEV_INFO lcd_dev_info = {0};        

        lcd_dev_info.block_id = MMITHEME_GetBgLayer();
        if (IsWindowDisableCommonBg(win_node_ptr->win_ptr))
        {
            UILAYER_RemoveBltLayer(&lcd_dev_info); 
        }
        else
        {
            append_layer.lcd_dev_info = lcd_dev_info;
            append_layer.layer_level = UILAYER_LEVEL_LOW;
            UILAYER_AppendBltLayer(&append_layer); 
        }
        */
//        UILAYER_Clear(&lcd_dev_info, 0);
    }
#endif
}

/*****************************************************************************/
//     Description : set window common bg on bottom
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMK_SetWinCommonBgOnBottom( 
//                                          MMI_HANDLE_T win_handle,
//                                          BOOLEAN is_common_bg_on_bottom
//                                          )
//{
//    BOOLEAN            result       = FALSE;
//    MMI_WINDOW_NODE_T  *win_node_ptr = PNULL;
//    //uint32             i = 0;
//    UILAYER_HANDLE_T   bg_layer = UILAYER_NULL_HANDLE;
//    
//    win_handle  = MMK_ConvertIdToHandle( win_handle );
//
//    if( PNULL != ( win_node_ptr = GetWindowNodePtr( win_handle ) ) )
//    {
//        win_node_ptr->win_ptr->is_common_bg_on_bottom = is_common_bg_on_bottom;
//
//        if (!is_common_bg_on_bottom)
//        {           
//            bg_layer = MMITHEME_GetBgLayer();
//            //remove bg_layer from blt layer
//            UILAYER_RemoveBltLayer(bg_layer);            
//        }
//        
//        result = TRUE;
//    }
//    
//    return result;
//}

/*****************************************************************************/
//     Description : is window disable common bg
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsWindowDisableCommonBg( 
                                      MMI_WINDOW_T *win_ptr
                                      )
{
    if (PNULL != win_ptr
        && ((win_ptr->window_style & WS_DISABLE_COMMON_BG ) == WS_DISABLE_COMMON_BG ))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
//end

/*****************************************************************************/
//     Description : Get angel
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:根据支持的角度和平台角度获得合适的角度
/*****************************************************************************/
LOCAL LCD_ANGLE_E GetCurAngle(
                              LCD_ANGLE_E angle,
                              WINDOW_SUPPORT_ANGLE support_angle
                              )
{
    if ( WIN_SUPPORT_ANGLE_ALL != support_angle )
    {
        uint32 i = 0;
        BOOLEAN support_arry[LCD_ANGLE_270 + 1] = {0};

        for ( i = 0; i < LCD_ANGLE_270 + 1; i++ )
        {
            if ( support_angle & (1<<i) )
            {
                support_arry[i] = TRUE;
            }
        }

        //不支持设为指定角度
        if ( !support_arry[s_screen_angle] )
        {
            for ( i = s_screen_angle; i < s_screen_angle + LCD_ANGLE_270 + 1; i++ )/*lint !e656*/
            {
                angle = i %(LCD_ANGLE_270 + 1);

                if ( support_arry[angle] )
                {
                    //找到离指定角度最近的支持角度
                    break;
                }
            }
        }
    }

    return angle;
}

#ifdef GUI_DUMP_CTRL_TREE

#define GUI_DUMP_MAX_INDENT     10      // 最大缩进个数
#define GUI_DUMP_SPACE_STEP     2       // 缩进步长

/*****************************************************************************/
// 	Description : print ctrl info
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PrintCtrlNodeInfo(
                             MMI_BASE_NODE_T    *ctrl_node_ptr,
                             uint16             indent_index
                             )
{
    char        space[GUI_DUMP_MAX_INDENT*GUI_DUMP_SPACE_STEP + 1] = {0};
    GUI_RECT_T  rect = {0};

    if (indent_index > GUI_DUMP_MAX_INDENT)
    {
        indent_index = GUI_DUMP_MAX_INDENT;
    }

    SCI_MEMSET(space, 0x20, indent_index*GUI_DUMP_SPACE_STEP);

    IGUICTRL_GetRect(MMK_GetCtrlPtrByCtrlNode(ctrl_node_ptr), &rect);

    SCI_TRACE_LOW("%sid=0x%08x, handle=0x%08x, ctrl_ptr=0x%08x, guid=%s, rect={%d,%d,%d,%d}",
            space,
            MMK_GetCtrlIdByCtrlNode(ctrl_node_ptr),
            MMK_GetHandleByCtrlNode(ctrl_node_ptr),
            MMK_GetCtrlPtrByCtrlNode(ctrl_node_ptr),
            MMITHEME_GetCtrlName(MMK_GetGuidByCtrlNode(ctrl_node_ptr)),
            rect.left, rect.top, rect.right, rect.bottom);
}

/*****************************************************************************/
// 	Description : dump all ctrl in tree
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
LOCAL uint16 DumpAllCtrl(
                       MMI_TREE_NODE_T  *root_ctrl_node_ptr,
                       uint16           indent_index,
                       MMI_HANDLE_T     *form_arr_ptr,
                       uint16           form_arr_size
                       )
{
    uint16  form_count = 0;
    MMI_BASE_NODE_T *ctrl_node_ptr = PNULL;
    MMI_TREE_NODE_T *find_node_ptr = root_ctrl_node_ptr;    // 调用者保证root_ctrl_node_ptr有值

    do
    {
        ctrl_node_ptr = MMK_GetCtrlNodeByHandle(find_node_ptr->data);

        PrintCtrlNodeInfo(ctrl_node_ptr, indent_index);

        if (0 == indent_index
            && SPRD_GUI_FORM_ID == MMK_GetGuidByCtrlNode(ctrl_node_ptr)
            && form_count < form_arr_size)
        {
            form_arr_ptr[form_count] = find_node_ptr->data;
            form_count++;
        }

        if (PNULL != find_node_ptr->child_ptr)
        {
            DumpAllCtrl(find_node_ptr->child_ptr, indent_index + 1, form_arr_ptr, form_arr_size);
        }

        find_node_ptr = find_node_ptr->next_ptr;
        
    } while( find_node_ptr != root_ctrl_node_ptr );

    return form_count;
}

/*****************************************************************************/
// 	Description : dump window
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
LOCAL uint16 DumpWindow(
                        MMI_HANDLE_T    win_handle,
                        MMI_HANDLE_T    *form_arr_ptr,
                        uint16          form_arr_size
                        )
{
    uint16  form_count = 0;
    MMI_WINDOW_NODE_T       *win_node_ptr = PNULL;

    win_node_ptr = GetWindowNodePtr(win_handle);

    if (PNULL != win_node_ptr)
    {
        SCI_TRACE_LOW("==============   Dump window 0x%08x control tree   ==============", win_handle);
        SCI_TRACE_LOW("== style=0x%08x, active=0x%08x ==",
                win_node_ptr->win_ptr->window_style,
                win_node_ptr->active_control_ptr);

        if (PNULL != win_node_ptr->ctrl_tree_root_ptr)
        {
            form_count = DumpAllCtrl(win_node_ptr->ctrl_tree_root_ptr, 0, form_arr_ptr, form_arr_size);
        }

        SCI_TRACE_LOW("==============================================================\n\n");
    }

    return form_count;
}

/*****************************************************************************/
// 	Description : dump focus window
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMK_DumpCurrentWindow(
                                    MMI_HANDLE_T    *form_arr_ptr,
                                    uint16          form_arr_size
                                    )
{
    return DumpWindow(s_focus_win_handle, form_arr_ptr, form_arr_size);
}

/*****************************************************************************/
// 	Description : dump all window
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DumpAllWindows(void)
{
    MMI_ZORDER_NODE_T   *zorder_node_ptr = PNULL;

    if (PNULL != s_zorder_system.top_most_ptr && PNULL != s_zorder_system.top_most_ptr->win_node_ptr)
    {
        zorder_node_ptr = s_zorder_system.top_most_ptr;

        do
        {
            DumpWindow(zorder_node_ptr->win_node_ptr->base_node.handle, PNULL, 0);

            zorder_node_ptr = zorder_node_ptr->next_ptr;
        }
        while (PNULL != zorder_node_ptr 
            && PNULL != zorder_node_ptr->win_node_ptr
            && zorder_node_ptr != s_zorder_system.top_most_ptr);
    }
}

#endif

#define PRINT_NODE_LEN 0xFF
#define PRINT_UNKNOWN  "UNKNOWN"
/*****************************************************************************/
// 	Description : print base node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN PrintBaseNode( 
                            MMI_BASE_NODE_T* node_ptr,
                            uint32 condition1, 
                            uint32 condition2
                            ) 
{
    char         str_arr[PRINT_NODE_LEN+1] = {0};
    uint32       str_len = 0;
    const char*  start_str = PNULL;
    const char*  name_ptr = PNULL;
    uint32       id = 0;
    uint32       parent_tree_handle = 0;
    uint32       parent_handle = 0;

    if ( PNULL == node_ptr )
    {
        return FALSE;
    }

    if( MMI_HANDLE_WINDOW == node_ptr->handle_type )
    {
        MMI_WINDOW_NODE_T* win_node_ptr = (MMI_WINDOW_NODE_T*)node_ptr;

        parent_tree_handle = GetParentHandle(win_node_ptr->tree_node_ptr);
        parent_handle = MMK_GetAppletHandleByHandle( node_ptr->handle );
        id = win_node_ptr->win_id;
#ifdef MMI_WINIDNAME_SUPPORT		
        MMITHEME_GetWinIdNameArr( id, (uint8 **)&name_ptr );
#endif
        start_str = "MMI_WINDOW_NODE_T";
    }
    else if( MMI_HANDLE_CONTROL == node_ptr->handle_type )
    {
        parent_tree_handle = GetParentHandle(MMK_GetTreeNodeByCtrlNode(node_ptr));
        parent_handle = MMK_GetWinHandleByCtrl( node_ptr->handle );
        id = MMK_GetCtrlIdByCtrlNode(node_ptr);
        name_ptr = MMITHEME_GetCtrlName( MMK_GetGuidByCtrlNode(node_ptr) );
        start_str = "MMI_CTRL_NODE_T";
    }
    else if ( MMI_HANDLE_APPLET == node_ptr->handle_type )
    {
        const CAF_STATIC_APPLET_T* applet_ptr = PNULL;

        id = MMK_GetGuidByHandle( node_ptr->handle );

        if ( PNULL != ( applet_ptr = MMK_GetStaticAppletPtr( id ) ) )
        {
            name_ptr = applet_ptr->name;
        }
        start_str = "MMI_APPLET_NODE_T";
    }

    if ( PNULL == start_str )
    {
        start_str = PRINT_UNKNOWN;
    }

    if ( PNULL == name_ptr )
    {
        name_ptr = PRINT_UNKNOWN;
    }
    
    str_len = _snprintf( str_arr, PRINT_NODE_LEN, "(%s*)0x%08x,handle=0x%08x,parent_tree_handle=0x%08x,parent_handle=0x%08x,id=0x%08x,name=%s",
        start_str, (uint32)node_ptr, node_ptr->handle, parent_tree_handle, parent_handle, id, name_ptr);
    
    OS_DumpPrint( str_arr, str_len );

    return FALSE;
}


/*****************************************************************************/
// 	Description : print zorder system
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void PrintZorderSystem(void)
{
    MMI_ZORDER_NODE_T   *zorder_node_ptr = PNULL;

    if (PNULL != s_zorder_system.top_most_ptr 
        && PNULL != s_zorder_system.top_most_ptr->win_node_ptr)
    {
        zorder_node_ptr = s_zorder_system.top_most_ptr;

        do
        {
            PrintBaseNode( (MMI_BASE_NODE_T*)zorder_node_ptr->win_node_ptr, 0, 0 );
            zorder_node_ptr = zorder_node_ptr->prev_ptr;
        }
        while (PNULL != zorder_node_ptr 
            && PNULL != zorder_node_ptr->win_node_ptr
            && zorder_node_ptr != s_zorder_system.top_most_ptr);
    }
}

/*****************************************************************************/
// 	Description : print info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_PrintInfo(void)
{
    const char* print_str = PNULL;

    print_str = "s_handle_list";
    OS_DumpPrint( (char*)print_str, strlen(print_str) );
    MMK_FindInBaseNodeByCondition( PrintBaseNode, 0, 0, 0, TRUE );
    
    print_str = "s_zorder_system";
    OS_DumpPrint( (char*)print_str, strlen(print_str) );
    PrintZorderSystem();
}

PUBLIC BOOLEAN MMK_SetFocusWin(
                          MMI_HANDLE_T win_handle
                          )
{
    SetFocusWin(win_handle);
    return TRUE;
}

/* old interface !!!! may remove!!!! */
