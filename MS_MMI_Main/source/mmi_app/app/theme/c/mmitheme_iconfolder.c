/*****************************************************************************
** File Name:      mmitheme_iconfolder.c                                     *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_ICONFOLDER_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmi_image.h"
#include "mmitheme_iconfolder.h"
#include "mmitheme_iconlist.h"
#include "mmidisplay_data.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMITHEME_ICONFOLDER_ICON_MAX_NUM    4

// #ifndef PIC_PREVIEW_U200_SUPPORT
// #define MMITHEME_ICONFOLDER_SHOW_WIDTH      MMITHEME_ICON_SHOW_WIDTH    //icon display width
// #define MMITHEME_ICONFOLDER_SHOW_HEIGHT     MMITHEME_ICON_SHOW_HEIGHT   //icon display height
// #else
#if defined MAINLCD_SIZE_320X480
#define MMITHEME_ICONFOLDER_SHOW_WIDTH      96    
#define MMITHEME_ICONFOLDER_SHOW_HEIGHT     70   
#else
#define MMITHEME_ICONFOLDER_SHOW_WIDTH      80    
#define MMITHEME_ICONFOLDER_SHOW_HEIGHT     58   
#endif
// #endif

#define MMITHEME_ICONFOLDER_OFFSET_MIN      0
#define MMITHEME_ICONFOLDER_OFFSET_MAX      0

#define MMITHEME_ICONFOLDER_MOVE_SPACE      10


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

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get icon folder theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetIconFolderTheme(
                                 MMITHEME_ICONFOLDER_T  *theme_ptr //in/out:
                                 )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    //set display icon max number in one folder
    theme_ptr->icon_max_num = MMITHEME_ICONFOLDER_ICON_MAX_NUM;
    
    //set icon width and height
    theme_ptr->icon_width  = MMITHEME_ICONFOLDER_SHOW_WIDTH;
    theme_ptr->icon_height = MMITHEME_ICONFOLDER_SHOW_HEIGHT;
    
    //set icon bg
    theme_ptr->icon_bg.bg_type = GUI_BG_NONE;
    theme_ptr->icon_bg.color   = MMI_GRAY_WHITE_COLOR;
    theme_ptr->icon_bg.shape   = GUI_SHAPE_RECT;
    
    //set icon border
    theme_ptr->icon_border.type  = GUI_BORDER_SOLID;
    theme_ptr->icon_border.width = 2;
    theme_ptr->icon_border.color = MMITHEME_UNSELECT_BORDER_COLOR;
    
    //set bg info
    theme_ptr->bg.bg_type = GUI_BG_IMG;
    theme_ptr->bg.img_id  = IMAGE_COMMON_BG;
    theme_ptr->bg.color   = MMI_GRAY_WHITE_COLOR;
    theme_ptr->bg.shape   = GUI_SHAPE_ROUNDED_RECT;
    
    //set border
    theme_ptr->border.type  = GUI_BORDER_NONE;
    theme_ptr->border.width = 1;
    theme_ptr->border.color = MMI_DARK_GRAY_COLOR;
    
    //set image id
    theme_ptr->mark_img_id = IMAGE_CHECK_SELECTED_ICON;
    
    theme_ptr->lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    theme_ptr->lcd_dev.block_id = GUI_BLOCK_MAIN;
}

/*****************************************************************************/
//  Description : get icon rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetIconRect(
                                       uint16       index,
                                       uint16       icon_width,
                                       uint16       icon_height,
                                       GUI_RECT_T   folder_rect
                                       )
{
    GUI_RECT_T      icon_rect = {0};
    GUI_POINT_T     folder_point = {0};
    GUI_POINT_T     icon_point = {0};

    //get folder center point
    folder_point.x = (int16)((folder_rect.left + folder_rect.right - 1) >> 1);
    folder_point.y = (int16)((folder_rect.top + folder_rect.bottom - 1) >> 1);

    //get icon center point
    switch (index)
    {
    case 0:
        icon_point = folder_point;
        break;

    case 1:
        icon_point = folder_point;
        icon_point.x = (int16)(icon_point.x - MMITHEME_ICONFOLDER_OFFSET_MIN);
        icon_point.y = (int16)(icon_point.y - MMITHEME_ICONFOLDER_OFFSET_MAX);
        break;

    case 2:
        icon_point = folder_point;
        icon_point.x = (int16)(icon_point.x + MMITHEME_ICONFOLDER_OFFSET_MAX);
        icon_point.y = (int16)(icon_point.y + MMITHEME_ICONFOLDER_OFFSET_MIN);
        break;

    case 3:
        icon_point = folder_point;
        icon_point.x = (int16)(icon_point.x - MMITHEME_ICONFOLDER_OFFSET_MAX);
        icon_point.y = (int16)(icon_point.y + MMITHEME_ICONFOLDER_OFFSET_MIN);
        break;

    default:
        icon_point = folder_point;
        break;
    }

    //get icon rect
    icon_rect.left   = (int16)(icon_point.x + 1 - (icon_width >> 1));
    icon_rect.top    = (int16)(icon_point.y + 1 - (icon_height >> 1));
    icon_rect.right  = (int16)(icon_rect.left + icon_width - 1);
    icon_rect.bottom = (int16)(icon_rect.top + icon_height - 1);

    return (icon_rect);
}

/*****************************************************************************/
//  Description : get tp down icon rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetTpDownIconRect(
                                             uint16         index,
                                             GUI_RECT_T     rect
                                             )
{
    GUI_RECT_T      icon_rect = {0};

    //get icon center point
    switch (index)
    {
    case 1:
        icon_rect.left   = rect.left;
        icon_rect.top    = (int16)(rect.top - MMITHEME_ICONFOLDER_MOVE_SPACE);
        icon_rect.right  = rect.right;
        icon_rect.bottom = (int16)(rect.bottom - MMITHEME_ICONFOLDER_MOVE_SPACE);
        break;

    case 2:
        icon_rect.left   = (int16)(rect.left + MMITHEME_ICONFOLDER_MOVE_SPACE);
        icon_rect.top    = rect.top;
        icon_rect.right  = (int16)(rect.right + MMITHEME_ICONFOLDER_MOVE_SPACE);
        icon_rect.bottom = rect.bottom;
        break;

    case 3:
        icon_rect.left   = (int16)(rect.left - MMITHEME_ICONFOLDER_MOVE_SPACE);
        icon_rect.top    = rect.top;
        icon_rect.right  = (int16)(rect.right - MMITHEME_ICONFOLDER_MOVE_SPACE);
        icon_rect.bottom = rect.bottom;
        break;

    case 0:
    default:
        icon_rect.left   = rect.left;
        icon_rect.top    = (int16)(rect.top + MMITHEME_ICONFOLDER_MOVE_SPACE);
        icon_rect.right  = rect.right;
        icon_rect.bottom = (int16)(rect.bottom + MMITHEME_ICONFOLDER_MOVE_SPACE);
        break;
    }

    return (icon_rect);
}

/*****************************************************************************/
//  Description : get icon rotate angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetIconAngle(
                                    uint16  index
                                    )
{
    uint16      angle = 0;

    //get icon rotate angle
    switch (index)
    {
    case 0:
        //not rotate
        break;

    case 1:
        angle = 350;
        break;

    case 2:
        angle = 10;
        break;

    case 3:
        angle = 340;
        break;

    default:
        //not rotate
        break;
    }

    return (angle);
}

