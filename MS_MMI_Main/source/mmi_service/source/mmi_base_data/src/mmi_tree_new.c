/****************************************************************************
** File Name:      mmi_tree.c                                              *
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
#define MMI_TREE_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_tree_new.h"
#include "mmi_base_common.h"

typedef struct mmi_treenode_tag
{
    MMI_TREENODE_HANDLE_T   parent_ptr;
    MMI_TREENODE_HANDLE_T   child_ptr;
    MMI_TREENODE_HANDLE_T   prev_ptr;
    MMI_TREENODE_HANDLE_T   next_ptr;
    uint32                  deep;
    uint32                  data;
} MMI_TREENODE_T;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : tree node new
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_TREENODE_HANDLE_T TreeNodeNew(
                               TREENODE_CONSTRUCT      func,
                               MMI_TREENODE_HANDLE_T    parent_node_ptr,
                               uint32                   param1,
                               uint32                   param2
                               );

/*****************************************************************************/
// 	Description : create travel info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMITREE_TRAVEL_INFO_T* MMI_CreateTreeTravelInfo(
                                                    void
                                                    )
{
    MMITREE_TRAVEL_INFO_T* travel_ptr = SCI_ALLOCA( sizeof( MMITREE_TRAVEL_INFO_T ) );/*lint !e26 !e64*/
    
    SCI_MEMSET(travel_ptr, 0x00, sizeof(MMITREE_TRAVEL_INFO_T));
    travel_ptr->max_num   = MMITREE_TRAVEL_MAX_LEN;
    travel_ptr->total_num = 0;
    travel_ptr->data_ptr  = SCI_ALLOCA( travel_ptr->max_num * sizeof(uint32) );/*lint !e26 !e64*/
    SCI_MEMSET(travel_ptr->data_ptr, 0x00, travel_ptr->max_num * sizeof(uint32));
    
    return travel_ptr;
}

/*****************************************************************************/
// 	Description : append travel info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void AppendTreeTravelInfo(
                                MMITREE_TRAVEL_INFO_T* travel_ptr,
                                uint32 data
                                )
{
    MMIBD_ASSERT_DEBUG( PNULL != travel_ptr ); /*assert verified*/
    MMIBD_ASSERT_DEBUG( PNULL != travel_ptr->data_ptr ); /*assert verified*/
    MMIBD_ASSERT_DEBUG( travel_ptr->total_num <= travel_ptr->max_num ); /*assert verified*/
    
    //set travel info
    if( travel_ptr->total_num == travel_ptr->max_num )
    {
        uint32* new_data_ptr = PNULL;
        
        travel_ptr->max_num = travel_ptr->max_num + MMITREE_TRAVEL_MAX_LEN;
        
        new_data_ptr = SCI_ALLOCA( travel_ptr->max_num * sizeof(uint32) );/*lint !e26 !e64*/
        
        SCI_MEMCPY( new_data_ptr, travel_ptr->data_ptr, travel_ptr->total_num * sizeof(uint32) );
        
        SCI_FREE( travel_ptr->data_ptr );
        
        travel_ptr->data_ptr = new_data_ptr;
    }

    travel_ptr->data_ptr[travel_ptr->total_num] = data;
    travel_ptr->total_num++;
}

/*****************************************************************************/
// 	Description : destroy travel info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMI_DestroyTreeTravelInfo(
                                  MMITREE_TRAVEL_INFO_T* travel_ptr
                                  )
{
    MMIBD_ASSERT_DEBUG( PNULL != travel_ptr ); /*assert verified*/
    MMIBD_ASSERT_DEBUG( PNULL != travel_ptr->data_ptr ); /*assert verified*/

    SCI_FREE( travel_ptr->data_ptr );
    SCI_FREE( travel_ptr );
}

/*****************************************************************************/
// 	Description : travel all tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:为防止遍历树时某个节点被销毁, 先遍历获得所有节点的信息, 再遍历发消息
/*****************************************************************************/
PUBLIC void MMI_TravelTree(
                           MMITREE_TRAVEL_INFO_T*  travel_ptr,
                           MMI_TREENODE_HANDLE_T    root_ptr,
                           MMITREE_TRAVEL_STATE_T  state
                           )
{
    MMI_TREENODE_HANDLE_T find_node_ptr = PNULL;

    if( PNULL != root_ptr )
    {
        //from parent to child
        if( ( state & MMITREE_TRAVEL_ORDER ) == MMITREE_TRAVEL_ORDER )
        {
            find_node_ptr = root_ptr;

            do
            {
                //find brother first, then find child
                AppendTreeTravelInfo( travel_ptr, find_node_ptr->data );
                
                if( PNULL != find_node_ptr->child_ptr
                    && ( state & MMITREE_TRAVEL_CHILD ) == MMITREE_TRAVEL_CHILD )
                {
                    MMI_TravelTree( travel_ptr, find_node_ptr->child_ptr, state );
                }
                
                find_node_ptr = find_node_ptr->next_ptr;
                
            } while( find_node_ptr != root_ptr );
        }
        //from child to parent
        else
        {
            find_node_ptr = root_ptr->prev_ptr;
            
            do
            {
                //find child first, then find brother
                if( PNULL != find_node_ptr->child_ptr
                    && ( state & MMITREE_TRAVEL_CHILD ) == MMITREE_TRAVEL_CHILD )
                {
                    MMI_TravelTree( travel_ptr, find_node_ptr->child_ptr, state );
                }

                AppendTreeTravelInfo( travel_ptr, find_node_ptr->data );

                find_node_ptr = find_node_ptr->prev_ptr;
                
            } while( find_node_ptr != root_ptr->prev_ptr );
        }
    }
}

/*****************************************************************************/
// 	Description : add tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREENODE_HANDLE_T MMI_AddTreeNode(
                                        TREENODE_CONSTRUCT     func,
                                        uint32                  param1,
                                        uint32                  param2,
                                        MMI_TREENODE_HANDLE_T*  root_pptr,
                                        MMI_TREENODE_HANDLE_T   parent_node_ptr
                                        )
{
    MMI_TREENODE_HANDLE_T root_ptr = PNULL;
    MMI_TREENODE_HANDLE_T node_ptr = PNULL;
    
    MMIBD_ASSERT_DEBUG( PNULL != root_pptr && PNULL != func ); /*assert verified*/

    root_ptr = *root_pptr;

    if ( PNULL == ( node_ptr = TreeNodeNew( func, parent_node_ptr, param1, param2 ) ) )
    {
        return PNULL;
    }

    if( PNULL != root_ptr )
    {
        if( PNULL != parent_node_ptr )
        {
            if( PNULL != parent_node_ptr->child_ptr )
            {
                node_ptr->prev_ptr = parent_node_ptr->child_ptr->prev_ptr;
                node_ptr->next_ptr = parent_node_ptr->child_ptr;
                
                parent_node_ptr->child_ptr->prev_ptr->next_ptr = node_ptr;
                parent_node_ptr->child_ptr->prev_ptr = node_ptr;
            }
            else
            {
                parent_node_ptr->child_ptr = node_ptr;
                node_ptr->prev_ptr = node_ptr;
                node_ptr->next_ptr = node_ptr;
            }
        }
        else
        {
            node_ptr->prev_ptr = root_ptr->prev_ptr;
            node_ptr->next_ptr = root_ptr;
            
            root_ptr->prev_ptr->next_ptr = node_ptr;
            root_ptr->prev_ptr = node_ptr;
        }
    }
    else
    {
        //no root, must add parent node
        MMIBD_ASSERT_DEBUG( PNULL == parent_node_ptr ); /*assert verified*/

        *root_pptr = node_ptr;
        
        node_ptr->prev_ptr = node_ptr;
        node_ptr->next_ptr = node_ptr;
    }

    node_ptr->parent_ptr = parent_node_ptr;
    node_ptr->child_ptr  = PNULL;

    if( PNULL != node_ptr->parent_ptr )
    {
        node_ptr->deep = node_ptr->parent_ptr->deep + 1;
    }

    return node_ptr;
}

/*****************************************************************************/
// 	Description : find tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMI_FindAllTreeNode(
                                  MMI_TREENODE_HANDLE_T     root_ptr,
                                  TREENODE_COMPARE         func,
                                  uint32                    condition1, 
                                  uint32                    condition2,
                                  MMITREE_TRAVEL_STATE_T   state
                                  )
{
    uint32 data = 0;
    uint32 i = 0;
    MMITREE_TRAVEL_INFO_T* travel_ptr = MMI_CreateTreeTravelInfo();
    
    MMIBD_ASSERT_DEBUG( PNULL != func ); /*assert verified*/
    
    MMI_TravelTree( travel_ptr, root_ptr, state );
    
    for ( ; i < travel_ptr->total_num; i++ )
    {
        if( func( travel_ptr->data_ptr[i], condition1, condition2 ) )
        {
            data = travel_ptr->data_ptr[i];
            break;
        }
    }
    
    MMI_DestroyTreeTravelInfo( travel_ptr );

    return data;
}

/*****************************************************************************/
// 	Description : tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMI_DispatchToAllTreeNode(
                                      MMI_TREENODE_HANDLE_T     root_ptr,
                                      TREENODE_HANDLE_EVENT    func,
                                      uint32                    msg_id,
                                      void*                     param,
                                      MMITREE_TRAVEL_STATE_T   state
                                      )
{
    uint32 i = 0;
    MMITREE_TRAVEL_INFO_T* travel_ptr = MMI_CreateTreeTravelInfo();

    MMIBD_ASSERT_DEBUG( PNULL != func ); /*assert verified*/

    MMI_TravelTree( travel_ptr, root_ptr, state );

    for ( ; i < travel_ptr->total_num; i++ )
    {
        func( travel_ptr->data_ptr[i], msg_id, param );
    }

    MMI_DestroyTreeTravelInfo( travel_ptr );
}

/*****************************************************************************/
// 	Description : remove child tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMI_RemoveAllTreeNode(
                                  MMI_TREENODE_HANDLE_T*    root_pptr,
                                  TREENODE_DESTRUCT        func
                                  )
{
    uint32 i = 0;
    MMITREE_TRAVEL_INFO_T* travel_ptr = MMI_CreateTreeTravelInfo();
    
    MMIBD_ASSERT_DEBUG( PNULL != func && PNULL != root_pptr ); /*assert verified*/
    
    MMI_TravelTree( travel_ptr, *root_pptr, MMITREE_TRAVEL_CHILD );
    
    *root_pptr = PNULL;

    for ( ; i < travel_ptr->total_num; i++ )
    {
        //树节点的内存, 由用户去销毁
        func( travel_ptr->data_ptr[i] );
    }
    
    MMI_DestroyTreeTravelInfo( travel_ptr );

    return;
}

/*****************************************************************************/
// 	Description : remove tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 销毁单个节点
/*****************************************************************************/
PUBLIC void MMI_RemoveTreeNode(
                               MMI_TREENODE_HANDLE_T*   root_pptr,
                               TREENODE_DESTRUCT       func,
                               MMI_TREENODE_HANDLE_T    tree_node_ptr
                               )
{
    MMI_TREENODE_HANDLE_T root_ptr = PNULL;
    
    MMIBD_ASSERT_DEBUG( PNULL != root_pptr && PNULL != func && PNULL != tree_node_ptr ); /*assert verified*/

    root_ptr = *root_pptr;

    if( PNULL == root_ptr )
    {
        return;
    }
    
    //remove child first
    if( PNULL != tree_node_ptr->child_ptr )
    {
        MMI_RemoveAllTreeNode( &tree_node_ptr->child_ptr, func );
    }

    if( tree_node_ptr == root_ptr )
    {
        //only one node
        if( tree_node_ptr->next_ptr == tree_node_ptr )
        {
            *root_pptr = PNULL;
        }
        else
        {
            tree_node_ptr->next_ptr->prev_ptr = tree_node_ptr->prev_ptr;
            tree_node_ptr->prev_ptr->next_ptr = tree_node_ptr->next_ptr;
            
            *root_pptr = tree_node_ptr->next_ptr;
        }
    }
    else
    {
        //only one node
        if( tree_node_ptr->next_ptr == tree_node_ptr )
        {
            if( PNULL != tree_node_ptr->parent_ptr )
            {
                tree_node_ptr->parent_ptr->child_ptr = PNULL;
            }
        }
        else
        {
            tree_node_ptr->next_ptr->prev_ptr = tree_node_ptr->prev_ptr;
            tree_node_ptr->prev_ptr->next_ptr = tree_node_ptr->next_ptr;

            if( PNULL != tree_node_ptr->parent_ptr 
                && tree_node_ptr->parent_ptr->child_ptr == tree_node_ptr )
            {
                tree_node_ptr->parent_ptr->child_ptr = tree_node_ptr->next_ptr;
            }
        }
    }

    func( tree_node_ptr->data );
}
#if 0
/*****************************************************************************/
// 	Description : remove tree nodes
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:销毁到指定节点, 用于return to idle等应用
/*****************************************************************************/
PUBLIC void MMI_RemoveToAppointTreeNode(
                                        MMI_TREENODE_HANDLE_T*  root_pptr,
                                        TREENODE_DESTRUCT      func,
                                        MMI_TREENODE_HANDLE_T   tree_node_ptr
                                        )
{
    MMI_TREENODE_HANDLE_T root_ptr = PNULL;
    MMITREE_TRAVEL_INFO_T* travel_ptr = PNULL;
    uint32 i = 0;
    BOOLEAN is_find = FALSE;

    MMIBD_ASSERT_DEBUG( PNULL != root_pptr && PNULL != func && PNULL != tree_node_ptr ); /*assert verified*/
    
    root_ptr = *root_pptr;
    
    MMIBD_ASSERT_DEBUG( PNULL != root_ptr ); /*assert verified*/
    MMIBD_ASSERT_DEBUG( PNULL != func ); /*assert verified*/
    //节点必须为父节点
    MMIBD_ASSERT_DEBUG( PNULL == tree_node_ptr->parent_ptr ); /*assert verified*/

    travel_ptr = MMI_CreateTreeTravelInfo();

    MMI_TravelTree( travel_ptr, *root_pptr, MMITREE_TRAVEL_CHILD );
    
    //销毁前整理树的节点
    root_ptr->prev_ptr = tree_node_ptr;
    tree_node_ptr->next_ptr = root_ptr;

    for ( ; i < travel_ptr->total_num; i++ )
    {
        if( travel_ptr->data_ptr[i] != tree_node_ptr->data )
        {
            //树节点的内存, 由用户去销毁
            func( travel_ptr->data_ptr[i] );
        }
        else
        {
            is_find = TRUE;
            break;
        }
    }

    MMIBD_ASSERT_DEBUG( is_find ); /*assert verified*/
    
    MMI_DestroyTreeTravelInfo( travel_ptr );
}
#endif

/*****************************************************************************/
// 	Description : get parent tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 取父节点, 如无，返回PNULL
/*****************************************************************************/
PUBLIC MMI_TREENODE_HANDLE_T MMI_GetParentTreeNode(
    MMI_TREENODE_HANDLE_T node_ptr
    )
{
    MMI_TREENODE_HANDLE_T parent_ptr = PNULL;

    if( PNULL != node_ptr)
    {
        parent_ptr = node_ptr->parent_ptr;
    }
    return parent_ptr;
}

/*****************************************************************************/
// 	Description : get child tree node head
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 取头子节点, 如无，返回PNULL
/*****************************************************************************/
PUBLIC MMI_TREENODE_HANDLE_T MMI_GetChildTreeNode(
    MMI_TREENODE_HANDLE_T node_ptr
    )
{
    MMI_TREENODE_HANDLE_T child_ptr = PNULL;

    if( PNULL != node_ptr)
    {
        child_ptr = node_ptr->child_ptr;
    }
    return child_ptr;
}

/*****************************************************************************/
// 	Description : get sibling tree node head
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 取兄弟节点, 如无，返回PNULL
/*****************************************************************************/
PUBLIC MMI_TREENODE_HANDLE_T MMI_GetSiblingTreeNode(
    MMI_TREENODE_HANDLE_T node_ptr
    )
{
    MMI_TREENODE_HANDLE_T sibling_ptr = PNULL;

    if( PNULL != node_ptr)
    {
        sibling_ptr = node_ptr->next_ptr;
    }
    return sibling_ptr;
}

/*****************************************************************************/
// 	Description : get tree node data
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 取子节点数据, 如无，返回PNULL
/*****************************************************************************/
PUBLIC uint32 MMI_GetTreeNodeData(
    MMI_TREENODE_HANDLE_T node_ptr
    )
{
    uint32 data = PNULL;

    if( PNULL != node_ptr)
    {
        data = node_ptr->data;
    }
    return data;
}

/*****************************************************************************/
// 	Description : set tree node data
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 设置节点数据
/*****************************************************************************/
PUBLIC void MMI_SetTreeNodeData(
                                MMI_TREENODE_HANDLE_T   node_ptr,
                                uint32                  data
                                )
{
    if( PNULL != node_ptr)
    {
        node_ptr->data = data;
    }
}

/*****************************************************************************/
// 	Description : get ancestor tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 判断两个节点中哪个是祖先节点, 如果无关系, 返回PNULL
/*****************************************************************************/
PUBLIC MMI_TREENODE_HANDLE_T MMI_GetAncestorTreeNode(
                                                MMI_TREENODE_HANDLE_T node1_ptr,
                                                MMI_TREENODE_HANDLE_T node2_ptr
                                                )
{
    MMI_TREENODE_HANDLE_T return_ptr = PNULL;

    if( PNULL != node1_ptr && PNULL != node2_ptr )
    {
        MMI_TREENODE_HANDLE_T child_ptr  = PNULL;
        MMI_TREENODE_HANDLE_T ancestor_ptr = PNULL;
        uint32 deep1 = node1_ptr->deep;
        uint32 deep2 = node2_ptr->deep;
        uint32 abs_num = 0;
        uint32 i = 0;
        
        if( deep1 > deep2 )
        {
            abs_num      = deep1 - deep2;
            child_ptr    = node1_ptr;
            ancestor_ptr = node2_ptr;
        }
        else
        {
            abs_num      = deep2 - deep1;
            child_ptr    = node2_ptr;
            ancestor_ptr = node1_ptr;
        }
        
        for ( ; i < abs_num; i++ )
        {
            if( child_ptr->parent_ptr == ancestor_ptr )
            {
                return_ptr = ancestor_ptr;
                break;
            }
            
            child_ptr = child_ptr->parent_ptr;
        }
    }

    return return_ptr;
}
/*****************************************************************************/
// 	Description : tree node new
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_TREENODE_HANDLE_T TreeNodeNew(
                                   TREENODE_CONSTRUCT      func,
                                   MMI_TREENODE_HANDLE_T    parent_node_ptr,
                                   uint32                   param1,
                                   uint32                   param2
                                   )
{
    MMI_TREENODE_HANDLE_T node_ptr = PNULL;

    node_ptr = SCI_ALLOCA( sizeof(MMI_TREENODE_T) );/*lint !e26 !e64*/
    SCI_MEMSET( node_ptr, 0, sizeof(MMI_TREENODE_T) );

    node_ptr->parent_ptr = parent_node_ptr;

    // construct failed
    if( !func( node_ptr, param1, param2 ) )
    {
        //树节点的内存, 由用户去销毁
        node_ptr = PNULL;
    }

    return node_ptr;
}

PUBLIC MMI_TREENODE_HANDLE_T MMI_GetTreeRootHandle(MMI_TREENODE_HANDLE_T node_ptr)
{
    MMI_TREENODE_HANDLE_T root_ptr = PNULL;

    do {
        root_ptr = node_ptr;
        node_ptr = node_ptr->parent_ptr;
    } while(node_ptr);
    return root_ptr;
}

#ifdef STANDALONE_TEST
#ifdef _MMI_TREE_UNIT_TEST_
BOOLEAN my_compare_func(uint32 handle, uint32 param1, uint32 param2)
{
    if(handle/sizeof(int32)-1 == param2)
        return TRUE;
    return FALSE;
}

BOOLEAN my_construct_func(MMI_TREENODE_HANDLE_T node_ptr, uint32 param1, uint32 param2)
{
    uint32 handle = (param2+1)*sizeof(int32);
    MMI_SetTreeNodeData(node_ptr, handle);
    return TRUE;
}

void main(void)
{
    int i;
    MMI_TREENODE_HANDLE_T tree_root_handle = PNULL;
    MMI_TREENODE_HANDLE_T my_tree_handle = PNULL;
    MMI_TREENODE_HANDLE_T parent_node_ptr = PNULL;
    MMITREE_TRAVEL_INFO_T travel_info;
    uint32 handle;

    //create tree root node
    tree_root_handle = MMI_AddTreeNode(my_construct_func, TRUE, 0, &tree_root_handle, parent_node_ptr);

    parent_node_ptr = tree_root_handle;

    //create 1st child node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 1, &tree_root_handle, parent_node_ptr);

    //create 1st sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 2, &tree_root_handle, parent_node_ptr);

    //create 2st sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 3, &tree_root_handle, parent_node_ptr);

    //create 2nd child node
    parent_node_ptr = my_tree_handle;
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 4, &tree_root_handle, my_tree_handle);

    //create 1st sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 5, &tree_root_handle, parent_node_ptr);

    //create 2st sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 6, &tree_root_handle, parent_node_ptr);

    //create 3nd child node
    parent_node_ptr = my_tree_handle;
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 7, &tree_root_handle, my_tree_handle);

    //create 1st sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 8, &tree_root_handle, parent_node_ptr);

    //create 2st sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 9, &tree_root_handle, parent_node_ptr);

    //create 3rd sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 10, &tree_root_handle, parent_node_ptr);

    //create 4th sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 11, &tree_root_handle, parent_node_ptr);

    //create 5th sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 12, &tree_root_handle, parent_node_ptr);

    //create 6th sibling node
    my_tree_handle = MMI_AddTreeNode(my_construct_func, TRUE, 13, &tree_root_handle, parent_node_ptr);

    if(tree_root_handle != MMI_GetTreeRootHandle(my_tree_handle))
    {
        MMIBD_ASSERT_DEBUG(0);
    }
    handle = MMI_FindAllTreeNode(tree_root_handle, my_compare_func, TRUE, 2, MMITREE_TRAVEL_ORDER|MMITREE_TRAVEL_CHILD);

}
#endif
#endif
