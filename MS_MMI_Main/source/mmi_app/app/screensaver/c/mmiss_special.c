/*****************************************************************************
** File Name:      mmiidle_screensaver_special.c                                     *
** Author:                                                                   *
** Date:           05/12/2008                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe special screensaver                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/12/2008       bin.ji             Creat
******************************************************************************/

#define _MMIIDLE_SCREENSAVER_SPECIAL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_screensaver_trc.h"
#ifdef SCREENSAVER_SUPPORT
#include "window_parse.h"
#include "mmidisplay_data.h"
//#include "mmiss_export.h"
#include "mmiss_special.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "mmi_appmsg.h"
#include "mmiset_id.h"
#include "mmiset_wintab.h"
#include "mmiset_export.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
#include "guibutton.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


#define SCR_TIMEOUT     600 //屏保定时器时间


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

typedef struct
{
    MMIIDLE_SPESCR_TYPE_E   scr_type;      //屏保的类型，0: star，1: snake，2: seed
    BOOLEAN is_preview;     //TRUE, 预览窗口，FALSE，屏保运行窗口
}MMIIDLE_SPESCR_WINPARAM_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

 /**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

extern MMIIDLE_SPESCR_T g_star_scr;     //“星”屏保
extern MMIIDLE_SPESCR_T g_snake_scr;    //“贪吃蛇”屏保
extern MMIIDLE_SPESCR_T g_seed_scr;     //“生命游戏”屏保

//动态屏保表
MMIIDLE_SPESCR_T *s_specscr_table[SPESCR_MAX]=
{
    &g_star_scr,
    &g_snake_scr,
    &g_seed_scr
};


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 菜单type转成screentype
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
LOCAL MMIIDLE_SPESCR_TYPE_E ConvertSCRSaverType(
                                   MMISET_SCREENSAVER_TYPE_E menu_type
                                   );

/*****************************************************************************/
// 	Description : screentype转成菜单type
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
LOCAL MMISET_SCREENSAVER_TYPE_E ConvertSCRSaverMenuType(
                                                    MMIIDLE_SPESCR_TYPE_E scr_type
                                                    );

/*****************************************************************************/
// 	Description : 屏保的消息处理函数
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSpecialScreenSaverWinMsg(
                                           MMI_WIN_ID_T         win_id, 
                                           MMI_MESSAGE_ID_E     msg_id, 
                                           DPARAM               param
                                           );



/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/

// the window for screen saver 
WINDOW_TABLE(MMIIDLE_SPECIAL_SCREENSAVER_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_LOWEST_LEVEL ),
//    WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC( (uint32)HandleSpecialScreenSaverWinMsg ),   
	WIN_ID( MMIIDLE_SCREENSAVER_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef MMI_PDA_SUPPORT
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
#endif
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN), 
	END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 打开屏保预览窗口
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_OpenSpeScrSaverPreviewWin(MMIIDLE_SPESCR_TYPE_E   scr_type)
{
    MMIIDLE_SPESCR_WINPARAM_T *scr_win_ptr = SCI_ALLOC_APP(sizeof(MMIIDLE_SPESCR_WINPARAM_T));

    //SCI_ASSERT(scr_win_ptr);
    if ((PNULL == scr_win_ptr)
        || (scr_type >= SPESCR_MAX))
    {
        //SCI_TRACE_LOW:"MMIIDLE:MMIIDLE_OpenSpeScrSaverPreviewWin PNULL == scr_win_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SPECIAL_123_112_2_18_2_49_49_6,(uint8*)"");
        return;
    }

    scr_win_ptr->is_preview = TRUE;
    scr_win_ptr->scr_type = scr_type;
    MMK_CreateWin((uint32*)MMIIDLE_SPECIAL_SCREENSAVER_WIN_TAB, (ADD_DATA)scr_win_ptr);   
}

/*****************************************************************************/
// 	Description : 打开屏保窗口
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
void MMIAPIIDLESS_OpenSpecialScreenSaver(MMIIDLE_SPESCR_TYPE_E   scr_type)
{
    MMIIDLE_SPESCR_WINPARAM_T *scr_win_ptr = SCI_ALLOC_APP(sizeof(MMIIDLE_SPESCR_WINPARAM_T));

    //SCI_ASSERT(scr_win_ptr);
    if ((PNULL == scr_win_ptr)
        || (scr_type >= SPESCR_MAX))
    {
        //SCI_TRACE_LOW:"MMIIDLE:MMIAPIIDLESS_OpenSpecialScreenSaver PNULL == scr_win_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SPECIAL_143_112_2_18_2_49_49_7,(uint8*)"");
        return;
    }

    scr_win_ptr->is_preview = FALSE;
    scr_win_ptr->scr_type = scr_type;    
    MMK_CreateWin((uint32*)MMIIDLE_SPECIAL_SCREENSAVER_WIN_TAB, (ADD_DATA)scr_win_ptr);    
}

/*****************************************************************************/
// 	Description : screentype转成菜单type
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
LOCAL MMISET_SCREENSAVER_TYPE_E ConvertSCRSaverMenuType(
                                                    MMIIDLE_SPESCR_TYPE_E scr_type
                                                    )
{
    MMISET_SCREENSAVER_TYPE_E menu_type = MMISET_SCREENSAVER_CLOSED;

    switch (scr_type)
    {
    case SPESCR_STAR:
        menu_type = MMISET_SCREENSAVER_STAR;
        break;
    case SPESCR_SNAKE:
        menu_type = MMISET_SCREENSAVER_SNAKE;
        break;
    case SPESCR_SEED:
        menu_type = MMISET_SCREENSAVER_SEED;
        break;
    default:
        break;
    }
    return menu_type;
}

/*****************************************************************************/
// 	Description : 菜单type转成screentype
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC MMIIDLE_SPESCR_TYPE_E MMIAPIIDLESS_ConvertSCRSaverType(
                                                 MMISET_SCREENSAVER_TYPE_E menu_type
                                                 )
{
    return ConvertSCRSaverType(menu_type);
}

/*****************************************************************************/
// 	Description : 菜单type转成screentype
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
LOCAL MMIIDLE_SPESCR_TYPE_E ConvertSCRSaverType(
                                   MMISET_SCREENSAVER_TYPE_E menu_type
                                   )
{
    MMIIDLE_SPESCR_TYPE_E scr_type = SPESCR_MAX;

    switch (menu_type)
    {
    case MMISET_SCREENSAVER_STAR:
        scr_type = SPESCR_STAR;
        break;
    case MMISET_SCREENSAVER_SNAKE:
        scr_type = SPESCR_SNAKE;
        break;
    case MMISET_SCREENSAVER_SEED:
        scr_type = SPESCR_SEED;
        break;
    default:
        break;
    }
    return scr_type;    
}

/*****************************************************************************/
// 	Description : 屏保的消息处理函数
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSpecialScreenSaverWinMsg(
                                           MMI_WIN_ID_T         win_id, 
                                           MMI_MESSAGE_ID_E     msg_id, 
                                           DPARAM               param
                                           )
{
    MMI_RESULT_E                      result             = MMI_RESULT_TRUE;
    //static MMISET_SCREENSAVER_INFO_T  ssaver_info        = {0};
    static uint8 s_special_screensaver_timer = 0;
    MMIIDLE_SPESCR_WINPARAM_T *scr_win_ptr = MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T   softkey_id = 0;         
    MMI_HANDLE_T    softkey_handle = 0;
    
    //SCI_ASSERT(scr_win_ptr);
    if ((PNULL == scr_win_ptr)
        || (scr_win_ptr->scr_type >= SPESCR_MAX)
        || (PNULL == s_specscr_table[scr_win_ptr->scr_type]))
    {
        //SCI_TRACE_LOW:"HandleSpecialScreenSaverWinMsg ERROR! PNULL == scr_win_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SPECIAL_172_112_2_18_2_49_49_8,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
        #ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
            if(scr_win_ptr->is_preview)
            {
                MMIAPISET_CreatSetButton( win_id, MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID, (GUIBUTTON_CALLBACK_FUNC)MMIAPISET_SetOkButtonCallback, (GUIBUTTON_CALLBACK_FUNC)MMIAPISET_SetCancelButtonCallback);
                MMIAPISET_CreateSetButtonLayer(win_id,MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID);
            }            
        #endif
            if (PNULL != s_specscr_table[scr_win_ptr->scr_type]->init_proc)
            {
                s_specscr_table[scr_win_ptr->scr_type]->init_proc();
            }
            s_special_screensaver_timer = MMK_CreateWinTimer(win_id, SCR_TIMEOUT, FALSE);
        }
        break;
        
    case MSG_FULL_PAINT:
        if (PNULL != s_specscr_table[scr_win_ptr->scr_type]->redraw_proc)
        {
            s_specscr_table[scr_win_ptr->scr_type]->redraw_proc();
        }
        if (scr_win_ptr->is_preview)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,TXT_NULL,STXT_RETURN,FALSE);
        }
        else
        {          

            softkey_id = MMK_GetWinSoftkeyCtrlId(win_id);//销毁softkey,播放时不显示softkey
            softkey_handle = MMK_GetCtrlHandleByWin(win_id, softkey_id);
            MMK_DestroyControl(softkey_handle);
            MMK_SetWinSoftkeyCtrlId(win_id, 0);
        }
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
        MMIAPISET_AppendSetButtonLayer();
#endif
        break;
        
    case MSG_LOSE_FOCUS:
    case MSG_BACKLIGHT_TURN_OFF:
        if (scr_win_ptr->is_preview)
        {
            //失去焦点时要停止定时器，防止无法响应来电等事件
            MMK_StopTimer(s_special_screensaver_timer);
            s_special_screensaver_timer = 0;
        }
        else
        {
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_LEFT:    
        if (!scr_win_ptr->is_preview)
        {
            MMK_CloseWin(win_id);
            break;
        }
        {
            
            uint16 sel_item_index = ConvertSCRSaverMenuType(scr_win_ptr->scr_type);//scr_win_ptr->scr_type + MMISET_SCREENSAVER_SPECIAL_BEGIN;
            //SCI_ASSERT( (MMISET_SCREENSAVER_MAX_NUM > sel_item_index)&&(0 < sel_item_index));
            if((MMISET_SCREENSAVER_MAX_NUM <= sel_item_index) || (0 == sel_item_index))
            {
                //SCI_TRACE_LOW:"HandleSpecialScreenSaverWinMsg ERROR! MSG_APP_LEFT sel_item_index =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SPECIAL_234_112_2_18_2_49_50_9,(uint8*)"d",sel_item_index);
                break;
            }
            if (MMISET_SCREENSAVER_SPECIAL_BEGIN == sel_item_index)
            {
                sel_item_index = MMISET_SCREENSAVER_SEA_WORLD;
            }
            else
            {
                sel_item_index --;
            }      
            if (MMISET_SCREENSAVER_SPECIAL_BEGIN <= sel_item_index
            && MMISET_SCREENSAVER_SPECIAL_END >= sel_item_index)
            {
                MMK_CloseWin(win_id);
                //算法屏保                
                MMIIDLE_OpenSpeScrSaverPreviewWin(ConvertSCRSaverType(sel_item_index));/*lint !e64*/
                
            }
            else
            {   
                MMK_CloseWin(win_id);
                 //进入屏保预览窗口
                MMIAPISET_OpenScreenSaverWin(sel_item_index);                  
            }

        }  
        
        break;

    case MSG_APP_RIGHT:
        if (!scr_win_ptr->is_preview)
        {
            MMK_CloseWin(win_id);
            break;
        }
        {
            uint16 sel_item_index = ConvertSCRSaverMenuType(scr_win_ptr->scr_type);//scr_win_ptr->scr_type + MMISET_SCREENSAVER_SPECIAL_BEGIN;
            //SCI_ASSERT( (MMISET_SCREENSAVER_MAX_NUM > sel_item_index)&&(0 < sel_item_index));
            if((MMISET_SCREENSAVER_MAX_NUM <= sel_item_index) || (0 == sel_item_index))
            {
                //SCI_TRACE_LOW:"HandleSpecialScreenSaverWinMsg ERROR! MSG_APP_RIGHT sel_item_index =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SPECIAL_275_112_2_18_2_49_50_10,(uint8*)"d",sel_item_index);
                break;
            }
            if ((MMISET_SCREENSAVER_SPECIAL_END) == sel_item_index)
            {
                sel_item_index = MMISET_SCREENSAVER_SEA_WORLD;
            }
            else
            {
                sel_item_index ++;
            }          
            if (MMISET_SCREENSAVER_SPECIAL_BEGIN <= sel_item_index
            && MMISET_SCREENSAVER_SPECIAL_END >= sel_item_index)
            {
                MMK_CloseWin(win_id);

                //算法屏保                
                MMIIDLE_OpenSpeScrSaverPreviewWin(ConvertSCRSaverType(sel_item_index));/*lint !e64*/
                
            }
            else
            {   
                MMK_CloseWin(win_id);
                 //进入屏保预览窗口
                MMIAPISET_OpenScreenSaverWin(sel_item_index); 
                
            }

        }
        
        break;

    case MSG_BACKLIGHT_TURN_ON:
    case MSG_GET_FOCUS:
        if (scr_win_ptr->is_preview)
        {
            s_special_screensaver_timer = MMK_CreateWinTimer(win_id, SCR_TIMEOUT, FALSE);
        }
        break;

    case MSG_TIMER:
        if (s_special_screensaver_timer == *((uint8 *)param))
        {
            if (PNULL != s_specscr_table[scr_win_ptr->scr_type]->trigger_proc)
            {
                s_specscr_table[scr_win_ptr->scr_type]->trigger_proc();
            }
            MMK_StartTimer(s_special_screensaver_timer, SCR_TIMEOUT, FALSE);
            if (scr_win_ptr->is_preview)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,TXT_NULL,STXT_RETURN,TRUE);
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    //case MSG_LOSE_FOCUS:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if (!scr_win_ptr->is_preview)
        {
            MMK_CloseWin(win_id);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_special_screensaver_timer);
        s_special_screensaver_timer = 0;
        MMK_FreeWinAddData(win_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        if (scr_win_ptr->is_preview)
        {
            MMIAPISET_SaveCurScreenSaver(scr_win_ptr->scr_type);     /*lint !e64*/   
            MMK_CloseWin( win_id );
            break;
        }
        else
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );  
        break;
        
    default:
        if (scr_win_ptr->is_preview)
        {
            result = MMI_RESULT_FALSE;
        }
        else
        {
            if (((msg_id >= MSG_KEYDOWN_UP) && 
                (msg_id <= MSG_KEYDOWN_MIDDLE))
#ifdef FLIP_PHONE_SUPPORT                
                ||(MSG_KEYUP_FLIP == msg_id)
#endif                
                ||(MSG_KEYUP_SLIDE == msg_id))
            {
#ifdef LCD_SLIDE_SUPPORT
                if (MSG_KEYUP_SLIDE == msg_id)
                {
                    MMIDEFAULT_HandleSlideKey(MSG_KEYUP_SLIDE); 
                }
#endif
                MMK_CloseWin(win_id);
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }
        break;
    }

    return (result);
}

#endif



/*Edit by script, ignore 1 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527
