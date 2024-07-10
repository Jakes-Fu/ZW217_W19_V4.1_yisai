/*****************************************************************************
** File Name:      mmitheme_richtext.c                                       *
** Author:         Great.Tian                                                *
** Date:           03/9/2011                                                 *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        Xiaoqing.lu      Create								     *
**																		     *
*****************************************************************************/

 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmitheme_richtext.h"
#include "mmi_theme.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guilcd.h"
#include "mmi_image.h"
#include "mmi_anim.h"
#include "mmidisplay_data.h"
#include "guisoftkey.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "mmi_text.h"
#include "mmi_id.h"
#include "mmi_default.h"
#include "mmi_textfun.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmitv_out.h"
#ifndef WIN32
#include "arm_reg.h"
#endif
#include "guibutton.h"
#ifdef _SMMI_UNIT_TEST_
#define LOCAL 
#endif
#include "guiref_scale.h"
#include "guifont.h"
#include "mmiidle_export.h"
#include "mmk_app.h"
//#include "mmi_wallpaper_export.h"
#include "mmiset_id.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmitheme_special_func.h"
#include "block_mem.h"
//#include "mmiset_display.h"
#include "guires.h"
#include "mmi_nv.h"
#ifdef UI_P3D_SUPPORT
#include "mmi3d_export.h"
#endif
#include "mmi_custom_define.h"
#include "mmi_position.h"
#include "guistring.h"
#include "ui_layer.h"
#include "mmi_color.h"
#include "mmi_resource.h"
#include "tp_id.h"
#include "mmi_modu_main.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

//richtext
#define RICHTEXT_BORDER_SPACE               DP2PX_VALUE(5)
#define RICHTEXT_IMG_TXT_SPACE              DP2PX_VALUE(7)
#ifdef MAINLCD_SIZE_128X64
#define RICHTEXT_TEXT_LINE_SPACE            1
#define RICHTEXT_ITEM_SPACE                 0
#elif defined (MAINLCD_SIZE_128X160) || defined (MAINLCD_SIZE_176X220)
#define RICHTEXT_TEXT_LINE_SPACE            2
#define RICHTEXT_ITEM_SPACE                 1
#else
#define RICHTEXT_ITEM_SPACE                 DP2PX_VALUE(7)
#define RICHTEXT_TEXT_LINE_SPACE            DP2PX_VALUE(4)
#endif
#define RICHTEXT_TEXT_CHAR_SPACE            0
#define RICHETXT_TEXT_MARGIN                DP2PX_VALUE(2)


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get the richtext style, is called by richtext control
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetRichTextStyle(
                                         MMI_RICHTEXT_STYLE_T  * richtext_style_ptr
                                         )
{
    if (PNULL == richtext_style_ptr)
    {
        return FALSE;
    }

#ifdef MONOCHROME_LCD_SUPPORT 
    richtext_style_ptr->bg_image_id = IMAGE_NULL;
    richtext_style_ptr->anim_parsing_icon_id = IMAGE_NULL;
    richtext_style_ptr->anim_error_icon_id = IMAGE_NULL;
#else
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    richtext_style_ptr->bg_image_id = MMITHEME_GetCommonLightBg().img_id;//IMAGE_COMMON_BG_LIGHT;
    richtext_style_ptr->bg_color = MMI_WHITE_COLOR;
#else
    richtext_style_ptr->bg_image_id = IMAGE_COMMON_BG;
    richtext_style_ptr->bg_color = MMI_BACKGROUND_COLOR;
#endif
    richtext_style_ptr->anim_parsing_icon_id = IMAGE_COMMON_PIC_ICON;
    richtext_style_ptr->anim_error_icon_id = IMAGE_COMMON_PIC_ERROR;
#endif 
    richtext_style_ptr->font = MMI_DEFAULT_TEXT_FONT;


    richtext_style_ptr->font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_RICHTEXT_FONT);// MMI_GRAY_WHITE_COLOR;
    richtext_style_ptr->focusline_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    richtext_style_ptr->focus_bg_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    richtext_style_ptr->border_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_RICHTEXT_BORDER);//0x39c7;
    richtext_style_ptr->border_focus_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2010/12/30
    richtext_style_ptr->item_space = 0;
    richtext_style_ptr->border_space = 0;
    richtext_style_ptr->char_space = 0;
    richtext_style_ptr->text_line_space = DP2PX_VALUE(1);
    richtext_style_ptr->img_text_space = 0;
    richtext_style_ptr->txt_margin = 0;
#else
    richtext_style_ptr->item_space = RICHTEXT_ITEM_SPACE;
    richtext_style_ptr->border_space = RICHTEXT_BORDER_SPACE;
    richtext_style_ptr->char_space = RICHTEXT_TEXT_CHAR_SPACE;
    richtext_style_ptr->text_line_space = RICHTEXT_TEXT_LINE_SPACE;
    richtext_style_ptr->img_text_space = RICHTEXT_IMG_TXT_SPACE;
    richtext_style_ptr->txt_margin = RICHETXT_TEXT_MARGIN;
#endif

    richtext_style_ptr->drag_mode = MMITHEME_GetDragMode();
    richtext_style_ptr->slider_display_period = CLIPBRD_SLIDER_DIS_PERIOD;

    return TRUE;
}

