/*****************************************************************************
** File Name:      netmgr_list.h                                             *
** Author:         juan.zhang                                                *
** Date:           11/02/2009                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    DAPS MEM FUNCTION                                         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/02/2009     juan.zhang          Create                                 *
******************************************************************************/

#ifndef _MMIPDP_LIST_H_
#define _MMIPDP_LIST_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

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
/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/
typedef void*   MMIPDP_NODE_OBJECT;

typedef BOOLEAN (*MMIPDP_NODE_QUERY_CALLBACK)(MMIPDP_NODE_OBJECT data, void *find_data);

typedef uint32 MMIPDP_LIST_HANDLE;

typedef uint32 MMIPDP_LIST_NODE_HANDLE;

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
// 	Description : Creat a list
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC MMIPDP_LIST_HANDLE MMIPDP_ListCreat(uint32 node_data_size);

/****************************************************************************/
// 	Description : Destroy a list
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC void MMIPDP_ListDestroy(MMIPDP_LIST_HANDLE list_handler);
/****************************************************************************/
// 	Description : to get the list head node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListGetHead(MMIPDP_LIST_HANDLE list_handler);
/****************************************************************************/
// 	Description : to get the list tail node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListGetTail(MMIPDP_LIST_HANDLE list_handler);
/****************************************************************************/
// 	Description : to get the the list's current node number
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC uint32 MMIPDP_ListGetMemberCount(MMIPDP_LIST_HANDLE list_handler);

/****************************************************************************/
// 	Description : To insert a node after the indicated node. If the indicated 
//  node was null, the new node would be inserted to be the head node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListInsertNode(
										MMIPDP_LIST_HANDLE			list_handler, 
										MMIPDP_LIST_NODE_HANDLE	after_which_handler,
										MMIPDP_NODE_OBJECT			new_node_data,
										uint32					data_size
										);

/****************************************************************************/
// 	Description : To delete a node from the indicated list
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC void MMIPDP_ListDeleteNode(
									MMIPDP_LIST_HANDLE			list_handler, 
									MMIPDP_LIST_NODE_HANDLE	to_delete_node_ptr
									);

/****************************************************************************/
// 	Description : To find a node in the indicated list using the rules that 
//  described in the callback function
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListFindNode(
										MMIPDP_LIST_HANDLE			list_handler,
										void						*find_data, 
										MMIPDP_NODE_QUERY_CALLBACK	node_query_callback
										);

/****************************************************************************/
// 	Description : to get the previous node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListGetPrevNode(
											MMIPDP_LIST_NODE_HANDLE cur_node_handler
											);

/****************************************************************************/
// 	Description : to get the next node
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListGetNextNode(
											MMIPDP_LIST_NODE_HANDLE cur_node_handler
											);
/****************************************************************************/
// 	Description : to get the node data
//	Global resource dependence : 
//  Author:juan.zhang 
//	Note:	
/****************************************************************************/
PUBLIC MMIPDP_NODE_OBJECT MMIPDP_ListGetNodeData(
											MMIPDP_LIST_NODE_HANDLE node_handler
											);



#ifdef   __cplusplus
    }
#endif


#endif 

