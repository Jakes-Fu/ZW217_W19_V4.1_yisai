/********************************************************************************
** File Name:   mmi_stack.c                                                     *
** Author:      Hongbing Ju                                                     *
** Date:        06/25/2012                                                      *
** Copyright:   2003 Spreadtrum Communications Incorporated. All Right Reserved *
** Description: This file is used to describe a base data structure of stack    *
*********************************************************************************
**                            Important Edit history                            *
**-----------------------------------------------------------------------------**
** DATE            NAME            DESCRIPTION                                  *
** 06/25/2012      Hongbing Ju     Created                                      *
*********************************************************************************/

/*------------------------------------------------------------------------------*
**                            Included Files                                    *
**------------------------------------------------------------------------------*/
#if defined(DEBUG) || defined(_MMI_STACK_UNIT_TEST_)
#include "mmi_stack.h"
#endif
/*------------------------------------------------------------------------------*
**                            MACRO DEFINITIONS                                 *
**------------------------------------------------------------------------------*/
#define STCK_ERROR_MASK(err) ((err) | __LINE__ << 8)

/*------------------------------------------------------------------------------*
**                            TYPES AND CONSTANTS                               *
**------------------------------------------------------------------------------*/
#ifdef DEBUG
/* make sure both data and the pointer to the data are in read-only section */
const char * const stack_id = "stack";
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
** Description: This function is used to validate a stack                       *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
int32 MMI_STACK_Validate(MMI_STACKHANDLE_T stk)
{
    MMIBD_ASSERT_DEBUG(stk);

    if (stk)
    {
        return (!strcmp(stk->id, stack_id));
    }

    return (0);
}
#endif

#ifdef _STCK_UNIT_TEST_
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

void DeleteElement(void * val)
{
    SCI_FREE(val);
}

/********************************************************************************
** Description: This function is used to provide test functions for unit test   *
** Global Resource Dependency:                                                  *
** Author: Hongbing Ju                                                          *
** Note:                                                                        *
*********************************************************************************/
void main(void)
{
    MMI_STACKHANDLE_T stk = MMI_STACK_Create(1024, sizeof(int32));

    if (stk)
    {
        int32 i;
        int32 error_code = 0;
        MMI_STACKNODEHANDLE_T node;
        MMI_STACKNODEHANDLE_T head;

#ifdef DEBUG
        MMIBD_ASSERT_DEBUG(MMI_STACK_Validate(stk));
#endif

        /* starting from empty stack and appending at tail are tested with this loop */
        for (i = 0; i < 1023; i++)
        {
            node = MMI_STACK_Push(stk, &i);
        }
      
        head = MMI_STACK_Push(stk, &i);
        printf("The stack is created like this:\n");
        MMI_STACK_Traverse(stk, PrintElement);

        i++;
        head = MMI_STACK_Push(stk, &i);
        error_code = MMI_STACK_GetCurrErrorCode(stk);
        printf("error code is %d, line is %d", MMI_STACK_ERROR_NUMBER(error_code), MMI_STACK_ERROR_LOCATION(error_code));

        for(i = 0; i < 1023; i++)
        {
            MMI_STACK_Pop(stk, &error_code);
        }
        printf("After 1023 pops, the stack has %d elements:\n", MMI_STACK_GetDepth(stk));
        MMI_STACK_Traverse(stk, PrintElement);

        MMI_STACK_Pop(stk, &i);
        printf("After deleting the tail the stack has %d elements:\n", MMI_STACK_GetDepth(stk));
        MMI_STACK_Traverse(stk, PrintElement);

        MMI_STACK_Pop(stk, &i);
        error_code = MMI_STACK_GetCurrErrorCode(stk);
        printf("error code is %d, line is %d", MMI_STACK_ERROR_NUMBER(error_code), MMI_STACK_ERROR_LOCATION(error_code));
        printf("After 1025 pops the stack has %d elements:\n", MMI_STACK_GetDepth(stk));
        MMI_STACK_Traverse(stk, PrintElement);

        MMI_STACK_Destroy(&stk);
    }
}
#endif