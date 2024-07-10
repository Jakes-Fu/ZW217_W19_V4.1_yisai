/*****************************************************************************
** File Name:      mmikl_drawgraphic.c                                        *
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
#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmikl_drawgraphic.h"
#include "ui_layer.h"
#include "guistring.h"
#include "guires.h"
#include "mmi_id.h"
#include "mmikl_export.h"
#include "mmidisplay_data.h"
#include "mmi_textfun.h"
#include "mmi_keylock_position.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "window_parse.h"
#include "mmiset_text.h"
#include "mmicom_time.h"
#include "mmiidle_statusbar.h"
#include "mmiset_export.h"
#include "mmikl_internal.h"
#include "mmiset_lock_effect_export.h"
#include "mmiidle_export.h"
#include "mmicc_text.h"
#include "guistatusbar.h"
#include "mmi_appmsg.h"
#include "mmk_tp.h"
#include "guictrl_api.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

#define MMIKL_KL_PASSWARD_UNLOCK_WIN_ID     MMIKL_CLOCK_DISP_WIN_ID

#ifdef MMI_PDA_SUPPORT
#define USE_SOFTKEY_BUTTON
#endif

#ifndef UILAYER_MAIN_HANDLE
#define UILAYER_MAIN_HANDLE     GUI_BLOCK_MAIN
#endif

//使用带Alpha的透明线绘制图形解锁过程，关闭则使用平台非透明线绘制
#define  MMI_KL_DIS_ALPHA_LINE

#define MMI_KL_PASSWARD_CIRCLE_RADIUS  30
#define MMI_KL_PASSWARD_LINE_WIDTH  24
#define  ANDROID_UNLOCK_CONFIRM_TIME             500//
#define  MMI_KL_PATTERN_LINE_ALPHA    112
#define  MMI_KL_PATTERN_TP_MOVE_TIMER    50
#define ZOOM_OFFSET(offset, lcd_width, lcd_height)    (((offset) * (lcd_width) * 2 + (lcd_height)) / ((lcd_height) * 2))

LOCAL GUI_LCD_DEV_INFO  s_keylock_date_layer_handle = {0,UILAYER_NULL_HANDLE};

#define KL_PATTERN_TIPS_BG_RECT   {0, MMI_MAINSCREEN_HEIGHT -KL_PATTERN_TIPS_HEIGHT, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT}
#define KL_PATTERN_TIPS_LEFT_RECT    {10, MMI_MAINSCREEN_HEIGHT -KL_PATTERN_TIPS_HEIGHT+5, MMI_MAINSCREEN_WIDTH/2-10, MMI_MAINSCREEN_HEIGHT -5}
#define KL_PATTERN_TIPS_RIGHT_RECT    {MMI_MAINSCREEN_WIDTH/2+10, MMI_MAINSCREEN_HEIGHT -KL_PATTERN_TIPS_HEIGHT+5, MMI_MAINSCREEN_WIDTH -10, MMI_MAINSCREEN_HEIGHT -5}

#define KL_PATTERN_TIPS_CENTER_RECT    {5+MMI_MAINSCREEN_WIDTH/4, MMI_MAINSCREEN_HEIGHT -32-5, 3*MMI_MAINSCREEN_WIDTH/4 -5, MMI_MAINSCREEN_HEIGHT-5}

#define KL_PATTERN_PSW_MAX  9
typedef enum
{
    KL_PATTERN_TIPS_LEFT_BLOCK =0,
    KL_PATTERN_TIPS_RIGHT_BLOCK =1,
    KL_PATTERN_TIPS_BLOCK_MAX
}MMI_KL_PATTERN_TIPS_BLOCK_E;

typedef enum
{
    KL_WIN_TYPE_SET =0,
    KL_WIN_TYPE_CONFIRM =1,
    KL_WIN_TYPE_MAX
}MMI_KL_WIN_TYPE_E;

typedef enum
{
    KL_KEY_STATE_NONE,
    KL_KEY_STATE_DOWN,
    KL_KEY_STATE_MOVE,
    KL_KEY_STATE_UP,
    KL_KEY_STATE_MAX
}MMI_KL_KEY_STATE_E;

typedef enum
{
    KL_KEY_TIPS_NONE_NONE  ,
    KL_KEY_TIPS_DIM_DIM = KL_KEY_TIPS_NONE_NONE,
    KL_KEY_TIPS_LIGHT_DIM =1,
    KL_KEY_TIPS_DIM_LIGHT =2, 
    KL_KEY_TIPS_LIGHT_LIGHT =3,
    KL_KEY_TIPS_MAX 
}MMI_KL_KEY_TIPS_E;

typedef enum
{
    KL_KEY_TIPS_MSG_NONE  ,
    KL_KEY_TIPS_MSG_FULL_PAINT,
    KL_KEY_TIPS_MSG_CLOSE,
    KL_KEY_TIPS_MSG_SET2CONFIRM ,
    KL_KEY_TIPS_MSG_SAVED,
    KL_KEY_TIPS_MSG_MAX 
}MMI_KL_KEY_TIPS_MSG_TYPE_E;

typedef struct
{
    //MMI_MESSAGE_ID_E    tips_msg;
    //GUI_RECT_T                tips_valid_rect;
    //MMI_STRING_T            tips_text;
    //BOOLEAN                     is_diaplay_tips;
    MMI_KL_KEY_TIPS_MSG_TYPE_E    msg_type;
    
}KEYLOCK_PATTERN_TIPS_INFO_T;

typedef struct 
{
    GUI_POINT_T st_passward_point[KL_PATTERN_PSW_MAX];                                       //九个宫格点在LCD上的坐标点
    GUI_LCD_DEV_INFO  slide_layer_handle;                                   //画不确定的不固定的直线图层句柄，用在TP_MOVE时
    GUI_LCD_DEV_INFO  title_layer_handle;                                    //画Title 与Tips 的图层句柄
    GUI_LCD_DEV_INFO  image_confirm_layer_handle;                  //
    GUI_LCD_DEV_INFO  wallpaper_layer;                                       //背景层(墙纸)
    
    int8 new_point_ptr[MMI_KL_PASSWARD_POINT_MAX];              //设置时，记录输入时宫格点
    int8 new_point_again_ptr[MMI_KL_PASSWARD_POINT_MAX];   //确认时，记录输入时宫格点
    int8 unlock_point_ptr[MMI_KL_PASSWARD_POINT_MAX];          //解锁时，记录输入时宫格点
    uint8  point_index;                                                                  //当前输入点的index
    uint8   KL_pattern_ok_timer_id;                                             //只对图案解锁匹配时使用
    uint8   KL_pattern_tips_timer_id;                                           //设置图案密码失败时使用
    uint8   KL_pattern_tp_move_id;                            //滑动屏幕后定时取点

    MMI_KL_WIN_TYPE_E  win_type;                                                  //只对设置图案密码时区分:设置和确认
    MMI_KL_KEY_STATE_E  key_state;                                               //四种按键类型，根据它来显示title和tips
    MMI_KL_KEY_TIPS_E   tips_type;                                                 //tips区域输入类型
    
    BOOLEAN input_result;                                                               //画图结果是否达到要求，是:只能对tips区域触摸有效
    BOOLEAN s_is_visible_passward;                                              //解锁画图是否可视
    BOOLEAN is_handled;                                                                 //画图结果正确后，是否处理过tips对应的消息

    KEYLOCK_PATTERN_TIPS_INFO_T   tips_info[KL_PATTERN_TIPS_BLOCK_MAX];     //Tips两个Block映射的消息
}KEYLOCK_INFO_T;

LOCAL KEYLOCK_INFO_T *s_kl_info_ptr = PNULL;


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
//  Description :设置图形宫格点的位置
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void SetKeyLockPatternDispRect(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Prepare layer for Date label
//  Global resource dependence : 
//  Author: David Chen
//  Date:
/*****************************************************************************/
//LOCAL void PrepareKeylockDateLayer(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Release layer for Date label
//  Global resource dependence : 
//  Author: David Chen
//  Date:
/*****************************************************************************/
//LOCAL void ReleaseKeylockDateLayer(void);

/*****************************************************************************/
//  Description : Append layer for Date label
//  Global resource dependence : 
//  Author: David Chen
//  Date:
/*****************************************************************************/
//LOCAL void AppendKeylockDateLayer(void);



/*****************************************************************************/
//  Description : 处理图形加密及解密入口函数
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetKLPasswardWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            );

#if 0
// the window for input passward
WINDOW_TABLE( MMIKL_KL_PASSWARD_UNLOCK_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetKLPasswardWindow ),    
    WIN_ID( MMIKL_KL_PASSWARD_UNLOCK_WIN_ID ),
    WIN_STATUSBAR,
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
    WIN_STYLE( WS_DISABLE_RETURN_WIN|WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG ),
    CREATE_ANIM_CTRL(MMIKL_PATTERN_UNLCOK_ANIM_CTRL_ID,MMIKL_KL_PASSWARD_UNLOCK_WIN_ID),
    END_WIN
};
#endif

WINDOW_TABLE( MMIKL_SET_KL_PASSWARD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetKLPasswardWindow ),    
    WIN_ID( MMIKL_SET_KL_PASSWARD_WIN_ID ),
    WIN_STYLE(WS_DISABLE_COMMON_BG),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
    WIN_TITLE( TXT_SET_CHANGE_PATTERN_UNLOCK),
    WIN_STATUSBAR,
    END_WIN
};
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
//  Description :判断点是否在有效范围
//                      point(in),                  当前坐标点
//                      point_ptr(out),          在有效范围时获取到点序号
//                      return,FALSE:不在点范围，TRUE:在点范围
//  Global resource dependence :
//  Author:
//  Date:
/*****************************************************************************/
LOCAL BOOLEAN IsPointAround(GUI_POINT_T point, int8 *pos)
{
    uint32 i = 0;
    uint32 sizeRatio = 0;
        
    if (PNULL == s_kl_info_ptr)
    {
        return FALSE;
    }
    sizeRatio =  sizeof(s_kl_info_ptr->st_passward_point)/sizeof(GUI_POINT_T);

    for (i=0; i < sizeRatio; i++)
    {
        if ((abs(point.x - s_kl_info_ptr->st_passward_point[i].x) <MMI_KL_PASSWARD_CIRCLE_RADIUS-1)  
            && (abs(point.y - s_kl_info_ptr->st_passward_point[i].y) <MMI_KL_PASSWARD_CIRCLE_RADIUS-1))
        {
            *pos = i;
            return TRUE;
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description :判断是否重复了点
//                      point(in),                  当前点坐标
//                      point_ptr(in),            对应当前记录序号数组
//                      return,FALSE:不是重复的点，TRUE:是重复的点
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL BOOLEAN IsRepeatPoint(GUI_POINT_T point, int8 *point_ptr)
{
    uint8 i=0;
    int8 index = 0;
    if (PNULL != s_kl_info_ptr)
    {
        for (i=0; i< s_kl_info_ptr->point_index; i++)
        {
            index = point_ptr[i];
            if ((abs(point.x - s_kl_info_ptr->st_passward_point[index].x) <MMI_KL_PASSWARD_CIRCLE_RADIUS)  && 
                (abs(point.y - s_kl_info_ptr->st_passward_point[index].y) <MMI_KL_PASSWARD_CIRCLE_RADIUS))
            {
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description :判断是否跨过了点，有就获取序号到passpoint_index指针中
//                      cur_index(in),           当前点序号
//                      point_ptr(in),            对应当前记录序号数组
//                      passpoint_index(out),获取跨过的点序号指针
//                      return,FALSE:不是跨过的点，TRUE:是跨过的点
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL BOOLEAN IsPassPoint(const int8 cur_index, int8 *point_ptr, int8 *passpoint_index)
{
    int8 i=0,j=0;
    int8 pre_index =0;
    uint8 valid_index[8][2]={{1,7},{3,5},{0,8},{2,6},{0,2},{0,6},{2,8},{6,8}};

    if (PNULL != s_kl_info_ptr)
    {
        pre_index =point_ptr[s_kl_info_ptr->point_index] ;

        for(i=0; i<8; i++)
        {
            for(j=0; j<2; j++)
            {
                if ((valid_index[i][j] == cur_index)&&(valid_index[i][1-j] == pre_index))
                {
                    *passpoint_index = (cur_index + pre_index)/2;
                
                    for(i=0; i<s_kl_info_ptr->point_index-1; i++)
                    {
                        if (*passpoint_index == point_ptr[i] )
                        {
                            return FALSE;
                        }
                    }
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description :密码比较
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL BOOLEAN ComparePassward(const int8 *passward1, const int8 *passward2)
{
    uint8 i=0;

    for (i =0; i< MMI_KL_PASSWARD_POINT_MAX; i++)
    {
        if (passward1[i] != passward2[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description :Create pattern key lock password slide layer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void CreatKLPasswardSlideLayer(MMI_WIN_ID_T win_id)
{
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    uint16              title_height =0;

    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO   slide_layer_handle ={0,UILAYER_NULL_HANDLE};
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    if(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
    {
        title_height =KL_PATTERN_TITLE_HEIGHT;
    }
    else
    {
        title_height =KL_PATTERN_TITLE_LINE_HEIGHT;
    }
    
    if (PNULL != s_kl_info_ptr && (UILAYER_NULL_HANDLE == s_kl_info_ptr->slide_layer_handle.block_id))
    {
        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = title_height;
        create_info.width = lcd_width-1;
        create_info.height = MMI_MAINSCREEN_HEIGHT - KL_PATTERN_TIPS_HEIGHT - title_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_IMMUTABLE;
        UILAYER_CreateLayer(&create_info,&slide_layer_handle);

        UILAYER_SetLayerColorKey(&slide_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        s_kl_info_ptr->slide_layer_handle =slide_layer_handle;
        UILAYER_SetLayerPosition(&slide_layer_handle, 0, title_height);
    }

    
    if (UILAYER_IsLayerActive(&slide_layer_handle))
    {
        UILAYER_Clear(&slide_layer_handle);
    }
}

/*****************************************************************************/
//  Description :Create pattern key lock password title layer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void CreatKLPasswardTitleLayer(MMI_WIN_ID_T win_id)
{
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    GUI_RECT_T          layer_rect = {0};

    UILAYER_CREATE_T    create_info = {0};
    
    layer_rect = MMITHEME_GetFullScreenRect();


    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    if (PNULL!= s_kl_info_ptr && (UILAYER_NULL_HANDLE == s_kl_info_ptr->title_layer_handle.block_id))
    {
        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = layer_rect.left;
        create_info.offset_y = layer_rect.top;
        create_info.width = layer_rect.right+ 1;
        create_info.height = layer_rect.bottom+ 1;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_IMMUTABLE;
        UILAYER_CreateLayer(&create_info,&s_kl_info_ptr->title_layer_handle);

        UILAYER_SetLayerColorKey(&s_kl_info_ptr->title_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(&s_kl_info_ptr->title_layer_handle);
        UILAYER_SetLayerPosition(&s_kl_info_ptr->title_layer_handle, 0, 0);
    }
}

/*****************************************************************************/
//  Description :Create pattern key lock password confirm layer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void CreatKLPasswardConfirmLayer(MMI_WIN_ID_T win_id)
{
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    uint16              title_height =0;

    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO   image_confirm_layer_handle ={0,UILAYER_NULL_HANDLE};
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    if(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
    {
        title_height =KL_PATTERN_TITLE_HEIGHT;
    }
    else
    {
        title_height =KL_PATTERN_TITLE_LINE_HEIGHT;
    }
    
    if (PNULL != s_kl_info_ptr && (UILAYER_NULL_HANDLE == s_kl_info_ptr->image_confirm_layer_handle.block_id))
    {
        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = title_height;
        create_info.width = lcd_width -1;
        create_info.height = MMI_MAINSCREEN_HEIGHT - KL_PATTERN_TIPS_HEIGHT - title_height -1;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_IMMUTABLE;
        UILAYER_CreateLayer(&create_info,&image_confirm_layer_handle);

        UILAYER_SetLayerColorKey(&image_confirm_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        s_kl_info_ptr->image_confirm_layer_handle =image_confirm_layer_handle;
        UILAYER_SetLayerPosition(&image_confirm_layer_handle, 0, title_height);
    }
    
    if (UILAYER_IsLayerActive(&image_confirm_layer_handle))
    {
        UILAYER_Clear(&image_confirm_layer_handle);
    }
}

/*****************************************************************************/
//  Description : Create pattern key lock wallpapaer layer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void CreatKLwallpaperLayer(MMI_WIN_ID_T win_id)
{
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;

    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO   wallpaper_layer ={0,UILAYER_NULL_HANDLE};
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    if (PNULL != s_kl_info_ptr && (UILAYER_NULL_HANDLE == s_kl_info_ptr->wallpaper_layer.block_id))
    {
        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = lcd_width;
        create_info.height = lcd_height;
        /* Modify by michael on 3/2/2012 for  NEWMS00161908: 此处不设为bg，可以使背景色红色小时，显示wallpaper*/
        create_info.is_bg_layer = FALSE;   
        /************************Modify over*************************/
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_IMMUTABLE;
        UILAYER_CreateLayer(&create_info,&wallpaper_layer);

        UILAYER_SetLayerColorKey(&wallpaper_layer, TRUE, UILAYER_TRANSPARENT_COLOR);
        s_kl_info_ptr->wallpaper_layer =wallpaper_layer;
        UILAYER_SetLayerPosition(&wallpaper_layer, 0, 0);
    }
    
    if (UILAYER_IsLayerActive(&wallpaper_layer))
    {
        UILAYER_Clear(&wallpaper_layer);
    }
}

/*****************************************************************************/
//  Description :创建记录当前图形密码窗口的结构体指针s_kl_info_ptr
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void CreateKeyLockPattern(MMI_WIN_ID_T win_id)
{
    if(PNULL == s_kl_info_ptr)
    {
        s_kl_info_ptr =(KEYLOCK_INFO_T *)SCI_ALLOCA(sizeof(KEYLOCK_INFO_T));
        if (PNULL != s_kl_info_ptr)
        {
            SCI_MEMSET(s_kl_info_ptr, 0, sizeof(MMISET_DATE_T));
        }
    }
    else
    {
        //在设置图形密码时可能会锁屏，要清掉设置图形密码时的数据
        SCI_MEMSET(s_kl_info_ptr, 0, sizeof(MMISET_DATE_T));
    }
}

/*****************************************************************************/
//  Description :初始化结构体指针s_kl_info_ptr
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void InitKeyLockPatternInfo(MMI_WIN_ID_T win_id)
{
    if (s_kl_info_ptr !=PNULL)
    {
        s_kl_info_ptr->slide_layer_handle.lcd_id =0;
        s_kl_info_ptr->slide_layer_handle.block_id =UILAYER_NULL_HANDLE;
            
        s_kl_info_ptr->title_layer_handle.lcd_id =0;
        s_kl_info_ptr->title_layer_handle.block_id =UILAYER_NULL_HANDLE;
     
        s_kl_info_ptr->image_confirm_layer_handle.lcd_id =0;
        s_kl_info_ptr->image_confirm_layer_handle.block_id =UILAYER_NULL_HANDLE;

        s_kl_info_ptr->wallpaper_layer.lcd_id =0;
        s_kl_info_ptr->wallpaper_layer.block_id = 0;//UILAYER_NULL_HANDLE;
        
        SCI_MEMSET(s_kl_info_ptr->new_point_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
        SCI_MEMSET(s_kl_info_ptr->new_point_again_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
        SCI_MEMSET(s_kl_info_ptr->unlock_point_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
        
        s_kl_info_ptr->point_index = 0;
        s_kl_info_ptr->KL_pattern_tips_timer_id =0 ;
        s_kl_info_ptr->KL_pattern_ok_timer_id =0;
        s_kl_info_ptr->KL_pattern_tp_move_id = 0;
        
        s_kl_info_ptr->s_is_visible_passward = MMIAPISET_GetVisibleKeyLock();
        s_kl_info_ptr->input_result = FALSE;
        s_kl_info_ptr->win_type =KL_WIN_TYPE_SET;
        s_kl_info_ptr->key_state = KL_KEY_STATE_NONE;
        s_kl_info_ptr->tips_type = KL_KEY_TIPS_NONE_NONE;

        s_kl_info_ptr->is_handled =FALSE;

        SetKeyLockPatternDispRect(win_id);
    }
    
}

/*****************************************************************************/
//  Description :获取显示时间日期周属性
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void GetClockTextStyle(
                            GUISTR_STYLE_T *time_style_ptr,
                            GUISTR_STYLE_T *date_style_ptr,
                            GUISTR_STYLE_T *week_style_ptr
                            )
{
    //MMITHEME_WIDGET_CLOCK_T clock_widget_theme = {0};

    //MMITHEME_GetClockWidgetTheme(&clock_widget_theme);    

    //time
    if (PNULL != time_style_ptr)
    {
        time_style_ptr->effect = FONT_EFFECT_NONE; 
        time_style_ptr->angle = ANGLE_0;
        time_style_ptr->align = ALIGN_LVMIDDLE;
        time_style_ptr->font = SONG_FONT_24;
        time_style_ptr->font_color = MMI_GRAY_WHITE_COLOR;
    }
    //date
    if (PNULL != date_style_ptr)
    {
        date_style_ptr->effect = FONT_EFFECT_NONE; 
        date_style_ptr->angle = ANGLE_0;
        date_style_ptr->align = ALIGN_LVMIDDLE;
        date_style_ptr->font = SONG_FONT_12;
        date_style_ptr->font_color = MMI_WHITE_COLOR;
    }

    //week
    if (PNULL != week_style_ptr)
    {
        week_style_ptr->effect = FONT_EFFECT_NONE; 
        week_style_ptr->angle = ANGLE_0;
        week_style_ptr->align = ALIGN_HVMIDDLE;
        week_style_ptr->font = SONG_FONT_12;
        week_style_ptr->font_color = MMI_WHITE_COLOR;
    }
}

/*****************************************************************************/
//  Description :在解锁窗口上显示时间日期周
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void DisplayClock(MMI_WIN_ID_T win_id)
{
    #define DATE_STR_LEN    50
    SCI_DATE_T                  sys_date        = {0};
    GUISTR_STYLE_T time_text_style = {0};
    GUISTR_STYLE_T date_text_style = {0};
    GUISTR_STYLE_T week_text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;

    char u8_date_str[DATE_STR_LEN] = {0};
    MMI_STRING_T time_str = {0};
    MMI_STRING_T time_type_str = {0};
    MMI_STRING_T date_str = {0};
    MMI_STRING_T week_str = {0};
    GUI_RECT_T clock_rect = {0};
    GUI_RECT_T time_rect = KL_PATTERN_TIME_RECT;
    GUI_RECT_T date_rect = KL_PATTERN_DATE_RECT;
    GUI_RECT_T week_rect = KL_PATTERN_WEEK_RECT;

    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;
    SCI_TIME_T         time = {0};

    MMI_TEXT_ID_T       week_text[] = {TXT_IDLE_SHORT_SUNDAY,
                                        TXT_IDLE_SHORT_MONDAY,
                                        TXT_IDLE_SHORT_TUESDAY,
                                        TXT_IDLE_SHORT_WEDNESDAY,
                                        TXT_IDLE_SHORT_THURSDAY,
                                        TXT_IDLE_SHORT_FRIDAY,
                                        TXT_IDLE_SHORT_SATURDAY};
 
    GUISTR_INFO_T    str_info={0};

    /* Modify by michael on 3/2/2012 for NEWMS00161908 : Layer 有效性检查(避免Lose focus数据消失的处理方式)*/
    GUI_LCD_DEV_INFO layer = {0};
    if (PNULL ==s_kl_info_ptr)
    {
        return;
    }
    layer = GetLayerDG(&s_kl_info_ptr->title_layer_handle);
    
    if (UILAYER_IsLayerActive(&layer))
    {
        clock_rect.left  = time_rect.left;
        clock_rect.top = time_rect.top;
        clock_rect.bottom =  week_rect.bottom;
        clock_rect.right = week_rect.right;		
        UILAYER_ClearRect(&layer, clock_rect);
    }
    /************************Modify over*************************/
    TM_GetSysDate(&sys_date);
    
    time_str.wstr_ptr = (wchar*)SCI_ALLOCA((DATE_STR_LEN+1)*sizeof(wchar));
    if(PNULL == time_str.wstr_ptr)
    {
        return;
    }
    SCI_MEMSET(time_str.wstr_ptr,0,((DATE_STR_LEN+1)*sizeof(wchar)));  
    date_str.wstr_ptr = (wchar*)SCI_ALLOCA((DATE_STR_LEN+1)*sizeof(wchar));
    if(date_str.wstr_ptr != PNULL)
    {
        SCI_MEMSET(date_str.wstr_ptr,0,((DATE_STR_LEN+1)*sizeof(wchar)));  
        
        //get time type
        MAIN_StatusbarGetSysTime(time_str.wstr_ptr, (DATE_STR_LEN - 1));
        time_str.wstr_len = MMIAPICOM_Wstrlen(time_str.wstr_ptr);

        SCI_MEMSET ((time_str.wstr_ptr)+5, 0, (DATE_STR_LEN - 1 -5));
        
        GetClockTextStyle(&time_text_style, &date_text_style, &week_text_style);
        //显示时间
        GUISTR_GetStringInfo(&time_text_style, &time_str, text_state,&str_info);
        
        /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&layer,
            &time_rect,
            &time_rect,
            (const MMI_STRING_T     *)&time_str,
            &time_text_style,
            text_state,
            0
            );
        //显示时间类型:
        time_type = MMIAPISET_GetTimeDisplayType();
        TM_GetSysTime(&time);
        
        if(time_type == MMISET_TIME_12HOURS)
        {
            if ((ACLOCK12_TIME > time.hour))//AM
            {
                MMI_GetLabelTextByLang(TXT_KL_TIME_AM, &time_type_str);
            }
            else//PM
            {
                MMI_GetLabelTextByLang(TXT_TIME_PM, &time_type_str);
            }
            time_text_style.font = SONG_FONT_16;
            time_text_style.align = ALIGN_BOTTOM;
            time_rect.left += str_info.width;
            
            /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)&layer,
                &time_rect,
                &time_rect,
                (const MMI_STRING_T     *)&time_type_str,
                &time_text_style,
                text_state,
                0
                );
        }
            
        // 显示date：xx-xx
        MMIAPISET_FormatDateStrByDateStyle(sys_date.year, sys_date.mon, sys_date.mday,
            ' ', (uint8*)u8_date_str, sizeof(u8_date_str) - 1);
        MMIAPICOM_StrToWstr((const uint8*)u8_date_str, date_str.wstr_ptr);  
        date_str.wstr_len = MMIAPICOM_Wstrlen(date_str.wstr_ptr);

        GUISTR_GetStringInfo(&date_text_style, &date_str, text_state, &str_info);
        /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&layer,
            &date_rect,
            &date_rect,
            (const MMI_STRING_T     *)&date_str,
            &date_text_style,
            text_state,
            0
            );
        
        //display week
        week_rect.left = date_rect.left+str_info.width+5;
        MMI_GetLabelTextByLang(week_text[sys_date.wday], &week_str);
        GUISTR_GetStringInfo(&date_text_style, &week_str, text_state, &str_info);
        week_rect.right = week_rect.left+str_info.width;
        /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&layer,
            &week_rect,
            &week_rect,
            (const MMI_STRING_T     *)&week_str,
            &date_text_style,
            text_state,
            0
            );
    }
    SCI_FREE(time_str.wstr_ptr);
    SCI_FREE(date_str.wstr_ptr);
}

///*****************************************************************************/
////    Description :在解锁窗口上显示未接来电及未读短信数目
////    Global resource dependence : 
////  Author:
////  Date:
///*****************************************************************************/
//LOCAL void DrawKLPatternEvent(MMI_WIN_ID_T win_id)
//{
//    uint32  miss_call = 0;
//    uint32  unread_sms =0;
//    uint8    str_len = 0;
//    wchar  miss_call_wstr[KEYLOCK_COUNT_STRING_SIZE*8]  = {0};
//    wchar  unread_message_wstr[KEYLOCK_COUNT_STRING_SIZE*8]  = {0};
//    char     count_str[KEYLOCK_COUNT_STRING_SIZE*8] = {0};
//    MMI_STRING_T        event_string = {0};
//    MMI_STRING_T        miss_call_text = {0};
//    MMI_STRING_T        unread_message_text = {0};
//    
//    GUISTR_STYLE_T event_text_style = {0};
//    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
//
//    GUI_RECT_T    event_rect = {0};
//
//    miss_call = MMIAPICC_GetMissedCallNumber();
//    unread_sms = MMIAPISMS_GetTobeReadMessageCount(MN_DUAL_SYS_MAX) + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX);
//
//    event_string.wstr_ptr = (wchar*)SCI_ALLOCA((KEYLOCK_COUNT_STRING_SIZE*8+1)*sizeof(wchar));
//    if(PNULL == event_string.wstr_ptr)
//    {
//        return;
//    }
//    SCI_MEMSET(event_string.wstr_ptr,0,((KEYLOCK_COUNT_STRING_SIZE*8+1)*sizeof(wchar)));  
//
//    if(0 != miss_call)
//    {
//        MMI_GetLabelTextByLang(TXT_KL_MISS_CALLS, &miss_call_text);
//        sprintf(count_str,"%d ",miss_call);
//        str_len = strlen(count_str);
//        MMI_STRNTOWSTR(miss_call_wstr, KEYLOCK_COUNT_STRING_SIZE*8, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE*8,str_len);
//        if (KEYLOCK_COUNT_STRING_SIZE*8 > str_len + miss_call_text.wstr_len +1)
//        {
//            MMIAPICOM_Wstrncpy(miss_call_wstr +str_len, miss_call_text.wstr_ptr , miss_call_text.wstr_len);
//        }
//
//        MMIAPICOM_Wstrncpy(event_string.wstr_ptr,  miss_call_wstr, miss_call_text.wstr_len+ str_len +1);
//        event_string.wstr_len = str_len + miss_call_text.wstr_len;
//    }
//    if(0 != unread_sms)
//    {
//        SCI_MEMSET(count_str, 0, KEYLOCK_COUNT_STRING_SIZE*8);
//        MMI_GetLabelTextByLang(TXT_KL_UNREAD_MESSAGES, &unread_message_text);
//        
//        sprintf(count_str,"%d ",unread_sms);
//        str_len = strlen(count_str);
//        
//        MMI_STRNTOWSTR(unread_message_wstr, KEYLOCK_COUNT_STRING_SIZE*8, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE*8,str_len);
//        
//        if (KEYLOCK_COUNT_STRING_SIZE*8 > str_len + unread_message_text.wstr_len +1)
//        {
//            MMIAPICOM_Wstrncpy(unread_message_wstr +str_len, unread_message_text.wstr_ptr , unread_message_text.wstr_len);
//        }
//
//        if(0 != miss_call)
//        {
//            MMIAPICOM_Wstrncpy(event_string.wstr_ptr + event_string.wstr_len, unread_message_wstr, unread_message_text.wstr_len + str_len +1);
//            event_string.wstr_len =event_string.wstr_len + unread_message_text.wstr_len + str_len;
//        }
//        else
//        {
//            MMIAPICOM_Wstrncpy(event_string.wstr_ptr,  unread_message_wstr, unread_message_text.wstr_len+ str_len +1);
//            event_string.wstr_len = unread_message_text.wstr_len + str_len;
//        }
//    }
//
//    if ((0 == miss_call) && (0 == unread_sms))
//    {
//        SCI_FREE(event_string.wstr_ptr);
//        return;
//    }
//
//#if defined(MAINLCD_SIZE_240X320)
//        event_rect.left = 120;
//        event_rect.right = 240;
//        event_rect.bottom = 56;
//        event_rect.top = 32;
//#elif defined(MAINLCD_SIZE_240X400)
//        event_rect.left = 16;
//        event_rect.right = 240;
//        event_rect.bottom = KL_PATTERN_TITLE_EVENT_HEIGHT;//KL_PATTERN_TITLE_LINE_HEIGHT;
//        event_rect.top = KL_PATTERN_TITLE_LINE_HEIGHT;//KL_PATTERN_TITLE_EVENT_HEIGHT;
//#elif defined(MAINLCD_SIZE_320X480)
//        event_rect.left = 16;
//        event_rect.right = 240;
//        event_rect.bottom = KL_PATTERN_TITLE_EVENT_HEIGHT;
//        event_rect.top = KL_PATTERN_TITLE_LINE_HEIGHT;
//#endif
//
//        event_text_style.effect = FONT_EFFECT_NONE; 
//        event_text_style.angle = ANGLE_0;
//        event_text_style.align = ALIGN_VMIDDLE;
//        event_text_style.font = SONG_FONT_16;
//        event_text_style.font_color = MMI_WHITE_COLOR;
//
//        if (UILAYER_IsLayerActive(&s_kl_info_ptr->title_layer_handle))
//        {
//            UILAYER_ClearRect(&s_kl_info_ptr->title_layer_handle, event_rect);
//        }
//        
//        GUISTR_DrawTextToLCDInRect(
//            (const GUI_LCD_DEV_INFO *)&s_kl_info_ptr->title_layer_handle,
//            &event_rect,
//            &event_rect,
//            (const MMI_STRING_T     *)&event_string,
//            &event_text_style,
//            text_state,
//            0
//            );
//        SCI_FREE(event_string.wstr_ptr);
// }

/*****************************************************************************/
//  Description :设置图形宫格点的位置
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void SetKeyLockPatternDispRect(MMI_WIN_ID_T win_id)
{
    uint32 i=0;
    GUI_POINT_T  center_rect[9] ={0};
    
    if((MMIKL_KL_PASSWARD_UNLOCK_WIN_ID != win_id))
    {
        GUI_POINT_T por_rect[9] ={{MMI_MAINSCREEN_WIDTH/2-MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2-MMI_KL_PASSWARD_POINT_WIDTH+SETTING_LOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2, MMI_MAINSCREEN_HEIGHT/2-MMI_KL_PASSWARD_POINT_WIDTH+SETTING_LOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2+MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2-MMI_KL_PASSWARD_POINT_WIDTH+SETTING_LOCK_DOT_Y_OFFSET},
              {MMI_MAINSCREEN_WIDTH/2-MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2+SETTING_LOCK_DOT_Y_OFFSET}, {MMI_MAINSCREEN_WIDTH/2,MMI_MAINSCREEN_HEIGHT/2+SETTING_LOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2+MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2+SETTING_LOCK_DOT_Y_OFFSET},
              {MMI_MAINSCREEN_WIDTH/2-MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2+MMI_KL_PASSWARD_POINT_WIDTH+SETTING_LOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2, MMI_MAINSCREEN_HEIGHT/2+MMI_KL_PASSWARD_POINT_WIDTH+SETTING_LOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2+MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2+MMI_KL_PASSWARD_POINT_WIDTH+SETTING_LOCK_DOT_Y_OFFSET}};
        SCI_MEMCPY(center_rect, por_rect, sizeof(por_rect));
    }
    else
    {
        GUI_POINT_T por_rect[9] ={{MMI_MAINSCREEN_WIDTH/2-MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2-MMI_KL_PASSWARD_POINT_WIDTH+ UNLOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2, MMI_MAINSCREEN_HEIGHT/2-MMI_KL_PASSWARD_POINT_WIDTH+ UNLOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2+MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2-MMI_KL_PASSWARD_POINT_WIDTH+ UNLOCK_DOT_Y_OFFSET},
                  {MMI_MAINSCREEN_WIDTH/2-MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2+ UNLOCK_DOT_Y_OFFSET}, {MMI_MAINSCREEN_WIDTH/2,MMI_MAINSCREEN_HEIGHT/2+ UNLOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2+MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2+ UNLOCK_DOT_Y_OFFSET},
                  {MMI_MAINSCREEN_WIDTH/2-MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2+MMI_KL_PASSWARD_POINT_WIDTH+ UNLOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2, MMI_MAINSCREEN_HEIGHT/2+MMI_KL_PASSWARD_POINT_WIDTH+ UNLOCK_DOT_Y_OFFSET},{MMI_MAINSCREEN_WIDTH/2+MMI_KL_PASSWARD_POINT_WIDTH, MMI_MAINSCREEN_HEIGHT/2+MMI_KL_PASSWARD_POINT_WIDTH+ UNLOCK_DOT_Y_OFFSET}};
        SCI_MEMCPY(center_rect, por_rect, sizeof(por_rect));
    }
    if (PNULL != s_kl_info_ptr)
    {
        for(i=0; i< sizeof(s_kl_info_ptr->st_passward_point)/sizeof(GUI_POINT_T); i++)
        {
            SCI_MEMCPY(&s_kl_info_ptr->st_passward_point[i], &center_rect[i], sizeof(GUI_POINT_T));
        }
    }

}

/*****************************************************************************/
//  Description :显示图片
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void DispDotImg(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO dev_info_ptr,  GUI_POINT_T  cur_point, MMI_IMAGE_ID_T img_id)
{
    uint16    width = 0,height = 0;
    /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
    GUI_LCD_DEV_INFO layer = {0};
    layer = GetLayerDG(&dev_info_ptr);
    
    if(UILAYER_IsLayerActive(&layer))
    {    
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
        cur_point.x -=width/2;
        cur_point.y -=height/2;
        
        GUIRES_DisplayImg(
            &cur_point,
            PNULL, 
            PNULL,
            win_id,
            img_id,
            &layer
            );
        /************************Modify over*************************/
    }
}

/*****************************************************************************/
//  Description :显示图形密码窗口的背景
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void DisplayKLPatternBG(MMI_WIN_ID_T win_id, MMI_IMAGE_ID_T img_id)
{
    
    int32 i =0;
    int32 sizeRatio = 0;
    GUI_POINT_T point ={0};
    uint16    bg_width = 0,bg_height = 0;
    uint16    line_y   = 0;
    /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
    GUI_LCD_DEV_INFO layer = {0};
    
    if (PNULL == s_kl_info_ptr)
    {
        return ;
    }
    sizeRatio = sizeof(s_kl_info_ptr->st_passward_point)/sizeof(GUI_POINT_T);
    
    layer = GetLayerDG(&s_kl_info_ptr->title_layer_handle);
    /************************Modify over*************************/
    
    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, img_id, win_id);
    
    for(i=0; i < sizeRatio; i++)
    {
        point.x = s_kl_info_ptr->st_passward_point[i].x -bg_width/2;
        point.y = s_kl_info_ptr->st_passward_point[i].y -bg_height/2;
        
        /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
        DispDotImg(win_id, layer, s_kl_info_ptr->st_passward_point[i], img_id);
        
    }
    
    if(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
    {
        line_y = KL_PATTERN_TITLE_LINE_HEIGHT + 20;
        
        /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
        LCD_DrawLayerLine(&layer, 4, line_y, MMITHEME_GetMainScreenRightPixel() - 4, line_y, MMI_DARK_GRAY_COLOR , 1);
        
    }
    
}

/*****************************************************************************/
//  Description :显示窗口上面标题及下面触摸按键
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void DisplayKLPatternTitleAndSK(MMI_WIN_ID_T win_id, MMI_KL_KEY_STATE_E key_state, 
            MMI_KL_WIN_TYPE_E win_type, BOOLEAN is_right)
{
    MMI_STRING_T        title = {0};
    GUISTR_STYLE_T     str_style = {0};
    uint16 title_height = 0;

    MMI_STRING_T   left_softkey ={0};
    MMI_STRING_T   middle_softkey ={0};
    
    GUISTR_STATE_T     text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;

    GUI_POINT_T  DispPoint= {0};
    
    GUI_RECT_T    title_rect = MMITHEME_GetFullScreenRect();
    
#ifndef USE_SOFTKEY_BUTTON                
    uint8 light =0;
    uint32 i=0;
    uint16 tips_color =MMI_WHITE_COLOR;
    GUI_RECT_T    tips_bg_rect =KL_PATTERN_TIPS_BG_RECT;
    GUI_RECT_T    tips_left_rect= KL_PATTERN_TIPS_LEFT_RECT;
    GUI_RECT_T    tips_right_rect= KL_PATTERN_TIPS_RIGHT_RECT;
#endif
    
    GUI_RECT_T    tips_center_rect= KL_PATTERN_TIPS_CENTER_RECT;
    MMI_TEXT_ID_T lsk_text_id = 0, rsk_text_id = 0;
    
    /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
    GUI_LCD_DEV_INFO layer = {0};
    if (PNULL == s_kl_info_ptr)
    {
        return;
    }

    layer = GetLayerDG(&s_kl_info_ptr->title_layer_handle);
    /************************Modify over*************************/
    
    str_style.angle = ANGLE_0;
    str_style.align = ALIGN_HVMIDDLE;
    str_style.font = SONG_FONT_16;
    str_style.font_color = MMI_WHITE_COLOR;

    title_height = GUIFONT_GetHeight(str_style.font);

    if(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
    {
        title_rect.top = KL_PATTERN_TITLE_LINE_HEIGHT + 1;
        title_rect.bottom =title_rect.top + title_height;
    }
    else
    {
        title_rect.top += (MMI_TITLE_THEME_HEIGHT + MMI_STATUSBAR_HEIGHT) + 1;
        title_rect.bottom =title_rect.top + title_height;
    }

    if (MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
    {
        MMI_GetLabelTextByLang(TXT_KL_PSW_EMERGENCY_CALL, &middle_softkey);

        switch(key_state)
        {
        case KL_KEY_STATE_NONE:
        case KL_KEY_STATE_DOWN:
        case KL_KEY_STATE_MOVE:
            //Title:Confirm saved pattern
            MMI_GetLabelTextByLang(TXT_KL_CONFIRM_SAVED_PATTERN, &title);
            break;
            
        case KL_KEY_STATE_UP:
            //Title:Sorry,try again
            MMI_GetLabelTextByLang(TXT_KL_ERROR_TRY_AGAIN, &title);
            break;
            
        default:
            break;
        }

    }
    else
    {
        switch(key_state)
        {
        case KL_KEY_STATE_NONE:
            if (KL_WIN_TYPE_SET == win_type)
            {
                //Draw an unlock pattern
                //Title:Draw an unlock pattern   softkey:Cancel(light)  Continue(dim)
                MMI_GetLabelTextByLang(STXT_CANCEL, &left_softkey);
                MMI_GetLabelTextByLang(TXT_COMMON_COMMON_CONTINUE, &middle_softkey);
                MMI_GetLabelTextByLang(TXT_KL_DRAW_AN_UNLOCK_PATTERN, &title);
                s_kl_info_ptr->tips_type = KL_KEY_TIPS_LIGHT_DIM;
                s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_LEFT_BLOCK].msg_type = KL_KEY_TIPS_MSG_CLOSE;
                s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_RIGHT_BLOCK].msg_type = KL_KEY_TIPS_MSG_NONE;
            }
            else
            {
                //Title:Draw pattern again to confirm
                //softkey:Cancel(dim)  Continue(dim)
                MMI_GetLabelTextByLang(STXT_CANCEL, &left_softkey);
                MMI_GetLabelTextByLang(TXT_COMMON_COMMON_CONTINUE, &middle_softkey);
                MMI_GetLabelTextByLang(TXT_KL_DRAW_PATTERN_AGAIN_TO_CONFIRM, &title);
                s_kl_info_ptr->tips_type = KL_KEY_TIPS_DIM_DIM;
                s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_LEFT_BLOCK].msg_type = KL_KEY_TIPS_MSG_NONE;
                s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_RIGHT_BLOCK].msg_type = KL_KEY_TIPS_MSG_NONE;
            }
            lsk_text_id = STXT_CANCEL;
            rsk_text_id = TXT_COMMON_COMMON_CONTINUE;            
            break;
            
        case KL_KEY_STATE_DOWN:
        case KL_KEY_STATE_MOVE:
            MMI_GetLabelTextByLang(STXT_CANCEL, &left_softkey);
            MMI_GetLabelTextByLang(TXT_COMMON_COMMON_CONTINUE, &middle_softkey);
            lsk_text_id = STXT_CANCEL;
            rsk_text_id = TXT_COMMON_COMMON_CONTINUE;
            
            MMI_GetLabelTextByLang(TXT_KL_RELEASE_FINGER_WHEN_DONE, &title);
            s_kl_info_ptr->tips_type = KL_KEY_TIPS_DIM_DIM;
            s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_LEFT_BLOCK].msg_type = KL_KEY_TIPS_MSG_NONE;
            s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_RIGHT_BLOCK].msg_type = KL_KEY_TIPS_MSG_NONE;
            break;
            
        case KL_KEY_STATE_UP:
            if (KL_WIN_TYPE_SET == win_type)
            {
                if (is_right)
                {
                    MMI_GetLabelTextByLang(TXT_KL_PATTERN_RECORDED, &title);
                    MMI_GetLabelTextByLang(TXT_COMMON_COMMON_CONTINUE, &middle_softkey);
                    s_kl_info_ptr->tips_type = KL_KEY_TIPS_LIGHT_LIGHT;
                    s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_LEFT_BLOCK].msg_type = KL_KEY_TIPS_MSG_FULL_PAINT;
                    s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_RIGHT_BLOCK].msg_type = KL_KEY_TIPS_MSG_SET2CONFIRM;
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_KL_AT_LEAST_FOUR_DOTS, &title);
                    MMI_GetLabelTextByLang(TXT_COMMON_COMMON_CONTINUE, &middle_softkey);
                    s_kl_info_ptr->tips_type = KL_KEY_TIPS_LIGHT_DIM;
                    s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_LEFT_BLOCK].msg_type = KL_KEY_TIPS_MSG_FULL_PAINT;
                    s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_RIGHT_BLOCK].msg_type = KL_KEY_TIPS_MSG_NONE;
                }
                MMI_GetLabelTextByLang(TXT_CMSBRW_RETRY, &left_softkey);
                lsk_text_id = TXT_CMSBRW_RETRY;
                rsk_text_id = TXT_COMMON_COMMON_CONTINUE;
            }
            else
            {
                if(is_right)
                {
                    MMI_GetLabelTextByLang(TXT_KL_YOUR_NEW_UNLOCK_PATTERN, &title);
                    MMI_GetLabelTextByLang(TXT_COMMON_OK, &middle_softkey);
                    s_kl_info_ptr->tips_type = KL_KEY_TIPS_LIGHT_LIGHT;
                    s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_LEFT_BLOCK].msg_type = KL_KEY_TIPS_MSG_CLOSE;
                    s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_RIGHT_BLOCK].msg_type = KL_KEY_TIPS_MSG_SAVED;
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_KL_ERROR_TRY_AGAIN, &title);
                    MMI_GetLabelTextByLang(TXT_COMMON_OK, &middle_softkey);
                    s_kl_info_ptr->tips_type = KL_KEY_TIPS_LIGHT_DIM;
                    s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_LEFT_BLOCK].msg_type = KL_KEY_TIPS_MSG_CLOSE;
                    s_kl_info_ptr->tips_info[KL_PATTERN_TIPS_RIGHT_BLOCK].msg_type = KL_KEY_TIPS_MSG_NONE;
                }
                MMI_GetLabelTextByLang(STXT_CANCEL, &left_softkey);
                lsk_text_id = STXT_CANCEL;
                rsk_text_id = TXT_COMMON_OK;                
            }
            break;
            
        default:

            break;
        }
    }
    
    //画Title
    //* Modify by michael on 3/2/2012 for NEWMS00161908  : */
    if(UILAYER_IsLayerActive(&layer) )
    {
        if (MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
        {
            DisplayClock(win_id);
            UILAYER_ClearRect(&layer, title_rect);
            GUISTR_DrawTextToLCDInRect(&layer, &title_rect, &title_rect, &title, &str_style, text_state, GUISTR_TEXT_DIR_AUTO);
        }
        else
        {
            UILAYER_ClearRect(&layer, title_rect);
            str_style.font_color = MMI_BLACK_COLOR;
            GUISTR_DrawTextToLCDInRect(&layer, &title_rect, &title_rect, &title, &str_style, text_state, GUISTR_TEXT_DIR_AUTO);
        }
    }
    //画TIPS 触摸
    if ((MMIKL_KL_PASSWARD_UNLOCK_WIN_ID != win_id))
    {
#ifndef USE_SOFTKEY_BUTTON            
        str_style.font_color = MMI_BLACK_COLOR;      
        GUI_FillRect(&layer, tips_bg_rect, MMI_DARK_GRAY_COLOR);
        for(i=KL_PATTERN_TIPS_LEFT_BLOCK; i< KL_PATTERN_TIPS_BLOCK_MAX; i++)
        {
            light =(s_kl_info_ptr->tips_type >> i)&0x01;
            if (light)
            {
                tips_color = MMI_WHITE_COLOR;
            }
            else
            {
                tips_color = MMI_DARK_GRAY_COLOR;
            }
            GUI_FillRect(&layer, tips_left_rect, tips_color);
            if(UILAYER_IsLayerActive(&layer))
            {
                GUISTR_DrawTextToLCDInRect(&layer, &tips_left_rect, &tips_left_rect, &left_softkey, &str_style, text_state, GUISTR_TEXT_DIR_AUTO);
            }
            left_softkey =middle_softkey;
            tips_left_rect.left =tips_right_rect.left;
            tips_left_rect.right =tips_right_rect.right;
        }
#else
        GUISOFTKEY_SetButtonState(win_id, MMITHEME_GetSoftkeyCtrlId(), 0, (BOOLEAN)(!((s_kl_info_ptr->tips_type)&0x01)), FALSE);
        GUISOFTKEY_SetButtonState(win_id, MMITHEME_GetSoftkeyCtrlId(), 2, (BOOLEAN)(!((s_kl_info_ptr->tips_type>>1)&0x01)), FALSE);
        GUISOFTKEY_SetTextId(win_id, MMITHEME_GetSoftkeyCtrlId(), lsk_text_id, 0, rsk_text_id, TRUE);
        
        //@CR NEWMS00180020 leon.wang 2012/3/21
        //Set the Title Button Text in Iphone Style
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, rsk_text_id, FALSE);
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);
#endif
    }
    else
    {
        str_style.font_color = MMI_BLACK_COLOR;
        
        DispPoint.x =MMI_MAINSCREEN_WIDTH/2;
        DispPoint.y =(tips_center_rect.top + tips_center_rect.bottom)/2;
        DispDotImg(win_id, layer,  DispPoint, IMAGE_PATTERN_TIPS_BG);
        if(UILAYER_IsLayerActive(&layer))
        {
            GUISTR_DrawTextToLCDInRect(&layer, &tips_center_rect, &tips_center_rect, &middle_softkey, &str_style, text_state, GUISTR_TEXT_DIR_AUTO);
        }
    }
    /************************Modify over*************************/
}
/*****************************************************************************/
//  Description :lose foces display red color                     
//  Global resource dependence :
//  Author:Michael wang
//  Date: 2012.3.2 for NEWMS00161908 (6530)
/*****************************************************************************/
LOCAL void DisplayWallpaperColor(MMI_WIN_ID_T    win_id)
{
       GUI_RECT_T win_rect = {0};
	   GUI_LCD_DEV_INFO layer = {0};
       if (PNULL == s_kl_info_ptr)
       {
           return;
       }
       layer = GetLayerDG(&s_kl_info_ptr->wallpaper_layer);
       MMK_GetWinRect(win_id, &win_rect);
       win_rect.top +=  MMI_STATUSBAR_HEIGHT; 
       GUI_FillRect(&layer, win_rect, MMI_RED_COLOR);
}
/*****************************************************************************/
//  Description :Update drawgraphic info       
//  Global resource dependence :
//  Author:Michael wang
//  Date: 2012.3.2 for NEWMS00161908 (6530)
/*****************************************************************************/
LOCAL void UpdateDG(MMI_WIN_ID_T    win_id)
{	      
    if (PNULL != s_kl_info_ptr)
    {
        DisplayKLPatternBG(win_id, IMAGE_KEYLOCK_BG_DOT);
        s_kl_info_ptr->input_result =FALSE;
        DisplayKLPatternTitleAndSK(win_id, s_kl_info_ptr->key_state, s_kl_info_ptr->win_type, s_kl_info_ptr->input_result);        
    }        
}

/*****************************************************************************/
//  Description :追加所有图层到图层队列
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void BltAllLayer(KEYLOCK_INFO_T *keylock_info_ptr)
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (PNULL == keylock_info_ptr)
    {
        return;
    }

    if (UILAYER_IsLayerActive(&keylock_info_ptr->wallpaper_layer))
    {
        append_layer.lcd_dev_info = keylock_info_ptr->wallpaper_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    if (UILAYER_IsLayerActive(&keylock_info_ptr->slide_layer_handle))
    {
        append_layer.lcd_dev_info = keylock_info_ptr->slide_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    if (UILAYER_IsLayerActive(&keylock_info_ptr->title_layer_handle))
    {
        append_layer.lcd_dev_info = keylock_info_ptr->title_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    if (UILAYER_IsLayerActive(&keylock_info_ptr->image_confirm_layer_handle))
    {
        append_layer.lcd_dev_info = keylock_info_ptr->image_confirm_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }
}

/*****************************************************************************/
//  Description :清除所有图层上的数据
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void ClearAllLayer(void)
{
    
    if (PNULL == s_kl_info_ptr)
    {
        return;
    }

    if(UILAYER_IsLayerActive(&s_kl_info_ptr->slide_layer_handle))
    {
        
        UILAYER_Clear(&s_kl_info_ptr->slide_layer_handle);
        UILAYER_RemoveBltLayer(&s_kl_info_ptr->slide_layer_handle);
    }
    
    if(UILAYER_IsLayerActive(&s_kl_info_ptr->image_confirm_layer_handle))
    {
        UILAYER_Clear(&s_kl_info_ptr->image_confirm_layer_handle);
        UILAYER_RemoveBltLayer(&s_kl_info_ptr->image_confirm_layer_handle);
    }
    
}

/*****************************************************************************/
//  Description :窗口上创建的所有图层
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void CreateAllLayer(MMI_WIN_ID_T win_id)
{
    CreatKLPasswardTitleLayer(win_id);

  //  CreatKLwallpaperLayer(win_id);
    
    //解锁画图不可视时，只保留创建title_layer_handle图层
    if((MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)&&(s_kl_info_ptr !=PNULL && !s_kl_info_ptr->s_is_visible_passward))
    {
        return;
    }
    
    CreatKLPasswardSlideLayer(win_id);
    CreatKLPasswardConfirmLayer(win_id);
}

/*****************************************************************************/
//  Description :销毁当前窗口上创建的所有图层
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void DestroyAllLayer(KEYLOCK_INFO_T *keylock_info_ptr)
{
    if (PNULL == keylock_info_ptr)
    {
        return;
    }

    if ((UILAYER_NULL_HANDLE != keylock_info_ptr->wallpaper_layer.block_id))
       // && (UILAYER_MAIN_HANDLE != keylock_info_ptr->wallpaper_layer.block_id))
    {
        UILAYER_RELEASELAYER(&keylock_info_ptr->wallpaper_layer);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&keylock_info_ptr->wallpaper_layer);
//        keylock_info_ptr->wallpaper_layer.block_id = UILAYER_NULL_HANDLE;
    }

    if ((UILAYER_NULL_HANDLE != keylock_info_ptr->image_confirm_layer_handle.block_id))
       //&& (UILAYER_MAIN_HANDLE != keylock_info_ptr->image_confirm_layer_handle.block_id))
    {
        UILAYER_RELEASELAYER(&keylock_info_ptr->image_confirm_layer_handle);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&keylock_info_ptr->image_confirm_layer_handle);
//        keylock_info_ptr->image_confirm_layer_handle.block_id = UILAYER_NULL_HANDLE;
    }

    if ((UILAYER_NULL_HANDLE != keylock_info_ptr->slide_layer_handle.block_id)
        //&& (UILAYER_MAIN_HANDLE != keylock_info_ptr->slide_layer_handle.block_id)
        )
    {
        UILAYER_RELEASELAYER(&keylock_info_ptr->slide_layer_handle);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&keylock_info_ptr->slide_layer_handle);
//        keylock_info_ptr->slide_layer_handle.block_id = UILAYER_NULL_HANDLE;
    }
    if ((UILAYER_NULL_HANDLE != keylock_info_ptr->title_layer_handle.block_id))
       //&& (UILAYER_MAIN_HANDLE != keylock_info_ptr->title_layer_handle.block_id))
    {
        UILAYER_RELEASELAYER(&keylock_info_ptr->title_layer_handle);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&keylock_info_ptr->title_layer_handle);
//        keylock_info_ptr->title_layer_handle.block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : Start pattern key lock tips timer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void StartKLPatternTipsTimer(MMI_WIN_ID_T win_id)
{
    if (PNULL != s_kl_info_ptr && 0 == s_kl_info_ptr->KL_pattern_tips_timer_id)
    {
        s_kl_info_ptr->KL_pattern_tips_timer_id = MMK_CreateWinTimer(win_id,ANDROID_UNLOCK_CONFIRM_TIME,FALSE);
    }
}

/*****************************************************************************/
//  Description :Stop pattern key lock tips timer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL BOOLEAN StopKLPatternTipsTimer(MMI_WIN_ID_T win_id)
{
    BOOLEAN     result  =   FALSE;

    if (PNULL !=s_kl_info_ptr && 0 < s_kl_info_ptr->KL_pattern_tips_timer_id)
    {
        MMK_StopTimer(s_kl_info_ptr->KL_pattern_tips_timer_id);
        s_kl_info_ptr->KL_pattern_tips_timer_id = 0;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description :Start pattern key lock OK timer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void StartKLPatternOKTimer(MMI_WIN_ID_T win_id)
{
    if (PNULL != s_kl_info_ptr && 0 == s_kl_info_ptr->KL_pattern_ok_timer_id)
    {
        s_kl_info_ptr->KL_pattern_ok_timer_id = MMK_CreateWinTimer(win_id,ANDROID_UNLOCK_CONFIRM_TIME,FALSE);
    }
}

/*****************************************************************************/
//  Description :Stop pattern key lock OK timer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL BOOLEAN StopKLPatternOKTimer(MMI_WIN_ID_T win_id)
{
    BOOLEAN     result  =   FALSE;

    if (PNULL !=s_kl_info_ptr && 0 < s_kl_info_ptr->KL_pattern_ok_timer_id)
    {
        MMK_StopTimer(s_kl_info_ptr->KL_pattern_ok_timer_id);
        s_kl_info_ptr->KL_pattern_ok_timer_id = 0;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Start pattern key lock tp move timer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void StartKLPatternTPMoveTimer(MMI_WIN_ID_T win_id)
{
    if (PNULL != s_kl_info_ptr && 0 == s_kl_info_ptr->KL_pattern_tp_move_id)
    {
        s_kl_info_ptr->KL_pattern_tp_move_id = MMK_CreateWinTimer(win_id,MMI_KL_PATTERN_TP_MOVE_TIMER,FALSE);
    }
}

/*****************************************************************************/
//  Description :Stop pattern key lock tp move timer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL BOOLEAN StopKLPatternTPMoveTimer(MMI_WIN_ID_T win_id)
{
    BOOLEAN     result  =   FALSE;

    if (PNULL !=s_kl_info_ptr && 0 < s_kl_info_ptr->KL_pattern_tp_move_id)
    {
        MMK_StopTimer(s_kl_info_ptr->KL_pattern_tp_move_id);
        s_kl_info_ptr->KL_pattern_tp_move_id = 0;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Restart pattern key lock tp move timer.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void RestartKLPatternTPMoveTimer(MMI_WIN_ID_T win_id)
{
    StopKLPatternTPMoveTimer(win_id);
    StartKLPatternTPMoveTimer(win_id);
}

/*****************************************************************************/
//  Description :处理响应对应的TIPS中的消息
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void HandlekLPatternTipsMsg(MMI_WIN_ID_T win_id, MMI_KL_PATTERN_TIPS_BLOCK_E index)
{

    if (PNULL != s_kl_info_ptr)
    {
        switch(s_kl_info_ptr->tips_info[index].msg_type)
        {
        case KL_KEY_TIPS_MSG_FULL_PAINT:
            SCI_MEMSET(s_kl_info_ptr->new_point_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            s_kl_info_ptr->input_result = FALSE;
            s_kl_info_ptr->key_state = KL_KEY_STATE_NONE;
            ClearAllLayer();
            break;
    
        case KL_KEY_TIPS_MSG_SET2CONFIRM:
            s_kl_info_ptr->win_type =KL_WIN_TYPE_CONFIRM;
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            s_kl_info_ptr->input_result = FALSE;
            s_kl_info_ptr->key_state = KL_KEY_STATE_NONE;
            ClearAllLayer();
            break;

        case KL_KEY_TIPS_MSG_SAVED:
            MMIAPISET_SetKeyLockPassward(s_kl_info_ptr->new_point_ptr);
            MMIAPISET_SetEnableKeyLock(TRUE);
            MMIDEFAULT_StartAutoKeylockTimer();
            s_kl_info_ptr->input_result = FALSE;
            s_kl_info_ptr->key_state = KL_KEY_STATE_NONE;
            ClearAllLayer();
            MMK_CloseWin(win_id);

            if (MMIKL_IsPreviewState())
            {
                MMIAPISET_OpenKeyLockEffect5SubWin();
            }//MMK_PostMsg(win_id, MSG_CLOSE_WINDOW, PNULL, 0);
            break;

        case KL_KEY_TIPS_MSG_CLOSE:
            s_kl_info_ptr->input_result = FALSE;
            s_kl_info_ptr->key_state = KL_KEY_STATE_NONE;
            ClearAllLayer();
            MMK_CloseWin(win_id);//MMK_PostMsg(win_id, MSG_CLOSE_WINDOW, PNULL, 0);
            break;

        default:
            break;
        }
    

    }
}

/*****************************************************************************/
//  Description :在开始点中TIPS处理
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL BOOLEAN HandleKLPatternTipsStart(MMI_WIN_ID_T win_id, GUI_POINT_T cur_point)
{
    MMI_KL_PATTERN_TIPS_BLOCK_E i =KL_PATTERN_TIPS_LEFT_BLOCK;
    MMI_KL_PATTERN_TIPS_BLOCK_E  block =KL_PATTERN_TIPS_LEFT_BLOCK;
    GUI_RECT_T tips_left_rect = KL_PATTERN_TIPS_LEFT_RECT;
    GUI_RECT_T tips_right_rect = KL_PATTERN_TIPS_RIGHT_RECT;
    GUI_RECT_T tips_center_rect = KL_PATTERN_TIPS_CENTER_RECT;
    GUI_RECT_T  both_rect = {0};
    if (PNULL == s_kl_info_ptr)
    {
        return FALSE;
    }
    if(MMIKL_SET_KL_PASSWARD_WIN_ID == win_id)
    {
        for(i = KL_PATTERN_TIPS_LEFT_BLOCK; i < KL_PATTERN_TIPS_BLOCK_MAX; i++)
        {
            both_rect.left = tips_left_rect.left;
            both_rect.top = tips_left_rect.top;
            both_rect.right = tips_left_rect.right;
            both_rect.bottom = tips_left_rect.bottom;
            if(GUI_PointIsInRect(cur_point, both_rect))
            {
                block = i;
                s_kl_info_ptr->is_handled =TRUE;
            #ifndef USE_SOFTKEY_BUTTON                                
                HandlekLPatternTipsMsg(win_id, block);
            #endif                    
                return s_kl_info_ptr->is_handled;
            }
            
            tips_left_rect.left =tips_right_rect.left;
            tips_left_rect.top =tips_right_rect.top;
            tips_left_rect.right =tips_right_rect.right;
            tips_left_rect.bottom =tips_right_rect.bottom;
        }
    }
    else
    {
        if(GUI_PointIsInRect(cur_point, tips_center_rect))
        {
            if(s_kl_info_ptr != PNULL)
            {
                s_kl_info_ptr->is_handled =TRUE;
            }
            if (!MMK_CreatePubEditWin((uint32*)MMICC_EMERGENCY_WIN_TAB, PNULL))
            {
                if (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus())
                {
                    MMIKL_SetWinStatus(KL_LOCKED);
                }
            }              
            return TRUE;
        }
    }

    s_kl_info_ptr->is_handled =FALSE;
    return s_kl_info_ptr->is_handled;
}

/*****************************************************************************/
//  Description :屏幕上触摸滑动开始或是点中TIPS
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void HandleKLPatternStart(MMI_WIN_ID_T win_id, DPARAM param)
{
    int8 Index = 0;
    GUI_POINT_T  pre_point= {0};
    GUI_POINT_T  cur_point= {0};
    GUI_RECT_T  title_rect ={0};

    title_rect.right =MMI_MAINSCREEN_WIDTH;
    title_rect.bottom =KL_PATTERN_TITLE_HEIGHT;
    
    pre_point.x = MMK_GET_TP_X(param);
    pre_point.y = MMK_GET_TP_Y(param);

    if (GUI_PointIsInRect(pre_point, title_rect))
    {
        return;
    }

    if(HandleKLPatternTipsStart(win_id, pre_point))
    {
        return;
    }
    if (PNULL == s_kl_info_ptr)
    {
        return;
    }
    if(0!=s_kl_info_ptr->KL_pattern_ok_timer_id)
    {
        return;
    }
    
    StopKLPatternTipsTimer(win_id);

    
    if (s_kl_info_ptr->input_result)
    {
        return;
    }
    else
    {
        ClearAllLayer();
    }
    
    if(IsPointAround(pre_point, &Index))
    {

        if (MMIKL_SET_KL_PASSWARD_WIN_ID == win_id)
        {
            if(KL_WIN_TYPE_CONFIRM == s_kl_info_ptr->win_type)
            {
                s_kl_info_ptr->new_point_again_ptr[s_kl_info_ptr->point_index] = Index;
            }
            else
            {
                s_kl_info_ptr->new_point_ptr[s_kl_info_ptr->point_index] = Index;
            }
        }
        else
        {
            s_kl_info_ptr->unlock_point_ptr[s_kl_info_ptr->point_index] = Index;
        }
        
        cur_point.x =s_kl_info_ptr->st_passward_point[Index].x;
        cur_point.y =s_kl_info_ptr->st_passward_point[Index].y;
        s_kl_info_ptr->key_state =KL_KEY_STATE_DOWN;
        RestartKLPatternTPMoveTimer(win_id);
        
        if (UILAYER_IsLayerActive(&s_kl_info_ptr->image_confirm_layer_handle))
        {
            DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, cur_point, IMAGE_KEYLOCK_SELECT_DOT);
            DisplayKLPatternTitleAndSK(win_id, s_kl_info_ptr->key_state, s_kl_info_ptr->win_type, s_kl_info_ptr->input_result);
        }
    }
    else
    {
        if (KL_WIN_TYPE_CONFIRM == s_kl_info_ptr->win_type)
        {
            SCI_MEMSET(s_kl_info_ptr->new_point_again_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
        } 
        else
        {
            SCI_MEMSET(s_kl_info_ptr->new_point_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
        }
    }
    BltAllLayer(s_kl_info_ptr);
}

/*****************************************************************************/
//  Description :屏幕上触摸滑动过程处理
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL BOOLEAN HandleKLPatternMove(MMI_WIN_ID_T win_id, DPARAM param)
{
    int8 Index = 0;
    int8 pre_index =0;
    int8 pass_index =-1;
    GUI_POINT_T  cur_point = {0};
    
    GUI_RECT_T  title_rect ={0};
    GUI_RECT_T  tips_bg_rect =KL_PATTERN_TIPS_BG_RECT;

#ifdef MMI_KL_DIS_ALPHA_LINE
    GUILCD_DRAW_ALPHA_LINE_T alpha_line_info = {MMI_GRAY_COLOR, MMI_KL_PASSWARD_LINE_WIDTH, MMI_KL_PATTERN_LINE_ALPHA, TRUE};
#endif

    title_rect.right =MMI_MAINSCREEN_WIDTH - 1;
    title_rect.bottom =KL_PATTERN_TITLE_HEIGHT - 1;
    UILAYER_SetDirectDraw(TRUE);
    cur_point.x = MMK_GET_TP_X(param);
    cur_point.y = MMK_GET_TP_Y(param);

    if (PNULL == s_kl_info_ptr)
    {
        return FALSE;
    }

    //预防处理后对title或tips再处理，已经在HandleKLPatterStart中处理了
    if((GUI_PointIsInRect(cur_point, title_rect)||GUI_PointIsInRect(cur_point, tips_bg_rect))&&(s_kl_info_ptr->is_handled))
    {
        return FALSE;
    }

    if (MMIKL_SET_KL_PASSWARD_WIN_ID == win_id)
    {
        if(KL_WIN_TYPE_CONFIRM == s_kl_info_ptr->win_type)
        {
            pre_index = s_kl_info_ptr->new_point_again_ptr[s_kl_info_ptr->point_index];
        }
        else
        {
            pre_index = s_kl_info_ptr->new_point_ptr[s_kl_info_ptr->point_index];
        }
    }
    else
    {
        pre_index = s_kl_info_ptr->unlock_point_ptr[s_kl_info_ptr->point_index];
    }
    //没有记录到上个点时
    if (pre_index <0)
    {
        return FALSE;
    }
    //不能超过10次
    if (s_kl_info_ptr->point_index >= MMI_KL_PASSWARD_POINT_MAX -1)
    {
        return FALSE;
    }

    if (s_kl_info_ptr->input_result)
    {
        return FALSE;
    }    
  
    if (IsPointAround(cur_point, &Index))
    {
        if (Index == pre_index)
        {
            return TRUE;
        }
        
        if (MMIKL_SET_KL_PASSWARD_WIN_ID == win_id)
        {
            if(KL_WIN_TYPE_CONFIRM == s_kl_info_ptr->win_type)
            {
                //是否是已经划过的点
                if (IsRepeatPoint(cur_point, s_kl_info_ptr->new_point_again_ptr))
                {
                    if (UILAYER_IsLayerActive(&s_kl_info_ptr->slide_layer_handle) && (pre_index >= 0))
                    {
                        UILAYER_Clear(&s_kl_info_ptr->slide_layer_handle);
#ifdef MMI_KL_DIS_ALPHA_LINE
                        LCD_DrawAlphaLine(&s_kl_info_ptr->slide_layer_handle, s_kl_info_ptr->st_passward_point[pre_index], cur_point, alpha_line_info);
#else
                        LCD_DrawLayerLine(&s_kl_info_ptr->slide_layer_handle, s_kl_info_ptr->st_passward_point[pre_index].x, 
                            s_kl_info_ptr->st_passward_point[pre_index].y, cur_point.x, cur_point.y, MMI_GRAY_COLOR, MMI_KL_PASSWARD_LINE_WIDTH); 
#endif
                    }
                    return TRUE;
                }
                //是否跨过没划过点
                if (IsPassPoint(Index, s_kl_info_ptr->new_point_again_ptr, &pass_index))
                {
                    s_kl_info_ptr->point_index ++;
                    s_kl_info_ptr->new_point_again_ptr[s_kl_info_ptr->point_index] =pass_index;
                }
                
                s_kl_info_ptr->point_index++;
                s_kl_info_ptr->new_point_again_ptr[s_kl_info_ptr->point_index] =Index;
            }
            else
            {
                //是否是已经划过的点
                if (IsRepeatPoint(cur_point, s_kl_info_ptr->new_point_ptr))
                {
                    if (UILAYER_IsLayerActive(&s_kl_info_ptr->slide_layer_handle) && (pre_index >= 0))
                    {
                        UILAYER_Clear(&s_kl_info_ptr->slide_layer_handle);
#ifdef MMI_KL_DIS_ALPHA_LINE
                        LCD_DrawAlphaLine(&s_kl_info_ptr->slide_layer_handle, s_kl_info_ptr->st_passward_point[pre_index], cur_point, alpha_line_info);
#else
                        LCD_DrawLayerLine(&s_kl_info_ptr->slide_layer_handle, s_kl_info_ptr->st_passward_point[pre_index].x, s_kl_info_ptr->st_passward_point[pre_index].y, 
                            cur_point.x, cur_point.y, MMI_GRAY_COLOR, MMI_KL_PASSWARD_LINE_WIDTH);
#endif
                    }
                    return TRUE;
                }
                //是否跨过没划过点
                if (IsPassPoint(Index, s_kl_info_ptr->new_point_ptr, &pass_index))
                {
                    s_kl_info_ptr->point_index ++;
                    s_kl_info_ptr->new_point_ptr[s_kl_info_ptr->point_index] =pass_index;
                }
                
                s_kl_info_ptr->point_index++;
                s_kl_info_ptr->new_point_ptr[s_kl_info_ptr->point_index] =Index;
            }
        }
        else
        {
            //是否是已经划过的点
            if (IsRepeatPoint(cur_point, s_kl_info_ptr->unlock_point_ptr))
            {              
                if (UILAYER_IsLayerActive(&s_kl_info_ptr->slide_layer_handle) && (pre_index >= 0))
                {
                    UILAYER_Clear(&s_kl_info_ptr->slide_layer_handle);
#ifdef MMI_KL_DIS_ALPHA_LINE
                    LCD_DrawAlphaLine(&s_kl_info_ptr->slide_layer_handle, s_kl_info_ptr->st_passward_point[pre_index], cur_point, alpha_line_info);
#else
                    LCD_DrawLayerLine(&s_kl_info_ptr->slide_layer_handle,s_kl_info_ptr->st_passward_point[pre_index].x, s_kl_info_ptr->st_passward_point[pre_index].y, 
                        cur_point.x, cur_point.y, MMI_GRAY_COLOR, MMI_KL_PASSWARD_LINE_WIDTH);
#endif
                }
                return TRUE;
            }
            //是否跨过没划过点
            if (IsPassPoint(Index, s_kl_info_ptr->unlock_point_ptr, &pass_index))
            {
                s_kl_info_ptr->point_index ++;
                s_kl_info_ptr->unlock_point_ptr[s_kl_info_ptr->point_index] =pass_index;
            }
            
            s_kl_info_ptr->point_index ++;
            s_kl_info_ptr->unlock_point_ptr[s_kl_info_ptr->point_index] =Index;
        }

        if (UILAYER_IsLayerActive(&s_kl_info_ptr->slide_layer_handle)&& UILAYER_IsLayerActive(&s_kl_info_ptr->image_confirm_layer_handle))
        {
            UILAYER_Clear(&s_kl_info_ptr->slide_layer_handle);
            
#ifdef MMI_KL_DIS_ALPHA_LINE
            LCD_DrawAlphaLine(&s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[pre_index], s_kl_info_ptr->st_passward_point[Index], alpha_line_info);
#else
            LCD_DrawLayerLine(&s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[pre_index].x, s_kl_info_ptr->st_passward_point[pre_index].y, 
                s_kl_info_ptr->st_passward_point[Index].x, s_kl_info_ptr->st_passward_point[Index].y, MMI_GRAY_COLOR, MMI_KL_PASSWARD_LINE_WIDTH);
#endif
            if (pass_index>0)
            {
                DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[pass_index], IMAGE_KEYLOCK_SELECT_DOT);
            }
            DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[pre_index], IMAGE_KEYLOCK_SELECT_DOT);
            DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[Index], IMAGE_KEYLOCK_SELECT_DOT);
        }
    }
    else
    {
        if (UILAYER_IsLayerActive(&s_kl_info_ptr->slide_layer_handle) && (pre_index >= 0))
        {
            UILAYER_Clear(&s_kl_info_ptr->slide_layer_handle);
#ifdef MMI_KL_DIS_ALPHA_LINE
            LCD_DrawAlphaLine(&s_kl_info_ptr->slide_layer_handle, s_kl_info_ptr->st_passward_point[pre_index], cur_point, alpha_line_info);
#else
            LCD_DrawLayerLine(&s_kl_info_ptr->slide_layer_handle, s_kl_info_ptr->st_passward_point[pre_index].x, s_kl_info_ptr->st_passward_point[pre_index].y, 
                cur_point.x, cur_point.y, MMI_GRAY_COLOR, MMI_KL_PASSWARD_LINE_WIDTH);
#endif
        }
    }
    BltAllLayer(s_kl_info_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description :触摸滑动结束时处理
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void HandleKLPatternEnd(MMI_WIN_ID_T win_id, DPARAM param)
{
    int8 index =0;
    int8 pre_index =0;
    GUI_POINT_T cur_point={0};
    GUI_RECT_T  title_rect ={0};
    GUI_RECT_T  tips_bg_rect =KL_PATTERN_TIPS_BG_RECT;
    int8 passward[MMI_KL_PASSWARD_POINT_MAX] ={0};
    int32 i ;
    
    title_rect.right =MMI_MAINSCREEN_WIDTH;
    title_rect.bottom =KL_PATTERN_TITLE_HEIGHT;

    if(PNULL ==s_kl_info_ptr)
    {
        return;
    }
    
    if(s_kl_info_ptr->key_state == KL_KEY_STATE_DOWN)
    {
    
        s_kl_info_ptr->key_state =KL_KEY_STATE_UP;
    }
    
    cur_point.x = MMK_GET_TP_X(param);
    cur_point.y = MMK_GET_TP_Y(param);
    
    if(UILAYER_IsLayerActive(&s_kl_info_ptr->slide_layer_handle))
    {
        UILAYER_Clear(&s_kl_info_ptr->slide_layer_handle);
    }

    if((GUI_PointIsInRect(cur_point, title_rect)||GUI_PointIsInRect(cur_point, tips_bg_rect))&&(s_kl_info_ptr->is_handled))
    {
       return;
    }

    if (s_kl_info_ptr->input_result)
    {
        return;
    }

    if (MMIKL_SET_KL_PASSWARD_WIN_ID == win_id)
    {
        if(KL_WIN_TYPE_CONFIRM == s_kl_info_ptr->win_type)
            pre_index = s_kl_info_ptr->new_point_again_ptr[s_kl_info_ptr->point_index];
        else
            pre_index = s_kl_info_ptr->new_point_ptr[s_kl_info_ptr->point_index];
    }
    else
    {
        pre_index = s_kl_info_ptr->unlock_point_ptr[s_kl_info_ptr->point_index];
    }
        
    if (pre_index <0)
    {
        return ;
    }

    if ((s_kl_info_ptr->point_index <3)&&(MMIKL_SET_KL_PASSWARD_WIN_ID == win_id)&&(KL_WIN_TYPE_SET ==  s_kl_info_ptr->win_type))
    {
        
        if (0 == s_kl_info_ptr->KL_pattern_tips_timer_id)
        {
        //点变成红色
            for(i=0; i<=s_kl_info_ptr->point_index; i++)
            {
                index =s_kl_info_ptr->new_point_ptr[i];
                DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[index], IMAGE_KEYLOCK_ERROR_DOT);
            }
            s_kl_info_ptr->input_result =FALSE;
            StartKLPatternTipsTimer(win_id);
            DisplayKLPatternTitleAndSK(win_id, s_kl_info_ptr->key_state, s_kl_info_ptr->win_type, s_kl_info_ptr->input_result);
        }
        s_kl_info_ptr->point_index =0;

        if(KL_WIN_TYPE_SET ==  s_kl_info_ptr->win_type)
        {
            SCI_MEMSET(s_kl_info_ptr->new_point_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
        }
        return;
    }

    if (MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
    {
        //g_new_point_ptr与NV中的密码比较
        MMIAPISET_GetKeyLockPassward(passward);

        if(ComparePassward(s_kl_info_ptr->unlock_point_ptr, passward))
        {
            //中间点变成白色
            for(i=0; i<=s_kl_info_ptr->point_index; i++)
            {
                index =s_kl_info_ptr->unlock_point_ptr[i];
                DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[index], IMAGE_KEYLOCK_WHITE_DOT);
            }

#ifdef LIVE_WALLPAPER_SUPPORT_LINEAR
                //if(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
                {
                    /* Linear wallpaper */
                    MMISET_WALLPAPER_SET_T  wallpaper_info = {0};
                    
                    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
                    //if(MMISET_WALLPAPER_LINEAR == wallpaper_info.wallpaper_type)
                    {
                        //MMIIDLE_HandleLinearWallPaper(win_id, MSG_LOSE_FOCUS, param);
                    }
                }
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_POP
                //if(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
                {
                    /* Linear wallpaper */
                    MMISET_WALLPAPER_SET_T  wallpaper_info = {0};
                    
                    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
                    //if(MMISET_WALLPAPER_POP == wallpaper_info.wallpaper_type)
                    {
                        //MMIIDLE_HandlePopWallPaper(win_id, MSG_LOSE_FOCUS, param);
                    }
                }
#endif
            //用定时器启动解锁
            StartKLPatternOKTimer(win_id);
        }
        else
        {
            //点变成红色
            for(i=0; i<=s_kl_info_ptr->point_index; i++)
            {
                index =s_kl_info_ptr->unlock_point_ptr[i];
                DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[index], IMAGE_KEYLOCK_ERROR_DOT);
            }
            s_kl_info_ptr->input_result =FALSE;
            DisplayKLPatternTitleAndSK(win_id, s_kl_info_ptr->key_state, s_kl_info_ptr->win_type, s_kl_info_ptr->input_result);
            StartKLPatternTipsTimer(win_id);
            SCI_MEMSET(s_kl_info_ptr->unlock_point_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
        }

    }
    else if (MMIKL_SET_KL_PASSWARD_WIN_ID == win_id)
    {
        if (KL_WIN_TYPE_SET ==  s_kl_info_ptr->win_type)
        {
        //中间点变成白色
            for(i=0; i<=s_kl_info_ptr->point_index; i++)
            {
                index =s_kl_info_ptr->new_point_ptr[i];
                DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[index], IMAGE_KEYLOCK_WHITE_DOT);
            }
            //s_kl_info_ptr->win_type =KL_WIN_TYPE_CONFIRM;
            s_kl_info_ptr->input_result =TRUE;
            DisplayKLPatternTitleAndSK(win_id, s_kl_info_ptr->key_state, s_kl_info_ptr->win_type, s_kl_info_ptr->input_result);
        }
        else
        {
            //g_new_point_again_ptr与g_new_point_ptr中的值比较
            
            if(ComparePassward(s_kl_info_ptr->new_point_ptr, s_kl_info_ptr->new_point_again_ptr))
            {
            //中间点变成白色
                for(i=0; i<=s_kl_info_ptr->point_index; i++)
                {
                    index =s_kl_info_ptr->new_point_ptr[i];
                    DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[index], IMAGE_KEYLOCK_WHITE_DOT);
                }
                s_kl_info_ptr->input_result =TRUE;
                DisplayKLPatternTitleAndSK(win_id, s_kl_info_ptr->key_state, s_kl_info_ptr->win_type, s_kl_info_ptr->input_result);
            }
            else
            {
            //点变成红色
                for(i=0; i<=s_kl_info_ptr->point_index; i++)
                {
                    index =s_kl_info_ptr->new_point_again_ptr[i];
                    DispDotImg(win_id, s_kl_info_ptr->image_confirm_layer_handle, s_kl_info_ptr->st_passward_point[index], IMAGE_KEYLOCK_ERROR_DOT);
                }
                s_kl_info_ptr->input_result =FALSE;
                StartKLPatternTipsTimer(win_id);
                DisplayKLPatternTitleAndSK(win_id, s_kl_info_ptr->key_state, s_kl_info_ptr->win_type, s_kl_info_ptr->input_result);
                SCI_MEMSET(s_kl_info_ptr->new_point_again_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
            }
        }
        }
    s_kl_info_ptr->point_index =0;
    BltAllLayer(s_kl_info_ptr);
}

#if defined( LIVE_WALLPAPER_SUPPORT_LINEAR)||defined(LIVE_WALLPAPER_SUPPORT_POP)
/*****************************************************************************/
//  Description : MMIKL_GetWallpaperLayer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_GetWallpaperLayer(GUI_LCD_DEV_INFO *layer_ptr)
{
    MMISET_WALLPAPER_SET_T  wallpaper_info = {0};
    
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
#ifdef LIVE_WALLPAPER_SUPPORT_LINEAR
    //if(MMISET_WALLPAPER_LINEAR == wallpaper_info.wallpaper_type)
    {
        if(PNULL != s_kl_info_ptr && PNULL != layer_ptr)
        {
            *layer_ptr = s_kl_info_ptr->wallpaper_layer;
        }
    }
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_POP
//    if(MMISET_WALLPAPER_POP == wallpaper_info.wallpaper_type)
    {
        if(PNULL != s_kl_info_ptr && PNULL != layer_ptr)
        {
            *layer_ptr = s_kl_info_ptr->wallpaper_layer;
        }
    }
#endif
}
#endif


/*****************************************************************************/
//  Description : 打开设置图形加密窗口
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
PUBLIC void MMIKL_OpenSetKLPassward(void)
{
    MMK_CreateWin((uint32*)MMIKL_SET_KL_PASSWARD_WIN_TAB, PNULL);
}

#if defined USE_SOFTKEY_BUTTON            
/*****************************************************************************/
//  Description : create soft key and set the param.
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL void CreateAndSetSoftKey(MMI_WIN_ID_T    win_id)
{
    MMI_CTRL_ID_T button[4];
#ifdef MMI_PDA_SUPPORT    
    MMK_SetWinDisplayStyleState(win_id, WS_HAS_BUTTON_SOFTKEY, TRUE);
#endif
    GUIWIN_CreateSoftkeyDyn(win_id, TXT_COMMON_OK,  0, STXT_RETURN);
    if (PNULL !=s_kl_info_ptr)
    {
        GUIAPICTRL_SetLcdDevInfo(MMITHEME_GetSoftkeyCtrlId(),&s_kl_info_ptr->wallpaper_layer);
        MMITHEME_GetSoftkeyButtonCtrlId(button);
        GUIAPICTRL_SetLcdDevInfo(button[0], &s_kl_info_ptr->wallpaper_layer);
        GUIAPICTRL_SetLcdDevInfo(button[1], &s_kl_info_ptr->wallpaper_layer);
        GUIAPICTRL_SetLcdDevInfo(button[2], &s_kl_info_ptr->wallpaper_layer);
    }
}
#endif

/*****************************************************************************/
//  Description : 处理图形加密及解密入口函数
//  Global resource dependence : 
//  Author:
//  Date:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetKLPasswardWindow(
                                            MMI_WIN_ID_T    win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
//#if defined( LIVE_WALLPAPER_SUPPORT_LINEAR)||defined(LIVE_WALLPAPER_SUPPORT_POP)
//    MMISET_WALLPAPER_SET_T  wallpaper_info = {0};
// #endif
    
    //SCI_TRACE_LOW:"HandleSetKLPasswardWindow: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_DRAWGRAPHIC_2177_112_2_18_2_30_50_1,(uint8*)"d",msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL != s_kl_info_ptr)
        {
            DestroyAllLayer(s_kl_info_ptr);
            SCI_FREE(s_kl_info_ptr);
            s_kl_info_ptr = PNULL;
        }
        CreateKeyLockPattern(win_id);
        InitKeyLockPatternInfo(win_id);
        
        //UILAYER_RemoveMainLayer(); //移除主层
        CreateAllLayer(win_id);
        /* Modify by michael on 3/2/2012 for NEWMS00161908  : Bly all layer when we created layer */
        BltAllLayer(s_kl_info_ptr);
        
        if(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
        {
            MMIKL_SetWinStatus(KL_LOCKED);
            GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); //禁用状态栏
     
            if (PNULL !=s_kl_info_ptr)
            {
                GUIAPICTRL_SetLcdDevInfo(MMIKL_PATTERN_UNLCOK_ANIM_CTRL_ID,&s_kl_info_ptr->wallpaper_layer);
            }
        }
        else
        {
            if (PNULL !=s_kl_info_ptr)
            {
                GUIAPICTRL_SetLcdDevInfo(MMK_GetWinTitleCtrlId(win_id),&s_kl_info_ptr->wallpaper_layer);
            }
#if defined USE_SOFTKEY_BUTTON            
            CreateAndSetSoftKey(win_id);        
#endif
        }
        if (PNULL !=s_kl_info_ptr)
        {
            GUIAPICTRL_SetLcdDevInfo(MMITHEME_GetStatusBarCtrlId(),&s_kl_info_ptr->title_layer_handle);
        }
        
        break;
    case MSG_FULL_PAINT:
        //画背景 
        if(MMIKL_SET_KL_PASSWARD_WIN_ID == win_id)
        {
            GUI_RECT_T win_rect = {0};
            MMK_GetWinRect(win_id, &win_rect);
            win_rect.top += MMI_TITLE_THEME_HEIGHT + MMI_STATUSBAR_HEIGHT;
            if (PNULL !=s_kl_info_ptr)
            {
                GUI_FillRect(&s_kl_info_ptr->wallpaper_layer, win_rect, MMI_WINDOW_BACKGROUND_COLOR);
            }
        }
        /* Modify by michael on 3/2/2012 for NEWMS00161908  : */
        UpdateDG(win_id);
        break;
        
        //时间分钟刷新时
    case MSG_IDLE_UPDATE_DATETIME:
    case MISC_MSG_RTC_MIN:
        if (MMK_IsFocusWin(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID))
        {
            DisplayClock(win_id);
        }
        break;
#if defined USE_SOFTKEY_BUTTON
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                HandlekLPatternTipsMsg(win_id, KL_PATTERN_TIPS_LEFT_BLOCK);

            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                HandlekLPatternTipsMsg(win_id, KL_PATTERN_TIPS_RIGHT_BLOCK);
            }
        }
        break;
#endif  //TOUCH_PANEL_SUPPORT //IGNORE9527  
#endif
    case MSG_BACKLIGHT_TURN_OFF:
        if(s_kl_info_ptr != PNULL)
        {
            StopKLPatternTPMoveTimer(win_id);
            StopKLPatternOKTimer(win_id);
            StopKLPatternTipsTimer(win_id);
        }
        break;
        
    case MSG_BACKLIGHT_TURN_ON:
        break;
        
    case MSG_LOSE_FOCUS:
        if(s_kl_info_ptr != PNULL)
        {
            StopKLPatternTPMoveTimer(win_id);
            StopKLPatternOKTimer(win_id);
            StopKLPatternTipsTimer(win_id);
        }
        break;
        
    case MSG_GET_FOCUS:	
        /* Modify by michael on 3/2/2012 for NEWMS00161908  : destory/recreate*/
        if (PNULL != s_kl_info_ptr)
        {
            DestroyAllLayer(s_kl_info_ptr);
            SCI_FREE(s_kl_info_ptr);
            s_kl_info_ptr = PNULL;
        }
        
        if (PNULL == s_kl_info_ptr)
        {
            CreateKeyLockPattern(win_id);
            InitKeyLockPatternInfo(win_id);
            CreateAllLayer(win_id);
        }
        BltAllLayer(s_kl_info_ptr);
        /************************Modify over*************************/
        break;
        
    case MSG_TIMER:
        if (PNULL != s_kl_info_ptr && s_kl_info_ptr->KL_pattern_ok_timer_id == *(uint8*)param) 
        {
            StopKLPatternOKTimer(win_id);
            if(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID == win_id)
            {
                MMIKL_SetWinStatus(KL_UNLOCKED);
                StopKLPatternTipsTimer(win_id);
                StopKLPatternTPMoveTimer(win_id);
                MMK_CloseWin(win_id);
                MMIKL_CheckPromptStatus();
                break;
            }
            else
            {
                ClearAllLayer();
            }
        }
        
        else if (PNULL != s_kl_info_ptr && (s_kl_info_ptr->KL_pattern_tips_timer_id == *(uint8*)param))
        {
            if(KL_WIN_TYPE_SET ==  s_kl_info_ptr->win_type)
            {
                SCI_MEMSET(s_kl_info_ptr->new_point_ptr, -1, MMI_KL_PASSWARD_POINT_MAX);
            }
            
            ClearAllLayer();
            StopKLPatternTipsTimer(win_id);
        }
        
        else if(PNULL != s_kl_info_ptr && (s_kl_info_ptr->KL_pattern_tp_move_id == *(uint8*)param))
        {
            GUI_POINT_T     point = {0, 0};
            MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
            MMI_MULTI_KEY_TP_MSG_PARAM_T   para = {0};
            MMK_GetLogicTPMsg(&tp_status, &point);
            para.cur_tp_point.x = point.x;
            para.cur_tp_point.y = point.y;
            
            StopKLPatternTPMoveTimer(win_id);
            if (HandleKLPatternMove(win_id , (DPARAM)&para))
            {
                RestartKLPatternTPMoveTimer(win_id);
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_KEYDOWN_RED:
        if (MMIKL_IsPreviewState())
        {
            recode = MMI_RESULT_FALSE;
        }   
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if (!MMIKL_IsPreviewState())
        {
            StopKLPatternTPMoveTimer(win_id);
            HandleKLPatternEnd(win_id, param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        if (!MMIKL_IsPreviewState())
        {
            HandleKLPatternStart(win_id, param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        //Timer handle tp_move
        //HandleKLPatternMove(win_id, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        //退出时，屏幕保持实际的状态
        GUILCD_SetLogicAngle( GUI_MAIN_LCD_ID, GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID));
        
        //退出当前WIN
        if (PNULL != s_kl_info_ptr)
        {
            DestroyAllLayer(s_kl_info_ptr);
            SCI_FREE(s_kl_info_ptr);
            s_kl_info_ptr = PNULL;
        }
        
        UILAYER_RestoreMainLayer();
        GUIAPICTRL_SetLcdDevInfo(MMITHEME_GetStatusBarCtrlId(), MMITHEME_GetDefaultLcdDev());
        
        //MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
        if(MMIKL_KL_PASSWARD_UNLOCK_WIN_ID != win_id ||MMIKL_IsPreviewState())
        {
            MMK_CloseWin(win_id);
        }
		else
		{
			recode = MMI_RESULT_FALSE;		
		}
        break;
    case MSG_APP_OK:
 //   case MSG_APP_MENU:
        if (MMIKL_IsPreviewState())
        {
            MMIAPISET_OpenKeyLockEffect5SubWin();
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


/******************************************************************************/
//  Description : handle kl display window msg of type4.
//  Global resource dependence : none
//  Author: 
//  Note: Pattern unlock style
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleDrawGraphicWinMsg(
                                            MMI_WIN_ID_T    win_id, 
                                            uint16          msg_id, 
                                            DPARAM          param
                                            )
{
    return HandleSetKLPasswardWindow(win_id, msg_id, param);
}
#endif	//MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
#endif




/*Edit by script, ignore 3 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
