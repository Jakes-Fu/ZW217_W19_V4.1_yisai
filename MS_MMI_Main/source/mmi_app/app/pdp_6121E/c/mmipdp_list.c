/****************************************************************************************
** File Name:      mmipdp_list.c                                                        *
** Author:         juan.zhang                                                           *
** Date:           2009.5.27                                                            *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is the simplification of DAPS CFL_list.c                   *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.5        juan.zhang         Create                                              *
**                                                                                      *
*****************************************************************************************/
/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmipdp_list.h"
#include "os_api.h"
/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef struct _MMIPDP_LIST_NODE_TAG
{
    MMIPDP_LIST_NODE_HANDLE next_node_ptr;
    MMIPDP_LIST_NODE_HANDLE prev_node_ptr;
    MMIPDP_LIST_HANDLE      list_handler; //the list which this node is in
    MMIPDP_NODE_OBJECT      data;
}MMIPDP_LIST_NODE_T;

typedef struct
{
    MMIPDP_LIST_NODE_HANDLE list_head;
    MMIPDP_LIST_NODE_HANDLE list_tail;
    uint32                  node_data_size;
    uint32                  cur_node_num;
}MMIPDP_LIST_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/
/****************************************************************************/
//  Description : Creat a list
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note: if the list's max node num is ZERO, the nodes to be inserted
//          in this list is unlimited
/****************************************************************************/
PUBLIC MMIPDP_LIST_HANDLE MMIPDP_ListCreat(uint32 node_data_size)
{
    MMIPDP_LIST_T *list_created_ptr = 0;
    
    if(node_data_size > 0)
    {
        list_created_ptr = SCI_ALLOCA(sizeof(MMIPDP_LIST_T));
        if(list_created_ptr != 0)
        {
            SCI_MEMSET(list_created_ptr,0,sizeof(MMIPDP_LIST_T));
            list_created_ptr->node_data_size = node_data_size;
        }
    }
    return (MMIPDP_LIST_HANDLE)list_created_ptr;
}

/****************************************************************************/
//  Description : Destroy a list
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
PUBLIC void MMIPDP_ListDestroy(MMIPDP_LIST_HANDLE list_handler)
{
    MMIPDP_LIST_T   *list_ptr = (MMIPDP_LIST_T*)list_handler;
    
    if(list_ptr != 0)
    {
        while(list_ptr->list_head != 0)
        {
            MMIPDP_ListDeleteNode(list_handler, list_ptr->list_head);
        }
        SCI_FREE(list_ptr);
    }
}

/****************************************************************************/
//  Description : To insert a node after the indicated node. If the indicated 
//  node was null, the new node would be inserted to be the head node
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListInsertNode(
                                                     MMIPDP_LIST_HANDLE         list_handler, 
                                                     MMIPDP_LIST_NODE_HANDLE        after_which_handler,
                                                     MMIPDP_NODE_OBJECT         new_node_data,
                                                     uint32                     data_size
                                                     )
{
    MMIPDP_LIST_NODE_T          *new_node_inserted = 0;
    uint32                      new_node_size = sizeof(MMIPDP_LIST_NODE_T) + data_size;
    MMIPDP_LIST_NODE_T          *temp_node_ptr = 0;
    
    if(list_handler != 0)
    {
        //if(data_size == ((MMIPDP_LIST_T*)list_handler)->node_data_size)
        {
            //creat a new node
            new_node_inserted = SCI_ALLOCA(new_node_size);
            if(new_node_inserted != 0)
            {
                SCI_MEMSET(new_node_inserted,0,new_node_size);
                new_node_inserted->list_handler = list_handler;
                new_node_inserted->data = (uint32*)new_node_inserted+sizeof(MMIPDP_LIST_NODE_T)/sizeof(uint32);
                SCI_MEMCPY(new_node_inserted->data,new_node_data,data_size);
                //to insert as the head node of the list
                if(0 == after_which_handler)
                {
                    new_node_inserted->prev_node_ptr = 0;
                    new_node_inserted->next_node_ptr = ((MMIPDP_LIST_T*)list_handler)->list_head;
                    if(((MMIPDP_LIST_T*)list_handler)->list_head != 0)
                    {
                        temp_node_ptr = (MMIPDP_LIST_NODE_T*)((MMIPDP_LIST_T*)list_handler)->list_head;
                        temp_node_ptr->prev_node_ptr = (MMIPDP_LIST_NODE_HANDLE)new_node_inserted;
                    }
                    //change the record in list in special cases
                    ((MMIPDP_LIST_T*)list_handler)->list_head = (MMIPDP_LIST_NODE_HANDLE)new_node_inserted;                 
                }
                //normal case, to insert after the indicated one
                else
                {
                    new_node_inserted->prev_node_ptr = after_which_handler;
                    temp_node_ptr = (MMIPDP_LIST_NODE_T*)after_which_handler;
                    new_node_inserted->next_node_ptr = temp_node_ptr->next_node_ptr;
                    temp_node_ptr->next_node_ptr = (MMIPDP_LIST_NODE_HANDLE)new_node_inserted;
                    if(new_node_inserted->next_node_ptr != 0)
                    {
                        temp_node_ptr = (MMIPDP_LIST_NODE_T*)new_node_inserted->next_node_ptr;
                        temp_node_ptr->prev_node_ptr = (MMIPDP_LIST_NODE_HANDLE)new_node_inserted;
                    }
                }
                //change the record in list in special cases
                if(after_which_handler == ((MMIPDP_LIST_T*)list_handler)->list_tail)
                {
                    ((MMIPDP_LIST_T*)list_handler)->list_tail = (MMIPDP_LIST_NODE_HANDLE)new_node_inserted;
                }
                ((MMIPDP_LIST_T*)list_handler)->cur_node_num++;
            }
        }
    }
    return (MMIPDP_LIST_NODE_HANDLE)new_node_inserted;
}

/****************************************************************************/
//  Description : To delete a node from the indicated list
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
PUBLIC void MMIPDP_ListDeleteNode(
                                  MMIPDP_LIST_HANDLE        list_handler, 
                                  MMIPDP_LIST_NODE_HANDLE   to_delete_node_handler
                                  )
{
    MMIPDP_LIST_NODE_T *node_ptr = 0;
    MMIPDP_LIST_NODE_T *temp_node_ptr = 0;
    
    if(list_handler!=0 && to_delete_node_handler!=0)
    {
        node_ptr = (MMIPDP_LIST_NODE_T*)to_delete_node_handler;
        //if the node is the head node of the list
        if(0 == node_ptr->prev_node_ptr)
        {
            ((MMIPDP_LIST_T*)list_handler)->list_head = node_ptr->next_node_ptr;
            if(node_ptr->next_node_ptr != 0)
            {
                temp_node_ptr = (MMIPDP_LIST_NODE_T*)node_ptr->next_node_ptr;
                temp_node_ptr->prev_node_ptr = 0;
            }
            else
            {
                ((MMIPDP_LIST_T*)list_handler)->list_tail = 0;
            }
        }
        //if the node is the tail node of the list
        else if(0 == node_ptr->next_node_ptr)
        {
            ((MMIPDP_LIST_T*)list_handler)->list_tail = node_ptr->prev_node_ptr;
            if(node_ptr->prev_node_ptr!= 0)
            {
                temp_node_ptr = (MMIPDP_LIST_NODE_T*)node_ptr->prev_node_ptr;
                temp_node_ptr->next_node_ptr = 0;
            }
        }
        else
        {
            temp_node_ptr = (MMIPDP_LIST_NODE_T*)node_ptr->prev_node_ptr;
            temp_node_ptr->next_node_ptr = node_ptr->next_node_ptr;
            temp_node_ptr = (MMIPDP_LIST_NODE_T*)node_ptr->next_node_ptr;
            temp_node_ptr->prev_node_ptr = node_ptr->prev_node_ptr;
        }
        ((MMIPDP_LIST_T*)list_handler)->cur_node_num --;
        SCI_FREE(node_ptr);
    }
}


/****************************************************************************/
//  Description : To find a node in the indicated list using the rules that 
//  described in the callback function
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
MMIPDP_LIST_NODE_HANDLE MMIPDP_ListFindNode(
                                            MMIPDP_LIST_HANDLE          list_handler, 
                                            void                        *find_data,
                                            MMIPDP_NODE_QUERY_CALLBACK  node_query_callback
                                            )
{
    MMIPDP_LIST_NODE_T          *node_find_ptr = 0;
    
    if(list_handler != 0)
    {
        node_find_ptr = (MMIPDP_LIST_NODE_T*)((MMIPDP_LIST_T*)list_handler)->list_head;
        
        while(node_find_ptr != 0)
        {
            if(node_query_callback(node_find_ptr->data, find_data))
            {
                break;
            }
            else
            {
                node_find_ptr = (MMIPDP_LIST_NODE_T*)node_find_ptr->next_node_ptr;
            }
        }
    }
    return (MMIPDP_LIST_HANDLE)node_find_ptr;
}

/****************************************************************************/
//  Description : to get the previous node
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListGetPrevNode(
                                                      MMIPDP_LIST_NODE_HANDLE cur_node_handler
                                                      )
{
    MMIPDP_LIST_NODE_HANDLE prev_handler = 0;
    MMIPDP_LIST_NODE_T          *cur_node_ptr = 0;
    
    if(cur_node_handler!=0)
    {
        cur_node_ptr = (MMIPDP_LIST_NODE_T*)cur_node_handler;
        prev_handler = cur_node_ptr->prev_node_ptr;
    }
    return prev_handler;
}

/****************************************************************************/
//  Description : to get the next node
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListGetNextNode(
                                                      MMIPDP_LIST_NODE_HANDLE cur_node_handler
                                                      )
{
    MMIPDP_LIST_NODE_HANDLE next_handler = 0;
    MMIPDP_LIST_NODE_T      *cur_node_ptr = 0;
    
    if(cur_node_handler!=0)
    {
        cur_node_ptr = (MMIPDP_LIST_NODE_T*)cur_node_handler;
        next_handler = cur_node_ptr->next_node_ptr;
    }
    return next_handler;
}

/****************************************************************************/
//  Description : to get the node data
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
PUBLIC MMIPDP_NODE_OBJECT MMIPDP_ListGetNodeData(
                                                 MMIPDP_LIST_NODE_HANDLE node_handler
                                                 )
{
    MMIPDP_NODE_OBJECT node_data = 0;
    MMIPDP_LIST_NODE_T *node_ptr = 0;
    
    if(node_handler != 0)
    {
        node_ptr = (MMIPDP_LIST_NODE_T*)node_handler;
        node_data = node_ptr->data;
    }
    return node_data;
}

/****************************************************************************/
//  Description : to get the list head node
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListGetHead(MMIPDP_LIST_HANDLE list_handler)
{
    if(list_handler != 0)
    {
        return (((MMIPDP_LIST_T*)list_handler)->list_head);
    }
    else
    {
        return 0;
    }
}

/****************************************************************************/
//  Description : to get the list tail node
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
PUBLIC MMIPDP_LIST_NODE_HANDLE MMIPDP_ListGetTail(MMIPDP_LIST_HANDLE list_handler)
{
    if(list_handler != 0)
    {
        return (((MMIPDP_LIST_T*)list_handler)->list_tail);
    }
    else
    {
        return 0;
    }
}

/****************************************************************************/
//  Description : to get the the list's current node number
//  Global resource dependence : 
//  Author:juan.zhang 
//  Note:   
/****************************************************************************/
PUBLIC uint32 MMIPDP_ListGetMemberCount(MMIPDP_LIST_HANDLE list_handler)
{
    if(list_handler != 0)
    {
        return (((MMIPDP_LIST_T*)list_handler)->cur_node_num);      
    }
    else
    {
        return 0;
    }
}
#endif
