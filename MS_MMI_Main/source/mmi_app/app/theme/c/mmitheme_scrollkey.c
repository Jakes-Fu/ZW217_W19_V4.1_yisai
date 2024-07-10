/*****************************************************************************
** File Name:      mmitheme_scrollkey.c                                      *
** Author:         hua.fang                                                  *
** Date:           08/01/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011        hua.fang          Create                                   *
******************************************************************************/

#ifdef GUIF_SCROLLKEY

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmitheme_scrollkey.h"
#include "mmi_position.h"
#include "mmitheme_pos.h"
#include "mmidisplay_data.h"
#include "mmi_theme.h"
#include "mmi_id.h"
#include "guiscrollkey.h"
#include "mmi_resource.h"
#include "mmi_image.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL GUI_RECT_T GetScrollkeyRect(
    uint16    scrollkey_index
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get the scrollkey style, is called by the scrollkey control
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetScrollkeyStyle(
                                        MMI_SCROLLKEY_STYLE_T* scrollkey_style_ptr,
                                        GUISCROLLSKIN_INDEX    left_scroll_skin_idx,
                                        GUISCROLLSKIN_INDEX    right_scroll_skin_idx
	                                    )
{

	GUISCROLLKEY_SKIN_T Scrollkey_Skin_Table = 
	{
		{
			{IMAGE_IDLE_KEYLOCK_TOG_L_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_L_DOWN, IMAGE_IDLE_KEYLOCK_TOG_L_GREEN, IMAGE_IDLE_KEYLOCK_TOG_GREEN_POINT},
			{IMAGE_IDLE_KEYLOCK_TOG_L_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_L_DOWN, IMAGE_IDLE_KEYLOCK_TOG_L_GRAY, IMAGE_IDLE_KEYLOCK_TOG_GRAY_POINT},
			{IMAGE_IDLE_KEYLOCK_TOG_L_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_L_DOWN, IMAGE_IDLE_KEYLOCK_TOG_L_YELLOW, IMAGE_IDLE_KEYLOCK_TOG_YELLOW_POINT},
#if  !defined(MMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG)&& !defined(MMI_LOW_MEMORY_RESOURCE)

			{IMAGE_IDLE_KEYLOCK_TOG_L_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_L_DOWN, IMAGE_IDLE_KEYLOCK_TOG_L_RED, IMAGE_IDLE_KEYLOCK_TOG_RED_POINT},
#endif
		},
		{
			{IMAGE_IDLE_KEYLOCK_TOG_R_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_R_DOWN, IMAGE_IDLE_KEYLOCK_TOG_R_GREEN, IMAGE_IDLE_KEYLOCK_TOG_GREEN_POINT},
			{IMAGE_IDLE_KEYLOCK_TOG_R_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_R_DOWN, IMAGE_IDLE_KEYLOCK_TOG_R_GRAY, IMAGE_IDLE_KEYLOCK_TOG_GRAY_POINT},
			{IMAGE_IDLE_KEYLOCK_TOG_R_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_R_DOWN, IMAGE_IDLE_KEYLOCK_TOG_R_YELLOW, IMAGE_IDLE_KEYLOCK_TOG_YELLOW_POINT},
#if !defined(MMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG)&& !defined(MMI_LOW_MEMORY_RESOURCE)
			{IMAGE_IDLE_KEYLOCK_TOG_R_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_R_DOWN, IMAGE_IDLE_KEYLOCK_TOG_R_RED, IMAGE_IDLE_KEYLOCK_TOG_RED_POINT},
#endif
		}
	};

    //SCI_PASSERT(PNULL != scrollkey_style_ptr, ("MMITHEME_GetScrollkeyStyle: the param is PNULL!")); /*assert verified*/
    if (PNULL == scrollkey_style_ptr)
    {
        return FALSE;
    }
    
    SCI_MEMSET(scrollkey_style_ptr, 0x00, sizeof(MMI_SCROLLKEY_STYLE_T));

    scrollkey_style_ptr->bg_type = GUI_SCROLLKEY_BG_NULL;

	scrollkey_style_ptr->bg_image 	= 0;  //背景图片
	scrollkey_style_ptr->bg_color 	= MMI_BACKGROUND_COLOR;

	scrollkey_style_ptr->font_color	= MMI_GRAY_WHITE_COLOR;	 //字体颜色
	scrollkey_style_ptr->pressed_font_color	= MMI_GRAY_WHITE_COLOR; 	//字体颜色
	scrollkey_style_ptr->font 		= MMI_FULLSCREEN_SCROLLKEY_TEXT_FONT; 		  //字体，这个参加暂时没用上，softkey实时去取
	scrollkey_style_ptr->frameType	= BUTTON_FRAME_NULL;		//边框风格：矩形、3D矩形、圆角矩形、无边框
	scrollkey_style_ptr->char_space	= 0;
	scrollkey_style_ptr->first_icon_offset   = MMI_SCROLLKEYBAR_FIRST_ICON_POS;		  // 第一个图标的偏移量
	scrollkey_style_ptr->second_icon_offset  = MMI_SCROLLKEYBAR_SECOND_ICON_POS;		  // 第二个图标的偏移量

	scrollkey_style_ptr->skb_button[LEFTSCROLL_BUTTON].icon_up_id 	     = Scrollkey_Skin_Table[LEFTSCROLL_BUTTON][left_scroll_skin_idx].scroll_bar_up_id;
	scrollkey_style_ptr->skb_button[LEFTSCROLL_BUTTON].icon_down_id	     = Scrollkey_Skin_Table[LEFTSCROLL_BUTTON][left_scroll_skin_idx].scroll_bar_down_id;
	scrollkey_style_ptr->skb_button[LEFTSCROLL_BUTTON].icon_confirm_id   = Scrollkey_Skin_Table[LEFTSCROLL_BUTTON][left_scroll_skin_idx].scroll_bar_confirm_id;
	scrollkey_style_ptr->skb_button[LEFTSCROLL_BUTTON].confirm_point_id  = Scrollkey_Skin_Table[LEFTSCROLL_BUTTON][right_scroll_skin_idx].scroll_point_id;			
	scrollkey_style_ptr->skb_button[RIGHTSCROLL_BUTTON].icon_up_id 	     = Scrollkey_Skin_Table[RIGHTSCROLL_BUTTON][right_scroll_skin_idx].scroll_bar_up_id;
	scrollkey_style_ptr->skb_button[RIGHTSCROLL_BUTTON].icon_down_id	 = Scrollkey_Skin_Table[RIGHTSCROLL_BUTTON][right_scroll_skin_idx].scroll_bar_down_id;
	scrollkey_style_ptr->skb_button[RIGHTSCROLL_BUTTON].icon_confirm_id  = Scrollkey_Skin_Table[RIGHTSCROLL_BUTTON][right_scroll_skin_idx].scroll_bar_confirm_id;
	scrollkey_style_ptr->skb_button[RIGHTSCROLL_BUTTON].confirm_point_id = Scrollkey_Skin_Table[RIGHTSCROLL_BUTTON][left_scroll_skin_idx].scroll_point_id;
	
	scrollkey_style_ptr->skb_button[LEFTSCROLL_BUTTON].rect = GetScrollkeyRect(LEFTSCROLL_BUTTON);
	scrollkey_style_ptr->skb_button[RIGHTSCROLL_BUTTON].rect = GetScrollkeyRect(RIGHTSCROLL_BUTTON);
	
	scrollkey_style_ptr->skb_button[LEFTSCROLL_BUTTON].is_show = TRUE;
	scrollkey_style_ptr->skb_button[RIGHTSCROLL_BUTTON].is_show = TRUE;
    return (TRUE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetScrollkeySkin(
    GUISCROLLKEY_SKIN_T skin_table
    )
{

	GUISCROLLKEY_SKIN_T Scrollkey_Skin_Table = 
	{
		{
			{IMAGE_IDLE_KEYLOCK_TOG_L_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_L_DOWN, IMAGE_IDLE_KEYLOCK_TOG_L_GREEN, IMAGE_IDLE_KEYLOCK_TOG_GREEN_POINT},
			{IMAGE_IDLE_KEYLOCK_TOG_L_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_L_DOWN, IMAGE_IDLE_KEYLOCK_TOG_L_GRAY, IMAGE_IDLE_KEYLOCK_TOG_GRAY_POINT},
			{IMAGE_IDLE_KEYLOCK_TOG_L_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_L_DOWN, IMAGE_IDLE_KEYLOCK_TOG_L_YELLOW, IMAGE_IDLE_KEYLOCK_TOG_YELLOW_POINT},
#if !defined(MMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG)&& !defined(MMI_LOW_MEMORY_RESOURCE)
			{IMAGE_IDLE_KEYLOCK_TOG_L_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_L_DOWN, IMAGE_IDLE_KEYLOCK_TOG_L_RED, IMAGE_IDLE_KEYLOCK_TOG_RED_POINT},
#endif
		},
		{
			{IMAGE_IDLE_KEYLOCK_TOG_R_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_R_DOWN, IMAGE_IDLE_KEYLOCK_TOG_R_GREEN, IMAGE_IDLE_KEYLOCK_TOG_GREEN_POINT},
			{IMAGE_IDLE_KEYLOCK_TOG_R_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_R_DOWN, IMAGE_IDLE_KEYLOCK_TOG_R_GRAY, IMAGE_IDLE_KEYLOCK_TOG_GRAY_POINT},
			{IMAGE_IDLE_KEYLOCK_TOG_R_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_R_DOWN, IMAGE_IDLE_KEYLOCK_TOG_R_YELLOW, IMAGE_IDLE_KEYLOCK_TOG_YELLOW_POINT},
#if !defined(MMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG)&& !defined(MMI_LOW_MEMORY_RESOURCE)
			{IMAGE_IDLE_KEYLOCK_TOG_R_NORMAL_UP, IMAGE_IDLE_KEYLOCK_TOG_R_DOWN, IMAGE_IDLE_KEYLOCK_TOG_R_RED, IMAGE_IDLE_KEYLOCK_TOG_RED_POINT},
#endif
		}
	};

    SCI_MEMCPY(skin_table, Scrollkey_Skin_Table, sizeof(Scrollkey_Skin_Table));
}

/*****************************************************************************/
// 	Description : get the height of the scrollkey area, is called by the MMI Kernel
//	Global resource dependence : 
//  Author:Robert.Lu
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMITHEME_GetScrollkeyHeight(void)
{
	return MMI_FULLSCREEN_SCROLLKEY_BAR_HEIGHT;	
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMITHEME_GetScrollkeyOffsetFromBottom(void)
{
	return MMI_SCROLLKEY_OFFSET_FROM_BOTTOM;	
}

/*****************************************************************************/
// 	Description : get scrollkey text font
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMITHEME_GetScrollKeyTextFont(void)
{
	return MMI_FULLSCREEN_SCROLLKEY_TEXT_FONT;
}

/*****************************************************************************/
// 	Description : get the scrollkey bar rect
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetScrollkeyCtrlId(void)
{
	return MMICOMMON_SCROLLKEY_CTRL_ID;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T GetScrollkeyRect(
    uint16    scrollkey_index
)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    uint16          scrollkey_width = 0;
    GUI_RECT_T      scrollkey_rect = {0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    //get scrollkey width
    scrollkey_width = (uint16)(lcd_width / GUISCROLLKEY_BUTTON_NUM);

    switch (scrollkey_index)
    {
        case LEFTSCROLL_BUTTON:
            scrollkey_rect.left   = 0;
            scrollkey_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
            scrollkey_rect.right  = (int16)(scrollkey_width - 1);
            scrollkey_rect.bottom = (int16)(lcd_height - 1);
            break;

        case RIGHTSCROLL_BUTTON:
            scrollkey_rect.left   = (int16)(scrollkey_width * GUISCROLLKEY_BUTTON_NUM);
            scrollkey_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);
            scrollkey_rect.right  = (int16)(lcd_width - 1);
            scrollkey_rect.bottom = (int16)(lcd_height -1);
            break;

        default:
            SCI_PASSERT(FALSE, ("GetScrollkeyRect: scrollkey_index %d is error!", scrollkey_index));   /*assert verified*/
            break;
    }

    return (scrollkey_rect);
}

#endif

