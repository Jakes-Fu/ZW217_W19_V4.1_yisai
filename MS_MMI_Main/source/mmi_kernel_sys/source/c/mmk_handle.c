/****************************************************************************
** File Name:      mmk_handle.c                                            *
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
#define MMK_HANDLE_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmk_handle_internal.h"
/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
#define MMI_HANDLE_NODE_MAX  1500

#define MMI_HANDLE_CHECK_ID MMK_MODULE_MAX_NUM //use MMK_MODULE_MAX_NUM, for check static id assert
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
//句柄节点
typedef struct mmi_handle_node_tag
{
    MMI_BASE_NODE_T*            data_ptr;
    struct mmi_handle_node_tag* prev_node_ptr;
    struct mmi_handle_node_tag* next_node_ptr;
}MMI_HANDLE_NODE_T;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//句柄池
LOCAL MMI_HANDLE_NODE_T  s_handle_list[MMI_HANDLE_NODE_MAX] = {0};
//空闲句柄的起始节点
LOCAL MMI_HANDLE_NODE_T* s_free_handle_node_ptr = PNULL;
//有效句柄的起始节点
LOCAL MMI_HANDLE_NODE_T* s_start_handle_node_ptr = PNULL;
//句柄值的高位
LOCAL uint16             s_handle_high = 0;

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : create handle 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T CreateHandle( 
                                MMI_BASE_NODE_T* data_ptr
                                );

/*****************************************************************************/
// 	Description : destroy handle list 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyHandle( 
                            MMI_HANDLE_TYPE_E type,
                            MMI_HANDLE_T handle
                            );

/*****************************************************************************/
// 	Description : find handle list 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_NODE_T* FindHandleNode( 
                                        MMI_HANDLE_T handle
                                        );

/*****************************************************************************/
// 	Description : init handle list 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_InitHandleList( void )
{
    uint32 i = 0;
    
    //系统id范围(0~0xff)|句柄范围|caf id范围(0xff00~0xffff)
    SCI_ASSERT( MMI_HANDLE_CHECK_ID < ( CAF_USER_ID_START >>16 ) );/*assert verified*/

    for( i = 0; i < MMI_HANDLE_NODE_MAX - 1; i++ )
    {
        s_handle_list[i].data_ptr   = PNULL;
        s_handle_list[i].next_node_ptr = &s_handle_list[i + 1];
    }

    s_handle_list[i].data_ptr      = PNULL;
    s_handle_list[i].next_node_ptr = PNULL;

    s_free_handle_node_ptr = s_handle_list;
}

/*****************************************************************************/
// 	Description : create base node 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_CreateBaseNode( 
                                           MMI_HANDLE_TYPE_E type,
                                           uint32 size
                                           )
{
    MMI_BASE_NODE_T* base_node_ptr = PNULL;
    
    SCI_ASSERT( size >= sizeof(MMI_BASE_NODE_T) );/*assert verified*/
    
    base_node_ptr = SCI_ALLOC_APP( size );
    SCI_MEMSET( base_node_ptr, 0, size );
    
    base_node_ptr->handle_type = type;
    base_node_ptr->handle      = CreateHandle( base_node_ptr );
    
    return base_node_ptr;
}

/*****************************************************************************/
// 	Description : destroy base node 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyBaseNode( 
                                   MMI_BASE_NODE_T* base_node_ptr
                                   )
{
    SCI_ASSERT( PNULL != base_node_ptr );/*assert verified*/
    
    DestroyHandle( base_node_ptr->handle_type, base_node_ptr->handle );
    SCI_FREE( base_node_ptr );
    
    return TRUE;
}


/*****************************************************************************/
// 	Description : find base node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_FindBaseNode(
                                         MMI_HANDLE_T handle
                                         )
{
    MMI_HANDLE_NODE_T* handle_node_ptr = FindHandleNode( handle );

    if( PNULL != handle_node_ptr )
    {
        return handle_node_ptr->data_ptr;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
// 	Description : find base node
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_BASE_NODE_T* MMK_FindInBaseNodeByCondition(
                                                      MMI_BASE_NODE_COMPARE func,  //比较的回调函数
                                                      uint32 condition1,           //用户传入的条件1
                                                      uint32 condition2,           //用户传入的条件2
                                                      MMI_HANDLE_T first_handle,   //查找的起始handle, 为0则默认从头\尾查找
                                                      BOOLEAN is_order             //顺序或逆序
                                                      )
{
    MMI_HANDLE_NODE_T* last_node_ptr  = PNULL;
    MMI_HANDLE_NODE_T* node_ptr       = PNULL;
    MMI_BASE_NODE_T*   base_ptr       = PNULL;

    //SCI_ASSERT( PNULL != func );
    if ( PNULL == func )
    {
        return PNULL;
    }

	if ( PNULL != s_start_handle_node_ptr )
	{
		//顺序查找
		if( is_order )
		{
			node_ptr      = FindHandleNode( first_handle );
			last_node_ptr = s_start_handle_node_ptr;
			
			//句柄为空则从头开始遍历
			if( PNULL == node_ptr )
			{
				node_ptr = last_node_ptr;
			}
			else
			{
				node_ptr = node_ptr->next_node_ptr;
			}
			
			do
			{           
				if( func( node_ptr->data_ptr, condition1, condition2 ) )
				{
					base_ptr = node_ptr->data_ptr;
					break;
				}
				
				node_ptr = node_ptr->next_node_ptr;
				
			} while( node_ptr != last_node_ptr );
			
		}
		//逆序查找
		else
		{
			node_ptr      = FindHandleNode( first_handle );
			last_node_ptr = s_start_handle_node_ptr->prev_node_ptr;
			
			//句柄为空则从尾开始遍历
			if( PNULL == node_ptr )
			{
				node_ptr = last_node_ptr;
			}
			else
			{
				node_ptr = node_ptr->prev_node_ptr;
			}
			
			do
			{           
				if( func( node_ptr->data_ptr, condition1, condition2 ) )
				{
					base_ptr = node_ptr->data_ptr;
					break;
				}
				
				node_ptr = node_ptr->prev_node_ptr;
				
			} while( node_ptr != last_node_ptr );
			
		}
	}
    return base_ptr;
}

/*****************************************************************************/
// 	Description : check static id
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:系统id的高16位必须<0xff
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsHandleValid(
                                 MMI_HANDLE_T handle
                                 )
{
    BOOLEAN result      = TRUE;
    uint16  handle_high = handle >> 16;
 
    if( 0 != handle )
    {
        //系统id范围(0~0xff)|句柄范围|caf id范围(0xff00~0xffff)
        if( handle_high < MMI_HANDLE_CHECK_ID
            || handle_high >= ( CAF_USER_ID_START >>16 ) )
        {
            result = FALSE;
        }
    }

    return result;
}
/*****************************************************************************/
// 	Description : create handle 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T CreateHandle( MMI_BASE_NODE_T* data_ptr )
{
    MMI_HANDLE_NODE_T* new_node_ptr = PNULL;
    MMI_HANDLE_T       new_handle   = 0;

    SCI_ASSERT( PNULL != s_free_handle_node_ptr );/*assert verified*/

    //remove from free list
    new_node_ptr           = s_free_handle_node_ptr;
    new_node_ptr->data_ptr = data_ptr;
    s_free_handle_node_ptr = s_free_handle_node_ptr->next_node_ptr;

    //add to start list
    if( PNULL == s_start_handle_node_ptr )
    {
        s_start_handle_node_ptr = new_node_ptr;
        new_node_ptr->next_node_ptr = new_node_ptr;
        new_node_ptr->prev_node_ptr = new_node_ptr;
    }
    else
    {
        new_node_ptr->prev_node_ptr = s_start_handle_node_ptr->prev_node_ptr;
        new_node_ptr->next_node_ptr = s_start_handle_node_ptr;
        
        s_start_handle_node_ptr->prev_node_ptr->next_node_ptr = new_node_ptr;
        s_start_handle_node_ptr->prev_node_ptr = new_node_ptr;
    }

    s_handle_high++;
    
    //系统id范围(0~0xff)|句柄范围|caf id范围(0xff00~0xffff)
    if( s_handle_high < MMI_HANDLE_CHECK_ID
        || s_handle_high >= ( CAF_USER_ID_START >>16 ) )
    {
        s_handle_high = MMI_HANDLE_CHECK_ID;
    }

    new_handle = ( s_handle_high << 16 ) + ( new_node_ptr - s_handle_list );

    return new_handle;
}

/*****************************************************************************/
// 	Description : destroy handle list 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyHandle( 
                            MMI_HANDLE_TYPE_E type,
                            MMI_HANDLE_T      handle
                            )
{
    MMI_HANDLE_NODE_T* node_ptr = FindHandleNode( handle );
    
    SCI_ASSERT( PNULL != node_ptr && PNULL != node_ptr->data_ptr && type == node_ptr->data_ptr->handle_type );/*assert verified*/
    
    node_ptr->data_ptr = PNULL;
    
    //remove from start list
    if( node_ptr == s_start_handle_node_ptr )
    {
        //only one node
        if( s_start_handle_node_ptr->next_node_ptr == s_start_handle_node_ptr )
        {
            s_start_handle_node_ptr = PNULL;
        }
        else
        {
            node_ptr->next_node_ptr->prev_node_ptr = node_ptr->prev_node_ptr;
            node_ptr->prev_node_ptr->next_node_ptr = node_ptr->next_node_ptr;
            
            s_start_handle_node_ptr = node_ptr->next_node_ptr;
        }
    }
    else
    {
        node_ptr->next_node_ptr->prev_node_ptr = node_ptr->prev_node_ptr;
        node_ptr->prev_node_ptr->next_node_ptr = node_ptr->next_node_ptr;
    }

    //add to free list
    node_ptr->next_node_ptr = s_free_handle_node_ptr;
    s_free_handle_node_ptr  = node_ptr;
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : destroy handle list 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_NODE_T* FindHandleNode( 
                                        MMI_HANDLE_T handle
                                        )
{
    MMI_HANDLE_NODE_T* node_ptr = PNULL;
    uint32             index    = handle & 0x0000FFFF;
    
    //check static id
    if( !MMK_IsHandleValid( handle ) )
    {
        //SCI_ASSERT( FALSE );
        return PNULL;
    }
    
    if( index < MMI_HANDLE_NODE_MAX )
    {
        node_ptr = &s_handle_list[index];
        
        /* invalid handle */
        if( PNULL == node_ptr->data_ptr
            || handle != node_ptr->data_ptr->handle )
        {
            node_ptr = PNULL;
        }
    }

    return node_ptr;
}
