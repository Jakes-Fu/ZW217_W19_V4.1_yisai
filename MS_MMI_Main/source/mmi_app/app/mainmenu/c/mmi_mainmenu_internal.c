/*****************************************************************************
** File Name:      gui_clipboard.c                                           *
** Author:         hua.fang                                                  *
** Date:           08/01/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/

#include "mmi_mainmenu_internal.h"
#include "mmi_mainmenu_data.h"

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
/*****************************************************************************/
// Description : ��item list�����item����
// Global resource dependence : 
//  Author:jin.wang
// Note: 
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *QbThemeMenuAddItem(
                                                MMIMAINEMENU_ITEM_LIST_T    *item_list_ptr,
                                                MMIMAINMENU_ITEM_INFO_T     *item_info_ptr
                                                );

/*****************************************************************************/
// Description : ����һ��������Ϣ���ָ��
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINEMENU_ITEM_LIST_T *CreateMenuItemNodeInfo(void);

/*****************************************************************************/
// Description : �ͷ�һ��ҳ
// Global resource dependence : 
//  Author:hua.fang
// Note:
/*****************************************************************************/
LOCAL void QbThemeDestroyPageNodeInfo(
                                   MMI_LINK_NODE_T const * node_ptr
                                   );

/*****************************************************************************/
// Description : �ͷ�һ��item���ݽṹ
// Global resource dependence : 
//  Author:hua.fang
// Note:
/*****************************************************************************/
LOCAL void QbThemeDestroyItemNodeInfo(
                                   MMI_LINK_NODE_T const * node_ptr
                                   );


/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : ���ҳ��Ϣ
// Global resource dependence : 
//  Author:hua.fang
// Note:
/*****************************************************************************/
PUBLIC MMIMAINEMENU_ITEM_LIST_T* MMIMENU_AddOnePage(
    MMIMAINEMENU_PAGE_LIST_T    *page_list_ptr
    )
{
    MMIMAINEMENU_ITEM_LIST_T *new_item_list_ptr = PNULL;

    if (PNULL == page_list_ptr)
    {
        return new_item_list_ptr;
    }
    
    // ����һ��������Ϣ�ڵ�,�����������ڴ�
    new_item_list_ptr = CreateMenuItemNodeInfo();

    if (PNULL != new_item_list_ptr)
    {
        if (PNULL == page_list_ptr->list_head_ptr)
        {
            page_list_ptr->list_head_ptr = MMILINK_CreateHead((uint32)new_item_list_ptr);
        }
        else
        {
            MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_item_list_ptr);
            MMILINK_AddNodeBeforeBaseNode(page_list_ptr->list_head_ptr, tmp_node_ptr);
        }

        page_list_ptr->page_count++;
    }

    return new_item_list_ptr;
}

/*****************************************************************************/
// Description : ��ҳ�������item
// Global resource dependence : 
//  Author:hua.fang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_AddItemInPage(
    MMIMAINEMENU_ITEM_LIST_T        *item_list_ptr,
    MMIMAINMENU_ITEM_INFO_T         *item_info_ptr,
    uint16                          item_num
    )
{
    uint16 item_index = 0;

    if (PNULL == item_list_ptr || PNULL == item_info_ptr || 0 == item_num)
    {
        return FALSE;
    }

    // ���item
    for (item_index = 0; item_index < item_num; item_index++)
    {
        MMIMAINMENU_ITEM_INFO_T     new_item = {0};

        new_item.menu_id = item_info_ptr[item_index].menu_id;
        new_item.group_id = item_info_ptr[item_index].group_id;

        QbThemeMenuAddItem(item_list_ptr, &new_item);
    }

    return TRUE;
}

/*****************************************************************************/
// Description : ��ʼ������
// Global resource dependence : 
//  Author:jin.wang
// Note: �ȿ����ǳ�ʼ��,Ҳ����������
/*****************************************************************************/
PUBLIC void MMIMENU_DestroyPageList(
                                   MMIMAINEMENU_PAGE_LIST_T    *page_list_ptr
                                   )
{
    if (PNULL == page_list_ptr)
    {
        return;
    }

    // �ͷ�item��������
    if (PNULL != page_list_ptr->list_head_ptr)
    {
        MMILINK_DestroyLink(page_list_ptr->list_head_ptr, QbThemeDestroyPageNodeInfo);
        page_list_ptr->list_head_ptr = PNULL;
    }

    page_list_ptr->page_count = 0;
}

/*****************************************************************************/
// Description : ����item����
// Global resource dependence : 
//  Author:jin.wang
// Note: 
/*****************************************************************************/
PUBLIC void MMIMENU_DestroyItemList(
    MMIMAINEMENU_ITEM_LIST_T    *item_list_ptr
    )
{
    if (PNULL == item_list_ptr)
    {
        return;
    }

    if (PNULL != item_list_ptr->list_head_ptr)
    {
        MMILINK_DestroyLink(item_list_ptr->list_head_ptr, QbThemeDestroyItemNodeInfo);
        item_list_ptr->list_head_ptr = PNULL;
    }

    item_list_ptr->item_count = 0;
}

/*****************************************************************************/
// Description : ɾ��һҳ
// Global resource dependence : 
//  Author: jin.wang
//  RETRUN:  TRUE: init success!
// Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DeleteOnePage(
                                  MMIMAINEMENU_PAGE_LIST_T      *page_list_ptr,
                                  uint16                        page_index
                                  )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    
    if (PNULL == page_list_ptr || PNULL == page_list_ptr->list_head_ptr)
    {
        return FALSE;
    }

    // ��ѯ����
    node_ptr = MMIMENU_GetItemListNode(page_list_ptr, page_index);

    if (PNULL != node_ptr)
    {
        // ɾ���ڵ�
        page_list_ptr->list_head_ptr = MMILINK_DestroyNode(
                node_ptr,
                page_list_ptr->list_head_ptr,
                QbThemeDestroyPageNodeInfo);

        page_list_ptr->page_count--;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *MMIMENU_GetItemListNode(
                                           MMIMAINEMENU_PAGE_LIST_T *page_list_ptr, 
                                           uint16                   item_index
                                           )
{
    BOOLEAN is_find = FALSE;
    uint32  i = 0;
    MMI_LINK_NODE_T *tmp_ptr = PNULL;
    MMI_LINK_NODE_T *tmp_head_ptr = PNULL;

    if (PNULL != page_list_ptr)
    {
        tmp_head_ptr = tmp_ptr = page_list_ptr->list_head_ptr;

        if (PNULL != tmp_ptr)
        {
            if (0 == item_index)
            {
                tmp_ptr = tmp_head_ptr;
                is_find = TRUE;
            }
            else
            {
                for (i = 0; i < page_list_ptr->page_count; i++)
                {
                    // ���ص���ʼ���ʱ�򣬿϶���û�ҵ�
                    if (i == item_index && tmp_ptr != tmp_head_ptr) 
                    {
                        is_find = TRUE;
                        break;
                    }
                    // ָ����һ��ָ��
                    tmp_ptr = tmp_ptr->next_ptr;
                }
            }
        }
    }

    if (!is_find)
    {
        // ���û�ҵ�����ʼ��ΪPNULL
        tmp_ptr = PNULL;
    }

    return tmp_ptr;    
}

/*****************************************************************************/
// Description : get item
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC MMIMAINEMENU_ITEM_LIST_T *MMIMENU_GetItemList(
                                                   MMIMAINEMENU_PAGE_LIST_T *page_list_ptr,
                                                   uint32                   page_index
                                                   )
{
    MMI_LINK_NODE_T *item_list_node_ptr = PNULL;
    MMIMAINEMENU_ITEM_LIST_T    *item_list_ptr = PNULL;

    item_list_node_ptr = MMIMENU_GetItemListNode(page_list_ptr, page_index);
    if (PNULL != item_list_node_ptr)
    {
        item_list_ptr = (MMIMAINEMENU_ITEM_LIST_T *)item_list_node_ptr->data;
    }

    return item_list_ptr;
}

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *MMIMENU_GetItemInfoNode(
                                           MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr, 
                                           uint16                   item_index
                                           )
{
    BOOLEAN is_find = FALSE;
    uint32  i = 0;
    MMI_LINK_NODE_T *tmp_ptr = PNULL;
    MMI_LINK_NODE_T *tmp_head_ptr = PNULL;

    if (PNULL != page_item_info_ptr)
    {
        tmp_head_ptr = tmp_ptr = page_item_info_ptr->list_head_ptr;

        if (PNULL != tmp_ptr)
        {
            if (0 == item_index)
            {
                tmp_ptr = tmp_head_ptr;
                is_find = TRUE;
            }
            else
            {
                for (i = 0; i < page_item_info_ptr->item_count; i++)
                {
                    // ���ص���ʼ���ʱ�򣬿϶���û�ҵ�
                    if (i == item_index && tmp_ptr != tmp_head_ptr) 
                    {
                        is_find = TRUE;
                        break;
                    }
                    // ָ����һ��ָ��
                    tmp_ptr = tmp_ptr->next_ptr;
                }
            }
        }
    }

    if (!is_find)
    {
        // ���û�ҵ�����ʼ��ΪPNULL
        tmp_ptr = PNULL;
    }

    return tmp_ptr;    
}

/*****************************************************************************/
// Description : get item info in list  
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC MMIMAINMENU_ITEM_INFO_T* MMIMENU_GetItemInfo(
                                                    MMIMAINEMENU_ITEM_LIST_T    *page_item_info_ptr, 
                                                    uint16                      item_index
                                                    )
{
    MMI_LINK_NODE_T *item_node_ptr = PNULL;
    MMIMAINMENU_ITEM_INFO_T *item_info_ptr = PNULL;

    item_node_ptr = MMIMENU_GetItemInfoNode(page_item_info_ptr, item_index);

    if (PNULL != item_node_ptr)
    {
        item_info_ptr = (MMIMAINMENU_ITEM_INFO_T *)(item_node_ptr->data);
    }

    return item_info_ptr;    
}

/*****************************************************************************/
// Description : ����һ��Item
// Global resource dependence : 
//  Author: jin.wang
//  RETRUN:  ����ָ��
// Note:    
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *QbThemeMenuAddItem(
                                                MMIMAINEMENU_ITEM_LIST_T    *item_list_ptr,
                                                MMIMAINMENU_ITEM_INFO_T     *item_info_ptr
                                                )
{
    MMIMAINMENU_ITEM_INFO_T *new_item_info_ptr = PNULL;

    if (PNULL == item_list_ptr || PNULL == item_info_ptr)
    {
        return new_item_info_ptr;
    }
    
    // ����һ��������Ϣ�ڵ�,�����������ڴ�
    new_item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));
    SCI_MEMCPY(new_item_info_ptr, item_info_ptr, sizeof(MMIMAINMENU_ITEM_INFO_T));

    if (PNULL == item_list_ptr->list_head_ptr)
    {
        item_list_ptr->list_head_ptr = MMILINK_CreateHead((uint32)new_item_info_ptr);
    }
    else
    {
        MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_item_info_ptr);
        MMILINK_AddNodeBeforeBaseNode(item_list_ptr->list_head_ptr, tmp_node_ptr);
    }

    item_list_ptr->item_count++;

    return new_item_info_ptr;
}

/*****************************************************************************/
// Description : ����һ��������Ϣ���ָ��
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
LOCAL MMIMAINEMENU_ITEM_LIST_T *CreateMenuItemNodeInfo(void)
{
    MMIMAINEMENU_ITEM_LIST_T *new_item_list_ptr = PNULL;

    new_item_list_ptr = SCI_ALLOC_APP(sizeof(MMIMAINEMENU_ITEM_LIST_T));
    if (PNULL != new_item_list_ptr)
    {
        new_item_list_ptr->item_count = 0;
        new_item_list_ptr->list_head_ptr = PNULL;
    }

    return new_item_list_ptr;
}

/*****************************************************************************/
// Description : �ͷ�һ��ҳ
// Global resource dependence : 
//  Author:hua.fang
// Note:
/*****************************************************************************/
LOCAL void QbThemeDestroyPageNodeInfo(
                                   MMI_LINK_NODE_T const * node_ptr
                                   )
{
    MMIMAINEMENU_ITEM_LIST_T *item_info_ptr = PNULL;

    if (node_ptr == PNULL)
    {
        return;
    }

    item_info_ptr = (MMIMAINEMENU_ITEM_LIST_T*)node_ptr->data;

    if (PNULL != item_info_ptr && PNULL != item_info_ptr->list_head_ptr)
    {
        // �ͷ�item������
        MMILINK_DestroyLink(item_info_ptr->list_head_ptr, QbThemeDestroyItemNodeInfo);
    }

    SCI_FREE(item_info_ptr);
}

/*****************************************************************************/
// Description : �ͷ�һ��item���ݽṹ
// Global resource dependence : 
//  Author:hua.fang
// Note:
/*****************************************************************************/
LOCAL void QbThemeDestroyItemNodeInfo(
                                   MMI_LINK_NODE_T const * node_ptr
                                   )
{
    MMIMAINMENU_ITEM_INFO_T *item_info_ptr = PNULL;

    if (node_ptr == PNULL)
    {
        return;
    }

    item_info_ptr = (MMIMAINMENU_ITEM_INFO_T*)node_ptr->data;

    SCI_FREE(item_info_ptr);
}

