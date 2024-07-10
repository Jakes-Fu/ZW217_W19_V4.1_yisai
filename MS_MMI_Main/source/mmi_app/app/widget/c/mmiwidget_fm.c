/*****************************************************************************
** File Name:      mmiwidget_fm.c                                        *
** Author:                                                                   *
** Date:                                              *
** Copyright:     *
** Description:   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**         Creat
******************************************************************************/


#define _MMIWIDGET_FM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#if defined(FM_SUPPORT)
#ifdef MMI_WIDGET_FM
#include "window_parse.h"
#include "guilabel.h"
#include "mmipub.h"
#include "mmifm_export.h"
#include "mmifm_text.h"
#include "mmifm_internal.h"
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
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmiwidget_position.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "gpio_prod_api.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//点中区域的标识
typedef enum
{
	    MMIWIDGET_FM_HIT_NONE,         //无
	    MMIWIDGET_FM_HIT_ENTRY,        //进入播放器
	    MMIWIDGET_FM_HIT_PREV,         //前一频道
	    MMIWIDGET_FM_HIT_NEXT,         //下一频道
	    MMIWIDGET_FM_HIT_PLAYPAUSE,    //播放暂停

	    MMIWIDGET_FM_HIT_MAX
}MMIWIDGET_FM_HIT_TYPE_E;   /*lint !e751 */

//点中区域的标识
LOCAL uint32 s_widget_fm_hit_type = MMIWIDGET_FM_HIT_NONE;

//LOCAL uint8 s_widget_fm_timer_id = 0;
//LOCAL int32 s_widget_fm_search_direct = 0;

#define MMIWIDGET_FM_CHANNEL_TITLE_ADD_MAX_LEN   10  //MMIFM_CHANNEL_TITLE_ADD_MAX_LEN
#define MMIWIDGET_FM_SEARCH_FREQ_PERIOD			 200
	
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN MMIWidgetFm_StartPlay(void);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  	Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetFmWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  	Author:
//    Note: 
/*****************************************************************************/

LOCAL void MMIWidgetFm_Play(void);

/*****************************************************************************/
//  Discription: handle press prev
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void MMIWidgetFm_Prev(MMI_WIN_ID_T        win_id);

/*****************************************************************************/
//  Discription: handle press next
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void MMIWidgetFm_Next(MMI_WIN_ID_T        win_id);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : UpdateFMSoftkey
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void UpdateFMSoftkey(MMI_WIN_ID_T win_id, BOOLEAN is_update);

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayFmPanel( MMI_WIN_ID_T     win_id );

LOCAL void ResetWidgetFm();
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
PUBLIC const MMIWIDGET_ITEM_INFO_T g_fm_widget =
{
	    MMIWIDGET_FM_ID, 
	    HandleWidgetFmWinMsg, 
        PNULL,
        ResetWidgetFm,
        PNULL,
	    WIDGET_FM_WIN_ID, 
	    IMAGE_WIDGET_FM_SHORTCUT, 
	    IMAGE_WIDGET_FM_BG, 
        TXT_WIDGET_FM,	    
	    0,//    75, 
	    0,//    75
        FALSE
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

LOCAL void ResetWidgetFm()
{
    s_widget_fm_hit_type = MMIWIDGET_FM_HIT_NONE;
}
#if 0
LOCAL void StartFmWidgetSearchFreqTimer(MMI_WIN_ID_T win_id,  int32 direct)
{
	if (0 == s_widget_fm_timer_id)
	{
		s_widget_fm_timer_id = MMK_CreateWinTimer(win_id, MMIWIDGET_FM_SEARCH_FREQ_PERIOD, FALSE);
		s_widget_fm_search_direct = direct;
	}
}

LOCAL void StopFmWidgetSearchFreqTimer(void)
{
	if (0 < s_widget_fm_timer_id)
	{
		MMK_StopTimer(s_widget_fm_timer_id);
		s_widget_fm_timer_id = 0;
	}
}
#endif
/*****************************************************************************/
//     Description : handle Tp Press Down
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    GUI_POINT_T		point = {0};
    GUI_RECT_T		playbutton_rect = MMIWIDGET_FM_PLAYBUTTON_RECT;
    GUI_RECT_T		button_prev_rect = MMIWIDGET_FM_LBUTTON_RECT;
    GUI_RECT_T		button_next_rect = MMIWIDGET_FM_RBUTTON_RECT;    
    GUI_RECT_T		name_rect = MMIWIDGET_FM_NAME_RECT; 
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if (GUI_PointIsInRect(point, playbutton_rect))
    {
        s_widget_fm_hit_type = MMIWIDGET_FM_HIT_PLAYPAUSE;
    }
    else if (GUI_PointIsInRect(point, button_prev_rect))
    {
        s_widget_fm_hit_type = MMIWIDGET_FM_HIT_PREV;
    }
    else if (GUI_PointIsInRect(point, button_next_rect))
    {
        s_widget_fm_hit_type = MMIWIDGET_FM_HIT_NEXT;
    }
    else if (GUI_PointIsInRect(point, name_rect))
    {
        s_widget_fm_hit_type = MMIWIDGET_FM_HIT_ENTRY;
    }
    else
    {
        s_widget_fm_hit_type = MMIWIDGET_FM_HIT_NONE;
        recode = MMI_RESULT_FALSE;
    }
    
    return recode;
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    GUI_POINT_T		point = {0};
    GUI_RECT_T		playbutton_rect = MMIWIDGET_FM_PLAYBUTTON_RECT;
    GUI_RECT_T		button_prev_rect = MMIWIDGET_FM_LBUTTON_RECT;
    GUI_RECT_T		button_next_rect = MMIWIDGET_FM_RBUTTON_RECT;   
    GUI_RECT_T		name_rect = MMIWIDGET_FM_NAME_RECT; 
    
    if(s_widget_fm_hit_type == MMIWIDGET_FM_HIT_NONE)
    {
         return MMI_RESULT_FALSE;
    }

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if (GUI_PointIsInRect(point, playbutton_rect)
        && s_widget_fm_hit_type == MMIWIDGET_FM_HIT_PLAYPAUSE)
    {
		//StopFmWidgetSearchFreqTimer();
        MMIWidgetFm_Play();	
    }
    else if (GUI_PointIsInRect(point, button_prev_rect)
        && s_widget_fm_hit_type == MMIWIDGET_FM_HIT_PREV)
    {		 
        MMIWidgetFm_Prev(win_id);
    }
    else if (GUI_PointIsInRect(point, button_next_rect)
        && s_widget_fm_hit_type == MMIWIDGET_FM_HIT_NEXT)
    {
        MMIWidgetFm_Next(win_id);
    }
    else if (GUI_PointIsInRect(point, name_rect)
        && s_widget_fm_hit_type == MMIWIDGET_FM_HIT_ENTRY)
    {
        MMIAPIFM_OpenMainWin();
    }
    
    s_widget_fm_hit_type = MMIWIDGET_FM_HIT_NONE;    
    return recode;
}

/*****************************************************************************/
//     Description : Handle WidgetFM Win Msg
//    Global resource dependence : 
//  	Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetFmWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_WIDGET_PAGE_SLIDE_START:  
        ResetWidgetFm();
        break;
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_WIDGET_TP_PRESS_DOWN:
        //点中显示图标按下效果
        recode = HandleTpPressDown(win_id, param);
        DisplayFmPanel(win_id);
        break;
        
    case MSG_WIDGET_TP_PRESS_UP:
        //处理hit消息
        recode = HandleTpPressUp(win_id, param);
        //DisplayFmPanel(win_id);        
        break;

    case MSG_WIDGET_TP_PRESS_LONG_UP:
        ResetWidgetFm();        
        //DisplayFmPanel(win_id);
        break;
#endif

#ifdef BAR_PHONE_WIDGET 
    case MSG_APP_UP:
        MMIWidgetFm_Prev(win_id);        
        DisplayFmPanel(win_id);
        break;
    case MSG_APP_DOWN:
        MMIWidgetFm_Next(win_id);        
        DisplayFmPanel(win_id);
        break;
#endif		
    case MSG_APP_WEB:
#ifdef BAR_PHONE_WIDGET 
        //StopFmWidgetSearchFreqTimer();
        MMIWidgetFm_Play();	        
        DisplayFmPanel(win_id);
#else
        MMIAPIFM_OpenMainWin();
#endif        
        break;

#if 0        
	case MSG_TIMER:
		if(*(uint8 *)param == s_widget_fm_timer_id)
		{
			uint16 ret = 0;
			StopFmWidgetSearchFreqTimer();
			
			ret = MMIFM_SearchValidFreq(s_widget_fm_search_direct);
			
			if (MMIFM_AUTOSEARCH_END == ret)
			{
				if (FM_STOP == MMIAPIFM_GetStatus())
				{
					MMIWidgetFm_StartPlay();
				}
				else
				{
					MMIAPIFM_Play();
				}
				
				if(FM_SUSPENDED == MMIAPIFM_GetStatus())
				{
					//MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_FM);
				}
			}
			else if (MMIFM_AUTOSEARCH_FAIL == ret)
			{
			}
			else
			{
				StartFmWidgetSearchFreqTimer(win_id, s_widget_fm_search_direct);
			}


		}
		break;
#endif

    case MSG_LCD_SWITCH:
    case MSG_LOSE_FOCUS:
		//StopFmWidgetSearchFreqTimer();
        ResetWidgetFm();
        break;

    case MSG_FULL_PAINT:	    
        DisplayFmPanel(win_id);
        break;
        
#ifdef BAR_PHONE_WIDGET 
    case MSG_WIDGET_UPDATE_SOFTKEY:
        UpdateFMSoftkey(win_id,FALSE);
        break;
#endif

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}

/*****************************************************************************/
//  Description : UpdateFMSoftkey
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
#ifdef BAR_PHONE_WIDGET 
LOCAL void UpdateFMSoftkey(MMI_WIN_ID_T win_id, BOOLEAN is_update)
{
    if(FM_PLAYING == MMIAPIFM_GetStatus())
    {           
            MMIWIDGET_SoftkeySetTextId(win_id,STXT_STOP,is_update);
    }
    else
    {
            MMIWIDGET_SoftkeySetTextId(win_id,TXT_FM_PLAY,is_update);
    }
}
#endif

/*****************************************************************************/
//  Description : display FM Panel
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayFmPanel(
                           MMI_WIN_ID_T     win_id
                           )
{
    GUI_RECT_T		win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_POINT_T		point = {0};
    MMI_IMAGE_ID_T  img_id = 0;
    MMIFM_CUR_INFO_T *cur_inof_ptr = PNULL;
    GUI_RECT_T		playbutton_rect   = MMIWIDGET_FM_PLAYBUTTON_RECT;
#ifdef BAR_PHONE_WIDGET 
    GUI_RECT_T		button_prev_rect  = MMIWIDGET_FM_LBUTTON_RECT;
    GUI_RECT_T		button_next_rect  = MMIWIDGET_FM_RBUTTON_RECT;
#endif    
    GUI_RECT_T		name_rect = MMIWIDGET_FM_NAME_RECT; 
    wchar            wfreq_number[MMIFM_FREQ_MAX_LEN + 1] = {0};
    uint8            freq_number[MMIFM_FREQ_MAX_LEN + 1] = {0};
    int32            str_len = 0;
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T string = {0};
    MMITHEME_WIDGET_FM_T fm_widget_theme = {0};
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    
    if (!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
    }

    //in case clear main layer
    if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&lcd_dev_info))
    {
        UILAYER_ClearRect(&lcd_dev_info, win_rect);
    }
        //显示背景图
        point.x = win_rect.left;
        point.y = win_rect.top;
        GUIRES_DisplayImg(&point, PNULL, PNULL, win_id,
            IMAGE_WIDGET_FM_BG, &lcd_dev_info);

        
        if(0 == MMIFM_GetValidChannelCount())
        {
            sprintf((char *)freq_number,"FM");
        }
        else
        {            
            cur_inof_ptr = MMIFM_GetFMCurrentInfo();
            if (PNULL != cur_inof_ptr)
            {
                sprintf((char *)freq_number,"%d.%d MHz",cur_inof_ptr->cur_freq/10, cur_inof_ptr->cur_freq%10);
            }
            else
            {
                sprintf((char *)freq_number,"88.8 MHz");
            }
		}
        str_len = strlen((char*)freq_number);
        str_len = MIN(MMIFM_FREQ_MAX_LEN, str_len);
        MMI_STRNTOWSTR(wfreq_number, MMIFM_FREQ_MAX_LEN, freq_number, MMIFM_FREQ_MAX_LEN, str_len);
        // display
        MMITHEME_GetFMWidgetTheme(&fm_widget_theme);
        text_style.align = ALIGN_HVMIDDLE;
        text_style.font = fm_widget_theme.name_font;
#ifdef BAR_PHONE_WIDGET 
        text_style.font_color = MMI_BLACK_COLOR;
#else        
        text_style.font_color = fm_widget_theme.name_color;
#endif        
		
        string.wstr_len = MMIAPICOM_Wstrlen(wfreq_number);
        string.wstr_ptr = wfreq_number;        
        //name_rect = win_rect;
        name_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, name_rect);
        name_rect.top = win_rect.top;        
        name_rect.bottom = win_rect.bottom;
        
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &name_rect,
            &name_rect,
            (const MMI_STRING_T     *)&string,
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO);        

        // PlayPause Button
        switch(MMIAPIFM_GetStatus())
        {
        case FM_PLAYING:
            if (MMIWIDGET_FM_HIT_PLAYPAUSE == s_widget_fm_hit_type)
            {
                img_id = IMAGE_WIDGET_FM_PAUSE_ON;
            }
            else
            {
#ifdef BAR_PHONE_WIDGET 
                img_id = IMAGE_WIDGET_FM_PAUSE;
#else
                img_id = IMAGE_WIDGET_FM_PAUSE_OFF;
#endif                
            }
            break;
        case FM_SUSPENDED:
        case FM_STOP:
        default:
            if (MMIWIDGET_FM_HIT_PLAYPAUSE == s_widget_fm_hit_type)
            {
                img_id = IMAGE_WIDGET_FM_PLAY_ON;
            }
            else
            {
#ifdef BAR_PHONE_WIDGET 
                img_id = IMAGE_WIDGET_FM_PLAY;
#else
                img_id = IMAGE_WIDGET_FM_PLAY_OFF;
#endif                
            }
            break;
        }
        playbutton_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, playbutton_rect);
        GUIRES_DisplayImg(PNULL, &playbutton_rect, PNULL, win_id, img_id, &lcd_dev_info);

#ifdef BAR_PHONE_WIDGET 
        // Prev Button
        if (MMIWIDGET_FM_HIT_PREV == s_widget_fm_hit_type) 
        {
            img_id = IMAGE_WIDGET_FM_PREV_ON;
        }
        else
        {
            img_id = IMAGE_WIDGET_FM_PREV_OFF; 
        }
        button_prev_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_prev_rect);
        GUIRES_DisplayImg(PNULL, &button_prev_rect, PNULL, win_id, img_id, &lcd_dev_info);
        // Next Button
        if (MMIWIDGET_FM_HIT_NEXT == s_widget_fm_hit_type)
        {
            img_id = IMAGE_WIDGET_FM_NEXT_ON;
        }
        else
        {
             img_id = IMAGE_WIDGET_FM_NEXT_OFF;
        }
        button_next_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_next_rect);
        GUIRES_DisplayImg(PNULL, &button_next_rect, PNULL, win_id, img_id, &lcd_dev_info);
        UpdateFMSoftkey(win_id, TRUE);
#endif

    
}

/*****************************************************************************/
//  Description : Fm is ready
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIWidgetFm_IsReady()
{
    if(MMITHEME_IsTVOpen())
    {
        //TV out, FM can't be used
        MMIPUB_OpenAlertWarningWin(TXT_FM_CANNOT_USE);
        return FALSE;
    }
    if(MMIAPISET_GetFlyMode())
    {
        MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST);	
        return FALSE;
    }
    
#ifndef WIN32
#ifdef MMI_FM_NEED_HEADSET   
    if(!GPIO_CheckHeadsetStatus())
    {
        //not headset, can not play     
#ifdef MMI_PDA_SUPPORT
        MMIPUB_DisplayTipsIDEx(PNULL, TXT_FM_INPUT_HEADSET, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
#else
        MMIPUB_OpenAlertWarningWin(TXT_FM_INPUT_HEADSET);
#endif
        return FALSE;
    }
#endif//MMI_FM_NEED_HEADSET
#endif//WIN32
        
        
    return TRUE;
}                         

/*****************************************************************************/
//  Description : Fm is start play
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIWidgetFm_StartPlay()
{    
    if (FM_PLAYING != MMIFM_GetStatus())
    {
        if(!MMIFM_Play(TRUE))
        {                
            MMIFM_Exit(FALSE);
            MMIPUB_OpenAlertFailWin(TXT_FM_INIT_FAIL);               

            return FALSE;
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Discription: handle press play
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void MMIWidgetFm_Play(void)
{
    if (MMIWidgetFm_IsReady())
    {
        if(FM_PLAYING == MMIAPIFM_GetStatus())
        {			
//#ifdef PDA_UI_DROPDOWN_WIN
//            MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_RUNNING_FM);
//#endif      
			MMIFM_Exit(FALSE);
        }
        else
        {
#ifdef MMI_TASK_MANAGER     
            MMI_APPLET_START_T start ={0};
            
            start.guid = SPRD_FM_APPLET_ID;
            start.state = MMI_APPLET_STATE_BG_HIDE_WIN|MMI_APPLET_STATE_START_BG|MMI_APPLET_STATE_TASK_MANAGER;
            
            MMK_StartApplet( &start );
#endif

#ifdef PDA_UI_DROPDOWN_WIN
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_FM);
#endif
            MMIWidgetFm_StartPlay();
        }
    }	
} 

/*****************************************************************************/
//  Discription: handle press prev
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void MMIWidgetFm_Prev(MMI_WIN_ID_T        win_id)
{
    if (MMIWidgetFm_IsReady())
    {
		//StopFmWidgetSearchFreqTimer();
		//StartFmWidgetSearchFreqTimer(win_id, -1);
        if(0 == MMIFM_GetValidChannelCount())
        {
            MMIAPIFM_AutosearchChannel();
        }
        else
        {
            if(MMIFM_GetValidChannelCount() > 1)
            {
                MMIFM_AdjustChannel((int)-1);
            }
        }
    }
}

/*****************************************************************************/
//  Discription: handle press next
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void MMIWidgetFm_Next(MMI_WIN_ID_T        win_id)
{
    if (MMIWidgetFm_IsReady())
    {
        if(0 == MMIFM_GetValidChannelCount())
        {
            MMIAPIFM_AutosearchChannel();
        }
        else
        {
            if(MMIFM_GetValidChannelCount() > 1)
            {
                MMIFM_AdjustChannel((int)1);
            }
        }
    }
}
#endif
#endif

