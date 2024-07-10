/*****************************************************************************
** File Name:      mmiwidget_dual_clock.c                                        *
** Author:                                                                   *
** Date:           10/15/2010                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/15/2010                      Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#ifdef MMI_WIDGET_DUAL_CLOCK
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
//#include "mmicc_text.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwidget_position.h"
//#include "mmiacc_text.h"
#include "mmiwclk_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MAX_MMIWIDGET_DUAL_CLOCK_TEXT_LEN              50
#if defined (MAINLCD_SIZE_320X480)
#define   TIME_IMG_RECT_GAP_WIDTH                      33                      
#define   TIME_IMG_RECT_GAP_HEIGHT                     31   
#elif defined(MAINLCD_SIZE_240X400)
#define   TIME_IMG_RECT_GAP_WIDTH                      24                      
#define   TIME_IMG_RECT_GAP_HEIGHT                     22   
#elif defined(MAINLCD_SIZE_240X320)
#define   TIME_IMG_RECT_GAP_WIDTH                      24                      
#define   TIME_IMG_RECT_GAP_HEIGHT                     22   
#else
#define   TIME_IMG_RECT_GAP_WIDTH                      26                      
#define   TIME_IMG_RECT_GAP_HEIGHT                    22   

#endif
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
//LOCAL GUI_RECT_T  s_time_rect={0};

//LOCAL GUI_RECT_T  s_date_rect={0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get dual clock text style.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetDualClockWidgetTextStyle(
                            GUISTR_STYLE_T *time_style_ptr,
                            GUISTR_STYLE_T *city_style_ptr
                            );

/*****************************************************************************/
//     Description : Handle WidgetClock Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetDualClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );


/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayDualClock(
                       MMI_WIN_ID_T win_id
                       );

LOCAL MMI_RESULT_E  MMIWIDGET_DualClock_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//  Description : get dual clock widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetDualClockWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_dual_clock_widget =
{
    MMIWIDGET_DUAL_CLOCK_ID, 
    HandleWidgetDualClockWinMsg, 
    PNULL, 
    PNULL,
    GetDualClockWidgetImage,
    WIDGET_DUAL_CLOCK_WIN_ID, 
    IMAGE_WIDGET_DUAL_CLOCK_SHORTCUT, 
    IMAGE_WIDGET_DUALCLOCK_BG,
    TXT_WIDGET_DUAL_CLOCK,
    50,//    75, 
    50,//    75 
    FALSE
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get dual clock widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetDualClockWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img)
{
    if (PNULL != bg_img)
    {
        *bg_img = IMAGE_WIDGET_DUALCLOCK_BG;
    }

    if (PNULL != sc_img)    
    {
        *sc_img = IMAGE_WIDGET_DUAL_CLOCK_SHORTCUT;
    }
    
#ifdef MMI_SAM_THEME	
    if ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM) && (PNULL != bg_img))
    {
        *bg_img = IMAGE_WIDGET_SAM_DUALCLOCK_BG;
    }
#endif    
}

/*****************************************************************************/
//     Description : Handle WidgetClock Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetDualClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_WIDGET_TP_PRESS_UP:
           recode= MMIWIDGET_DualClock_HandleTpPressUp(win_id,param);
           break;

    case MSG_APP_WEB:
         MMIAPIWCLK_OpenMainWin();
         break;

        
    case MSG_IDLE_UPDATE_DATETIME:
    case MSG_FULL_PAINT:
        DisplayDualClock(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}

/*****************************************************************************/
//  Description : convert clock time.
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SCI_TIME_T ConvertSysTime(MCC_WT_CITY_INFO_T *wlk_info)
{
    uint8 hour_char[3]  = {0};
    uint8 min_char[3]   = {0};
    SCI_TIME_T sys_time = {0};

    MMI_MEMCPY(&hour_char, 2, wlk_info->time, 2, 2);
    MMI_MEMCPY(&min_char, 2, wlk_info->time + 3, 2, 2); 

    sys_time.hour = atoi((char*) hour_char);  
    sys_time.min  = atoi((char*) min_char );  

    return sys_time;
}

/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayDualClock(
                       MMI_WIN_ID_T win_id
                       )
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    SCI_TIME_T                  sys_time        = {0};
    // SCI_DATE_T                  sys_date        = {0};
    GUISTR_STYLE_T time_text_style = {0};
    GUISTR_STYLE_T city_text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    char u8_time_str[MAX_MMIWIDGET_DUAL_CLOCK_TEXT_LEN] = {0};
    // char u8_date_str[MAX_MMIWIDGET_DUAL_CLOCK_TEXT_LEN] = {0};
    MMI_STRING_T time_str = {0};
    MMI_STRING_T city_str1 = {0};
    MMI_STRING_T city_str2 = {0};    
    GUI_RECT_T time1_rect = MMIWIDGET_DUAL_CLOCK_TIME1_RECT;
    GUI_RECT_T time2_rect = MMIWIDGET_DUAL_CLOCK_TIME2_RECT;
    GUI_RECT_T city1_rect = MMIWIDGET_DUAL_CLOCK_CITY1_RECT;
    GUI_RECT_T city2_rect = MMIWIDGET_DUAL_CLOCK_CITY2_RECT;    
    MCC_WT_CITY_INFO_T *wlk_city_info  = PNULL;
    GUISTR_INFO_T	 str_info={0};
    MMI_IMAGE_ID_T   bg_img = IMAGE_WIDGET_DUALCLOCK_BG;

    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&lcd_dev_info))
    {
        UILAYER_ClearRect(&lcd_dev_info, win_rect);
    }

#ifdef MMI_SAM_THEME	
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        bg_img = IMAGE_WIDGET_SAM_DUALCLOCK_BG;
        time1_rect.left   = MMIWIDGET_DUAL_SAM_CLOCK_TIME1_LEFT;
        time1_rect.top    = MMIWIDGET_DUAL_SAM_CLOCK_TIME1_TOP;
        time1_rect.right  = MMIWIDGET_DUAL_SAM_CLOCK_TIME1_RIGHT;        
        time1_rect.bottom = MMIWIDGET_DUAL_SAM_CLOCK_TIME1_BOTTOM;        
        
        time2_rect.top    = MMIWIDGET_DUAL_SAM_CLOCK_TIME2_TOP;
        time2_rect.bottom = MMIWIDGET_DUAL_SAM_CLOCK_TIME2_BOTTOM;
        time2_rect.left   = MMIWIDGET_DUAL_SAM_CLOCK_TIME2_LEFT;
        time2_rect.right  = MMIWIDGET_DUAL_SAM_CLOCK_TIME2_RIGHT;

        city1_rect.top    = MMIWIDGET_DUAL_SAM_CLOCK_CITY1_TOP;
        city1_rect.bottom = MMIWIDGET_DUAL_SAM_CLOCK_CITY1_BOTTOM;
        city1_rect.left   = MMIWIDGET_DUAL_SAM_CLOCK_CITY1_LEFT;
        city1_rect.right  = MMIWIDGET_DUAL_SAM_CLOCK_CITY1_RIGHT;
        
        city2_rect.top    = MMIWIDGET_DUAL_SAM_CLOCK_CITY2_TOP; 
        city2_rect.bottom = MMIWIDGET_DUAL_SAM_CLOCK_CITY2_BOTTOM;         
        city2_rect.left   = MMIWIDGET_DUAL_SAM_CLOCK_CITY2_LEFT;         
        city2_rect.right  = MMIWIDGET_DUAL_SAM_CLOCK_CITY2_RIGHT;                
    }
#endif    
    
    //ÏÔÊ¾Êý×ÖÊ±ÖÓ±³¾°     
    GetDualClockWidgetTextStyle(&time_text_style, &city_text_style);

    GUIRES_DisplayImg(
        PNULL,
        &win_rect,
        PNULL,
        win_id,
        bg_img,
        &lcd_dev_info
        );         
 
    //display time1.
    wlk_city_info = MMIAPIACC_GetLocalTimeZoneInfo();  
    if(wlk_city_info == PNULL)
    {
        return;
    }

#ifdef MMI_SAM_THEME	
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        time_text_style.align = ALIGN_HVMIDDLE;
        time_text_style.font       = SONG_FONT_32;
        time_text_style.font_color = MMI_WHITE_COLOR;
    }
#endif    
    sys_time = ConvertSysTime(wlk_city_info);

    time_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((MAX_MMIWIDGET_DUAL_CLOCK_TEXT_LEN+1)*sizeof(wchar));
    SCI_MEMSET(time_str.wstr_ptr,0,((MAX_MMIWIDGET_DUAL_CLOCK_TEXT_LEN+1)*sizeof(wchar)));  
    MMIAPISET_FormatTimeStrByTime(sys_time.hour, sys_time.min,(uint8*)u8_time_str,MAX_MMIWIDGET_DUAL_CLOCK_TEXT_LEN);
    MMIAPICOM_StrToWstr((const uint8*)u8_time_str, time_str.wstr_ptr);
    time_str.wstr_len = MMIAPICOM_Wstrlen(time_str.wstr_ptr);
                
    GUISTR_GetStringInfo(&time_text_style, &time_str, text_state,&str_info);	
    time1_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, time1_rect);

#ifdef MMI_SAM_THEME	
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {    
        time1_rect.right      = win_rect.right - 5;
        city_text_style.align = ALIGN_LEFT;
        city_text_style.font_color = MMI_WHITE_COLOR;
    }
#endif    
    
    GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&time1_rect,
		&time1_rect,
        (const MMI_STRING_T     *)&time_str,
        &time_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO);
    
    //display city one.
    MMI_GetLabelTextByLang(MMIAPIACC_GetLocalCityStringID(), &city_str1); 
    GUISTR_GetStringInfo(&city_text_style, &city_str1, text_state,&str_info);	
    city1_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, city1_rect);

#ifdef MMI_SAM_THEME	
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        city1_rect.right = win_rect.right - 10;
    }
#endif

    GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&city1_rect,
		&city1_rect,
        (const MMI_STRING_T     *)&city_str1,
        &city_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO);
    
    //display time2.
    wlk_city_info = MMIAPIACC_GetOtherTimeZoneInfo();
    if(wlk_city_info == PNULL) /*lint !e831*/
    {
        SCI_FREE(time_str.wstr_ptr);
        return;
    }
    sys_time = ConvertSysTime(wlk_city_info);  

    SCI_MEMSET(time_str.wstr_ptr,0,((MAX_MMIWIDGET_DUAL_CLOCK_TEXT_LEN+1)*sizeof(wchar)));  

    MMIAPISET_FormatTimeStrByTime(sys_time.hour, sys_time.min,(uint8*)u8_time_str,MAX_MMIWIDGET_DUAL_CLOCK_TEXT_LEN);
    MMIAPICOM_StrToWstr((const uint8*)u8_time_str, time_str.wstr_ptr);
    time_str.wstr_len = MMIAPICOM_Wstrlen(time_str.wstr_ptr);
                
    GUISTR_GetStringInfo(&time_text_style, &time_str, text_state,&str_info);	
    time2_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, time2_rect);

#ifdef MMI_SAM_THEME	
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        time2_rect.right = win_rect.right - 5;
        city_text_style.font_color = MMI_WHITE_COLOR;
    }
#endif    

    GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&time2_rect,
		&time2_rect,
        (const MMI_STRING_T     *)&time_str,
        &time_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO);

    //display city second.
    MMI_GetLabelTextByLang(MMIAPIACC_GetOtherCityStringID(), &city_str2); 
    GUISTR_GetStringInfo(&city_text_style, &city_str2, text_state,&str_info);	
    city2_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, city2_rect);    

#ifdef MMI_SAM_THEME	
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        city2_rect.right = win_rect.right - 10;
        city_text_style.align = ALIGN_RIGHT;      
    }
#endif

    GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&city2_rect,
		&city2_rect,
        (const MMI_STRING_T     *)&city_str2,
        &city_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO);

    SCI_FREE(time_str.wstr_ptr);
}



/*****************************************************************************/
//  Description : get clock text style.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetDualClockWidgetTextStyle(
                            GUISTR_STYLE_T *time_style_ptr,
                            GUISTR_STYLE_T *city_style_ptr
                            )
{
    MMITHEME_WIDGET_CLOCK_T clock_widget_theme = {0};

    MMITHEME_GetDualClockWidgetTheme(&clock_widget_theme);    

    //time
    if (PNULL != time_style_ptr)
    {
        time_style_ptr->effect = FONT_EFFECT_NONE; 
        time_style_ptr->angle = ANGLE_0;
        time_style_ptr->align = ALIGN_LVMIDDLE;
        time_style_ptr->font = clock_widget_theme.time_font;
        time_style_ptr->font_color = clock_widget_theme.clock_font_color;
    }

    //city
    if (PNULL != city_style_ptr)
    {
        city_style_ptr->effect = FONT_EFFECT_NONE; 
        city_style_ptr->angle = ANGLE_0;
        city_style_ptr->align = ALIGN_RVMIDDLE;
        city_style_ptr->font = clock_widget_theme.city_font;
        city_style_ptr->font_color = clock_widget_theme.city_font_color;
    }
}

/*****************************************************************************/
//  Description : handle tp up message.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_DualClock_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T win_rect = {0}; //s_time_rect;  

     point.x = MMK_GET_TP_X(param);
     point.y = MMK_GET_TP_Y(param);

    MMK_GetWinRect(win_id, &win_rect);
    
    if (GUI_PointIsInRect(point, win_rect))
    {
         MMIAPIWCLK_OpenMainWin();
    }

    return recode;
}
#endif
