/****************************************************************************
** File Name:      mmi_theme.c                                             *
** Author:         Great.Tian                                              *
** Date:           06/21/2004                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the mmi theme.            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2004        Great.Tian       Create
** 
****************************************************************************/
#define MMI_THEME_C


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#include "std_header.h"
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
#include "mmitv_out.h"
#include "mmk_tp.h"
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
//#include "mmiset.h"
#include "mmiset_export.h"
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
#ifdef MMI_READMENU_ENABLE
#include "mmitts_export.h"
#include "Mmisrvaud_api.h"
#endif
#ifdef HERO_ENGINE_TTS_SUPPORT 
#include "mmitts_export.h"
#endif
#include "mmiss_export.h"
#ifdef PRINTSCREEN_SUPPORT
#include "mmiptscr_export.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

// 所有的按像素滚动的像素数
#define MMITHEME_SCROLL_PIXEL_STEP			10


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

#ifdef BG_ON_BOTTOM_SUPPORT
LOCAL BOOLEAN s_is_bglayer_changed = TRUE;     //背景层数据发生变化
LOCAL UILAYER_HANDLE_T s_bg_layer_handle = UILAYER_NULL_HANDLE;
#endif
LOCAL MMI_DIRECTION_SET_E   s_display_direction   = MMI_DIRECTION_LEFT;  //0：左排；1：右排；2：竖排  
LOCAL BOOLEAN   is_scroll_as_pixel                      = FALSE; //win title, label,menu,listbox scroll as pixel or character

// 这里的值必须要与MMI_THEME_FONT_COLOR_INDEX_E对应，还要在common_mdu_def.h里RES_ADD_COLOR
LOCAL const uint32 s_theme_font_color_arry[MMI_THEME_FONT_COLOR_INDEX_MAX] =
{
    COLOR_RES_TITLE,              
    COLOR_RES_MENU,               
    COLOR_RES_POPMENU,  
    COLOR_RES_MAINMENU_FONT,
    COLOR_RES_PUBWIN_SOFTKEY_FONT,
    COLOR_RES_TAB_FONT,           
    COLOR_RES_SOFTKEY_FONT,       
    COLOR_RES_SOFTKEY_PRESSED,    
    COLOR_RES_LABEL_FONT,         
    COLOR_RES_LIST_ITEM0,         
    COLOR_RES_LIST_ITEM1,         
    COLOR_RES_LIST_ITEM2,
    COLOR_RES_LIST_ITEM_SMALL_FONT,
    COLOR_RES_CALENDOR_TITLE,     
    COLOR_RES_IDLE_SOFTKEY,       
    COLOR_RES_IDLE_TIME,          
    COLOR_RES_IDLE_DATE,          
    COLOR_RES_IDLE_WEEK,          
    COLOR_RES_IDLE_IDLEWORD,      
    COLOR_RES_IDLE_MP3,           
    COLOR_RES_IDLE_KUR,           
    COLOR_RES_IDLE_FM,            
    COLOR_RES_IDLE_COUNTTIME,     
    COLOR_RES_IDLE_NETWORKNAME,   
    COLOR_RES_IDLE_PROVIDERNAME,  
    COLOR_RES_IDLE_CALENDAR,      
    COLOR_RES_IDLE_STK,
    COLOR_RES_BACKGROUND_TIPS,
    COLOR_RES_EDIT_FONT,
    COLOR_RES_POPMENU_BG,
    COLOR_RES_POPMENU_BG_BORDER,
    COLOR_RES_CALENDAR_DAY,
    COLOR_RES_CALENDAR_WEEK,
    COLOR_RES_CALENDAR_YEAR,
    COLOR_RES_CALENDAR_MONTH,
    COLOR_RES_CALENDAR_TASK,
    COLOR_RES_TIPS_FONT,
    COLOR_RES_RICHTEXT_FONT,
    COLOR_RES_TEXT_FONT,
    COLOR_RES_RICHTEXT_BORDER,
    COLOR_RES_MMS_RICHTEXT_BORDER,
    COLOR_RES_SELECT,
    COLOR_RES_FORM_BG,
    COLOR_RES_CUBEMENU_TOP_SIDE,
    COLOR_RES_VKB_KEY,      //虚拟键盘字符颜色
    COLOR_RES_VKB_KEY_HIGHLIGHT,      //虚拟键盘字符高亮颜色
    COLOR_RES_SPLIT_LINE,
    COLOR_RES_MASK_LAYER,
    COLOR_RES_EDIT_TOUCH_CUR,
	COLOR_RES_SUB_LCD,
	COLOR_RES_EDIT_DEFAULT_FONT,
	COLOR_RES_EDIT_TOUCH_NORMAL,
	COLOR_RES_EDIT_TOUCH_TIPS,
	COLOR_RES_CONTEXTMENU_TITLE_FONT,
	COLOR_RES_PUBWIN_BG,
};

LOCAL const GUI_LCD_DEV_INFO s_default_lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

LOCAL MMITHEME_HIDE_ANIM_E s_hide_anim_type = MMITHEME_HIDE_FADE;

#ifdef MMI_PDA_SUPPORT
LOCAL MMITHEME_TRIGGER_MODE_E	s_pretrigger_mode = MMITHEME_TRIGGER_MODE_TP;
LOCAL MMITHEME_TRIGGER_MODE_E	s_trigger_mode = MMITHEME_TRIGGER_MODE_TP;
#else
LOCAL MMITHEME_TRIGGER_MODE_E	s_pretrigger_mode = MMITHEME_TRIGGER_MODE_KEY;
LOCAL MMITHEME_TRIGGER_MODE_E	s_trigger_mode = MMITHEME_TRIGGER_MODE_KEY;
#endif

#ifdef GUIF_CLIPBOARD
LOCAL MMITHEME_DRAGMODE_E s_drag_mode = MMITHEME_DRAGMODE_SLIDER;
#else
LOCAL MMITHEME_DRAGMODE_E s_drag_mode = MMITHEME_DRAGMODE_DEFAULT;
#endif

#ifdef GUI_DUMP_CTRL_TREE
#define GUIFORM_GUID_NAME(guid_name)    guid_name, #guid_name
#define GUIFORM_SPACE_LENGTH            20      // 最多空格数
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#define MMITHEME_ACCENT_COLOR_DEFAULT   MMITHEME_ACCENT_COLOR_YELLOW

LOCAL GUI_COLOR_T s_theme_accent_color_array[MMITHEME_ACCENT_COLOR_MAX][MMITHEME_OFFSET_COLOR_MAX] = 
{
    {GUI_RGB2RGB565(0xde, 0x94, 0xe0), GUI_RGB2RGB565(0xcc, 0x76, 0xca), GUI_RGB2RGB565(0xc2, 0x39, 0xb3), GUI_RGB2RGB565(0xb4, 0x00, 0x9e), 

GUI_RGB2RGB565(0x9a, 0x00, 0x89), GUI_RGB2RGB565(0x80, 0x00, 0x74), GUI_RGB2RGB565(0x5c, 0x00, 0x5c)},
    {GUI_RGB2RGB565(0xf5, 0xc1, 0xb2), GUI_RGB2RGB565(0xee, 0x99, 0x80), GUI_RGB2RGB565(0xe6, 0x68, 0x44), GUI_RGB2RGB565(0xe0, 0x42, 0x15), 

GUI_RGB2RGB565(0xbf, 0x38, 0x12), GUI_RGB2RGB565(0xa2, 0x30, 0x0f), GUI_RGB2RGB565(0x6e, 0x20, 0x0a)},
    {GUI_RGB2RGB565(0xff, 0xda, 0xbb), GUI_RGB2RGB565(0xff, 0xc9, 0x88), GUI_RGB2RGB565(0xff, 0xaa, 0x44), GUI_RGB2RGB565(0xff, 0x8c, 0x00), 

GUI_RGB2RGB565(0xd4, 0x73, 0x00), GUI_RGB2RGB565(0xb0, 0x5e, 0x0d), GUI_RGB2RGB565(0x7f, 0x42, 0x00)},
    {GUI_RGB2RGB565(0x93, 0xd7, 0x93), GUI_RGB2RGB565(0x68, 0xc8, 0x68), GUI_RGB2RGB565(0x36, 0xb4, 0x36), GUI_RGB2RGB565(0x0a, 0xa5, 0x0a), 

GUI_RGB2RGB565(0x0a, 0x7c, 0x07), GUI_RGB2RGB565(0x04, 0x57, 0x04), GUI_RGB2RGB565(0x00, 0x43, 0x04)},
    {GUI_RGB2RGB565(0xb5, 0xc5, 0xc2), GUI_RGB2RGB565(0x97, 0xaf, 0xa9), GUI_RGB2RGB565(0x74, 0x94, 0x8c), GUI_RGB2RGB565(0x56, 0x7c, 0x73), 

GUI_RGB2RGB565(0x40, 0x5d, 0x56), GUI_RGB2RGB565(0x2e, 0x42, 0x3c), GUI_RGB2RGB565(0x23, 0x32, 0x2f)},
    {GUI_RGB2RGB565(0x8f, 0xc6, 0xf1), GUI_RGB2RGB565(0x60, 0xb0, 0xf1), GUI_RGB2RGB565(0x2b, 0x90, 0xe2), GUI_RGB2RGB565(0x00, 0x77, 0xd6), 

GUI_RGB2RGB565(0x02, 0x59, 0xa2), GUI_RGB2RGB565(0x00, 0x3d, 0x6f), GUI_RGB2RGB565(0x00, 0x2f, 0x56)},
};

LOCAL MMITHEME_ACCENT_COLOR_E s_theme_accent_id = MMITHEME_ACCENT_COLOR_DEFAULT;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
PUBLIC GUI_COLOR_T MMITHEME_GetAccentColorByOffset(
                                                   MMITHEME_OFFSET_COLOR_E offset_id
                                                   )
{
    MMITHEME_ACCENT_COLOR_E accent_id = s_theme_accent_id;
	
    if((accent_id < MMITHEME_ACCENT_COLOR_MAX) && (offset_id < MMITHEME_OFFSET_COLOR_MAX))
    {
        return s_theme_accent_color_array[accent_id][offset_id];
    }
    else
    {
        SCI_TRACE_LOW("MMITHEME_GetAccentColorByOffset: accent_id = %d, offset_id = %d, return default color!", accent_id, offset_id);
        return s_theme_accent_color_array[MMITHEME_ACCENT_COLOR_DEFAULT][MMITHEME_OFFSET_COLOR_DEFAULT];
    }
}

/*****************************************************************************/
// 	Description : get the calendar style, is called by the calendar control
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetCalendarStyle(
                                         MMI_CALENDAR_STYLE_T* calendar_style_ptr
	                                     )
{
    if (PNULL == calendar_style_ptr)
    {
        //SCI_TRACE_LOW:"MMITHEME_GetCalendarStyle: the param is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_THEME_224_112_2_18_3_1_47_0,(uint8*)"");
        return FALSE;
    }

    SCI_MEMSET(calendar_style_ptr, 0x00, sizeof(MMI_CALENDAR_STYLE_T));

    calendar_style_ptr->title_height        = MMI_DEFAULT_SMALL_FONT_SIZE;
    calendar_style_ptr->week_height         = MMI_DEFAULT_SMALL_FONT_SIZE;
    calendar_style_ptr->back_image          = IMAGE_NULL;
    calendar_style_ptr->day_back_color      = MMI_WINDOW_BACKGROUND_COLOR;
    calendar_style_ptr->day_font            = SONG_FONT_8;
    calendar_style_ptr->day_font_color      = MMI_DEFAULT_TEXT_COLOR;
    calendar_style_ptr->day_highlight_color = MMI_DARK_CYAN_COLOR;
    calendar_style_ptr->saturday_color      = MMI_YELLOW_COLOR;
    calendar_style_ptr->sunday_color        = MMI_DARK_RED_COLOR;
    calendar_style_ptr->title_font          = MMI_DEFAULT_SMALL_FONT;
    calendar_style_ptr->title_font_color    = MMITHEME_GetCurThemeFontColor(MMI_THEME_CALENDOR_TITLE);//MMI_BLUE_COLOR;
    calendar_style_ptr->title_back_color    = MMI_YELLOW_COLOR;
    calendar_style_ptr->week_back_color     = MMI_GRAY_WHITE_COLOR;
    calendar_style_ptr->week_font           = MMI_DEFAULT_SMALL_FONT;
    calendar_style_ptr->week_font_color     = MMI_BLUE_COLOR;
    return (TRUE);
}

/*****************************************************************************/
// 	Description : get the msgbox style, is called by the msgbox control
//	Global resource dependence : 
//  Author:lin.lin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetMsgBoxStyle(
                                        MMI_MSGBOX_STYLE_T* msgbox_style_ptr
	                                    )
{
    GUI_RECT_T image_rect = MMI_PROMPT_IMAGE_RECT;
    GUI_RECT_T  text_rect = MMI_PROMPT_TEXT_RECT;
    GUI_RECT_T  sk_rect   = MMI_PROMPT_SK_RECT;

    if (PNULL == msgbox_style_ptr)
    {
        //SCI_TRACE_LOW:"MMITHEME_GetMsgBoxStyle: the param is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_THEME_262_112_2_18_3_1_47_1,(uint8*)"");
        return FALSE;
    }

    SCI_MEMSET(msgbox_style_ptr, 0x00, sizeof(MMI_MSGBOX_STYLE_T));

    msgbox_style_ptr->back_color        = 0;
    msgbox_style_ptr->back_image_id     = IMAGE_NULL;
    msgbox_style_ptr->char_space        = 0;
    msgbox_style_ptr->font              = MMI_DEFAULT_TEXT_FONT;
    msgbox_style_ptr->font_color        = MMI_DEFAULT_TEXT_COLOR;
    msgbox_style_ptr->line_space        = 0;
    msgbox_style_ptr->move_line         = 1;
    msgbox_style_ptr->text_back_color   = 0;
    msgbox_style_ptr->title_color       = 0;
    msgbox_style_ptr->title_height      = MMI_DEFAULT_NORMAL_FONT_SIZE;
    msgbox_style_ptr->img_vspace        = 0;
    msgbox_style_ptr->img_hspace        = 0;
    msgbox_style_ptr->image_rect        = image_rect;
    msgbox_style_ptr->text_rect         = text_rect;
    //msgbox_style_ptr->sk_height         = 18;
    msgbox_style_ptr->sk_rect           = sk_rect;
    msgbox_style_ptr->sk_bg_color       = MMI_PROMPT_SK_BG_COLOR;
    msgbox_style_ptr->sk_text_id[0]     = TXT_NULL;
    msgbox_style_ptr->sk_text_id[1]     = TXT_NULL;
    msgbox_style_ptr->sk_text_id[2]     = TXT_NULL;
    msgbox_style_ptr->title_text_id     = TXT_NULL;
    return TRUE;
}

/*****************************************************************************/
//  Description : 判断lcd是否点亮        
//  Global resource dependence : 
//  Author: xiaoqing.lu 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsBacklightOn(void)
{
    return MMIDEFAULT_IsBacklightOn();
}

/*****************************************************************************/
// 	Description : get the transparence color 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetTransparenceColor(
                                                 void
                                                 )
{
    return (MMI_TRANSPARENCE_COLOR);
}

/*****************************************************************************/
// 	Description : get the return text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMITHEME_GetReturnTextID(
                                        void
                                        )
{
    return (STXT_RETURN);
}

/*****************************************************************************/
// 	Description : get the Cancel text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMITHEME_GetCancelTextID(
                                        void
                                        )
{
    return (STXT_CANCEL);
}

/*****************************************************************************/
// 	Description : get the Cancel text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetSoftkeyLeftCtrlId(
                                        void
                                        )
{
    return (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID);
}

/*****************************************************************************/
// 	Description : get the Cancel text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetSoftkeyRightCtrlId(
                                        void
                                        )
{
    return (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID);
}

/*****************************************************************************/
// 	Description : get the Cancel text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetSoftkeyMidCtrlId(
                                        void
                                        )
{
    return (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID);
}

/*****************************************************************************/
// 	Description : get the invalid text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMITHEME_GetInvalidTextID(void)
{
    return (TXT_NULL);
}

/*****************************************************************************/
// 	Description : check the image id
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_CheckImageID(
                                     MMI_IMAGE_ID_T image_id
                                     )
{
    uint16 label_id = (uint16)(image_id & 0x0000ffff); 

    if ( label_id > NULL)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/*****************************************************************************/
// 	Description : display the background picture for the window
//	Global resource dependence : 
//  Author: Tracy Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DisplayWinBackground(
										  MMI_IMAGE_ID_T	image_id,	//IN:
										  MMI_WIN_ID_T win_id
										  )
{ 
	//GUI_RECT_T	rect = MMITHEME_GetClientRect();
    GUI_POINT_T         dis_point = {0};
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
	if (IMAGE_NULL != image_id)
	{
//         dis_point.x = rect.left;
//         dis_point.y = rect.top;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                image_id,
                &lcd_dev_info);
	}
}

/*****************************************************************************/
// 	Description : get the invalid image id 
//	Global resource dependence : none
//  Author: Tracy Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetInvalidImageID(void)
{
    return (IMAGE_NULL);
}

/*****************************************************************************/
// 	Description : get default lcd device
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC const GUI_LCD_DEV_INFO *MMITHEME_GetDefaultLcdDev(void)
{
	return &s_default_lcd_dev;
}

/*****************************************************************************/
// 	Description : get tv status
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsTVOpen(void)
{
	return MMIAPITVOUT_GetStartingStatus();	
}

/*****************************************************************************/
// 	Description : get tv out rect
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetTVOutShowRect(GUI_RECT_T * show_rect_ptr)
{
	return MMIAPITVOUT_GetTVOutShowRect(show_rect_ptr);	
}

/*****************************************************************************/
// 	Description : get menu status
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsMenuItemGrayed(
                                         MMI_WIN_ID_T           win_id,
										 MMI_MENU_GROUP_ID_T    group_id,
										 MMI_MENU_ID_T          menu_id
                                         )
{
	return MMIAPICOM_IsGrayed(win_id,group_id, menu_id);
}

/*****************************************************************************/
// 	Description : get gray color
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetGrayColor(
                                         void
                                         )
{
	return MMI_DARK_GRAY_COLOR;
}

/*****************************************************************************/
// 	Description : free all menu grayed by window id
//	Global resource dependence : none
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_FreeMenuGrayed(
                                    MMI_HANDLE_T    win_handle
                                    )
{
    MMI_WIN_ID_T    win_id = 0;

    //get window id
    win_id = MMK_GetWinId(win_handle);

    //free menu item grayed
    MMIAPICOM_FreeGrayed(win_id);
}

/*****************************************************************************/
//  Description : 计算字符串中UCS2,default ext,R8单，双字节编码字符的个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CountSpecialChar(
                                      BOOLEAN                   is_sms,         //in:
                                      wchar                     *txt_ptr,       //in:
                                      uint16                    len,            //in:
									  GUIEDIT_SMS_ENCODE_E		sms_encode,		//in:
                                      GUIEDIT_TEXT_SPCHAR_NUM_T *spchar_num_ptr //in/out:
                                      )
{
    uint16                      i = 0;
    GUIEDIT_TEXT_SPCHAR_NUM_T   spchar_num = {0};

    for (i=0; i<len; i++)
    {
        if (MMITHEME_IsUcs2Char(is_sms,txt_ptr[i],sms_encode))
        {
            spchar_num.ucs2++;
        }
#ifdef MMI_R8_SUPPORT
        else if ((is_sms) && (MMITHEME_IsR82ByteChar(txt_ptr[i])))
        {
            spchar_num.r8_2byte++;
        }
        else if ((is_sms) && (MMITHEME_IsR81ByteChar(txt_ptr[i])))
        {
            spchar_num.r8_1byte++;
        }
#endif
        else if (MMITHEME_IsDefaultChar(txt_ptr[i],sms_encode))
        {
            spchar_num.default_ext++;
        }
    }
    
    //set output
    if (PNULL != spchar_num_ptr)
    {
        *spchar_num_ptr = spchar_num;
    }
}

/*****************************************************************************/
//  Description : 计算字符串中Turkish,Spanish,Portuguese特有字符,共有字符个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CountLangChar(
                                   wchar                        *txt_ptr,           //in:
                                   uint16                       len,                //in:
                                   GUIEDIT_TEXT_LANGCHAR_NUM_T  *langchar_num_ptr   //in/out:
                                   )
{
    uint16                      i = 0;
    GUIEDIT_TEXT_LANGCHAR_NUM_T langchar_num = {0};

#ifdef MMI_R8_SUPPORT
    for (i=0; i<len; i++)
    {
        if (MMITHEME_IsR8Turkish(txt_ptr[i]))
        {
            langchar_num.tur++;
        }
        else if (MMITHEME_IsR8Spanish(txt_ptr[i]))
        {
            langchar_num.esp++;
        }
        else if (MMITHEME_IsR8Portuguese(txt_ptr[i]))
        {
            langchar_num.por++;
        }
        else if (MMITHEME_IsSpanishAndTurkish(txt_ptr[i]))
        {
            langchar_num.tur_esp++;
        }
        else if (MMITHEME_IsSpanishAndPortuguese(txt_ptr[i]))
        {
            langchar_num.esp_por++;
        }
    }
#endif
    
    //set output
    if (PNULL != langchar_num_ptr)
    {
        *langchar_num_ptr = langchar_num;
    }
}

/*****************************************************************************/
//  Description : 计算字符串中ucs2字符的个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_CountUcs2Char(
                                     BOOLEAN				is_sms,
                                     uint16					*txt_ptr,
                                     uint16					len,
									 GUIEDIT_SMS_ENCODE_E	sms_encode	//in:
                                     )
{
    uint16      i = 0;
    uint16      ucs2_num = 0;

    for (i=0; i<len; i++)
    {
        if (MMITHEME_IsUcs2Char(is_sms,txt_ptr[i],sms_encode))
        {
            ucs2_num++;
        }
    }

    return (ucs2_num);
}

/*****************************************************************************/
//  Description : 计算字符串中R8双字节编码字符的个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_CountR82ByteChar(
                                        wchar   *txt_ptr, //in:
                                        uint16  len       //in:
                                        )
{
    uint16      i = 0;
    uint16      r8_2byte_num = 0;

#ifdef MMI_R8_SUPPORT
    for (i=0; i<len; i++)
    {
        if (MMITHEME_IsR82ByteChar(txt_ptr[i]))
        {
            r8_2byte_num++;
        }
    }
#endif

    return (r8_2byte_num);
}

/*****************************************************************************/
//  Description : 计算字符串中R8双字节编码字符的个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_CountR81ByteChar(
                                        wchar   *txt_ptr, //in:
                                        uint16  len       //in:
                                        )
{
    uint16      i = 0;
    uint16      r8_1byte_num = 0;

#ifdef MMI_R8_SUPPORT
    for (i=0; i<len; i++)
    {
        if (MMITHEME_IsR81ByteChar(txt_ptr[i]))
        {
            r8_1byte_num++;
        }
    }
#endif

    return (r8_1byte_num);
}

/*****************************************************************************/
//  Description : 获得字符串中最大和最小ucs2的值
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetMaxAndMinUcs2(
                                         wchar      *txt_ptr,       //in:
                                         uint16     len,            //in:
                                         wchar      *max_ucs2_ptr,  //in/out:
                                         wchar      *min_ucs2_ptr   //in/out:
                                         )
{
    BOOLEAN     result = FALSE;
    uint16      i = 0;
    wchar       min_ucs2 = 0xFFFF;
    wchar       max_ucs2 = 0;

    for (i=0; i<len; i++)
    {
        if (MMITHEME_IsUcs2Char(FALSE,txt_ptr[i],GUIEDIT_SMS_ENCODE_AUTO))
        {
            max_ucs2 = (wchar)MAX(max_ucs2,txt_ptr[i]);
            min_ucs2 = (wchar)MIN(min_ucs2,txt_ptr[i]);
        }
    }

    if (0xFFFF != min_ucs2)
    {
        *min_ucs2_ptr = min_ucs2;
        *max_ucs2_ptr = max_ucs2;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 判断是否为 UCS2 字符
//  Global resource dependence : none
//  Author: ZhangJuntao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsUcs2Char(
                                   BOOLEAN				is_sms,		//in:
                                   uint16				ch,			//in:
								   GUIEDIT_SMS_ENCODE_E	sms_encode	//in:
                                   )
{
    BOOLEAN     result = FALSE;

    if (is_sms)
    {
#ifdef MMI_R8_SUPPORT
        if ((MMIAPICOM_IsUcs2Char(ch)) &&
            (!MMITHEME_IsR82ByteChar(ch)) && 
            (!MMITHEME_IsR81ByteChar(ch)) &&
            (!MMIAPICOM_IsDefaultChar(ch)) &&
            (!MMIAPICOM_IsDefaultExtChar(ch)))
        {
            result = TRUE;
        }
#else
        if (GUIEDIT_SMS_ENCODE_AUTO == sms_encode)
        {
            if ((MMIAPICOM_IsUcs2Char(ch)) &&
                (!MMIAPICOM_IsDefaultChar(ch)) &&
                (!MMIAPICOM_IsDefaultExtChar(ch)))
            {
                result = TRUE;
            }
        }
        else if (GUIEDIT_SMS_ENCODE_UCS2 == sms_encode)
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
#endif
    }
    else
    {
        result = MMIAPICOM_IsUcs2Char(ch);
    }

    return (result);
}

/*****************************************************************************/
//  Description : 判断是否为 default ext 字符
//  Global resource dependence : none
//  Author: ZhangJuntao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsDefaultChar(
                                      uint16				ch,			//in:
									  GUIEDIT_SMS_ENCODE_E	sms_encode	//in:
                                      )
{
	BOOLEAN		result = FALSE;
    uint8       temp_char = 0;

    result = MMIAPICOM_WcharAsDefaultExt(ch,&temp_char);
	if ((result) &&
		(GUIEDIT_SMS_ENCODE_8BIT == sms_encode))
	{
		result = FALSE;
	}

	return (result);
}

/*****************************************************************************/
//  Description : 判断是否为 R8 2byte 字符
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR82ByteChar(
                                      wchar     ch
                                      )
{
    BOOLEAN     result = FALSE;

#ifdef MMI_R8_SUPPORT
    if (( ch == 0x00E7 ) ||( ch == 0x00C1 ) || ( ch == 0x00CD ) || 
        ( ch == 0x00D3 ) ||( ch == 0x00DA ) || ( ch == 0x00E1 ) || 
        ( ch == 0x00ED ) ||( ch == 0x00F3 ) || ( ch == 0x00FA ) ||
        ( ch == 0x00EA ) ||( ch == 0x00D4 ) || ( ch == 0x00F4 ) || 
        ( ch == 0x00CA ) ||( ch == 0x00C0 ) || ( ch == 0x00C3 ) ||
        ( ch == 0x00D5 ) ||( ch == 0x00C2 ) || ( ch == 0x00E3 ) ||
        ( ch == 0x00F5 ) ||( ch == 0x00E2 ) || ( ch == 0x001b ) ||
        ( ch == 0x015E ) ||( ch == 0x015F ) || ( ch == 0x0131 ) ||
        ( ch == 0x011E ) ||( ch == 0x011F ) || ( ch == 0x0130 )
        )
    {
        result = TRUE;
    }
#endif

    return (result);
}

/*****************************************************************************/
//  Description : 判断是否为 R8 1byte 字符
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR81ByteChar(
                                      wchar     ch
                                      )
{
    BOOLEAN     result = FALSE;

#ifdef MMI_R8_SUPPORT
    if (( ch == 0x00AA ) || ( ch == 0x221E ) || ( ch == 0x00BA ) || 
        ( ch == 0x0060 ))
    {
        result = TRUE;
    }
#endif 

    return (result);
}

/*****************************************************************************/
// 	Description : Count default extension character
//	Global resource dependence : none
//  Author: Tracy Zhang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_CountDefaultExt(
									   wchar	            *ascii_ptr, //in:
									   uint16	            str_len,    //in:
                                       GUIEDIT_SMS_ENCODE_E	sms_encode	//in:
									   )
{
    uint8       temp_char = 0;
    uint16      i = 0;
    uint16      default_num = 0;

    if (GUIEDIT_SMS_ENCODE_8BIT != sms_encode)
    {
        for (i=0; i<str_len; i++)
        {
            if (MMIAPICOM_WcharAsDefaultExt(ascii_ptr[i],&temp_char))
            {
                default_num++;
            }
        }
    }

    return (default_num);
}

/*****************************************************************************/
//  Description : 判断是否为 土耳其 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR8Turkish(
                                    wchar   ch
                                    )
{
    BOOLEAN is_turkish = FALSE;

#ifdef MMI_R8_SUPPORT
    if((0x015E == ch) || (0x015F == ch) || (0x011E == ch) ||
        (0x011F == ch) || (0x0130 == ch) || (0x0131 == ch))
    {
        is_turkish = TRUE;
    }
#endif
    
    return (is_turkish);
}

/*****************************************************************************/
//  Description : 判断是否为 西班牙 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR8Spanish(
                                    wchar   ch
                                    )
{
    BOOLEAN is_default_spanish = FALSE;

    return (is_default_spanish);
}

/*****************************************************************************/
//  Description : 判断是否为 葡萄牙 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR8Portuguese(
                                       wchar    ch
                                       )
{
    BOOLEAN is_portuguese = FALSE;

#ifdef MMI_R8_SUPPORT
    if((0x00C0 == ch) || (0x00C2 == ch) || (0x00EA == ch) ||
        (0x00D4 == ch) || (0x00F4 == ch) || (0x00CA == ch) ||
        (0x00C3 == ch) || (0x00E3 == ch) || (0x00D5 == ch) ||
        (0x00F5 == ch) || (0x00E2 == ch) || (0x00BA == ch) || 
        (0x221E == ch) || (0x00AA == ch) || (0x0060 == ch))
    {
        is_portuguese = TRUE;
    }
#endif
    
    return (is_portuguese);
}

/*****************************************************************************/
//  Description : 判断是否为土耳其和西班牙共用 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsSpanishAndTurkish(
                                            wchar   ch
                                            )
{
    BOOLEAN is_spanish_or_turkish = FALSE;
    
    return (is_spanish_or_turkish);
}

/*****************************************************************************/
//  Description : 判断是否为葡萄牙和西班牙共用 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsSpanishAndPortuguese(
                                               wchar    ch
                                               )
{
    BOOLEAN is_spanish_or_portuguese = FALSE;

#ifdef MMI_R8_SUPPORT
    if((0x00DA == ch) || (0x00FA == ch) || (0x00CD == ch) ||
        (0x00ED == ch) || (0x00D3 == ch) || (0x00F3 == ch) ||
        (0x00C1 == ch) || (0x00E1 == ch))
    {
        is_spanish_or_portuguese = TRUE;
    }
#endif
    
    return (is_spanish_or_portuguese);
}

/*****************************************************************************/
//  Description : 判断是否为R8 共用 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR8All(
                                wchar   ch
                                )
{
    BOOLEAN is_r8_all = FALSE;

#ifdef MMI_R8_SUPPORT
    if(0x00E7 == ch)
    {
        is_r8_all = TRUE;
    }
#endif
    
    return (is_r8_all);
}

/*****************************************************************************/
//  Description : get pb encode type
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMITHEME_PB_ENCODE_TYPE_E MMITHEME_GetEncodeTypeForPb(
                                                             wchar  max_ucs2,   //in:
                                                             wchar  min_ucs2    //in:
                                                             )
{
    uint8                       base_addr = 0;
    MMITHEME_PB_ENCODE_TYPE_E   encode_type = MMITHEME_PB_ENCODE_NONE;

    if ((0xFFFF == min_ucs2) || (0x7F > max_ucs2))
    {
        encode_type = MMITHEME_PB_ENCODE_NONE;
    }
    else 
    {
        // the range of ucs2, not include default alphabet, is out of "half page"
        if (0X80 <= (max_ucs2 - min_ucs2))
        {
            //80
            encode_type = MMITHEME_PB_ENCODE_80;
        }
        else
        {
            base_addr = (uint8)(min_ucs2 >> 7);
            if ((max_ucs2 >> 7) == base_addr)
            {
                //81[81,基址,len]
                encode_type = MMITHEME_PB_ENCODE_81;
            }
            else
            {
                encode_type = MMITHEME_PB_ENCODE_82;
            }
        }
    }

    return (encode_type);
}

/*****************************************************************************/
//  Description : 根据PB压缩算法，获得可输入的最大长度
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetMaxLenForPb(
                                      uint16                    ascii_max_len,  //in:
                                      uint16                    ucs2_max_len,   //in:
                                      MMITHEME_PB_ENCODE_TYPE_E encode_type     //in:
                                      )
{
    uint16      max_len = 0;

    switch (encode_type)
    {
    case MMITHEME_PB_ENCODE_NONE:
        max_len = ascii_max_len;
        break;

    case MMITHEME_PB_ENCODE_80:
        max_len = ucs2_max_len;
        break;

    case MMITHEME_PB_ENCODE_81:
        if (3 > ascii_max_len)
        {
            max_len = 0;
        }
        else
        {
            max_len = (uint16)(ascii_max_len - 3);
        }
        break;

    case MMITHEME_PB_ENCODE_82:
        if (4 > ascii_max_len)
        {
            max_len = 0;
        }
        else
        {
            //82[82,基址,len]
            max_len = (uint16)(ascii_max_len - 4);
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"MMITHEME_GetMaxLenForPb:encode_type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_THEME_1038_112_2_18_3_1_49_2,(uint8*)"d",encode_type);
        break;
    }

    return (max_len);
}

/*****************************************************************************/
// 	Description : to get the text by label and language
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetLabelTextByLang(
                                        MMI_TEXT_ID_T    label,     //[in] the label 
                                        MMI_STRING_T     *str_ptr   //[out] the data of text
                                        )
{
    MMI_GetLabelTextByLang(label, str_ptr);
}

/*****************************************************************************/
//  Description : get text ptr by text id
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC const wchar* MMITHEME_GetResText(
                                        MMI_TEXT_ID_T   text_id,
                                        CAF_HANDLE_T    handle_id,
                                        MMI_STRING_T    *str_ptr
                                        )  
{
     return (MMIRES_GetText(text_id,handle_id,str_ptr));
}

/*****************************************************************************/
//  Description : switch static theme
//  Global resource dependence : none
//  Author:  Jassmine
//  Note: 横竖屏切换时调用
/*****************************************************************************/
PUBLIC void MMITHEME_SwitchStaticThemeRes(void)
{
    MMIRES_SwitchStaticThemeRes();

    //设置背景没有备份,需要重新备份
    MMITHEME_SetBgLayerChanged();
}

/*****************************************************************************/
// 	Description : Set Display direction
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_SetDisDirection(MMI_DIRECTION_SET_E direction)
{
    s_display_direction = direction;
}

/*****************************************************************************/
// 	Description : Get Display direction
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC MMI_DIRECTION_SET_E MMITHEME_GetDisDirection(void)
{
    return s_display_direction;
}

/*****************************************************************************/
// 	Description : Set is scroll as pixel
//	Global resource dependence : 
//  Author:yongwei.he
//	Note:
//  TRUE: scroll as pixel 
//  FALSE: scroll as character 
/*****************************************************************************/
PUBLIC void MMITHEME_SetIsScrollAsPixel(BOOLEAN is_scroll_pixel)
{
    is_scroll_as_pixel = TRUE;
}

/*****************************************************************************/
// 	Description : Get is scroll as pixel
//	Global resource dependence : 
//  Author:yongwei.he
//	Note:
//  TRUE: scroll as pixel 
//  FALSE: scroll as character 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetIsScrollAsPixel(void)
{
    return is_scroll_as_pixel;
}

/*****************************************************************************/
// 	Description : 获取按像素滚动的像素数
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMITHEME_ScrollPixelNum(void)
{
    return MMITHEME_SCROLL_PIXEL_STEP;
}

/*****************************************************************************/
// 	Description : get default lcd id and block id
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDefaultLcdBlock(
										uint32		*lcd_id, 
										uint32		*block_id
										)
{
    if (PNULL != lcd_id && PNULL != block_id)
    {
    	*lcd_id = GUI_MAIN_LCD_ID;
    	*block_id = GUI_BLOCK_MAIN;
    }
}

/*****************************************************************************/
// 	Description : check lcd id and block id is valid
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_CheckLcdBlockId(
										uint32		lcd_id, 
										uint32		block_id
										)
{
	BOOLEAN		result = TRUE;

	if (lcd_id < GUI_LCD_MAX && block_id < GUI_BLOCK_MAX)
	{
        result = TRUE;
	}
    else
    {
        result = FALSE;

        //SCI_TRACE_LOW:"== MMITHEME_CheckLcdBlockId == lcd id: %d, block id: %d is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_THEME_1166_112_2_18_3_1_49_3,(uint8*)"dd", lcd_id, block_id);
    }

	return result;
}

/*****************************************************************************/
// 	Description : 获取应图标所在区域
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetHardIconRect(void)
{
    GUI_RECT_T rect = {MMI_HW_SHORTCUT_BAR_LEFT, MMI_HW_SHORTCUT_BAR_TOP, MMI_HW_SHORTCUT_BAR_RIGHT, MMI_HW_SHORTCUT_BAR_BOTTOM};

    return rect;
}

/*****************************************************************************/
// 	Description : display bg in rect,title,softkey etc.
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 图片显示区域必须和lcd显示区域相同
/*****************************************************************************/
PUBLIC void MMITHEME_DisplayBg(
                               GUI_RECT_T               *rect_ptr,      //in
                               MMI_WIN_ID_T             win_id,         //in
                               const GUI_LCD_DEV_INFO   *dev_info_ptr   //in:
                               )
{
    if (PNULL == rect_ptr || PNULL == dev_info_ptr)
    {
        return;
    }
    
#ifdef BG_ON_BOTTOM_SUPPORT   
    if (UILAYER_IsMultiLayerEnable())
    {
        //禁止背景层时需要绘制窗口背景
        if ( MMK_GetWinDisplayStyleState( win_id, WS_DISABLE_COMMON_BG ) )
        {
            GUIRES_DisplayImg(PNULL,
                rect_ptr,
                rect_ptr,
                win_id,
                MMK_GetWinBackgroundImageId(win_id),
                dev_info_ptr);
        }
        else
        {
            if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
            {
                UILAYER_ClearRect((GUI_LCD_DEV_INFO *)dev_info_ptr, *rect_ptr);
            }
            else
            {
                UILAYER_BltLowerLayer((GUI_LCD_DEV_INFO *)dev_info_ptr, rect_ptr);
            }           
            MMITHEME_StoreUpdateRect(dev_info_ptr,*rect_ptr);
        }
    }
    else
#endif
    {
        //display bg
        GUIRES_DisplayImg(PNULL,
            rect_ptr,
            rect_ptr,
            win_id,
            IMAGE_THEME_BG,
            dev_info_ptr);
    }
}

/*****************************************************************************/
// 	Description : display common bg in rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 图片显示区域必须和lcd显示区域相同
/*****************************************************************************/
PUBLIC void MMITHEME_DisplayCommonBg(
                                     GUI_RECT_T               *rect_ptr,      //in
                                     MMI_WIN_ID_T             win_id,         //in
                                     const GUI_LCD_DEV_INFO   *dev_info_ptr   //in:
                                     )
{
    if (PNULL != rect_ptr && PNULL != dev_info_ptr)
    {
        //display bg
        GUIRES_DisplayImg(PNULL,
            rect_ptr,
            rect_ptr,
            win_id,
            IMAGE_COMMON_BG,
            dev_info_ptr);
    }
}

/*****************************************************************************/
// 	Description : 返回背景层句柄
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC UILAYER_HANDLE_T MMITHEME_GetBgLayer(void)
{
#ifdef BG_ON_BOTTOM_SUPPORT
    return s_bg_layer_handle;
#else
    return UILAYER_NULL_HANDLE;
#endif
}

/*****************************************************************************/
// 	Description : 返回common背景
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC GUI_BG_T MMITHEME_GetCommonBg(void)
{
    GUI_BG_T    bg_info = {0};

#ifdef MMI_RES_ORIENT_BOTH
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id  = IMAGE_COMMON_BG;  
#elif defined MMI_RES_ORIENT_PORTRAIT
    if (MMITHEME_IsMainScreenLandscape())
    {
        bg_info.bg_type = GUI_BG_COLOR;
#ifdef MONOCHROME_LCD_SUPPORT
        bg_info.color   =  MMI_WHITE_COLOR;
#else        
        bg_info.color   = MMI_WINDOW_BACKGROUND_COLOR;
#endif        
    }
    else
    {
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id  = IMAGE_COMMON_BG;
    }
#elif defined MMI_RES_ORIENT_LANDSCAPE
    if (MMITHEME_IsMainScreenLandscape())
    {
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id  = IMAGE_COMMON_BG;
    }
    else
    {
        bg_info.bg_type = GUI_BG_COLOR;
        bg_info.color   = MMI_WINDOW_BACKGROUND_COLOR;
    }
#else
#error
#endif

    return bg_info;
}

/*****************************************************************************/
// 	Description : 返回Theme背景
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC GUI_BG_T MMITHEME_GetThemeBg(void)
{
    GUI_BG_T bg_info = {0};

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_THEME_BG;

    return bg_info;
}
#if defined (MAINLCD_SIZE_240X320)  || (defined DP_SCREEN_SIZE_240X240)
/*****************************************************************************/
// 	Description : 返回common light背景
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC GUI_BG_T MMITHEME_GetCommonLightBg(void)
{
    GUI_BG_T bg_info = {0};

    bg_info.bg_type = GUI_BG_IMG;
//@for light function modify
#if (defined MAINLCD_SIZE_240X320) && (defined LOW_MEMORY_SUPPORT_256X64 || !defined LOW_MEMORY_SUPPORT)
    bg_info.img_id  = IMAGE_COMMON_BG_LIGHT;
#if defined MMI_PDA_SUPPORT
	bg_info.img_id  = IMAGE_COMMON_BG;
#endif

#else
    bg_info.img_id  = IMAGE_COMMON_BG;
#endif

    return bg_info;
}
/*****************************************************************************/
// 	Description : 返回second menu背景
//	Global resource dependence : 
//  Author:chunjuan.liang
//	Note: 
/*****************************************************************************/
PUBLIC GUI_BG_T MMITHEME_GetSecMenuBg(void)
{
    GUI_BG_T bg_info = {0};

    bg_info.bg_type = GUI_BG_COLOR;
    bg_info.color   = MMI_BLACK_COLOR;

    return bg_info;
}
#endif
/*****************************************************************************/
// 	Description : （如果需要的话）创建并重画背景层
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMITHEME_RedrawBgLayer(void)
{
#ifdef BG_ON_BOTTOM_SUPPORT
    BOOLEAN             result = FALSE;
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    GUI_RECT_T          rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    UILAYER_APPEND_BLT_T append_layer = {0};
    UILAYER_CREATE_T    create_info = {0};

    //display bg
    if (s_is_bglayer_changed)
    {
        if (UILAYER_NULL_HANDLE == s_bg_layer_handle)
        {
            //get lcd width and height
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = VIRTUAL_WIN_ID;
            create_info.offset_x = 0;
            create_info.offset_y = 0;
            create_info.width = lcd_width;
            create_info.height = lcd_height;
            create_info.is_bg_layer = TRUE;
            create_info.is_static_layer = TRUE;
            UILAYER_CreateLayer(
                &create_info,
                &lcd_dev_info
                );
            s_bg_layer_handle = lcd_dev_info.block_id;
        }
        else
        {
            lcd_dev_info.block_id = s_bg_layer_handle;
        }
        if (UILAYER_IsLayerActive(&lcd_dev_info))
        {
            append_layer.lcd_dev_info = lcd_dev_info;
            append_layer.layer_level = UILAYER_LEVEL_LOW;
            UILAYER_AppendBltLayer(&append_layer);
            result = GUIRES_DisplayImg(PNULL,
                &rect,
                &rect,
                VIRTUAL_WIN_ID,
                IMAGE_THEME_BG,
                &lcd_dev_info);
            if (result)
            {
                s_is_bglayer_changed = FALSE;
            }
        }
    }
#endif
}

/*****************************************************************************/
// 	Description : 设置背景层数据发生变化
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMITHEME_SetBgLayerChanged(void)
{
#ifdef BG_ON_BOTTOM_SUPPORT
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, 0};

    if (!s_is_bglayer_changed)
    {
        s_is_bglayer_changed = TRUE;
        lcd_dev_info.block_id = s_bg_layer_handle;
        UILAYER_RELEASELAYER(&lcd_dev_info);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&lcd_dev_info);
        s_bg_layer_handle = UILAYER_NULL_HANDLE;
    }
#endif
}

/*****************************************************************************/
// 	Description : clear rect
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_ClearRect(
                               const GUI_LCD_DEV_INFO   *dev_info_ptr,
                               GUI_RECT_T               *rect_ptr
                               )
{
#ifdef UI_MULTILAYER_SUPPORT
    if ((PNULL != dev_info_ptr) && (PNULL != rect_ptr))
    {
        UILAYER_ClearRect((GUI_LCD_DEV_INFO *)dev_info_ptr, *rect_ptr);
    }
    else
#endif
    {
        //SCI_TRACE_LOW:"PNULL != dev_info_ptr, PNULL != rect_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_THEME_1449_112_2_18_3_1_50_4,(uint8*)"");
    }
}

/*****************************************************************************/
// 	Description : get bg image id
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetBgImgId(void)
{
    MMI_IMAGE_ID_T  bg_id = IMAGE_THEME_BG;

    return (bg_id);
}

/*****************************************************************************/
// 	Description : get font color of cur theme
//	Global resource dependence : 
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetCurThemeFontColor(MMI_THEME_FONT_COLOR_INDEX_E font_color_index)
{
    GUI_COLOR_T                 color = 0;
    
    MMIRES_GetColor(s_theme_font_color_arry[font_color_index], MMK_GetFirstAppletHandle(), &color);
    
    return color;
}

/*****************************************************************************/
// 	Description : trigger the mmi task
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMITHEME_TriggerMMITask(void)
{
    MMIAPICOM_TriggerMMITask();
}

#ifdef MMI_WINIDNAME_SUPPORT
/*****************************************************************************/
//  Description : register the menu group for every module  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetWinIdNameArr(
                                     MMI_WIN_ID_T win_id, 
                                     uint8 **name_arr_ptr
                                     )
{
    MMI_GetWinIdNameArr(win_id, name_arr_ptr);
}

#endif

/******************************************************************************/
/*! \brief get the invalid ctrl id
 *  \author jibin 
 *  \return invalid ctrl id
 */
/******************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetInvalidCtrlId(void)
{
    return MMIAPICOM_GetInvalidCtrlId();
}

/******************************************************************************/
/*! \brief get the invalid window id
 *  \author jibin 
 *  \return invalid window id
 */
/******************************************************************************/
PUBLIC MMI_WIN_ID_T MMITHEME_GetInvalidWinId(void)
{
    return MMIAPICOM_GetInvalidWinId();
}

/*****************************************************************************/
//  Description : g-sensor is support
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsSupportGsensor(void)
{
    BOOLEAN     result = FALSE;

#ifdef MOTION_SENSOR_TYPE
    result = TRUE;
#endif

    return (result);
}

/*****************************************************************************/
//  Description : get owner draw control default height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetOwnDrawDefaultHeight(void)
{
    uint16      height = 0;

    height = GUI_GetFontHeight(MMI_DEFAULT_BIG_FONT,UNICODE_HANZI);

    return (height);
}

/*****************************************************************************/
//  Description : focus win is tp coordinate win
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsFocusTpCoordinateWin(void)
{
#ifdef TOUCH_PANEL_SUPPORT 
    //此接口比需要遍历窗口树的MMK_IsFocusWin效率高
    return MMK_IsCoordinatingTp();
#else
    return TRUE;
#endif
//     BOOLEAN     result = FALSE;
// 
//     if (MMK_IsFocusWin(TP_COORDINATE_WIN_ID))
//     {
//         result = TRUE;
//     }
// 
//     return (result);
}

/*****************************************************************************/
//  Description : hide anim type
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_SetHideAnimType(
                                        MMITHEME_HIDE_ANIM_E type
                                        )
{
    BOOLEAN result = FALSE;

    if ( type < MMITHEME_HIDE_MAX )
    {
        s_hide_anim_type = type;

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get hide anim
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetHideAnim( 
                                 MMITHEME_HIDE_ANIM_T* anim_ptr
                                 )
{    
    if ( PNULL != anim_ptr )
    {
        anim_ptr->type = s_hide_anim_type;
        anim_ptr->frame = 5;
    }
}

/*****************************************************************************/
//  Description : 获得星期的字符串ID
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetWeekdayId( 
                                  MMI_TEXT_ID_T *weekday_id_ptr,
                                  uint16 day_num
                                  )
{    
    uint16 i = 0;
    MMI_TEXT_ID_T calendar_weekday_name[WEEKDAY_NUM] 
        = {TXT_SHORT_SUNDAY, TXT_SHORT_MONDAY, TXT_SHORT_TUESDAY, TXT_SHORT_WEDNESDAY, TXT_SHORT_THURSDAY,TXT_SHORT_FRIDAY, TXT_SHORT_SATURDAY};

    if( WEEKDAY_NUM == day_num && PNULL != weekday_id_ptr )
    {
        for (i = 0; i < day_num; i++)
        {
            weekday_id_ptr[i] = calendar_weekday_name[i];
        }
    }

    return;
}

#ifndef WIN32
/*****************************************************************************/
//  Description : get freg handler
//  Global resource dependence :                          
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetFreqHandler(void)
{
    return MMI_GetFreqHandler();

}
#endif

/*****************************************************************************/
//  Description : tts interface to theme file
//  Global resource dependence : 
//  Author: violent.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMITheme_StopControlText(void)
{
#if defined(MMI_READMENU_ENABLE) || defined(HERO_ENGINE_TTS_SUPPORT)
    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_MENU);
#endif
}

/*****************************************************************************/
//  Description : tts interface to theme file
//  Global resource dependence : 
//  Author: violent.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMITheme_PlayControlText(
                                     MMI_STRING_T* str_ptr, 
                                     uint32 num
                                     )
{
#if defined(MMI_READMENU_ENABLE) || defined(HERO_ENGINE_TTS_SUPPORT)
    MMIAPITTS_PlayTextForControl(str_ptr, num, MMISRVAUD_ROUTE_AUTO);
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: violent.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITheme_IsControlTTSOpen(void)
{
    BOOLEAN is_open = FALSE;
    
#if defined(MMI_READMENU_ENABLE) || defined(HERO_ENGINE_TTS_SUPPORT)
    is_open = TRUE;
#endif
    return is_open;
}

/*****************************************************************************/
//  Description : 根据速度获得加速度
//  Global resource dependence :                          
//  Author: xiaoqing.lu
//  Note: 用户可以修改这个函数，目前的实现是根据速度获取，如果用户觉得不需要这个速度，可以忽略
/*****************************************************************************/
PUBLIC int32 MMITHEME_GetFlingFriction(
                                       float velocity // 速度
                                       )
{
    int32 a = 0;

    if (0 < velocity)
    {
        a = (int32)(2*velocity/3);
    }
    else
    {
        a = (int32)(-2*velocity/3);/*lint !e524*/
    }

    a = MAX(a,FLING_FRICTION); /*lint !e524*/
    
    return a;
}

/*****************************************************************************/
//  Description : 根据速度获得加速度
//  Global resource dependence :                          
//  Author: xiaoqing.lu
//  Note: 用户可以修改这个函数，目前的实现是根据速度获取，如果用户觉得不需要这个速度，可以忽略
/*****************************************************************************/
PUBLIC float MMITHEME_GetFlingFrictionExt(void)
{
#define GRAVITY_EARTH             (9.80665f)// 引力常量
#define SCROLL_FRICTION           (0.015f)  // 默认摩擦系数
#define DENSITY_LOW               120       // 点密集度，low
#define DENSITY_MEDIUM            160       // 点密集度，midium
#define DENSITY_HIGH              240       // 点密集度，high
#define DENSITY_DEFAULT           DENSITY_MEDIUM       // 点密集度，high
#define DENSITY_DEVICE            DENSITY_DEFAULT       // 点密集度，high

    float a = 0;

    a = GRAVITY_EARTH   // g (m/s^2)
        * 39.37f        // inch/meter
        * 160.0f        // pixels per inch
        * SCROLL_FRICTION;

    a = MAX(a,FLING_FRICTION);
    
    return a;
}

/*****************************************************************************/
//  Description : get current trigger mode
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMITHEME_TRIGGER_MODE_E MMITHEME_GetTiggerMode(void)
{
    return (s_trigger_mode);
}

/*****************************************************************************/
//  Description : get current trigger mode
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMITHEME_TRIGGER_MODE_E MMITHEME_GetPreTiggerMode(void)
{
    return (s_pretrigger_mode);
}

/*****************************************************************************/
//  Description : get current trigger mode
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_SetTiggerMode(
                                   MMITHEME_TRIGGER_MODE_E  mode
                                   )
{
#ifdef MMI_PDA_SUPPORT
    s_pretrigger_mode = s_trigger_mode;
    s_trigger_mode = mode;
#endif
}

/*****************************************************************************/
//  Description : get current drag mode
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMITHEME_DRAGMODE_E MMITHEME_GetDragMode(void)
{
    return s_drag_mode;
}

/*****************************************************************************/
//  Description : set current drag mode
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_SetDragMode(
                                 MMITHEME_DRAGMODE_E drag_mode
                                 )
{
    s_drag_mode = drag_mode;
}

/*****************************************************************************/
// 	Description : get default window style
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC WINDOW_DISPLAY_STYLE MMITHEME_GetDefaultWinStyle(void)
{
    WINDOW_DISPLAY_STYLE    win_style = 0;

#ifdef MRAPP_SUPPORT
	if(!MMIMRAPP_IsWindowCreatedBySky())
 	{
    	win_style = WS_HAS_STATUSBAR;
 	}
	else if(MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle() &&
		    !MMIMRAPP_IsWindowOfPB())
	{
		win_style = WS_HAS_BUTTON_SOFTKEY | WS_HAS_SOFTKEY;
	}
#elif defined(HERO_ENGINE_SUPPORT) && !defined(WIN32)
	if(!MMIHEROAPP_IsWindowCreatedBySky())
 	{
    	win_style = WS_HAS_STATUSBAR;
 	}
	else if(MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle() &&
		    !MMIHEROAPP_IsWindowOfPB())
	{
		win_style = WS_HAS_BUTTON_SOFTKEY | WS_HAS_SOFTKEY;
	}

#else
	win_style = WS_HAS_STATUSBAR;
#endif

    return win_style;
}

/*****************************************************************************/
//  Description :Is IStyle
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsIstyle(void)
{
#ifdef MMI_ISTYLE_SUPPORT
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        return TRUE;
    }
#endif
    return FALSE;
}

/*****************************************************************************/
//  Description : is mainlayer type changable
//  Global resource dependence :                     
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsMainLayerTypeChangable(void)
{
#ifdef UILAYER_MAINLAYER_ARGB888_TYPE
    return TRUE;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description : get mainlayer default type
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC DISPLAY_DATA_TYPE_E MMITHEME_GetMainLayerDefaultType(void)
{
    if (MMITHEME_IsMainLayerTypeChangable())
    {
//        return DATA_TYPE_RGB565;
        return DATA_TYPE_ARGB888;
    }
    else
    {
        return DATA_TYPE_RGB565;
    }
}

/*****************************************************************************/
//  Description : restart screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC void MMITHEME_RestartIdleSsTimer(void)
// {
//     MMIDLE_RestartSsTimer(MAIN_IDLE_WIN_ID);
// }

/*****************************************************************************/
//  Description : stop screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC void MMITHEME_StopIdleSsTimer(void)
// {
//     MMIAPIIDLESS_StopTimer();
// }

LOCAL MMISRV_HANDLE_T s_theme_vibrate = 0;
/*****************************************************************************/
//  Description : 震动
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Vibrate(uint32 duaration)
{
#ifndef WIN32
    // 启动震动器
    s_theme_vibrate = MMISRVAUD_StartVibrate(MMISRVAUD_PRI_NORMAL, duaration, 0, 0, PNULL);
#endif
}

/*****************************************************************************/
//  Description : 震动
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_StopVibrate(void)
{
	if (0 != s_theme_vibrate)
	{
	#ifdef VIRTUAL_VIBRATE_FUNC
		MMISRVAUD_StopVibrate();//
	#else
		MMISRVAUD_StopVibrate(s_theme_vibrate);	
	#endif
		s_theme_vibrate = 0;
	}
}

#ifdef GUI_DUMP_CTRL_TREE

/*****************************************************************************/
//  Description : get control guid name
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC char* MMITHEME_DumpCurrentWindow(void)
{
    MMI_HANDLE_T    form_handle[10] = {0};
    uint16          form_handle_count = 0;
    uint16          i = 0;

    // 先打印窗口的控件树
    form_handle_count = MMK_DumpCurrentWindow(form_handle, 10);

    // 再打印一级FORM的控件树
    for (i = 0; i < form_handle_count; i++)
    {
        GUIFORM_DumpTree(form_handle[i]);
    }
}

#endif

/*****************************************************************************/
//  Description : dump info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_PrintInfo(void *exp, void *file, uint32 line)
{
    MMK_PrintInfo();
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_PrintInfo();
#endif
    MMISRVAUD_PrintInfo();
}

