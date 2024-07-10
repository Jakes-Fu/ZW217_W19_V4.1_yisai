/****************************************************************************
** File Name:      mmk_tree.c                                              *
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
#define MMK_TREE_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmk_tree.h"

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
LOCAL MMI_TREE_NODE_T* TreeNodeNew(
                                   TREE_NODE_CONSTRUCT func,
                                   MMI_TREE_NODE_T*    parent_node_ptr,
                                   uint32 param1,
                                   uint32 param2
                                   );

/*****************************************************************************/
// 	Description : create travel info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_TREE_TRAVEL_INFO_T* MMK_CreateTreeTravelInfo(
                                                        void
                                                        )
{
    MMI_TREE_TRAVEL_INFO_T* travel_ptr = SCI_ALLOC_APP( sizeof( MMI_TREE_TRAVEL_INFO_T ) );/*lint !e26 !e64*/
    
    travel_ptr->max_num   = MMI_TREE_TRAVEL_MAX_LEN;
    travel_ptr->total_num = 0;
    travel_ptr->data_ptr  = SCI_ALLOC_APP( travel_ptr->max_num * sizeof(uint32) );/*lint !e26 !e64*/
    
    return travel_ptr;
}

/*****************************************************************************/
// 	Description : append travel info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void AppendTreeTravelInfo(
                                MMI_TREE_TRAVEL_INFO_T* travel_ptr,
                                uint32 data
                                )
{
    SCI_ASSERT( PNULL != travel_ptr ); /*assert verified*/
    SCI_ASSERT( PNULL != travel_ptr->data_ptr ); /*assert verified*/
    SCI_ASSERT( travel_ptr->total_num <= travel_ptr->max_num ); /*assert verified*/
    
    //set travel info
    if( travel_ptr->total_num == travel_ptr->max_num )
    {
        uint32* new_data_ptr = PNULL;
        
        travel_ptr->max_num = travel_ptr->max_num + MMI_TREE_TRAVEL_MAX_LEN;
        
        new_data_ptr = SCI_ALLOC_APP( travel_ptr->max_num * sizeof(uint32) );/*lint !e26 !e64*/
        
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
PUBLIC void MMK_DestroyTreeTravelInfo(
                                      MMI_TREE_TRAVEL_INFO_T* travel_ptr
                                      )
{
    SCI_ASSERT( PNULL != travel_ptr ); /*assert verified*/
    SCI_ASSERT( PNULL != travel_ptr->data_ptr ); /*assert verified*/

    SCI_FREE( travel_ptr->data_ptr );
    SCI_FREE( travel_ptr );
}

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
                           )
{
    MMI_TREE_NODE_T* find_node_ptr = PNULL;

    if( PNULL != root_ptr )
    {
        //from parent to child
        if( ( state & MMI_TREE_TRAVEL_ORDER ) == MMI_TREE_TRAVEL_ORDER )
        {
            find_node_ptr = root_ptr;

            do
            {
                //find brother first, then find child
                AppendTreeTravelInfo( travel_ptr, find_node_ptr->data );
                
                if( PNULL != find_node_ptr->child_ptr
                    && ( state & MMI_TREE_TRAVEL_CHILD ) == MMI_TREE_TRAVEL_CHILD )
                {
                    MMK_TravelTree( travel_ptr, find_node_ptr->child_ptr, state );
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
                    && ( state & MMI_TREE_TRAVEL_CHILD ) == MMI_TREE_TRAVEL_CHILD )
                {
                    MMK_TravelTree( travel_ptr, find_node_ptr->child_ptr, state );
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
PUBLIC MMI_TREE_NODE_T* MMK_AddTreeNode(
                                        TREE_NODE_CONSTRUCT         func,
                                        uint32                      param1,
                                        uint32                      param2,
                                        MMI_TREE_NODE_T**           root_pptr,
                                        MMI_TREE_NODE_T*            parent_node_ptr
                                        )
{
    MMI_TREE_NODE_T* root_ptr = PNULL;
    MMI_TREE_NODE_T* node_ptr = PNULL;
    
    SCI_ASSERT( PNULL != root_pptr && PNULL != func ); /*assert verified*/

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
        SCI_ASSERT( PNULL == parent_node_ptr ); /*assert verified*/

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
PUBLIC uint32 MMK_FindAllTreeNode(
                                  MMI_TREE_NODE_T*  root_ptr,
                                  TREE_NODE_COMPARE func,
                                  uint32            condition1, 
                                  uint32            condition2,
                                  MMI_TREE_TRAVEL_STATE_T state
                                  )
{
    uint32 data = 0;
    uint32 i = 0;
    MMI_TREE_TRAVEL_INFO_T* travel_ptr = MMK_CreateTreeTravelInfo();
    
    SCI_ASSERT( PNULL != func ); /*assert verified*/
    
    MMK_TravelTree( travel_ptr, root_ptr, state );
    
    for ( ; i < travel_ptr->total_num; i++ )
    {
        if( func( travel_ptr->data_ptr[i], condition1, condition2 ) )
        {
            data = travel_ptr->data_ptr[i];
            break;
        }
    }
    
    MMK_DestroyTreeTravelInfo( travel_ptr );

    return data;
}

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
                                      )
{
    uint32 i = 0;
    MMI_TREE_TRAVEL_INFO_T* travel_ptr = MMK_CreateTreeTravelInfo();

    SCI_ASSERT( PNULL != func ); /*assert verified*/

    MMK_TravelTree( travel_ptr, root_ptr, state );

    for ( ; i < travel_ptr->total_num; i++ )
    {
        func( travel_ptr->data_ptr[i], msg_id, param );
    }

    MMK_DestroyTreeTravelInfo( travel_ptr );
}

/*****************************************************************************/
// 	Description : remove child tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_RemoveAllTreeNode(
                                  MMI_TREE_NODE_T** root_pptr,
                                  TREE_NODE_DESTRUCT func
                                  )
{
    uint32 i = 0;
    MMI_TREE_TRAVEL_INFO_T* travel_ptr = MMK_CreateTreeTravelInfo();
    
    SCI_ASSERT( PNULL != func && PNULL != root_pptr ); /*assert verified*/
    
    MMK_TravelTree( travel_ptr, *root_pptr, MMI_TREE_TRAVEL_CHILD );
    
    *root_pptr = PNULL;

    for ( ; i < travel_ptr->total_num; i++ )
    {
        //树节点的内存, 由用户去销毁
        func( travel_ptr->data_ptr[i] );
    }
    
    MMK_DestroyTreeTravelInfo( travel_ptr );

    return;
}

/*****************************************************************************/
// 	Description : remove tree node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 销毁单个节点
/*****************************************************************************/
PUBLIC void MMK_RemoveTreeNode(
                               MMI_TREE_NODE_T** root_pptr,
                               TREE_NODE_DESTRUCT func,
                               MMI_TREE_NODE_T*  tree_node_ptr
                               )
{
    MMI_TREE_NODE_T* root_ptr = PNULL;
    
    SCI_ASSERT( PNULL != root_pptr && PNULL != func && PNULL != tree_node_ptr ); /*assert verified*/

    root_ptr = *root_pptr;

    if( PNULL == root_ptr )
    {
        return;
    }
    
    //remove child first
    if( PNULL != tree_node_ptr->child_ptr )
    {
        MMK_RemoveAllTreeNode( &tree_node_ptr->child_ptr, func );
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
PUBLIC void MMK_RemoveToAppointTreeNode(
                                        MMI_TREE_NODE_T**  root_pptr,
                                        TREE_NODE_DESTRUCT func,
                                        MMI_TREE_NODE_T*   tree_node_ptr
                                        )
{
    MMI_TREE_NODE_T* root_ptr = PNULL;
    MMI_TREE_TRAVEL_INFO_T* travel_ptr = PNULL;
    uint32 i = 0;
    BOOLEAN is_find = FALSE;

    SCI_ASSERT( PNULL != root_pptr && PNULL != func && PNULL != tree_node_ptr ); /*assert verified*/
    
    root_ptr = *root_pptr;
    
    SCI_ASSERT( PNULL != root_ptr ); /*assert verified*/
    SCI_ASSERT( PNULL != func ); /*assert verified*/
    //节点必须为父节点
    SCI_ASSERT( PNULL == tree_node_ptr->parent_ptr ); /*assert verified*/

    travel_ptr = MMK_CreateTreeTravelInfo();

    MMK_TravelTree( travel_ptr, *root_pptr, MMI_TREE_TRAVEL_CHILD );
    
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

    SCI_ASSERT( is_find ); /*assert verified*/
    
    MMK_DestroyTreeTravelInfo( travel_ptr );
}
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
                                                )
{
    MMI_TREE_NODE_T* return_ptr = PNULL;

    if( PNULL != node1_ptr && PNULL != node2_ptr )
    {
        MMI_TREE_NODE_T* child_ptr  = PNULL;
        MMI_TREE_NODE_T* ancestor_ptr = PNULL;
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
LOCAL MMI_TREE_NODE_T* TreeNodeNew(
                                   TREE_NODE_CONSTRUCT func,
                                   MMI_TREE_NODE_T*    parent_node_ptr,
                                   uint32 param1,
                                   uint32 param2
                                   )
{
    MMI_TREE_NODE_T* node_ptr = PNULL;

    node_ptr = SCI_ALLOC_APP( sizeof(MMI_TREE_NODE_T) );/*lint !e26 !e64*/
    SCI_MEMSET( node_ptr, 0, sizeof(MMI_TREE_NODE_T) );

    node_ptr->parent_ptr = parent_node_ptr;

    // construct failed
    if( !func( node_ptr, param1, param2 ) )
    {
        //树节点的内存, 由用户去销毁
        node_ptr = PNULL;
    }

    return node_ptr;
}
