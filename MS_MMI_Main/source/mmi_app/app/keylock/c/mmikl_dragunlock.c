/*****************************************************************************
** File Name:      mmikl_dragunlock.c                                        *
** Author:         jian.ma                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe drag icon or ring unlock    *
**                 function                                                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create
******************************************************************************/

#ifndef _MMIKL_DRAGUNLOCK_C_

#include "mmi_app_keylock_trc.h"
#ifdef MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmikl_dragunlock.h"
#include "mmikl_effectmain.h"
#include "mmidisplay_data.h"
#include "mmi_textfun.h"
#include "mmi_keylock_position.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmi_id.h"
#include "ui_layer.h"
#include "guistring.h"
#include "guires.h"
#include "graphics_draw.h"
#include "mmikl_export.h"
#include "mmk_tp.h"
#include "mmk_timer.h"
#include "mmikl_internal.h"
#include "mmiset_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define GET_RECT(x)  s_kl_effect_custom->x.rect
#define GET_IMG(x)    s_kl_effect_custom->x.img_id
#define GET_PROG_NUM s_kl_effect_custom->program_num
//#define GET_PROG(id) s_kl_effect_custom->program_id[id]
#define GET_ICON_RECT(i)    s_kl_effect_custom->icons[i].rect
#define GET_ICON_IMG(i)    s_kl_effect_custom->icons[i].img_id

#define GET_LAYER(layer, my_layer)  \
{   \
    if(my_layer->block_id != 0)  \
    {   \
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(my_layer))   \
        {   \
            layer = my_layer;   \
        }   \
    }   \
}

//#define DISP_IMG(element)       GUIRES_DisplayImg(PNULL, &GET_RECT(element), PNULL, MMIKL_CLOCK_DISP_WIN_ID, GET_IMG(element),&s_kl_effect6_info.##element##_layer)
#define DISP_IMG(element)       GUIRES_DisplayImg(PNULL, &GET_RECT(element), PNULL, MMIKL_CLOCK_DISP_WIN_ID, GET_IMG(element), GetLayerEffect6(&s_kl_effect6_info.element##_layer))
#define DISP_ICON_IMG(i)         MMIKL_DispImg(&GET_ICON_RECT(i), MMIKL_CLOCK_DISP_WIN_ID, GET_ICON_IMG(i), GetLayerEffect6(&s_kl_effect6_info.icon_panel_layer), TRUE)
#define DISP_ICON_IMG_BG(i)   GUIRES_DisplayImg(PNULL, &GET_ICON_RECT(i), PNULL, MMIKL_CLOCK_DISP_WIN_ID, GET_IMG(icon_bg),GetLayerEffect6(&s_kl_effect6_info.icon_panel_layer))  
#define DISP_IMG_GEN(rect, img, layer)        GUIRES_DisplayImg(PNULL, rect, PNULL, MMIKL_CLOCK_DISP_WIN_ID, img, layer);

#define KEY_LOCK_EFFECT6_PANEL_FADE_FRAME_NUM  8
#define KEY_LOCK_EFFECT6_ZOOM_FRAME_NUM     8
#define KEY_LOCK_EFFECT6_MOVE_ICON_FADE_FRAME_NUM   10
#define KEY_LOCK_EFFECT6_TARGET_MOVE_FRAME_NUM   8

#define KEY_LOCK_EFFECT6_TP_MOVE_TIME   20
#define KEY_LOCK_EFFECT6_ICON_PANEL_FADE_TIME  10
#define KEY_LOCK_EFFECT6_WAITINGE_ICON_FADE_TIME    500
#define KEY_LOCK_EFFECT6_MOVE_ICON_FADE_TIME    20
#define KEY_LOCK_EFFECT6_CLEAR_TARGET_TIME      20
#define KEY_LOCK_EFFECT6_MOVE_TARGET_TIME   10
#define KEY_LOCK_EFFECT6_WAITING_ZOOM_TARGET_TIME 20
#define KEY_LOCK_EFFECT6_ZOOM_TARGET_TIME        10

#define KEY_LOCK_EFFECT6_PROG_MAX   4

typedef struct
{
    GUI_LCD_DEV_INFO main_layer;    
    GUI_LCD_DEV_INFO target_layer;
    GUI_LCD_DEV_INFO move_icon_layer;
    GUI_LCD_DEV_INFO icon_panel_layer;
    GUI_LCD_DEV_INFO time_layer;
    GUI_LCD_DEV_INFO zoom_layer;
    GUI_LCD_DEV_INFO lcd_layer;

    uint8 effect_timer;
    uint8 tp_move_timer;
}KEY_LOCK_EFFECT6_INFO_T;

typedef enum
{
    MMI_KL_ELEMENT_NONE,
    MMI_KL_ELEMENT_STR,
    MMI_KL_ELEMENT_IMG,
    
    MMI_KL_ELEMENT_MAX
}MMI_KL_ELEMENT_E;

typedef struct
{
    wchar name[MMI_KL_PATH_LEN_MAX];
    uint16 len;
}MMI_KL_PATH_T;

typedef struct
{
    wchar str[MMI_KL_STR_LEN_MAX];
    uint16 str_len;
}MMI_KL_STR_T;

typedef struct
{
    MMK_KL_ELEMENT_BASE
}MMI_KL_ELEMENT_NONE_T;

typedef struct
{
    MMK_KL_ELEMENT_BASE
    MMI_KL_STR_T string;
}MMI_KL_ELEMENT_STR_T;

typedef struct
{
    MMK_KL_ELEMENT_BASE
    MMI_IMAGE_ID_T img_id;
    MMI_KL_PATH_T path;
}MMI_KL_ELEMENT_IMG_T;

typedef union
{
    MMI_KL_ELEMENT_NONE_T none;
    MMI_KL_ELEMENT_STR_T str_element;
    MMI_KL_ELEMENT_IMG_T img_element;
}MMI_KL_ELEMENT_U;
typedef struct
{
    MMI_KL_ELEMENT_IMG_T    bottom_bg;
    MMI_KL_ELEMENT_IMG_T    target;
    MMI_KL_ELEMENT_IMG_T    target_focused;
    
    MMI_KL_ELEMENT_IMG_T    icons[KEY_LOCK_EFFECT6_PROG_MAX];
    MMI_KL_ELEMENT_IMG_T    icon_bg;
    
    MMI_KL_ELEMENT_IMG_T    icon_panel;
    MMI_KL_ELEMENT_IMG_T    time;
    MMI_KL_ELEMENT_STR_T    remind;
    uint8 program_id[KEY_LOCK_EFFECT6_PROG_MAX];
    uint8 program_num;

    uint16 target_rise;
    uint16 target_width;
    uint16 target_offset;   
    
}KEY_LOCK_EFFECT6_CUSTOM_T;

LOCAL KEY_LOCK_EFFECT6_INFO_T s_kl_effect6_info = {0};
LOCAL KEY_LOCK_EFFECT6_CUSTOM_T *s_kl_effect_custom = PNULL;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
  
/**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/******************************************************************************/
//  Description : Get the icon index which tp down in.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTpDownInIconEffect6(GUI_POINT_T pos, uint8 *index);
/******************************************************************************/
//  Description : Get the icon index which tp down in.
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN JudgeTpDownInRingRect(GUI_POINT_T tp_pos);

/*****************************************************************************/
//  Description :  Append layer to blt layer array.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendLayer(GUI_LCD_DEV_INFO *layer);
/******************************************************************************/
//  Description : Init custom parameter.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  InitCustomParamEffect6(void)
{
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 img_width = 0, img_height = 0;
    GUI_RECT_T rect = {0};
    uint16 interval = 0;
    uint i = 0;
    s_kl_effect_custom = SCI_ALLOCA(sizeof(KEY_LOCK_EFFECT6_CUSTOM_T));
    if( PNULL == s_kl_effect_custom)
    {
        return FALSE;
    }
    SCI_MEMSET(s_kl_effect_custom, 0, sizeof(KEY_LOCK_EFFECT6_CUSTOM_T));

    s_kl_effect_custom->program_num = KEY_LOCK_EFFECT6_PROG_MAX;
    for(i = 0; i < KEY_LOCK_EFFECT6_PROG_MAX; i++)
    {
        s_kl_effect_custom->program_id[i] = i;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    /* bottom bg */    
    s_kl_effect_custom->bottom_bg.img_id = IMAGE_KL_EFFECT6_BOTTOM_BG;
    s_kl_effect_custom->icon_bg.img_id = IMAGE_KL_EFFECT6_ICON_BG;
    
    s_kl_effect_custom->icons[0].img_id = IMAGE_MAINMENU_ICON_DIAL;
    s_kl_effect_custom->icons[1].img_id = IMAGE_MAINMENU_ICON_AUDIO_PLAYER;
    s_kl_effect_custom->icons[2].img_id = IMAGE_MAINMENU_ICON_CAMERA;
    s_kl_effect_custom->icons[3].img_id = IMAGE_MAINMENU_ICON_MESSAGE;
    GUIRES_GetImgWidthHeight(&img_width,&img_height, s_kl_effect_custom->bottom_bg.img_id, MMIKL_CLOCK_DISP_WIN_ID);
    rect.left = (lcd_width - img_width) / 2;
    rect.right = rect.left + img_width;
    rect.top = lcd_height - 1 - img_height;
    rect.bottom = lcd_height - 1;
    s_kl_effect_custom->bottom_bg.rect = rect;
    img_height = 0;
    GUIRES_GetImgWidthHeight(&img_width,&img_height, s_kl_effect_custom->icon_bg.img_id, MMIKL_CLOCK_DISP_WIN_ID);
    rect.left = (lcd_width - img_width) / 2;
    rect.right = rect.left + img_width;
    rect.top = lcd_height - 1 - img_height;
    rect.bottom = lcd_height - 1;
    s_kl_effect_custom->icon_bg.rect = rect;

    /* move target */
    s_kl_effect_custom->target.img_id = IMAGE_KL_EFFECT6_ROUND_EMPTY;
    GUIRES_GetImgWidthHeight(&img_width,&img_height, s_kl_effect_custom->target.img_id, MMIKL_CLOCK_DISP_WIN_ID);
    rect.left = (lcd_width - img_width) / 2;
    rect.right = rect.left + img_width;
    rect.top = lcd_height - 1 - img_height / 3 ;    /* display 1/3 of the image */
    rect.bottom = rect.top + img_height;
    s_kl_effect_custom->target.rect = rect;
    s_kl_effect_custom->target_rise = KEY_LOCK_EFFECT6_TARGET_RISE;
    s_kl_effect_custom->target_width = KEY_LOCK_EFFECT6_TARGET_WIDTH;
    s_kl_effect_custom->target_offset = KEY_LOCK_EFFECT6_TARGET_OFFSET;

    /* target focused */
    s_kl_effect_custom->target_focused.img_id = IMAGE_KL_EFFECT6_ROUND;
    s_kl_effect_custom->target_focused.rect = rect;

    /* icons panel */
    s_kl_effect_custom->icon_panel.img_id = 0;
    rect.left = 0;
    rect.right = lcd_width - 1;
    rect.top = KEY_LOCK_EFFECT6_PANEL_Y;
    rect.bottom = rect.top + KEY_LOCK_EFFECT6_PANEL_HEIGHT;
    s_kl_effect_custom->icon_panel.rect = rect;

    /* icons -----------------------------------------------------------------*/
      
    /* 1. icon 0 and icon BG */
    GUIRES_GetImgWidthHeight(&img_width,&img_height, s_kl_effect_custom->icon_bg.img_id, MMIKL_CLOCK_DISP_WIN_ID);
    rect.left = KEY_LOCK_EFFECT6_ICONS_MARGIN_LEFT;
    rect.right = rect.left + img_width;
    rect.bottom = s_kl_effect_custom->icon_panel.rect.bottom ;
    rect.top = rect.bottom - img_height;
    s_kl_effect_custom->icons[0].rect = rect;
    
    /* 去掉左右空白，再去掉1张图片宽度，有三个空白间隔*/    
    interval = (lcd_width - rect.left*2 - img_width) / 3;

    /* 2. icon 1 */
    rect.left += interval;
    rect.right = rect.left + img_width;
    rect.top = s_kl_effect_custom->icon_panel.rect.top;
    rect.bottom = rect.top + img_height;
    s_kl_effect_custom->icons[1].rect = rect;

    /* 3. icon 2 */
    rect.left += interval;
    rect.right = rect.left + img_width;
    s_kl_effect_custom->icons[2].rect = rect;

    /* 4. icon 3 */
    rect.left += interval;
    rect.right = rect.left + img_width;
    rect.bottom = s_kl_effect_custom->icon_panel.rect.bottom ;
    rect.top = rect.bottom - img_height;
    s_kl_effect_custom->icons[3].rect = rect;
    /* icons end -----------------------------------------------------------------*/

    s_kl_effect_custom->time.img_id = 0;
    GUIRES_GetImgWidthHeight(&img_width,&img_height, IMAGE_KL_EFFECT6_DIGIT0, MMIKL_CLOCK_DISP_WIN_ID);
    
    rect.left = (lcd_width - img_width * 6) / 2;
    rect.right = lcd_width - rect.left;
    rect.top = 50;
    rect.bottom = rect.top + img_height;
    s_kl_effect_custom->time.rect = rect;

    return TRUE;
}

/******************************************************************************/
//  Description : Create layer of effect6.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateLayerEffect6(MMI_WIN_ID_T win_id, GUI_RECT_T rect)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 width = 0, height = 0;
    //uint16 image_width = 0, image_height = 0;
    //UILAYER_APPEND_BLT_T append_layer = {0};
    
    if(rect.top < rect.bottom && rect.left < rect.right)
    {
        width = rect.right - rect.left + 1;
        height = rect.bottom - rect.top + 1;

        MMI_KL_CREATE_LAYER(create_info, win_id, width, height, layer, result);
        
        if(UILAYER_RESULT_SUCCESS == result)
        {
            AppendLayer(&layer);
            UILAYER_SetLayerPosition(&layer, rect.left, rect.top);
            UILAYER_Clear(&layer);
        }
    }
    return layer;
}

/******************************************************************************/
//  Description : Get the real layer info, if a layer in whose window is not active, return the main layer.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO *GetLayerEffect6(GUI_LCD_DEV_INFO *my_layer)
{
    GUI_LCD_DEV_INFO *layer = &s_kl_effect6_info.lcd_layer;
    if(MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
    {
        GET_LAYER(layer, my_layer);
    }
    return layer;
}

/******************************************************************************/
//  Description : Init the all param.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitParamEffect6(void)
{
    GUI_RECT_T rect = {0}; 
    uint16 lcd_width = 0, lcd_height = 0;
    MMI_WIN_ID_T win_id = MMIKL_CLOCK_DISP_WIN_ID;
    if(InitCustomParamEffect6())
    {
        SCI_MEMSET(&s_kl_effect6_info, 0, sizeof(s_kl_effect6_info));

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
        rect.right = lcd_width - 1;
        rect.bottom = lcd_height - 1;

        if (PNULL != s_kl_effect_custom)
        {
        //    s_kl_effect6_info.main_layer = CreateLayerEffect6(win_id, rect);
            s_kl_effect6_info.icon_panel_layer = CreateLayerEffect6(win_id, GET_RECT(icon_panel));
            s_kl_effect6_info.target_layer = CreateLayerEffect6(win_id, GET_RECT(target));
            s_kl_effect6_info.move_icon_layer = CreateLayerEffect6(win_id, GET_RECT(icon_bg));
            s_kl_effect6_info.time_layer = s_kl_effect6_info.main_layer;
            return TRUE;
        }
    }
    return FALSE;

}

/******************************************************************************/
//  Description : Display icon panel.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayIconPanelEffect6(void)
{
    uint32 i = 0;
    UILAYER_Clear(&s_kl_effect6_info.icon_panel_layer);
    if (PNULL != s_kl_effect_custom)
    {
        for(i = 0; i < GET_PROG_NUM; i++)
        {
            DISP_ICON_IMG_BG(i);        
            DISP_ICON_IMG(i);
        }
    }
}

/******************************************************************************/
//  Description : Display icon without display focus icon.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayIconWithoutFocusEffect6(uint8 focus_index)
{
    uint32 i = 0;
    UILAYER_Clear(&s_kl_effect6_info.icon_panel_layer);
    if (PNULL != s_kl_effect_custom)
    {
        for(i = 0; i < GET_PROG_NUM; i++)
        {
            if(i != focus_index)
            {
                DISP_ICON_IMG(i);
            }
        } 
    }
    //UILAYER_StoreLayerUpdateRect(&GET_RECT(icon_panel));
}

/******************************************************************************/
//  Description : Display moveing icon.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayMoveIconEffect6(uint8 index)
{
    UILAYER_Clear(&s_kl_effect6_info.move_icon_layer);
    if (PNULL != s_kl_effect_custom)
    {
        UILAYER_SetLayerPosition(&s_kl_effect6_info.move_icon_layer, GET_ICON_RECT(index).left, GET_ICON_RECT(index).top);
        MMIKL_DispImg(&GET_ICON_RECT(index), MMIKL_CLOCK_DISP_WIN_ID, GET_ICON_IMG(index), &s_kl_effect6_info.move_icon_layer, TRUE);
    }
}

/******************************************************************************/
//  Description : Clear move icon.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void ClearMoveIconEffect6(void)
{
    UILAYER_Clear(&s_kl_effect6_info.move_icon_layer);
}

/******************************************************************************/
//  Description : Display bottom back ground.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayBottomBgEffect6(void)
{
    //GUI_LCD_DEV_INFO layer = {0};
    if (PNULL != s_kl_effect_custom)
    {
        GUIRES_DisplayImg(PNULL, &GET_RECT(bottom_bg), PNULL, MMIKL_CLOCK_DISP_WIN_ID, GET_IMG(bottom_bg), GetLayerEffect6(&s_kl_effect6_info.main_layer));
    }
}

/******************************************************************************/
//  Description : Display targe.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayTargetEffect6(void)
{
    UILAYER_Clear(&s_kl_effect6_info.target_layer);
    if (PNULL != s_kl_effect_custom)
    {
        UILAYER_SetLayerPosition(&s_kl_effect6_info.target_layer, GET_RECT(target).left, GET_RECT(target).top);
        DISP_IMG(target);
    }
    //UILAYER_StoreLayerUpdateRect(&s_kl_effect6_info.target_layer);
}

/******************************************************************************/
//  Description : Display digital clock.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayDigitalClockEffect6(void)
{
    if (PNULL != s_kl_effect_custom)
    {
        MMIKL_DisplayDigitalClock(GetLayerEffect6(&s_kl_effect6_info.time_layer), GET_RECT(time), IMAGE_KL_EFFECT6_DIGIT0);
    }
}

/*****************************************************************************/
//  Description : Display Missed event of effect6.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
void DisplayMissedEventEffect6(void)
{
    GUI_RECT_T rect = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    GUISTR_STYLE_T style= {0};
    MMI_STRING_T string = {0};
    GUISTR_INFO_T info = {0};    
    GUI_LCD_DEV_INFO lcd_dev = {0};
    uint16 rect_height = 0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    style.font = MMI_DEFAULT_NORMAL_FONT;

    MMI_GetLabelTextByLang(TXT_KL_MISS_CALLS, &string);
    
    GUISTR_GetStringInfo(&style, &string, GUISTR_STATE_NONE, &info);
    
    rect_height = info.height * 2 + 8;

    rect.right = lcd_width - 1;
    
    if (PNULL != s_kl_effect_custom)
    {
        rect.top = GET_RECT(time).bottom + (lcd_height/2 - GET_RECT(time).bottom - rect_height)/2;
    }
    rect.bottom = rect.top + rect_height;

    //GET_LAYER(lcd_dev, s_kl_effect6_info.main_layer);
    lcd_dev = *GetLayerEffect6(&s_kl_effect6_info.main_layer);
    MMIKL_DisplayMissedEvent(MMIKL_CLOCK_DISP_WIN_ID, &rect, &lcd_dev);
}

/******************************************************************************/
//  Description : Start tp move timer.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void StartTpMoveTimerEffect6(void)
{
    START_TIMER(s_kl_effect6_info.tp_move_timer, KEY_LOCK_EFFECT6_TP_MOVE_TIME);
}

/******************************************************************************/
//  Description : Stop tp move timer.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void StopTpMoveTimerEffect6(void)
{
    STOP_TIMER(s_kl_effect6_info.tp_move_timer);
}

/******************************************************************************/
//  Description : Start effect move timer.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void StartEffectTimerEffect6(uint32 time)
{
    START_TIMER(s_kl_effect6_info.effect_timer, time);    
}

/******************************************************************************/
//  Description : Stop tp move timer.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void StopEffectTimerEffect6(void)
{
    STOP_TIMER(s_kl_effect6_info.effect_timer);    
}

/******************************************************************************/
//  Description : Set move icon layer position.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetMoveIconLayeEffect6(uint8 index, int16 offset_x, int16 offset_y)
{
    if (PNULL != s_kl_effect_custom)
    {
        UILAYER_SetLayerPosition(&s_kl_effect6_info.move_icon_layer, GET_ICON_RECT(index).left + offset_x, GET_ICON_RECT(index).top + offset_y);
    }
}

/******************************************************************************/
//  Description : Get the icon index which tp down in.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTpDownInIconEffect6(GUI_POINT_T pos, uint8 *index)
{
    uint i = 0;
    BOOLEAN res = FALSE;
    if (PNULL != s_kl_effect_custom)
    {
        for(i = 0; i < GET_PROG_NUM; i++)
        {
            if(GUI_PointIsInRect(pos, GET_ICON_RECT(i)))
            {
                res = TRUE;
                *index = i;
                break;
            }
        }
    }
    return res;
}

LOCAL BOOLEAN JudgeTpDownInRingRect(GUI_POINT_T tp_pos)
{
    BOOLEAN res = FALSE;
    if (PNULL != s_kl_effect_custom)
    {
        if(GUI_PointIsInRect(tp_pos, GET_RECT(target)))
        {
            res = TRUE;
        }
    }
    return res;
}

LOCAL BOOLEAN GetIconIndexInRingRect(int16 rect_left,int16 rect_top,uint8 *index)
{
    uint16 i = 0;
    int16 offset = 0;

    BOOLEAN res = FALSE;
    GUI_RECT_T icon_rect = {0};
   
    if (PNULL != s_kl_effect_custom)
    {
        offset =  (s_kl_effect_custom->target.rect.right - s_kl_effect_custom->target.rect.left -
        s_kl_effect_custom->target_width)/2;
    
        for(i = 0; i < GET_PROG_NUM; i++)
        {
            icon_rect =  GET_ICON_RECT(i);
            if(icon_rect.left>=(rect_left + offset)&&
                icon_rect.right<=(rect_left + offset+s_kl_effect_custom->target_width)&&
                icon_rect.top >= (rect_top+offset)&&
                icon_rect.bottom<=(rect_top+offset+s_kl_effect_custom->target_width))
            {
                res = TRUE;
                *index = i;
                break;
            }
        }
    }
    return res;
}
/******************************************************************************/
//  Description : Fade icon panel.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void FadeIconPanelEffect6(uint16 frame_index)
{
    GUI_LCD_DEV_INFO layer = {0};
    uint8 alpha = 0;
    
    layer = s_kl_effect6_info.icon_panel_layer;
    alpha = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, MMI_KL_COLOR_ALPHA_MAX, KEY_LOCK_EFFECT6_PANEL_FADE_FRAME_NUM, frame_index);

    UILAYER_WeakLayerAlpha(&layer, MMI_KL_COLOR_ALPHA_MAX - alpha);
}

/******************************************************************************/
//  Description : Fade move icon.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void FadeMoveIconEffect6(uint16 frame_index)
{
    GUI_LCD_DEV_INFO layer = {0};
    uint8 alpha = 0;
    
    layer = s_kl_effect6_info.move_icon_layer;
    alpha = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_3, 255, KEY_LOCK_EFFECT6_MOVE_ICON_FADE_FRAME_NUM, frame_index);

    UILAYER_WeakLayerAlpha(&layer, MMI_KL_COLOR_ALPHA_MAX - alpha);
}

/******************************************************************************/
//  Description : Get the focused target rect.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetFocusedTargetRectEffect6(uint8 index)
{
    GUI_RECT_T rect = {0};
    int16 offset;
    if (PNULL != s_kl_effect_custom)
    {
        if((GET_PROG_NUM % 2) == 0)
        {
            if(index >= GET_PROG_NUM/2)
            {
                offset = (index - (GET_PROG_NUM/2) + 1) * s_kl_effect_custom->target_offset;
            }
            else
            {
                offset = (index - (GET_PROG_NUM/2)) * s_kl_effect_custom->target_offset;
            }
        }
        else
        {
            offset = (index - (GET_PROG_NUM/2)) * s_kl_effect_custom->target_offset;
        }

        /* the full rect of the whole layer */
        rect = GET_RECT(target);


        /* add the horison offset */
        rect.left += offset;
        rect.right += offset;
    
        /* add rising distance */
        rect.top -= s_kl_effect_custom->target_rise;
        rect.bottom -= s_kl_effect_custom->target_rise;
    }
    return rect;
    
}

/******************************************************************************/
//  Description : Get reach target rect.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetReachTargetRectEffect6(uint8 index)
{
    GUI_RECT_T rect = {0};
    int16 offset;
    if (PNULL != s_kl_effect_custom)
    {
        if((GET_PROG_NUM % 2) == 0)
        {
            offset = (index - (GET_PROG_NUM/2) + 1) * s_kl_effect_custom->target_offset;
        }
        else
        {
            offset = (index - (GET_PROG_NUM/2)) * s_kl_effect_custom->target_offset;
        }

        /* the full rect of the whole layer */
        if (PNULL != s_kl_effect_custom)
        {
            rect = GET_RECT(target);
        }

        /* get the target center rect after rising */
        rect.left += (rect.right - rect.left + 1 - s_kl_effect_custom->target_width)/2;
        rect.right = rect.left + s_kl_effect_custom->target_width;
        rect.top += (rect.bottom - rect.top + 1 - s_kl_effect_custom->target_width)/2;
        rect.bottom = rect.top + s_kl_effect_custom->target_width;
    
        /* add the horisen offset by index */
        rect.left += offset;
        rect.right += offset;

        /* add rising distance */
        rect.top -= s_kl_effect_custom->target_rise;
        rect.bottom -= s_kl_effect_custom->target_rise;
    }
    return rect;
}

/******************************************************************************/
//  Description : Display focused target.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayFocusedTargetEffect6(uint8 index)
{
    GUI_RECT_T rect = GetFocusedTargetRectEffect6(index);
    
    UILAYER_Clear(&s_kl_effect6_info.target_layer);
    UILAYER_SetLayerPosition(&s_kl_effect6_info.target_layer, rect.left, rect.top);
    if (PNULL != s_kl_effect_custom)
    {
        DISP_IMG_GEN(&rect, s_kl_effect_custom->target_focused.img_id, &s_kl_effect6_info.target_layer);
    }
}

/******************************************************************************/
//  Description : Make a judgement if move icon reach target.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMoveIconReachTargetEffect6(uint8 index, int16 offset_x, int16 offset_y)
{
    GUI_RECT_T rect = {0};
    int16 x = 0, y = 0;
    
    if (PNULL != s_kl_effect_custom)
    {
        rect = GET_ICON_RECT(index);
    }
    /* the current position of move icon */
    x = rect.left + offset_x;
    y = rect.top + offset_y;

    /* the center point of move icon */
    x += (rect.right - rect.left) / 2;
    y += (rect.bottom - rect.top) / 2;

    /* if the center point in the rect of corresponding, we think the move icon reach the target */
    rect = GetReachTargetRectEffect6(index);

    return (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom);
}

/******************************************************************************/
//  Description : Do something when move icon enter the target.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DoMoveIconEnterTargetEffect6(uint8 index)
{
    GUI_RECT_T rect = {0};
    uint16 x = 0;
    uint16 y = 0;

    rect = GetFocusedTargetRectEffect6(index);
    if (PNULL != s_kl_effect_custom)
    {
        x = rect.left + (rect.right - rect.left + 1 - (GET_ICON_RECT(index).right - GET_ICON_RECT(index).left + 1))/2;
        y = rect.top + (rect.bottom -rect.top + 1 -(GET_ICON_RECT(index).bottom - GET_ICON_RECT(index).top + 1))/2;
    }

    UILAYER_SetLayerPosition(&s_kl_effect6_info.move_icon_layer, x, y);
}

/******************************************************************************/
//  Description : Zoom out target.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DoZoomTargetEffect6(uint16 frame_index, GUI_LCD_DEV_INFO* lcd_dev_ptr)
{
    SCALE_IMAGE_IN_T    scale_in  = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};
    UILAYER_INFO_T layer_info = {0};
//    UILAYER_INFO_T zoom_layer_info = {0};
//    uint8 *zoom_layer_buffer_ptr = PNULL;

    UILAYER_CREATE_T    create_info = {0};
    uint16 width = 0, height = 0;
    UILAYER_APPEND_BLT_T append_layer = {0};
    UILAYER_RESULT_E result  = 0;
    uint16 lcd_width = 0, lcd_height = 0;
    int16 trim_width = 0, trim_height = 0;
    
    uint16 val_w = 0, val_h = 0;
    uint16 *alloc_buf_ptr = PNULL;  

    uint16 frame_num = 0;
    uint16 frame_index_tmp = frame_index;
    UILAYER_COPYBUF_T           copy_buf = {0};
    UILAYER_COPYBUF_PARAM_T     copy_param = {0};
    
    UILAYER_GetLayerInfo(lcd_dev_ptr, &layer_info);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if(0 == layer_info.layer_width || 0 == layer_info.layer_height)
    {
        return;
    }
    frame_num = KEY_LOCK_EFFECT6_ZOOM_FRAME_NUM - 1;
//    if(0 == frame_num) 
//    {
//        return;
//     }
    val_w = (lcd_width << 8)/layer_info.layer_width;
    val_h = (lcd_height << 8)/layer_info.layer_height;
    if(val_w > val_h)
    {
    }
    else
    {
        if(frame_index_tmp > frame_num)
        {
            frame_index_tmp = frame_num;
        }
        height = layer_info.layer_height + (lcd_height - layer_info.layer_height) * frame_index_tmp/frame_num;

        /* to keep the same zoom out rate */
        width = (layer_info.layer_width) * height /layer_info.layer_height;

        if(width > lcd_width)
        {
            trim_width = layer_info.layer_width * lcd_width/width;
            trim_height = layer_info.layer_height;
            width = lcd_width;
        }
        else
        {
            trim_width = layer_info.layer_width;
            trim_height = layer_info.layer_height;
        }

        if(frame_index > frame_num)
        {
            trim_width -= (frame_index - frame_num) * (layer_info.layer_width / frame_num);
            trim_height -= (frame_index - frame_num) * (layer_info.layer_height / frame_num);
        }
        if(trim_width < lcd_width/4 || trim_height < lcd_height/4)
        {
            trim_width = lcd_width/4;
            trim_height = lcd_height/4;
        }
    }

    if(s_kl_effect6_info.zoom_layer.block_id != 0)
    {
        UILAYER_RELEASELAYER(&s_kl_effect6_info.zoom_layer);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_kl_effect6_info.zoom_layer);        
    }


    /* 256 padding is used by Driver for align */
    alloc_buf_ptr = SCI_ALLOCA(width*height*4 + MMI_KL_256_ALIGN_PADDING);  
    if(alloc_buf_ptr != PNULL)
    {
        SCI_MEMSET(alloc_buf_ptr, 0, width*height*4 + MMI_KL_256_ALIGN_PADDING);
//        scale_in.src_chn.chn0.ptr     = layer_info.layer_buf_ptr;
        scale_in.src_chn.chn0.ptr     = UILAYER_GetLayerBufferPtr(lcd_dev_ptr);
        if (PNULL == scale_in.src_chn.chn0.ptr)
        {
            SCI_FREE(alloc_buf_ptr);
            return;
        }
        scale_in.src_chn.chn0.size    = layer_info.mem_width * layer_info.layer_height * 4;
        scale_in.src_format           = IMGREF_FORMAT_ARGB888;
        scale_in.src_size.w           = layer_info.mem_width;
        scale_in.src_size.h           =  layer_info.layer_height;

        scale_in.src_trim_rect.x      = (layer_info.layer_width - trim_width)/2;
        scale_in.src_trim_rect.y      = (layer_info.layer_height - trim_height)/2;;
        scale_in.src_trim_rect.w      = trim_width;
        scale_in.src_trim_rect.h      = trim_height;

        scale_in.target_buf.size      = width * height * 4;          
        scale_in.target_buf.ptr       = alloc_buf_ptr; 
        scale_in.target_format        = IMGREF_FORMAT_ARGB888;

        scale_in.target_size.w = width;
        scale_in.target_size.h = height;
        if (SCI_SUCCESS == GRAPH_ScaleImage(&scale_in, &scale_out))
        {
	        MMI_KL_CREATE_LAYER(create_info, MMIKL_CLOCK_DISP_WIN_ID, scale_out.output_size.w, scale_out.output_size.h, s_kl_effect6_info.zoom_layer, result);
	        if(UILAYER_RESULT_SUCCESS == result)
	        {
	            append_layer.lcd_dev_info = s_kl_effect6_info.zoom_layer;
	            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
	            UILAYER_AppendBltLayer(&append_layer);
	            UILAYER_Clear(&s_kl_effect6_info.zoom_layer);
	            copy_param.rect = UILAYER_GetLayerRect(&s_kl_effect6_info.zoom_layer);
	//            copy_param.rect.left   = 0;
	//            copy_param.rect.top    = 0;
	            copy_param.rect.right  = (int16)(copy_param.rect.left+scale_out.output_size.w - 1);
	            copy_param.rect.bottom = (int16)(copy_param.rect.top+scale_out.output_size.h - 1);

	            copy_buf.data_type = DATA_TYPE_ARGB888;
	            copy_buf.width     = scale_out.output_size.w;
	            copy_buf.height    = scale_out.output_size.h; 
	            copy_buf.buf_ptr   = (uint8*)alloc_buf_ptr;
	            UILAYER_CopyBufferToLayer(&s_kl_effect6_info.zoom_layer,&copy_param,&copy_buf);
	//            UILAYER_GetLayerInfo(&s_kl_effect6_info.zoom_layer, &zoom_layer_info);
	//            zoom_layer_buffer_ptr = UILAYER_GetLayerBufferPtr(&s_kl_effect6_info.zoom_layer);
	//            SCI_MEMCPY(zoom_layer_buffer_ptr, alloc_buf_ptr, scale_out.output_size.w * scale_out.output_size.h * 4);
	            
	            UILAYER_SetLayerPosition(&s_kl_effect6_info.zoom_layer, (lcd_width - scale_out.output_size.w)/2, (lcd_height - scale_out.output_size.h)/2);
	            
	            MMITHEME_UpdateRect();
	        }
        }
        SCI_FREE(alloc_buf_ptr);
    }

}

/******************************************************************************/
//  Description : Append layer.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendLayerEffect6(void)
{
    AppendLayer(&s_kl_effect6_info.icon_panel_layer);
    AppendLayer(&s_kl_effect6_info.target_layer);
    AppendLayer(&s_kl_effect6_info.move_icon_layer);    
}

/******************************************************************************/
//  Description : remove layer.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void RemoveLayerEffect6(void)
{
    UILAYER_RemoveBltLayer(&s_kl_effect6_info.target_layer);
    UILAYER_RemoveBltLayer(&s_kl_effect6_info.move_icon_layer);
    UILAYER_RemoveBltLayer(&s_kl_effect6_info.icon_panel_layer);                    
}

/******************************************************************************/
//  Description : Entry the appointed APP.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void EntryAppEffect6(MMI_WIN_ID_T win_id, uint8 program_id)
{
    if(program_id < KEY_LOCK_EFFECT6_PROG_MAX)
    {
        MMIKL_SetProcOpenFlag((KL_PROC_FLAG_E)program_id);
    }
}

/******************************************************************************/
//  Description : handle kl display window msg of type6.
//  Global resource dependence : none
//  Author: 
//  Note: Icon moving unlock style
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleDragIconOrRingUnlockWinMsg(
                                                        MMI_WIN_ID_T    win_id, 
                                                        uint16          msg_id, 
                                                        DPARAM          param
                                                        )
{
    typedef enum
    {
        STATE_PANEL_FADE = 0,
        STATE_WAITING_ICON_FADE,
        STATE_ICON_FADE,
        STATE_ICON_FADE_OF_RING,
        STATE_CLEAR_TARGET,
        STATE_MOVE_TARGET,
        STATE_MOVE_ICON_OF_RING,
        STATE_WAITING_ZOOM_TARGET,
        STATE_ZOOM_TARTGET,
         STATE_ZOOM_TARTGET_OF_RING,
          STATE_ACTION_MAX
    }EFFECT_STATE_E;

    typedef enum
    {
        UNLOCK_TYPE_NONE,
        UNLOCK_TYPE_ICON,
        UNLOCK_TYPE_RING,
        UNLOCK_TYPE_MAX
    }UNLOCK_TYPE_E;
    
    //MMI_KEY_LOCK_PSW_KEY_E key = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    static GUI_POINT_T tp_down_point = {0};
    static GUI_POINT_T tp_last_point = {0};
    
    static uint8 panel_fade_frame_index = 0;
    static uint8 move_icon_fade_index = 0;
    static uint8 target_move_index = 0;
    static uint8 target_zoom_index = 0;
    static uint8 icon_index = 0;
    static BOOLEAN effect6_can_unlock = FALSE;
    static EFFECT_STATE_E effect_state = STATE_PANEL_FADE;
    static UNLOCK_TYPE_E unlock_type = UNLOCK_TYPE_NONE;
    
    //SCI_TRACE_LOW:"HandleKLWinEffect6Msg: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_DRAGUNLOCK_1007_112_2_18_2_30_44_0,(uint8*)"d",msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(!InitParamEffect6())
        {
            MMK_CloseWin(win_id);
            break;
        }
	    GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
        GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR        
		GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif        
        MMK_SetWinLcdDevInfoNotifyCtrl(win_id, GetLayerEffect6(&s_kl_effect6_info.main_layer));
   //     MMK_SetWinDisplayStyleState( win_id, WS_HAS_TRANSPARENT, TRUE );
//        MMK_SetWinMoveStyle(win_id,MOVE_NOT_FULL_SCREEN_WINDOW);
        
        panel_fade_frame_index = 0;
        move_icon_fade_index = 0;
        target_zoom_index = 0;
        icon_index = 0;
        effect_state = STATE_PANEL_FADE;
        tp_last_point.x = 0;
        tp_last_point.y = 0;
        unlock_type = UNLOCK_TYPE_NONE;
        break;
    case MSG_FULL_PAINT:
        //if(UNLOCK_TYPE_NONE == unlock_type)
            DisplayBottomBgEffect6();
            DisplayDigitalClockEffect6();
            DisplayMissedEventEffect6();
            if(UNLOCK_TYPE_NONE == unlock_type)
            {
                DisplayIconPanelEffect6();
                DisplayTargetEffect6();
            }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        if(STATE_PANEL_FADE == effect_state)
        {
            tp_down_point.x = MMK_GET_TP_X(param);
            tp_down_point.y = MMK_GET_TP_Y(param);

            if(GetTpDownInIconEffect6(tp_down_point, &icon_index))
            {
                DisplayIconWithoutFocusEffect6(icon_index);
                DisplayMoveIconEffect6(icon_index);
                DisplayFocusedTargetEffect6(icon_index);
                StartEffectTimerEffect6(KEY_LOCK_EFFECT6_ICON_PANEL_FADE_TIME);
                StartTpMoveTimerEffect6();
                unlock_type = UNLOCK_TYPE_ICON;
            }
            else if(JudgeTpDownInRingRect(tp_down_point))
            {
                unlock_type = UNLOCK_TYPE_RING;
                        StartTpMoveTimerEffect6();
                
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if(STATE_PANEL_FADE == effect_state)
        {
            StopTpMoveTimerEffect6();
            tp_last_point.x = 0;
            tp_last_point.y = 0;
            if(UNLOCK_TYPE_ICON == unlock_type||UNLOCK_TYPE_RING == unlock_type)
            {
                StopEffectTimerEffect6();
                ClearMoveIconEffect6();
                DisplayIconPanelEffect6();
                DisplayTargetEffect6();  
                panel_fade_frame_index = 0;
                unlock_type = UNLOCK_TYPE_NONE;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TIMER:
        if(s_kl_effect6_info.tp_move_timer == *(uint8*)param)
        {
            MMI_TP_STATUS_E tp_status = 0;
            int16 offset_x = 0,  offset_y = 0;
            MMK_GetLogicTPMsg(&tp_status, &point);
            offset_x = point.x - tp_down_point.x;
            offset_y = point.y - tp_down_point.y;
            if(UNLOCK_TYPE_ICON == unlock_type)
            {
                if(IsMoveIconReachTargetEffect6(icon_index, offset_x, offset_y))
                {
                    StopTpMoveTimerEffect6();
                    DoMoveIconEnterTargetEffect6(icon_index);
                    if(panel_fade_frame_index != 0)
                    {
                        panel_fade_frame_index = 0;
                        UILAYER_Clear(&s_kl_effect6_info.icon_panel_layer);
                    }
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_WAITINGE_ICON_FADE_TIME);
                    effect_state = STATE_WAITING_ICON_FADE;
                    MMIKL_SetWinStatus(KL_UNLOCKED);
                }
                else
                {
                    if(tp_last_point.x != point.x || tp_last_point.y != point.y)
                    {
                        tp_last_point = point;
                        SetMoveIconLayeEffect6(icon_index, offset_x, offset_y);
                    }
                    StartTpMoveTimerEffect6();
                }
            }
            else if(UNLOCK_TYPE_RING == unlock_type)
            {
                offset_x += s_kl_effect_custom->target.rect.left ;
                offset_y += s_kl_effect_custom->target.rect.top;
                UILAYER_SetLayerPosition(&s_kl_effect6_info.target_layer, offset_x,offset_y);
        
                if(GetIconIndexInRingRect(offset_x,offset_y,&icon_index))//(GetTpDownInIconEffect6(point, &icon_index))
                {
                    effect_state = STATE_ICON_FADE_OF_RING;
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_ICON_PANEL_FADE_TIME);
                }
                else
                {
                    MMK_GetLogicTPMsg(&tp_status, &point);
                    offset_x = point.x - tp_down_point.x;
                    offset_y = point.y - tp_down_point.y;
                    
                    if(tp_last_point.x != point.x || tp_last_point.y != point.y)
                    {
                        //#define ABS(x)  ((x > 0)?(x):(-x))
                        uint16 lcd_width = 0, lcd_height = 0;
                        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
                
                        if(ABS(offset_x) > lcd_width/2 || ABS(offset_y) > lcd_height/2)
                        {
                            StartEffectTimerEffect6(KEY_LOCK_EFFECT6_WAITINGE_ICON_FADE_TIME);
                            effect_state = STATE_WAITING_ICON_FADE;
                            MMIKL_SetWinStatus(KL_UNLOCKED);
                            icon_index = 100;//return to preview win,not sms,pb and so on
                            break;
                        }
                        else
                        {
                            tp_last_point = point;
                        }
                    }
                    StartTpMoveTimerEffect6();
                }
            }
            else
            {}
        }
        else if(s_kl_effect6_info.effect_timer == *(uint8*)param)
        {
            switch(effect_state)
            {
            case STATE_PANEL_FADE:
                panel_fade_frame_index++;
                if(panel_fade_frame_index <= KEY_LOCK_EFFECT6_PANEL_FADE_FRAME_NUM)
                {    
                    FadeIconPanelEffect6(panel_fade_frame_index);
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_ICON_PANEL_FADE_TIME);
                }
                else
                {
                    StopEffectTimerEffect6();
                    panel_fade_frame_index = 0;
                }
                break;
            case STATE_ICON_FADE_OF_RING:
                StopEffectTimerEffect6();
                panel_fade_frame_index++;
                if(panel_fade_frame_index <= KEY_LOCK_EFFECT6_PANEL_FADE_FRAME_NUM)
                {    
                    FadeIconPanelEffect6(panel_fade_frame_index);
                    DisplayMoveIconEffect6(icon_index);
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_ICON_PANEL_FADE_TIME);
                }
                else
                {
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_MOVE_ICON_FADE_TIME);
                    UILAYER_Clear(&s_kl_effect6_info.target_layer);
                    effect_state = STATE_MOVE_ICON_OF_RING;
                    panel_fade_frame_index = 0;
                }
                break;

            case STATE_WAITING_ICON_FADE:
                StartEffectTimerEffect6(KEY_LOCK_EFFECT6_MOVE_ICON_FADE_TIME);
                effect_state = STATE_ICON_FADE;
                break;
            case STATE_ICON_FADE:
                move_icon_fade_index++;
                if(move_icon_fade_index <= KEY_LOCK_EFFECT6_MOVE_ICON_FADE_FRAME_NUM)
                {
                    FadeMoveIconEffect6(move_icon_fade_index);
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_MOVE_ICON_FADE_TIME);
                }
                else
                {
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_CLEAR_TARGET_TIME);
                    effect_state = STATE_CLEAR_TARGET;
                    move_icon_fade_index = 0;
                }                
                break;
            case STATE_CLEAR_TARGET:
            {
                int16 x, y;
                UILAYER_GetLayerPosition(&s_kl_effect6_info.target_layer, &x, &y);
                DisplayTargetEffect6();
                UILAYER_SetLayerPosition(&s_kl_effect6_info.target_layer, x, y);                
                StartEffectTimerEffect6(KEY_LOCK_EFFECT6_MOVE_TARGET_TIME);
                effect_state = STATE_MOVE_TARGET;
                target_move_index = 0;
                break;
            }
            case STATE_MOVE_TARGET:
            {
                int16 pos_x, pos_y;
                uint16 lcd_width = 0, lcd_height = 0;
                int16 total_width = 0, total_height = 0;

                UILAYER_INFO_T layer_info = {0};
                UILAYER_GetLayerInfo(&s_kl_effect6_info.target_layer, &layer_info);
                
                target_move_index++;
                if(target_move_index <= KEY_LOCK_EFFECT6_TARGET_MOVE_FRAME_NUM)
                {
                    UILAYER_GetLayerPosition(&s_kl_effect6_info.target_layer, &pos_x, &pos_y);
                    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
                    
                    /* the distance from the center point of start to the center point of end */
                    total_width = pos_x + (layer_info.layer_width - lcd_width)/2;
                    total_height = pos_y + (layer_info.layer_height - lcd_height)/2;

                    /* Get each the distance of each step */
                    pos_x -= MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_3, total_width, KEY_LOCK_EFFECT6_TARGET_MOVE_FRAME_NUM,  target_move_index);
                    pos_y -= MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_3, total_height, KEY_LOCK_EFFECT6_TARGET_MOVE_FRAME_NUM,  target_move_index);
                    UILAYER_SetLayerPosition(&s_kl_effect6_info.target_layer, pos_x, pos_y);    
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_MOVE_TARGET_TIME);                
                }
                else
                {
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_WAITING_ZOOM_TARGET_TIME);
                    target_move_index = 0;
                    effect_state = STATE_WAITING_ZOOM_TARGET;
                }
                break;
            }
            case STATE_MOVE_ICON_OF_RING:
               {
                int16 pos_x, pos_y;
                uint16 lcd_width = 0, lcd_height = 0;
                int16 total_width = 0, total_height = 0;

                UILAYER_INFO_T layer_info = {0};
                UILAYER_GetLayerInfo(&s_kl_effect6_info.move_icon_layer, &layer_info);
                
                target_move_index++;
                if(target_move_index <= KEY_LOCK_EFFECT6_TARGET_MOVE_FRAME_NUM)
                {
                    UILAYER_GetLayerPosition(&s_kl_effect6_info.move_icon_layer, &pos_x, &pos_y);
                    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
                    
                    /* the distance from the center point of start to the center point of end */
                    total_width = pos_x + (layer_info.layer_width - lcd_width)/2;
                    total_height = pos_y + (layer_info.layer_height - lcd_height)/2;

                    /* Get each the distance of each step */
                    pos_x -= MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_3, total_width, KEY_LOCK_EFFECT6_TARGET_MOVE_FRAME_NUM,  target_move_index);
                    pos_y -= MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_3, total_height, KEY_LOCK_EFFECT6_TARGET_MOVE_FRAME_NUM,  target_move_index);
                    UILAYER_SetLayerPosition(&s_kl_effect6_info.move_icon_layer, pos_x, pos_y);    
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_MOVE_TARGET_TIME);                
                }
                else
                {
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_WAITING_ZOOM_TARGET_TIME);
                    target_move_index = 0;
                    effect_state = STATE_ZOOM_TARTGET_OF_RING;
                }
                break;
            }
            case STATE_WAITING_ZOOM_TARGET:
                StartEffectTimerEffect6(KEY_LOCK_EFFECT6_ZOOM_TARGET_TIME);
                //UILAYER_Clear(&s_kl_effect6_info.target_layer);
                target_zoom_index = 0;
                effect_state = STATE_ZOOM_TARTGET;
                break;
            case STATE_ZOOM_TARTGET:
                if(0 == target_zoom_index)
                {
                    // to increase the blt speed, we remove the useless layer 
                    RemoveLayerEffect6();
                }
                if(++target_zoom_index <= KEY_LOCK_EFFECT6_ZOOM_FRAME_NUM)
                {
                    DoZoomTargetEffect6(target_zoom_index,&s_kl_effect6_info.target_layer);
                     
                }
                if(target_zoom_index >= KEY_LOCK_EFFECT6_ZOOM_FRAME_NUM)
                {
                    /* To gurranty the last frame is disppeared quickly, we display it and clear it sync. */
                    SCI_Sleep(5);
                    //UILAYER_Clear(&s_kl_effect6_info.zoom_layer);
                    UILAYER_RELEASELAYER(&s_kl_effect6_info.zoom_layer);   /*lint !e506 !e774*/
//                    UILAYER_ReleaseLayer(&s_kl_effect6_info.zoom_layer);
                    MMITHEME_UpdateRect();
                    SCI_Sleep(5);
                    target_zoom_index = 0;
                    effect_state = STATE_PANEL_FADE;    
                    EntryAppEffect6(win_id, icon_index);
                    MMK_CloseWin(win_id);                
                }
                else
                {
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_ZOOM_TARGET_TIME);
                }
                break;

            case STATE_ZOOM_TARTGET_OF_RING:
               if(0 == target_zoom_index)
                {
                   RemoveLayerEffect6();     
                }
               ++target_zoom_index;
                if(target_zoom_index <= KEY_LOCK_EFFECT6_ZOOM_FRAME_NUM)
                {
                    DoZoomTargetEffect6(target_zoom_index,&s_kl_effect6_info.move_icon_layer);
                }
                 if(target_zoom_index > KEY_LOCK_EFFECT6_ZOOM_FRAME_NUM)
                {
                    /* To gurranty the last frame is disppeared quickly, we display it and clear it sync. */
                    SCI_Sleep(5);
               //     UILAYER_Clear(&s_kl_effect6_info.move_icon_layer);
                    UILAYER_RELEASELAYER(&s_kl_effect6_info.zoom_layer);   /*lint !e506 !e774*/
//                    UILAYER_ReleaseLayer(&s_kl_effect6_info.zoom_layer);
                    MMITHEME_UpdateRect();
                    SCI_Sleep(5);
                    target_zoom_index = 0;
                    effect_state = STATE_PANEL_FADE;    
                    EntryAppEffect6(win_id, icon_index);
                    MMK_CloseWin(win_id);                
                }
                else
                {
                    StartEffectTimerEffect6(KEY_LOCK_EFFECT6_ZOOM_TARGET_TIME);
                }
                break;
                
            default:
                break;
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    case MSG_GET_FOCUS:
        //MMK_SetWinLcdDevInfoNotifyCtrl(win_id, GetLayerEffect6(&s_kl_effect6_info.main_layer));        
        //if(KL_LOCKED == MMIKL_GetWinStatus())
        {
            AppendLayer(&s_kl_effect6_info.main_layer);        
            AppendLayerEffect6();
            tp_last_point.x = 0;
            tp_last_point.y = 0;
            
            unlock_type = UNLOCK_TYPE_NONE;
        }
        break;
    case MSG_LOSE_FOCUS:
        if(KL_UNLOCKED == MMIKL_GetWinStatus())
        {
            MMK_CloseWin(win_id);
            StopEffectTimerEffect6();
        }
        else
        {
            StopTpMoveTimerEffect6();
        }
        //MMK_SetWinLcdDevInfoNotifyCtrl(win_id, GetLayerEffect6(&s_kl_effect6_info.lcd_layer));
        break;
    case MSG_CLOSE_WINDOW:
        if(s_kl_effect_custom != PNULL)
        {
            SCI_FREE(s_kl_effect_custom);
            s_kl_effect_custom = PNULL;
        }
        break;
    
    case MSG_APP_OK:
  //  case MSG_APP_MENU:
        if (MMIKL_IsPreviewState())
        {
            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT6);
            MMK_CloseWin(win_id);
        }
		else
		{
			recode = MMI_RESULT_FALSE;		
		}
        break;
    case MSG_APP_CANCEL:
        if (MMIKL_IsPreviewState())
        {
            MMK_CloseWin(win_id);
        }
		else
		{
			recode = MMI_RESULT_FALSE;		
		}
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description :  Append layer to blt layer array.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendLayer(GUI_LCD_DEV_INFO *layer)
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    append_layer.lcd_dev_info = *layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);
}
#endif	//MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT

#endif




/*Edit by script, ignore 3 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
