/*****************************************************************************
** File Name:      mmi_mainmenu_pm.c                                         *
** Author:                                                                   *
** Date:           10/16/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      xiaoqing.lu             Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#include "mmi_mainmenu_data.h"
#include "mmi_mainmenu_synchronize.h"
#include "mmitheme_mainmenu.h"
#include "mmitheme_mainmenu.h"

#include "mmi_mainmenu_pm.h"
#include "guires.h"

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_DrawItemIcon(
                                        MMI_HANDLE_T win_handle,
                                        CTRLMAINMENU_ITEM_T const *item_ptr,
                                        GUI_LCD_DEV_INFO const *lcd_dev_info_ptr,
                                        GUI_RECT_T const *icon_rect_ptr,
                                        GUI_RECT_T const *disp_rect_ptr
                                        )
{
    GUI_RECT_T icon_rect = {0};
    
    if (PNULL == item_ptr || PNULL == lcd_dev_info_ptr || PNULL == icon_rect_ptr || PNULL == disp_rect_ptr)
    {
        return FALSE;
    }

    //0 calc rect
    icon_rect = *icon_rect_ptr;

    //1 draw icon
    switch(item_ptr->icon.type)
    {
    case CTRL_ICON_ID:
        GUIRES_DisplayImg(PNULL,
            disp_rect_ptr,
            &icon_rect,
            win_handle,
            item_ptr->icon.data.icon_id,
            lcd_dev_info_ptr);
        break;

    case CTRL_ICON_BUFFER:
        {
            GUI_POINT_T img_point = {0};
            GUIIMG_BITMAP_T img_map_info = {0};
            GUIIMG_DISPLAYBMP_PARAM_T display_param = {0};
            
            img_point.x = icon_rect.left;
            img_point.y = icon_rect.top;
            
            img_map_info.bit_ptr    = item_ptr->icon.data.buffer.data;
            img_map_info.img_width  = item_ptr->icon.data.buffer.width;
            img_map_info.img_height = item_ptr->icon.data.buffer.height;
            img_map_info.date_type  = GUIIMG_DATE_TYPE_ARGB888;
            
            display_param.is_horizontal = FALSE;
            display_param.rect_ptr = icon_rect_ptr;
            display_param.point_ptr = &img_point;
            display_param.bitmap_ptr = &img_map_info;
            display_param.lcd_dev_ptr = lcd_dev_info_ptr;
            display_param.is_transparent_mode = TRUE;
            display_param.transparent_color = 0x00;
            GUIIMG_DisplayBmpEx(&display_param);
        }
        break;

    default:
        break;
    }
    
    return TRUE;
}
