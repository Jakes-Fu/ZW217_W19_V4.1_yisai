/*****************************************************************************
** File Name:      mmitheme_softkey.c                                        *
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
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmitheme_softkey.h"
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
#include "mmiset_id.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmitheme_special_func.h"
#include "block_mem.h"
//#include "mmiset_display.h"
//#include "guires.h"
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
#include "mmitheme_button.h"
#include "mmistk_id.h"
#include "mmieng_id.h"
#include "mmialarm_id.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

//#ifdef GUIF_SOFTKEY_PDASTYLE
//#ifdef MAINLCD_SIZE_320X480
//#define MMI_PDA_SOFTKEY_BAR_HEIGHT          54  //全屏下SOFTKEY栏高度
//#elif defined MAINLCD_SIZE_240X400
//#define MMI_PDA_SOFTKEY_BAR_HEIGHT          40//54  //全屏下SOFTKEY栏高度
//#else
//#error
//#endif
//#endif

//button softkey data

#define BUTTONSOFTKEY_STYLE_NUM     3   //(GUISOFTKEY_STYLE_3BUTTON-GUISOFTKEY_STYLE_1BUTTON+1)

typedef struct
{
    uint32 col_num;
    uint16 h_side_margin;
    uint16 v_side_margin;
    uint16 h_item_space;
    uint16 v_item_space;
}MMI_BUTTONSOFTKEY_MARGIN_T;


LOCAL const MMI_BUTTONSOFTKEY_MARGIN_T s_buttonsoftkey_margin[BUTTONSOFTKEY_STYLE_NUM]=
{
    {
        1,
        MMITHEME_SOFTKEY_1BUTTON_H_SIDE_MARGIN,
        MMITHEME_SOFTKEY_1BUTTON_V_SIDE_MARGIN,
        MMITHEME_SOFTKEY_1BUTTON_H_ITEM_SPACE,
        MMITHEME_SOFTKEY_1BUTTON_V_ITEM_SPACE,
    },
    {
        2,
        MMITHEME_SOFTKEY_2BUTTON_H_SIDE_MARGIN,
        MMITHEME_SOFTKEY_2BUTTON_V_SIDE_MARGIN,
        MMITHEME_SOFTKEY_2BUTTON_H_ITEM_SPACE,
        MMITHEME_SOFTKEY_2BUTTON_V_ITEM_SPACE,
    },
    {
        3,
        MMITHEME_SOFTKEY_3BUTTON_H_SIDE_MARGIN,
        MMITHEME_SOFTKEY_3BUTTON_V_SIDE_MARGIN,
        MMITHEME_SOFTKEY_3BUTTON_H_ITEM_SPACE,
        MMITHEME_SOFTKEY_3BUTTON_V_ITEM_SPACE,
    }
};

LOCAL const MMI_BUTTONSOFTKEY_MARGIN_T s_buttonsoftkey_h_margin[BUTTONSOFTKEY_STYLE_NUM]=
{
    {
        1,
        MMITHEME_H_SOFTKEY_1BUTTON_H_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_1BUTTON_V_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_1BUTTON_H_ITEM_SPACE,
        MMITHEME_H_SOFTKEY_1BUTTON_V_ITEM_SPACE,
    },
    {
        2,
        MMITHEME_H_SOFTKEY_2BUTTON_H_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_2BUTTON_V_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_2BUTTON_H_ITEM_SPACE,
        MMITHEME_H_SOFTKEY_2BUTTON_V_ITEM_SPACE,
    },
    {
        3,
        MMITHEME_H_SOFTKEY_3BUTTON_H_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_3BUTTON_V_SIDE_MARGIN,
        MMITHEME_H_SOFTKEY_3BUTTON_H_ITEM_SPACE,
        MMITHEME_H_SOFTKEY_3BUTTON_V_ITEM_SPACE,
    }
};

#define MMI_SOFTKEY_BAR_HEIGHT   DP2PX_VALUE(60)
/*****************************************************************************/
// Description : get button style index
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL uint32 GetButtonStyleIndex(
                                 GUISOFTKEY_BUTTONSTYLE_E style
                                 );

/*****************************************************************************/
// 	Description : get the softkey style, is called by the softkey control
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSoftkeyStyle(
                                        MMI_SOFTKEY_STYLE_T* softkey_style_ptr
	                                    )
{
    //SCI_PASSERT(PNULL != softkey_style_ptr, ("MMITHEME_GetSoftkeyStyle: the param is PNULL!")); /*assert verified*/
    if (PNULL == softkey_style_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET(softkey_style_ptr, 0x00, sizeof(MMI_SOFTKEY_STYLE_T));

   softkey_style_ptr->bg_type =  GUI_SOFTKEY_BG_COLOR_ONLY;
#ifdef MONOCHROME_LCD_SUPPORT 
    softkey_style_ptr->bg_image     = 0;  //背景图片
    softkey_style_ptr->bg_color     = MMI_WHITE_COLOR;
#else
    softkey_style_ptr->bg_image     = PNULL;  //背景图片
    softkey_style_ptr->bg_color     = MMITHEME_SOFTKEY_BACKGROUND_COLOR;
#endif//syy modify 2010.11.30 -end
    softkey_style_ptr->font_color   = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);     //字体颜色
    softkey_style_ptr->pressed_font_color   = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_PRESSED);     //字体颜色
    softkey_style_ptr->font         = MMI_FULLSCREEN_SOFTKEY_TEXT_FONT;           //字体，这个参加暂时没用上，softkey实时去取
    softkey_style_ptr->first_icon_offset   = MMI_SOFTKEYBAR_FIRST_ICON_POS ;          // 第一个图标的偏移量
    softkey_style_ptr->second_icon_offset  = MMI_SOFTKEYBAR_SECOND_ICON_POS ;         // 第二个图标的偏移量
    softkey_style_ptr->third_icon_offset   = MMI_SOFTKEYBAR_THIRD_ICON_POS;           // 第三个图标的偏移量

#ifdef MONOCHROME_LCD_SUPPORT 
#else
#ifndef GUIF_SOFTKEY_PDASTYLE
    softkey_style_ptr->skb_button[LEFT_BUTTON].icon_up_id = IMAGE_SOFTKEY_BG;
    softkey_style_ptr->skb_button[MIDDLE_BUTTON].icon_up_id = IMAGE_SOFTKEY_BG;
    softkey_style_ptr->skb_button[RIGHT_BUTTON].icon_up_id = IMAGE_SOFTKEY_BG;
    softkey_style_ptr->skb_button[LEFT_BUTTON].icon_down_id = IMAGE_SOFTKEY_BG_DOWN;
    softkey_style_ptr->skb_button[MIDDLE_BUTTON].icon_down_id = IMAGE_SOFTKEY_BG_DOWN;
    softkey_style_ptr->skb_button[RIGHT_BUTTON].icon_down_id = IMAGE_SOFTKEY_BG_DOWN;
#endif
#endif//syy modify 2010.11.30 -end

    softkey_style_ptr->skb_button[LEFT_BUTTON].rect   = MMITHEME_GetSoftkeyRect(LEFT_BUTTON);
    softkey_style_ptr->skb_button[MIDDLE_BUTTON].rect = MMITHEME_GetSoftkeyRect(MIDDLE_BUTTON);
    softkey_style_ptr->skb_button[RIGHT_BUTTON].rect  = MMITHEME_GetSoftkeyRect(RIGHT_BUTTON);

    softkey_style_ptr->skb_button[LEFT_BUTTON].is_show = TRUE;
    softkey_style_ptr->skb_button[MIDDLE_BUTTON].is_show = TRUE;
    softkey_style_ptr->skb_button[RIGHT_BUTTON].is_show = TRUE;
#if defined SCREEN_SHAPE_CIRCULAR
    softkey_style_ptr->align[LEFT_BUTTON] = TEXT_ALIGN_HVMIDDLE;
    softkey_style_ptr->align[MIDDLE_BUTTON] = TEXT_ALIGN_HVMIDDLE;
    softkey_style_ptr->align[RIGHT_BUTTON] = TEXT_ALIGN_HVMIDDLE;
#elif defined SCREEN_SHAPE_SQUARE
    softkey_style_ptr->align[LEFT_BUTTON] = TEXT_ALIGN_LVMIDDLE;
    softkey_style_ptr->align[MIDDLE_BUTTON] = TEXT_ALIGN_HVMIDDLE;
    softkey_style_ptr->align[RIGHT_BUTTON] = TEXT_ALIGN_HVMIDDLE;
#endif
    return (TRUE);
}
/*****************************************************************************/
// 	Description : get the mid softkey is null
//	Global resource dependence : 
//  Author: nanji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsMidkeyNull(
									 MMI_TEXT_ID_T text_id
									 )
{
    BOOLEAN result = FALSE;
	if (TXT_NULL == text_id)
	{
		result = TRUE;
	}

	return result;
}
/*****************************************************************************/
// 	Description : get the default softkey image info
//	Global resource dependence : 
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDefaultSoftkeyImg(
                                          GUISOFTKEY_IMG_T    *img_ptr    //in/out
                                          )
{
    //SCI_ASSERT(PNULL != img_ptr);/*assert verified*/
    if (PNULL == img_ptr)//xiyuan edit for pclint
    {
return;
}
#ifndef GUIF_SOFTKEY_PDASTYLE
        //set image
        img_ptr->bg_img         = IMAGE_SOFTKEY_BG;
        img_ptr->left_down_img  = IMAGE_SOFTKEY_BG_DOWN;
        img_ptr->left_up_img    = IMAGE_SOFTKEY_BG;
        img_ptr->mid_down_img   = IMAGE_SOFTKEY_BG_DOWN;
        img_ptr->mid_up_img     = IMAGE_SOFTKEY_BG;
        img_ptr->right_down_img = IMAGE_SOFTKEY_BG_DOWN;
        img_ptr->right_up_img   = IMAGE_SOFTKEY_BG;
#endif
}

/*****************************************************************************/
// 	Description : get the height of the softkey area, is called by the MMI Kernel
//	Global resource dependence : 
//  Author:Robert.Lu
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMITHEME_GetSoftkeyHeight(void)
{
#ifdef GUIF_SOFTKEY_PDASTYLE
    return MMI_PDA_SOFTKEY_BAR_HEIGHT;
#else
    return MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
#endif
}

LOCAL MMI_WIN_ID_T s_uitest_win_id[]=
{
#if defined(GPS_SUPPORT)
   MMIENG_UITEST_GPS_WIN_ID,
   MMIENG_UITESTGPS_WIN_ID,
   MMIENG_TEST_GPS_GNSSMODE_WIN_ID,
   MMIENG_TEST_GPS_CRICLETEST_PARAMSET_WIN_ID,
   MMIENG_TEST_GPS_NMEA_LOG_WIN_ID,
#endif
   MMIENG_UITESTATV_CHANNEL_WIN_ID,
   MMIENG_UITESTATV_SIGNAL_WIN_ID,
   MMIENG_UITESTATV_CHANNEL_EDIT_WIN_ID,
   MMIENG_UITEST_CHARGING_WIN_ID,
   MMIENG_UITEST_EMCC_WIN_ID,
   MMIENG_UITESTCAMERA_SET_PARAM_WIN_ID,
   MMIENG_UITESTBLUETOOTH_WIN_ID,
   MMIENG_UITESTFM_WIN_ID,
   MMIENG_UITESTCOM_WIN_ID,
   MMIENG_UITESTTFLASH_WIN_ID,
   MMIENG_UITESTTVOUT_WIN_ID,
   MMIENG_UITESTSD_WIN_ID,
   MMIENG_UITESTMELODY_WIN_ID,
   MMIENG_UITESTLOOPBACK_WIN_ID,
   MMIENG_UITESTHEADSET_WIN_ID,
#ifdef  CAMERA_SUPPORT
   MMIENG_UITESTCAMERA_WIN_ID,
   MMIENG_UITESTVIDEO_WIN_ID,
#endif
   MMIENG_UITEST_RESULT_WIN_ID,
   MMIENG_UITEST_PHONEINFO_WIN_ID,
   MMIENG_UITESTKEY_WIN_ID,
   MMIENG_UITESTMAINLCD_WIN_ID,
#ifdef ENG_SUPPORT
   MMIENG_UITESTSIM_WIN_ID,
#endif
#ifdef SENSORHUB_SUPPORT
   MMIENG_UITEST_SENSORHUB_WIN_ID,
#endif
   MMIENG_UITESTSUBLCD_WIN_ID,
   MMIENG_UITESTRTC_WIN_ID,
   MMIENG_UITESTVIBRATE_WIN_ID,
   MMIENG_UITESTBACKLIGHT_WIN_ID,
   MMIENG_UITEST_NTC_WIN_ID, 
   MMIENG_SHOWVERSION_WIN_ID,
   MMIENG_MOTIONSENSOR_WIN_ID,
   MMIENG_UIREADY_WIN_ID,
   MMIENG_AudioVol_WIN_ID,
   MMIENG_UITESTKEY_WIN_ID,
   MMIENG_FACTORY_RESET_INPUT_PWD_WIN_ID,
   MMIENG_UITEST_MENU_WIN_ID,
   ENG_FREQ_EDIT_WIN_ID,
   POWERONOFF_EDIT_WIN_ID,
   MMIENG_ADD_CONTACT_WIN_ID,
   MMISTK_GETINKEY_WIN_ID,
   MMISTK_GETINPUT_WIN_ID,
   MMIENG_TELE_VOLTE_SET_WIN_ID
};

/*****************************************************************************/
//  Description : get win softkey height
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetWinSoftkeyHeight(
                                           MMI_HANDLE_T win_handle
                                           )
{
#ifdef GUIF_SOFTKEY_PDASTYLE
    if (MMK_GetWinDisplayStyleState(win_handle, WS_HAS_BUTTON_SOFTKEY))
    {
        return MMI_PDA_SOFTKEY_BAR_HEIGHT;
    }
    else
    {
        return 0;
    }
#else
    {
        uint8 ayyay_len = sizeof(s_uitest_win_id)/sizeof(s_uitest_win_id[0]);
        uint8 index = 0;

        while(index < ayyay_len)
        {
            if(win_handle == MMK_ConvertIdToHandle(s_uitest_win_id[index]))
            {
                return MMI_SOFTKEY_BAR_HEIGHT;
            }
            index++;
        }
        return MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
    }
#endif
}

/*****************************************************************************/
// 	Description : copy rect
//	Global resource dependence : 
//  Author:lin.lin
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetSoftKeyTextRect(
                                              GUI_ALIGN_E    align_num,
                                              GUI_RECT_T     rect
                                              )
{
    GUI_RECT_T button_rect;

    switch(align_num)
    {
    case ALIGN_LVMIDDLE:
        button_rect = GUI_CreateRect(
            (uint16)(rect.left + MMI_SOFTKEY_TEXT_HOR_SPACE),
            (uint16)(rect.top),
            (uint16)(rect.right),
            rect.bottom
            );
        break;

    case ALIGN_HVMIDDLE:
        button_rect = GUI_CreateRect(
            rect.left,
            rect.top ,
            rect.right,
            rect.bottom
            );
        break;

    case ALIGN_RVMIDDLE:
        button_rect = GUI_CreateRect(
            (uint16)(rect.left),
            (uint16)(rect.top),
            (uint16)(rect.right - MMI_SOFTKEY_TEXT_HOR_SPACE),
            rect.bottom
            );
        break;

    default:
        button_rect = GUI_CreateRect(
            rect.left,
            rect.top,
            rect.right,
            rect.bottom
            );
        break;
    }

    return button_rect;
}

/*****************************************************************************/
// 	Description : get softkey text font
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMITHEME_GetSoftKeyTextFont(void)
{
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;

    MMIAPISET_GetLanguageType(&lang_type);

//中文与外文用两种不同字体，中文情况下用大字体，外文用小字体
    if (
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
        MMISET_LANGUAGE_SIMP_CHINESE == lang_type
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
     || MMISET_LANGUAGE_TRAD_CHINESE == lang_type
#endif
    )
    {
        return MMI_FULLSCREEN_SOFTKEY_TEXT_FONT;
    }
    else
    {
        return MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT;
    }
}

/*****************************************************************************/
// 	Description : get the softkey bar rect
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetSoftkeyCtrlId(void)
{
	return MMICOMMON_SOFTKEY_CTRL_ID;
}

/*****************************************************************************/
// 	Description : get the softkey button ctrl id
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetSoftkeyButtonCtrlId(
                                            MMI_CTRL_ID_T *button_ctrl_id_arr     //out
                                            )
{
    if (PNULL != button_ctrl_id_arr)
    {
        button_ctrl_id_arr[0] = MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID;
        button_ctrl_id_arr[1] = MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID;
        button_ctrl_id_arr[2] = MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID;
    }
}

/*****************************************************************************/
// Description : get button style index
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL uint32 GetButtonStyleIndex(
                                 GUISOFTKEY_BUTTONSTYLE_E style
                                 )
{
    uint32 style_index = 0;

    switch (style)
    {
    case GUISOFTKEY_BUTTONSTYLE_2BUTTON:
        style_index = 1;
        break;
    case GUISOFTKEY_BUTTONSTYLE_3BUTTON:
        style_index = 2;
        break;
    case GUISOFTKEY_BUTTONSTYLE_1BUTTON:
    default:
        style_index = 0;
        break;
    }
    return style_index;
}

/*****************************************************************************/
// Description : calc softkey button rect
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_CalcSoftkeyButtonRect(
                             const GUI_RECT_T *rect_ptr,    //in
                             GUISOFTKEY_BUTTONSTYLE_E style,    //in
                             GUI_RECT_T *button_rect_ptr   //out
                             )
{
    MMI_BUTTONSOFTKEY_MARGIN_T margin = {0};
    GUI_RECT_T rect_arr[GUISOFTKEY_BUTTON_NUM] = {0};
    uint32 col_num = 0;
    uint32 i = 0;
    uint32 style_index = 0;

    if ((PNULL == rect_ptr) || (PNULL == button_rect_ptr))
    {
        return FALSE;
    }
    if ((style<GUISOFTKEY_BUTTONSTYLE_1BUTTON) || (style>GUISOFTKEY_BUTTONSTYLE_3BUTTON))
    {
        return FALSE;
    }
    style_index = GetButtonStyleIndex(style);
    if (MMITHEME_IsMainScreenLandscape())
    {
        margin = s_buttonsoftkey_h_margin[style_index];
    }
    else
    {
        margin = s_buttonsoftkey_margin[style_index];
    }
    col_num = margin.col_num;
    //middle softkey
    GUI_CreateMatrixRect(
        (GUI_RECT_T*)rect_ptr, 
        margin.h_side_margin, 
        margin.v_side_margin, 
        margin.h_item_space, 
        margin.v_item_space,
        1, 
        col_num, 
        rect_arr
        ); 
    for (i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
    {
        button_rect_ptr[i] = rect_arr[i];
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : get the button softkey style, is called by the softkey control
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetButtonSoftkeyStyle(
                                              MMI_BUTTONSOFTKEY_STYLE_T* softkey_style_ptr
                                              )
{
#ifdef GUIF_SOFTKEY_PDASTYLE
    if (PNULL == softkey_style_ptr)
    {
        return FALSE;
    }
    softkey_style_ptr->bg_image = IMAGE_PDASOFTKEY_BG;
    softkey_style_ptr->font.font = MMI_DEFAULT_BUTTON_FONT;
    softkey_style_ptr->font.color = MMI_BLACK_COLOR;

    softkey_style_ptr->img_id = IMAGE_PUBWIN_BUTTON_PDA;
    softkey_style_ptr->pressed_img_id = IMAGE_PUBWIN_BUTTON_HL_PDA;
    softkey_style_ptr->grayed_img_id = IMAGE_PUBWIN_BUTTON_GRAY_PDA;
#endif

    return (TRUE);
}

/*****************************************************************************/
// 	Description : get common softkey background color
//  Global resource dependence :
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetSoftkeyBg(void)
{
    return MMITHEME_SOFTKEY_BACKGROUND_COLOR;
}

