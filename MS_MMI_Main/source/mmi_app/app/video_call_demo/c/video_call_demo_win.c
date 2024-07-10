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
#include "mmikl_export.h"
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

#include "guictrl_api.h"
#include "guitab.h"
#include "mmiatv_export.h"
#include "guiedit.h"

#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif

#include "cjson.h"
#include "mmiset_export.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "zdt_common.h"
#include "zdt_net.h"

uint8 dialing_timer_id = 0;
char * video_roomid = NULL;
char * video_ticket = NULL;
BOOLEAN g_is_makecallout = 0;


void Trace_Readboy_Log(const char *pszFmt, ...);


////////////////////////////////////// demo main list //////////////////////////////////////////////////
LOCAL void Display_DslVideoDemo_ListItem(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{

}


LOCAL MMI_RESULT_E HandleVideoMainWin(
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            
        }
        break;
    case MSG_FULL_PAINT:
        {

        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
        {

        }
        break;
#endif
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

WINDOW_TABLE(MMI_VIDEO_CALL_CONTACT_LIST_WIN_TAB) = 
{
    WIN_ID(WATCH_VIDEO_CALL_CONTACT_LIST_WIN_ID),
    WIN_FUNC((uint32)HandleVideoMainWin),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void Video_Call_CreateMainWin()
{
    MMI_WIN_ID_T win_id = WATCH_VIDEO_CALL_CONTACT_LIST_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = {0,0,239,239};

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    
    win_handle = MMK_CreateWin((uint32*)MMI_VIDEO_CALL_CONTACT_LIST_WIN_TAB, PNULL);
    MMK_SetWinRect(win_handle, &rect);
}
////////////////////////////////////// dialing //////////////////////////////////////////////////

LOCAL MMI_RESULT_E HandleVideoDialingWin(
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T hangup_rect = {80,155,160,235};
    
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            
        }
        break;
    case MSG_FULL_PAINT:
        {
            
        }
        break;
    case MSG_TIMER:
        {

        }
        break;
    case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_DOWN:
        {
        }
        break;
    case MSG_TP_PRESS_UP:
        {
            
        }
        break;
    case MSG_CLOSE_WINDOW:
        {

        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

WINDOW_TABLE(MMI_VIDEO_CALL_OUT_WIN_TAB) = 
{
    WIN_ID(WATCH_VIDEO_CALL_OUT_WIN_ID),
    WIN_FUNC((uint32)HandleVideoDialingWin),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL void Video_Call_Out()
{
    MMI_WIN_ID_T win_id = WATCH_VIDEO_CALL_OUT_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = {0,0,239,239};

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    
    win_handle = MMK_CreateWin((uint32*)MMI_VIDEO_CALL_OUT_WIN_TAB, PNULL);
    MMK_SetWinRect(win_handle, &rect);;
}

//////////////////////////////////// incomming ///////////////////////////////////////
LOCAL MMI_RESULT_E HandleVideoIncommingWin(
    MMI_WIN_ID_T win_id, 
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T hangup_rect = {80,150,160,230};
    GUI_RECT_T answer_rect = {140,150,220,230};

    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {

        }
        break;
    case MSG_FULL_PAINT:

        break;
    case MSG_TIMER:
        {

        }
        break;
    case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_DOWN:
        {

        }
        break;
    case MSG_TP_PRESS_UP:
        {

        }
        break;
    case MSG_CLOSE_WINDOW:
        {
            voipcall_params.is_clear_osdlayer = FALSE;
            voip_ring_off_action();
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

WINDOW_TABLE(MMI_VIDEO_CALL_INCOMMING_WIN_TAB) = 
{
    WIN_ID(WATCH_VIDEO_CALL_INCOMING_WIN_ID),
    WIN_FUNC((uint32)HandleVideoIncommingWin),
     CREATE_BUTTON_CTRL(IMG_ANSWER_ICON,  MMI_VIDEOCHAT_ACCEPT_BTN_ID),
    CREATE_BUTTON_CTRL(IMG_REJECT_ICON,  MMI_VIDEOCHAT_HANGUP_BTN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

//////////////////////////// video create //////////////////////////
LOCAL void Video_Call_Incoming()
{
    MMI_WIN_ID_T win_id = WATCH_VIDEO_CALL_INCOMING_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = {0,0,239,239};

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    
    win_handle = MMK_CreateWin((uint32*)MMI_DSLVIDEO_INCOMMING_WIN_TAB, PNULL);
    MMK_SetWinRect(win_handle, &rect);;
}

PUBLIC BOOLEAN MMIVIDEO_IsOpenVideoWin(void)
{
    return MMK_IsOpenWin(WATCH_VIDEO_CALL_WIN_ID);
}

