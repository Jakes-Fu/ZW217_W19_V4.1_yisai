/********************************************************************************
** File Name:   mmi_dlist.c                                                     *
** Author:      Hongbing Ju                                                     *
** Date:        06/25/2012                                                      *
** Copyright:   2003 Spreadtrum Communications Incorporated. All Right Reserved *
** Description: This file is used to describe a base data structure of doubly   *
**              linked list                                                     *
*********************************************************************************
**                            Important Edit history                            *
**-----------------------------------------------------------------------------**
** DATE            NAME            DESCRIPTION                                  *
** 06/25/2012      Hongbing Ju     Created                                      *
*********************************************************************************/

/*------------------------------------------------------------------------------*
**                            Included Files                                    *
**------------------------------------------------------------------------------*/
#include "mmi_dlist.h"

typedef struct MMI_DLISTNODE_TAG
{
    struct MMI_DLISTNODE_TAG *next;
    struct MMI_DLISTNODE_TAG *prev;
    /*
    * flexible array member whose size is defined at run-time, which helps
    * reduce the chance of memory fragmentation
    * refer to ISO/IEC 9899:1999, Section 6.7.2.1, paragraph 16
    * if your compiler is not compliant, declare it as
    * char element[1];
    */
    char                    element[];
} MMI_DLISTNODE_T;

typedef struct MMI_DLIST_TAG
{
    MMI_DLISTNODEHANDLE_T   head;
    //MMI_DLISTNODEHANDLE_T tail; no need for this due to tail = head->prev
    int32                   error;
    int32                   length;
    uint32                  capacity;
    uint32                  elementSize;
#ifdef DEBUG
    const char              *id;
#endif
} MMI_DLIST_T;


/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/
#define MMI_DLST_ERROR_MASK(err) ((err) | __LINE__ << 8)

/*------------------------------------------------------------------------------*
**                            TYPES AND CONSTANTS                               *
**------------------------------------------------------------------------------*/
#ifdef DEBUG
/* make sure both data and the pointer to the data are in read-only section */
const char * const dlist_id = "doubly_list";
#endif

/*------------------------------------------------------------------------------*
**                            STATIC DEFINITIONS                                *
**------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
**                            EXTERNAL DECLARE                                  *
**------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
**                            Constant Variables                                *
**------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
**                            GLOBAL DEFINITIONS                                *
**------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*
**                            static FUCTION DECLARE                             *
**------------------------------------------------------------------------------*/

/********************************************************************************
** Description: This function is used to create a list node                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
static MMI_DLISTNODEHANDLE_T 
MMI_DLIST_CreateNode(void * element, 
                 uint32 elementSize
                 );

/********************************************************************************
** Description: This function is used to insert an element after a list node    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
static MMI_DLISTNODEHANDLE_T 
MMI_DLIST_InsertAft(MMI_DLISTNODEHANDLE_T node, 
                void * element, 
                uint32 elementSize
                );

/********************************************************************************
** Description: This function is used to insert an element before a list node   *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
static MMI_DLISTNODEHANDLE_T 
MMI_DLIST_InsertBfr(MMI_DLISTNODEHANDLE_T node, 
                void * element, 
                uint32 elementSize
                );

/********************************************************************************
** Description: This function is used to remove a node                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
static void MMI_DLIST_Remove(MMI_DLISTNODEHANDLE_T *node);

/*------------------------------------------------------------------------------*
**                            FUNCTIONS                                         *
**------------------------------------------------------------------------------*/

/********************************************************************************
** Description: This function is used to create a list                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTHANDLE_T MMI_DLIST_Create(uint32 capacity, uint32 elementSize)
{
    MMI_DLISTHANDLE_T ret = (MMI_DLISTHANDLE_T) SCI_ALLOCA(sizeof(MMI_DLIST_T));
    /* create a head node for efficiency */
    MMI_DLISTNODEHANDLE_T node = MMI_DLIST_CreateNode("H", 2);//cid609449

    if (PNULL == ret || PNULL == node)
    {
        SCI_FREE(ret);
        SCI_FREE(node);
    }
    else
    {
        SCI_MEMSET(ret, 0x00, sizeof(MMI_DLIST_T));
        ret->head = node;
        //ret->error = 0;
        //ret->length = 0;
        ret->elementSize = elementSize;
#ifdef DEBUG
        ret->id = dlist_id;
#endif
    }

    return (ret);
}

/********************************************************************************
** Description: This function is used to destroy a list                         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Destroy(MMI_DLISTHANDLE_T *lst)
{

    MMIBD_ASSERT_DEBUG(lst && *lst);

    if (PNULL == lst || PNULL == *lst)
    {
        return;
    }

    MMI_DLIST_RemoveAll(*lst);

    SCI_FREE((*lst)->head);
    SCI_FREE(*lst);
    *lst = PNULL;
}

/********************************************************************************
** Description: This function is used to remove all nodes except head node      *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_RemoveAll(MMI_DLISTHANDLE_T lst)
{
    MMI_DLISTNODEHANDLE_T pt = PNULL;

    MMIBD_ASSERT_DEBUG(lst);

    if (PNULL == lst)
    {
        return;
    }

    pt = lst->head->next;
    while (pt != lst->head)
    {
        MMI_DLISTNODEHANDLE_T tmp = pt->next;

        SCI_FREE(pt);
        pt=tmp;
    }
    lst->head->next = lst->head->prev = lst->head;
    lst->length = 0;
}

/********************************************************************************
** Description: This function is used to prepend an element                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_Prepend(MMI_DLISTHANDLE_T lst, void * element)
{
    MMI_DLISTNODEHANDLE_T node;

    MMIBD_ASSERT_DEBUG(lst && element);

    if (PNULL == lst)
    {
        return (PNULL);
    }

    if (PNULL == element)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_NULL_ELEMENT);

        return (PNULL);
    }

    node = MMI_DLIST_InsertAft(lst->head, element, lst->elementSize);
    if (PNULL == node)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_OUT_OF_MEMORY);

        return (PNULL);
    }

    lst->length++;

    return (node);
}

/********************************************************************************
** Description: This function is used to append an element                      *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_Append(MMI_DLISTHANDLE_T lst, void * element)
{
    MMI_DLISTNODEHANDLE_T node;

    MMIBD_ASSERT_DEBUG(lst && element);

    if (PNULL == lst)
    {
        return (PNULL);
    }

    if (PNULL == element)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_NULL_ELEMENT);

        return (PNULL);
    }

    node = MMI_DLIST_InsertAft(lst->head->prev, element, lst->elementSize);
    if (PNULL == node)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_OUT_OF_MEMORY);

        return (PNULL);
    }

    lst->length++;

    return (node);
}

/********************************************************************************
** Description: This function is used to insert an element after a node         *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_InsertAfter(MMI_DLISTHANDLE_T lst, MMI_DLISTNODEHANDLE_T node, void * element)
{
    MMI_DLISTNODEHANDLE_T newNode;

    MMIBD_ASSERT_DEBUG(lst && element);

    if (PNULL == lst)
    {
        return (PNULL);
    }

    if (PNULL == element)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_NULL_ELEMENT);

        return (PNULL);
    }

    newNode = MMI_DLIST_InsertAft(node, element, lst->elementSize);
    if (PNULL == newNode)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_OUT_OF_MEMORY);

        return (PNULL);
    }

    lst->length++;

    return (newNode);
}

/********************************************************************************
** Description: This function is used to insert an element before a node        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_InsertBefore(MMI_DLISTHANDLE_T lst, MMI_DLISTNODEHANDLE_T node, void * element)
{
    MMI_DLISTNODEHANDLE_T newNode;

    MMIBD_ASSERT_DEBUG(lst && element);

    if (PNULL == lst)
    {
        return (PNULL);
    }

    if (PNULL == element)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_NULL_ELEMENT);

        return (PNULL);
    }

    newNode = MMI_DLIST_InsertBfr(node, element, lst->elementSize);
    if (PNULL == newNode)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_OUT_OF_MEMORY);

        return (PNULL);
    }

    lst->length++;

    return (newNode);
}

/********************************************************************************
** Description: This function is used to retrieve an element in head            *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Retrieve(MMI_DLISTHANDLE_T lst, void * element)
{
    MMI_DLIST_Peek(lst, element);

    if(0 != MMI_DLIST_GetCurrErrorCode(lst))
    {
        return;
    }

    MMI_DLIST_Remove(&lst->head->next);

    lst->length--;
}

/********************************************************************************
** Description: This function is used to peek an element in head                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Peek(MMI_DLISTHANDLE_T lst, void * element)
{
    MMIBD_ASSERT_DEBUG(lst && element);

    if (PNULL == lst)
    {
        return;
    }

    if (PNULL == element)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_NULL_ELEMENT);

        return;
    }

    if(0 >= lst->length)
    {
        lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_UNDERFLOW);
        return;
    }

    SCI_MEMCPY(element, lst->head->next->element, lst->elementSize);
}

/********************************************************************************
 * Delete a list entry by making the prev/next entries                          *
 * point to each other.                                                         *
 *                                                                              *
 * This is only for internal list manipulation where we know                    *
 * the prev/next entries already!                                               *
 ********************************************************************************/
static void __list_del(MMI_DLISTNODEHANDLE_T prev, MMI_DLISTNODEHANDLE_T next)
{
    next->prev = prev;
    prev->next = next;
}

/********************************************************************************
 * list_del - deletes entry from list.                                          *
 * @entry: the element to delete from the list.                                 *
 * Note: list_empty() on entry does not return true after this, the entry is    *
 * in an undefined state.                                                       *
 ********************************************************************************/
static void list_del(MMI_DLISTNODEHANDLE_T entry)
{
    __list_del(entry->prev, entry->next);
}

/********************************************************************************
** Description: This function is used to remove a node                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
static void MMI_DLIST_Remove(MMI_DLISTNODEHANDLE_T *node)
{
    MMIBD_ASSERT_DEBUG(node);

    if (node)
    {
        MMI_DLISTNODEHANDLE_T node_del = *node;
        list_del(*node);
        SCI_FREE(node_del);
    }
}

/********************************************************************************
** Description: This function is used to delete a node                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Delete(MMI_DLISTHANDLE_T lst, MMI_DLISTNODEHANDLE_T *node)
{
    MMIBD_ASSERT_DEBUG(lst && node);

    if (lst && node)
    {
        list_del(*node);
        SCI_FREE(*node);
        lst->length--;
    }
}

/********************************************************************************
** Description: This function is used to check if a list is empty or not        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_IsEmpty(MMI_DLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (!lst->length);
    }

    return (1);
}

/********************************************************************************
** Description: This function is used to check if a stack is full or not        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_IsFull(MMI_DLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->length == (int32)lst->capacity);
    }

    return (0);
}

/********************************************************************************
** Description: This function is used to get item numbers in a list             *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_GetLength(MMI_DLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->length);
    }

    return (0);
}

/********************************************************************************
** Description: This function is used to get first node in a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_DLISTNODEHANDLE_T MMI_DLIST_GetFirst(MMI_DLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->head->next);
    }

    return (PNULL);
}

/********************************************************************************
** Description: This function is used to get last node in a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                         *
** Note:                                                                        *
*********************************************************************************/
PUBLIC MMI_DLISTNODEHANDLE_T MMI_DLIST_GetLast(MMI_DLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->head->prev);
    }

    return (PNULL);
}

/********************************************************************************
** Description: This function is used to get next node in a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_GetNext(MMI_DLISTNODEHANDLE_T node)
{
    MMIBD_ASSERT_DEBUG(node);

    if (node)
    {
        return (node->next);
    }

    return (PNULL);
}

/********************************************************************************
** Description: This function is used to get previous node in a list            *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
MMI_DLISTNODEHANDLE_T MMI_DLIST_GetPrev(MMI_DLISTNODEHANDLE_T node)
{
    MMIBD_ASSERT_DEBUG(node);

    if (node)
    {
        return (node->prev);
    }

    return (PNULL);
}

/********************************************************************************
** Description: This function is used to get item data in a node                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void * MMI_DLIST_GetElement(MMI_DLISTNODEHANDLE_T node)
{
    MMIBD_ASSERT_DEBUG(node);

    if (node)
    {
        return (node->element);
    }

    return (PNULL);
}

/********************************************************************************
** Description: This function is used to replace item data in a node            *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_SetElement(MMI_DLISTNODEHANDLE_T node, void *element, uint32 element_size)
{
    MMIBD_ASSERT_DEBUG(node && element);

    if (node && element)
    {
        SCI_MEMCPY(node->element, element, element_size);
    }
}

/********************************************************************************
** Description: This function is used to traverse through a list                *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Traverse(MMI_DLISTHANDLE_T lst, MMI_DLIST_TrvFunc func)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && func)
    {
        MMI_DLISTNODEHANDLE_T pt;    
        
        for (pt = lst->head->next; pt != lst->head; pt = pt->next)
        {
            func(pt->element);
        }
    }
}

/********************************************************************************
** Description: This function is used to traverse through a list and stop when  *
**                condition meets                                                    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void* MMI_DLIST_TraverseStop(MMI_DLISTHANDLE_T lst, void *param, MMI_DLIST_TrvStopFunc func, int32 condition)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && func)
    {
        MMI_DLISTNODEHANDLE_T pt;

        for (pt = lst->head->next; pt != lst->head; pt = pt->next)
        {
            if(condition == func(pt->element, param))
            {
                return pt->element;
            }
        }
    }
    return PNULL;
}

/********************************************************************************
** Description: This function is used to traverse through a list in reverse     *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Traverse_Reverse(MMI_DLISTHANDLE_T lst, MMI_DLIST_TrvFunc func)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && func)
    {
        MMI_DLISTNODEHANDLE_T pt;

        for (pt = lst->head->prev; pt != lst->head; pt = pt->prev)
        {
            func(pt->element);
        }
    }
}

/********************************************************************************
** Description: This function is used to traverse through a list in reverse     *
**              direction, and stop if condition meets                          *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void* MMI_DLIST_TraverseStop_Reverse(MMI_DLISTHANDLE_T lst, void *param, MMI_DLIST_TrvStopFunc func, int32 condition)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && func)
    {
        MMI_DLISTNODEHANDLE_T pt;

        for (pt = lst->head->prev; pt != lst->head; pt = pt->prev)
        {
            if(condition == func(pt->element, param))
            {
                return pt->element;
            }
        }
    }
    return PNULL;
}

#ifdef DEBUG
/********************************************************************************
** Description: This function is used to validate a list                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_Validate(MMI_DLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (!strcmp(lst->id, dlist_id));
    }

    return (0);
}
#endif

/********************************************************************************
** Description: This function is used to create a list node                     *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
static MMI_DLISTNODEHANDLE_T 
MMI_DLIST_CreateNode(void * element, 
                 uint32 elementSize
                 )
{
    MMI_DLISTNODEHANDLE_T node;

    node = (MMI_DLISTNODEHANDLE_T) SCI_ALLOCA(sizeof(MMI_DLISTNODE_T) + elementSize);
    if (node)
    {
        node->next = node;
        node->prev = node;
        SCI_MEMCPY(node->element, element, elementSize);
    }

    return (node);
}

/********************************************************************************
** Description: This function is used to insert an element after a list node    *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
static MMI_DLISTNODEHANDLE_T 
MMI_DLIST_InsertAft(MMI_DLISTNODEHANDLE_T node, 
                void * element, 
                uint32 elementSize
                )
{
    if(PNULL != node)
    {
        MMI_DLISTNODEHANDLE_T newNode = MMI_DLIST_CreateNode(element, elementSize);

        if (newNode)
        {
            node->next->prev = newNode;
            newNode->next = node->next;
            newNode->prev = node;
            node->next = newNode;
        }

        return (newNode);
    }
    return PNULL;
}

/********************************************************************************
** Description: This function is used to insert an element before a list node   *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
static MMI_DLISTNODEHANDLE_T 
MMI_DLIST_InsertBfr(MMI_DLISTNODEHANDLE_T node, 
                void * element, 
                uint32 elementSize
                )
{
    if(PNULL != node)
    {
        MMI_DLISTNODEHANDLE_T newNode = MMI_DLIST_CreateNode(element, elementSize);

        if (newNode)
        {
            node->prev->next = newNode;
            newNode->next = node;
            newNode->prev = node->prev;
            node->prev = newNode;
        }

        return (newNode);
    }
    return PNULL;
}

/********************************************************************************
** Description: This function is used to sort a list with cmp_func             *
**              direction                                                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void MMI_DLIST_Sort(MMI_DLISTHANDLE_T lst, MMI_DLIST_CmpFunc func)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && lst->length && func)
    {
        MMI_DLISTNODEHANDLE_T pt;
        int32 i = 0;
        int32 length = lst->length;
        
        //create a array for qsort
        MMI_DLISTNODEHANDLE_T* pt_array = SCI_ALLOCA((uint32)lst->length * sizeof(MMI_DLISTNODEHANDLE_T));
        if(PNULL == pt_array)
        {
            lst->error = MMI_DLST_ERROR_MASK(MMI_DLST_ERROR_OUT_OF_MEMORY);
            return;
        }

        SCI_MEMSET(pt_array, 0x00, (uint32)lst->length * sizeof(MMI_DLISTNODEHANDLE_T));
        for (pt = lst->head->next; pt != lst->head; pt = pt->next)
        {
            pt_array[i++] = pt;            
        }

        qsort(pt_array, lst->length, sizeof(void *), func);

        // relink the list with new order
        for(i = 0; i < length - 1; i++)
        {
            pt_array[i]->next = pt_array[i+1];
            pt_array[i+1]->prev = pt_array[i];
        }
        // link back to virtual list head
        lst->head->next = pt_array[0];
        pt_array[0]->prev = lst->head;
        // for circular head and tail
        pt_array[length - 1]->next = lst->head;
        lst->head->prev = pt_array[length - 1];
        SCI_FREE(pt_array);
    }
}

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_DLIST_GetCurrErrorCode(MMI_DLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->error);
    }

    return (0);    
}

#ifdef _MMI_DLST_UNIT_TEST_
/********************************************************************************
** Description: This function is used to print an element value for unit test   *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void PrintElement(void * val)
{
    printf("%d\n", * (int32 *) val);
}

int32 CheckElement(void * val, void *param)
{
    if(param != PNULL)
    {
        if(* (int32 *) val == * (int32 *) param)
        {
            return 0;
        }
    }
    else if(* (int32 *) val == 4)
    {
        return 0;
    }
    return 1;
}

void DeleteElement(void *val)
{
    SCI_FREE(val);
}

int32 CompareElement(void *src, void *dst)
{
    int32 data1 = *(int32 *)MMI_DLIST_GetElement(*(MMI_DLISTNODEHANDLE_T*)src);
    int32 data2 = *(int32 *)MMI_DLIST_GetElement(*(MMI_DLISTNODEHANDLE_T*)dst);

    if(data1 > data2)
    {
        return -1;
    }
    else if(data1 < data2)
    {
        return 1;
    }
    return 0;
}

/********************************************************************************
** Description: This function is used to provide test functions for unit test   *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void main(void)
{
    MMI_DLISTHANDLE_T lst = MMI_DLIST_Create(0x7fffffff, sizeof(int32));

    if (lst)
    {
        int32 i;
        MMI_DLISTNODEHANDLE_T node;
        MMI_DLISTNODEHANDLE_T head;
        MMI_DLISTNODEHANDLE_T tail;

#ifdef DEBUG
        MMIBD_ASSERT_DEBUG(MMI_DLIST_Validate(lst));
#endif

        /* starting from empty list and appending at tail are tested with this loop */
        for (i = 0; i < 10; i++)
        {
            node = MMI_DLIST_Append(lst, &i);
        }
        head = MMI_DLIST_Prepend(lst, &i);
        printf("The list is created like this:\n");
        MMI_DLIST_Traverse(lst, PrintElement);

        i++;
        node = MMI_DLIST_GetNext(head);
        MMI_DLIST_InsertAfter(lst, node, &i);
        printf("The second element is: ");
        PrintElement(MMI_DLIST_GetElement(node));

        MMI_DLIST_Sort(lst, CompareElement);
        printf("After sorting, the list has %d elements:\n", MMI_DLIST_GetLength(lst));
        MMI_DLIST_Traverse(lst, PrintElement);

        head = MMI_DLIST_GetFirst(lst);
        node = MMI_DLIST_GetNext(head);
        MMI_DLIST_Delete(lst, &node);
        printf("After deleting the second element the list has %d elements:\n", MMI_DLIST_GetLength(lst));
        MMI_DLIST_Traverse(lst, PrintElement);

        /* boundary condition tests */
        tail = MMI_DLIST_GetLast(lst);
        MMI_DLIST_Delete(lst, &tail);
        printf("After deleting the tail the list has %d elements:\n", MMI_DLIST_GetLength(lst));
        MMI_DLIST_Traverse(lst, PrintElement);

        MMI_DLIST_Delete(lst, &head);
        head = MMI_DLIST_GetFirst(lst);
        printf("After deleting the head the list has %d elements:\n", MMI_DLIST_GetLength(lst));
        MMI_DLIST_Traverse(lst, PrintElement);

        i = 6;
        MMI_DLIST_TraverseStop(lst, &i, CheckElement, 0);

        MMI_DLIST_FOREACH(lst, node)
        {
            PrintElement(MMI_DLIST_GetElement(node));
        }

        MMI_DLIST_Destroy(&lst);
    }
}
#endif