/*****************************************************************************
** File Name:      cfl_list.h                                                *
** Author:         juan.zhang                                                *
** Date:           10/02/2009                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    list functions                                            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/02/2009     juan.zhang          Create                                 *
******************************************************************************/

#ifndef _CFL_LIST_H_
#define _CFL_LIST_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "cfl_mem.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define CFL_ListCreat(_NODE_DATA_SIZE, _MAX_NODE_NUM)  \
        CFL_ListCreateEx(_NODE_DATA_SIZE, _MAX_NODE_NUM, 0, __FILE__, __LINE__)
        
#define CFL_ListCreateDM(_NODE_DATA_SIZE, _MAX_NODE_NUM, _MSP_HANDLE)  \
        CFL_ListCreateEx(_NODE_DATA_SIZE, _MAX_NODE_NUM, _MSP_HANDLE, __FILE__, __LINE__)

#define CFL_ListInsertNode(_LIST_HANDLER, _AFTER_WHICH_HANDLER, _NEW_NODE_DATA, _DATA_SIZE) \
        CFL_ListInsertNodeEx(_LIST_HANDLER, _AFTER_WHICH_HANDLER, _NEW_NODE_DATA, _DATA_SIZE, __FILE__, __LINE__)

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef void*   CFL_NODE_OBJECT;

typedef BOOLEAN (*LIST_NODE_QUERY_CALLBACK)(CFL_NODE_OBJECT data, void *find_data);
typedef uint32 CFL_LIST_HANDLE;
#ifdef WIN32
	typedef DWORD64 CFL_LIST_NODE_HANDLE;
#else
	typedef long long CFL_LIST_NODE_HANDLE;
#endif

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/


/****************************************************************************/
// 	Description : Create a list
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note: if the list's max node num is ZERO, the nodes to be inserted
//			in this list is unlimited
/****************************************************************************/
PUBLIC CFL_LIST_HANDLE CFL_ListCreateEx(
                                        uint32 node_data_size,
                                        uint32 max_node_num,
                                        uint32 msp_handle,
                                        char   *file_ptr,
                                        uint32 line
                                        );

/****************************************************************************/
// 	Description : Destroy a list
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC void CFL_ListDestroy(CFL_LIST_HANDLE list_handler);

/****************************************************************************/
// 	Description : to get the list head node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListGetHead(CFL_LIST_HANDLE list_handler);

/****************************************************************************/
// 	Description : to get the list tail node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListGetTail(CFL_LIST_HANDLE list_handler);

/****************************************************************************/
// 	Description : to get the the list's current node number
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC uint32 CFL_ListGetMemberCount(CFL_LIST_HANDLE list_handler);

/****************************************************************************/
// 	Description : To insert a node after the indicated node. If the indicated 
//  node was null, the new node would be inserted to be the head node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListInsertNodeEx(
                                                 CFL_LIST_HANDLE	   list_handler, 
                                                 CFL_LIST_NODE_HANDLE  after_which_handler,
                                                 CFL_NODE_OBJECT	   new_node_data,
                                                 uint32				   data_size,
                                                 char                  *file_ptr,
                                                 uint32                line
                                                 );

/****************************************************************************/
// 	Description : To delete a node from the indicated list
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC void CFL_ListDeleteNode(
                               CFL_LIST_HANDLE			list_handler, 
                               CFL_LIST_NODE_HANDLE	to_delete_node_ptr
                               );

/****************************************************************************/
// 	Description : To delete a node from the indicated list
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN CFL_ListUpdateNodeData(
									CFL_LIST_HANDLE			list_handler, 
									CFL_LIST_NODE_HANDLE	to_update_node_handler,
									CFL_NODE_OBJECT			new_data,
									uint32					data_size
									);

/****************************************************************************/
// 	Description : To find a node in the indicated list using the rules that 
//  described in the callback function
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
CFL_LIST_NODE_HANDLE CFL_ListFindNode(
										CFL_LIST_HANDLE				list_handler,
										void						*find_data, 
										LIST_NODE_QUERY_CALLBACK	node_query_callback
										);
/****************************************************************************/
// 	Description : To the node is exist in the list, using node pointer and node
//				creat time. 
//  described in the callback function
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC BOOLEAN CFL_ListCheckIfNodeValid(
										CFL_LIST_NODE_HANDLE node_handler
										);

PUBLIC BOOLEAN CFL_ListCheckIfNodeValidFromTail(
										CFL_LIST_NODE_HANDLE node_handler
										);
/****************************************************************************/
// 	Description : to get the previous node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListGetPrevNode(
											CFL_LIST_NODE_HANDLE cur_node_handler
											);

PUBLIC CFL_LIST_NODE_HANDLE CFL_ListGetPrevNodeFromTail(
											CFL_LIST_NODE_HANDLE cur_node_handler
											);

/****************************************************************************/
// 	Description : to get the next node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListGetNextNode(
											CFL_LIST_NODE_HANDLE cur_node_handler
											);
/****************************************************************************/
// 	Description : to get the node data
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC CFL_NODE_OBJECT CFL_ListGetNodeData(
											CFL_LIST_NODE_HANDLE node_handler
											);

PUBLIC CFL_NODE_OBJECT CFL_ListGetNodeDataFromTail(
											CFL_LIST_NODE_HANDLE node_handler
											);
#ifdef   __cplusplus
    }
#endif


#endif 

