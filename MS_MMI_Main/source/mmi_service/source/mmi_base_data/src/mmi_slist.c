/********************************************************************************
** File Name:    mmi_slist.c                                                    *
** Author:       Xiaomao Xiao                                                   *
** Date:         06/25/2012                                                     *
** Copyright:    2003 Spreadtrum Communications Incorporated. All Right Reserved*
** Description: This file is used to describe a base data structure of single-  *
**                direction linked list                                         *
*********************************************************************************
**                            Important Edit history                            *
**-----------------------------------------------------------------------------**
** DATE            NAME            DESCRIPTION                                  *
** 06/25/2012    Xiaomao Xiao    Created                                        *
*********************************************************************************/

/*------------------------------------------------------------------------------*
**                            Included Files                                    *
**------------------------------------------------------------------------------*/
#include "mmi_slist.h"
/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/
#define MMI_SLST_ERROR_MASK(err) ((err) | __LINE__ << 8)

/*------------------------------------------------------------------------------*
**                            TYPES AND CONSTANTS                               *
**------------------------------------------------------------------------------*/
#ifdef DEBUG
/* make sure both data and the pointer to the data are in read-only section */
const char * const slist_id = "single_list";
#endif

typedef struct MMI_SLISTNODE_TAG
{
    struct MMI_SLISTNODE_TAG *next;
    /*
    * flexible array member whose size is defined at run-time, which helps
    * reduce the chance of memory fragmentation
    * refer to ISO/IEC 9899:1999, Section 6.7.2.1, paragraph 16
    * if your compiler is not compliant, declare it as
    * char element[1];
    */
    char                 element[];
} MMI_SLISTNODE_T;

typedef struct MMI_SLIST_TAG
{
    MMI_SLISTNODEHANDLE_T head;
    MMI_SLISTNODEHANDLE_T tail;
    int32               error;
    int32               length;
    uint32              elementSize;
#ifdef DEBUG
    const char        *id;
#endif
} MMI_SLIST_T;

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
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
static MMI_SLISTNODEHANDLE_T 
MMI_SLIST_CreateNode(void * element, 
                 uint32 elementSize
                 );

/********************************************************************************
** Description: This function is used to insert an element after a list node    *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
static MMI_SLISTNODEHANDLE_T 
MMI_SLIST_InsertAft(MMI_SLISTNODEHANDLE_T node, 
                void * element, 
                uint32 elementSize
                );

/*------------------------------------------------------------------------------*
**                            FUNCTIONS                                         *
**------------------------------------------------------------------------------*/

/********************************************************************************
** Description: This function is used to create a list                          *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
MMI_SLISTHANDLE_T MMI_SLIST_Create(uint32 elementSize)
{
    MMI_SLISTHANDLE_T ret = (MMI_SLISTHANDLE_T) SCI_ALLOCA(sizeof(MMI_SLIST_T));
    /* create a head node for efficiency */
    MMI_SLISTNODEHANDLE_T node = MMI_SLIST_CreateNode("H", 2);//cid608042

    if (PNULL == ret || PNULL == node)
    {
        SCI_FREE(ret);
        SCI_FREE(node);
    }
    else
    {
        SCI_MEMSET(ret, 0x00, sizeof(MMI_SLIST_T));
        ret->head = node;
        ret->tail = node;
        //ret->error = 0;
        //ret->length = 0;
        ret->elementSize = elementSize;
#ifdef DEBUG
        ret->id = slist_id;
#endif
    }

    return (ret);
}

/********************************************************************************
** Description: This function is used to destroy a list                         *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
void MMI_SLIST_Destroy(MMI_SLISTHANDLE_T *lst)
{
    MMI_SLISTNODEHANDLE_T pt;

    MMIBD_ASSERT_DEBUG(lst && *lst);

    if (PNULL == lst || PNULL == *lst)
    {
        return;
    }

    pt = (*lst)->head;
    while (pt)
    {
        MMI_SLISTNODEHANDLE_T tmp = pt->next;

        SCI_FREE(pt);
        pt=tmp;
    }

    SCI_FREE(*lst);
    *lst = PNULL;
}

/********************************************************************************
** Description: This function is used to prepend an element                     *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
MMI_SLISTNODEHANDLE_T MMI_SLIST_Prepend(MMI_SLISTHANDLE_T lst, void * element)
{
    MMI_SLISTNODEHANDLE_T node;

    MMIBD_ASSERT_DEBUG(lst && element);

    if (PNULL == lst)
    {
        return (PNULL);
    }

    if (PNULL == element)
    {
        lst->error = MMI_SLST_ERROR_MASK(MMI_SLST_ERROR_NULL_ELEMENT);

        return (PNULL);
    }

    node = MMI_SLIST_InsertAft(lst->head, element, lst->elementSize);
    if (PNULL == node)
    {
        lst->error = MMI_SLST_ERROR_MASK(MMI_SLST_ERROR_OUT_OF_MEMORY);

        return (PNULL);
    }

    lst->length++;

    return (node);
}

/********************************************************************************
** Description: This function is used to append an element                      *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
MMI_SLISTNODEHANDLE_T MMI_SLIST_Append(MMI_SLISTHANDLE_T lst, void * element)
{
    MMI_SLISTNODEHANDLE_T node;

    MMIBD_ASSERT_DEBUG(lst && element);

    if (PNULL == lst)
    {
        return (PNULL);
    }

    if (PNULL == element)
    {
        lst->error = MMI_SLST_ERROR_MASK(MMI_SLST_ERROR_NULL_ELEMENT);

        return (PNULL);
    }

    node = MMI_SLIST_InsertAft(lst->tail, element, lst->elementSize);
    if (PNULL == node)
    {
        lst->error = MMI_SLST_ERROR_MASK(MMI_SLST_ERROR_OUT_OF_MEMORY);

        return (PNULL);
    }

    lst->tail = node;
    lst->length++;

    return (node);
}

/********************************************************************************
** Description: This function is used to insert an element after a node         *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
MMI_SLISTNODEHANDLE_T MMI_SLIST_InsertAfter(MMI_SLISTHANDLE_T lst, MMI_SLISTNODEHANDLE_T node, void * element)
{
    MMI_SLISTNODEHANDLE_T newNode;

    MMIBD_ASSERT_DEBUG(lst && element);

    if (PNULL == lst)
    {
        return (PNULL);
    }

    if (PNULL == element)
    {
        lst->error = MMI_SLST_ERROR_MASK(MMI_SLST_ERROR_NULL_ELEMENT);

        return (PNULL);
    }

    newNode = MMI_SLIST_InsertAft(node, element, lst->elementSize);
    if (PNULL == newNode)
    {
        lst->error = MMI_SLST_ERROR_MASK(MMI_SLST_ERROR_OUT_OF_MEMORY);

        return (PNULL);
    }

    lst->length++;

    return (newNode);
}

/********************************************************************************
** Description: This function is used to delete a node                          *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
void MMI_SLIST_Delete(MMI_SLISTHANDLE_T lst, MMI_SLISTNODEHANDLE_T *node)
{
    MMIBD_ASSERT_DEBUG(lst && node);

    if (lst && node)
    {
        MMI_SLISTNODEHANDLE_T pt;

        for (pt = lst->head; pt; pt = pt->next)
        {
            if (pt->next == *node)
            {
                pt->next = (*node)->next;
                SCI_FREE(*node);
                *node = PNULL;
                lst->length--;
                break;
            }
        }
    }
}

/********************************************************************************
** Description: This function is used to check if a list is empty or not        *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_SLIST_IsEmpty(MMI_SLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (!lst->length);
    }

    return (1);
}

/********************************************************************************
** Description: This function is used to get item numbers in a list             *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_SLIST_GetLength(MMI_SLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->length);
    }

    return (0);
}

/********************************************************************************
** Description: This function is used to get next node in a list                *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
MMI_SLISTNODEHANDLE_T MMI_SLIST_GetNext(MMI_SLISTNODEHANDLE_T node)
{
    MMIBD_ASSERT_DEBUG(node);

    if (node)
    {
        return (node->next);
    }

    return (PNULL);
}

/********************************************************************************
** Description: This function is used to get next node in a list                *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
MMI_SLISTNODEHANDLE_T MMI_SLIST_GetFirst(MMI_SLISTHANDLE_T lst)
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
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
MMI_SLISTNODEHANDLE_T MMI_SLIST_GetLast(MMI_SLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->tail);
    }

    return (PNULL);
}

/********************************************************************************
** Description: This function is used to get item data in a node                *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
void * MMI_SLIST_GetElement(MMI_SLISTNODEHANDLE_T node)
{
    MMIBD_ASSERT_DEBUG(node);

    if (node)
    {
        return (node->element);
    }

    return (PNULL);
}

/********************************************************************************
** Description: This function is used to traverse through a list                *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
void MMI_SLIST_Traverse(MMI_SLISTHANDLE_T lst, MMI_SLIST_TrvFunc func)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && func)
    {
        MMI_SLISTNODEHANDLE_T pt;

        for (pt = lst->head->next; pt; pt = pt->next)
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
void* MMI_SLIST_TraverseStop(MMI_SLISTHANDLE_T lst, void *param, MMI_SLIST_TrvStopFunc func, int32 condition)
{
    MMIBD_ASSERT_DEBUG(lst && func);

    if (lst && func)
    {
        MMI_SLISTNODEHANDLE_T pt;

        for (pt = lst->head->next; pt != PNULL; pt = pt->next)
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
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_SLIST_Validate(MMI_SLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (!strcmp(lst->id, slist_id));
    }

    return (0);
}
#endif

/********************************************************************************
** Description: This function is used to create a list node                     *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
static MMI_SLISTNODEHANDLE_T 
MMI_SLIST_CreateNode(void * element, 
                 uint32 elementSize
                 )
{
    MMI_SLISTNODEHANDLE_T node;

    node = (MMI_SLISTNODEHANDLE_T) SCI_ALLOCA(sizeof(MMI_SLISTNODE_T) + elementSize);
    if (node)
    {
        node->next = PNULL;
        SCI_MEMCPY(node->element, element, elementSize);
    }

    return (node);
}

/********************************************************************************
** Description: This function is used to insert an element after a list node    *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
static MMI_SLISTNODEHANDLE_T 
MMI_SLIST_InsertAft(MMI_SLISTNODEHANDLE_T node, 
                void * element, 
                uint32 elementSize
                )
{
    if(PNULL != node)
    {
        MMI_SLISTNODEHANDLE_T newNode = MMI_SLIST_CreateNode(element, elementSize);

        if (newNode)
        {
            newNode->next = node->next;
            node->next = newNode;
        }

        return (newNode);
    }
    return PNULL;
}

/********************************************************************************
** Description: This function is used to get the error code of action           *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_SLIST_GetCurrErrorCode(MMI_SLISTHANDLE_T lst)
{
    MMIBD_ASSERT_DEBUG(lst);

    if (lst)
    {
        return (lst->error);
    }

    return (0);    
}

#ifdef _MMI_SLST_UNIT_TEST_
/********************************************************************************
** Description: This function is used to print an element value for unit test   *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
void PrintElement(void * val)
{
    printf("%d\n", * (int32 *) val);
}

/********************************************************************************
** Description: This function is used to provide test functions for unit test   *
** Global Resource Dependency:                                                  *
** Author: Xiaomao Xiao                                                         *
** Note:                                                                        *
*********************************************************************************/
void main(void)
{
    MMI_SLISTHANDLE_T lst = MMI_SLIST_Create(sizeof(int32));

    if (lst)
    {
        int32 i;
        MMI_SLISTNODEHANDLE_T node;
        MMI_SLISTNODEHANDLE_T head;
        MMI_SLISTNODEHANDLE_T tail;

#ifdef DEBUG
        MMIBD_ASSERT_DEBUG(MMI_SLIST_Validate(lst));
#endif

        /* starting from empty list and appending at tail are tested with this loop */
        for (i = 0; i < 10; i++)
        {
            node = MMI_SLIST_Append(lst, &i);
        }
        tail = node;
        head = MMI_SLIST_Prepend(lst, &i);
        printf("The list is created like this:\n");
        MMI_SLIST_Traverse(lst, PrintElement);

        i++;
        node = MMI_SLIST_GetNext(head);
        MMI_SLIST_InsertAfter(lst, node, &i);
        printf("The second element is: ");
        PrintElement(MMI_SLIST_GetElement(node));

        MMI_SLIST_Delete(lst, &node);
        printf("After deleting the second element the list has %d elements:\n", MMI_SLIST_GetLength(lst));
        MMI_SLIST_Traverse(lst, PrintElement);

        /* boundary condition tests */
        MMI_SLIST_Delete(lst, &tail);
        printf("After deleting the tail the list has %d elements:\n", MMI_SLIST_GetLength(lst));
        MMI_SLIST_Traverse(lst, PrintElement);

        MMI_SLIST_Delete(lst, &head);
        printf("After deleting the head the list has %d elements:\n", MMI_SLIST_GetLength(lst));
        MMI_SLIST_Traverse(lst, PrintElement);

        MMI_SLIST_FOREACH(lst, node)
        {
            PrintElement(MMI_SLIST_GetElement(node));
        }

        MMI_SLIST_Destroy(&lst);
    }
}
#endif