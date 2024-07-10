/*****************************************************************************
** File Name:      mmiwidget_schedule.c                                        *
** Author:                                                                   *
** Date:           11/22/2011                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/22/2011     juan.wu             Creat
******************************************************************************/

#define _MMIWIDGET_ASSISTANT_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#ifdef SCH_ASSISTANT_WIDGET
#include "window_parse.h"
//#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
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
#include "mmischedule_export.h"
#include "mmicalendar_export.h"
#include "mmiacc_image.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmialarm_id.h"
#include "mmipub.h"
#include "mmiwidget_anim.h"
#include "mmifmm_export.h"
#include "mmiwidget_nv.h"
//#include "guictrl_api.h"
#include "mmialarm_export.h"
#include "mmialarm_text.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#if defined MAINLCD_SIZE_320X480
#define TEXT_LEFT_OFFSET   20
#define TEXT_RIGHT_OFFSET  20
#define TEXT_TOP_OFFSET   10
#define TEXT_BOTTOM_OFFSET  10
#define TEXT_FONT 13
#else 
#define TEXT_LEFT_OFFSET   24
#define TEXT_RIGHT_OFFSET  24
#define TEXT_TOP_OFFSET   4
#define TEXT_BOTTOM_OFFSET  5
#define TEXT_FONT 13
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
MMISCH_DETAIL_WIN_PARAM_T   * s_win_param_ptr          = PNULL;
LOCAL BOOLEAN               is_sch_widget_on_top = FALSE;
//LOCAL BOOLEAN               is_msg_from_widget   = FALSE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : HandleWidgetScheduleWinMsg
//  Global resource dependence : 
//  Author:
//  Note: juan.wu
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleWidgetScheduleWinMsg(
											   MMI_WIN_ID_T        win_id,        //IN:
											   MMI_MESSAGE_ID_E    msg_id,        //IN:
											   DPARAM            param        //IN:
											   );
											   
LOCAL MMI_RESULT_E HandleSCHTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

LOCAL GUI_RECT_T GetAnimRect( MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : DisplayCalendarAssistantUI
//  Global resource dependence : 
//  Author:
//  Note: juan.wu
/*****************************************************************************/
LOCAL void DisplayCalendarAssistantUI( MMI_WIN_ID_T        win_id);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_schedule_widget =
{
    MMIWIDGET_SCHEDULE_ID, 
	HandleWidgetScheduleWinMsg, 
	PNULL,
	PNULL,
	PNULL,
	WIDGET_SCHEDULE_WIN_ID, 
	IMAGE_WIDGET_SCHEDULE_SHORTCUT, 
	IMAGE_WIDGET_SCHEDULE_BG,
    TXT_WIDGET_SCHEDULE,		
	50,//    75, 
	50,//    50
	FALSE,
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : HandleWidgetScheduleWinMsg
//  Global resource dependence : 
//  Author:
//  Note: juan.wu
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetScheduleWinMsg(
											   MMI_WIN_ID_T        win_id,        //IN:
											   MMI_MESSAGE_ID_E    msg_id,        //IN:
											   DPARAM            param        //IN:
											   )
{
    static BOOLEAN          is_sidekey_deal = FALSE; 
    static uint16           list_index = 0;
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           anim_ctrl_id = MMIWIDGET_SCHEDULE_ANIM_CTRL_ID;
    GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};
    GUIANIM_DATA_INFO_T     anim_data_info = {0};
    GUIANIM_CTRL_INFO_T     anim_ctrl_info = {0};         
    GUIANIM_DISPLAY_INFO_T  anim_display_info = {0}; 
	MMI_CONTROL_CREATE_T    anim_ctrl_create = {0};
	GUIANIM_INIT_DATA_T     anim_init_data = {0};
	MN_RETURN_RESULT_E      nv_result = MN_RETURN_SUCCESS;
	BOOLEAN                 schedule_widget_flag = FALSE;
	static     BOOLEAN      is_anim_play   =  FALSE;
   

    anim_ctrl_info.is_ctrl_id = TRUE;
    anim_ctrl_info.ctrl_id = anim_ctrl_id;
    anim_ctrl_info.ctrl_ptr = PNULL;
    anim_display_info.is_syn_decode = TRUE;
    anim_display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
	anim_init_data.is_form_bg = FALSE;			
	anim_ctrl_create.ctrl_id = anim_ctrl_id;
	anim_ctrl_create.guid = SPRD_GUI_ANIM_ID;
	anim_ctrl_create.parent_win_handle = win_id;
    anim_ctrl_create.init_data_ptr = &anim_ctrl_create;

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    //SCI_TRACE_LOW:"HandleWidgetScheduleWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SCHEDULE_149_112_2_18_3_8_30_227,(uint8*)"d",msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_INIT_DATA_T            list_init_data   = {0};        	

    	    anim_data_info.img_id= IMAGE_WIDGET_SCHEDULE_ASSISTANT;
            list_init_data.type                = GUILIST_TEXTLIST_E;
		    s_win_param_ptr = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
		    if(PNULL == s_win_param_ptr)//内存没分配到
		    {
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SCHEDULE_162_112_2_18_3_8_30_228,(uint8*)"");
                break;
		    }		 
        	MMK_CreateControl(&anim_ctrl_create);
        	GUIANIM_SetParam(&anim_ctrl_info,&anim_data_info,PNULL,&anim_display_info);
		}
        break;
        
    case MSG_WIDGET_TP_PRESS_DOWN:
        {
            MMK_SetChildWinFocus(MMK_GetParentWinHandle( win_id), win_id);
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_WIDGET_TP_PRESS_UP:
        recode = HandleSCHTpPressUp(win_id,param);
        break;
        

    case MSG_GET_FOCUS:
        if(PNULL == s_win_param_ptr)//如果内存被释放了，重新申请
        {
            s_win_param_ptr = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
        }
        MMINV_READ(MMIWIDGET_ASSISTANT_NV_INFO, &schedule_widget_flag, nv_result);
        if (MN_RETURN_SUCCESS != nv_result)
        {
            schedule_widget_flag = TRUE;
            MMINV_WRITE(MMIWIDGET_ASSISTANT_NV_INFO, &schedule_widget_flag);
        }
        is_sch_widget_on_top = TRUE; 
        break;

    case MSG_NOTIFY_UPDATE:
        if(PNULL != param)
        {
            MMITHEME_ClearRect(&lcd_dev_info, (GUI_RECT_T *)param);
        }
        break;
        
    case MSG_FULL_PAINT:
        DisplayCalendarAssistantUI(win_id);  
        break;

    case MSG_SCH_EVENT_ARRIVE:
        {
            MMIACC_SMART_EVENT_T* event_ptr = {0};
 
            MMK_SetChildWinFocus(MMK_GetParentWinHandle( win_id), win_id);

            event_ptr = (MMIACC_SMART_EVENT_T*) param;
            
            anim_data_info.anim_id = ANIM_WIDGET_SCHEDULE_ASSISTANT;
            
            GUIANIM_SetParam(&anim_ctrl_info,&anim_data_info,PNULL,&anim_display_info);
            GUIANIM_PlayAnim(anim_ctrl_id);
            is_anim_play = TRUE;
            recode = MMIAPIALARM_HandleEventAliveMSG(win_id,MSG_OPEN_WINDOW,param);
        }
        break;

    case MSG_TIMER:
        if(PNULL != param)
        {
            if(MMIAPIALARM_GetALARMTimerID() == *( GUI_TIMER_ID_T*)param)
            {
                GUIANIM_PauseAnim(anim_ctrl_id);
                anim_data_info.img_id= IMAGE_WIDGET_SCHEDULE_ASSISTANT;
                GUIANIM_SetParam(&anim_ctrl_info,&anim_data_info,PNULL,&anim_display_info);
                GUIANIM_UpdateDisplay(anim_ctrl_id);
            }
            recode = MMIAPIALARM_HandleEventAliveMSG(win_id,msg_id,param);
        }       
        break;
        
    case MSG_LOSE_FOCUS:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYUP_POWER:
    case MSG_KEYUP_CANCEL:
    case MSG_KEYPRESSUP_CANCEL:
        if(is_anim_play)
        {
            GUIANIM_PauseAnim(anim_ctrl_id);
            anim_data_info.img_id= IMAGE_WIDGET_SCHEDULE_ASSISTANT;
            GUIANIM_SetParam(&anim_ctrl_info,&anim_data_info,PNULL,&anim_display_info);
            GUIANIM_UpdateDisplay(anim_ctrl_id);
            is_anim_play =FALSE;
            MMIAPIALARM_DealWithCancelOprate();
            is_sidekey_deal = TRUE;
            if(MSG_KEYUP_POWER == msg_id)
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_KEYLONG_DOWNSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
    case MSG_KEYUP_DOWNSIDE:
    case MSG_KEYLONG_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYUP_UPSIDE:
        if (is_sidekey_deal)
        {
            if (MSG_KEYUP_DOWNSIDE == msg_id || MSG_KEYUP_UPSIDE == msg_id)
            {
                is_sidekey_deal = FALSE;
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_GRID_REMOVE_WIDGET:
        is_sch_widget_on_top = FALSE;
        if(PNULL != s_win_param_ptr)
		{
		    SCI_FREE(s_win_param_ptr);
		    s_win_param_ptr = PNULL;
		}
        break;
        
    case MSG_SCH_EVENT_EDIT_FINISH:
        if(0 == s_win_param_ptr->index)
        {
            MMIALM_AddOneEvent(&s_win_param_ptr->event,&list_index);
        }
        else if(!MMIAPIALM_SaveInteEventInfo(s_win_param_ptr->index, &s_win_param_ptr->event))
        {
            MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        }
        break;

    case MSG_WIDGET_PAGE_SLIDE_START:
        if(is_anim_play)
        {
            GUIANIM_PauseAnim(anim_ctrl_id);
            anim_data_info.img_id= IMAGE_WIDGET_SCHEDULE_ASSISTANT;
            GUIANIM_SetParam(&anim_ctrl_info,&anim_data_info,PNULL,&anim_display_info);
            GUIANIM_UpdateDisplay(anim_ctrl_id);
            is_anim_play =FALSE;
            MMIAPIALARM_DealWithCancelOprate();
        }
        break;
        
    case MSG_WIDGET_MODIFY_RECT:
        {            
            GUI_RECT_T  assistant_rect = {0};
            assistant_rect = GetAnimRect(win_id);
            GUIANIM_SetCtrlRect(anim_ctrl_id, &assistant_rect);
        }
        break;
#ifdef CALENDAR_SUPPORT            
    case MSG_APP_WEB:
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
        break;
#endif
    case MSG_CLOSE_WINDOW:
        if(PNULL != s_win_param_ptr)
		{
		    SCI_FREE(s_win_param_ptr);
		    s_win_param_ptr = PNULL;
		}
        break;
        
    default:
        recode = MMIAPIALARM_HandleEventAliveMSG(win_id,msg_id,param);
        break;
    }
    return recode;
}
/*****************************************************************************/
// 	Description : 文本风格
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL  void SetSchTextStyle(
                                 GUISTR_STYLE_T *style_ptr,
                                 GUI_ALIGN_E align,
                                 GUI_FONT_T  font,
                                 GUI_COLOR_T font_color
                                 )
{
    if(PNULL == style_ptr)
    {
        return;
    }
    
    style_ptr->align = align;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = 0;
}
#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : DisplayCalendarAssistantUI
//  Global resource dependence : 
//  Author:
//  Note: juan.wu
/*****************************************************************************/
LOCAL void DisplaySch( MMI_WIN_ID_T  win_id,GUI_RECT_T* text_rect)
{
    GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};
    uint32 index=MMIAPISCH_GetNextActiveSchEventId();
    MMIACC_SMART_EVENT_T event={0};
    uint8  date_str[MMISET_DATE_STR_LEN] = {0}; 
    uint8  time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]  = {0}; 
    wchar  w_str[MMISET_DATE_STR_LEN+MMIALM_TIME_STR_12HOURS_LEN+1]={0};
    GUI_RECT_T date_rect={0};
    GUI_RECT_T time_rect={0};
    GUI_RECT_T sch_rect={0};
    MMI_STRING_T str={0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;


    date_rect=*text_rect;
    time_rect=*text_rect;
    sch_rect=*text_rect;

    date_rect.bottom=date_rect.top+GUIFONT_GetHeight(TEXT_FONT)+2;
    //date_rect.right=date_rect.left+(text_rect->right-text_rect->left)/2+10;
    time_rect.top = date_rect.bottom + 2;
    time_rect.bottom=time_rect.top+GUIFONT_GetHeight(TEXT_FONT)+2;
    //time_rect.left=date_rect.right;
    sch_rect.top=time_rect.bottom+2;
    
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info); 

    if(0!=index)
    {
        MMIAPIALM_FormInteEventInfo(index,&event);
        
        MMIAPISET_FormatDateStrByDateStyle(event.event_fast_info.year,
                                            event.event_fast_info.mon,
                                            event.event_fast_info.day,
                                            '/',
                                            date_str,
                                            MMISET_DATE_STR_LEN
                                            );
        MMI_STRNTOWSTR(w_str,
                 MMISET_DATE_STR_LEN,
                date_str,
                strlen((char*)date_str),
                strlen((char*)date_str));
        str.wstr_ptr=w_str;
        str.wstr_len=strlen((char*)date_str);

        SetSchTextStyle(&text_style, ALIGN_LVMIDDLE, TEXT_FONT, MMI_BLACK_COLOR);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&date_rect,
            (const GUI_RECT_T      *)&date_rect,
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
        );
        
        MMIAPISET_FormatTimeStrByTime(event.event_fast_info.hour,event.event_fast_info.minute,time_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
        SCI_MEMSET(w_str,0,sizeof(wchar)*(MMISET_DATE_STR_LEN+MMIALM_TIME_STR_12HOURS_LEN+1));
        MMI_STRNTOWSTR(w_str,
                 MMIALM_TIME_STR_12HOURS_LEN+1,
                time_str,
                strlen((char*)time_str),
                strlen((char*)time_str));
        str.wstr_ptr=w_str;
        str.wstr_len=strlen((char*)time_str);
        SetSchTextStyle(&text_style, ALIGN_LVMIDDLE, TEXT_FONT, MMI_BLACK_COLOR);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&time_rect,
            (const GUI_RECT_T      *)&time_rect,
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
        );
        
        if(event.event_content_info.w_length>0)
        {
            str.wstr_ptr=event.event_content_info.w_content;
            str.wstr_len=event.event_content_info.w_length;
        }
        else
        {
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_NO_TITLE, &str);
        }
        //state =GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_ELLIPSIS;
        SetSchTextStyle(&text_style, ALIGN_LVMIDDLE, TEXT_FONT, MMI_BLACK_COLOR);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&sch_rect,
            (const GUI_RECT_T      *)&sch_rect,
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
        );
    }
    else
    {
        date_rect.right=text_rect->right;
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WIDGET_SCHEDULE, &str);
        SetSchTextStyle(&text_style, ALIGN_LVMIDDLE, TEXT_FONT, MMI_BLACK_COLOR);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&date_rect,
            (const GUI_RECT_T      *)&date_rect,
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
        );

        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CALENDAR_NEW_SCHEDULE, &str);
        SetSchTextStyle(&text_style, ALIGN_LVMIDDLE, TEXT_FONT, MMI_BLACK_COLOR);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&sch_rect,
            (const GUI_RECT_T      *)&sch_rect,
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO);
    }
    
}
#endif
LOCAL GUI_RECT_T GetAnimRect( MMI_WIN_ID_T  win_id)
{    
    GUI_RECT_T              win_rect = {0};
    GUI_RECT_T              assistant_rect = {0};
    uint16                  assistant_img_h = 0;
    uint16                  assistant_img_w = 0;

    MMK_GetWinRect(win_id, &win_rect);
    
	GUIRES_GetImgWidthHeight(&assistant_img_w, &assistant_img_h, IMAGE_WIDGET_SCHEDULE_ASSISTANT, win_id);

    assistant_rect = win_rect;
    assistant_rect.right = win_rect.right - 2;    
    assistant_rect.left = win_rect.right-assistant_img_w - 2; 
    return assistant_rect;
}

/*****************************************************************************/
//  Description : DisplayCalendarAssistantUI
//  Global resource dependence : 
//  Author:
//  Note: juan.wu
/*****************************************************************************/
LOCAL void DisplayCalendarAssistantUI( MMI_WIN_ID_T        win_id)
{
    GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};
    MMI_CTRL_ID_T           anim_ctrl_id = MMIWIDGET_SCHEDULE_ANIM_CTRL_ID;
    GUI_RECT_T              win_rect = {0};
    GUI_RECT_T              assistant_rect = {0};
    uint16                  text_img_h = 0;
    uint16                  text_img_w = 0;
    GUI_RECT_T              text_rect={0};
    //GUI_POINT_T point={0};
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    //UILAYER_ClearRect(&lcd_dev_info, win_rect);
    
	GUIRES_GetImgWidthHeight(&text_img_w, &text_img_h, IMAGE_WIDGET_SCHEDULE_TEXT_BG, win_id);

    assistant_rect = GetAnimRect(win_id);
    GUIANIM_SetCtrlRect(anim_ctrl_id, &assistant_rect);

    text_rect = win_rect;
    text_rect.top = (win_rect.bottom + win_rect.top - text_img_h)/2;
    text_rect.bottom = text_rect.top + text_img_h;
    text_rect.left   = text_rect.left + 2;
    text_rect.right = text_rect.left + text_img_w + 2;
    //text_rect.right = text_rect.right - assistant_img_w -2; 

    GUIRES_DisplayImg(PNULL, 
                        &text_rect, 
                        PNULL, 
                        win_id, 
                        IMAGE_WIDGET_SCHEDULE_TEXT_BG, 
                        &lcd_dev_info);

    text_rect.left+=TEXT_LEFT_OFFSET;
    text_rect.top+=TEXT_TOP_OFFSET ;
    text_rect.bottom-=TEXT_BOTTOM_OFFSET ;
    text_rect.right-=TEXT_RIGHT_OFFSET;
#ifdef CALENDAR_SUPPORT
    DisplaySch(win_id,&text_rect);
#endif
}
/*****************************************************************************/
//  Description : MMIAPIWIDGET_SetScheduleWidgetFlag
//  Global resource dependence : 
//  Author:
//  Note: juan.wu
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_SetScheduleWidgetFlag(BOOLEAN is_already_add)
{
    MMINV_WRITE(MMIWIDGET_ASSISTANT_NV_INFO, &is_already_add);
}

/*****************************************************************************/
//  Description : MMIAPIWIDGET_GetScheduleWidgetFlag
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIDGET_GetScheduleWidgetFlag(void)
{
    MN_RETURN_RESULT_E      nv_result = MN_RETURN_SUCCESS;
    BOOLEAN                 is_add = FALSE;
    
    MMINV_READ(MMIWIDGET_ASSISTANT_NV_INFO, &is_add, nv_result);
    return is_add;
}
/*****************************************************************************/
//  Description : MMIAPIWIDGET_IsScheduleOnTop
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 判断当前页面是否有日程widget
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIDGET_IsScheduleOnTop(void)
{
    GUI_RECT_T win_rect = {0};
    GUI_RECT_T full_screen_rect = {0};
    BOOLEAN    result = FALSE;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr=MMIAPIGRID_GetGridPtr();

    full_screen_rect = MMITHEME_GetFullScreenRect();
    MMK_GetWinRect(WIDGET_SCHEDULE_WIN_ID, &win_rect);
    if((MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())&&
		is_sch_widget_on_top
        && MMK_IsFocusWin(MAIN_IDLE_WIN_ID)
        && GUI_RectIsCovered(full_screen_rect, win_rect))
    {
        if(PNULL==idle_grid_ptr)
        {
            result = TRUE;
        }
        else if(idle_grid_ptr->run_mode != MMIGRID_RUN_MODE_THUMBVIEW)
        {
            result = TRUE;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : MMIAPIWIDGET_SCH_Reset
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 恢复出厂设置时调用
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_SCH_Reset(void)
{
    BOOLEAN                 is_add = FALSE;

    MMINV_WRITE(MMIWIDGET_ASSISTANT_NV_INFO,&is_add);
}

/*****************************************************************************/
//  Description : HandleSCHTpPressUp
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void OpenSCHCallbackForPrivacyProtection(void)
{
    if(PNULL == s_win_param_ptr)
    {
        return ;
    }
    SCI_MEMSET(s_win_param_ptr,0,sizeof(MMISCH_DETAIL_WIN_PARAM_T));
#ifdef CALENDAR_SUPPORT
    s_win_param_ptr->index = MMIAPISCH_GetNextActiveSchEventId();
#endif
    if(0 == s_win_param_ptr->index )
    {
        SCI_DATE_T  date_info = {0};
        
        TM_GetSysDate(&date_info);
#ifdef CALENDAR_SUPPORT
        MMIAPISCH_InitNewSch(date_info.year, 
            date_info.mon, 
            date_info.mday, 
            &s_win_param_ptr->event
            );

        s_win_param_ptr->win_id = g_schedule_widget.win_id;
        
        MMIAPISCH_OpenEditWin((ADD_DATA)s_win_param_ptr);
#endif
    }
    else
    {
#ifdef CALENDAR_SUPPORT
        MMIAPISCH_OpenViewListAllWin(PNULL);
#endif
    }

}

/*****************************************************************************/
//  Description : HandleSCHTpPressUp
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSCHTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E	recode          = MMI_RESULT_TRUE;
    GUI_POINT_T		point           = {0};
	GUI_RECT_T      win_rect = {0};
    uint16          assistant_img_h = 0;
    uint16          assistant_img_w = 0;
    uint16          text_img_h = 0;
    uint16          text_img_w = 0;
    GUI_RECT_T      text_rect={0};
    BOOLEAN         is_text_rect = FALSE;
    
    MMK_GetWinRect(win_id, &win_rect);
    GUIRES_GetImgWidthHeight(&assistant_img_w, &assistant_img_h, IMAGE_WIDGET_SCHEDULE_ASSISTANT, win_id);
	GUIRES_GetImgWidthHeight(&text_img_w, &text_img_h, IMAGE_WIDGET_SCHEDULE_TEXT_BG, win_id);
	
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    text_rect = win_rect;
    text_rect.top = (win_rect.bottom + win_rect.top - text_img_h)/2;
    text_rect.bottom = text_rect.top + text_img_h;
    text_rect.right = text_rect.right - assistant_img_w -2;  

    is_text_rect = GUI_PointIsInRect(point, text_rect);
    
    if (is_text_rect)
    {       
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, OpenSCHCallbackForPrivacyProtection);
    }
	else
	{
        recode = MMI_RESULT_FALSE;
	}
    return recode;
    
}
#endif
