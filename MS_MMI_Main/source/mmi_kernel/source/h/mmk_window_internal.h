/****************************************************************************
** File Name:      mmk_window_internal.h                                   *
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

#ifndef  _MMK_WINDOW_INTERNAL_H_    
#define  _MMK_WINDOW_INTERNAL_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmk_tree.h"
#include "mmk_handle_internal.h"

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

typedef enum
{
    MMI_TREE_NODE_FIND_HANDLE,
    MMI_TREE_NODE_FIND_ID,
    MMI_TREE_NODE_FIND_GUID,
    MMI_TREE_NODE_FIND_STATE,
    MMI_TREE_NODE_FIND_TYPE_MAX
}MMI_TREE_NODE_FIND_TYPE_E;

typedef enum
{
    MMK_TRAVEL_ACTIVE_UP,
    MMK_TRAVEL_ACTIVE_DOWN,
    MMK_TRAVEL_ACTIVE_LEFT,
    MMK_TRAVEL_ACTIVE_RIGHT,
    MMK_TRAVEL_ACTIVE_MAX
} MMK_TRAVEL_ACTIVE_E;

/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get the window handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_GetWinTreeNode(
                                           MMI_HANDLE_T win_handle
                                           );

/*****************************************************************************/
// 	Description : is win go focus
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 窗口是否曾经获得焦点
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinGotFocus(
                                 MMI_HANDLE_T win_handle
                                 );

/******************************************************************************/
/*! \brief default process the window message
*  \author Great.Tian 
*  \param[in]	 win_ptr 		the window pointer
*  \param[in]	 msg_id 		the message id
*  \param[in]	 param 			the param
*  \return the result of process
*/
/******************************************************************************/
PUBLIC MMI_RESULT_E MMK_DefaultProcessWinMsg(
                                             MMI_HANDLE_T       win_handle,
                                             MMI_MESSAGE_ID_E	 msg_id,   
                                             DPARAM             param    
                                             );

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
                                       );

/******************************************************************************/
/*! \brief must process the window message
*  \author Great.Tian 
*  \param[in]	 win_ptr 		the window pointer
*  \param[in]	 msg_id 		the message id
*  \param[in]	 param 			the param
*  \return 
*		- <i><c>TRUE</c></i>: 	sucess
* 		- <i><c>FALSE</c></i>:	failed
*/
/******************************************************************************/
PUBLIC BOOLEAN MMK_ProcSpecialWinMsg(
                                     MMI_HANDLE_T		win_handle, 
                                     MMI_MESSAGE_ID_E	msg_id,   
                                     DPARAM			param    
                                     );

/******************************************************************************/
/*! \brief run the control process 
*  \author Great.Tian 
*  \param[in]	 hwnd			handle of window
*  \param[in]	 msg_id			message id
*  \param[in]	 param			param
*  \return the result of process
*/
/******************************************************************************/
PUBLIC MMI_RESULT_E MMK_RunCtrlProc(
                                    MMI_HANDLE_T       ctrl_handle,
                                    MMI_MESSAGE_ID_E   msg_id,
                                    DPARAM 		       param
                                    );

/******************************************************************************/
/*! \brief run the control process 
*  \author Great.Tian 
*  \param[in]	 hwnd			handle of window
*  \param[in]	 msg_id			message id
*  \param[in]	 param			param
*  \return the result of process
*/
/******************************************************************************/
PUBLIC MMI_RESULT_E MMK_RunWinProc(
                                   MMI_HANDLE_T         win_handle,
                                   MMI_MESSAGE_ID_E 	msg_id,
                                   DPARAM 				param
                                   );

/******************************************************************************/
/*! \brief handle the MSG_OPEN_WINDOW message
*  \author Great.Tian 
*  \param[in]	 win_ptr		the window pointer
*  \param[in]	 msg_id			message id
*  \param[in]	 param			param
*  \return the result of process
*/
/******************************************************************************/
PUBLIC BOOLEAN MMK_ProcOpenWinMsg(
                                  MMI_HANDLE_T		win_handle,
                                  MMI_MESSAGE_ID_E	msg_id,   
                                  DPARAM			param     
                                  );

/*****************************************************************************/
// 	Description : zorder system update, for close window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ZorderSystemUpdate( void );

/******************************************************************************/
/*! \brief dispatch to parent ctrl
*  \author Great.Tian 
*  \param[in]	 ctrl handle
*  \return result
*/
/******************************************************************************/
// PUBLIC MMI_RESULT_E MMK_DispatchToParentCtrl(
//                                              MMI_HANDLE_T     ctrl_handle,
//                                              MMI_MESSAGE_ID_E msg_id,
//                                              DPARAM           param
//                                              );

/*****************************************************************************/
// 	Description : window node construct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_WindowTreeNodeConstruct( 
                                           MMI_TREE_NODE_T* node_ptr,
                                           uint32 param1,
                                           uint32 param2
                                           );

/*****************************************************************************/
// 	Description : window node destruct
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_WindowTreeNodeDestruct( 
                                          uint32 handle
                                          );

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
                                         );

/*****************************************************************************/
// 	Description : 是否响应硬图标
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:临时方案, 避免模块在硬图标退出时未释放完整, 增加禁止响应硬图标的功能
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsResponseHWIcon( void );

/*****************************************************************************/
// 	Description : 是否需要关闭窗口
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:防止窗口被重复关闭导致暴栈
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsNeedCloseWin( 
                                  MMI_HANDLE_T win_handle
                                  );

/*****************************************************************************/
// 	Description : 是否需要关闭窗口
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:防止窗口被重复关闭导致暴栈
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinClose( 
                               MMI_HANDLE_T win_handle
                               );

/*****************************************************************************/
// 	Description : travel active ctrl
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_TravelActiveCtrl(
                                 MMI_HANDLE_T           win_handle,         // window handle
                                 MMK_TRAVEL_ACTIVE_E    travel_active       // travel enum
                                 );

/*****************************************************************************/
//     Description : get the window pointer on the point
//    Global resource dependence : 
//  Author:Robert.Lu
//    Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_GetTPPressWin(
                                       GUI_POINT_T point,
                                       BOOLEAN     is_find_parent
                                       );



/*****************************************************************************/
// 	Description : get win handle by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandleByWinNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
// 	Description : set active ctrl by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetActiveCtrlByWinNode(
    MMI_BASE_NODE_T *node_ptr,
    MMI_BASE_NODE_T *ctrl_node_ptr
    );

/*****************************************************************************/
// 	Description : get active ctrl by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_GetActiveCtrlByWinNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
// 	Description : get ctrl tree by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_GetCtrlTreeByWinNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
// 	Description : get ctrl tree ptr by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T** MMK_GetCtrlTreePtrByWinNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
// 	Description : get win tree ptr by win node
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T** MMK_GetWinTreeByWinNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
// 	Description : get win node ptr by win handle
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_GetWinNodeByHandle(
    MMI_HANDLE_T    win_handle
    );

#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC IDC_T* MMK_GetDCByWinNode(
    MMI_BASE_NODE_T *node_ptr
    );
#endif

/*****************************************************************************/
//  Description : get handle by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetHandleByCtrlNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
//  Description : get guid by ctrl handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CAF_GUID_T MMK_GetGuidByCtrlNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
//  Description : get ctrl ptr by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC IGUICTRL_T* MMK_GetCtrlPtrByCtrlNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
//  Description : get window handle by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandleByCtrlNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
//  Description : get ctrl id by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetCtrlIdByCtrlNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
//  Description : get tree node by ctrl node
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_GetTreeNodeByCtrlNode(
    MMI_BASE_NODE_T *node_ptr
    );

/*****************************************************************************/
//  Description : get tree node by ctrl handle
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_GetCtrlNodeByHandle(
    MMI_HANDLE_T       ctrl_handle
    );

/*****************************************************************************/
//  Description : destroy all controls
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMK_DestroyAllControlByNode(
    MMI_BASE_NODE_T* node_ptr
    );

/*****************************************************************************/
//  Description : dispatch to all controls
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchToAllControl(
    MMI_BASE_NODE_T* node_ptr,
    uint32  msg_id,
    void*   param,
    MMI_TREE_TRAVEL_STATE_T state
    );

/*****************************************************************************/
// 	Description : set has statusbar tag
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetHasStb(
    MMI_BASE_NODE_T *node_ptr,
    BOOLEAN         has_stb
    );

/*****************************************************************************/
// 	Description : set has tips tag
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetHasTips(
    MMI_BASE_NODE_T *node_ptr,
    BOOLEAN         has_tips
    );

#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC IDC_T* MMK_GetDCByCtrlNode(
    MMI_BASE_NODE_T *node_ptr
    );
#endif

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_WINDOW_INTERNAL_H_  */
