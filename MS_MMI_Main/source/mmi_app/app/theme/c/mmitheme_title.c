/*****************************************************************************
** File Name:      mmitheme_title.c                                          *
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
#include "mmitheme_title.h"
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
//title
#define TITLE_SCROLL_TIMEOUT		    500  //ms

/**---------------------------------------------------------------------------*
 **                         public function                                   *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get the title style, is called by the title control
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetTitleStyle(
                                      MMI_TITLE_STYLE_T* title_style_ptr
                                      )
{
    //SCI_PASSERT(PNULL != title_style_ptr, ("MMITHEME_GetTitleStyle: the param is PNULL!")); /*assert verified*/
    if (PNULL == title_style_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET(title_style_ptr, 0x00, sizeof(MMI_TITLE_STYLE_T));/*assert verified*/

#ifdef MONOCHROME_LCD_SUPPORT 
    title_style_ptr->bg_image     = IMAGE_NULL;		//±³¾°Í¼Æ¬
    title_style_ptr->bg_color     = MMI_WHITE_COLOR; 
	title_style_ptr->font         = MMI_TITLE_TEXT_FONT;		//×ÖÌå
    title_style_ptr->font_color   = MMI_BLACK_COLOR;	//×ÖÌåÑÕÉ«
    title_style_ptr->char_space   = 0;
#else
// #ifdef MAINLCD_SIZE_240X320 //@for light function modify
//         title_style_ptr->bg_image     = IMAGE_COMMON_TITLE_BAR;//IMAGE_TITLE_BAR;		//±³¾°Í¼Æ¬
// #else
        title_style_ptr->bg_image     = MMITHEME_GetWinTitleBar();    //±³¾°Í¼Æ¬
//#endif
#endif
#ifdef ADULT_WATCH_SUPPORT
    title_style_ptr->bg_color     = MMI_BLACK_COLOR; //MMI_BLACK_COLOR;
#else
    title_style_ptr->bg_color     = MMI_BACKGROUND_COLOR; //MMI_BLACK_COLOR;
#endif
#ifdef GUIF_TITLE_PDASTYLE
	// PDA ÖØÐÂÈ¡±³¾°Í¼Æ¬
	title_style_ptr->bg_image     = MMITHEME_GetWinTitleBar();    //±³¾°Í¼Æ¬
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        title_style_ptr->font         = MMI_DEFAULT_BIG_FONT;		//×ÖÌå
    }
    else
#endif
#endif
    {
#ifdef ADULT_WATCH_SUPPORT
        title_style_ptr->font         = ADULT_WATCH_DEFAULT_SMALL_FONT;		//×ÖÌå
#else
        title_style_ptr->font         = MMI_TITLE_TEXT_FONT;		//×ÖÌå
#endif
    }
    title_style_ptr->font_color   = MMITHEME_GetCurThemeFontColor(MMI_THEME_TITLE);//MMI_WHITE_COLOR;			//×ÖÌåÑÕÉ«
    title_style_ptr->char_space   = 0;

#ifdef GUIF_TITLE_PDASTYLE
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)   
    if (MMITHEME_IsIstyle())
    {
        title_style_ptr->align_type = ALIGN_HMIDDLE;
    }
    else
#endif
    {
        title_style_ptr->align_type = ALIGN_LVMIDDLE;
    }
    title_style_ptr->title_text_margin = MMI_TITLE_TEXT_MARGIN_PDA;
    title_style_ptr->sub_font       = SONG_FONT_12;
#else
//fixed by hongjun.jia.176X220ºÍ128X160ÕâÁ½ÖÖ·Ö±æÂÊÏÂ£¬title¿¿×óÏÔÊ¾
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160) || defined (MAINLCD_SIZE_240X320)
#ifdef ADULT_WATCH_SUPPORT
    title_style_ptr->align_type = ALIGN_HMIDDLE;
#else
    title_style_ptr->align_type = ALIGN_HMIDDLE ;//ALIGN_LVMIDDLE;
#endif
#else
	title_style_ptr->align_type = ALIGN_HMIDDLE;
#endif
    title_style_ptr->title_text_margin = MMI_TITLE_TEXT_L_MARGIN;
    title_style_ptr->sub_font       = DP_FONT_14;//SONG_FONT_12;
#endif

    title_style_ptr->scroll_time    = TITLE_SCROLL_TIMEOUT;
    title_style_ptr->scroll_pixel_step = MMITHEME_ScrollPixelNum();

    title_style_ptr->button_margin = MMI_TITLE_ARROW_MARGIN;

    title_style_ptr->icon_space = DP2PX_VALUE(4);

    title_style_ptr->is_need_button = FALSE;

#ifdef GUIF_TITLE_PDASTYLE
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        title_style_ptr->is_need_button = TRUE;

        title_style_ptr->left_button_icon_down = IMAGE_ISTYLE_TITLE_RETURN_DOWN;
        title_style_ptr->left_button_icon_up = IMAGE_ISTYLE_TITLE_RETURN;

        title_style_ptr->right_button_icon_up = IMAGE_ISTYLE_TITLE_MENU;
        title_style_ptr->right_button_icon_down = IMAGE_ISTYLE_TITLE_MENU_DOWN;
        
        title_style_ptr->left_button_text_id = STXT_RETURN;
        title_style_ptr->right_button_text_id = STXT_OPTION;
        title_style_ptr->button_font_type = MMI_TITLE_TEXT_FONT;
        // title_style_ptr->istyle_menu_icon_margin = MMI_TITLE_ISTYLE_MENU_ICON_MARGIN;
    }
    else
#endif
#endif
    {
        title_style_ptr->is_need_button = FALSE;
        
#ifdef TOUCH_PANEL_SUPPORT
        title_style_ptr->left_button_icon_down = IMAGE_LIST_TITLE_ARROW_LEFT_DOWN;
        title_style_ptr->left_button_icon_up = IMAGE_LIST_TITLE_ARROW_LEFT_UP;
        title_style_ptr->left_button_icon_disable = IMAGE_LIST_LEFT_ARROW_DISABLE;
        title_style_ptr->right_button_icon_down = IMAGE_LIST_TITLE_ARROW_RIGHT_DOWN;
        title_style_ptr->right_button_icon_up = IMAGE_LIST_TITLE_ARROW_RIGHT_UP;
        title_style_ptr->right_button_icon_disable = IMAGE_LIST_RIGHT_ARROW_DISABLE;
#else
        title_style_ptr->left_button_icon_down = IMAGE_LIST_TITLE_ARROW_LEFT_UP;
        title_style_ptr->left_button_icon_up = IMAGE_LIST_TITLE_ARROW_LEFT_UP;
        title_style_ptr->left_button_icon_disable= IMAGE_LIST_LEFT_ARROW_DISABLE;
        title_style_ptr->right_button_icon_down = IMAGE_LIST_TITLE_ARROW_RIGHT_UP;
        title_style_ptr->right_button_icon_up = IMAGE_LIST_TITLE_ARROW_RIGHT_UP;
        title_style_ptr->right_button_icon_disable = IMAGE_LIST_RIGHT_ARROW_DISABLE;
#endif
    }

    return (TRUE);
}

/*****************************************************************************/
// 	Description : get title default ctrl id
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetTitleCtrlId(void)
{
	return MMICOMMON_TITLE_CTRL_ID;
}

/*****************************************************************************/
// 	Description : get the window style
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetWinTitleMaxLen(void)
{
    return GUITITLE_TEXT_MAXLEN;
}

/*****************************************************************************/
// 	Description : get the title bar
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetWinTitleBar(void)
{
#if defined (MMI_PDA_SUPPORT) && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        return IMAGE_ISTYLE_TITLE_BAR;
    }
    else
#endif
    {
#ifdef ADULT_WATCH_SUPPORT
        return IMAGE_NULL;
#else
        //return IMAGE_TITLE_BAR;
        return IMAGE_NULL;
#endif
    }
}

#ifdef GUIF_TITLE_PDASTYLE
/*****************************************************************************/
// 	Description : get the Theme title height
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetThemeTitleHeight_V(void)
{
#if defined (MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        return MMI_TITLE_THEME_HEIGHT_ISTYLE_PDA;
    }
    else
#endif
    {
        return MMI_TITLE_THEME_HEIGHT_PDA;
    }
}

/*****************************************************************************/
// 	Description : get the Theme title height Horizontal
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetThemeTitleHeight_H(void)
{
#if defined (MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        return MMI_TITLE_THEME_HEIGHT_H_ISTYLE_PDA;
    }
    else
#endif
    {
        return MMI_TITLE_THEME_HEIGHT_H_PDA;
    }
}
#endif

