/****************************************************************************
** File Name:      mmitheme_tips.c                                          *
** Author:         Xiaoqing.lu                                              *
** Date:           2/20/2009                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 2/2009         Xiaoqing.lu      Create                                   *
**                                                                          *
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmitheme_tips.h"
#include "mmi_id.h"
#include "mmi_image.h"
#include "guifont.h"
#include "mmidisplay_data.h"
#include "mmi_theme.h"
#include "guistring.h"
#include "mmi_color.h"
#include "mmi_resource.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMITIPS_START_TIMEROUT      500             // tips开始显示的时间
#define MMITIPS_LIE_TIMEROUT        3000            // tips存在的时间
#define MMITIPS_FONT_TYPE           MMI_DEFAULT_SMALL_FONT    // tips文字类型
#define MMITIPS_FONT_COLOR          MMI_WHITE_COLOR // tips文字颜色

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
//  Description : 获取tips控件的风格信息.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetTipsStyle(
                                     MMITIPS_STYLE_INFO_T *tips_style_ptr
                                     )
{
    BOOLEAN result = FALSE;

    //SCI_ASSERT(PNULL != tips_style_ptr);/*assert verified*/
    if (PNULL == tips_style_ptr)
    {
        return FALSE;
    }

    MMIRES_GetColor(COLOR_RES_BACKGROUND_TIPS, MMK_GetFirstAppletHandle(), &(tips_style_ptr->bg_style.bg_color_info.bg_color) );
    
    // text style
    tips_style_ptr->text_style.align = ALIGN_HVMIDDLE;
    tips_style_ptr->text_style.font_color = MMITIPS_FONT_COLOR;
    tips_style_ptr->text_style.font_type = MMITIPS_FONT_TYPE;
    tips_style_ptr->text_style.str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;
    
    // bg style，当图片和背景都为TRUE时，tips会优先显示图片
    tips_style_ptr->bg_style.tail1_id = 0;
    tips_style_ptr->bg_style.tail2_id = 0;
    tips_style_ptr->bg_style.bg_color_info.is_display = TRUE;
    //tips_style_ptr->bg_style.bg_color_info.bg_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    tips_style_ptr->bg_style.bg_color_info.rect_type = MMITIPS_RECT_RIGHTANGLE;
    tips_style_ptr->bg_style.bg_image_info.bg_image = 0;
    tips_style_ptr->bg_style.bg_image_info.is_display = FALSE;
    
    // timer info
    tips_style_ptr->start_timerout = MMITIPS_START_TIMEROUT;
    tips_style_ptr->display_timerout = MMITIPS_LIE_TIMEROUT;
    tips_style_ptr->timer_is_valid = FALSE;
    
    // 设置边界区域,tips只在这个区域内显示
    tips_style_ptr->edge_rect.left = 0;
    tips_style_ptr->edge_rect.top = 0;
    tips_style_ptr->edge_rect.right = MMITHEME_GetMainScreenRightPixel() - MMITHEME_GetScrollBarWidth();
    tips_style_ptr->edge_rect.bottom = MMI_CLIENT_RECT_BOTTOM;
    
    // 设置显示基准
    tips_style_ptr->tips_align = MMITIPS_ALIGN_RIGHT;
    
    tips_style_ptr->min_height = MMITIPS_RECT_MIN_HEIGHT;
    tips_style_ptr->max_width = MMITIPS_RECT_MAX_WIDTH;
    // 返回值
    result = TRUE;
    return result;
}

/*****************************************************************************/
// 	Description : get the tips ctrl id
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetTipsCtrlId(void)
{
	return MMICOMMON_TIPS_CTRL_ID;
}

/*****************************************************************************/
//    Description : 获取默认提示信息字符串格式
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetTipsStrInfo(GUI_RECT_T  *clip_rect_ptr, GUISTR_STYLE_T  *style_ptr, GUISTR_STATE_T *state, MMI_IMAGE_ID_T   *bg_img_ptr)
{
    uint16           lcd_width          = 0;
    uint16           lcd_height         = 0;
    
    GUISTR_STATE_T   text_state         = GUISTR_STATE_ALIGN | GUISTR_STATE_EFFECT | GUISTR_STATE_EFFECT | GUISTR_STATE_ELLIPSIS;
    GUISTR_STYLE_T   text_style         = {0};
    GUI_RECT_T       PromptText_rect    = {0};

    if (PNULL != clip_rect_ptr)
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
        
        PromptText_rect.left   = 0;
        PromptText_rect.right  = lcd_width;
        PromptText_rect.top    = lcd_height * 3 / 4;
        PromptText_rect.bottom = PromptText_rect.top + 25;

        *clip_rect_ptr = PromptText_rect;
    }

    if (PNULL != style_ptr)
    {
        text_style.align      = ALIGN_HVMIDDLE; 
        text_style.effect     = FONT_EFFECT_NONE;
        text_style.angle      = ANGLE_0;
        text_style.font       = SONG_FONT_16;
        text_style.font_color = MMI_DEFAULT_TEXT_COLOR;

        *style_ptr     = text_style;
    }

    if (PNULL != state)
    {
        *state         = text_state;
    }

    if (PNULL != bg_img_ptr)
    {
        *bg_img_ptr = IMAGE_COMMON_TIPS_BG;
    }
}
