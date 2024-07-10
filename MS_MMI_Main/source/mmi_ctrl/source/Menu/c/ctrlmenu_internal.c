/*****************************************************************************
** File Name:      ctrlmenu_internal.c                                        *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "ctrlmenu.h"
#include "ctrlmenu_export.h"
#include "mmi_theme.h"
#include "mmi_textfun.h"
#include "mmi_gui_trc.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         TYPE AND CONSTANT                                 *
 *---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         LOCAL FUNCTION DECLARE                            *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN MenuTypeOf(
                         CTRLBASE_OBJ_T     *form_ptr
                         );

/*****************************************************************************/
//  Description : get item
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for static menu.
/*****************************************************************************/
LOCAL GUIMENU_ITEM_T* GetItemByVisibleIndex(
                                            CTRLMENU_OBJ_T          *menu_ctrl_ptr,     //[in]
                                            uint16                  visible_index,      //[in]
                                            CTRLMENU_ITEM_INFO_T    *out_item_info_ptr  //[out]
                                            );

/*****************************************************************************/
//  Description : get item for static menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_STATIC_ITEM_T* GetStaticItem(
                                            CTRLMENU_OBJ_T      *menu_ctrl_ptr, // [in]
                                            MMI_MENU_GROUP_ID_T group_id,       // [in]
                                            MMI_MENU_ID_T       menu_id         // [in]
                                            );

#ifndef GUIF_SOFTKEY_PDASTYLE
/*****************************************************************************/
//  Description : get softkey text id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetSoftkeyText(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr, //in
                          GUI_BUTTON_T      *button_id_ptr  //out
                          );
#endif

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
/*****************************************************************************/
//  Description : check sub menu style
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckSubMenuStyle(
                             CTRLMENU_OBJ_T *menu_ctrl_ptr
                             );
#endif

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get menu pointer by control id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLMENU_OBJ_T* CTRLMENU_GetPtr(
                                       MMI_HANDLE_T   ctrl_handle
                                       )
{
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        if (!MenuTypeOf(ctrl_ptr))
        {
            ctrl_ptr = PNULL;
            //SCI_TRACE_LOW:"== GetMenuPtr == ctrl_handle 0x%x is not menu"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMENU_3281_112_2_18_3_20_24_242,(uint8*)"d", ctrl_handle);
        }
    }
    
    return (CTRLMENU_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN MenuTypeOf(
                         CTRLBASE_OBJ_T     *menu_ptr
                         )
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)menu_ptr, CTRL_MENU_TYPE);
}

/*****************************************************************************/
//  Description : init menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_Init( 
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          )
{
    //init menu
    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
        CTRLMENU_InitSec(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        CTRLMENU_InitPop(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
        CTRLMENU_InitOptions(menu_ctrl_ptr);
#endif
        break;

    default:
        SCI_TRACE_LOW("CTRLMENU_Init:menu style %d is error!",menu_ctrl_ptr->cur_style);
        break;
    }
}

/*****************************************************************************/
//  Description : creat one new node
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLMENU_NODE_T* CTRLMENU_CreatNewNode(
                                              CTRLMENU_NODE_MEMORY_T    *node_mem_ptr
                                              )
{
    CTRLMENU_NODE_T     *new_node_ptr = PNULL;

    if (PNULL == node_mem_ptr)
    {
        return PNULL;
    }

    if (0 == node_mem_ptr->free_node_num)
    {
        return PNULL;
    }

    //creat one new node
    new_node_ptr = node_mem_ptr->free_node_ptr;

    //set free node value
    node_mem_ptr->free_node_num--;
    node_mem_ptr->free_node_ptr = node_mem_ptr->free_node_ptr->next_free_node_ptr;

    //init new node
    SCI_MEMSET(new_node_ptr,0,sizeof(CTRLMENU_NODE_T));

    return (new_node_ptr);
}


/*****************************************************************************/
//  Description : display menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_Draw(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr
                          )
{
    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
        CTRLMENU_DrawSec(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        CTRLMENU_DrawPop(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
        CTRLMENU_DrawOptions(menu_ctrl_ptr);
#endif
        break;

    default:
        SCI_TRACE_LOW("CTRLMENU_Draw:menu style %d is error!",menu_ctrl_ptr->cur_style);
        break;
    }
}

/*****************************************************************************/
//  Description : destroy menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_Destroy(
                             CTRLMENU_OBJ_T     *menu_ctrl_ptr
                             )
{
    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
        CTRLMENU_DestroySec(menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
    case GUIMENU_STYLE_OPTIONS:
        CTRLMENU_DestroyPop(menu_ctrl_ptr);
        break;

    default:
        SCI_TRACE_LOW("CTRLMENU_Destroy:menu style %d is error!",menu_ctrl_ptr->cur_style);
        break;
    }
}

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetStatePtr( 
                                    CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                    uint32          src_state
                                    )
{
    BOOLEAN result = FALSE;

    if (PNULL != menu_ctrl_ptr)
    {
        result = (BOOLEAN)((menu_ctrl_ptr->menu_state & src_state) == src_state);
    }

    return result;
}

/*****************************************************************************/
//  Description : set state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetStatePtr(
                                 CTRLMENU_OBJ_T *menu_ctrl_ptr,
                                 uint32         src_state,
                                 BOOLEAN        is_true
                                 )
{
    if (PNULL != menu_ctrl_ptr)
    {
        if( is_true )
        {
            menu_ctrl_ptr->menu_state |= src_state;
        }
        else
        {
            menu_ctrl_ptr->menu_state &= ~src_state;
        }
    }
}

/*****************************************************************************/
//  Description : is pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsPopMenu(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                  )
{
    BOOLEAN     result = FALSE;

    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
    case GUIMENU_STYLE_OPTIONS:
        result = TRUE;
        break;

    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : is second menu
//  Global resource dependence : 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsSecMenu(
                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr  //in
                                  )
{
    BOOLEAN     result = FALSE;

    if(PNULL == menu_ctrl_ptr)
    {
        SCI_TRACE_LOW("CTRLMENU_IsSecMenu menu_ctrl_ptr is null");
        return (result);
    }

    if(GUIMENU_STYLE_SECOND == menu_ctrl_ptr->cur_style)
    {
        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
//  Description : get pop sel info
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for menu view
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetPopSelInfo(
                                      CTRLMENU_OBJ_T            *menu_ctrl_ptr, //[in]
                                      uint16                    item_index,     //[in]
                                      GUIMENU_POP_SEL_INFO_T    *item_info_ptr  //[out]
                                      )
{
    BOOLEAN             result          = FALSE;
    CTRLBASE_OBJ_T      *base_ctrl_ptr  = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
    GUIMENU_ITEM_T      *item_ptr       = PNULL;
    CTRLMENU_NODE_T     *item_node_ptr  = PNULL;

    if ((PNULL != menu_ctrl_ptr) && 
        (PNULL != item_info_ptr))
    {
        item_info_ptr->ctrl_id   = base_ctrl_ptr->handle;
        item_info_ptr->is_static = menu_ctrl_ptr->is_static;
        if (menu_ctrl_ptr->is_static)
        {
            item_ptr = GetItemByVisibleIndex(menu_ctrl_ptr, item_index, PNULL);
            if (PNULL != item_ptr)
            {
                item_info_ptr->menu_id = item_ptr->menu_id;
                item_info_ptr->group_id = menu_ctrl_ptr->cur_group_id;

                result = TRUE;
            }
        }
        else
        {
            item_node_ptr = CTRLMENU_GetNodeByVisibleIndex(item_index,menu_ctrl_ptr->cur_parent_node_ptr);
            if (PNULL != item_node_ptr)
            {
                item_info_ptr->node_id = item_node_ptr->node_id;

                result = TRUE;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get item
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for static menu.
/*****************************************************************************/
LOCAL GUIMENU_ITEM_T* GetItemByVisibleIndex(
                                            CTRLMENU_OBJ_T          *menu_ctrl_ptr,     //[in]
                                            uint16                  visible_index,      //[in]
                                            CTRLMENU_ITEM_INFO_T    *out_item_info_ptr  //[out]
                                            )
{
    int32                   i = 0;
    uint16                  item_count = 0;
    uint16                  cur_visible_index = 0;
    GUIMENU_ITEM_T          *item_ptr = PNULL;
    CTRLMENU_ITEM_INFO_T    *item_info_ptr = PNULL;
    GUIMENU_GROUP_T         *menu_group_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != menu_ctrl_ptr->cur_group_ptr);  /*assert verified*/
    if ((PNULL == menu_ctrl_ptr)||(PNULL == menu_ctrl_ptr->cur_group_ptr))
    {
        return PNULL;
    }

    // here, we should get group and total item count to find all items.
    // and we should get item info to see whether item is visible or not.
    // 静态菜单必须要有cur_group_ptr和cur_group_info_ptr
    SCI_ASSERT(PNULL != menu_ctrl_ptr->cur_group_info_ptr); /*assert verified*/

    menu_group_ptr = menu_ctrl_ptr->cur_group_ptr;
    item_count = menu_ctrl_ptr->cur_group_ptr->item_count;
    item_info_ptr = GET_ITEM_INFO(menu_ctrl_ptr->cur_group_info_ptr);

    // check all items in this group. for using visible index, we do not 
    // consider invisible item
    for (i = 0; i < item_count; i++)
    {
        if (item_info_ptr[i].is_visible)
        {
            if (visible_index == cur_visible_index)
            {
                item_ptr = (GUIMENU_ITEM_T*)&menu_group_ptr->item_ptr[i];

                if (PNULL != out_item_info_ptr)
                {
                    *out_item_info_ptr = item_info_ptr[i];
                }

                break;
            }

            cur_visible_index++;
        }
    }

    return item_ptr;
}

/*****************************************************************************/
//  Description : get node pointer by visible index
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLMENU_NODE_T* CTRLMENU_GetNodeByVisibleIndex(
                                                       uint16           node_index,      //[in]
                                                       CTRLMENU_NODE_T  *parent_node_ptr //[in]
                                                       )
{
    int32           i = 0;
    CTRLMENU_NODE_T *find_node_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != parent_node_ptr); /*assert verified*/
    if (PNULL == parent_node_ptr)
    {
        return PNULL;
    }

    if (node_index >= parent_node_ptr->visible_child_node_num)
    {
        return PNULL;
    }

    SCI_ASSERT(PNULL != parent_node_ptr->child_node_ptr);   /*assert verified*/
    find_node_ptr = parent_node_ptr->child_node_ptr;
    while (PNULL != find_node_ptr)
    {
        if (!find_node_ptr->is_visible)
        {
            find_node_ptr = find_node_ptr->neighbour_node_ptr;
            continue;
        }

        if (i == node_index)
        {
            break;
        }

        find_node_ptr = find_node_ptr->neighbour_node_ptr;
        i++;
    }

    // 如果找不到对应的node, 说明parent_node_ptr存的信息有误，肯定是插删节点时没有更新所致
    SCI_ASSERT(PNULL != find_node_ptr); /*assert verified*/

    return (find_node_ptr);
}

/*****************************************************************************/
//  Description : set pop-up menu item status for radio or check pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetPopItemStatus(
                                         GUIMENU_POP_SEL_INFO_T     *selected_info_ptr
                                         )
{
    BOOLEAN                 result = FALSE;
    CTRLMENU_OBJ_T          *menu_ctrl_ptr = PNULL;
    CTRLMENU_POP_SEL_ITEM_T *item_ptr = PNULL;

    if (PNULL == selected_info_ptr)
    {
        return result;
    }
    
    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(selected_info_ptr->ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //convert handle
        selected_info_ptr->ctrl_id = MMK_ConvertIdToHandle(selected_info_ptr->ctrl_id);

        item_ptr = menu_ctrl_ptr->selected_list.init_item_ptr;
        while (PNULL != item_ptr)
        {
            if ((item_ptr->item_info.is_static == selected_info_ptr->is_static) &&
                (item_ptr->item_info.node_id == selected_info_ptr->node_id) &&
                (item_ptr->item_info.ctrl_id == selected_info_ptr->ctrl_id) &&
                (item_ptr->item_info.menu_id == selected_info_ptr->menu_id) &&
                (item_ptr->item_info.group_id == selected_info_ptr->group_id))
            {
                result = TRUE;
                break;
            }
            item_ptr = item_ptr->next_ptr;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get item text string, icon id, and is_grayed state
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for menu view
//        if it is option page style, get the item in page.
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetItem(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr, // [in]
                                uint16          item_index,     // [in]
                                MMI_STRING_T    *text_ptr,      // [out]
                                MMI_IMAGE_ID_T  *icon_id_ptr,   // [out]
                                BOOLEAN         *is_grayed_ptr  // [out]
                                )
{
    BOOLEAN                 result = FALSE;
    GUIMENU_ITEM_T          *item_ptr = PNULL;
    CTRLMENU_NODE_T         *item_node_ptr = PNULL;
    CTRLMENU_ITEM_INFO_T    item_info = {0};
    CTRLMENU_STATIC_ITEM_T  *static_item_ptr = PNULL;

    if (PNULL == menu_ctrl_ptr ||
        (PNULL == text_ptr && PNULL == icon_id_ptr && PNULL == is_grayed_ptr))
    {
        return result;
    }

	if (menu_ctrl_ptr->is_static)
	{
		item_ptr = GetItemByVisibleIndex(menu_ctrl_ptr, item_index, &item_info);
		
		static_item_ptr = GetStaticItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_group_id, item_ptr->menu_id);
		
		if (PNULL != text_ptr)
		{
			if (PNULL != static_item_ptr && PNULL != static_item_ptr->text_str.wstr_ptr)
			{
				*text_ptr = static_item_ptr->text_str;
			}
			else
			{
				MMITHEME_GetLabelTextByLang(item_ptr->text_str_id, text_ptr);
			}
		}
		
		if (PNULL != icon_id_ptr)
		{
			if (PNULL != static_item_ptr && 0 != static_item_ptr->icon_id)
			{
				*icon_id_ptr = static_item_ptr->icon_id;
			}
			else
			{
				*icon_id_ptr = item_ptr->select_icon_id;
			}
		}
		
		if (PNULL != is_grayed_ptr)
		{
			// check global setting first
			*is_grayed_ptr = MMITHEME_IsMenuItemGrayed(
                menu_ctrl_ptr->win_handle,
                menu_ctrl_ptr->cur_group_id,
                item_ptr->menu_id);
			
			// then check whether user set this menu object grayed
			if (!(*is_grayed_ptr))
			{
				*is_grayed_ptr = item_info.is_grayed;
			}
		}
		
		result = TRUE;
	}
	else
	{
		//get current item node pointer
		item_node_ptr = CTRLMENU_GetNodeByVisibleIndex(
			item_index,
			menu_ctrl_ptr->cur_parent_node_ptr);
		if (PNULL != item_node_ptr)
		{
			if (PNULL != text_ptr)
			{
				text_ptr->wstr_len = item_node_ptr->node_wstr_len;
				text_ptr->wstr_ptr = item_node_ptr->node_wstr_ptr;
			}
			
			if (PNULL != is_grayed_ptr)
			{
				*is_grayed_ptr = item_node_ptr->is_grayed;
			}
			
			if (PNULL != icon_id_ptr && DYNA_ITEM_ICON_ID == item_node_ptr->select_icon_type)
			{
				*icon_id_ptr = item_node_ptr->select_icon_data.icon_id;
			}
			
			result = TRUE;
		}
	}
	
    return result;
}

/*****************************************************************************/
//  Description : get item text string, icon buffer data, and is_grayed state
//  Global resource dependence : 
//  Author: Hongjun.Jia
//  Note:   this method is for dynamic menu
//			只适用于item icon为buffer的情况。
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetItemForIconBuffer(
                                CTRLMENU_OBJ_T					*menu_ctrl_ptr,		// [in]
                                uint16							item_index,			// [in]
                                MMI_STRING_T					*text_ptr,			// [out]
                                CTRLMENU_SELECT_ICON_DATA_T		*icon_buffer_ptr,   // [out]
                                BOOLEAN							*is_grayed_ptr		// [out]
                                )
{
    BOOLEAN                 result = FALSE;
    CTRLMENU_NODE_T         *item_node_ptr = PNULL;

    if (PNULL == menu_ctrl_ptr ||
        (PNULL == text_ptr && PNULL == icon_buffer_ptr && PNULL == is_grayed_ptr))
    {
        return result;
    }

	if ( !menu_ctrl_ptr->is_static )
	{
		//get current item node pointer
		item_node_ptr = CTRLMENU_GetNodeByVisibleIndex(item_index, menu_ctrl_ptr->cur_parent_node_ptr);
		if (PNULL != item_node_ptr)
		{
			if (PNULL != text_ptr)
			{
				text_ptr->wstr_len = item_node_ptr->node_wstr_len;
				text_ptr->wstr_ptr = item_node_ptr->node_wstr_ptr;
			}
			
			if (PNULL != is_grayed_ptr)
			{
				*is_grayed_ptr = item_node_ptr->is_grayed;
			}
			
			if ( PNULL != icon_buffer_ptr && DYNA_ITEM_ICON_BUFFER == item_node_ptr->select_icon_type)
			{
				icon_buffer_ptr->buffer.data = item_node_ptr->select_icon_data.buffer.data;
				icon_buffer_ptr->buffer.height = item_node_ptr->select_icon_data.buffer.height;
				icon_buffer_ptr->buffer.width = item_node_ptr->select_icon_data.buffer.width;
			}
			
			result = TRUE;
		}
	}
	
    return result;
}

/*****************************************************************************/
//  Description : get item for static menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_STATIC_ITEM_T* GetStaticItem(
                                            CTRLMENU_OBJ_T      *menu_ctrl_ptr, // [in]
                                            MMI_MENU_GROUP_ID_T group_id,       // [in]
                                            MMI_MENU_ID_T       menu_id         // [in]
                                            )
{
    CTRLMENU_STATIC_ITEM_T *cur_static_item_ptr = menu_ctrl_ptr->static_item_ptr;

    while (PNULL != cur_static_item_ptr)
    {
        if (cur_static_item_ptr->group_id == group_id 
            && cur_static_item_ptr->menu_id == menu_id)
        {
            break;
        }

        cur_static_item_ptr = cur_static_item_ptr->next_item_ptr;
    }

    return cur_static_item_ptr;
}

/*****************************************************************************/
//  Description : get menu font color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T CTRLMENU_GetFontColor(
                                         CTRLMENU_OBJ_T     *menu_ctrl_ptr
                                         )
{
    GUI_COLOR_T     font_color = 0;

    if (menu_ctrl_ptr->is_set_font_color)
    {
        font_color = menu_ctrl_ptr->font_color;
    }
    else
    {
        if (CTRLMENU_IsPopMenu(menu_ctrl_ptr))
        {
            font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_POPMENU);
        }
        else
        {
            font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_MENU);
        }
    }
    
    return (font_color);
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_StartItemTextTimer(
                                        CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                        )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr  = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if ((PNULL != menu_ctrl_ptr) && 
        (0 == menu_ctrl_ptr->item_timer_id))
    {
        menu_ctrl_ptr->item_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,menu_ctrl_ptr->sec_theme.item_scroll_period,FALSE);
    }
}

/*****************************************************************************/
//  Description : stop item text timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_StopItemTextTimer(
                                       CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                       )
{
    if (PNULL != menu_ctrl_ptr && 0 < menu_ctrl_ptr->item_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->item_timer_id);
        menu_ctrl_ptr->item_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : get title text
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for menu view
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetTitleText(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr, //[in]
                                     MMI_STRING_T       *title_str_ptr, //[out]
                                     MMI_STRING_T       *sub_title_str_ptr, // [out]
                                     MMI_IMAGE_ID_T     *title_icon_id_ptr  // [out]
                                     )
{
    if (PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (menu_ctrl_ptr->is_static)
    {
        if (PNULL != title_str_ptr)
        {
            if (PNULL != menu_ctrl_ptr->cur_group_info_ptr->title_str.wstr_ptr
                && 0 != menu_ctrl_ptr->cur_group_info_ptr->title_str.wstr_len)
            {
                *title_str_ptr = menu_ctrl_ptr->cur_group_info_ptr->title_str;
            }
            else if (0 < menu_ctrl_ptr->title_len)
            {
                title_str_ptr->wstr_len = menu_ctrl_ptr->title_len;
                title_str_ptr->wstr_ptr = menu_ctrl_ptr->title_wstr;
            }
            else
            {
                MMITHEME_GetLabelTextByLang(
                        menu_ctrl_ptr->cur_group_ptr->title_string_id,
                        title_str_ptr);
            }
        }

        if (PNULL != sub_title_str_ptr)
        {
            if (PNULL != menu_ctrl_ptr->cur_group_info_ptr->sub_title_str.wstr_ptr
                && 0 != menu_ctrl_ptr->cur_group_info_ptr->sub_title_str.wstr_len)
            {
                *sub_title_str_ptr = menu_ctrl_ptr->cur_group_info_ptr->sub_title_str;
            }
        }

        if (PNULL != title_icon_id_ptr)
        {
            *title_icon_id_ptr = menu_ctrl_ptr->cur_group_info_ptr->title_icon_id;
        }
    }
    else
    {
        if (PNULL != title_str_ptr)
        {
            if (PNULL != menu_ctrl_ptr->cur_parent_node_ptr->title_str.wstr_ptr
                && 0 != menu_ctrl_ptr->cur_parent_node_ptr->title_str.wstr_len)
            {
                *title_str_ptr = menu_ctrl_ptr->cur_parent_node_ptr->title_str;
            }
            //if it is the first level menu
            else if (PNULL == menu_ctrl_ptr->cur_parent_node_ptr->parent_node_ptr)
            {
                title_str_ptr->wstr_len = menu_ctrl_ptr->title_len;
                title_str_ptr->wstr_ptr = menu_ctrl_ptr->title_wstr;
            }
            else
            {
                title_str_ptr->wstr_len = menu_ctrl_ptr->cur_parent_node_ptr->node_wstr_len;
                title_str_ptr->wstr_ptr = menu_ctrl_ptr->cur_parent_node_ptr->node_wstr_ptr;
            }
        }

        if (PNULL != sub_title_str_ptr)
        {
            if (PNULL != menu_ctrl_ptr->cur_parent_node_ptr->sub_title_str.wstr_ptr
                && 0 != menu_ctrl_ptr->cur_parent_node_ptr->sub_title_str.wstr_len)
            {
                *sub_title_str_ptr = menu_ctrl_ptr->cur_parent_node_ptr->sub_title_str;
            }
        }

        if (PNULL != title_icon_id_ptr)
        {
            *title_icon_id_ptr = menu_ctrl_ptr->cur_parent_node_ptr->title_icon_id;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : set menu softkey param
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSoftkey(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr
                                )
{
// PDA不能设置softkey
#ifdef GUIF_SOFTKEY_PDASTYLE
    GUI_INVALID_PARAM(menu_ctrl_ptr);   /*lint !e522*/
#else
    GUI_COLOR_T     font_color = 0;
    //GUI_SOFTKEY_T   softkey_info = {0};
    GUI_BUTTON_T    button_arr[GUISOFTKEY_BUTTON_NUM] = {0};

    //SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }

    GetSoftkeyText(menu_ctrl_ptr, button_arr);
        
    font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);
    
	GUIWIN_SetSoftkeyTextId(menu_ctrl_ptr->win_handle,  
        button_arr[0].content.data_u.text_id,
        button_arr[1].content.data_u.text_id,
        button_arr[2].content.data_u.text_id,
        TRUE);
#endif
}

#ifndef GUIF_SOFTKEY_PDASTYLE
/*****************************************************************************/
//  Description : get softkey text id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetSoftkeyText(
                          CTRLMENU_OBJ_T    *menu_ctrl_ptr, //in
                          GUI_BUTTON_T      *button_id_ptr  //out
                          )
{
    BOOLEAN                 is_null_softkey = TRUE;
    uint16                  i = 0;
    MMI_TEXT_ID_T           *softkey_id_ptr = PNULL;
    GUIMENU_ITEM_T          *item_ptr = PNULL;

    if (PNULL != button_id_ptr)
    {
        if (menu_ctrl_ptr->is_static)
        {
            item_ptr = GetItemByVisibleIndex(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, PNULL);
            if (PNULL != item_ptr)
            {
                softkey_id_ptr = (MMI_IMAGE_ID_T *)item_ptr->button_id;
            }
        }
        else
        {
            softkey_id_ptr = menu_ctrl_ptr->dyna_softkey_id;
        }

        //SCI_ASSERT(PNULL != softkey_id_ptr);/*assert verified*/
        if (PNULL == softkey_id_ptr)
        {
            return;
        }

        //judge all softkey is null
        for (i=0; i<GUISOFTKEY_BUTTON_NUM; i++)
        {
            if (!MMI_IsTextNull(softkey_id_ptr[i]))
            {
                is_null_softkey = FALSE;
                break;
            }
        }

        //set default softkey text
        if (is_null_softkey)
        {
            button_id_ptr[0].content.data_u.text_id = menu_ctrl_ptr->common_theme.text_ok_id;
            button_id_ptr[1].content.data_u.text_id = menu_ctrl_ptr->common_theme.text_null_id;
            button_id_ptr[2].content.data_u.text_id = menu_ctrl_ptr->common_theme.text_return_id;
        }
        else
        {
            for (i=0; i<GUISOFTKEY_BUTTON_NUM; i++)
            {
		        button_id_ptr[i].content.data_u.text_id = softkey_id_ptr[i];
            }
        }
    }
}
#endif

/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMENU_NotifyMsg(
                                       MMI_HANDLE_T       ctrl_handle,
                                       MMI_MESSAGE_ID_E   msg_id
                                       )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    MMI_MESSAGE_ID_E    notify_msg = MSG_NOTIFY_START;

    switch (msg_id)
    {
    case MSG_APP_WEB:
        notify_msg = MSG_NOTIFY_MIDSK;
        break;

    case MSG_APP_OK:
        notify_msg = MSG_NOTIFY_OK;
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_UP:
        notify_msg = MSG_NOTIFY_PENOK;
        break;
#endif

    case MSG_APP_CANCEL:
        notify_msg = MSG_NOTIFY_CANCEL;
        break;

    default:
        break;
    }

    if (MSG_NOTIFY_START != notify_msg)
    {
        //notify parent control
        result = GUICTRL_PostNotify(ctrl_handle,notify_msg);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get lcd dev info
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO* CTRLMENU_GetPopLcdDev(
                                               CTRLMENU_OBJ_T     *menu_ctrl_ptr,     //[in]
                                               BOOLEAN            *is_common_lcd      // [out]
                                               )
{
#if defined UI_MULTILAYER_SUPPORT && defined GUI_FULLSCREEN_POPMENU_SUPPORT 
    return &menu_ctrl_ptr->item_layer;
#else
#ifdef UI_MULTILAYER_SUPPORT
    if (CTRLMENU_IsLayerValid(&menu_ctrl_ptr->popup_layer_handle))
    {
        if (PNULL != is_common_lcd)
        {
            *is_common_lcd = FALSE;
        }

        return &menu_ctrl_ptr->popup_layer_handle;
    }
    else
#endif
    {
        CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

        if (PNULL != is_common_lcd)
        {
            *is_common_lcd = TRUE;
        }

        return &base_ctrl_ptr->lcd_dev_info;
    }
#endif
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 判断层是否有效
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsLayerValid(
                                     GUI_LCD_DEV_INFO   *lcd_dev_info_ptr
                                     )
{
    BOOLEAN is_valid = FALSE;
    
    if (PNULL == lcd_dev_info_ptr
        || 0 == lcd_dev_info_ptr->block_id
        || UILAYER_NULL_HANDLE == lcd_dev_info_ptr->block_id)
    {
        is_valid = FALSE;
    }
    else
    {
        is_valid = TRUE;
    }

    return is_valid;
}
#endif

/*****************************************************************************/
//  Description : current menu is have child menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for menu view.
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsExistChildMenu(                               // [ret] TRUE when has child menu
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr, // [in]
                                         uint16         visible_index,  // [in]
                                         BOOLEAN        *is_grayed_ptr  // [out] may PNULL
                                         )
{
    BOOLEAN                 is_exist_child_menu = FALSE;
    BOOLEAN                 is_grayed = FALSE;
    GUIMENU_ITEM_T          *item_ptr = PNULL;
    CTRLMENU_NODE_T         *item_node_ptr  = PNULL;
    CTRLMENU_ITEM_INFO_T    item_info   = {0};

    if (menu_ctrl_ptr->is_static)
    {
        item_ptr = GetItemByVisibleIndex(menu_ctrl_ptr, visible_index, &item_info);

        // check global setting first
        is_grayed = MMITHEME_IsMenuItemGrayed(
                                        menu_ctrl_ptr->win_handle,
                                        menu_ctrl_ptr->cur_group_id,
                                        item_ptr->menu_id);

        // then check whether user set this menu object grayed
        if (!is_grayed)
        {
            is_grayed = item_info.is_grayed;
        }

        //get the next menu group
        if (PNULL != MMITHEME_GetMenuGroup(item_ptr->link_group_id))
        {
            is_exist_child_menu  = TRUE;
        }
    }
    else
    {
        item_node_ptr = CTRLMENU_GetNodeByVisibleIndex(
                                        visible_index,
                                        menu_ctrl_ptr->cur_parent_node_ptr);
        if (PNULL != item_node_ptr)
        {
            is_grayed = item_node_ptr->is_grayed;

            if (PNULL != item_node_ptr->child_node_ptr)
            {
                is_exist_child_menu  = TRUE;
            }
        }
    }

    if (PNULL != is_grayed_ptr)
    {
        *is_grayed_ptr = is_grayed;
    }

    return (is_exist_child_menu);
}

/*****************************************************************************/
//  Description : calculate menu rect for pop-up,options
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_CalculateRect(
                                   uint16           rect_top,       //only for sub pop-up menu
                                   CTRLMENU_OBJ_T   *menu_ctrl_ptr
                                   )
{
    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        CTRLMENU_CalcPopupRect(rect_top,menu_ctrl_ptr);
        break;

    case GUIMENU_STYLE_OPTIONS:
#ifdef MMI_OPTIONMENU_SUPPORT
        CTRLMENU_OptionsLayout(menu_ctrl_ptr);
#endif
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : set menu info,include menu level,current and first index etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetInfo(
                             uint16                 menu_level,
                             uint16                 first_item_index,
                             uint16                 cur_item_index,
                             GUI_RECT_T             menu_rect,
                             int16                  offset_y,
                             MMI_MENU_GROUP_ID_T    cur_group_id,
                             CTRLMENU_NODE_T        *cur_parent_node_ptr,
                             CTRLMENU_OBJ_T         *menu_ctrl_ptr
                             )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    menu_ctrl_ptr->menu_level       = menu_level;
    menu_ctrl_ptr->cur_group_id     = cur_group_id;
    menu_ctrl_ptr->first_item_index = first_item_index;
    menu_ctrl_ptr->cur_item_index   = cur_item_index;
    base_ctrl_ptr->rect             = menu_rect;
    menu_ctrl_ptr->offset_y         = offset_y;

    if (menu_ctrl_ptr->is_static)
    {
        //set menu group etc. info
        menu_ctrl_ptr->cur_group_ptr      = (GUIMENU_GROUP_T *)MMITHEME_GetMenuGroup(menu_ctrl_ptr->cur_group_id);
        menu_ctrl_ptr->cur_group_info_ptr = CTRLMENU_GetGroupInfo(menu_ctrl_ptr, menu_ctrl_ptr->cur_group_id);
        menu_ctrl_ptr->item_total_num     = menu_ctrl_ptr->cur_group_info_ptr->visible_child_item_num;
        menu_ctrl_ptr->cur_style          = menu_ctrl_ptr->cur_group_ptr->menu_style;
    }
    else
    {
        //set menu group etc. info
        menu_ctrl_ptr->cur_parent_node_ptr = cur_parent_node_ptr;
        menu_ctrl_ptr->item_total_num      = menu_ctrl_ptr->cur_parent_node_ptr->visible_child_node_num;
        menu_ctrl_ptr->cur_style           = menu_ctrl_ptr->cur_parent_node_ptr->sub_menu_style;
    }
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style
		|| GUIMENU_STYLE_POPUP_AUTO == menu_ctrl_ptr->cur_style)
	{
		menu_ctrl_ptr->cur_style = GUIMENU_STYLE_SECOND;
	}	
#endif
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    if (menu_level > 0)
    {
        CheckSubMenuStyle(menu_ctrl_ptr);
    }
#endif

    //destroy progress
    if (PNULL != menu_ctrl_ptr->prgbox_ctrl_ptr)
    {
        MMK_DestroyControl(IGUICTRL_GetCtrlHandle(menu_ctrl_ptr->prgbox_ctrl_ptr));
        menu_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : get group info
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for static menu
/*****************************************************************************/
PUBLIC CTRLMENU_GROUP_INFO_T* CTRLMENU_GetGroupInfo(
                                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr, //[in]
                                                    MMI_MENU_GROUP_ID_T group_id        //[in]
                                                    )
{
    CTRLMENU_GROUP_INFO_T *group_info_ptr = menu_ctrl_ptr->root_group_info_ptr;
    
    while(PNULL != group_info_ptr)
    {
        if (group_info_ptr->group_id == group_id)
        {
            break;
        }

        group_info_ptr = group_info_ptr->next_group_info_ptr;
    }

    return group_info_ptr;
}

/*****************************************************************************/
//  Description : check button style
//  Global resource dependence : 
//  Author: hua.fang
//  Note: button指的是context menu底下的按钮
/*****************************************************************************/
PUBLIC void CTRLMENU_CheckButtonStyle(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      )
{
    // 因为CHECK风格点击后不发消息，所以必须要有OK CANCEL键
    if (GUIMENU_STYLE_POPUP_CHECK == menu_ctrl_ptr->cur_style)
    {
        menu_ctrl_ptr->cur_button_style = GUIMENU_BUTTON_STYLE_OK_CANCEL;
    }
    else
    {
        // iphone风格下，因为没有cancel按钮，所以要出cancel button
        if ((GUIMENU_BUTTON_STYLE_NONE == menu_ctrl_ptr->cur_button_style) && 
            (MMITHEME_IsIstyle()))
        {
            menu_ctrl_ptr->cur_button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        }
    }
}

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
/*****************************************************************************/
//  Description : check sub menu style
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckSubMenuStyle(
                             CTRLMENU_OBJ_T *menu_ctrl_ptr
                             )
{
    if (GUIMENU_STYLE_POPUP_CHECK != menu_ctrl_ptr->cur_style
        && GUIMENU_STYLE_POPUP_RADIO != menu_ctrl_ptr->cur_style
        && GUIMENU_STYLE_SECOND != menu_ctrl_ptr->cur_style)
    {
        menu_ctrl_ptr->cur_style = GUIMENU_STYLE_POPUP_AUTO;
    }
}
#endif

/*****************************************************************************/
//  Description : reset menu, stop timer and free resource etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_ResetInfo(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               )
{
    if (PNULL != menu_ctrl_ptr)
    {
        //stop item text timer
        CTRLMENU_StartItemTextTimer(menu_ctrl_ptr);
        menu_ctrl_ptr->item_str_index = 0;

        menu_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
    }
}

/*****************************************************************************/
//  Description : find node pointer by index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLMENU_NODE_T* CTRLMENU_FindNodeByIndex(
                                                 uint16             node_index,
                                                 CTRLMENU_NODE_T    *parent_node_ptr
                                                 )
{
    uint16          i = 0;
    CTRLMENU_NODE_T *find_node_ptr = PNULL;

    if (PNULL == parent_node_ptr || PNULL == parent_node_ptr->child_node_ptr)
    {
        return find_node_ptr;
    }

    find_node_ptr = parent_node_ptr->child_node_ptr;
    for (i=0; i<node_index; i++)
    {
        find_node_ptr = find_node_ptr->neighbour_node_ptr;
    }

    return (find_node_ptr);
}

/*****************************************************************************/
//  Description : 更新下面的窗口
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_UpdatePrevWindow(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr  // [in]
                                      )
{
    MMI_HANDLE_T    prev_win_handle = 0;

    prev_win_handle = MMK_GetPrevWinHandle(menu_ctrl_ptr->win_handle);
    MMK_SendMsg(prev_win_handle,MSG_FULL_PAINT,PNULL);
}

/*****************************************************************************/
//  Description : get item information
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetInfoInOK(
                                    CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                    BOOLEAN             *is_gray_ptr,
                                    BOOLEAN             *is_exist_child_ptr,
                                    GUIMENU_ITEM_T      *cur_item_ptr,
                                    CTRLMENU_NODE_T     **cur_node_pptr
                                    )
{
    BOOLEAN                 result = FALSE;
    BOOLEAN                 is_grayed = FALSE;
    BOOLEAN                 is_exist_child = FALSE;
    MMI_MENU_GROUP_ID_T     cur_group_id  = 0;
    GUIMENU_ITEM_T          cur_item = {0};
    CTRLMENU_ITEM_INFO_T    cur_item_info = {0};
    GUIMENU_GROUP_T         *menu_group_ptr = PNULL;
    CTRLMENU_NODE_T         *cur_node_ptr = PNULL;

    if (menu_ctrl_ptr->is_static)
    {
        //get current menu item pointer
        if (CTRLMENU_GetCurItem(menu_ctrl_ptr, &cur_item, &cur_item_info))   // 有可能菜单项为0
        {
            //get current group id
            cur_group_id = CTRLMENU_GetCurGroupId(menu_ctrl_ptr);

            //judge item is gray
            is_grayed = MMITHEME_IsMenuItemGrayed(menu_ctrl_ptr->win_handle, cur_group_id, cur_item.menu_id);
            if (!is_grayed)
            {
                is_grayed = cur_item_info.is_grayed;
            }

            menu_group_ptr = (GUIMENU_GROUP_T *)MMITHEME_GetMenuGroup(cur_item.link_group_id);
            if (PNULL == menu_group_ptr)
            {
                is_exist_child = FALSE;
            }
            else
            {
                is_exist_child = TRUE;
            }

            result = TRUE;
        }
    }
    else
    {
        cur_node_ptr = CTRLMENU_GetNodeByVisibleIndex(menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr->cur_parent_node_ptr);
        if (PNULL != cur_node_ptr)
        {
            is_grayed = cur_node_ptr->is_grayed;

            if (PNULL == cur_node_ptr->child_node_ptr)
            {
                is_exist_child = FALSE;
            }
            else
            {
                is_exist_child = TRUE;
            }

            result = TRUE;
        }
    }

    if (PNULL != is_gray_ptr)
    {
        *is_gray_ptr = is_grayed;
    }

    if (PNULL != is_exist_child_ptr)
    {
        *is_exist_child_ptr = is_exist_child;
    }

    if (PNULL != cur_item_ptr)
    {
        *cur_item_ptr = cur_item;
    }

    if (PNULL != cur_node_pptr)
    {
        *cur_node_pptr = cur_node_ptr;
    }

    return result;
}

/*****************************************************************************/
//  Description : restore menu info in ok
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_RestoreInfoInOK(
                                     CTRLMENU_OBJ_T     *menu_ctrl_ptr,
                                     GUIMENU_ITEM_T     *cur_item_ptr,
                                     CTRLMENU_NODE_T    *cur_node_ptr
                                     )
{
    if (menu_ctrl_ptr->is_static)
    {
        SCI_ASSERT(PNULL != cur_item_ptr);  /*assert verified*/

        menu_ctrl_ptr->cur_group_id         = cur_item_ptr->link_group_id;
        menu_ctrl_ptr->cur_group_ptr        = (GUIMENU_GROUP_T *)MMITHEME_GetMenuGroup(menu_ctrl_ptr->cur_group_id);
        menu_ctrl_ptr->cur_group_info_ptr   = CTRLMENU_GetGroupInfo(menu_ctrl_ptr, menu_ctrl_ptr->cur_group_id);
        menu_ctrl_ptr->item_total_num       = menu_ctrl_ptr->cur_group_info_ptr->visible_child_item_num;
        menu_ctrl_ptr->cur_style            = menu_ctrl_ptr->cur_group_ptr->menu_style;
        menu_ctrl_ptr->cur_button_style     = menu_ctrl_ptr->cur_group_info_ptr->button_style;
    }
    else
    {
        SCI_ASSERT(PNULL != cur_node_ptr);  /*assert verified*/

        menu_ctrl_ptr->cur_parent_node_ptr  = cur_node_ptr;
        menu_ctrl_ptr->item_total_num       = menu_ctrl_ptr->cur_parent_node_ptr->visible_child_node_num;
        menu_ctrl_ptr->cur_style            = menu_ctrl_ptr->cur_parent_node_ptr->sub_menu_style;
        menu_ctrl_ptr->cur_button_style     = menu_ctrl_ptr->cur_parent_node_ptr->button_style;
    }
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style
		|| GUIMENU_STYLE_POPUP_AUTO == menu_ctrl_ptr->cur_style)
	{
		menu_ctrl_ptr->cur_style = GUIMENU_STYLE_SECOND;
	}	
#endif

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    CheckSubMenuStyle(menu_ctrl_ptr);
#endif

    CTRLMENU_CheckButtonStyle(menu_ctrl_ptr);

    menu_ctrl_ptr->cur_item_index       = 0;
    menu_ctrl_ptr->first_item_index     = 0;
    menu_ctrl_ptr->is_first_disp        = TRUE;
}

/*****************************************************************************/
//  Description : 重置经过cancel后menu数据
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLMENU_RestoreInfoInCancel(
                                         CTRLMENU_OBJ_T *menu_ctrl_ptr
                                         )
{
    if (menu_ctrl_ptr->is_static)
    {
        menu_ctrl_ptr->cur_group_ptr        = (GUIMENU_GROUP_T *)MMITHEME_GetMenuGroup(menu_ctrl_ptr->cur_group_id);
        menu_ctrl_ptr->cur_group_info_ptr   = CTRLMENU_GetGroupInfo(menu_ctrl_ptr, menu_ctrl_ptr->cur_group_id);
        menu_ctrl_ptr->item_total_num       = menu_ctrl_ptr->cur_group_info_ptr->visible_child_item_num;
        menu_ctrl_ptr->cur_style            = menu_ctrl_ptr->cur_group_ptr->menu_style;
        menu_ctrl_ptr->cur_button_style     = menu_ctrl_ptr->cur_group_info_ptr->button_style;
    }
    else
    {
        menu_ctrl_ptr->cur_parent_node_ptr  = menu_ctrl_ptr->cur_parent_node_ptr->parent_node_ptr;
        menu_ctrl_ptr->item_total_num       = menu_ctrl_ptr->cur_parent_node_ptr->visible_child_node_num;
        menu_ctrl_ptr->cur_style            = menu_ctrl_ptr->cur_parent_node_ptr->sub_menu_style;
        menu_ctrl_ptr->cur_button_style     = menu_ctrl_ptr->cur_parent_node_ptr->button_style;
    }
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	if (GUIMENU_STYLE_POPUP == menu_ctrl_ptr->cur_style
		|| GUIMENU_STYLE_POPUP_AUTO == menu_ctrl_ptr->cur_style)
	{
		menu_ctrl_ptr->cur_style = GUIMENU_STYLE_SECOND;
	}	
#endif

    CTRLMENU_CheckButtonStyle(menu_ctrl_ptr);

    menu_ctrl_ptr->is_first_disp = FALSE;
}

/*****************************************************************************/
//  Description : get current item pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: this method is for static menu style
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetCurItem(
                                   CTRLMENU_OBJ_T       *menu_ctrl_ptr, //in
                                   GUIMENU_ITEM_T       *item_ptr,      //out
                                   CTRLMENU_ITEM_INFO_T *item_info_ptr  //out
                                   )
{
    BOOLEAN         result = TRUE;
    GUIMENU_ITEM_T  *temp_item_ptr = PNULL;

    if ((PNULL == menu_ctrl_ptr)||(PNULL == item_ptr))
    {
        return FALSE;
    }

    if (PNULL != menu_ctrl_ptr->cur_group_ptr)
    {
        temp_item_ptr = GetItemByVisibleIndex(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index, item_info_ptr);

        if (PNULL != temp_item_ptr)
        {
            *item_ptr = *temp_item_ptr;
        }
        else
        {
            result = FALSE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : get current group id
//  Global resource dependence : 
//  Author: hua.fang
//  Note: this method is for static menu style
/*****************************************************************************/
PUBLIC MMI_MENU_GROUP_ID_T CTRLMENU_GetCurGroupId(
                                                  CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                                  )
{
    MMI_MENU_GROUP_ID_T cur_group_id    = 0;

    if (menu_ctrl_ptr->is_static)
    {
        //get current group id
        cur_group_id = menu_ctrl_ptr->cur_group_id;
    }

    return cur_group_id;
}

/*****************************************************************************/
//  Description : adjust first item index and display before ok
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_AdjustFirstAndDisplay(
                                           CTRLMENU_OBJ_T   *menu_ctrl_ptr,
                                           BOOLEAN          is_need_update
                                           )
{
    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
        CTRLMENU_SecAdjustFirstAndDisplay(menu_ctrl_ptr,is_need_update);
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        CTRLMENU_PopAdjustFirstAndDisplay(menu_ctrl_ptr,is_need_update);
        break;

    case GUIMENU_STYLE_OPTIONS:
        break;

    default:
        SCI_TRACE_LOW("CTRLMENU_Init:menu style %d is error!",menu_ctrl_ptr->cur_style);
        break;
    }
}

/*****************************************************************************/
//  Description : set first and current item index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetCurAndFirstItemIndex(
                                                CTRLMENU_OBJ_T      *menu_ctrl_ptr,
                                                uint16              line_num_page,
                                                MMI_MESSAGE_ID_E    msg_id
                                                )
{
    BOOLEAN result = FALSE;

    switch (msg_id)
    {
    case MSG_APP_UP:
    case MSG_APP_LEFT:
        //set current item index
        if (0 < menu_ctrl_ptr->cur_item_index)
        {
            menu_ctrl_ptr->cur_item_index--;

            result = TRUE;
        }
        else if (menu_ctrl_ptr->common_theme.is_circle)
        {
            menu_ctrl_ptr->cur_item_index = (uint16)(menu_ctrl_ptr->item_total_num - 1);

            result = TRUE;
        }
        break;

    case MSG_APP_DOWN:
    case MSG_APP_RIGHT:
        //set current item index
        if (menu_ctrl_ptr->cur_item_index < (menu_ctrl_ptr->item_total_num - 1))
        {
            menu_ctrl_ptr->cur_item_index++;

            result = TRUE;
        }
        else if (menu_ctrl_ptr->common_theme.is_circle)
        {
            menu_ctrl_ptr->cur_item_index = 0;

            result = TRUE;
        }
        break;

    default:
        SCI_PASSERT(FALSE,("CTRLMENU_SetCurAndFirstItemIndex: msg_id %d is error!",msg_id)); /*assert verified*/
        break;
    }

	//set first item index
	if (result && menu_ctrl_ptr->item_total_num > line_num_page)
	{
		//current item is not in the current page
		if (menu_ctrl_ptr->cur_item_index < menu_ctrl_ptr->first_item_index)
		{
			menu_ctrl_ptr->first_item_index = menu_ctrl_ptr->cur_item_index;
		}
		else if (menu_ctrl_ptr->cur_item_index >= (menu_ctrl_ptr->first_item_index + line_num_page))
		{
			menu_ctrl_ptr->first_item_index = (uint16)(menu_ctrl_ptr->cur_item_index - line_num_page + 1);
		}
	}

    return result;
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_StopRedrawTimer(
                                     CTRLMENU_OBJ_T *menu_ctrl_ptr
                                     )
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->redrew_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->redrew_timer_id);
        menu_ctrl_ptr->redrew_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_StartRedrawTimer(
                                      CTRLMENU_OBJ_T    *menu_ctrl_ptr
                                      )
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->redrew_timer_id)
    {
        CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

        menu_ctrl_ptr->redrew_timer_id = MMK_CreateWinTimer(
            base_ctrl_ptr->handle, 
            MMITHEME_MENU_REDRAW_TIME,
            FALSE );
    }
}
/*****************************************************************************/
//  Description : GetItemFontColor
//  Global resource dependence:
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T CTRLMENU_GetItemFontColor(
                                       CTRLMENU_OBJ_T*      menu_ctrl_ptr,  // [in]
                                       uint16               item_index      // [in]
                                       )
{
    CTRLMENU_ITEM_INFO_T    *item_info_ptr  = PNULL;
    CTRLMENU_GROUP_INFO_T   *group_info_ptr = PNULL;
    MMI_MENU_GROUP_ID_T      group_id = 0;
    GUI_COLOR_T              color = CTRLMENU_GetFontColor(menu_ctrl_ptr);
    if (PNULL != menu_ctrl_ptr)
    {
        group_id = CTRLMENU_GetCurGroupId(menu_ctrl_ptr);

        group_info_ptr = CTRLMENU_GetGroupInfo(menu_ctrl_ptr, group_id);

        if (PNULL != group_info_ptr)
        {
            item_info_ptr = GET_ITEM_INFO(group_info_ptr);
             if(item_info_ptr->is_set_color == TRUE)
             {
                 color = item_info_ptr[item_index].item_font_color;
             }
             else
             {
                 color = CTRLMENU_GetFontColor(menu_ctrl_ptr);
             }
        }
    }
    return color;
}

