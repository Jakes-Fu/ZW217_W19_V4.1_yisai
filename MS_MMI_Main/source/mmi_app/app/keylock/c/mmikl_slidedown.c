/*****************************************************************************
** File Name:      mmikl_slidedown.c                                         *
** Author:         jian.ma                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe keylock slide down function *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create
******************************************************************************/
#ifdef MMI_KEY_LOCK_SUPPORT
#ifndef _MMIKL_SLIDEDOWN_C_

#include "mmi_app_keylock_trc.h"
#ifdef MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmikl_slidedown.h"
#include "mmk_type.h"
#include "guistring.h"
#include "mmi_custom_define.h"
#include "mmicc_text.h"
#include "mmi_textfun.h"
#include "mmi_text.h"
#include "dal_time.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmikl_effectmain.h"
#include "mmikl_export.h"
#include "guires.h"
#include "mmi_wallpaper_export.h"
#include "mmiset_nv.h"
#include "mmidisplay_data.h"
#include "mmi_keylock_position.h"
#include "mmi_appmsg.h"
#include "mmi_id.h"
#include "mmk_tp.h"
#include "mmi_image.h"
#include "mmikl_internal.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


#define KEY_LOCK_EFFECT1_TP_MOVE_TIME   20
#define KEY_LOCK_EFFECT1_ANIM_TIME  200

#define KEY_LOCK_EFFECT1_ANIM_FRAME_NUM 9

typedef struct
{
    GUI_LCD_DEV_INFO layer;
    uint16 layer_pos_y;
    //uint16 anim_layer_pos_x;
    //uint16 anim_layer_pos_y;
    uint16 tp_move_timer;
    uint16 anim_timer;
    //wchar *plmn;
}MMI_KEY_LOCK_EFFECT1_INFO_T;

LOCAL MMI_KEY_LOCK_EFFECT1_INFO_T s_key_lock_effect1_info = {0};


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
  
/**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  set key lock control parameters of effect type1.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetKeyLockControlParamEffect1(void);


/*****************************************************************************/
//  Description :  Display date.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayDate(
                       GUI_LCD_DEV_INFO *dev, 
                       GUI_RECT_T rect, 
                       GUI_FONT_T font, 
                       GUI_ALIGN_E align, 
                       GUI_COLOR_T color
                       );

/*****************************************************************************/
//  Description :  Display Missed event of effect type1.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayMissedEventEffect1(void)
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
    rect.top = (lcd_height - s_key_lock_effect1_info.layer_pos_y - rect_height) / 2;
    rect.bottom = rect.top + rect_height;
    
    MMIKL_DisplayMissedEvent(MMIKL_CLOCK_DISP_WIN_ID, &rect, &lcd_dev);
}

/*****************************************************************************/
//  Description :  Display clock for effect1.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayDigitalClockEffect1(GUI_LCD_DEV_INFO *dev, GUI_RECT_T rect)
{
    GUI_POINT_T                 point = {0};
    SCI_TIME_T                  time = {0};
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();
    BOOLEAN                     b_am = TRUE;
    uint16                      image_width = 0,image_height = 0;
    uint16                      time_total_width = 0;
    
    TM_GetSysTime(&time);

    //12小时制
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        GET_12STYLE_HOUR(time.hour, b_am);
    }

    //计算时间总的宽度，为了居中显示
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_NUM_COLON,MMIKL_CLOCK_DISP_WIN_ID);
    time_total_width = time_total_width  + image_width;//colon
    
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_NUM_AM,MMIKL_CLOCK_DISP_WIN_ID);
        time_total_width = time_total_width  + image_width;//am.pm
    }
    
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_NUM_0,MMIKL_CLOCK_DISP_WIN_ID);
    time_total_width = time_total_width + image_width * 4 ;//有四个数字

    point.y = rect.top;
    if(rect.right - rect.left >= time_total_width)
    {
        point.x = rect.left + (rect.right - rect.left - time_total_width)/2;
    }
    else
    {
        point.x = rect.left;
    }

    //HOUR
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_NUM_0 + time.hour/10, dev);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_NUM_0 + time.hour%10, dev);

    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_NUM_COLON, dev);

    //COLON IMAGE WIDTH
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_NUM_COLON,MMIKL_CLOCK_DISP_WIN_ID);
    //MIN
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_NUM_0 + time.min/10, dev);

    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_NUM_0,MMIKL_CLOCK_DISP_WIN_ID);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_NUM_0 + time.min%10, dev);
    //AM.PM
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        point.x = point.x + image_width;
        if(b_am )
            GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_NUM_AM, dev);
        else
            GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_NUM_PM, dev);
    }
}

/*****************************************************************************/
//  Description :  Create move layer of effect1
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateMoveLayerEffect1(void)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    UILAYER_APPEND_BLT_T append_layer;
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 image_width = 0, image_height = 0;
    uint16 height = 0;

    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT1_ARCGLASS_BOTTOM, MMIKL_CLOCK_DISP_WIN_ID);
    height += image_height * 2;
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT1_ARCGLASS_TOP, MMIKL_CLOCK_DISP_WIN_ID);
    height -= image_height * 2;
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT1_PROMPT_BACK, MMIKL_CLOCK_DISP_WIN_ID);
    height += image_height;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = MMIKL_CLOCK_DISP_WIN_ID;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = lcd_width;
    create_info.height = height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    result = UILAYER_CreateLayer(
        &create_info,  
        &layer
        );
    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_key_lock_effect1_info.layer = layer;
        
        append_layer.lcd_dev_info = layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 

        UILAYER_Clear(&layer);
        s_key_lock_effect1_info.layer_pos_y = lcd_height - height;
        UILAYER_SetLayerPosition(&layer, 0, s_key_lock_effect1_info.layer_pos_y);
    }
}

/*****************************************************************************/
//  Description :  Get move layer of effet1
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetMoveLayerEffect1(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if(s_key_lock_effect1_info.layer.block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_key_lock_effect1_info.layer))
        {
            lcd_dev_info = s_key_lock_effect1_info.layer;
        }
    }
    return lcd_dev_info;
}

/*****************************************************************************/
//  Description :  set move layer pos off effect1.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetMoveLayerPosEffect1(uint16 y)
{
    GUI_LCD_DEV_INFO layer = GetMoveLayerEffect1();

    if(layer.block_id != 0)
    {
        UILAYER_SetLayerPosition(&layer, 0, y);
    }    
}

/*****************************************************************************/
//  Description :  Append move layer of effect1.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendMoveLayerEffect1(void)
{
    UILAYER_APPEND_BLT_T append_layer;

    append_layer.lcd_dev_info = GetMoveLayerEffect1();
    if(append_layer.lcd_dev_info.block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}

/*****************************************************************************/
//  Description :  set key lock control parameters of effect type1.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetKeyLockControlParamEffect1(void)
{
    GUI_BG_T                    bg = {0};

    CreateMoveLayerEffect1();

    //set softkey common bg
    GUIWIN_SetSoftkeyCommonBg(MMIKL_CLOCK_DISP_WIN_ID,&bg);

}

LOCAL void UpdateLayerEffect1(BOOLEAN is_pen_down)
{
    uint16 image_width = 0, image_height = 0;
    uint16 lcd_width = 0, lcd_height = 0;
    int16 layer_pos_x = 0, layer_pos_y = 0;
    uint16 width = 0;
    GUI_POINT_T pos = {0};
    GUI_LCD_DEV_INFO layer = {0};
    uint16 back_img_y = 0, top_img_y; 
    GUI_RECT_T rect = {0};
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T string = {0};
    uint16 offset_y = 0;
    
    layer = GetMoveLayerEffect1();

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    if(0 == layer.block_id)
    {
        offset_y = s_key_lock_effect1_info.layer_pos_y;
        pos.y = offset_y;
    }
        
        UILAYER_GetLayerPosition(&layer, &layer_pos_x, &layer_pos_y);
        UILAYER_SetLayerPosition(&layer, 0, 0);
        UILAYER_Clear(&layer);
        AppendMoveLayerEffect1();
        
        /* UP BG  IMG */
        GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT1_ARCGLASS_BOTTOM, &layer);
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT1_ARCGLASS_BOTTOM, MMIKL_CLOCK_DISP_WIN_ID);
        pos.y += image_height;
        back_img_y = image_height * 2 + offset_y;
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT1_ARCGLASS_TOP, MMIKL_CLOCK_DISP_WIN_ID);
        pos.y -= image_height;
        back_img_y -= image_height * 2;
        /* Down BG IMG */
        GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT1_ARCGLASS_BOTTOM, &layer);

        /* Here maybe need to be adjusted */
        pos.y -= (image_height + 5);
        top_img_y = pos.y;
        GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT1_ARC, &layer);

        /* BOTTOM BG IMG */
        pos.y = back_img_y;
        GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT1_PROMPT_BACK, &layer);


        text_style.align = ALIGN_HMIDDLE;
        text_style.font = MMI_DEFAULT_SMALL_FONT;
        text_style.char_space = 0;
        text_style.line_space = 0;
        text_style.font_color = MMI_WHITE_COLOR;

        rect.right = lcd_width - 1;

        if(is_pen_down)
        {
            GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT1_LOCK, MMIKL_CLOCK_DISP_WIN_ID);
            MMI_GetLabelTextByLang(TXT_KL_EFFECT1_SCREEN_LOCKED, &string);

            text_style.font = MMI_DEFAULT_BIG_FONT;
            width = GUISTR_GetStringWidth(&text_style, &string, GUISTR_STATE_ALIGN);
            pos.x = (lcd_width - width) / 2 - image_width - 1;
            pos.y = top_img_y + KEY_LOCK_EFFECT1_TIME_OFFSET_Y;
            GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT1_LOCK, &layer);
            rect.top = pos.y;
            rect.bottom = rect.top + MMI_DEFAULT_FONT_HEIGHT * 2;
            GUISTR_DrawTextToLCDInRect( 
                            (const GUI_LCD_DEV_INFO *)&layer,
                            (const GUI_RECT_T *)&rect,
                            (const GUI_RECT_T *)&rect,
                            (const MMI_STRING_T *)&string,
                            &text_style,
                            GUISTR_STATE_ALIGN,
                            GUISTR_TEXT_DIR_AUTO
                            );       
            MMI_GetLabelTextByLang(TXT_KL_SLIDE_DOWN, &string);

            rect.top = KEY_LOCK_EFFECT1_ANIM_POS_Y + 10;
            rect.bottom = rect.top + MMI_DEFAULT_FONT_HEIGHT;
            text_style.font = MMI_DEFAULT_SMALL_FONT;
            GUISTR_DrawTextToLCDInRect( 
                            (const GUI_LCD_DEV_INFO *)&layer,
                            (const GUI_RECT_T *)&rect,
                            (const GUI_RECT_T *)&rect,
                            (const MMI_STRING_T *)&string,
                            &text_style,
                            GUISTR_STATE_ALIGN,
                            GUISTR_TEXT_DIR_AUTO
                            );             
        }
        else
        {
            rect.top = top_img_y - 1;
            rect.bottom = rect.top + MMI_DEFAULT_FONT_HEIGHT;

            DisplayDigitalClockEffect1(&layer, rect);
       
            rect.top = top_img_y - MMI_DEFAULT_FONT_HEIGHT + 2;
            rect.bottom = rect.top + MMI_DEFAULT_FONT_HEIGHT;
            rect.right = lcd_width;
#if 0
            DisplayPLMN(&layer, rect, MMI_DEFAULT_SMALL_FONT, ALIGN_HMIDDLE, MMI_WHITE_COLOR);
#endif    

            rect.top = back_img_y - MMI_DEFAULT_FONT_HEIGHT + 3;
            rect.bottom = rect.top + MMI_DEFAULT_FONT_HEIGHT;
            DisplayDate(&layer, rect, MMI_DEFAULT_SMALL_FONT, ALIGN_HMIDDLE, MMI_WHITE_COLOR); 
            
        }
        UILAYER_SetLayerPosition(&layer, layer_pos_x, layer_pos_y);        
}

/*****************************************************************************/
//  Description :  Draw anim of effect1.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DrawAnimFrameEffect1(uint32 frame_num)
{   
    GUI_POINT_T pos = {0};
    uint16 lcd_width = 0, lcd_height = 0;    
    uint16 image_width = 0, image_height = 0;
    int16 layer_pos_x = 0, layer_pos_y = 0;
    GUI_LCD_DEV_INFO layer = GetMoveLayerEffect1();

    UpdateLayerEffect1(TRUE);
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT1_LOCK, MMIKL_CLOCK_DISP_WIN_ID);

    UILAYER_GetLayerPosition(&layer, &layer_pos_x, &layer_pos_y);

    pos.x = KEY_LOCK_EFFECT1_ANIM_MARGIN_HOR;
    pos.y = KEY_LOCK_EFFECT1_ANIM_POS_Y + layer_pos_y;
    GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT1_ARROW0 + frame_num, &layer);
    pos.x = lcd_width - KEY_LOCK_EFFECT1_ANIM_MARGIN_HOR - image_width;
    GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT1_ARROW0 + frame_num, &layer);
}

/*****************************************************************************/
//  Description :  Handle win msg of effect type1 key lock win.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note: Slide down unlock style with arc picture
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleSlideDownUnlockWinMsg(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param
                        )
{
    MMI_RESULT_E recode  =   MMI_RESULT_TRUE;
    //static BOOLEAN s_is_timer_on       = FALSE;
    static GUI_POINT_T tp_down_point = {0};
    static GUI_POINT_T tp_last_point = {0};
    static uint8 anim_frame_num = 0;
    static uint8 is_tp_down_again = FALSE;
    
    //SCI_TRACE_LOW:"HandleKLWinEffect1Msg: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_SLIDEDOWN_487_112_2_18_2_31_8_10,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        tp_down_point.y = 0;
        SetKeyLockControlParamEffect1();
        break;
    case MSG_FULL_PAINT:
        DisplayMissedEventEffect1();
        if(0 == s_key_lock_effect1_info.anim_timer)
        {
            UpdateLayerEffect1(FALSE);
        }
        break;
    case MSG_BACKLIGHT_TURN_ON:
        if(0 == s_key_lock_effect1_info.anim_timer)
        {
            UpdateLayerEffect1(FALSE);
        }
        SetMoveLayerPosEffect1(s_key_lock_effect1_info.layer_pos_y);
        tp_down_point.y = 0;
        tp_last_point.y = 0;
        is_tp_down_again = FALSE;        
        break;
    case MSG_BACKLIGHT_TURN_OFF:
    case MSG_LOSE_FOCUS:        
        STOP_TIMER(s_key_lock_effect1_info.anim_timer);
        STOP_TIMER(s_key_lock_effect1_info.tp_move_timer);
        SetMoveLayerPosEffect1(s_key_lock_effect1_info.layer_pos_y);
        break;
    case MSG_GET_FOCUS:
        break;        
        
    case MSG_TIMER:
        if (s_key_lock_effect1_info.tp_move_timer == *(uint8*)param)
        {
            GUI_POINT_T     tp_point = {0};
            MMI_TP_STATUS_E state = {0};
            
            MMK_GetLogicTPMsg(&state, &tp_point);

            if(tp_down_point.y > s_key_lock_effect1_info.layer_pos_y)
            {
                uint16 lcd_width = 0, lcd_height = 0;
                GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
                if(tp_point.y - tp_down_point.y > (lcd_height - s_key_lock_effect1_info.layer_pos_y)/2)
                {
                    STOP_TIMER(s_key_lock_effect1_info.tp_move_timer);
                    MMK_CloseWin(win_id);
                }
                else if(tp_point.y > tp_down_point.y )
                {
                    //if(abs(tp_point.y - tp_last_point.y) > 2)
                    if(tp_point.y != tp_last_point.y)
                    {
                        SetMoveLayerPosEffect1(s_key_lock_effect1_info.layer_pos_y + tp_point.y - tp_down_point.y);
                        tp_last_point.y = tp_point.y;
                    }   
                    START_TIMER(s_key_lock_effect1_info.tp_move_timer, KEY_LOCK_EFFECT1_TP_MOVE_TIME);
                }
                else
                {
                    if(tp_last_point.y != s_key_lock_effect1_info.layer_pos_y)
                    {
                        SetMoveLayerPosEffect1(s_key_lock_effect1_info.layer_pos_y);
                        tp_last_point.y = s_key_lock_effect1_info.layer_pos_y;
                    }
                    START_TIMER(s_key_lock_effect1_info.tp_move_timer, KEY_LOCK_EFFECT1_TP_MOVE_TIME);
                }
                
            }
        }
        else if(s_key_lock_effect1_info.anim_timer == *(uint8*)param)
        {
            DrawAnimFrameEffect1(anim_frame_num % KEY_LOCK_EFFECT1_ANIM_FRAME_NUM);
            if(anim_frame_num < KEY_LOCK_EFFECT1_ANIM_FRAME_NUM * 2)
            {
                START_TIMER(s_key_lock_effect1_info.anim_timer, KEY_LOCK_EFFECT1_ANIM_TIME);
                anim_frame_num++;
            }
            else
            {
                anim_frame_num = 0;
                if(is_tp_down_again)
                {
                    is_tp_down_again = FALSE;
                    START_TIMER(s_key_lock_effect1_info.anim_timer, KEY_LOCK_EFFECT1_ANIM_TIME);
                }
                else
                {
                    UpdateLayerEffect1(FALSE);
                    s_key_lock_effect1_info.anim_timer = 0;
                }
            }
        }
        else
        {
			recode = MMI_RESULT_FALSE;		
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        tp_down_point.y = MMK_GET_TP_Y(param);
        if(tp_down_point.y > s_key_lock_effect1_info.layer_pos_y)
        {
            if(s_key_lock_effect1_info.anim_timer > 0)
            {
                is_tp_down_again = TRUE;
            }
            else
            {
                anim_frame_num = 0;
                START_TIMER(s_key_lock_effect1_info.anim_timer, KEY_LOCK_EFFECT1_ANIM_TIME);
                UpdateLayerEffect1(TRUE);        
            }
            START_TIMER(s_key_lock_effect1_info.tp_move_timer, KEY_LOCK_EFFECT1_TP_MOVE_TIME);
            tp_last_point.y = tp_down_point.y;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
    {
        GUI_POINT_T     tp_point = {0};
        uint16 lcd_width = 0, lcd_height = 0;
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
        
        tp_point.y = MMK_GET_TP_Y(param);
        if(tp_down_point.y >= s_key_lock_effect1_info.layer_pos_y)
        {
            UILAYER_SetLayerPosition(&s_key_lock_effect1_info.layer, 0, s_key_lock_effect1_info.layer_pos_y);
            SetMoveLayerPosEffect1(s_key_lock_effect1_info.layer_pos_y);
        }
        STOP_TIMER(s_key_lock_effect1_info.tp_move_timer);
        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEY_LOCK_CLOSE_WIN:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:  
        STOP_TIMER(s_key_lock_effect1_info.tp_move_timer);
        STOP_TIMER(s_key_lock_effect1_info.anim_timer);
        break;
    case MSG_KEYLOCK_UNDEAL_EVENT:
        MMK_WinGrabFocus(win_id);
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_NOTIFY_STATUSBAR_PAINT:
    case MSG_NOTIFY_STATUSBAR_PAINT_END:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_NOTIFY_PENLONGOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_LCD_SWITCH:
 #if 0
    case MSG_KEYDOWN_RED:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYDOWN_HASH:
  #endif
        break;
    case MSG_KEYLONG_RED:
        if (MMIKL_IsPreviewState())
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    case MSG_APP_OK:
   // case MSG_APP_MENU:
        if (MMIKL_IsPreviewState())
        {
            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT1);
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
//  Description :  Display date.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayDate(GUI_LCD_DEV_INFO *dev, GUI_RECT_T rect, GUI_FONT_T font, GUI_ALIGN_E align, GUI_COLOR_T color)
{
    SCI_DATE_T date = {0};
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T m_string = {0};
    MMI_STRING_T final_string = {0};
    wchar m_buf[5] ={0}; 
    wchar datewstr[30] = {0};
    wchar wstr_buf[16] = {0};
    uint8 datestr[16] = {0};

    MMI_TEXT_ID_T week_table[] = {
                                 TXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
                                 TXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
                                 TXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
                                 TXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
                                 TXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
                                 TXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
                                 TXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
                                    };
    SCI_MEMSET(m_buf,0,sizeof(m_buf));
    SCI_MEMSET(datewstr,0,sizeof(datewstr));
    SCI_MEMSET(datestr,0,sizeof(datestr));
    SCI_MEMSET(wstr_buf,0,sizeof(wstr_buf));
    
    
    m_string.wstr_ptr = m_buf;
    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &m_string);
  
    MMIAPISET_GetCurrentDateStr(datestr,'-',sizeof(datestr));
    
    strncat(datestr," ",1); /*lint !e64*/
    MMIAPICOM_StrToWstr(datestr, wstr_buf);

    final_string.wstr_ptr = datewstr;
    MMIAPICOM_Wstrncpy(final_string.wstr_ptr,wstr_buf,MMIAPICOM_Wstrlen(wstr_buf));
    MMIAPICOM_Wstrncat(final_string.wstr_ptr, m_string.wstr_ptr,m_string.wstr_len);
    
    
    final_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(final_string.wstr_ptr);
    
    text_style.font_color = color;
    text_style.font = font;
    text_style.align = align;

    GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO *)dev,
                    (const GUI_RECT_T *)&rect,
                    (const GUI_RECT_T *)&rect,
                    (const MMI_STRING_T *)&final_string,
                    &text_style,
                    GUISTR_STATE_ALIGN,
                    GUISTR_TEXT_DIR_AUTO
                    );
}

#endif	//MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT

#endif
#endif



/*Edit by script, ignore 4 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:37:19 2012*/ //IGNORE9527
