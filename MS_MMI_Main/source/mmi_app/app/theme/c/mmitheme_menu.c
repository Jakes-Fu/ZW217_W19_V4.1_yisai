/*****************************************************************************
** File Name:      mmi_thememenu.c                                          *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_MENU_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#include "std_header.h" /* lint !e766 */
#include "guimenu.h"
#include "mmi_theme.h"
#include "mmi_modu_main.h"
#include "mmi_image.h"
#include "mmitheme_menu.h"
#include "mmidisplay_data.h"
#include "mmi_image.h"
#include "mmi_menutable.h"
#include "mmicc_export.h"
#include "mmisms_export.h"
#include "guires.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMITHEME_ITEM_TEXT_SCROLL_PERIOD        500 //ms

// POPUP MENU
#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
#define MMITHEME_POPUPMENU_BORDER_WIDTH         DP2PX_VALUE(5)
#else
#define MMITHEME_POPUPMENU_BORDER_WIDTH         DP2PX_VALUE(2)
#endif

#define MMITHEME_POPUPMENU_MIN_LEFT             DP2PX_VALUE(10)

// OPTIONS MENU
#define MMITHEME_OPTIONSMENU_BORDER_WIDTH       DP2PX_VALUE(2)

#define MMITHEME_BG_ALPHA                       240   // 95%的不透明度


#define MMIMENU_SEC_ICON_NUM                    20

#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMIMENU_SELECTED_BAR_HEIGHT         20//highlight bar height
#define MMITHEME_MENU_GAP_WIDTH             2
#define MMITHEME_SECMENU_GAP_WIDTH          MMITHEME_MENU_GAP_WIDTH

#define MMIMENU_POPUP_ITEM_HEIGHT           MMIMENU_SELECTED_BAR_HEIGHT //popup menu height

#elif defined MAINLCD_SIZE_128X64
#define MMIMENU_SELECTED_BAR_HEIGHT         16  //highlight bar height
#define MMITHEME_MENU_GAP_WIDTH             0
#define MMITHEME_SECMENU_GAP_WIDTH          0
#define MMIMENU_POPUP_ITEM_HEIGHT           MMIMENU_SELECTED_BAR_HEIGHT //popup menu height

#elif defined MAINLCD_SIZE_240X320
#ifdef GUISECMENU_PDASTYLE_SUPPORT
#define MMITHEME_SECMENU_GAP_WIDTH          DP2PX_VALUE(10)
#define MMIMENU_SELECTED_BAR_HEIGHT         DP2PX_VALUE(40)//highlight bar height
#else
#define MMITHEME_SECMENU_GAP_WIDTH          DP2PX_VALUE(5)
#define MMIMENU_SELECTED_BAR_HEIGHT         DP2PX_VALUE(25)//highlight bar height
#endif

#define MMITHEME_MENU_GAP_WIDTH             DP2PX_VALUE(5)

#if defined(GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT) || defined(GUIPOPMENU_POP_CONTEXT_SUPPORT)
#define MMIMENU_POPUP_ITEM_HEIGHT           MMIMENU_SELECTED_BAR_HEIGHT //popup menu height
#else
#define MMIMENU_POPUP_ITEM_HEIGHT           DP2PX_VALUE(40) //popup menu height
#endif

#elif  defined MAINLCD_SIZE_240X400
#ifdef GUISECMENU_PDASTYLE_SUPPORT
#define MMITHEME_SECMENU_GAP_WIDTH          10
#define MMIMENU_SELECTED_BAR_HEIGHT         40//highlight bar height
#else
#define MMITHEME_SECMENU_GAP_WIDTH          5
#define MMIMENU_SELECTED_BAR_HEIGHT         25//highlight bar height
#endif
#define MMITHEME_MENU_GAP_WIDTH             5

#if defined(GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT) || defined(GUIPOPMENU_POP_CONTEXT_SUPPORT)
#define MMIMENU_POPUP_ITEM_HEIGHT           MMIMENU_SELECTED_BAR_HEIGHT //popup menu height
#else
#define MMIMENU_POPUP_ITEM_HEIGHT           40 //popup menu height
#endif

#elif  defined MAINLCD_SIZE_320X480
#ifdef GUISECMENU_PDASTYLE_SUPPORT
#define MMITHEME_SECMENU_GAP_WIDTH          15
#define MMIMENU_SELECTED_BAR_HEIGHT         50//highlight bar height
#else
#define MMITHEME_SECMENU_GAP_WIDTH          5
#define MMIMENU_SELECTED_BAR_HEIGHT         41//highlight bar height
#endif
#define MMITHEME_MENU_GAP_WIDTH             5

#if defined(GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT) || defined(GUIPOPMENU_POP_CONTEXT_SUPPORT)
#define MMIMENU_POPUP_ITEM_HEIGHT           MMIMENU_SELECTED_BAR_HEIGHT //popup menu height
#else
#define MMIMENU_POPUP_ITEM_HEIGHT           DP2PX_VALUE(50) //popup menu height
#endif

#else
#error
#endif

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
//second menu
const MMI_IMAGE_ID_T s_mmimenu_sec_icon[] =
{
    //item icon image id
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_1,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_2,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_3,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_4,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_5,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_6,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_7,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_8,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_9,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_10,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_11,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_12,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_13,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_14,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_15,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_16,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_17,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_18,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_19,
    (MMI_IMAGE_ID_T)IMAGE_NUMBER_20
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get the menu group
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void* MMITHEME_GetMenuGroup(
                                   MMI_MENU_GROUP_ID_T   group_id
                                   )
{
    return ((void *)MMI_GetMenuGroup(group_id));
}

/*****************************************************************************/
//  Description : get menu common theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetCommonMenuTheme(
                                 MMITHEME_MENU_COMMON_T     *theme_ptr //in/out:
                                 )
{
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    theme_ptr->is_used_slide    = TRUE;

#ifdef MMI_PDA_SUPPORT
    theme_ptr->is_circle        = FALSE;
    theme_ptr->is_has_button    = TRUE;
    theme_ptr->is_hl_disappear  = TRUE;
#else
    theme_ptr->is_circle        = TRUE;
    theme_ptr->is_has_button    = TRUE;
    theme_ptr->is_hl_disappear  = FALSE;
#endif

    theme_ptr->bg_image         = IMAGE_THEME_BG;

    theme_ptr->gap_width        = MMITHEME_MENU_GAP_WIDTH;
    theme_ptr->bg_color         = MMI_BACKGROUND_COLOR;
    theme_ptr->split_line_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SPLIT_LINE);

    theme_ptr->lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    theme_ptr->lcd_dev.block_id = GUI_BLOCK_MAIN;

    theme_ptr->selected_img = IMAGE_COMMON_SELECTED_BAR;

    theme_ptr->text_null_id   = TXT_NULL;
    theme_ptr->text_ok_id     = TXT_COMMON_OK;
    theme_ptr->text_cancel_id = STXT_CANCEL;
    theme_ptr->text_return_id = STXT_RETURN;
}

/*****************************************************************************/
//  Description : get second menu theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetSecondMenuTheme(
                                 MMITHEME_SECONDMENU_T  *theme_ptr //in/out:
                                 )
{
    if (PNULL == theme_ptr)
    {
        return;
    }

    //set second menu item font
    theme_ptr->gap_width = MMITHEME_SECMENU_GAP_WIDTH;
    theme_ptr->item_font = MMI_LIST_DEFAULT_ITEM_FONT;
    theme_ptr->cur_item_font = MMI_LIST_EXPAND_ITEM_FONT;

    //set item text scroll
    theme_ptr->pixel_scroll_step  = (uint16)(MMITHEME_ScrollPixelNum());
    theme_ptr->item_scroll_period = MMITHEME_ITEM_TEXT_SCROLL_PERIOD;

    theme_ptr->item_height        = MMI_SECMENU_ITEM_HEIGHT;
    theme_ptr->expand_item_height = MMI_SECMENU_ITEM_HEIGHT;
    theme_ptr->item_space         = 0;

    theme_ptr->is_draw_default_icon = TRUE;

    //set title icon
//#ifdef MAINLCD_SIZE_240X320 //@for light function modify
//    theme_ptr->title_icon_id = IMAGE_COMMON_TITLE_BAR;
//#else
//    theme_ptr->title_icon_id = MMITHEME_GetWinTitleBar();
//#endif
    theme_ptr->title_icon_id = MMITHEME_GetWinTitleBar();
}

/*****************************************************************************/
//  Description : get second menu icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetSecItemIcon(
                                              uint16    item_index
                                              )
{
    MMI_IMAGE_ID_T  img_id = 0;

    if (MMIMENU_SEC_ICON_NUM > item_index)
    {
        img_id = s_mmimenu_sec_icon[item_index];
    }

    return (img_id);
}

/*****************************************************************************/
//  Description : get pop-up menu theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetPopMenuTheme(
                              MMITHEME_POPMENU_T    *theme_ptr //in/out:
                              )
{
    //set mask,65%透明
    theme_ptr->mask_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_MASK_LAYER);
    theme_ptr->mask_alpha = MMI_MASK_ALPHA;

    //set pop-up menu background
#ifdef MONOCHROME_LCD_SUPPORT 
    theme_ptr->bg_color     = MMI_WHITE_COLOR;
#else
    theme_ptr->bg_color     = MMITHEME_GetCurThemeFontColor(MMI_THEME_POPMENU_BG);//    MMITHEME_POPUPMENU_BG_COLOR;
#endif//syy modify 2010.11.30 -end

    theme_ptr->bg_alpha = MMITHEME_BG_ALPHA;

    //set pop-up menu border
    theme_ptr->border_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_POPMENU_BG_BORDER); //MMITHEME_POPUPMENU_BORDER_COLOR;
    theme_ptr->border_width = MMITHEME_POPUPMENU_BORDER_WIDTH;
    
    //set sub pop-up menu left coordinate range
    theme_ptr->sub_min_left = MMITHEME_POPUPMENU_MIN_LEFT;
    
    theme_ptr->pixel_scroll_step  = (uint16)(MMITHEME_ScrollPixelNum());
    
    //set pop-up menu item
    theme_ptr->item_height = MMIMENU_POPUP_ITEM_HEIGHT;
    theme_ptr->item_space = 0;
    theme_ptr->item_font   = theme_ptr->cur_item_font = MMI_DEFAULT_TEXT_FONT;

    theme_ptr->context_title_font = MMI_DEFAULT_SMALL_FONT;

#ifdef GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
    theme_ptr->border_img      = IMAGE_OPTIONSMENU_MORE_BORDER;
#endif
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    theme_ptr->down_arrow_img  = IMAGE_PUBWIN_DOWNARROW;
#endif
    theme_ptr->radio_sel_img   = IMAGE_RADIO_SELECTED_ICON;
    theme_ptr->radio_unsel_img = IMAGE_RADIO_UNSELECTED_ICON;
    theme_ptr->check_sel_img   = IMAGE_CHECK_SELECTED_ICON;
    theme_ptr->check_unsel_img = IMAGE_CHECK_UNSELECTED_ICON;
    theme_ptr->left_img        = IMAGE_COMMON_POP_MENU_LEFT;
    theme_ptr->right_img       = IMAGE_COMMON_POP_MENU_RIGHT;
    theme_ptr->left_gray_img   = IMAGE_COMMON_POP_MENU_LEFT_GRAY;
    theme_ptr->left_gray_img   = IMAGE_COMMON_POP_MENU_RIGHT_GRAY;

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    MMITHEME_GetPubLayoutTheme(&theme_ptr->layout);
#endif
}

#ifdef MMI_OPTIONMENU_SUPPORT
/*****************************************************************************/
//  Description : get OptionsMenu theme
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
void MMITHEME_GetOptionsMenuTheme(
                                  MMITHEME_OPTIONSMENU_T    *theme_ptr // [out]
                                  )
{
    theme_ptr->item_height  = MMITHEME_OPTIONSMENU_ITEM_HEIGHT;
    theme_ptr->image_height = MMITHEME_OPTIONSMENU_IAMGE_HEIGHT;
    theme_ptr->max_item_num = MMITHEME_OPTIONSMENU_MAX_ITEM_NUM;
    theme_ptr->border_width = MMITHEME_OPTIONSMENU_BORDER_WIDTH;
    theme_ptr->v_min_item_width = MMITHEME_OPTIONSMENU_V_MIN_ITEM_WIDTH;
    theme_ptr->h_min_item_width = MMITHEME_OPTIONSMENU_H_MIN_ITEM_WIDTH;
    theme_ptr->more_item_text_id = TXT_COMN_MORE;
#if defined GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
    theme_ptr->more_item_image_id = IMAGE_OPTIONSMENU_MORE;
    theme_ptr->shadow_img = IMAGE_OPTIONSMENU_SHADOW;
#endif
    theme_ptr->bg_alpha     = MMITHEME_BG_ALPHA;
    theme_ptr->bg_color     = MMITHEME_GetCurThemeFontColor(MMI_THEME_POPMENU_BG);
    theme_ptr->border_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_POPMENU_BG_BORDER);

    theme_ptr->item_font        = MMI_DEFAULT_SMALL_FONT;
    theme_ptr->cur_item_font    = MMI_DEFAULT_SMALL_FONT;
}
#endif

//#endif
