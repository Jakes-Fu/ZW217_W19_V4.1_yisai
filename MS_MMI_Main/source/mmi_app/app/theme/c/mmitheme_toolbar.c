/****************************************************************************
** File Name:      mmitheme_toolbar.h                                       *
** Author:         James.Zhang                                              *
** Date:           9/02/2010                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 9/2010         James.Zhang      Create                                   *
**                                                                          *
****************************************************************************/

#define _MMITHEME_TOOLBAR_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#ifdef GUIF_TOOLBAR

#include "std_header.h"
#include "mmitheme_toolbar.h"
#include "mmidisplay_data.h"
//#include "mmi_id.h"
#include "mmi_image.h"
#include "mmi_theme.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define GUITOOLBAR_SLIP_ANIM_INTERVAl       40
#define GUITOOLBAR_SLIP_ANIM_DELAY_TICKS    8
#define GUITOOLBAR_SLIP_ANIM_STEP_V         10
#define GUITOOLBAR_SLIP_ANIM_STEP_H         20
#define GUITOOLBAR_DEFAULT_SHRINK_TIMEOUT   5000

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                          LOCAL FUNCTION DECLARE                           *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获取toolbar的风格信息.
//  Global resource dependence : 
//  Author: 
//  Note:James.Zhang
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetToolbarStyle(
                                        MMITHEMETOOLBAR_STYLE_T *style_ptr
                                        )
{
#ifdef GUIF_TOOLBAR
    if (PNULL == style_ptr)
    {
        return FALSE;
    }
    
    style_ptr->bg_image     = IMAGE_TOOLBAR_BG;
    style_ptr->bg_icon      = IMAGE_TOOLBAR_ICON;
    
    style_ptr->bg_icon_highlight = IMAGE_TOOLBAR_ICON_HIGHLIGHT;
    style_ptr->bg_hit       = IMAGE_TOOLBAR_ITEM_HIGHLIGHT;
    style_ptr->separator_img    = IMAGE_TOOLBAR_SEPARATOR;

    style_ptr->left_margin  = 15;
    style_ptr->right_margin = 15;
    style_ptr->top_margin   = 8;

#ifdef MAINLCD_SIZE_320X480
    style_ptr->item_width   = 44;
    style_ptr->item_height  = 44;    
#else
    style_ptr->item_width   = 34;
    style_ptr->item_height  = 34;
#endif
#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON
    style_ptr->icon_more    = IMAGE_IDLE_PROMPT_SCHEDULE;
#endif
    style_ptr->font_color   = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);
    style_ptr->font         = MMI_DEFAULT_TEXT_FONT;

    style_ptr->auto_shrink_interval    = GUITOOLBAR_DEFAULT_SHRINK_TIMEOUT;
    style_ptr->pop_anim_interval       = GUITOOLBAR_SLIP_ANIM_INTERVAl;
    style_ptr->pop_anim_delay_ticks    = GUITOOLBAR_SLIP_ANIM_DELAY_TICKS;
    style_ptr->pop_anim_step_v         = GUITOOLBAR_SLIP_ANIM_STEP_V ;
    style_ptr->pop_anim_step_h         = GUITOOLBAR_SLIP_ANIM_STEP_H;             
#endif
    return TRUE;
}
#endif
