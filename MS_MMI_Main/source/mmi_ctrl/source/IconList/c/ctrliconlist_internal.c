#ifdef GUIF_ICONLIST
/*****************************************************************************
** File Name:      guiiconlist_internal.c                                    *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "ctrliconlist.h"
#include "mmi_theme.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN IconListTypeOf(
                             CTRLBASE_OBJ_T     *ctrl_ptr
                             );

/*****************************************************************************/
//  Description : get text relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetTextRelativeRect(
                                         uint16             index,              //in:
                                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr  //in
                                         );
#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************/
// Description: create a group node ptr
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:
/*****************************************************************************/
LOCAL GUIICONLIST_GROUP_INFO_T *CreateIconListGroupNode(
                                                        GUIICONLIST_DELIMITER_INFO_T *group_ptr
                                                        );

/*****************************************************************************/
//  Description : ����group�ľ�������
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetGroupRelativeRect(
									  CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
									  uint16             group_index
									  );

/*****************************************************************************/
//  Description : ����icon�����group������
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetIconToGroupRect(
										CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
										uint16             item_index
										);

/*****************************************************************************/
//  Description : ����text�����group������
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetTextToGroupRect(
										CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
										uint16             item_index
										);

/*****************************************************************************/
// 	Description : ����group�����һ�����
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
LOCAL void DestroyIconListGroupNode(
									MMI_LINK_NODE_T const *node_ptr
									);
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get icon list pointer by control handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLICONLIST_OBJ_T* ICONLIST_GetPtr(
                                              MMI_HANDLE_T  ctrl_handle
                                              )
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT(IconListTypeOf(ctrl_ptr)); /*assert verified*/
    }
    
    return (CTRLICONLIST_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN IconListTypeOf(
							  CTRLBASE_OBJ_T     *ctrl_ptr
							  )
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_ICONLIST_TYPE);
}

/*****************************************************************************/
//  Description : get icon list rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetRect(
                                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                                      )
{
    BOOLEAN         is_border = TRUE;
    GUI_RECT_T      iconlist_rect = {0};
	CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
	if (PNULL == iconlist_ctrl_ptr)
	{
		return iconlist_rect;
	}

    //icon list is exist border
    if (GUI_BORDER_NONE == iconlist_ctrl_ptr->theme.border.type)
    {
        is_border = FALSE;
    }

    if ((is_border) &&
        (!GUI_IsRectEmpty(base_ctrl_ptr->rect)))
    {
        iconlist_rect.left   = (int16)(base_ctrl_ptr->rect.left + iconlist_ctrl_ptr->theme.border.width);
        iconlist_rect.right  = (int16)(base_ctrl_ptr->rect.right - iconlist_ctrl_ptr->theme.border.width);
        iconlist_rect.top    = (int16)(base_ctrl_ptr->rect.top + iconlist_ctrl_ptr->theme.border.width);
        iconlist_rect.bottom = (int16)(base_ctrl_ptr->rect.bottom - iconlist_ctrl_ptr->theme.border.width);
    }
    else
    {
        iconlist_rect = base_ctrl_ptr->rect;
    }
    SCI_ASSERT(iconlist_rect.top <= iconlist_rect.bottom); /*assert verified*/

    return (iconlist_rect);
}

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetScrollBarRect(
                                               CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                                               )
{
    GUI_RECT_T      prg_rect = {0};
    GUI_RECT_T      iconlist_rect = {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return prg_rect;
	}

    //get icon list rect
    iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);

    //set prg rect
    prg_rect = iconlist_rect;
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        prg_rect.right = (int16)(prg_rect.left + iconlist_ctrl_ptr->theme.groove_width - 1);
    }
    else
    {
        prg_rect.left = (int16)(prg_rect.right - iconlist_ctrl_ptr->theme.groove_width + 1);
    }

    return (prg_rect);
}

/*****************************************************************************/
//  Description : get icon list content rect,except border,margin and scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetContRect(
                                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                                          )
{
    GUI_RECT_T      rect = {0};
    GUI_RECT_T      iconlist_rect = {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return rect;
	}

    //get icon list rect
    iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);

    //set content rect
    rect = iconlist_rect;
    rect.left  = (int16)(iconlist_rect.left + iconlist_ctrl_ptr->theme.hmargin_space);
    rect.right = (int16)(iconlist_rect.right - iconlist_ctrl_ptr->theme.hmargin_space);
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        rect.left = (int16)(rect.left + iconlist_ctrl_ptr->theme.groove_width);
    }
    else
    {
        rect.right = (int16)(rect.right - iconlist_ctrl_ptr->theme.groove_width);
    }

    return (rect);
}

/*****************************************************************************/
//  Description : get item width,include icon,font and space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetItemWidth(
                                       BOOLEAN              is_include_space,   //in:
                                       CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr  //in
                                       )
{
    uint16      item_width = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

    switch (iconlist_ctrl_ptr->style)
    {
    case GUIICONLIST_STYLE_ICON:
    case GUIICONLIST_STYLE_ICON_UIDT:
        if (is_include_space)
        {
            item_width = (uint16)(iconlist_ctrl_ptr->theme.icon_item.width + iconlist_ctrl_ptr->theme.icon_item.hspace);
        }
        else
        {
            item_width = iconlist_ctrl_ptr->theme.icon_item.width;
        }
        break;

    case GUIICONLIST_STYLE_FOLDER_UIDT:
        if (is_include_space)
        {
            item_width = (uint16)(iconlist_ctrl_ptr->theme.folder_item.width + iconlist_ctrl_ptr->theme.folder_item.hspace);
        }
        else
        {
            item_width = iconlist_ctrl_ptr->theme.folder_item.width;
        }
        break;

    default:
        //SCI_TRACE_LOW:"GUIICONLIST_GetItemWidth:icon list style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_INTERNAL_265_112_2_18_3_19_16_177,(uint8*)"d",iconlist_ctrl_ptr->style);
        break;
    }

    return (item_width);
}

/*****************************************************************************/
//  Description : get item height,include icon,font and space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetItemHeight(
                                        BOOLEAN             is_include_space,   //in:
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                                        )
{
    uint16      item_height = 0;
    uint16      font_height = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

    //get font height
    font_height = GUI_GetFontHeight(iconlist_ctrl_ptr->theme.font.font,UNICODE_HANZI);

    switch (iconlist_ctrl_ptr->style)
    {
    case GUIICONLIST_STYLE_ICON:
        if (is_include_space)
        {
            item_height = (uint16)(iconlist_ctrl_ptr->theme.icon_item.height + iconlist_ctrl_ptr->theme.icon_item.vspace);
        }
        else
        {
            item_height = iconlist_ctrl_ptr->theme.icon_item.height;
        }
        break;

    case GUIICONLIST_STYLE_ICON_UIDT:
        if (is_include_space)
        {
            item_height = (uint16)(iconlist_ctrl_ptr->theme.icon_item.height + font_height + iconlist_ctrl_ptr->theme.font_vspace + iconlist_ctrl_ptr->theme.icon_item.vspace);
        }
        else
        {
            item_height = (uint16)(iconlist_ctrl_ptr->theme.icon_item.height + font_height + iconlist_ctrl_ptr->theme.font_vspace);
        }
        break;

    case GUIICONLIST_STYLE_FOLDER_UIDT:
        if (is_include_space)
        {
            item_height = (uint16)(iconlist_ctrl_ptr->theme.folder_item.height + font_height + iconlist_ctrl_ptr->theme.font_vspace + iconlist_ctrl_ptr->theme.folder_item.vspace);
        }
        else
        {
            item_height = (uint16)(iconlist_ctrl_ptr->theme.folder_item.height + font_height + iconlist_ctrl_ptr->theme.font_vspace);
        }
        break;

    default:
        //SCI_TRACE_LOW:"ICONLIST_GetItemHeight:icon list style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_INTERNAL_323_112_2_18_3_19_17_178,(uint8*)"d",iconlist_ctrl_ptr->style);
        break;
    }

    return (item_height);
}

/*****************************************************************************/
//  Description : get item horizontal space width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetItemHspace(
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                                        )
{
    uint16      hor_space = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

    switch (iconlist_ctrl_ptr->style)
    {
    case GUIICONLIST_STYLE_ICON:
    case GUIICONLIST_STYLE_ICON_UIDT:
        hor_space = iconlist_ctrl_ptr->theme.icon_item.hspace;
        break;

    case GUIICONLIST_STYLE_FOLDER_UIDT:
        hor_space = iconlist_ctrl_ptr->theme.folder_item.hspace;
        break;

    default:
        //SCI_TRACE_LOW:"ICONLIST_GetItemHspace:icon list style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_INTERNAL_352_112_2_18_3_19_17_179,(uint8*)"d",iconlist_ctrl_ptr->style);
        break;
    }

    return (hor_space);
}

/*****************************************************************************/
//  Description : get item vertical space width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetItemVspace(
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                                        )
{
    uint16      ver_space = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

    switch (iconlist_ctrl_ptr->style)
    {
    case GUIICONLIST_STYLE_ICON:
    case GUIICONLIST_STYLE_ICON_UIDT:
        ver_space = iconlist_ctrl_ptr->theme.icon_item.vspace;
        break;

    case GUIICONLIST_STYLE_FOLDER_UIDT:
        ver_space = iconlist_ctrl_ptr->theme.folder_item.vspace;
        break;

    default:
        //SCI_TRACE_LOW:"ICONLIST_GetItemVspace:icon list style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_INTERNAL_381_112_2_18_3_19_17_180,(uint8*)"d",iconlist_ctrl_ptr->style);
        break;
    }

    return (ver_space);
}

/*****************************************************************************/
//  Description : get icon width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetIconWidth(
                                       CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr  //in
                                       )
{
    uint16      icon_width = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

    switch (iconlist_ctrl_ptr->style)
    {
    case GUIICONLIST_STYLE_ICON:
    case GUIICONLIST_STYLE_ICON_UIDT:
        icon_width = iconlist_ctrl_ptr->theme.icon_item.width;
        break;

    case GUIICONLIST_STYLE_FOLDER_UIDT:
        icon_width = iconlist_ctrl_ptr->theme.folder_item.width;
        break;

    default:
        //SCI_TRACE_LOW:"GUIICONLIST_GetIconWidth:icon list style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_INTERNAL_410_112_2_18_3_19_17_181,(uint8*)"d",iconlist_ctrl_ptr->style);
        break;
    }

    return (icon_width);
}

/*****************************************************************************/
//  Description : get icon height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetIconHeight(
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                                        )
{
    uint16      icon_height = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

    switch (iconlist_ctrl_ptr->style)
    {
    case GUIICONLIST_STYLE_ICON:
    case GUIICONLIST_STYLE_ICON_UIDT:
        icon_height = iconlist_ctrl_ptr->theme.icon_item.height;
        break;

    case GUIICONLIST_STYLE_FOLDER_UIDT:
        icon_height = iconlist_ctrl_ptr->theme.folder_item.height;
        break;

    default:
        //SCI_TRACE_LOW:"GUIICONLIST_GetIconHeight:icon list style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_INTERNAL_439_112_2_18_3_19_17_182,(uint8*)"d",iconlist_ctrl_ptr->style);
        break;
    }

    return (icon_height);
}

/*****************************************************************************/
//  Description : get icon rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetIconRect(
                                          uint16                index,              //in:
                                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                          )
{
    GUI_RECT_T               icon_rect                    = {0};
    GUI_RECT_T               iconlist_rect                = {0};
    GUI_BIG_RECT_T           relative_rect                = {0};
    GUI_BIG_RECT_T           icon_to_group_rect           = {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return icon_rect;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return icon_rect;
	}

	//get icon list rect
	iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);

#ifdef GUIF_ICONLIST_DELIMITER
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
	{
		//����icon�����group������
		icon_to_group_rect  = GetIconToGroupRect(iconlist_ctrl_ptr, index);

		icon_rect.left   = (int16)(icon_to_group_rect.left   + iconlist_rect.left);
		icon_rect.top    = (int16)(icon_to_group_rect.top    + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
		icon_rect.right  = (int16)(icon_to_group_rect.right  + iconlist_rect.left);
		icon_rect.bottom = (int16)(icon_to_group_rect.bottom + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
	}
	else
#endif
	{
		//get icon relative rect
		relative_rect = ICONLIST_GetIconRelativeRect(index,iconlist_ctrl_ptr);
		
		//���򻬶�
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			//convert relative rect to icon rect
			icon_rect.left   = (int16)(relative_rect.left   + iconlist_rect.left);
			icon_rect.top    = (int16)(relative_rect.top    + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
			icon_rect.right  = (int16)(relative_rect.right  + iconlist_rect.left);
			icon_rect.bottom = (int16)(relative_rect.bottom + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
		}
		//���򻬶�
		else
		{
			//convert relative rect to icon rect
			icon_rect.left   = (int16)(relative_rect.left   + iconlist_rect.left - iconlist_ctrl_ptr->relative_disp_rect.left);
			icon_rect.top    = (int16)(relative_rect.top    + iconlist_rect.top);
			icon_rect.right  = (int16)(relative_rect.right  + iconlist_rect.left - iconlist_ctrl_ptr->relative_disp_rect.left);
			icon_rect.bottom = (int16)(relative_rect.bottom + iconlist_rect.top);		
		}

	}
    return icon_rect;
}

/*****************************************************************************/
//  Description : get icon relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T ICONLIST_GetIconRelativeRect(
                                                      uint16                index,              //in:
                                                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                                      )
{
    uint16                  display_width = 0;
    uint16                  item_width = 0;
    uint16                  item_height = 0;
    uint16                  hor_space = 0;
    uint16                  all_item_width = 0;
    uint16                  left_offset = 0;
    uint16                  col_index = 0;
    uint16                  row_index = 0;
    GUI_RECT_T              content_rect = {0};
    GUI_BIG_RECT_T          relative_rect = {0};
    MMITHEME_ICONLIST_T     *theme_ptr = &(iconlist_ctrl_ptr->theme);
 
	if (PNULL == iconlist_ctrl_ptr)
	{
		return relative_rect;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return relative_rect;
	}
	
    //get icon list content rect
    content_rect = ICONLIST_GetContRect(iconlist_ctrl_ptr);

    //get display width
    display_width = (uint16)(content_rect.right - content_rect.left + 1);

    //get item width,height and space
    item_width  = ICONLIST_GetItemWidth(TRUE,iconlist_ctrl_ptr);
    item_height = ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr);

	hor_space   = ICONLIST_GetItemHspace(iconlist_ctrl_ptr);
	
	//get all icon width
	all_item_width = (uint16)(iconlist_ctrl_ptr->col_num_page * item_width);
	if (all_item_width > hor_space)
	{
		all_item_width = (uint16)(all_item_width - hor_space);
	}
		
	//���򻬶�
	if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{
		//get left offset
		left_offset = (uint16)((display_width - all_item_width)>>1);
		//column/row index,from 0
		col_index = (uint16)(index % iconlist_ctrl_ptr->col_num_page);
		row_index = (uint16)(index / iconlist_ctrl_ptr->col_num_page);
	}
	//���򻬶�
	else
	{
		//column/row index,from 0,��������
		col_index = (uint16)(index / iconlist_ctrl_ptr->row_num_page);
		row_index = (uint16)(index % iconlist_ctrl_ptr->row_num_page);
	}
	
	//get icon relative rect
	relative_rect.left   = theme_ptr->hmargin_space + left_offset + col_index*item_width;
	
	if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
	{
		relative_rect.left = relative_rect.left + theme_ptr->groove_width;
	}

    relative_rect.right  = relative_rect.left + ICONLIST_GetIconWidth(iconlist_ctrl_ptr) - 1;
    relative_rect.top    = theme_ptr->vmargin_space + row_index*item_height;
    relative_rect.bottom = relative_rect.top + ICONLIST_GetIconHeight(iconlist_ctrl_ptr) - 1;

    return relative_rect;
}

/*****************************************************************************/
//  Description : get text rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan.ma edit 2011.12
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetTextRect(
											  uint16                index,              //in:
											  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
											  )
{
    GUI_RECT_T               text_rect          = {0};
    GUI_RECT_T               iconlist_rect      = {0};
    GUI_BIG_RECT_T           relative_rect      = {0};
#ifdef GUIF_ICONLIST_DELIMITER
    GUI_BIG_RECT_T           text_to_group_rect = {0};
#endif

	if (PNULL == iconlist_ctrl_ptr)
	{
		return text_rect;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return text_rect;
	}

	//get icon list rect
	iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);

#ifdef GUIF_ICONLIST_DELIMITER
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
	{
		//����icon�����group������
		text_to_group_rect  = GetTextToGroupRect(iconlist_ctrl_ptr, index);

		text_rect.left   = (int16)(text_to_group_rect.left   + iconlist_rect.left);
		text_rect.top    = (int16)(text_to_group_rect.top    + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
		text_rect.right  = (int16)(text_to_group_rect.right  + iconlist_rect.left);
		text_rect.bottom = (int16)(text_to_group_rect.bottom + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
	}
	else
#endif
	{
		//get text relative rect
		relative_rect = GetTextRelativeRect(index,iconlist_ctrl_ptr);
	
		//���򻬶�
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			//convert relative rect to icon rect
			text_rect.left   = (int16)(relative_rect.left   + iconlist_rect.left);
			text_rect.top    = (int16)(relative_rect.top    + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
			text_rect.right  = (int16)(relative_rect.right  + iconlist_rect.left);
			text_rect.bottom = (int16)(relative_rect.bottom + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
		}
		//���򻬶�
		else
		{
			//convert relative rect to icon rect
			text_rect.left   = (int16)(relative_rect.left   + iconlist_rect.left - iconlist_ctrl_ptr->relative_disp_rect.left);
			text_rect.top    = (int16)(relative_rect.top    + iconlist_rect.top);
			text_rect.right  = (int16)(relative_rect.right  + iconlist_rect.left - iconlist_ctrl_ptr->relative_disp_rect.left);
			text_rect.bottom = (int16)(relative_rect.bottom + iconlist_rect.top);
		}
	}

    return text_rect;
}

/*****************************************************************************/
//  Description : get text relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetTextRelativeRect(
                                         uint16             index,              //in:
                                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr  //in
                                         )
{
    uint16              font_height = 0;
    GUI_BIG_RECT_T      icon_relative_rect = {0};
    GUI_BIG_RECT_T      text_relative_rect = {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return text_relative_rect;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return text_relative_rect;
	}

    //get font height
    font_height = GUI_GetFontHeight(iconlist_ctrl_ptr->theme.font.font,UNICODE_HANZI);

    //get icon relative rect
    icon_relative_rect = ICONLIST_GetIconRelativeRect(index,iconlist_ctrl_ptr);  

    //set text relative rect
    text_relative_rect        = icon_relative_rect;
    text_relative_rect.top    = icon_relative_rect.bottom + iconlist_ctrl_ptr->theme.font_vspace;
    text_relative_rect.bottom = text_relative_rect.top + font_height - 1;

    return (text_relative_rect);
}

/*****************************************************************************/
//  Description : get item rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan.ma edit 2011.12
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetItemRect(
                                          uint16                index,              //in:
                                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                          )
{
    GUI_RECT_T            item_rect          = {0};
    GUI_RECT_T            iconlist_rect      = {0};
    GUI_BIG_RECT_T        relative_rect      = {0};
#ifdef GUIF_ICONLIST_DELIMITER
    GUI_BIG_RECT_T        item_to_group_rect = {0};

#endif

	if (PNULL == iconlist_ctrl_ptr)
	{
		return item_rect;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return item_rect;
	}

	//get icon list rect
	iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);

#ifdef GUIF_ICONLIST_DELIMITER
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
	{
		//����icon�����group������
		item_to_group_rect  = ICONLIST_GetItemToGroupRect(iconlist_ctrl_ptr, index);

		item_rect.left   = (int16)(item_to_group_rect.left   + iconlist_rect.left);
		item_rect.top    = (int16)(item_to_group_rect.top    + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
		item_rect.right  = (int16)(item_to_group_rect.right  + iconlist_rect.left);
		item_rect.bottom = (int16)(item_to_group_rect.bottom + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
	}	
	else
#endif
	{
		//get icon relative rect
		relative_rect = ICONLIST_GetItemRelativeRect(index,iconlist_ctrl_ptr,TRUE);
		
		//���򻬶�
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			//convert relative rect to icon rect
			item_rect.left   = (int16)(relative_rect.left   + iconlist_rect.left);
			item_rect.top    = (int16)(relative_rect.top    + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
			item_rect.right  = (int16)(relative_rect.right  + iconlist_rect.left);
			item_rect.bottom = (int16)(relative_rect.bottom + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
		}
		//���򻬶�
		else
		{
			//convert relative rect to icon rect
			item_rect.left   = (int16)(relative_rect.left + iconlist_rect.left - iconlist_ctrl_ptr->relative_disp_rect.left);
			item_rect.top    = (int16)(relative_rect.top + iconlist_rect.top);
			item_rect.right  = (int16)(relative_rect.right + iconlist_rect.left - iconlist_ctrl_ptr->relative_disp_rect.left);
			item_rect.bottom = (int16)(relative_rect.bottom + iconlist_rect.top);
		}
	}
    return (item_rect);
}

/*****************************************************************************/
//  Description : get item relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T ICONLIST_GetItemRelativeRect(
                                                      uint16                index,              //in:
                                                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
													  BOOLEAN               is_include_space 
                                                      )
{
    uint16              font_height = 0;
	uint16              item_height = 0;
    GUI_BIG_RECT_T      icon_relative_rect = {0};
    GUI_BIG_RECT_T      item_relative_rect = {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return item_relative_rect;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return item_relative_rect;
	}

    //get font height
    font_height = GUI_GetFontHeight(iconlist_ctrl_ptr->theme.font.font,UNICODE_HANZI);

    //get icon relative rect
    icon_relative_rect = ICONLIST_GetIconRelativeRect(index,iconlist_ctrl_ptr);  

	//xiyuan edit ������������item�߶Ⱥ�item height�ĸ߶ȼ��㲻һ��
	switch (iconlist_ctrl_ptr->style)
    {
    case GUIICONLIST_STYLE_ICON:
        if (is_include_space)
        {
            item_height = iconlist_ctrl_ptr->theme.icon_item.vspace;
        }
        else
        {
            item_height = 0;
        }
        break;
		
    case GUIICONLIST_STYLE_ICON_UIDT:
        if (is_include_space)
        {
            item_height = font_height + iconlist_ctrl_ptr->theme.font_vspace + iconlist_ctrl_ptr->theme.icon_item.vspace;
        }
        else
        {
            item_height = font_height + iconlist_ctrl_ptr->theme.font_vspace;
        }
        break;
		
    case GUIICONLIST_STYLE_FOLDER_UIDT:
        if (is_include_space)
        {
            item_height = font_height + iconlist_ctrl_ptr->theme.font_vspace + iconlist_ctrl_ptr->theme.folder_item.vspace;
        }
        else
        {
            item_height = font_height + iconlist_ctrl_ptr->theme.font_vspace;
        }
        break;
		
    default:
        //SCI_TRACE_LOW:"ICONLIST_GetItemHeight:icon list style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_INTERNAL_835_112_2_18_3_19_18_183,(uint8*)"d",iconlist_ctrl_ptr->style);
        break;
    }

    //set text relative rect
    item_relative_rect        = icon_relative_rect;
    item_relative_rect.bottom = icon_relative_rect.bottom + item_height - 1;

    return (item_relative_rect);
}

/*****************************************************************************/
//  Description : get icon list relative height,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 ICONLIST_GetRelativeHeight(
                                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                            )
{
    uint32      relative_height = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}
    //get relative height
    relative_height = iconlist_ctrl_ptr->relative_rect.bottom - iconlist_ctrl_ptr->relative_rect.top + 1;

    return (relative_height);
}

/*****************************************************************************/
//  Description : set icon list display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
PUBLIC BOOLEAN ICONLIST_SetDispRect(
                                       int32                display_min_position,  //�����Ű�Ϊdis_left,�����Ű�Ϊdis_top
                                       CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                                       )
{
    BOOLEAN     result = FALSE;
    uint16      display_height = 0;
    uint16      display_width  = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return FALSE;
	}

	//���򻬶�
	if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{
		//adjust display top
		if (display_min_position != iconlist_ctrl_ptr->relative_disp_rect.top)
		{
			//get display height
			display_height = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);
			
			//set display top and bottom
			iconlist_ctrl_ptr->relative_disp_rect.top    = display_min_position;
			iconlist_ctrl_ptr->relative_disp_rect.bottom = display_min_position + display_height - 1;
			
			result = TRUE;
		}
	}
	//���򻬶�
	else
	{
		//adjust display left
		if (display_min_position != iconlist_ctrl_ptr->relative_disp_rect.left)
		{
			//get display width
			display_width = ICONLIST_GetDispWidth(iconlist_ctrl_ptr);
			
			//set display top and bottom
			iconlist_ctrl_ptr->relative_disp_rect.left    = display_min_position;
			iconlist_ctrl_ptr->relative_disp_rect.right   = display_min_position + display_width - 1;

			result = TRUE;
		}
	}

    return (result);
}

/*****************************************************************************/
//  Description : reset icon list display rect,only for slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
//          ������ʾ������[0,display_max_rect]֮��
/*****************************************************************************/
PUBLIC BOOLEAN ICONLIST_ResetDispRect(
                                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                                         )
{
    BOOLEAN     result                    = FALSE;
    int32       display_min_position      = 0; //dis_left or dis_top
    int32       display_max_position      = 0; //dis_max_left or dis_max_top

	if (PNULL == iconlist_ctrl_ptr)
	{
		return FALSE;
	}

	//���򻬶�
	if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{
		//init display top
		display_min_position = iconlist_ctrl_ptr->relative_disp_rect.top;
		
		//get display max top
		display_max_position = ICONLIST_GetDispMaxTop(iconlist_ctrl_ptr);
	}
	//���򻬶�
	else
	{
		//init display left
		display_min_position = iconlist_ctrl_ptr->relative_disp_rect.left;
		
		//get display max top
		display_max_position = ICONLIST_GetDispMaxLeft(iconlist_ctrl_ptr);
	}	
	
	if (0 > display_min_position)
	{
		display_min_position = 0;
	}
	else if (display_min_position > display_max_position)
	{
		display_min_position = display_max_position;
	}
	
	//set display rect
	result = ICONLIST_SetDispRect(display_min_position,iconlist_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : get icon list display rect height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetDispHeight(
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                        )
{
    uint16      display_height = 0;
    GUI_RECT_T  iconlist_rect = {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

    //get icon list rect
    iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);

    //get display height
    display_height = (uint16)(iconlist_rect.bottom - iconlist_rect.top + 1);

    return (display_height);
}

/*****************************************************************************/
//  Description : get icon list display rect max top
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC int32 ICONLIST_GetDispMaxTop(
                                       CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                                       )
{
    uint16      display_height = 0;
    int32       max_top = 0;
    uint32      iconlist_height = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

    //get display height
    display_height = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);

    //get icon list relative height
    iconlist_height = ICONLIST_GetRelativeHeight(iconlist_ctrl_ptr);
    
    //get max top
    if (iconlist_height > display_height)
    {
        max_top = iconlist_ctrl_ptr->relative_rect.bottom - display_height + 1;
    }

    return (max_top);
}

/*****************************************************************************/
//  Description : get icon list display rect max left
//  Global resource dependence : 
//  Author: xiyuan edit 2011.12
//  Note: 
/*****************************************************************************/
PUBLIC int32 ICONLIST_GetDispMaxLeft(
										CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
										)
{
    int32       max_left       = 0;
    uint16      display_width  = 0;
    uint32      iconlist_width = 0;

	if (PNULL != iconlist_ctrl_ptr)
	{		
		//get display height
		display_width = ICONLIST_GetDispWidth(iconlist_ctrl_ptr);
		
		//get icon list relative height
		iconlist_width = ICONLIST_GetRelativeWidth(iconlist_ctrl_ptr);
		
		//get max top
		if (iconlist_width > display_width)
		{
			max_left = iconlist_ctrl_ptr->relative_rect.right - display_width + 1;
		}
	}

    return max_left;
}

/*****************************************************************************/
//  Description : get icon list display rect width
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetDispWidth(
									   CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
									   )
{
    uint16      display_width  = 0;
    GUI_RECT_T  iconlist_rect  = {0};

	if (PNULL != iconlist_ctrl_ptr)
	{		
		//get icon list rect
		iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);
		
		//get display height
		display_width = (uint16)(iconlist_rect.right - iconlist_rect.left + 1);
	}	
    return display_width;
}

/*****************************************************************************/
//  Description : get icon list relative Width,except border
//  Global resource dependence : 
//  Author: xiyuan edit 2011.12
//  Note:
/*****************************************************************************/
PUBLIC uint32 ICONLIST_GetRelativeWidth(
										   CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
										   )
{
    uint32  relative_width = 0;
	
	if (PNULL != iconlist_ctrl_ptr)
	{
		//get relative height
		relative_width = iconlist_ctrl_ptr->relative_rect.right - iconlist_ctrl_ptr->relative_rect.left + 1;
	}
    return relative_width;
}

/*****************************************************************************/
// Description : ����֧�ֺ��򻬶������ڲ�ʹ��
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN ICONLIST_SetLayoutStyleInter(
											   CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
											   GUIICONLIST_LAYOUT_STYLE_E layout_style
											   )
{
	BOOLEAN result = FALSE;

	if (PNULL != iconlist_ctrl_ptr)
	{
		iconlist_ctrl_ptr->layout_style = layout_style;
		result = TRUE;
	}

	return result;
}

/*****************************************************************************/
// Description : �Ƿ�֧�ֺ��򻬶������ڲ�ʹ��
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:
/*****************************************************************************/
PUBLIC GUIICONLIST_LAYOUT_STYLE_E ICONLIST_GetLayoutStyleInter(
																  CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
																  )
{
	GUIICONLIST_LAYOUT_STYLE_E layout_style = GUIICONLIST_LAYOUT_V;

    if (PNULL != iconlist_ctrl_ptr)
    {
		switch(iconlist_ctrl_ptr->layout_style)
		{
		case GUIICONLIST_LAYOUT_V:
			layout_style = GUIICONLIST_LAYOUT_V;
			break;
		case GUIICONLIST_LAYOUT_H:
			layout_style = GUIICONLIST_LAYOUT_H;
			break;
		default:
			//SCI_TRACE_LOW:"GUIICONLIST_GetLayoutStyle:iconlist LayoutStyle %d is error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_INTERNAL_1116_112_2_18_3_19_18_184,(uint8*)"d", iconlist_ctrl_ptr->layout_style);
			break;
		}
    }

    return layout_style;
}

/*******************************************************************************************/
//  Description : ͨ��item���ڵ��С��еõ�item��index���޺��򻬶�ʹ��
//  Global resource dependence :
//  Author: xiyuan.ma 2011.12
//  Note:
/*********************************************************************************************/
PUBLIC uint16 ICONLIST_GetIndexByRowCol(
										   CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr,
										   uint16                 row,                     //�ڼ���
										   uint16                 col                      //�ڼ���
										   )
{
	uint16              item_index         = 0;
	uint16              pre_num            = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

	if (GUIICONLIST_LAYOUT_H == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{		
		//����item������֮ǰ (ҳ�� * ÿҳ����)
		pre_num = col * iconlist_ctrl_ptr->row_num_page;
				
		//j��i�е����item ����ҳ���е�index
		item_index = pre_num + row;
	}

	return item_index;
}

#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************/
// Description: create a group node ptr
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:
/*****************************************************************************/
LOCAL GUIICONLIST_GROUP_INFO_T *CreateIconListGroupNode(
                                                        GUIICONLIST_DELIMITER_INFO_T *group_ptr
                                                        )
{
    GUIICONLIST_GROUP_INFO_T *new_group_ptr = PNULL;
	wchar                    *tmp_str_ptr   = PNULL;

    if (PNULL != group_ptr)
    {
        new_group_ptr = SCI_ALLOCA(sizeof(GUIICONLIST_GROUP_INFO_T));

		if (PNULL == new_group_ptr)
		{
			return new_group_ptr;
		}
 
		new_group_ptr->start_item_index              = group_ptr->start_item_index;
		new_group_ptr->delimiter_str_info.wstr_len   = group_ptr->delimiter_str_info.wstr_len;

		//�����ַ���ָ�������
		tmp_str_ptr = SCI_ALLOCA(sizeof(wchar)*(group_ptr->delimiter_str_info.wstr_len + 1));

		if (PNULL != tmp_str_ptr)
		{
			//��ʼ��
			SCI_MEMSET(tmp_str_ptr, 0, (sizeof(wchar)*(group_ptr->delimiter_str_info.wstr_len + 1)));
			//����copy
			SCI_MEMCPY(tmp_str_ptr, group_ptr->delimiter_str_info.wstr_ptr, (sizeof(wchar)*group_ptr->delimiter_str_info.wstr_len));
		}

		new_group_ptr->delimiter_str_info.wstr_ptr   = tmp_str_ptr;
    }

    return new_group_ptr;
}

/*****************************************************************************/
// 	Description : ����group�����һ�����
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
PUBLIC void ICONLIST_DestroyIconListGroupNode(
												 MMI_LINK_NODE_T const *node_ptr
												 )
{
	DestroyIconListGroupNode(node_ptr);

	return;
}

/*****************************************************************************/
// 	Description : ����group�����һ�����
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
LOCAL void DestroyIconListGroupNode(
									MMI_LINK_NODE_T const *node_ptr
									)
{
	GUIICONLIST_GROUP_INFO_T *group_ptr = PNULL;

    if (node_ptr == PNULL)
    {
        return;
    }

    group_ptr = (GUIICONLIST_GROUP_INFO_T *)node_ptr->data;
    
    //���ַ���ָ�벻Ϊ��ʱ�����ͷ��ַ���ָ��
    if (PNULL != group_ptr)
    {
		if (PNULL != group_ptr->delimiter_str_info.wstr_ptr)
		{
			SCI_FREE(group_ptr->delimiter_str_info.wstr_ptr);
		}

        SCI_FREE(group_ptr);
    }

	return;
}

/*****************************************************************************/
// Description : ����һ��group���ָ��
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:    
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *ICONLIST_IconListAddGroup(
													 CTRLICONLIST_OBJ_T           *iconlist_ctrl_ptr,
													 GUIICONLIST_DELIMITER_INFO_T *group_ptr
													 )
{
	GUIICONLIST_GROUP_INFO_T *new_group_ptr      = PNULL;
	GUIICONLIST_GROUP_INFO_T *tmp_ptr            = PNULL;
	GUIICONLIST_GROUP_INFO_T *add_prev_data_ptr  = PNULL;
	GUIICONLIST_GROUP_INFO_T *add_next_data_ptr  = PNULL;
	GUIICONLIST_GROUP_INFO_T *tmp_node_data_ptr  = PNULL;
	MMI_LINK_NODE_T          *add_prev_ptr       = PNULL;
	MMI_LINK_NODE_T          *add_next_ptr       = PNULL;
	MMI_LINK_NODE_T          *tmp_next_ptr       = PNULL;
	//uint16                   i                   = 0;
    BOOLEAN                  is_find             = FALSE;

	if ((PNULL == group_ptr)||(PNULL == iconlist_ctrl_ptr))
	{
		return (MMI_LINK_NODE_T  *)new_group_ptr;
	}

    // ����һ��group�ڵ�,�����������ڴ�
    new_group_ptr = CreateIconListGroupNode(group_ptr);
	
    // �������
    if (PNULL != new_group_ptr)
    {
        if (PNULL == iconlist_ctrl_ptr->group_list_info.group_list_ptr)
        {
            iconlist_ctrl_ptr->group_list_info.group_list_ptr = MMILINK_CreateHead((uint32)new_group_ptr);
			((GUIICONLIST_GROUP_INFO_T *)iconlist_ctrl_ptr->group_list_info.group_list_ptr->data)->group_id = 0;
			iconlist_ctrl_ptr->group_list_info.group_num = 0;
			iconlist_ctrl_ptr->group_list_info.group_num++;
        }
        else
        {
			tmp_ptr = (GUIICONLIST_GROUP_INFO_T *)iconlist_ctrl_ptr->group_list_info.group_list_ptr->prev_ptr->data;
			
			if (PNULL != tmp_ptr)
			{
				//�����������ʼitem_index > ��ǰ���һ�������ʼitem_index, ��Ϊ˳�����
				if (group_ptr->start_item_index > tmp_ptr->start_item_index)
				{				
					MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_group_ptr);
					if (PNULL == tmp_node_ptr)
					{
					       return PNULL;
					}
					
					//group nodeΪѭ��˫������β������һ��node
					MMILINK_AddNodeBeforeBaseNode(iconlist_ctrl_ptr->group_list_info.group_list_ptr, tmp_node_ptr);
					new_group_ptr->group_id = iconlist_ctrl_ptr->group_list_info.group_num;
					iconlist_ctrl_ptr->group_list_info.group_num++;
				}
				//�����������ʼitem_index > ��ǰ���һ�������ʼitem_index, ��Ϊ��˳�����
				else if (group_ptr->start_item_index < tmp_ptr->start_item_index)
				{
					MMI_LINK_NODE_T  *tmp_node_ptr = MMILINK_CreateNode((uint32)new_group_ptr);
					if (PNULL == tmp_node_ptr)
					{
					       return PNULL;
					}
					
					tmp_node_data_ptr = (GUIICONLIST_GROUP_INFO_T *)tmp_node_ptr->data;
					if (PNULL == tmp_node_data_ptr)
					{
					       return PNULL;
					}
								   
					//get head node ptr of group list
					add_prev_ptr = iconlist_ctrl_ptr->group_list_info.group_list_ptr;
					
					if (PNULL != add_prev_ptr)
					{
						do 
						{
							add_prev_data_ptr = (GUIICONLIST_GROUP_INFO_T *)add_prev_ptr->data;
							add_next_ptr = add_prev_ptr->next_ptr;				
							if(PNULL == add_next_ptr)
							{
								return PNULL;
							}
							add_next_data_ptr = (GUIICONLIST_GROUP_INFO_T *)add_next_ptr->data;
							
							if ((PNULL == add_prev_data_ptr)||(PNULL == add_next_data_ptr))
							{
								return PNULL;
							}
							
							//if find the position's group
             						if ((tmp_node_data_ptr->start_item_index > add_prev_data_ptr->start_item_index)&&
								(tmp_node_data_ptr->start_item_index < add_next_data_ptr->start_item_index))
							{
								is_find = TRUE;
								//�²���ķָ���֮��ĸ�����group id+1
								do 
								{
									tmp_next_ptr = add_next_ptr;
									if (PNULL == tmp_next_ptr)
									{
                                                                     return PNULL;
									}
									((GUIICONLIST_GROUP_INFO_T *)tmp_next_ptr->data)->group_id++;
									tmp_next_ptr = tmp_next_ptr->next_ptr;									
								} 
								while(tmp_next_ptr == iconlist_ctrl_ptr->group_list_info.group_list_ptr);
								//�²���ķָ���֮ǰ�Ľ����·ָ�������group id��group num���м���
      								tmp_node_data_ptr->group_id = add_prev_data_ptr->group_id + 1;
								tmp_node_data_ptr->group_item_total_num = add_prev_data_ptr->group_item_total_num - tmp_node_data_ptr->start_item_index +1;
								add_prev_data_ptr->group_item_total_num = tmp_node_data_ptr->start_item_index - add_prev_data_ptr->start_item_index;
								
								//�²���ķָ�������������
								tmp_node_ptr->prev_ptr = add_prev_ptr;
								tmp_node_ptr->next_ptr = add_next_ptr;
								add_prev_ptr->next_ptr = tmp_node_ptr;
								add_next_ptr->prev_ptr = tmp_node_ptr;
								iconlist_ctrl_ptr->group_list_info.group_num++;						
								
								break;
							}
							else
							{
								add_prev_ptr = add_prev_ptr->next_ptr;
							}
						} 
						while(add_prev_ptr != iconlist_ctrl_ptr->group_list_info.group_list_ptr);
					}
		
					// �������û���ҵ������ʼ��PNULL
					if (!is_find)
					{
						add_prev_data_ptr = PNULL;
					}
				}//else if
			}				
		}
    }

    return (MMI_LINK_NODE_T *)new_group_ptr;
}

/*****************************************************************************/
// 	Description : ����groupID��group�����в���group��� 
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *ICONLIST_SearchGroupInListById(
														  CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr,
														  uint32 group_id
														  )
{
    BOOLEAN         is_find  = FALSE;
    MMI_LINK_NODE_T *tmp_ptr = PNULL;
    GUIICONLIST_GROUP_INFO_T  *tmp_data_ptr = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return tmp_ptr;
	}

    tmp_ptr = iconlist_ctrl_ptr->group_list_info.group_list_ptr;

    if (PNULL != tmp_ptr)
    {
        do 
        {
			tmp_data_ptr = (GUIICONLIST_GROUP_INFO_T*)tmp_ptr->data;
			if (PNULL == tmp_data_ptr)
			{
				return tmp_ptr;
			}
            // ����ID���ң��ҵ�֮�󷵻�ָ��
            if (tmp_data_ptr->group_id == group_id)
            {
                is_find = TRUE;
                break;
            }
            else
            {
                tmp_ptr = tmp_ptr->next_ptr;
            }
        } 
        while(tmp_ptr != iconlist_ctrl_ptr->group_list_info.group_list_ptr);
    }

    // �������û���ҵ������ʼ��PNULL
    if (!is_find)
    {
        tmp_ptr = PNULL;
    }

    return tmp_ptr;    
}

/*****************************************************************************/
// 	Description : ����item index��group�����в���group��� 
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:    
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *ICONLIST_SearchGroupInListByIndex(
															 CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr,
															 uint16 item_index
															 )
{
	BOOLEAN                   is_find            = FALSE;
    MMI_LINK_NODE_T           *tmp_ptr           = PNULL;
    GUIICONLIST_GROUP_INFO_T  *tmp_data_ptr      = PNULL;
    GUIICONLIST_GROUP_INFO_T  *tmp_next_data_ptr = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return tmp_ptr;
	}

	if (item_index >= iconlist_ctrl_ptr->total_num)
	{
		return tmp_ptr;
	}
	
    tmp_ptr           = iconlist_ctrl_ptr->group_list_info.group_list_ptr;
	if (!tmp_ptr)
	{
		return tmp_ptr;
	}
	tmp_data_ptr      = (GUIICONLIST_GROUP_INFO_T*)tmp_ptr->data;
	
	if (PNULL != tmp_data_ptr)
	{
		do 
		{
			tmp_next_data_ptr = (GUIICONLIST_GROUP_INFO_T*)tmp_ptr->next_ptr->data;
			if (PNULL == tmp_next_data_ptr)
			{
				return tmp_ptr;
			}
			// ����ID���ң��ҵ�֮�󷵻�ָ��
			if (PNULL != tmp_data_ptr 
				&& (tmp_data_ptr->start_item_index <= item_index)
				&&(tmp_next_data_ptr->start_item_index > item_index))
			{
				is_find = TRUE;
				break;
			}
			//���һ��group�����
			else if (PNULL != tmp_data_ptr 
				&& (tmp_data_ptr->start_item_index <= item_index)
				&& (tmp_ptr->next_ptr == iconlist_ctrl_ptr->group_list_info.group_list_ptr))
			{
				is_find = TRUE;
				break;
			}
			else
			{
				tmp_ptr = tmp_ptr->next_ptr;
			}
		} 
		while(tmp_ptr != iconlist_ctrl_ptr->group_list_info.group_list_ptr);
	}
	
	// �������û���ҵ������ʼ��PNULL
	if (!is_find)
	{
		tmp_ptr = PNULL;
	}
	
	return tmp_ptr; 	
}

/*****************************************************************************/
// Description : ��ȡgroup��item�ܸ���
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:    
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetGroupItemNum(
										  CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
										  MMI_LINK_NODE_T *group_info_ptr
										  )
{
	uint16                   group_item_num     = 0;
	MMI_LINK_NODE_T          *tmp_ptr           = PNULL;
	MMI_LINK_NODE_T          *tmp_next_ptr      = PNULL;
	GUIICONLIST_GROUP_INFO_T *tmp_data_ptr      = PNULL;
	GUIICONLIST_GROUP_INFO_T *tmp_next_data_ptr = PNULL;
	
	if ((PNULL == iconlist_ctrl_ptr)||(PNULL == group_info_ptr))
	{
		return 0;
	}
	
	//��ȡ��ǰgroup��data
    tmp_ptr = group_info_ptr;
	tmp_data_ptr = (GUIICONLIST_GROUP_INFO_T*)tmp_ptr->data;
	
	//��ȡ��ǰgroup����һ��group��data
	tmp_next_ptr = tmp_ptr->next_ptr;	
	if (PNULL == tmp_next_ptr)
	{
		return 0;
	}	
	tmp_next_data_ptr = (GUIICONLIST_GROUP_INFO_T*)tmp_next_ptr->data;
	
	//calculate number of group item
    if (PNULL != tmp_data_ptr)
    {
        if (tmp_data_ptr->group_id == (iconlist_ctrl_ptr->group_list_info.group_num - 1))
        {
            group_item_num = iconlist_ctrl_ptr->total_num - tmp_data_ptr->start_item_index;
        }
        else
        {
            if ((PNULL != tmp_next_data_ptr)&&
				(tmp_next_data_ptr->start_item_index > tmp_data_ptr->start_item_index))
            {
                group_item_num = tmp_next_data_ptr->start_item_index - tmp_data_ptr->start_item_index;
            }
        }
    }
    
	return group_item_num;
}

/*****************************************************************************/
//  Description : ����һ��group�Ĵ�С
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetGroupRelativeRect(
										  CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
										  uint16             group_index
										  )
{
	uint16              row_index                   = 0;	
	uint16              row_height                  = 0;
	uint16              item_total_num              = 0;
	GUI_BIG_RECT_T      group_relative_rect         = {0};
	GUI_RECT_T          delimiter_rect              = {0};
    MMI_LINK_NODE_T     *search_group_ptr           = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return group_relative_rect;
	}
	
	if (group_index >= iconlist_ctrl_ptr->group_list_info.group_num)
	{
		return group_relative_rect;
	}

    search_group_ptr = ICONLIST_SearchGroupInListById(iconlist_ctrl_ptr, group_index); 

	if (PNULL ==search_group_ptr)
    {
        return group_relative_rect;
    }
    
	item_total_num  = ICONLIST_GetGroupItemNum(iconlist_ctrl_ptr, search_group_ptr);
	((GUIICONLIST_GROUP_INFO_T*)search_group_ptr->data)->group_item_total_num = item_total_num;
	
	//set delimiter relative rect
	delimiter_rect.bottom = iconlist_ctrl_ptr->theme.delimiter_height + 1;
	delimiter_rect.right  = iconlist_ctrl_ptr->theme.delimiter_width + 1;
	
	group_relative_rect.top   = 0;
	group_relative_rect.left  = 0;
	group_relative_rect.right = delimiter_rect.right;
	
	//��������0��ʼ
	row_index = (uint16)(item_total_num / iconlist_ctrl_ptr->col_num_page);
	if (0 != (item_total_num % iconlist_ctrl_ptr->col_num_page))
	{
		row_index++;
	}
	//�и�
    row_height = ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr);
	//group���ܸ߶� = �ָ����߶� + item���ܸ߶�
	group_relative_rect.bottom = iconlist_ctrl_ptr->theme.delimiter_height + row_index * row_height;
	
	return group_relative_rect;//group�ľ�������	
}

/*****************************************************************************/
//  Description : ����ÿ��group��iconlist�е�rect,������
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_GetGroupRect(
									 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
									 )
{	
	GUI_BIG_RECT_T           group_rect              = {0};
	GUI_BIG_RECT_T           group_disp_rect         = {0};
	GUI_BIG_RECT_T           group_relative_rect     = {0};
    MMI_LINK_NODE_T          *search_group_ptr       = PNULL;
	GUIICONLIST_GROUP_INFO_T *tmp_ptr                = PNULL;
	GUIICONLIST_GROUP_INFO_T *tmp_pre_ptr            = PNULL;
	GUI_BIG_RECT_T           move_rect               = {0};
	GUI_BIG_RECT_T           iconlist_relative_rect  = {0};
	uint32                   group_index             = 0;
	
	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}
		
	//get head node ptr of group list
	search_group_ptr = iconlist_ctrl_ptr->group_list_info.group_list_ptr;
	
	do 
	{
		if(PNULL == search_group_ptr)
		{
		     return;
		}
	
		//�����ƶ�ƫ����������ƫ����
		if(search_group_ptr == iconlist_ctrl_ptr->group_list_info.group_list_ptr)
		{
			//�����ƶ�ʱ����iconlist��һ����Ե�����ƫ��
			move_rect                      = iconlist_relative_rect;
			move_rect.top                 -= iconlist_ctrl_ptr->relative_disp_rect.top;
			move_rect.bottom              -= iconlist_ctrl_ptr->relative_disp_rect.top;
		}
		else
		{
			//��ȡǰһ��group��λ��
			tmp_pre_ptr                = (GUIICONLIST_GROUP_INFO_T *)search_group_ptr->prev_ptr->data;
			if (PNULL != tmp_pre_ptr)
			{
				iconlist_relative_rect     = tmp_pre_ptr->group_relative_rect; 
				move_rect                  = tmp_pre_ptr->group_rect;            
			}
		}
		
		group_index                        = ((GUIICONLIST_GROUP_INFO_T *)search_group_ptr->data)->group_id;
		//��ȡgroup�ľ�������
		group_rect                         = GetGroupRelativeRect(iconlist_ctrl_ptr, group_index);
		
		//����group�ƶ�ʱ����iconlist�е�����
		group_disp_rect.top                =  group_rect.top    + move_rect.bottom;
		group_disp_rect.bottom             =  group_rect.bottom + move_rect.bottom;
		group_disp_rect.left               =  group_rect.left   + move_rect.left;
		group_disp_rect.right              =  group_rect.right  + move_rect.left;
		
		//����group��iconlist�е�����		
		group_relative_rect.top            =  group_rect.top    + iconlist_relative_rect.bottom;
		group_relative_rect.bottom         =  group_rect.bottom + iconlist_relative_rect.bottom;
		group_relative_rect.left           =  group_rect.left   + iconlist_relative_rect.left;
		group_relative_rect.right          =  group_rect.right  + iconlist_relative_rect.left;
		
		//����group�����iconlist������
		tmp_ptr                        = (GUIICONLIST_GROUP_INFO_T  *)search_group_ptr->data;
		if (PNULL != tmp_ptr)
		{
			tmp_ptr->group_rect            = group_disp_rect;
			tmp_ptr->group_relative_rect   = group_relative_rect;
		}
		
		search_group_ptr = search_group_ptr->next_ptr;
		
	} 
	while(search_group_ptr != iconlist_ctrl_ptr->group_list_info.group_list_ptr);
				
	return;
}

/*****************************************************************************/
//  Description : ����icon�����group������
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetIconToGroupRect(
										CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
										uint16             item_index
										)
{
	GUI_BIG_RECT_T      icon_relative_rect         = {0};
	GUI_BIG_RECT_T      icon_to_group_rect         = {0};
	MMI_LINK_NODE_T     *group_ptr                 = PNULL;
	GUI_BIG_RECT_T      tmp_rect                   = {0};
 
	if (PNULL == iconlist_ctrl_ptr)
	{
		return icon_to_group_rect;
	}

	if (item_index >= iconlist_ctrl_ptr->total_num)
	{
		return icon_to_group_rect;
	}
		
	//get group ptr
	group_ptr = ICONLIST_SearchGroupInListByIndex(iconlist_ctrl_ptr, item_index);

	if ((PNULL == group_ptr)||(PNULL == (GUIICONLIST_GROUP_INFO_T  *)group_ptr->data))
	{
       return icon_to_group_rect;
	}

	//get icon relative rect
	item_index        -= ((GUIICONLIST_GROUP_INFO_T  *)group_ptr->data)->start_item_index;
	icon_relative_rect = ICONLIST_GetIconRelativeRect(item_index, iconlist_ctrl_ptr);
    //get group relative rect
	tmp_rect           = ((GUIICONLIST_GROUP_INFO_T  *)group_ptr->data)->group_relative_rect;

    //calculate icon rect in the group
	icon_to_group_rect.top    = tmp_rect.top + iconlist_ctrl_ptr->theme.delimiter_height + icon_relative_rect.top;
	icon_to_group_rect.bottom = tmp_rect.top + iconlist_ctrl_ptr->theme.delimiter_height + icon_relative_rect.bottom;
	icon_to_group_rect.left   = icon_relative_rect.left;
	icon_to_group_rect.right  = icon_relative_rect.right;
	
    return icon_to_group_rect;
}

/*****************************************************************************/
//  Description : ����text�����group������
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetTextToGroupRect(
									CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
									uint16             item_index
									)
{
	uint8               font_height                = 0;
	GUI_BIG_RECT_T      icon_rect                  = {0};
	GUI_BIG_RECT_T      text_to_group_rect         = {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return text_to_group_rect;
	}

	if (item_index >= iconlist_ctrl_ptr->total_num)
	{
		return text_to_group_rect;
	}

	//get font height
    font_height = GUI_GetFontHeight(iconlist_ctrl_ptr->theme.font.font,UNICODE_HANZI);
	
	//set icon relative rect
	icon_rect = GetIconToGroupRect(iconlist_ctrl_ptr, item_index);

	//set text relative rect
    text_to_group_rect        = icon_rect;
    text_to_group_rect.top    = icon_rect.bottom + iconlist_ctrl_ptr->theme.font_vspace;
    text_to_group_rect.bottom = text_to_group_rect.top + font_height - 1;
	
    return text_to_group_rect;
}

/*****************************************************************************/
//  Description : ����item�����group������
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T ICONLIST_GetItemToGroupRect(
													 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
													 uint16             item_index
													 )
{
	GUI_BIG_RECT_T      item_relative_rect         = {0};
	GUI_BIG_RECT_T      item_to_group_rect         = {0};
	MMI_LINK_NODE_T     *group_ptr                 = PNULL;
	GUI_BIG_RECT_T      tmp_rect                   = {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return item_to_group_rect;
	}

	if (item_index >= iconlist_ctrl_ptr->total_num)
	{
		return item_to_group_rect;
	}

	//get group ptr
	group_ptr = ICONLIST_SearchGroupInListByIndex(iconlist_ctrl_ptr, item_index);
	
	if ((PNULL == group_ptr)||(PNULL == (GUIICONLIST_GROUP_INFO_T  *)group_ptr->data))
	{
       return item_to_group_rect;
	}

	//get item relative rect
	item_index        -= ((GUIICONLIST_GROUP_INFO_T  *)group_ptr->data)->start_item_index;
	item_relative_rect = ICONLIST_GetItemRelativeRect(item_index, iconlist_ctrl_ptr,TRUE);	
	//get group relative rect
	tmp_rect           = ((GUIICONLIST_GROUP_INFO_T  *)group_ptr->data)->group_relative_rect;
	
    //calculate icon rect in the group
	item_to_group_rect.top    = tmp_rect.top + iconlist_ctrl_ptr->theme.delimiter_height + item_relative_rect.top;
	item_to_group_rect.bottom = tmp_rect.top + iconlist_ctrl_ptr->theme.delimiter_height + item_relative_rect.bottom;
	item_to_group_rect.left   = item_relative_rect.left;
	item_to_group_rect.right  = item_relative_rect.right;
	
    return item_to_group_rect;
}
#endif

#endif //end of GUIF_ICONLIST
