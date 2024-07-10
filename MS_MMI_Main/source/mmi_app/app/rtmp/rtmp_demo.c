#include "rtmp_demo.h"

#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "mmicc_export.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmi_common.h"


#include "rtmp_id.h"
#include "rtmp_text.h"
#include "rtmp_image.h"

#include "rtmp_player.h"

LOCAL MMI_RESULT_E  rtmp_demo_handle_msg(
	MMI_WIN_ID_T        win_id,
	MMI_MESSAGE_ID_E    msg_id,
	DPARAM              param);

BOOLEAN g_rtmp_need_stop_voice = TRUE;
extern uint8  g_yx_net_rtmp_url[1024];
BOOLEAN g_rtmp_is_start = FALSE;
BOOLEAN g_rtmp_win_is_open = FALSE;
WINDOW_TABLE(MMI_RTMP_DEMO_WIN_TAB) =
{
	WIN_FUNC((uint32)rtmp_demo_handle_msg),
	WIN_ID(MMI_RTMP_WIN_ID_DEMO),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_RTMP_CTRL_ID_LABEL_TITLE),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_HIDE_STATUS,
	END_WIN
};

uint8 g_zdt_rtmp_timer_id = 0;
uint32 g_zdt_rtmp_img_index = 0;
static uint8 rtmp_play_anim_timer_id = 0;
LOCAL void MMIZDT_StartRtmpAnimTimer()
{
    if(rtmp_play_anim_timer_id != 0)
    {
        MMK_StopTimer(rtmp_play_anim_timer_id);
    }
    rtmp_play_anim_timer_id =  MMK_CreateWinTimer(MMI_RTMP_WIN_ID_DEMO, 500,TRUE);

}

LOCAL void MMIZDT_StopRtmpAnimTimer()
{
    if(rtmp_play_anim_timer_id != 0)
    {
        MMK_StopTimer(rtmp_play_anim_timer_id);
    }
    rtmp_play_anim_timer_id = 0;
}

void MMIZDT_RTMP_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    MMK_CreateWin((uint32*)MMI_RTMP_DEMO_WIN_TAB, NULL);
}

void MMIZDT_RTMP_TimeStart(uint32 time_ms)
{
    if(0 != g_zdt_rtmp_timer_id)
    {
        MMK_StopTimer(g_zdt_rtmp_timer_id);
        g_zdt_rtmp_timer_id = 0;
    }
    
    g_zdt_rtmp_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        MMIZDT_RTMP_HandleTimer, 
                                                                        (uint32)PNULL, 
                                                                        FALSE);
}

void MMIZDT_RTMP_TimeStop(void)
{
    if(0 != g_zdt_rtmp_timer_id)
    {
        MMK_StopTimer(g_zdt_rtmp_timer_id);
        g_zdt_rtmp_timer_id = 0;
    }
}

BOOLEAN MMIZDT_RTMP_IsPlaying(void)
{
    return g_rtmp_is_start;
    #if 0
    if(MMK_IsOpenWin(MMI_RTMP_WIN_ID_DEMO))
    {
        return TRUE;
    }
    #endif
    return FALSE;  
}

PUBLIC void MMIAPIMENU_EnterRTMPDemo(void)
{
    if(MMICC_GetCallNum() > 0)
    {
        return;
    }
#ifdef FM_SUPPORT
    //¹Ø±ÕFM
    MMIAPIFM_Exit();
#endif
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    //¹Ø±ÕMP3
    MMIAPMAINWIN_Destory();
#endif
    g_rtmp_is_start = TRUE;
    if(MMK_IsOpenWin(MMI_RTMP_WIN_ID_DEMO))
    {
        g_rtmp_need_stop_voice = FALSE;
        MMK_CloseWin(MMI_RTMP_WIN_ID_DEMO);
    }
    else
    {
        g_rtmp_need_stop_voice = TRUE;
    }
    MMIZDT_RTMP_TimeStart(2000);
    MMIAPISET_PlayCustomRing(MMISET_CUST_RTMP_START,1);
}

PUBLIC void MMIAPIMENU_QuitRTMPDemo(void)
{
    MMIZDT_RTMP_TimeStop();
    if(MMK_IsOpenWin(MMI_RTMP_WIN_ID_DEMO))
    {
        g_rtmp_need_stop_voice = TRUE;
        MMK_CloseWin(MMI_RTMP_WIN_ID_DEMO);
    }
    g_rtmp_is_start = FALSE;
}

LOCAL MMI_IMAGE_ID_T RTMP_GetAnimalFrame(uint32 cur_frame_index)
{
    MMI_IMAGE_ID_T  anim_frame[6] = {IMAGE_RTMP_BTN_ANIM_1, IMAGE_RTMP_BTN_ANIM_2,IMAGE_RTMP_BTN_ANIM_3, IMAGE_RTMP_BTN_ANIM_4,IMAGE_RTMP_BTN_ANIM_5};
    if(cur_frame_index < 5)
    {
        return anim_frame[cur_frame_index];
    }
    return 0;
}
LOCAL MMI_RESULT_E  rtmp_demo_handle_msg(
	MMI_WIN_ID_T        win_id,
	MMI_MESSAGE_ID_E    msg_id,
	DPARAM              param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;

	switch (msg_id)
	{
	case MSG_OPEN_WINDOW: {
		GUI_RECT_T title_rect = { 40, 5, 40 + 165, 5 + 25 };
		MMI_STRING_T	text_buffer = { 0 };

            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_RTMP_PLAYING,&text_buffer);

		GUILABEL_SetRect(MMI_RTMP_CTRL_ID_LABEL_TITLE, &title_rect, FALSE);
		GUILABEL_SetFont(MMI_RTMP_CTRL_ID_LABEL_TITLE, MMI_DEFAULT_NORMAL_FONT, MMI_GRAY_WHITE_COLOR);
		GUILABEL_SetText(MMI_RTMP_CTRL_ID_LABEL_TITLE, &text_buffer, TRUE);
#if 0
		w_len = GUI_UTF8ToWstr(w_text, 255, (const uint8*)"Play", strlen("Play"));
		GUIBUTTON_SetRect(MMI_RTMP_CTRL_ID_BTN_PLAY, &play_rect);
		GUIBUTTON_SetRunSheen(MMI_RTMP_CTRL_ID_BTN_PLAY, FALSE);
		GUIBUTTON_SetText(MMI_RTMP_CTRL_ID_BTN_PLAY, &w_text, w_len);
		GUIBUTTON_SetFont(MMI_RTMP_CTRL_ID_BTN_PLAY, &btn_font);
		GUIBUTTON_SetPressedFg(MMI_RTMP_CTRL_ID_BTN_PLAY, &btn_bg);

		w_len = GUI_UTF8ToWstr(w_text, 255, (const uint8*)"Stop", strlen("Stop"));
		GUIBUTTON_SetRect(MMI_RTMP_CTRL_ID_BTN_STOP, &stop_rect);
		GUIBUTTON_SetRunSheen(MMI_RTMP_CTRL_ID_BTN_STOP, FALSE);
		GUIBUTTON_SetText(MMI_RTMP_CTRL_ID_BTN_STOP, &w_text, w_len);
		GUIBUTTON_SetFont(MMI_RTMP_CTRL_ID_BTN_STOP, &btn_font);
		GUIBUTTON_SetPressedFg(MMI_RTMP_CTRL_ID_BTN_STOP, &btn_bg);

		MMK_SetAtvCtrlEx(win_id, MMI_RTMP_CTRL_ID_BTN_PLAY, TRUE);
#endif
            g_rtmp_win_is_open = TRUE;
             g_rtmp_is_start = TRUE;
		MMIZDT_StartRtmpAnimTimer();
             rtmp_player_play(g_yx_net_rtmp_url);
             rtmp_player_set_cache_packet_count(100); // 26ms * 100 = 2.6 seconds
		break;
	}
	case MSG_TIMER:
        if (*(uint8*)param == rtmp_play_anim_timer_id)
        {
            if(g_zdt_rtmp_img_index < 4)
            {
                g_zdt_rtmp_img_index++;
            }
            else
            {
                g_zdt_rtmp_img_index = 0;
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

	case MSG_FULL_PAINT: 
        {
            GUI_RECT_T bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
            GUI_BG_T bg_ptr = { 0 };
            GUI_BG_DISPLAY_T bg_display = { 0 };
            MMI_IMAGE_ID_T cur_img_id = 0;
            GUI_POINT_T      point = {50, 240-40};
            GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
		bg_ptr.bg_type = GUI_BG_COLOR;
		bg_ptr.color = MMI_BLACK_COLOR;

		bg_display.win_handle = MMK_ConvertIdToHandle(win_id);
		bg_display.display_rect = bg_rect;
		bg_display.rect = bg_rect;

		GUI_DisplayBg(&bg_ptr, &bg_display, MMITHEME_GetDefaultLcdDev());		
            cur_img_id = RTMP_GetAnimalFrame(g_zdt_rtmp_img_index);
            point.x = 100;
            point.y = 100;
            if(0 != cur_img_id)
            {
                    GUIRES_DisplayImg(&point,
                        PNULL,
                        PNULL,
                        win_id,
                        cur_img_id,
                        &lcd_dev_info);
            }
        
	}
		break;

	case MSG_CTL_BUTTON_UP: {
		break;
	}

          case MSG_KEYDOWN_CAMERA:
            #ifndef RELEASE_INFO
                //acc
                rtmp_player_play("rtmp://121.37.10.152:1935/live/aactest");
            #endif
            break;

      case MSG_KEYDOWN_MP3:
            #ifndef RELEASE_INFO
                rtmp_player_stop();
            #endif
            break;

    
    case MSG_KEYDOWN_DOWNSIDE:
        break;
    case MSG_KEYLONG_DOWNSIDE:
        break;
    case MSG_KEYPRESSUP_DOWNSIDE:
        break;
    case MSG_KEYREPEAT_DOWNSIDE:
        break;
    case MSG_KEYUP_DOWNSIDE:
        break;

    case MSG_KEYDOWN_UPSIDE:
        break;
    case MSG_KEYLONG_UPSIDE:
        break;
    case MSG_KEYPRESSUP_UPSIDE:
        break;
    case MSG_KEYREPEAT_UPSIDE:
        break;
    case MSG_KEYUP_UPSIDE:
        break;

        
    case MSG_KEYDOWN_LEFT:
        break;
        
    case MSG_KEYDOWN_RIGHT:
       break;
	
	case MSG_TP_PRESS_LONG:
	case MSG_NOTIFY_PENLONGOK: {
		break;
	}

	case MSG_CTL_PENOK:
	case MSG_CTL_MIDSK: {
		break;
	}

	case MSG_KEYREPEAT_WEB:
	case MSG_KEYREPEAT_OK:
		break;

	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
       {
            #ifndef RELEASE_INFO
                MMK_CloseWin(win_id);
            #endif
		break;
	}
	case MSG_GET_FOCUS:
             MMIZDT_StartRtmpAnimTimer();
             rtmp_player_play(g_yx_net_rtmp_url);
             rtmp_player_set_cache_packet_count(100); // 26ms * 100 = 2.6 seconds
        break;
        
	case MSG_LOSE_FOCUS:
            MMIZDT_StopRtmpAnimTimer();
            rtmp_player_stop();
        break;
        
	case MSG_CLOSE_WINDOW:
                MMIZDT_StopRtmpAnimTimer();
                rtmp_player_stop();
                g_rtmp_is_start = FALSE;
                g_rtmp_win_is_open = FALSE;
                if(g_rtmp_need_stop_voice)
                {
                    MMIAPISET_PlayCustomRing(MMISET_CUST_RTMP_OVER,1);
                }
                
		break;

	case MSG_APP_RED:
            #ifndef RELEASE_INFO
                MMK_CloseWin(win_id);
            #endif
		break;

	default:
		recode = MMI_RESULT_FALSE;
		break;
	}

	return recode;
}



