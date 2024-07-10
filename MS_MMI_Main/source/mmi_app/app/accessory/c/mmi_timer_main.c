/****************************************************************************
** File Name:      mmi_timer_main.c                            
** Author:                                                                   
** Date:            2011/8/1
** Copyright:             
** Description:    This file is used to integrate all of the timer items
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 7/11/2011        yayejiang          Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)

#include "guitab.h"
#include "mmidisplay_data.h"
#include "mmi_appmsg.h"
#include "mmipub.h"

#include "mmicc_export.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"

#include "os_api.h"
#include "mmk_type.h"
#include "mmi_menutable.h"
#include "mmi_default.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiacc_id.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "mmialarm_text.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_id.h"
#include "mmiidle_export.h"

//#include "guires.h"
#include "mmikl_export.h"
#include "guiedit.h"
#include "guistring.h"
#include "mmiidle_subwintab.h"
//#include "guictrl_api.h"    
#include "mmi_theme.h"

#include "mmi_timer_export.h" 
//#ifdef COUNTEDTIME_SUPPORT
#include "mmicountedtime_export.h"
//#ifdef STOPWATCH_SUPPORT
#include "mmistopwatch_export.h"
#include "mmi_id.h"
#include "mmipb_image.h"
#include "mmi_position.h"
#include "mmialarm_id.h"
#include "mmialarm_image.h"


/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE								 */
/*---------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//LOCAL MMI_CUR_TIMER_E s_timer_type = MMI_ST_TIMER;    //当前item,默认是stopwatch

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : the process message function of the StopWatch
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimerParentWin(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );
 
 /*****************************************************************************/
//  Description : 增加TAB项
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL void AppendTimerTabInfo(uint32 tab_num);


/*****************************************************************************/
//  Description :set both rect of bottons
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
//LOCAL void SetTabStyle(MMI_CTRL_ID_T ctrl_id);

//定义TAB父窗口
WINDOW_TABLE(TIMER_PARENT_WIN_TAB)=
{
    WIN_STATUSBAR,
    WIN_FUNC((uint32)HandleTimerParentWin),
    WIN_ID(MMI_TIMER_PARENT_WIN_ID),

    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMI_TIMER_PARENT_TAB_ID),    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMI_TIMER_PARENT_TAB_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    END_WIN
};

#if defined(STOPWATCH_SUPPORT) && defined(OLD_STOPWATCH_SUPPORT)
extern WINDOW_TABLE( STOPWTACH_WIN_TAB ) ;
#endif
#ifdef COUNTEDTIME_SUPPORT
extern WINDOW_TABLE( COUTIME_WIN_TAB ) ;
#endif
#ifdef WORLD_CLOCK_SUPPORT
extern WINDOW_TABLE( WCLK_WIN_TAB ) ; /*lint !e762*/
#endif
extern WINDOW_TABLE( ALARM_MAINWIN_TAB ) ;


/*****************************************************************************/
//  Description :  时钟功能主窗口的回调函数
//  Global resource dependence : 
//  Author:        yayejiang
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimerParentWin(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E record     = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:        
        GUITAB_SetState(MMI_TIMER_PARENT_TAB_ID, GUITAB_STATE_SWITCH_ITEM, FALSE);
        MMK_SetAtvCtrl(win_id, MMI_TIMER_PARENT_TAB_ID);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(win_id);
        break;

    default:
        record = MMI_RESULT_FALSE;
        break;
    }
    
    return record;
    
}


/*****************************************************************************/
//  Description : 增加TAB项
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL void AppendTimerTabInfo(uint32  tab_num)
{   
    MMI_STRING_T    str_text    = {0};
    uint8           tab_index   = 0;

    // 设置最大选项卡数 
    GUITAB_SetMaxItemNum(MMI_TIMER_PARENT_TAB_ID, tab_num);
   
#ifdef WORLD_CLOCK_SUPPORT
     //世界时间 
    MMI_GetLabelTextByLang(TXT_TOOLS_WORLDTIME, &str_text);
#ifdef MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
        GUITAB_AppendSimpleItem(
            MMI_TIMER_PARENT_TAB_ID,
            &str_text,
            IMAGE_CLOCK_WORLDCLOCK_ISTYLETAB_FOCUSED,
            IMAGE_CLOCK_WORLDCLOCK_ISTYLETAB_DEFAULT);
    else
#endif		
        GUITAB_AppendSimpleItem(
            MMI_TIMER_PARENT_TAB_ID,
            &str_text,
            IMAGE_CLOCK_WORLDCLOCK_TAB_FOCUSED,
            IMAGE_CLOCK_WORLDCLOCK_TAB_DEFAULT);
    
    GUITAB_AddChildWindow(MMI_TIMER_PARENT_TAB_ID, MMIACC_WCLK_WIN_ID, tab_index);
    tab_index++;
#endif
    
    //闹钟
    MMI_GetLabelTextByLang(TXT_ALARM_CLOCK, &str_text);
#ifdef MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
        GUITAB_AppendSimpleItem(
            MMI_TIMER_PARENT_TAB_ID,
            &str_text,
            IMAGE_CLOCK_ALARM_ISTYLETAB_FOCUSED,
            IMAGE_CLOCK_ALARM_ISTYLETAB_DEFAULT);
    else
#endif		
        GUITAB_AppendSimpleItem(
            MMI_TIMER_PARENT_TAB_ID,
            &str_text,
            IMAGE_CLOCK_ALARM_TAB_FOCUSED,
            IMAGE_CLOCK_ALARM_TAB_DEFAULT);
        
    GUITAB_AddChildWindow(MMI_TIMER_PARENT_TAB_ID, MMIALM_MAINWIN_ID, tab_index);
    tab_index++;

#ifdef STOPWATCH_SUPPORT
    // 秒表倒计时
    MMI_GetLabelTextByLang(TXT_ACC_STOPWATCH, &str_text);
#ifdef MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
        GUITAB_AppendSimpleItem(
            MMI_TIMER_PARENT_TAB_ID,
            &str_text,
            IMAGE_CLOCK_STOPWATCH_ISTYLETAB_FOCUSED,
            IMAGE_CLOCK_STOPWATCH_ISTYLETAB_DEFAULT);
    else
#endif		
        GUITAB_AppendSimpleItem(
            MMI_TIMER_PARENT_TAB_ID,
            &str_text,
            IMAGE_CLOCK_ALARM_STOPWATCH_FOCUSED,
            IMAGE_CLOCK_ALARM_STOPWATCH_DEFAULT);
    
    GUITAB_AddChildWindow(MMI_TIMER_PARENT_TAB_ID, MMISTOPWATCH_MAIN_WIN_ID, tab_index);
    tab_index++;
#endif

#ifdef COUNTEDTIME_SUPPORT    
    //倒计时
    MMI_GetLabelTextByLang(TXT_TOOLS_COUNTEDTIME, &str_text);
#ifdef MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
        GUITAB_AppendSimpleItem(
        MMI_TIMER_PARENT_TAB_ID,
        &str_text,
        IMAGE_CLOCK_TIMER_ISTYLETAB_FOCUSED,
        IMAGE_CLOCK_TIMER_ISTYLETAB_DEFAULT);
    else
#endif		
        GUITAB_AppendSimpleItem(
        MMI_TIMER_PARENT_TAB_ID,
        &str_text,
        IMAGE_CLOCK_ALARM_TIMER_FOCUSED,
        IMAGE_CLOCK_ALARM_TIMER_DEFAULT);

    GUITAB_AddChildWindow(MMI_TIMER_PARENT_TAB_ID, MMIACC_COUTIME_WIN_ID, tab_index);
    tab_index++;
#endif
}



/*****************************************************************************/
//  Description : Open StopWatch Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPI_OpenTimerWin( uint32 index )
{
    MMI_CUR_TIMER_E type = MMI_AL_TIMER;
    
    switch (index)
    {
        case ID_CLOCK:
            type = MMI_AL_TIMER;
        break;
#ifdef WORLD_CLOCK_SUPPORT        
        case ID_WORLDTIME:
            type = MMI_WC_TIMER;
        break;
#endif
        
        case ID_TOOLS_ALARM:
            type = MMI_AL_TIMER;
        break;
        
#ifdef STOPWATCH_SUPPORT         
        case ID_STOPWATCH:
            type = MMI_ST_TIMER;
        break;
#endif

#ifdef COUNTEDTIME_SUPPORT
        case ID_COUNTDOWN:
            type = MMI_CT_TIMER;
        break;
#endif

        default:
            type = MMI_AL_TIMER;
        break;
    }
//if only alarm left in clock
#if !defined(WORLD_CLOCK_SUPPORT) && !defined(STOPWATCH_SUPPORT) && !defined(COUNTEDTIME_SUPPORT)
    MMIAPIALM_OpenMainWin();
#else
    MMIAPI_OpenTimerWinByType(type);
#endif
}
/*****************************************************************************/
//  Description : Open StopWatch Win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPI_OpenTimerWinByType( MMI_CUR_TIMER_E type )
{
    MMI_WIN_ID_T win_id = 0;
    uint32 tab_num = 0;    
    MMK_CreateParentWin((uint32*)TIMER_PARENT_WIN_TAB, PNULL);
    
#ifdef WORLD_CLOCK_SUPPORT
    MMK_CreateChildWin(MMI_TIMER_PARENT_WIN_ID,(uint32*)WCLK_WIN_TAB, NULL);
    tab_num++;
#endif
    
    MMK_CreateChildWin(MMI_TIMER_PARENT_WIN_ID,(uint32*)ALARM_MAINWIN_TAB, NULL);
    tab_num++;
    
#if defined(STOPWATCH_SUPPORT) && defined(OLD_STOPWATCH_SUPPORT)
    MMK_CreateChildWin(MMI_TIMER_PARENT_WIN_ID,(uint32*)STOPWTACH_WIN_TAB, NULL);
    tab_num++;
#endif
#ifdef COUNTEDTIME_SUPPORT
    MMK_CreateChildWin(MMI_TIMER_PARENT_WIN_ID,(uint32*)COUTIME_WIN_TAB, NULL);
    tab_num++;
#endif

    AppendTimerTabInfo(tab_num);
    GUITAB_SetCurSel(MMI_TIMER_PARENT_TAB_ID, type);

#ifdef WORLD_CLOCK_SUPPORT    
    if ( MMI_WC_TIMER == type)//world clock
    {
        win_id = MMIACC_WCLK_WIN_ID;
    }
    else 
#endif
#ifdef STOPWATCH_SUPPORT
    if ( MMI_ST_TIMER == type)//stopwatch
    {
        win_id = MMISTOPWATCH_MAIN_WIN_ID;
    }
    else
#endif
#ifdef COUNTEDTIME_SUPPORT
    if ( MMI_CT_TIMER == type)//countdown
    {
        win_id = MMIACC_COUTIME_WIN_ID;
    }
    else 
#endif
    {
        win_id = MMIALM_MAINWIN_ID; //alarm
    }

    MMK_SetChildWinFocus(MMI_TIMER_PARENT_WIN_ID, win_id);   
}
#endif
