/****************************************************************************
** File Name:      mmk_tree.h                                     *
** Author:         James.Zhang                                             *
** Date:           05/15/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _MMK_TREE_H_    
#define  _MMK_TREE_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "os_api.h"
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
#define MMI_TREE_TRAVEL_MAX_LEN  32

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef uint32 MMI_TREE_TRAVEL_STATE_T;
/* item state */
#define MMI_TREE_TRAVEL_ORDER          0x000000001   //顺序, 否则为逆序
#define MMI_TREE_TRAVEL_CHILD          0x000000002   //遍历子节点, 否则只遍历父节点

typedef struct
{
    uint32* data_ptr;
    uint32  total_num;
    uint32  max_num;

}MMI_TREE_TRAVEL_INFO_T;

typedef struct mmi_tree_node_tag MMI_TREE_NODE_T;
struct mmi_tree_node_tag
{
    uint32                      data;
    uint32                      deep;
    MMI_TREE_NODE_T*            parent_ptr;
    MMI_TREE_NODE_T*            child_ptr;
    MMI_TREE_NODE_T*            prev_ptr;
    MMI_TREE_NODE_T*            next_ptr;
};

typedef BOOLEAN (*TREE_NODE_CONSTRUCT)    ( MMI_TREE_NODE_T* node_ptr, uint32 param1, uint32 param2 );

typedef BOOLEAN (*TREE_NODE_DESTRUCT)     ( uint32 handle );
typedef uint32  (*TREE_NODE_HANDLE_EVENT) ( uint32 handle, uint32 msg_id, void* param );
typedef BOOLEAN (*TREE_NODE_COMPARE)      ( uint32 handle, uint32 condition1, uint32 condition2 );

/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : add tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 如果失败, 需要用户销毁tree node
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_AddTreeNode(
                                        TREE_NODE_CONSTRUCT         func,
                                        uint32                      param1,
                                        uint32                      param2,
                                        MMI_TREE_NODE_T**           root_pptr,
                                        MMI_TREE_NODE_T*            parent_node_ptr
                                        );

/*****************************************************************************/
// 	Description : find tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMK_FindAllTreeNode(
                                  MMI_TREE_NODE_T*  root_ptr,
                                  TREE_NODE_COMPARE func,
                                  uint32            condition1, 
                                  uint32            condition2,
                                  MMI_TREE_TRAVEL_STATE_T state
                                  );


/*****************************************************************************/
// 	Description : tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchToAllTreeNode(
                                      MMI_TREE_NODE_T*       root_ptr,
                                      TREE_NODE_HANDLE_EVENT func,
                                      uint32                 msg_id,
                                      void*                  param,
                                      MMI_TREE_TRAVEL_STATE_T state
                                      );

/*****************************************************************************/
// 	Description : remove tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_RemoveTreeNode(
                               MMI_TREE_NODE_T** root_pptr,
                               TREE_NODE_DESTRUCT func,
                               MMI_TREE_NODE_T*  tree_node_ptr
                               );

/*****************************************************************************/
// 	Description : remove child tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_RemoveAllTreeNode(
                                  MMI_TREE_NODE_T** root_pptr,
                                  TREE_NODE_DESTRUCT func
                                  );
#if 0
/*****************************************************************************/
// 	Description : remove tree nodes
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:销毁到指定节点, 用于return to idle等应用
/*****************************************************************************/
PUBLIC void MMK_RemoveToAppointTreeNode(
                                        MMI_TREE_NODE_T**  root_pptr,
                                        TREE_NODE_DESTRUCT func,
                                        MMI_TREE_NODE_T*   tree_node_ptr
                                        );
#endif
/*****************************************************************************/
// 	Description : get ancestor tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 判断两个节点中哪个是祖先节点, 如果无关系, 返回PNULL
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_GetAncestorTreeNode(
                                                MMI_TREE_NODE_T* node1_ptr,
                                                MMI_TREE_NODE_T* node2_ptr
                                                );

/*****************************************************************************/
// 	Description : create travel info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_TRAVEL_INFO_T* MMK_CreateTreeTravelInfo(
                                                        void
                                                        );

/*****************************************************************************/
// 	Description : travel all tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:为防止遍历树时某个节点被销毁, 先遍历获得所有节点的信息, 再遍历发消息
/*****************************************************************************/
PUBLIC void MMK_TravelTree(
                           MMI_TREE_TRAVEL_INFO_T* travel_ptr,
                           MMI_TREE_NODE_T*        root_ptr,
                           MMI_TREE_TRAVEL_STATE_T state
                           );

/*****************************************************************************/
// 	Description : destroy travel info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DestroyTreeTravelInfo(
                                      MMI_TREE_TRAVEL_INFO_T* travel_ptr
                                      );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_TREE_INTERNAL_H_  */
