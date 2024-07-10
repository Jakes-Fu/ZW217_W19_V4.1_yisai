/*****************************************************************************
** File Name:      mmikl_doublecirclelock.c                                  *
** Author:         michael.wang                                              *
** Date:           1/21/2012                                                 *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe move the little circle to   *
**                 unlock function                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/2012        michael.wang         Create
******************************************************************************/

#ifndef _MMIKL_DCLOCK_C_

#ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmikl_doublecirclelock.h"
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
#include "chg_drvapi.h"
#include "mmiidle_export.h"
#include "Guistatusbar.h"
#include "mmicc_export.h"
#include "mmidc_export.h"
#include "lcd_cfg.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define KEY_LOCK_DC_TP_MOVE_TIME   20
#define KEY_LOCK_DC_CIRCLE_MOVE_TIME   100
#define KEY_LOCK_DC_FLING_TIME   10
#define CHARGE_REMAIN_FULL 100
#define CHARGE_STANDARD_STEP    5
#define KEY_LOCK_PROG_MAX_DC 4
#define TOUCH_FLING_TIME_DC                ((float)0.05)   // fling time
#define MMIKL_CIRCLE_ANIM_COUNT_DC      40

#ifndef MAINLCD_SIZE_320X480
//the info about big and little circle, the same part of 240x400 and 240x320
#define DC_BIG_CIRCLE_D     148     //大圆直径
#define DC_BIG_CIRCLE_R     (DC_BIG_CIRCLE_D/2)
#define DC_LITTLE_CIRCLE_D      60  //小圆直径
#define DC_LITTLE_CIRCLE_R      (DC_LITTLE_CIRCLE_D/2)
#define DC_LCD_WINTH_V      240  //竖屏屏幕宽度
#define DC_CHARGE_STEP      3
#define DC_LITTLE_CIRCLE_LAYER_OFFSET 40
#else
//for 320x480
#define DC_BIG_CIRCLE_D     220     //大圆直径          //214
#define DC_BIG_CIRCLE_R     (DC_BIG_CIRCLE_D/2)
#define DC_LITTLE_CIRCLE_D      80  //小圆直径          //94
#define DC_LITTLE_CIRCLE_R      (DC_LITTLE_CIRCLE_D/2)
#define DC_LCD_WINTH_V      320  //竖屏屏幕宽度
#define DC_CHARGE_STEP1     4
#define DC_CHARGE_STEP2     5
#define DC_LITTLE_CIRCLE_LAYER_OFFSET 20
#endif

#ifdef MAINLCD_SIZE_240X400
//the info about the location of each image for vertical
#define DC_BIG_CIRCLE_TOP_V     159
#define DC_BIG_CIRCLE_BOTTOM_V      (DC_BIG_CIRCLE_TOP_V+DC_BIG_CIRCLE_D)
#define DC_BIG_CIRCLE_PRESSED_TOP_V     163
#define DC_BIG_CIRCLE_PRESSED_BOTTOM_V     (DC_BIG_CIRCLE_PRESSED_TOP_V+DC_BIG_CIRCLE_D)
#define DC_ANIM_CIRCLR_TOP_V    194
#define DC_ANIM_CIRCLE_BOTTOM_V 272
#define DC_TIME_TOP_V       46
#define DC_TIME_BOTTOM_V       (DC_TIME_TOP_V + 32)
#define DC_DATE_TOP_V       90
#define DC_DATE_FONT_HEIGHT 14
#define DC_CALL_RECT    {105,122,135,152}
#define DC_SMS_RECT     {9,218,39,248}
#define DC_MAINMENU_RECT    {200,218,230,248}
#define DC_CURRENT_RECT {105,314,135,344}
#define DC_CALL_VAILD_RECT    {105,122,135,199}
#define DC_SMS_VAILD_RECT    {9,218,85,248}
#define DC_MAINMENU_VAILD_RECT    {153,208,238,258}
#define DC_CURRENT_VAILD_RECT {105,267,135,344}

//the info about the location of each image for horizontal
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
#define DC_BIG_CIRCLE_TOP_H     86
#define DC_BIG_CIRCLE_PRESSED_TOP_H     90
#define DC_TIME_TOP_H       18
#define DC_TIME_BOTTOM_H       (DC_TIME_TOP_H + 32)
#define DC_DATE_TOP_H      65
#define DC_LCD_WINTH_H    400
#define DC_SMS_RECT_H    {90,144,120,174}
#define DC_CALL_RECT_H    {281,144,311,174}
#define DC_CALL_VAILD_RECT_H    {241,144,311,174}
#define DC_SMS_VAILD_RECT_H     {90,144,160,174}
#endif

#elif defined MAINLCD_SIZE_240X320
//the info about the location of each image for vertical
#define DC_BIG_CIRCLE_TOP_V     129
#define DC_BIG_CIRCLE_BOTTOM_V      (DC_BIG_CIRCLE_TOP_V+DC_BIG_CIRCLE_D)
#define DC_BIG_CIRCLE_PRESSED_TOP_V     134
#define DC_BIG_CIRCLE_PRESSED_BOTTOM_V     (DC_BIG_CIRCLE_PRESSED_TOP_V + DC_BIG_CIRCLE_D)
#define DC_ANIM_CIRCLR_TOP_V    164
#define DC_ANIM_CIRCLE_BOTTOM_V 242
#define DC_TIME_TOP_V       40
#define DC_TIME_BOTTOM_V       (DC_TIME_TOP_V + 32)
#define DC_DATE_TOP_V       84
#define DC_DATE_FONT_HEIGHT 14
#define DC_CALL_RECT    {105,92,135,122}
#define DC_SMS_RECT     {9,188,39,218}
#define DC_MAINMENU_RECT    {200,188,230,218}
#define DC_CURRENT_RECT {105,284,135,314}
#define DC_CALL_VAILD_RECT    {105,122,135,169}
#define DC_SMS_VAILD_RECT     {9,188,79,218}
#define DC_MAINMENU_VAILD_RECT     {160,188,230,218}
#define DC_CURRENT_VAILD_RECT {105,244,135,314}
//the info about the location of each image for horizontal
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
#define DC_BIG_CIRCLE_TOP_H     86
#define DC_BIG_CIRCLE_PRESSED_TOP_H     90
#define DC_TIME_TOP_H       20
#define DC_TIME_BOTTOM_H       (DC_TIME_TOP_H + 32)
#define DC_DATE_TOP_H      61
#define DC_LCD_WINTH_H    320
#define DC_SMS_RECT_H    {50,144,80,174}
#define DC_CALL_RECT_H    {241,144,271,174}
#define DC_CALL_VAILD_RECT_H    {201,144,271,174}
#define DC_SMS_VAILD_RECT_H     {50,144,120,174}
#endif

#else //for 320x480
//the info about the location of each image for vertical
#define DC_BIG_CIRCLE_TOP_V     161 //166
#define DC_BIG_CIRCLE_BOTTOM_V      (DC_BIG_CIRCLE_TOP_V+DC_BIG_CIRCLE_D)
#define DC_BIG_CIRCLE_PRESSED_TOP_V     143
#define DC_BIG_CIRCLE_PRESSED_BOTTOM_V     400
#define DC_ANIM_CIRCLR_TOP_V    (DC_BIG_CIRCLE_TOP_V + DC_BIG_CIRCLE_R -94/2)
#define DC_ANIM_CIRCLE_BOTTOM_V (DC_ANIM_CIRCLR_TOP_V + 94)
#define DC_TIME_TOP_V      60 
#define DC_TIME_BOTTOM_V       (DC_TIME_TOP_V + 36)
#define DC_DATE_TOP_V       109
#define DC_DATE_FONT_HEIGHT 14
#define DC_CALL_RECT    {140,123,180,163}
#define DC_SMS_RECT     {12,251,52,291}
#define DC_MAINMENU_RECT    {268,251,308,291}
#define DC_CURRENT_RECT {140,379,180,419}
#define DC_CALL_VAILD_RECT    {140,123,180,211}
#define DC_SMS_VAILD_RECT    {12,251,100,291}
#define DC_MAINMENU_VAILD_RECT    {220,251,308,291}
#define DC_CURRENT_VAILD_RECT {140,340,180,379}


//the info about the location of each image for horizontal
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
#define DC_BIG_CIRCLE_TOP_H     96
#define DC_BIG_CIRCLE_PRESSED_TOP_H    96
#define DC_TIME_TOP_H       21
#define DC_TIME_BOTTOM_H       (DC_TIME_TOP_H + 36)
#define DC_DATE_TOP_H      69
#define DC_LCD_WINTH_H    480
#define DC_SMS_RECT_H    {0}
#define DC_CALL_RECT_H    {0}
#define DC_CALL_VAILD_RECT_H {0}
#define DC_SMS_VAILD_RECT_H {0}
#endif

#endif

//创建静态layer,防止lose_focus时layer自动deactive
#define MMI_KL_DC_CREATE_LAYER(create_info, win_handle, layer_width, layer_height, layer, result) \
{   \
    create_info.lcd_id = GUI_MAIN_LCD_ID;   \
    create_info.owner_handle = win_handle;  \
    create_info.offset_x = 0;   \
    create_info.offset_y = 0;   \
    create_info.width = layer_width;  \
    create_info.height = layer_height;    \
    create_info.is_bg_layer = FALSE;    \
    create_info.is_static_layer = FALSE ;    \
    result = UILAYER_CreateLayer(&create_info, &layer);     \
}



/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    GUI_RECT_T rect;
    MMI_IMAGE_ID_T img_id;
}MMI_KL_DCELEMENT_IMG_T;

typedef enum
{
    UNLOCK_TYPE_NONE,
    UNLOCK_TYPE_RING,
    UNLOCK_TYPE_MAX
}UNLOCK_TYPE_E;

typedef enum
{
    MMIKL_DC_APP_NONE,
    MMIKL_DC_APP_CALL,
    MMIKL_DC_APP_SMS,    
    MMIKL_DC_APP_MAINMENU,
    MMIKL_DC_APP_CAM,
    MMIKL_DC_APP_CURRENT,
    MMIKL_DC_APP_MAX
} MMIKL_DC_APP_TYPE;

typedef struct
{
    GUI_LCD_DEV_INFO big_circle_layer;
    GUI_LCD_DEV_INFO little_circle_layer;
    GUI_LCD_DEV_INFO change_layer;
    uint8 tp_move_timer;
    uint8 circle_anim_timer;
    uint8 fling_timer;
}KEY_LOCK_DC_INFO_T;

typedef struct
{
    MMI_KL_DCELEMENT_IMG_T    big_circle;
    MMI_KL_DCELEMENT_IMG_T    little_circle;
    MMI_KL_DCELEMENT_IMG_T    big_circle_pressed;    
    MMI_KL_DCELEMENT_IMG_T    little_circle_pressed;
    MMI_KL_DCELEMENT_IMG_T    anim_circle;
    MMI_KL_DCELEMENT_IMG_T    charged;
    MMI_KL_DCELEMENT_IMG_T    time;
}KEY_LOCK_DC_CUSTOM_T;

typedef struct
{
    int16 first_x;                              //拖动效果用，记录up 时的layer x位置，相对于LCD的(0,0)
    int16 first_y;                              //拖动效果用，记录up 是的layer y位置，相对于LCD的(0,0)
    int16 little_circle_layer_x;                //记录移动小圆layer的坐标位置_x
    int16 little_circle_layer_y;                //记录移动小圆layer的坐标位置_y        
    float velocity_x;                           //拖动效果中x 方向的速度
    float velocity_y;                           //拖动效果中y  方向的速度
    GUI_POINT_T tp_down_point;
    GUI_POINT_T point_offset;                   //touch点击点减去layer的起始位置的偏移量，用于fling 特效stelayerposition的补数
}KEY_LOCK_DC_FLAG_T;


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint8 s_circle_anim_count = 0; //波纹扩散次数统计
LOCAL UNLOCK_TYPE_E s_unlock_type = UNLOCK_TYPE_NONE;    
LOCAL MMIKL_DC_APP_TYPE s_app_type = MMIKL_DC_APP_NONE;//app 标记 
LOCAL KEY_LOCK_DC_FLAG_T *s_kl_dc_flag =PNULL;
LOCAL KEY_LOCK_DC_CUSTOM_T *s_kl_dc_custom = PNULL;
LOCAL KEY_LOCK_DC_INFO_T s_kl_dc_info = {0};



/*****************************************************************************/
//  Description :获取可用layer
//                         当创建的layer处于激活状态时则用此layer
//                         否则，用main layer.防止lose focus时layer数据
//                         消失。
//  Global resource dependence :
//  Author:Michael wang
//  Date: 2012.3.2 for NEWMS00161908 (6530)
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetLayerDG(GUI_LCD_DEV_INFO *layer_info_ptr)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    
    if(layer_info_ptr->block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(layer_info_ptr))
        {
            lcd_dev_info = *layer_info_ptr;
        }
    }
    return lcd_dev_info;
}

/*****************************************************************************/
//  Description : clear the chaege layer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void ClearChargeLayerDC(void)
{
    UILAYER_Clear(&s_kl_dc_info.change_layer);
}

/*****************************************************************************/
//  Description : clear the big circle  layer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void ClearBigCircleLayerDC(void)
{
    UILAYER_Clear(&s_kl_dc_info.big_circle_layer);
}

/*****************************************************************************/
//  Description : clear the little circle  layer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void ClearLittleCircleLayerDC(void)
{
    UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
}

/*****************************************************************************/
//  Description : display the normal big circle image
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayBigCircleDC(void)
{
    GUI_LCD_DEV_INFO lcd_dev = {0};
    
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        UILAYER_Clear(&s_kl_dc_info.big_circle_layer);
    }
    /*
    lcd_dev = GetLayerDG(&(s_kl_dc_info.big_circle_layer));
    
    if(PNULL != s_kl_dc_custom)
    {
        GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->big_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, s_kl_dc_custom->big_circle.img_id, &lcd_dev);
    }
    */
    if(PNULL != s_kl_dc_custom)
    {
        GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->big_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, s_kl_dc_custom->big_circle.img_id, &s_kl_dc_info.big_circle_layer);
    }
}

/*****************************************************************************/
//  Description : display the big circle image when pressed event is valid
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayBigCirclePressDC(void)
{
    GUI_LCD_DEV_INFO lcd_dev = {0};

    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
       	UILAYER_Clear(&s_kl_dc_info.big_circle_layer);

        if(PNULL != s_kl_dc_custom)
        {       
            //有两个image需要画上去
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->big_circle_pressed.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, s_kl_dc_custom->big_circle_pressed.img_id, &(s_kl_dc_info.big_circle_layer));
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->big_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, s_kl_dc_custom->big_circle.img_id, &(s_kl_dc_info.big_circle_layer));
        }
    }
}

/*****************************************************************************/
//  Description : display the normal little circle image
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayLittleCircleDC(void)
{
    GUI_LCD_DEV_INFO lcd_dev = {0};
    
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.little_circle_layer)))
    {
        UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
        UILAYER_SetLayerPosition(&s_kl_dc_info.little_circle_layer, s_kl_dc_custom->anim_circle.rect.left, s_kl_dc_custom->anim_circle.rect.top);
    }
    /*
    lcd_dev = GetLayerDG(&(s_kl_dc_info.little_circle_layer));
    
    if(PNULL != s_kl_dc_custom)
    {
        if(!MMIAPIIDLE_IsChargeConnected())
        {
            //正常图标
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->little_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, s_kl_dc_custom->little_circle.img_id, &lcd_dev);
        }
        else
        {
            //充电图标
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->little_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_LITTLE_CIRCLE_CHARGE, &lcd_dev);
        }
    }  
    */

    if(PNULL != s_kl_dc_custom)
    {
        if(!MMIAPIIDLE_IsChargeConnected())
        {
            //正常图标
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->little_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, s_kl_dc_custom->little_circle.img_id, &(s_kl_dc_info.little_circle_layer));
        }
        else
        {
            //充电图标
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->little_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_LITTLE_CIRCLE_CHARGE, &(s_kl_dc_info.little_circle_layer));
        }
    }  
    
}

/*****************************************************************************/
//  Description : display the little circle image when pressed event is valid
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 这个image需要随Layer 移动而动，所以画的时候
//             不要UILAYER_SetLayerPosition() 来规定其初始位置
/*****************************************************************************/
LOCAL void DisplayLittleCirclePressDC(void)
{
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.little_circle_layer)))
    {
        UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
        
        if(PNULL != s_kl_dc_custom)
        {
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->little_circle_pressed.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, s_kl_dc_custom->little_circle_pressed.img_id, &(s_kl_dc_info.little_circle_layer));           
        }        
    }
}

/*****************************************************************************/
//  Description : display app icon (sms)
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 此image 画在大圆图层上
/*****************************************************************************/
LOCAL void DisplaySMSIconDC(void)
{
    GUI_RECT_T rect = DC_SMS_RECT;
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE      
    GUI_RECT_T rect_h = DC_SMS_RECT_H;    
#endif

    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect);
            GUIRES_DisplayImg(PNULL, &rect, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_SMS, &(s_kl_dc_info.big_circle_layer));           
        }
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE          
        else
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect_h);
            GUIRES_DisplayImg(PNULL, &rect_h, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_SMS, &(s_kl_dc_info.big_circle_layer));           
        }
#endif        
    }
}

/*****************************************************************************/
//  Description : display app icon (sms pressed)
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 此image 画在大圆图层上
/*****************************************************************************/
LOCAL void DisplaySMSIconFocusedDC(void)
{
    GUI_RECT_T rect = DC_SMS_RECT;
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
    GUI_RECT_T rect_h = DC_SMS_RECT_H;    
#endif
    
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect);
            GUIRES_DisplayImg(PNULL, &rect, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_SMS_PRESSED, &(s_kl_dc_info.big_circle_layer));           
        }
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE          
        else
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect_h);
            GUIRES_DisplayImg(PNULL, &rect_h, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_SMS_PRESSED, &(s_kl_dc_info.big_circle_layer));           
        }
#endif
    }
}

/*****************************************************************************/
//  Description : display app icon (call)
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 此image 画在大圆图层上
/*****************************************************************************/
LOCAL void DisplayCallIconDC(void)
{
    GUI_RECT_T rect = DC_CALL_RECT;
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE      
    GUI_RECT_T rect_h = DC_CALL_RECT_H;    
#endif

    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect);
            GUIRES_DisplayImg(PNULL, &rect, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_CALL, &(s_kl_dc_info.big_circle_layer));           
        }
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE          
        else
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect_h);
            GUIRES_DisplayImg(PNULL, &rect_h, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_CALL, &(s_kl_dc_info.big_circle_layer));           
        }
#endif        
    }
}

/*****************************************************************************/
//  Description : display app icon (call pressed)
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 此image 画在大圆图层上
/*****************************************************************************/
LOCAL void DisplayCallIconFocusedDC(void)
{
    GUI_RECT_T rect = DC_CALL_RECT;
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE      
    GUI_RECT_T rect_h = DC_CALL_RECT_H;    
#endif
    
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect);
            GUIRES_DisplayImg(PNULL, &rect, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_CALL_PRESSED, &(s_kl_dc_info.big_circle_layer));           
        }
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE          
        else
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect_h);
            GUIRES_DisplayImg(PNULL, &rect_h, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_CALL_PRESSED, &(s_kl_dc_info.big_circle_layer));           
        }
#endif        
    }
}

/*****************************************************************************/
//  Description : display app icon (unlock)
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 此image 画在大圆图层上
/*****************************************************************************/
LOCAL void DisplayCurrentIconDC(void)
{
    GUI_RECT_T rect = DC_CURRENT_RECT;

    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect);
            GUIRES_DisplayImg(PNULL, &rect, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_CURRENT, &(s_kl_dc_info.big_circle_layer));           
        }
    }
}

/*****************************************************************************/
//  Description : display app icon (unlock pressed)
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 此image 画在大圆图层上
/*****************************************************************************/
LOCAL void DisplayCurrentIconFocusedDC(void)
{
 
    GUI_RECT_T rect = DC_CURRENT_RECT;

    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect);
            GUIRES_DisplayImg(PNULL, &rect, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_CURRENT_PRESSED, &(s_kl_dc_info.big_circle_layer));           
        }
    }
}

/*****************************************************************************/
//  Description : display app icon (main menu)
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 此image 画在大圆图层上
/*****************************************************************************/
LOCAL void DisplayMainMenuIconDC(void)
{
    GUI_RECT_T rect = DC_MAINMENU_RECT;

    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect);
            GUIRES_DisplayImg(PNULL, &rect, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_MAINMENU, &(s_kl_dc_info.big_circle_layer));           
        }
    }
}

/*****************************************************************************/
//  Description : display app icon (main menu pressed)
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 此image 画在大圆图层上
/*****************************************************************************/
LOCAL void DisplayMainMenuIconFocusedDC(void)
{
    GUI_RECT_T rect = DC_MAINMENU_RECT;

    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
            UILAYER_ClearRect(&s_kl_dc_info.big_circle_layer,rect);
            GUIRES_DisplayImg(PNULL, &rect, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_MAINMENU_PRESSED, &(s_kl_dc_info.big_circle_layer));           
        }
    }
}

/*****************************************************************************/
//  Description : display the charge image when chargeing(controled by timer)
//  Global resource dependence : 
//  Author: Michael.wang
//  Note:  chage full when offeset equal "0"
/*****************************************************************************/
LOCAL void DisplayChargeDC(int16 offeset)
{
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.change_layer)))
    {
        UILAYER_Clear(&s_kl_dc_info.change_layer);
        UILAYER_SetLayerPosition(&s_kl_dc_info.change_layer, s_kl_dc_custom->charged.rect.left, s_kl_dc_custom->charged.rect.top+offeset);
        
        if(PNULL != s_kl_dc_custom)
        {
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->charged.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, s_kl_dc_custom->charged.img_id, &(s_kl_dc_info.change_layer));
        }        
    }
}

/*****************************************************************************/
//  Description : display the charge anim image
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayChargeAnimDC(void)
{           
    int16 charge_layer_offset = 0;
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    
    if(PNULL == p_chgmng_info)
    {
        return;
    }    
    
    if(MMIAPIIDLE_IsChargeConnected())
    {                
        if(CHARGE_REMAIN_FULL == p_chgmng_info->bat_remain_cap)
        {
            DisplayChargeDC(0);
        }
        else
        {
            //利用layer 的位置偏移实现电池电量变化的需求
#ifndef MAINLCD_SIZE_320X480     
            charge_layer_offset = DC_LITTLE_CIRCLE_D - DC_CHARGE_STEP * (p_chgmng_info->bat_remain_cap /CHARGE_STANDARD_STEP +1);
#else            
            charge_layer_offset = DC_LITTLE_CIRCLE_D - DC_CHARGE_STEP1 * (p_chgmng_info->bat_remain_cap /CHARGE_STANDARD_STEP +1);
            //320_480情况下分两阶段显示充电图标，两阶段的step 不同
            //if(p_chgmng_info->bat_remain_cap <= 50)
            //{
            //    charge_layer_offset = DC_LITTLE_CIRCLE_D - DC_CHARGE_STEP1 * (p_chgmng_info->bat_remain_cap /CHARGE_STANDARD_STEP +1);
            //}
            //else
            //{
            //    charge_layer_offset = DC_LITTLE_CIRCLE_D - DC_CHARGE_STEP2 * (p_chgmng_info->bat_remain_cap /CHARGE_STANDARD_STEP +1);
            //}
#endif
            //设置charge layer top 位置的偏移量
            DisplayChargeDC(charge_layer_offset);
        }                        
    }
    else
    {
        ClearChargeLayerDC();
    }
}

/*****************************************************************************/
//  Description : display time
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDigitalClockDC(void)
{
    GUI_LCD_DEV_INFO lcd_dev = {0};
    MMIKL_DisplayDigitalClock(&(lcd_dev), s_kl_dc_custom->time.rect, IMAGE_KL_DC_DIGIT0);
}

/*****************************************************************************/
//  Description : display date
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDateDC(void)
{
    SCI_DATE_T date = {0};
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T m_string = {0};
    MMI_STRING_T final_string = {0};
    wchar m_buf[5] ={0}; 
    wchar datewstr[30] = {0};
    wchar wstr_buf[16] = {0};
    uint8 datestr[16] = {0};
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO lcd_dev = {0};
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE    
    BOOLEAN is_landscape = FALSE;    
#endif
    
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
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    is_landscape = MMITHEME_IsMainScreenLandscape();
#endif    
    m_string.wstr_ptr = m_buf;
    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &m_string);
    
    MMIAPISET_GetCurrentDateStr(datestr,'-',sizeof(datestr));
    
    strncat(datestr," ",1);/*lint !e64*/
    MMIAPICOM_StrToWstr(datestr, wstr_buf);
    
    final_string.wstr_ptr = datewstr;
    MMIAPICOM_Wstrncpy(final_string.wstr_ptr,wstr_buf,MMIAPICOM_Wstrlen(wstr_buf));
    MMIAPICOM_Wstrncat(final_string.wstr_ptr, m_string.wstr_ptr,m_string.wstr_len);
    
    
    final_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(final_string.wstr_ptr);
    
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = MMI_DEFAULT_SMALL_FONT;
    text_style.align = ALIGN_HMIDDLE;
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    if(is_landscape)
    {
        rect.left = 0;
        rect.right = DC_LCD_WINTH_H;
        rect.top = DC_DATE_TOP_H;
        rect.bottom = DC_DATE_TOP_H + DC_DATE_FONT_HEIGHT + 1;
    }
    else
#endif  
    {      
        rect.left = 0;
        rect.right = DC_LCD_WINTH_V;
        rect.top = DC_DATE_TOP_V;
        rect.bottom = DC_DATE_TOP_V + DC_DATE_FONT_HEIGHT + 1;                 
    }    
    
    GUISTR_DrawTextToLCDInRect( 
                                (const GUI_LCD_DEV_INFO *)(&lcd_dev),
                                (const GUI_RECT_T *)&rect,
                                (const GUI_RECT_T *)&rect,
                                (const MMI_STRING_T *)&final_string,
                                &text_style,
                                GUISTR_STATE_ALIGN,
                                GUISTR_TEXT_DIR_AUTO
                                );
}


/*****************************************************************************/
//  Description : display missed event
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL  void DisplayMissedEvent(MMI_WIN_ID_T win_id, GUI_RECT_T *rect, const GUI_LCD_DEV_INFO * dev_info_ptr)
{
    uint8   str_len = 0;
    uint32  missed_call = 0;
    uint32  missed_sms =0;   
    uint16 img_width = 0, img_height = 0;  
    wchar    missed_call_wstr[KEYLOCK_COUNT_STRING_SIZE*8]  = {0};
    wchar    missed_sms_wstr[KEYLOCK_COUNT_STRING_SIZE*8]  = {0};
    char     count_str[KEYLOCK_COUNT_STRING_SIZE*8] = {0};
    MMI_STRING_T        missed_call_text = {0};
    MMI_STRING_T        missed_sms_text = {0};
    
    GUISTR_STYLE_T event_text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;
    GUI_RECT_T disp_rect = *rect;
  
    missed_call = MMIAPICC_GetMissedCallNumber();
    missed_sms = MMIAPISMS_GetAllUnreadMsgCount()
    + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX)
    ;

    if(0 == missed_call && 0 == missed_sms)
    {
        return;
    }
    
    event_text_style.effect = FONT_EFFECT_REVERSE_EDGE; 
    event_text_style.angle = ANGLE_0;
    event_text_style.align = ALIGN_VMIDDLE;
    event_text_style.font = MMI_DEFAULT_SMALL_FONT;
    event_text_style.font_color = MMI_WHITE_COLOR;

#ifdef MAINLCD_SIZE_320X480    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_IDLE_KEYLOCK_CALL_EFFECT2, win_id);
#else
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_DC_MISS_CALL, win_id);
#endif    
    if(missed_call > 0)
    {
#ifdef MAINLCD_SIZE_320X480
        MMI_GetLabelTextByLang(TXT_KL_MISS_CALLS, &missed_call_text);
#endif
        sprintf(count_str,"%d ",missed_call);
        str_len = strlen(count_str);
        MMI_STRNTOWSTR(missed_call_wstr, KEYLOCK_COUNT_STRING_SIZE*8, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE*8, str_len);
        if (KEYLOCK_COUNT_STRING_SIZE*8 > str_len + missed_call_text.wstr_len + 1)
        {
            MMIAPICOM_Wstrncpy(missed_call_wstr + str_len, missed_call_text.wstr_ptr , missed_call_text.wstr_len);
        }
        missed_call_text.wstr_ptr = missed_call_wstr;
        missed_call_text.wstr_len = str_len + missed_call_text.wstr_len;
#ifdef MAINLCD_SIZE_320X480    
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_IDLE_KEYLOCK_CALL_EFFECT2, dev_info_ptr, FALSE);
#else
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_DC_MISS_CALL, dev_info_ptr, FALSE);
#endif    

        disp_rect.left += img_width + 2;
        disp_rect.bottom = disp_rect.top + img_height;
        GUISTR_DrawTextToLCDInRect(
            dev_info_ptr,
            &disp_rect,
            &disp_rect,
            (const MMI_STRING_T *)&missed_call_text,
            &event_text_style,
            text_state,
            0
            );
    }

    if(missed_sms > 0)
    {
        SCI_MEMSET(count_str, 0, KEYLOCK_COUNT_STRING_SIZE*8);
#ifdef MAINLCD_SIZE_320X480        
        /* Modify by michael on 3/8/2012 for NEWMS00177382 : Add new string accorrding the num of unread msg*/	
        if(1 == missed_sms )
        {
            MMI_GetLabelTextByLang(TXT_KL_UNREAD_MESSAGES, &missed_sms_text);
        }
        else if(missed_sms > 1)
        {
            MMI_GetLabelTextByLang(TXT_KL_UNREAD_MESSAGES2, &missed_sms_text);
        }
        /************************Modify over*************************/
#endif        
        sprintf(count_str,"%d ",missed_sms);
        str_len = strlen(count_str);
        
        MMI_STRNTOWSTR(missed_sms_wstr, KEYLOCK_COUNT_STRING_SIZE*8, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE*8,str_len);
        
        if (KEYLOCK_COUNT_STRING_SIZE*8 > str_len + missed_sms_text.wstr_len +1)
        {
            MMIAPICOM_Wstrncpy(missed_sms_wstr + str_len, missed_sms_text.wstr_ptr , missed_sms_text.wstr_len);
        }
        missed_sms_text.wstr_ptr = missed_sms_wstr;
        missed_sms_text.wstr_len = str_len + missed_sms_text.wstr_len;

        disp_rect = *rect;
        disp_rect.top += img_height + 2;
#ifdef  MAINLCD_SIZE_320X480 
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_IDLE_KEYLOCK_MESSAGE_EFFECT2, dev_info_ptr, FALSE);

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_IDLE_KEYLOCK_MESSAGE_EFFECT2, win_id);
#else
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_DC_SMS_UNREAD, dev_info_ptr, FALSE);

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_DC_SMS_UNREAD, win_id);
#endif
        disp_rect.left += img_width + 2;
        disp_rect.bottom = disp_rect.top + img_height;        
        GUISTR_DrawTextToLCDInRect(
            dev_info_ptr,
            &disp_rect,
            &disp_rect,
            (const MMI_STRING_T *)&missed_sms_text,
            &event_text_style,
            text_state,
            0
            );
    }
}


/*****************************************************************************/
//  Description : display missed event
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayMissedEventDC(void)
{
    GUI_RECT_T rect = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    GUISTR_STYLE_T style= {0};
    MMI_STRING_T string = {0};
    GUISTR_INFO_T info = {0};    
    GUI_LCD_DEV_INFO lcd_dev = {0};
    uint16 rect_height = 0;
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    BOOLEAN is_landscape = FALSE;    
    
    is_landscape = MMITHEME_IsMainScreenLandscape();
#endif     
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    style.font = MMI_DEFAULT_NORMAL_FONT;
    
    MMI_GetLabelTextByLang(TXT_KL_MISS_CALLS, &string);
    
    GUISTR_GetStringInfo(&style, &string, GUISTR_STATE_NONE, &info);
    
    rect_height = info.height * 2 + 8;    
    rect.right = lcd_width - 1;     

    if(PNULL != s_kl_dc_custom)
    {
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
        if(is_landscape)
        {
            rect.top = s_kl_dc_custom->big_circle.rect.bottom - (DC_BIG_CIRCLE_R -DC_LITTLE_CIRCLE_R);
            rect.bottom = rect.top + rect_height;
        }
        else
#endif 
        {
        #ifdef MAINLCD_SIZE_240X320//240X320 only digital,no character
            rect.top = s_kl_dc_custom->big_circle.rect.bottom - rect_height/2;
            rect.bottom = rect.top + rect_height; 
        #else
            rect.top = s_kl_dc_custom->big_circle.rect.bottom + (lcd_height - s_kl_dc_custom->big_circle.rect.bottom)/2 - rect_height/2;
            rect.bottom = rect.top + rect_height;  
        #endif    
        } 
        
        DisplayMissedEvent(MMIKL_CLOCK_DISP_WIN_ID, &rect, &lcd_dev);
    }
}

/*****************************************************************************/
//  Description : do 50% alpha blending for bmp in LCD buffer and a color
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 给背景蒙上一层透明色，防止白色背景影响显示效果
/*****************************************************************************/
LOCAL void DisplayShadowBGDC(void)
{        
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_COLOR_T main_color = MMI_BLACK_COLOR;
    GUI_LCD_DEV_INFO main_lcd_info = {0};
    GUI_RECT_T main_rect = {0};        
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    main_rect.left = 0;
    main_rect.top = 0;
    main_rect.right = lcd_width -1;
    main_rect.bottom = lcd_height -1;
    
    LCD_MistBmp(&main_lcd_info,main_rect,main_color);
}

/*****************************************************************************/
//  Description : display the anim of little circle 
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 小圆波纹扩散效果，扩散5圈结束
/*****************************************************************************/
LOCAL void DisplayAnimCircleDC(uint8 count)
{
    uint8 count_temp = count%8;
    
    ClearLittleCircleLayerDC();

    if(PNULL != s_kl_dc_custom)
    {
         if(!MMIAPIIDLE_IsChargeConnected())
        {
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->little_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, s_kl_dc_custom->little_circle.img_id, &(s_kl_dc_info.little_circle_layer));
        }
        else
        {        //为了节省全局变量，测出image直接引用
            GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->little_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_DC_LITTLE_CIRCLE_CHARGE, &(s_kl_dc_info.little_circle_layer));
        }
        
        GUIRES_DisplayImg(PNULL, &(s_kl_dc_custom->anim_circle.rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, (s_kl_dc_custom->anim_circle.img_id + count_temp), &(s_kl_dc_info.little_circle_layer));
       
    }    
}

//#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
/*****************************************************************************/
//  Description : release all layers of DC
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: when rotate the screen,release all layers first.
/*****************************************************************************/
LOCAL void ReleaseAllLayersDC(void)
{
    //to reslove the pc-lint error
    GUI_LCD_DEV_INFO *little_circle_layer = &( s_kl_dc_info.little_circle_layer);
    GUI_LCD_DEV_INFO *change_layer = &(s_kl_dc_info.change_layer);
    GUI_LCD_DEV_INFO *big_circle_layer = &(s_kl_dc_info.big_circle_layer);
    UILAYER_Clear(& s_kl_dc_info.little_circle_layer);
    UILAYER_Clear(&s_kl_dc_info.change_layer);
    UILAYER_Clear(&s_kl_dc_info.big_circle_layer);
    UILAYER_RELEASELAYER(little_circle_layer);
    UILAYER_RELEASELAYER(change_layer);
    UILAYER_RELEASELAYER(big_circle_layer);
}
//#endif

/*****************************************************************************/
//  Description : full paint 显示函数
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayFullPaintDC(void)
{        
    DisplayShadowBGDC();
    DisplayDigitalClockDC();
    DisplayDateDC();
    DisplayMissedEventDC();
    
    if(UNLOCK_TYPE_NONE == s_unlock_type)
    {
        DisplayBigCircleDC();
        DisplayLittleCircleDC();
        DisplayChargeAnimDC();                
    }
}

/*****************************************************************************/
//  Description : Display app icon 
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayAppIconDC(void)
{        
    DisplayCallIconDC();
    DisplayCurrentIconDC();
    DisplayMainMenuIconDC();
    DisplaySMSIconDC();
}

/*****************************************************************************/
//  Description : full paint 显示函数
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayVaildTpDownDC(void)
{        
    DisplayBigCirclePressDC();
    DisplayLittleCirclePressDC(); 
    DisplayAppIconDC();
}


/*****************************************************************************/
//  Description : Display one focused app icon 
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayOneFocusedAppIconDC(void)
{        
    switch(s_app_type)
    {
    case MMIKL_DC_APP_CALL:
        DisplayCallIconFocusedDC();
        break;

    case MMIKL_DC_APP_CURRENT:
        DisplayCurrentIconFocusedDC();
        break;

    case MMIKL_DC_APP_CAM:  //由于资源和历史原因，导致此处CAM和MAINMENU 一致，后续可以分开处理
    case MMIKL_DC_APP_MAINMENU:
        DisplayMainMenuIconFocusedDC();
        break;

    case MMIKL_DC_APP_SMS:
        DisplaySMSIconFocusedDC();
        break;

    default:
        break;
    }
    
}

/*****************************************************************************/
//  Description : call a app routine when unlock screen
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void CallAppRoutineDC( MMI_WIN_ID_T    win_id)
{ 
    if (!MMIKL_IsPreviewState())  
    {
        switch(s_app_type)
        {
        case MMIKL_DC_APP_CALL:
            MMIAPIIDLE_EnterDialWin();
            break;
            
        case MMIKL_DC_APP_SMS:
            MMIAPISMS_OpenMainMenuWin();
            break;
            
        case MMIKL_DC_APP_MAINMENU:
            {    
#if defined MMI_ISTYLE_SUPPORT 				
                if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
                {
                    MMK_ReturnIdleWin();
                }
                else
#endif					
                {
                    MMK_ReturnIdleWin();
                    MMIAPIMENU_CreatMainMenu();
                }
            }                    
            break;
            
        case MMIKL_DC_APP_CAM:
                {
                    MMK_ReturnIdleWin();
                    MMIAPIDC_OpenPhotoWin(); 
                }
        break;
            
        case MMIKL_DC_APP_CURRENT:     
            break;
            
        default:
            break;
        }
    }
    else
    {
        MMK_CloseWin(win_id);  
    }
}

/*****************************************************************************/
//  Description : start tp move timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StartTpMoveTimerDC(void)
{
    START_TIMER(s_kl_dc_info.tp_move_timer, KEY_LOCK_DC_TP_MOVE_TIME);
}


/*****************************************************************************/
//  Description : stop tp move timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StopTpMoveTimerDC(void)
{
    STOP_TIMER(s_kl_dc_info.tp_move_timer);
}

/*****************************************************************************/
//  Description : 波纹扩散效果动画timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StartCircleAnimTimerDC(void)
{
    START_TIMER(s_kl_dc_info.circle_anim_timer, KEY_LOCK_DC_CIRCLE_MOVE_TIME);
}

/*****************************************************************************/
//  Description : stop anim timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StopCircleAnimTimerDC(void)
{
    STOP_TIMER(s_kl_dc_info.circle_anim_timer);
}

/*****************************************************************************/
//  Description : 拖动惯性滑行效果timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StartFlingTimerDC(void)
{
    START_TIMER(s_kl_dc_info.fling_timer, KEY_LOCK_DC_FLING_TIME);
}

/*****************************************************************************/
//  Description : stop fling timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StopFlingTimerDC(void)
{
    STOP_TIMER(s_kl_dc_info.fling_timer);
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

/*****************************************************************************/
//  Description : judge condition of unlock screen
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 2012.3.17 根据需求更改实现方式，超过大圆半径即解锁
//             之前是超出大圆和小圆的半径和 解锁
/*****************************************************************************/
LOCAL BOOLEAN IsBeyondExtent(int32 x,int32 y)
{
    int32 temp = DC_BIG_CIRCLE_R ;//有效范围内最长的直角三角形斜边
    //直角三角形，两直角边平方等于斜边平方
    return ((x * x) +(y * y))>(temp * temp) ? TRUE:FALSE;
}

/*****************************************************************************/
//  Description : judge little and big circle is crossing
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 判断两个圆是否相交
/*****************************************************************************/
LOCAL BOOLEAN IsTwoCircleCrossing(int32 x,int32 y)
{
    int32 temp = DC_BIG_CIRCLE_R - DC_LITTLE_CIRCLE_R ; //有效范围内最长的直角三角形斜边
    //直角三角形，两直角边平方等于斜边平方
    return ((x * x) +(y * y))>(temp * temp) ? TRUE:FALSE;
}

/*****************************************************************************/
//  Description : judge little and big circle is crossing
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 判断两个圆是否相交
/*****************************************************************************/
LOCAL BOOLEAN IsLittleCircleMoveEnalble(int32 x,int32 y)
{
    int32 temp = 5 ; //有效范围内最长的直角三角形斜边
    //直角三角形，两直角边平方等于斜边平方
    return ((x * x) +(y * y))>(temp * temp) ? TRUE:FALSE;
}

/*****************************************************************************/
//  Description : judge the point is in app icon rect
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 判断此时touch point 是否在app icon的有效区域内
/*****************************************************************************/
LOCAL BOOLEAN IsTPVaildInAppRect(GUI_POINT_T point)
{
    BOOLEAN ret = FALSE;
    GUI_RECT_T rect_call = DC_CALL_VAILD_RECT;       
    GUI_RECT_T rect_sms = DC_SMS_VAILD_RECT;
    GUI_RECT_T rect_mainmenu = DC_MAINMENU_VAILD_RECT;
    GUI_RECT_T rect_current = DC_CURRENT_VAILD_RECT;
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
    GUI_RECT_T rect_call_h = DC_CALL_VAILD_RECT_H;
    GUI_RECT_T rect_sms_h = DC_SMS_VAILD_RECT_H;
#endif


    if(!MMITHEME_IsMainScreenLandscape())
    {
            if(GUI_PointIsInRect(point,rect_call))
            {
                s_app_type = MMIKL_DC_APP_CALL;
                ret = TRUE;
            }
            else if(GUI_PointIsInRect(point,rect_sms))
            {
                s_app_type = MMIKL_DC_APP_SMS;
                ret = TRUE;
            }
            else if(GUI_PointIsInRect(point,rect_mainmenu))
            {
                //s_app_type = MMIKL_DC_APP_MAINMENU;
                s_app_type = MMIKL_DC_APP_CAM;
                ret = TRUE;
            }
            else if(GUI_PointIsInRect(point,rect_current))
            {
                s_app_type = MMIKL_DC_APP_CURRENT;
                ret = TRUE;
            }
    }
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE      
    else
    {
         if(GUI_PointIsInRect(point,rect_call_h))
        {
            s_app_type = MMIKL_DC_APP_CALL;
            ret = TRUE;
        }
        else if(GUI_PointIsInRect(point,rect_sms_h))
        {
            s_app_type = MMIKL_DC_APP_SMS;
            ret = TRUE;
        }
    }
#endif    
    
    return ret;    
}

/******************************************************************************/
//  Description : Create layer.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateLayerDC(MMI_WIN_ID_T win_id, GUI_RECT_T rect)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 width = 0, height = 0;
    
    if(rect.top < rect.bottom && rect.left < rect.right)
    {
        width = rect.right - rect.left + 1;
        height = rect.bottom - rect.top + 1;
        
        //2012.2.1 ---create static layer in case the layer will not deactived when win_id lose focus. create_info."is_static_layer = TRUE" ; 
        //2012.2.13---Change to dynamic layer because there is a bug when come a call when preview the lock screen "is_static_layer = FALSE"
        //the little circle should  display when lose focus.
        MMI_KL_DC_CREATE_LAYER(create_info, win_id, width, height, layer, result);
        
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
//  Description : rest key lock param state of DC.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  ResetkeylockStateParamDC(void)
{
    s_unlock_type = UNLOCK_TYPE_NONE;  
    s_app_type = MMIKL_DC_APP_NONE;   
    s_circle_anim_count = 0;

    StopTpMoveTimerDC();
    StopCircleAnimTimerDC();
    StopFlingTimerDC();
}

/******************************************************************************/
//  Description : set the param of dynamic layer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInitParamOfLayerDC()
{        
    s_kl_dc_info.big_circle_layer.block_id = UILAYER_NULL_HANDLE;
    s_kl_dc_info.little_circle_layer.block_id = UILAYER_NULL_HANDLE;
    s_kl_dc_info.change_layer.block_id = UILAYER_NULL_HANDLE;
}
//  Description : Init custom parameter.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  InitCustomParamDC(void)
{
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 img_width = 0, img_height = 0;
    GUI_RECT_T rect = {0};
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    BOOLEAN is_landscape = FALSE;
    
    is_landscape = MMITHEME_IsMainScreenLandscape();
#endif    
    
    if(PNULL == s_kl_dc_custom)
    {
        s_kl_dc_custom = SCI_ALLOCA(sizeof(KEY_LOCK_DC_CUSTOM_T));
        
        if( PNULL == s_kl_dc_custom)
        {
            return FALSE;
        }
    }  

    if(PNULL == s_kl_dc_flag)
    {
        s_kl_dc_flag = SCI_ALLOCA(sizeof(KEY_LOCK_DC_FLAG_T));
        
        if( PNULL == s_kl_dc_flag)
        {
            return FALSE;
        }

    }
    
    SCI_MEMSET(s_kl_dc_custom, 0, sizeof(KEY_LOCK_DC_CUSTOM_T));
    SCI_MEMSET(s_kl_dc_flag, 0, sizeof(KEY_LOCK_DC_FLAG_T));
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    //load img    
    s_kl_dc_custom->big_circle.img_id = IMAGE_KL_DC_BIG_CIRCLE;
    s_kl_dc_custom->big_circle_pressed.img_id= IMAGE_KL_DC_BIG_CIRCLE_PRESS;
    s_kl_dc_custom->little_circle.img_id= IMAGE_KL_DC_LITTLE_CIRCLE;
    s_kl_dc_custom->little_circle_pressed.img_id=IMAGE_KL_DC_LITTLE_CIRCLE_PRESS;
    s_kl_dc_custom->anim_circle.img_id = IMAGE_DC_ANIM1;
    s_kl_dc_custom->charged.img_id= IMAGE_KL_DC_CHARGE;
    s_kl_dc_custom->time.img_id = IMAGE_KL_DC_DIGIT0;
    
    //caulcate the rect of each img
    //big circle
    GUIRES_GetImgWidthHeight(&img_width,&img_height, s_kl_dc_custom->big_circle.img_id, MMIKL_CLOCK_DISP_WIN_ID);
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    if(is_landscape)
    {
        rect.left = (lcd_width - img_width) / 2;
        rect.right = rect.left + img_width;
        rect.top = DC_BIG_CIRCLE_TOP_H;
        rect.bottom =  DC_BIG_CIRCLE_TOP_H + DC_BIG_CIRCLE_D;	 
        s_kl_dc_custom->big_circle.rect = rect;
    }
    else
#endif          
    {      
        rect.left = (lcd_width - img_width) / 2;
        rect.right = rect.left + img_width;
        rect.top = DC_BIG_CIRCLE_TOP_V;
        rect.bottom =  DC_BIG_CIRCLE_BOTTOM_V;	 
        s_kl_dc_custom->big_circle.rect = rect;      
    }    

    //big circle pressed
    GUIRES_GetImgWidthHeight(&img_width,&img_height, s_kl_dc_custom->big_circle_pressed.img_id, MMIKL_CLOCK_DISP_WIN_ID);
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    if(is_landscape)
    {
        rect.left = (lcd_width - img_width) / 2;
        rect.right = rect.left + img_width;
        rect.top = DC_BIG_CIRCLE_PRESSED_TOP_H;
        rect.bottom =  DC_BIG_CIRCLE_PRESSED_TOP_H + DC_BIG_CIRCLE_D;	 
        s_kl_dc_custom->big_circle_pressed.rect = rect;
    }
    else
#endif          
    {      
        rect.left = (lcd_width - img_width) / 2;
        rect.right = rect.left + img_width;
        rect.top = DC_BIG_CIRCLE_PRESSED_TOP_V;
        rect.bottom =  DC_BIG_CIRCLE_PRESSED_TOP_V + img_height;	 
        s_kl_dc_custom->big_circle_pressed.rect = rect;         
    }    

    
    //little circle
    GUIRES_GetImgWidthHeight(&img_width,&img_height, s_kl_dc_custom->little_circle.img_id, MMIKL_CLOCK_DISP_WIN_ID);
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE 
    if(is_landscape)
    {
        rect.left = (lcd_width - img_width) / 2;
        rect.right = rect.left + img_width;
        rect.top = DC_BIG_CIRCLE_TOP_H +   DC_BIG_CIRCLE_R - DC_LITTLE_CIRCLE_R;
        rect.bottom =   DC_BIG_CIRCLE_TOP_H + DC_BIG_CIRCLE_D  - (DC_BIG_CIRCLE_R -DC_LITTLE_CIRCLE_R  ); 
        s_kl_dc_custom->little_circle.rect = rect;
        s_kl_dc_custom->little_circle_pressed.rect = rect;
    }
    else
#endif
    {        
        rect.left = (lcd_width - img_width) / 2;
        rect.right = rect.left + img_width;
        rect.top = DC_BIG_CIRCLE_TOP_V +   DC_BIG_CIRCLE_R - DC_LITTLE_CIRCLE_R;
        rect.bottom =   DC_BIG_CIRCLE_BOTTOM_V - (DC_BIG_CIRCLE_R -DC_LITTLE_CIRCLE_R  ); 
        s_kl_dc_custom->little_circle.rect = rect;
        s_kl_dc_custom->little_circle_pressed.rect = rect;           
    }    

    //change
#ifndef MAINLCD_SIZE_320X480  
    rect.left =  rect.left -3;
    rect.right  =  rect.right  -3;
    rect.top = rect.top +2;    //由于图片原因，对充电图标的位置进行 "+4" 微调
    rect.bottom = rect.bottom +2;       //由于图片原因，对充电图标的位置进行 "+4" 微调
    s_kl_dc_custom->charged.rect = rect;
#else
    rect.left = rect.left ;   //微调
    rect.right = rect.right  ;
    rect.top = rect.top;   
    rect.bottom = rect.bottom+10;  //10---微调值       
    s_kl_dc_custom->charged.rect = rect;
#endif

    //anim circle，扩散动画的rect
    GUIRES_GetImgWidthHeight(&img_width,&img_height, s_kl_dc_custom->anim_circle.img_id, MMIKL_CLOCK_DISP_WIN_ID);
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    if(is_landscape)
    {
        rect.left = (lcd_width - img_width) / 2;
        rect.right = rect.left + img_width;
        rect.top = DC_BIG_CIRCLE_TOP_H + DC_BIG_CIRCLE_R - img_width/2;
        rect.bottom=  rect.top + img_height;
        s_kl_dc_custom->anim_circle.rect = rect;
    }
    else
#endif 
    {       
        rect.left = (lcd_width - img_width) / 2;
        rect.right = rect.left + img_width;
        rect.top = DC_ANIM_CIRCLR_TOP_V;
        rect.bottom= DC_ANIM_CIRCLE_BOTTOM_V;
        s_kl_dc_custom->anim_circle.rect = rect;           
    }

    //time
    GUIRES_GetImgWidthHeight(&img_width,&img_height, s_kl_dc_custom->time.img_id, MMIKL_CLOCK_DISP_WIN_ID);
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    if(is_landscape)
    {
        rect.left = (lcd_width - img_width*6) / 2;
        rect.right = rect.left + img_width*6;
        rect.top = DC_TIME_TOP_H;
        rect.bottom= DC_TIME_TOP_H + img_height;
        s_kl_dc_custom->time.rect = rect;
    }
    else
#endif 
    {       
        rect.left = (lcd_width - img_width*6) / 2;
        rect.right = rect.left + img_width*6;
        rect.top = DC_TIME_TOP_V;
        rect.bottom= DC_TIME_BOTTOM_V;
        s_kl_dc_custom->time.rect = rect;           
    }
    
    return TRUE;
}

/******************************************************************************/
//  Description : Init the all param.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitParamDC(void)
{
    GUI_RECT_T rect = {0}; 
    uint16 lcd_width = 0, lcd_height = 0;
    MMI_WIN_ID_T win_id = MMIKL_CLOCK_DISP_WIN_ID;
    
    if(InitCustomParamDC())
    {
        
        SCI_MEMSET(&s_kl_dc_info, 0, sizeof(s_kl_dc_info));
        SetInitParamOfLayerDC();
        
        //大圆的图层以外围的大圈为为准，所以图层向四周扩散 offset 值
        rect.left = s_kl_dc_custom->big_circle_pressed.rect.left - DC_LITTLE_CIRCLE_LAYER_OFFSET;
        rect.top = s_kl_dc_custom->big_circle_pressed.rect.top - DC_LITTLE_CIRCLE_LAYER_OFFSET;
        rect.right =s_kl_dc_custom->big_circle_pressed.rect.right + DC_LITTLE_CIRCLE_LAYER_OFFSET;
        rect.bottom = s_kl_dc_custom->big_circle_pressed.rect.bottom + DC_LITTLE_CIRCLE_LAYER_OFFSET;
        
        //create layer
        //需要按顺序创建layer,因为创建和blt layer现在封装在一起
        s_kl_dc_info.big_circle_layer = CreateLayerDC(win_id,rect);
        s_kl_dc_info.change_layer = CreateLayerDC(win_id,s_kl_dc_custom->charged.rect);
        //小圆的图层以波纹扩散效果的最大光圈为准
        s_kl_dc_info.little_circle_layer = CreateLayerDC(win_id, s_kl_dc_custom->anim_circle.rect);	        
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
//  Description : handle kl display window msg.
//  Global resource dependence : none
//  Author: michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleDoubleCircleWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              uint16          msg_id, 
                                              DPARAM          param
                                              )
{   
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};   

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
//320x240分辨率下不支持横竖平切换           
#ifndef MAINLCD_DEV_SIZE_320X240            
            MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
            MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE); 
#else
            LCD_ANGLE_E angle_ptr = LCD_ANGLE_0;
            MMK_GetWinAngle(win_id,&angle_ptr);
            
            MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_CUR);
            MMK_SetWinAngle(win_id, angle_ptr, FALSE); 
#endif 
            if(!InitParamDC())
            {
                MMK_CloseWin(win_id);
                break;
            }            
        }        
        break;  
        
    case MSG_FULL_PAINT:  
        {
            DisplayFullPaintDC();
        }          
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
           s_kl_dc_flag->tp_down_point.x = MMK_GET_TP_X(param);
           s_kl_dc_flag->tp_down_point.y = MMK_GET_TP_Y(param);
            
            if(GUI_PointIsInRect(s_kl_dc_flag->tp_down_point,s_kl_dc_custom->little_circle.rect))
            {
                s_circle_anim_count = 0;
                s_unlock_type = UNLOCK_TYPE_RING;

                StopCircleAnimTimerDC(); 
                DisplayVaildTpDownDC();                           
                
                if(MMIAPIIDLE_IsChargeConnected())
                {
                    ClearChargeLayerDC();
                }
                
                StartTpMoveTimerDC();                
            }
        }
        break;

    case MSG_TP_PRESS_UP:
        {         
            GUI_POINT_T tp_up_point = {0};    
            
            if(UNLOCK_TYPE_RING == s_unlock_type )
            {
                float max_velocity = (float)FLING_MAX_VELOCITY;
                s_app_type = MMIKL_DC_APP_NONE;
                s_unlock_type = UNLOCK_TYPE_NONE;
                StopTpMoveTimerDC();
                
                //增加速度点
                MMK_AddVelocityItem(tp_up_point.x,tp_up_point.y);
                //计算当前滑动速度
                MMK_ComputeCurrentVelocity( &s_kl_dc_flag->velocity_x, &s_kl_dc_flag->velocity_y, max_velocity, max_velocity );
                
                if ((FLING_MIN_VELOCITY <= abs((int32)s_kl_dc_flag->velocity_x)) ||
                    (FLING_MIN_VELOCITY <= abs((int32)s_kl_dc_flag->velocity_y)))  // 达到一定速度，需要滑动
                {
                    // 进入滑动状态，起动滑动定时器  
                    tp_up_point.x = MMK_GET_TP_X(param);
                    tp_up_point.y = MMK_GET_TP_Y(param);                                                                         
                    
                    UILAYER_GetLayerPosition(&s_kl_dc_info.little_circle_layer,&s_kl_dc_flag->first_x,&s_kl_dc_flag->first_y);
                    
                    s_kl_dc_flag->point_offset.x = tp_up_point.x - s_kl_dc_flag->first_x;    //up point 减去此时layer的位置(相对于LCD (0,0))，得出
                    s_kl_dc_flag->point_offset.y = tp_up_point.y - s_kl_dc_flag->first_y;    //此时ponit点与layer的offset值，用于在timer中通过layer得出实际的touch 点
                    
                    StartFlingTimerDC();
                }
                else
                {
                    StartCircleAnimTimerDC();
                    
                    if (MMK_IsFocusWin(win_id))
                    {
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                } 
            }     
        }	   
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_TIMER:	
        {
            if(s_kl_dc_info.tp_move_timer == *(uint8*)param)    //tp timer
            {
                MMI_TP_STATUS_E tp_status = 0;
                int16 offset_x = 0,  offset_y = 0;

                StopTpMoveTimerDC();
                MMK_GetLogicTPMsg(&tp_status, &point);
                
                offset_x = point.x -s_kl_dc_flag->tp_down_point.x;
                offset_y = point.y -s_kl_dc_flag->tp_down_point.y;           
                                
                if(IsLittleCircleMoveEnalble(offset_x,offset_y))
                {                                      
                    if(IsBeyondExtent((int32)offset_x,(int32)offset_y))   
                    {
                        CallAppRoutineDC(win_id);
                        
                        if (MMIKL_IsPreviewState())
                        {
                            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_DOUBLE_CIRCLE);
                        }
                        
                        MMK_CloseWin(win_id);                    
                    }
                    else
                    {
                        DisplayAppIconDC();
                        
                        if(IsTwoCircleCrossing((int32)offset_x,(int32)offset_y)&&
                            IsTPVaildInAppRect(point))
                        {
                            DisplayOneFocusedAppIconDC();
                        }
                        
                        if(UNLOCK_TYPE_RING == s_unlock_type )
                        {
                            s_kl_dc_flag->little_circle_layer_x = s_kl_dc_custom->anim_circle.rect.left + offset_x;
                            s_kl_dc_flag->little_circle_layer_y = s_kl_dc_custom->anim_circle.rect.top + offset_y;
                            
                            UILAYER_SetLayerPosition(&s_kl_dc_info.little_circle_layer, s_kl_dc_flag->little_circle_layer_x, s_kl_dc_flag->little_circle_layer_y);
                            
                            StartTpMoveTimerDC();
                        }   
                    }        
                }
                else
                {
                    StartTpMoveTimerDC();
                }
            }        
            else if(s_kl_dc_info.fling_timer== *(uint8*)param) //fling timer
            {
                int32 old_x = 0;
                int32 old_y = 0;
                int32 cur_x = 0;
                int32 cur_y =0;     
                int32 offset_x = 0;
                int32 offset_y = 0;
                
                old_x = s_kl_dc_flag->first_x;
                old_y = s_kl_dc_flag->first_y;
                
                StopFlingTimerDC();
                
                offset_x = s_kl_dc_flag->first_x + s_kl_dc_flag->point_offset.x - s_kl_dc_flag->tp_down_point.x;
                offset_y = s_kl_dc_flag->first_y + s_kl_dc_flag->point_offset.y - s_kl_dc_flag->tp_down_point.y;
                
                point.x = s_kl_dc_flag->first_x + s_kl_dc_flag->point_offset.x;
                point.y = s_kl_dc_flag->first_y +s_kl_dc_flag->point_offset.y;
                
                if(IsLittleCircleMoveEnalble(offset_x,offset_y))
                {
                    if(IsTwoCircleCrossing((int32)offset_x,(int32)offset_y)&&
                        IsTPVaildInAppRect(point))
                    {
                        DisplayOneFocusedAppIconDC();
                    }
                    
                    if(IsBeyondExtent((int32)offset_x,(int32)offset_y))   
                    {
                        CallAppRoutineDC(win_id);
                        
                        if (MMIKL_IsPreviewState())
                        {
                            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_DOUBLE_CIRCLE);
                        }
                        
                        MMK_CloseWin(win_id);                    
                    }
                    else
                    {
                        // 重算加速度
                        cur_x = MMK_GetFlingOffset(
                            old_x, s_kl_dc_flag->velocity_x,
                            TOUCH_FLING_TIME_DC, &s_kl_dc_flag->velocity_x);
                        
                        cur_y = MMK_GetFlingOffset(
                            old_y, s_kl_dc_flag->velocity_y,
                            TOUCH_FLING_TIME_DC, &s_kl_dc_flag->velocity_y);
                        
                        if (FLING_RUN_MIN_VELOCITY <= abs((int32)s_kl_dc_flag->velocity_x)||
                            FLING_RUN_MIN_VELOCITY <= abs((int32)s_kl_dc_flag->velocity_y) ) // 还可以加速运动
                        {
                            UILAYER_SetLayerPosition(&s_kl_dc_info.little_circle_layer,( cur_x ),( cur_y ));
                            s_kl_dc_flag->first_x = cur_x;
                            s_kl_dc_flag->first_y = cur_y;
                            StartFlingTimerDC();
                        }
                        else
                        {
                            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                        }
                    }
                }    

            }
            else if(s_kl_dc_info.circle_anim_timer== *(uint8*)param) //波纹timer
            {
                
                StopCircleAnimTimerDC();
                
                if( MMIKL_CIRCLE_ANIM_COUNT_DC == s_circle_anim_count )
                {
                    ClearLittleCircleLayerDC();
                    DisplayLittleCircleDC();
                    s_circle_anim_count = 0;
                    break;
                }
                
                DisplayAnimCircleDC(s_circle_anim_count);
                s_circle_anim_count++;
                StartCircleAnimTimerDC();
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        break;

    case MSG_GET_FOCUS:
        {
            ReleaseAllLayersDC();
            InitParamDC();
            StartCircleAnimTimerDC();
        }
        break;
        
    case MSG_LOSE_FOCUS:
        {            
            ResetkeylockStateParamDC();
            
            if(KL_UNLOCKED == MMIKL_GetWinStatus())
            {
                MMK_CloseWin(win_id);                
            }
        }        
        break;
        
    case MSG_BACKLIGHT_TURN_ON:
        {
            StartCircleAnimTimerDC();
        }
        break;
        
    case MSG_BACKLIGHT_TURN_OFF:
        {
            ResetkeylockStateParamDC();
        }
        break;

        
    case MSG_APP_OK:
  //  case MSG_APP_MENU:
        {
            if (MMIKL_IsPreviewState())
            {
                MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_DOUBLE_CIRCLE);
                MMK_CloseWin(win_id);
            }
            else if ((MMIDEFAULT_IsBacklightOn()) && 
            (MMIKL_GetWinStatus() == KL_LOCKED))
            {
                GUISTR_STYLE_T style_infor = {0};
                MMI_STRING_T str_t = {0};
                GUISTR_INFO_T str_infor = {0};
                GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
                GUI_LCD_DEV_INFO    layer = {0};
                GUI_RECT_T time_rect = {0};
                MMIKL_SetWinStatus(KL_UNLOCKED_ONCE);
            //s_is_press_down = FALSE;
            //start key lock timer
            MMIKL_StartUnlockConfirmTimer(win_id);
                style_infor.effect = FONT_EFFECT_NONE; 
        style_infor.angle = ANGLE_0;
        style_infor.align = ALIGN_LVMIDDLE;
        style_infor.font = SONG_FONT_24;
        style_infor.font_color = MMI_GRAY_WHITE_COLOR;
        MMI_GetLabelTextByLang(TXT_COMMON_OK,&str_t);
                GUISTR_GetStringInfo(&style_infor,&str_t, text_state, &str_infor);

            time_rect.bottom = 2*str_infor.width;
            time_rect.left = 0;
            time_rect.right = LCD_WIDTH-1;
            time_rect.top = time_rect.bottom-2*str_infor.height;
            
               GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&layer,
            &time_rect,
            &time_rect,
            (const MMI_STRING_T     *)&str_t,
            &style_infor,
            text_state,
            0
            );
               
                MMIDEFAULT_TurnOnBackLight();
            }       
            else
            {
                recode = MMI_RESULT_FALSE;
            }
        }        
        break;
        
    case MSG_APP_CANCEL:
        {
            if (MMIKL_IsPreviewState())
            {
                MMK_CloseWin(win_id);
            }
            else if ((MMIDEFAULT_IsBacklightOn()) && 
            (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus()))
            {
            MMIKL_SetWinStatus(KL_UNLOCKED);
            //检查s_st_mmi_kl_event，判断是否有未读SMS、MMS、MISSED CALL发生
            //如果有，需要调用这3个模块的函数，来弹出各自的提示窗口。
            MMK_CloseWin(win_id);   
    #ifdef MMIWIDGET_WEATHER_ANIM
            MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_WEATHER_NEED_ANIM, PNULL, 0);
    #endif            
                MMIKL_CheckPromptStatus();
            }
            else
            {
                recode = MMI_RESULT_FALSE;   //popup the prompt info
            }
        }        
        break;
        /*
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
    case MSG_LCD_SWITCH:        
        {
            ReleaseAllLayersDC();            
            InitParamDC();
            StartCircleAnimTimerDC();
        }
        break;
#endif     
    */
    case MSG_CLOSE_WINDOW:
        {
            ResetkeylockStateParamDC();
            ReleaseAllLayersDC();
            if(s_kl_dc_custom != PNULL)
            {
                SCI_FREE(s_kl_dc_custom);
                s_kl_dc_custom = PNULL;
            }

            if(s_kl_dc_flag != PNULL)
            {
                SCI_FREE(s_kl_dc_flag);
                s_kl_dc_flag = PNULL;
            }           
        }             
        break;        
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    } 
    
    return recode;
}
#endif	//MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT

#endif




/*Edit by script, ignore 2 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
