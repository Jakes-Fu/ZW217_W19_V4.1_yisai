/*****************************************************************************
** File Name:      mmikl_fruitsplit.c                                        *
** Author:         jian.ma                                                   *
** Date:           20/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe keylock fruit split function*
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create
******************************************************************************/

#ifndef _MMIKL_FRUITSPLIT_C_

#ifdef MMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmikl_fruitsplit.h"
#include "ui_layer.h"
#include "mmi_image.h"
#include "mmikl_effectmain.h"
#include "mmikl_internal.h"
#include "mmiset_export.h"
#include "guires.h"
#include "mmikl_export.h"
#include "mmk_tp.h"
#include "mmi_position.h"
#ifndef WIN32
#include "math.h"
#endif
#include "graphics_draw.h"
//#include "mmiset_lock_effect.h"
#include "mmiset_lock_effect_export.h"
#include "mmi_appmsg.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define FRUIT_RING_NUM 4
#define FRUIT_IDLE_ANIM_NUM 5
#define FRUIT_SPLIT_ANIM_NUM 6
#define FRUIT_RING_PIC_WIDTH 109
#define FRUIT_RING_PIC_HEIGHT 109
#define FRUIT_TOMATO_PIC_WIDTH 48
#define FRUIT_TOMATO_PIC_HEIGHT 48
#define FRUIT_DROP_INTERVAL 90

#define FRUIT_MISSEVENT_RECT_LEFT 130
#define FRUIT_MISSEVENT_RECT_WIDTH  200
#define FRUIT_MISSEVENT_RECT_HEIGHT  50

#define FRUIT_ANIM_TIME 80

typedef enum
{
    FRUIT_STATE_IDLE,
  //  FRUIT_STATE_TP_SLIDE, 
    FRUIT_STATE_SPLITED,    
    FRUIT_STATE_MAX    
}FRUIT_STATE_TYPE_E;

typedef enum
{
    FRUIT_HORIZONTAL_LEFT_RIGHT,
    FRUIT_HORIZONTAL_RIGHT_LEFT,
    FRUIT_VERTICAL_UP_DOWN,
    FRUIT_VERTICAL_DOWN_UP,
    FRUIT_ANGLE_PHASE_1,
    FRUIT_ANGLE_PHASE_2,
    FRUIT_ANGLE_PHASE_3,
    FRUIT_ANGLE_PHASE_4,
    FRUIT_ANGLE_PHASE_MAX
}FRUIT_ANGLE_PHASE_E;

LOCAL uint16 s_fruit_ring_width = 0;
LOCAL uint16 s_fruit_ring_height = 0;
LOCAL uint16 s_fruit_hand_width = 0;
LOCAL uint16 s_fruit_hand_height = 0;

LOCAL uint16 s_fruit_tomato_width = 0;
LOCAL uint16 s_fruit_tomato_height = 0;

LOCAL uint16 s_fruit_light_width = 0;
LOCAL uint16 s_fruit_light_height = 0;

LOCAL uint16 s_fruit_info_width = 0;
LOCAL uint16 s_fruit_info_height = 0;

LOCAL uint16 s_fruit_frame_index = 0;
LOCAL uint16 s_fruit_hand_interval = 0;
LOCAL int16 s_fruit_cut_angle = 0;

LOCAL uint8 s_fruit_anim_timer = 0;

#if defined MAINLCD_SIZE_240X320
LOCAL GUI_POINT_T s_fruit_ring_point[FRUIT_RING_NUM] = {{10,65},{121,60},{7,175},{113,170}}; 
#elif defined MAINLCD_SIZE_240X400
LOCAL GUI_POINT_T s_fruit_ring_point[FRUIT_RING_NUM] = {{10,86},{121,64},{7,247},{113,209}}; 
#else
LOCAL GUI_POINT_T s_fruit_ring_point[FRUIT_RING_NUM] = {{40,126},{181,94},{37,287},{173,249}};
#endif 
//此处比水果数目加了一个layer,用来显示一个水果切成2半时的另外一半
LOCAL GUI_LCD_DEV_INFO s_fruit_icon_layer[FRUIT_RING_NUM+1] = {0};
LOCAL GUI_LCD_DEV_INFO s_fruit_light_layer = {0};
LOCAL GUI_LCD_DEV_INFO s_fruit_light_temp_layer = {0};
LOCAL GUI_LCD_DEV_INFO s_fruit_hand_layer = {0};
LOCAL GUI_LCD_DEV_INFO s_fruit_main_layer = {0};

LOCAL FRUIT_ANGLE_PHASE_E s_fruit_cut_phase = FRUIT_ANGLE_PHASE_MAX;

LOCAL const float _sin_0_90_table[91] = 
{
    0,
    0.017, 0.034, 0.052, 0.069, 0.087, 0.104, 0.121, 0.139, 0.156, 0.173,
    0.190, 0.207, 0.224, 0.241, 0.258, 0.275, 0.292, 0.309, 0.325, 0.342,
    0.358, 0.374, 0.390, 0.406, 0.422, 0.438, 0.453, 0.469, 0.484, 0.499,
    0.515, 0.529, 0.544, 0.559, 0.573, 0.587, 0.601, 0.615, 0.629, 0.642,
    0.656, 0.669, 0.681, 0.694, 0.707, 0.719, 0.731, 0.743, 0.754, 0.766,
    0.777, 0.788, 0.798, 0.809, 0.819, 0.829, 0.838, 0.848, 0.857, 0.866,
    0.874, 0.882, 0.891, 0.898, 0.906, 0.913, 0.920, 0.927, 0.933, 0.939,
    0.945, 0.951, 0.956, 0.961, 0.965, 0.970, 0.974, 0.978, 0.981, 0.984,
    0.987, 0.990, 0.992, 0.994, 0.996, 0.997, 0.998, 0.999, 0.999, 1.000,
};



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
//  Description :app fruit split layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void AppendLayerOfFruitSplit(GUI_LCD_DEV_INFO* lcd_dev_ptr);
/******************************************************************************/
//  Description :create fruit split layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void CreateIconLayerOfFruitSplit(MMI_WIN_ID_T win_id);
/******************************************************************************/
//  Description :create fruit split handle layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void CreateHandLayerOfFruitSplit(MMI_WIN_ID_T win_id);

/******************************************************************************/
//  Description :create fruit split light layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void CreateLightLayerOfFruitSplit(MMI_WIN_ID_T win_id);

/******************************************************************************/
//  Description :create fruit split temp layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void CreateTempLayerOfFruitSplit(MMI_WIN_ID_T win_id);

/******************************************************************************/
//  Description :Calculate the sin value of the angle
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL int16 CalcTpSlideSinValueOfFruit(GUI_POINT_T old_point,GUI_POINT_T new_point);

/******************************************************************************/
//  Description : check slide ring rect
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JudgePointInRingRect(GUI_POINT_T tp_point,int16* index);

/******************************************************************************/
//  Description :Calculate the angle of the Slide
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL int16 CalcTpSlideAngleOfFruit(GUI_POINT_T old_point,GUI_POINT_T new_point);
/******************************************************************************/
//  Description : display the idle image of fruit style
//  Global resource dependence : none
//  Author: 
//  Note: Icon moving unlock style
/*****************************************************************************/
LOCAL void DisplayIdleImageOfFruitSplit(MMI_WIN_ID_T win_id);

/******************************************************************************/
//  Description : display the split fruit anim when fruit splited apart
//  Global resource dependence : none
//  Author: 
//  Note: Icon moving unlock style
/*****************************************************************************/
LOCAL void PlayFruitSplitAinm(
                              MMI_WIN_ID_T win_id,
                              int16 rotate_angle,
                              uint16 index,
                              GUI_POINT_T old_point,
                              GUI_POINT_T new_point
                              );
/******************************************************************************/
//  Description : display the split light image when fruit splited apart
//  Global resource dependence : none
//  Author: 
//  Note: Icon moving unlock style
/*****************************************************************************/
LOCAL void DisplaySplitLightOfFruit(
                                    MMI_WIN_ID_T win_id,
                                    uint16 icon_index,
                                    int16 rotate_angle,
                                    GUI_POINT_T old_point,
                                    GUI_POINT_T new_point
                                    );

/******************************************************************************/
//  Description :app fruit split layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void AppendLayerOfFruitSplit(GUI_LCD_DEV_INFO* lcd_dev_ptr)
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    append_layer.lcd_dev_info = *lcd_dev_ptr;
    if(lcd_dev_ptr->block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}

/******************************************************************************/
//  Description :create fruit split layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void CreateIconLayerOfFruitSplit(MMI_WIN_ID_T win_id)
{
    uint16 index = 0;
    
    UILAYER_RESULT_E result  = 0;
    UILAYER_CREATE_T    create_info = {0};

    for(index = 0;index < FRUIT_RING_NUM+1;index++)
    {
        create_info.lcd_id = GUI_MAIN_LCD_ID; 
        create_info.owner_handle = win_id;
        create_info.offset_x = 0; 
        create_info.offset_y = 0; 
        create_info.width = s_fruit_ring_width;
        create_info.height = s_fruit_ring_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;

        result = UILAYER_CreateLayer(&create_info, &s_fruit_icon_layer[index]);  
        if(UILAYER_RESULT_SUCCESS ==result)
        {
            AppendLayerOfFruitSplit(&s_fruit_icon_layer[index]);
            if(index == FRUIT_RING_NUM)
            {
                UILAYER_SetLayerPosition(&s_fruit_icon_layer[index],0,0);
            }
            else
            {
                UILAYER_SetLayerPosition(&s_fruit_icon_layer[index],s_fruit_ring_point[index].x,s_fruit_ring_point[index].y);
            }
            UILAYER_Clear(&s_fruit_icon_layer[index]);
        }
    }
}
/******************************************************************************/
//  Description :create fruit split handle layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void CreateHandLayerOfFruitSplit(MMI_WIN_ID_T win_id)
{
    UILAYER_RESULT_E result  = 0;
    UILAYER_CREATE_T    create_info = {0};

    create_info.lcd_id = GUI_MAIN_LCD_ID; 
        create_info.owner_handle = win_id;
        create_info.offset_x = 0; 
        create_info.offset_y = 0; 
        create_info.width = s_fruit_hand_width;
        create_info.height = s_fruit_hand_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;

        result = UILAYER_CreateLayer(&create_info, &s_fruit_hand_layer);  
        if(UILAYER_RESULT_SUCCESS ==result)
        {
            AppendLayerOfFruitSplit(&s_fruit_hand_layer);
            UILAYER_Clear(&s_fruit_hand_layer);
        }
}
/******************************************************************************/
//  Description :create fruit split light layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void CreateLightLayerOfFruitSplit(MMI_WIN_ID_T win_id)
{
    UILAYER_RESULT_E result  = 0;
    UILAYER_CREATE_T    create_info = {0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    create_info.lcd_id = GUI_MAIN_LCD_ID; 
    create_info.owner_handle = win_id;
        create_info.offset_x = 0; 
        create_info.offset_y = 0; 
        create_info.width = lcd_width;
        create_info.height = lcd_height;//s_fruit_light_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;

        result = UILAYER_CreateLayer(&create_info, &s_fruit_light_layer);  
        if(UILAYER_RESULT_SUCCESS ==result)
        {
            AppendLayerOfFruitSplit(&s_fruit_light_layer);
            UILAYER_Clear(&s_fruit_light_layer);
        }
}

/******************************************************************************/
//  Description :create fruit split temp layer 
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void CreateTempLayerOfFruitSplit(MMI_WIN_ID_T win_id)
{
    UILAYER_RESULT_E result  = 0;
    UILAYER_CREATE_T    create_info = {0};

    create_info.lcd_id = GUI_MAIN_LCD_ID; 
        create_info.owner_handle = win_id;
        create_info.offset_x = 0; 
        create_info.offset_y = 0; 
        create_info.width = s_fruit_light_width;
        create_info.height = s_fruit_light_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;

        result = UILAYER_CreateLayer(&create_info, &s_fruit_light_temp_layer);  
        if(UILAYER_RESULT_SUCCESS ==result)
        {
            UILAYER_Clear(&s_fruit_light_temp_layer);
        }
}

/******************************************************************************/
//  Description : display the idle image of fruit style
//  Global resource dependence : none
//  Author: 
//  Note: Icon moving unlock style
/*****************************************************************************/
LOCAL void DisplayIdleImageOfFruitSplit(MMI_WIN_ID_T win_id)
{
    uint16 i=0;

    uint16 offset_x = 0;
    uint16 offset_y = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_SIMP_CHINESE;
    GUI_POINT_T dis_point = {0};

    offset_x = (s_fruit_ring_width - s_fruit_tomato_width)/2;
    offset_y = (s_fruit_ring_height- s_fruit_tomato_height)/2;

    IMG_EnableTransparentColor(TRUE);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    dis_point.x = 0;
    dis_point.y = MMI_STATUSBAR_HEIGHT;
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_FRUIT_BG , &s_fruit_main_layer);
   
    MMIAPISET_GetLanguageType(&language_type);
    dis_point.x = 15;
    dis_point.y = lcd_height - s_fruit_info_height-15;
    if(MMISET_LANGUAGE_SIMP_CHINESE == language_type )
    {
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, IMAGE_KL_FRUIT_INFO,
            &s_fruit_main_layer);
    }
    else
    {
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, IMAGE_KL_FRUIT_INFO_ENG, 
            &s_fruit_main_layer);
    }
    for(i=0;i<FRUIT_RING_NUM;i++)
    {
        dis_point = s_fruit_ring_point[i];
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, (IMAGE_KL_RING_BLUE+i), &s_fruit_icon_layer[i]);
       // if(i>0)
        {
            if(MMISET_LANGUAGE_SIMP_CHINESE == language_type )
            {
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, (IMAGE_KL_FRUIT_UNLOCK+i), &s_fruit_icon_layer[i]);
            }
            else
            {
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, (IMAGE_KL_FRUIT_UNLOCK_ENG+i), &s_fruit_icon_layer[i]);
            }
        }
        dis_point.x+=offset_x;
        dis_point.y +=offset_y;
        switch(i)
        {  
            case 0:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, IMAGE_KL_FRUIT_TOMATO1, &s_fruit_icon_layer[i]);
                break;
            case 1:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, IMAGE_KL_APPLE1, &s_fruit_icon_layer[i]);
            break;
            case 2:
                 GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, IMAGE_KL_ORANGE1, &s_fruit_icon_layer[i]);
                break;
            case 3:
                  GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, IMAGE_KL_PEAR1, &s_fruit_icon_layer[i]);
                 break;
            default:
                break;
        }
    }
    IMG_EnableTransparentColor(FALSE);
    
}

/******************************************************************************/
//  Description : check slide ring rect
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JudgePointInRingRect(GUI_POINT_T tp_point,int16* index)
{
    BOOLEAN result = FALSE;
    uint16 i=0;
    uint16 offset_x = (s_fruit_ring_width - s_fruit_tomato_width)/2;
    uint16 offset_y = (s_fruit_ring_height- s_fruit_tomato_height)/2;
    
    for(i = 0;i<FRUIT_RING_NUM;i++)
    {
        if(tp_point.x> (s_fruit_ring_point[i].x+offset_x)&&(tp_point.x <(s_fruit_ring_point[i].x +offset_x+ s_fruit_tomato_width))&&
            tp_point.y> (s_fruit_ring_point[i].y+offset_y)&&(tp_point.y <(s_fruit_ring_point[i].y +offset_y+ s_fruit_tomato_height)))
            {
                result = TRUE;
                *index = i;
                break;
            }
    }
    return result;
}
/******************************************************************************/
//  Description : check slide ring rect
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JudgeSlideRingRect(GUI_POINT_T tp_down_point,GUI_POINT_T tp_up_point,int16* index)
{
    BOOLEAN result = FALSE;
    uint16 i=0;
    uint16 offset_x = (s_fruit_ring_width - s_fruit_tomato_width)/2;
    uint16 offset_y = (s_fruit_ring_height- s_fruit_tomato_height)/2;
    
    for(i = 0;i<FRUIT_RING_NUM;i++)
    {
        if(tp_down_point.x< (s_fruit_ring_point[i].x+offset_x)&&tp_down_point.y< (s_fruit_ring_point[i].y+offset_y)&&
            (tp_up_point.x >(s_fruit_ring_point[i].x +offset_x+ s_fruit_tomato_width))&&
            (tp_up_point.y >(s_fruit_ring_point[i].y +offset_y+ s_fruit_tomato_height)))
            {
                result = TRUE;
                *index = i;
                break;
            }
    }
    return result;
}

LOCAL void StartFruitTimer(MMI_HANDLE_T win_id)
{
    if(0 == s_fruit_anim_timer)
    {
        s_fruit_anim_timer = MMK_CreateWinTimer(win_id,FRUIT_ANIM_TIME,FALSE);
    }
}

LOCAL void StopFruitTimer(void)
{
    if(s_fruit_anim_timer>0)
    {
        MMK_StopTimer(s_fruit_anim_timer);
        s_fruit_anim_timer = 0;
    }
}

LOCAL void PlayIdleHandAnimOfFruit(MMI_WIN_ID_T win_id,uint16 anim_index,
    GUI_POINT_T point)
{
    GUI_POINT_T dis_point = {0};
    uint16 layer_x = 0;
    uint16 layer_y = 0;
    
        
    layer_x = point.x + anim_index*(s_fruit_ring_width/FRUIT_IDLE_ANIM_NUM);
    layer_y = point.y + anim_index*( s_fruit_ring_height/FRUIT_IDLE_ANIM_NUM);

    UILAYER_SetLayerPosition(&s_fruit_hand_layer,layer_x, layer_y);

    dis_point.x = layer_x+(s_fruit_ring_width-s_fruit_hand_width)/2;
    dis_point.y = layer_y;
        
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, IMAGE_KL_FRUIT_HAND,
        &s_fruit_hand_layer);
}

LOCAL void PlayIdleFruitAnim(MMI_WIN_ID_T win_id)
{
    uint16 i=0;

    uint16 offset_x = 0;
    uint16 offset_y = 0;
    GUI_POINT_T dis_point = {0};
    //GUI_RECT_T lcd_rect = MMITHEME_GetFullScreenRect();
    MMISET_LANGUAGE_TYPE_E  language_type = MMISET_LANGUAGE_SIMP_CHINESE;
        
    IMG_EnableTransparentColor(TRUE);

    UILAYER_Clear(&s_fruit_light_layer);
    
    MMIAPISET_GetLanguageType(&language_type);
    
     if(s_fruit_frame_index<FRUIT_IDLE_ANIM_NUM)
    {
        offset_x = (s_fruit_ring_width - s_fruit_tomato_width)/2;
        offset_y = (s_fruit_ring_height- s_fruit_tomato_height)/2;

        for(i=0;i<FRUIT_RING_NUM;i++)
        {
            dis_point = s_fruit_ring_point[i];
            
            UILAYER_Clear(&s_fruit_icon_layer[i]);

            GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, (IMAGE_KL_RING_BLUE+i), &s_fruit_icon_layer[i]);
            //if(i>0)
            {
                if(MMISET_LANGUAGE_SIMP_CHINESE == language_type )
                {
                    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, (IMAGE_KL_FRUIT_UNLOCK+i), &s_fruit_icon_layer[i]);
                }
                else
                {
                    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, (IMAGE_KL_FRUIT_UNLOCK_ENG+i), &s_fruit_icon_layer[i]);
                }
            }
            dis_point.x+=offset_x;
            dis_point.y +=offset_y;
            switch(i)
            {  
                case 0:
                    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, 
                        IMAGE_KL_FRUIT_TOMATO1+s_fruit_frame_index,&s_fruit_icon_layer[i]);
                break;
                case 1:
                    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, 
                        IMAGE_KL_APPLE1+s_fruit_frame_index, &s_fruit_icon_layer[i]);
                break;
                case 2:
                    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, 
                        IMAGE_KL_ORANGE1+s_fruit_frame_index, &s_fruit_icon_layer[i]);
                break;
                case 3:
                    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id, 
                        IMAGE_KL_PEAR1+s_fruit_frame_index, &s_fruit_icon_layer[i]);
                 break;
                default:
                break;
            }
        }
        if(FRUIT_IDLE_ANIM_NUM == s_fruit_hand_interval)
        {
            PlayIdleHandAnimOfFruit(win_id,s_fruit_frame_index, s_fruit_ring_point[0]);
        }
        else
        {
            UILAYER_Clear(&s_fruit_hand_layer);
        }
        s_fruit_frame_index++;        
    }

   
    
    if(s_fruit_frame_index>=FRUIT_IDLE_ANIM_NUM)
    {
        s_fruit_frame_index = 0;
        s_fruit_hand_interval ++;
        s_fruit_hand_interval = s_fruit_hand_interval%(FRUIT_IDLE_ANIM_NUM + 1);
    }
    IMG_EnableTransparentColor(FALSE);
}

/******************************************************************************/
//  Description :Calculate the sin value of the angle
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL int16 CalcTpSlideSinValueOfFruit(GUI_POINT_T old_point,GUI_POINT_T new_point)
{//取得sin值
    int32 sin_v = 0;
    int32 fenzi = 0;
    //int32 fenmu = 0;
    int32 temp1 = 0;
    int32 temp2 = 0;
    if(old_point.x == new_point.x && old_point.y == new_point.y)
    {
        return 0;
    }
    //防止小数出现乘以1000
    fenzi = ((new_point.y - old_point.y)*1000);
    temp1 = new_point.x - old_point.x;
    if(temp1<0)
    {
        temp1 = -temp1;
    }
    temp2 = new_point.y - old_point.y;
    if(temp2<0)
    {
        temp2 = -temp2;
    }
    sin_v = fenzi/sqrt(temp1*temp1+temp2*temp2); /*lint !e747 !e790 !e524*/
    return sin_v;
}
/**********************************************/
/***********************************************/
LOCAL void PlaySplitedFruitDropAnim(
                                    MMI_WIN_ID_T win_id,
                                    int16 angle_v,
                                    GUI_POINT_T centre_point,
                                    MMI_IMAGE_ID_T left_image,
                                    MMI_IMAGE_ID_T right_image,
                                    uint16 frame_index,
                                    GUI_LCD_DEV_INFO* left_lcd_dev_ptr,
                                    GUI_LCD_DEV_INFO* right_lcd_dev_ptr
                                    )
{
    uint16 P_LEFT = 0;//抛物线参数
    uint16 P_RIGHT = 0;
    uint16 OFFSET_LEFT = 0;
    uint16 OFFSET_RIGHT = 05;
    uint16 left_layer_x = 0;
    uint16 left_layer_y = 0;
    uint16 right_layer_x = 0;
    uint16 right_layer_y = 0;

    GUI_POINT_T left_dis = {0};
    GUI_POINT_T right_dis = {0};
                
    UILAYER_Clear(left_lcd_dev_ptr);
    UILAYER_Clear(right_lcd_dev_ptr);
    
    if(s_fruit_cut_angle>=0&&s_fruit_cut_angle<45)
    {
        P_LEFT = 8;
        P_RIGHT = 6;
        OFFSET_LEFT = 30;
        OFFSET_RIGHT = 25;
        left_layer_x = centre_point.x-20 - frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y - 20+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x- 10 - frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y+20 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);


        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }    
    else if(s_fruit_cut_angle>=45&&s_fruit_cut_angle<80)
    {
        P_LEFT = 6;
        P_RIGHT = 5;
        OFFSET_LEFT = 30;
        OFFSET_RIGHT = 25;
        left_layer_x = centre_point.x-20 - frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y - 20+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x-10 - frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y+20 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);

        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }  
    else if(s_fruit_cut_angle>=80&&s_fruit_cut_angle<100)
    {
        P_LEFT = 7;
        P_RIGHT = 7;
        OFFSET_LEFT = 25;
        OFFSET_RIGHT = 25;
        left_layer_x = centre_point.x-30 - frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y + 30+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x+30 + frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y+30 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);

        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }  
     else if(s_fruit_cut_angle>=100&&s_fruit_cut_angle<135)
    {
        P_LEFT = 5;
        P_RIGHT = 6;
        OFFSET_LEFT = 25;
        OFFSET_RIGHT = 30;
        left_layer_x = centre_point.x+20 + frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y - 20+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x+20 + frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y+20 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);

        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }    
    else  if(s_fruit_cut_angle>=135&&s_fruit_cut_angle<180)
    {
        P_LEFT = 6;
        P_RIGHT = 8;
        OFFSET_LEFT = 25;
        OFFSET_RIGHT = 30;
        left_layer_x = centre_point.x+20 + frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y - 20+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x+20 + frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y+20 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);

        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }       
    else if(s_fruit_cut_angle>=180&&s_fruit_cut_angle<225)
            //抛物线模型
    {
        P_LEFT = 2;
        P_RIGHT = 3;
        OFFSET_LEFT = 10;
        OFFSET_RIGHT = 15;
        left_layer_x = centre_point.x+20 + frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y - 20+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x+20 + frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y+20 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);

        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }
    else if(s_fruit_cut_angle>=225&&s_fruit_cut_angle<260)
    {
        P_LEFT = 2;
        P_RIGHT = 2;
        OFFSET_LEFT = 15;
        OFFSET_RIGHT = 15;
        left_layer_x = centre_point.x-10 - frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y - 20+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x+20 + frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y+20 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);

        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }
    else if(s_fruit_cut_angle>=260&&s_fruit_cut_angle<280)
    {
        P_LEFT = 8;
        P_RIGHT = 8;
        OFFSET_LEFT = 25;
        OFFSET_RIGHT = 25;
        left_layer_x = centre_point.x-30 - frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y - 20+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x+30 + frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y-20 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);

        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }  
    else if(s_fruit_cut_angle>=280&&s_fruit_cut_angle<315)
    {
        P_LEFT = 2;
        P_RIGHT = 2;
        OFFSET_LEFT = 15;
        OFFSET_RIGHT = 10;
        left_layer_x = centre_point.x-20 - frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y - 20+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x-20 - frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y+20 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);

        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }
    else
    {
        P_LEFT = 4;
        P_RIGHT = 3;
        OFFSET_LEFT = 20;
        OFFSET_RIGHT = 15;
        left_layer_x = centre_point.x-20 - frame_index*OFFSET_LEFT;
        left_layer_y = centre_point.y - 20+ (frame_index*OFFSET_LEFT)*(frame_index*OFFSET_LEFT)/(2*P_LEFT);
        right_layer_x = centre_point.x-20 - frame_index*OFFSET_RIGHT;
        right_layer_y = centre_point.y+20 + (frame_index*OFFSET_RIGHT)*(frame_index*OFFSET_RIGHT)/(2*P_RIGHT);

        left_dis.x = left_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        left_dis.y = left_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(left_lcd_dev_ptr,left_layer_x,left_layer_y);
        GUIRES_DisplayImg(&left_dis,PNULL,PNULL,win_id, left_image+frame_index, left_lcd_dev_ptr);

        right_dis.x = right_layer_x+(s_fruit_ring_width-s_fruit_tomato_width)/2;
        right_dis.y = right_layer_y+(s_fruit_ring_height-s_fruit_tomato_height)/2;

        UILAYER_SetLayerPosition(right_lcd_dev_ptr,right_layer_x,right_layer_y);
        GUIRES_DisplayImg(&right_dis,PNULL,PNULL,win_id, right_image+frame_index, right_lcd_dev_ptr);
    }
   
}


LOCAL void PlayTomatoSplitAnim(
                               MMI_WIN_ID_T win_id,
                               int16 rotate_angle,
                               GUI_POINT_T old_point,
                               GUI_POINT_T new_point
                               )
{
    uint16 i = 0;
    uint16   layer_x = 0;
    uint16 layer_y = 0;
    
    GUI_POINT_T dis_point = {0};
    
    IMG_EnableTransparentColor(TRUE);
    
    for(i = 0;i<FRUIT_RING_NUM;i++)
    {
        UILAYER_Clear(&s_fruit_icon_layer[i]);

        if((FRUIT_SPLIT_ANIM_NUM-1)==s_fruit_frame_index)
        {
           
        }
        else
        {
            layer_x = s_fruit_ring_point[i].x;
            layer_y = s_fruit_ring_point[i].y + s_fruit_frame_index*FRUIT_DROP_INTERVAL;
        
            UILAYER_SetLayerPosition(&s_fruit_icon_layer[i],layer_x, layer_y);

            dis_point.x = layer_x+(s_fruit_ring_width - s_fruit_tomato_width)/2;
        dis_point.y = layer_y+(s_fruit_ring_height- s_fruit_tomato_height)/2;
        
            switch(i)
            {
                case 0:
                    PlaySplitedFruitDropAnim(win_id,rotate_angle,s_fruit_ring_point[i],IMAGE_KL_TOMATO_LEFT1,
                        IMAGE_KL_TOMATO_RIGHT1,s_fruit_frame_index,&s_fruit_icon_layer[i],
                        &s_fruit_icon_layer[FRUIT_RING_NUM]);
                    break;
                case 1:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_APPLE1+s_fruit_frame_index,
                            &s_fruit_icon_layer[i]);
                break;
                case 2:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_ORANGE1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                case 3:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_PEAR1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                default:
                    break;
            }
        }
    }
   // if(0 == s_fruit_frame_index)//display juice image
    {   
        dis_point.x = s_fruit_ring_point[0].x + (s_fruit_ring_width - s_fruit_tomato_width)/2;
        dis_point.y = s_fruit_ring_point[0].y + (s_fruit_ring_height- s_fruit_tomato_height)/2;
        
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_TOMATO_JUICE,
            &s_fruit_main_layer);
    }
    
    IMG_EnableTransparentColor(FALSE);
}

LOCAL void PlayAppleSplitAnim(
                              MMI_WIN_ID_T win_id,
                              int16 rotate_angle,
                              GUI_POINT_T old_point,
                              GUI_POINT_T new_point
                              )
{
    uint16 i = 0;
    uint16   layer_x = 0;
    uint16 layer_y = 0;
    GUI_POINT_T dis_point = {0};

    IMG_EnableTransparentColor(TRUE);

    for(i = 0;i<FRUIT_RING_NUM;i++)
    {
        UILAYER_Clear(&s_fruit_icon_layer[i]);
        if((FRUIT_SPLIT_ANIM_NUM-1)!=s_fruit_frame_index)
        {
            layer_x = s_fruit_ring_point[i].x;
            layer_y = s_fruit_ring_point[i].y + s_fruit_frame_index*FRUIT_DROP_INTERVAL;
        
            UILAYER_SetLayerPosition(&s_fruit_icon_layer[i],layer_x, layer_y);

            dis_point.x = layer_x+(s_fruit_ring_width - s_fruit_tomato_width)/2;
            dis_point.y = layer_y+(s_fruit_ring_height- s_fruit_tomato_height)/2;
        
            switch(i)
            {
                case 0:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_FRUIT_TOMATO1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                case 1:
                    PlaySplitedFruitDropAnim(win_id,rotate_angle,s_fruit_ring_point[i],IMAGE_KL_APPLE_LEFT1,
                        IMAGE_KL_APPLE_RIGHT1,s_fruit_frame_index,&s_fruit_icon_layer[i],
                        &s_fruit_icon_layer[FRUIT_RING_NUM]);
                    break;
                case 2:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_ORANGE1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                case 3:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_PEAR1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                default:
                    break;
            }
        }
    }
  //  if(0 == s_fruit_frame_index)//display juice image
    {
        dis_point.x = s_fruit_ring_point[1].x + (s_fruit_ring_width - s_fruit_tomato_width)/2;
        dis_point.y = s_fruit_ring_point[1].y + (s_fruit_ring_height- s_fruit_tomato_height)/2;
        
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_APPLE_JUICE,
            &s_fruit_main_layer);
    }
    
    IMG_EnableTransparentColor(FALSE);
}


LOCAL void PlayOrangeSplitAnim(
                               MMI_WIN_ID_T win_id,
                               int16 rotate_angle,
                               GUI_POINT_T old_point,
                               GUI_POINT_T new_point
                               )
{
    uint16 i = 0;
    uint16   layer_x = 0;
    uint16 layer_y = 0;
    GUI_POINT_T dis_point = {0};

    IMG_EnableTransparentColor(TRUE);

    for(i = 0;i<FRUIT_RING_NUM;i++)
    {
        UILAYER_Clear(&s_fruit_icon_layer[i]);

        if((FRUIT_SPLIT_ANIM_NUM-1)!=s_fruit_frame_index)
        {
            layer_x = s_fruit_ring_point[i].x;
            layer_y = s_fruit_ring_point[i].y + s_fruit_frame_index*FRUIT_DROP_INTERVAL;
        
            UILAYER_SetLayerPosition(&s_fruit_icon_layer[i],layer_x, layer_y);

            dis_point.x = layer_x+(s_fruit_ring_width - s_fruit_tomato_width)/2;
            dis_point.y = layer_y+(s_fruit_ring_height- s_fruit_tomato_height)/2;
        
            switch(i)
            {
                case 0:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_FRUIT_TOMATO1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                case 1:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_APPLE1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                case 2:
                    PlaySplitedFruitDropAnim(win_id,rotate_angle,s_fruit_ring_point[i],IMAGE_KL_ORANGE_LEFT1,
                        IMAGE_KL_ORANGE_RIGHT1,s_fruit_frame_index,&s_fruit_icon_layer[i],
                        &s_fruit_icon_layer[FRUIT_RING_NUM]);
                    break;
                case 3:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_PEAR1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                default:
                    break;
            }
        }
    }
  //  if(0 == s_fruit_frame_index)//display juice image
    {
        dis_point.x = s_fruit_ring_point[2].x + (s_fruit_ring_width - s_fruit_tomato_width)/2;
        dis_point.y = s_fruit_ring_point[2].y + (s_fruit_ring_height- s_fruit_tomato_height)/2;
        
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_ORANGE_JUICE,
            &s_fruit_main_layer);
    }
    
    IMG_EnableTransparentColor(FALSE);
}

LOCAL void PlayPearSplitAnim(
                             MMI_WIN_ID_T win_id,
                             int16 rotate_angle,
                             GUI_POINT_T old_point,
                             GUI_POINT_T new_point
                             )
{
    uint16 i = 0;
    uint16   layer_x = 0;
    uint16 layer_y = 0;
    GUI_POINT_T dis_point = {0};

    IMG_EnableTransparentColor(TRUE);
    
    for(i = 0;i<FRUIT_RING_NUM;i++)
    {
        UILAYER_Clear(&s_fruit_icon_layer[i]);
        if((FRUIT_SPLIT_ANIM_NUM-1)!=s_fruit_frame_index)
        {
            layer_x = s_fruit_ring_point[i].x;
            layer_y = s_fruit_ring_point[i].y + s_fruit_frame_index*FRUIT_DROP_INTERVAL;
        
            UILAYER_SetLayerPosition(&s_fruit_icon_layer[i],layer_x, layer_y);

            dis_point.x = layer_x+(s_fruit_ring_width - s_fruit_tomato_width)/2;
            dis_point.y = layer_y+(s_fruit_ring_height- s_fruit_tomato_height)/2;
        
            switch(i)
            {
                case 0:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_FRUIT_TOMATO1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                case 1:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_APPLE1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                case 2:
                GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_ORANGE1+s_fruit_frame_index,
                    &s_fruit_icon_layer[i]);
                break;
                case 3:
                    PlaySplitedFruitDropAnim(win_id,rotate_angle,s_fruit_ring_point[i],IMAGE_KL_PEAR_LEFT1,
                        IMAGE_KL_PEAR_RIGHT1,s_fruit_frame_index,&s_fruit_icon_layer[i],
                        &s_fruit_icon_layer[FRUIT_RING_NUM]);
                    break;
                default:
                    break;
            }
        }
    }
  //  if(0 == s_fruit_frame_index)//display juice image
    {
        dis_point.x = s_fruit_ring_point[3].x + (s_fruit_ring_width - s_fruit_tomato_width)/2;
        dis_point.y = s_fruit_ring_point[3].y + (s_fruit_ring_height- s_fruit_tomato_height)/2;
        
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_PEAR_JUICE,
            &s_fruit_main_layer);
    }
    
    IMG_EnableTransparentColor(FALSE);
}


/******************************************************************************/
//  Description : display the split light image when fruit splited apart
//  Global resource dependence : none
//  Author: 
//  Note: Icon moving unlock style
/*****************************************************************************/
LOCAL void DisplaySplitLightOfFruit(
                                    MMI_WIN_ID_T win_id,
                                    uint16 icon_index,
                                    int16 rotate_angle,
                                    GUI_POINT_T old_point,
                                    GUI_POINT_T new_point
                                    )
{
        uint8* light_layer_buf_ptr = PNULL;
        uint8* temp_layer_buf_ptr = PNULL;
        //uint8* fruit_rotate_buf = PNULL;
        uint32 img_buf_size = 4*s_fruit_light_width*s_fruit_light_height;
        
        uint16 lcd_width = 0;
        uint16 lcd_height = 0;
        GUI_RECT_T update_rect = {0};
        GUI_POINT_T dis_point = {0};
        
        ROTATE_ARBITRARY_IN_T rotate_input = {0};

       ROTATE_ARBITRARY_OUT_T rotate_output = {0};

        int16 angle = 0;
        if(rotate_angle<=180)
        {
            angle = 90 -rotate_angle;
            if(angle<0)
            {
                angle +=360; 
            }
        }
        else if(rotate_angle<=270)
        {
            angle = 270 - rotate_angle;
        }
        else
        {
            angle = 360+90 - rotate_angle;
        }
    
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
        
  
        UILAYER_SetLayerPosition(&s_fruit_light_temp_layer,0,0);
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_KL_CUT_SHORT,
                &s_fruit_light_temp_layer);

        temp_layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_fruit_light_temp_layer);
       
        light_layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_fruit_light_layer);

        rotate_input.src_size.w = s_fruit_light_width;
        rotate_input.src_size.h = s_fruit_light_height;
        rotate_input.src_format = IMGREF_FORMAT_ARGB888;
        rotate_input.mode = ROTATE_ARBITRARY_AUTO_CROP;
        rotate_input.angle = angle;
        rotate_input.src_chn.chn0.ptr = temp_layer_buf_ptr;
        rotate_input.src_chn.chn0.size = img_buf_size;
        rotate_input.src_center.x = s_fruit_light_width/2;
        rotate_input.src_center.y = s_fruit_light_height/2;

        rotate_input.target_format = IMGREF_FORMAT_ARGB565;
        rotate_input.target_size.w = lcd_width;//s_fruit_light_height;
        rotate_input.target_size.h = lcd_height;//s_fruit_light_height;
        rotate_input.target_chn.chn0.ptr = light_layer_buf_ptr;//fruit_rotate_buf;
        rotate_input.target_chn.chn0.size = 4*lcd_width*lcd_height;//s_fruit_light_height;
    if(icon_index<FRUIT_RING_NUM)
    {
        rotate_input.target_center.x = s_fruit_ring_point[icon_index].x + s_fruit_ring_width/2 ;
        rotate_input.target_center.y = s_fruit_ring_point[icon_index].y + s_fruit_ring_height/2;
    }
    else
    {
        
    }
    GRAPH_RotateArbitrary(&rotate_input,&rotate_output);

         update_rect.right = lcd_width-1;
         update_rect.bottom = lcd_height-1;//s_fruit_light_height;
       MMITHEME_StoreUpdateRect(&s_fruit_light_layer,update_rect);

//       if(PNULL != fruit_rotate_buf)
//       {
//            SCI_FREE(fruit_rotate_buf);
//        }
}


/******************************************************************************/
//  Description : display the split fruit anim when fruit splited apart
//  Global resource dependence : none
//  Author: 
//  Note: Icon moving unlock style
/*****************************************************************************/
LOCAL void PlayFruitSplitAinm(
                              MMI_WIN_ID_T win_id,
                              int16 rotate_angle,
                              uint16 index,
                              GUI_POINT_T old_point,
                              GUI_POINT_T new_point
                              )
{
    switch(index)
    {
        case 0:
            PlayTomatoSplitAnim(win_id,rotate_angle,old_point,new_point);
            break;
        case 1:
            PlayAppleSplitAnim(win_id,rotate_angle,old_point,new_point);
            break;
        case 2:
            PlayOrangeSplitAnim(win_id,rotate_angle,old_point,new_point);
            break;
        case 3:
            PlayPearSplitAnim(win_id,rotate_angle,old_point,new_point);
            break;
        default:
            break;
    }
    UILAYER_Clear(&s_fruit_light_layer);
    UILAYER_Clear(&s_fruit_hand_layer);
    if(s_fruit_frame_index<1)
    {
        MMIAPIKL_PlayKeyLockRing();
        DisplaySplitLightOfFruit(win_id,index,rotate_angle,old_point,new_point);
    }

    s_fruit_frame_index++;
    
}

LOCAL void AppendAllIconLayerOfFruit(void)
{
    uint16 i = 0;

    UILAYER_APPEND_BLT_T append_blt = {0};
    
    for(i = 0;i < FRUIT_RING_NUM+1;i++)
    {
        append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_fruit_icon_layer[i];
        UILAYER_AppendBltLayer(&append_blt);
    }

    append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_fruit_hand_layer;
    UILAYER_AppendBltLayer(&append_blt);

    append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_fruit_light_layer;
    UILAYER_AppendBltLayer(&append_blt);
}

LOCAL void GetTheSplitAnglePhase(GUI_POINT_T old_point,GUI_POINT_T new_point)
{
    if(old_point.x < new_point.x && old_point.y < new_point.y)
    {
        s_fruit_cut_phase = FRUIT_ANGLE_PHASE_2;
    }
    else if(old_point.x < new_point.x && old_point.y > new_point.y)
    {
        s_fruit_cut_phase = FRUIT_ANGLE_PHASE_3;
    }
    else if(old_point.x > new_point.x && old_point.y > new_point.y)
    {
        s_fruit_cut_phase = FRUIT_ANGLE_PHASE_4;
    }
    else if(old_point.x > new_point.x && old_point.y < new_point.y)
    {
        s_fruit_cut_phase = FRUIT_ANGLE_PHASE_1;
    }
    else if(old_point.x == new_point.x && old_point.y < new_point.y)
    {
        s_fruit_cut_phase = FRUIT_VERTICAL_UP_DOWN;
    }
    else if(old_point.x == new_point.x && old_point.y > new_point.y)
    {
        s_fruit_cut_phase = FRUIT_VERTICAL_DOWN_UP;
    }
    else if(old_point.y == new_point.y && old_point.x < new_point.x)
    {
        s_fruit_cut_phase = FRUIT_HORIZONTAL_LEFT_RIGHT;
    }
    else
    {
        s_fruit_cut_phase = FRUIT_HORIZONTAL_RIGHT_LEFT;
    }
}

/******************************************************************************/
//  Description :Calculate the angle of the Slide
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL int16 CalcTpSlideAngleOfFruit(GUI_POINT_T old_point,GUI_POINT_T new_point)
{
    uint i = 0;
    int16 angle = 0;
    int16 sin_value = CalcTpSlideSinValueOfFruit(old_point,new_point);
/******_sin_0_90_table表没有负值因此先计算正值角度再根据相位纠正***********/
    if(sin_value<0)
    {
        sin_value = -sin_value;
    }
    /**计算sin值时已乘以1000，此处也要乘以1000**/
    for(i = 0;i<(sizeof(_sin_0_90_table)/sizeof(_sin_0_90_table[0])-1);i++)
    {
        if(sin_value>=(_sin_0_90_table[i]*1000)&&sin_value<(_sin_0_90_table[i+1]*1000))
        {
            angle = i;
            break;
        }
    }
    if(sin_value>990&&0==angle)
    {
        switch(s_fruit_cut_phase)
        {
            case FRUIT_VERTICAL_UP_DOWN:
                angle = 90;
            break;
            case FRUIT_VERTICAL_DOWN_UP:
                angle = 270;
            break;
            default:
                break;
        }
    }
    switch(s_fruit_cut_phase)
    {
        case FRUIT_HORIZONTAL_LEFT_RIGHT:
        case FRUIT_ANGLE_PHASE_2:
            angle = 180-angle;
            break;
        case FRUIT_ANGLE_PHASE_3:
            angle = angle+180;
            break;
         case FRUIT_ANGLE_PHASE_4:
            angle = 360 - angle;
            break;
         default:
             break;
    }
    return angle;
}

LOCAL void SetProcOpenFlagOfFruit(uint16 index)
{
    KL_PROC_FLAG_E flag = PROC_MAX;
    switch(index)
    {
        case 1:
            flag = PROC_CALL;
            break;
        case 2:
            flag = PROC_SMS;
                break;
         case 3:
            flag = PROC_MAINMENU;
            break;
         default:
            break;
    }
    MMIKL_SetProcOpenFlag(flag);
}

/******************************************************************************/
//  Description : handle keylock display window msg of split fruit.
//  Global resource dependence : none
//  Author: 
//  Note: Icon moving unlock style
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleFruitSplitWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            uint16          msg_id, 
                                            DPARAM          param
                                            )
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    MMI_TP_STATUS_E tp_status = 0;
    static GUI_POINT_T tp_point = {0};
    static GUI_POINT_T tp_old_point = {0};
     GUI_POINT_T tp_up_point = {0};
    
    static FRUIT_STATE_TYPE_E fruit_state = FRUIT_STATE_IDLE;
    static int16 icon_index = PROC_MAX;
    
    static BOOLEAN have_select_icon = FALSE;
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
          //  MMIKeyLock_CreatWallpaperCtrl();
 
            GUIRES_GetImgWidthHeight(&s_fruit_ring_width, &s_fruit_ring_height,
                IMAGE_KL_RING_BLUE,win_id);

            if(0==s_fruit_ring_width||0==s_fruit_ring_height)
            {
                s_fruit_ring_width = FRUIT_RING_PIC_WIDTH;
                s_fruit_ring_height = FRUIT_RING_PIC_HEIGHT;
            }

            GUIRES_GetImgWidthHeight(&s_fruit_tomato_width, &s_fruit_tomato_height,
                IMAGE_KL_FRUIT_TOMATO1,win_id);

            if(0==s_fruit_tomato_width||0== s_fruit_tomato_height)
            {
                s_fruit_tomato_width = FRUIT_TOMATO_PIC_WIDTH;
                s_fruit_tomato_height= FRUIT_TOMATO_PIC_HEIGHT;
            }

            GUIRES_GetImgWidthHeight(&s_fruit_hand_width, &s_fruit_hand_height,
                IMAGE_KL_FRUIT_HAND,win_id);

            GUIRES_GetImgWidthHeight(&s_fruit_light_width, &s_fruit_light_height,
                IMAGE_KL_CUT_SHORT,win_id);

            GUIRES_GetImgWidthHeight(&s_fruit_info_width, &s_fruit_info_height,
                IMAGE_KL_FRUIT_INFO,win_id);
            
            CreateIconLayerOfFruitSplit(win_id);
            CreateHandLayerOfFruitSplit(win_id);
            CreateLightLayerOfFruitSplit(win_id);
            CreateTempLayerOfFruitSplit(win_id);

	        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
            GUIWIN_SetStbBgIsDesplay(win_id,TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR            
		    GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
            MMK_SetWinLcdDevInfoNotifyCtrl(win_id,&s_fruit_main_layer);
            
            s_fruit_cut_phase = FRUIT_ANGLE_PHASE_MAX;
            have_select_icon = FALSE;
            tp_old_point.x = 0;
            tp_old_point.y = 0;
            fruit_state = FRUIT_STATE_IDLE;
            icon_index = PROC_MAX;
            s_fruit_hand_interval = 0;
           break;
            
        case MSG_FULL_PAINT:
            DisplayIdleImageOfFruitSplit(win_id);
            {
                GUI_RECT_T  missevent_rect = {0};
                MMI_BACKLIGHT_STATUS_E backlight_status = {0};
                
                missevent_rect.left = FRUIT_MISSEVENT_RECT_LEFT;    
                missevent_rect.right = FRUIT_MISSEVENT_RECT_LEFT + FRUIT_MISSEVENT_RECT_WIDTH;
                missevent_rect.top = MMI_STATUSBAR_HEIGHT;
                missevent_rect.bottom =missevent_rect.top + FRUIT_MISSEVENT_RECT_HEIGHT;
                MMIKL_DisplayMissedEvent(win_id, &missevent_rect, &s_fruit_main_layer);

                 backlight_status = MMIDEFAULT_GetBackLightStateEx();
                if(MMI_BACKLIGHT_ON == backlight_status)
                {//锁屏界面来电话挂断后创建锁屏界面需启动这个timer
                    StartFruitTimer(win_id);
                }
            }
            break;
        case MSG_TIMER:
            if(s_fruit_anim_timer == (*(uint8*)param))
            {
                if(FRUIT_STATE_IDLE == fruit_state)
                {
                    StopFruitTimer();
                    PlayIdleFruitAnim(win_id);
                    StartFruitTimer(win_id);
                }
                else if(FRUIT_STATE_SPLITED == fruit_state)
                {
                    StopFruitTimer();
                    PlayFruitSplitAinm(win_id,s_fruit_cut_angle,icon_index,tp_old_point,tp_point);
                    if(s_fruit_frame_index >= FRUIT_SPLIT_ANIM_NUM)
                    {
                        uint32 i = 0;
                        s_fruit_frame_index = 0;
                        fruit_state = FRUIT_STATE_IDLE;
                        MMIKL_SetWinStatus(KL_UNLOCKED);
                        icon_index = -1;
                        for (i = 0; i < FRUIT_IDLE_ANIM_NUM; i++)
                        {
                            UILAYER_RemoveBltLayer(&s_fruit_icon_layer[i]);
                            UILAYER_RELEASELAYER(&s_fruit_icon_layer[i]);/*lint !e774 !e831*/
                        }
                        UILAYER_RemoveBltLayer(&s_fruit_main_layer);
                        MMK_CloseWin(win_id);

                    }
                    else
                    {
                        StartFruitTimer(win_id);
                        if(1 == s_fruit_frame_index)
                        {
                            UILAYER_RemoveBltLayer(&s_fruit_hand_layer);
                            UILAYER_RELEASELAYER(&s_fruit_hand_layer);/*lint !e774 !e506 !e831*/
                        }
                        else if(2 == s_fruit_frame_index)
                        {
                            UILAYER_RemoveBltLayer(&s_fruit_light_temp_layer);
                            UILAYER_RemoveBltLayer(&s_fruit_light_layer);
                            UILAYER_RELEASELAYER(&s_fruit_light_temp_layer);/*lint !e774 !e506 !e831*/
                            UILAYER_RELEASELAYER(&s_fruit_light_layer);/*lint !e774 !e506 !e831*/
                        }
                    }
                }
            }
            else
            {
                res = MMI_RESULT_FALSE;
            }
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
       case MSG_TP_PRESS_DOWN:
            tp_old_point.x = MMK_GET_TP_X(param);
            tp_old_point.y = MMK_GET_TP_Y(param);
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_MOVE:
            MMK_GetLogicTPMsg(&tp_status, &tp_point);
             if(FRUIT_STATE_SPLITED!=fruit_state&& tp_old_point.x >0&&
                JudgePointInRingRect(tp_point,&icon_index))
            {
                have_select_icon = TRUE;
                GetTheSplitAnglePhase(tp_old_point,tp_point);
                s_fruit_cut_angle = CalcTpSlideAngleOfFruit(tp_old_point,tp_point);
                StopFruitTimer();
                fruit_state = FRUIT_STATE_SPLITED;
                s_fruit_frame_index = 0;
                StartFruitTimer(win_id);
                SetProcOpenFlagOfFruit(icon_index);
            
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:
            tp_up_point.x = MMK_GET_TP_X(param);
            tp_up_point.y = MMK_GET_TP_Y(param);
            if(FRUIT_STATE_SPLITED!=fruit_state&& tp_old_point.x >0&&
                tp_up_point.x>0&&
                JudgeSlideRingRect(tp_old_point,tp_up_point,&icon_index))
            {
                have_select_icon = TRUE;
                GetTheSplitAnglePhase(tp_old_point,tp_up_point);
                s_fruit_cut_angle = CalcTpSlideAngleOfFruit(tp_old_point,tp_up_point);
                StopFruitTimer();
                fruit_state = FRUIT_STATE_SPLITED;
                s_fruit_frame_index = 0;
                StartFruitTimer(win_id);
                SetProcOpenFlagOfFruit(icon_index);
            
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
        case MSG_GET_FOCUS:
            AppendAllIconLayerOfFruit();
            StartFruitTimer(win_id);
            break;
        case MSG_BACKLIGHT_TURN_ON:
            StartFruitTimer(win_id);
            break;    
        case MSG_LOSE_FOCUS:
        case MSG_BACKLIGHT_TURN_OFF:
            StopFruitTimer();
            break;
        case MSG_KEYDOWN_RED://过滤，不能删除,锁键盘界面不能关机
        case MSG_KEYLONG_RED:
            if(MMIKL_IsPreviewState())
            {
                res = MMI_RESULT_FALSE;
            }
            break;
        case MSG_APP_OK:
      //  case MSG_APP_MENU:
            if(MMIKL_IsPreviewState())
            {
                MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_FRUIT_SPLIT);
                MMK_CloseWin(win_id);
            }
		    else
		    {
			    res = MMI_RESULT_FALSE;		
		    }
            break;
        case MSG_APP_CANCEL:
            if(MMIKL_IsPreviewState())
            {
                MMK_CloseWin(win_id);
            }
		    else
		    {
			    res = MMI_RESULT_FALSE;		
		    }
            break;
        case MSG_CLOSE_WINDOW:
            StopFruitTimer();
            break;
       case MSG_NOTIFY_STATUSBAR_PAINT:
            break;
       case MSG_NOTIFY_STATUSBAR_PAINT_END:
            break;      
       default:
            res = MMI_RESULT_FALSE;
            break;
    }
    
    return res;
}
#endif	//MMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT

#endif




/*Edit by script, ignore 3 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
