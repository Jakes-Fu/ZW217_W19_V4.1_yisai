#include "mmi_app_accessory_trc.h"
#ifdef STOPWATCH_SUPPORT
/****************************************************************************
** File Name:      mmistopwatch_main.c                            
** Author:                                                                   
** Date:            2011/6/27
** Copyright:             
** Description:    This file is used to process msg to main window of stop watch
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 6/27/2011        yayejiang          Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "dal_time.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "guibutton.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmipub.h"

#include "mmicc_export.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmialarm_export.h"

#include "os_api.h"
#include "mmk_type.h"


#include "mmi_default.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiacc_id.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "mmiacc_position.h"
#include "mmistopwatch_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_id.h"

#include "guires.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "guiedit.h"
#include "guistring.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
//#include "guictrl_api.h"    
#include "mmi_theme.h"
#include "mmiacc_nv.h"
#include "guictrl_api.h"

#include "mmi_timer_export.h"
#include "guitab.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define ST_TIME_STRING_LENGTH               32      // X...X:XX:XX.X(Сʱ���7λ
//#define CT_ALERT_RING_DURATION              56000   // about one minute
//#define MMICT_MAX_HOUR 99   //
//#define MMICT_MAX_MIN 59 //
//#define MMICT_MAX_SEC 59
//#define MMICT_MAX_DSEC 10


//#define MAX_COUNT_TIME 0xFFFFFFFF //����ʱʱ��:32λsecond(��dsecΪ��λ)





/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
//typedef uint8 MMIACC_ST_TIME_T[ST_TIME_STRING_LENGTH + 1];

typedef struct  //�����Լ���ʱ������
{
    uint8   dsec;       //one-tenth of the second-[0,9]
    uint8   sec;        // secondsafter the minute - [0,59] 
    uint8   min;        // minutesafter the hour - [0,59] 
    uint32  hour;       // hours since midnight - [0-...]
}MMI_ST_TIME_T;

typedef enum
{ 
  MMI_ST_STOP,
  MMI_ST_PAUSE,
  MMI_ST_RUNNING
} MMI_ST_STATUS_E;

typedef enum//ˢ�µ�time��ʾ����
{ 
  MMI_ST_TIME1,
  MMI_ST_TIME2
} MMI_ST_TIME_E;

typedef struct MMINV_ST_TIME_COUNT_T 
{
    uint32  count_no;
    uint64  count_time;
} MMINV_ST_TIME_COUNT_T;

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                           */
/*---------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//iphone ����ֻ��ļ�ʱ��������������ʾ��ʱ�䣬һ�����ܼ�ʱʱ�䣬�趨Ϊtime2
//һ���ǰ���count(�ƴ�)�����¼�ʱ��ʱ�䣬�趨Ϊtime1

LOCAL uint64 s_st_start_time = 0;   //ÿ�ΰ���start��ʱ�� (��dsecΪ��λ)
LOCAL uint64 s_st_count_time = 0;   //ÿ�ΰ���count��ʱ�� (��dsecΪ��λ) ����time1���൱��time2��s_st_start_time

LOCAL uint64 s_st_stop_time1  = 0;   //ÿ�ΰ���stopʱ��time1�Ѿ���ʱ������ʱ�� (��dsecΪ��λ)
LOCAL uint64 s_st_stop_time2  = 0;   //ÿ�ΰ���stopʱ��time2�Ѿ���ʱ������ʱ�� (��dsecΪ��λ)

LOCAL uint64 s_st_current_time1 = 0;//�ƴ�ʱ�䣬��ĿǰΪֹ������ʱ��(��dsecΪ��λ)
LOCAL uint64 s_st_current_time2 = 0;//�ܼ�ʱʱ��,��ĿǰΪֹ������ʱ��(��dsecΪ��λ)

LOCAL uint8  s_stopwatch_time_id = 0;//����timerID, ��ʱʱ��Ϊ0

LOCAL MMI_ST_STATUS_E s_stopwatich_status = MMI_ST_STOP;
LOCAL uint32  max_count_number = 0;

LOCAL MMINV_ST_TIME_COUNT_T st_time_count[ST_COUNT_RESTORE_NUM] = {0};

//LOCAL BOOLEAN s_st_is_event_delayed = FALSE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : the process message function of the StopWatch
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStopWatchWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );

/*****************************************************************************/
//  Description : when pen ok, handle the message
//  Global resource dependence : 
//  Author:
//  Note:   
/*****************************************************************************/
LOCAL void DisplayStopWatchBG(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : display the background pic of time running
//  Global resource dependence : 
//  Author:       
//  Note:   
/*****************************************************************************/
LOCAL void ST_DisplayTimeBg(
							MMI_WIN_ID_T win_id,
							const GUI_BOTH_RECT_T *rect_ptr
							);

/*****************************************************************************/
//  Description : DisplayCurTime
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ST_DisplayCurTime(
							 MMI_WIN_ID_T            win_id,
							 const GUI_BOTH_RECT_T   *rect_ptr,
							 GUISTR_STYLE_T          *style_ptr,
							 const uint64            curent_time,
                             const uint8             current_rect
							 );

/*****************************************************************************/
//  Description : get lcd dev info
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ST_GetCurTime( 
                uint8 *str1_ptr,
                const uint64 curent_time
                );

/*****************************************************************************/
//  Description : SendMsgtoDispStopWatch
//  Global resource dependence : 
//  Author:       
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SendMsgtoDispStopWatch(void);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : the opration after button1 selected
//  Global resource dependence : 
//  Author:       
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void HandleButton1(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : the opration after button2 selected
//  Global resource dependence : 
//  Author:       
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void HandleButton2(MMI_WIN_ID_T win_id);
#endif

/*****************************************************************************/
//  Description :set both rect of bottons
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL void GetSTCTRLBothRect(
                             MMI_WIN_ID_T	 win_id,
                             GUI_BOTH_RECT_T *button1_ptr,                          
                             GUI_BOTH_RECT_T *button2_ptr,
                             GUI_BOTH_RECT_T *time1_ptr,
                             GUI_BOTH_RECT_T *time2_ptr,
                             GUI_BOTH_RECT_T *time_ptr,
                             GUI_BOTH_RECT_T *list_ptr
                             );	  

/*****************************************************************************/
//  Description :close the win of stopwatch
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
//LOCAL void CloseSTWin(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description :append empty list
//  Global resource dependence : 
//  Author:juan.wu
//  Note:   
/*****************************************************************************/
LOCAL void AppendEmptyList(MMI_CTRL_ID_T    ctrl_id);

/*****************************************************************************/
//  Description :append list item
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL void AppendListItem(
                         MMI_CTRL_ID_T    ctrl_id,
                         uint32           num,
                         uint64           count_time
                         );



WINDOW_TABLE( STOPWTACH_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleStopWatchWinMsg ),    
    WIN_ID( MMISTOPWATCH_MAIN_WIN_ID),
#ifdef MMI_ALARM_MINI_SUPPORT//MMI_STOPWATCH_MINI_SUPPORT ��alarm����ͬ����title��ʽ
    WIN_TITLE(TXT_ACC_STOPWATCH),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIST_LIST_CRTL_ID),
#ifdef TOUCH_PANEL_SUPPORT
    CREATE_BUTTON_CTRL(PNULL,MMIST_BUTTON1_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL,MMIST_BUTTON2_CRTL_ID),
#else
    WIN_SOFTKEY(TXT_COMMON_START, TXT_RESET, STXT_RETURN),
#endif    
    END_WIN
}; 





/*****************************************************************************/
//  Description : the process message function of the StopWatch
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStopWatchWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E    recode          = MMI_RESULT_TRUE;
//    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;//nv read�ķ���ֵ

    GUI_BOTH_RECT_T button1_rect    = {0};
    GUI_BOTH_RECT_T button2_rect    = {0};
    GUI_BOTH_RECT_T time1_rect      = {0}; //�ִμ�ʱʱ��
    GUI_BOTH_RECT_T time2_rect      = {0}; //�ܼ�ʱʱ��
    GUI_BOTH_RECT_T time_rect       = {0};  //
    GUI_BOTH_RECT_T *time1_ptr      = {0}; //time1ָ��
    GUI_BOTH_RECT_T *time2_ptr      = {0}; //time2ָ��
    GUI_BOTH_RECT_T *time_ptr       = {0};  //time������ָ��
    GUI_BOTH_RECT_T list_rect       = {0};  //

    GUISTR_STYLE_T  time_style      = {0};
#ifdef TOUCH_PANEL_SUPPORT
    GUI_BG_T        button_bg       = {0};
    GUI_BG_T        button_pressed  = {0};    
    
    GUI_FONT_ALL_T  button_font_all = {0};//button����������ɫ
    
    button_font_all.font  = MMI_DEFAULT_NORMAL_FONT ;
    button_font_all.color =  MMITHEME_GetCurThemeFontColor(MMI_THEME_MENU);
#endif

    GetSTCTRLBothRect(win_id, &button1_rect, &button2_rect, &time1_rect, &time2_rect, &time_rect, &list_rect);
    time1_ptr          = &time1_rect;
    time2_ptr          = &time2_rect;
    time_ptr           = &time_rect;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIAPICTRL_SetBothRect(MMIST_LIST_CRTL_ID, &list_rect);
#ifdef TOUCH_PANEL_SUPPORT
            GUIAPICTRL_SetBothRect(MMIST_BUTTON1_CTRL_ID, &button1_rect);
            GUIAPICTRL_SetBothRect(MMIST_BUTTON2_CRTL_ID, &button2_rect);
            GUILIST_SetNeedHiLightBar(MMIST_LIST_CRTL_ID,FALSE);
            //ST_DisplayTimeBg(win_id, &time1_rect);
            
            //����button1��λ�á����뷽ʽ������
            GUIBUTTON_SetTextAlign(MMIST_BUTTON1_CTRL_ID,  ALIGN_HVMIDDLE);
            GUIBUTTON_SetFont(MMIST_BUTTON1_CTRL_ID, &button_font_all);
            GUIBUTTON_SetRunSheen( MMIST_BUTTON1_CTRL_ID, FALSE );
            //����button2��λ�á����뷽ʽ������
            GUIBUTTON_SetTextAlign(MMIST_BUTTON2_CRTL_ID,  ALIGN_HVMIDDLE);
            GUIBUTTON_SetFont(MMIST_BUTTON2_CRTL_ID, &button_font_all);
            GUIBUTTON_SetRunSheen( MMIST_BUTTON2_CRTL_ID, FALSE );

            button_bg.bg_type   = GUI_BG_IMG;
            button_bg.img_id    = IMAGE_CLOCK_STOPWATCH_RESET_DEFAULT;
            button_pressed.bg_type = GUI_BG_IMG;
            button_pressed.img_id  = IMAGE_CLOCK_STOPWATCH_RESET_PRESSED;
            GUIBUTTON_SetBg(MMIST_BUTTON2_CRTL_ID, &button_bg);
            GUIBUTTON_SetPressedBg(MMIST_BUTTON2_CRTL_ID, &button_pressed);
#endif            
            //����button����ʾ�ַ�
            if (MMI_ST_RUNNING == s_stopwatich_status)
            {
#ifdef TOUCH_PANEL_SUPPORT
                GUIBUTTON_SetTextId(MMIST_BUTTON1_CTRL_ID,STXT_STOP);
                GUIBUTTON_SetTextId(MMIST_BUTTON2_CRTL_ID,TXT_ACC_COUNT);
                button_bg.img_id    = IMAGE_CLOCK_STOPWATCH_STOP;
                button_pressed.img_id = IMAGE_CLOCK_STOPWATCH_STOP_PRESSED;
                GUIBUTTON_SetBg(MMIST_BUTTON1_CTRL_ID, &button_bg);
                GUIBUTTON_SetPressedBg(MMIST_BUTTON1_CTRL_ID, &button_pressed);
#else                
                GUIWIN_SetSoftkeyTextId(win_id, STXT_STOP, TXT_ACC_COUNT, STXT_RETURN, FALSE);
#endif
                s_stopwatch_time_id = MMK_CreateWinTimer(win_id, 100, FALSE);
            } 
            else
            {
#ifdef TOUCH_PANEL_SUPPORT
                GUIBUTTON_SetTextId(MMIST_BUTTON1_CTRL_ID,TXT_COMMON_START);
                GUIBUTTON_SetTextId(MMIST_BUTTON2_CRTL_ID,TXT_RESET);
                button_bg.img_id    = IMAGE_CLOCK_STOPWATCH_START;
                button_pressed.img_id = IMAGE_CLOCK_STOPWATCH_START_PRESSED;
                GUIBUTTON_SetBg(MMIST_BUTTON1_CTRL_ID, &button_bg);
                GUIBUTTON_SetPressedBg(MMIST_BUTTON1_CTRL_ID, &button_pressed);
                /********�˴�������ɫ��button1 Ϊ��ɫ***********/
                /******����button2Ϊ�һ�*******/
                if( MMI_ST_STOP == s_stopwatich_status)
                {
                    button_bg.img_id    = IMAGE_CLOCK_STOPWATCH_RESET_DEFAULT;
                    GUIBUTTON_SetBg(MMIST_BUTTON2_CRTL_ID, &button_bg);

                    button_font_all.color =  MMI_GRAY_COLOR;
                    GUIBUTTON_SetFont(MMIST_BUTTON2_CRTL_ID, &button_font_all);
                    GUIBUTTON_SetGrayed(MMIST_BUTTON2_CRTL_ID, TRUE, &button_bg, TRUE);//�һ�
                }
#else              
                GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_START, TXT_NULL, STXT_RETURN, FALSE);
#endif
            }
	        
            //����list�������,list������
            GUILIST_SetMaxItem(MMIST_LIST_CRTL_ID, ST_COUNT_RESTORE_NUM, FALSE);

            if(MMI_ST_STOP == s_stopwatich_status)
            {
                AppendEmptyList(MMIST_LIST_CRTL_ID);
            }
            else
            {
                uint32 i = 0;
                if(0 == max_count_number)
                {
                    AppendEmptyList(MMIST_LIST_CRTL_ID);
                }
                else    //��count�ļ�¼ʱ
                {
                    if(max_count_number <= ST_COUNT_RESTORE_NUM)//Ŀǰ��¼��count����С��20ʱ
                    {
                        for(i = 0; i < max_count_number; i++)
                        {
                            AppendListItem(MMIST_LIST_CRTL_ID, (i + 1), st_time_count[i].count_time);
                        }
                    }
                    else
                    {
                        for(i = 0; i < ST_COUNT_RESTORE_NUM; i++)
                        {
                            AppendListItem(MMIST_LIST_CRTL_ID, (max_count_number - ST_COUNT_RESTORE_NUM + 1 + i), st_time_count[i].count_time);
                        }
                    }
                }
            }
            break;

        case MSG_FULL_PAINT:
            DisplayStopWatchBG(win_id);
            ST_DisplayTimeBg(win_id, time_ptr);
            ST_DisplayCurTime(win_id, time1_ptr, &time_style, s_st_current_time1, MMI_ST_TIME1);//û�п�ʼ��ʱʱ����ʾ00:00:00.0
            ST_DisplayCurTime(win_id, time2_ptr, &time_style, s_st_current_time2, MMI_ST_TIME2);//û�п�ʼ��ʱʱ����ʾ00:00:00.0
            break;

	case MSG_LCD_SWITCH: //�������л�
            break;

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                MMI_NOTIFY_T notify_param = {0};
                notify_param.src_id =  MMIST_BUTTON1_CTRL_ID ;
                MMK_SendMsg(win_id, MSG_CTL_PENOK , &notify_param );
            }
            break;

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:    
            {
                MMI_NOTIFY_T notify_param = {0};
                notify_param.src_id =  MMIST_BUTTON2_CRTL_ID ;
                MMK_SendMsg(win_id, MSG_CTL_PENOK , &notify_param );
            }
            break;
#else
        case MSG_CTL_OK:
        case MSG_APP_OK:
            {
                SCI_TICK_TIME_T tick_time      = {0};//��ǰʱ�̣�ticktime                
                uint64          current_time   = 0;  //��ticktimeת��Ϊ��dsecΪ��λ������
                
                if(MMI_ST_RUNNING ==  s_stopwatich_status)
                { 
                    MMK_StopTimer(s_stopwatch_time_id);//stop timer
                    s_st_stop_time1          = s_st_current_time1;
                    s_st_stop_time2          = s_st_current_time2;
                    s_stopwatich_status      = MMI_ST_PAUSE;
                    s_stopwatch_time_id      = 0;       //û�����У�����0
                
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_START, TXT_RESET, STXT_RETURN, FALSE);
                }
                //û��running������£����µ�Ϊstart(������stop���Ǵ�δ��ʼ�����ƴ���)
                else
                {   
                    SCI_GetTickTime(&tick_time);
                    current_time            = ((uint64)(tick_time.second) * 1000 + (uint64)(tick_time.milliseconds));
                    s_st_start_time         = current_time;
                    s_st_count_time         = current_time;
                    s_stopwatich_status     = MMI_ST_RUNNING;
                    s_stopwatch_time_id     = MMK_CreateWinTimer( win_id, 100, FALSE );            
                    
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_STOP, TXT_ACC_COUNT, STXT_RETURN, FALSE);
                }            
                MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
            }
            break;

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:    
            {
                SCI_TICK_TIME_T tick_time       = {0};//��ǰʱ�̣�ticktime                
                uint64          current_time   = 0;  //��ticktimeת��Ϊ��dsecΪ��λ������
                
                if(MMI_ST_STOP == s_stopwatich_status)
                {
                    break;
                }

                SCI_GetTickTime(&tick_time);
                current_time = ((uint64)(tick_time.second )* 1000 + (uint64)(tick_time.milliseconds));

                //�˴������жϣ��Ǽƴε����������reset�����!!!!!
                if(MMI_ST_RUNNING == s_stopwatich_status)//��ʱ�����µ�button2Ϊ"Count"
                {
                    s_st_stop_time1     = 0;
                    s_st_count_time     = current_time;
                    if(0 == max_count_number)
                    {
                        GUILIST_RemoveAllItems(MMIST_LIST_CRTL_ID);
                    }
                    max_count_number++;
                    if(max_count_number <= ST_COUNT_RESTORE_NUM)//Ŀǰ��¼��count����С��20ʱ
                    {
                        st_time_count[max_count_number - 1].count_no   = max_count_number;
                        st_time_count[max_count_number - 1].count_time = s_st_current_time1;
                        AppendListItem(MMIST_LIST_CRTL_ID, max_count_number, st_time_count[max_count_number-1].count_time);
                    }
                    else //(max_count_number >ST_COUNT_RESTORE_NUM)
                    {
                       memmove(st_time_count, &st_time_count[1], sizeof(MMINV_ST_TIME_COUNT_T)*(ST_COUNT_RESTORE_NUM - 1));
                       st_time_count[ST_COUNT_RESTORE_NUM - 1].count_no = max_count_number;
                       st_time_count[ST_COUNT_RESTORE_NUM - 1].count_time = s_st_current_time1;
                       GUILIST_RemoveItem(MMIST_LIST_CRTL_ID, (ST_COUNT_RESTORE_NUM- 1 ));//ɾ����ǰ�������(����list�������)
                       AppendListItem(MMIST_LIST_CRTL_ID, max_count_number, st_time_count[ST_COUNT_RESTORE_NUM-1].count_time);//��������
                    }
                    //MMI_WriteNVItem(MMINV_ST_TIME_COUNT_RESTORE, st_time_count);
                }
                else //���µ�button2Ϊ"Reset"
                {     
                    MMK_StopTimer(s_stopwatch_time_id);
                    s_stopwatch_time_id = 0;      //��0����־��ʱ����
                    s_st_current_time1  = 0;
                    s_st_current_time2  = 0;
                    s_st_start_time     = 0;
                    s_st_stop_time1     = 0;
                    s_st_stop_time2     = 0;
                    s_st_count_time     = 0;
                    
                    s_stopwatich_status = MMI_ST_STOP;
                    max_count_number    = 0;
                    
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_START, TXT_NULL, STXT_RETURN, FALSE);

                    SCI_MEMSET(st_time_count, 0, (ST_COUNT_RESTORE_NUM * sizeof(MMINV_ST_TIME_COUNT_T)));
                    GUILIST_RemoveAllItems(MMIST_LIST_CRTL_ID);//ȫ��item��Ϊ��
                    AppendEmptyList(MMIST_LIST_CRTL_ID);
                    //MMI_WriteNVItem(MMINV_ST_TIME_COUNT_RESTORE, st_time_count);
                }
                MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
            }
            break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
            if(MMIST_BUTTON1_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//button1��ѡ��
            {                
                HandleButton1(win_id);
            }
            else if(MMIST_BUTTON2_CRTL_ID == ((MMI_NOTIFY_T*)param)->src_id)//button2��reset��ѡ��
            {            
                HandleButton2(win_id);
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

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

        case MSG_LOSE_FOCUS:
            break;

//         case MSG_GET_FOCUS:
// // #ifdef PDA_UI_DROPDOWN_WIN
// //             s_timer_cur_item_num = MMI_ST_TIMER;
// // #endif 
//             break;

        case MSG_CLOSE_WINDOW:
            break;           

        case MSG_ACC_UPDATE_STOPWATCH:
            //��ʾʱ���ı�����
            ST_DisplayTimeBg(win_id, time_ptr);
            //��ʾ����ʱʱ����Ϣ
            ST_DisplayCurTime(win_id, time1_ptr, &time_style, s_st_current_time1, MMI_ST_TIME1);
            ST_DisplayCurTime(win_id, time2_ptr, &time_style, s_st_current_time2, MMI_ST_TIME2);
            break;
            
        case MSG_TIMER:
            if ( *(uint8*)param == s_stopwatch_time_id )
            {
                MMK_StopTimer(s_stopwatch_time_id);
                s_stopwatch_time_id = MMK_CreateWinTimer(win_id, 100, FALSE);
                MMIAPIST_APIST_HandleTimer(s_stopwatch_time_id, (uint32)(param));
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
//  Author:
//  Note:   
/*****************************************************************************/
LOCAL void DisplayStopWatchBG(MMI_WIN_ID_T win_id)
{
    //GUI_POINT_T         dis_point       = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          image_rect      = {0}; 
    
    GUI_BOTH_RECT_T     both_rect       = {0};

    both_rect = MMITHEME_GetWinClientBothRect(win_id);
    if(!MMK_IsWindowLandscape(win_id))//����
    {
        image_rect = both_rect.v_rect;
    }
    else
    {
        image_rect = both_rect.h_rect;
    }
    GUIRES_DisplayImg(PNULL,
        &image_rect,
        &image_rect,
        win_id,
        IMAGE_COMMON_BG, 
        &lcd_dev_info); 
}

/*****************************************************************************/
//  Description : display the background pic of time running
//  Global resource dependence : 
//  Author:       
//  Note:   
/*****************************************************************************/
LOCAL void ST_DisplayTimeBg(
                    MMI_WIN_ID_T win_id,
                    const GUI_BOTH_RECT_T *rect_ptr
                    )
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          rect_p       = {0};
    
//    uint16              image_width  = 0;
//    uint16              image_height = 0;    
//    GUI_RECT_T          image_rect   = {0};
//    MMI_IMAGE_ID_T      image_id     = 0;

    if(!MMK_IsWindowLandscape(win_id))//����
    {
        rect_p = (*rect_ptr).v_rect;
    }
    else
    {
        rect_p = (*rect_ptr).h_rect;
    }
    
    GUIRES_DisplayImg(PNULL,
        &rect_p,
        &rect_p,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
}

/*****************************************************************************/
//  Description : DisplayCurTime
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ST_DisplayCurTime(
                    MMI_WIN_ID_T            win_id,
                    const GUI_BOTH_RECT_T   *rect_ptr,
                    GUISTR_STYLE_T          *style_ptr,
                    const uint64            current_time,
                    const uint8             current_rect
                    )
{
    uint8                   time_str[ST_TIME_STRING_LENGTH+1] = {0};
    wchar                   time_temp[ST_TIME_STRING_LENGTH+1] = {0};
    MMI_STRING_T            cur_time     = {0};
 
    GUI_LCD_DEV_INFO        lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    //uint32                  width        = 0;
    //uint32                  height       = 0;
    size_t                  str_len      = 0;
    GUI_RECT_T              rect         = {0};
    
    GUISTR_STATE_T  state           = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;

    if(!MMK_IsWindowLandscape(win_id))//����
    {
        //width   = rect_ptr->v_rect.right - rect_ptr->v_rect.left + 1;
        //height  = rect_ptr->v_rect.bottom - rect_ptr->v_rect.top + 1;
        rect    = rect_ptr->v_rect;
        if(MMI_ST_TIME1 == current_rect)
        {
            style_ptr->font = GUI_ConvertSizeToGUIFont(ST_TIMER1_HEIGHT_V);
        } 
        else if(MMI_ST_TIME2 == current_rect)
        {
            style_ptr->font = GUI_ConvertSizeToGUIFont(ST_TIMER2_HEIGHT_V);
        }
    }
    else
    {  
        //width   = rect_ptr->h_rect.right - rect_ptr->h_rect.left + 1;        
        //height  = rect_ptr->h_rect.bottom - rect_ptr->h_rect.top + 1;
        rect    = rect_ptr->h_rect;
        if(MMI_ST_TIME1 == current_rect)
        {
            style_ptr->font = GUI_ConvertSizeToGUIFont(ST_TIMER1_HEIGHT_H);
        } 
        else if(MMI_ST_TIME2 == current_rect)
        {
            style_ptr->font = GUI_ConvertSizeToGUIFont(ST_TIMER2_HEIGHT_H);
        }
    }
    
    //��μ��
    if (GUI_IsInvalidRect(rect))
    {
        //SCI_TRACE_LOW:"ST_DisplayCurTime: the rect is invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTOPWATCH_MAIN_692_112_2_18_2_2_19_101,(uint8*)"");
        return;
    }
    ST_GetCurTime(time_str, current_time);
    str_len = strlen((char *)time_str);
    
    MMI_STRNTOWSTR(time_temp, ST_TIME_STRING_LENGTH, time_str, ST_TIME_STRING_LENGTH, str_len);
    cur_time.wstr_ptr = time_temp;
    cur_time.wstr_len = str_len;

    //��ȡ��ǰ�����������ɫ
    if(MMI_ST_TIME1 == current_rect)
    {
        style_ptr->font_color = MMI_DARK_GRAY_COLOR;
    }
    else if(MMI_ST_TIME2 == current_rect)
    {
        style_ptr->font_color = MMI_BLACK_COLOR;
    }
    style_ptr->align = ALIGN_RVMIDDLE;//ˮƽ�����Ҷ���,��ֱ�������
    // display
    GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    (const GUI_RECT_T       *)&rect,       //the fixed display area
                    (const GUI_RECT_T       *)&rect,       //�û�Ҫ���е�ʵ������
                    (const MMI_STRING_T     *)&cur_time,
                    style_ptr,
                    state,
                    GUISTR_TEXT_DIR_AUTO
                    ); 
}


/*****************************************************************************/
//  Description : get lcd dev info
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ST_GetCurTime( 
						 uint8 *str1_ptr,
						 const uint64 curent_time
						 )
{
    MMI_ST_TIME_T   curtime     = {0};
    uint32          time_temp   = (uint32)(curent_time / 100);

    uint32          time_sec    = 0;

    if (PNULL == str1_ptr)
    {
        return;
    }
    curtime.dsec    = (uint32)time_temp % 10;
    time_sec        = (uint32)time_temp / 10;//��secΪ��λ
    curtime.sec     = time_sec % 60; 
    time_sec        = time_sec / 60; //��minΪ��λ
    curtime.min     = time_sec % 60; 
    time_sec        = time_sec / 60; //��hourΪ��λ
    curtime.hour    = time_sec;

    if(!curtime.hour)//û�г���1Сʱ,ֻ��ʾ 00:00.0
    { 
        sprintf((char*)str1_ptr, "%02d:%02d.%d", curtime.min, curtime.sec,curtime.dsec);
    }
    else //����1Сʱ������ʾ 0...0:00:00.0
    {
        sprintf((char*)str1_ptr, "%d:%02d:%02d.%d", curtime.hour, curtime.min, curtime.sec,curtime.dsec);
    }
}

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIST_APIST_HandleTimer(uint8 timer_id, uint32 param)
{
    SCI_TICK_TIME_T tick_time      = {0};//��ǰʱ�̣�ticktime
    uint32          current_time   = 0;  //��ticktimeת��Ϊ��dsecΪ��λ������
    
    //SCI_TRACE_LOW:"MMIAPIST_APIST_HandleTimer timer_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISTOPWATCH_MAIN_770_112_2_18_2_2_20_102,(uint8*)"d",timer_id);
    if (s_stopwatch_time_id == timer_id)
    { 
        SCI_GetTickTime(&tick_time);
        current_time        = tick_time.second * 1000 + tick_time.milliseconds;
        s_st_current_time1  = s_st_stop_time1 + current_time - s_st_count_time;//��ǰ�ļƴ�ʱ��
        s_st_current_time2  = s_st_stop_time2 + current_time - s_st_start_time;//�ܼ�ʱʱ��
        SendMsgtoDispStopWatch();
    }       
}

/*****************************************************************************/
//  Description : SendMsgtoDispStopWatch
//  Global resource dependence : 
//  Author:       
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL BOOLEAN SendMsgtoDispStopWatch(void)
{ 
    //�ڴ����½���ˢ����ʾ
    if (MMISTOPWATCH_MAIN_WIN_ID == MMK_GetFocusWinId()||MMISTOPWATCH_MAIN_WIN_ID == MMK_GetFocusChildWinId())
    {
        if ( MMI_ST_RUNNING == s_stopwatich_status )//ֻ�м�ʱ����ʱ��ˢ��
        {
            return MMK_SendMsg(MMISTOPWATCH_MAIN_WIN_ID, MSG_ACC_UPDATE_STOPWATCH, PNULL);
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : Open StopWatch Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIST_OpenStopWatchWin(void)
{
    MMK_CreateWin((uint32*)STOPWTACH_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : chage the active button when press down the left/right key
//  Global resource dependence : 
//  Author:       
//  RETRUN:  
//  Note:    
/*****************************************************************************/
// LOCAL void StartMaxCountTimeArrive(void)
// {   
//     s_st_is_event_delayed = FALSE;//clear delayed event;
// 
//     //delay�����
//     if ( MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID) 
//                 || MMIAPICC_IsInState(CC_IN_CALL_STATE)//in call
// #ifndef _WIN32    
//                 || MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID)
//                 || MMIAPIUDISK_IsFormatWaitWinOpened()
//              
//                 || MMIAPISD_IsFormatWaitWinOpened()
// #endif
//                 //�����¼��������¼����Զ��ػ��¼�        
//                 ||MMIAPIALM_IsAliveWinOpened()
// #ifdef VIDEO_PLAYER_SUPPORT
//                 ||MMIAPIVP_IsLiving()
// #endif
// #ifdef MOBILE_VIDEO_SUPPORT
//                 || MMIAPIMV_IsLiving()
// #endif
//     )
//     {   
//         // delay processing alarm
//         SCI_TRACE_LOW("Mmistopwatch_main.c, delay ct arrive win pop");
//         s_st_is_event_delayed = TRUE;   
//         MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ALARM); 
//     }
//     else
//     {   
//         SCI_TRACE_LOW("Mmistopwatch_main.c, open st max_count_time arrive win at once");
//         s_stopwatich_status = FALSE;
//         s_st_is_event_delayed = FALSE;
//         ShowMaxCountTimeArrive();
//     }
// }

/*****************************************************************************/
//  Description : chage the active button when press down the left/right key
//  Global resource dependence : 
//  Author:       
//  RETRUN:  
//  Note:    
/*****************************************************************************/
// LOCAL void ShowMaxCountTimeArrive(void)
// {
//         MMI_STRING_T        prompt_str1 = {0};   
//             
//         MMI_GetLabelTextByLang(TXT_ACC_ST_ARRIVE_MAXTIME,&prompt_str1);
//     
//     
//         MMIDEFAULT_TurnOnBackLight();
//         MMIPUB_OpenAlertWinByTextPtr(PNULL,&prompt_str1,PNULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
// }

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : the opration after button1 selected
//  Global resource dependence : 
//  Author:       
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void HandleButton1(MMI_WIN_ID_T win_id)
{
    SCI_TICK_TIME_T tick_time      = {0};//��ǰʱ�̣�ticktime
    uint64          current_time   = 0;  //��ticktimeת��Ϊ��dsecΪ��λ������

    GUI_BG_T        button_bg      = {0};
    GUI_BG_T        button_pressed = {0};

    GUI_FONT_ALL_T  button_font_all = {0};//button����������ɫ
    button_bg.bg_type   = GUI_BG_IMG;
    button_pressed.bg_type = GUI_BG_IMG;

    //�����running�Ļ�
    if(MMI_ST_RUNNING ==  s_stopwatich_status)
    { 
        MMK_StopTimer(s_stopwatch_time_id);//stop timer
        s_st_stop_time1          = s_st_current_time1;
        s_st_stop_time2          = s_st_current_time2;
        s_stopwatich_status      = MMI_ST_PAUSE;
        s_stopwatch_time_id      = 0;       //û�����У�����0

        GUIBUTTON_SetTextId(MMIST_BUTTON1_CTRL_ID,TXT_COMMON_START);
        GUIBUTTON_SetTextId(MMIST_BUTTON2_CRTL_ID,TXT_RESET);

        button_bg.img_id    = IMAGE_CLOCK_STOPWATCH_START;
        button_pressed.img_id = IMAGE_CLOCK_STOPWATCH_START_PRESSED;
        GUIBUTTON_SetBg(MMIST_BUTTON1_CTRL_ID, &button_bg);
        GUIBUTTON_SetPressedBg(MMIST_BUTTON1_CTRL_ID, &button_pressed);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
    }
    //û��running������£����µ�Ϊstart(������stop���Ǵ�δ��ʼ�����ƴ���)
    else
    {	
        SCI_GetTickTime(&tick_time);
        current_time            = ((uint64)(tick_time.second) * 1000 + (uint64)(tick_time.milliseconds));
        s_st_start_time         = current_time;
        s_st_count_time			= current_time;
        s_stopwatich_status     = MMI_ST_RUNNING;
        s_stopwatch_time_id     = MMK_CreateWinTimer( win_id, 100, FALSE );

        button_bg.img_id    = IMAGE_CLOCK_STOPWATCH_STOP;
        button_pressed.img_id = IMAGE_CLOCK_STOPWATCH_STOP_PRESSED;
        GUIBUTTON_SetBg(MMIST_BUTTON1_CTRL_ID, &button_bg);
        GUIBUTTON_SetPressedBg(MMIST_BUTTON1_CTRL_ID, &button_pressed);

        if(MMI_ST_STOP == s_stopwatich_status)
        {
            button_bg.img_id    = IMAGE_CLOCK_STOPWATCH_RESET_DEFAULT;
            GUIBUTTON_SetBg(MMIST_BUTTON2_CRTL_ID, &button_bg);
        }

        GUIBUTTON_SetGrayed(MMIST_BUTTON2_CRTL_ID, FALSE, &button_bg, TRUE);//���һ�

        button_font_all.font = MMI_DEFAULT_NORMAL_FONT ;
        button_font_all.color =  MMITHEME_GetCurThemeFontColor(MMI_THEME_MENU);
        GUIBUTTON_SetFont(MMIST_BUTTON2_CRTL_ID, &button_font_all);
        GUIBUTTON_SetTextId(MMIST_BUTTON1_CTRL_ID,STXT_STOP);		
        GUIBUTTON_SetTextId(MMIST_BUTTON2_CRTL_ID,TXT_ACC_COUNT);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
    }
}

/*****************************************************************************/
//  Description : the opration after button2 selected
//  Global resource dependence : 
//  Author:       
//  RETRUN:  
//  Note:    
/*****************************************************************************/
LOCAL void HandleButton2(MMI_WIN_ID_T win_id)
{	
    SCI_TICK_TIME_T tick_time       = {0};//��ǰʱ�̣�ticktime
    uint64          current_time    = 0;  //��ticktimeת��Ϊ��dsecΪ��λ������

    GUI_BG_T        button_bg       = {0};
    GUI_BG_T        button_pressed  = {0};
    GUI_FONT_ALL_T  button_font_all = {0};//button����������ɫ
    button_bg.bg_type   = GUI_BG_IMG;
    button_pressed.bg_type = GUI_BG_IMG;
    
    //��δ������ʱ��ʱ��button2ΪGRAY��������!
    if(MMI_ST_STOP == s_stopwatich_status)
    {
        return;
    }

    SCI_GetTickTime(&tick_time);
    current_time = ((uint64)(tick_time.second )* 1000 + (uint64)(tick_time.milliseconds));

    //�˴������жϣ��Ǽƴε����������reset�����!!!!!
    if(MMI_ST_RUNNING == s_stopwatich_status)//��ʱ�����µ�button2Ϊ"Count"
    {
        s_st_stop_time1     = 0;
        s_st_count_time     = current_time;
        if(0 == max_count_number)
        {
            GUILIST_RemoveAllItems(MMIST_LIST_CRTL_ID);
        }
        max_count_number++;
        if(max_count_number <= ST_COUNT_RESTORE_NUM)//Ŀǰ��¼��count����С��20ʱ
        {
            st_time_count[max_count_number - 1].count_no   = max_count_number;
            st_time_count[max_count_number - 1].count_time = s_st_current_time1;
			AppendListItem(MMIST_LIST_CRTL_ID, max_count_number, st_time_count[max_count_number-1].count_time);
        }
        else //(max_count_number >ST_COUNT_RESTORE_NUM)
        {
           memmove(st_time_count, &st_time_count[1], sizeof(MMINV_ST_TIME_COUNT_T)*(ST_COUNT_RESTORE_NUM - 1));
           st_time_count[ST_COUNT_RESTORE_NUM - 1].count_no = max_count_number;
           st_time_count[ST_COUNT_RESTORE_NUM - 1].count_time = s_st_current_time1;
		   GUILIST_RemoveItem(MMIST_LIST_CRTL_ID, (ST_COUNT_RESTORE_NUM- 1 ));//ɾ����ǰ�������(����list�������)
		   AppendListItem(MMIST_LIST_CRTL_ID, max_count_number, st_time_count[ST_COUNT_RESTORE_NUM-1].count_time);//��������
        }
        //MMI_WriteNVItem(MMINV_ST_TIME_COUNT_RESTORE, st_time_count);
    }
    else //���µ�button2Ϊ"Reset"
    {     
        MMK_StopTimer(s_stopwatch_time_id);
        s_stopwatch_time_id = 0;      //��0����־��ʱ����
        s_st_current_time1  = 0;
        s_st_current_time2  = 0;
        s_st_start_time     = 0;
        s_st_stop_time1     = 0;
        s_st_stop_time2     = 0;
        s_st_count_time     = 0;
        
        s_stopwatich_status = MMI_ST_STOP;
        max_count_number    = 0;        
        GUILIST_RemoveAllItems(MMIST_LIST_CRTL_ID);        
        // button1����ʾ���ַ�����ɫ�ı仯      
        button_bg.img_id    = IMAGE_CLOCK_STOPWATCH_START;
        button_pressed.img_id = IMAGE_CLOCK_STOPWATCH_START_PRESSED;
        GUIBUTTON_SetBg(MMIST_BUTTON1_CTRL_ID, &button_bg);
        GUIBUTTON_SetPressedBg(MMIST_BUTTON1_CTRL_ID, &button_pressed);
        
        GUIBUTTON_SetTextId(MMIST_BUTTON1_CTRL_ID, TXT_COMMON_START);
        button_font_all.font = MMI_DEFAULT_NORMAL_FONT ;
        button_font_all.color =  MMI_GRAY_COLOR ;
        GUIBUTTON_SetFont(MMIST_BUTTON2_CRTL_ID, &button_font_all);
        GUIBUTTON_SetTextId(MMIST_BUTTON2_CRTL_ID, TXT_RESET);

        button_bg.img_id    = IMAGE_CLOCK_STOPWATCH_RESET_DEFAULT;
        GUIBUTTON_SetBg(MMIST_BUTTON2_CRTL_ID, &button_bg);
        GUIBUTTON_SetGrayed(MMIST_BUTTON2_CRTL_ID, TRUE, &button_bg, TRUE);//�һ�

        SCI_MEMSET(st_time_count, 0, (ST_COUNT_RESTORE_NUM * sizeof(MMINV_ST_TIME_COUNT_T)));
        GUILIST_RemoveAllItems(MMIST_LIST_CRTL_ID);//ȫ��item��Ϊ��
        AppendEmptyList(MMIST_LIST_CRTL_ID);
        //MMI_WriteNVItem(MMINV_ST_TIME_COUNT_RESTORE, st_time_count);
    }
    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
}
#endif

/*****************************************************************************/
//  Description :set both rect of bottons
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL void GetSTCTRLBothRect(
                             MMI_WIN_ID_T win_id,
                             GUI_BOTH_RECT_T *button1_ptr,                          
                             GUI_BOTH_RECT_T *button2_ptr,                                                
                             GUI_BOTH_RECT_T *time1_ptr,
                             GUI_BOTH_RECT_T *time2_ptr,
                             GUI_BOTH_RECT_T *time_ptr,
                             GUI_BOTH_RECT_T *list_ptr
                             )
{
    //���Ȼ�ȡclient������(�����п�����statusbar,title,�Լ�softkey�Ƿ���ڵ������
    //���Ƕ�softkey ����Ĭ�������·�~~~~���������Ҫ�޸�)
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);//client����,ֻ���������޸ĵĲ���;
#ifdef TOUCH_PANEL_SUPPORT
//    uint16          v_height        = 0;       //client�߶�
//    uint16          h_height        = 0;
    uint16          v_width        = 0;        //client���
    uint16          h_width        = 0;
    uint16          v_button_width = 0;
    uint16          h_button_width = 0;

//    v_height	= both_rect.v_rect.bottom - both_rect.v_rect.top + 1;
//    h_height	= both_rect.h_rect.bottom - both_rect.h_rect.top + 1;

    v_width		= both_rect.v_rect.right - both_rect.v_rect.left + 1;
    h_width		= both_rect.h_rect.right - both_rect.h_rect.left + 1;
#endif  
//time1
    time1_ptr->v_rect.top       = both_rect.v_rect.top+MMIST_TIME1_OFFSET_Y_V;
    time1_ptr->h_rect.top       = both_rect.h_rect.top+MMIST_TIME1_OFFSET_Y_H;
    time1_ptr->v_rect.bottom    = time1_ptr->v_rect.top + ST_TIMER1_HEIGHT_V- 1;
    time1_ptr->h_rect.bottom    = time1_ptr->h_rect.top + ST_TIMER1_HEIGHT_H- 1;
    time1_ptr->v_rect.left      = both_rect.v_rect.left;
    time1_ptr->h_rect.left      = both_rect.h_rect.left;
    time1_ptr->v_rect.right     = both_rect.v_rect.right -ST_BUTTON_OFFSET_X_V;
    time1_ptr->h_rect.right     = both_rect.h_rect.right -ST_BUTTON_OFFSET_X_H ;


//time2
    time2_ptr->v_rect.top       = time1_ptr->v_rect.bottom + MMIST_TIME2_OFFSET_Y_V;
    time2_ptr->h_rect.top       = time1_ptr->h_rect.bottom + MMIST_TIME2_OFFSET_Y_H;
    time2_ptr->v_rect.bottom    = time2_ptr->v_rect.top + ST_TIMER2_HEIGHT_V - 1;
    time2_ptr->h_rect.bottom    = time2_ptr->h_rect.top + ST_TIMER2_HEIGHT_H - 1;
    time2_ptr->v_rect.left      = both_rect.v_rect.left ;
    time2_ptr->h_rect.left      = both_rect.h_rect.left ;
    time2_ptr->v_rect.right     = both_rect.v_rect.right - ST_BUTTON_OFFSET_X_V;
    time2_ptr->h_rect.right     = both_rect.h_rect.right - ST_BUTTON_OFFSET_X_H;

//time(������ʱʹ�ã�û�б߽�)
    time_ptr->v_rect.top        = both_rect.v_rect.top;
    time_ptr->h_rect.top        = both_rect.h_rect.top;
    time_ptr->v_rect.bottom     = time2_ptr->v_rect.bottom;
    time_ptr->h_rect.bottom     = time2_ptr->h_rect.bottom;
    time_ptr->v_rect.left       = both_rect.v_rect.left;
    time_ptr->h_rect.left       = both_rect.h_rect.left;
    time_ptr->v_rect.right      = both_rect.v_rect.right;
    time_ptr->h_rect.right      = both_rect.h_rect.right;
#ifdef TOUCH_PANEL_SUPPORT
    v_button_width = (v_width - ST_BUTTON_MARGIN - ST_BUTTON_OFFSET_X_V*2)/2;
    h_button_width = (h_width - ST_BUTTON_MARGIN - ST_BUTTON_OFFSET_X_H*2)/2;
//botton1
    button1_ptr->v_rect.left    = both_rect.v_rect.left + ST_BUTTON_OFFSET_X_V;
    button1_ptr->h_rect.left    = both_rect.h_rect.left + ST_BUTTON_OFFSET_X_H;
    button1_ptr->v_rect.right   = button1_ptr->v_rect.left + v_button_width - 1;
    button1_ptr->h_rect.right   = button1_ptr->h_rect.left + h_button_width - 1;
    button1_ptr->v_rect.top		= time2_ptr->v_rect.bottom + MMIST_TIME2_OFFSET_Y_V;
    button1_ptr->h_rect.top		= time2_ptr->h_rect.bottom + MMIST_TIME2_OFFSET_Y_H;
    button1_ptr->v_rect.bottom  = button1_ptr->v_rect.top + ST_BUTTON_HEIGHT_V - 1;
    button1_ptr->h_rect.bottom  = button1_ptr->h_rect.top + ST_BUTTON_HEIGHT_H - 1;

//button2
    button2_ptr->v_rect.right   = both_rect.v_rect.right - ST_BUTTON_OFFSET_X_V;
    button2_ptr->h_rect.right   = both_rect.h_rect.right - ST_BUTTON_OFFSET_X_H;
    button2_ptr->v_rect.left    = button2_ptr->v_rect.right - v_button_width + 1;
    button2_ptr->h_rect.left    = button2_ptr->h_rect.right  - h_button_width + 1;
    button2_ptr->v_rect.top		= time2_ptr->v_rect.bottom + MMIST_TIME2_OFFSET_Y_V;
    button2_ptr->h_rect.top		= time2_ptr->h_rect.bottom + MMIST_TIME2_OFFSET_Y_H;
    button2_ptr->v_rect.bottom  = button1_ptr->v_rect.top + ST_BUTTON_HEIGHT_V - 1;
    button2_ptr->h_rect.bottom  = button1_ptr->h_rect.top + ST_BUTTON_HEIGHT_H - 1;

//list
//#ifdef TOUCH_PANEL_SUPPORT
    list_ptr->v_rect.top        = button1_ptr->v_rect.bottom+1;
    list_ptr->h_rect.top        = button1_ptr->h_rect.bottom+1;
    list_ptr->v_rect.bottom     = both_rect.v_rect.bottom ;
    list_ptr->h_rect.bottom     = both_rect.h_rect.bottom;
    list_ptr->v_rect.left       = both_rect.v_rect.left;
    list_ptr->h_rect.left       = both_rect.h_rect.left;
    list_ptr->v_rect.right      = both_rect.v_rect.right;
    list_ptr->h_rect.right      = both_rect.h_rect.right;
#else
    list_ptr->v_rect.top        = time2_ptr->v_rect.bottom + MMIST_TIME2_OFFSET_Y_V;
    list_ptr->h_rect.top        = time2_ptr->h_rect.bottom + MMIST_TIME2_OFFSET_Y_H;
    list_ptr->v_rect.bottom     = both_rect.v_rect.bottom ;
    list_ptr->h_rect.bottom     = both_rect.h_rect.bottom;
    list_ptr->v_rect.left       = both_rect.v_rect.left;
    list_ptr->h_rect.left       = both_rect.h_rect.left;
    list_ptr->v_rect.right      = both_rect.v_rect.right;
    list_ptr->h_rect.right      = both_rect.h_rect.right;
#endif
}

#if 0///////nvȥ�������Դ˴�����Ҫ��
/*****************************************************************************/
// 	Description : init stopwatch
//	Global resource dependence : 
//  Author:      yaye.jiang
//	Note:
/*****************************************************************************/

PUBLIC BOOLEAN MMISTOPWATCH_Init(void)//�ú����ڿ���ʱ����
{
    //uint16                  nv_return       = 0;
    MMI_ST_STATUS_E         st_status;
    MMINV_ST_TIME_CURREN_T  st_time_current = {0};

	SCI_TICK_TIME_T tick_time      = {0};//��ǰʱ�̣�ticktime
    uint64          current_time   = 0;  //��ticktimeת��Ϊ��dsecΪ��λ������
	
	uint32			i			   = 0;
    SCI_MEMSET(st_time_count, 0x00, (ST_COUNT_RESTORE_NUM * sizeof(MMINV_ST_TIME_COUNT_T)));
    MMI_ReadNVItem(MMI_ST_TIME_STATUE, &st_status);
    if(MMI_ST_STOP == st_status)
    {
        MMI_WriteNVItem(MMINV_ST_TIME_CURREN_RESTORE, &st_time_current);//���
        MMI_WriteNVItem(MMINV_ST_TIME_COUNT_RESTORE, st_time_count);
    }
    else
    {
        MMI_ReadNVItem(MMINV_ST_TIME_CURREN_RESTORE, &st_time_current);
        MMI_ReadNVItem(MMINV_ST_TIME_COUNT_RESTORE, st_time_count);
        
        SCI_GetTickTime(&tick_time);
        current_time        = tick_time.second * 1000 + tick_time.milliseconds;
        s_st_start_time     = st_time_current.start_time;
        s_st_count_time     = st_time_current.count_time;
        s_st_stop_time1     = st_time_current.stop_time1;
        s_st_stop_time2     = st_time_current.stop_time2;
        s_st_current_time1  = st_time_current.current_time1;
        s_st_current_time2  = st_time_current.current_time2;

        for(i = 0; (i <= ST_COUNT_RESTORE_NUM) && (0 != st_time_count[i].count_no); i++)
        {
            max_count_number = st_time_count[i].count_no;
        }
        if(MMI_ST_RUNNING == st_status)//running״̬��һ������Ӧ������������Ӧ�ü��´�ʱ��starttime��
        {
            s_st_start_time = current_time;
            s_st_count_time = current_time;
        }
    }
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description :close the win of stopwatch
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
// LOCAL void CloseSTWin(MMI_WIN_ID_T win_id)
// {
// #if 0///////nvȥ�������Դ˴�����Ҫ��
// 
//     MMINV_ST_TIME_CURREN_T  st_time_current = {0};//��¼��ǰ�ĸ�����ʱʱ��
//     
//     st_time_current.start_time      = s_st_start_time;
//     st_time_current.count_time      = s_st_count_time;
//     st_time_current.stop_time1      = s_st_stop_time1;
//     st_time_current.stop_time2      = s_st_stop_time2;
//     st_time_current.current_time1   = s_st_current_time1;
//     st_time_current.current_time2   = s_st_current_time2;
//     
//     //�����¼
//     MMI_WriteNVItem(MMI_ST_TIME_STATUE, &s_stopwatich_status);
//     MMI_WriteNVItem(MMINV_ST_TIME_CURREN_RESTORE, &st_time_current);
//     MMI_WriteNVItem(MMINV_ST_TIME_COUNT_RESTORE, st_time_count);
// #endif
//     MMK_CloseWin(win_id);
// }

/*****************************************************************************/
//  Description :append list item
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL void AppendListItem(
                         MMI_CTRL_ID_T    ctrl_id,
                         uint32           num,
                         uint64           count_time
                         )
{
    GUILIST_ITEM_T       item_t    = {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    MMI_STRING_T		 str_ptr   = {0};	 
    wchar temp_wstr1[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar temp_wstr2[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint8 temp_wstr[GUILIST_STRING_MAX_NUM + 1]	 = {0};
    uint16				 str_len   = 0;
    uint16				 str_len1  = 0;
    uint16               index = 0;

#ifdef MMI_STOPWATCH_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT;
#else
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;  
#endif 
    item_t.item_data_ptr = &item_data;

#ifndef MMI_STOPWATCH_MINI_SUPPORT
    //�����ļ���������ͼ��
    item_data.item_content[index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
#ifdef WORLD_CLOCK_SUPPORT
    item_data.item_content[index].item_data.image_id = IMAGE_CLOCK_WORLDCLOCK_DST;
#endif
    index++;
#endif
    //���üƴδ���
    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    //get text string
    MMITHEME_GetLabelTextByLang(TXT_ACC_COUNT,&str_ptr);
    str_len		= str_ptr.wstr_len;
    MMI_MEMCPY(temp_wstr1, sizeof(wchar) * str_len, str_ptr.wstr_ptr, sizeof(wchar) * str_len, sizeof(wchar) * str_len);
    sprintf((char*)(temp_wstr), ":%d", num);
    str_len1	= strlen((char *)temp_wstr);
    MMI_STRNTOWSTR(temp_wstr2, GUILIST_STRING_MAX_NUM, temp_wstr, GUILIST_STRING_MAX_NUM, str_len1);
    MMI_MEMCPY((temp_wstr1 + str_len), sizeof(wchar) * str_len1, temp_wstr2, sizeof(wchar) * str_len1, sizeof(wchar) * str_len1);
    str_len		+= str_len1;
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = temp_wstr1;
    item_data.item_content[index].item_data.text_buffer.wstr_len = str_len;
    
    index++;
    //���üƴ�ʱ��
    item_data.item_content[index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    ST_GetCurTime(temp_wstr, count_time);
    str_len		= strlen((char *)temp_wstr);

    MMI_STRNTOWSTR(temp_wstr2, GUILIST_STRING_MAX_NUM, temp_wstr, GUILIST_STRING_MAX_NUM, str_len);
    item_data.item_content[index].item_data.text_buffer.wstr_ptr = temp_wstr2;
    item_data.item_content[index].item_data.text_buffer.wstr_len = str_len;

    GUILIST_InsertItem(ctrl_id, &item_t, 0);//ÿ�ζ���ͷ�ϲ��룬ʹ������count�����ݷ���list��������
}

/*****************************************************************************/
//  Description :append empty list
//  Global resource dependence : 
//  Author:juan.wu
//  Note:   
/*****************************************************************************/
LOCAL void AppendEmptyList(MMI_CTRL_ID_T    ctrl_id)
{
    uint8  i = 0;
    GUILIST_ITEM_T       item_t    = {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    GUI_RECT_T           list_rect = {0};
    uint16               list_total_height = 0;
    uint16               max_num = 0;
#ifdef MMI_STOPWATCH_MINI_SUPPORT
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT;
#else
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;  
#endif
    item_t.item_data_ptr = &item_data;

    IGUICTRL_GetRect(MMK_GetCtrlPtr(ctrl_id), &list_rect);
    list_total_height = list_rect.bottom - list_rect.top + 1;
    max_num = list_total_height/MMI_LIST_ITEM_HEIGHT;
    for(i = 0; i < max_num; i++)
    {
        GUILIST_AppendItem(ctrl_id, &item_t);
    }
}
#endif// STOPWATCH_SUPPORT




/*Edit by script, ignore 1 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
