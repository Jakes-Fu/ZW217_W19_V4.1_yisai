/******************************************************************************
 ** File Name:      mmi_mem.c                                               *
 ** Author:         gang.tong                                              *
 ** DATE:           03/13/2008                                                *
 ** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 large block memory including malloc and free              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/13/2008     gang.tong        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef _MMI_LINK_H_
#define _MMI_LINK_H_

#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//typedef enum
//{
//    MMI_DOUBLE_BREAK_LINKED = 0,
//    MMI_SINGLE_BREAK_LINKED,
//    MMI_DOUBLE_CIRCLE_LINKED,
//    MMI_SINGLE_CIRCLE_LINKED,
//    MMI_LINK_TYPE_MAX
//}MMI_LINK_TYPE_E;
//
typedef enum
{
    MMI_LINK_HEAD = 0,
    MMI_LINK_TAIL, 
    MMI_LINK_END_MAX
}MMI_LINK_END_TYPE_E;

//typedef enum
//{
//    MMI_BEFORE_BASE_NODE = 0,
//    MMI_AFTER_BASE_NODE,    
//    MMI_LINK_INSERT_TYPE_MAX
//}MMI_LINK_INSERT_TYPE_E;

//
//typedef struct _mmi_link_base_node_t_
//{
//    MMI_LINK_TYPE_E link_type; 
//    uint32 data;
//}MMI_LINK_BASE_NODE_T;
//
//typedef struct _mmi_single_link_node_t_
//{
//    MMI_LINK_BASE_NODE_T node_base; 
//    struct _mmi_single_link_node_t_ *next_ptr;    
//}MMI_SINGLE_LINK_NODE_T;
//
//typedef struct _mmi_double_link_node_t_
//{
//    MMI_LINK_BASE_NODE_T node_base; 
//    struct _mmi_double_link_node_t_ *next_ptr;
//    struct _mmi_double_link_node_t_ *prev_ptr;    
//}MMI_DOUBLE_LINK_NODE_T;
//   

typedef struct _mmi_link_node_t_
{
    uint32 data;
    struct _mmi_link_node_t_ *next_ptr;
    struct _mmi_link_node_t_ *prev_ptr;    
}MMI_LINK_NODE_T;

/* find condition function */
typedef BOOLEAN (*MMILINK_SEARCHCONDITION)(MMI_LINK_NODE_T const* node_ptr, uint32 condition1, uint32 condition2);
/* data destruct function for user. */
typedef void (*MMILINK_DATADESTRUCTFUNC)(MMI_LINK_NODE_T const * node_ptr);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   create a link head according list type
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_CreateHeadEx(
                                             uint32 data,
                                             const char*  file_name_ptr,
                                             uint32       file_line
                                             );

#define MMILINK_CreateHead( data ) \
    MMILINK_CreateHeadEx( data, _D_FILE_NAME, _D_FILE_LINE )

/*****************************************************************************/
//  Description:   add a node after base node
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_AddNodeAfterBaseNode(MMI_LINK_NODE_T* base_node_ptr, MMI_LINK_NODE_T* add_node_ptr);
/*****************************************************************************/
//  Description:   add a node before base node
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_AddNodeBeforeBaseNode(MMI_LINK_NODE_T* base_node_ptr, MMI_LINK_NODE_T* add_node_ptr);
/*****************************************************************************/
//  Description:   create node
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_CreateNodeEx(
                                             uint32       data,
                                             const char*  file_name_ptr,
                                             uint32       file_line
                                             );

#define MMILINK_CreateNode( data ) \
    MMILINK_CreateNodeEx( data, _D_FILE_NAME, _D_FILE_LINE )

/*****************************************************************************/
//  Description:   destroy node
//	Global resource dependence: 
//  Author: gang.tong
//  Return: the header pointer of link
//	Note: If the input node_ptr is header, the node_ptr->next_ptr will be return as header of the link
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_DestroyNode(MMI_LINK_NODE_T* node_ptr, MMI_LINK_NODE_T* link_head_ptr, MMILINK_DATADESTRUCTFUNC destroy_func);

/*****************************************************************************/
//  Description:   remove a node to list;
//	Global resource dependence: 
//  Author: gang.tong
//  Return: the header pointer of link
//	Note: If the input node_ptr is header, the node_ptr->next_ptr will be return as header of the link
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_RemoveNode(MMI_LINK_NODE_T* node_ptr, MMI_LINK_NODE_T* link_head_ptr);

/*****************************************************************************/
//  Description:   find a node with condition
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_SearchNode(MMI_LINK_NODE_T const* node_ptr, BOOLEAN towards_next, MMILINK_SEARCHCONDITION condition_func, uint32 condition1, uint32 condition2);

/*****************************************************************************/
//  Description:   destroy link
//	Global resource dependence: 
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC void MMILINK_DestroyLink(MMI_LINK_NODE_T * link_head_ptr, MMILINK_DATADESTRUCTFUNC data_destruct_func);

/*****************************************************************************/
//  Description:   get list node's number
//	Global resource dependence: 
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMILINK_GetListNodesNum(MMI_LINK_NODE_T const* head_node_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif 

