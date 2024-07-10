/*****************************************************************************
** File Name:      mmikl_smartslide.c                                        *
** Author:         jian.ma                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe keylock smart slide function*
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create
******************************************************************************/

#ifndef _MMIKL_SMARTSLIDE_C_

#include "mmi_app_keylock_trc.h"
#ifdef MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmikl_smartslide.h"
#include "mmk_type.h"
#include "ui_layer.h"
#include "mmi_id.h"
#include "mmi_keylock_position.h"
#include "mmi_image.h"
#include "mmikl_effectmain.h"
#include "mmi_wallpaper_export.h"
#include "cafcontrol.h"
#include "mmikl_internal.h"
#include "mmiset_export.h"
#include "guires.h"
#include "mmiset_nv.h"
#include "mmikl_export.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmk_tp.h"
#include "mmiidle_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  LINK_NODE  MMI_LINK_NODE_T
#define BUBBLE_X_DEC_INIT_VALUE     4
#define BUBBLE_X_OFFSET_INIT    (rand()%3 - 1)
#define BUBBLE_RAND_RANGE_MAX     20
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


#define KEY_LOCK_EFFECT3_TP_MOVE_TIME           20
#define KEY_LOCK_EFFECT3_AUTO_MOVE_TIME       50
#define KEY_LOCK_EFFECT3_AUTO_MOVE_FRAME_NUM             5
#define KEY_LOCK_EFFECT3_CHARGE_TIME     50
#define KEY_LOCK_EFFECT3_BATT_UPDATE_TIME     2000

#define KEY_LOCK_EFFECT3_BATT_FADE_HEIGHT    20
#define KEY_LOCK_EFFECT3_BATT_VALUE_MAX    100 /* MAX_BAT_LEVEL */
#define KEY_LOCK_EFFECT3_BATT_COLOR     MMI_GREEN_COLOR
#define KEY_LOCK_EFFECT3_BATT_FADE_RANGE   220

#define  KEY_LOCK_EFFECT3_BUBBLE_GEN_NUM    1

typedef struct
{
    GUI_LCD_DEV_INFO bar_layer;
    GUI_LCD_DEV_INFO charge_layer;
    GUI_LCD_DEV_INFO batt_layer;

    uint16 bar_layer_pos_y;

    uint16 batt_layer_pos_y;
    uint16 batt_layer_height;
    uint16 batt_layer_width;
    
    uint16 charge_layer_pos_x;
    uint16 charge_layer_pos_y;
    
    uint8 tp_move_timer;    
    uint8 auto_move_timer;
    uint8 charge_timer;   
    uint8 batt_update_timer;
}MMI_KEY_LOCK_EFFECT3_INFO_T;

typedef enum
{
    BUBBLE_TYPE_S,
    BUBBLE_TYPE_M,
    BUBBLE_TYPE_L,
    BUBBLE_TYPE_MAX
}BUBBLE_TYPE_E;

typedef struct
{
    BUBBLE_TYPE_E type;
    GUI_POINT_T pos;
    uint8 x_dec;
    int8 x_offset;
//    uint16 color;
//    uint8 dir;
//    uint8 v;
//    uint8 a;
}MMI_BUBBLE_T;



LOCAL MMI_KEY_LOCK_EFFECT3_INFO_T s_key_lock_effect3_info = {0};
LOCAL LINK_NODE *s_bubble_link_head = PNULL;


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
//  Description :  set key lock control parameters of effect type3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetKeyLockControlParamEffect3(void);

/*****************************************************************************/
//  Description :  Init bubble effect.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void InitBubble(void);

/*****************************************************************************/
//  Description :  Add a bubble to bubble link.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AddBubble(uint32 data);

/*****************************************************************************/
//  Description :  Delete a bubble from bubble link.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DelBubbleNode(LINK_NODE *node);

/*****************************************************************************/
//  Description :  Destroy the bubble link.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DestroyBubble(void);
/*****************************************************************************/
//  Description :  Generate a new bubble.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void GenNewBubble(uint16 offset_x, uint16 offset_y, uint16 area_width, uint16 area_height);

/*****************************************************************************/
//  Description :  Rise all the bubbles in bubble link.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void RiseBubble(uint16 margin_l, uint16 margin_r, uint16 top);

/*****************************************************************************/
//  Description :  Display the bubble to the appointed layer.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayBubble(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *layer);
/*****************************************************************************/
//  Description :  Make a judgement the the if the phone in charging state.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInCharging(void);

/*****************************************************************************/
//  Description :  Get move layer of effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetMoveLayerEffect3(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if(s_key_lock_effect3_info.bar_layer.block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_key_lock_effect3_info.bar_layer))
        {
            lcd_dev_info = s_key_lock_effect3_info.bar_layer;
        }
    }
    return lcd_dev_info;
}

/*****************************************************************************/
//  Description :  Get charge layer of effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetChargeLayerEffect3(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if(s_key_lock_effect3_info.charge_layer.block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_key_lock_effect3_info.charge_layer))
        {
            lcd_dev_info = s_key_lock_effect3_info.charge_layer;
        }
    }
    return lcd_dev_info;
}

/*****************************************************************************/
//  Description :  Get battery layer of effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetBattLayerEffect3(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if(s_key_lock_effect3_info.batt_layer.block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_key_lock_effect3_info.batt_layer))
        {
            lcd_dev_info = s_key_lock_effect3_info.batt_layer;
        }
    }
    return lcd_dev_info;
}

/*****************************************************************************/
//  Description :  Append move layer of effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendMoveLayerEffect3(void)
{
    UILAYER_APPEND_BLT_T append_layer;

    append_layer.lcd_dev_info = GetMoveLayerEffect3();
    if(append_layer.lcd_dev_info.block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}

/*****************************************************************************/
//  Description :  Append charge layer of effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendChargeLayerEffect3(void)
{
    UILAYER_APPEND_BLT_T append_layer;

    append_layer.lcd_dev_info = GetChargeLayerEffect3();
    if(append_layer.lcd_dev_info.block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}

/*****************************************************************************/
//  Description :  Append battery layer of effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendBattLayerEffect3(void)
{
    UILAYER_APPEND_BLT_T append_layer;

    append_layer.lcd_dev_info = GetBattLayerEffect3();
    if(append_layer.lcd_dev_info.block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}

/*****************************************************************************/
//  Description :  set key lock control parameters of effect type3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateMoveLayerEffect3(void)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 image_width = 0, image_height = 0;

//    if(s_key_lock_effect3_info.bar_layer.block_id != 0)
//    {
//        UILAYER_ReleaseLayer(&s_key_lock_effect3_info.bar_layer);
//    }
    
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT3_BAR_NORMAL, MMIKL_CLOCK_DISP_WIN_ID);
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = MMIKL_CLOCK_DISP_WIN_ID;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = lcd_width;
    create_info.height = image_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    result = UILAYER_CreateLayer(
        &create_info,  
        &layer
        );
    
    s_key_lock_effect3_info.bar_layer_pos_y = lcd_height - image_height * 2;
    
    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_key_lock_effect3_info.bar_layer = layer;
        UILAYER_SetLayerPosition(&layer, 0, s_key_lock_effect3_info.bar_layer_pos_y);
        
        AppendMoveLayerEffect3();
        UILAYER_Clear(&layer);
    }
    else
    {
        s_key_lock_effect3_info.bar_layer.block_id = 0;
    }
    
}

/*****************************************************************************/
//  Description :  Append charge layer of effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateChargeLayerEffect3(void)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 image_width = 0, image_height = 0;
    uint16 width = 0, height = 0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);


    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_KL_EFFECT3_CHARGE_BG, MMIKL_CLOCK_DISP_WIN_ID);
    width = image_width;
    s_key_lock_effect3_info.charge_layer_pos_x = (lcd_width - width)/2;
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT3_BAR_NORMAL, MMIKL_CLOCK_DISP_WIN_ID);
    s_key_lock_effect3_info.charge_layer_pos_y = s_key_lock_effect3_info.bar_layer_pos_y + image_height - KEY_LOCK_EFFECT3_MOVE_BAR_IMG_DOWN_MARGIN;;
    height = lcd_height - s_key_lock_effect3_info.charge_layer_pos_y;
    
    MMI_KL_CREATE_LAYER(
        create_info, 
        MMIKL_CLOCK_DISP_WIN_ID, 
        width, 
        height,
        layer, 
        result
        );
    
    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_key_lock_effect3_info.charge_layer = layer;
        AppendChargeLayerEffect3();
        UILAYER_SetLayerPosition(&layer, s_key_lock_effect3_info.charge_layer_pos_x, s_key_lock_effect3_info.charge_layer_pos_y);
        UILAYER_Clear(&layer);
    }
    else
    {
        s_key_lock_effect3_info.charge_layer.block_id = 0;
    }
}

/*****************************************************************************/
//  Description :  Append battery layer of effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateBattLayerEffect3(void)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    GUI_LCD_DEV_INFO     bar_layer = {0};
    UILAYER_INFO_T    layer_info = {0};
    
    //uint16 lcd_width = 0, lcd_height = 0;
    uint16 width = 0, height = 0;
    uint16 start_x = 0, start_y  = 0;
    
    bar_layer = GetMoveLayerEffect3();
    UILAYER_GetLayerInfo(&s_key_lock_effect3_info.bar_layer, &layer_info);
        
    start_x = 0;
    start_y = s_key_lock_effect3_info.bar_layer_pos_y + KEY_LOCK_EFFECT3_MOVE_BAR_IMG_UP_MARGIN;
    width = layer_info.layer_width;
    height = layer_info.layer_height - (KEY_LOCK_EFFECT3_MOVE_BAR_IMG_DOWN_MARGIN + KEY_LOCK_EFFECT3_MOVE_BAR_IMG_UP_MARGIN);
    
    MMI_KL_CREATE_LAYER(create_info, MMIKL_CLOCK_DISP_WIN_ID, width, height, layer, result);

    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_key_lock_effect3_info.batt_layer = layer;
        s_key_lock_effect3_info.batt_layer_pos_y = start_y;
        s_key_lock_effect3_info.batt_layer_height = height;
        s_key_lock_effect3_info.batt_layer_width = width;
        
        AppendBattLayerEffect3();
        UILAYER_SetLayerPosition(&layer, start_x, start_y);
        UILAYER_Clear(&layer);
    }
    else
    {
        s_key_lock_effect3_info.charge_layer.block_id = 0;
    }
}

/*****************************************************************************/
//  Description :  set key lock control parameters of effect type3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetKeyLockControlParamEffect3(void)
{
    GUI_BG_T                    bg = {0};

    SCI_MEMSET(&s_key_lock_effect3_info, 0, sizeof(s_key_lock_effect3_info));
    CreateMoveLayerEffect3();
    
    //set softkey common bg
    GUIWIN_SetSoftkeyCommonBg(MMIKL_CLOCK_DISP_WIN_ID,&bg);

}

/*****************************************************************************/
//  Description :  Display clock for effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayDigitalClockEffect3(GUI_LCD_DEV_INFO *dev, GUI_RECT_T rect, MMI_IMAGE_ID_T start_img_id)
{
    MMIKL_DisplayDigitalClock(dev, rect, start_img_id);
}

/*****************************************************************************/
//  Description : DisplayKeyLockTips not use layer just directly draw to main lcd
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayKeyLockTipsEffect3(void)
{
    uint16                      str_width = 0;
    GUI_RECT_T                  tips_rect = {0};
    MMI_STRING_T                tips_string = {0};
    GUISTR_STYLE_T              str_style = {0};
    GUISTR_STATE_T              str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK;
    uint16                      image_width = 0,image_height = 0;
    uint16                      lcd_width = 0;
    uint16                      lcd_height = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    uint8                       str_margin = 4;
    
    //get rect info
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT3_TIPS_BG, MMIKL_CLOCK_DISP_WIN_ID);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    //get string
    MMI_GetLabelTextByLang(TXT_KL_SLIDE_DOWN, &tips_string);

    //set string style
    str_style.font       = MMI_DEFAULT_BIG_FONT;
    str_style.font_color = MMI_DEFAULT_TEXT_COLOR;
    str_style.align      = ALIGN_HVMIDDLE;

    //get string width
    str_width = GUISTR_GetStringWidth(&str_style,
        &tips_string,
        str_state);

    //get tips rect
    if(str_width > lcd_width)
    {
        tips_rect.left = 0;
    }
    else
    {
        tips_rect.left = (lcd_width - (str_width + str_margin * 2) ) / 2;
    }
    tips_rect.right = lcd_width - tips_rect.left + 1;
    tips_rect.bottom = s_key_lock_effect3_info.bar_layer_pos_y - 1;
    tips_rect.top  = tips_rect.bottom - image_height;

    if((str_width + str_margin*2) > lcd_width) //Ë«ÐÐÏÔÊ¾ 
    {
        //str_style.font = MMI_DEFAULT_SMALL_FONT;
        tips_rect.top  = tips_rect.bottom - (image_height+1)*3/2;
    }
    
    IMG_EnableTransparentColor(TRUE);
    //display bg image
    GUIRES_DisplayImg(PNULL,&tips_rect,
        PNULL,
        MMIKL_CLOCK_DISP_WIN_ID,
        IMAGE_KL_EFFECT3_TIPS_BG,
        &lcd_dev_info);


    //display text
    tips_rect.right = tips_rect.right - str_margin;
    tips_rect.left = tips_rect.left + str_margin;
    tips_rect.bottom = tips_rect.bottom - str_margin;
    tips_rect.top  = tips_rect.top + str_margin;
    GUISTR_DrawTextToLCDInRect(&lcd_dev_info,
        &tips_rect,
        &tips_rect,
        &tips_string,
        &str_style,
        str_state,
        GUISTR_TEXT_DIR_AUTO);
    IMG_EnableTransparentColor(FALSE);

}

/*****************************************************************************/
//  Description : Display Missed event of effect3.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
void DisplayMissedEventEffect3(void)
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
    rect.top = (lcd_height - s_key_lock_effect3_info.bar_layer_pos_y - rect_height) / 2;
    rect.bottom = rect.top + rect_height;
    
    MMIKL_DisplayMissedEvent(MMIKL_CLOCK_DISP_WIN_ID, &rect, &lcd_dev);    
}

/*****************************************************************************/
//  Description : Update move layer of effect3.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void UpdateLayerEffect3(BOOLEAN is_pen_down, uint16 x, uint16 y)
{
#define IS_IN_RECT(x, y, start_x, start_y, width, height)   \
    (x >= start_x && x <= start_x + width && y >= start_y && y <= start_y + height)
#define IS_IN_LEFT_RECT(x, y, start_x, start_y, width, height) \
    (x >= start_x && x <= start_x + width / 3 && y >= start_y && y <= start_y + height)
#define IS_IN_MID_RECT(x, y, start_x, start_y, width, height) \
    (x >= start_x + width / 3 && x <= start_x + width * 2 / 3 && y >= start_y && y <= start_y + height)
#define IS_IN_RIGHT_RECT(x, y, start_x, start_y, width, height) \
    (x >= start_x + width * 2 / 3 && x <= start_x + width && y >= start_y && y <= start_y + height)
    
    uint16 image_width = 0, image_height = 0;
    uint16 lcd_width = 0, lcd_height = 0;
    int16 layer_pos_x = 0, layer_pos_y = 0;
    uint16 height = 0, width = 0;
    uint16 start_x = 0, start_y = 0;
    GUI_POINT_T pos = {0};
    GUI_LCD_DEV_INFO layer = {0};
    GUI_RECT_T rect = {0};
    MMI_IMAGE_ID_T img_id = 0;
    UILAYER_INFO_T layer_info = {0};

    layer = GetMoveLayerEffect3();
    UILAYER_GetLayerInfo(&layer, &layer_info);
    UILAYER_GetLayerPosition(&layer, &layer_pos_x, &layer_pos_y);
   
    
    if(is_pen_down)
    {
        start_x = layer_pos_x;
        start_y = layer_pos_y + KEY_LOCK_EFFECT3_MOVE_BAR_IMG_UP_MARGIN;
        width = layer_info.layer_width;
        height = layer_info.layer_height - (KEY_LOCK_EFFECT3_MOVE_BAR_IMG_DOWN_MARGIN + KEY_LOCK_EFFECT3_MOVE_BAR_IMG_UP_MARGIN);
        if(IS_IN_RECT(x, y, start_x, start_y, width, height))
        {
            if(IS_IN_LEFT_RECT(x, y, start_x, start_y, width, height))
            {
                img_id = IMAGE_KL_EFFECT3_BAR_LFEF_HILIGHT;
            }
            else if (IS_IN_MID_RECT(x, y, start_x, start_y, width, height))
            {
                img_id = IMAGE_KL_EFFECT3_BAR_MID_HILIGHT;
            }
            else if (IS_IN_RECT(x, y, start_x, start_y, width, height))
            {
                img_id = IMAGE_KL_EFFECT3_BAR_RIGHT_HILIGHT;
            }
            else
            {
                return;/* would never get here */
            }
        }
        else
        {
            return;
        }
    }
    else 
    {
        img_id = IMAGE_KL_EFFECT3_BAR_NORMAL;
    }

    if(layer.block_id != 0)
    {
        UILAYER_Clear(&layer);
    }
    else
    {
        pos.y = s_key_lock_effect3_info.bar_layer_pos_y;
    }
    
    UILAYER_SetLayerPosition(&layer, 0, 0);
    
    GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, img_id, &layer);
    
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT3_HILIGHT_DIGIT0, MMIKL_CLOCK_DISP_WIN_ID);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    rect.top = KEY_LOCK_EFFECT3_TIME_UP_MARGIN + pos.y;
    rect.right = lcd_width - 1;
    rect.bottom = rect.top + image_height;
    
    if(is_pen_down)
    {
        DisplayDigitalClockEffect3(&layer, rect, IMAGE_KL_EFFECT3_HILIGHT_DIGIT0);
    }
    else
    {
        DisplayDigitalClockEffect3(&layer, rect, IMAGE_KL_EFFECT3_NORMAL_DIGIT0);
    }
    
    UILAYER_SetLayerPosition(&layer, layer_pos_x, layer_pos_y);        
}

/*****************************************************************************/
//  Description : make a judgement if tp point is in move layer.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpPointInMoveLayerEffect3(uint16 x, uint16 y)
{
    GUI_LCD_DEV_INFO layer = {0};
    UILAYER_INFO_T layer_info = {0};
    BOOLEAN res = FALSE;
    int16 layer_pos_x = 0, layer_pos_y = 0;

    layer = GetMoveLayerEffect3();

    if(layer.block_id != 0)
    {
        UILAYER_GetLayerInfo(&layer, &layer_info);
        UILAYER_GetLayerPosition(&layer, &layer_pos_x, &layer_pos_y);    
        if(x >= layer_pos_x 
            && x <= layer_pos_x + layer_info.layer_width 
            && y >= layer_pos_y + KEY_LOCK_EFFECT3_MOVE_BAR_IMG_UP_MARGIN 
            && y <= layer_pos_y + layer_info.layer_height - KEY_LOCK_EFFECT3_MOVE_BAR_IMG_DOWN_MARGIN
        )
        {
            res = TRUE;
        }
        else
        {
            res = FALSE;
        }
    }
    return res;
}

/*****************************************************************************/
//  Description :  set move layer pos off effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetMoveLayerPosEffect3(uint16 y)
{
    GUI_LCD_DEV_INFO layer = GetMoveLayerEffect3();

    if(layer.block_id != 0)
    {
        UILAYER_SetLayerPosition(&layer, 0, y);
    }    
}

/*****************************************************************************/
//  Description : Move layer auto move by anim frame index.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void MoveLayerAutoMoveEffect3(uint16 frame_index)
{
    GUI_LCD_DEV_INFO layer = {0};
    UILAYER_INFO_T layer_info = {0};
    int16 layer_pos_x = 0, layer_pos_y = 0;
    uint16 new_layer_pos_y = 0;

    layer = GetMoveLayerEffect3();

    if(layer.block_id != 0)
    {
        UILAYER_GetLayerInfo(&layer, &layer_info);
        UILAYER_GetLayerPosition(&layer, &layer_pos_x, &layer_pos_y);    
        new_layer_pos_y = s_key_lock_effect3_info.bar_layer_pos_y + layer_info.layer_height + layer_info.layer_height * frame_index / KEY_LOCK_EFFECT3_AUTO_MOVE_FRAME_NUM;
        UILAYER_SetLayerPosition(&layer, 0, new_layer_pos_y);
    }    
}

/*****************************************************************************/
//  Description : make a judgement if tp move distance is enough to unlock key.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisEnoughToUnlockEffect3(uint16 distance)
{
    GUI_LCD_DEV_INFO layer = {0};
    UILAYER_INFO_T layer_info = {0};

    layer = GetMoveLayerEffect3();
    
    if(layer.block_id != 0)
    {
        UILAYER_GetLayerInfo(&layer, &layer_info);
    
        if(distance > layer_info.layer_height)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Do unlock key.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void UnlockKeyEffect3(MMI_WIN_ID_T win_id, GUI_POINT_T tp_down_point)
{
    GUI_LCD_DEV_INFO layer = {0};
    UILAYER_INFO_T layer_info = {0};

    layer = GetMoveLayerEffect3();
    
    if(layer.block_id != 0)
    {
        UILAYER_GetLayerInfo(&layer, &layer_info);
        if(tp_down_point.x <= layer_info.layer_width/3)
        {
            MMIKL_SetProcOpenFlag(PROC_CALL);
        }
        else if(tp_down_point.x >= layer_info.layer_width * 2 / 3)
        {
            MMIKL_SetProcOpenFlag(PROC_SMS);
        }   
        //MMK_CloseWin(win_id);
        MMK_PostMsg(win_id, MSG_KEY_LOCK_CLOSE_WIN,NULL, 0);
    }
}

/*****************************************************************************/
//  Description :  Init charging.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void InitChargeEffect3(void)
{
    InitBubble();
}

/*****************************************************************************/
//  Description :  Deinit chargeing
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DeInitChargeEffect3(void)
{
    DestroyBubble();
}

/*****************************************************************************/
//  Description :  Generate followed charge anim data.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void GenNextChargeAnimEffect3(void)
{
    uint16 lcd_width = 0, lcd_height = 0;
    UILAYER_INFO_T layer_info = {0};
    uint8 num = rand()%KEY_LOCK_EFFECT3_BUBBLE_GEN_NUM + 1;
    uint32 i = 0;
    static uint8 gen_duation_rand = 0;

    if(gen_duation_rand == 0)
    {
        UILAYER_GetLayerInfo(&s_key_lock_effect3_info.charge_layer, &layer_info);
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);        
        
        while( i++ < num)
        {
            //GenNewBubble(s_key_lock_effect3_info.charge_layer_pos_x, lcd_height - 1, layer_info.layer_width, layer_info.layer_height);
            GenNewBubble(lcd_width/2 - 2, lcd_height - 1, 4, layer_info.layer_height);
        }
        gen_duation_rand =  (1 +  rand()%3) * 4;
    }
    else
    {
        gen_duation_rand--;
    }
}

/*****************************************************************************/
//  Description :  Display charge animate.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayChargeAnimEffect3(void)
{
    GUI_LCD_DEV_INFO layer = GetChargeLayerEffect3();
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 img_width = 0, img_height = 0;
    GUI_POINT_T point = {0};

    UILAYER_Clear(&layer);
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);        
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_EFFECT3_CHARGE_BG, MMIKL_CLOCK_DISP_WIN_ID);
    point.y = lcd_height - img_height;
    point.x = (lcd_width - img_width)/2;
    GUIRES_DisplayImg(&point, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT3_CHARGE_BG, &layer);

    /* margin_r: -4 is to display the whole bubble, otherwise it may display part of the bubble */
    RiseBubble(s_key_lock_effect3_info.charge_layer_pos_x, lcd_width - s_key_lock_effect3_info.charge_layer_pos_x - 4,s_key_lock_effect3_info.charge_layer_pos_y);

    DisplayBubble(MMIKL_CLOCK_DISP_WIN_ID, &layer);
}

/*****************************************************************************/
//  Description :  Fade the battery effect.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void FadeBattEffect3(uint32 *buff, uint16 fade_height, uint16 buff_width, uint16 whole_height, uint8 begin_alpha, uint8 end_alpha)
{   
    uint32 i = 0;
    uint32 j = 0;
    uint32 *tmp_ptr = buff;
    uint8 alpha = 0;

    if(PNULL == buff)
    {
        return;
    }

    for(i = 1; i <= fade_height; i++)
    {          
        //alpha = begin_alpha + (end_alpha - begin_alpha) * i / fade_height;
        alpha = begin_alpha + KEY_LOCK_EFFECT3_BATT_FADE_RANGE * i / fade_height;
        for(j = 0; j < buff_width; j++)
        {
            if( 0 != *tmp_ptr )
            {
                *tmp_ptr = (alpha<<24) | ((*tmp_ptr)&0x00FFFFFF);
            }
            tmp_ptr++;
        }
    }
    for(i = 0 + 1; i <= whole_height; i++)
    {
        alpha = end_alpha;
        for(j = 0; j < buff_width; j++)
        {
            if( 0 != *tmp_ptr )
            {
                *tmp_ptr = (alpha<<24) | ((*tmp_ptr)&0x00FFFFFF);
            }
            tmp_ptr++;
        }        
    }
}

/*****************************************************************************/
//  Description :  update battery.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void UpdateBattEffect3()
{
    uint8 cur_batt = 0;
    uint16 fade_height = 0;
    uint16 batt_height = 0;
    uint16 batt_hilight_height = 0;
    GUI_LCD_DEV_INFO layer = {0};
    GUI_RECT_T rect = {0};
    UILAYER_INFO_T layer_info = {0};
    uint32 *layer_buf_ptr = PNULL;
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    
    layer = GetBattLayerEffect3();
    if(layer.block_id != 0)
    {
        cur_batt = p_chgmng_info->bat_remain_cap;
        if(cur_batt > KEY_LOCK_EFFECT3_BATT_VALUE_MAX)
        {
            cur_batt = KEY_LOCK_EFFECT3_BATT_VALUE_MAX;
        }
        batt_hilight_height = s_key_lock_effect3_info.batt_layer_height * cur_batt/KEY_LOCK_EFFECT3_BATT_VALUE_MAX;

        if(batt_hilight_height + KEY_LOCK_EFFECT3_BATT_FADE_HEIGHT > s_key_lock_effect3_info.batt_layer_height)
        {
            fade_height = s_key_lock_effect3_info.batt_layer_height - batt_hilight_height;
        }
        else
        {
            fade_height = KEY_LOCK_EFFECT3_BATT_FADE_HEIGHT;
        }
        batt_height = batt_hilight_height + fade_height;

        rect.top = s_key_lock_effect3_info.batt_layer_pos_y + s_key_lock_effect3_info.batt_layer_height - batt_height;
        rect.right = s_key_lock_effect3_info.batt_layer_width - 1;
        rect.bottom = rect.top + batt_height;
        UILAYER_Clear(&layer);
        LCD_FillRect(&layer, rect, KEY_LOCK_EFFECT3_BATT_COLOR);
        //UILAYER_WeakLayerAlpha(&layer, KEY_LOCK_EFFECT3_BATT_FADE_RANGE);
        UILAYER_GetLayerInfo(&layer, &layer_info);
        /*
        SCI_MEMSET(
            layer_info.layer_buf_ptr + layer_info.mem_width * (layer_info.layer_height - batt_height), 
            0x7FffFFff,
            layer_info.mem_width + batt_height
            ); 
            */
        layer_buf_ptr = (uint32*)UILAYER_GetLayerBufferPtr(&layer);
        if ((PNULL == layer_buf_ptr)
            || (s_key_lock_effect3_info.batt_layer_height < batt_height)
            || (s_key_lock_effect3_info.batt_layer_height - batt_height+batt_hilight_height)>layer_info.layer_height)
        {
            return;
        }
        FadeBattEffect3(
            (uint32*)layer_buf_ptr + (s_key_lock_effect3_info.batt_layer_height - batt_height)*layer_info.mem_width, 
            fade_height, 
            layer_info.mem_width, 
            batt_hilight_height, 
            0, 
            255
            );
    }
}

/*****************************************************************************/
//  Description :  Set the layer order.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetLayerOrderEffect3(void)
{
        UILAYER_RemoveBltLayer(&s_key_lock_effect3_info.charge_layer);
        UILAYER_RemoveBltLayer(&s_key_lock_effect3_info.batt_layer);
        UILAYER_RemoveBltLayer(&s_key_lock_effect3_info.bar_layer);    
        AppendBattLayerEffect3();
        AppendChargeLayerEffect3();
        AppendMoveLayerEffect3();
}

/*****************************************************************************/
//  Description :  Stop charge animate.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void StopChargeAnimEffect3(void)
{
    if(IsInCharging())
    {  
        UILAYER_RemoveBltLayer(&s_key_lock_effect3_info.charge_layer);
        UILAYER_RemoveBltLayer(&s_key_lock_effect3_info.batt_layer);
        STOP_TIMER(s_key_lock_effect3_info.charge_timer);
        STOP_TIMER(s_key_lock_effect3_info.batt_update_timer);
    }
}

/*****************************************************************************/
//  Description :  Stop charge animate.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note: Only used in pen press up Msg
/*****************************************************************************/
LOCAL void ResumeChargeAnimEffect3(void)
{
    if(IsInCharging())
    {  
        SetLayerOrderEffect3();
        START_TIMER(s_key_lock_effect3_info.charge_timer, KEY_LOCK_EFFECT3_CHARGE_TIME);
        START_TIMER(s_key_lock_effect3_info.batt_update_timer, KEY_LOCK_EFFECT3_BATT_UPDATE_TIME);
    }
}

/******************************************************************************/
//  Description : handle kl display window msg of effect3.
//  Global resource dependence : none
//  Author: 
//  Note: Slide down the rectangle bar unlock style
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleSmartSlideDoneWinMsg(
                                                MMI_WIN_ID_T    win_id, 
                                                uint16          msg_id, 
                                                DPARAM          param
                                                )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    static GUI_POINT_T tp_down_point = {0};
    static GUI_POINT_T tp_last_point = {0};
    static uint16 auto_move_frame_index = 0;
    
    //SCI_TRACE_LOW:"HandleKLWinEffect3Msg: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_SMARTSLIDE_1019_112_2_18_2_31_12_13,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetKeyLockControlParamEffect3();
        InitChargeEffect3();
        
        tp_down_point.y = 0;
        tp_last_point.y = 0;
        break;
    case MSG_FULL_PAINT:
        DisplayMissedEventEffect3();
        if(s_key_lock_effect3_info.tp_move_timer != 0 || s_key_lock_effect3_info.auto_move_timer != 0)
        {
            UpdateLayerEffect3(TRUE, tp_down_point.x, tp_last_point.y);
        }
        else
        {
            UpdateLayerEffect3(FALSE, 0, 0);
            if(IsInCharging() && MMK_IsFocusWin(win_id))
            {  
                if(s_key_lock_effect3_info.charge_layer.block_id == 0)
                {
                    UILAYER_RemoveBltLayer(&s_key_lock_effect3_info.bar_layer);
                    CreateBattLayerEffect3();
                    CreateChargeLayerEffect3();
                    AppendMoveLayerEffect3();
                }
                else
                {
                    SetLayerOrderEffect3();
                }
                UpdateBattEffect3();
                START_TIMER(s_key_lock_effect3_info.charge_timer, KEY_LOCK_EFFECT3_CHARGE_TIME);
                START_TIMER(s_key_lock_effect3_info.batt_update_timer, KEY_LOCK_EFFECT3_BATT_UPDATE_TIME);
            }
            else
            {
                AppendMoveLayerEffect3();
                STOP_TIMER(s_key_lock_effect3_info.charge_timer);
                STOP_TIMER(s_key_lock_effect3_info.batt_update_timer);
                UILAYER_RELEASELAYER(&s_key_lock_effect3_info.charge_layer);   /*lint !e506 !e774*/
                UILAYER_RELEASELAYER(&s_key_lock_effect3_info.batt_layer);   /*lint !e506 !e774*/
//                UILAYER_ReleaseLayer(&s_key_lock_effect3_info.charge_layer);
//                UILAYER_ReleaseLayer(&s_key_lock_effect3_info.batt_layer);
                s_key_lock_effect3_info.charge_layer.block_id = 0;
                s_key_lock_effect3_info.batt_layer.block_id = 0;
            }
        }
        if(0 != MMIKL_GetTipsTimerID() )
        {
            DisplayKeyLockTipsEffect3();
        }
        break;
    case MSG_BACKLIGHT_TURN_ON:
        UpdateLayerEffect3(FALSE, 0, 0);
        tp_down_point.y = 0;
        tp_last_point.y = 0;
        break;
    case MSG_KEYDOWN_RED:
        if (MMIKL_IsPreviewState())
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_BACKLIGHT_TURN_OFF:
    case MSG_LOSE_FOCUS:
        STOP_TIMER(s_key_lock_effect3_info.tp_move_timer);
        STOP_TIMER(s_key_lock_effect3_info.charge_timer);
        STOP_TIMER(s_key_lock_effect3_info.batt_update_timer);
        SetMoveLayerPosEffect3(s_key_lock_effect3_info.bar_layer_pos_y);
        tp_down_point.y = 0;
        tp_last_point.y = 0;
        break;

    case MSG_GET_FOCUS:
        if(MMIKL_GetWinStatus() == KL_UNLOCKED)
        {
            MMK_CloseWin(win_id);
        } 
        break;
        
  //  case MSG_APP_MENU:
    case MSG_APP_OK:
        if (MMIKL_IsPreviewState())
        {
            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT3);
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
      
    case MSG_TIMER:
        if (s_key_lock_effect3_info.tp_move_timer == *(uint8*)param)
        {
            GUI_POINT_T     tp_point = {0};
            MMI_TP_STATUS_E state = {0};
            
            MMK_GetLogicTPMsg(&state, &tp_point);

            if(tp_point.y > tp_down_point.y)
            {
                if(IsDisEnoughToUnlockEffect3(tp_point.y - tp_down_point.y))
                {
                    auto_move_frame_index = 0;
                    MoveLayerAutoMoveEffect3(auto_move_frame_index++);
                    START_TIMER(s_key_lock_effect3_info.auto_move_timer, KEY_LOCK_EFFECT3_AUTO_MOVE_TIME);
                    RESET_TIMER_KL(s_key_lock_effect3_info.tp_move_timer);
                }
                else
                {
                    if(tp_point.y != tp_last_point.y)
                    {
                        SetMoveLayerPosEffect3(s_key_lock_effect3_info.bar_layer_pos_y + tp_point.y - tp_down_point.y);
                        tp_last_point.y = tp_point.y;
                    }
                    START_TIMER(s_key_lock_effect3_info.tp_move_timer, KEY_LOCK_EFFECT3_TP_MOVE_TIME);
                }
            }
            else
            {
                if(tp_last_point.y != s_key_lock_effect3_info.bar_layer_pos_y)
                {
                    SetMoveLayerPosEffect3(s_key_lock_effect3_info.bar_layer_pos_y);
                    tp_last_point.y = s_key_lock_effect3_info.bar_layer_pos_y;
                }
                START_TIMER(s_key_lock_effect3_info.tp_move_timer, KEY_LOCK_EFFECT3_TP_MOVE_TIME);
            }
        }
        else if(s_key_lock_effect3_info.auto_move_timer == *(uint8*)param)
        {
            MoveLayerAutoMoveEffect3(auto_move_frame_index++);
            if(auto_move_frame_index > KEY_LOCK_EFFECT3_AUTO_MOVE_FRAME_NUM)
            {
                UnlockKeyEffect3(win_id, tp_down_point);
            }
            else
            {
                START_TIMER(s_key_lock_effect3_info.auto_move_timer, KEY_LOCK_EFFECT3_AUTO_MOVE_TIME);
            }
        }
        else if(MMIKL_GetTipsTimerID() == *(uint8*)param)
        {
            MMIKL_StopTipsShowTimer();
            if(MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            }
        }
        else if(s_key_lock_effect3_info.charge_timer == *(uint8 *)param)
        {
            DisplayChargeAnimEffect3();
            GenNextChargeAnimEffect3();
            START_TIMER(s_key_lock_effect3_info.charge_timer, KEY_LOCK_EFFECT3_CHARGE_TIME);
        }
        else if(s_key_lock_effect3_info.batt_update_timer == *(uint8 *)param)
        {
            UpdateBattEffect3();
            START_TIMER(s_key_lock_effect3_info.batt_update_timer, KEY_LOCK_EFFECT3_BATT_UPDATE_TIME);
        }
		else
		{
			recode = MMI_RESULT_FALSE;		
		}
        
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        if (KL_LOCKED == MMIKL_GetWinStatus()||MMIKL_IsPreviewState())
        {
            GUI_POINT_T point = {0};
        
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(0 == MMIKL_GetTipsTimerID() )
            {
                DisplayKeyLockTipsEffect3();
                MMIKL_StopUnlockConfirmTimer();
            }
            if(IsTpPointInMoveLayerEffect3(point.x, point.y))
            {
                tp_down_point.x = point.x;
                tp_down_point.y = point.y;

                StopChargeAnimEffect3();
                UpdateLayerEffect3(TRUE, point.x, point.y);
                START_TIMER(s_key_lock_effect3_info.tp_move_timer, KEY_LOCK_EFFECT3_TP_MOVE_TIME);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if(s_key_lock_effect3_info.tp_move_timer != 0)
        {
            STOP_TIMER(s_key_lock_effect3_info.tp_move_timer);
            tp_down_point.x = 0;
            tp_down_point.y = 0;
            ResumeChargeAnimEffect3();
            SetMoveLayerPosEffect3(s_key_lock_effect3_info.bar_layer_pos_y);
            UpdateLayerEffect3(FALSE, 0, 0);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEY_LOCK_CLOSE_WIN:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:  
        //stop timer
        //MMIKL_StopTipsShowTimer();
        //MMIKL_StopUnlockConfirmTimer();
        STOP_TIMER(s_key_lock_effect3_info.tp_move_timer);
        STOP_TIMER(s_key_lock_effect3_info.batt_update_timer);
        STOP_TIMER(s_key_lock_effect3_info.charge_timer);
        DeInitChargeEffect3();
        
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;

    }
    
    return recode;
}
/*****************************************************************************/
//  Description :  Init bubble effect.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void InitBubble(void)
{
    srand(BUBBLE_RAND_RANGE_MAX);
    if(s_bubble_link_head != PNULL)
    {
        DestroyBubble();
    }
    s_bubble_link_head = MMILINK_CreateHead(0);
}


/*****************************************************************************/
//  Description :  Add a bubble to bubble link.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AddBubble(uint32 data)
{
    if(s_bubble_link_head != PNULL)
    {
        LINK_NODE *node = MMILINK_CreateNode(data);
        MMILINK_AddNodeAfterBaseNode(s_bubble_link_head, node);
    }
}

/*****************************************************************************/
//  Description :  Delete a bubble from bubble link.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DelBubbleNode(LINK_NODE *node)
{
    if(node != PNULL)
    {
        if(node->data != PNULL)
        {
            //SCI_FREE(((void *)(node->data)));
            SCI_Free(((void *)(node->data)));
            node->data = PNULL;
        }
        MMILINK_DestroyNode(node, s_bubble_link_head, PNULL);
    }
}

/*****************************************************************************/
//  Description :  Destroy the bubble link.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DestroyBubble(void)
{
    LINK_NODE *node = PNULL;
    if(s_bubble_link_head != PNULL)
    {
        node = s_bubble_link_head->next_ptr;
        while(node != s_bubble_link_head)
        {
            if(node->data != PNULL)
            {
                //SCI_FREE(((void *)(node->data)));
                SCI_Free(((void *)(node->data)));
                node->data = PNULL;
            }    
            node = node->next_ptr;
        }
        MMILINK_DestroyLink(s_bubble_link_head, PNULL);
        s_bubble_link_head = PNULL;
    }
}

/*****************************************************************************/
//  Description :  Generate a new bubble.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void GenNewBubble(uint16 offset_x, uint16 offset_y, uint16 area_width, uint16 area_height)
{
    MMI_BUBBLE_T *bubble = PNULL;
    bubble = (MMI_BUBBLE_T *)SCI_ALLOCA(sizeof(MMI_BUBBLE_T));
    if(bubble != PNULL)
    {
        bubble->type = rand()%BUBBLE_TYPE_MAX;
        bubble->pos.x = offset_x + rand()%area_width;
        bubble->pos.y = offset_y;
        bubble->x_dec = BUBBLE_X_DEC_INIT_VALUE;
        bubble->x_offset = BUBBLE_X_OFFSET_INIT;
        AddBubble((uint32)bubble);
    }
}

/*****************************************************************************/
//  Description :  Rise all the bubbles in bubble link.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void RiseBubble(uint16 margin_l, uint16 margin_r, uint16 top)
{
    LINK_NODE *node = PNULL;
    MMI_BUBBLE_T *bubble = PNULL;
    int16 x = 0; //y = 0;
    
    if(s_bubble_link_head == PNULL)
    {
        return;
    }
    node = s_bubble_link_head->next_ptr;
    while(node != s_bubble_link_head)
    {
        bubble = (MMI_BUBBLE_T *)node->data;
        node = node->next_ptr;
        if(bubble == PNULL || bubble->pos.y < top)
        {
            DelBubbleNode(node->prev_ptr);
            continue;
        }
        if(0 == bubble->x_dec)
        {
            bubble->x_offset = BUBBLE_X_OFFSET_INIT;
            bubble->x_dec = BUBBLE_X_DEC_INIT_VALUE;
        } 
        else
        {
            bubble->x_dec--;
        }
        x = bubble->x_offset;
        if(bubble->pos.x + x > margin_r || bubble->pos.x + x < margin_l)
        {
            bubble->pos.x -= x;
            bubble->x_offset = -x;
        }
        else
        {
            bubble->pos.x += x;
        }
        bubble->pos.y -= (5 - bubble->type);
    }
    
}

/*****************************************************************************/
//  Description :  Display the bubble to the appointed layer.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayBubble(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *layer)
{
    LINK_NODE *node = PNULL;
    MMI_BUBBLE_T *bubble = PNULL;
    MMI_IMAGE_ID_T img_id = 0;
    if(s_bubble_link_head == PNULL)
    {
        return;
    }
    node = s_bubble_link_head->next_ptr;
    while(node != s_bubble_link_head)
    {
        bubble = (MMI_BUBBLE_T *)node->data;
        node = node->next_ptr;
        if(bubble == PNULL)
        {
            continue;
        }
        img_id = IMAGE_KL_EFFECT3_CHARGE_BUBBLE1 + bubble->type;
        GUIRES_DisplayImg(&bubble->pos, PNULL, PNULL, win_id, img_id, layer);
        
    }
}

/*****************************************************************************/
//  Description :  Make a judgement the the if the phone in charging state.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInCharging(void)
{
    return MMIAPIIDLE_IsChargeConnected();
}

#endif	//MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT

#endif




/*Edit by script, ignore 3 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
