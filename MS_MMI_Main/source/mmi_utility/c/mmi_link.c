/******************************************************************************
 ** File Name:      mmi_link.c                                               *
 ** Author:         gang.tong                                              *
 ** DATE:           05/15/2008                                                *
 ** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 large block memory including malloc and free              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/15/2008     gang.tong        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_link.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                                                             */
/*---------------------------------------------------------------------------*/
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
                                             )
{	
    MMI_LINK_NODE_T* head_ptr = PNULL;
    /* create a node as head node */
    head_ptr = MMILINK_CreateNodeEx(data, file_name_ptr, file_line);
    if(PNULL != head_ptr)
    {
        /* init link pointer */
        head_ptr->next_ptr = head_ptr;
        head_ptr->prev_ptr = head_ptr;         
    }
    return head_ptr;
}


/*****************************************************************************/
//  Description:   add a node after special node
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
LOCAL void AddNodeAfterBase(MMI_LINK_NODE_T* base_node_ptr, MMI_LINK_NODE_T* insert_node_ptr)
{        
    MMI_LINK_NODE_T* after_node_ptr = PNULL;      
    /* get the pointer of the node which before base node */
    after_node_ptr = base_node_ptr->next_ptr;  
    /* set pointer of insert and base node */
    insert_node_ptr->prev_ptr = base_node_ptr;
    insert_node_ptr->next_ptr = base_node_ptr->next_ptr;
    base_node_ptr->next_ptr = insert_node_ptr;
    insert_node_ptr->next_ptr->prev_ptr = insert_node_ptr;                       
}

/*****************************************************************************/
//  Description:   add a node before special node;
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
//LOCAL void AddNodeBeforeBase(MMI_LINK_NODE_T* base_node_ptr, MMI_LINK_NODE_T* insert_node_ptr)
//{    
//    MMI_LINK_NODE_T* before_node_ptr = PNULL;    
//    SCI_ASSERT(MMI_DOUBLE_BREAK_LINKED == base_node_ptr->node_base.link_type || MMI_DOUBLE_CIRCLE_LINKED == base_node_ptr->node_base.link_type);
//    SCI_ASSERT(MMI_DOUBLE_BREAK_LINKED == insert_node_ptr->node_base.link_type || MMI_DOUBLE_CIRCLE_LINKED == insert_node_ptr->node_base.link_type);
//    SCI_ASSERT(base_node_ptr->node_base.link_type == insert_node_ptr->node_base.link_type);
//    /* get the pointer of the node which before base node */
//    before_node_ptr = base_node_ptr->prev_ptr;
//
//    if(NULL == before_node_ptr)
//    {
//        /* the link type of base node must be double no circle linked, for that the "prev_ptr" can be available */
//        SCI_ASSERT(MMI_DOUBLE_BREAK_LINKED == base_node_ptr->node_base.link_type);
//        /* set every param of inserted node */
//        insert_node_ptr->node_base.link_type = MMI_DOUBLE_BREAK_LINKED;
//        insert_node_ptr->prev_ptr = PNULL;
//        insert_node_ptr->next_ptr = base_node_ptr;
//        /* set base node previous pointer */
//        base_node_ptr->prev_ptr = insert_node_ptr;
//    }
//    else
//    {
//        /* the link type of base node must be double circle linked, for that the "prev_ptr" can be available */
//        SCI_ASSERT(MMI_DOUBLE_CIRCLE_LINKED == base_node_ptr->node_base.link_type);
//        /* set every param of inserted node */
//        insert_node_ptr->node_base.link_type = MMI_DOUBLE_CIRCLE_LINKED;
//        insert_node_ptr->prev_ptr = before_node_ptr;
//        insert_node_ptr->next_ptr = base_node_ptr;
//        /* set base node previous pointer */
//        base_node_ptr->prev_ptr = insert_node_ptr;
//        /* set before node next pointer */
//        before_node_ptr->next_ptr = insert_node_ptr;
//    }
//}

/*****************************************************************************/
//  Description:   add a node as a head
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
//LOCAL void AddNodeToHead(MMI_LINK_NODE_T* base_node_ptr, MMI_LINK_NODE_T* insert_node_ptr)
//{        
//    MMI_LINK_NODE_T* head_node_ptr = PNULL;      
//    /* get the pointer of the node which before base node */
//    head_node_ptr = (MMI_LINK_NODE_T*)(((MMI_LINK_NODE_T*)(base_node_ptr))->next_ptr);
//
//    if(NULL == head_node_ptr)
//    {
//        /* the link type of base node must be no circle linked */
//        SCI_ASSERT(MMI_DOUBLE_BREAK_LINKED == base_node_ptr->link_type || MMI_SINGLE_BREAK_LINKED == base_node_ptr->link_type);        
//        SCI_ASSERT(insert_node_ptr->link_type == base_node_ptr->link_type);
//        switch(base_node_ptr->link_type)
//        {
//        case MMI_DOUBLE_BREAK_LINKED:            
//            ((MMI_LINK_NODE_T*)(base_node_ptr))->next_ptr = (MMI_LINK_NODE_T*)insert_node_ptr;
//            ((MMI_LINK_NODE_T*)(insert_node_ptr))->prev_ptr = (MMI_LINK_NODE_T*)base_node_ptr;
//            ((MMI_LINK_NODE_T*)(insert_node_ptr))->next_ptr = PNULL;
//            break;
//        case MMI_SINGLE_BREAK_LINKED:
//            ((MMI_LINK_NODE_T*)(base_node_ptr))->next_ptr = (MMI_LINK_NODE_T*)insert_node_ptr;            
//            ((MMI_LINK_NODE_T*)(insert_node_ptr))->next_ptr = PNULL;
//            break;
//        default:
//            SCI_ALLOC(0);
//            break;
//        }
//    }
//    else
//    {
//        /* the link type of base node must be double linked, for that the "prev_ptr" can be available */
//        SCI_ASSERT(MMI_DOUBLE_CIRCLE_LINKED == base_node_ptr->link_type || MMI_SINGLE_CIRCLE_LINKED == base_node_ptr->link_type);        
//        SCI_ASSERT(insert_node_ptr->link_type == base_node_ptr->link_type);
//
//        switch(base_node_ptr->link_type)
//        {
//        case MMI_DOUBLE_CIRCLE_LINKED:            
//            ((MMI_LINK_NODE_T*)(insert_node_ptr))->prev_ptr = (MMI_LINK_NODE_T*)base_node_ptr;
//            ((MMI_LINK_NODE_T*)(insert_node_ptr))->next_ptr = ((MMI_LINK_NODE_T*)(base_node_ptr))->next_ptr;
//            ((MMI_LINK_NODE_T*)(base_node_ptr))->next_ptr = (MMI_LINK_NODE_T*)insert_node_ptr;
//            ((MMI_LINK_NODE_T*)(insert_node_ptr))->next_ptr->prev_ptr = (MMI_LINK_NODE_T*)insert_node_ptr;            
//            break;
//        case MMI_SINGLE_CIRCLE_LINKED:
//            ((MMI_LINK_NODE_T*)(insert_node_ptr))->next_ptr = ((MMI_LINK_NODE_T*)(base_node_ptr))->next_ptr;
//            ((MMI_LINK_NODE_T*)(base_node_ptr))->next_ptr = (MMI_LINK_NODE_T*)insert_node_ptr;            
//            break;
//        default:
//            SCI_ALLOC(0);
//            break;
//        }        
//    }
//}

/*****************************************************************************/
//  Description:   catenate two break link
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
///*****************************************************************************/
//PUBLIC void MMILINK_BreakLinkCat(MMI_LINK_NODE_T* link1_head_ptr, MMI_LINK_NODE_T * link2_head_ptr)
//{
//    MMI_LINK_NODE_T *link_tail_ptr = PNULL;
//    SCI_ASSERT(PNULL != link1_head_ptr);
//    SCI_ASSERT(PNULL != link2_head_ptr);    
//    /* get the tail of link1 */
//    link_tail_ptr = MMILINK_GetBreakLinkEnd(link1_head_ptr, MMI_LINK_TAIL);
//    
//    /* catenate the tail of link1 and the head of link2 */
//    switch(link_tail_ptr->link_type)
//    {
//    case MMI_DOUBLE_BREAK_LINKED:
//        SCI_ASSERT(PNULL == ((MMI_LINK_NODE_T*)(link_tail_ptr))->next_ptr);        
//        SCI_ASSERT(PNULL == ((MMI_LINK_NODE_T*)(link2_head_ptr))->prev_ptr);        
//        ((MMI_LINK_NODE_T*)(link_tail_ptr))->next_ptr = (MMI_LINK_NODE_T*)link2_head_ptr;
//        ((MMI_LINK_NODE_T*)(link2_head_ptr))->prev_ptr = (MMI_LINK_NODE_T*)link_tail_ptr;            
//        break;
//    case MMI_SINGLE_BREAK_LINKED:
//        SCI_ASSERT(PNULL == ((MMI_LINK_NODE_T*)(link_tail_ptr))->next_ptr);        
//        ((MMI_LINK_NODE_T*)(link_tail_ptr))->next_ptr = (MMI_LINK_NODE_T*)link2_head_ptr;                  
//        break;
//    default:
//        SCI_ALLOC(0);
//        break;           
//    }        
//}

/*****************************************************************************/
//  Description:   add a node after base node
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_AddNodeAfterBaseNode(MMI_LINK_NODE_T* base_node_ptr, MMI_LINK_NODE_T* add_node_ptr)
{    
    SCI_ASSERT(PNULL != base_node_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != add_node_ptr);  /*assert verified*/
    
    /* whether the node will be added to head or tail of this link, the processing is same. */        
    AddNodeAfterBase(base_node_ptr, add_node_ptr);     
    return base_node_ptr;
}

/*****************************************************************************/
//  Description:   add a node before base node
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_AddNodeBeforeBaseNode(MMI_LINK_NODE_T* base_node_ptr, MMI_LINK_NODE_T* add_node_ptr)
{
    MMI_LINK_NODE_T *link_last_ptr = PNULL;
    SCI_ASSERT(PNULL != base_node_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != add_node_ptr);  /*assert verified*/
    
    /* get last node */
    link_last_ptr = base_node_ptr->prev_ptr;
    /* whether the node will be added to head or tail of this link, the processing is same. */        
    AddNodeAfterBase(link_last_ptr, add_node_ptr);        
    
    return base_node_ptr;
}
//
///*****************************************************************************/
////  Description:   add a node to list;
////	Global resource dependence: 
////  Author: gang.tong
////	Note:
///*****************************************************************************/
//PUBLIC void MMILINK_InsertNode(MMI_LINK_NODE_T* base_node_ptr, MMI_LINK_NODE_T* insert_node_ptr)
//{	    
//    SCI_ASSERT(PNULL != base_node_ptr);
//    SCI_ASSERT(PNULL != insert_node_ptr);
//
//    SCI_ASSERT(base_node_ptr->link_type < MMI_LINK_TYPE_MAX);
//    SCI_ASSERT(base_node_ptr->link_type == insert_node_ptr->link_type);    
//
//    /* add node behind the base node */
//    AddNodeAfterBase(base_node_ptr, insert_node_ptr);
//
////    switch(insert_type)
////    {
////    case MMI_BEFORE_BASE_NODE:
////        AddNodeBeforeBase(base_node_ptr, insert_node_ptr);
////        break;
////    case MMI_AFTER_BASE_NODE:
////        AddNodeAfterBase(base_node_ptr, insert_node_ptr);
////        break;
////    default:
////        SCI_ASSERT(0);
////        break;
////    }
//}

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
                                             )
{
    MMI_LINK_NODE_T* node_ptr = PNULL;        

    /* create a double link node */
    node_ptr = SCI_MallocApp(sizeof(MMI_LINK_NODE_T), file_name_ptr, file_line );   /*lint !e26 !e64*/
    
    SCI_ASSERT(PNULL != node_ptr); /*assert verified*/

    SCI_MEMSET(node_ptr, 0, sizeof(MMI_LINK_NODE_T));
    
    /* init node pointer */
    node_ptr->data = data;
    
    return node_ptr;
}

/*****************************************************************************/
//  Description:   destroy node
//	Global resource dependence: 
//  Author: gang.tong
//  Return: the header pointer of link
//	Note: If the input node_ptr is header, the node_ptr->next_ptr will be return as header of the link
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_DestroyNode(MMI_LINK_NODE_T* node_ptr, MMI_LINK_NODE_T* link_head_ptr, MMILINK_DATADESTRUCTFUNC destroy_func)
{    
    /* ger rid of node from the link its belong to */
    link_head_ptr = MMILINK_RemoveNode(node_ptr, link_head_ptr);
    /* free data of node */
    if(PNULL != destroy_func)
    {
        destroy_func(node_ptr);
    }
    /* free node memory */
    SCI_FREE(node_ptr);    
    return link_head_ptr;
}

/*****************************************************************************/
//  Description:   remove a node from double link
//	Global resource dependence: 
//  Author: gang.tong
//  Return: the header pointer of link
//	Note: If the input node_ptr is header, the node_ptr->next_ptr will be return as header of the link
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T* RemoveDoubleLinkNode(MMI_LINK_NODE_T* node_ptr, MMI_LINK_NODE_T* link_head_ptr)
{
    MMI_LINK_NODE_T* prev_node_ptr = node_ptr->prev_ptr;
    MMI_LINK_NODE_T* next_node_ptr = node_ptr->next_ptr;
    MMI_LINK_NODE_T* return_ptr = PNULL;
    SCI_ASSERT(PNULL != prev_node_ptr && PNULL != next_node_ptr); /*assert verified*/
    if(node_ptr != link_head_ptr && link_head_ptr->next_ptr != link_head_ptr)
    {
        prev_node_ptr->next_ptr = next_node_ptr;        
        next_node_ptr->prev_ptr = prev_node_ptr;
        return_ptr = link_head_ptr;
    }
    else if(node_ptr == link_head_ptr && link_head_ptr->next_ptr != link_head_ptr)
    {
        prev_node_ptr->next_ptr = next_node_ptr;        
        next_node_ptr->prev_ptr = prev_node_ptr;
        return_ptr = next_node_ptr;
    }
    else if(link_head_ptr->next_ptr == link_head_ptr)
    {
        return_ptr = PNULL;   
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }

    /* reset pointer */
    node_ptr->next_ptr = PNULL;
    node_ptr->prev_ptr = PNULL;

    /* return head ptr */
    return return_ptr;    
}
/*****************************************************************************/
//  Description:   remove a node from single link;
//	Global resource dependence: 
//  Author: gang.tong
//  Return: the header pointer of link
//	Note: If the input node_ptr is header, the node_ptr->next_ptr will be return as header of the link
/*****************************************************************************/
//LOCAL MMI_LINK_NODE_T* RemoveSingleLinkNode(MMI_LINK_NODE_T* node_ptr, MMI_LINK_NODE_T* owner_link_head_ptr)
//{
//    MMI_LINK_NODE_T* move_node_ptr = owner_link_head_ptr;
//    /* find the previous one of node_ptr, then remove the next of the previous one */
//    if(MMI_SINGLE_BREAK_LINKED == owner_link_head_ptr->node_base.link_type)
//    {
//        if(node_ptr == owner_link_head_ptr)
//        {
//            /* reset pointer */
//            node_ptr->next_ptr = PNULL;
//            return move_node_ptr->next_ptr;
//        }
//        else
//        {
//            while((node_ptr != move_node_ptr->next_ptr) && (owner_link_head_ptr != move_node_ptr->next_ptr))
//            {
//                move_node_ptr = move_node_ptr->next_ptr;
//                if(PNULL == move_node_ptr)
//                {
//                    /* the node is not in this link */
//                    SCI_ASSERT(0);
//                }
//            }
//        }
//    }
//    else if(MMI_SINGLE_CIRCLE_LINKED == owner_link_head_ptr->node_base.link_type)
//    {
//        while((node_ptr != move_node_ptr->next_ptr) && (owner_link_head_ptr != move_node_ptr->next_ptr))
//        {
//            move_node_ptr = move_node_ptr->next_ptr;     
//        }
//    }
//    else
//    {
//        SCI_ASSERT(0);
//    }
//
//    /* found or not found */
//    if(node_ptr == move_node_ptr->next_ptr)
//    {
//        move_node_ptr->next_ptr = node_ptr->next_ptr;
//        if(node_ptr == owner_link_head_ptr)
//        {
//            owner_link_head_ptr = owner_link_head_ptr->next_ptr;
//        }        
//    }
//    else
//    {
//        /* the node is not in this link */
//        SCI_ASSERT(0);
//    }
//    
//    /* reset pointer */
//    node_ptr->next_ptr = PNULL;
//    return owner_link_head_ptr;
//}

/*****************************************************************************/
//  Description:   remove a node to list;
//	Global resource dependence: 
//  Author: gang.tong
//  Return: the header pointer of link
//	Note: If the input node_ptr is header, the node_ptr->next_ptr will be return as header of the link
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_RemoveNode(MMI_LINK_NODE_T* node_ptr, MMI_LINK_NODE_T* link_head_ptr)
{    
    SCI_ASSERT(PNULL != node_ptr);      /*assert verified*/
    SCI_ASSERT(PNULL != link_head_ptr); /*assert verified*/
    
    link_head_ptr = (MMI_LINK_NODE_T*)RemoveDoubleLinkNode((MMI_LINK_NODE_T*)node_ptr, (MMI_LINK_NODE_T*)link_head_ptr);        
        
    return link_head_ptr;
}

/*****************************************************************************/
//  Description:   get break list head or tail base node pointer;
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
//PUBLIC MMI_LINK_NODE_T* MMILINK_GetBreakLinkEnd(MMI_LINK_NODE_T const * node_ptr, MMI_LINK_END_TYPE_E end_type)
//{
//    MMI_LINK_NODE_T *double_link_node_ptr = (MMI_LINK_NODE_T*)node_ptr;
//    MMI_LINK_NODE_T *single_link_node_ptr = (MMI_LINK_NODE_T*)node_ptr;
//
//    SCI_ASSERT(PNULL != node_ptr);    
//    if(MMI_LINK_HEAD == end_type)
//    {
//        /* get head */
//        SCI_ASSERT(MMI_DOUBLE_BREAK_LINKED == node_ptr->link_type);        
//        while(PNULL != double_link_node_ptr->prev_ptr)
//        {
//            double_link_node_ptr = double_link_node_ptr->prev_ptr;
//        }        
//        return (MMI_LINK_NODE_T*)double_link_node_ptr;
//    }
//    else
//    {
//        /* get tail */
//        SCI_ASSERT(MMI_DOUBLE_BREAK_LINKED == node_ptr->link_type || MMI_SINGLE_BREAK_LINKED == node_ptr->link_type);
//        while(PNULL != single_link_node_ptr->next_ptr)
//        {
//            single_link_node_ptr = single_link_node_ptr->next_ptr;
//        }   
//        return (MMI_LINK_NODE_T*)single_link_node_ptr;
//    }
//}



/*****************************************************************************/
//  Description:   find a node with condition
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
//PUBLIC MMI_LINK_NODE_T* MMILINK_SearchNode(MMI_LINK_NODE_T const* node_ptr, MMILINK_SearchCondition condition_func, uint32 condition1, uint32 condition2)
//{
//    BOOLEAN is_found = FALSE;
//    MMI_LINK_NODE_T *move_node_ptr = (MMI_LINK_NODE_T*)node_ptr;
//    MMI_LINK_NODE_T *next_node_ptr = PNULL;
//    SCI_ASSERT(PNULL != node_ptr);
//    //SCI_ASSERT(PNULL != condition_func);
//    do 
//    {
//        /* record the next pointer of move_node_ptr before deal with it, 
//            avoid the node pointed by move_node_ptr will be deleted in CallBack function. */
//        next_node_ptr = move_node_ptr->next_ptr;
//        /* invoke find condition function, the judgement is in it that whether the node be match */
//        if(PNULL != condition_func)
//        {
//            is_found = condition_func((MMI_LINK_NODE_T*)move_node_ptr, condition1, condition2);
//        }
//        else
//        {
//            is_found = (BOOLEAN)(condition1 == (((MMI_LINK_NODE_T*)move_node_ptr)->data));                
//        }
//        if(is_found)
//        {
//            break;
//        }
//        /* to next */
//    	move_node_ptr = next_node_ptr;
//    }while(PNULL != move_node_ptr && (MMI_LINK_NODE_T*)move_node_ptr != node_ptr);
//            
//    if(is_found)
//    {
//        /* found */
//        return (MMI_LINK_NODE_T*)move_node_ptr;        
//    }
//    else
//    {
//        /* not found */
//        return PNULL;
//    }
//}

/*****************************************************************************/
//  Description:   get list node's number
//	Global resource dependence: 
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMILINK_GetListNodesNum(MMI_LINK_NODE_T const* head_node_ptr)
{
    uint32 node_num = 0;
    MMI_LINK_NODE_T *cur_node_ptr = PNULL;

    if (head_node_ptr == PNULL)
    {
        return 0;
    }
    cur_node_ptr = (MMI_LINK_NODE_T *)head_node_ptr;
    do 
    {
        node_num++;
        cur_node_ptr = cur_node_ptr->next_ptr;
    }while (cur_node_ptr != head_node_ptr);

    return node_num;
}

/*****************************************************************************/
//  Description:   find a node with condition
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T* MMILINK_SearchNode(MMI_LINK_NODE_T const* node_ptr, BOOLEAN towards_next, MMILINK_SEARCHCONDITION condition_func, uint32 condition1, uint32 condition2)
{
    BOOLEAN is_found = FALSE;
    MMI_LINK_NODE_T *move_node_ptr = (MMI_LINK_NODE_T*)node_ptr;
    MMI_LINK_NODE_T *next_node_ptr = PNULL;
    SCI_ASSERT(PNULL != node_ptr);           /*assert verified*/

    do 
    {
        /* record the next pointer of move_node_ptr before deal with it, 
            avoid the node pointed by move_node_ptr will be deleted in CallBack function. */
        next_node_ptr = (towards_next)?(move_node_ptr->next_ptr):(move_node_ptr->prev_ptr);                
        //SCI_TRACE_LOW("[LINK]:MMILINK_SearchNode:move_node_ptr = 0x%x, next_node_ptr = 0x%x;", move_node_ptr, next_node_ptr); 
        /* invoke find condition function, the judgement is in it that whether the node be match */
        if(PNULL != condition_func)
        {
            is_found = condition_func((MMI_LINK_NODE_T*)move_node_ptr, condition1, condition2);
        }
        else
        {
            is_found = (BOOLEAN)(condition1 == (((MMI_LINK_NODE_T*)move_node_ptr)->data));                
        }                    

        if(is_found)
        {
            //SCI_TRACE_LOW("[LINK]:MMILINK_SearchNode:is_found break;"); 
            break;
        }
        
        if(PNULL == next_node_ptr->next_ptr)
        {
            //SCI_TRACE_LOW("[LINK]:MMILINK_SearchNode:PNULL == next_node_ptr->next_ptr break;"); 
            /* mean the next_node_ptr is a released node. */
            break;
        }        

        /* if the move_node_ptr be removed from the link, so the node_ptr as head must be set to next node. */
        if(node_ptr == move_node_ptr && PNULL == move_node_ptr->next_ptr)
        {
            node_ptr = next_node_ptr;
            /* to next or previous */
            move_node_ptr = next_node_ptr;        
            continue;
        }        
        /* to next or previous */
        move_node_ptr = next_node_ptr;        

        if(move_node_ptr == node_ptr)
        {
            //SCI_TRACE_LOW("[LINK]:MMILINK_SearchNode:move_node_ptr = 0x%x, node_ptr = 0x%x break;", move_node_ptr, node_ptr); 
            break;
        }

    }while(PNULL != move_node_ptr);
            
    //SCI_TRACE_LOW("[LINK]:MMILINK_SearchNode: while end:move_node_ptr = 0x%x, node_ptr = 0x%x;", move_node_ptr, node_ptr); 
    if(is_found)
    {
        /* found */
        return move_node_ptr;        
    }
    else
    {
        /* not found */
        return PNULL;
    }
}

/*****************************************************************************/
//  Description:   destroy link
//	Global resource dependence: 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC void MMILINK_DestroyLink(MMI_LINK_NODE_T * link_head_ptr, MMILINK_DATADESTRUCTFUNC data_destruct_func)
{        
    MMI_LINK_NODE_T *move_node_ptr = (MMI_LINK_NODE_T*)link_head_ptr;
    MMI_LINK_NODE_T *delete_node_ptr = PNULL;
    SCI_ASSERT(PNULL != link_head_ptr);     /*assert verified*/
    do 
    {
        delete_node_ptr = move_node_ptr;
        move_node_ptr = move_node_ptr->next_ptr;                 
        if(PNULL != data_destruct_func)
        {
            /* invoke the function, so that the user has chance to deal with data. */
            data_destruct_func((MMI_LINK_NODE_T*)delete_node_ptr);
        }                        
        /* reset node content */        
        SCI_MEMSET(delete_node_ptr, 0, sizeof(MMI_LINK_NODE_T));            
        /* free node buffer  */
        SCI_FREE(delete_node_ptr);
        
    }while(PNULL != move_node_ptr && (MMI_LINK_NODE_T*)move_node_ptr != link_head_ptr);
}
/*
BOOLEAN foundfunc(MMI_LINK_NODE_T const * node_ptr)
{
       return (node_ptr->data == 5);
}

void DataDestructFunc(MMI_LINK_NODE_T const * node_ptr)
{
       node_ptr->data;
}

LOCAL MMI_LINK_NODE_T* s_head_ptr = PNULL;
void MMIAPILINK_test(void)
{
    MMI_LINK_TYPE_E link_type = MMI_DOUBLE_BREAK_LINKED;
    uint32 i = 0;
    
    MMI_LINK_NODE_T* node_ptr = PNULL;
    s_head_ptr = MMILINK_CreateHead(link_type, 1);

    for(i = 0; i < 10 ; i++)
    {
        node_ptr = MMILINK_CreateNode(link_type, i +2);
        s_head_ptr = MMILINK_AddNodeToLinkEnd(s_head_ptr, node_ptr, MMI_LINK_HEAD);
        //MMILINK_InsertNode(s_head_ptr, node_ptr);
    }

    node_ptr = MMILINK_FindNode(s_head_ptr, foundfunc);
    
    MMILINK_InsertNode(node_ptr, MMILINK_CreateNode(link_type, 33));


    MMILINK_DestroyLink(s_head_ptr, DataDestructFunc);
}
*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

