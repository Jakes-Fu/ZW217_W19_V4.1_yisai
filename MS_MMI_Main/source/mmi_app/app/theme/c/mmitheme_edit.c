/*****************************************************************************
** File Name:      mmitheme_edit.c                                           *
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

#define _MMITHEME_EDIT_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#include "std_header.h"
#include "guiedit.h"
#include "guiblock.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "mmi_theme.h"
#ifdef GUIF_EDIT_PDASTYLE
#include "window_parse.h"
#include "mmiclipbrd_id.h"
#include "mmiim_id.h"
#include "guiform.h"
#include "guibutton.h"
#include "guires.h"
#include "guictrl_api.h"
#include "mmitheme_pubwin.h"
#endif

#ifdef CMCC_UI_STYLE
#include "Mmipub.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 
//begin 20101202
#ifdef MAINLCD_SIZE_128X64
#define MMITHEME_EDIT_CHAR_SPACE        1
#define MMITHEME_EDIT_LINE_SPACE        1
#else
#define MMITHEME_EDIT_LINE_SPACE        2
#define MMITHEME_EDIT_CHAR_SPACE        2
#endif
//end 20101202
#if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
#define MMITHEME_EDIT_MARGIN_LR_SPACE   2
#define MMITHEME_EDIT_MARGIN_TB_SPACE   2
#else
#define MMITHEME_EDIT_MARGIN_LR_SPACE   5
#define MMITHEME_EDIT_MARGIN_TB_SPACE   5
#endif

#define MMITHEME_EDIT_CURSOR_PERIOD     500 //500ms
#define MMITHEME_EDIT_TP_SCROLL_PERIOD  100 //100ms

#define MMITHEME_EDIT_PW_PERIOD         500 //500ms

#define MMITHEME_EDI_DATE_YEAR_SET_MIN      1980
#define MMITHEME_EDI_DATE_YEAR_SET_MAX      2099

#define MMITHEME_EDI_DATE_YEAR_QUERY_MIN    1950
#define MMITHEME_EDI_DATE_YEAR_QUERY_MAX    2099

#ifdef GUIF_EDIT_PDASTYLE
#define MMITHEME_LANDSCAPE_BUTTON_WIDTH     70
#endif

#define MMITHEME_EDIT_SLIDE_TIME            10

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
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : ExceedMaxLen fun
//  Global resource dependence : 
//  Author: panvs
//  Note:
/*****************************************************************************/
void MMITHEME_ExceedMaxLenFun(void)
{
    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_BEYOND_LEN,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}
#endif

/*****************************************************************************/
//  Description : get edit input type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetEditInputType(
    GUIIM_INPUT_TYPE_T  *init_input_type_ptr,
    GUIIM_INPUT_TYPE_T  *allow_input_type_ptr,
    GUIIM_INPUT_TYPE_T  *no_handwriting_input_type_ptr
    );

#if defined GUIF_EDIT_PDASTYLE && defined MMI_IM_PDA_SUPPORT   //与IM相关，有可能IM 不支持使用PDA 版本的UI
/*****************************************************************************/
//  Description : handle landscape edit win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLandscapeEditWinMsg(
                                             MMI_WIN_ID_T       win_id,
                                             MMI_MESSAGE_ID_E   msg_id,
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description : init landscape edit
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitLandscapeEdit(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  edit_ctrl_id
                             );

/*****************************************************************************/
//  Description : set button rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetButtonRect(
                         MMI_WIN_ID_T   win_id
                         );

/*****************************************************************************/
//  Description : get button rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetButtonRect(
                               MMI_WIN_ID_T win_id,
                               GUI_RECT_T   total_rect
                               );

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
//edit text
WINDOW_TABLE(MMIPUB_LANDSCAPE_EDIT_TEXT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLandscapeEditWinMsg),
    WIN_ID(MMIPUB_LANDSCAPE_EDIT_WIN_ID),
    CREATE_BUTTON_CTRL(IMAGE_PUBWIN_BUTTON_PDA,MMIPUB_LANDSCAPE_BUTTON_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE,0,MMIPUB_LANDSCAPE_EDIT_CTRL_ID,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
    END_WIN
};

//edit phonenum
WINDOW_TABLE(MMIPUB_LANDSCAPE_EDIT_PHONENUM_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLandscapeEditWinMsg),
    WIN_ID(MMIPUB_LANDSCAPE_EDIT_WIN_ID),
    CREATE_BUTTON_CTRL(IMAGE_PUBWIN_BUTTON_PDA,MMIPUB_LANDSCAPE_BUTTON_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
        CHILD_EDIT_PHONENUM_CTRL(TRUE,0,MMIPUB_LANDSCAPE_EDIT_CTRL_ID,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
    END_WIN
};

//edit digital
WINDOW_TABLE(MMIPUB_LANDSCAPE_EDIT_DIGITAL_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLandscapeEditWinMsg),
    WIN_ID(MMIPUB_LANDSCAPE_EDIT_WIN_ID),
    CREATE_BUTTON_CTRL(IMAGE_PUBWIN_BUTTON_PDA,MMIPUB_LANDSCAPE_BUTTON_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
        CHILD_EDIT_DIGITAL_CTRL(TRUE,0,MMIPUB_LANDSCAPE_EDIT_CTRL_ID,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
    END_WIN
};

//edit password
WINDOW_TABLE(MMIPUB_LANDSCAPE_EDIT_PASSWORD_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLandscapeEditWinMsg),
    WIN_ID(MMIPUB_LANDSCAPE_EDIT_WIN_ID),
    CREATE_BUTTON_CTRL(IMAGE_PUBWIN_BUTTON_PDA,MMIPUB_LANDSCAPE_BUTTON_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
        CHILD_EDIT_PASSWORD_CTRL(TRUE,0,MMIPUB_LANDSCAPE_EDIT_CTRL_ID,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
    END_WIN
};

//edit list
WINDOW_TABLE(MMIPUB_LANDSCAPE_EDIT_LIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLandscapeEditWinMsg),
    WIN_ID(MMIPUB_LANDSCAPE_EDIT_WIN_ID),
    CREATE_BUTTON_CTRL(IMAGE_PUBWIN_BUTTON_PDA,MMIPUB_LANDSCAPE_BUTTON_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
        CHILD_EDIT_LIST_CTRL(TRUE,0,0,MMIPUB_LANDSCAPE_EDIT_CTRL_ID,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
    END_WIN
};

//edit date
WINDOW_TABLE(MMIPUB_LANDSCAPE_EDIT_DATE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLandscapeEditWinMsg),
    WIN_ID(MMIPUB_LANDSCAPE_EDIT_WIN_ID),
    CREATE_BUTTON_CTRL(IMAGE_PUBWIN_BUTTON_PDA,MMIPUB_LANDSCAPE_BUTTON_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
        CHILD_EDIT_DATE_CTRL(TRUE,MMIPUB_LANDSCAPE_EDIT_CTRL_ID,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
    END_WIN
};

//edit time
WINDOW_TABLE(MMIPUB_LANDSCAPE_EDIT_TIME_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLandscapeEditWinMsg),
    WIN_ID(MMIPUB_LANDSCAPE_EDIT_WIN_ID),
    CREATE_BUTTON_CTRL(IMAGE_PUBWIN_BUTTON_PDA,MMIPUB_LANDSCAPE_BUTTON_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
        CHILD_EDIT_TIME_CTRL(TRUE,MMIPUB_LANDSCAPE_EDIT_CTRL_ID,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
    END_WIN
};

//edit ip
WINDOW_TABLE(MMIPUB_LANDSCAPE_EDIT_IP_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLandscapeEditWinMsg),
    WIN_ID(MMIPUB_LANDSCAPE_EDIT_WIN_ID),
    CREATE_BUTTON_CTRL(IMAGE_PUBWIN_BUTTON_PDA,MMIPUB_LANDSCAPE_BUTTON_CTRL_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
        CHILD_EDIT_IP_CTRL(TRUE,MMIPUB_LANDSCAPE_EDIT_CTRL_ID,MMIPUB_LANDSCAPE_FORM_CTRL_ID),
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get edit common theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetEditTheme(
                           MMITHEME_EDIT_T  *theme_ptr  //in/out:
                           )
{
#if defined(GUIF_EDIT_PDASTYLE) && !(defined(KEYPAD_TYPE_TYPICAL_KEYPAD) || defined(KEYPAD_TYPE_QWERTY_KEYPAD))
    //set cancel delete
    theme_ptr->is_cancel_delete = FALSE;
#else
    //set cancel delete
    theme_ptr->is_cancel_delete = TRUE;
#endif

#ifdef IM_ENGINE_T9
    theme_ptr->is_disp_highlight = TRUE;
#else
    theme_ptr->is_disp_highlight = FALSE;
#endif

#ifdef GUIF_EDIT_PDASTYLE
    //set remaining number info
    theme_ptr->info_font        = SONG_FONT_12;

    //set circular handle key msg
    theme_ptr->is_cir_handle_ud = FALSE;
    theme_ptr->is_cir_handle_lf = FALSE;
#else
    //set remaining number info
    theme_ptr->info_font        = SONG_FONT_14;//SONG_FONT_12;

    //set circular handle key msg
    theme_ptr->is_cir_handle_ud = TRUE;
    theme_ptr->is_cir_handle_lf = TRUE;
#endif
    //set space
    theme_ptr->char_space      = MMITHEME_EDIT_CHAR_SPACE;
    theme_ptr->line_space      = MMITHEME_EDIT_LINE_SPACE;
    theme_ptr->margin_lr_space = MMITHEME_EDIT_MARGIN_LR_SPACE;
    theme_ptr->margin_tb_space = MMITHEME_EDIT_MARGIN_TB_SPACE;

    //set border
    theme_ptr->border.type      = GUI_BORDER_NONE;
    theme_ptr->border.width     = 1;
    theme_ptr->border.color     = MMI_DARK_GRAY_COLOR;

    //set dividing
    theme_ptr->dividing_line_width = 0;
    theme_ptr->dividing_line_color = MMI_DARK_GRAY_COLOR;

#ifdef ADULT_WATCH_SUPPORT
    //set background
    theme_ptr->bg.bg_type       = GUI_BG_COLOR;
    theme_ptr->bg.color         = MMI_WHITE_COLOR;
#else
    //set background
    theme_ptr->bg.bg_type       = GUI_BG_IMG;
    theme_ptr->bg.color         = MMI_BACKGROUND_COLOR;
#endif
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    theme_ptr->bg.img_id        = MMITHEME_GetCommonLightBg().img_id;//IMAGE_COMMON_BG_LIGHT;
#else
    theme_ptr->bg.img_id        = IMAGE_COMMON_BG;
#endif

    theme_ptr->bg.shape         = GUI_SHAPE_ROUNDED_RECT;

    //set font
    theme_ptr->font.font        = MMI_DEFAULT_TEXT_FONT;
#ifdef MAINLCD_SIZE_128X64
    theme_ptr->font.color       = MMI_BLACK_COLOR;
#else
    theme_ptr->font.color       = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_FONT);
#endif

    theme_ptr->highlight_str_color = MMI_WHITE_COLOR;
    theme_ptr->highlight_bg_color  = GUI_RGB2RGB565(255,190,0);/*lint !e572*/

    //set softkey,默认显示设置的softkey,输入字符后右softkey显示为删除
    theme_ptr->softkey.button[2].str_len = 1;
    theme_ptr->softkey.button[2].text_2  = TXT_COMMON_CLEAR;

    theme_ptr->lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    theme_ptr->lcd_dev.block_id = GUI_BLOCK_MAIN;
}

/*****************************************************************************/
//  Description : get baseflex edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetBaseFlexTheme(
                           MMITHEME_BASEFLEX_T   *flex_theme_ptr  //in/out:
                           )
{
#if defined(GUIF_EDIT_PDASTYLE) && !(defined(KEYPAD_TYPE_TYPICAL_KEYPAD) || defined(KEYPAD_TYPE_QWERTY_KEYPAD))
    //handle red key
    flex_theme_ptr->is_handle_red_key = FALSE;
#else
    //handle red key
    flex_theme_ptr->is_handle_red_key = TRUE;
#endif

    // whether handle tp message
    flex_theme_ptr->is_handle_tp = TRUE;

    //set long cancel delete char one by one
    flex_theme_ptr->is_delete_all = TRUE;

    flex_theme_ptr->is_support_slide = TRUE;
    flex_theme_ptr->slide_period = MMITHEME_EDIT_SLIDE_TIME;
    flex_theme_ptr->reset_slide_start = MMK_RECOVER_SLIDE_START_FRAME;
    flex_theme_ptr->reset_slide_frame = MMK_RECOVER_SLIDE_TOTAL_FRAME;

    flex_theme_ptr->default_str_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_DEFAULT_FONT);

    //set cursor
    flex_theme_ptr->cursor_period    = MMITHEME_EDIT_CURSOR_PERIOD;
    flex_theme_ptr->tp_scroll_period = MMITHEME_EDIT_TP_SCROLL_PERIOD;

    flex_theme_ptr->drag_mode = MMITHEME_GetDragMode();
    flex_theme_ptr->slider_display_period = CLIPBRD_SLIDER_DIS_PERIOD;

#ifdef CMCC_UI_STYLE
    flex_theme_ptr->exceedMaxLenFun = MMITHEME_ExceedMaxLenFun;
#endif
}

/*****************************************************************************/
//  Description : get edit input type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetEditInputType(
    GUIIM_INPUT_TYPE_T  *init_input_type_ptr,
    GUIIM_INPUT_TYPE_T  *allow_input_type_ptr,
    GUIIM_INPUT_TYPE_T  *no_handwriting_input_type_ptr
    )
{
#ifdef KEYPAD_TYPE_COMPACT_KEYPAD   //no key
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_TOUCHPEN;
#elif defined KEYPAD_TYPE_SINGLE_KEY_UNIVERSE
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_TOUCHPEN;
#elif defined(KEYPAD_TYPE_FOUR_KEY_UNIVERSE)//no key
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_TOUCHPEN;
#elif defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_QWERT;   
#else
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_KEYBOARD;
#endif
    GUIIM_INPUT_TYPE_T  allow_input_type = MMITHEME_IMGetInputSet();
    GUIIM_INPUT_TYPE_T  no_handwriting_input_type = allow_input_type;

    if (no_handwriting_input_type & GUIIM_INPUT_TYPE_HANDWRITING)
    {
        no_handwriting_input_type = (GUIIM_INPUT_TYPE_T)(no_handwriting_input_type ^ GUIIM_INPUT_TYPE_HANDWRITING);
    }

    if (PNULL != init_input_type_ptr)
    {
        *init_input_type_ptr = init_input_type;
    }

    if (PNULL != allow_input_type_ptr)
    {
        *allow_input_type_ptr = allow_input_type;
    }

    if (PNULL != no_handwriting_input_type_ptr)
    {
        *no_handwriting_input_type_ptr = no_handwriting_input_type;
    }
}

/*****************************************************************************/
//  Description : get text edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetTextEditTheme(
    MMITHEME_EDIT_TEXT_T    *text_theme_ptr
    )
{
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  allow_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  no_handwriting_input_type = GUIIM_INPUT_TYPE_NONE;

    if (PNULL != text_theme_ptr)
    {
        GetEditInputType(&init_input_type, &allow_input_type, &no_handwriting_input_type);

        text_theme_ptr->align = ALIGN_LVMIDDLE;
        
        //set im
        text_theme_ptr->im.is_forbid_symbol = FALSE;
        text_theme_ptr->im.init_input_type  = init_input_type;
        text_theme_ptr->im.allow_input_type = allow_input_type;
#if (defined(ADULT_WATCH_SUPPORT) && defined(TOUCH_PANEL_SUPPORT))
        text_theme_ptr->im.init_im          = GUIIM_TYPE_SYS;
        text_theme_ptr->im.allow_im         = GUIIM_TYPE_SYS;
#else
        text_theme_ptr->im.init_im          = GUIIM_TYPE_SMART;
        text_theme_ptr->im.allow_im         = GUIIM_TYPE_SET_ALL;
#endif
        text_theme_ptr->im.capital_mode     = GUIIM_MODE_LOWER;
        text_theme_ptr->im.tag              = GUIIM_TAG_NONE;
    }
}

/*****************************************************************************/
//  Description : get phonenum edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPhoneNumEditTheme(
    MMITHEME_EDIT_TEXT_T    *phonenum_theme_ptr
    )
{
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  allow_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  no_handwriting_input_type = GUIIM_INPUT_TYPE_NONE;

    if (PNULL != phonenum_theme_ptr)
    {
        GetEditInputType(&init_input_type, &allow_input_type, &no_handwriting_input_type);

        phonenum_theme_ptr->align = ALIGN_LVMIDDLE;

        //set im,config[*,+,P,W]
        phonenum_theme_ptr->im.is_forbid_symbol = FALSE;
        phonenum_theme_ptr->im.init_input_type  = init_input_type;
        phonenum_theme_ptr->im.allow_input_type = no_handwriting_input_type;
        phonenum_theme_ptr->im.init_im          = GUIIM_TYPE_DIGITAL;
        phonenum_theme_ptr->im.allow_im         = GUIIM_TYPE_DIGITAL;
        phonenum_theme_ptr->im.capital_mode     = GUIIM_MODE_LOWER;
        phonenum_theme_ptr->im.tag              = GUIIM_TAG_PHONE_NUM;
    }
}

/*****************************************************************************/
//  Description : get digital edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDigitalEditTheme(
    MMITHEME_EDIT_TEXT_T    *digital_theme_ptr
    )
{
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  allow_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  no_handwriting_input_type = GUIIM_INPUT_TYPE_NONE;

    if (PNULL != digital_theme_ptr)
    {
        GetEditInputType(&init_input_type, &allow_input_type, &no_handwriting_input_type);

        digital_theme_ptr->align = ALIGN_LVMIDDLE;

        //set im,config[.](fm)
        digital_theme_ptr->im.is_forbid_symbol = FALSE;
        digital_theme_ptr->im.init_input_type  = init_input_type;
        digital_theme_ptr->im.allow_input_type = no_handwriting_input_type;
        digital_theme_ptr->im.init_im          = GUIIM_TYPE_DIGITAL;
        digital_theme_ptr->im.allow_im         = GUIIM_TYPE_SET_ALL;
        digital_theme_ptr->im.capital_mode     = GUIIM_MODE_LOWER;
        digital_theme_ptr->im.tag              = GUIIM_TAG_DOT_NUM;
    }
}

/*****************************************************************************/
//  Description : get password edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPswEditTheme(
    MMITHEME_EDIT_PSW_T     *password_theme_ptr
    )
{
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  allow_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  no_handwriting_input_type = GUIIM_INPUT_TYPE_NONE;

    if (PNULL != password_theme_ptr)
    {
        GetEditInputType(&init_input_type, &allow_input_type, &no_handwriting_input_type);

        //set visible time
        password_theme_ptr->period = MMITHEME_EDIT_PW_PERIOD;

        password_theme_ptr->align = ALIGN_LVMIDDLE;

        //set im
        password_theme_ptr->im.is_forbid_symbol = FALSE;
        password_theme_ptr->im.init_input_type  = init_input_type;
        password_theme_ptr->im.allow_input_type = no_handwriting_input_type;
        password_theme_ptr->im.init_im          = GUIIM_TYPE_DIGITAL;
        password_theme_ptr->im.allow_im         = GUIIM_TYPE_SET_ALL;
        password_theme_ptr->im.capital_mode     = GUIIM_MODE_LOWER;
        password_theme_ptr->im.tag              = GUIIM_TAG_PURE_NUM;
    }
}

/*****************************************************************************/
//  Description : get list edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetListEditTheme(
    MMITHEME_EDIT_LIST_T    *list_theme_ptr
    )
{
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  allow_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  no_handwriting_input_type = GUIIM_INPUT_TYPE_NONE;

    if (PNULL != list_theme_ptr)
    {
        GetEditInputType(&init_input_type, &allow_input_type, &no_handwriting_input_type);

        list_theme_ptr->list_style      = GUIEDIT_LIST_STYLE_PHONENUM;
        list_theme_ptr->separator_style = GUIEDIT_SEPARATOR_SEMICOLON;
        list_theme_ptr->align           = ALIGN_LVMIDDLE;

        //set phone number im
        list_theme_ptr->phonenum_im.is_forbid_symbol = FALSE;
        list_theme_ptr->phonenum_im.init_input_type  = init_input_type;
        list_theme_ptr->phonenum_im.allow_input_type = no_handwriting_input_type;
        list_theme_ptr->phonenum_im.init_im          = GUIIM_TYPE_DIGITAL;
        list_theme_ptr->phonenum_im.allow_im         = GUIIM_TYPE_DIGITAL;
        list_theme_ptr->phonenum_im.capital_mode     = GUIIM_MODE_LOWER;
        list_theme_ptr->phonenum_im.tag              = GUIIM_TAG_PHONE_NUM;

        //set email im
        list_theme_ptr->email_im.is_forbid_symbol = FALSE;
        list_theme_ptr->email_im.init_input_type  = init_input_type;
        list_theme_ptr->email_im.allow_input_type = no_handwriting_input_type;
        list_theme_ptr->email_im.init_im          = GUIIM_TYPE_ABC;
        list_theme_ptr->email_im.allow_im         = GUIIM_TYPE_ABC;
        list_theme_ptr->email_im.capital_mode     = GUIIM_MODE_LOWER;
        list_theme_ptr->email_im.tag              = GUIIM_TAG_EMAIL;

        //set all im
        list_theme_ptr->all_im.is_forbid_symbol = FALSE;
        list_theme_ptr->all_im.init_input_type  = init_input_type;
        list_theme_ptr->all_im.allow_input_type = no_handwriting_input_type;
        list_theme_ptr->all_im.init_im          = GUIIM_TYPE_ABC;
        list_theme_ptr->all_im.allow_im         = (GUIIM_TYPE_T)(GUIIM_TYPE_DIGITAL|GUIIM_TYPE_ABC);    /*lint !e655*/
        list_theme_ptr->all_im.capital_mode     = GUIIM_MODE_LOWER;
        list_theme_ptr->all_im.tag              = GUIIM_TAG_NONE;
    }
}

/*****************************************************************************/
//  Description : get date edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDateEditTheme(
    MMITHEME_EDIT_DATE_T    *date_theme_ptr
    )
{
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  allow_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  no_handwriting_input_type = GUIIM_INPUT_TYPE_NONE;

    if (PNULL != date_theme_ptr)
    {
        GetEditInputType(&init_input_type, &allow_input_type, &no_handwriting_input_type);

        date_theme_ptr->date_style      = GUIEDIT_DATE_STYLE_YMD;
        date_theme_ptr->separator_style = GUIEDIT_SEPARATOR_DOT;
        date_theme_ptr->align           = ALIGN_HVMIDDLE;

        //set im
        date_theme_ptr->im.is_forbid_symbol = FALSE;
        date_theme_ptr->im.init_input_type  = init_input_type;
        date_theme_ptr->im.allow_input_type = no_handwriting_input_type;
        date_theme_ptr->im.init_im          = GUIIM_TYPE_DIGITAL;
        date_theme_ptr->im.allow_im         = GUIIM_TYPE_DIGITAL;
        date_theme_ptr->im.capital_mode     = GUIIM_MODE_LOWER;
        date_theme_ptr->im.tag              = GUIIM_TAG_PURE_NUM;
    }
}

/*****************************************************************************/
//  Description : get time edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetTimeEditTheme(
    MMITHEME_EDIT_TIME_T    *time_theme_ptr
    )
{
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  allow_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  no_handwriting_input_type = GUIIM_INPUT_TYPE_NONE;

    if (PNULL != time_theme_ptr)
    {
        GetEditInputType(&init_input_type, &allow_input_type, &no_handwriting_input_type);

        time_theme_ptr->time_style      = GUIEDIT_TIME_STYLE_24;
        time_theme_ptr->separator_style = GUIEDIT_SEPARATOR_COLON;
        time_theme_ptr->display_style   = GUIEDIT_TIME_DISPLAY_STYLE_HM;
        time_theme_ptr->align           = ALIGN_HVMIDDLE;

        //set im
        time_theme_ptr->im.is_forbid_symbol = FALSE;
        time_theme_ptr->im.init_input_type  = init_input_type;
        time_theme_ptr->im.allow_input_type = no_handwriting_input_type;
        time_theme_ptr->im.init_im          = GUIIM_TYPE_DIGITAL;
        time_theme_ptr->im.allow_im         = GUIIM_TYPE_DIGITAL;
        time_theme_ptr->im.capital_mode     = GUIIM_MODE_LOWER;
        time_theme_ptr->im.tag              = GUIIM_TAG_PURE_NUM;
    }
}

/*****************************************************************************/
//  Description : get IP edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetIPEditTheme(
    MMITHEME_EDIT_IP_T      *ip_theme_ptr
    )
{
    GUIIM_INPUT_TYPE_T  init_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  allow_input_type = GUIIM_INPUT_TYPE_NONE;
    GUIIM_INPUT_TYPE_T  no_handwriting_input_type = GUIIM_INPUT_TYPE_NONE;

    if (PNULL != ip_theme_ptr)
    {
        GetEditInputType(&init_input_type, &allow_input_type, &no_handwriting_input_type);

        ip_theme_ptr->align = ALIGN_HVMIDDLE;

        //set im
        ip_theme_ptr->im.is_forbid_symbol = FALSE;
        ip_theme_ptr->im.init_input_type  = init_input_type;
        ip_theme_ptr->im.allow_input_type = no_handwriting_input_type;
        ip_theme_ptr->im.init_im          = GUIIM_TYPE_DIGITAL;
        ip_theme_ptr->im.allow_im         = GUIIM_TYPE_DIGITAL;
        ip_theme_ptr->im.capital_mode     = GUIIM_MODE_LOWER;
        ip_theme_ptr->im.tag              = GUIIM_TAG_PURE_NUM;
    }
}

/*****************************************************************************/
//  Description : get password display theme,include border,bg etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPwDisplayTheme(
                                       MMITHEME_EDIT_PSW_DISPLAY_T  *theme_ptr  //in/out:
                                       )
{
    //set border
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
    theme_ptr->border.type      = GUI_BORDER_NONE;
#else
    theme_ptr->border.type      = GUI_BORDER_ROUNDED;
#endif 
    theme_ptr->border.width     = 2;
    theme_ptr->border.color     = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);

    //set background
    theme_ptr->bg.bg_type       = GUI_BG_COLOR;
    theme_ptr->bg.color         = MMI_WHITE_COLOR;
    theme_ptr->bg.shape         = GUI_SHAPE_ROUNDED_RECT;

    //set font
    theme_ptr->font.font        = MMI_DEFAULT_TEXT_FONT;
    theme_ptr->font.color       = MMI_BLACK_COLOR;
}

/*****************************************************************************/
//  Description : get edit date min and max year
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetEditDateRange(
                               uint16    *min_year_ptr,  //in/out:
                               uint16    *max_year_ptr,  //in/out:
                               uint32    year_range_type //in:GUIEDIT_DATE_YEAR_RANGE_E
                               )
{
    //SCI_ASSERT(PNULL != min_year_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != max_year_ptr); /*assert verified*/
    if(
        (PNULL == max_year_ptr)||
        (PNULL == min_year_ptr)
        )
    {
        return;
    }

    switch ((GUIEDIT_DATE_YEAR_RANGE_E)year_range_type)
    {
    case GUIEDIT_DATE_YEAR_RANGE_SET:
        *min_year_ptr = MMITHEME_EDI_DATE_YEAR_SET_MIN;
        *max_year_ptr = MMITHEME_EDI_DATE_YEAR_SET_MAX;
        break;
        
    case GUIEDIT_DATE_YEAR_RANGE_QUERY:
        *min_year_ptr = MMITHEME_EDI_DATE_YEAR_QUERY_MIN;
        *max_year_ptr = MMITHEME_EDI_DATE_YEAR_QUERY_MAX;
        break;
        
    default:
        //SCI_TRACE_LOW:"MMITHEME_GetEditDateRange: year range type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_EDIT_652_112_2_18_3_1_57_11,(uint8*)"d",year_range_type);
        break;
    }
}

#if defined GUIF_EDIT_PDASTYLE && defined MMI_IM_PDA_SUPPORT   //与IM相关，有可能IM 不支持使用PDA 版本的UI
/*****************************************************************************/
//  Description : open landscape edit win
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_OpenLandscapeEdit(//edit ctrl id
                                                uint32          edit_type,      //GUIEDIT_TYPE_E
                                                MMI_CTRL_ID_T   parent_ctrl_id
                                                )
{
    MMI_CTRL_ID_T   ctrl_id = MMIPUB_LANDSCAPE_EDIT_CTRL_ID;

    switch ((GUIEDIT_TYPE_E)edit_type)
    {
    case GUIEDIT_TYPE_TEXT:
        MMK_CreateWin((uint32 *)MMIPUB_LANDSCAPE_EDIT_TEXT_WIN_TAB,(ADD_DATA)parent_ctrl_id);
        break;

    case GUIEDIT_TYPE_PHONENUM:
        MMK_CreateWin((uint32 *)MMIPUB_LANDSCAPE_EDIT_PHONENUM_WIN_TAB,(ADD_DATA)parent_ctrl_id);
        break;

    case GUIEDIT_TYPE_DIGITAL:
        MMK_CreateWin((uint32 *)MMIPUB_LANDSCAPE_EDIT_DIGITAL_WIN_TAB,(ADD_DATA)parent_ctrl_id);
        break;

    case GUIEDIT_TYPE_PASSWORD:
        MMK_CreateWin((uint32 *)MMIPUB_LANDSCAPE_EDIT_PASSWORD_WIN_TAB,(ADD_DATA)parent_ctrl_id);
        break;

    case GUIEDIT_TYPE_LIST:
        MMK_CreateWin((uint32 *)MMIPUB_LANDSCAPE_EDIT_LIST_WIN_TAB,(ADD_DATA)parent_ctrl_id);
        break;

    case GUIEDIT_TYPE_DATE:
        MMK_CreateWin((uint32 *)MMIPUB_LANDSCAPE_EDIT_DATE_WIN_TAB,(ADD_DATA)parent_ctrl_id);
        break;

    case GUIEDIT_TYPE_TIME:
        MMK_CreateWin((uint32 *)MMIPUB_LANDSCAPE_EDIT_TIME_WIN_TAB,(ADD_DATA)parent_ctrl_id);
        break;

    case GUIEDIT_TYPE_IP:
        MMK_CreateWin((uint32 *)MMIPUB_LANDSCAPE_EDIT_IP_WIN_TAB,(ADD_DATA)parent_ctrl_id);
        break;

    default:
        ctrl_id = 0;
        break;
    }

    return (ctrl_id);
}

/*****************************************************************************/
//  Description : handle landscape edit win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLandscapeEditWinMsg(
                                             MMI_WIN_ID_T       win_id,
                                             MMI_MESSAGE_ID_E   msg_id,
                                             DPARAM             param
                                             )
{
    uint8               time_hour = 0;
    uint8               time_min = 0;
    uint8               time_sec = 0;
    uint8               date_mon = 0;
    uint8               date_day = 0;
    uint16              date_year = 0;
    uint16              item_num = 0;
    uint32              ip_addr = 0;
    MMI_WIN_ID_T        focus_win_id = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T        edit_str = {0};
    MMI_CTRL_ID_T       edit_ctrl_id = MMIPUB_LANDSCAPE_EDIT_CTRL_ID;
    MMI_CTRL_ID_T       parent_edit_ctrl_id = (MMI_CTRL_ID_T)MMK_GetWinAddDataPtr(win_id);
    GUIEDIT_TYPE_E      edit_type = GUIEDIT_TYPE_NULL;
    GUIEDIT_LIST_ITEM_T *item_array_ptr = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //init form and button
        InitLandscapeEdit(win_id,edit_ctrl_id);

        MMK_SetAtvCtrl(win_id,edit_ctrl_id);
        break;

    case MSG_FULL_PAINT:
        //set button
        SetButtonRect(win_id);
        break;

    case MSG_NOTIFY_RESIZE:
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        }
        break;

    case MSG_LOSE_FOCUS:
    case MSG_PRE_LCD_SWITCH:
    case MSG_CTL_CANCEL:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //get focus win id
        focus_win_id = MMK_GetFocusWinId();

        /*
        1 剪切板窗口
        2 mmiim_touch setting窗口
        3 原10a输入法的切换窗口
        4 符号面板窗口
        */
        if ((MSG_LOSE_FOCUS == msg_id) && 
            (MMICLIPBRD_MENU_WIN_ID == focus_win_id
            || MMIIM_TOUCH_SETTING_WIN_ID == focus_win_id
            || MMIIM_SWITCH_IM_WIN_ID == focus_win_id
            || MMIIM_SP_SYMB_WIN_ID == focus_win_id))
        {
            break;
        }

        //get edit type
        edit_type = GUIEDIT_GetType(edit_ctrl_id);
        switch (edit_type)
        {
        case GUIEDIT_TYPE_TEXT:
        case GUIEDIT_TYPE_PHONENUM:
        case GUIEDIT_TYPE_DIGITAL:
        case GUIEDIT_TYPE_PASSWORD:
            //get edit string
            GUIEDIT_GetString(edit_ctrl_id,&edit_str);

            //set parent win string
            if (0 == edit_str.wstr_len)
            {
                GUIEDIT_ClearAllStr(parent_edit_ctrl_id);
            }
            else
            {
                GUIEDIT_SetString(parent_edit_ctrl_id,edit_str.wstr_ptr,edit_str.wstr_len);
            }
            break;

        case GUIEDIT_TYPE_LIST:
            //get list string
            item_num = GUIEDIT_GetListItemNum(edit_ctrl_id);
            if (0 == item_num)
            {
                GUIEDIT_ClearAllStr(parent_edit_ctrl_id);
            }
            else 
            {
                //alloc
                item_array_ptr = SCI_ALLOC_APP(item_num*sizeof(GUIEDIT_LIST_ITEM_T));
            
                GUIEDIT_GetListItemArray(edit_ctrl_id,
                    item_num,
                    item_array_ptr);

                //clear string
                GUIEDIT_ClearAllStr(parent_edit_ctrl_id);

                //set list string
                GUIEDIT_AddListItemArray(parent_edit_ctrl_id,item_array_ptr,item_num);

                //free
                SCI_FREE(item_array_ptr);
            } 
            break;

        case GUIEDIT_TYPE_DATE:
            //get date
            GUIEDIT_GetDate(edit_ctrl_id,&date_year,&date_mon,&date_day);

            //set date
            GUIEDIT_SetDate(parent_edit_ctrl_id,date_year,date_mon,date_day);
            break;

        case GUIEDIT_TYPE_TIME:
            //get time
            GUIEDIT_GetTime(edit_ctrl_id,&time_hour,&time_min,&time_sec);

            //set time
            GUIEDIT_SetTime(parent_edit_ctrl_id,time_hour,time_min,time_sec);
            break;

        case GUIEDIT_TYPE_IP:
            //get ip
            ip_addr = GUIEDIT_GetIP(edit_ctrl_id);
            //set ip
            GUIEDIT_SetIP(parent_edit_ctrl_id,ip_addr);
            break;

        default:
            //SCI_TRACE_LOW:"HandleLandscapeEditWinMsg:edit_type %d is not set string!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_EDIT_917_112_2_18_3_1_58_12,(uint8*)"d");
            break;
        }

        //close win
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : init landscape edit
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitLandscapeEdit(
                             MMI_WIN_ID_T   win_id,
                             MMI_CTRL_ID_T  edit_ctrl_id
                             )
{
    GUI_BG_T                ctrl_bg = {0};
    MMI_CTRL_ID_T           form_ctrl_id = MMIPUB_LANDSCAPE_FORM_CTRL_ID;
    MMI_CTRL_ID_T           button_ctrl_id = MMIPUB_LANDSCAPE_BUTTON_CTRL_ID;
    GUI_BOTH_RECT_T         form_both_rect = {0};
    GUIFORM_CHILD_HEIGHT_T  child_height = {0};
    MMITHEME_PUBWIN_T       pubwin_theme;               //public window theme
    GUI_FONT_ALL_T          font = {0};

    //get client rect
    form_both_rect = MMITHEME_GetWinClientBothRect(win_id);

    //set form rect
    form_both_rect.v_rect.right = (int16)(form_both_rect.v_rect.right - MMITHEME_LANDSCAPE_BUTTON_WIDTH);
    form_both_rect.h_rect.right = (int16)(form_both_rect.h_rect.right - MMITHEME_LANDSCAPE_BUTTON_WIDTH);
    GUIAPICTRL_SetBothRect(form_ctrl_id,&form_both_rect);

    //set form margin
    GUIFORM_SetMargin(form_ctrl_id,8);
    
    //set edit min height
    child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
    GUIFORM_SetChildHeight(form_ctrl_id,edit_ctrl_id,&child_height);
    
    //set bg
    MMITHEME_GetPubTheme(0, &pubwin_theme);
    font.color = MMI_BLACK_COLOR;
    font.font  = pubwin_theme.normal_font;
    GUIBUTTON_SetFont(button_ctrl_id, &font);

    GUIBUTTON_SetRunSheen(button_ctrl_id,FALSE);

    ctrl_bg.bg_type = GUI_BG_IMG;
    ctrl_bg.img_id  = IMAGE_PUBWIN_BUTTON_HL_PDA;
    GUIBUTTON_SetPressedBg(button_ctrl_id,&ctrl_bg);

    //set text
    GUIBUTTON_SetTextId(button_ctrl_id,STXT_FINISH);
}

/*****************************************************************************/
//  Description : set button rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetButtonRect(
                         MMI_WIN_ID_T   win_id
                         )
{
    GUI_RECT_T          form_rect = {0};
    GUI_RECT_T          button_rect = {0};
    MMI_CTRL_ID_T       form_ctrl_id = MMIPUB_LANDSCAPE_FORM_CTRL_ID;
    MMI_CTRL_ID_T       button_ctrl_id = MMIPUB_LANDSCAPE_BUTTON_CTRL_ID;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    
    //get form rect
    GUIAPICTRL_GetRect(form_ctrl_id,&form_rect);

    //get client rect
    button_rect = MMITHEME_GetClientRectEx(win_id);

    //get button rect
    button_rect.left   = (int16)(button_rect.right - MMITHEME_LANDSCAPE_BUTTON_WIDTH + 1);

    //display bg
    GUIRES_DisplayImg(PNULL,
        &button_rect,
        &button_rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);

    //get button rect
    button_rect.top    = form_rect.top;
    button_rect.bottom = form_rect.bottom;

    button_rect = GetButtonRect(win_id,button_rect);

    //set button rect
    GUIBUTTON_SetRect(button_ctrl_id,&button_rect);
}

/*****************************************************************************/
//  Description : get button rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetButtonRect(
                               MMI_WIN_ID_T win_id,
                               GUI_RECT_T   total_rect
                               )
{
    uint16      button_height = 0;
    GUI_RECT_T  button_rect = {0};

    //get button height
    GUIRES_GetImgWidthHeight(PNULL,&button_height,IMAGE_PUBWIN_BUTTON_PDA,win_id);

    //get button rect
    GUI_CreateMatrixRect(&total_rect,
        4,
        0,
        0,
        0,
        1,
        1,
        &button_rect);

    button_rect = GUI_GetCenterRect(button_rect,(uint16)(button_rect.right-button_rect.left+1),button_height);

    return (button_rect);
}

/*****************************************************************************/
//  Description : is landscape edit win
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsLandscapeEdit(
                                        MMI_HANDLE_T    win_handle
                                        )
{
    BOOLEAN     result = FALSE;

    if (MMK_ConvertIdToHandle(MMIPUB_LANDSCAPE_EDIT_WIN_ID) == win_handle)
    {
        result = TRUE;
    }

    return (result);
}

#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:55 2012*/ //IGNORE9527
