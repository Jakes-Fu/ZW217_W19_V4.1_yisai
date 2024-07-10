/*****************************************************************************
** File Name:      mmk_ctrl.c                                                *
** Author:         hua.fang                                                  *
** Date:           07/21/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
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

#include "type_manager.h"
#include "ctrlbase.h"
#include "ctrlinfo_export.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

//control compare param
typedef struct
{
    GUI_POINT_T   point;
    MMI_HANDLE_T  exclude_handle;
    uint32        topmost_level;
} MMI_CTRL_COMPARE_PARAM_T;

typedef struct _ctrl_node_tag
{
    MMI_BASE_NODE_T             base_node;
    CTRLBASE_OBJ_T*             ctrl_ptr;

    //CAF_GUID_T                  guid;
    //MMI_HANDLE_T            	parent_win_handle;
    //MMI_CTRL_ID_T        		ctrl_id;
    //MMI_TREE_NODE_T*            tree_node_ptr;
} MMI_CTRL_NODE_T;

// 分组，控件横向坐标有交集的控件集合为一组
// 用于上下键时控件焦点切换
typedef struct
{
    MMI_LIST_T          group_node;     // 组的链表节点
    MMI_LIST_T          cell_header;    // 组里的控件集合头节点
    int16               top;            // 一组的顶
    int16               bottom;         // 一组的底
} MMK_GROUP_LIST_T;

// 组里的控件集合
typedef struct
{
    MMI_LIST_T          ctrl_node;      // 单元节点
    int16               left;           // 控件的左坐标
    MMI_CTRL_NODE_T*    ctrl_node_ptr;  // 控件节点
} MMK_CELL_LIST_T;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : get control rect
//    Global resource dependence :
//  Author: bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCtrlBothRect (
    MMI_HANDLE_T    ctrl_handle,
    GUI_BOTH_RECT_T     *both_rect_ptr    // [IN] point
);

/*****************************************************************************/
//     Description : set control rect
//    Global resource dependence :
//  Author: bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SetCtrlBothRect (
    MMI_HANDLE_T    ctrl_handle,
    GUI_BOTH_RECT_T     *both_rect_ptr    // [IN] point
);

/*****************************************************************************/
//     Description : set control lcd dev info
//    Global resource dependence :
//  Author: bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SetCtrlLcdDevInfo (
    MMI_HANDLE_T    ctrl_handle,
    GUI_LCD_DEV_INFO     *lcd_dev_info_ptr    // [IN] point
);

/*****************************************************************************/
// 	Description : get the ctrl node
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetCtrlNodePtr (
    MMI_HANDLE_T ctrl_handle
);

/*****************************************************************************/
// 	Description : create a dyanmic control (need control id and window id)
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL IGUICTRL_T* MMKCreateControl (
    MMI_BASE_NODE_T* win_node_ptr,
    MMI_TREE_NODE_T* parent_ctrl_ptr,
    const MMI_CONTROL_CREATE_T* create_ptr
);

/*****************************************************************************/
// 	Description : control node construct
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ControlTreeNodeConstruct (
    MMI_TREE_NODE_T* node_ptr,
    uint32 param1,
    uint32 param2
);

/*****************************************************************************/
// 	Description : control node destruct
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ControlTreeNodeDestruct (
    uint32 handle
);

/*****************************************************************************/
// 	Description : control node compare
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ControlTreeNodeCompare (
    uint32 handle,
    uint32 condition1,
    uint32 condition2
);

/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 ControlTreeNodeHandleEvent (
    uint32     handle,
    uint32     msg_id,
    void*      param
);

/*****************************************************************************/
// 	Description : get the window handle
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_TREE_NODE_T* GetCtrlTreeNode (
    MMI_HANDLE_T ctrl_handle
);

/*****************************************************************************/
//  Description : get current window progress bar rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetWinPrgRect (
    MMI_HANDLE_T    ctrl_handle,    //in:
    GUI_RECT_T      *prg_rect_ptr   //in/out
);

/*****************************************************************************/
// 	Description : is progress control
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPrgCtrl (
    uint32  handle,
    uint32  condition1,
    uint32  condition2
);

/*****************************************************************************/
// 	Description : whether the control can be set active state
//	Global resource dependence :
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCtrlEnableActive (
    MMI_CTRL_NODE_T* ctrl_node_ptr
);

/*****************************************************************************/
// 	Description : switch ctrl rect by screen
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void SwitchCtrlRect (
    MMI_CTRL_NODE_T* ctrl_node_ptr
);

/*****************************************************************************/
// 	Description : get the control pointer base on control_id from the appointed
//                window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetCtrlNodeByWinHandle (
    MMI_HANDLE_T win_handle,
    MMI_CTRL_ID_T ctrl_id,  // control id
    MMI_TREE_NODE_FIND_TYPE_E find_type
);

/*****************************************************************************/
// 	Description : get the active control id of the window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetActiveCtrlNode (
    MMI_HANDLE_T win_handle
);

/*****************************************************************************/
//  Description : add to group
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 分为两种情况:
// 1. 控件的区域与grouplist里的节点没有重叠，则新加一个
// 2. 控件区域与grouplist的节点重叠，则放到该节点中
/*****************************************************************************/
LOCAL void AddToGroup (
    MMK_GROUP_LIST_T      *header_group_ptr,              // group list header
    MMI_CTRL_NODE_T       *ctrl_node_ptr                  // ctrl node
);

/*****************************************************************************/
//  Description : insert a group node before the specified one
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddNewGroupNode (
    MMK_GROUP_LIST_T   *base_group_ptr,              // base group node
    MMI_CTRL_NODE_T    *ctrl_node_ptr,               // ctrl node ptr
    GUI_RECT_T         *ctrl_rect_ptr                // ctrl rect
);

/*****************************************************************************/
//  Description : add to existing group node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 当group与其他group重叠时，检查并合并重叠部分
/*****************************************************************************/
LOCAL void AddToExistingGroupNode (
    MMK_GROUP_LIST_T      *header_group_ptr,  // group list header
    MMK_GROUP_LIST_T      *base_group_ptr,    // the existing group node
    MMI_CTRL_NODE_T       *ctrl_node_ptr,     // ctrl node
    GUI_RECT_T            *ctrl_rect_ptr      // ctrl rect
);

/*****************************************************************************/
//  Description : check group node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 当group与其他group重叠时，检查并合并重叠部分
/*****************************************************************************/
LOCAL void CheckOverlapGroupNode (
    MMK_GROUP_LIST_T       *header_group_ptr,           // group header
    MMK_GROUP_LIST_T       *base_group_ptr              // group node
);

/*****************************************************************************/
//  Description : add a cell into cell list
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddToCellList (
    MMK_CELL_LIST_T        *header_cell_node_ptr,      // cell header
    MMK_CELL_LIST_T        *cell_node_ptr              // added cell node
);

/*****************************************************************************/
//  Description : create window group list
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 仅遍历窗口里第一层的控件
/*****************************************************************************/
LOCAL void CreateGroupList (
    MMI_BASE_NODE_T      *win_node_ptr,              // window node
    MMK_GROUP_LIST_T     *header_group_ptr           // group list header
);

/*****************************************************************************/
//  Description : destroy window group list
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DestroyGroupList (
    MMK_GROUP_LIST_T       *header_group_ptr        // group list header
);

/*****************************************************************************/
//  Description : destroy a group node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DestroyGroupNode (
    MMI_LIST_T *list_node_ptr                       // group node
);

/*****************************************************************************/
// 	Description : get ancestor ctrl handle
//	Global resource dependence :
//  Author: hua.fang
//	Note:
// 获得最顶层的父控件句柄
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetAncestorCtrlHandle (
    MMI_TREE_NODE_T* node_ptr          // ctrl node
);

/*****************************************************************************/
// 	Description : get active ctrl's group node and cell node
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL MMK_GROUP_LIST_T* GetActiveCtrlGroupInfo (
    MMI_BASE_NODE_T      *win_node_ptr,      // window node
    MMK_GROUP_LIST_T     *header_group_ptr,  // group list header
    MMK_CELL_LIST_T      **cell_node_pptr    // [out] cell node, can be null
);

/*****************************************************************************/
// 	Description : get active ctrl by up down
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetUpDownActiveCtrl (
    MMI_BASE_NODE_T      *win_node_ptr,      // window node
    MMK_GROUP_LIST_T     *header_group_ptr,  // group list header
    BOOLEAN              is_down             // find by down
);

/*****************************************************************************/
// 	Description : get active ctrl by left right
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetLeftRightActiveCtrl (
    MMI_BASE_NODE_T       *win_node_ptr,          // window node
    MMK_GROUP_LIST_T      *header_group_ptr,      // group list header
    BOOLEAN               is_right                // find by right
);

/*****************************************************************************/
// 	Description : get parent handle
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetParentHandle (
    MMI_TREE_NODE_T* node_ptr
);

/*****************************************************************************/
// 	Description : ctrl point compare
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CtrlPointCompare (
    uint32 handle,
    uint32 condition1,
    uint32 condition2
);

/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CompareIsEnanbleActive (
    uint32 handle,
    uint32 condition1,
    uint32 condition2
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get the window handle
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_TREE_NODE_T* GetCtrlTreeNode (
    MMI_HANDLE_T ctrl_handle
)
{
    MMI_TREE_NODE_T* tree_node_ptr = PNULL;
    MMI_CTRL_NODE_T* ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle);

    if (PNULL != ctrl_node_ptr)
    {
        tree_node_ptr = ctrl_node_ptr->ctrl_ptr->tree_node_ptr;
    }

    return tree_node_ptr;
}

/*****************************************************************************/
// 	Description : create a dyanmic control (need control id and window id)
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC IGUICTRL_T* MMK_CreateControl (
    const MMI_CONTROL_CREATE_T* create_ptr
)
{
    MMI_BASE_NODE_T*     win_node_ptr      = PNULL;
    MMI_HANDLE_T         parent_win_handle = 0;
    MMI_HANDLE_T         parent_ctrl_handle = 0;
    MMI_CONTROL_CREATE_T create = {0};

    if (PNULL == create_ptr)
    {
        //SCI_ASSERT( FALSE );
        return PNULL;/*lint !e527*/
    }

    parent_win_handle  = MMK_ConvertIdToHandle (create_ptr->parent_win_handle);
    parent_ctrl_handle = MMK_ConvertIdToHandle (create_ptr->parent_ctrl_handle);

    if (PNULL == (win_node_ptr = MMK_GetWinNodeByHandle (parent_win_handle)))
    {
        //SCI_ASSERT( FALSE );
        return PNULL; /*lint !e527*/
    }

    /* ctrl id == 0 mean inner control */
    if (0 != create_ptr->ctrl_id && MMK_GetCtrlPtrByWin (parent_win_handle, create_ptr->ctrl_id))
    {
        //SCI_ASSERT( FALSE );
        return PNULL; /*lint !e527*/
    }

    if (parent_ctrl_handle
            && !MMK_FindAllTreeNode (MMK_GetCtrlTreeByWinNode (win_node_ptr),
                                     ControlTreeNodeCompare, MMI_TREE_NODE_FIND_HANDLE, parent_ctrl_handle, MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD))
    {
        //parent ctrl error
        //SCI_ASSERT( FALSE );
        return PNULL; /*lint !e527*/
    }

    create = *create_ptr;
    create.parent_ctrl_handle = parent_ctrl_handle;
    create.parent_win_handle  = parent_win_handle;

    // 重构后，有些控件会被分为多个控件，但应用仍旧用guid+type来确定控件，这里
    // 为了兼容，将guid+type确定为最终新的guid，并且有可能转换初始化参数
    MMITHEME_CheckCtrlParam(&create);

    return MMKCreateControl (win_node_ptr, GetCtrlTreeNode (parent_ctrl_handle), &create);
}

/*****************************************************************************/
// 	Description : destroy a dyanmic control (need control id)
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyControl (
    MMI_HANDLE_T ctrl_handle
)
{
    return MMK_DestroyAppointControl (MMK_GetWinHandleByCtrl (ctrl_handle), ctrl_handle);
}

/*****************************************************************************/
// 	Description : destroy a dyanmic control (need control id)
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyDynaCtrl (
    MMI_CTRL_ID_T    ctrl_id     // the parent window id
)
{
    return MMK_DestroyControl (ctrl_id);

}

/*****************************************************************************/
// 	Description : destroy an appointed control (need control id and win handle)
//	Global resource dependence :
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyAppointControl (
    MMI_HANDLE_T win_handle,
    MMI_HANDLE_T ctrl_handle
)
{
    BOOLEAN            result       = FALSE;
    MMI_BASE_NODE_T*   win_node_ptr = PNULL;
    MMI_CTRL_NODE_T*   ctrl_node_ptr = PNULL;
    MMI_CTRL_NODE_T*   atv_ctrl_ptr = PNULL;
    MMI_TREE_NODE_T* ancestro_ptr = PNULL;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (PNULL != (win_node_ptr = MMK_GetWinNodeByHandle (win_handle)))
    {
        BOOLEAN is_ancestor = FALSE;

        ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle);
        atv_ctrl_ptr = MMK_GetActiveCtrlByWinNode (win_node_ptr);

        if (PNULL != atv_ctrl_ptr)
        {
            SCI_ASSERT (PNULL != ctrl_node_ptr); /*assert verified*/

            ancestro_ptr = MMK_GetAncestorTreeNode (atv_ctrl_ptr->ctrl_ptr->tree_node_ptr,
                                                    ctrl_node_ptr->ctrl_ptr->tree_node_ptr);

            //销毁的控件是active控件的祖先, 也需要重置active控件, 否则产生野指针
            if (PNULL != ancestro_ptr
                    && ctrl_node_ptr->ctrl_ptr->tree_node_ptr == ancestro_ptr)
            {
                is_ancestor = TRUE;
            }
        }

        if (atv_ctrl_ptr == ctrl_node_ptr || is_ancestor)
        {
            //销毁控件, 如果active控件为子控件, 则设置其父控件为active
            //否则嵌套控件如滚动条被销毁, 父控件将接受不到按键消息
            MMK_SetActiveCtrlByWinNode (
                win_node_ptr,
                GetCtrlNodePtr (MMK_GetParentCtrlHandle (ctrl_handle)));
        }

        if (ctrl_node_ptr != PNULL)
        {
            if (SPRD_GUI_STATUSBAR_ID == ctrl_node_ptr->ctrl_ptr->object.guid)
            {
                MMK_SetHasStb (win_node_ptr, FALSE);
            }
            else if (SPRD_GUI_TIPS_ID == ctrl_node_ptr->ctrl_ptr->object.guid)
            {
                MMK_SetHasTips (win_node_ptr, FALSE);
            }

            MMK_RemoveTreeNode (
                MMK_GetCtrlTreePtrByWinNode (win_node_ptr),
                ControlTreeNodeDestruct,
                ctrl_node_ptr->ctrl_ptr->tree_node_ptr);
        }

        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
// 	Description : destroy all control of window
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyAllControl (
    MMI_HANDLE_T win_handle
)
{
    BOOLEAN result = FALSE;
    MMI_BASE_NODE_T *win_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle (win_handle);

    if (PNULL != (win_node_ptr = MMK_GetWinNodeByHandle (win_handle)))
    {
        MMK_RemoveAllTreeNode (MMK_GetCtrlTreePtrByWinNode (win_node_ptr), ControlTreeNodeDestruct);

        MMK_SetActiveCtrlByWinNode (win_node_ptr, PNULL);
        MMK_SetHasStb (win_node_ptr, FALSE);
        MMK_SetHasTips (win_node_ptr, FALSE);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get the control pointer base on control_id from the processing
//                message window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC IGUICTRL_T* MMK_GetCtrlPtr (
    MMI_HANDLE_T ctrl_handle
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (NULL != (ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle)))
    {
        return (IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
// 	Description : get the control id base on control_id from the processing
//                message window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetCtrlId (
    MMI_HANDLE_T ctrl_handle
)

{
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (NULL != (ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle)))
    {
        return ctrl_node_ptr->ctrl_ptr->ctrl_id;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
// 	Description : get the window handle of the control
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandleByCtrl (
    MMI_HANDLE_T ctrl_handle
)
{
    MMI_HANDLE_T win_handle = 0;
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (PNULL != (ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle)))
    {
        win_handle = ctrl_node_ptr->ctrl_ptr->win_handle;
    }

    return win_handle;
}

/*****************************************************************************/
// 	Description : get the control pointer base on control_id from the appointed
//                window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC IGUICTRL_T* MMK_GetCtrlPtrByWin (
    MMI_HANDLE_T win_handle,
    MMI_CTRL_ID_T ctrl_id  // control id
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle (win_handle);

    if (PNULL != (ctrl_node_ptr = GetCtrlNodeByWinHandle (win_handle, ctrl_id , MMI_TREE_NODE_FIND_ID)))
    {
        return (IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
// 	Description : get the control pointer base on control_id from the appointed
//                window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetCtrlHandleByWin (
    MMI_HANDLE_T win_handle,
    MMI_CTRL_ID_T ctrl_id
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle (win_handle);

    if (PNULL != (ctrl_node_ptr = GetCtrlNodeByWinHandle (win_handle, ctrl_id, MMI_TREE_NODE_FIND_ID)))
    {
        return ctrl_node_ptr->base_node.handle;
    }
    else
    {
        return 0;
    }
}

/******************************************************************************/
/*! \brief get the parent ctrl handle
*  \author Great.Tian
*  \param[in]	 ctrl handle
*  \return the parent ctrl handle
*/
/******************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetParentCtrlHandle (
    MMI_HANDLE_T ctrl_handle
)
{
    MMI_HANDLE_T     parent_handle = 0;
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (PNULL != (ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle)))
    {
        parent_handle = GetParentHandle (ctrl_node_ptr->ctrl_ptr->tree_node_ptr);
    }

    return parent_handle;
}

/*****************************************************************************/
//  Description : 根据guid获得控件handle
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetCtrlHandleByGuid (
    MMI_HANDLE_T win_handle,
    CAF_GUID_T   guid
)
{
    MMI_HANDLE_T ctrl_handle = 0;
    MMI_BASE_NODE_T* win_node_ptr = PNULL;

    win_handle  = MMK_ConvertIdToHandle (win_handle);

    if (PNULL != (win_node_ptr = MMK_GetWinNodeByHandle (win_handle)))
    {
        ctrl_handle = MMK_FindAllTreeNode (
                          MMK_GetCtrlTreeByWinNode (win_node_ptr),
                          ControlTreeNodeCompare,
                          MMI_TREE_NODE_FIND_GUID, guid,
                          MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD);
    }

    return ctrl_handle;
}

/*****************************************************************************/
//  Description : get handle of control or window that is at the position of screen
//  Global resource dependence :
//  Author:gang.tong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetHandleByPos (
    GUI_POINT_T   point,
    BOOLEAN       is_find_parent,
    MMI_HANDLE_T* win_handle_ptr,
    MMI_HANDLE_T* ctrl_handle_ptr,
    MMI_HANDLE_T  exclude_handle //排除的ctrl handle, 如果找到, 需要继续找下一个
)
{
    BOOLEAN                 result             = FALSE;
    MMI_BASE_NODE_T*        win_node_ptr       = PNULL;
    MMI_CTRL_COMPARE_PARAM_T param        = {0};

    if (PNULL == win_handle_ptr)
    {
        return FALSE;
    }

    //SCI_ASSERT( PNULL != win_handle_ptr );

    *win_handle_ptr  = 0;

    param.exclude_handle = exclude_handle;
    param.point = point;
    param.topmost_level = GUICTRL_STATE_TOPMOST_MAX;

    if (PNULL != (win_node_ptr = MMK_GetTPPressWin (point, is_find_parent)))
    {
        *win_handle_ptr = MMK_GetWinHandleByWinNode (win_node_ptr);

        //need find ctrl
        if (PNULL != ctrl_handle_ptr)
        {
            MMI_CTRL_NODE_T* ctrl_node_ptr = MMK_GetActiveCtrlByWinNode (win_node_ptr);

            *ctrl_handle_ptr = 0;

            //优先派发给active topmost控件
            if (PNULL != ctrl_node_ptr && PNULL != ctrl_node_ptr->ctrl_ptr->tree_node_ptr)
            {
                //有子控件, 先派发给子控件
                if (PNULL != ctrl_node_ptr->ctrl_ptr->tree_node_ptr->child_ptr)
                {
                    for (param.topmost_level = GUICTRL_STATE_TOPMOST_MAX;
                            param.topmost_level > 0;
                            param.topmost_level -= GUICTRL_STATE_TOPMOST_STEP)
                    {
                        *ctrl_handle_ptr = MMK_FindAllTreeNode (
                                ctrl_node_ptr->ctrl_ptr->tree_node_ptr->child_ptr,
                                CtrlPointCompare, (uint32) & param, 0, MMI_TREE_TRAVEL_CHILD);

                        if (0 != *ctrl_handle_ptr)
                        {
                            break;
                        }
                    }
                }

                //再派发给自己
                if (0 == *ctrl_handle_ptr)
                {
                    for (param.topmost_level = GUICTRL_STATE_TOPMOST_MAX;
                            param.topmost_level > 0;
                            param.topmost_level -= GUICTRL_STATE_TOPMOST_STEP)
                    {
                        if (CtrlPointCompare (ctrl_node_ptr->base_node.handle, (uint32) &param, 0))
                        {
                            *ctrl_handle_ptr = ctrl_node_ptr->base_node.handle;
                            break;
                        }
                    }
                }
            }

            //need find other control
            if (0 == *ctrl_handle_ptr)
            {
                //优先派发给topmost控件
                for (param.topmost_level = GUICTRL_STATE_TOPMOST_MAX;
                        param.topmost_level > 0;
                        param.topmost_level -= GUICTRL_STATE_TOPMOST_STEP)
                {
                    *ctrl_handle_ptr = MMK_FindAllTreeNode (
                            MMK_GetCtrlTreeByWinNode (win_node_ptr),
                            CtrlPointCompare, (uint32) & param, 0, MMI_TREE_TRAVEL_CHILD);

                    if (0 != *ctrl_handle_ptr)
                    {
                        break;
                    }
                }

                //优先派发给topmost控件，尝试派发给父窗口上的topmost控件
                if (0 == *ctrl_handle_ptr)
                {
                    MMI_BASE_NODE_T* parent_node_ptr = MMK_GetWinNodeByHandle (
                                                           GetParentHandle (
                                                               MMK_GetWinTreeByWinNode (win_node_ptr)));

                    if (PNULL != parent_node_ptr)
                    {
                        for (param.topmost_level = GUICTRL_STATE_TOPMOST_MAX;
                                param.topmost_level > 0;
                                param.topmost_level -= GUICTRL_STATE_TOPMOST_STEP)
                        {
                            *ctrl_handle_ptr = MMK_FindAllTreeNode (
                                    MMK_GetCtrlTreeByWinNode (parent_node_ptr),
                                    CtrlPointCompare, (uint32) & param, 0, MMI_TREE_TRAVEL_CHILD);

                            if (0 != *ctrl_handle_ptr)
                            {
                                break;
                            }
                        }
                    }
                }
            }

            //再派发给active控件以及其他控件
            param.topmost_level = 0;

            //控件重叠, 先给active
            if (PNULL != ctrl_node_ptr)
            {
                if (0 == *ctrl_handle_ptr)
                {
                    //有子控件, 先派发给子控件
                    if (PNULL != ctrl_node_ptr->ctrl_ptr->tree_node_ptr
                        && PNULL != ctrl_node_ptr->ctrl_ptr->tree_node_ptr->child_ptr)
                    {
                        *ctrl_handle_ptr = MMK_FindAllTreeNode (
                                ctrl_node_ptr->ctrl_ptr->tree_node_ptr->child_ptr,
                                CtrlPointCompare, (uint32) & param, 0, MMI_TREE_TRAVEL_CHILD);
                    }

                }

                //再派发给自己
                if (0 == *ctrl_handle_ptr)
                {
                    if (CtrlPointCompare (ctrl_node_ptr->base_node.handle, (uint32) &param, 0))
                    {
                        *ctrl_handle_ptr = ctrl_node_ptr->base_node.handle;
                    }
                }
            }

            //need find other control
            if (0 == *ctrl_handle_ptr)
            {
                *ctrl_handle_ptr = MMK_FindAllTreeNode (
                        MMK_GetCtrlTreeByWinNode (win_node_ptr),
                        CtrlPointCompare, (uint32) & param, 0, MMI_TREE_TRAVEL_CHILD);
            }

            if( 0 == *ctrl_handle_ptr )
            {
                //idle下的子窗口处理比较特殊，不需要子窗口处理tp消息
                if(MMK_GetWinDisplayStyleState( *win_handle_ptr, WS_WIDGET_WIN ) )
                {
                    *win_handle_ptr = MMK_GetParentWinHandle( *win_handle_ptr);
                }
            }
        }
        else
        { 
            //idle下的子窗口处理比较特殊，不需要子窗口处理tp消息
            if(MMK_GetWinDisplayStyleState( *win_handle_ptr, WS_WIDGET_WIN ) )
            {
                *win_handle_ptr = MMK_GetParentWinHandle( *win_handle_ptr);
            }
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get handle of child control that is at the position of screen
//  Global resource dependence :
//  Author:Jassmine
//  Note: 根据位置查找某控件下的所有子控件
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetChildHandleByPos (
    GUI_POINT_T  point,              //in:
    MMI_HANDLE_T parent_handle,      //in:
    MMI_HANDLE_T *child_handle_ptr   //in/out:
)
{
    BOOLEAN                     result = FALSE;
    MMI_CTRL_NODE_T             *ctrl_node_ptr = PNULL;
    MMI_CTRL_COMPARE_PARAM_T    param = {0};

    if (PNULL != child_handle_ptr)
    {
        //reset child handle
        *child_handle_ptr = 0;

        //set param
        param.point = point;
        param.topmost_level = GUICTRL_STATE_TOPMOST_MAX;

        //get control node
        ctrl_node_ptr = GetCtrlNodePtr (parent_handle);

        if (PNULL != ctrl_node_ptr)
        {
            //派发给top most子控件
            for (param.topmost_level = GUICTRL_STATE_TOPMOST_MAX; param.topmost_level > 0; param.topmost_level -= GUICTRL_STATE_TOPMOST_STEP)
            {
                *child_handle_ptr = MMK_FindAllTreeNode (ctrl_node_ptr->ctrl_ptr->tree_node_ptr->child_ptr,
                                    CtrlPointCompare,
                                    (uint32) & param,
                                    0,
                                    MMI_TREE_TRAVEL_CHILD);

                if (0 != *child_handle_ptr)
                {
                    break;
                }
            }

            //set level
            param.topmost_level = 0;

            if (0 == *child_handle_ptr)
            {
                //派发给子控件
                *child_handle_ptr = MMK_FindAllTreeNode (ctrl_node_ptr->ctrl_ptr->tree_node_ptr->child_ptr,
                                    CtrlPointCompare,
                                    (uint32) & param,
                                    0,
                                    MMI_TREE_TRAVEL_CHILD);
            }

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get root form control handle from the appointed
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetRootFormCtrlHandle (
    MMI_HANDLE_T  win_handle
)
{
    MMI_HANDLE_T        ctrl_handle = 0;
    MMI_BASE_NODE_T     *win_node_ptr = PNULL;

    //convert win handle
    win_handle = MMK_ConvertIdToHandle (win_handle);

    //get window node
    win_node_ptr = MMK_GetWinNodeByHandle (win_handle);

    if (PNULL != win_node_ptr)
    {
        ctrl_handle = MMK_FindAllTreeNode (
                          MMK_GetCtrlTreeByWinNode (win_node_ptr),
                          ControlTreeNodeCompare,
                          MMI_TREE_NODE_FIND_GUID,
                          SPRD_GUI_FORM_ID,
                          MMI_TREE_TRAVEL_ORDER);
    }

    return (ctrl_handle);
}

/*****************************************************************************/
// 	Description : get ctrl handle by handle
//	Global resource dependence :
//  Author:James.Zhang
//	Note:根据当前应用下的窗口\控件\应用的handle获得指定控件的handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetCtrlHandleByHandle (
    MMI_HANDLE_T  src_handle, // 窗口\控件\应用的handle
    MMI_WIN_ID_T  win_id,     // win id
    MMI_CTRL_ID_T ctrl_id     // control id
)
{
    MMI_HANDLE_TYPE_E handle_type   = MMK_GetHandleType (src_handle);
    MMI_HANDLE_T      win_handle    = 0;
    MMI_HANDLE_T      applet_handle = 0;
    MMI_HANDLE_T      ctrl_handle   = 0;

    switch (handle_type)
    {
    case MMI_HANDLE_APPLET:

        if (0 != (win_handle = MMK_GetWinHandle (src_handle, win_id)))
        {
            ctrl_handle = MMK_GetCtrlHandleByWin (win_handle, ctrl_id);
        }

        break;

    case MMI_HANDLE_WINDOW:

        if (0 != (applet_handle = MMK_GetAppletHandleByWin (src_handle)))
        {
            if (0 != (win_handle = MMK_GetWinHandle (applet_handle, win_id)))
            {
                ctrl_handle = MMK_GetCtrlHandleByWin (win_handle, ctrl_id);
            }
        }

        break;

    case MMI_HANDLE_CONTROL:

        if (0 != (applet_handle = MMK_GetAppletHandleByWin (MMK_GetWinHandleByCtrl (src_handle))))
        {
            if (0 != (win_handle = MMK_GetWinHandle (applet_handle, win_id)))
            {
                ctrl_handle = MMK_GetCtrlHandleByWin (win_handle, ctrl_id);
            }
        }

        break;

    default:
        break;
    }

    return ctrl_handle;
}

/*****************************************************************************/
// 	Description : get win handle by handle
//	Global resource dependence :
//  Author:James.Zhang
//	Note:根据当前应用下的窗口\控件\应用的handle获得指定窗口的handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandleByHandle (
    MMI_HANDLE_T  src_handle, // 窗口\控件\应用的handle
    MMI_WIN_ID_T  win_id      // win id
)
{
    MMI_HANDLE_TYPE_E  handle_type   = MMK_GetHandleType (src_handle);
    MMI_HANDLE_T       win_handle    = 0;
    MMI_HANDLE_T       applet_handle = 0;

    //wind id为0, 只能使用handle
    if (0 != win_id)
    {
        switch (handle_type)
        {
        case MMI_HANDLE_APPLET:
            win_handle = MMK_GetWinHandle (applet_handle, win_id);
            break;

        case MMI_HANDLE_WINDOW:

            if (0 != (applet_handle = MMK_GetAppletHandleByWin (src_handle)))
            {
                win_handle = MMK_GetWinHandle (applet_handle, win_id);
            }

            break;

        case MMI_HANDLE_CONTROL:

            if (0 != (applet_handle = MMK_GetAppletHandleByWin (MMK_GetWinHandleByCtrl (src_handle))))
            {
                win_handle = MMK_GetWinHandle (applet_handle, win_id);
            }

            break;

        default:
            break;
        }
    }

    return win_handle;
}

/*****************************************************************************/
// 	Description : get applet handle by handle
//	Global resource dependence :
//  Author:James.Zhang
//	Note:根据当前应用下的窗口\控件\应用的handle获得applet的handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetAppletHandleByHandle (
    MMI_HANDLE_T  src_handle // 窗口\控件\应用的handle
)
{
    MMI_HANDLE_TYPE_E  handle_type   = MMK_GetHandleType (src_handle);
    MMI_HANDLE_T       applet_handle = 0;

    switch (handle_type)
    {
    case MMI_HANDLE_APPLET:
        applet_handle = src_handle;
        break;

    case MMI_HANDLE_WINDOW:
        applet_handle = MMK_GetAppletHandleByWin (src_handle);
        break;

    case MMI_HANDLE_CONTROL:
        applet_handle = MMK_GetAppletHandleByWin (MMK_GetWinHandleByCtrl (src_handle));
        break;

    default:
        break;
    }

    return applet_handle;
}

/*****************************************************************************/
// 	Description : set the active control
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetAtvCtrl (
    MMI_HANDLE_T  win_handle, // the parent window handle
    MMI_HANDLE_T  ctrl_handle // the control handle
)
{
    return MMK_SetAtvCtrlEx (win_handle, ctrl_handle, FALSE);
}

/*****************************************************************************/
// 	Description : set the active control
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetAtvCtrlEx (
    MMI_HANDLE_T  win_handle, // the parent window handle
    MMI_HANDLE_T  ctrl_handle, // the control handle
    BOOLEAN       is_need_paint //是否需要刷新
)
{
    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    return MMK_SetActiveCtrl (ctrl_handle, is_need_paint);
}

/*****************************************************************************/
// 	Description : set the active control
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetActiveCtrl (
    MMI_HANDLE_T  ctrl_handle,
    BOOLEAN       is_need_paint //是否需要刷新
)
{
    MMI_RESULT_E        result          = MMI_RESULT_TRUE;
    MMI_BASE_NODE_T     *win_node_ptr    = PNULL;
    MMI_CTRL_NODE_T     *atv_ctrl_ptr    = PNULL;
    MMI_CTRL_NODE_T     *prev_atv_ctrl_ptr    = PNULL;
    MMI_TREE_NODE_T     *ancestro_ptr    = PNULL;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    win_node_ptr = MMK_GetWinNodeByHandle (MMK_GetWinHandleByCtrl (ctrl_handle));
    atv_ctrl_ptr = GetCtrlNodePtr (ctrl_handle);

    if (PNULL != win_node_ptr
            && PNULL != atv_ctrl_ptr)
    {
        /* check the control whether it can be set active, some control status can not be set to active. */
        if (!IsCtrlEnableActive (atv_ctrl_ptr))
        {
            return FALSE;
        }

        prev_atv_ctrl_ptr = MMK_GetActiveCtrlByWinNode (win_node_ptr);

        if (prev_atv_ctrl_ptr != atv_ctrl_ptr)
        {
            //两个控件为祖先和子孙的关系
            if (PNULL != prev_atv_ctrl_ptr
                    && PNULL != (ancestro_ptr = MMK_GetAncestorTreeNode (prev_atv_ctrl_ptr->ctrl_ptr->tree_node_ptr, atv_ctrl_ptr->ctrl_ptr->tree_node_ptr)))
            {
                //设置的Active是儿子,则直接发送get active消息给儿子
                //设置的Active是祖先,则不处理
                if (ancestro_ptr != atv_ctrl_ptr->ctrl_ptr->tree_node_ptr)
                {
                    //设置的Active是儿子
                    MMK_SetActiveCtrlByWinNode (win_node_ptr, atv_ctrl_ptr);

                    if ( (MMK_GetWinHandleByWinNode (win_node_ptr) == MMK_GetProcMsgHandle()) ||
                            (0 != MMK_GetProcMsgHandle()))
                    {
                        result = MMK_RunCtrlProc (MMK_GetActiveCtrlByWinNode(win_node_ptr)->handle, MSG_CTL_GET_ACTIVE, PNULL);

                        if (is_need_paint)
                        {
                            MMK_RunCtrlProc (MMK_GetActiveCtrlByWinNode(win_node_ptr)->handle, MSG_CTL_PAINT, PNULL);
                        }
                    }
                }
            }
            //不为祖先控件, 需要发消息
            else
            {
                MMK_SetActiveCtrlByWinNode (win_node_ptr, atv_ctrl_ptr);

                if ( (MMK_GetWinHandleByWinNode (win_node_ptr) == MMK_GetProcMsgHandle()) ||
                        (0 != MMK_GetProcMsgHandle()))
                {
                    if (PNULL != prev_atv_ctrl_ptr)
                    {
                        MMK_RunCtrlProc (prev_atv_ctrl_ptr->base_node.handle, MSG_CTL_LOSE_ACTIVE, PNULL);

                        if (is_need_paint)
                        {
                            MMK_RunCtrlProc (prev_atv_ctrl_ptr->base_node.handle, MSG_CTL_PAINT, PNULL);
                        }
                    }
                }

                if ( (MMK_GetWinHandleByWinNode (win_node_ptr) == MMK_GetProcMsgHandle()) ||
                        (0 != MMK_GetProcMsgHandle()))
                {
                    result = MMK_RunCtrlProc (MMK_GetActiveCtrlByWinNode(win_node_ptr)->handle, MSG_CTL_GET_ACTIVE, PNULL);

                    if (is_need_paint)
                    {
                        MMK_RunCtrlProc (MMK_GetActiveCtrlByWinNode(win_node_ptr)->handle, MSG_CTL_PAINT, PNULL);
                    }
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMI:MMK_SetActiveCtrl, the control is active!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMK_WINDOW_3518_112_2_18_3_23_29_51, (uint8*) "");
            result = MMI_RESULT_FALSE;
        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    if (MMI_RESULT_TRUE == result)
    {
        return (TRUE);
    }

    return (FALSE);
}

/*****************************************************************************/
// 	Description : get the active control id of the window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetActiveCtrlId (
    MMI_HANDLE_T win_handle
)
{
    MMI_CTRL_ID_T       ctrl_id = MMITHEME_GetInvalidCtrlId();
    MMI_HANDLE_T        parent_handle = 0;
    MMI_CTRL_NODE_T     *ctrl_node_ptr = PNULL;
    MMI_CTRL_NODE_T     *parent_ctrl_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle (win_handle);

    if (PNULL != (ctrl_node_ptr = GetActiveCtrlNode (win_handle)))
    {
        if (0 == ctrl_node_ptr->ctrl_ptr->ctrl_id)
        {
            //get parent control
            parent_handle = GetParentHandle (ctrl_node_ptr->ctrl_ptr->tree_node_ptr);
            parent_ctrl_ptr = GetCtrlNodePtr (parent_handle);

            while (PNULL != parent_ctrl_ptr)
            {
                ctrl_id = parent_ctrl_ptr->ctrl_ptr->ctrl_id;

                if (0 != ctrl_id)
                {
                    break;
                }

                parent_ctrl_ptr = GetCtrlNodePtr (parent_ctrl_ptr->base_node.handle);
            }
        }
        else
        {
            ctrl_id = ctrl_node_ptr->ctrl_ptr->ctrl_id;
        }
    }

    return (ctrl_id);
}

/*****************************************************************************/
// 	Description : get the active control id of the window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetActiveCtrl (
    MMI_HANDLE_T win_handle
)
{
    MMI_HANDLE_T     ctrl_handle   = 0;
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle (win_handle);

    if (PNULL != (ctrl_node_ptr = GetActiveCtrlNode (win_handle)))
    {
        ctrl_handle = ctrl_node_ptr->base_node.handle;
    }

    return ctrl_handle;
}

/*****************************************************************************/
// 	Description : is active ctrl
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsActiveCtrl (
    MMI_HANDLE_T ctrl_handle
)
{
    BOOLEAN result = FALSE;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (0 != ctrl_handle
            && PNULL != GetCtrlNodePtr (ctrl_handle)
            && (GetCtrlNodePtr (ctrl_handle) == GetActiveCtrlNode (MMK_GetWinHandleByCtrl (ctrl_handle))))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : travel active ctrl
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_TravelActiveCtrl (
    MMI_HANDLE_T           win_handle,         // window handle
    MMK_TRAVEL_ACTIVE_E    travel_active       // travel enum
)
{
    MMI_BASE_NODE_T         *win_node_ptr = PNULL;
    MMK_GROUP_LIST_T        group_list_header = {0};
    MMK_GROUP_LIST_T        *group_node_ptr = PNULL;
    MMK_CELL_LIST_T         *cell_node_ptr = PNULL;
    MMI_CTRL_NODE_T         *next_active_ctrl_node_ptr = PNULL;

    win_handle = MMK_ConvertIdToHandle (win_handle);

    if (PNULL != (win_node_ptr = MMK_GetWinNodeByHandle (win_handle)))
    {
        MMILIST_Init (&group_list_header);

        // create group list
        CreateGroupList (win_node_ptr, &group_list_header);

        if (!MMILIST_IsEmpty (&group_list_header.group_node))
        {
            if (PNULL != MMK_GetActiveCtrlByWinNode (win_node_ptr))
            {
                switch (travel_active)
                {
                case MMK_TRAVEL_ACTIVE_UP:
                    next_active_ctrl_node_ptr = GetUpDownActiveCtrl (win_node_ptr, &group_list_header, FALSE);
                    break;

                case MMK_TRAVEL_ACTIVE_DOWN:
                    next_active_ctrl_node_ptr = GetUpDownActiveCtrl (win_node_ptr, &group_list_header, TRUE);
                    break;

                case MMK_TRAVEL_ACTIVE_LEFT:
                    next_active_ctrl_node_ptr = GetLeftRightActiveCtrl (win_node_ptr, &group_list_header, FALSE);
                    break;

                case MMK_TRAVEL_ACTIVE_RIGHT:
                default:
                    next_active_ctrl_node_ptr = GetLeftRightActiveCtrl (win_node_ptr, &group_list_header, TRUE);
                    break;
                }
            }
            else        // if haven't active ctrl, set the first control active
            {
                group_node_ptr = (MMK_GROUP_LIST_T*) group_list_header.group_node.next;
				if(group_node_ptr)
				{
					cell_node_ptr = (MMK_CELL_LIST_T*) group_node_ptr->cell_header.next;
				}
                if(cell_node_ptr)
				{
					next_active_ctrl_node_ptr = cell_node_ptr->ctrl_node_ptr;
				}
            }

            if (PNULL != next_active_ctrl_node_ptr)
            {
                MMK_SetActiveCtrl (next_active_ctrl_node_ptr->base_node.handle, TRUE);
            }
        }

        DestroyGroupList (&group_list_header);
    }
}

/*****************************************************************************/
// 	Description : check win active while ctrl is hiden or shown
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CheckActive (
    MMI_HANDLE_T ctrl_handle,
    BOOLEAN      is_hiden
)
{
    BOOLEAN             result          = FALSE;
    MMI_BASE_NODE_T     *win_node_ptr   = PNULL;
    MMI_CTRL_NODE_T     *ctrl_node_ptr  = PNULL;
    MMI_CTRL_NODE_T     *atv_ctrl_ptr   = PNULL;
    MMI_HANDLE_T        next_ctrl_handle     = 0;

    win_node_ptr = MMK_GetWinNodeByHandle (MMK_GetWinHandleByCtrl (ctrl_handle));

    if (PNULL != win_node_ptr)
    {
        atv_ctrl_ptr = MMK_GetActiveCtrlByWinNode (win_node_ptr);

        if (is_hiden)
        {
            // 如果此控件是当前窗口的焦点控件，取下一个可获取焦点的控件，
            // 如果没有，取消窗口的焦点控件
            if (PNULL != atv_ctrl_ptr)
            {
                if (ctrl_handle == atv_ctrl_ptr->base_node.handle
                        || MMK_IsAncestorCtrl (ctrl_handle, atv_ctrl_ptr->base_node.handle))
                {
                    // BUGBUG: 暂时以逆序方式来找下一个焦点控件
                    next_ctrl_handle = MMK_FindAllTreeNode (
                                           MMK_GetCtrlTreeByWinNode (win_node_ptr),
                                           CompareIsEnanbleActive,
                                           ctrl_handle,
                                           0,
                                           MMI_TREE_TRAVEL_CHILD);

                    if (0 != next_ctrl_handle)
                    {
                        if (MMK_IsAncestorCtrl (next_ctrl_handle, ctrl_handle))
                        {
                            // 如果是祖控件，因为MMK_SetActiveCtrl不会改值，
                            // 所以这里强行改变active_control_ptr
                            MMK_SetActiveCtrlByWinNode (
                                win_node_ptr,
                                GetCtrlNodePtr (next_ctrl_handle));
                        }
                        else
                        {
                            MMK_SetActiveCtrl (next_ctrl_handle, FALSE);
                        }
                    }
                    else
                    {
                        // 找不到可以获取焦点的控件，则去除焦点控件
                        MMK_RunCtrlProc (
                            atv_ctrl_ptr->base_node.handle,
                            MSG_CTL_LOSE_ACTIVE, PNULL);

                        MMK_SetActiveCtrlByWinNode (win_node_ptr, PNULL);
                    }
                }
            }
        }
        else
        {
            ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle);

            if (PNULL != ctrl_node_ptr)
            {
                // 如果当前窗口没有焦点控件，且此控件可以获取焦点的话，激活之
                if (PNULL == atv_ctrl_ptr
                        && IsCtrlEnableActive (ctrl_node_ptr))      // BUGBUG: 还需判断父亲是否可以获取焦点
                {
                    MMK_SetActiveCtrl (ctrl_node_ptr->base_node.handle, FALSE);
                }
            }
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : run the control process
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMK_RunCtrlProc (
    MMI_HANDLE_T     ctrl_handle,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_CTRL_NODE_T*     ctrl_node_ptr  = GetCtrlNodePtr (ctrl_handle);
    MMI_RESULT_E         result         = MMI_RESULT_FALSE;

    if (PNULL != ctrl_node_ptr)
    {
        if (MSG_CTL_PAINT != msg_id || !IGUICTRL_GetState ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, GUICTRL_STATE_INVISIBLE))
        {
#ifdef JAMES_TEST
            uint32 tick = SCI_GetTickCount();

            //SCI_TRACE_LOW:"james test, MMK_RunCtrlProc start = %d, guid = %d, msg_id = 0x%08x"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMK_WINDOW_3323_112_2_18_3_23_29_48, (uint8*) "ddd", tick, ctrl_node_ptr->ctrl_ptr->object.guid, msg_id);
#endif
#ifdef UI_MULTILAYER_SUPPORT

            if (UILAYER_IsMultiLayerEnable())
            {
                switch (msg_id)
                {
                case MSG_CTL_OPEN:

                    if (MMK_IsFocusWin (MMK_GetWinHandleByCtrl (ctrl_handle)))
                    {
                        UILAYER_ActiveAllLayerOfHandle (ctrl_handle);
                    }

                    break;

                case MSG_CTL_LOSE_FOCUS:
                    UILAYER_DeactiveAllLayerOfHandle (ctrl_handle);
                    break;

                case MSG_CTL_GET_FOCUS:
                    UILAYER_ActiveAllLayerOfHandle (ctrl_handle);
                    break;

                case MSG_CTL_CLOSE:
                    UILAYER_ReleaseAllLayerOfHandle (ctrl_handle);
                    break;

                default:
                    break;
                }
            }

#endif

            if (MSG_LCD_SWITCH == msg_id)
            {
                SwitchCtrlRect (ctrl_node_ptr);
            }
            //tp down去掉滑动状态
            else if (MSG_TP_PRESS_DOWN == msg_id)
            {
                GUICTRL_SetState ( (IGUICTRL_T*) ctrl_node_ptr->ctrl_ptr, GUICTRL_STATE_SLIDE, FALSE);
            }

            result = IGUICTRL_HandleEvent ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, msg_id, param);

#ifdef JAMES_TEST
            tick = SCI_GetTickCount() - tick;

            //SCI_TRACE_LOW:"james ticks, MMK_RunCtrlProc end ticks = %d, guid = %d, msg_id = 0x%08x"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMK_WINDOW_3363_112_2_18_3_23_29_49, (uint8*) "ddd", tick, ctrl_node_ptr->ctrl_ptr->object.guid, msg_id);
#endif
        }
        //规避子控件隐藏时，父控件发送ctrl paint消息，递归爆栈的问题。
        else
        {
            result = MMI_RESULT_TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMI: MMK_RunCtrlProc! the hwnd is not control!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMK_WINDOW_3369_112_2_18_3_23_29_50, (uint8*) "");
    }

    return result;
}

/*****************************************************************************/
//     Description : Whether the control has response to  the pressing the touch pannel.
//    Global resource dependence :
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsResponseToClick (
    MMI_HANDLE_T   ctl_handle
)
{
    BOOLEAN          result        = FALSE;
    MMI_CTRL_NODE_T* ctrl_node_ptr = GetCtrlNodePtr (ctl_handle);

    if ( (PNULL != ctrl_node_ptr) &&
            (!IGUICTRL_GetState ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, GUICTRL_STATE_INVISIBLE)))
    {
        /* not disable can response */
        result = (BOOLEAN) (!IGUICTRL_GetState ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, GUICTRL_STATE_DISABLE_TP));
    }

    return result;
}

/*****************************************************************************/
//  Description : the ancestor_handle is ctrl_handle's ancestor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsAncestorCtrl (
    MMI_HANDLE_T      ancestor_handle,
    MMI_HANDLE_T      ctrl_handle
)
{
    BOOLEAN             result = FALSE;
    MMI_CTRL_NODE_T     *ancestor_ctrl_ptr = PNULL;
    MMI_CTRL_NODE_T     *ctrl_ptr = PNULL;
    MMI_TREE_NODE_T     *ancestro_ptr = PNULL;

    //get control node
    ancestor_ctrl_ptr = GetCtrlNodePtr (ancestor_handle);
    ctrl_ptr = GetCtrlNodePtr (ctrl_handle);

    if ( (PNULL != ancestor_ctrl_ptr) &&
            (PNULL != ctrl_ptr))
    {
        ancestro_ptr = MMK_GetAncestorTreeNode (ancestor_ctrl_ptr->ctrl_ptr->tree_node_ptr, ctrl_ptr->ctrl_ptr->tree_node_ptr);

        if ( (PNULL != ancestro_ptr) &&
                (ancestro_ptr == ancestor_ctrl_ptr->ctrl_ptr->tree_node_ptr))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//     Description : whether the point is in the control ctl_ptr!
//    Global resource dependence :
//  Author: Robert.Lu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_PosIsCtrl (
    MMI_HANDLE_T    ctrl_handle,
    GUI_POINT_T     point    // [IN] point
)
{
    BOOLEAN         result = FALSE;
    CAF_RECT_T      rect = {0};
    CAF_RECT_T      prg_rect = {0};
    MMI_CTRL_NODE_T *ctrl_node_ptr = PNULL;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (PNULL != (ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle)))
    {
        if (!IGUICTRL_GetDisplayRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, &rect))
        {
            IGUICTRL_GetRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, &rect);
        }

        if (GUI_PointIsInRect (point, rect))
        {
            result = TRUE;

            //if im,get progress bar rect,is tp progress bar
            if ( (SPRD_GUI_IM_ID == IGUICTRL_GetCtrlGuid ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr)) &&
                    (GetWinPrgRect (ctrl_handle, &prg_rect)) &&
                    (GUI_PointIsInRect (point, prg_rect)))
            {
                result = FALSE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : 设置控件的block信息
//	Global resource dependence :
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetCtrlBlockInfo (
    MMI_HANDLE_T win_handle,
    MMI_CTRL_ID_T ctrl_id,		// 控件的ID
    GUI_LCD_DEV_INFO dev_info)
{

    MMI_BASE_NODE_T*  win_node_ptr = PNULL;
    MMI_CTRL_NODE_T*  ctrl_node_ptr = PNULL;

    MMITHEME_CheckLcdBlockId (dev_info.lcd_id, dev_info.block_id);


    win_handle = MMK_ConvertIdToHandle (win_handle);

    /* get window pointer */
    win_node_ptr = MMK_GetWinNodeByHandle (win_handle);

    if (PNULL == win_node_ptr)
    {
        return FALSE;
    }

    /* get control pointer */
    ctrl_node_ptr = GetCtrlNodeByWinHandle (win_handle, ctrl_id, MMI_TREE_NODE_FIND_ID);

    if (PNULL == ctrl_node_ptr)
    {
        return FALSE;
    }

    /* make sure the type of id is control */

    //SCI_ASSERT(MMI_HANDLE_CONTROL == ctrl_node_ptr->base_node.handle_type);
    if (MMI_HANDLE_CONTROL != ctrl_node_ptr->base_node.handle_type)
    {
        return FALSE;
    }

    IGUICTRL_SetLcdDevInfo ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, &dev_info);
    //GUI_SetCtrlLcdDevInfo(ctrl_node_ptr->base_node.handle, dev_info);

    return TRUE;
}

/*****************************************************************************/
//     Description : set control lcd dev info
//    Global resource dependence :
//  Author: bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetCtrlLcdDevInfo (
    MMI_HANDLE_T    ctrl_handle,
    GUI_LCD_DEV_INFO     *lcd_dev_info_ptr    // [IN] point
)
{
    return SetCtrlLcdDevInfo (ctrl_handle, lcd_dev_info_ptr);
}

/*****************************************************************************/
//     Description : set window lcd dev info and notify ctrl
//    Global resource dependence :
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinLcdDevInfoNotifyCtrl (
    MMI_HANDLE_T win_handle,
    const GUI_LCD_DEV_INFO *dev_info_ptr  //in
)
{
    MMI_BASE_NODE_T *win_node_ptr = PNULL;
    MMI_TREE_NODE_T *node_ptr = PNULL;
//    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    BOOLEAN result = FALSE;

    if (PNULL == dev_info_ptr)
    {
        return result;
    }

    win_handle  = MMK_ConvertIdToHandle (win_handle);

    if (PNULL != (win_node_ptr = MMK_GetWinNodeByHandle (win_handle)))
    {
        MMK_SetWinLcdDevInfo (win_handle, dev_info_ptr);

        //notify ctrl
        node_ptr = MMK_GetCtrlTreeByWinNode (win_node_ptr);

        while (PNULL != node_ptr)
        {
            SetCtrlLcdDevInfo (node_ptr->data, (GUI_LCD_DEV_INFO *) dev_info_ptr);
            node_ptr = node_ptr->next_ptr;

            if (node_ptr == MMK_GetCtrlTreeByWinNode (win_node_ptr))
            {
                break;
            }
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//     Description : set window both rect and notify ctrl
//    Global resource dependence :
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetBothRectNotifyCtrl (
    MMI_HANDLE_T           handle,
    const GUI_BOTH_RECT_T* both_rect_ptr
)
{
    BOOLEAN             result = FALSE;
    MMI_BASE_NODE_T     *win_node_ptr = PNULL;
    GUI_POINT_T         h_point_offset = {0};
    GUI_POINT_T         v_point_offset = {0};
    GUI_BOTH_RECT_T     old_both_rect = {0};
    GUI_BOTH_RECT_T     ctrl_both_rect = {0};
    MMI_TREE_NODE_T     *node_ptr = PNULL;

    if (PNULL == both_rect_ptr)
    {
        return FALSE;
    }

//    SCI_ASSERT( PNULL != both_rect_ptr );

    handle = MMK_ConvertIdToHandle (handle);

    if (PNULL != (win_node_ptr = MMK_GetWinNodeByHandle (handle)))
    {
        MMK_GetWinBothRect (handle, &old_both_rect);
        MMK_SetWinBothRect (handle, both_rect_ptr);

        h_point_offset.x = (both_rect_ptr->h_rect.left - old_both_rect.h_rect.left);
        h_point_offset.y = (both_rect_ptr->h_rect.top - old_both_rect.h_rect.top);
        v_point_offset.x = (both_rect_ptr->v_rect.left - old_both_rect.v_rect.left);
        v_point_offset.y = (both_rect_ptr->v_rect.top - old_both_rect.v_rect.top);
        //notify ctrl
        node_ptr = MMK_GetCtrlTreeByWinNode (win_node_ptr);

        while (PNULL != node_ptr)
        {
            //change rect
            GetCtrlBothRect (node_ptr->data, &ctrl_both_rect);
            ctrl_both_rect.h_rect.left += h_point_offset.x;
            ctrl_both_rect.h_rect.top += h_point_offset.y;
            ctrl_both_rect.h_rect.right += h_point_offset.x;
            ctrl_both_rect.h_rect.bottom += h_point_offset.y;

            ctrl_both_rect.v_rect.left += v_point_offset.x;
            ctrl_both_rect.v_rect.top += v_point_offset.y;
            ctrl_both_rect.v_rect.right += v_point_offset.x;
            ctrl_both_rect.v_rect.bottom += v_point_offset.y;
            SetCtrlBothRect (node_ptr->data, &ctrl_both_rect);

            node_ptr = node_ptr->next_ptr;

            if (node_ptr == MMK_GetCtrlTreeByWinNode (win_node_ptr))
            {
                break;
            }
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//     Description : get both rect
//    Global resource dependence :
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetBothRect (
    MMI_HANDLE_T           handle,
    const GUI_BOTH_RECT_T* both_rect_ptr
)
{
    return MMK_SetWinBothRect (handle, both_rect_ptr);
}

/*****************************************************************************/
//     Description : get both rect
//    Global resource dependence :
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetBothRect (
    MMI_HANDLE_T     handle,
    GUI_BOTH_RECT_T* both_rect_ptr
)
{
    BOOLEAN            result       = FALSE;
    MMI_HANDLE_TYPE_E  type         = MMI_HANDLE_NONE;
    MMI_CTRL_NODE_T*   ctrl_node_ptr = PNULL;

    if (PNULL == both_rect_ptr)
    {
        return FALSE;
    }

    handle = MMK_ConvertIdToHandle (handle);
    type   = MMK_GetHandleType (handle);

    switch (type)
    {
    case MMI_HANDLE_WINDOW:
        result = MMK_GetWinBothRect (handle, both_rect_ptr);
        break;

    case MMI_HANDLE_CONTROL:

        if (PNULL != (ctrl_node_ptr = GetCtrlNodePtr (handle)))
        {
            IGUICTRL_GetBothRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, both_rect_ptr);

            result = TRUE;
        }

        break;

    default:
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : draw all child ctrl
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DrawAllChildCtrl (
    MMI_HANDLE_T ctrl_handle
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;
    BOOLEAN result = FALSE;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (PNULL != (ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle)))
    {
        MMK_DispatchToAllTreeNode (ctrl_node_ptr->ctrl_ptr->tree_node_ptr->child_ptr,
                                   ControlTreeNodeHandleEvent, MSG_CTL_PAINT, PNULL, MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get handle by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetHandleByCtrlNode (
    MMI_BASE_NODE_T *node_ptr
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = (MMI_CTRL_NODE_T*) node_ptr;
    return ctrl_node_ptr->base_node.handle;
}

/*****************************************************************************/
//  Description : get guid by ctrl handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CAF_GUID_T MMK_GetGuidByCtrlNode (
    MMI_BASE_NODE_T *node_ptr
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = (MMI_CTRL_NODE_T*) node_ptr;
    return ctrl_node_ptr->ctrl_ptr->object.guid;
}

/*****************************************************************************/
//  Description : get ctrl ptr by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC IGUICTRL_T* MMK_GetCtrlPtrByCtrlNode (
    MMI_BASE_NODE_T *node_ptr
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = (MMI_CTRL_NODE_T*) node_ptr;
    return (IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr;
}

/*****************************************************************************/
//  Description : get window handle by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandleByCtrlNode (
    MMI_BASE_NODE_T *node_ptr
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = (MMI_CTRL_NODE_T*) node_ptr;
    return ctrl_node_ptr->ctrl_ptr->win_handle;
}

/*****************************************************************************/
//  Description : get ctrl id by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetCtrlIdByCtrlNode (
    MMI_BASE_NODE_T *node_ptr
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = (MMI_CTRL_NODE_T*) node_ptr;
    return ctrl_node_ptr->ctrl_ptr->ctrl_id;
}

/*****************************************************************************/
//  Description : get tree node by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_GetTreeNodeByCtrlNode (
    MMI_BASE_NODE_T *node_ptr
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = (MMI_CTRL_NODE_T*) node_ptr;
    return ctrl_node_ptr->ctrl_ptr->tree_node_ptr;
}

/*****************************************************************************/
//  Description : get tree node by ctrl handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_GetCtrlNodeByHandle (
    MMI_HANDLE_T       ctrl_handle
)
{
    MMI_CTRL_NODE_T   *ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle);
    return (MMI_BASE_NODE_T*) ctrl_node_ptr;
}

/*****************************************************************************/
//  Description : destroy all controls
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMK_DestroyAllControlByNode (
    MMI_BASE_NODE_T* node_ptr
)
{
    MMK_RemoveAllTreeNode (MMK_GetCtrlTreePtrByWinNode (node_ptr), ControlTreeNodeDestruct);
}

/*****************************************************************************/
//  Description : dispatch to all controls
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchToAllControl (
    MMI_BASE_NODE_T* node_ptr,
    uint32  msg_id,
    void*   param,
    MMI_TREE_TRAVEL_STATE_T state
)
{
    MMK_DispatchToAllTreeNode (
        MMK_GetCtrlTreeByWinNode (node_ptr),
        ControlTreeNodeHandleEvent, msg_id, param, state);
}

/*****************************************************************************/
// 	Description : get the active control id of the window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetActiveCtrlNode (
    MMI_HANDLE_T win_handle
)
{
    MMI_CTRL_NODE_T*    ctrl_node_ptr = PNULL;
    MMI_BASE_NODE_T*    win_node_ptr  = PNULL;

    if (PNULL != (win_node_ptr = MMK_GetWinNodeByHandle (win_handle))
            && MMK_IsOpenWin (win_handle))
    {
        ctrl_node_ptr = MMK_GetActiveCtrlByWinNode (win_node_ptr);
    }

    return ctrl_node_ptr;
}

/*****************************************************************************/
//     Description : set control lcd dev info
//    Global resource dependence :
//  Author: bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SetCtrlLcdDevInfo (
    MMI_HANDLE_T    ctrl_handle,
    GUI_LCD_DEV_INFO     *lcd_dev_info_ptr    // [IN] point
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;
    BOOLEAN result = FALSE;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (PNULL != (ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle)))
    {
        IGUICTRL_SetLcdDevInfo ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, lcd_dev_info_ptr);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get current window progress bar rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetWinPrgRect (
    MMI_HANDLE_T    ctrl_handle,    //in:
    GUI_RECT_T      *prg_rect_ptr   //in/out
)
{
    BOOLEAN             result = FALSE;
    MMI_HANDLE_T        win_handle = 0;
    MMI_HANDLE_T        prg_handle = 0;
    MMI_CTRL_NODE_T     *ctrl_node_ptr = PNULL;
    MMI_BASE_NODE_T     *win_node_ptr = PNULL;

    //get window handle
    win_handle = MMK_GetWinHandleByCtrl (ctrl_handle);

    //get win ptr
    win_node_ptr = MMK_GetWinNodeByHandle (win_handle);
    SCI_ASSERT (PNULL != win_node_ptr); /*assert verified*/

    //find all child is exist progress bar
    prg_handle = MMK_FindAllTreeNode (
                     MMK_GetCtrlTreeByWinNode (win_node_ptr),
                     IsPrgCtrl, 0, 0, MMI_TREE_TRAVEL_CHILD);

    if (0 != prg_handle)
    {
        //get control pointer
        ctrl_node_ptr = GetCtrlNodePtr (prg_handle);

        if (PNULL != ctrl_node_ptr)
        {		
			if (!IGUICTRL_GetDisplayRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, prg_rect_ptr))
			{
				IGUICTRL_GetRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, prg_rect_ptr);
			}
			result = TRUE;
        }        
    }

    return (result);
}

/*****************************************************************************/
// 	Description : is progress control
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPrgCtrl (
    uint32  handle,
    uint32  condition1,
    uint32  condition2
)
{
    BOOLEAN         result = FALSE;
    MMI_CTRL_NODE_T *ctrl_node_ptr = PNULL;

    //get parent control pointer
    ctrl_node_ptr = GetCtrlNodePtr (handle);

    if (PNULL != ctrl_node_ptr)
    {	
		if (SPRD_GUI_PRGBOX_ID == IGUICTRL_GetCtrlGuid ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr))
		{
			result = TRUE;
		}
    }
    return (result);
}

/*****************************************************************************/
// 	Description : get the ctrl node
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetCtrlNodePtr (
    MMI_HANDLE_T ctrl_handle
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;
    MMI_BASE_NODE_T* base_node_ptr = MMK_FindBaseNode (ctrl_handle);

    if (PNULL != base_node_ptr && MMI_HANDLE_CONTROL == base_node_ptr->handle_type)
    {
        ctrl_node_ptr = (MMI_CTRL_NODE_T*) base_node_ptr;

        SCI_ASSERT (PNULL != ctrl_node_ptr->ctrl_ptr); /*assert verified*/
    }

    return ctrl_node_ptr;
}

/*****************************************************************************/
// 	Description : create a dyanmic control (need control id and window id)
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL IGUICTRL_T* MMKCreateControl (
    MMI_BASE_NODE_T* win_node_ptr,
    MMI_TREE_NODE_T* parent_ctrl_ptr,
    const MMI_CONTROL_CREATE_T* create_ptr
)
{
    MMI_TREE_NODE_T* new_ctrl_ptr  = PNULL;
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;
    IGUICTRL_T* ctrl_ptr = PNULL;

    new_ctrl_ptr = MMK_AddTreeNode(
            ControlTreeNodeConstruct,
            0, (uint32) create_ptr,
            MMK_GetCtrlTreePtrByWinNode(win_node_ptr),
            parent_ctrl_ptr);

    if (PNULL != new_ctrl_ptr)
    {
        ctrl_node_ptr = GetCtrlNodePtr (new_ctrl_ptr->data);

        SCI_ASSERT (PNULL != ctrl_node_ptr); /*assert verified*/

        IGUICTRL_Construct ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, create_ptr->init_data_ptr);

        //初始化rect
        IGUICTRL_SetState ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, create_ptr->state, TRUE);

        SwitchCtrlRect (ctrl_node_ptr);

        /* make sure focus applet and parent control */
        if (MMK_IsFocusWin (MMK_GetWinHandleByWinNode (win_node_ptr)) /*&& PNULL == parent_ctrl_ptr*/)   //support inner object
        {
            MMI_MESSAGE_PTR_T mmi_msg_ptr = MMK_GetCurQueueMsg();

            if (! (MMK_GetWinHandleByWinNode (win_node_ptr) == mmi_msg_ptr->handle
                    && MSG_OPEN_WINDOW == mmi_msg_ptr->msg_id))
            {
                MMK_RunCtrlProc (ctrl_node_ptr->base_node.handle, MSG_CTL_OPEN, PNULL);
            }
        }

        //should get state
        if (ctrl_node_ptr->ctrl_ptr->object.guid == SPRD_GUI_STATUSBAR_ID)
        {
            MMK_SetHasStb (win_node_ptr, TRUE);
        }
        else if (ctrl_node_ptr->ctrl_ptr->object.guid == SPRD_GUI_TIPS_ID)
        {
            MMK_SetHasTips (win_node_ptr, TRUE);
        }

        ctrl_ptr = (IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr;
    }
    else
    {
        SCI_ASSERT (FALSE); /*assert verified*/
    }

    return ctrl_ptr;
}

/*****************************************************************************/
// 	Description : switch ctrl rect by screen
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void SwitchCtrlRect (
    MMI_CTRL_NODE_T* ctrl_node_ptr
)
{
    GUI_BOTH_RECT_T both_rect  = {0};
    GUI_RECT_T      rect       = {0};

    SCI_ASSERT (PNULL != ctrl_node_ptr); /*assert verified*/

    IGUICTRL_GetBothRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, &both_rect);

    rect = MMK_IsWindowLandscape (MMK_GetWinHandleByCtrl (ctrl_node_ptr->base_node.handle)) ? both_rect.h_rect : both_rect.v_rect;

    IGUICTRL_SetRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, &rect);
}

/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ControlTreeNodeConstruct (
    MMI_TREE_NODE_T* node_ptr,
    uint32 param1,
    uint32 param2
)
{
    BOOLEAN                     result        = FALSE;
    const MMI_CONTROL_CREATE_T* create_ptr    = (const MMI_CONTROL_CREATE_T*) param2;
    MMI_BASE_NODE_T*            win_node_ptr  = MMK_GetWinNodeByHandle (create_ptr->parent_win_handle);
    MMI_CTRL_NODE_T*            ctrl_node_ptr = PNULL;
    void* object_ptr = PNULL;

    SCI_ASSERT (PNULL != create_ptr); /*assert verified*/
    SCI_ASSERT (PNULL != win_node_ptr); /*assert verified*/

    if (CAF_RESULT_SUCCEEDED == MMKMNG_CreateInstance (create_ptr->guid, &object_ptr))
    {
        ctrl_node_ptr = (MMI_CTRL_NODE_T *) MMK_CreateBaseNode (MMI_HANDLE_CONTROL, sizeof (MMI_CTRL_NODE_T));
        SCI_ASSERT (PNULL != ctrl_node_ptr); /*assert verified*/

        ctrl_node_ptr->ctrl_ptr		     = (CTRLBASE_OBJ_T*)object_ptr;

        ctrl_node_ptr->ctrl_ptr->win_handle = create_ptr->parent_win_handle;
        ctrl_node_ptr->ctrl_ptr->ctrl_id           = create_ptr->ctrl_id;
        //ctrl_node_ptr->guid              = create_ptr->guid;
        ctrl_node_ptr->ctrl_ptr->tree_node_ptr     = node_ptr;

        node_ptr->data                   = ctrl_node_ptr->base_node.handle;

        GUICTRL_SetCtrlHandle ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, ctrl_node_ptr->base_node.handle);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : control node destruct
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ControlTreeNodeDestruct (
    uint32 handle
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = GetCtrlNodePtr (handle);
    MMI_TREE_NODE_T* tree_node_ptr = PNULL;

    if (PNULL == ctrl_node_ptr)
    {
        return FALSE;
    }

    // 由于tree_node记录在ctrl_ptr里，它会在release时释放，这样再去free时就会出错，
    // 现在将它存为临时变量，释放完控件后再释放它
    tree_node_ptr = ctrl_node_ptr->ctrl_ptr->tree_node_ptr;

    MMK_RunCtrlProc (ctrl_node_ptr->base_node.handle, MSG_CTL_CLOSE, PNULL);

    MMK_DeleteMSGByHandle (ctrl_node_ptr->base_node.handle);
    MMK_DeleteTimerByHandle (ctrl_node_ptr->base_node.handle);
    MMK_CloseGsensor (ctrl_node_ptr->base_node.handle);

    if (0 != IGUICTRL_Release ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr))
    {
        SCI_ASSERT (FALSE); /*assert verified*/
    }
    else
    {
        ctrl_node_ptr->ctrl_ptr = PNULL;
    }

    SCI_FREE (tree_node_ptr);

    MMI_FreeRes (ctrl_node_ptr->base_node.handle);
    MMK_DestroyBaseNode ( (MMI_BASE_NODE_T*) ctrl_node_ptr);

    return TRUE;
}

/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL uint32 ControlTreeNodeHandleEvent (
    uint32     handle,
    uint32     msg_id,
    void*      param
)
{
    return MMK_RunCtrlProc (handle, msg_id, param);
}

/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ControlTreeNodeCompare (
    uint32 handle,
    uint32 condition1,
    uint32 condition2
)
{
    BOOLEAN result = FALSE;
    MMI_CTRL_NODE_T*   ctrl_node_ptr = PNULL;

    SCI_ASSERT (condition1 < MMI_TREE_NODE_FIND_TYPE_MAX); /*assert verified*/

    switch (condition1)
    {
    case MMI_TREE_NODE_FIND_HANDLE:
        {
            if (condition2 == handle)
            {
                result = TRUE;
            }
        }
        break;

    case MMI_TREE_NODE_FIND_ID:
        {
            ctrl_node_ptr = GetCtrlNodePtr (handle);

            if (ctrl_node_ptr != PNULL
                    && condition2 == ctrl_node_ptr->ctrl_ptr->ctrl_id)
            {
                result = TRUE;
            }
        }
        break;

    case MMI_TREE_NODE_FIND_GUID:
        {
            ctrl_node_ptr = GetCtrlNodePtr (handle);

            if (ctrl_node_ptr != PNULL
                    && condition2 == ctrl_node_ptr->ctrl_ptr->object.guid)
            {
                result = TRUE;
            }
        }
        break;

    case MMI_TREE_NODE_FIND_STATE:
        {
            IGUICTRL_T* ctrl_ptr = PNULL;
            ctrl_node_ptr = GetCtrlNodePtr (handle);

            SCI_ASSERT (PNULL != ctrl_node_ptr); /*assert verified*/

            ctrl_ptr = (IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr;

            result = IGUICTRL_GetState (ctrl_ptr, condition2);
        }
        break;

    default:
        //SCI_ASSERT( FALSE );
        break;
    }

    return result;
}


/*****************************************************************************/
// 	Description : whether the control can be set active state
//	Global resource dependence :
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCtrlEnableActive (
    MMI_CTRL_NODE_T* ctrl_node_ptr
)
{
    BOOLEAN result = TRUE;

    SCI_ASSERT (PNULL != ctrl_node_ptr); /*assert verified*/

//#ifdef MMI_PDA_SUPPORT
    // 增强KEY模式下可以获取焦点的条件
    if ( (IGUICTRL_GetState ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, GUICTRL_STATE_INVISIBLE))
            || ( (IGUICTRL_GetState ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, GUICTRL_STATE_DISABLE_ACTIVE))
                 && ! (IGUICTRL_GetState ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, GUICTRL_STATE_KEYMODE_ACTIVE)
                       && MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode())))
//#else
//    if ((IGUICTRL_GetState( (IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, GUICTRL_STATE_INVISIBLE)) ||
//        (IGUICTRL_GetState( (IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, GUICTRL_STATE_DISABLE_ACTIVE)))
//#endif
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get the control pointer base on control_id from the appointed
//                window
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetCtrlNodeByWinHandle (
    MMI_HANDLE_T  win_handle,
    MMI_CTRL_ID_T ctrl_id,  // control id
    MMI_TREE_NODE_FIND_TYPE_E find_type
)
{
    MMI_CTRL_NODE_T*   ctrl_node_ptr   = PNULL;
    MMI_BASE_NODE_T*   win_node_ptr    = PNULL;
    MMI_HANDLE_T       ctrl_handle     = 0;

    //ctrl id为0, 只能使用handle
    if (0 != ctrl_id)
    {
        if (PNULL != (win_node_ptr = MMK_GetWinNodeByHandle (win_handle)))
        {
            if (0 != (ctrl_handle = MMK_FindAllTreeNode (MMK_GetCtrlTreeByWinNode (win_node_ptr),
                                    ControlTreeNodeCompare, find_type, ctrl_id,
                                    MMI_TREE_TRAVEL_ORDER | MMI_TREE_TRAVEL_CHILD)))
            {
                ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle);

                SCI_ASSERT (PNULL != ctrl_node_ptr); /*assert verified*/
            }
        }
    }

    return ctrl_node_ptr;
}

/*****************************************************************************/
// 	Description : ctrl point compare
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CtrlPointCompare (
    uint32 handle,
    uint32 condition1,
    uint32 condition2
)
{
    BOOLEAN result = FALSE;
    const MMI_CTRL_COMPARE_PARAM_T* param = (const MMI_CTRL_COMPARE_PARAM_T*) condition1;
    MMI_CTRL_NODE_T* ctrl_node_ptr = GetCtrlNodePtr (handle);

    if (PNULL != ctrl_node_ptr)
    {
        if (MMK_PosIsCtrl (handle, param->point)
                && MMK_IsResponseToClick (handle)
                && handle != param->exclude_handle
                && IGUICTRL_GetState ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, param->topmost_level))
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//     Description : get control rect
//    Global resource dependence :
//  Author: bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCtrlBothRect (
    MMI_HANDLE_T    ctrl_handle,
    GUI_BOTH_RECT_T     *both_rect_ptr    // [IN] point
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;
    BOOLEAN result = FALSE;

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (PNULL != (ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle)))
    {
        IGUICTRL_GetBothRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, both_rect_ptr);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//     Description : set control rect
//    Global resource dependence :
//  Author: bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN SetCtrlBothRect (
    MMI_HANDLE_T    ctrl_handle,
    GUI_BOTH_RECT_T     *both_rect_ptr    // [IN] point
)
{
    MMI_CTRL_NODE_T* ctrl_node_ptr = PNULL;
    BOOLEAN result = FALSE;
    GUI_RECT_T rect = {0};

    ctrl_handle = MMK_ConvertIdToHandle (ctrl_handle);

    if (PNULL != (ctrl_node_ptr = GetCtrlNodePtr (ctrl_handle)))
    {
        IGUICTRL_SetBothRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, both_rect_ptr);

        rect = MMK_IsWindowLandscape (MMK_GetWinHandleByCtrl (ctrl_node_ptr->base_node.handle))
               ? both_rect_ptr->h_rect : both_rect_ptr->v_rect;

        IGUICTRL_SetRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, &rect);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : add to group
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 分为两种情况:
// 1. 控件的区域与grouplist里的节点没有重叠，则新加一个
// 2. 控件区域与grouplist的节点重叠，则放到该节点中
/*****************************************************************************/
LOCAL void AddToGroup (
    MMK_GROUP_LIST_T      *header_group_ptr,              // group list header
    MMI_CTRL_NODE_T       *ctrl_node_ptr                  // ctrl node
)
{
    BOOLEAN             is_in_pos_list = FALSE;
    GUI_RECT_T          ctrl_rect = {0};
    MMK_GROUP_LIST_T    *group_node_ptr = PNULL;

    IGUICTRL_GetRect ((IGUICTRL_T*)ctrl_node_ptr->ctrl_ptr, &ctrl_rect);

    group_node_ptr = header_group_ptr;

    // group链表里的值肯定是从小到大排序的
    while (header_group_ptr != (group_node_ptr = (MMK_GROUP_LIST_T*) group_node_ptr->group_node.next))
    {
        if (group_node_ptr->top > ctrl_rect.bottom)       // 由于是排序的，所以当底都比group链表top都小，说明后面肯定不会有重叠的
        {
            break;
        }
        else if (group_node_ptr->bottom < ctrl_rect.top)  // 在所在group链表的下方，再去找下一个
        {
            continue;
        }
        else                                            // 位置重叠了
        {
            is_in_pos_list = TRUE;
            break;
        }
    }

    if (is_in_pos_list) // 存在于已有的group节点
    {
        AddToExistingGroupNode (header_group_ptr, group_node_ptr, ctrl_node_ptr, &ctrl_rect);
    }
    else    // 没有与其他控件重叠，则新增节点，插到group_node_ptr前
    {
        AddNewGroupNode (group_node_ptr, ctrl_node_ptr, &ctrl_rect);
    }
}

/*****************************************************************************/
//  Description : insert a group node before the specified one
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddNewGroupNode (
    MMK_GROUP_LIST_T   *base_group_ptr,              // base group node
    MMI_CTRL_NODE_T    *ctrl_node_ptr,               // ctrl node ptr
    GUI_RECT_T         *ctrl_rect_ptr                // ctrl rect
)
{
    MMK_GROUP_LIST_T    *group_node_ptr = PNULL;
    MMK_CELL_LIST_T     *cell_node_ptr = PNULL;

    // 创建一个新的节点，并插入到链表中
    group_node_ptr = (MMK_GROUP_LIST_T*) MMILIST_CreateNode (sizeof (MMK_GROUP_LIST_T));

    MMILIST_Init (&group_node_ptr->cell_header);        // cell_header需要初始化一下，否则数据为0

    group_node_ptr->top = ctrl_rect_ptr->top;
    group_node_ptr->bottom = ctrl_rect_ptr->bottom;

    MMILIST_InsertBefore (base_group_ptr, group_node_ptr);

    // 创建cell节点，并插入到链表中
    cell_node_ptr = (MMK_CELL_LIST_T*) MMILIST_CreateNode (sizeof (MMK_CELL_LIST_T));
    cell_node_ptr->left = ctrl_rect_ptr->left;
    cell_node_ptr->ctrl_node_ptr = ctrl_node_ptr;

    AddToCellList ( (MMK_CELL_LIST_T*) &group_node_ptr->cell_header, cell_node_ptr);
}

/*****************************************************************************/
//  Description : add to existing group node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 当group与其他group重叠时，检查并合并重叠部分
/*****************************************************************************/
LOCAL void AddToExistingGroupNode (
    MMK_GROUP_LIST_T      *header_group_ptr,  // group list header
    MMK_GROUP_LIST_T      *base_group_ptr,    // the existing group node
    MMI_CTRL_NODE_T       *ctrl_node_ptr,     // ctrl node
    GUI_RECT_T            *ctrl_rect_ptr      // ctrl rect
)
{
    BOOLEAN             is_check_other = FALSE;
    MMK_CELL_LIST_T     *cell_node_ptr = PNULL;

    // 加入节点
    cell_node_ptr = (MMK_CELL_LIST_T*) MMILIST_CreateNode (sizeof (MMK_CELL_LIST_T));
    cell_node_ptr->left = ctrl_rect_ptr->left;
    cell_node_ptr->ctrl_node_ptr = ctrl_node_ptr;

    AddToCellList ( (MMK_CELL_LIST_T*) &base_group_ptr->cell_header, cell_node_ptr);

    // 修改top, bottom
    if (ctrl_rect_ptr->top < base_group_ptr->top)
    {
        base_group_ptr->top = ctrl_rect_ptr->top;
        is_check_other = TRUE;
    }

    if (ctrl_rect_ptr->bottom > base_group_ptr->bottom)
    {
        base_group_ptr->bottom = ctrl_rect_ptr->bottom;
        is_check_other = TRUE;
    }

    // 当所加入的控件区域超过了原来的区域，检查其他节点是否重叠
    if (is_check_other)
    {
        CheckOverlapGroupNode (header_group_ptr, base_group_ptr);
    }
}

/*****************************************************************************/
//  Description : check group node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 当group与其他group重叠时，检查并合并重叠部分
/*****************************************************************************/
LOCAL void CheckOverlapGroupNode (
    MMK_GROUP_LIST_T       *header_group_ptr,           // group header
    MMK_GROUP_LIST_T       *base_group_ptr              // group node
)
{
    MMK_GROUP_LIST_T    *cur_group_node_ptr = PNULL;
    MMK_GROUP_LIST_T    *tmp_group_node_ptr = PNULL;
    MMK_CELL_LIST_T     *cur_cell_node_ptr = PNULL;

    cur_group_node_ptr = (MMK_GROUP_LIST_T*) header_group_ptr->group_node.next;

    while (header_group_ptr != cur_group_node_ptr)
    {
        // 由于group list保证是由小到大的，所以一旦出现比原来底还大的节点时，就退出
        if (cur_group_node_ptr->top > base_group_ptr->bottom)
        {
            break;
        }

        // 同样道理，如果节点的底比参考节点的顶小，找下一个
        if (cur_group_node_ptr == base_group_ptr || cur_group_node_ptr->bottom < base_group_ptr->top)
        {
            cur_group_node_ptr = (MMK_GROUP_LIST_T*) cur_group_node_ptr->group_node.next;
            continue;
        }

        base_group_ptr->top = MIN (cur_group_node_ptr->top, base_group_ptr->top);
        base_group_ptr->bottom = MAX (cur_group_node_ptr->bottom, base_group_ptr->bottom);

        // 将节点里的所有cell都搬到参考节点里
        //cur_cell_node_ptr = (MMK_CELL_LIST_T*)&cur_group_node_ptr->cell_header;     // 把cur_group_node_ptr->cell_header当成MMK_CELL_LIST_T
        while ( (MMK_CELL_LIST_T*) &cur_group_node_ptr->cell_header != (cur_cell_node_ptr = (MMK_CELL_LIST_T*) (cur_group_node_ptr->cell_header.next)))
        {
            MMILIST_Detach (cur_cell_node_ptr);
            AddToCellList ( (MMK_CELL_LIST_T*) &base_group_ptr->cell_header, cur_cell_node_ptr);
        }

        tmp_group_node_ptr = cur_group_node_ptr;
        cur_group_node_ptr = (MMK_GROUP_LIST_T*) cur_group_node_ptr->group_node.next;

        MMILIST_DestroyNode (tmp_group_node_ptr, PNULL);
    }
}

/*****************************************************************************/
//  Description : add a cell into cell list
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddToCellList (
    MMK_CELL_LIST_T        *header_cell_node_ptr,      // cell header
    MMK_CELL_LIST_T        *cell_node_ptr              // added cell node
)
{
    MMK_CELL_LIST_T     *cur_cell_node_ptr = header_cell_node_ptr;

    while (header_cell_node_ptr != (cur_cell_node_ptr = (MMK_CELL_LIST_T*) cur_cell_node_ptr->ctrl_node.next))
    {
        if (cur_cell_node_ptr->left < cell_node_ptr->left)
        {
            continue;
        }
    }

    MMILIST_InsertBefore ( (MMI_LIST_T*) cur_cell_node_ptr, (MMI_LIST_T*) cell_node_ptr);
}

/*****************************************************************************/
//  Description : create window group list
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 仅遍历窗口里第一层的控件
/*****************************************************************************/
LOCAL void CreateGroupList (
    MMI_BASE_NODE_T      *win_node_ptr,              // window node
    MMK_GROUP_LIST_T     *header_group_ptr           // group list header
)
{
    uint32 i = 0;
    MMI_CTRL_NODE_T         *ctrl_node_ptr = PNULL;
    MMI_TREE_TRAVEL_INFO_T  *travel_ptr = MMK_CreateTreeTravelInfo();

    MMK_TravelTree (travel_ptr, MMK_GetCtrlTreeByWinNode (win_node_ptr), MMI_TREE_TRAVEL_ORDER);

    for (; i < travel_ptr->total_num; i++)
    {
        ctrl_node_ptr = GetCtrlNodePtr (travel_ptr->data_ptr[i]);

        if (PNULL != ctrl_node_ptr && IsCtrlEnableActive (ctrl_node_ptr))
        {
            AddToGroup (header_group_ptr, ctrl_node_ptr);
        }
    }

    MMK_DestroyTreeTravelInfo (travel_ptr);
}

/*****************************************************************************/
//  Description : destroy window group list
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DestroyGroupList (
    MMK_GROUP_LIST_T       *header_group_ptr        // group list header
)
{
    // 由于头节点是栈上变量，所以不用释放
    MMILIST_DestroyAllNodes (header_group_ptr, FALSE, DestroyGroupNode);
}

/*****************************************************************************/
//  Description : destroy a group node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DestroyGroupNode (
    MMI_LIST_T *list_node_ptr                       // group node
)
{
    MMK_GROUP_LIST_T    *group_node_ptr = (MMK_GROUP_LIST_T*) list_node_ptr;

    // 头节点由DestroyGroupList释放，这里不用释放
    MMILIST_DestroyAllNodes (&group_node_ptr->cell_header, FALSE, PNULL);
}

/*****************************************************************************/
// 	Description : get ancestor ctrl handle
//	Global resource dependence :
//  Author: hua.fang
//	Note:
// 获得最顶层的父控件句柄
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetAncestorCtrlHandle (
    MMI_TREE_NODE_T* node_ptr          // ctrl node
)
{
    MMI_TREE_NODE_T *temp_node_ptr  = node_ptr;
    MMI_HANDLE_T    ancestro_handle = 0;

    SCI_ASSERT (PNULL != node_ptr); /*assert verified*/

    do
    {
        if (MMI_HANDLE_CONTROL != MMK_GetHandleType (temp_node_ptr->data))
        {
            break;
        }

        ancestro_handle = temp_node_ptr->data;
    }
    while (PNULL != (temp_node_ptr = temp_node_ptr->parent_ptr));

    return ancestro_handle;
}

/*****************************************************************************/
// 	Description : get active ctrl's group node and cell node
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL MMK_GROUP_LIST_T* GetActiveCtrlGroupInfo (
    MMI_BASE_NODE_T      *win_node_ptr,      // window node
    MMK_GROUP_LIST_T     *header_group_ptr,  // group list header
    MMK_CELL_LIST_T      **cell_node_pptr    // [out] cell node, can be null
)
{
    GUI_RECT_T          ctrl_rect = {0};
    MMK_GROUP_LIST_T    *group_node_ptr = header_group_ptr;
    MMI_CTRL_NODE_T     *ancestor_ctrl_node_ptr = PNULL;
    MMI_CTRL_NODE_T     *atv_ctrl_ptr = PNULL;
    MMK_CELL_LIST_T     *header_cell_node_ptr = PNULL;
    MMK_CELL_LIST_T     *cell_node_ptr = PNULL;

    atv_ctrl_ptr = MMK_GetActiveCtrlByWinNode (win_node_ptr);

    // get active ctrl rect
    ancestor_ctrl_node_ptr = GetCtrlNodePtr (GetAncestorCtrlHandle (atv_ctrl_ptr->ctrl_ptr->tree_node_ptr));

    if (PNULL != ancestor_ctrl_node_ptr)
    {
        IGUICTRL_GetRect ((IGUICTRL_T*)ancestor_ctrl_node_ptr->ctrl_ptr, &ctrl_rect);

        while (header_group_ptr != (group_node_ptr = (MMK_GROUP_LIST_T*) group_node_ptr->group_node.next))
        {
            // when in group region
            if (ctrl_rect.top >= group_node_ptr->top && ctrl_rect.bottom <= group_node_ptr->bottom)
            {
                if (PNULL != cell_node_pptr)
                {
                    header_cell_node_ptr = (MMK_CELL_LIST_T*) & group_node_ptr->cell_header;
                    cell_node_ptr = header_cell_node_ptr;

                    // find this active ctrl in cell list
                    while (header_cell_node_ptr != (cell_node_ptr = (MMK_CELL_LIST_T*) cell_node_ptr->ctrl_node.next))
                    {
                        if (cell_node_ptr->ctrl_node_ptr == ancestor_ctrl_node_ptr)
                        {
                            *cell_node_pptr = cell_node_ptr;
                            break;
                        }
                    }

                    // must have this control
                    SCI_ASSERT (PNULL != *cell_node_pptr);  /*assert verified*/
                }

                return group_node_ptr;
            }
        }
    }

    return PNULL;
}

/*****************************************************************************/
// 	Description : get active ctrl by up down
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetUpDownActiveCtrl (
    MMI_BASE_NODE_T      *win_node_ptr,      // window node
    MMK_GROUP_LIST_T     *header_group_ptr,  // group list header
    BOOLEAN              is_down             // find by down
)
{
    MMK_GROUP_LIST_T    *cur_group_node_ptr = PNULL;
    MMK_GROUP_LIST_T    *next_group_node_ptr = PNULL;
    MMK_CELL_LIST_T     *cell_node_ptr = PNULL;
    MMI_CTRL_NODE_T     *next_ctrl_node_ptr = PNULL;

    // get active ctrl group info
    cur_group_node_ptr = GetActiveCtrlGroupInfo (win_node_ptr, header_group_ptr, PNULL);

    if (PNULL != cur_group_node_ptr)
    {
        if (is_down)
        {
            next_group_node_ptr = (MMK_GROUP_LIST_T*) cur_group_node_ptr->group_node.next;
        }
        else
        {
            next_group_node_ptr = (MMK_GROUP_LIST_T*) cur_group_node_ptr->group_node.prev;
        }

        // when is not the group header, set the first ctrl in cell list
        if (next_group_node_ptr != header_group_ptr)
        {
            cell_node_ptr = (MMK_CELL_LIST_T*) next_group_node_ptr->cell_header.next;
            next_ctrl_node_ptr = cell_node_ptr->ctrl_node_ptr;
        }
    }

    return next_ctrl_node_ptr;
}

/*****************************************************************************/
// 	Description : get active ctrl by left right
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL MMI_CTRL_NODE_T* GetLeftRightActiveCtrl (
    MMI_BASE_NODE_T       *win_node_ptr,          // window node
    MMK_GROUP_LIST_T      *header_group_ptr,      // group list header
    BOOLEAN               is_right                // find by right
)
{
    MMK_GROUP_LIST_T    *cur_group_node_ptr = PNULL;
    MMK_CELL_LIST_T     *cur_cell_node_ptr = PNULL;
    MMK_CELL_LIST_T     *next_cell_node_ptr = PNULL;
    MMI_CTRL_NODE_T     *next_ctrl_node_ptr = PNULL;

    // get active ctrl group info
    cur_group_node_ptr = GetActiveCtrlGroupInfo (win_node_ptr, header_group_ptr, &cur_cell_node_ptr);

    if (PNULL != cur_group_node_ptr && PNULL != cur_cell_node_ptr)
    {
        if (is_right)
        {
            next_cell_node_ptr = (MMK_CELL_LIST_T*) cur_cell_node_ptr->ctrl_node.next;
        }
        else
        {
            next_cell_node_ptr = (MMK_CELL_LIST_T*) cur_cell_node_ptr->ctrl_node.prev;
        }

        // when the next cell node is not the cell header, set it
        if (next_cell_node_ptr != (MMK_CELL_LIST_T*) &cur_group_node_ptr->cell_header)
        {
            next_ctrl_node_ptr = next_cell_node_ptr->ctrl_node_ptr;
        }
    }

    return next_ctrl_node_ptr;
}

/*****************************************************************************/
// 	Description : applet node construct
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CompareIsEnanbleActive (
    uint32 handle,
    uint32 condition1,
    uint32 condition2
)
{
    BOOLEAN result = FALSE;

    // 隐藏的节点不是所找控件的祖先，且所找控件可以获取焦点
    if (!MMK_IsAncestorCtrl (condition1, handle)
            && IsCtrlEnableActive (GetCtrlNodePtr (handle)))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get parent handle
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetParentHandle (
    MMI_TREE_NODE_T* node_ptr
)
{
    MMI_HANDLE_T parent_handle = 0;

    SCI_ASSERT (PNULL != node_ptr); /*assert verified*/

    if (PNULL != node_ptr->parent_ptr)
    {
        parent_handle = node_ptr->parent_ptr->data;
    }

    return parent_handle;
}

