/********************************************************************************
** File Name:   mmi_queue.c                                                     *
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
#if defined(DEBUG) || defined(_MMI_QUEUE_UNIT_TEST_)
#include "mmi_queue_new.h"
#endif
/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/
#define MMI_QUEUE_ERROR_MASK(err) ((err) | __LINE__ << 8)

/*------------------------------------------------------------------------------*
**                            TYPES AND CONSTANTS                               *
**------------------------------------------------------------------------------*/
#ifdef DEBUG
/* make sure both data and the pointer to the data are in read-only section */
const char * const queue_id = "queue";
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

/*------------------------------------------------------------------------------*
**                            FUNCTIONS                                         *
**------------------------------------------------------------------------------*/
#ifdef DEBUG
/********************************************************************************
** Description: This function is used to validate a list                        *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_QUEUE_Validate(MMI_QUEUEHANDLE_T queue)
{
    assert(queue); /*assert verified*/ 

    if (queue)
    {
        return (!strcmp(queue->id, queue_id));
    }

    return (0);
}
#endif

#ifdef _MMI_QUEUE_UNIT_TEST_
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

void DeleteElement(void *val)
{
    free(val);
}

/********************************************************************************
** Description: This function is used to provide test functions for unit test   *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void main(void)
{
    MMI_QUEUEHANDLE_T queue = MMI_QUEUE_Create(1024, sizeof(int32));

    if (queue)
    {
        int32 i;
        int32 error_code = 0;
        MMI_QUEUENODEHANDLE_T node;
        MMI_QUEUENODEHANDLE_T head;

#ifdef DEBUG
        assert(MMI_QUEUE_Validate(queue)); /*assert verified*/ 
#endif

        /* starting from empty list and appending at tail are tested with this loop */
        for (i = 0; i < 1023; i++)
        {
            node = MMI_QUEUE_Post(queue, &i);
        }
        head = MMI_QUEUE_PostInFront(queue, &i);
        printf("The list is created like this:\n");
        MMI_QUEUE_Traverse(queue, PrintElement);

        i++;
        head = MMI_QUEUE_Post(queue, &i);
        error_code = MMI_QUEUE_GetCurrErrorCode(queue);
        printf("error code is %d, line is %d", MMI_QUEUE_ERROR_NUMBER(error_code), MMI_QUEUE_ERROR_LOCATION(error_code));

        for(i = 0; i < 1023; i++)
        {
            MMI_QUEUE_Peek(queue, &error_code);
        }
        printf("After 1023 pops, the queue has %d elements:\n", MMI_QUEUE_GetDepth(queue));
        MMI_QUEUE_Traverse(queue, PrintElement);

        MMI_QUEUE_Peek(queue, &i);
        printf("After deleting the tail the queue has %d elements:\n", MMI_QUEUE_GetDepth(queue));
        MMI_QUEUE_Traverse(queue, PrintElement);

        MMI_QUEUE_Peek(queue, &i);
        error_code = MMI_QUEUE_GetCurrErrorCode(queue);
        printf("error code is %d, line is %d", MMI_QUEUE_ERROR_NUMBER(error_code), MMI_QUEUE_ERROR_LOCATION(error_code));
        printf("After 1025 pops the queue has %d elements:\n", MMI_QUEUE_GetDepth(queue));
        MMI_QUEUE_Traverse(queue, PrintElement);

        MMI_QUEUE_Destroy(&queue);
    }
}
#endif