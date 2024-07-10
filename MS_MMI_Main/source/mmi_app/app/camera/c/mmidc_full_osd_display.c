#include "mmi_app_camera_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef CAMERA_SUPPORT
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "block_mem.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "guiblock.h"
#include "mmi_text.h"
#include "mmidc_camera_id.h"
#include "mmidc_display.h"
#include "dal_dcamera.h"
#include "mmi_image.h"
#include "mmidc_save.h"
#include "mmitv_out.h"
#include "guiref_scale.h"
#include "mmidc_option.h"
#include "guilcd.h"
#include "mmidc_setting.h"
#include "mmi_theme.h"
#include "guistring.h"
#include "sci_api.h"
#include "mmi_modu_main.h"
#include "guires.h"
#include "mmifmm_export.h"
#if defined(MMIDC_DESKTOP_ICON_BAR)
#include "mmidc_guiiconbar.h"
#endif
#if !defined(MMIDC_DESKTOP_PDA) && defined(MMIDC_F_WORK_MODE)
#include "mmidc_workmode.h"
#endif

#if defined(MMI_CAMERA_F_PHOTO_DATE)
#include "mmidc_photodate.h"
#endif

#include "mmimultim_image.h"

#if defined(MMIDC_F_N_IN_ONE)
#include "mmidc_ninone.h"
#endif

#if !defined(MMIDC_DESKTOP_PDA)
#include "mmidc_gui.h"
#endif
#include "mmicom_trace.h"

#include "mmi_adapt_screen.h"

#define OSD_STRING_LENGTH 100
#ifdef MMIDC_MINI_RESOURCE_SIZE
#define OSD_IMG_TIP_WIDTH 20
#define OSD_IMG_TIP_HEIGHT 16
#endif

typedef struct
{
    GUIFONT_EFFECT_T				effect;
    GUI_RECT_T   					box;
    uint16       					len;
    uint16       					space;
    uint16							text_ptr[OSD_STRING_LENGTH+1];
    GUI_COLOR_T  					color;
    GUI_FONT_T   					font;
    uint8        					      line_space;
    uint8							start_pos;
    GUI_COLOR_T					background_color;
    GUISTR_STATE_T                        state;
}OSD_STRING;

static uint8						s_need_scroll_number = 0;
static OSD_STRING					s_osd_string[2] = {0};

static GUI_COLOR_T			        s_transparent_color = MMIDC_TRANSPARENT_COLOR;
static int32				        s_middle_string_id = 0;

LOCAL GUI_LCD_DEV_INFO s_main_osd_dev_info = {0};

#if defined MAINLCD_SIZE_128X160
/*****************************************************************************/
// 	Description : display zoom tip rect
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MMIDC_GetTipMaxDisplayRect(void);

/*****************************************************************************/
// 	Description : display current photo name rect
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MMIDC_GetDCFileNameRect(void);
#endif
/*****************************************************************************/
// 	Description : set transparent color
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetTransparentColor(GUI_COLOR_T color)
{
    s_transparent_color = color;
}

/*****************************************************************************/
// 	Description : get transparent color
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIDC_GetTransparentColor(void)
{
    return s_transparent_color;
}

/*****************************************************************************/
// 	Description : clear block buffer in transparent color
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_GUIBLOCK_ClearBlock(GUI_LCD_DEV_INFO* dev_info_ptr, GUI_COLOR_T color)
{
//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    MMIDC_ClearMainOSDLayer();
#else
{
    GUI_RECT_T lcd_rect = {0};
#if 0 //ndef MMIDC_MINI_RESOURCE_SIZE
    GUIBLOCK_ClearByColor(color, dev_info_ptr); //this function will cause defect on 454 lcd screen?
#endif
    lcd_rect = MMIDC_GetFullScreenRect(dev_info_ptr->lcd_id);
    LCD_FillRect(dev_info_ptr, lcd_rect, color);
    MMITHEME_StoreUpdateRect(dev_info_ptr, lcd_rect);
}
#endif
}

/*****************************************************************************/
// 	Description : clear osd buffer in transparent color
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearOSDBlock(void)
{
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    GUI_COLOR_T  color = MMIDC_GetTransparentColor();

    MMIDC_GUIBLOCK_ClearBlock(&dev_info, color);
    //MMIDC_ClearSUBLCD();
}

/*****************************************************************************/
// 	Description : clear main block
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearMainBlock(GUI_COLOR_T color)
{
    GUI_RECT_T       rect = {0};
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};


    rect = MMITHEME_GetFullScreenRect();

    LCD_FillRect((GUI_LCD_DEV_INFO const *)&dev_info, rect, color);
#if defined UI_MULTILAYER_SUPPORT
	MMITHEME_UpdateMultiLayer();
#endif
}
/*****************************************************************************/
// 	Description : complete fill content into the osd buffer befor invalidate
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_CompleteDisplayOSD(void)
{

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_CompleteDisplayOSD"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_143_112_2_18_2_8_38_206,(uint8*)"");

    MMITHEME_UpdateRect();
}

/*****************************************************************************/
// 	Description : display soft key base hor or ver screen
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsMiddleSoftKeyNULL(void)
{
    return (TXT_NULL == s_middle_string_id);
}

/*****************************************************************************/
// 	Description : display soft key base hor or ver screen
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void ShowSoftKey(int32 left_string_id, int32 middle_string_id, int32 right_string_id)
{
    GUI_RECT_T box_right = {0};
    GUI_RECT_T box_left = {0};
    GUI_RECT_T box_middle = {0};

    MMI_STRING_T text_str = {0};
    uint16       text_width = 0;
    uint16       text_height = 0;

    GUI_COLOR_T clear = MMIDC_GetTransparentColor();
    int16 screen_width = MMIDC_GetPreviewWidth();
    int16 screen_height = MMIDC_GetPreviewHeight();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();

    DC_FLOW_MODE_E  mode = MMIDC_GetCurrentMode();

    //SCI_TRACE_LOW:"[MMIDC]: ShowSoftKey layout_style = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_178_112_2_18_2_8_38_207,(uint8*)"d", layout_style);

    s_middle_string_id = middle_string_id;

    //SUB LCD DO NOT DISPLAY
    if (SUB_LCD_ID == MMIDC_GetLcdID())
    {
        return ;
    }

    switch(layout_style)
    {
    case MMIDC_LAYOUT_RIGHTWARD:
        box_left.bottom = screen_height - 1 - 1;
        box_left.right = screen_width - 1 - 1;

        box_right.top = 1;
        box_right.right = screen_width - 1 - 1;

        box_middle.top = box_middle.bottom = screen_height / 2;
        box_middle.right = screen_width - 1 - 1;
        break;
    case MMIDC_LAYOUT_LEFTWARD:
        box_left.top = 1;
        box_left.left = 1;

        box_right.bottom = screen_height - 1 - 1;
        box_right.left = 1;

        box_middle.top = box_middle.bottom = screen_height / 2;
        box_middle.left = 1;
        break;
    case MMIDC_LAYOUT_TRADITION :
    default:
        box_left.bottom = screen_height - 1 - 1;
        box_left.left = 1;

        box_right.bottom = screen_height - 1 - 1;
        box_right.right = screen_width - 1 - 1;

        box_middle.left = box_middle.right = screen_width / 2;
        box_middle.bottom = screen_height - 1 - 1;
        break;
    }

    //right softkey
    if(TXT_NULL != right_string_id)
    {
        MMI_GetLabelTextByLang(right_string_id, &text_str);
        text_width = MMIDC_GetStringWidth(&text_str);
        text_height = MMIDC_GetStringHeight(&text_str);

        if (MMIDC_LAYOUT_LEFTWARD == layout_style)
        {
            box_right.right = box_right.left + text_width - 1;
            box_right.top = box_right.bottom - text_height + 1;
        }
        else if( MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            box_right.left = box_right.right - text_width + 1;
            box_right.bottom = box_right.top + text_height - 1;
        }
        else
        {
            box_right.left = box_right.right - text_width + 1;
            box_right.top = box_right.bottom - text_height + 1;
        }

        if (DV_REVIEW_MODE != mode)
        {
            MMIDC_DisplayBMPMask(box_right.left,box_right.top,box_right);
        }
        MMIDC_DisplayString(box_right, &text_str,clear, FALSE);
    }
    else
    {
        SCI_MEMSET(&box_right, 0, sizeof(GUI_RECT_T));
    }
    MMIDC_SetRightSoftkey(box_right);

    //middle softkey
    if(TXT_NULL != middle_string_id)
    {
        MMI_GetLabelTextByLang(middle_string_id, &text_str);
        text_width = MMIDC_GetStringWidth(&text_str);
        text_height = MMIDC_GetStringHeight(&text_str);

        if (MMIDC_LAYOUT_LEFTWARD == layout_style)
        {
            box_middle.right = box_middle.left + text_width - 1;
            box_middle.top -= (text_height + 1) / 2;
            box_middle.bottom += (text_height + 1) / 2;
        }
        else if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            box_middle.left = box_middle.right - text_width + 1;
            box_middle.top -= (text_height + 1) / 2;
            box_middle.bottom += (text_height + 1) / 2;
        }
        else
        {
            box_middle.left -= (text_width + 1) / 2;
            box_middle.right += (text_width + 1) / 2;
            box_middle.top = box_middle.bottom - text_height + 1;
        }

        if (DV_REVIEW_MODE != mode)
        {
            MMIDC_DisplayBMPMask(box_middle.left,box_middle.top,box_middle);
        }
        MMIDC_DisplayString(box_middle, &text_str,clear, FALSE);
    }
    else
    {
		SCI_MEMSET(&box_middle, 0, sizeof(GUI_RECT_T));
	}
	MMIDC_SetMiddleSoftkey(box_middle);

    //left softkey
    if(TXT_NULL != left_string_id)
    {
        MMI_GetLabelTextByLang(left_string_id, &text_str);
        text_width = MMIDC_GetStringWidth(&text_str);
        text_height = MMIDC_GetStringHeight(&text_str);

        if (MMIDC_LAYOUT_LEFTWARD == layout_style)
        {
            box_left.right = box_left.left + text_width - 1;
            box_left.bottom = box_left.top + text_height - 1;
        }
        else if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            box_left.top = box_left.bottom - text_height + 1;
            box_left.left = box_left.right - text_width + 1;
        }
        else
        {
            box_left.top = box_left.bottom - text_height + 1;
            box_left.right = box_left.left + text_width - 1;
        }

        if (DV_REVIEW_MODE != mode)
        {
            MMIDC_DisplayBMPMask(box_left.left,box_left.top,box_left);
        }
        MMIDC_DisplayString(box_left, &text_str,clear, FALSE);
    }
    else
    {
        SCI_MEMSET(&box_left, 0, sizeof(GUI_RECT_T));
    }
    MMIDC_SetLeftSoftkey(box_left);

}
#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
/*****************************************************************************/
// 	Description : display soft key base hor or ver screen
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void ShowMiddleIconSoftKey(int32 left_string_id, int32 middle_string_id, int32 right_string_id)
{
    GUI_RECT_T box_right = {0};
    GUI_RECT_T box_left = {0};
    GUI_RECT_T box_middle = {0};

    MMI_STRING_T text_str = {0};
    uint16       text_width = 0;
    uint16       text_height = 0;

    GUI_COLOR_T clear = MMIDC_GetTransparentColor();
    int16 screen_width = MMIDC_GetPreviewWidth();
    int16 screen_height = MMIDC_GetPreviewHeight();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();

    DC_FLOW_MODE_E  mode = MMIDC_GetCurrentMode();

	uint16  icon_lef = 0;
	uint16	icon_top = 0;
	uint16 icon_width = 0;
	uint16 icon_height = 0;

	MMI_IMAGE_ID_T  img_id = IMAGE_NULL;
	GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

#if defined MAINLCD_SIZE_128X160
	GUI_RECT_T rect = {0};
#endif

    //SCI_TRACE_LOW:"[MMIDC]: ShowSoftKey layout_style = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_178_112_2_18_2_8_38_207,(uint8*)"d", layout_style);

    s_middle_string_id = middle_string_id;

    //SUB LCD DO NOT DISPLAY
    if (SUB_LCD_ID == MMIDC_GetLcdID())
    {
        return ;
    }

    switch(layout_style)
    {
    case MMIDC_LAYOUT_RIGHTWARD:
        box_left.bottom = screen_height - 1 - 1;
        box_left.right = screen_width - 1 - 1;

        box_right.top = 1;
        box_right.right = screen_width - 1 - 1;

        box_middle.top = box_middle.bottom = screen_height / 2;
        box_middle.right = screen_width - 1 - 1;
        break;
    case MMIDC_LAYOUT_LEFTWARD:
        box_left.top = 1;
        box_left.left = 1;

        box_right.bottom = screen_height - 1 - 1;
        box_right.left = 1;

        box_middle.top = box_middle.bottom = screen_height / 2;
        box_middle.left = 1;
        break;
    case MMIDC_LAYOUT_TRADITION :
    default:
        box_left.bottom = screen_height - 1 - 1;
        box_left.left = 1;

        box_right.bottom = screen_height - 1 - 1;
        box_right.right = screen_width - 1 - 1;

        box_middle.left = box_middle.right = screen_width / 2;
        box_middle.bottom = screen_height - 1 - 1;
        break;
    }

    //right softkey
    if(TXT_NULL != right_string_id)
    {
        MMI_GetLabelTextByLang(right_string_id, &text_str);
        text_width = MMIDC_GetStringWidth(&text_str);
        text_height = MMIDC_GetStringHeight(&text_str);

        if (MMIDC_LAYOUT_LEFTWARD == layout_style)
        {
            box_right.right = box_right.left + text_width - 1;
            box_right.top = box_right.bottom - text_height + 1;
        }
        else if( MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            box_right.left = box_right.right - text_width + 1;
            box_right.bottom = box_right.top + text_height - 1;
#if defined MAINLCD_SIZE_128X160
			if(DC_PREVIEW_MODE == mode || DV_PREVIEW_MODE == mode)
			{
				rect = MMIDC_GetTipMaxDisplayRect();
				if(box_right.left <= rect.right)
				{
					box_right.left = rect.right + MMIDC_TIP_ADJUST_SPACE + 1;
				}
			}
			else if(DC_SAVE_MODE == mode)
			{
				rect = MMIDC_GetDCFileNameRect();
				if(box_right.left <= rect.right)
				{
					box_right.left = rect.right + MMIDC_TIP_ADJUST_SPACE;
				}
			}
#endif
        }
        else
        {
            box_right.left = box_right.right - text_width + 1;
            box_right.top = box_right.bottom - text_height + 1;

			GUIRES_GetImgWidthHeight(&icon_width,&icon_height, IMG_DC_SOFTKEY_CAPTURE, MMIDC_MAIN_WIN_ID);
			if(box_right.left <= box_middle.right + (icon_width + 1) / 2)
			{
				box_right.left = box_middle.right + (icon_width + 1) / 2 + 1 + 1;
			}
        }

        if (DV_REVIEW_MODE != mode)
        {
            MMIDC_DisplayBMPMask(box_right.left,box_right.top,box_right);
        }
		MMIDC_DisplayString(box_right, &text_str,clear, FALSE);
    }
    else
    {
        SCI_MEMSET(&box_right, 0, sizeof(GUI_RECT_T));
    }
    MMIDC_SetRightSoftkey(box_right);

    //middle softkey
	if(TXT_NULL != middle_string_id)
    {
		switch(mode)
		{
		case DC_PREVIEW_MODE:
			img_id = IMG_DC_SOFTKEY_CAPTURE;
			break;

		case DC_SAVE_MODE:
			img_id = IMG_DC_SOFTKEY_SEND;
			break;

		case DV_PREVIEW_MODE:
			img_id = IMG_DC_SOFTKEY_RECORD;
			break;

		case DV_RECORD_MODE:
			img_id = IMG_DC_SOFTKEY_PAUSE;
			break;

		case DV_PAUSE_MODE:
		case DV_REVIEW_MODE:
#ifdef VIDEO_PLAYER_SUPPORT
			img_id = IMG_DC_SOFTKEY_PLAY;
#else
			img_id = IMAGE_NULL;
#endif
			break;

		default:
			img_id = IMAGE_NULL;
			break;
		}
	}
	else
	{
		img_id = IMAGE_NULL;
	}

	GUIRES_GetImgWidthHeight(&icon_width,&icon_height, IMG_DC_SOFTKEY_CAPTURE, MMIDC_MAIN_WIN_ID);

	//横竖屏切换时，采取不同显示位置
	if (MMIDC_LAYOUT_LEFTWARD == layout_style)
    {
		icon_lef = box_middle.left;
		icon_top = screen_height / 2 - (icon_height + 1) / 2;

    }
    else if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
    {
		icon_lef = box_middle.right - icon_width + 1;
		icon_top = screen_height / 2 - (icon_height + 1) / 2;
    }
    else
    {
		icon_lef = screen_width / 2 - (icon_width + 1) / 2;
		icon_top = box_middle.bottom - icon_height + 1;
    }

	MMIDC_DisplayBmp(icon_lef, icon_top, img_id);

	box_middle.left = icon_lef;
	box_middle.right = box_middle.left + icon_width - 1;
	box_middle.top = icon_top;
	box_middle.bottom = box_middle.top + icon_height - 1;
	if(IMAGE_NULL == img_id)
	{
		LCD_FillRect(&dev_info, box_middle, clear);
	}

	MMIDC_SetMiddleSoftkey(box_middle);

	/*从工程模式转横屏，chindwin和softkey可能会重叠*/
#ifdef MMIDC_ADD_SPECIAL_EFFECT
	if(MMK_IsOpenWin(MMIDC_CONTRAST_ADJUST_WIN_ID))
    {
		MMK_SendMsg(MMIDC_CONTRAST_ADJUST_WIN_ID,MSG_FULL_PAINT,PNULL);
	}
	else if(MMK_IsOpenWin(MMIDC_BRIGHTNESS_ADJUST_WIN_ID))
	{
		MMK_SendMsg(MMIDC_BRIGHTNESS_ADJUST_WIN_ID,MSG_FULL_PAINT,PNULL);
	}
#endif

    //left softkey
    if(TXT_NULL != left_string_id)
    {
        MMI_GetLabelTextByLang(left_string_id, &text_str);
        text_width = MMIDC_GetStringWidth(&text_str);
        text_height = MMIDC_GetStringHeight(&text_str);

        if (MMIDC_LAYOUT_LEFTWARD == layout_style)
        {
            box_left.right = box_left.left + text_width - 1;
            box_left.bottom = box_left.top + text_height - 1;
        }
        else if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            box_left.top = box_left.bottom - text_height + 1;
            box_left.left = box_left.right - text_width + 1;
        }
        else
        {
            box_left.top = box_left.bottom - text_height + 1;
            box_left.right = box_left.left + text_width - 1;

			if(box_left.right >= box_middle.left)
			{
				box_left.right = box_middle.left - 1 - 1;
			}
        }

        if (DV_REVIEW_MODE != mode)
        {
            MMIDC_DisplayBMPMask(box_left.left,box_left.top,box_left);
        }
		MMIDC_DisplayString(box_left, &text_str,clear, FALSE);
    }
    else
    {
        SCI_MEMSET(&box_left, 0, sizeof(GUI_RECT_T));
    }
    MMIDC_SetLeftSoftkey(box_left);

}
#endif
/*****************************************************************************/
// 	Description : clear softkey rect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearSoftkeyRect(void)
{
    GUI_RECT_T  rect = {0};
    GUI_COLOR_T color = MMIDC_GetTransparentColor();

    rect = MMIDC_GetLeftSoftkey();
    MMIDC_ClearRect(rect, color);

    rect = MMIDC_GetMiddleSoftkey();
    MMIDC_ClearRect(rect, color);

    rect = MMIDC_GetRightSoftkey();
    MMIDC_ClearRect(rect, color);
}


/*****************************************************************************/
// 	Description : display softkey
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayQuickViewSoftkey()
{
   MMIDC_DisplaySoftKey(DC_REVIEW_MODE);
}
#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
/*****************************************************************************/
// 	Description : display softkey of dv review mode
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShowDVReviewSoftKey()
{
	ShowSoftKey(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL);
}
#endif
/*****************************************************************************/
// 	Description : display softkey
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplaySoftKey(DC_FLOW_MODE_E mode)
{
    if(!MMK_IsFocusWin(MMIDC_MAIN_WIN_ID))
    {
        return;
    }
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_DisplaySoftKey mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_372_112_2_18_2_8_38_208,(uint8*)"d", mode);

#ifdef MMIDC_DESKTOP_PDA

     MMIDC_DisplayAllCmdBtns(mode);
#else

    switch(mode)
    {
    case DC_PREVIEW_MODE:
        if(MMIDC_GetIconHandle()->IconIsOpen())
        {
            ShowSoftKey(STXT_SELECT, TXT_NULL, STXT_RETURN);
        }
#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
        else if(MMIDC_GetMenuHandle()->MenuIsOpen())
        {
            ShowMiddleIconSoftKey(TXT_COMMON_OK,TXT_NULL,STXT_RETURN);
        }
        else
        {
#if defined MMIDC_ADD_SPECIAL_EFFECT
            if(MMK_IsOpenWin(MMIDC_CONTRAST_ADJUST_WIN_ID) || MMK_IsOpenWin(MMIDC_BRIGHTNESS_ADJUST_WIN_ID))
            {
                ShowMiddleIconSoftKey(TXT_COMMON_OK,TXT_NULL,STXT_RETURN);
            }
            else
#endif
            {
                ShowMiddleIconSoftKey(STXT_OPTION,TXT_COMMON_CAPTURE,STXT_RETURN);
            }
        }
#else
        else if(MMIDC_GetMenuHandle()->MenuIsOpen())
        {
			ShowSoftKey(TXT_COMMON_OK,TXT_NULL,STXT_RETURN);
		}
        else
        {
#if defined	MMIDC_ADD_SPECIAL_EFFECT
			if(MMK_IsOpenWin(MMIDC_CONTRAST_ADJUST_WIN_ID) || MMK_IsOpenWin(MMIDC_BRIGHTNESS_ADJUST_WIN_ID))
			{
				ShowSoftKey(TXT_COMMON_OK,TXT_NULL,STXT_RETURN);
			}
			else
#endif
			{
				ShowSoftKey(STXT_OPTION, TXT_COMMON_CAPTURE, STXT_RETURN);
			}
		}
#endif
		break;

	case DC_CAPTURE_CONT_DOWN_MODE:
        ShowSoftKey(TXT_NULL, TXT_NULL, STXT_CANCEL);
        break;

    case DC_CAPTURE_MODE:
    case DC_REVIEW_MODE:
#if defined(MMIDC_F_WORK_MODE)
        if(MMIDC_IsWorkMode())
        {
            ShowSoftKey(TXT_COMMON_OK, STXT_RETURN, STXT_EXIT);
        }
        else
#endif
        {
            ShowSoftKey(STXT_SAVE, TXT_NULL, STXT_RETURN);
        }
        break;

    case DC_SAVE_MODE:
#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
		if(MMIDC_GetMenuHandle()->MenuIsOpen())
        {
			ShowMiddleIconSoftKey(TXT_COMMON_OK,TXT_NULL,STXT_RETURN);
		}
        else
        {
			ShowMiddleIconSoftKey(STXT_OPTION,TXT_SEND,STXT_RETURN);
        }
#else
		if(MMIDC_GetMenuHandle()->MenuIsOpen())
        {
			ShowSoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
		}
        else
        {
			ShowSoftKey(STXT_OPTION, TXT_SEND, STXT_RETURN);
        }
#endif
		break;

    case DV_PREVIEW_MODE:
        if(MMIDC_GetIconHandle()->IconIsOpen())
        {
            ShowSoftKey(STXT_SELECT, TXT_NULL, STXT_RETURN);
        }
#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
		else if(MMIDC_GetMenuHandle()->MenuIsOpen())
        {
			ShowMiddleIconSoftKey(TXT_COMMON_OK,TXT_NULL,STXT_RETURN);
		}
        else
        {
#if defined	MMIDC_ADD_SPECIAL_EFFECT
			if(MMK_IsOpenWin(MMIDC_CONTRAST_ADJUST_WIN_ID) || MMK_IsOpenWin(MMIDC_BRIGHTNESS_ADJUST_WIN_ID))
			{
				ShowMiddleIconSoftKey(TXT_COMMON_OK,TXT_NULL,STXT_RETURN);
			}
			else
#endif
			{
				ShowMiddleIconSoftKey(STXT_OPTION,TXT_DV_SHOOT,STXT_RETURN);
			}
        }
#else
		else if(MMIDC_GetMenuHandle()->MenuIsOpen())
        {
			ShowSoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
		}
        else
        {
#if defined	MMIDC_ADD_SPECIAL_EFFECT
			if(MMK_IsOpenWin(MMIDC_CONTRAST_ADJUST_WIN_ID) || MMK_IsOpenWin(MMIDC_BRIGHTNESS_ADJUST_WIN_ID))
			{
				ShowSoftKey(TXT_COMMON_OK,TXT_NULL,STXT_RETURN);
			}
			else
#endif
			{
				ShowSoftKey(STXT_OPTION, TXT_DV_SHOOT, STXT_RETURN);
			}
        }
#endif
        break;

    case DV_RECORD_MODE:
#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
		ShowMiddleIconSoftKey(STXT_STOP,TXT_PAUSE,STXT_CANCEL);
#else
		ShowSoftKey(STXT_STOP, TXT_PAUSE, STXT_CANCEL);
#endif
        break;

    case DV_PAUSE_MODE:
#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
		ShowMiddleIconSoftKey(STXT_STOP,TXT_COMMON_COMMON_CONTINUE,STXT_CANCEL);
#else
		ShowSoftKey(STXT_STOP, TXT_COMMON_COMMON_CONTINUE, STXT_CANCEL);
#endif
		break;

    case DV_REVIEW_MODE:
#if defined (MMIDC_F_WORK_MODE)
        if(MMIDC_IsWorkMode())
        {
            ShowSoftKey(TXT_COMMON_OK, STXT_RETURN, STXT_EXIT);
        }
        else
#endif
#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
		{
            if(MMIDC_GetMenuHandle()->MenuIsOpen())
            {
                ShowMiddleIconSoftKey(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL);
            }
            else
            {
#ifdef VIDEO_PLAYER_SUPPORT
				ShowMiddleIconSoftKey(STXT_OPTION,TXT_COMMON_PLAY,STXT_RETURN);
#else
                ShowMiddleIconSoftKey(STXT_OPTION,TXT_NULL,STXT_RETURN);
#endif
            }
        }
#else/*MMIDC_MINI_DISPLAY_STYLE*/
		{
            if(MMIDC_GetMenuHandle()->MenuIsOpen())
            {
                ShowSoftKey(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL);
            }
            else
            {
#ifdef VIDEO_PLAYER_SUPPORT
				ShowSoftKey(STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN);
#else
                ShowSoftKey(STXT_OPTION, TXT_NULL, STXT_RETURN);
#endif
            }
        }
#endif/*MMIDC_MINI_DISPLAY_STYLE*/
        break;
    default:
        break;
    }

#endif
}

/*****************************************************************************/
// 	Description : display zoom tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void	MMIDC_DisplayZoomTip(int32 zoom)
{

#if !defined(MMIDC_DESKTOP_PDA)
    MMI_STRING_T str = {0};
    char rec_str[12] = {0};
    wchar w_str[12] = {0};
    GUI_RECT_T rect = {2, 1,0, 20};
	int32  adjust_space = MMIDC_TIP_ADJUST_SPACE; // between icon and 1X

    int32  max_size_text_number = 9;
    wchar size_text[10] = {'8','8','8','8','X','8','8','8','8',0};
    MMI_STRING_T  size_str ={0};
    int32 text_height = 0;
    int32 text_width = 0;
    int32 max_text_width = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;

    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();
    uint16          key_width = MMIDC_GetMaxWidthOfSoftkey();
#if defined MMIDC_MINI_DISPLAY_STYLE
	GUI_RECT_T full_rect = {0};
#endif

    if(zoom > 0)
    {
        //str.wstr_len = sprintf((char*)rec_str, "ZOOM  %d X", (int)zoom);
        sprintf((char*)rec_str, "%dX", (int)zoom);
        if(MMIDC_GetNeedPhotoDisplaySettingTip() == PHOTO_DISPLAY_SETTING_TIP_OFF)
        {
            MMIDC_StartTipDisplayTimer();
        }
    }
    else
    {
        //"8X" occupy space pixels
        sprintf((char*)rec_str, "8X");
    }

    str.wstr_len = strlen(rec_str);
    MMIAPICOM_StrToWstr(rec_str, w_str);/*lint !e64*/
    str.wstr_ptr = w_str;

    /*****get space *****/
    //get icon width and height
    GUIRES_GetImgWidthHeight(&icon_width,&icon_height,IMG_DC_PHOTO_TIP,MMIDC_MAIN_WIN_ID);

    //get max size text rect
    size_str.wstr_ptr = size_text;
    size_str.wstr_len = max_size_text_number;
    max_text_width = MMIDC_GetSpecialStringWidth(&size_str);

    if (MMIDC_LAYOUT_LEFTWARD == layout_style)
    {
        rect.left = key_width + 1;
    }

	/*****从工程模式转横屏，zoom和softkey可能会重叠*****/
#if defined MMIDC_MINI_DISPLAY_STYLE
	if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
	{
		rect.left =rect.left + 2 * MMIDC_TIP_ICON_SPACE + max_text_width + icon_width + adjust_space;
	}
	else
#endif
	{
		rect.left =rect.left + 2 * MMIDC_TIP_ICON_SPACE + max_text_width + 2*icon_width + adjust_space;
	}

    text_width = MMIDC_GetSpecialStringWidth(&str);
    text_height = MMIDC_GetSpecialStringHeight(&str);
    rect.right = rect.left + text_width - 1;
    rect.bottom = rect.top + text_height -1;

#if defined MMIDC_MINI_DISPLAY_STYLE
	full_rect = MMITHEME_GetFullScreenRect();
	if(rect.right > full_rect.right && 1 < full_rect.right)
	{
		rect.right = full_rect.right - 1;
		if(rect.right > text_width)
		{
			rect.left = rect.right - text_width + 1;
		}
	}
#endif

    MMIDC_ClearRect(rect,MMIDC_GetTransparentColor());
    if(zoom > 0)
    {
        MMIDC_DisplayBMPMask(rect.left, rect.top,rect);
        MMIDC_DisplaySpecialString(rect, &str,MMIDC_GetTransparentColor(), FALSE);
    }
#endif

}

#if defined MAINLCD_SIZE_128X160
/*****************************************************************************/
// 	Description : display zoom tip rect
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MMIDC_GetTipMaxDisplayRect(void)
{
    MMI_STRING_T str = {0};
    char rec_str[12] = {0};
    wchar w_str[12] = {0};
    GUI_RECT_T rect = {2, 1,0, 20};
	int32  adjust_space = MMIDC_TIP_ADJUST_SPACE; // between icon and 1X

    int32  max_size_text_number = 9;
    wchar size_text[10] = {'8','8','8','8','X','8','8','8','8',0};
    MMI_STRING_T  size_str ={0};
    int32 text_height = 0;
    int32 text_width = 0;
    int32 max_text_width = 0;
    uint16 icon_width = 0;
    uint16 icon_height = 0;

    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();

    //"8X" occupy space pixels
    sprintf((char*)rec_str, "8X");
    str.wstr_len = strlen(rec_str);
    MMIAPICOM_StrToWstr(rec_str, w_str);/*lint !e64*/
    str.wstr_ptr = w_str;

    /*****get space *****/
    //get icon width and height
    GUIRES_GetImgWidthHeight(&icon_width,&icon_height,IMG_DC_PHOTO_TIP,MMIDC_MAIN_WIN_ID);

    //get max size text rect
    size_str.wstr_ptr = size_text;
    size_str.wstr_len = max_size_text_number;
    max_text_width = MMIDC_GetSpecialStringWidth(&size_str);

	/*****从工程模式转横屏，zoom和softkey可能会重叠*****/
	if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
	{
		rect.left = rect.left + 2 * MMIDC_TIP_ICON_SPACE + max_text_width + icon_width + adjust_space;
		text_width = MMIDC_GetSpecialStringWidth(&str);
		text_height = MMIDC_GetSpecialStringHeight(&str);

		rect.right = rect.left + text_width + 1;
		rect.bottom = rect.top + text_height -1;
	}
	else
	{
		return rect;
	}
	return rect;
}
#endif
/*****************************************************************************/
// 	Description : display video review mode
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayVideoReview(void)
{
    MMIDC_DisplayCurrentFileName();
}

/*****************************************************************************/
// 	Description : display all setting tips in preview mode
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void	MMIDC_DisplaySettingTip(void)
{
#if !defined(MMIDC_DESKTOP_PDA)
#ifdef MMIDC_MINI_RESOURCE_SIZE
	uint16	icon_top = 0;
#else
    uint16	icon_top = 2;
#endif
    uint16  icon_lef = 0;

    MMI_STRING_T text_str = {0};

    MMI_IMAGE_ID_T  img_id = IMAGE_NULL;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    int32  adjust_space = MMIDC_TIP_ICON_SPACE;

    int32  max_size_text_number = 9;
    wchar size_text[10] = {'8','8','8','8','X','8','8','8','8',0};
    MMI_STRING_T  size_str ={0};
    int32 text_height = 0;
    int32 text_width = 0;
    int32 max_text_width = 0;
    int32 max_text_height = 0;

    uint16          screen_width = MMIDC_GetPreviewWidth();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();
    uint16          key_width = MMIDC_GetMaxWidthOfSoftkey();
    int16           max_line_height = 0;

    VIDEO_SIZE_E video_size = 0;
    MMIDC_PHOTO_SIZE_E photo_size = 0;
    MMIDC_DESKTOP_STYLE_E desktop_layout = 0;
    //MMIFILE_DEVICE_E file_dev = 0;

    desktop_layout = MMIDC_GetDesktopStyle();
    if (DESKTOP_STYLE_ICON == desktop_layout)
    {
        return ;
    }


    //SUB LCD DO NOT DISPLAY
    if (SUB_LCD_ID == MMIDC_GetLcdID())
    {
        return ;
    }

//    if(MMIAPIDC_IsPreviewing())
    {
        if(MMIDC_GetCurrentMode() == DC_PREVIEW_MODE || MMIDC_GetCurrentMode() == DC_CAPTURE_CONT_DOWN_MODE)
        {
            uint32 zoom_max = 0;
            if(MMIDC_GetNeedPhotoDisplaySettingTip() == PHOTO_DISPLAY_SETTING_TIP_ON)
            {
                GUI_RECT_T rect1 = {2, 1, 65, 20};

                if (MMIDC_LAYOUT_LEFTWARD == layout_style)
                {
                    rect1.left = key_width + 1;
                }

                //get max size text rect
                size_str.wstr_ptr = size_text;
                size_str.wstr_len = max_size_text_number;
                max_text_width = MMIDC_GetSpecialStringWidth(&size_str);
                max_text_height = MMIDC_GetSpecialStringHeight(&size_str);
#ifndef MMIDC_MINI_RESOURCE_SIZE
                img_id = MMIAPIFMM_GetOSDImageIDByDeviceType(MMIDC_GetPrefPhotoStorageDevice());
                //get icon width and height
                GUIRES_GetImgWidthHeight(&icon_width,&icon_height,img_id,MMIDC_MAIN_WIN_ID);
#else
				icon_width = OSD_IMG_TIP_WIDTH;
				icon_height = OSD_IMG_TIP_HEIGHT;
#endif
                //get line max height
                if (max_text_height > icon_height)
                {
                    max_line_height = max_text_height;
                }
                else
                {
                    max_line_height = icon_height;
                }

                //clear max rect
                if (MMIDC_LAYOUT_RIGHTWARD == layout_style)
                {
                    rect1.right = screen_width - 1 - key_width - 2;
                }
                else
                {
                    rect1.right = screen_width - 1 ;
                }

                rect1.bottom = rect1.top + max_line_height - 1;

                //MMIDC_ClearRect(rect1, MMIDC_GetTransparentColor());
                MMIDC_ClearRect(rect1, MMIDC_GetTransparentColor());

                //size text tip
                photo_size = MMIDC_GetPhotoSize();
                MMIDC_GetSizeString(photo_size, &text_str);
                text_width = MMIDC_GetSpecialStringWidth(&text_str);
                text_height = MMIDC_GetSpecialStringHeight(&text_str);

                rect1.right = rect1.left + text_width - 1;
                rect1.bottom = rect1.top + text_height - 1;
                MMIDC_DisplayBMPMask(rect1.left,rect1.top,rect1);

                MMIDC_DisplaySpecialString(rect1, &text_str,MMIDC_GetTransparentColor(), FALSE);

#ifndef MMIDC_MINI_RESOURCE_SIZE
                //disk tip
                icon_lef = rect1.right + adjust_space +max_text_width - text_width;
                //file_dev = ;

                img_id = MMIAPIFMM_GetOSDImageIDByDeviceType(MMIDC_GetPrefPhotoStorageDevice());

                MMIDC_DisplayBmp(icon_lef, icon_top, img_id);

                //get icon width and height
                GUIRES_GetImgWidthHeight(&icon_width,&icon_height,img_id,MMIDC_MAIN_WIN_ID);

                //multi shot tip
                icon_lef += icon_width + adjust_space;

                if(MMIDC_GetMultiShootEnum() != MULTI_SHOOT_DISABLE)
                {
                    MMIDC_DisplayBmp(icon_lef, icon_top, IMG_DC_MULTISHOT_TIP);
                    img_id = IMG_DC_MULTISHOT_TIP;
                }
                else if(MMIDC_GetShootMode() != SHOOT_MODE_NORMAL)
                {
                    MMIDC_DisplayBmp(icon_lef, icon_top, IMG_DC_PANORAMA_TIP);
                    img_id = IMG_DC_PANORAMA_TIP;
                }
                else
#else
				//photo tip
                icon_lef = rect1.right + adjust_space + max_text_width - text_width;
#endif

                {
                    MMIDC_DisplayBmp(icon_lef, icon_top, IMG_DC_PHOTO_TIP);
                    img_id = IMG_DC_PHOTO_TIP;
                }
                //GUIRES_GetImgWidthHeight(&icon_width,&icon_height,img_id,MMIDC_MAIN_WIN_ID);

                //zoom tip
                zoom_max = MMIDC_GetMaxZoomLevel();
                if (zoom_max > 1 && MMIDC_GetFrameIndex() == FRAME_DISABLE
                    //&& MMIDC_GetMultiShootEnum() == MULTI_SHOOT_DISABLE
                   )
                {
                    MMIDC_DisplayZoomTip(MMIDC_GetPhotoZoomValue() + 1);
                }
                else
                {
                    MMIDC_DisplayZoomTip(INVALID_DATA);
                }
            }
        }
        else if(MMIDC_GetCurrentMode() == DV_PREVIEW_MODE)
        {
			uint32 zoom_max = 0;

			if(MMIDC_GetNeedVideoDisplaySettingTip() == VIDEO_DISPLAY_SETTING_TIP_ON)
            {
                GUI_RECT_T rect2 = {2, 1, 65, 20};

                if (MMIDC_LAYOUT_LEFTWARD == layout_style)
                {
                    rect2.left = key_width + 1;
                }

                //get max size text rect
                size_str.wstr_ptr = size_text;
                size_str.wstr_len = max_size_text_number;
                max_text_width = MMIDC_GetSpecialStringWidth(&size_str);
                max_text_height = MMIDC_GetSpecialStringHeight(&size_str);
#ifndef MMIDC_MINI_RESOURCE_SIZE
                //get icon width and height
                img_id = MMIAPIFMM_GetOSDImageIDByDeviceType(MMIDC_GetPrefVideoStorageDevice());
                GUIRES_GetImgWidthHeight(&icon_width,&icon_height,img_id,MMIDC_MAIN_WIN_ID);
#else
				icon_width = OSD_IMG_TIP_WIDTH;
				icon_height = OSD_IMG_TIP_HEIGHT;
#endif
                //get line max height
                if (max_text_height > icon_height)
                {
                    max_line_height = max_text_height;
                }
                else
                {
                    max_line_height = icon_height;
                }

                //clear max rect
                if (MMIDC_LAYOUT_RIGHTWARD == layout_style)
                {
                    rect2.right = screen_width - 1 - key_width - 2;
                }
                else
                {
                    rect2.right = screen_width - 1 ;
                }
                rect2.bottom = rect2.top + max_line_height - 1;
                MMIDC_ClearRect(rect2, MMIDC_GetTransparentColor());


                //size text tip
                video_size = MMIDC_GetVideoSize();
                MMIDC_GetSizeString(video_size, &text_str);
                text_width = MMIDC_GetSpecialStringWidth(&text_str);
                text_height = MMIDC_GetSpecialStringHeight(&text_str);

                rect2.right = rect2.left + text_width - 1;
                rect2.bottom = rect2.top + text_height - 1;
                MMIDC_DisplayBMPMask(rect2.left,rect2.top,rect2);

                MMIDC_DisplaySpecialString(rect2, &text_str,MMIDC_GetTransparentColor(), FALSE);

#ifndef MMIDC_MINI_RESOURCE_SIZE
                //disk tip
                icon_lef = rect2.right + adjust_space + max_text_width - text_width;

                img_id = MMIAPIFMM_GetOSDImageIDByDeviceType(MMIDC_GetPrefVideoStorageDevice());

                MMIDC_DisplayBmp(icon_lef, icon_top, img_id);

                GUIRES_GetImgWidthHeight(&icon_width,&icon_height,img_id,MMIDC_MAIN_WIN_ID);

                //video tip
                icon_lef += icon_width + adjust_space;

#else
                icon_lef = rect2.right + adjust_space + max_text_width - text_width;
#endif
				MMIDC_DisplayBmp(icon_lef, icon_top, IMG_DC_VIDEO_TIP);
				//zoom tip
#if defined(MMIDC_F_DV_ZOOM)
                zoom_max = MMIDC_GetDVMaxZoomLevel();
				if (zoom_max > 1)
                {
                    MMIDC_DisplayZoomTip(MMIDC_GetDVZoomValue() + 1);
                }
                else
                {
                    MMIDC_DisplayZoomTip(INVALID_DATA);
                }
#endif
            }
        }
    }
#endif
}

/*****************************************************************************/
// 	Description : display count down seconds
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayDelaySecondsTip(uint8 seconds)
{
    MMI_STRING_T str = {0};
    char delay_seconds_arr[12] = {0};
    wchar wstr[20] = {0};

    int32  text_width = 0;
    int32  text_height = 0;

    GUI_RECT_T rect = {0};
    GUI_RECT_T line_rect = {0};
    int16  screen_width = MMIDC_GetPreviewWidth();
    int16  screen_height = MMIDC_GetPreviewHeight();
    GUI_COLOR_T   color = MMIDC_GetTransparentColor();
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();

    GUI_FONT_T  font = 30;

    //get digital str
    str.wstr_ptr = wstr;
    sprintf((char*)delay_seconds_arr, "%d", seconds);
    str.wstr_len = strlen((char*)delay_seconds_arr);
    MMIAPICOM_StrToWstr((uint8 *)delay_seconds_arr, wstr);

    text_width = MMIDC_GetStringWidthByFont(font, &str);
    text_height = MMIDC_GetStringHeightByFont(font, &str);

    rect.top = (screen_height  - text_height )/2;
    rect.left = (screen_width - text_width)/2;
    rect.bottom = rect.top + text_height - 1;
    rect.right = rect.left + text_width - 1;

    line_rect.left = 0;
    line_rect.right = screen_width - 1;
    line_rect.top = rect.top;
    line_rect.bottom = rect.bottom;

    MMIDC_ClearRect(line_rect,color);

    if(MMIDC_GetFrameIndex() != FRAME_DISABLE)
    {
        MMIDC_DisplayFrame();
    }

    MMIDC_DisplayStringByFont(font, rect, &str, color, FALSE);

   MMIDC_DisplaySoftKey(mode);
}

extern PUBLIC uint32 MMIDC_GetRecordSecond(void);

/*****************************************************************************/
// 	Description : display video record tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayVideoRecordTip(DC_FLOW_MODE_E mode)
{
    wchar rec[] = {'R', 'E', 'C', ' ', 0};
    MMI_STRING_T  rec_str ={0};
    char str_buffer_arr[20] = {0};

    uint32 time = 0;
    GUI_RECT_T rect = {0};
    uint16 str_len = 0;
    uint16  red_width=0;
    uint16  red_height = 0;
    MMI_STRING_T  time_str ;
    wchar  time_wstr[12] = {0};

    int16  bmp_space = 2 ;

    wchar show_text[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    MMI_STRING_T show_str = {0};

    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();
    uint16          key_width = MMIDC_GetMaxWidthOfSoftkey();

    int16 screen_width = MMIDC_GetPreviewWidth();
    int16 text_height = 0;

#if defined(MMIDC_DESKTOP_PDA)
    MMI_IMAGE_ID_T img_id = IMG_ICON_RECORD_TIP;
#else
    MMI_IMAGE_ID_T img_id = IMG_DC_RECORD;
#endif

#if(defined MAINLCD_SIZE_240X320)
    int16 left = 1;
#else
    int16 left = 5;
#endif

#if defined(MMIDC_F_DV_ZOOM) && defined(MMIDC_DESKTOP_PDA)
    uint16 zoom_seekbar_width = MMIDC_GetZoomSeekBarWidth();
#endif

#if defined MAINLCD_SIZE_128X160
	if (MMIDC_LAYOUT_RIGHTWARD == layout_style)
    {
		left = 2;
	}
#endif
    //get bmp width and height
    GUIRES_GetImgWidthHeight(&red_width,&red_height,img_id,MMIDC_MAIN_WIN_ID);


    //"REC"
    rec_str.wstr_ptr = rec;
    rec_str.wstr_len = MMIAPICOM_Wstrlen(rec);
    text_height = MMIDC_GetStringHeight(&rec_str);

#if defined(MMIDC_F_DV_ZOOM) && defined(MMIDC_DESKTOP_PDA)
    left += zoom_seekbar_width;
#endif

    rect.top = 3;
    rect.left = left;

    if (MMIDC_LAYOUT_LEFTWARD == layout_style)
    {
        rect.left += key_width;
        rect.right = screen_width - 1;
    }
    else if (MMIDC_LAYOUT_RIGHTWARD == layout_style)
    {
        rect.right = screen_width - key_width - 1;
    }
    else
    {
        rect.right = screen_width - 1;
    }

    if(text_height > red_height)
    {
        rect.bottom = rect.top + text_height - 1;
    }
    else
    {
        rect.bottom = rect.top + red_height - 1;
    }

    //clear red bitmap and max string rect
    MMIDC_ClearRect(rect,MMIDC_GetTransparentColor());


    //time str
    time = MMIDC_GetRecordSecond();
    str_len = sprintf(str_buffer_arr, "%.2lu:%.2lu:%.2lu", time / 60 / 60, time / 60 % 60, time % 60);

    time_str.wstr_ptr = time_wstr;
    time_str.wstr_len = str_len;
    MMIAPICOM_StrToWstr((uint8 *)str_buffer_arr, time_str.wstr_ptr);

    //get show text by rec and time str
    SCI_MEMSET(show_text, 0x00, sizeof(show_text));
    show_str.wstr_ptr = show_text;
    MMI_WSTRNCPY(show_str.wstr_ptr, MMIFILE_FILE_NAME_MAX_LEN,\
                     rec_str.wstr_ptr, rec_str.wstr_len,\
                     rec_str.wstr_len);
    show_str.wstr_len = rec_str.wstr_len;
    MMI_WSTRNCPY(show_str.wstr_ptr + show_str.wstr_len, MMIFILE_FILE_NAME_MAX_LEN,\
                     time_str.wstr_ptr, time_str.wstr_len,\
                     time_str.wstr_len);
    show_str.wstr_len += time_str.wstr_len;


    //display  RED  bitmap
    if((time % 2) == 0)
    {
        if(text_height > red_height)
        {
            MMIDC_DisplayBmp(rect.left, (rect.top + (text_height - red_height)/2), img_id);
        }
        else
        {
            MMIDC_DisplayBmp(rect.left, rect.top, img_id);
        }

    }

    //display REC and times strings
    rect.left += red_width + bmp_space;
#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
	if(red_height > text_height)
    {
        rect.top = rect.top + (red_height - text_height)/2;
    }
#endif
    MMIDC_DisplayString(rect, &show_str, MMIDC_GetTransparentColor(), FALSE);

}

/*****************************************************************************/
// 	Description : display bright tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void	MMIDC_DisplayBrightTip(void)
{
}

/*****************************************************************************/
// 	Description : display photo frame 0
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMIDC_GetFrameDisplayPos(FRAME_E id)
{
    GUI_POINT_T hvga_ver[] = {{FRAME_DISPLAY_POS_HVGA_VER_X_1, FRAME_DISPLAY_POS_HVGA_VER_Y_1},
                              {FRAME_DISPLAY_POS_HVGA_VER_X_2, FRAME_DISPLAY_POS_HVGA_VER_Y_2},
                              {FRAME_DISPLAY_POS_HVGA_VER_X_3, FRAME_DISPLAY_POS_HVGA_VER_Y_3},
                              {FRAME_DISPLAY_POS_HVGA_VER_X_4, FRAME_DISPLAY_POS_HVGA_VER_Y_4}};

    GUI_POINT_T hvga_hor[] = {{FRAME_DISPLAY_POS_HVGA_HOR_X_1, FRAME_DISPLAY_POS_HVGA_HOR_Y_1},
                              {FRAME_DISPLAY_POS_HVGA_HOR_X_2, FRAME_DISPLAY_POS_HVGA_HOR_Y_2},
                              {FRAME_DISPLAY_POS_HVGA_HOR_X_3, FRAME_DISPLAY_POS_HVGA_HOR_Y_3},
                              {FRAME_DISPLAY_POS_HVGA_HOR_X_4, FRAME_DISPLAY_POS_HVGA_HOR_Y_4}};

    GUI_POINT_T qvga_ver[] = {{FRAME_DISPLAY_POS_QVGA_VER_X_1, FRAME_DISPLAY_POS_QVGA_VER_Y_1},
                              {FRAME_DISPLAY_POS_QVGA_VER_X_2, FRAME_DISPLAY_POS_QVGA_VER_Y_2},
                              {FRAME_DISPLAY_POS_QVGA_VER_X_3, FRAME_DISPLAY_POS_QVGA_VER_Y_3},
                              {FRAME_DISPLAY_POS_QVGA_VER_X_4, FRAME_DISPLAY_POS_QVGA_VER_Y_4}};

    GUI_POINT_T qvga_hor[] = {{FRAME_DISPLAY_POS_QVGA_HOR_X_1, FRAME_DISPLAY_POS_QVGA_HOR_Y_1},
                              {FRAME_DISPLAY_POS_QVGA_HOR_X_2, FRAME_DISPLAY_POS_QVGA_HOR_Y_2},
                              {FRAME_DISPLAY_POS_QVGA_HOR_X_3, FRAME_DISPLAY_POS_QVGA_HOR_Y_3},
                              {FRAME_DISPLAY_POS_QVGA_HOR_X_4, FRAME_DISPLAY_POS_QVGA_HOR_Y_4}};

    GUI_POINT_T wqvga_ver[] ={{FRAME_DISPLAY_POS_WQVGA_VER_X_1, FRAME_DISPLAY_POS_WQVGA_VER_Y_1},
                              {FRAME_DISPLAY_POS_WQVGA_VER_X_2, FRAME_DISPLAY_POS_WQVGA_VER_Y_2},
                              {FRAME_DISPLAY_POS_WQVGA_VER_X_3, FRAME_DISPLAY_POS_WQVGA_VER_Y_3},
                              {FRAME_DISPLAY_POS_WQVGA_VER_X_4, FRAME_DISPLAY_POS_WQVGA_VER_Y_4}};

    GUI_POINT_T wqvga_hor[] ={{FRAME_DISPLAY_POS_WQVGA_HOR_X_1, FRAME_DISPLAY_POS_WQVGA_HOR_Y_1},
                              {FRAME_DISPLAY_POS_WQVGA_HOR_X_2, FRAME_DISPLAY_POS_WQVGA_HOR_Y_2},
                              {FRAME_DISPLAY_POS_WQVGA_HOR_X_3, FRAME_DISPLAY_POS_WQVGA_HOR_Y_3},
                              {FRAME_DISPLAY_POS_WQVGA_HOR_X_4, FRAME_DISPLAY_POS_WQVGA_HOR_Y_4}};
    GUI_POINT_T res = {0};

    GUI_POINT_T qcif_ver[] = {{FRAME_DISPLAY_POS_QCIF_VER_X_1, FRAME_DISPLAY_POS_QCIF_VER_Y_1},
                              {FRAME_DISPLAY_POS_QCIF_VER_X_2, FRAME_DISPLAY_POS_QCIF_VER_Y_2},
                              {FRAME_DISPLAY_POS_QCIF_VER_X_3, FRAME_DISPLAY_POS_QCIF_VER_Y_3},
                              {FRAME_DISPLAY_POS_QCIF_VER_X_4, FRAME_DISPLAY_POS_QCIF_VER_Y_4}};

    GUI_POINT_T qcif_hor[] = {{FRAME_DISPLAY_POS_QCIF_HOR_X_1, FRAME_DISPLAY_POS_QCIF_HOR_Y_1},
                              {FRAME_DISPLAY_POS_QCIF_HOR_X_2, FRAME_DISPLAY_POS_QCIF_HOR_Y_2},
                              {FRAME_DISPLAY_POS_QCIF_HOR_X_3, FRAME_DISPLAY_POS_QCIF_HOR_Y_3},
                              {FRAME_DISPLAY_POS_QCIF_HOR_X_4, FRAME_DISPLAY_POS_QCIF_HOR_Y_4}};//for CR NEWMS00110691
    int32 lcd_size = 0;

    if( ((id - 1) < 0) || (id >= (FRAME_E)FRAME_MAX) )
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_GetFrameDisplayPos id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_1014_112_2_18_2_8_40_209,(uint8*)"");

        return res;
    }

    lcd_size = MMIDC_GetLCDSize();
    switch(lcd_size)
    {
    case MMIDC_HVGA_PREVIEW_HEIGHT:
        if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)
        {
            res = hvga_hor[id - 1];
        }
        else
        {
            res = hvga_ver[id - 1];
        }
        break;
    case MMIDC_QVGA_PREVIEW_HEIGHT:
        if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)
        {
            res = qvga_hor[id - 1];
        }
        else
        {
            res = qvga_ver[id - 1];
        }
       break;
    case MMIDC_WQVGA_PREVIEW_HEIGHT:
        if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)
        {
            res = wqvga_hor[id - 1];
        }
        else
        {
            res = wqvga_ver[id - 1];
        }
        break;
    case MMIDC_QCIF_PREVIEW_HEIGHT:
    default:
        if( SCREEN_MODE_HORIIZONTAL == MMIDC_GetScreenMode() )
        {
            res = qcif_hor[id - 1];
        }
        else
        {
            res = qcif_ver[id - 1];
        }
       break;
    }

    return res;
}

/*****************************************************************************/
// 	Description : display photo frame 0
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void DisplayFrame(FRAME_E index)
{
    if (FRAME_DISABLE == index)
    {
        return ;
    }
    else
    {
#ifdef CAMERA_FRAME_SUPPORT
    MMI_IMAGE_ID_T image_id = (MMI_IMAGE_ID_T)(IMAGE_COMMON_FRAME_01 + index - 1);
    GUI_POINT_T point = MMIDC_GetFrameDisplayPos(index);
    uint16      img_width = 0;
    uint16      img_height = 0;
    GUI_RECT_T  rect = {0};

    //get resources width or height
    GUIRES_GetImgWidthHeight(&img_width, &img_height, image_id, MMIDC_MAIN_WIN_ID);
    rect.left = point.x;
    rect.right = rect.left + img_width - 1;
    rect.top = point.y;
    rect.bottom = rect.top + img_height - 1;
    MMIDC_SetFrameRect(rect);

    MMIDC_DisplayBmp(point.x, point.y, image_id);
#endif
    }
}

/*****************************************************************************/
// 	Description : display photo frame 0
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayFrame0(void)
{
    MMIDC_SetFrameIndex(FRAME_DISABLE, TRUE);
}

/*****************************************************************************/
// 	Description : display photo frame 1
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayFrame1(void)
{
    MMIDC_SetFrameIndex(FRAME_1, TRUE);
}

/*****************************************************************************/
// 	Description : display photo frame 2
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayFrame2(void)
{
    MMIDC_SetFrameIndex(FRAME_2, TRUE);
}

/*****************************************************************************/
// 	Description : display photo frame 3
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayFrame3(void)
{
    MMIDC_SetFrameIndex(FRAME_3, TRUE);
}

/*****************************************************************************/
// 	Description : display photo frame 3
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayFrame4(void)
{
    MMIDC_SetFrameIndex(FRAME_4, TRUE);
}

/*****************************************************************************/
// 	Description : display photo frame
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayFrame(void)
{
    if (MMIDC_GetFrameIndex() >= FRAME_MAX)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_DisplayFrame frame index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_1153_112_2_18_2_8_40_210,(uint8*)"");
        return ;
    }
    DisplayFrame(MMIDC_GetFrameIndex());
}

/*****************************************************************************/
// 	Description : clear photo frame rect
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearFrameRect(void)
{
    GUI_RECT_T  rect = {0};
    GUI_COLOR_T color = MMIDC_GetTransparentColor();

    rect = MMIDC_GetFrameRect();
    MMIDC_ClearRect(rect, color);
}

#if defined(MMIDC_DESKTOP_PDA)
/*****************************************************************************/
//Description : display current photo name
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void PDA_DisplayCurrentFileName(void)
{

    MMI_STRING_T str = {0};/*lint !e830*/
    GUI_RECT_T rect = {0};
    int16  text_width = 0;
    int16  key_width = MMIDC_GetMaxWidthOfSoftkey();
    int16  item_height = MMIDC_ITEM_TEXT_HEIGHT;
    int16  screen_width = MMIDC_GetPreviewWidth();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();

    GUI_RECT_T line_rect = {0};
    int16 line_space = 0;

    uint16 img_width = 0;
    uint16 img_height = 0;
    MMI_IMAGE_ID_T img_id = IMG_BTN_ARROW_LEFT_FOCUS;

    SCREEN_MODE_E screen_mode = MMIDC_GetScreenMode();
    int16 arrow_left_x = 0;

    if(MMIDC_GetCurrentMode() != DC_SAVE_MODE && MMIDC_GetCurrentMode() != DV_REVIEW_MODE)
    {
        return;
    }

    if (SCREEN_MODE_VERTICAL == screen_mode)
    {
        arrow_left_x = BTN_ARROW_LEFT_V_X;
    }
    else
    {
        arrow_left_x = BTN_ARROW_LEFT_H_X;
    }

    GUIRES_GetImgWidthHeight(&img_width, &img_height,img_id, MMIDC_MAIN_WIN_ID);

    if (1 < MMIDC_GetCapturedPhotosNumber())
    {
        line_space = screen_width - arrow_left_x - key_width;
    }

    //review in full osd
    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {
        str.wstr_ptr = MMIDC_GetPhotoName(MMIDC_GetCurrentPhotoID());
        str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);

        text_width = MMIDC_GetStringWidth(&str);
        if (MMIDC_LAYOUT_LEFTWARD == layout_style)
        {
            rect.left = key_width + (screen_width - key_width - text_width )/ 2 ;
            line_rect.left = key_width + 1;
            line_rect.right = screen_width - 1 - line_space;
        }
        else if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            rect.left = (screen_width - key_width - text_width )/ 2 ;
            line_rect.left = 0;
            line_rect.right = screen_width - key_width - 1 - line_space;
        }
        else
        {
            rect.left = (screen_width - text_width) / 2;
            line_rect.left = 0;
            line_rect.right = screen_width - 1;
        }
        if(rect.left < 0)
        {
            rect.left = 0;
        }
        rect.right = rect.left + text_width - 1;
        rect.top = 2;
        rect.bottom = rect.top + item_height -1;

        line_rect.top = rect.top;
        line_rect.bottom = rect.bottom;
        MMIDC_ClearRect(line_rect, MMIDC_GetTransparentColor());
        MMIDC_DisplayString(rect, &str, MMIDC_GetTransparentColor(), FALSE);
        MMIDC_SetMultiPhotoRect(rect);
    }
    else   // dv
    {
        if (PNULL == MMIDC_GetVideoName())
        {
            //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_DisplayCurrentFileName NAME = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_1289_112_2_18_2_8_40_211,(uint8*)"");
            return ;
        }

        str.wstr_ptr = MMIDC_GetVideoName();
        str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);

        text_width = MMIDC_GetStringWidth(&str);

        if (MMIDC_LAYOUT_LEFTWARD == layout_style)
        {
            rect.left = key_width + (screen_width - key_width - text_width )/ 2 ;
        }
        else if (MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            rect.left = (screen_width - key_width - text_width )/ 2 ;
        }
        else
        {
            rect.left = (screen_width - text_width) / 2;
        }

        if(rect.left < 0)
        {
            rect.left = 0;
        }
        rect.right = rect.left + text_width ;
        rect.top = 2;
        rect.bottom = rect.top + item_height -1;

        MMIDC_DisplayString( rect, &str,MMIDC_GetTransparentColor(), FALSE);
    }
}
#endif

/*****************************************************************************/
// 	Description : display current photo name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayCurrentFileName(void)
{
#if defined(MMIDC_DESKTOP_PDA)
    PDA_DisplayCurrentFileName();
#else

    MMI_STRING_T str = {0};/*lint !e830*/
    wchar *file_name_ptr = PNULL;
    int16 file_name_len = 0;
    wchar wstr[DC_SHOW_TIME_LENGTH] = {0};
    GUI_RECT_T rect = {0};
    int16  text_width = 0;
    int16  key_width = 0;
    int16  item_height = MMIDC_ITEM_TEXT_HEIGHT;
    int16  screen_width = MMIDC_GetPreviewWidth();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();

    GUI_RECT_T line_rect = {0};


    if(MMIDC_GetCurrentMode() != DC_SAVE_MODE && MMIDC_GetCurrentMode() != DV_REVIEW_MODE)
    {
        return;
    }

    //review in full osd
    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {
        if(MMIDC_GetCapturedPhotosNumber() == 1)
        {
            str.wstr_ptr = MMIDC_GetPhotoName(MMIDC_GetCurrentPhotoID());
            str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
        }
        else
        {
            str.wstr_ptr = wstr;
            MMIAPICOM_Wstrncpy(str.wstr_ptr, L"<", 1);
            file_name_ptr = MMIDC_GetPhotoName(MMIDC_GetCurrentPhotoID());
            file_name_len = MMIAPICOM_Wstrlen(file_name_ptr);
            MMIAPICOM_Wstrncat(str.wstr_ptr, file_name_ptr, file_name_len),
            MMIAPICOM_Wstrncat(str.wstr_ptr, L">", 1);
            str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
        }

        text_width = MMIDC_GetStringWidth(&str);
        if (MMIDC_LAYOUT_LEFTWARD == layout_style)
        {
            key_width = MMIDC_GetMaxWidthOfSoftkey();
            rect.left = key_width + (screen_width - key_width - text_width )/ 2 ;
            line_rect.left = key_width + 1;
            line_rect.right = screen_width - 1;
        }
        else if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            key_width = MMIDC_GetMaxWidthOfSoftkey();
            rect.left = (screen_width - key_width - text_width )/ 2 ;
            line_rect.left = 0;
            line_rect.right = screen_width - key_width - 1;
        }
        else
        {
            rect.left = (screen_width - text_width) / 2;
            line_rect.left = 0;
            line_rect.right = screen_width - 1;
        }

        if(rect.left < 0)
        {
            rect.left = 0;
        }
        rect.right = rect.left + text_width - 1;
        rect.top = 2;
        rect.bottom = rect.top + item_height -1;

        line_rect.top = rect.top;
        line_rect.bottom = rect.bottom;
        MMIDC_ClearRect(line_rect, MMIDC_GetTransparentColor());
        MMIDC_DisplayString(rect, &str, MMIDC_GetTransparentColor(), FALSE);
        MMIDC_SetMultiPhotoRect(rect);
    }
    else   // dv
    {
        if (PNULL == MMIDC_GetVideoName())
        {
            //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_DisplayCurrentFileName NAME = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_1412_112_2_18_2_8_40_212,(uint8*)"");
            return ;
        }

        str.wstr_ptr = MMIDC_GetVideoName();
        str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);

        text_width = MMIDC_GetStringWidth(&str);

        if (MMIDC_LAYOUT_LEFTWARD == layout_style)
        {
            key_width = MMIDC_GetMaxWidthOfSoftkey();
            rect.left = key_width + (screen_width - key_width - text_width )/ 2 ;
        }
        else if (MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            key_width = MMIDC_GetMaxWidthOfSoftkey();
            rect.left = (screen_width - key_width - text_width )/ 2 ;
        }
        else
        {
            rect.left = (screen_width - text_width) / 2;
        }

        if(rect.left < 0)
        {
            rect.left = 0;
        }
        rect.right = rect.left + text_width ;
        rect.top = 2;
        rect.bottom = rect.top + item_height -1;

        MMIDC_DisplayString( rect, &str,MMIDC_GetTransparentColor(), FALSE);
    }

#endif
}

#if defined MAINLCD_SIZE_128X160
/*****************************************************************************/
// 	Description : display current photo name rect
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
LOCAL GUI_RECT_T MMIDC_GetDCFileNameRect(void)
{
    MMI_STRING_T str = {0};
    wchar *file_name_ptr = PNULL;
    int16 file_name_len = 0;
    wchar wstr[DC_SHOW_TIME_LENGTH] = {0};
    GUI_RECT_T rect = {0};
    int16  text_width = 0;
    int16  key_width = 0;
    int16  item_height = MMIDC_ITEM_TEXT_HEIGHT;
    int16  screen_width = MMIDC_GetPreviewWidth();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();

    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {
        if(MMIDC_GetCapturedPhotosNumber() == 1)
        {
            str.wstr_ptr = MMIDC_GetPhotoName(MMIDC_GetCurrentPhotoID());
            str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
        }
        else
        {
            str.wstr_ptr = wstr;
            MMIAPICOM_Wstrncpy(str.wstr_ptr, L"<", 1);
            file_name_ptr = MMIDC_GetPhotoName(MMIDC_GetCurrentPhotoID());
            file_name_len = MMIAPICOM_Wstrlen(file_name_ptr);
            MMIAPICOM_Wstrncat(str.wstr_ptr, file_name_ptr, file_name_len),
				MMIAPICOM_Wstrncat(str.wstr_ptr, L">", 1);
            str.wstr_len = MMIAPICOM_Wstrlen(str.wstr_ptr);
        }

        text_width = MMIDC_GetStringWidth(&str);
		if(MMIDC_LAYOUT_RIGHTWARD == layout_style)
        {
            key_width = MMIDC_GetMaxWidthOfSoftkey();
            rect.left = (screen_width - key_width - text_width )/ 2 ;
        }

        if(rect.left < 0)
        {
            rect.left = 0;
        }

        rect.right = rect.left + text_width + 1;
        rect.top = 2;
        rect.bottom = rect.top + item_height -1;
    }

	return rect;
}
#endif
/*****************************************************************************/
// 	Description : get osd block information
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMIDC_GetOSDBlockInfo(void)
{
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

//    #if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    dev_info = MMIDC_GetMainOSDLayerDevInfo();
#endif

    return dev_info;
}

/*****************************************************************************/
// 	Description : reset need scroll number
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetScrollNumber(void)
{
    s_need_scroll_number = 0;
}

/*****************************************************************************/
// 	Description : display string scroll if the length is too long
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//   当区域中显示不下时，会产生滚动，
//   原始的内容会被添加焦点外框，
/*****************************************************************************/
PUBLIC void MMIDC_StringStyleDisplay(MMIDC_STRING_STYLE_T *string,BOOLEAN    is_auto_scroll )
{
    uint16								text_width = 0;
    GUI_LCD_DEV_INFO					dev_info_ptr = MMIDC_GetOSDBlockInfo();
    BOOLEAN  is_need_scroll=FALSE;
    wchar  show_text[OSD_STRING_LENGTH+1] ={0};
    int16    show_number=0;
    wchar   dot_text[] ={'.','.','.',0};


    MMI_STRING_T text_str = {0};
    GUISTR_STYLE_T      text_style = {0};/*lint !e64*/
    GUISTR_STATE_T      state =0;
    GUI_RECT_T                  box ={0};
    GUIFONT_EFFECT_T		effect = 0;
    wchar                  *text_ptr = PNULL; //string to display
    uint16       			len = 0;		//length of string to display
    uint16       			space = DP2PX_VALUE(1); 	    //space between two chars.
    uint16        			line_space = DP2PX_VALUE(1);  //space between 2 lines
    GUI_COLOR_T  			color = 0;    //string color
    GUI_FONT_T   			font = SONG_FONT_0; /*lint !e64*/		//font pointer
    GUI_COLOR_T			background_color = 0;
    GUI_ALIGN_E                 align = 0;/*lint !e64*/


    if(PNULL != string->text_ptr)
    {
        text_ptr = string->text_ptr->wstr_ptr;
        len = string->text_ptr->wstr_len;
    }

    if(text_ptr != PNULL && len > 0)
    {
        box = string->rect ;
        effect = string->text_style.effect;
        space = string->text_style.char_space;
        line_space = string->text_style.line_space;
        color = string->text_style.font_color;
        font = string->text_style.font;
        background_color = string->text_style.bg_color;
        align = string->text_style.align;

        SCI_MEMCPY(&text_style,&string->text_style,sizeof(text_style));
        state = string->state;

        text_str.wstr_ptr = text_ptr;
        text_str.wstr_len = len;
        text_width = GUISTR_GetStringWidth(&text_style,&text_str,state);//GUI_CalculateStringPiexlNum(text_ptr, len, font, space);
        if(text_width > (box.right - box.left + 1) && is_auto_scroll)
        {
            if(s_need_scroll_number >= 2)
            {
                s_need_scroll_number = 0;
            }
            SCI_MEMSET(&s_osd_string[s_need_scroll_number], 0x00, sizeof(s_osd_string[0]));
            s_osd_string[s_need_scroll_number].effect = effect;
            SCI_MEMCPY(&(s_osd_string[s_need_scroll_number].box), &box, sizeof(GUI_RECT_T));
            len = MIN(OSD_STRING_LENGTH, len);
            MMIAPICOM_Wstrncpy(s_osd_string[s_need_scroll_number].text_ptr, text_ptr, len);
            s_osd_string[s_need_scroll_number].len = len;
            s_osd_string[s_need_scroll_number].space = space;
            s_osd_string[s_need_scroll_number].color = color;
            s_osd_string[s_need_scroll_number].font = font;
            s_osd_string[s_need_scroll_number].line_space = line_space;
            s_osd_string[s_need_scroll_number].background_color = background_color;
            s_osd_string[s_need_scroll_number].state = state;
            MMIDC_StartTextScrollTimer();
            s_osd_string[s_need_scroll_number].start_pos = 0;
            s_need_scroll_number ++;
        }

        if(text_width > (box.right - box.left + 1))
        {
            is_need_scroll = TRUE;
        }

        //for multi language  displaying from right
        show_number=len ;

        if(is_need_scroll)
        {
            while(show_number>0)
            {
                text_str.wstr_len = show_number;
                text_width=GUISTR_GetStringWidth(&text_style,&text_str,state);
                if(text_width <=(box.right - box.left + 1))
                {
                    break;
                }
                show_number--;
            }
        }

        //SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayString1 text_widht=%d,show_number=%d",text_width,show_number);
        //SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayString1 show rect=%d",box.right - box.left);


        SCI_MEMSET(show_text,0x00,sizeof(show_text));
        MMIAPICOM_Wstrncpy(show_text,text_ptr,show_number);

        //显示不下的内容，并且不是焦点的显示
       //最后显示3个点
       if(is_need_scroll && !is_auto_scroll && (show_number >=5))
       {
            MMIAPICOM_Wstrncpy((show_text + show_number -3),dot_text,3);
       }

        //get show string  value
        text_str.wstr_len = show_number;
        text_str.wstr_ptr = show_text;

        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
            (const GUI_RECT_T      *)&box,       //the fixed display area
            (const GUI_RECT_T      *)&box,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&text_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );


    }
    else
    {
        uint16 i = 0;

        for(i = 0; i < s_need_scroll_number; i ++)
        {
            OSD_STRING* temp = &(s_osd_string[i]);

            if(temp->len <= temp->start_pos)
            {
                temp->start_pos = 0;
            }

            // text style
            text_style.align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
            text_style.char_space = temp->space;
            text_style.line_space = temp->line_space;
            text_style.effect = temp->effect;
            text_style.font = temp->font;
            text_style.font_color = temp->color;
            text_style.bg_color = temp->background_color;
            text_style.region_num = 0;
            text_style.region_ptr = PNULL;
#if defined MMIDC_MINI_RESOURCE_SIZE
			text_style.edge_color = MMI_BACKGROUND_COLOR;
#endif
            state = temp->state;

            LCD_FillRect(&dev_info_ptr, temp->box, temp->background_color);

            text_str.wstr_ptr = temp->text_ptr + temp->start_pos;
            text_str.wstr_len = temp->len - temp->start_pos;


            show_number=temp->len - temp->start_pos;

            while(show_number>0)
            {
                text_str.wstr_len = show_number;
                text_width = GUISTR_GetStringWidth(&text_style,&text_str,state);;
                if(text_width <(temp->box.right - temp->box.left))
                {
                    break;
                }
                show_number--;
            }

            //SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayString text_widht=%d,show_number=%d",text_width,show_number);
            //SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayString show rect=%d",temp->box.right - temp->box.left);


            SCI_MEMSET(show_text,0x00,sizeof(show_text));
            MMIAPICOM_Wstrncpy(show_text,temp->text_ptr + temp->start_pos,show_number);

            //added, @robert.wang,  09-9-7, cr150160
            //get show string  value
            text_str.wstr_len = show_number;
            text_str.wstr_ptr = show_text;

            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
                (const GUI_RECT_T      *)&temp->box,       //the fixed display area
                (const GUI_RECT_T      *)&temp->box,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&text_str,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );

            temp->start_pos += 1;

            MMIDC_StartTextScrollTimer();
        }
    }
}

/*****************************************************************************/
// 	Description : display bmp
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayBmp(int16 x, int16 y, MMI_IMAGE_ID_T	image_id)
{
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    dev_info_ptr = MMIDC_GetOSDBlockInfo();


    dis_point.x = x;
    dis_point.y = y;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        MMIDC_MAIN_WIN_ID,
        image_id,
        &dev_info_ptr);

}

/*****************************************************************************/
// 	Description : display background bmp by picture
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayBackgroundBMP(uint16 x, uint16 y, GUI_RECT_T rect,MMI_IMAGE_ID_T image_id)
{
    GUI_LCD_DEV_INFO    dev_info_ptr = MMIDC_GetOSDBlockInfo();

    GUIRES_DisplayImg(PNULL,
        &rect,
        PNULL,
        MMIDC_MAIN_WIN_ID,
        image_id,
        &dev_info_ptr);
}

/*****************************************************************************/
// 	Description : display string background bmp mask by picture
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayBMPMask(uint16 x, uint16 y, GUI_RECT_T rect)
{
#if !defined(MMI_PDA_SUPPORT)
	GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    GUI_COLOR_T      transparent_color = MMIDC_GetTransparentColor();
    GUI_COLOR_T      opacus_color = 0;
    GUI_COLOR_T      color = 0;

    int16   i = 0;
    int16   j = 0;
    int16   row_type = 0;
    int16   column_type = 0;

    uint16      lcd_width = 0;
    uint16      lcd_height = 0;
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    GUI_COLOR_T *buffer_ptr = PNULL;
    GUI_RECT_T  main_rect = {0};
    GUI_RECT_T  dst_rect = {0};

    lcd_width = MMIDC_GetPreviewWidth();
    lcd_height = MMIDC_GetPreviewHeight();

    main_rect.right = lcd_width - 1;
    main_rect.bottom = lcd_height - 1;

    if (GUI_IntersectRect(&dst_rect, main_rect, rect))
    {
#if defined VECTOR_FONT_SUPPORT_ETRUMP
		opacus_color = transparent_color + 128;
#else
		opacus_color = transparent_color + 32;
#endif
#ifdef UI_MULTILAYER_SUPPORT
        block_buffer_ptr = (GUI_COLOR_T *)UILAYER_GetLayerBufferPtr((GUI_LCD_DEV_INFO *)&dev_info);
#else
		if(0 == dev_info.block_id)
		{
			block_buffer_ptr = (GUI_COLOR_T *)GUILCD_GetMainLcdBufPtr();
		}
#endif
		if(PNULL == block_buffer_ptr)
		{
			return;
		}

        for (j = dst_rect.top; j <= dst_rect.bottom; j++)
        {
            //odd
            column_type = j % 2;

            for (i = dst_rect.left; i <= dst_rect.right ; i++)
            {
                //odd
                row_type = i % 2;

                if (0 == column_type)
                {
                    if (0 == row_type)
                    {
                        color = opacus_color;
                    }
                    else
                    {
                        color = transparent_color;
                    }
                }
                else
                {
                    if (0 == row_type)
                    {
                        color = transparent_color;
                    }
                    else
                    {
                        color = opacus_color;
                    }
                }

                buffer_ptr = block_buffer_ptr + j * lcd_width + i;
                *buffer_ptr = color;
            }
        }
    }
    // MMIDC_DisplayBackgroundBMP(x,y,rect,IMG_DC_BMP_MASK);
#endif
}

/*****************************************************************************/
// 	Description : display title background bmp by picture
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayTitleBackground(uint16 x, uint16 y, GUI_RECT_T rect)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE
#if !defined(MMIDC_DESKTOP_PDA)
    MMIDC_DisplayBackgroundBMP(x,y,rect,IMG_DC_TITLE_BG);
#endif
#endif
}

/*****************************************************************************/
//Description : get string default style
//Global resource dependence : none
// Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetStringDefaultStyle(GUISTR_STYLE_T *style //out
                                     )
{
    style->align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
    style->char_space = DP2PX_VALUE(1);
    style->line_space = DP2PX_VALUE(1);
    style->effect = FONT_EFFECT_CUSTOM;
    style->font = MMIDC_TEXT_FONT;
    style->font_color = MMIDC_TEXT_COLOR;
    style->edge_color = MMI_BACKGROUND_COLOR;
    style->bg_color = MMI_BACKGROUND_COLOR;
    style->region_num = 0;
    style->region_ptr = PNULL;
}

/*****************************************************************************/
// 	Description : get normal text string height pixels as normal text font
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetStringHeight(MMI_STRING_T *string)
{
// #if defined MMIDC_MINI_DISPLAY_STYLE
#ifndef MMIDC_DESKTOP_PDA
		GUISTR_STYLE_T style = {0};
		GUISTR_INFO_T str_info = {0};
		GUISTR_STATE_T state = MMIDC_GetOSDFontState();

		GetStringDefaultStyle(&style);
		GUISTR_GetStringInfo(&style, string, state, &str_info);

		return str_info.height;
#else
	return (GUI_GetStringHeight(MMIDC_TEXT_FONT,string->wstr_ptr,string->wstr_len));
#endif
}

/*****************************************************************************/
// 	Description : get normal text string width pixels as normal text font
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetStringWidth(MMI_STRING_T  *string)
{
    GUISTR_STYLE_T style_ptr = {0};/*lint !e64*/
    GUISTR_STATE_T state = MMIDC_GetOSDFontState();

    // text style
    style_ptr.align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
    style_ptr.char_space = DP2PX_VALUE(1);
    style_ptr.line_space = DP2PX_VALUE(1);
    style_ptr.effect = FONT_EFFECT_CUSTOM;
    style_ptr.font = MMIDC_TEXT_FONT;
    style_ptr.font_color = MMIDC_TEXT_COLOR;
    style_ptr.edge_color = MMI_BACKGROUND_COLOR; //MMI_BACKGROUND_COLOR;
    style_ptr.bg_color = MMI_BACKGROUND_COLOR;
    style_ptr.region_num = 0;
    style_ptr.region_ptr = PNULL;

    return (GUISTR_GetStringWidth(&style_ptr, string,state));
}

/*****************************************************************************/
// 	Description : get special text string width pixels as special text font
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetSpecialStringWidth(MMI_STRING_T  *string)
{
    GUISTR_STYLE_T style_ptr = {0};/*lint !e64*/
    GUISTR_STATE_T state = MMIDC_GetOSDFontState();

    // text style
    style_ptr.align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
    style_ptr.char_space = DP2PX_VALUE(1);
    style_ptr.line_space = DP2PX_VALUE(1);
    style_ptr.effect = FONT_EFFECT_CUSTOM;
    style_ptr.font = MMIDC_TIP_FONT;
    style_ptr.font_color = MMIDC_TEXT_COLOR;
    style_ptr.edge_color = MMI_BACKGROUND_COLOR; //MMI_BACKGROUND_COLOR;
    style_ptr.bg_color = MMI_BACKGROUND_COLOR;
    style_ptr.region_num = 0;
    style_ptr.region_ptr = PNULL;

    return (GUISTR_GetStringWidth(&style_ptr, string,state));
}

/*****************************************************************************/
// 	Description : get special text string height pixels as special text font
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetSpecialStringHeight(MMI_STRING_T *string)
{
    return (GUI_GetStringHeight(MMIDC_TIP_FONT,string->wstr_ptr,string->wstr_len));
}

/*****************************************************************************/
// 	Description : get special text string display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplaySpecialString(GUI_RECT_T  rect,
                                                        MMI_STRING_T  *text_ptr,
                                                        GUI_COLOR_T bg_color,
                                                        BOOLEAN is_auto_scroll)
{
    MMIDC_STRING_STYLE_T    string = {0};

    string.rect = rect;
    string.text_ptr = text_ptr;
    string.state = MMIDC_GetOSDFontState();

    // text style
    string.text_style.align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
    string.text_style.char_space = DP2PX_VALUE(1);
    string.text_style.line_space = DP2PX_VALUE(1);
    string.text_style.effect = FONT_EFFECT_CUSTOM;
    string.text_style.font = MMIDC_TIP_FONT;
    string.text_style.font_color = MMIDC_TEXT_COLOR;
#if defined VECTOR_FONT_SUPPORT_ETRUMP
	string.text_style.edge_alpha = 0xff;
	string.text_style.edge_color = MMIDC_CHILDWIN_CTRL_BACKGROUND;
#else
	string.text_style.edge_color = MMI_BACKGROUND_COLOR; //MMI_BACKGROUND_COLOR;
#endif
    string.text_style.bg_color = bg_color;
    string.text_style.region_num = 0;
    string.text_style.region_ptr = PNULL;


    MMIDC_StringStyleDisplay(&string,is_auto_scroll);
}

/*****************************************************************************/
// 	Description : get item text string display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayString(GUI_RECT_T  rect,
                                                        MMI_STRING_T  *text_ptr,
                                                        GUI_COLOR_T bg_color,
                                                        BOOLEAN is_auto_scroll)
{
    MMIDC_STRING_STYLE_T    string ={0};

    string.rect = rect;
    string.text_ptr = text_ptr;
    string.state = MMIDC_GetOSDFontState();

    // text style
#if !defined(MMIDC_DESKTOP_PDA)
	if(MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection()
		&& MMIDC_GetMenuHandle()->MenuIsOpen())
	{
		string.text_style.align = ALIGN_RIGHT;
	}
	else
#endif
	{
		string.text_style.align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
	}
	string.text_style.char_space = DP2PX_VALUE(1);
    string.text_style.line_space = DP2PX_VALUE(1);
    string.text_style.effect = FONT_EFFECT_CUSTOM;
    string.text_style.font = MMIDC_TEXT_FONT;
    string.text_style.font_color = MMIDC_TEXT_COLOR;
#if defined VECTOR_FONT_SUPPORT_ETRUMP
	string.text_style.edge_alpha = 0xff;
	string.text_style.edge_color = MMIDC_CHILDWIN_CTRL_BACKGROUND;
#else
	string.text_style.edge_color = MMI_BACKGROUND_COLOR; //MMI_BACKGROUND_COLOR;
#endif
	string.text_style.bg_color = bg_color;
    string.text_style.region_num = 0;
    string.text_style.region_ptr = PNULL;


    MMIDC_StringStyleDisplay(&string,is_auto_scroll);
}

/*****************************************************************************/
// 	Description : get gray item text string display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayGrayString(GUI_RECT_T  rect,
                                                        MMI_STRING_T  *text_ptr,
                                                        GUI_COLOR_T bg_color,
                                                        BOOLEAN is_auto_scroll)
{
    MMIDC_STRING_STYLE_T    string ={0};

    string.rect = rect;
    string.text_ptr = text_ptr;
    string.state = MMIDC_GetOSDFontState();

    // text style
    string.text_style.align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
    string.text_style.char_space = DP2PX_VALUE(1);
    string.text_style.line_space = DP2PX_VALUE(1);
    string.text_style.effect = FONT_EFFECT_CUSTOM;
    string.text_style.font = MMIDC_TEXT_FONT;
    string.text_style.font_color = MMI_DARK_GRAY_COLOR;
#if defined VECTOR_FONT_SUPPORT_ETRUMP
	string.text_style.edge_alpha = 0xff;
	string.text_style.edge_color = MMIDC_CHILDWIN_CTRL_BACKGROUND;
#else
	string.text_style.edge_color = MMI_BACKGROUND_COLOR; //MMI_BACKGROUND_COLOR;
#endif
    string.text_style.bg_color = bg_color;
    string.text_style.region_num = 0;
    string.text_style.region_ptr = PNULL;


    MMIDC_StringStyleDisplay(&string,is_auto_scroll);
}

/*****************************************************************************/
// 	Description : get title text string display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayTitleString(GUI_RECT_T  rect,MMI_STRING_T  *text_ptr,GUI_COLOR_T bg_color,BOOLEAN is_auto_scroll)
{
    MMIDC_STRING_STYLE_T    string = {0};

    string.rect = rect;
    string.text_ptr = text_ptr;
    string.state = MMIDC_GetOSDFontState();

    // text style
    string.text_style.align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
    string.text_style.char_space = DP2PX_VALUE(1);
    string.text_style.line_space = DP2PX_VALUE(1);
    string.text_style.effect = FONT_EFFECT_NONE;
    string.text_style.font = MMIDC_TEXT_FONT;
    string.text_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    //string.text_style.edge_color = edge_color; //MMI_BACKGROUND_COLOR;
    string.text_style.bg_color = bg_color;
    string.text_style.region_num = 0;
    string.text_style.region_ptr = PNULL;


    MMIDC_StringStyleDisplay(&string,is_auto_scroll);
}

/*****************************************************************************/
//Description : get normal text string height pixels by font
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetStringHeightByFont(GUI_FONT_T font,
                                                    MMI_STRING_T *string //in
                                                    )
{
    return (GUI_GetStringHeight(font,string->wstr_ptr,string->wstr_len));
}

/*****************************************************************************/
//Description : get normal text string width pixels by font
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetStringWidthByFont(GUI_FONT_T font,
                                                    MMI_STRING_T  *string //in
                                                    )
{
    GUISTR_STYLE_T style = {0};/*lint !e64*/
    GUISTR_STATE_T state = MMIDC_GetOSDFontState();

    GetStringDefaultStyle(&style);
    style.font = font;

    return (GUISTR_GetStringWidth(&style, string,state));
}

/*****************************************************************************/
//Description : get item text string display by font
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayStringByFont(GUI_FONT_T font,
                                               GUI_RECT_T  rect,
                                               MMI_STRING_T  *text_ptr,
                                               GUI_COLOR_T bg_color,
                                               BOOLEAN is_auto_scroll
                                               )
{
    MMIDC_STRING_STYLE_T    string ={0};

    string.rect = rect;
    string.text_ptr = text_ptr;
    string.state = MMIDC_GetOSDFontState();

    GetStringDefaultStyle(&string.text_style);

    // text style
    string.text_style.font = font;
    string.text_style.bg_color = bg_color;

    MMIDC_StringStyleDisplay(&string,is_auto_scroll);
}
/*****************************************************************************/
// 	Description : clear rect by  color
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearRect(GUI_RECT_T  rect,GUI_COLOR_T  color)
{
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();

//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    UILAYER_ClearRect(&dev_info, rect);
#else
    LCD_FillRect((GUI_LCD_DEV_INFO const *)&dev_info, rect, color);

#endif
}

/*****************************************************************************/
// 	Description : get osd font state
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
//  Param:
//      Return: GUISTR_STATE_T
/*****************************************************************************/
PUBLIC GUISTR_STATE_T MMIDC_GetOSDFontState(void)
{
    GUISTR_STATE_T   state = 0;

    state = GUISTR_STATE_EFFECT | GUISTR_STATE_NOT_HALF_CHAR | GUISTR_STATE_SINGLE_LINE;

    #if defined(MMIDC_BOLD_FONT) || defined(FONT_TYPE_SUPPORT_CSTAR_BITMAP)
    state |=GUISTR_STATE_BOLD;
    #endif

    return state;
}

/*****************************************************************************/
//  Description : clear sublcd screen rect by  color
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearSUBLCD(void)
{
#ifdef SUBLCD_SIZE
    GUI_LCD_DEV_INFO dev_info = {GUI_SUB_LCD_ID, GUI_BLOCK_MAIN};
    GUI_COLOR_T  color = MMIDC_GetTransparentColor();

    MMIDC_GUIBLOCK_ClearBlock(&dev_info, color);
#endif
}

PUBLIC GUI_RECT_T MMIDC_GetFullScreenRect(GUI_LCD_ID_E lcd_id)
{
    GUI_RECT_T  rect = {0};
    uint16      logic_width = 0;
    uint16      logic_height = 0;

    GUILCD_GetLogicWidthHeight(lcd_id, &logic_width, &logic_height);
    rect.right = logic_width - 1;
    rect.bottom = logic_height - 1;

    return rect;
}

/*****************************************************************************/
//  Description : clean all menu rect
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_CleanAllMenuRect(void)
{
#if !defined(MMIDC_DESKTOP_PDA)
    MMIDC_ClearAdjustRect();
    MMIDC_GetMenuHandle()->MenuClearRect();
    MMIDC_GetIconHandle()->IconClearRect();

#if defined(MMIDC_DESKTOP_ICON_BAR)
    MMIDC_ClearIconDesktopRectList();
#endif

#endif

    return 0;
}


/*****************************************************************************/
//Description :  reset all ctrls dev info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetAllCtrlsDevInfo(void)
{
    MMI_WIN_ID_T win_id = MMIDC_MAIN_WIN_ID;
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    uint32 i = 0;

    for (i = DC_CTRL_ID_ID_START+1; i < DC_MAX_CTRL_ID; i++)
    {
        if (PNULL != MMK_GetCtrlHandleByWin(win_id, i))
        {
           MMK_SetCtrlLcdDevInfo(i, &dev_info);
        }
    }
}


/*****************************************************************************/
//Description : create main osd layer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreateMainOSDLayer(void)
{
    BOOLEAN result = FALSE;

//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16) //there is no pda any more.
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    int16   lcd_width = 0;
    int16   lcd_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO dev_info = {0};
    UILAYER_RESULT_E ret = 0;
    UILAYER_APPEND_BLT_T append_layer = {0};

    dev_info = MMIDC_GetMainOSDLayerDevInfo();

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CreateMainOSDLayer block_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3834_112_2_18_2_8_36_175,(uint8*)"d",dev_info.block_id);

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&dev_info))
    {
        lcd_width = MMIDC_GetPreviewWidth();
        lcd_height = MMIDC_GetPreviewHeight();

        //SCI_TRACE_LOW:"[MMIDC] MMIDC_CreateMainOSDLayer width=%d, height=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3843_112_2_18_2_8_36_176,(uint8*)"dd",lcd_width, lcd_height);

         //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = MMIDC_MAIN_WIN_ID;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = lcd_width;
        create_info.height = lcd_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = TRUE;
        ret = UILAYER_CreateLayer(
            &create_info,
            &dev_info
            );

        if (UILAYER_RESULT_SUCCESS == ret)
        {
            result = TRUE;

            UILAYER_RemoveMainLayer();

            UILAYER_EnableOsdLayer(TRUE);

            s_main_osd_dev_info = dev_info;

            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            append_layer.lcd_dev_info = dev_info;
            UILAYER_AppendBltLayer(&append_layer);

            #ifdef PLATFORM_ANTISW3
            //开启ARGB888 Mode
            LCD_SetARGB888Mode(TRUE);
            #endif
        }
        else
        {
            SCI_TRACE_LOW("MMIDC_CreateMainOSDLayer:fail to create uilayer. ret:%d",ret);
        }

    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CreateMainOSDLayer result=%d,dev_info.block_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3878_112_2_18_2_8_36_177,(uint8*)"dd",result, dev_info.block_id);
#else
    result = TRUE;
#endif
   return result;
}

/*****************************************************************************/
//Description : destroy main osd layer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroyMainOSDLayer(void)
{
//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16) //there is no pda any more.
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    GUI_LCD_DEV_INFO dev_info = {0};

    dev_info = MMIDC_GetMainOSDLayerDevInfo();
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&dev_info))
    {
        UILAYER_RELEASELAYER(&dev_info);    /*lint !e506 !e774*/
//      UILAYER_ReleaseLayer(&dev_info);

        dev_info.block_id = 0;
        s_main_osd_dev_info = dev_info;
    }

    UILAYER_EnableOsdLayer(FALSE);
    UILAYER_RestoreMainLayer();
    #ifdef PLATFORM_ANTISW3
    //??ARGB888??????
    LCD_SetARGB888Mode(FALSE);
    #endif
#endif
}

/*****************************************************************************/
//Description : get main osd layer device info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO MMIDC_GetMainOSDLayerDevInfo(void)
{
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16) //there is no pda any more.
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_main_osd_dev_info))
    {
        dev_info = s_main_osd_dev_info;
    }
#endif
    return dev_info;
}

/*****************************************************************************/
//Description : clear main osd layer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearMainOSDLayer(void)
{
//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16) //there is no pda any more.
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    GUI_LCD_DEV_INFO dev_info = {0};

    dev_info = MMIDC_GetMainOSDLayerDevInfo();

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ClearMainOSDLayer block_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3939_112_2_18_2_8_36_178,(uint8*)"d",dev_info.block_id);


    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&dev_info))
    {
        UILAYER_Clear(&dev_info);
    }
#endif

}


/*****************************************************************************/
//Description : recreate main osd layer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_ReCreateMainOSDLayer(void)
{
    BOOLEAN result = FALSE;
    BOOLEAN ret = FALSE;

    ret = MMIDC_CreateMainOSDLayer();
    if (ret)
    {
        result = TRUE;
        MMIDC_ResetAllCtrlsDevInfo();
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ReCreateMainOSDLayer ret=%d, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4069_112_2_18_2_8_36_182,(uint8*)"dd",ret, result);

    return result;
}


#ifdef DC_WATCH_UI_SUPPORT
//watch face ui functions//////////////////////////////////////////////////////////////////////////////////
LOCAL DC_ICON_INFO_T s_icon_table[ICON_MAX]={

#ifdef ADULT_WATCH_SUPPORT
   //image id              |           rect: L   T     R   B
    {IMG_DC_WATCH_OPEN_GALLERY, DP2PX_RECT( 48, 176,  79, 207)}, //ICON_OPEN_GALLERY,
    {IMG_DC_WATCH_CAPTURE_IMAGE,DP2PX_RECT( 96, 168, 143, 215)}, //ICON_CAPTURE_IMAGE,
    {IMG_DC_WATCH_START_RECORD, DP2PX_RECT( 96, 168, 143, 215)},`//ICON_RECORD_START,
    {IMG_DC_WATCH_STOP_RECORD,  DP2PX_RECT( 96, 168, 143, 215)}, //ICON_RECORD_STOP,
    {IMG_DC_WATCH_SWITCH_TO_DV, DP2PX_RECT(160, 176, 191, 207)}, //ICON_SWITCH_DV,
    {IMG_DC_WATCH_SWITCH_TO_DC, DP2PX_RECT(160, 176, 191, 207)}, //ICON_SWITCH_DC,
    {IMG_DC_WATCH_PLAY,         DP2PX_RECT( 96,  84, 143, 131)}, //ICON_DV_PLAY,
    {IMG_DC_WATCH_PAUSE_PLAY,   DP2PX_RECT( 96,  84, 143, 131)}, //ICON_DV_PAUSE,
    {IMG_DC_WATCH_CANCEL,       DP2PX_RECT( 60, 168,  107, 215)}, //ICON_DC_CANCEL,
    {IMG_DC_WATCH_CONFIRM,      DP2PX_RECT(132, 168, 179, 215)}, //ICON_DC_CONFIRM,
    {IMG_DC_WATCH_CANCEL,       DP2PX_RECT( 60, 168,  107, 215)}, //ICON_DV_CANCEL,
    {IMG_DC_WATCH_CONFIRM,      DP2PX_RECT(132, 168, 179, 215)}, //ICON_DV_CONFIRM,
//    {IMG_DC_WATCH_WAIT,         DP2PX_RECT( 96,  84, 143, 131)} //ICON_WAITING, not used now
#else
#if defined (SCREEN_SHAPE_CIRCULAR)  //Child  watch.  W217
   //image id              |           rect: L   T     R   B
   #if defined(MAINLCD_DEV_SIZE_240X284)// wuxx add for CAMERA BF20A6,GC6133, 240X284 预览全屏, and isp_service.a
   {IMG_DC_WATCH_OPEN_GALLERY, DP2PX_RECT( 40, 208,  87, 255)}, //ICON_OPEN_GALLERY,
   {IMG_DC_WATCH_CAPTURE_IMAGE,DP2PX_RECT( 96, 208, 143, 255)}, //ICON_CAPTURE_IMAGE,
   {IMG_DC_WATCH_START_RECORD, DP2PX_RECT( 96, 208, 143, 255)}, //ICON_RECORD_START,
   {IMG_DC_WATCH_STOP_RECORD,  DP2PX_RECT( 96, 208, 143, 255)}, //ICON_RECORD_STOP,
   {IMG_DC_WATCH_SWITCH_TO_DV, DP2PX_RECT(152, 208, 199, 255)}, //ICON_SWITCH_DV,
   {IMG_DC_WATCH_SWITCH_TO_DC, DP2PX_RECT(152, 208, 199, 255)}, //ICON_SWITCH_DC,
   {IMG_DC_WATCH_PLAY,         DP2PX_RECT( 96,  124, 143, 171)}, //ICON_DV_PLAY,
   {IMG_DC_WATCH_PAUSE_PLAY,   DP2PX_RECT( 96,  124, 143, 171)}, //ICON_DV_PAUSE,
   {IMG_DC_WATCH_CANCEL,       DP2PX_RECT( 60, 208,  107, 255)}, //ICON_DC_CANCEL,
   {IMG_DC_WATCH_CONFIRM_GRAY,      DP2PX_RECT(132, 208, 179, 255)}, //ICON_DC_CONFIRM,
   {IMG_DC_WATCH_CANCEL,       DP2PX_RECT( 60, 208,  107, 255)}, //ICON_DV_CANCEL,
   {IMG_DC_WATCH_CONFIRM_GRAY,      DP2PX_RECT(132, 208, 179, 255)}, //ICON_DV_CONFIRM,
//    {IMG_DC_WATCH_WAIT,         DP2PX_RECT( 96,  84, 143, 131)} //ICON_WAITING, not used now
   #else
    {IMG_DC_WATCH_OPEN_GALLERY, DP2PX_RECT( 40, 168,  87, 215)}, //ICON_OPEN_GALLERY,
    {IMG_DC_WATCH_CAPTURE_IMAGE,DP2PX_RECT( 96, 168, 143, 215)}, //ICON_CAPTURE_IMAGE,
    {IMG_DC_WATCH_START_RECORD, DP2PX_RECT( 96, 168, 143, 215)}, //ICON_RECORD_START,
    {IMG_DC_WATCH_STOP_RECORD,  DP2PX_RECT( 96, 168, 143, 215)}, //ICON_RECORD_STOP,
    {IMG_DC_WATCH_SWITCH_TO_DV, DP2PX_RECT(152, 168, 199, 215)}, //ICON_SWITCH_DV,
    {IMG_DC_WATCH_SWITCH_TO_DC, DP2PX_RECT(152, 168, 199, 215)}, //ICON_SWITCH_DC,
    {IMG_DC_WATCH_PLAY,         DP2PX_RECT( 96,  84, 143, 131)}, //ICON_DV_PLAY,
    {IMG_DC_WATCH_PAUSE_PLAY,   DP2PX_RECT( 96,  84, 143, 131)}, //ICON_DV_PAUSE,
    {IMG_DC_WATCH_CANCEL,       DP2PX_RECT( 60, 168,  107, 215)}, //ICON_DC_CANCEL,
    {IMG_DC_WATCH_CONFIRM_GRAY,      DP2PX_RECT(132, 168, 179, 215)}, //ICON_DC_CONFIRM,
    {IMG_DC_WATCH_CANCEL,       DP2PX_RECT( 60, 168,  107, 215)}, //ICON_DV_CANCEL,
    {IMG_DC_WATCH_CONFIRM_GRAY,      DP2PX_RECT(132, 168, 179, 215)}, //ICON_DV_CONFIRM,
//    {IMG_DC_WATCH_WAIT,         DP2PX_RECT( 96,  84, 143, 131)} //ICON_WAITING, not used now
    #endif

#else  //SCREEN_SHAPE_RECTANGLE
   //image id        |       rect: L   T     R   B
    {IMG_DC_WATCH_OPEN_GALLERY, DP2PX_RECT( 21, 173,  68, 220)}, //ICON_OPEN_GALLERY,
    {IMG_DC_WATCH_CAPTURE_IMAGE,DP2PX_RECT( 89, 165, 152, 228)}, //ICON_CAPTURE_IMAGE,
    {IMG_DC_WATCH_START_RECORD, DP2PX_RECT( 89, 165, 152, 228)}, //ICON_RECORD_START,
    {IMG_DC_WATCH_STOP_RECORD,  DP2PX_RECT( 89, 165, 152, 228)}, //ICON_RECORD_STOP,
    {IMG_DC_WATCH_SWITCH_TO_DV, DP2PX_RECT(173, 173, 220, 220)}, //ICON_SWITCH_DV,
    {IMG_DC_WATCH_SWITCH_TO_DC, DP2PX_RECT(173, 173, 220, 220)}, //ICON_SWITCH_DC,
    {IMG_DC_WATCH_PLAY,         DP2PX_RECT( 89,  65, 152, 128)}, //ICON_DV_PLAY,
    {IMG_DC_WATCH_PAUSE_PLAY,   DP2PX_RECT( 89,  65, 152, 128)}, //ICON_DV_PAUSE,
    {IMG_DC_WATCH_CANCEL,       DP2PX_RECT( 33, 165,  96, 228)}, //ICON_DC_CANCEL,
    {IMG_DC_WATCH_CONFIRM,      DP2PX_RECT(145, 165, 208, 228)}, //ICON_DC_CONFIRM,
    {IMG_DC_WATCH_CANCEL,       DP2PX_RECT( 33, 165,  96, 228)}, //ICON_DV_CANCEL,
    {IMG_DC_WATCH_CONFIRM,      DP2PX_RECT(145, 165, 208, 228)}, //ICON_DV_CONFIRM,
//    {IMG_DC_WATCH_WAIT,         DP2PX_RECT( 89,  89, 152, 152)} //ICON_WAITING,

#endif
#endif
};

DC_OSD_LAYOUT_T s_osd_layout_list[DC_FLOW_MODE_MAX] = {
    { {ICON_OPEN_GALLERY, ICON_CAPTURE_IMAGE, ICON_MAX, ICON_MAX }, 1, 1, TRUE , TRUE },//{ {ICON_OPEN_GALLERY, ICON_CAPTURE_IMAGE, ICON_SWITCH_DV, ICON_MAX }, 1, 1, TRUE , TRUE },//dc preview mode
    { {ICON_MAX,          ICON_MAX,           ICON_MAX,       ICON_MAX }, 0, 0, FALSE, FALSE},//capture count down mode, not used.
    { {ICON_MAX,          ICON_MAX,           ICON_MAX,       ICON_MAX }, 0, 0, FALSE, FALSE},//capture mode, no button displayed.
    { {ICON_DC_CANCEL,    ICON_DC_CONFIRM,    ICON_MAX,       ICON_MAX }, 0, 0, TRUE,  TRUE},//dc review mode
    { {ICON_MAX,          ICON_MAX,           ICON_MAX,       ICON_MAX }, 0, 0, FALSE, FALSE},//dc save mode, not used.
    { {ICON_OPEN_GALLERY, ICON_RECORD_START,  ICON_SWITCH_DC, ICON_MAX }, 1, 1, TRUE , TRUE },//dv preview mode
    { {ICON_RECORD_STOP,  ICON_MAX,           ICON_MAX,       ICON_MAX }, 0, 0, TRUE , TRUE },//dv record mode
    { {ICON_MAX,          ICON_MAX,           ICON_MAX,       ICON_MAX }, 0, 0, FALSE, FALSE},//dv pause mode, not used
    { {ICON_MAX,          ICON_MAX,           ICON_MAX,       ICON_MAX }, 0, 0, TRUE , TRUE },//dv save mode
    { {ICON_DV_PLAY,      ICON_DV_CANCEL,     ICON_DV_CONFIRM,ICON_MAX }, 0, 0, TRUE , TRUE } //dv review mode
};

/*****************************************************************************/
//  Description: caculate icon rect add line boarder.
//  Parameter: [In] icon_rect: icon rect,
//                  boarder_width: boarder width
//             [Out] None
//             [Return] GUI_RECT after caculation.
//  Author: fulu.song
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T Watch_GetIconRectWithBoarder(GUI_RECT_T icon_rect, int16 boarder_width)
{
    GUI_RECT_T rect={0};
    rect.left = icon_rect.left - boarder_width;
    rect.top = icon_rect.top - boarder_width;
    rect.right = icon_rect.right + boarder_width;
    rect.bottom = icon_rect.bottom + boarder_width;
    return rect;
}

/*****************************************************************************/
//  Description: get image display rect by icon index.
//  Parameter: [In] icon_index: icon index in s_icon_table
//             [Out] None
//             [Return] GUI_RECT
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T WDC_GetIconRect(DC_ICON_INDEX_E icon_index)
{
    GUI_RECT_T rect = {0};
    if(icon_index < ICON_MAX)
    {
        return s_icon_table[icon_index].rect;
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_GetIconRect input icon_index invalid:%d", icon_index);
        return rect;
    }
}

/*****************************************************************************/
//  Description: get image id by icon index.
//  Parameter: [In] icon_index: icon index in s_icon_table
//             [Out] None
//             [Return] IMAGE_ID
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T WDC_GetIconImageID(DC_ICON_INDEX_E icon_index)
{
    if(icon_index < ICON_MAX)
    {
        return s_icon_table[icon_index].image_id;
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_GetIconImageID input icon_index invalid:%d", icon_index);
        return 0;
    }
}

/*****************************************************************************/
//  Description: get osd visiable or not at current flow mode.
//  Parameter: [In] mode: flow mode.
//             [Out] None
//             [Return] BOOLEAN: : true if visable
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WDC_GetOsdVisiablity(DC_FLOW_MODE_E mode)
{
    if(mode < DC_FLOW_MODE_MAX)
    {
        return s_osd_layout_list[mode].visiable;
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_GetOsdVisiablity input flow mode invalid:%d", mode);
        return FALSE;
    }

}

/*****************************************************************************/
//  Description: set osd visiable or not at current flow mode.
//  Parameter: [In] mode: flow mode.
//                  visiable: true if set to visable
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_SetOsdVisiablity(DC_FLOW_MODE_E mode, BOOLEAN visiable)
{
    if(mode < DC_FLOW_MODE_MAX)
    {
        s_osd_layout_list[mode].visiable = visiable;
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_GetOsdVisiablity input flow mode invalid:%d", mode);
    }

}

/*****************************************************************************/
//  Description: get icon index in s_osd_layout_list[mode].icon_list[index]
//  Parameter: [In] mode: flow mode.
//                  index: index in s_osd_layout_list.
//             [Out] None
//             [Return] icon_index: icon index in s_icon_table
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC DC_ICON_INDEX_E WDC_GetOsdIconIndex(DC_FLOW_MODE_E mode, uint16 index)
{
    if((mode < DC_FLOW_MODE_MAX)&& (index <= MAX_ICON_ON_SCREEN))
    {
        return s_osd_layout_list[mode].icon_list[index];
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_GetOsdIconIndex input param invalid: mode %d, index %d", mode, index);
        return ICON_MAX;
    }
}

/*****************************************************************************/
//  Description: change icon index in s_osd_layout_list[mode].icon_list[index]
//  Parameter: [In] mode: flow mode.
//                  index: index in s_osd_layout_list.
//                  icon_index: icon index in s_icon_table
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_ChangeOsdIconIndex(DC_FLOW_MODE_E mode, uint16 index, DC_ICON_INDEX_E icon_index)
{
    if((mode < DC_FLOW_MODE_MAX)&& (index <= MAX_ICON_ON_SCREEN))
    {
        s_osd_layout_list[mode].icon_list[index] = icon_index;
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_ChangeOsdIconIndex input param invalid: mode %d, index %d", mode, index);
    }

}

/*****************************************************************************/
//  Description: reset icon focus to default.
//  Parameter: [In] mode: flow mode.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_ResetOsdIconFocus(DC_FLOW_MODE_E mode)
{
    if(mode < DC_FLOW_MODE_MAX)
    {
        s_osd_layout_list[mode].focus_index = s_osd_layout_list[mode].default_focus_index;
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_ResetOsdIconFocus input flow mode invalid:%d", mode);
    }

}

/*****************************************************************************/
//  Description: get focused index in s_osd_layout_list.
//  Parameter: [In] mode: flow mode.
//             [Out] None
//             [Return] focused index
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC uint16 WDC_GetOsdIconFocus(DC_FLOW_MODE_E mode)
{
    if(mode < DC_FLOW_MODE_MAX)
    {
        return s_osd_layout_list[mode].focus_index;
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_GetOsdIconFocus input flow mode invalid:%d", mode);
    }
    return 0;
}

/*****************************************************************************/
//  Description: set icon focus.
//  Parameter: [In] mode: flow mode,
//                  index: index in s_osd_layout_list.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_SetOsdIconFocus(DC_FLOW_MODE_E mode, uint16 index)
{
    if((mode < DC_FLOW_MODE_MAX)&& (index <= MAX_ICON_ON_SCREEN))
    {
        s_osd_layout_list[mode].focus_index = index;
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_SetOsdIconFocus input param invalid: mode %d, index %d", mode, index);
    }
}

/*****************************************************************************/
//  Description: set icon focus.
//  Parameter: [In] mode: flow mode,
//                  icon: icon name.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_IconSetFocus(DC_FLOW_MODE_E mode, DC_ICON_INDEX_E icon)
{
    uint16 index=0;

    if((mode < DC_FLOW_MODE_MAX)&& (icon < ICON_MAX))
    {
        while(s_osd_layout_list[mode].icon_list[index]!=ICON_MAX)
        {
            if(s_osd_layout_list[mode].icon_list[index]==icon)
            {
                s_osd_layout_list[mode].focus_index = index;
                return;
            }
            else
            {
                index++;
            }
        }
    }
    else
    {
        TRACE_APP_CAMERA("[MMIDC] WDC_SetOsdIconFocus input param invalid: mode %d, index %d", mode, index);
    }
}


/*****************************************************************************/
//  Description: change play control icon to playing state icon.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN , TRUE if OK.
//  Author: fulu.song
//  Note: pause/stop state display ICON_DV_PLAY, playing state display ICON_DV_PAUSE
/*****************************************************************************/
PUBLIC BOOLEAN WDC_SetPlayingStateIcon(void)
{
    uint16 index=0;
    while(WDC_GetOsdIconIndex(DV_REVIEW_MODE, index)!=ICON_MAX && WDC_GetOsdIconIndex(DV_REVIEW_MODE, index)!=ICON_DV_PLAY)
    {
        index++;
    }
    if(WDC_GetOsdIconIndex(DV_REVIEW_MODE, index)==ICON_DV_PLAY)
    {
        WDC_ChangeOsdIconIndex(DV_REVIEW_MODE, index, ICON_DV_PAUSE);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description: change play control icon to pause/stop state icon.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN , TRUE if OK.
//  Author: fulu.song
//  Note: pause/stop state display ICON_DV_PLAY, playing state display ICON_DV_PAUSE
/*****************************************************************************/
PUBLIC BOOLEAN WDC_SetPausedOrStopedStateIcon(void)
{
    uint16 index=0;
    while(WDC_GetOsdIconIndex(DV_REVIEW_MODE, index)!=ICON_MAX && WDC_GetOsdIconIndex(DV_REVIEW_MODE, index)!=ICON_DV_PAUSE)
    {
        index++;
    }
    if(WDC_GetOsdIconIndex(DV_REVIEW_MODE, index)==ICON_DV_PAUSE)
    {
        WDC_ChangeOsdIconIndex(DV_REVIEW_MODE, index, ICON_DV_PLAY);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description: draw icon
//  Parameter: [In] icon_id: icon to draw.
//                  rect: cordination.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
LOCAL void Watch_DrawIcon(uint32 icon_id,  GUI_RECT_T rect)
{
    //int16 iconBorder = 1;
    MMIDC_DisplayBmp(rect.left, rect.top , icon_id);
}


/*****************************************************************************/
//  Description: ShowSoftKey
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
LOCAL void Watch_ShowSoftKey(void)
{
    int32 right_string_id = STXT_RETURN;
    GUI_RECT_T box_right = {0};
    GUI_RECT_T bg_rect = DP2PX_RECT(0,240,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT);
    MMI_STRING_T text_str = {0};
    uint16       text_width = 0;
    uint16       text_height = 0;
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    GUI_COLOR_T colorBg = RGB888_TO_RGB565(155,155,155);
    int16 screen_width = DP2PX_VALUE(MMI_MAINSCREEN_WIDTH);
    int16 screen_height = DP2PX_VALUE(MMI_MAINSCREEN_HEIGHT);
    DC_FLOW_MODE_E  mode = MMIDC_GetCurrentMode();

    TRACE_APP_CAMERA("[MMIDC]: Watch_ShowSoftKey");
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&dev_info))
    {
        LCD_FillRectARGB888((GUI_LCD_DEV_INFO const *)&dev_info, bg_rect, RGB5652ARGB888(colorBg,255));
    }
    else
#endif
    {
        LCD_FillRect((GUI_LCD_DEV_INFO const *)&dev_info, bg_rect, colorBg);
    }


    //SUB LCD DO NOT DISPLAY
    if (SUB_LCD_ID == MMIDC_GetLcdID())
    {
        return ;
    }
    MMI_GetLabelTextByLang(right_string_id, &text_str);
    text_width = MMIDC_GetStringWidth(&text_str);
    text_height = MMIDC_GetStringHeight(&text_str);
    box_right.bottom = screen_height - 1 - 1;
    box_right.right = screen_width - 1 - 1;
    box_right.left = box_right.right - text_width;
    box_right.top = box_right.bottom - text_height;

    MMIDC_DisplayString(box_right, &text_str,colorBg, FALSE);
}


/*****************************************************************************/
//  Description: draw icon line boarder for focus display.
//  Parameter: [In] rect: icon rect include line boarder.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: only for testers. do not display on customer device.
/*****************************************************************************/
LOCAL void Watch_DrawIconFocus(GUI_RECT_T rect)
{
#ifndef TOUCH_PANEL_SUPPORT
    GUI_LCD_DEV_INFO    lcd_dev_info = MMIDC_GetOSDBlockInfo();
    GUI_COLOR_T color = RGB888_TO_RGB565(0x42, 0x85, 0xf4);

    LCD_DrawRect(&lcd_dev_info, rect, color );
    Watch_ShowSoftKey();
#endif

}

#ifdef DC_OSD_DBG
LOCAL GUI_LCD_DEV_INFO       s_tips_layer = {0,  0};
PUBLIC void WDC_DrawTPFocus(MMI_HANDLE_T win_id, uint32 color, GUI_RECT_T rect)
{
#ifdef TOUCH_PANEL_SUPPORT
//#ifndef RELEASE_INFO
    //uint32 color = 0xB2FFF200;
    if(UILAYER_IsMultiLayerEnable()){
        UILAYER_CREATE_T create_info = {0};
        UILAYER_APPEND_BLT_T append_layer = {0};

        if(s_tips_layer.block_id!= 0)
        {
            UILAYER_RemoveBltLayer(&s_tips_layer);
            UILAYER_ReleaseLayer(&s_tips_layer);
        }

        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = rect.left - 1;
        create_info.offset_y = rect.top - 1;
        create_info.width = rect.right - rect.left + 2;
        create_info.height = rect.bottom - rect.top + 2;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, &s_tips_layer)){
            UILAYER_Clear(&s_tips_layer);
        }
        append_layer.lcd_dev_info = s_tips_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
        UILAYER_FillColor(&s_tips_layer,  color );
    }
//#endif
#endif
}
/*****************************************************************************/
//  Description: draw icon line boarder
//  Parameter: [In] rect: icon rect include line boarder.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: only for testers. do not display on customer device.
/*****************************************************************************/
LOCAL void Watch_DrawIconBorder(GUI_RECT_T rect)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = MMIDC_GetOSDBlockInfo();
    GUI_COLOR_T color = RGB888_TO_RGB565(0xff, 0x0, 0xff);
    LCD_DrawRect(&lcd_dev_info, rect, color );
}

#endif

/*****************************************************************************/
//  Description: clear icons before exit flow mode.
//  Parameter: [In] mode: flow mode
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_Display_FlowModeExit(DC_FLOW_MODE_E mode)
{
    uint16 iter_icon=0;
    uint16 boarder = 1;
    GUI_COLOR_T color = MMIDC_GetTransparentColor();
    while(WDC_GetOsdIconIndex(mode, iter_icon) != ICON_MAX)
    {
        GUI_RECT_T rect = Watch_GetIconRectWithBoarder( WDC_GetIconRect(WDC_GetOsdIconIndex(mode, iter_icon)), boarder);
        MMIDC_ClearRect(rect, color);
        iter_icon++;
    }
}

/*****************************************************************************/
//  Description: display ui when enter ? flow mode.
//  Parameter: [In] mode: flow mode.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_Display_FlowModeEnter(DC_FLOW_MODE_E mode)
{
    uint16 iter_icon=0;
    uint16 boarder = 1;

    WDC_ResetOsdIconFocus(mode);
    if(MMIDC_GetMMIFocusStatus != MMIDC_GET_FOCUS)
    {
        return;
    }
    while(WDC_GetOsdIconIndex(mode, iter_icon) != ICON_MAX)
    {
#ifdef DC_OSD_DBG
        Watch_DrawIconBorder(Watch_GetIconRectWithBoarder( WDC_GetIconRect(WDC_GetOsdIconIndex(mode, iter_icon)), 1));
#endif

        Watch_DrawIcon(WDC_GetIconImageID(WDC_GetOsdIconIndex(mode, iter_icon)),
                        WDC_GetIconRect(WDC_GetOsdIconIndex(mode, iter_icon)));

        if(iter_icon == WDC_GetOsdIconFocus(mode))
        {
            Watch_DrawIconFocus(Watch_GetIconRectWithBoarder( WDC_GetIconRect(WDC_GetOsdIconIndex(mode, iter_icon)), boarder));
        }
        iter_icon++;
    }
    MMIDC_CompleteDisplayOSD();
}


/*****************************************************************************/
//  Description: draw video recording time and red dot.
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_DisplayVideoRecordTip(void)
{
#ifdef ADULT_WATCH_SUPPORT
    uint16 spacing_top = DP2PX_VALUE(24);
    uint16 spacing_img = DP2PX_VALUE(6);
    uint16 spacing_left = DP2PX_VALUE(12);
    uint16 spacing_right = DP2PX_VALUE(12);
    uint16 rect_hight = DP2PX_VALUE(32);
#else
    uint16 spacing_top = DP2PX_VALUE(8);
    uint16 spacing_img = DP2PX_VALUE(8);
    uint16 spacing_left = DP2PX_VALUE(8);
    uint16 spacing_right = DP2PX_VALUE(8);
    uint16 rect_hight = DP2PX_VALUE(24);
#endif

    uint16 img_width = 0;
    uint16 img_hight = 0;
    uint32 time = 0;
    uint16 str_len = 0;
    uint16 text_width = 0;
    uint16 text_hight = 0;
    uint16 total_width = 0;
    GUI_RECT_T str_rect = {0};
    GUI_RECT_T img_rect = {0};
    GUI_RECT_T whole_rect = {0};
    MMI_STRING_T  time_str ;
    wchar  time_wstr[12] = {'0','0',':','0','0',':','0','0',0};
    MMI_IMAGE_ID_T img_id = IMG_DC_WATCH_RECORDING_INDICATOR;

    char str_buffer_arr[20] = {0};

    int16 screen_width = MMIDC_GetPreviewWidth();

    GUISTR_STYLE_T style ;

    GUISTR_INFO_T text_info = {0};

    static uint32 last_time_val = 0;
    //get bmp width and height
    GUIRES_GetImgWidthHeight(&img_width, &img_hight, img_id, MMIDC_MAIN_WIN_ID);

    //get time str display rect
    time_str.wstr_ptr = time_wstr;
    time_str.wstr_len = MMIAPICOM_Wstrlen(time_wstr);
    GetStringDefaultStyle(&style);
    GUISTR_GetStringInfo(&style, &time_str , MMIDC_GetOSDFontState(), &text_info);
    text_width = text_info.width;
    text_hight = text_info.height;
    total_width = spacing_left + img_width + spacing_img + text_width + spacing_right ;

    whole_rect.top = spacing_top;
    whole_rect.bottom = whole_rect.top + rect_hight;
    whole_rect.left = screen_width/2 - total_width/2;
    whole_rect.right = whole_rect.left + total_width;

    str_rect.left = whole_rect.left + spacing_left + img_width + spacing_img;
    str_rect.right = str_rect.left + text_width;
    str_rect.top = whole_rect.top + (rect_hight-text_hight)/2;
    str_rect.bottom = str_rect.top + text_hight;

    img_rect.left = whole_rect.left + spacing_left;
    img_rect.right = img_rect.left + img_width;
    img_rect.top = spacing_top + (rect_hight - img_hight)/2;
    img_rect.bottom = img_rect.top + img_hight;

    //time str
    time = MMIDC_GetRecordSecond();
    if(time!=0 && time == last_time_val )
    {
        return;// don't update screen for performance.
    }
    else
    {
        last_time_val = time;
    }
    str_len = (uint16)(sprintf(str_buffer_arr, "%.2lu:%.2lu:%.2lu", time / 60 / 60, time / 60 % 60, time % 60));

    //TRACE_APP_CAMERA("[MMIDC] WDC_DisplayVideoRecordTip %s", str_buffer_arr);

    //time_str.wstr_ptr = time_wstr;
    time_str.wstr_len = str_len;
    MMIAPICOM_StrToWstr((uint8 *)str_buffer_arr, time_str.wstr_ptr);

    //clear rect
    MMIDC_ClearRect(whole_rect,MMIDC_GetTransparentColor());
    //display string.
    MMIDC_DisplayString(str_rect, &time_str, MMIDC_GetTransparentColor(), FALSE);
    //display  RED  bitmap
    if((time % 2) == 0)
    {
            MMIDC_DisplayBmp(img_rect.left, img_rect.top, img_id);
    }

}


/*****************************************************************************/
//  Description: show all osd and commit to lcd
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, TRUE if need to display icons.
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WDC_ShowAllOSD(void)
{
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();
    BOOLEAN is_show = WDC_GetOsdVisiablity(mode);

    //which flow mode to show content or not
    TRACE_APP_CAMERA("[MMIDC] WDC_ShowAllOSD is_show = %d",is_show);
    //display content by flow mode
    switch(mode)
    {
        case DV_RECORD_MODE:
            WDC_DisplayVideoRecordTip();
            break;
        case DV_REVIEW_MODE:
            if(WDC_isInVpState()==FALSE)
            {
                MMIDC_DisplayLastFrame();
            }
            break;
        default:
            break;
    }

    if (is_show)
    {
        uint16 iter_icon=0;
        uint16 boarder = 1;
        while(WDC_GetOsdIconIndex(mode, iter_icon) != ICON_MAX)
        {
#ifdef DC_OSD_DBG
            Watch_DrawIconBorder(Watch_GetIconRectWithBoarder( WDC_GetIconRect(WDC_GetOsdIconIndex(mode, iter_icon)), 1));
#endif

            Watch_DrawIcon(WDC_GetIconImageID(WDC_GetOsdIconIndex(mode, iter_icon)),
                            WDC_GetIconRect(WDC_GetOsdIconIndex(mode, iter_icon)));


            if(iter_icon == WDC_GetOsdIconFocus(mode))
            {
                Watch_DrawIconFocus(Watch_GetIconRectWithBoarder( WDC_GetIconRect(WDC_GetOsdIconIndex(mode, iter_icon)), boarder));
            }
            iter_icon++;
        }
    }
    MMIDC_CompleteDisplayOSD();
    return TRUE;
}

/*****************************************************************************/
//  Description: clear all icons rect
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_ClearAllRect(void)
{
    uint16 iter_icon = 0;
    uint16 boarder = 1;
    GUI_COLOR_T color = MMIDC_GetTransparentColor();
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();
    TRACE_APP_CAMERA("[MMIDC] WDC_ClearAllRect  ");
    while(WDC_GetOsdIconIndex(mode, iter_icon) != ICON_MAX)
    {
        GUI_RECT_T rect = Watch_GetIconRectWithBoarder( WDC_GetIconRect(WDC_GetOsdIconIndex(mode, iter_icon)), boarder);
        MMIDC_ClearRect(rect, color);
        iter_icon++;
    }
    MMIDC_ResetScrollNumber();
}

/*****************************************************************************/
//  Description: reset icon focus on screen.
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void WDC_ResetIconFocus(void)
{
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();
    TRACE_APP_CAMERA("[MMIDC] WDC_ResetIconFocus flow mode = %d", mode);
    WDC_ResetOsdIconFocus(mode);
}


/*****************************************************************************/
//  Description:
//  Parameter: [In] None
//             [Out] None
//             [Return] return 240 for lcd height.
//  Author: fulu.song
//  Note: temp function, on phone lcd, get screen height function will return 320
//       only for developing, avoid hard coding in other functions.
/*****************************************************************************/
PUBLIC int16 WDC_GetOsdHeight(void)
{
    return DP2PX_VALUE(MMI_MAINSCREEN_HEIGHT);
}

#endif

#if 0
/*****************************************************************************/
//Description : display waitting photo
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayWaitPhoto(void)
{
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    GUI_POINT_T      dis_point = {0};

    int16  screen_width = MMIDC_GetPreviewWidth();
    int16  screen_height = MMIDC_GetPreviewHeight();
    uint16 img_width = 0;
    uint16 img_height = 0;
#ifdef DC_WATCH_UI_SUPPORT
    MMI_IMAGE_ID_T wait_img_id = IMG_DC_WATCH_WAIT;
    screen_height = WDC_GetOsdHeight();
#else
    MMI_IMAGE_ID_T wait_img_id = IMG_DC_WAIT;
#endif
    //get resources width or height
    GUIRES_GetImgWidthHeight(&img_width, &img_height, wait_img_id, MMIDC_MAIN_WIN_ID);

    dis_point.x = (screen_width - img_width) / 2;
    dis_point.y = (screen_height - img_height) / 2;

    dev_info.lcd_id = MMIDC_GetLcdID();
    GUIRES_DisplayImg(&dis_point, PNULL, PNULL, MMIDC_MAIN_WIN_ID, wait_img_id, &dev_info);

    MMIDC_CompleteDisplayOSD();
}
#endif

/*****************************************************************************/
//  Description : show all menu and softkey
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 ShowAllOSD(void)
{
#if !defined(MMIDC_DESKTOP_PDA)
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();
    MMIDC_SHOW_STATE_E   show_state = MMIDC_GetShowState();
    int32 count_down_seconds = 0;
    BOOLEAN is_show = FALSE;
#if defined(MMIDC_DESKTOP_ICON_BAR)
    BOOLEAN is_desktop_icon_open = FALSE;
#endif

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShowAllOSD cur_mode = %d, show_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_2091_112_2_18_2_8_42_213,(uint8*)"dd", mode, show_state);

    //display background image, i.e frame or last dv frame
    if ( (MMIDC_IsShowPhotoFrame() || MMIAPIDC_IsPreviewing()
          )
       )
    {
        MMIDC_SetPhotoFrameForShow(FALSE);
        if (FRAME_DISABLE != MMIDC_GetFrameIndex() )
        {
            MMIDC_DisplayFrame();
        }
    }

    //display last frame
    if(DV_REVIEW_MODE == mode)
    {
#if !defined(MMIDC_DESKTOP_PDA) || defined(MMIDC_F_PDA_BIT_16)
        if (SCREEN_MODE_HORIIZONTAL == MMIDC_GetScreenMode() && LCD_ANGLE_0 == MMIDC_GetPlatformAngle())
        {
            MMIDC_SetLcdForRotate(FALSE);
            MMIDC_SetScreenMode(SCREEN_MODE_VERTICAL);
        }
#endif
        MMIDC_DisplayLastFrame();
    }


    //which flow mode to show content or not
    switch(mode)
    {
    case DC_PREVIEW_MODE:
    case DV_PREVIEW_MODE:
    case DV_RECORD_MODE:
    case DV_REVIEW_MODE:
    case DV_PAUSE_MODE:
        is_show = TRUE;
        break;
    case DC_SAVE_MODE:
        is_show = TRUE;
        break;
    default:
        break;
    }
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShowAllOSD is_show = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_2128_112_2_18_2_8_42_214,(uint8*)"d", is_show);

    //if show content ,then display softkey
    if (is_show)
    {
         MMIDC_DisplaySoftKey(mode);
    }

    //display osd, before preview
    if (MMIDC_GetOSDFlagBeforePreview())
    {
        MMIDC_SetOSDFlagBeforePreview(FALSE);

        MMIDC_DisplaySoftKey(mode);
        MMIDC_DisplaySettingTip();

#if defined(MMIDC_DESKTOP_ICON_BAR)
        MMIDC_OpenIconDesktop();
#endif

#ifdef MMIDC_F_N_IN_ONE
        if(MMIDC_IsNInOneEnable()
                && (DC_PREVIEW_MODE == mode)
                )
        {
            MMIDC_DisplayNInOne();
        }
#endif

    }

    //display menu by state ,while preview
    if (MMIAPIDC_IsPreviewing() && DC_CAPTURE_CONT_DOWN_MODE != mode)
    {
#if defined(MMIDC_DESKTOP_ICON_BAR)
        is_desktop_icon_open = MMIDC_IsIconDesktopOpen();
        if (is_desktop_icon_open)
        {
            MMIDC_DisplayIconDesktop();
        }
        else
#endif
        {
            MMIDC_DisplaySettingTip();

            switch(show_state)
            {
            case MMIDC_SHOW_NONE:
                break;
            case MMIDC_SHOW_MENU:
                MMIDC_GetMenuHandle()->MenuDisplay();
                break;
            case MMIDC_SHOW_ADJUST:
                MMIDC_DisplayAdjust();
                MMIDC_GetIconHandle()->IconDisplay();
                break;
            case MMIDC_SHOW_ICON:
            case MMIDC_SHOW_ALLMENU:
            default:
                if (MMIDC_IsAdjustMenu())
                {
                    MMIDC_DisplayAdjust();
                }
                else
                {
                    MMIDC_GetMenuHandle()->MenuDisplay();
                }
                MMIDC_GetIconHandle()->IconDisplay();
                break;
            }
        }
    }

    //display content by flow mode
    switch(mode)
    {
    case DC_CAPTURE_CONT_DOWN_MODE:
        count_down_seconds = MMIDC_GetCountDownSeconds();
        MMIDC_DisplayDelaySecondsTip(count_down_seconds);
        break;
    case DV_RECORD_MODE:
        MMIDC_DisplayVideoRecordTip(DV_RECORD_MODE);
#if defined(MMIDC_DESKTOP_ICON_BAR) && defined(MMIDC_F_DV_ZOOM)
        MMIDC_DisplayZoomButton();
#endif
        break;
	case DV_PAUSE_MODE:
#if defined(MMIDC_DESKTOP_ICON_BAR) && defined(MMIDC_F_DV_ZOOM)
        MMIDC_DisplayZoomButton();
#endif
		break;
    case DV_REVIEW_MODE:
#if defined(MMIDC_F_WORK_MODE)
        if(!MMIDC_IsWorkMode())
#endif
        {
            MMIDC_DisplayVideoReview();
        }

        if (MMIDC_SHOW_MENU == show_state || MMIDC_SHOW_ALLMENU == show_state)
        {
            MMIDC_GetMenuHandle()->MenuDisplay();
        }
        break;
    default:
        break;
    }

    return is_show;
#else
    return FALSE;
#endif
}

#ifdef WRE_SUPPORT
extern int32 wre_in_camera_scan_mode(void );
#endif

/*****************************************************************************/
//  Description : show all menu and softkey
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ShowAllOSD(void)
{
    BOOLEAN is_show = FALSE;
    MMIDC_DESKTOP_STYLE_E desktop_style = MMIDC_GetDesktopStyle();

#ifdef DC_WATCH_UI_SUPPORT
    return WDC_ShowAllOSD();
#endif
    #ifdef WRE_SUPPORT
            if(1 == wre_in_camera_scan_mode()){
                //BYPASS  Camera  APP OSD for WRE's OSD
                return 0;
            }
    #endif

    switch(desktop_style)
    {
#ifdef MMIDC_DESKTOP_PDA
    case DESKTOP_STYLE_PDA:
        is_show = MMIDC_DisplayPDADesktop();
        break;
#endif

    default:
        is_show = ShowAllOSD();
        break;
    }

    //if show content then brush to lcd
    if (is_show)
    {
        MMIDC_CompleteDisplayOSD();
    }

    return 0;
}

/*****************************************************************************/
//  Description : clear all icon, menu, softkey, frame
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ClearAllRect(void)
{
    //clean softkey rect, menu rect and frame rect
    MMIDC_ResetScrollNumber();

    MMIDC_ClearSoftkeyRect();

    MMIDC_CleanAllMenuRect();
    MMIDC_ClearFrameRect();

    return 0;
}

/*****************************************************************************/
//  Description : clean all icon, menu ,softkey, and set icon showed state
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_CleanAllOSDRect(void)
{
    //clean softkey rect, menu rect and frame rect
    MMIDC_SetShowState(MMIDC_SHOW_NONE);

    MMIDC_ClearAllRect();

    return 0;
}

/*****************************************************************************/
// 	Description : display dv last frame
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayLastFrame(void)
{
//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    uint8 *frame_ptr = (uint8*)MMIDC_GetA888LastFrameByPtr();
#else
    GUI_COLOR_T  *frame_ptr = MMIDC_GetLastFramePtr();
    GUI_POINT_T     dis_point = {0};
    GUIIMG_BITMAP_T  bitmap = {0};
#endif

    uint16 lcd_width = MMIDC_GetPreviewWidth();
    uint16 lcd_height = MMIDC_GetPreviewHeight();


    GUI_RECT_T       display_rect = MMIDC_GetFullScreenRect(GUI_MAIN_LCD_ID);
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayLastFrame width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_2332_112_2_18_2_8_42_215,(uint8*)"dd",lcd_width, lcd_height);

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayLastFrame frame_ptr = 0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_2335_112_2_18_2_8_42_216,(uint8*)"d",(uint32)frame_ptr);

    if (PNULL != frame_ptr)
    {
//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
        {
            uint32 buf_size = 0;
            uint8 *layer_ptr = PNULL;

            layer_ptr = UILAYER_GetLayerBufferPtr(&dev_info);
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayLastFrame layer_ptr = 0x%lX, block_id=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_2346_112_2_18_2_8_42_217,(uint8*)"dd",(uint32)layer_ptr, dev_info.block_id);

            if (PNULL != layer_ptr)
            {
                buf_size = lcd_width * lcd_height * LAST_FRAME_BUFFER_ARGB888;

                SCI_MEMCPY(layer_ptr, frame_ptr, buf_size);
            }
            else
            {
                MMIDC_ClearOSDBlock();
            }
        }
#else
        bitmap.bit_ptr = frame_ptr;
        bitmap.img_width = lcd_width;
        bitmap.img_height = lcd_height;
        GUIIMG_DisplayBmp(FALSE,
            &display_rect,
            &dis_point,
            &bitmap,
            &dev_info);

#endif
       MMITHEME_StoreUpdateRect(&dev_info, display_rect);
   }
   else
   {
        //刷整屏为黑色
        MMIDC_ClearOSDBlock();
   }
}

/*****************************************************************************/
//Description :  get desktop layout
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC MMIDC_DESKTOP_STYLE_E MMIDC_GetDesktopStyle(void)
{
    MMIDC_DESKTOP_STYLE_E desktop_layout = 0;

#ifdef MMIDC_DESKTOP_ICON_BAR
    desktop_layout = DESKTOP_STYLE_ICON;
#elif defined(MMIDC_DESKTOP_PDA)
    desktop_layout = DESKTOP_STYLE_PDA;
#endif

    return desktop_layout;
}

/*****************************************************************************/
//Description : get item text string display by font
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayDateOSDStringByFont(GUI_FONT_T font,
                                               GUI_RECT_T  rect,
                                               MMI_STRING_T  *text_ptr,
                                               GUI_COLOR_T bg_color,
                                               BOOLEAN is_auto_scroll
                                               )
{
#if defined(MMI_CAMERA_F_PHOTO_DATE)
    MMIDC_STRING_STYLE_T    string ={0};
    PHOTO_DATE_LAYER_INFO_T layer_info = {0};
    GUI_COLOR_T font_color = 0;
    GUISTR_BUF_T des_buf = {0};

    layer_info = MMIDC_GetPhotoDateDisplayLayerInfo();
    string.rect = rect;
    string.text_ptr = text_ptr;
    string.state = MMIDC_GetOSDFontState();

    GetStringDefaultStyle(&string.text_style);

    font_color = MMIDC_GetPhotoDateOSDFontColor();
    // text style
    string.text_style.font = font;
    string.text_style.font_color = font_color;
    string.text_style.bg_color = bg_color;

    if (PNULL != layer_info.buffer_ptr)
    {
        des_buf.buffer = layer_info.buffer_ptr;

        des_buf.mem_width = layer_info.layer_width;
        des_buf.width = layer_info.layer_width;
        des_buf.height = layer_info.layer_height;
        des_buf.type = GUISTR_BUF_ARGB888;


        GUISTR_DrawTextInRect(
            &des_buf,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)text_ptr,
            &string.text_style,
            string.state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayDateOSDStringByFont BUFFER NULL!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FULL_OSD_DISPLAY_2448_112_2_18_2_8_42_218,(uint8*)"");
    }
#endif
}



#endif  //#ifdef  CAMERA_SUPPORT

