/****************************************************************************
** File Name:      mmicountedtime_main.c                            
** Author:                                                                   
** Date:            2007/12/14
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to process msg to main window of worlk clock
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 12/14/2004     zhaohui          Create
** 
****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_accessory_trc.h"
#ifdef WIN32
#include "std_header.h" 
#endif
#include "dal_time.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "guibutton.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#ifdef COUNTEDTIME_SUPPORT
#include "mmicc_export.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmialarm_export.h"

#include "mmi_default.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiacc_id.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "mmiacc_position.h"
#include "mmialarm_image.h"
#include "mmicountedtime_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_id.h"
#include "mmiidle_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "guires.h"
#ifdef MMI_KEY_LOCK_SUPPOR
#include "mmikl_export.h"
#endif
#include "guiedit.h"
#include "guistring.h"
#include "mmiidle_subwintab.h"
//#include "guictrl_api.h"    
#include "mmi_theme.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
#include "mmipicview_export.h"
#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif

#include "mmi_timer_export.h"

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#include "guictrl_api.h"
#ifdef MMI_GRID_IDLE_SUPPORT
#include "mmiwidget_export.h"
#endif
#include "guitab.h"
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#include "guiedit.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define CT_TIME_STRING_LENGTH               8       // XX:XX:XX
#define CT_ALERT_RING_DURATION              56000   // about one minute
#define MMICT_MAX_HOUR 23   //@zhaohui,cr108071
#define MMICT_MAX_MINUTE 59 //@zhaohui,cr108071
#if defined MAINLCD_SIZE_240X320
#define MMICT_OPTMZ_TOP 10  // cbk for toobar fit for
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef uint8 MMIACC_CT_TIME_T[CT_TIME_STRING_LENGTH + 1];

// typedef struct
// {
//     uint32                  id;
//     GUI_RECT_T              rect;
//     MMI_IMAGE_ID_T          select_image_id;
//     MMI_IMAGE_ID_T          unselect_image_id;
// } CT_BUTTON_REGION_T;

// typedef enum
// {
// #ifdef TOUCH_PANEL_SUPPORT    
//   ACC_CT_BUTTON_KEY_1,
//   ACC_CT_BUTTON_KEY_2,
//   ACC_CT_BUTTON_KEY_3,
//   ACC_CT_BUTTON_KEY_4,
//   ACC_CT_BUTTON_KEY_5,
//   ACC_CT_BUTTON_KEY_6,
//   ACC_CT_BUTTON_KEY_7,
//   ACC_CT_BUTTON_KEY_8,
//   ACC_CT_BUTTON_KEY_9,
//   ACC_CT_BUTTON_KEY_0,
// #endif
//   ACC_CT_BUTTON_UP,  
//   ACC_CT_BUTTON_DOWN,
//   ACC_CT_BUTTON_MAX
// } ACC_CT_BUTTON_E;
/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                           */
/*---------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
#if defined MMI_GRID_IDLE_SUPPORT && defined MMI_GRID_COUNTEDTIME
extern SCI_TIME_T s_ct_current_time;//启动后记录的倒计时当前时间
extern APP_POSITION_E s_app_get_focus_to_run;
extern uint8 s_countedtime_time_id;//倒记时的timer
extern COUNTEDTIME_STATE_E s_is_countedtime_running;
#else
LOCAL SCI_TIME_T s_ct_current_time = {0};//启动后记录的倒计时当前时间
LOCAL APP_POSITION_E s_app_get_focus_to_run = {0};
LOCAL uint8 s_countedtime_time_id = {0};//倒记时的timer
LOCAL BOOLEAN s_is_countedtime_running = {0};
#endif
LOCAL SCI_TIME_T s_ct_set_time = {0,1,0};//进入设置界面时看到的时间，记录为最近设置的时间值 
LOCAL BOOLEAN s_ct_is_lessthan_onemin = FALSE;
LOCAL BOOLEAN s_ct_is_event_delayed = FALSE;
LOCAL BOOLEAN s_ct_arrived_event_is_on = FALSE;//定时器事件到
LOCAL uint8 s_ct_event_timer_id = 0;
LOCAL uint8 s_ct_vibrator_time_id = 0;
LOCAL BOOLEAN s_ct_is_vibrator_on = FALSE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : the opration after button selected
//  Global resource dependence :
//  Author:
//  RETRUN:
//  Note:
/*****************************************************************************/
LOCAL void HandleButton(MMI_WIN_ID_T win_id);
#endif
/*****************************************************************************/
//  Description : the process message function of the CountedTime Setting
//  Global resource dependence : 
//  Author:        zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCountedTimeWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );

/*****************************************************************************/
//  Description : when pen ok, handle the message
//  Global resource dependence : 
//  Author:   zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void DisplayCountedTimeBG(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 设置输入的时间为倒记时开始的时间
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetTime(
                      MMI_CTRL_ID_T     ctrl_id
                      );

/*****************************************************************************/
//  Description : get lcd dev info
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMIACC_CT_TIME_T *GetCurTime( void );

/*****************************************************************************/
//  Description : DisplayCurTime
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void DisplayCurTime(
                        MMI_WIN_ID_T            win_id,
                        const GUI_BOTH_RECT_T   *rect_ptr
                        );

/*****************************************************************************/
//  Description : to show counted time arrive prompt
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void ShowCountedTimeArrive(void);

/*****************************************************************************/
//  Description : SendMsgtoDispCoutime
//  Global resource dependence : 
//  Author:       zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SendMsgtoDispCoutime(void);

/*****************************************************************************/
//  Description : Start CountedTime arrive Ring or Vib
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void StartCountedTimeRingOrVib(void);

/*****************************************************************************/
//  Description : Stop ring or vibrate
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void StopCountedTimeRingOrVib(BOOLEAN is_close_ring_timer);

//@zhaohui,cr108071
/*****************************************************************************/
//  Description : check the time if is valid
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
//LOCAL BOOLEAN CheckTimeIsValid(
//                                          MMISET_TIME_T       time        //the time
//                                           );

/*****************************************************************************/
//  Description :set both rect of bottons
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL void GetCTCTRLBothRect(
                             MMI_WIN_ID_T win_id,
                             GUI_BOTH_RECT_T *edit_ptr,
                             GUI_BOTH_RECT_T *button1_ptr,                          
                             GUI_BOTH_RECT_T *time_ptr
                             );
                  
WINDOW_TABLE( COUTIME_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleCountedTimeWinMsg ),    
    WIN_ID( MMIACC_COUTIME_WIN_ID ),
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
    WIN_TITLE(TXT_TOOLS_COUNTEDTIME),
#endif
#ifdef TOUCH_PANEL_SUPPORT
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    CREATE_EDIT_TOUCH_TIME_CTRL(MMICT_TIME_TIMEEDITBOX_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL,MMICT_TIME_BUTTON_CTRL_ID),
#else
     CREATE_EDIT_TIME_CTRL(MMICT_TIME_TIMEEDITBOX_CTRL_ID),
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
    WIN_SOFTKEY(TXT_RESET, TXT_COMMON_START, STXT_RETURN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMICT_TIME_TITLE_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_COMMON_START, TXT_NULL, STXT_RETURN),
#endif
#endif
    END_WIN
};

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : the opration after button selected
//  Global resource dependence : 
//  Author:       
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void HandleButton(MMI_WIN_ID_T win_id)
{
    GUI_BG_T                button_pressed  = {0};//按下按键时，button的背景显示
    GUI_BG_T                button_bg       = {0};

    button_bg.bg_type   = GUI_BG_IMG;//背景为图片
    button_pressed.bg_type = GUI_BG_IMG;
#ifndef MMI_GRID_IDLE_SUPPORT
    if(s_is_countedtime_running==FALSE)
#else
    if(s_is_countedtime_running == STATE_STOP)
#endif
    {
        GUIEDIT_EnbaleAnim(MMICT_TIME_TIMEEDITBOX_CTRL_ID,FALSE);
        GUIEDIT_GetTime( MMICT_TIME_TIMEEDITBOX_CTRL_ID,& (s_ct_current_time.hour), &(s_ct_current_time.min), &(s_ct_current_time.sec));
        GUIEDIT_SetTime(MMICT_TIME_TIMEEDITBOX_CTRL_ID,s_ct_current_time.hour,s_ct_current_time.min,s_ct_current_time.sec);
        //s_ct_current_time.sec=0;
        
        if (!(s_ct_current_time.hour==0 && s_ct_current_time.min==0 && s_ct_current_time.sec==0))
        {
            //将edittouch中的时间存到s_ct_set_time，在停止之后editttouch会再次显示这个时间
            GUIEDIT_GetTime( MMICT_TIME_TIMEEDITBOX_CTRL_ID,& (s_ct_set_time.hour), &(s_ct_set_time.min),&(s_ct_set_time.sec));
            //s_ct_set_time.sec=0;
            s_ct_is_lessthan_onemin = FALSE;
#ifndef MMI_GRID_IDLE_SUPPORT
            s_is_countedtime_running=TRUE;
#else
            s_is_countedtime_running = STATE_RUNNING;
#endif
            //更改buTton2的文字为停止计时
            GUIBUTTON_SetTextId( MMICT_TIME_BUTTON_CTRL_ID, STXT_STOP);
            button_bg.img_id        = IMAGE_CLOCK_TIMER_STOP;
            button_pressed.img_id   = IMAGE_CLOCK_TIMER_STOP_PRESSED;
            GUIBUTTON_SetBg(MMICT_TIME_BUTTON_CTRL_ID, &button_bg);
            GUIBUTTON_SetPressedBg(MMICT_TIME_BUTTON_CTRL_ID, &button_pressed);
            //隐藏edittouch控件
            GUIAPICTRL_SetState(MMICT_TIME_TIMEEDITBOX_CTRL_ID,GUICTRL_STATE_INVISIBLE,TRUE);

            if (0 != s_countedtime_time_id)
            {
                MMK_StopTimer(s_countedtime_time_id);//stop timer
                s_countedtime_time_id = 0;
            }
#ifdef MMI_GRID_IDLE_SUPPORT
            s_app_get_focus_to_run = APP_IN_MENU;
#endif
            s_countedtime_time_id = MMK_CreateTimerCallback(1000,  MMIAPICT_HandleTimer, NULL, TRUE);
#ifndef MMI_GRID_IDLE_SUPPORT					
            s_is_countedtime_running = TRUE;
#else
            s_is_countedtime_running = STATE_RUNNING;
#endif
#ifdef PDA_UI_DROPDOWN_WIN
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_COUNTEDTIME);
#endif  				
        }   
    }
#ifndef MMI_GRID_IDLE_SUPPORT					
    else if(s_is_countedtime_running==TRUE)
#else
    else if(s_is_countedtime_running == STATE_RUNNING)
#endif
    {
#ifndef MMI_GRID_IDLE_SUPPORT			
        s_is_countedtime_running=FALSE;
#else
        s_is_countedtime_running = STATE_STOP;
#endif				
#ifdef MMI_GRID_IDLE_SUPPORT
        s_app_get_focus_to_run = APP_NOT_RUN;
#endif
        MMK_StopTimer(s_countedtime_time_id);
        MMK_StopTimer(s_ct_vibrator_time_id);
        s_countedtime_time_id = 0;

        /*if (0==s_ct_current_time.hour && 0== s_ct_current_time.min)
        {
            s_ct_current_time.min++;
        }*/
        GUIEDIT_SetTime( MMICT_TIME_TIMEEDITBOX_CTRL_ID, s_ct_set_time.hour, s_ct_set_time.min, s_ct_set_time.sec);

        //button2 显示"启动"这几个字
        GUIBUTTON_SetTextId( MMICT_TIME_BUTTON_CTRL_ID,  TXT_COMMON_START);   
        button_bg.img_id    = IMAGE_CLOCK_TIMER_START;        
        button_pressed.img_id   = IMAGE_CLOCK_TIMER_START_PRESSED;
        GUIBUTTON_SetBg(MMICT_TIME_BUTTON_CTRL_ID, &button_bg);        
        GUIBUTTON_SetPressedBg(MMICT_TIME_BUTTON_CTRL_ID, &button_pressed);
        //取消edittouch控件的隐藏
        GUIAPICTRL_SetState(MMICT_TIME_TIMEEDITBOX_CTRL_ID,GUICTRL_STATE_INVISIBLE,FALSE);
    }       
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}
#endif
/*****************************************************************************/
//  Description : the process message function of the CountedTime Setting
//  Global resource dependence : 
//  Author:        zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCountedTimeWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{

    MMI_RESULT_E            recode = MMI_RESULT_TRUE;

    SCI_TIME_T              time = s_ct_set_time;
    GUI_BOTH_RECT_T         button_rect    = {0};
    GUI_BOTH_RECT_T         edit_rect      = {0};
    GUI_BOTH_RECT_T         time_rect      = {0}; 
    GUI_BOTH_RECT_T         *button_ptr     = {0};
    GUI_BOTH_RECT_T         *edit_ptr       = {0};
    GUI_BOTH_RECT_T         *time_ptr       = {0};     
    GUIEDIT_TIME_STYLE_E    time_style = GUIEDIT_TIME_STYLE_TIMER;
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
    GUI_BOTH_RECT_T         editbox_rect    =   {0};
    GUI_BOTH_RECT_T         label_rect    =   {0};
    MMI_STRING_T            text = {0};
    wchar                   titlestring[7] = {0};//0x003a,0x004d,0x004d,0}; 
    char                    *str_ptr = "HH:MM";
    uint8                   str_len = strlen(str_ptr);
#endif
#ifdef TOUCH_PANEL_SUPPORT

    GUI_BG_T                button_pressed  = {0};//按下按键时，button的背景显示
    GUI_BG_T                button_bg       = {0};
    GUI_FONT_ALL_T          button_font_all = {0};//button的字体与颜色
    GUI_FONT_ALL_T active_font = {0};
    GUI_FONT_ALL_T normal_font = {0};
    GUIEDIT_TIME_DISPLAY_STYLE_E    time_display_style = GUIEDIT_TIME_DISPLAY_STYLE_HMS;
    button_bg.bg_type   = GUI_BG_IMG;//背景为图片
    button_pressed.bg_type = GUI_BG_IMG;

    button_font_all.font = MMI_DEFAULT_NORMAL_FONT ;
    button_font_all.color =  MMITHEME_GetCurThemeFontColor(MMI_THEME_MENU);
#endif
    GetCTCTRLBothRect(win_id, &edit_rect, &button_rect,&time_rect);
    button_ptr         = &button_rect;
    edit_ptr           = &edit_rect;
    time_ptr           = &time_rect;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_GRID_IDLE_SUPPORT
        if (STATE_CANCEL == s_is_countedtime_running)
        {
            s_is_countedtime_running = STATE_STOP;
        }
        if (APP_IN_IDLE == s_app_get_focus_to_run)
        {
            if (0 != s_countedtime_time_id)//@zhaohui,cr108080
            {
                MMK_StopTimer(s_countedtime_time_id);//stop timer
                s_countedtime_time_id = 0;
            }
            s_app_get_focus_to_run = APP_IN_MENU;
            if(0  == s_countedtime_time_id)
            {
                s_countedtime_time_id = MMK_CreateTimerCallback(1000,  MMIAPICT_HandleTimer, NULL, TRUE);			
            }
        }
#endif
#ifdef TOUCH_PANEL_SUPPORT
        //IWIN_SetTitleIsVisible(win_id, FALSE, FALSE);

        //设置控件位置
        GUIAPICTRL_SetBothRect(MMICT_TIME_BUTTON_CTRL_ID, &button_rect);
        GUIAPICTRL_SetBothRect(MMICT_TIME_TIMEEDITBOX_CTRL_ID, &edit_rect);
        //设置edittouch字符提示文字放到时间右边
        GUIEDIT_SetTipsPos(MMICT_TIME_TIMEEDITBOX_CTRL_ID, GUIEDIT_TIPS_POS_TOP);
        normal_font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_NORMAL);
        normal_font.font = CT_PICKER_NORMAL_FONT;            
        active_font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_CUR);
        active_font.font = CT_PICKER_ACTIVE_FONT;
        GUIEDIT_SetPickerFont(MMICT_TIME_TIMEEDITBOX_CTRL_ID, &normal_font, &active_font);
        GUIEDIT_SetPickerLineHeight(MMICT_TIME_TIMEEDITBOX_CTRL_ID, CT_PICKER_HEIGHT);            
        //设置button的对齐方式、字体、取消光晕
        GUIBUTTON_SetTextAlign(MMICT_TIME_BUTTON_CTRL_ID,  ALIGN_HVMIDDLE);
        GUIBUTTON_SetFont(MMICT_TIME_BUTTON_CTRL_ID, &button_font_all);
        GUIBUTTON_SetRunSheen( MMICT_TIME_BUTTON_CTRL_ID, FALSE );
        //set time style
        GUIEDIT_SetTimeStyle(MMICT_TIME_TIMEEDITBOX_CTRL_ID,PNULL,&time_style,&time_display_style,FALSE);
	    MMK_SetAtvCtrl(win_id,MMICT_TIME_TIMEEDITBOX_CTRL_ID);         

        if(0==s_countedtime_time_id)
        {
            //button2 显示"启动"这几个字
            GUIBUTTON_SetTextId( MMICT_TIME_BUTTON_CTRL_ID,  TXT_COMMON_START);
            button_bg.img_id        = IMAGE_CLOCK_TIMER_START;
            button_pressed.img_id   = IMAGE_CLOCK_TIMER_START_PRESSED;
            GUIBUTTON_SetBg(MMICT_TIME_BUTTON_CTRL_ID, &button_bg);
            GUIBUTTON_SetPressedBg(MMICT_TIME_BUTTON_CTRL_ID, &button_pressed);

            //获得初始时间
            GUIEDIT_SetTime( MMICT_TIME_TIMEEDITBOX_CTRL_ID, time.hour, time.min, 0);
        }
        else
        {
            //计时已开始，显示倒计时
            //button2 显示"停止"这几个字
            GUIBUTTON_SetTextId( MMICT_TIME_BUTTON_CTRL_ID,  STXT_STOP);
            button_bg.img_id        = IMAGE_CLOCK_TIMER_STOP;
            button_pressed.img_id   = IMAGE_CLOCK_TIMER_STOP_PRESSED;
            GUIBUTTON_SetBg(MMICT_TIME_BUTTON_CTRL_ID, &button_bg);
            GUIBUTTON_SetPressedBg(MMICT_TIME_BUTTON_CTRL_ID, &button_pressed);

            //隐藏edittouch控件	
            GUIAPICTRL_SetState(MMICT_TIME_TIMEEDITBOX_CTRL_ID,GUICTRL_STATE_INVISIBLE,TRUE);
        }
#else
        //set time style
        GUIEDIT_SetTimeStyle(MMICT_TIME_TIMEEDITBOX_CTRL_ID,PNULL,&time_style,PNULL,FALSE);

        // MMK_SetAtvCtrl(win_id,MMICT_TIME_TIMEEDITBOX_CTRL_ID);        

        GUIEDIT_SetFont(MMICT_TIME_TIMEEDITBOX_CTRL_ID,MMICT_TEXT_FONT);
        GUIEDIT_SetFontColor(MMICT_TIME_TIMEEDITBOX_CTRL_ID, MMI_BLACK_COLOR);
        
        IGUICTRL_SetCircularHandleLeftRight(MMK_GetCtrlPtr(MMICT_TIME_TIMEEDITBOX_CTRL_ID), FALSE);
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
        /*
         *  fixed by chunjie.liu
         *	disable lable get active, no need to do this
         */
        GUIAPICTRL_SetState(MMICT_TIME_TITLE_CTRL_ID, GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
#endif

#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
      
        if(MMITHEME_IsMainScreenLandscape())
        {
        label_rect.h_rect.top = edit_rect.h_rect.top+CT_LABEL_MARGIN;
        label_rect.h_rect.left = edit_rect.h_rect.left;
        label_rect.h_rect.right = edit_rect.h_rect.right;
        label_rect.h_rect.bottom = edit_rect.h_rect.top+(edit_rect.h_rect.bottom-edit_rect.h_rect.top)/2;

        editbox_rect.h_rect.top = label_rect.h_rect.bottom;
        editbox_rect.h_rect.left = edit_rect.h_rect.left;
        editbox_rect.h_rect.right = edit_rect.h_rect.right;
        editbox_rect.h_rect.bottom = edit_rect.h_rect.bottom;
        }
        else
        {
        label_rect.v_rect.top = edit_rect.v_rect.top+CT_LABEL_MARGIN;
        label_rect.v_rect.left = edit_rect.v_rect.left;
        label_rect.v_rect.right = edit_rect.v_rect.right;
        label_rect.v_rect.bottom = edit_rect.v_rect.top+(edit_rect.v_rect.bottom-edit_rect.v_rect.top)/2;

        editbox_rect.v_rect.top = label_rect.v_rect.bottom;
        editbox_rect.v_rect.left = edit_rect.v_rect.left;
        editbox_rect.v_rect.right = edit_rect.v_rect.right;
        editbox_rect.v_rect.bottom = edit_rect.v_rect.bottom;
        }

        GUIAPICTRL_SetBothRect(MMICT_TIME_TIMEEDITBOX_CTRL_ID, &editbox_rect);
        //HH:MM
        MMI_STRNTOWSTR(titlestring, 6, str_ptr, str_len, str_len);   /*lint !e64*/
        text.wstr_ptr = titlestring;
        text.wstr_len = MMIAPICOM_Wstrlen(text.wstr_ptr);

        GUIAPICTRL_SetBothRect(MMICT_TIME_TITLE_CTRL_ID, &label_rect);
        GUILABEL_SetText(MMICT_TIME_TITLE_CTRL_ID, &text, FALSE);
        GUILABEL_SetFont(MMICT_TIME_TITLE_CTRL_ID,MMICT_TITLE_FONT,MMI_BLACK_COLOR);

#endif
        if(0==s_countedtime_time_id)
        {
            //获得初始时间
            GUIEDIT_SetTime( MMICT_TIME_TIMEEDITBOX_CTRL_ID, time.hour, time.min, 0);
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
            GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET, TXT_COMMON_START, STXT_RETURN, FALSE);
            GUICTRL_SetActive(MMK_GetCtrlPtr(MMICT_TIME_TIMEEDITBOX_CTRL_ID), TRUE);
#else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_START, TXT_NULL, STXT_RETURN, FALSE);
#endif
        }
        else
        {
            //隐藏edittouch控件 
            GUIAPICTRL_SetState(MMICT_TIME_TIMEEDITBOX_CTRL_ID,GUICTRL_STATE_INVISIBLE,TRUE);
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
            GUIAPICTRL_SetState(MMICT_TIME_TITLE_CTRL_ID,GUICTRL_STATE_INVISIBLE,TRUE);
#endif
            GUIWIN_SetSoftkeyTextId(win_id, STXT_STOP, TXT_NULL, STXT_RETURN, FALSE);
        }
#endif
        break;

    case MSG_FULL_PAINT:
        DisplayCountedTimeBG(win_id);	
        DisplayCurTime(win_id, &time_rect);		
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMI_NOTIFY_T notify_param = {0};
            notify_param.src_id =  MMICT_TIME_BUTTON_CTRL_ID ;
            MMK_SendMsg(win_id, MSG_CTL_PENOK , &notify_param );
        }
        break;
#else
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    //case MSG_APP_WEB:
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
         if(STATE_STOP == s_is_countedtime_running
             && ((MSG_CTL_OK == msg_id) || (MSG_APP_OK == msg_id))
            )
         {
            GUIEDIT_SetTime(MMICT_TIME_TIMEEDITBOX_CTRL_ID, 0, 0, 0);
            MMK_PostMsg(MMICT_TIME_TIMEEDITBOX_CTRL_ID, MSG_CTL_PAINT, PNULL, 0);
            MMK_PostMsg(MMICT_TIME_TITLE_CTRL_ID, MSG_CTL_PAINT, PNULL, 0);
            s_ct_is_lessthan_onemin = TRUE;
            break;
         }
#endif

#ifndef MMI_GRID_IDLE_SUPPORT
        if(s_is_countedtime_running==FALSE)
#else
        if(s_is_countedtime_running == STATE_STOP)
#endif
        {
            SetTime(MMICT_TIME_TIMEEDITBOX_CTRL_ID);
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
            if(s_ct_is_lessthan_onemin)
            {
                break;
            }
#else
            s_ct_is_lessthan_onemin = FALSE;
#endif
            //隐藏edittouch控件
            GUIAPICTRL_SetState(MMICT_TIME_TIMEEDITBOX_CTRL_ID,GUICTRL_STATE_INVISIBLE,TRUE);
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
            GUIAPICTRL_SetState(MMICT_TIME_TITLE_CTRL_ID,GUICTRL_STATE_INVISIBLE,TRUE);
#endif
            GUIWIN_SetSoftkeyTextId(win_id, STXT_STOP, TXT_NULL, STXT_RETURN, FALSE);

            if (0 != s_countedtime_time_id)
            {
                MMK_StopTimer(s_countedtime_time_id);//stop timer
                s_countedtime_time_id = 0;
            }
#ifdef MMI_GRID_IDLE_SUPPORT
            s_app_get_focus_to_run = APP_IN_MENU;
#endif
            s_countedtime_time_id = MMK_CreateTimerCallback(1000,  MMIAPICT_HandleTimer, NULL, TRUE);
#ifndef MMI_GRID_IDLE_SUPPORT					
            s_is_countedtime_running = TRUE;
#else
            s_is_countedtime_running = STATE_RUNNING;
#endif
#ifdef PDA_UI_DROPDOWN_WIN
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_COUNTEDTIME);
#endif  				
            // for cr 124219
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        }
#ifndef MMI_GRID_IDLE_SUPPORT					
        else if(s_is_countedtime_running==TRUE)
#else
        else if(s_is_countedtime_running == STATE_RUNNING)
#endif
        {
#ifndef MMI_GRID_IDLE_SUPPORT			
                s_is_countedtime_running=FALSE;
#else
                s_is_countedtime_running = STATE_STOP;
#endif				
#ifdef MMI_GRID_IDLE_SUPPORT
                s_app_get_focus_to_run = APP_NOT_RUN;
#endif
                MMK_StopTimer(s_countedtime_time_id);
                MMK_StopTimer(s_ct_vibrator_time_id);
                s_countedtime_time_id = 0;

                if (0==s_ct_current_time.hour && 0== s_ct_current_time.min)
                {
                    s_ct_current_time.min++;
                }
                GUIEDIT_SetTime( MMICT_TIME_TIMEEDITBOX_CTRL_ID, s_ct_set_time.hour, s_ct_set_time.min, 0);

                GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_START, TXT_NULL, STXT_RETURN, FALSE);

                //取消edittouch控件的隐藏
                GUIAPICTRL_SetState(MMICT_TIME_TIMEEDITBOX_CTRL_ID,GUICTRL_STATE_INVISIBLE,FALSE);
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
                GUIAPICTRL_SetState(MMICT_TIME_TITLE_CTRL_ID,GUICTRL_STATE_INVISIBLE,FALSE);
                GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET, TXT_COMMON_START, STXT_RETURN, FALSE);
#ifdef GUI_MID_SOFTKEY_USE_IMG
                GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_START_ICON, 1, FALSE);
#endif
#else
                GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_START, TXT_NULL, STXT_RETURN, FALSE);
#endif

                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
#endif

//右键返回，但不停止倒计时
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:  
		if(!MMK_IsChildWin(win_id))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            MMK_CloseParentWin(MMI_TIMER_PARENT_WIN_ID);
        }
        break;

    case MSG_KEYDOWN_LEFT:
    case MSG_KEYREPEAT_LEFT:
        if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
        {
            GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID,  GUITAB_SWITCH_PRE);
        }
        break;

    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
        {
            GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID, GUITAB_SWITCH_NEXT);
        }
        break;

    case MSG_ACC_UPDATE_COUNTEDTIME:
        MMK_SendMsg(win_id, MSG_FULL_PAINT , PNULL );
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:      
        if (PNULL == param)
        {            
            break;
        }
        {            
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id; 
            if(MMICT_TIME_BUTTON_CTRL_ID==src_id)
            {
                HandleButton(win_id);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_LOSE_FOCUS:
        GUICTRL_SetActive(MMK_GetCtrlPtr(MMICT_TIME_TIMEEDITBOX_CTRL_ID), FALSE);
        break;

   case MSG_GET_FOCUS:
        if(0==s_countedtime_time_id)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_RESET, TXT_COMMON_START, STXT_RETURN, FALSE);
#ifdef GUI_MID_SOFTKEY_USE_IMG
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_START_ICON, 1, FALSE);
#endif
            GUICTRL_SetActive(MMK_GetCtrlPtr(MMICT_TIME_TIMEEDITBOX_CTRL_ID), TRUE);
        }
        break;
//     case MSG_GET_FOCUS:
// // #ifdef MMI_PDA_SUPPORT
// //         s_timer_cur_item_num = MMI_CT_TIMER;
// // #endif
//         break;

    case MSG_CLOSE_WINDOW:
        break;

    case MSG_NOTIFY_TAB_SWITCH:
        GUICTRL_SetActive(MMK_GetCtrlPtr(MMICT_TIME_TIMEEDITBOX_CTRL_ID), TRUE);
        break;

    case MSG_KEYDOWN_DOWN:
    case MSG_KEYREPEAT_DOWN:
        if (GUICTRL_IsActive(MMK_GetCtrlPtr(MMICT_TIME_TIMEEDITBOX_CTRL_ID)))
        {
            recode = MMI_RESULT_FALSE;
        }
        else
        {   
            GUICTRL_SetActive(MMK_GetCtrlPtr(MMICT_TIME_TIMEEDITBOX_CTRL_ID), TRUE);
            MMK_PostMsg(MMICT_TIME_TIMEEDITBOX_CTRL_ID,MSG_CTL_PAINT,0,0);
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
            MMK_PostMsg(MMICT_TIME_TITLE_CTRL_ID,MSG_CTL_PAINT,0,0);
#endif
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}


/*****************************************************************************/
//  Description : when pen ok, handle the message
//  Global resource dependence : 
//  Author:   zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void DisplayCountedTimeBG(MMI_WIN_ID_T win_id)
{
    //GUI_POINT_T         dis_point    = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          image_rect   = {0};  
    GUI_RECT_T          image_bg_rect   = {0};  
    GUI_BOTH_RECT_T     both_rect       = {0};
    
    both_rect   = MMITHEME_GetWinClientBothRect(win_id);

    if(!MMK_IsWindowLandscape(win_id))//纵向
    {
        image_rect = both_rect.v_rect;
	    image_bg_rect = both_rect.v_rect;
    }
    else
    {
        image_rect = both_rect.h_rect;
	    image_bg_rect = both_rect.h_rect;
    }

    // display background
    GUIRES_DisplayImg(PNULL,
        &image_rect,
        &image_bg_rect,
        win_id,
        IMAGE_COMMON_BG ,
        &lcd_dev_info);     
}

/*****************************************************************************/
//  Description : 设置输入的时间为倒记时开始的时间
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetTime(
                      MMI_CTRL_ID_T     ctrl_id
                      )
{
    uint8           hour = 0,minute = 0;
    BOOLEAN         result = TRUE;
    MMISET_TIME_T   time = {0};

    //get time from ctrl
    GUIEDIT_GetTime(ctrl_id,&hour,&minute,PNULL);

    time.time_hour = hour;
    time.time_minute = minute;

    //check time if is valid
    if (MMIAPICOM_CheckTimeIsValid(time))//@zhaohui,cr108071
    {
        s_ct_set_time.sec = 0;
        s_ct_set_time.min = minute;
        s_ct_set_time.hour = hour;
        s_ct_current_time.sec = 0;
        s_ct_current_time.min = minute;
        s_ct_current_time.hour = hour;

     s_ct_current_time = s_ct_set_time;
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
     if(1>minute && 1>hour )
     {
         s_ct_is_lessthan_onemin = TRUE;
     }
     else
     {
         s_ct_is_lessthan_onemin = FALSE;
     }
#endif
    }
    else
    {
//        SCI_TRACE_LOW("SetTime:hour is %d,minute is %d is invalid!",hour,minute);
        result = FALSE;
    }

    return (result);
}

//@zhaohui,cr108071
/*****************************************************************************/
//  Description : check the time if is valid
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
//LOCAL BOOLEAN CheckTimeIsValid(
//                                          MMISET_TIME_T       time        //the time
//                                          )
//{
//    BOOLEAN     result = TRUE;
//    
//    //check the minute
//    if ((MMICT_MAX_MINUTE < time.time_minute))
//    {
//        result = FALSE;
//    }
//    else
//    {
//        //check the time of 99 Hour if is valid,check the hour
//        if ((MMICT_MAX_HOUR < time.time_hour))
//        {
//            result = FALSE;
//        }
//    }
//    
//    return (result);
// }

/*****************************************************************************/
//  Description : get lcd dev info
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMIACC_CT_TIME_T *GetCurTime( void )//直接用alarm中的MMIACC_RTC_TIME_T是否会更好
{
    static  MMIACC_CT_TIME_T time;
    SCI_TIME_T curtime = s_ct_current_time;
/*
    if(curtime.hour==0)
    {
        sprintf((char*)time, "%02d:%02d", curtime.min, curtime.sec);
    }
    else
*/
    {
        sprintf((char*)time, "%02d:%02d:%02d", curtime.hour, curtime.min, curtime.sec);
    }
 
    return( time ); /*lint !e64*/
}


/*****************************************************************************/
//  Description : DisplayCurTime
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void DisplayCurTime(
                        MMI_WIN_ID_T            win_id,
                        const GUI_BOTH_RECT_T   *rect_ptr
                        )

{
    wchar            time_temp[CT_TIME_STRING_LENGTH+1]  = {0};
    MMIACC_CT_TIME_T *time = {0};
    MMI_STRING_T        cur_time     = {0};
    MMISET_LANGUAGE_TYPE_E  lang    = MMISET_MAX_LANGUAGE;//language    
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_RECT_T          time_rect       = {0};//非pda
    
    if(PNULL== rect_ptr)
    {
        //SCI_TRACE_LOW:"ST_DisplayCurTime: the rect is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOUNTEDTIME_MAIN_771_112_2_18_2_1_48_76,(uint8*)"");
        return;
    }

    if(!MMK_IsWindowLandscape(win_id))//纵向
    {
        time_rect = rect_ptr->v_rect;
    }
    else
    { 
        time_rect = rect_ptr->h_rect;
    }
    if(GUI_IsInvalidRect(time_rect))
    {
        //SCI_TRACE_LOW:"ST_DisplayCurTime: the rect is invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOUNTEDTIME_MAIN_785_112_2_18_2_1_48_77,(uint8*)"");
        return;
    }
    time = GetCurTime();
    
    MMIAPISET_GetLanguageType(&lang);
    time_rect = MMITHEME_GetClientRect();
    MMI_STRNTOWSTR(time_temp,CT_TIME_STRING_LENGTH,(uint8*)time,CT_TIME_STRING_LENGTH,CT_TIME_STRING_LENGTH);
    cur_time.wstr_ptr      = time_temp;
    cur_time.wstr_len = CT_TIME_STRING_LENGTH;
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = MMICT_TEXT_FONT;
    text_style.font_color = MMI_BLACK_COLOR;
    // display
    GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&time_rect,       //the fixed display area
            (const GUI_RECT_T      *)&time_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&cur_time,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 
}

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICT_HandleTimer(uint8 timer_id, uint32 param)
{
#ifdef MMI_GRID_IDLE_SUPPORT
	if (APP_IN_MENU == s_app_get_focus_to_run)
#endif	
	{
#ifdef TOUCH_PANEL_SUPPORT
		GUI_BG_T                button_bg       = {0};
		GUI_BG_T                button_pressed  = {0};
#endif
		//SCI_TRACE_LOW:"MMIAPICT_HandleTimer timer_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOUNTEDTIME_MAIN_825_112_2_18_2_1_48_78,(uint8*)"d",timer_id);
		if (s_countedtime_time_id == timer_id)
		{
			//时间减1处理
			if (s_ct_current_time.sec <60)
			{
				s_ct_current_time.sec -- ;
				if (((s_ct_current_time.sec == 0)||(s_ct_current_time.sec == 0xff) ) && (s_ct_current_time.min == 0) && (s_ct_current_time.hour == 0))//时间到
				{   
					s_ct_current_time.sec=0;
					SendMsgtoDispCoutime();
					MMIAPICT_StartCountedTimeArrive();
#ifdef TOUCH_PANEL_SUPPORT
					button_bg.bg_type   = GUI_BG_IMG;//背景为图片
					button_pressed.bg_type  = GUI_BG_IMG;
					//button2 显示"启动"这几个字
					GUIBUTTON_SetTextId( MMICT_TIME_BUTTON_CTRL_ID,  TXT_COMMON_START);	
					button_bg.img_id        = IMAGE_CLOCK_TIMER_START;
					button_pressed.img_id   = IMAGE_CLOCK_TIMER_START_PRESSED;
					GUIBUTTON_SetBg(MMICT_TIME_BUTTON_CTRL_ID, &button_bg);
					GUIBUTTON_SetPressedBg(MMICT_TIME_BUTTON_CTRL_ID, &button_pressed);
                    //取消edittouch控件的隐藏
					GUIAPICTRL_SetState(MMICT_TIME_TIMEEDITBOX_CTRL_ID,GUICTRL_STATE_INVISIBLE,FALSE);
#else
            		//取消edittouch控件的隐藏,GUIWINsoft设置应该在界面内控件状态都改变过之后再设置
					GUIAPICTRL_SetState(MMICT_TIME_TIMEEDITBOX_CTRL_ID,GUICTRL_STATE_INVISIBLE,FALSE);    
#ifdef MMI_COUNTEDTIME_MINI_SUPPORT
                    GUIAPICTRL_SetState(MMICT_TIME_TITLE_CTRL_ID,GUICTRL_STATE_INVISIBLE,FALSE); 
                    GUIWIN_SetSoftkeyTextId(MMIACC_COUTIME_WIN_ID, TXT_RESET, TXT_COMMON_START, STXT_RETURN, FALSE);
#ifdef GUI_MID_SOFTKEY_USE_IMG
                    GUIWIN_SeSoftkeytButtonIconId(MMIACC_COUTIME_WIN_ID, IMAGE_COMMON_SOFTKEY_START_ICON, 1, FALSE);
#endif
#else
                    GUIWIN_SetSoftkeyTextId(MMIACC_COUTIME_WIN_ID, TXT_COMMON_START, TXT_NULL, STXT_RETURN, FALSE);
    #endif
#endif
					//获得初始时间
					GUIEDIT_SetTime(MMICT_TIME_TIMEEDITBOX_CTRL_ID, s_ct_set_time.hour, s_ct_set_time.min, s_ct_set_time.sec);
					if (0 != s_countedtime_time_id)
					{
#ifdef MMI_GRID_IDLE_SUPPORT
                                                s_app_get_focus_to_run = APP_NOT_RUN;
#endif
                                                MMK_StopTimer(s_countedtime_time_id);//倒计时到，停止计时器
                                                s_countedtime_time_id = 0;
					}
					return;                    
				}
				else if ((s_ct_current_time.sec == 0xff) && (s_ct_current_time.min<=MMICT_MAX_MINUTE))//@zhaohui,cr110181
				{
					s_ct_current_time.sec = 59;
					s_ct_current_time.min --;
					if ((s_ct_current_time.min == 0xff) && (s_ct_current_time.hour>0) && (s_ct_current_time.hour <= MMICT_MAX_HOUR))//@zhaohui,cr110181
					{
						s_ct_current_time.min = 59;
						s_ct_current_time.hour --;
					}
					else if ((s_ct_current_time.min == 0) && (s_ct_current_time.hour == 0))//时间小于1分钟
					{
						s_ct_is_lessthan_onemin = TRUE;
					}
				}
				//发消息到IDLE或倒计时界面显示
				SendMsgtoDispCoutime();
			}
		}
		else if (s_ct_vibrator_time_id == timer_id)
		{
			//SCI_TRACE_LOW:"MMIAPICT_HandleTimer s_ct_vibrator_time_id in"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOUNTEDTIME_MAIN_883_112_2_18_2_1_48_79,(uint8*)"");
			MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_ALARM);
			MMK_StopTimer(s_ct_vibrator_time_id);
			s_ct_vibrator_time_id = 0;
		}
	}
#if defined MMI_GRID_IDLE_SUPPORT && defined MMI_GRID_COUNTEDTIME
	else if(APP_IN_IDLE == s_app_get_focus_to_run) 
	{
		MMIAPIWIDGET_HandleTimer(s_countedtime_time_id);
	}
#endif	
}


/*****************************************************************************/
//  Description : SendMsgtoDispCoutime
//  Global resource dependence : 
//  Author:       zhaohui
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SendMsgtoDispCoutime(void)
{
//    if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId()&&
//        ((s_ct_current_time.sec == 59)||(s_ct_is_lessthan_onemin)))//只在需要的时候才去刷新
    if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        if (MMIAPIIDLE_IdleWinIsOpen())
        {
            return MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_UPDATE_COUNTEDTIME, PNULL);
        }
    }   
    else if (MMIACC_COUTIME_WIN_ID == MMK_GetFocusWinId() || MMIACC_COUTIME_WIN_ID == MMK_GetFocusChildWinId())
    {
        if (MMK_IsOpenWin(MMIACC_COUTIME_WIN_ID))
        {
            return MMK_SendMsg(MMIACC_COUTIME_WIN_ID,MSG_ACC_UPDATE_COUNTEDTIME,PNULL);
        }
    }
    return TRUE;
}


/*****************************************************************************/
//  Description : to show counted time arrive prompt
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void ShowCountedTimeTip(void)
{

    MMI_STRING_T    prompt_str1 = {0};

    MMI_WIN_PRIORITY_E  win_priority   = WIN_LOWEST_LEVEL;
    uint32              time_out = 0;
        
    MMI_GetLabelTextByLang(TXT_TOOLS_COUNTEDTIME_ARRIVE/*倒计时时间到*/,&prompt_str1);
    

    win_priority = WIN_ONE_LEVEL;

    MMIDEFAULT_TurnOnBackLight();
    //@zhaohui,cr108565
#ifndef MMI_GRID_IDLE_SUPPORT
        s_is_countedtime_running = FALSE;
#else
        s_is_countedtime_running = STATE_STOP;
#endif
        s_ct_arrived_event_is_on = FALSE;
    MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str1,PNULL,IMAGE_PUBWIN_SUCCESS,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);

}
/*****************************************************************************/
//  Description : MMIAPICT_StartCountedTimeArrive
//  Global resource dependence : 
//  Author:       zhaohui
//  RETRUN: void
//  Note:    
/*****************************************************************************/
PUBLIC void MMIAPICT_StartCountedTimeArrive(void)
{
    s_ct_arrived_event_is_on = TRUE;

    s_ct_is_event_delayed = FALSE;//clear delayed event;

        if ( MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID) 
                         || MMIAPICC_IsInState(CC_IN_CALL_STATE)//in call
#ifndef _WIN32    
            || MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID)
             || MMIAPIUDISK_IsFormatWaitWinOpened()
         
        || MMIAPISD_IsFormatWaitWinOpened()
#endif
        //闹钟事件，日历事件，自动关机事件        
        //闹钟事件注掉，需求改变，希望在闹钟事件发生时，如果倒计时到，也能弹出提示界面 yaye.jiang
        //||MMIAPIALM_IsAliveWinOpened()
#ifdef VIDEO_PLAYER_SUPPORT
        ||MMIAPIVP_IsLiving()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
		|| MMIAPIATV_IsRecording()
            )
        {   
            // delay processing alarm
            //SCI_TRACE_LOW:"Mmicountedtime_main.c, delay ct arrive win pop"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOUNTEDTIME_MAIN_983_112_2_18_2_1_49_80,(uint8*)"");
            s_ct_is_event_delayed = TRUE;//push delayed event
          //  MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
            if (0 != s_ct_vibrator_time_id)
            {
                MMK_StopTimer(s_ct_vibrator_time_id);
                s_ct_vibrator_time_id = 0;
            }
#ifdef MMI_GRID_IDLE_SUPPORT
            s_app_get_focus_to_run = APP_IN_MENU;
#endif
          //  s_ct_vibrator_time_id = MMK_CreateTimerCallback(1000,  MMIAPICT_HandleTimer, NULL, FALSE);  
        }
        else
        {   
            //SCI_TRACE_LOW:"Mmicountedtime_main.c, open ct arrive win at once"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOUNTEDTIME_MAIN_998_112_2_18_2_1_49_81,(uint8*)"");
#ifndef MMI_GRID_IDLE_SUPPORT
            s_is_countedtime_running = FALSE;
#else
            s_is_countedtime_running = STATE_STOP;
#endif
            ShowCountedTimeArrive();

        }

}

/*****************************************************************************/
//  Description : to show counted time arrive prompt
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void ShowCountedTimeArrive(void)
{
    MMI_WIN_ID_T alert_win_id = MMIACC_COUTIME_ARRIVE_WIN_ID;//倒计时到窗口
    MMI_STRING_T    prompt_str1 = {0};

    SCI_TIME_T  time = {0};//从系统获得的时间
    uint8   timestr[CT_TIME_STRING_LENGTH*2+1] = {0};//用于显示时间的buffer
    MMI_STRING_T        prompt_str2 = {0};
    MMI_WIN_PRIORITY_E  win_priority   = WIN_LOWEST_LEVEL;
    uint32              time_out = 0;
    MMI_STRING_T        cur_time_str = {0};  
    MMI_STRING_T        prompt_str3 = {0};

    MMI_GetLabelTextByLang(TXT_TOOLS_COUNTEDTIME_ARRIVE/*倒计时时间到*/,&prompt_str1);
    /*需要换图片文字*/
    MMI_GetLabelTextByLang(TXT_CUR_CITYTIME/*当前时间: */, &cur_time_str);
    //prompt2
    //init the string, which will be displayed
    prompt_str2.wstr_len  = 0;
    prompt_str2.wstr_ptr = PNULL;

    TM_GetSysTime(&time);

    MMIAPISET_FormatTimeStrByTime(time.hour,time.min,timestr,(CT_TIME_STRING_LENGTH*2+1));
    prompt_str2.wstr_len = strlen((char *)timestr);
    
    // allocate memory
    prompt_str2.wstr_ptr = SCI_ALLOC_APP((prompt_str2.wstr_len+1)*sizeof(wchar));
    if(PNULL == prompt_str2.wstr_ptr)
    {
        return;
    }

//  prompt_str2.is_ucs2 = FALSE;/*不知此处该如何处理*/
    // copy the num
    MMI_STRNTOWSTR(
                prompt_str2.wstr_ptr,
                prompt_str2.wstr_len,
                timestr,  
                prompt_str2.wstr_len,
                prompt_str2.wstr_len
                );

    /**合并显示字符: "当前时间:" + "prompt_str2"**/
    prompt_str3.wstr_ptr = SCI_ALLOC_APP((cur_time_str.wstr_len + prompt_str2.wstr_len + 1) *sizeof(wchar));
    if (PNULL == prompt_str3.wstr_ptr)
    {
        if (PNULL != prompt_str2.wstr_ptr)
        {
            SCI_FREE(prompt_str2.wstr_ptr);
            prompt_str2.wstr_ptr = PNULL;
        }
        return;
    }

    SCI_MEMSET(prompt_str3.wstr_ptr, 0x00, (cur_time_str.wstr_len + prompt_str2.wstr_len + 1) *sizeof(wchar));
    prompt_str3.wstr_len = cur_time_str.wstr_len + prompt_str2.wstr_len;
    MMIAPICOM_MergeTwoString(&cur_time_str, &prompt_str2, &prompt_str3, prompt_str3.wstr_len);

    win_priority = WIN_ONE_LEVEL;

    MMIDEFAULT_TurnOnBackLight();
    //@zhaohui,cr108565
    MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str1,&prompt_str3,IMAGE_PUBWIN_SUCCESS,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,MMIAPICT_HandleCountedTimeArriveWin);
    //free memory
    if (PNULL != prompt_str2.wstr_ptr)
    {
        SCI_FREE(prompt_str2.wstr_ptr);
        prompt_str2.wstr_ptr = PNULL;
    }
    if (PNULL != prompt_str3.wstr_ptr)
    {
        SCI_FREE(prompt_str3.wstr_ptr);
        prompt_str3.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : to handle the countedtime arrive window
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPICT_HandleCountedTimeArriveWin(
                                    MMI_WIN_ID_T        win_id,     //IN:
                                    MMI_MESSAGE_ID_E    msg_id,     //IN:
                                    DPARAM              param       //IN:
                                    )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_STRING_T            temp_str = {0};
    
    //SCI_TRACE_LOW:"MMIAPICT_HandleCountedTimeArriveWin: msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOUNTEDTIME_MAIN_1081_112_2_18_2_1_49_82,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //播放铃声和振动

        if(MMK_IsFocusWin(win_id))
        {
            s_ct_event_timer_id =  MMK_CreateWinTimer(win_id, CT_ALERT_RING_DURATION, FALSE);
            StartCountedTimeRingOrVib();  
        }
        s_ct_is_event_delayed = FALSE;
        MMI_GetLabelTextByLang(TXT_TOOLS_COUNTEDTIME, &temp_str);
#ifndef SUBLCD_SIZE_NONE
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_COUNTEDTIME,&temp_str);
#endif
        MMIPUB_SetWinTitleText(win_id, &temp_str, FALSE);
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        break;

    case MSG_FULL_PAINT:
        MMIPUB_SetWinSoftkey(MMIACC_COUTIME_ARRIVE_WIN_ID, TXT_NULL, STXT_EXIT, FALSE);

        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;

    case MSG_GET_FOCUS:
        if (0 != s_ct_event_timer_id)
        {
            // MMK_StopTimer(s_ct_event_timer_id);
            //s_ct_event_timer_id = 0;
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            StartCountedTimeRingOrVib();
        }
        else
        {
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        }
        //s_ct_event_timer_id =  MMK_CreateWinTimer(win_id, CT_ALERT_RING_DURATION, FALSE);
        //StartCountedTimeRingOrVib();  
        //recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);//@zhaohui,cr108565
        break;

    case MSG_LOSE_FOCUS:
        StopCountedTimeRingOrVib(FALSE);
        break;

    case MSG_TIMER:
        if (s_ct_vibrator_time_id ==  *( GUI_TIMER_ID_T*)param)//振动
        {
            if (s_ct_is_vibrator_on)
            {
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_ALARM);
                s_ct_is_vibrator_on = FALSE;
                s_ct_vibrator_time_id = MMK_CreateTimer(MMI_3SECONDS, FALSE);
            }
            else
            {
                MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
                s_ct_is_vibrator_on = TRUE;
                s_ct_vibrator_time_id = MMK_CreateTimer(MMI_3SECONDS, FALSE);
            }
        }
        else if (s_ct_event_timer_id ==  *( GUI_TIMER_ID_T*)param)//显示1分钟到
        {
            // 停止
            StopCountedTimeRingOrVib(TRUE);
            if (0 != s_ct_vibrator_time_id)
            {
                MMK_StopTimer(s_ct_vibrator_time_id);
                s_ct_vibrator_time_id = 0;
            }   
            s_ct_is_vibrator_on = FALSE;
#ifdef MMI_KEY_LOCK_SUPPORT
            if (MMIAPIKL_IsPhoneLocked())
            {
                MMK_WinGrabFocus(MMIKL_CLOCK_DISP_WIN_ID);
            }
#endif
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        }     
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_CLOSE_WINDOW:
        StopCountedTimeRingOrVib(TRUE);
#ifndef MMI_GRID_IDLE_SUPPORT
        s_is_countedtime_running = FALSE;
#else
        s_is_countedtime_running = STATE_STOP;
#endif
        s_ct_arrived_event_is_on = FALSE;
        MMIDEFAULT_TurnOnBackLight();
#ifndef SUBLCD_SIZE_NONE
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_COUNTEDTIME,&temp_str);
#endif
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        // suspend java installing.
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, PNULL);
        break;

//#ifndef MMI_PDA_SUPPORT //CR162722
    case MSG_KEYUP_POWER:
//#endif
    case MSG_KEYLONG_RED:
    //case MSG_KEYDOWN_FLIP:
    case MSG_KEYDOWN_RED:       
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_PROMPTWIN_CANCEL:
        StopCountedTimeRingOrVib(TRUE);
        MMK_CloseWin(win_id);
        // 特殊处理red键
        if ((MSG_KEYDOWN_RED == msg_id) || ((MSG_KEYUP_POWER == msg_id)))//return idle
        {
#ifdef MMI_KEY_LOCK_SUPPORT
                if (MMIAPIKL_IsPhoneLocked())
                {
                    MMK_CloseWin(win_id);
#ifdef MMI_PDA_SUPPORT
                    recode = MMI_RESULT_FALSE;
#endif
                }
                else
#endif
                {
                    recode = MMI_RESULT_FALSE;
                }
        }
        break;

    default:
//@zhaohui,cr108965,start
        if ((msg_id>= MSG_KEYDOWN_UP && msg_id <= MSG_KEYDOWN_SPDW ) ||
            (MSG_KEYUP_FLIP == msg_id) ||
            (MSG_KEYDOWN_SPDW == msg_id) ||
            (MSG_KEYDOWN_UPSIDE == msg_id) || 
            (MSG_KEYDOWN_DOWNSIDE == msg_id)||
            (MSG_KEYREPEAT_UPSIDE == msg_id)||
            (MSG_KEYREPEAT_DOWNSIDE == msg_id)||
            (MSG_TP_PRESS_DOWN == msg_id))
        {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
            if (MSG_KEYUP_FLIP == msg_id)
            {
                MMIDEFAULT_HandleFlipKey(TRUE);  
            }
#endif            
        }
        else
        {
            recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        }
//@zhaohui,cr108965,end 
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : Start CountedTime arrive Ring or Vib
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
#ifdef WRE_SUPPORT
extern PUBLIC BOOLEAN WRE_IsFocusRunWin(void);
#endif
LOCAL void StartCountedTimeRingOrVib(void)
{
    uint8 ring_type = MMIAPIENVSET_GetActiveModeOptValue(0,ALARM_RING_TYPE);/*铃音，振动，静音等*/

    if(MMIAPIUDISK_UdiskIsRun()
        ||MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef JAVA_SUPPORT
        ||MMIAPIJAVA_IsFocusJAVARunWin()
#endif
#ifdef WRE_SUPPORT
        ||WRE_IsFocusRunWin()
#endif
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsLiving()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
#ifdef VT_SUPPORT
        || MMIAPIVT_IsVtCalling()
#endif
        || MMIAPIATV_IsRecording()
//NEWMS00220222
#if defined(BROWSER_SUPPORT) && defined(LOW_MEMORY_SUPPORT_32X16)  
        || MMIAPIBROWSER_IsRunning()   
#endif
        ) 
    {
        if((MMISET_MSG_RING == ring_type)||(MMISET_MSG_VIBRA_AND_RING == ring_type))
        {
            ring_type = MMISET_MSG_VIBRA;
        }        
    }
    else if ((MMIAPIFMM_IsFileOption()) ||
             (MMIAPIPICVIEW_IsDeleteFiles()))
    {
        if(MMISET_MSG_RING == ring_type
            || MMISET_MSG_VIBRA_AND_RING == ring_type)
        {
            ring_type = MMISET_MSG_VIBRA;
        }
    }
    
    switch (ring_type)
    {
    case MMISET_MSG_RING:
        //固定铃声1
        MMIAPISET_PlayRingById(TRUE,1,0xffff,MMISET_RING_TYPE_ALARM,PNULL);//闹钟 ，日历，倒计时采用同样的处理 
        break;

    case MMISET_MSG_VIBRA:
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_ALARM))
        {
            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
            s_ct_vibrator_time_id = MMK_CreateTimer(MMI_3SECONDS, FALSE);
            s_ct_is_vibrator_on = TRUE;
        }
        break;

    case MMISET_MSG_VIBRA_AND_RING:
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_ALARM))
        {
            //播放固定铃声
            MMIAPISET_PlayRingById(TRUE,1,0xffff,MMISET_RING_TYPE_ALARM,PNULL);
            //开启振动        
            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM);
            s_ct_vibrator_time_id = MMK_CreateTimer(MMI_3SECONDS, FALSE);
            s_ct_is_vibrator_on = TRUE;
        }
        break;

    default:
        break;
    }   
}

/*****************************************************************************/
//  Description : Stop ring or vibrate
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void StopCountedTimeRingOrVib(BOOLEAN is_close_ring_timer)
{
    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_ALARM);

    if (0 != s_ct_vibrator_time_id)
    {
        MMK_StopTimer(s_ct_vibrator_time_id);
        s_ct_vibrator_time_id = 0;
    }

    MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
    if(is_close_ring_timer)
    {
        if (0 != s_ct_event_timer_id)
        {
            MMK_StopTimer(s_ct_event_timer_id);
            s_ct_event_timer_id = 0;
        }   
    }
}

/*****************************************************************************/
//  Description : Open Coutedtime Win
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICT_OpenCoutimeWin(void)
{
    MMK_CreateWin((uint32*)COUTIME_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : Open Coutedtime Running Win
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICT_OpenCoutimeRunningWin(void)
{
	MMK_CreateWin((uint32*)COUTIME_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : get Current Running Time
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC SCI_TIME_T MMIAPICT_GetCurTime(void)
{
    return s_ct_current_time;
}

/*****************************************************************************/
//  Description : if show countedtime in idle or not
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_IsShowIdleCoutime(void)
{
    return (s_is_countedtime_running == STATE_RUNNING) ? TRUE:FALSE;
}

/*****************************************************************************/
//  Description : current Coutedtime is les than 1min
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_IsLessThanOnemin(void)
{
    return s_ct_is_lessthan_onemin;
}

/*****************************************************************************/
//  Description : CheckandStartDelayedEvent
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_CheckandStartDelayedCoutimeEvent(void)
{
    if (s_ct_is_event_delayed && s_ct_arrived_event_is_on)
    {
        ShowCountedTimeTip();
        return TRUE;
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : CheckDelayedEvent
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_CheckDelayedCoutimeEvent(void)
{
    if (s_ct_is_event_delayed && s_ct_arrived_event_is_on)
    {
        return TRUE;
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : Check if counted arrive win is opened
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_IsArriveWinOpened(void)
{
    if (MMK_IsOpenWin(MMIACC_COUTIME_ARRIVE_WIN_ID))
    {
     return TRUE;
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description :set both rect of bottons
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL void GetCTCTRLBothRect(
                             MMI_WIN_ID_T win_id,
                             GUI_BOTH_RECT_T *edit_ptr,
                             GUI_BOTH_RECT_T *button1_ptr,                          
                             GUI_BOTH_RECT_T *time_ptr
                             )
{
    GUI_BOTH_RECT_T both_rect;
    //uint16          v_height        = 0;   //client高度
    //uint16          h_height        = 0;
    //uint16          v_width        = 0;   //client高度
    //uint16          h_width        = 0;

    both_rect = MMITHEME_GetWinClientBothRect(win_id);//client区域,只包括可以修改的部分

    //v_height = both_rect.v_rect.bottom - both_rect.v_rect.top + 1;
    //h_height = both_rect.h_rect.bottom - both_rect.h_rect.top + 1;
    //v_width = both_rect.v_rect.right - both_rect.v_rect.left + 1;
    //h_width = both_rect.h_rect.right - both_rect.h_rect.left + 1;

//button
    button1_ptr->v_rect.bottom = both_rect.v_rect.bottom -CT_BUTTON_MARGIN;
    button1_ptr->h_rect.bottom = both_rect.h_rect.bottom -CT_BUTTON_MARGIN;
    button1_ptr->v_rect.top    = button1_ptr->v_rect.bottom - CT_BUTTON_HEIGHT_V + 1;
    button1_ptr->h_rect.top    = button1_ptr->h_rect.bottom - CT_BUTTON_HEIGHT_H + 1;
    button1_ptr->v_rect.left   = both_rect.v_rect.left + CT_BUTTON_MARGIN -1;
    button1_ptr->h_rect.left   = both_rect.h_rect.left + CT_BUTTON_MARGIN -1;
    button1_ptr->v_rect.right  = both_rect.v_rect.right - CT_BUTTON_MARGIN;
    button1_ptr->h_rect.right  = both_rect.h_rect.right - CT_BUTTON_MARGIN;

//edit_rect
    edit_ptr->v_rect.top       = both_rect.v_rect.top + CT_EDIT_MARGIN;
    edit_ptr->h_rect.top       = both_rect.h_rect.top + CT_EDIT_MARGIN;
    edit_ptr->v_rect.bottom    = button1_ptr->v_rect.top - CT_EDIT_MARGIN;
    edit_ptr->h_rect.bottom    = button1_ptr->h_rect.top - CT_EDIT_MARGIN;
    edit_ptr->v_rect.left      = both_rect.v_rect.left + CT_EDIT_MARGIN;
    edit_ptr->h_rect.left      = both_rect.h_rect.left + CT_EDIT_MARGIN;
    edit_ptr->v_rect.right     = both_rect.v_rect.right -CT_EDIT_MARGIN ;
    edit_ptr->h_rect.right     = both_rect.h_rect.right - CT_EDIT_MARGIN;

//time:显示时间的区域
    time_ptr->v_rect.top       = both_rect.v_rect.top;
    time_ptr->h_rect.top       = both_rect.h_rect.top;
    time_ptr->v_rect.bottom    = button1_ptr->v_rect.top;
    time_ptr->h_rect.bottom    = button1_ptr->h_rect.top;
    time_ptr->v_rect.left      = both_rect.v_rect.left;
    time_ptr->h_rect.left      = both_rect.h_rect.left;
    time_ptr->v_rect.right     = both_rect.v_rect.right;
    time_ptr->h_rect.right     = both_rect.h_rect.right;
 }

/*****************************************************************************/
//  Description : stop the countedtimer
//  Global resource dependence : 
//  Author:junxue zhu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICT_StopCoutedTime(void)
{
#ifdef TOUCH_PANEL_SUPPORT
		GUI_BG_T                button_pressed  = {0};//按下按键时，button的背景显示
	    GUI_BG_T                button_bg       = {0};

	    button_bg.bg_type   = GUI_BG_IMG;//背景为图片
	    button_pressed.bg_type = GUI_BG_IMG;

#ifndef MMI_GRID_IDLE_SUPPORT			
        s_is_countedtime_running=FALSE;
#else
        s_is_countedtime_running = STATE_STOP;
#endif				
#ifdef MMI_GRID_IDLE_SUPPORT
        s_app_get_focus_to_run = APP_NOT_RUN;
#endif
        MMK_StopTimer(s_countedtime_time_id);
        MMK_StopTimer(s_ct_vibrator_time_id);
        s_countedtime_time_id = 0;

        /*if (0==s_ct_current_time.hour && 0== s_ct_current_time.min)
        {
            s_ct_current_time.min++;
        }*/
        GUIEDIT_SetTime( MMICT_TIME_TIMEEDITBOX_CTRL_ID, s_ct_set_time.hour, s_ct_set_time.min, s_ct_set_time.sec);

        //button2 显示"启动"这几个字
        GUIBUTTON_SetTextId( MMICT_TIME_BUTTON_CTRL_ID,  TXT_COMMON_START);   
        button_bg.img_id    = IMAGE_CLOCK_TIMER_START;        
        button_pressed.img_id   = IMAGE_CLOCK_TIMER_START_PRESSED;
        GUIBUTTON_SetBg(MMICT_TIME_BUTTON_CTRL_ID, &button_bg);        
        GUIBUTTON_SetPressedBg(MMICT_TIME_BUTTON_CTRL_ID, &button_pressed);
        //取消edittouch控件的隐藏
        GUIAPICTRL_SetState(MMICT_TIME_TIMEEDITBOX_CTRL_ID,GUICTRL_STATE_INVISIBLE,FALSE);
       
    	MMK_SendMsg(MMIACC_COUTIME_WIN_ID, MSG_FULL_PAINT, PNULL);	
#endif
}

#else
/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICT_HandleTimer(uint8 timer_id, uint32 param)
{
    return ;
}

/*****************************************************************************/
//  Description : Open Coutedtime Win
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICT_OpenCoutimeWin(void)
{

}

/*****************************************************************************/
//  Description : Open Coutedtime Running Win
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICT_OpenCoutimeRunningWin(void)
{

}

/*****************************************************************************/
//  Description : get Current Running Time
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC SCI_TIME_T MMIAPICT_GetCurTime(void)
{
    SCI_TIME_T cur_time = {0};

    return cur_time;
}

/*****************************************************************************/
//  Description : if show countedtime in idle or not
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_IsShowIdleCoutime(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : current Coutedtime is les than 1min
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_IsLessThanOnemin(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : CheckandStartDelayedEvent
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_CheckandStartDelayedCoutimeEvent(void)
{   
    return FALSE;
}

/*****************************************************************************/
//  Description : CheckDelayedEvent
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_CheckDelayedCoutimeEvent(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : Check if counted arrive win is opened
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICT_IsArriveWinOpened(void)
{
    return FALSE;
}

#endif//COUNTEDTIME_SUPPORT


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
