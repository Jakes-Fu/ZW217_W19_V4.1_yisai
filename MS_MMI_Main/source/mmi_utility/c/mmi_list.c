/*****************************************************************************
** File Name:      gui_clipboard.c                                           *
** Author:         hua.fang                                                  *
** Date:           07/16/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2011        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_list.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create a list node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_LIST_T* MMILIST_CreateNode(
                                      uint16    node_size                       // �ڵ��С
                                      )
{
    MMI_LIST_T* list_node_ptr = SCI_ALLOC_APP(node_size);

    SCI_MEMSET(list_node_ptr, 0, node_size);

    MMILIST_Init(list_node_ptr);

    return list_node_ptr;
}

/*****************************************************************************/
//  Description : destroy a list node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// ע��: list_node_ptr�����Ǳ�������ڴ��ַ
/*****************************************************************************/
PUBLIC void MMILIST_DestroyNode(
                                MMI_LIST_T                  *list_node_ptr,     // Ҫ���ͷŵĽڵ�
                                MMILIST_DATADESTRUCTFUNC    destroy_func        // ���ٽڵ�ǰ�����û��Ļص�
                                )
{
    if (PNULL != list_node_ptr)
    {
        MMILIST_Detach(list_node_ptr);

        if (PNULL != destroy_func)
        {
            destroy_func(list_node_ptr);
        }

        SCI_FREE(list_node_ptr);
    }
}

/*****************************************************************************/
//  Description : destroy all list nodes
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// ������Щͷ�ڵ㲻�Ƕ�̬������ڴ棬������ͷ�ڵ��ַ���ڴ�����ַ�������޷�
// �ͷţ�����ͨ��is_destroy_header�������Ƿ�Ҫ�ͷ�ͷ�ڵ��ڴ�
// ��������뱣֤��������Ľڵ��ַ���������ڴ�����ַ
/*****************************************************************************/
PUBLIC void MMILIST_DestroyAllNodes(
                                    MMI_LIST_T                  *list_header_ptr,   // ͷ�ڵ�
                                    BOOLEAN                     is_destroy_header,  // �Ƿ�Ҫ����ͷ�ڵ�
                                    MMILIST_DATADESTRUCTFUNC    destroy_func        // ���ٽڵ�ǰ�����û��Ļص�
                                    )
{
    MMI_LIST_T  *list_node_ptr = PNULL;

    if (PNULL != list_header_ptr)
    {
        while (list_header_ptr != (list_node_ptr = list_header_ptr->next))
        {
            MMILIST_DestroyNode(list_node_ptr, destroy_func);
        }

        if (is_destroy_header)
        {
            MMILIST_DestroyNode(list_header_ptr, destroy_func);
        }
    }
}

/*****************************************************************************/
//  Description : init list
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_Init(
                         MMI_LIST_T     *list_node_ptr
                         )
{
    if (PNULL != list_node_ptr)
    {
        list_node_ptr->next = list_node_ptr;
        list_node_ptr->prev = list_node_ptr;
    }
}

/*****************************************************************************/
//  Description : is list empty
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMILIST_IsEmpty(
                               MMI_LIST_T   *list_header_ptr
                               )
{
    BOOLEAN is_empty = TRUE;

    if (PNULL != list_header_ptr)
    {
        if (list_header_ptr != list_header_ptr->next)
        {
            is_empty = FALSE;
        }
    }

    return is_empty;
}

/*****************************************************************************/
//  Description : detach a list node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_LIST_T* MMILIST_Detach(
                                  MMI_LIST_T    *list_node_ptr
                                  )
{
    if (PNULL != list_node_ptr)
    {
        list_node_ptr->prev->next = list_node_ptr->next;
        list_node_ptr->next->prev = list_node_ptr->prev;

        list_node_ptr->prev = list_node_ptr;
        list_node_ptr->next = list_node_ptr;
    }

    return list_node_ptr;
}

/*****************************************************************************/
//  Description : insert before base node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_InsertBefore(
                                 MMI_LIST_T     *base_node_ptr,     // ��׼�ڵ�
                                 MMI_LIST_T     *insert_node_ptr    // ������Ľڵ�
                                 )
{
    if (PNULL != base_node_ptr && PNULL != insert_node_ptr)
    {
        insert_node_ptr->prev = base_node_ptr->prev;
        insert_node_ptr->next = base_node_ptr;

        base_node_ptr->prev->next = insert_node_ptr;
        base_node_ptr->prev = insert_node_ptr;
    }
}

/*****************************************************************************/
//  Description : insert after base node
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_InsertAfter(
                                MMI_LIST_T     *base_node_ptr,      // ��׼�ڵ�
                                MMI_LIST_T     *insert_node_ptr     // ������Ľڵ�
                                )
{
    if (PNULL != base_node_ptr && PNULL != insert_node_ptr)
    {
        insert_node_ptr->prev = base_node_ptr;
        insert_node_ptr->next = base_node_ptr->next;

        base_node_ptr->next->prev = insert_node_ptr;
        base_node_ptr->next = insert_node_ptr;
    }
}

/*****************************************************************************/
//  Description : insert into the first
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// ���ڵ���뵽ͷ�ڵ�֮��
/*****************************************************************************/
PUBLIC void MMILIST_InsertFirst(
                                MMI_LIST_T     *list_header_ptr,
                                MMI_LIST_T     *list_node_ptr
                                )
{
    MMILIST_InsertAfter(list_header_ptr, list_node_ptr);
}

/*****************************************************************************/
//  Description : insert last
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// ���ڵ���뵽β�ڵ�֮��
/*****************************************************************************/
PUBLIC void MMILIST_InsertLast(
                               MMI_LIST_T     *list_header_ptr,
                               MMI_LIST_T     *list_node_ptr
                               )
{
    MMILIST_InsertBefore(list_header_ptr, list_node_ptr);
}

/*****************************************************************************/
//  Description : insert by index
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMILIST_InsertByIndex(
                                  MMI_LIST_T      *list_header_ptr,
                                  MMI_LIST_T      *list_node_ptr,
                                  uint16          index
                                  )
{
    if (PNULL != list_header_ptr && PNULL != list_node_ptr)
    {
        // TODO: not imp
    }
}
