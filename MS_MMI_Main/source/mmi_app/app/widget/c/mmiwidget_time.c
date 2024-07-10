/*****************************************************************************
** File Name:      mmiwidget_time.c                                        *
** Author:                                                                   *
** Date:           2012/02/07                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/02/07     juan.wu             Creat
******************************************************************************/
#define _MMIWIDGET_TIME_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#ifdef MMI_WIDGET_TIME
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget_image.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmipub.h"
#include "mmiwidget_anim.h"
#include "mmiwidget_nv.h"
#include "graphics_3d_rotate.h"
#if defined QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef MAINLCD_SIZE_320X480
#define MMIWIDGET_TIME_AXLE_X      47//第一个轴离背景图最左边的偏移
#define MMIWIDGET_TIME_AXLE_Y      44//轴 离 背景图最顶端的偏移
#define MMIWIDGET_TIME_X_BETWEEN_TWO      107 //每个轴水平方向的间距
#define MMIWIDGET_TIME_HOUR_BG_OFFSET_X    54
#define MMIWIDGET_TIME_HOUR_BG_OFFSET_Y    8
#define MMIWIDGET_TIME_MIN_BG_OFFSET_X    159
#define MMIWIDGET_TIME_HOUR_OFFSET_X  65
#define MMIWIDGET_TIME_HOUR_OFFSET_Y  27
#define MMIWIDGET_TIME_MIN_OFFSET_X   171
#define MMIWIDGET_TIME_WEEK_OFFSET_X   60
#define MMIWIDGET_TIME_WEEK_WIDTH     58
#define MMIWIDGET_TIME_WEEK_OFFSET_Y  115
#define MMIWIDGET_TIME_DATE_OFFSET_X  161
#define MMIWIDGET_TIME_DATE_WIDTH     97
#ifdef MMI_SAM_THEME
#define MMIWIDGET_SAM_TIME_HOUR_OFFSET_X  15
#define MMIWIDGET_SAM_TIME_HOUR_OFFSET_Y  27
#define MMIWIDGET_SAM_TIME_MIN_OFFSET_X   121

#define MMIWIDGET_SAM_TIME_WEEK_OFFSET_X   230
#define MMIWIDGET_SAM_TIME_WEEK_WIDTH      58
#define MMIWIDGET_SAM_TIME_WEEK_OFFSET_Y   30
#define MMIWIDGET_SAM_TIME_DATE_OFFSET_X   230
#define MMIWIDGET_SAM_TIME_DATE_WIDTH      97
#endif
#else
#define MMIWIDGET_TIME_AXLE_X      8//第一个轴离背景图最左边的偏移
#define MMIWIDGET_TIME_AXLE_Y      36//轴 离 背景图最顶端的偏移
#define MMIWIDGET_TIME_X_BETWEEN_TWO      93 //每个轴水平方向的间距
#define MMIWIDGET_TIME_HOUR_BG_OFFSET_X    13
#define MMIWIDGET_TIME_HOUR_BG_OFFSET_Y    6
#define MMIWIDGET_TIME_MIN_BG_OFFSET_X    105
#define MMIWIDGET_TIME_HOUR_OFFSET_X  26
#define MMIWIDGET_TIME_HOUR_OFFSET_Y  21
#define MMIWIDGET_TIME_MIN_OFFSET_X   118
#define MMIWIDGET_TIME_WEEK_OFFSET_X  18
#define MMIWIDGET_TIME_WEEK_WIDTH     62
#define MMIWIDGET_TIME_WEEK_OFFSET_Y  100
#define MMIWIDGET_TIME_DATE_OFFSET_X  105
#define MMIWIDGET_TIME_DATE_WIDTH     85
#endif
#define MMIWIDGET_TIME_AXLE_NUM     3//三个轴
#define MMIWIDGET_TIME_DATE_MAX_LEN   16//2001-01-01
#define TIME_UPDATE_SE_FRAME_NUM        (16)
#define HOUR_PAGE_OFFSET_FRAME_NUM      (5)     
#define TIME_UPDATE_SE_TIMER       (1000/TIME_UPDATE_SE_FRAME_NUM)
#define TIME_ANIM_MAX_ANGLE         1800
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL const MMI_IMAGE_ID_T s_time_img[] = {IMAGE_WIDGET_TIME_0,
                                             IMAGE_WIDGET_TIME_1,
                                             IMAGE_WIDGET_TIME_2,
                                             IMAGE_WIDGET_TIME_3,
                                             IMAGE_WIDGET_TIME_4,
                                             IMAGE_WIDGET_TIME_5,
                                             IMAGE_WIDGET_TIME_6,
                                             IMAGE_WIDGET_TIME_7,
                                             IMAGE_WIDGET_TIME_8,
                                             IMAGE_WIDGET_TIME_9,                                     
                                             };
LOCAL GUI_LCD_DEV_INFO  s_time_min_old_layer    = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_time_min_new_layer    = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_time_min_calc_layer   = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_time_hour_old_layer    = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_time_hour_new_layer    = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_time_hour_calc_layer   = {0,UILAYER_NULL_HANDLE};

LOCAL uint8       s_anim_timer_id = 0;
LOCAL uint8       s_anim_frame_id = 0;
LOCAL BOOLEAN     s_is_h_changed = FALSE;
LOCAL SCI_TIME_T  s_sys_time = {0}; 

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : HandleWidgetScheduleWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetTimeWinMsg(
											   MMI_WIN_ID_T        win_id,        //IN:
											   MMI_MESSAGE_ID_E    msg_id,        //IN:
											   DPARAM            param        //IN:
											   );
/*****************************************************************************/
//  Description : DisplayAxleIcons
//  Global resource dependence : 
//  Author:
//  Note: 显示三个轴
/*****************************************************************************/
LOCAL void DisplayAxleIcons(void);
						   
/*****************************************************************************/
//  Description : DisplayTimeUI
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void  DisplayTimeUI(MMI_WIN_ID_T     win_id);

/*****************************************************************************/
//    Description :DisplayCurrentTime
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL void DisplayCurrentTime(void);

/*****************************************************************************/
//    Description :在时间widget上显示星期和日期
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL void DisplayWeekAndDay(void);

/*****************************************************************************/
//    Description :播放时间更新特效
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayTimeUpdateSe(SCI_TIME_T pre_sys_time);
/*****************************************************************************/
//    Description : 创建时间翻页特效Layer
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateTimeBoxLayer(GUI_LCD_DEV_INFO *layer_ptr, 
                                    int16 offset_x, 
                                    int16 offset_y,
                                    uint16 img_w,
                                    uint16 img_h,
                                    BOOLEAN is_min);
/*****************************************************************************/
//  Discription: 释放时间特效图层
//  Global resource dependence: None
//  Author: 
//  Note : 
/*****************************************************************************/
LOCAL void RealeaseAllLayer(void);
/*****************************************************************************/
//  Discription: 创建时间特效图层
//  Global resource dependence: None
//  Author: 
//  Note : 一共需要6个层，只要有一个层没有创建成功，就会释放所有图层且返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN CreateAllTimeAnimLayer(void);

/*****************************************************************************/
//  Discription: DisplayAmPmString
//  Global resource dependence: None
//  Author: 
//  Note : 显示AM/PM字符 
/*****************************************************************************/
LOCAL void DisplayAmPmString(BOOLEAN is_am,
                                GUI_POINT_T  display_point,
                                uint16 time_w,
                                uint16 time_h,
                                GUI_LCD_DEV_INFO * lcd_info_ptr);

/*****************************************************************************/
//    Description :停止播放时间更新特效
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL void StopTimeUpdateSe(void);

/*****************************************************************************/
//  Description : get dual aclock widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetTimeWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
PUBLIC const MMIWIDGET_ITEM_INFO_T g_time_widget =
{
    MMIWIDGET_TIME_ID, 
	HandleWidgetTimeWinMsg, 
	PNULL,
	PNULL,
	GetTimeWidgetImage,
	WIDGET_TIME_WIN_ID, 
	IMAGE_WIDGET_TIME_ICON_SHORTCUT, 
	IMAGE_WIDGET_TIME_BG,
    TXT_WIDGET_TIME,		
	50,//    75, 
	50,//    50
	FALSE,
};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get dual aclock widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetTimeWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img)
{
    if (PNULL != bg_img)
    {
        *bg_img = IMAGE_WIDGET_TIME_BG;
    }

    if (PNULL != sc_img)    
    {
        *sc_img = IMAGE_WIDGET_TIME_ICON_SHORTCUT;
    }
    
#ifdef MMI_SAM_THEME	
    if ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM) && (PNULL != bg_img))
    {
        *bg_img = IMAGE_WIDGET_SAM_TIME_BG;
    }
#endif    
}

/*****************************************************************************/
//  Description : HandleWidgetScheduleWinMsg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetTimeWinMsg(
											   MMI_WIN_ID_T        win_id,        //IN:
											   MMI_MESSAGE_ID_E    msg_id,        //IN:
											   DPARAM            param        //IN:
										   )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    static MMI_MESSAGE_ID_E previous_msg_id = 0;
    
    SCI_TRACE_LOW("HandleWidgetTimeWinMsg msg_id = %x",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        TM_GetSysTime(&s_sys_time);       
        break;

    case MSG_GRID_ADD_WIDGET:
        TM_GetSysTime(&s_sys_time);
        break;

    case MSG_WIDGET_TP_PRESS_DOWN:
        {
            previous_msg_id = msg_id; 
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_WIDGET_TP_PRESS_UP:
        if(MSG_WIDGET_TP_PRESS_DOWN == previous_msg_id)
        {
            MMIAPISET_SetTime();
        }
        previous_msg_id = msg_id; 
        break;

    case MSG_WIDGET_TP_PRESS_LONG_DOWN:
    case MSG_WIDGET_PAGE_SLIDE_START:   
        result = MMI_RESULT_FALSE;        
        previous_msg_id = msg_id; 
        break;
        
    case MSG_APP_WEB:
        MMIAPISET_SetTime();
        break;

    case MSG_LOSE_FOCUS:
        if(0 != s_anim_timer_id && s_anim_frame_id <= TIME_UPDATE_SE_FRAME_NUM + HOUR_PAGE_OFFSET_FRAME_NUM)
        {
            StopTimeUpdateSe();
        }
        break;

    case MSG_IDLE_UPDATE_DATETIME:
    case MSG_FULL_PAINT:
        DisplayTimeUI(win_id);
        DisplayCurrentTime();
        
#ifdef MMI_SAM_THEME
        if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)
#endif
        {
            MMIAPIWIDGET_Time_HandleTimeUpdate();
        }
        break;
        
    case MSG_GRID_REMOVE_WIDGET:
        RealeaseAllLayer();
        break;
       
    case MSG_CLOSE_WINDOW:
        RealeaseAllLayer();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
/*****************************************************************************/
//    Description : 创建时间翻页特效Layer
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateTimeBoxLayer(GUI_LCD_DEV_INFO *layer_ptr, 
                                    int16 offset_x, 
                                    int16 offset_y,
                                    uint16 img_w,
                                    uint16 img_h,
                                    BOOLEAN is_min)
{
    BOOLEAN             result          = FALSE;
    UILAYER_CREATE_T    create_info     = {0};
    GUI_LCD_DEV_INFO    layer_handle    = {0};
    GUI_RECT_T          win_rect        = {0};
    GUI_POINT_T         display_point   = {0};
    MMI_IMAGE_ID_T      img_id          = 0;;

    if (PNULL != layer_ptr)
    {
        MMK_GetWinRect(g_time_widget.win_id, &win_rect);
        
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = MAIN_IDLE_WIN_ID;
        create_info.offset_x        = win_rect.left + offset_x; 
        create_info.offset_y        = win_rect.top + offset_y;
        create_info.width           = img_w;
        create_info.height          = img_h;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format          = UILAYER_MEM_FORMAT_DOUBLE;

        if(is_min)
        {
            img_id = IMAGE_WIDGET_TIME_R;
        }
        else
        {
            img_id = IMAGE_WIDGET_TIME_L;
        }
        if (UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, &layer_handle))
        {
            display_point.x = win_rect.left + offset_x;
            display_point.y = win_rect.top  + offset_y;
            GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &layer_handle);

            *layer_ptr = layer_handle;
            
            result = TRUE;
        }
        else
        {
            RealeaseAllLayer();
            //SCI_TRACE_LOW:"[time]CreateTimeBoxLayer failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_TIME_295_112_2_18_3_8_42_240,(uint8*)"");
        }
    }

    return result;
}
/*****************************************************************************/
//  Description : DisplayAxleIcons
//  Global resource dependence : 
//  Author:
//  Note: 显示widget上的三个轴
/*****************************************************************************/
LOCAL void DisplayAxleIcons(void)
{
    uint16         x  = 0;
    GUI_POINT_T             point        = {0};
    GUI_LCD_DEV_INFO        time_layer   = {0};
    GUI_RECT_T              win_rect     = {0};
    uint8                   i            = 0;
    MMI_WIN_ID_T           win_id       = WIDGET_TIME_WIN_ID;
    MMI_IMAGE_ID_T         icon_id      = IMAGE_WIDGET_TIME_ZHOU;
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &time_layer);

    x = win_rect.left + MMIWIDGET_TIME_AXLE_X;
    point.y = win_rect.top + MMIWIDGET_TIME_AXLE_Y - 1;
    for(i = 0; i < MMIWIDGET_TIME_AXLE_NUM; i++)
    {
        point.x  = x + (MMIWIDGET_TIME_X_BETWEEN_TWO * i);
        GUIRES_DisplayImg(&point, 
                            PNULL, 
                            PNULL, 
                            win_id, 
                            icon_id, 
                            &time_layer);
    }
        
}
/*****************************************************************************/
//  Description : DisplayTimeUI
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void  DisplayTimeUI(MMI_WIN_ID_T     win_id)
{
    GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};
    GUI_RECT_T              win_rect     = {0};
    MMI_IMAGE_ID_T          bg_img       = IMAGE_WIDGET_TIME_BG;    

    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&lcd_dev_info))
    {
    	UILAYER_ClearRect(&lcd_dev_info, win_rect);
    }

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM) 
    {
        bg_img = IMAGE_WIDGET_SAM_TIME_BG;
    }
#endif
    
    GUIRES_DisplayImg(PNULL, 
                        &win_rect, 
                        PNULL, 
                        win_id, 
                        bg_img, 
                        &lcd_dev_info);  
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)                       
#endif    
    {
        DisplayAxleIcons();    
    }
    
    DisplayWeekAndDay();
}

/*****************************************************************************/
//    Description :显示当前时间。
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL void DisplayCurrentTime(void)
{
    GUI_LCD_DEV_INFO    time_widget_layer = {0};
    GUI_RECT_T          win_rect        = {0};
    GUI_POINT_T         display_point   = {0};
    MMI_IMAGE_ID_T      img_id          = 0;
    SCI_TIME_T          sys_time        = s_sys_time;
    uint16              time_w          = 0;
    uint16              time_h          = 0;
    uint16              time_bg_h       = 0;
    uint16              time_bg_w       = 0;
#ifdef MMI_SAM_THEME
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUISTR_STYLE_T      str_style = {0};
    GUI_RECT_T          dis_rect = {0};    
    GUISTR_INFO_T       str_info = {0};
    uint8               time_str[10] = {0};
    wchar               time_wstr[10] = {0};    
    MMI_STRING_T        time_disp_str    = {0};    
#endif
    BOOLEAN             b_am            = TRUE;
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();

    MMK_GetWinRect(g_time_widget.win_id, &win_rect);
    MMK_GetWinLcdDevInfo(g_time_widget.win_id, &time_widget_layer);
    
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)                       
#endif
    {
        //12小时制
        if(time_display_type == MMISET_TIME_12HOURS)
        {
            if ((12 > s_sys_time.hour))//AM
            {
                if ((1 > s_sys_time.hour))//00:00-1:00
                {
                    s_sys_time.hour = s_sys_time.hour + 12;
                }
            }
            else//PM
            {
                if ((12 <= s_sys_time.hour) && (13 > s_sys_time.hour))//12:00-13:00
                {
                }
                else
                {
                    s_sys_time.hour = s_sys_time.hour-12;
                }
                b_am = FALSE;
            }   
        }
    
        sys_time  = s_sys_time;
        GUIRES_GetImgWidthHeight(&time_w, &time_h, IMAGE_WIDGET_TIME_0, g_time_widget.win_id);
        GUIRES_GetImgWidthHeight(&time_bg_w, &time_bg_h, IMAGE_WIDGET_TIME_L, g_time_widget.win_id);
    
        //SCI_TRACE_LOW:"[time]:DisplayCurrentTime---%02d : %02d ---"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_TIME_375_112_2_18_3_8_42_241,(uint8*)"dd" ,sys_time.hour, sys_time.min);
    
        //小时的背景
        display_point.x = win_rect.left + MMIWIDGET_TIME_HOUR_BG_OFFSET_X;
        display_point.y = win_rect.top + MMIWIDGET_TIME_HOUR_BG_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, IMAGE_WIDGET_TIME_L, &time_widget_layer);
        
        //小时十位数
        img_id = s_time_img[sys_time.hour/ 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_HOUR_OFFSET_X;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &time_widget_layer);
        
        //小时个位数
        img_id = s_time_img[sys_time.hour % 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_HOUR_OFFSET_X + time_w + 2;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &time_widget_layer);
    
        //分钟的背景
        display_point.x = win_rect.left + MMIWIDGET_TIME_MIN_BG_OFFSET_X;
        display_point.y = win_rect.top + MMIWIDGET_TIME_HOUR_BG_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, IMAGE_WIDGET_TIME_R, &time_widget_layer);
        //分钟十位数
        img_id = s_time_img[sys_time.min / 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_MIN_OFFSET_X;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &time_widget_layer);
        
        //分钟个位数
        img_id = s_time_img[sys_time.min % 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_MIN_OFFSET_X + time_w + 2;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &time_widget_layer);
        DisplayAxleIcons();    
        //am/pm
        DisplayAmPmString(b_am,display_point,time_w,time_h,&time_widget_layer);
    }
#ifdef MMI_SAM_THEME
    else
    {
        str_style.font = SONG_FONT_64;
        str_style.font_color = MMI_WHITE_COLOR;
        MMIAPISET_GetTimeStr24(time_str);
    
        MMI_STRNTOWSTR(time_wstr, 10, time_str, 10, strlen((char*)time_str));
    
        time_disp_str.wstr_ptr = time_wstr;
        time_disp_str.wstr_len = strlen((char*)time_str);
        
        GUISTR_GetStringInfo(&str_style, &time_disp_str, text_state, &str_info);
        dis_rect.top = win_rect.top + MMIWIDGET_SAM_TIME_HOUR_OFFSET_Y;
        dis_rect.left = win_rect.left + MMIWIDGET_SAM_TIME_HOUR_OFFSET_X;
        dis_rect.right = dis_rect.left + 200;
        dis_rect.bottom = dis_rect.top + str_info.height;
        GUISTR_DrawTextToLCDInRect(&time_widget_layer, 
                                    &dis_rect, 
                                    &dis_rect, 
                                    &time_disp_str, 
                                    &str_style, 
                                    text_state, 
                                    GUISTR_TEXT_DIR_AUTO);
    }
#endif

    MMITHEME_StoreUpdateRect(&time_widget_layer, win_rect);
}
/*****************************************************************************/
//    Description :在时间widget上显示星期和日期
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL void DisplayWeekAndDay(void)
{
    SCI_DATE_T          sys_date = {0};
    MMI_STRING_T        week_str    = {0};
    MMI_STRING_T        data_dis_str    = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_LCD_DEV_INFO    time_widget_layer = {0};
    GUISTR_STYLE_T      str_style = {0};
    GUI_RECT_T          dis_rect = {0};
    GUI_RECT_T          win_rect = {0};
    GUISTR_INFO_T       str_info = {0};
    uint8               datestr[MMIWIDGET_TIME_DATE_MAX_LEN] = {0};
    wchar               datewstr[MMIWIDGET_TIME_DATE_MAX_LEN] = {0};
    uint16              datewstr_len = 0;   
    int16               week_offsetX = 0; 
    int16               week_offsetY = 0;
    int16               date_offsetX = 0;
#ifdef MMI_SAM_THEME
    MMISET_DATE_DISPLAY_TYPE_E  date_display_type   = MMISET_DATE_YMD;
    MMI_TEXT_ID_T       sam_week_text[] = {
                                           TXT_IDLE_SUNDAY,
                                           TXT_IDLE_MONDAY, 
                                           TXT_IDLE_TUESDAY,
                                           TXT_IDLE_WEDNESDAY,
                                           TXT_IDLE_THURSDAY,
                                           TXT_IDLE_FRIDAY,
                                           TXT_IDLE_SATURDAY 
                                           };
#endif

    MMI_TEXT_ID_T       week_text[] = {TXT_SUNDAY, 
                                        TXT_MONDAY,
                                        TXT_TUESDAY,
                                        TXT_WEDNESDAY,
                                        TXT_THURSDAY,
                                        TXT_FRIDAY,
                                        TXT_SATURDAY
                                        };

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        week_offsetX = MMIWIDGET_SAM_TIME_WEEK_OFFSET_X; 
        week_offsetY = MMIWIDGET_SAM_TIME_WEEK_OFFSET_Y;
        date_offsetX = MMIWIDGET_SAM_TIME_DATE_OFFSET_X;
        str_style.font = SONG_FONT_18;        
    }
    else
#endif
    {
        week_offsetX = MMIWIDGET_TIME_WEEK_OFFSET_X; 
        week_offsetY = MMIWIDGET_TIME_WEEK_OFFSET_Y;
        date_offsetX = MMIWIDGET_TIME_DATE_OFFSET_X;    
        str_style.font = MMI_DEFAULT_SMALL_FONT;
    }
    
    str_style.font_color = MMI_WHITE_COLOR;
    MMK_GetWinLcdDevInfo(g_time_widget.win_id, &time_widget_layer);
    MMK_GetWinRect(g_time_widget.win_id, &win_rect);
    TM_GetSysDate(&sys_date);

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        MMI_GetLabelTextByLang(sam_week_text[sys_date.wday],&week_str);    
    }
    else
#endif
    {
        MMI_GetLabelTextByLang(week_text[sys_date.wday],&week_str);
    }
    
    GUISTR_GetStringInfo(&str_style, &week_str, text_state, &str_info);
    dis_rect.top = win_rect.top + week_offsetY;
    dis_rect.left = win_rect.left + week_offsetX;
    dis_rect.right = dis_rect.left + MMIWIDGET_TIME_WEEK_WIDTH;
    dis_rect.bottom = dis_rect.top + str_info.height;
    GUISTR_DrawTextToLCDInRect(&time_widget_layer, 
                                &dis_rect, 
                                &dis_rect, 
                                &week_str, 
                                &str_style, 
                                text_state, 
                                GUISTR_TEXT_DIR_AUTO);

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        date_display_type = MMIAPISET_GetDateDisplayType();
        
        SCI_MEMSET(datestr,0,MMIWIDGET_TIME_DATE_MAX_LEN);

        if (MMISET_DATE_DMY == date_display_type)
        {
            sprintf((char*) datestr, "%02d%s%02d", sys_date.mday, "/",sys_date.mon);
        }
        else
        {
            sprintf((char*) datestr, "%02d%s%02d", sys_date.mon, "/",sys_date.mday);
        }
    }
    else
#endif
    {
        MMIAPISET_GetCurrentDateStr(datestr,'-',MMIWIDGET_TIME_DATE_MAX_LEN);  
    }
    
    datewstr_len = strlen((char*)datestr);
    if(0 == datewstr_len)
    {
        //SCI_TRACE_LOW:"DisplayWeekAndDay ERROR! datewstr_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_TIME_458_112_2_18_3_8_42_242,(uint8*)"d",datewstr_len);
        return;
    }
    //convert string
    MMI_STRNTOWSTR(datewstr,MMIWIDGET_TIME_DATE_MAX_LEN,
        datestr,MMIWIDGET_TIME_DATE_MAX_LEN,
        datewstr_len);

    data_dis_str.wstr_ptr = datewstr;
    data_dis_str.wstr_len = datewstr_len;

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {    
        dis_rect.top  += str_info.height;
    }
#endif

    dis_rect.left = win_rect.left + date_offsetX;
    dis_rect.right = dis_rect.left + MMIWIDGET_TIME_DATE_WIDTH;
    dis_rect.bottom = dis_rect.top + str_info.height;
    str_style.align = ALIGN_LEFT;
    GUISTR_DrawTextToLCDInRect(&time_widget_layer, 
                                &dis_rect, 
                                &dis_rect, 
                                &data_dis_str, 
                                &str_style, 
                                text_state, 
                                GUISTR_TEXT_DIR_AUTO);
    
}

/*****************************************************************************/
//    Description :处理时间更新消息
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIDGET_Time_HandleTimeUpdate(void)
{
    GUI_RECT_T  rect        = {0};
    GUI_RECT_T  win_rect    = {0};
    GUI_RECT_T  scr_rect    = MMITHEME_GetFullScreenRect();


    if(!MMK_IsFocusWin(MAIN_IDLE_WIN_ID)) 
    {
        return FALSE;
    }

#if defined MMI_GRID_IDLE_SUPPORT
    if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
    {
        if(MMIGRID_IsForbidPaint(MMIWIDGET_TIME_ID))
        {
            return FALSE;
        }
    }
#endif
        
#if defined QBTHEME_SUPPORT
    if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
    {
        if(MMIQBIDLE_IsForbidPaint(MMIWIDGET_TIME_ID))
        {
            return FALSE;
        }
    }
#endif

  
    MMK_GetWinRect(WIDGET_TIME_WIN_ID, &win_rect);
    
    if(GUI_IntersectRect(&rect, win_rect, scr_rect))        
    {
        if(PlayTimeUpdateSe(s_sys_time))
        {
            return TRUE;
        }
        else
        {
            TM_GetSysTime(&s_sys_time);
            DisplayCurrentTime();
        }
    }
    else
    {
        TM_GetSysTime(&s_sys_time);
        DisplayCurrentTime();
    }
  
    return FALSE;
}
/*****************************************************************************/
//    Description :画时间的背景
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL void DrawTimeBg(
    GUI_LCD_DEV_INFO *layer_handle, 
    int16 offset_x, 
    int16 offset_y,
    BOOLEAN is_min_bg
)
{
    GUI_RECT_T  win_rect = {0};
    MMI_IMAGE_ID_T  img_id = 0;
    GUI_POINT_T     display_point   = {0};

    if(UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(layer_handle))
    {
        return;
    }

    if(is_min_bg)
    {
        img_id = IMAGE_WIDGET_TIME_R;
    }
    else
    {
        img_id = IMAGE_WIDGET_TIME_L;
    }
    UILAYER_Clear(layer_handle); 
        
    MMK_GetWinRect(g_time_widget.win_id, &win_rect);
    display_point.x = win_rect.left + offset_x;
    display_point.y = win_rect.top  + offset_y;
    
    UILAYER_SetLayerPosition(layer_handle,display_point.x,display_point.y);
    GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, layer_handle);
}
/*****************************************************************************/
//  Discription: Start time update se timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopTimeUpdateSeTimer(void)
{
    //SCI_TRACE_LOW:"[weather]:Weather_StopSockTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_TIME_576_112_2_18_3_8_42_243,(uint8*)"");

    if(0 != s_anim_timer_id)
    {
        MMK_StopTimer(s_anim_timer_id);
        s_anim_timer_id = 0;
    }
}

/*****************************************************************************/
//    Description :停止播放时间更新特效
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL void StopTimeUpdateSe(void)
{
    TM_GetSysTime(&s_sys_time);
    StopTimeUpdateSeTimer();
    DisplayCurrentTime();
    RealeaseAllLayer();
    return;
}

/*****************************************************************************/
//  Description : CalcuteOneFrame
//  Global resource dependence : none
//  Author: 
//  Note:输出计算结果的layer
/*****************************************************************************/
LOCAL void CalcuteOneFrame(
    uint8  frame_id,                          //[in]第几帧      
    uint16 width,           
    uint16 height,
    GUI_LCD_DEV_INFO*  old_src_layer_ptr,     //[in]更新前的时间
    GUI_LCD_DEV_INFO*  new_src_layer_ptr,     //[in]更新后的时间
    GUI_LCD_DEV_INFO*  dst_layer_ptr          //[out]输出计算结果的Layer
)
{
    S3D_ROTATION_GEOMETRY_T       s3d_geometry = {0};
    S3D_ROTATION_IMG_T            s3d_src_img = {0};
    S3D_ROTATION_INOUT_IMG_INFO_T s3d_img = {0};
    UILAYER_INFO_T layer_info = {0};
    int16   time_se_angle_able[]={200,240,278,322,374,433,501,579,668,769,882,1008,1147,1297,1458,1625};

    s3d_geometry.angle = TIME_ANIM_MAX_ANGLE-time_se_angle_able[frame_id];

    s3d_geometry.img_size.w = width;//NUM_WIDTH;
    s3d_geometry.img_size.h = height;//NUM_HEIGHT;
    s3d_geometry.img_size.x = 0;
    s3d_geometry.img_size.y = height/2;

    s3d_geometry.eye.x = width/2;
    s3d_geometry.eye.y = height;
    s3d_geometry.eye.z = -MMI_MAINSCREEN_WIDTH*50;

    s3d_geometry.axis.x = width/2;
    s3d_geometry.axis.y = height;
    s3d_geometry.axis.z = MMI_MAINSCREEN_HEIGHT*0;
    s3d_geometry.axis.axis = 0;

    s3d_geometry.is_axis_use_default = SCI_FALSE;
    s3d_geometry.is_eye_use_default = SCI_FALSE;

    s3d_src_img.color       = 0;
    s3d_src_img.src_type    = S3D_DATA_ARGB888;
    
    if(s3d_geometry.angle > TIME_ANIM_MAX_ANGLE/2)
    {
        s3d_src_img.src_ptr = UILAYER_GetLayerBufferPtr(old_src_layer_ptr) ;
        if (PNULL == s3d_src_img.src_ptr)
        {
            return;
        }
    }
    else
    {
		s3d_geometry.img_size.y = -height/2;
		s3d_img.color_ptr = UILAYER_GetLayerBufferPtr(dst_layer_ptr);
        UILAYER_GetLayerInfo(dst_layer_ptr, &layer_info);
        if ((PNULL == s3d_img.color_ptr) 
        || (width > layer_info.layer_width) || (height > layer_info.layer_height)
        || layer_info.data_type == DATA_TYPE_RGB565)
        {
            return;
        }
        s3d_img.color_ptr = (uint8*)s3d_img.color_ptr + (width * (height) * sizeof(uint32));
		s3d_geometry.angle = TIME_ANIM_MAX_ANGLE - s3d_geometry.angle;
        s3d_src_img.src_ptr = UILAYER_GetLayerBufferPtr(new_src_layer_ptr);
        UILAYER_GetLayerInfo(new_src_layer_ptr, &layer_info);
        if ((PNULL == s3d_src_img.src_ptr) || (width > layer_info.layer_width) || (height > layer_info.layer_height))
        {
            return;
        }        
        s3d_src_img.src_ptr = (uint8*)s3d_src_img.src_ptr + (width * (height) * sizeof(uint32));
    }

    s3d_img.size.w = width;
    s3d_img.size.h = height*2;
    s3d_img.color_ptr = UILAYER_GetLayerBufferPtr(dst_layer_ptr);
    if (PNULL == s3d_img.color_ptr)
    {
        return;
    }
    UILAYER_Clear(dst_layer_ptr);
    S3D_ImageMidAxisRotation(&s3d_geometry, &s3d_src_img, &s3d_img);

	s3d_geometry.img_size.y = height/2;

	s3d_src_img.src_ptr = UILAYER_GetLayerBufferPtr(new_src_layer_ptr);
    if (PNULL == s3d_src_img.src_ptr)
    {
        return;
    }
	s3d_geometry.angle = TIME_ANIM_MAX_ANGLE;
	S3D_ImageMidAxisRotation(&s3d_geometry,
							&s3d_src_img,
							&s3d_img);

    s3d_img.color_ptr = UILAYER_GetLayerBufferPtr(dst_layer_ptr);
    UILAYER_GetLayerInfo(dst_layer_ptr, &layer_info);
    if ((PNULL == s3d_src_img.src_ptr) || (width > layer_info.layer_width) || (height > layer_info.layer_height))
    {
        return;
    }
    s3d_img.color_ptr = (uint8*)s3d_img.color_ptr + (width * (height) * sizeof(uint32));
    
	s3d_src_img.src_ptr = UILAYER_GetLayerBufferPtr(old_src_layer_ptr);
    UILAYER_GetLayerInfo(old_src_layer_ptr, &layer_info);
    if ((PNULL == s3d_src_img.src_ptr) || (width > layer_info.layer_width) || (height > layer_info.layer_height))
    {
        return;
    }
    s3d_src_img.src_ptr = (uint8*)s3d_src_img.src_ptr + (width * (height) * sizeof(uint32));
	s3d_geometry.angle = TIME_ANIM_MAX_ANGLE;
	S3D_ImageMidAxisRotation(&s3d_geometry,
							&s3d_src_img,
							&s3d_img);
}

/*****************************************************************************/
//  Description : HandleTimeUpdateSeTimer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void HandleTimeUpdateSeTimer(
    uint8  timer_id,
    uint32 param
)
{
    GUI_RECT_T      win_rect    = {0};
    GUI_RECT_T      minute_rect = {0};
    GUI_RECT_T      hour_rect   = {0};
    GUI_RECT_T      rect        = {0};
    GUI_RECT_T      scr_rect    = MMITHEME_GetFullScreenRect();
    uint16          time_box_h  = 0;
    uint16          time_box_w  = 0;
    
    //SCI_TRACE_LOW:"[time]:HandleTimeUpdateTimer---%d/16---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_TIME_727_112_2_18_3_8_43_244,(uint8*)"d" ,s_anim_frame_id );
    if(!MMK_IsFocusWin(MAIN_IDLE_WIN_ID)) 
    {
        return ;
    }

#if defined MMI_GRID_IDLE_SUPPORT
    if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
    {
        if(MMIGRID_IsForbidPaint(MMIWIDGET_TIME_ID))
        {
            return ;
        }
    }
#endif
        
#if defined QBTHEME_SUPPORT
    if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
    {
        if(MMIQBIDLE_IsForbidPaint(MMIWIDGET_TIME_ID))
        {
            return ;
        }
    }
#endif
    
    GUIRES_GetImgWidthHeight(&time_box_w, &time_box_h, IMAGE_WIDGET_TIME_L, g_time_widget.win_id);
    MMK_GetWinRect(WIDGET_TIME_WIN_ID, &win_rect);
    
    if(!GUI_IntersectRect(&rect, win_rect, scr_rect))
    {
        return ;
    }
    
    if(s_anim_frame_id <= TIME_UPDATE_SE_FRAME_NUM + HOUR_PAGE_OFFSET_FRAME_NUM)
    {
        GUI_LCD_DEV_INFO    widget_layer = {0};
        MMK_GetWinLcdDevInfo(g_time_widget.win_id, &widget_layer);
        minute_rect.left   = win_rect.left + MMIWIDGET_TIME_MIN_BG_OFFSET_X;
        minute_rect.top    = win_rect.top  + MMIWIDGET_TIME_HOUR_BG_OFFSET_Y;
        minute_rect.right  = minute_rect.left + time_box_w;
        minute_rect.bottom = minute_rect.top  + time_box_h;
        hour_rect.left   = win_rect.left + MMIWIDGET_TIME_HOUR_BG_OFFSET_X;
        hour_rect.top    = win_rect.top  + MMIWIDGET_TIME_HOUR_BG_OFFSET_Y;
        hour_rect.right  = hour_rect.left + time_box_w;
        hour_rect.bottom = hour_rect.top  + time_box_h;
        if(s_is_h_changed)//小时有更新
        {
            if(s_anim_frame_id < HOUR_PAGE_OFFSET_FRAME_NUM)
            {
                //Minute
                CalcuteOneFrame(s_anim_frame_id, 
                                time_box_w, 
                                time_box_h/2,
                                &s_time_min_old_layer, 
                                &s_time_min_new_layer, 
                                &s_time_min_calc_layer);
                UILAYER_BltLayerToLayer(&widget_layer, &s_time_min_calc_layer, &minute_rect, FALSE);
                MMITHEME_StoreUpdateRect(&widget_layer, minute_rect);
                
                //Hour
                UILAYER_BltLayerToLayer(&widget_layer, &s_time_hour_old_layer, &hour_rect, FALSE);
                MMITHEME_StoreUpdateRect(&widget_layer, hour_rect);
                DisplayAxleIcons();
                
            }
            else if(s_anim_frame_id < TIME_UPDATE_SE_FRAME_NUM)
            {
                //Minute
                CalcuteOneFrame(s_anim_frame_id, 
                                time_box_w, 
                                time_box_h/2,
                                &s_time_min_old_layer, 
                                &s_time_min_new_layer, 
                                &s_time_min_calc_layer);
                UILAYER_BltLayerToLayer(&widget_layer, &s_time_min_calc_layer, &minute_rect, FALSE);
                MMITHEME_StoreUpdateRect(&widget_layer, minute_rect);
                
                //Hour
                CalcuteOneFrame(s_anim_frame_id - HOUR_PAGE_OFFSET_FRAME_NUM, 
                                time_box_w, 
                                time_box_h/2,
                                &s_time_hour_old_layer, 
                                &s_time_hour_new_layer, 
                                &s_time_hour_calc_layer);
                
                UILAYER_BltLayerToLayer(&widget_layer, &s_time_hour_calc_layer, &hour_rect, FALSE);
                MMITHEME_StoreUpdateRect(&widget_layer, hour_rect);
                DisplayAxleIcons();
            }
            else if(s_anim_frame_id < TIME_UPDATE_SE_FRAME_NUM + HOUR_PAGE_OFFSET_FRAME_NUM)
            {
                //Minute
                UILAYER_BltLayerToLayer(&widget_layer, &s_time_min_new_layer, &minute_rect, FALSE);
                MMITHEME_StoreUpdateRect(&widget_layer, minute_rect);
            
                //Hour
                CalcuteOneFrame(s_anim_frame_id - HOUR_PAGE_OFFSET_FRAME_NUM, 
                                time_box_w, 
                                time_box_h/2,
                                &s_time_hour_old_layer, 
                                &s_time_hour_new_layer, 
                                &s_time_hour_calc_layer);
                UILAYER_BltLayerToLayer(&widget_layer, &s_time_hour_calc_layer, &hour_rect, FALSE);
                MMITHEME_StoreUpdateRect(&widget_layer, hour_rect);
                DisplayAxleIcons();
            }
            else
            {
                UILAYER_BltLayerToLayer(&widget_layer, &s_time_hour_new_layer, &hour_rect, FALSE);
                MMITHEME_StoreUpdateRect(&widget_layer, hour_rect);
                DisplayAxleIcons();
            }

        }
        else //小时没变，只更新分钟
        {
			if(s_anim_frame_id < TIME_UPDATE_SE_FRAME_NUM)
			{
				//Hour
				UILAYER_BltLayerToLayer(&widget_layer, &s_time_hour_old_layer, &hour_rect, FALSE);
				MMITHEME_StoreUpdateRect(&widget_layer, hour_rect);
                
				//Minute
				CalcuteOneFrame(s_anim_frame_id, 
								time_box_w, 
								time_box_h/2,
								&s_time_min_old_layer, 
								&s_time_min_new_layer, 
								&s_time_min_calc_layer);
				UILAYER_BltLayerToLayer(&widget_layer, &s_time_min_calc_layer, &minute_rect, FALSE);
				MMITHEME_StoreUpdateRect(&widget_layer, minute_rect);
				DisplayAxleIcons();
			}
            else
            {
				UILAYER_BltLayerToLayer(&widget_layer, &s_time_min_new_layer, &minute_rect, FALSE);
				MMITHEME_StoreUpdateRect(&widget_layer, minute_rect);
				DisplayAxleIcons();
            }

        }

#ifdef MMIWIDGET_SUPPORT
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()) 
        {        
            MMIWIDGET_UpdateWidgetByWidgetId(MMIWIDGET_TIME_ID);
        }
#endif
        MMITHEME_UpdateRect();
        
        s_anim_frame_id++;
    }
    else
    {
        StopTimeUpdateSe();
    }
}

/*****************************************************************************/
//  Discription: Start time update se timer
//  Global resource dependence: None
//  Author: 
//  Note : 
/*****************************************************************************/
LOCAL void StartTimeUpdateSeTimer(void)
{
    if(0 != s_anim_timer_id)
    {
        MMK_StopTimer(s_anim_timer_id);
        s_anim_timer_id = 0;
    }

    s_anim_frame_id = 0;  
    
    s_anim_timer_id = MMK_CreateTimerCallback(TIME_UPDATE_SE_TIMER, 
                                            HandleTimeUpdateSeTimer, 
                                            0, 
                                            TRUE);

    //SCI_TRACE_LOW:"[TIME]:StartTimeUpdateSeTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_TIME_905_112_2_18_3_8_43_245,(uint8*)"");
}

/*****************************************************************************/
//    Description :播放时间更新特效
//    Global resource dependence : 
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayTimeUpdateSe(SCI_TIME_T pre_sys_time)
{
    GUI_RECT_T          win_rect        = {0};
    SCI_TIME_T          cur_sys_time    = {0};
    GUI_POINT_T         display_point   = {0};
    MMI_IMAGE_ID_T      img_id          = 0;
    uint16              time_w          = 0;
    uint16              time_h          = 0;
    BOOLEAN             b_am            = TRUE;
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();
    
    MMK_GetWinRect(g_time_widget.win_id, &win_rect);
    TM_GetSysTime(&cur_sys_time);
    //12小时制
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        if ((12 > cur_sys_time.hour))//AM
        {
            if ((1 > cur_sys_time.hour))//00:00-1:00
            {
                cur_sys_time.hour = cur_sys_time.hour + 12;
            }
        }
        else//PM
        {
            if ((12 <= cur_sys_time.hour) && (13 > cur_sys_time.hour))//12:00-13:00
            {
            }
            else
            {
                cur_sys_time.hour = cur_sys_time.hour - 12;
            }
            b_am = FALSE;
        }   
    }
    GUIRES_GetImgWidthHeight(&time_w, &time_h, IMAGE_WIDGET_TIME_0, g_time_widget.win_id);
    if(0 == s_anim_timer_id && (pre_sys_time.hour != cur_sys_time.hour || pre_sys_time.min != cur_sys_time.min))
    {       
        if(pre_sys_time.hour == cur_sys_time.hour)
        {
            s_is_h_changed = FALSE;
        }
        else
        {
            s_is_h_changed = TRUE;
        }

        if(!CreateAllTimeAnimLayer())//6层未创建成功，则不播放动画，直接显示当前时间
        {
            return FALSE;
        }

        // -1- ======= Create Old Time Layer ========
        DrawTimeBg(&s_time_min_old_layer,  MMIWIDGET_TIME_MIN_BG_OFFSET_X, MMIWIDGET_TIME_HOUR_BG_OFFSET_Y,TRUE);
        //分钟十位数
        img_id = s_time_img[pre_sys_time.min / 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_MIN_OFFSET_X;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &s_time_min_old_layer);
        //分钟个位数
        img_id = s_time_img[pre_sys_time.min % 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_MIN_OFFSET_X + time_w + 2;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &s_time_min_old_layer);
        //am/pm
        DisplayAmPmString(b_am,display_point,time_w,time_h,&s_time_min_old_layer);
        
        // -2- ======= Create New Time Layer ========
        DrawTimeBg(&s_time_min_new_layer,  MMIWIDGET_TIME_MIN_BG_OFFSET_X, MMIWIDGET_TIME_HOUR_BG_OFFSET_Y,TRUE);
        //分钟十位数
        img_id = s_time_img[cur_sys_time.min / 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_MIN_OFFSET_X;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &s_time_min_new_layer);
        //分钟个位数
        img_id = s_time_img[cur_sys_time.min % 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_MIN_OFFSET_X + time_w + 2;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &s_time_min_new_layer);
        //am/pm
        DisplayAmPmString(b_am,display_point,time_w,time_h,&s_time_min_new_layer);
        
        // -3- ======= Create temp layer used to calcute frame data========
        DrawTimeBg(&s_time_min_calc_layer,  MMIWIDGET_TIME_MIN_BG_OFFSET_X, MMIWIDGET_TIME_HOUR_BG_OFFSET_Y, TRUE);
        // -4- ======= hour Old ========
        DrawTimeBg(&s_time_hour_old_layer,  MMIWIDGET_TIME_HOUR_BG_OFFSET_X, MMIWIDGET_TIME_HOUR_BG_OFFSET_Y, FALSE);
         //小时十位数
        img_id = s_time_img[pre_sys_time.hour / 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_HOUR_OFFSET_X;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &s_time_hour_old_layer);
        //小时个位数
        img_id = s_time_img[pre_sys_time.hour % 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_HOUR_OFFSET_X + time_w + 2;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &s_time_hour_old_layer);

        // -5- ======= hour New ========
        DrawTimeBg(&s_time_hour_new_layer,  MMIWIDGET_TIME_HOUR_BG_OFFSET_X, MMIWIDGET_TIME_HOUR_BG_OFFSET_Y, FALSE);
        //小时十位数
        img_id = s_time_img[cur_sys_time.hour / 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_HOUR_OFFSET_X;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &s_time_hour_new_layer);
        //小时个位数
        img_id = s_time_img[cur_sys_time.hour % 10];
        display_point.x = win_rect.left + MMIWIDGET_TIME_HOUR_OFFSET_X + time_w + 2;
        display_point.y = win_rect.top  + MMIWIDGET_TIME_HOUR_OFFSET_Y;
        GUIRES_DisplayImg(&display_point, PNULL, PNULL, g_time_widget.win_id, img_id, &s_time_hour_new_layer);

        // -6- ======= hour calcute ========
        DrawTimeBg(&s_time_hour_calc_layer,  MMIWIDGET_TIME_HOUR_BG_OFFSET_X, MMIWIDGET_TIME_HOUR_BG_OFFSET_Y, FALSE);
        StartTimeUpdateSeTimer();

        return TRUE;    
    }

    return FALSE;
}

/*****************************************************************************/
//  Discription: 释放时间特效图层
//  Global resource dependence: None
//  Author: 
//  Note : 
/*****************************************************************************/
LOCAL void RealeaseAllLayer(void)
{
    GUI_LCD_DEV_INFO *layer_array[] = {&s_time_min_new_layer,
                                      &s_time_min_old_layer,
                                      &s_time_min_calc_layer,
                                      &s_time_hour_new_layer,
                                      &s_time_hour_old_layer,
                                      &s_time_hour_calc_layer};
    uint8 i = 0;
    
    for (i = 0; i < ARR_SIZE(layer_array); i++)
    {
        if (0 != layer_array[i]->block_id 
            && UILAYER_NULL_HANDLE != layer_array[i]->block_id)
        {
            UILAYER_RELEASELAYER(layer_array[i]);   /*lint !e506 !e774*/
        }
    }
    return;
}

/*****************************************************************************/
//  Discription: 创建时间特效图层
//  Global resource dependence: None
//  Author: 
//  Note : 一共需要6个层，只要有一个层没有创建成功，就会释放所有图层且返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN CreateAllTimeAnimLayer(void)
{
    uint16       min_img_h = 0;
    uint16       min_img_w = 0;
    uint16       hour_img_h = 0;
    uint16       hour_img_w = 0;
    MMI_WIN_ID_T  win_id = g_time_widget.win_id;
    
    GUIRES_GetImgWidthHeight(&min_img_w, &min_img_h, IMAGE_WIDGET_TIME_R, win_id);
    GUIRES_GetImgWidthHeight(&hour_img_w, &hour_img_h, IMAGE_WIDGET_TIME_L, win_id);
    // -1- ======= Create Old Time Layer ========
    if(UILAYER_NULL_HANDLE == s_time_min_old_layer.block_id)
    {
        if(!CreateTimeBoxLayer(&s_time_min_old_layer,  
                            MMIWIDGET_TIME_MIN_BG_OFFSET_X, 
                            MMIWIDGET_TIME_HOUR_BG_OFFSET_Y,
                            min_img_w,
                            min_img_h,
                            TRUE))
        {
            SCI_TRACE_LOW("CreateTimeBoxLayer s_time_min_old_layer failed");
            return FALSE;
        }

   }      
    // -2- ======= Create New Time Layer ========
    if(UILAYER_NULL_HANDLE == s_time_min_new_layer.block_id)
    {
        if(!CreateTimeBoxLayer(&s_time_min_new_layer,  
                            MMIWIDGET_TIME_MIN_BG_OFFSET_X, 
                            MMIWIDGET_TIME_HOUR_BG_OFFSET_Y,
                            min_img_w,
                            min_img_h,
                            TRUE))
        {
            SCI_TRACE_LOW("CreateTimeBoxLayer s_time_min_new_layer failed");
            return FALSE;
        }
    }
    // -3- ======= Create temp layer used to calcute frame data========
    if(UILAYER_NULL_HANDLE == s_time_min_calc_layer.block_id)
    {
        if(!CreateTimeBoxLayer(&s_time_min_calc_layer,  
                            MMIWIDGET_TIME_MIN_BG_OFFSET_X,
                            MMIWIDGET_TIME_HOUR_BG_OFFSET_Y,
                            min_img_w,
                            min_img_h,
                            TRUE))
        {
            SCI_TRACE_LOW("CreateTimeBoxLayer s_time_min_calc_layer failed");
            return FALSE;
        }
    }
     // -4- ======= hour Old ========
    if(UILAYER_NULL_HANDLE == s_time_hour_old_layer.block_id)
    {
        if(!CreateTimeBoxLayer(&s_time_hour_old_layer,  
                            MMIWIDGET_TIME_HOUR_BG_OFFSET_X, 
                            MMIWIDGET_TIME_HOUR_BG_OFFSET_Y,
                            hour_img_w,
                            hour_img_h,
                            FALSE))
        {
            SCI_TRACE_LOW("CreateTimeBoxLayer s_time_hour_old_layer failed");
            return FALSE;
        }
    }
    // -5- ======= hour New ========
    if(UILAYER_NULL_HANDLE == s_time_hour_new_layer.block_id)
    {
        if(!CreateTimeBoxLayer(&s_time_hour_new_layer,  
                            MMIWIDGET_TIME_HOUR_BG_OFFSET_X, 
                            MMIWIDGET_TIME_HOUR_BG_OFFSET_Y,
                            hour_img_w,
                            hour_img_h,
                            FALSE))
        {
            SCI_TRACE_LOW("CreateTimeBoxLayer s_time_hour_new_layer failed");
            return FALSE;
        }
    }
    // -6- ======= hour calcute ========
    if(UILAYER_NULL_HANDLE == s_time_hour_calc_layer.block_id)
    {
        if(!CreateTimeBoxLayer(&s_time_hour_calc_layer, 
                            MMIWIDGET_TIME_HOUR_BG_OFFSET_X, 
                            MMIWIDGET_TIME_HOUR_BG_OFFSET_Y,
                            hour_img_w,
                            hour_img_h,
                            FALSE))
        {
            SCI_TRACE_LOW("CreateTimeBoxLayer s_time_hour_calc_layer failed");
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Discription: DisplayAmPmString
//  Global resource dependence: None
//  Author: 
//  Note : 显示AM/PM字符 
/*****************************************************************************/
LOCAL void DisplayAmPmString(BOOLEAN is_am,
                                GUI_POINT_T  display_point,
                                uint16 time_w,
                                uint16 time_h,
                                GUI_LCD_DEV_INFO * lcd_info_ptr)
{
    MMI_STRING_T        time_pa_string  = {0};
    GUI_RECT_T          win_rect        = {0};
    GUI_RECT_T          pa_rect         = {0};
    GUISTR_STYLE_T      str_style       = {0};
    GUISTR_INFO_T       str_info        = {0};
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();
    GUISTR_STATE_T      pa_text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;

    MMK_GetWinRect(g_time_widget.win_id, &win_rect);
    
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        if(is_am)
        {
            time_pa_string.wstr_ptr = L"AM";
        }
        else
        {
            time_pa_string.wstr_ptr = L"PM";
        }
        time_pa_string.wstr_len = MMIAPICOM_Wstrlen(time_pa_string.wstr_ptr);

        str_style.font = MMI_DEFAULT_SMALL_FONT;
        str_style.font_color = MMI_BLACK_COLOR;

        GUISTR_GetStringInfo(&str_style, &time_pa_string, pa_text_state, &str_info);

        pa_rect = win_rect;
        
        pa_rect.top  = display_point.y + time_h + 2;
        pa_rect.bottom = pa_rect.top + str_info.height;
        pa_rect.right = display_point.x + time_w;
        pa_rect.left = pa_rect.right - str_info.width;
        
        GUISTR_DrawTextToLCDInRect(lcd_info_ptr, 
                                    &pa_rect, 
                                    &pa_rect, 
                                    &time_pa_string, 
                                    &str_style, 
                                    pa_text_state, 
                                    GUISTR_TEXT_DIR_AUTO
                                    );
    }      

}

#endif
