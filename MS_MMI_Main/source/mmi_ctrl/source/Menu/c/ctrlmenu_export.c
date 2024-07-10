/*****************************************************************************
** File Name:      ctrlmenu_export.c                                         *
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
#include "mmi_gui_trc.h"
#include "mmi_string.h"

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
 *                          LOCAL FUNCTION DECLARE                           *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : replace string
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReplaceString(
                         MMI_STRING_T   *dst_str_ptr,
                         MMI_STRING_T   *src_str_ptr
                         );

/*****************************************************************************/
//  Description : find node pointer by node id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_NODE_T* FindNodeById(
                                    uint32          node_id,        //node id
                                    CTRLMENU_NODE_T *root_node_ptr  //root node pointer
                                    );

/*****************************************************************************/
//  Description : insert new node in dynamic menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertNewNode(
                         uint16             new_node_index, //new node index,from o
                         CTRLMENU_NODE_T    *new_node_ptr,  //new node pointer
                         CTRLMENU_NODE_T    *parent_node_ptr//parent node pointer
                         );

/*****************************************************************************/
//  Description : 通过node id获取index
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetIndexByNodeId(
                               MMI_CTRL_ID_T   ctrl_id,         // [in]
                               uint32          parent_node_id,  // [in]
                               uint32          node_id,         // [in]
                               uint16          *index_ptr       // [out]
                               );

/*****************************************************************************/
//  Description : get item for static menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_STATIC_ITEM_T *GetStaticItem(
                                            CTRLMENU_OBJ_T       *menu_ctrl_ptr,    // [in]
                                            MMI_MENU_GROUP_ID_T  group_id,          // [in]
                                            MMI_MENU_ID_T        menu_id            // [in]
                                            );

/*****************************************************************************/
//  Description : adjust top item index
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 AdjustTopItemIndex(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                uint16          top_item_index
                                );

/*****************************************************************************/
//  Description : get item num in current page
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetCurPageItemNum(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : selected item is exist for radio/check pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_POP_SEL_ITEM_T* IsExistSelItem(
                                              GUIMENU_POP_SEL_INFO_T        *selected_info_ptr, //in
                                              CTRLMENU_POP_SELECT_LIST_T    *selected_list_ptr  //in
                                              );

/*****************************************************************************/
//  Description : creat one new selected item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_POP_SEL_ITEM_T* CreatNewSelItem(
                                               GUIMENU_POP_SEL_INFO_T   *selected_info_ptr
                                               );

/*****************************************************************************/
//  Description : add one new selected item to list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddSelItemToList(
                            CTRLMENU_POP_SEL_ITEM_T     *sel_item_ptr,      //in
                            CTRLMENU_POP_SELECT_LIST_T  *selected_list_ptr  //in
                            );

/*****************************************************************************/
//  Description : delete one selected item from list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DeleteSelItem(
                         CTRLMENU_POP_SEL_ITEM_T    *sel_item_ptr,      //in
                         CTRLMENU_POP_SELECT_LIST_T *selected_list_ptr  //in
                         );

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : return menu group id and menu id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_GetId(
                           MMI_HANDLE_T         ctrl_handle,    //in:
                           MMI_MENU_GROUP_ID_T  *group_id_ptr,  //in/out:
                           MMI_MENU_ID_T        *menu_id_ptr    //in/out:
                           )
{
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    GUIMENU_ITEM_T  cur_item = {0};

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_handle);
    if ((PNULL != menu_ctrl_ptr) && 
        (menu_ctrl_ptr->is_static))
    {
        //get current item pointer
        if (CTRLMENU_GetCurItem(menu_ctrl_ptr, &cur_item, PNULL))
        {
            if (PNULL != menu_id_ptr)
            {
                *menu_id_ptr = cur_item.menu_id;
            }
        }

        //get cur group id
        if (PNULL != group_id_ptr)
        {
            *group_id_ptr = CTRLMENU_GetCurGroupId(menu_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set menu the first and selected item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetFirstAndSelItem(
                                        uint16          first_item_index,
                                        uint16          cur_item_index,
                                        MMI_HANDLE_T    ctrl_handle
                                        )
{
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_handle);
    if (PNULL != menu_ctrl_ptr)
    {
        //set first and selected item
        if (0 == menu_ctrl_ptr->menu_level)
        {
            menu_ctrl_ptr->cur_item_index   = cur_item_index;
            menu_ctrl_ptr->first_item_index = first_item_index;
        }
        else
        {
            menu_ctrl_ptr->menu_stack[menu_ctrl_ptr->menu_level].first_item_index = first_item_index;
            menu_ctrl_ptr->menu_stack[menu_ctrl_ptr->menu_level].cur_item_index   = cur_item_index;
        }

        CTRLMENU_AdjustFirstAndDisplay(menu_ctrl_ptr,FALSE);
    }
}

/*****************************************************************************/
//  Description :set menu title,for dynamic menu or volatile static menu title
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetTitle(
                              MMI_STRING_T      *title_text_ptr,//title text
                              MMI_CTRL_ID_T     ctrl_id         //control id
                              )
{
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    
    if (PNULL != title_text_ptr)
    {
        //get menu pointer by control id
        menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
        if (PNULL != menu_ctrl_ptr)
        {
            //set menu title
            menu_ctrl_ptr->title_len = (uint16)(MIN(CTRLMENU_TITLE_MAX_LEN,title_text_ptr->wstr_len));
            MMI_WSTRNCPY( menu_ctrl_ptr->title_wstr,CTRLMENU_TITLE_MAX_LEN,
                title_text_ptr->wstr_ptr,title_text_ptr->wstr_len,
                menu_ctrl_ptr->title_len);
        }
    }
}

/***************************************************************************//*!
//  Description :设置Title的扩展方法
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetTitleEx(
                                   MMI_CTRL_ID_T        ctrl_id,        // [in]
                                   GUIMENU_TITLE_INFO_T *title_info_ptr // [in]
                                   )
{
    BOOLEAN                 result = FALSE;
    CTRLMENU_OBJ_T          *menu_ctrl_ptr  = PNULL;
    CTRLMENU_NODE_T         *root_node_ptr  = PNULL;
    CTRLMENU_NODE_T         *node_ptr       = PNULL;
    CTRLMENU_GROUP_INFO_T   *group_info_ptr = PNULL;

    if (PNULL != title_info_ptr)
    {
        menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
        if (PNULL != menu_ctrl_ptr)
        {
            if (menu_ctrl_ptr->is_static && title_info_ptr->is_static)
            {
                group_info_ptr = CTRLMENU_GetGroupInfo(menu_ctrl_ptr, title_info_ptr->group_id);
                if (PNULL != group_info_ptr)
                {
                    ReplaceString(&group_info_ptr->title_str, title_info_ptr->title_ptr);
                    ReplaceString(&group_info_ptr->sub_title_str, title_info_ptr->sub_title_ptr);

                    if (0 != title_info_ptr->title_icon_id)
                    {
                        group_info_ptr->title_icon_id = title_info_ptr->title_icon_id;
                    }

                    result = TRUE;
                }
            }
            else if (!menu_ctrl_ptr->is_static && !title_info_ptr->is_static)
            {
                //get parent node pointer
                root_node_ptr = menu_ctrl_ptr->root_node_ptr;
                node_ptr = FindNodeById(title_info_ptr->node_id, root_node_ptr);

                if (PNULL != node_ptr)
                {
                    ReplaceString(&node_ptr->title_str, title_info_ptr->title_ptr);
                    ReplaceString(&node_ptr->sub_title_str, title_info_ptr->sub_title_ptr);

                    if (0 != title_info_ptr->title_icon_id)
                    {
                        node_ptr->title_icon_id = title_info_ptr->title_icon_id;
                    }

                    result = TRUE;
                }
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : replace string
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReplaceString(
                         MMI_STRING_T   *dst_str_ptr,
                         MMI_STRING_T   *src_str_ptr
                         )
{
    if (PNULL != dst_str_ptr && PNULL != src_str_ptr)
    {
        if (PNULL != dst_str_ptr->wstr_ptr)
        {
            SCI_FREE(dst_str_ptr->wstr_ptr);
            dst_str_ptr->wstr_len = 0;
        }

        if (PNULL != src_str_ptr->wstr_ptr && 0 != src_str_ptr->wstr_len)
        {
            dst_str_ptr->wstr_len = src_str_ptr->wstr_len;
            dst_str_ptr->wstr_ptr = SCI_ALLOC_APP((dst_str_ptr->wstr_len + 1) * sizeof(wchar));
            SCI_MEMSET(dst_str_ptr->wstr_ptr, 0, (dst_str_ptr->wstr_len + 1) * sizeof(wchar));

            MMI_WSTRNCPY(
                dst_str_ptr->wstr_ptr, dst_str_ptr->wstr_len,
                src_str_ptr->wstr_ptr, src_str_ptr->wstr_len,
                src_str_ptr->wstr_len);
        }
    }
}

/*****************************************************************************/
//  Description : find node pointer by node id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_NODE_T* FindNodeById(
                                    uint32          node_id,        //node id
                                    CTRLMENU_NODE_T *root_node_ptr  //root node pointer
                                    )
{
    CTRLMENU_NODE_T     *node_ptr = PNULL;

    if (PNULL != root_node_ptr)
    {
        //find child node
        node_ptr = FindNodeById(node_id,root_node_ptr->child_node_ptr);
        if (PNULL == node_ptr)
        {
            //find neighbour node
            node_ptr = FindNodeById(node_id,root_node_ptr->neighbour_node_ptr);
            if (PNULL == node_ptr)
            {
                //compare node id
                if (node_id == root_node_ptr->node_id)
                {
                    node_ptr = root_node_ptr;
                }
            }
        }
    }

    return (node_ptr);
}

/*****************************************************************************/
//  Description : creat dynamic menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_CreatDynamic(
                                  GUI_BOTH_RECT_T   *rect_ptr,  //may PNULL,pop-up no need rect
                                  MMI_WIN_ID_T      win_id,
                                  MMI_CTRL_ID_T     ctrl_id,
                                  GUIMENU_STYLE_E   menu_style  //menu style
                                  )
{
    CTRLMENU_CreatDynamicEx(GUIMENU_DYNA_NODE_MAX_NUM,rect_ptr,win_id,ctrl_id,menu_style);
}

/*****************************************************************************/
//  Description : creat dynamic menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_CreatDynamicEx(
                                    uint16          node_max_num,   //in:
                                    GUI_BOTH_RECT_T *rect_ptr,      //may PNULL,pop-up no need rect
                                    MMI_WIN_ID_T    win_id,
                                    MMI_CTRL_ID_T   ctrl_id,
                                    GUIMENU_STYLE_E menu_style      //menu style
                                    )
{
    BOOLEAN                 is_param_ok = FALSE;
    GUIMENU_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};

    //set init data
    init_data.is_static  = FALSE;
    init_data.node_max_num = node_max_num;
    init_data.menu_style = menu_style;
    if (PNULL != rect_ptr)
    {
        init_data.both_rect = *rect_ptr;
    }

    switch (menu_style)
    {
    case GUIMENU_STYLE_SECOND:
        if (PNULL != rect_ptr)
        {
            is_param_ok = TRUE;
        }
        break;

    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
    case GUIMENU_STYLE_OPTIONS:
        is_param_ok = TRUE;
        break;

    default:
        SCI_TRACE_LOW("CTRLMENU_CreatDynamicEx:menu style %d is error!",menu_style);
        break;
    }

    if (is_param_ok)
    {
        //set control param
        ctrl_create.ctrl_id           = ctrl_id;
        ctrl_create.guid              = SPRD_GUI_MENU_ID;
        ctrl_create.init_data_ptr     = &init_data;
        ctrl_create.parent_win_handle = win_id;

        //creat control
        MMK_CreateControl(&ctrl_create);
    }
}

/*****************************************************************************/
//  Description : return dynamic menu current node id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLMENU_GetCurNodeId(
                                    MMI_CTRL_ID_T   ctrl_id //control id
                                    )
{
    uint32          node_id = 0;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    CTRLMENU_NODE_T *cur_node_ptr = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //get current node pointer
        cur_node_ptr = CTRLMENU_GetNodeByVisibleIndex(menu_ctrl_ptr->cur_item_index,menu_ctrl_ptr->cur_parent_node_ptr);
        if (PNULL != cur_node_ptr)
        {
            node_id = cur_node_ptr->node_id;
        }
        else
        {
            //SCI_TRACE_LOW:"== GUIMENU_GetCurNodeId == error when GetNodeByVisibleIndex"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMENU_4028_112_2_18_3_20_25_245,(uint8*)"");
        }
    }
    
    return (node_id);
}

/*****************************************************************************/
//  Description : in dynamic menu, insert node
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_InsertNode(
                                uint16                  nodex_index,    //node index,from 0
                                uint32                  node_id,        //node id,from 1
                                uint32                  parent_node_id, //parent node id
                                GUIMENU_DYNA_ITEM_T     *node_item_ptr, //node item
                                MMI_CTRL_ID_T           ctrl_id         //control id
                                )
{
    CTRLMENU_OBJ_T      *menu_ctrl_ptr = PNULL;
    CTRLMENU_NODE_T     *root_node_ptr = PNULL;
    CTRLMENU_NODE_T     *parent_node_ptr = PNULL;
    CTRLMENU_NODE_T     *new_node_ptr = PNULL;
    
    if (PNULL == node_item_ptr)
    {
        return;
    }

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //get parent node pointer
        root_node_ptr = menu_ctrl_ptr->root_node_ptr;
        parent_node_ptr = FindNodeById(parent_node_id,root_node_ptr);

        if (PNULL != parent_node_ptr)
        {
            //creat new node
            new_node_ptr = CTRLMENU_CreatNewNode(&menu_ctrl_ptr->node_memory);
            if (PNULL == new_node_ptr)
            {
                return;
            }
        
            //set node values
            new_node_ptr->node_id = node_id;

            new_node_ptr->is_visible = TRUE;
            new_node_ptr->is_grayed = node_item_ptr->is_grayed;

            // init style is inherited from parent style and style of root
            // node is set by creating.
            new_node_ptr->sub_menu_style = parent_node_ptr->sub_menu_style;

            new_node_ptr->node_wstr_len = node_item_ptr->item_text_ptr->wstr_len;
            new_node_ptr->node_wstr_ptr = SCI_ALLOC_APP((new_node_ptr->node_wstr_len + 1) * sizeof(wchar));
            SCI_MEMSET(new_node_ptr->node_wstr_ptr, 0, (new_node_ptr->node_wstr_len + 1) * sizeof(wchar));

            MMI_WSTRNCPY( new_node_ptr->node_wstr_ptr, new_node_ptr->node_wstr_len,
                node_item_ptr->item_text_ptr->wstr_ptr, node_item_ptr->item_text_ptr->wstr_len,
                new_node_ptr->node_wstr_len);
			
			if ((DYNA_ITEM_ICON_ID == node_item_ptr->item_type) && (0 != node_item_ptr->select_icon_id))
			{
				new_node_ptr->select_icon_type = DYNA_ITEM_ICON_ID;
				new_node_ptr->select_icon_data.icon_id = node_item_ptr->select_icon_id;
			} 
			else if((DYNA_ITEM_ICON_BUFFER == node_item_ptr->item_type) && (PNULL != node_item_ptr->icon_buffer.data))
			{
				new_node_ptr->select_icon_type = DYNA_ITEM_ICON_BUFFER;
				new_node_ptr->select_icon_data.buffer.data = node_item_ptr->icon_buffer.data;
				new_node_ptr->select_icon_data.buffer.height = node_item_ptr->icon_buffer.height;
				new_node_ptr->select_icon_data.buffer.width = node_item_ptr->icon_buffer.width;
			}
			
            //insert node
            InsertNewNode(nodex_index,new_node_ptr,parent_node_ptr);
        }

        //set item count
        if (parent_node_id == menu_ctrl_ptr->cur_parent_node_ptr->node_id)
        {
            menu_ctrl_ptr->item_total_num = menu_ctrl_ptr->cur_parent_node_ptr->visible_child_node_num;
        }
    }
}

/*****************************************************************************/
//  Description : insert new node in dynamic menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertNewNode(
                         uint16             new_node_index, //new node index,from o
                         CTRLMENU_NODE_T    *new_node_ptr,  //new node pointer
                         CTRLMENU_NODE_T    *parent_node_ptr//parent node pointer
                         )
{
    uint16              i = 0;
    CTRLMENU_NODE_T     *previous_node_ptr = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != new_node_ptr);      /*assert verified*/
    //SCI_ASSERT(PNULL != parent_node_ptr);   /*assert verified*/
    if ((PNULL == new_node_ptr)||(PNULL == parent_node_ptr))
    {
        return;
    }

    //set parent node chile number
    parent_node_ptr->child_node_num++;
    parent_node_ptr->visible_child_node_num++;

    //set new node parent
    new_node_ptr->parent_node_ptr = parent_node_ptr;

    // add to header
    if (0 == new_node_index || PNULL == parent_node_ptr->child_node_ptr)
    {
        new_node_ptr->neighbour_node_ptr = parent_node_ptr->child_node_ptr;
        parent_node_ptr->child_node_ptr = new_node_ptr;
    }
    else
    {
        previous_node_ptr = parent_node_ptr->child_node_ptr;

        for (i = 1; i < new_node_index && PNULL != previous_node_ptr->neighbour_node_ptr; i++)
        {
            previous_node_ptr = previous_node_ptr->neighbour_node_ptr;
        }

        SCI_ASSERT(PNULL != previous_node_ptr); /*assert verified*/
        new_node_ptr->neighbour_node_ptr = previous_node_ptr->neighbour_node_ptr;
        previous_node_ptr->neighbour_node_ptr = new_node_ptr;
    }
}

/*****************************************************************************/
//  Description : in dynamic menu, replace node
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_ReplaceNode(
                                 uint16         nodex_index,    //node index,from 0
                                 uint32         node_id,        //node id,from 1
                                 uint32         parent_node_id, //parent node id
                                 MMI_STRING_T   *node_text_ptr, //node text
                                 MMI_CTRL_ID_T  ctrl_id         //control id
                                 )
{
    CTRLMENU_OBJ_T      *menu_ctrl_ptr = PNULL;
    CTRLMENU_NODE_T     *root_node_ptr = PNULL;
    CTRLMENU_NODE_T     *parent_node_ptr = PNULL;
    CTRLMENU_NODE_T     *cur_node_ptr = PNULL;

    if (PNULL == node_text_ptr)
    {
        return;
    }

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //get parent node pointer
        root_node_ptr = menu_ctrl_ptr->root_node_ptr;
        parent_node_ptr = FindNodeById(parent_node_id,root_node_ptr);

        if (PNULL != parent_node_ptr)
        {
            cur_node_ptr = CTRLMENU_FindNodeByIndex(nodex_index, parent_node_ptr);
            if (PNULL != cur_node_ptr)
            {
                //set node values
                cur_node_ptr->node_id = node_id;

                if (PNULL != cur_node_ptr->node_wstr_ptr)
                {
                    SCI_FREE(cur_node_ptr->node_wstr_ptr);
                }

                cur_node_ptr->node_wstr_len = node_text_ptr->wstr_len;
                cur_node_ptr->node_wstr_ptr = SCI_ALLOC_APP((cur_node_ptr->node_wstr_len + 1) * sizeof(wchar));
                SCI_MEMSET(cur_node_ptr->node_wstr_ptr, 0, (cur_node_ptr->node_wstr_len + 1) * sizeof(wchar));

                MMI_WSTRNCPY( cur_node_ptr->node_wstr_ptr,cur_node_ptr->node_wstr_len,
                    node_text_ptr->wstr_ptr,node_text_ptr->wstr_len,
                    cur_node_ptr->node_wstr_len);

                //set item count
                if (parent_node_id == menu_ctrl_ptr->cur_parent_node_ptr->node_id)
                {
                    menu_ctrl_ptr->item_total_num = menu_ctrl_ptr->cur_parent_node_ptr->child_node_num;
                }
            }
        }
    }
}

#if defined (MRAPP_SUPPORT)||defined (HERO_ENGINE_SUPPORT )  
/*****************************************************************************/
//  Description : in dynamic menu, replace node, 携带icon变更
//  Global resource dependence : 
//  Author: sky.qgp
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_ReplaceNodeEx(
                                   uint16               node_index,     //node index,from 0
                                   uint32               node_id,        //node id,from 1
                                   uint32               parent_node_id, //parent node id
                                   GUIMENU_DYNA_ITEM_T  *node_item_ptr, //node item
                                   MMI_CTRL_ID_T        ctrl_id         //control id
                                   )
{
    CTRLMENU_OBJ_T      *menu_ctrl_ptr = PNULL;
    CTRLMENU_NODE_T     *root_node_ptr = PNULL;
    CTRLMENU_NODE_T     *parent_node_ptr = PNULL;
    CTRLMENU_NODE_T     *cur_node_ptr = PNULL;

	if(PNULL == node_item_ptr)
	{
		return;
	}

    //get menu control pointer
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
	if(PNULL == menu_ctrl_ptr)
	{
		return;
	}
	
    //get parent node pointer
    root_node_ptr = menu_ctrl_ptr->root_node_ptr;
    parent_node_ptr = FindNodeById(parent_node_id,root_node_ptr);

    if (PNULL != parent_node_ptr)
    {
		cur_node_ptr = CTRLMENU_FindNodeByIndex(node_index, parent_node_ptr);
		if(PNULL == cur_node_ptr)
		{
			return;
		}
		if (PNULL != node_item_ptr->item_text_ptr)
		{		
			//set node values
			cur_node_ptr->node_id = node_id;
			cur_node_ptr->node_wstr_len = node_item_ptr->item_text_ptr->wstr_len;
			if(cur_node_ptr->node_wstr_ptr != NULL)
			{
				SCI_FREE(cur_node_ptr->node_wstr_ptr);
				cur_node_ptr->node_wstr_ptr = NULL;
			}
			cur_node_ptr->node_wstr_ptr = SCI_ALLOC_APP((cur_node_ptr->node_wstr_len + 1) * sizeof(wchar));
			SCI_MEMSET(cur_node_ptr->node_wstr_ptr, 0, (cur_node_ptr->node_wstr_len + 1) * sizeof(wchar));
			if(NULL == cur_node_ptr->node_wstr_ptr)
			{
				return;
			}

			MMI_WSTRNCPY( cur_node_ptr->node_wstr_ptr, cur_node_ptr->node_wstr_len,
				node_item_ptr->item_text_ptr->wstr_ptr, node_item_ptr->item_text_ptr->wstr_len, cur_node_ptr->node_wstr_len );
			if ( (DYNA_ITEM_ICON_ID == node_item_ptr->item_type) && (0 != node_item_ptr->select_icon_id))
			{
				cur_node_ptr->select_icon_type = DYNA_ITEM_ICON_ID;
				cur_node_ptr->select_icon_data.icon_id = node_item_ptr->select_icon_id;
			} 
			else if((DYNA_ITEM_ICON_BUFFER == node_item_ptr->item_type) && (PNULL != node_item_ptr->icon_buffer.data))
			{
				cur_node_ptr->select_icon_type = DYNA_ITEM_ICON_BUFFER;
				cur_node_ptr->select_icon_data.buffer.data = node_item_ptr->icon_buffer.data;
				cur_node_ptr->select_icon_data.buffer.height = node_item_ptr->icon_buffer.height;
				cur_node_ptr->select_icon_data.buffer.width = node_item_ptr->icon_buffer.width;
			}
		}
    }
}
#endif

/*****************************************************************************/
//  Description : in dynamic menu, replace node
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetNodeGrayed(
                                  BOOLEAN       is_grayed,      //set node grayed
                                  uint16        nodex_index,    //node index,from 0
                                  uint32        parent_node_id, //parent node id
                                  MMI_CTRL_ID_T ctrl_id         //control id
                                  )
{
    CTRLMENU_OBJ_T      *menu_ctrl_ptr = PNULL;
    CTRLMENU_NODE_T     *root_node_ptr = PNULL;
    CTRLMENU_NODE_T     *parent_node_ptr = PNULL;
    CTRLMENU_NODE_T     *cur_node_ptr = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //get parent node pointer
        root_node_ptr = menu_ctrl_ptr->root_node_ptr;
        parent_node_ptr = FindNodeById(parent_node_id,root_node_ptr);

        if (PNULL != parent_node_ptr)
        {
            cur_node_ptr = CTRLMENU_FindNodeByIndex(nodex_index,parent_node_ptr);
            if (PNULL != cur_node_ptr)
            {
                //set node is grayed
                cur_node_ptr->is_grayed = is_grayed;
            }
        }
    }
}

/*****************************************************************************/
//  Description : in dynamic menu, set node grayed
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetNodeGrayedByNodeId(
                                           BOOLEAN          is_grayed,      //set node grayed
                                           uint32           node_id,                // [in]
                                           uint32           parent_node_id, //parent node id
                                           MMI_CTRL_ID_T    ctrl_id         //control id
                                           )
{
    uint16 node_index = 0;

    if(GetIndexByNodeId(ctrl_id,parent_node_id,node_id,&node_index))
    {
        CTRLMENU_SetNodeGrayed(is_grayed,node_index,parent_node_id,ctrl_id);
    }
}

/*****************************************************************************/
//  Description : 通过node id获取index
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetIndexByNodeId(
                               MMI_CTRL_ID_T   ctrl_id,         // [in]
                               uint32          parent_node_id,  // [in]
                               uint32          node_id,         // [in]
                               uint16          *index_ptr       // [out]
                               )
{
    BOOLEAN             result = FALSE;
    CTRLMENU_OBJ_T      *menu_ctrl_ptr = PNULL;
    CTRLMENU_NODE_T     *root_node_ptr = PNULL;
    CTRLMENU_NODE_T     *parent_node_ptr = PNULL;
    CTRLMENU_NODE_T     *cur_node_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if ((PNULL != menu_ctrl_ptr) && 
        (!menu_ctrl_ptr->is_static) && 
        (PNULL != index_ptr))
    {
        //get parent node pointer
        root_node_ptr = menu_ctrl_ptr->root_node_ptr;
        parent_node_ptr = FindNodeById(parent_node_id, root_node_ptr);

        if (PNULL != parent_node_ptr)
        {
            *index_ptr = 0;

            cur_node_ptr = parent_node_ptr->child_node_ptr;
            while (PNULL != cur_node_ptr)
            {
                if (cur_node_ptr->node_id == node_id)
                {
                    result = TRUE;
                    break;
                }

                (*index_ptr)++;

                cur_node_ptr = cur_node_ptr->neighbour_node_ptr;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set item visible, this method is for dynamic menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetNodeVisible(
                                       MMI_CTRL_ID_T    ctrl_id,        // [in]
                                       uint16           nodex_index,    // [in] node index,from 0
                                       uint32           parent_node_id, // [in]
                                       BOOLEAN          is_visible      // [in]
                                       )
{
    BOOLEAN             result = FALSE;
    CTRLMENU_OBJ_T      *menu_ctrl_ptr  = PNULL;
    CTRLMENU_NODE_T     *root_node_ptr  = PNULL;
    CTRLMENU_NODE_T     *parent_node_ptr = PNULL;
    CTRLMENU_NODE_T     *cur_node_ptr   = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        // get parent node pointer
        root_node_ptr = menu_ctrl_ptr->root_node_ptr;
        parent_node_ptr = FindNodeById(parent_node_id, root_node_ptr);

        if (PNULL != parent_node_ptr)
        {
            // find specified node
            cur_node_ptr = CTRLMENU_FindNodeByIndex(nodex_index, parent_node_ptr);

            if (PNULL != cur_node_ptr)
            {
                if (cur_node_ptr->is_visible && !is_visible)        // while being invisible
                {
                    cur_node_ptr->is_visible = is_visible;
                    parent_node_ptr->visible_child_node_num--;

                    // when it is the current display menu, set item_total_num too
                    if (parent_node_ptr == menu_ctrl_ptr->cur_parent_node_ptr)
                    {
                        menu_ctrl_ptr->item_total_num--;
                    }
                }
                else if (!cur_node_ptr->is_visible && is_visible)   // while being visible
                {
                    cur_node_ptr->is_visible = is_visible;
                    parent_node_ptr->visible_child_node_num++;

                    if (parent_node_ptr == menu_ctrl_ptr->cur_parent_node_ptr)
                    {
                        menu_ctrl_ptr->item_total_num++;
                    }
                }

                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : in dynamic menu, set menu softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetDynamicMenuSoftkey(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           MMI_TEXT_ID_T    left_softkey_text_id,
                                           MMI_TEXT_ID_T    middle_softkey_text_id,
                                           MMI_TEXT_ID_T    right_softkey_text_id
                                           )
{
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    
    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        menu_ctrl_ptr->dyna_softkey_id[0] = left_softkey_text_id;
        menu_ctrl_ptr->dyna_softkey_id[1] = middle_softkey_text_id;
        menu_ctrl_ptr->dyna_softkey_id[2] = right_softkey_text_id;
    }
}

/*****************************************************************************/
//  Description : set menu background color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetBgColor(
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_COLOR_T		bg_color
                                )
{
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    
    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //set background color
        menu_ctrl_ptr->common_theme.bg_color = bg_color;
        menu_ctrl_ptr->common_theme.bg_image = 0;
    }
}

/*****************************************************************************/
//  Description : set menu font & height
//  Global resource dependence : 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSecMenuFontSize(
                                  MMI_CTRL_ID_T ctrl_id,
                                  GUI_FONT_T    font,
                                  GUI_FONT_T    expand_font
                                  )
{
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if ((PNULL != menu_ctrl_ptr) && 
        (TRUE == CTRLMENU_IsSecMenu(menu_ctrl_ptr)))
    {
        //set font size
        menu_ctrl_ptr->sec_theme.item_font = font;
        menu_ctrl_ptr->sec_theme.cur_item_font = expand_font;
    }
}

/*****************************************************************************/
//  Description : set item height
//  Global resource dependence : 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSecMenuItemHeight(
                                  MMI_CTRL_ID_T ctrl_id,
                                  uint16        item_height,
                                  uint16        expand_item_height
                                  )
{
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if ((PNULL != menu_ctrl_ptr) && 
        (TRUE == CTRLMENU_IsSecMenu(menu_ctrl_ptr)))
    {
        //set item height
        menu_ctrl_ptr->sec_theme.expand_item_height = expand_item_height;
        menu_ctrl_ptr->sec_theme.item_height= item_height;
    }
}
/*****************************************************************************/
//  Description : set menu font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetFontColor(
                                  MMI_CTRL_ID_T ctrl_id,
                                  GUI_COLOR_T   font_color
                                  )
{
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    
    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //set font
        menu_ctrl_ptr->is_set_font_color = TRUE;
        menu_ctrl_ptr->font_color        = font_color;
    }
}

/*****************************************************************************/
// 获取radio或check风格处于选中状态的菜单项
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLMENU_GetCheckInfo(
                                   MMI_CTRL_ID_T        ctrl_id,
                                   GUIMENU_CHECK_INFO_T *check_info_ptr,
                                   uint16               array_len
                                   )
{
    uint16                  check_count = 0;
    CTRLMENU_OBJ_T          *menu_ctrl_ptr = PNULL;
    CTRLMENU_POP_SEL_ITEM_T *item_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        item_ptr = menu_ctrl_ptr->selected_list.init_item_ptr;

        while (PNULL != item_ptr)
        {
            if (check_count < array_len && PNULL != check_info_ptr)
            {
                check_info_ptr[check_count].group_id = item_ptr->item_info.group_id;
                check_info_ptr[check_count].menu_id = item_ptr->item_info.menu_id;
                check_info_ptr[check_count].node_id = item_ptr->item_info.node_id;
            }

            check_count++;

            item_ptr = item_ptr->next_ptr;
        }
    }

    return check_count;
}

/*****************************************************************************/
//  Description : set menu rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetRect(
                             MMI_HANDLE_T   ctrl_handle,
                             GUI_RECT_T     *rect_ptr
                             )
{
    GUI_RECT_T      menu_rect = {0};
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_handle);
    if (PNULL != menu_ctrl_ptr)
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

        menu_rect = base_ctrl_ptr->rect;
        base_ctrl_ptr->rect = *rect_ptr;

        GUI_SetVOrHRect(base_ctrl_ptr->handle,rect_ptr,&(base_ctrl_ptr->both_rect));

        // 当区域不相等的时候，需要重新初始化
        if (!GUI_EqualRect(menu_rect,base_ctrl_ptr->rect))
        {
            CTRLMENU_Destroy(menu_ctrl_ptr);
            CTRLMENU_Init(menu_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set item grayed or not, this method is for static menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetItemGrayed(
                                      MMI_CTRL_ID_T         ctrl_id,    // [in]
                                      MMI_MENU_GROUP_ID_T   group_id,   // [in]
                                      MMI_MENU_ID_T         menu_id,    // [in]
                                      BOOLEAN               is_grayed   // [in]
                                      )
{
    int32                   i = 0;
    BOOLEAN                 result = FALSE;
    CTRLMENU_OBJ_T          *menu_ctrl_ptr  = PNULL;
    GUIMENU_GROUP_T         *group_ptr      = PNULL;
    CTRLMENU_ITEM_INFO_T    *item_info_ptr  = PNULL;
    CTRLMENU_GROUP_INFO_T   *group_info_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        group_info_ptr = CTRLMENU_GetGroupInfo(menu_ctrl_ptr, group_id);

        if (PNULL != group_info_ptr)
        {
            group_ptr = (GUIMENU_GROUP_T*)MMITHEME_GetMenuGroup(group_id);
            SCI_ASSERT(PNULL != group_ptr); /*assert verified*/

            item_info_ptr = GET_ITEM_INFO(group_info_ptr);

            for (i = 0; i < group_ptr->item_count; i++)
            {
                if (group_ptr->item_ptr[i].menu_id == menu_id)
                {
                    item_info_ptr[i].is_grayed = is_grayed;

                    result = TRUE;

                    break;
                }
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set item visible, this method is for static menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetItemVisible(
                                       MMI_CTRL_ID_T        ctrl_id,    // [in]
                                       MMI_MENU_GROUP_ID_T  group_id,   // [in]
                                       MMI_MENU_ID_T        menu_id,    // [in]
                                       BOOLEAN              is_visible  // [in]
                                       )
{
    int32                   i = 0;
    BOOLEAN                 result = FALSE;
    CTRLMENU_OBJ_T          *menu_ctrl_ptr  = PNULL;
    GUIMENU_GROUP_T         *group_ptr      = PNULL;
    CTRLMENU_ITEM_INFO_T    *item_info_ptr  = PNULL;
    CTRLMENU_GROUP_INFO_T   *group_info_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        group_info_ptr = CTRLMENU_GetGroupInfo(menu_ctrl_ptr, group_id);

        if (PNULL != group_info_ptr)
        {
            group_ptr = (GUIMENU_GROUP_T*)MMITHEME_GetMenuGroup(group_id);
            SCI_ASSERT(PNULL != group_ptr); /*assert verified*/

            item_info_ptr = GET_ITEM_INFO(group_info_ptr);

            for (i = 0; i < group_ptr->item_count; i++)
            {
                if (group_ptr->item_ptr[i].menu_id == menu_id)
                {
                    if (item_info_ptr[i].is_visible && !is_visible)        // while being invisible
                    {
                        item_info_ptr[i].is_visible = is_visible;
                        group_info_ptr->visible_child_item_num--;

                        // when it is the current display menu, set item_total_num too
                        if (group_id == menu_ctrl_ptr->cur_group_id)
                        {
                            menu_ctrl_ptr->item_total_num--;
                        }
                    }
                    else if (!item_info_ptr[i].is_visible && is_visible)   // while being visible
                    {
                        item_info_ptr[i].is_visible = is_visible;
                        group_info_ptr->visible_child_item_num++;

                        if (group_id == menu_ctrl_ptr->cur_group_id)
                        {
                            menu_ctrl_ptr->item_total_num++;
                        }
                    }
                    
					//adjust menu_ctrl_ptr->cur_item_index
					if( (0 < group_info_ptr->visible_child_item_num) && (menu_ctrl_ptr->cur_item_index >= group_info_ptr->visible_child_item_num))
					{
						menu_ctrl_ptr->cur_item_index = (uint16)(group_info_ptr->visible_child_item_num - 1);
					}
                    result = TRUE;

                    break;
                }
            }
        }
    }

    return result;
}
/***************************************************************************//*!
@brief 设置某个菜单项文本的颜色
@author longwei.qiao
@param ctrl_id [in] 控件ID
@param group_id [in]
@param item_index [in] 菜单项的索引值
@param font_color [in] 需要显示的颜色
@return 是否设置成功
@note 该方法仅在静态二级菜单风格下才起作用
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetItemFontColor(
                                       MMI_CTRL_ID_T        ctrl_id,    // [in]
                                       MMI_MENU_GROUP_ID_T  group_id,   // [in]
                                       uint16               item_index, // [in]
                                       GUI_COLOR_T          font_color  // [in]
                                       )
{
    BOOLEAN                 result = FALSE;
    CTRLMENU_OBJ_T          *menu_ctrl_ptr  = PNULL;
    GUIMENU_GROUP_T         *group_ptr      = PNULL;
    CTRLMENU_ITEM_INFO_T    *item_info_ptr  = PNULL;
    CTRLMENU_GROUP_INFO_T   *group_info_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        group_info_ptr = CTRLMENU_GetGroupInfo(menu_ctrl_ptr, group_id);

        if (PNULL != group_info_ptr)
        {
            group_ptr = (GUIMENU_GROUP_T*)MMITHEME_GetMenuGroup(group_id);
            SCI_ASSERT(PNULL != group_ptr); /*assert verified*/
            if(item_index >= group_ptr->item_count)
            {
                SCI_TRACE_LOW("CTRLMENU_SetItemFontColor item_index is invalid");
                return FALSE;
            }
            item_info_ptr = GET_ITEM_INFO(group_info_ptr);
            item_info_ptr->is_set_color = TRUE;
            item_info_ptr[item_index].item_font_color = font_color;
            result = TRUE;
        }
    }
    SCI_TRACE_LOW("CTRLMENU_SetItemFontColor result is %d", result);
    return result;
}

/*****************************************************************************/
//  Description : set item text and image
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetStaticItem(
                                      MMI_CTRL_ID_T         ctrl_id,    // [in]
                                      MMI_MENU_GROUP_ID_T   group_id,   // [in]
                                      MMI_MENU_ID_T         menu_id,    // [in]
                                      MMI_STRING_T          *text_ptr,  // [in] PNULL表示不设置文本
                                      MMI_IMAGE_ID_T        icon_id     // [in] 0或者IMAGE_NULL表示不设置图片
                                      )
{
    BOOLEAN                 result = FALSE;
    BOOLEAN                 is_item_found = FALSE;
    CTRLMENU_OBJ_T          *menu_ctrl_ptr  = PNULL;
    CTRLMENU_STATIC_ITEM_T  *cur_static_item_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        cur_static_item_ptr = GetStaticItem(menu_ctrl_ptr, group_id, menu_id);

        if (PNULL == cur_static_item_ptr)
        {
            cur_static_item_ptr = SCI_ALLOC_APP(sizeof(CTRLMENU_STATIC_ITEM_T));
            SCI_MEMSET(cur_static_item_ptr, 0, sizeof(CTRLMENU_STATIC_ITEM_T));

            cur_static_item_ptr->group_id = group_id;
            cur_static_item_ptr->menu_id  = menu_id;
        }
        else
        {
            is_item_found = TRUE;
        }

        if (PNULL != text_ptr && text_ptr->wstr_len > 0)
        {
            if (PNULL != cur_static_item_ptr->text_str.wstr_ptr)
            {
                SCI_FREE(cur_static_item_ptr->text_str.wstr_ptr);
            }

            cur_static_item_ptr->text_str.wstr_len = text_ptr->wstr_len;
            cur_static_item_ptr->text_str.wstr_ptr = SCI_ALLOC_APP((text_ptr->wstr_len + 1) * sizeof(wchar));
            SCI_MEMSET(cur_static_item_ptr->text_str.wstr_ptr, 0, (text_ptr->wstr_len + 1) * sizeof(wchar));

            MMI_WSTRNCPY(
                    cur_static_item_ptr->text_str.wstr_ptr, cur_static_item_ptr->text_str.wstr_len,
                    text_ptr->wstr_ptr, text_ptr->wstr_len,
                    text_ptr->wstr_len);
        }

        if (MMITHEME_CheckImageID(icon_id))
        {
            cur_static_item_ptr->icon_id = icon_id;
        }

        if (!is_item_found)
        {
            if (PNULL != menu_ctrl_ptr->static_item_ptr)
            {
                cur_static_item_ptr->next_item_ptr = menu_ctrl_ptr->static_item_ptr;
            }

            menu_ctrl_ptr->static_item_ptr = cur_static_item_ptr;
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get item for static menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_STATIC_ITEM_T *GetStaticItem(
                                            CTRLMENU_OBJ_T       *menu_ctrl_ptr,    // [in]
                                            MMI_MENU_GROUP_ID_T  group_id,          // [in]
                                            MMI_MENU_ID_T        menu_id            // [in]
                                            )
{
    CTRLMENU_STATIC_ITEM_T  *cur_static_item_ptr = menu_ctrl_ptr->static_item_ptr;

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
//  Description : set the max width for auto popup menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note: only effective at auto popup menu
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetMaxWidth(
                                    MMI_CTRL_ID_T   ctrl_id,        // [in]
                                    uint16          max_horz_width, // [in]
                                    uint16          max_vert_width  // [in]
                                    )
{
    BOOLEAN         result = FALSE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        menu_ctrl_ptr->max_horz_width = max_horz_width;
        menu_ctrl_ptr->max_vert_width = max_vert_width;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set effective point in menu rect
//  Global resource dependence : 
//  Author: hua.fang
//  Note: only effective at auto popup menu
//        effective_point will indicate which point in menu rect will be effective
//        eg. if effective_point is GUIMENU_EP_BOTTOM_RIGHT, then right and bottom
//          in menu rect will be the start display point. the left and top will
//          be minus width and height automatically.
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetEffectivePoint(
                                          MMI_CTRL_ID_T     ctrl_id,          // [in]
                                          GUIMENU_EP_E      effective_point   // [in]
                                          )
{
    BOOLEAN         result = FALSE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        menu_ctrl_ptr->effective_point = effective_point;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetState( 
                                 MMI_CTRL_ID_T  ctrl_id,
                                 uint32         src_state
                                 )
{
    BOOLEAN         result = FALSE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        result = CTRLMENU_GetStatePtr(menu_ctrl_ptr,src_state);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetState( 
                                 MMI_CTRL_ID_T  ctrl_id,
                                 uint32         src_state,
                                 BOOLEAN        is_true
                                 )
{
    BOOLEAN         result = FALSE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        CTRLMENU_SetStatePtr(menu_ctrl_ptr,src_state,is_true);
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set current selected item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 可以自动调整top值
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSelectedItem(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     uint16         cur_item_index
                                     )
{
    int16           offset_y = 0;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //set first and selected item
        if (0 == menu_ctrl_ptr->menu_level)
        {
            menu_ctrl_ptr->cur_item_index = cur_item_index;

            //adjust menu first item
            CTRLMENU_AdjustFirstAndDisplay(menu_ctrl_ptr,FALSE);

            menu_ctrl_ptr->first_item_index = AdjustTopItemIndex(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index);
        }
        else
        {
            menu_ctrl_ptr->menu_stack[menu_ctrl_ptr->menu_level].cur_item_index = cur_item_index;

            //adjust menu first item
            CTRLMENU_AdjustFirstAndDisplay(menu_ctrl_ptr,FALSE);

            menu_ctrl_ptr->menu_stack[menu_ctrl_ptr->menu_level].first_item_index = AdjustTopItemIndex(menu_ctrl_ptr, menu_ctrl_ptr->first_item_index);
        }

        if (GUIMENU_STYLE_SECOND == menu_ctrl_ptr->cur_style)
        {
            CTRLMENU_SecResetDisplayItem(menu_ctrl_ptr,&offset_y,TRUE);
        }
    }
}

/*****************************************************************************/
//  Description : adjust top item index
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 AdjustTopItemIndex(
                                CTRLMENU_OBJ_T  *menu_ctrl_ptr,
                                uint16          top_item_index
                                )
{
    uint16 page_item_num = 0;
    uint16 top_index = 0;

    top_index = top_item_index;

    switch (menu_ctrl_ptr->cur_style)
    {
    case GUIMENU_STYLE_SECOND:
    case GUIMENU_STYLE_POPUP:
    case GUIMENU_STYLE_POPUP_RADIO:
    case GUIMENU_STYLE_POPUP_CHECK:
    case GUIMENU_STYLE_POPUP_AUTO:
    case GUIMENU_STYLE_POPUP_RECT:
        page_item_num = GetCurPageItemNum(menu_ctrl_ptr);

        if (top_item_index + (page_item_num - 1) < menu_ctrl_ptr->cur_item_index)
        {
            top_index = (uint16)(menu_ctrl_ptr->cur_item_index - (page_item_num - 1));
        }
        break;

    case GUIMENU_STYLE_OPTIONS:
        break;

    default:
        break;
    }

    return top_index;
}

/*****************************************************************************/
//  Description : get item num in current page
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetCurPageItemNum(
                               CTRLMENU_OBJ_T   *menu_ctrl_ptr
                               )
{
    uint16          i = 0;
    uint16          item_num = 0;
    GUI_RECT_T      item_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;

    if ((PNULL == menu_ctrl_ptr)||
        (PNULL == menu_ctrl_ptr->item_pos_info_ptr))
    {
        return item_num;
    }

    item_num = menu_ctrl_ptr->first_item_index;

    for (i = 0; i < menu_ctrl_ptr->item_total_num; i++)
    {
        item_rect = menu_ctrl_ptr->item_pos_info_ptr[i].item_rect;
        if (GUI_IntersectRect(&item_rect, item_rect, base_ctrl_ptr->rect))
        {
            item_num++;
        }
    }

    return item_num;
}

/*****************************************************************************/
//  Description : 设置button的显示风格
//  Global resource dependence : 
//  Author: hua.fang
//  Note:该方法只对GUIMENU_STYLE_POPUP_AUTO,GUIMENU_STYLE_POPUP_CHECK,GUIMENU_STYLE_POPUP_RADIO,GUIMENU_STYLE_POPUP_RECT有效
/*****************************************************************************/
PUBLIC void CTRLMENU_SetButtonStyle(
                                    MMI_CTRL_ID_T           ctrl_id,         // [in]
                                    GUIMENU_BUTTON_INFO_T   *button_info_ptr // [in]
                                    )
{
    CTRLMENU_OBJ_T          *menu_ctrl_ptr  = PNULL;
    CTRLMENU_NODE_T         *root_node_ptr  = PNULL;
    CTRLMENU_NODE_T         *node_ptr       = PNULL;
    CTRLMENU_GROUP_INFO_T   *group_info_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if ((PNULL != button_info_ptr) && 
        (PNULL != menu_ctrl_ptr))
    {
        if (menu_ctrl_ptr->is_static && button_info_ptr->is_static)
        {
            group_info_ptr = CTRLMENU_GetGroupInfo(menu_ctrl_ptr, button_info_ptr->group_id);
            if (PNULL != group_info_ptr)
            {
                group_info_ptr->button_style = button_info_ptr->button_style;

                if (menu_ctrl_ptr->cur_group_id == button_info_ptr->group_id)
                {
                    menu_ctrl_ptr->cur_button_style = button_info_ptr->button_style;
                }
            }
        }
        else if (!menu_ctrl_ptr->is_static && !button_info_ptr->is_static)
        {
            //get parent node pointer
            root_node_ptr = menu_ctrl_ptr->root_node_ptr;
            node_ptr = FindNodeById(button_info_ptr->node_id, root_node_ptr);

            if (PNULL != node_ptr)
            {
                node_ptr->button_style = button_info_ptr->button_style;

                if (menu_ctrl_ptr->cur_parent_node_ptr->node_id == button_info_ptr->node_id)
                {
                    menu_ctrl_ptr->cur_button_style = button_info_ptr->button_style;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : get node string
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetNodeStr(
                                   MMI_CTRL_ID_T    ctrl_id,        //in:
                                   uint16           node_index,     //in:
                                   MMI_STRING_T     *node_str_ptr   //in/out:
                                   )
{
    BOOLEAN             result = FALSE;
    CTRLMENU_OBJ_T      *menu_ctrl_ptr = PNULL;
    CTRLMENU_NODE_T     *node_ptr = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //find node
        node_ptr = CTRLMENU_FindNodeByIndex(node_index,menu_ctrl_ptr->cur_parent_node_ptr);
        if ((PNULL != node_ptr) &&
            (PNULL != node_str_ptr))
        {
            node_str_ptr->wstr_ptr = node_ptr->node_wstr_ptr;
            node_str_ptr->wstr_len = node_ptr->node_wstr_len;

            result = TRUE; 
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : is pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsPopMenuEx(
                                    MMI_HANDLE_T    ctrl_handle
                                    )
{
    BOOLEAN         result = FALSE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_handle);
    if (PNULL != menu_ctrl_ptr)
    {
        result = CTRLMENU_IsPopMenu(menu_ctrl_ptr);
    }

    return (result);
}


/***************************************************************************//*!
@brief 设置格数
@author hua.fang
@param ctrl_id [in] 控件ID
@param item_height [in] 二级菜单的杭高
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetPopItemHeight(
                                         MMI_CTRL_ID_T  ctrl_id,    // [in]
                                         uint16         item_height // [in]
                                         )
{
    BOOLEAN         result = FALSE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr  = PNULL;

    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if ((PNULL != menu_ctrl_ptr) && 
        (CTRLMENU_IsPopMenu(menu_ctrl_ptr)))
    {
        menu_ctrl_ptr->pop_theme.item_height = item_height;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set pop-up menu item status for radio or check pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetPopItemStatus(
                                      BOOLEAN                   is_selected,
                                      GUIMENU_POP_SEL_INFO_T    *selected_info_ptr
                                      )
{
    CTRLMENU_OBJ_T          *menu_ctrl_ptr = PNULL;
    CTRLMENU_POP_SEL_ITEM_T *sel_item_ptr = PNULL;

    if (PNULL == selected_info_ptr)
    {
        return;
    }

    //convert handle
    selected_info_ptr->ctrl_id = MMK_ConvertIdToHandle(selected_info_ptr->ctrl_id);
    
    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(selected_info_ptr->ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        //is exist
        sel_item_ptr = IsExistSelItem(selected_info_ptr,&menu_ctrl_ptr->selected_list);
    
        if (is_selected)
        {
            if (PNULL == sel_item_ptr)
            {
                //creat new selected item
                sel_item_ptr = CreatNewSelItem(selected_info_ptr);

                //add list
                AddSelItemToList(sel_item_ptr,&menu_ctrl_ptr->selected_list);
            }
            else
            {
                //SCI_TRACE_LOW:"GUIMENU_SetPopItemStatus:selected item has exist"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMENU_4074_112_2_18_3_20_25_246,(uint8*)"");
            }
        }
        else
        {
            if (PNULL == sel_item_ptr)
            {
                //SCI_TRACE_LOW:"GUIMENU_SetPopItemStatus:selected item not exist"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIMENU_4081_112_2_18_3_20_25_247,(uint8*)"");
            }
            else
            {
                //delete the selected item
                DeleteSelItem(sel_item_ptr,&menu_ctrl_ptr->selected_list);
            }
        }
    }
}

/*****************************************************************************/
//  Description : selected item is exist for radio/check pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_POP_SEL_ITEM_T* IsExistSelItem(
                                              GUIMENU_POP_SEL_INFO_T        *selected_info_ptr, //in
                                              CTRLMENU_POP_SELECT_LIST_T    *selected_list_ptr  //in
                                              )
{
    CTRLMENU_POP_SEL_ITEM_T *item_ptr = PNULL;

    if ((PNULL == selected_info_ptr) || 
        (PNULL == selected_list_ptr))
    {
        return PNULL;
    }

    item_ptr = selected_list_ptr->init_item_ptr;
    while (PNULL != item_ptr)
    {
        if ((item_ptr->item_info.is_static == selected_info_ptr->is_static) &&
            (item_ptr->item_info.node_id == selected_info_ptr->node_id) &&
            (item_ptr->item_info.ctrl_id == selected_info_ptr->ctrl_id) &&
            (item_ptr->item_info.menu_id == selected_info_ptr->menu_id) &&
            (item_ptr->item_info.group_id == selected_info_ptr->group_id))
        {
            break;
        }
        item_ptr = item_ptr->next_ptr;
    }

    return (item_ptr);
}

/*****************************************************************************/
//  Description : creat one new selected item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLMENU_POP_SEL_ITEM_T* CreatNewSelItem(
                                               GUIMENU_POP_SEL_INFO_T   *selected_info_ptr
                                               )
{
    CTRLMENU_POP_SEL_ITEM_T *sel_item_ptr = PNULL;

    if (PNULL == selected_info_ptr)
    {
        return PNULL;
    }

    //alloc selected item memory
    sel_item_ptr = SCI_ALLOC_APP(sizeof(CTRLMENU_POP_SEL_ITEM_T));
    SCI_MEMSET(sel_item_ptr,0,sizeof(CTRLMENU_POP_SEL_ITEM_T));

    //set selected item memory
    sel_item_ptr->item_info.is_static = selected_info_ptr->is_static;
    sel_item_ptr->item_info.node_id   = selected_info_ptr->node_id;
    sel_item_ptr->item_info.ctrl_id   = selected_info_ptr->ctrl_id;
    sel_item_ptr->item_info.menu_id   = selected_info_ptr->menu_id;
    sel_item_ptr->item_info.group_id  = selected_info_ptr->group_id;

    return (sel_item_ptr);
}

/*****************************************************************************/
//  Description : add one new selected item to list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddSelItemToList(
                            CTRLMENU_POP_SEL_ITEM_T     *sel_item_ptr,      //in
                            CTRLMENU_POP_SELECT_LIST_T  *selected_list_ptr  //in
                            )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != selected_list_ptr); /*assert verified*/
    if (PNULL == selected_list_ptr)
    {
        return;
    }

    if (PNULL != sel_item_ptr)
    {
        if (PNULL == selected_list_ptr->init_item_ptr)
        {
            selected_list_ptr->init_item_ptr = sel_item_ptr;
        }
        else
        {
            sel_item_ptr->prev_ptr = selected_list_ptr->last_item_ptr;
            selected_list_ptr->last_item_ptr->next_ptr = sel_item_ptr;
        }

        selected_list_ptr->last_item_ptr = sel_item_ptr;
    }
}

/*****************************************************************************/
//  Description : delete one selected item from list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DeleteSelItem(
                         CTRLMENU_POP_SEL_ITEM_T    *sel_item_ptr,      //in
                         CTRLMENU_POP_SELECT_LIST_T *selected_list_ptr  //in
                         )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != sel_item_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != selected_list_ptr); /*assert verified*/
    if ((PNULL == sel_item_ptr)||(PNULL == selected_list_ptr))
    {
        return;
    }

    if (PNULL == sel_item_ptr->prev_ptr)
    {
        if (PNULL == sel_item_ptr->next_ptr)
        {
            selected_list_ptr->init_item_ptr = PNULL;
            selected_list_ptr->last_item_ptr = PNULL;
        }
        else
        {
            sel_item_ptr->next_ptr->prev_ptr = PNULL;
            selected_list_ptr->init_item_ptr = sel_item_ptr->next_ptr;
        }
    }
    else
    {
        if (PNULL == sel_item_ptr->next_ptr)
        {
            sel_item_ptr->prev_ptr->next_ptr = PNULL;
            selected_list_ptr->last_item_ptr = sel_item_ptr->prev_ptr;
        }
        else
        {
            sel_item_ptr->next_ptr->prev_ptr = sel_item_ptr->prev_ptr;
            sel_item_ptr->prev_ptr->next_ptr = sel_item_ptr->next_ptr;
        }
    }

    SCI_FREE(sel_item_ptr);
}

/*****************************************************************************/
//  Description : set style of sub menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetSubMenuStyle(
                                        MMI_CTRL_ID_T       ctrl_id,    // [in]
                                        uint32              node_id,    // [in]
                                        GUIMENU_STYLE_E     style       // [in]
                                        )
{
    BOOLEAN         result          = FALSE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr  = PNULL;
    CTRLMENU_NODE_T *root_node_ptr  = PNULL;
    CTRLMENU_NODE_T *node_ptr       = PNULL;

    // this method is available only in pop-up menu
    if ((GUIMENU_STYLE_POPUP == style) || 
        (GUIMENU_STYLE_POPUP_RADIO == style) ||
        (GUIMENU_STYLE_POPUP_CHECK == style))
    {
        //get menu pointer by control id
        menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
        if (PNULL != menu_ctrl_ptr)
        {
            //get parent node pointer
            root_node_ptr = menu_ctrl_ptr->root_node_ptr;
            node_ptr = FindNodeById(node_id, root_node_ptr);

            if ((PNULL != node_ptr) && 
                    ((GUIMENU_STYLE_POPUP == node_ptr->sub_menu_style) ||
#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
                    (GUIMENU_STYLE_POPUP_AUTO == node_ptr->sub_menu_style) ||
#endif
                    (GUIMENU_STYLE_POPUP_RADIO == node_ptr->sub_menu_style) ||
                    (GUIMENU_STYLE_POPUP_CHECK == node_ptr->sub_menu_style)))
            {
                node_ptr->sub_menu_style = style;
                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : is first display menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsFirstDisp(
                                    MMI_CTRL_ID_T   ctrl_id //in:
                                    )
{
    BOOLEAN         result = FALSE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr  = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        result = menu_ctrl_ptr->is_first_disp;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set is first display menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetFirstDisp(
                                     MMI_CTRL_ID_T  ctrl_id,    //in:
                                     BOOLEAN        is_first    //in:
                                     )
{
    BOOLEAN         result = FALSE;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr  = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        menu_ctrl_ptr->is_first_disp = is_first;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get menu level
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLMENU_GetLevel(
                                MMI_CTRL_ID_T   ctrl_id //in:
                                )
{
    uint16          menu_level = 0;
    CTRLMENU_OBJ_T  *menu_ctrl_ptr  = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        menu_level = menu_ctrl_ptr->menu_level;
    }

    return (menu_level);
}

/*****************************************************************************/
//  Description : get menu rect, display rect,not full screen
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLMENU_GetRect(
                                   MMI_CTRL_ID_T    ctrl_id //in:
                                   )
{
    GUI_RECT_T      menu_rect = {0};
    CTRLMENU_OBJ_T  *menu_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;

    //get menu pointer by control id
    menu_ctrl_ptr = CTRLMENU_GetPtr(ctrl_id);
    if (PNULL != menu_ctrl_ptr)
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T*)menu_ctrl_ptr;
        menu_rect = base_ctrl_ptr->rect;
    }

    return (menu_rect);
}
