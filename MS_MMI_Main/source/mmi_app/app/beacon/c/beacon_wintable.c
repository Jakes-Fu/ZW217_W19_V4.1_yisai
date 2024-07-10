/****************************************************************************
** File Name:      mmiphsapp_wintab.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef _MMIBEACON_WINTAB_C_
#define _MMIBEACON_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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

#include "beacon_app.h"
#include "mmiidle_export.h"
#include "mmi_position.h"
#include "ldo_drvapi.h"


#define  BEACON_TEST_MENU_LEN  20
#define  BEACON_TEST_MENU_NUM  5

BOOLEAN MMIIB_OpenConnectWait_Win(void);
BOOLEAN MMIIB_CloseConnectWait_Win(void);
BOOLEAN MMIIB_UpdateConnectWait_Win(void);
BOOLEAN MMIIB_OpenConnectStatus_Win(void);
BOOLEAN MMIIB_CloseConnectStatus_Win(void);
BOOLEAN MMIIB_UpdateConnectStatus_Win(void);
BOOLEAN MMIIB_OpenConnectResult_Win(void);
BOOLEAN MMIIB_CloseConnectResult_Win(void);
BOOLEAN MMIIB_UpdateConnectResult_Win(void);

BOOLEAN MMIIB_UpdateConnectWin(uint32 status);
void MMIIB_StopConnectCheckTimer();

extern BEACON_DATA_T g_cur_beacon_ts_data;
uint32 g_ib_last_play_num = 0;

const uint8 beacon_test_menu_text[BEACON_TEST_MENU_NUM][BEACON_TEST_MENU_LEN]= 
{
    "Trace Del",
    "Trace Save",
    "Log Enable",
    "Log Disable",
    "SCI_ASSERT"

};

#if 1
BOOLEAN MMIIB_TTS_PlayBT_Test(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //蓝牙跳绳测试
        uint16 wstr[12] = {0x84DD, 0x7259, 0x8DF3, 0x7EF3, 0x6D4B, 0x8BD5, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayConnectStart(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //开始连接跳绳
        uint16 wstr[12] = {0x5F00, 0x59CB, 0x8FDE, 0x63A5, 0x8DF3, 0x7EF3, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayConnectTimeOut(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //连接超时
        uint16 wstr[12] = {0x8FDE, 0x63A5, 0x8D85, 0x65F6, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayConnectOK(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //连接成功
        uint16 wstr[12] = {0x8FDE, 0x63A5, 0x6210, 0x529F, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayConnectFail(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //连接失败
        uint16 wstr[12] = {0x8FDE, 0x63A5, 0x5931, 0x8D25, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayConnectHWError(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //硬件异常
        uint16 wstr[12] = {0x786C, 0x4EF6, 0x5F02, 0x5E38, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayDisConnect(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //蓝牙跳绳已断开
        uint16 wstr[12] = {0x84DD, 0x7259, 0x8DF3, 0x7EF3, 0x5DF2, 0x65AD, 0x5F00, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayNoMac(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //未绑定跳绳
        uint16 wstr[12] = {0x672A, 0x7ED1, 0x5B9A, 0x8DF3, 0x7EF3, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayPostDataOK(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //数据上传成功
        uint16 wstr[12] = {0x6570, 0x636E, 0x4E0A, 0x4F20, 0x6210, 0x529F, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayCurNum(void)
{
    MMI_STRING_T play_str = {0};
    char utf8_str[200] = {0};
    uint16 ucs_str[200] = {0};
    uint32 ucs_len = 0;
    BEACON_DATA_T last_ts_data = g_cur_beacon_ts_data;
    if(g_ib_last_play_num != last_ts_data.cur_sum)
    {
        g_ib_last_play_num = last_ts_data.cur_sum;
        sprintf(utf8_str,"%d",last_ts_data.cur_sum);
        ucs_len = GUI_UTF8ToWstr(ucs_str, 200, utf8_str, strlen(utf8_str));
        #ifdef HERO_ENGINE_TTS_SUPPORT
            play_str.wstr_ptr = ucs_str;
            play_str.wstr_len = ucs_len;
            TTSAPI_PlayTextForZDT(&play_str,1,1);
        #endif
    }
    return TRUE;
}
#endif

LOCAL MMI_RESULT_E HandleBEACON_TestMainMenuWinMsg (
                                      MMI_WIN_ID_T   win_id, 	// 窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM            param		// 相应消息的参数
                                      );

WINDOW_TABLE( MMIBEACON_TEST_MENU_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleBEACON_TestMainMenuWinMsg),    
    WIN_ID( MMIBEACON_TEST_MENU_WIN_ID ),
    WIN_TITLE(TXT_BEACON_TEST),
    //WIN_STATUSBAR,
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMIBEACON_TEST_MENU_CTRL_ID),
    END_WIN
};

BOOLEAN MMIBEACON_OpenTestMenuWin(void)
{
    MMK_CreateWin((uint32*)MMIBEACON_TEST_MENU_WIN_TAB,PNULL);
}

LOCAL void MMIAPISET_CommonTextId(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}
/*****************************************************************************/
// 	Description : to handle the message of atcmd test 
//	Global resource dependence : 
//  Author:jianshengqi
//	Note: 2006/3/19 
/*****************************************************************************/
//extern uint32 test_main_lcd_id[20];
//extern uint32 test_sub_lcd_id[20];

LOCAL MMI_RESULT_E  HandleBEACON_TestMainMenuWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T            ctrl_id = MMIBEACON_TEST_MENU_CTRL_ID;
    uint16                    node_id = 0;
    static BOOLEAN            s_need_update   = FALSE;
  //  MMISET_VALIDATE_PRIVACY_RETURN_T    result_info = {MMISET_PROTECT_CL, FALSE};    
#if defined(DISABLE_STATUSBAR_DISPLAY) 
    GUI_RECT_T                          title_rect =   {0, 0, 239, 23};
#else
    GUI_RECT_T                          title_rect = {0, 18, 239, 18 + 23};
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 i;
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            MMI_STRING_T		    item_str = {0};
            uint8				copy_len = 0;
            wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};

            
            if (MMK_IsIncludeStatusBar(win_id))
            {
                GUIWIN_SetTitleRect(win_id, title_rect);   
            }  
            
            GUILIST_SetMaxItem(ctrl_id, BEACON_TEST_MENU_NUM,FALSE);   
            
            for ( i = 0; i < BEACON_TEST_MENU_NUM; i++ )
            {
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
                item_t.item_data_ptr = &item_data;
                copy_len = SCI_STRLEN(beacon_test_menu_text[i]);
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[0].item_data.image_id = IMAGE_SIM1;
                item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = buff;
                GUI_GB2UCS(item_data.item_content[1].item_data.text_buffer.wstr_ptr, beacon_test_menu_text[i],copy_len );
                item_data.item_content[1].item_data.text_buffer.wstr_len = MIN( GUILIST_STRING_MAX_NUM, copy_len);
                                   
                item_data.softkey_id[0] = TXT_COMMON_OK;
                item_data.softkey_id[1] = TXT_NULL;
                item_data.softkey_id[2] = STXT_RETURN;
                
                GUILIST_AppendItem(ctrl_id, &item_t);      
            }
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }

        break;
        
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    {
        node_id = GUILIST_GetCurItemIndex(  ctrl_id );
        node_id++;
        switch( node_id )
        {
#ifndef WIN32
            case  1:
                    #ifdef FILE_LOG_SUPPORT
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_DEL_ALL, 0,0);
                    #endif
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
                
            case  2:
                    #ifdef FILE_LOG_SUPPORT
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_SAVE, 0,0);
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_AT_SAVE, 0,0);
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_BUF_SAVE, 0,0);
                        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_USRE_LOG_SOCKET_SAVE, 0,0);
                    #endif
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
                
            case  3:
                {
                    REFPARAM_SetEnableArmLogFlag(TRUE);
                    #ifdef FILE_LOG_SUPPORT
                        TraceUser_Init();
                    #endif
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
                break;
                
            case  4:
                    REFPARAM_SetEnableArmLogFlag(FALSE);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
                                
            case  5:
                    SCI_ASSERT(0);
                break;
#endif
            default:
                break;
        }
    }          
        break;
       
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

static void Beacon_BackLight(BOOLEAN is_alway_on)
{
      if(is_alway_on)
      {
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		MMIDEFAULT_TurnOnBackLight();
      }
      else
      {
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
      }
}

#if 1
static int s_ib_tp_x;
static int s_ib_tp_y;
static BOOLEAN s_is_ib_tp_down = FALSE;
static uint8 ib_wait_timer_id = 0;
static MMI_WIN_ID_T g_ib_cur_win_id =0;
static BOOLEAN ib_is_connecting =FALSE;
static char s_ib_cur_connect_mac_str[BEACON_MAC_MAX_LEN+1] = {0};
static uint32 s_connect_win_status = 0;
static BOOLEAN connect_win_is_need_disconnect = FALSE;
static BOOLEAN result_win_is_need_disconnect = FALSE;

BOOLEAN g_ib_is_timer_connect = FALSE;
extern uint8 g_ib_need_get_57_data;
void MMIIB_ConnectWait_Win_Draw(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    MMI_STRING_T	cur_str_t	= {0};
    MMI_STRING_T	other_str_t	= {0};
    GUI_RECT_T	 cur_rect		   = {0};
    GUI_RECT_T	 rect		 = {0}; 
    GUISTR_STYLE_T		text_style		= {0};
    GUISTR_STATE_T		state = 		GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big =SONG_FONT_30;
    GUI_RECT_T	 title_rect		   = {0};
    GUI_RECT_T	 other_rect		   = {0};
    BOOLEAN hw_is_ok = beacon_hw_is_ok();
    uint16 connect_title[20] = {0x6B63, 0x5728, 0x8FDE, 0x63A5, 0x8DF3, 0x7EF3, 0x002E, 0x002E, 0x002E, 0x0 }; // 正在连接跳绳...6B63 5728 8FDE 63A5 8DF3 7EF3 002E 002E 002E 
    uint16 hw_err[20] = {0x84DD, 0x7259, 0x6A21, 0x5757, 0x5F02, 0x5E38, 0x0021, 0x0021, 0x0021, 0x0 }; // 84DD 7259 6A21 5757 5F02 5E38 0021 0021 0021 
    uint16 no_mac[20] = {0x8BF7, 0x5148, 0x5728, 0x0041, 0x0070, 0x0070, 0x7AEF, 0x7ED1, 0x5B9A, 0x8DF3, 0x7EF3, 0x0021, 0x0 }; 
    uint16 connect_err[20] = {0x8DF3, 0x7EF3, 0x8FDE, 0x63A5, 0x5931, 0x8D25, 0xFF01, 0x0 }; 
    uint16 disp_wstr[50] = {0}; 
    rect.left	= 0;
    rect.top	= 0;
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    cur_rect = rect;
    title_rect =rect;
    other_rect=rect;
    
    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_BLACK_COLOR;
    
    if(ib_is_connecting)
    {
        if(hw_is_ok)
        {
             if(strlen(s_ib_cur_connect_mac_str) > 0)
             {
                 cur_str_t.wstr_ptr = connect_title;
                 cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
                 text_style.font_color = MMI_BLACK_COLOR;
             }
             else
             {
                 cur_str_t.wstr_ptr = no_mac;
                 cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
                 text_style.font_color = MMI_DARK_BLUE_COLOR;
             }
        }
        else
        {
             cur_str_t.wstr_ptr = hw_err;
             cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
             text_style.font_color = MMI_DARK_RED_COLOR;
        }
         text_style.align = ALIGN_HVMIDDLE;
         text_style.font = f_big;
         title_rect.top = 40;
         title_rect.bottom = 80;
         GUISTR_DrawTextToLCDInRect( 
                                         MMITHEME_GetDefaultLcdDev(),
                                         (const GUI_RECT_T      *)&title_rect,
                                         (const GUI_RECT_T      *)&title_rect,
                                         (const MMI_STRING_T    *)&cur_str_t,
                                         &text_style,
                                         state,
                                         GUISTR_TEXT_DIR_AUTO
                                     );
         
      }
      else
      { 
         cur_str_t.wstr_ptr = connect_err;
         cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
         text_style.font_color = MMI_DARK_RED_COLOR;
         text_style.align = ALIGN_HVMIDDLE;
         text_style.font = f_big;
         title_rect.top = 40;
         title_rect.bottom = 80;
         GUISTR_DrawTextToLCDInRect( 
                                         MMITHEME_GetDefaultLcdDev(),
                                         (const GUI_RECT_T      *)&title_rect,
                                         (const GUI_RECT_T      *)&title_rect,
                                         (const MMI_STRING_T    *)&cur_str_t,
                                         &text_style,
                                         state,
                                         GUISTR_TEXT_DIR_AUTO
                                     );
	}

      
    MMIAPICOM_StrToWstr(s_ib_cur_connect_mac_str, disp_wstr);
    cur_str_t.wstr_ptr = disp_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    text_style.font_color = MMI_BLACK_COLOR;
    if(cur_str_t.wstr_len > 0)
    {
        title_rect.top = 100;
        title_rect.bottom = 140;
        GUISTR_DrawTextToLCDInRect( 
                                        MMITHEME_GetDefaultLcdDev(),
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                    );
    }
    return;
}

LOCAL void MMIIB_StopWaitWinTimer()
{
    if(ib_wait_timer_id != 0)
    {
        MMK_StopTimer(ib_wait_timer_id);
        ib_wait_timer_id = 0;
    }
}

void MMIIB_HandleWaitWinTimer(uint8 timer_id,uint32 param)
{
    MMIIB_UpdateConnectWin(0);
    return;
}

LOCAL void MMIIB_StartWaitWinTimer()
{
	MMIIB_StopWaitWinTimer();
	ib_wait_timer_id = MMK_CreateTimerCallback(15*1000, 
												   MMIIB_HandleWaitWinTimer, 
												    (uint32)PNULL, 
												     FALSE);
	return;

}

PUBLIC void MMIIB_StopConnect()
{
    if(ib_is_connecting)
    {
        MMIIB_StopWaitWinTimer();
        ib_is_connecting = FALSE;
    }
    
    if(s_connect_win_status == 4 || s_connect_win_status == 10)
    {
        ZDT_IB_AT_Send_DisConnect();
        MMIIB_StartDisconnectWaitTimer();
    }
    return;
}

PUBLIC void MMIIB_DisConnect_CB()
{
    if(s_connect_win_status == 4 || s_connect_win_status == 10)
    {
        s_connect_win_status = 5;
        if(g_ib_is_timer_connect == FALSE)
        {
            MMIIB_TTS_PlayDisConnect();
        }
    }
    return;
}

PUBLIC void MMIIB_StartConnect()
{
    BEACONAPP_TRACE("BEACON MMIIB_StartConnect ib_is_connecting=%d,status=%d,time_connect=%d",ib_is_connecting,s_connect_win_status,g_ib_is_timer_connect);
    MMIDEFAULT_TurnOnBackLight();
    if(ib_is_connecting == FALSE)
    {
        if(s_connect_win_status == 4 || s_connect_win_status == 10)
        {
            if(g_ib_is_timer_connect)
            {
                g_ib_is_timer_connect = FALSE;
                MMIIB_TTS_PlayConnectOK();
            }
            else
            {
                ZDT_IB_AT_Send_DisConnect();
                MMIIB_StartDisconnectWaitTimer();
            }
        }
        else
        {
            char * cur_mac = MMIBEACON_NVGetMAC();
            if(strlen(cur_mac) == 0)
            {
                MMIIB_TTS_PlayNoMac();
                return;
            }
            MMIIB_StopConnectCheckTimer();
            MMIIB_StopDisconnectCheckTimer();
            MMIIB_StopDisconnectWaitTimer();
            SCI_MEMSET(s_ib_cur_connect_mac_str,0,BEACON_MAC_MAX_LEN+1);
            SCI_STRCPY(s_ib_cur_connect_mac_str,cur_mac);
            MMIIB_StartWaitWinTimer();
            ZDT_IB_PowerOn();
            ZDT_IB_AT_Send_Wakeup();
            ZDT_IB_AT_Send_Connect(s_ib_cur_connect_mac_str);
            MMIIB_TTS_PlayConnectStart();
            g_ib_is_timer_connect = FALSE;
            ib_is_connecting = TRUE;
            s_connect_win_status = 0;
        }
    }
    else
    {
        MMIIB_StopConnect();
    }
    return;
}

PUBLIC void MMIIB_Reboot_Connect()
{
    if(ib_is_connecting)
    {
        MMIIB_StopWaitWinTimer();
        ib_is_connecting = FALSE;
    }
    
    if(s_connect_win_status == 4 || s_connect_win_status == 10)
    {
        s_connect_win_status = 5;
        MMIIB_StartConnect();
    }
    return;
}

PUBLIC void MMIIB_Start_Timer_Connect()
{
    BEACONAPP_TRACE("BEACON MMIIB_Start_Timer_Connect ib_is_connecting=%d,status=%d,time_connect=%d",ib_is_connecting,s_connect_win_status,g_ib_is_timer_connect);
    MMIDEFAULT_TurnOnBackLight();
    if(ib_is_connecting == FALSE)
    {
        if(s_connect_win_status == 4 || s_connect_win_status == 10)
        {
            g_ib_need_get_57_data = 1;
        }
        else
        {
            char * cur_mac = MMIBEACON_NVGetMAC();
            if(strlen(cur_mac) == 0)
            {
                return;
            }
            MMIIB_StopConnectCheckTimer();
            MMIIB_StopDisconnectCheckTimer();
            MMIIB_StopDisconnectWaitTimer();
            SCI_MEMSET(s_ib_cur_connect_mac_str,0,BEACON_MAC_MAX_LEN+1);
            SCI_STRCPY(s_ib_cur_connect_mac_str,cur_mac);
            MMIIB_StartWaitWinTimer();
            ZDT_IB_PowerOn();
            ZDT_IB_AT_Send_Wakeup();
            ZDT_IB_AT_Send_Connect(s_ib_cur_connect_mac_str);
            g_ib_is_timer_connect = TRUE;
            ib_is_connecting = TRUE;
            s_connect_win_status = 0;
        }
    }
    return;
}

LOCAL MMI_RESULT_E  MMIIB_HandleConnectWaitWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
	MMI_RESULT_E				recode			=	MMI_RESULT_TRUE;
	GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
	GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	GUISTR_STYLE_T		text_style = {0};
	GUISTR_STATE_T		state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
	GUI_RECT_T rect = {0,0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_HEIGHT -1)};
	MMI_STRING_T cur_str_t	 = {0};
	GUI_RECT_T	  cur_rect			= {0};
	GUI_FONT_T f_big =SONG_FONT_42;
       text_style.align = ALIGN_HVMIDDLE;
       text_style.font = f_big;
       text_style.font_color = MMI_BLACK_COLOR;
	switch(msg_id) {
		case MSG_OPEN_WINDOW:
              {
                    Beacon_BackLight(TRUE);
                    MMIIB_StartConnect();
			GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);	
		}
			break;
            
           case MSG_FULL_PAINT:
                    MMIIB_ConnectWait_Win_Draw(win_id);
           	     break;
		
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			s_ib_tp_x = point.x;
			s_ib_tp_y = point.y;
			s_is_ib_tp_down = TRUE;
		}
		break;
		
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			int offset_y =	0;//up to down
			int offset_x =	0;//up to down
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if(s_is_ib_tp_down)
			{
				offset_y =	point.y - s_ib_tp_y;
				offset_x =	point.x - s_ib_tp_x;
				if(offset_x <= -(20))	
				{
					MMK_CloseWin(win_id);
				}	
				else if(offset_x >= (20))
				{
					MMK_CloseWin(win_id);
				}

			}
			s_is_ib_tp_down = FALSE;
		}
		break;
		
	case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
	case MSG_TP_PRESS_LONG:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
#endif

		case MSG_APP_WEB:
		case MSG_APP_OK:
		case MSG_APP_GREEN:
		case MSG_CTL_MIDSK:
                        if(ib_is_connecting == FALSE)
                        {
                            MMIIB_StartConnect();
                            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
                        }
                    break;
            
		case MSG_LOSE_FOCUS:
			  MMIIB_StopWaitWinTimer();
                    break;
                
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
                        MMK_CloseWin(win_id);
			break;

		case MSG_CLOSE_WINDOW:
                        MMIIB_StopWaitWinTimer();
                        Beacon_BackLight(FALSE);
			break;
			
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;

}

WINDOW_TABLE( MMIBEACON_CONNECT_WAIT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) MMIIB_HandleConnectWaitWinMsg),    
    WIN_ID(MMIBEACON_CONNECT_WAIT_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIIB_OpenConnectWait_Win(void)
{
    //BEACON_HTTP_GetInfo();
    #ifdef ZDT_LCD_NOT_SUPPORT
        MMIIB_StartConnect();
    #else
    MMK_CreateWin((uint32*)MMIBEACON_CONNECT_WAIT_WIN_TAB,PNULL);
    #endif
    return TRUE;
}

BOOLEAN MMIIB_CloseConnectWait_Win(void)
{
    if(MMK_IsOpenWin(MMIBEACON_CONNECT_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBEACON_CONNECT_WAIT_WIN_ID);
    }
    return TRUE;
}

BOOLEAN MMIIB_UpdateConnectWait_Win(void)
{
    if(MMK_IsFocusWin(MMIBEACON_CONNECT_WAIT_WIN_ID))
    {
        MMK_PostMsg(MMIBEACON_CONNECT_WAIT_WIN_ID,MSG_FULL_PAINT,PNULL,0);
        return TRUE;
    }
    return FALSE;
}
#endif

#if 1
static uint8 ib_connect_timer_id = 0;
LOCAL void MMIIB_StopConnectWinTimer()
{
    if(ib_connect_timer_id != 0)
    {
        MMK_StopTimer(ib_connect_timer_id);
        ib_connect_timer_id = 0;
    }
}

void MMIIB_HandleConnectWinTimer(uint8 timer_id,uint32 param)
{
    MMIIB_UpdateConnectWin(3);
    return;
}

LOCAL void MMIIB_StartConnectWinTimer()
{
	MMIIB_StopConnectWinTimer();
	ib_connect_timer_id = MMK_CreateTimerCallback(20*1000, 
												   MMIIB_HandleConnectWinTimer, 
												    (uint32)PNULL, 
												     FALSE);
	return;

}

void MMIIB_ConnectStatus_Win_Draw(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    MMI_STRING_T	cur_str_t	= {0};
    MMI_STRING_T	other_str_t	= {0};
    GUI_RECT_T	 cur_rect		   = {0};
    GUI_RECT_T	 rect		 = {0}; 
    GUISTR_STYLE_T		text_style		= {0};
    GUISTR_STATE_T		state = 		GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big =SONG_FONT_30;
    GUI_RECT_T	 title_rect		   = {0};
    GUI_RECT_T	 other_rect		   = {0};
    BOOLEAN hw_is_ok = beacon_hw_is_ok();
    uint16 scan_wstr[20] = {0x6B63, 0x5728, 0x626B, 0x63CF, 0x002E, 0x002E, 0x002E,0x0 }; // 6B63 5728 626B 63CF 002E 002E 002E 
    uint16 connecting_wstr[20] = {0x6B63, 0x5728, 0x8FDE, 0x63A5, 0x002E, 0x002E, 0x002E, 0x0 }; 
    uint16 timeout_wstr[20] = {0x8FDE, 0x63A5, 0x8D85, 0x65F6, 0x0 }; 
    uint16 timeout_wstr2[20] = {0x8BF7, 0x6309, 0x004F, 0x004B, 0x952E, 0x91CD, 0x8BD5, 0x0 }; 
    uint16 connect_title[20] = {0x6B63, 0x5728, 0x8FDE, 0x63A5, 0x8DF3, 0x7EF3, 0x002E, 0x002E, 0x002E, 0x0 }; // 正在连接跳绳...6B63 5728 8FDE 63A5 8DF3 7EF3 002E 002E 002E 
    uint16 disconnect_wstr[20] = {0x8DF3, 0x7EF3, 0x8FDE, 0x63A5, 0x5DF2, 0x65AD, 0x5F00, 0x0021, 0x0 }; 
    uint16 connect_ok[20] = {0x8DF3, 0x7EF3, 0x8FDE, 0x63A5, 0x6210, 0x529F, 0x0021, 0x0 }; 
    uint16 disp_wstr[50] = {0}; 
    uint16 name_wstr[100] = {0}; 
    rect.left	= 0;
    rect.top	= 0;
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    cur_rect = rect;
    title_rect = rect;
    other_rect = rect;
    
    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_BLACK_COLOR;

    if(s_connect_win_status == 1)
    {
        cur_str_t.wstr_ptr = scan_wstr;
    }
    else if(s_connect_win_status == 2)
    {
        cur_str_t.wstr_ptr = connecting_wstr;
    }
    else if(s_connect_win_status == 3)
    {
        cur_str_t.wstr_ptr = timeout_wstr;
    }
    else if(s_connect_win_status == 4)
    {
        cur_str_t.wstr_ptr = connect_ok;
    }
    else if(s_connect_win_status == 5)
    {
        cur_str_t.wstr_ptr = disconnect_wstr;
    }
    else
    {
        cur_str_t.wstr_ptr = connect_title;
    }
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    text_style.font_color = MMI_DARK_BLUE_COLOR;
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    title_rect.top = 30;
    title_rect.bottom = 65;
    GUISTR_DrawTextToLCDInRect(
                                 MMITHEME_GetDefaultLcdDev(),
                                 (const GUI_RECT_T      *)&title_rect,
                                 (const GUI_RECT_T      *)&title_rect,
                                 (const MMI_STRING_T    *)&cur_str_t,
                                 &text_style,
                                 state,
                                 GUISTR_TEXT_DIR_AUTO
                             );
    
    if(s_connect_win_status == 3)
    {
        cur_str_t.wstr_ptr = timeout_wstr2;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        text_style.font_color = MMI_BLACK_COLOR;
        if(cur_str_t.wstr_len > 0)
        {
            title_rect.top = 70;
            title_rect.bottom = 105;
            GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&title_rect,
                                            (const GUI_RECT_T      *)&title_rect,
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
                                        );
        }
    }
    MMIAPICOM_StrToWstr(s_ib_cur_connect_mac_str, disp_wstr);
    cur_str_t.wstr_ptr = disp_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    text_style.font_color = MMI_BLACK_COLOR;
    if(cur_str_t.wstr_len > 0)
    {
        title_rect.top = 110;
        title_rect.bottom = 145;
        GUISTR_DrawTextToLCDInRect( 
                                        MMITHEME_GetDefaultLcdDev(),
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                    );
    }
    
    if(s_connect_win_status == 4)
    {
        MMIAPICOM_StrToWstr(BEACON_API_GetBTName(), name_wstr);
        cur_str_t.wstr_ptr = name_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        text_style.font_color = MMI_BLACK_COLOR;
        if(cur_str_t.wstr_len > 0)
        {
            title_rect.top = 150;
            title_rect.bottom = 190;
            GUISTR_DrawTextToLCDInRect(
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&title_rect,
                                            (const GUI_RECT_T      *)&title_rect,
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
                                        );
        }
    }


    return;
}

LOCAL MMI_RESULT_E  MMIIB_HandleConnectStatusWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
	MMI_RESULT_E				recode			=	MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	GUI_RECT_T rect = {0,0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_HEIGHT -1)};
	switch(msg_id) {
		case MSG_OPEN_WINDOW:
                    connect_win_is_need_disconnect = TRUE;
                    Beacon_BackLight(TRUE);
                    MMIIB_StartConnectWinTimer();
			GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);	
			break;
            
           case MSG_FULL_PAINT:
                    MMIIB_ConnectStatus_Win_Draw(win_id);
           	     break;
		
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			s_ib_tp_x = point.x;
			s_ib_tp_y = point.y;
			s_is_ib_tp_down = TRUE;
		}
		break;
		
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			int offset_y =	0;//up to down
			int offset_x =	0;//up to down
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if(s_is_ib_tp_down)
			{
				offset_y =	point.y - s_ib_tp_y;
				offset_x =	point.x - s_ib_tp_x;
				if(offset_x <= -(20))	
				{
					MMK_CloseWin(win_id);
				}	
				else if(offset_x >= (20))
				{
					MMK_CloseWin(win_id);
				}

			}
			s_is_ib_tp_down = FALSE;
		}
		break;
		
	case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
	case MSG_TP_PRESS_LONG:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
#endif

		case MSG_APP_WEB:
		case MSG_APP_OK:
		case MSG_APP_GREEN:
		case MSG_CTL_MIDSK:
                        if(s_connect_win_status == 3)
                        {
                            MMIIB_StopConnectWinTimer();
                            MMIIB_OpenConnectWait_Win();
                            MMK_CloseWin(win_id);
                        }
                    break;
            
		case MSG_LOSE_FOCUS:
                    break;
                
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
                        MMIIB_StopConnectWinTimer();
                        MMK_CloseWin(win_id);
			break;

		case MSG_CLOSE_WINDOW:
                        if(connect_win_is_need_disconnect)
                        {
                            ZDT_IB_AT_Send_DisConnect();
                            connect_win_is_need_disconnect = FALSE;
                        }
                        MMIIB_StopConnectWinTimer();
                        Beacon_BackLight(FALSE);
			break;
			
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;

}

WINDOW_TABLE( MMIBEACON_CONNECT_STATUS_WIN_TAB ) = 
{
    WIN_FUNC((uint32) MMIIB_HandleConnectStatusWinMsg),    
    WIN_ID(MMIBEACON_CONNECT_STATUS_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIIB_OpenConnectStatus_Win(void)
{
    MMK_CreateWin((uint32*)MMIBEACON_CONNECT_STATUS_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIIB_CloseConnectStatus_Win(void)
{
    if(MMK_IsOpenWin(MMIBEACON_CONNECT_STATUS_WIN_ID))
    {
        MMK_CloseWin(MMIBEACON_CONNECT_STATUS_WIN_ID);
    }
    return TRUE;
}

BOOLEAN MMIIB_UpdateConnectStatus_Win(void)
{
    if(MMK_IsFocusWin(MMIBEACON_CONNECT_STATUS_WIN_ID))
    {
        MMK_PostMsg(MMIBEACON_CONNECT_STATUS_WIN_ID,MSG_FULL_PAINT,PNULL,0);
        return TRUE;
    }
    return FALSE;
}
#endif

#if 1
void MMIIB_ConnectResult_Win_Draw(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    MMI_STRING_T	cur_str_t	= {0};
    MMI_STRING_T	other_str_t	= {0};
    GUI_RECT_T	 cur_rect		   = {0};
    GUI_RECT_T	 rect		 = {0}; 
    GUISTR_STYLE_T		text_style		= {0};
    GUISTR_STATE_T		state = 		GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big = SONG_FONT_24;
    GUI_RECT_T	 title_rect		   = {0};
    GUI_RECT_T	 other_rect		   = {0};
    BOOLEAN hw_is_ok = beacon_hw_is_ok();
    uint16 connect_ok[20] = {0x8DF3, 0x7EF3, 0x8FDE, 0x63A5, 0x6210, 0x529F, 0x0021, 0x0 }; 
    char disp_str[50] = {0};
    uint16 disp_wstr[50] = {0};
    BEACON_DATA_T last_ts_data = g_cur_beacon_ts_data;
    // 跳绳类型:8DF3 7EF3 7C7B 578B 003A 
    uint16 type_wstr[50] = {0x8DF3, 0x7EF3, 0x7C7B, 0x578B, 0x003A, 0x0};
    // 跳绳时间:8DF3 7EF3 65F6 95F4 003A 
    uint16 cur_start_wstr[50] = {0x8DF3, 0x7EF3, 0x65F6, 0x95F4, 0x003A, 0x0};
    //跳绳数量:
    uint16 cur_num_wstr[50] = {0x8DF3, 0x7EF3, 0x6570, 0x91CF, 0x003A, 0x0};
    // 当天跳绳总时间:
    uint16 day_time_wstr[50] = {0x5F53, 0x5929, 0x8DF3, 0x7EF3, 0x603B, 0x65F6, 0x95F4, 0x003A, 0x0};
    //当天跳绳总数:
    uint16 day_sum_wstr[50] = {0x5F53, 0x5929, 0x8DF3, 0x7EF3, 0x603B, 0x6570, 0x003A, 0x0};
    uint16 min_wstr[3] = {0x5206, 0x949F , 0x0};
    uint16 sec_wstr[3] = {0x79D2, 0x0};
    rect.left	= 0;
    rect.top	= 0;
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    cur_rect = rect;
    title_rect =rect;
    other_rect=rect;
    
    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_BLACK_COLOR;
    MMIAPICOM_StrToWstr(BEACON_API_GetBTName(), disp_wstr);
    cur_str_t.wstr_ptr = disp_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    if(cur_str_t.wstr_len > 0)
    {
        title_rect.top = 2;
        title_rect.bottom = 28;
        GUISTR_DrawTextToLCDInRect( 
                                        MMITHEME_GetDefaultLcdDev(),
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                    );
    }

    cur_str_t.wstr_ptr = connect_ok;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    title_rect.top = 30;
    title_rect.bottom = 56;
    GUISTR_DrawTextToLCDInRect( 
                                 MMITHEME_GetDefaultLcdDev(),
                                 (const GUI_RECT_T      *)&title_rect,
                                 (const GUI_RECT_T      *)&title_rect,
                                 (const MMI_STRING_T    *)&cur_str_t,
                                 &text_style,
                                 state,
                                 GUISTR_TEXT_DIR_AUTO
                             );

    //跳绳类型
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    sprintf(disp_str,"%d",last_ts_data.type);
    MMIAPICOM_StrToWstr(disp_str, disp_wstr);
    MMIAPICOM_Wstrcat(type_wstr, disp_wstr);
    text_style.align = ALIGN_LVMIDDLE;
    cur_str_t.wstr_ptr = type_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    if(cur_str_t.wstr_len > 0)
    {
        title_rect.top = 58;
        title_rect.bottom = 84;
        GUISTR_DrawTextToLCDInRect( 
                                        MMITHEME_GetDefaultLcdDev(),
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                    );
    }
    //跳绳时间
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    if(last_ts_data.cur_time >= 60)
    {
        sprintf(disp_str," %d",last_ts_data.cur_time/60);
        MMIAPICOM_StrToWstr(disp_str, disp_wstr);
        MMIAPICOM_Wstrcat(cur_start_wstr, disp_wstr);
        MMIAPICOM_Wstrcat(cur_start_wstr, min_wstr);
    }
    else
    {
        sprintf(disp_str," %d",last_ts_data.cur_time);
        MMIAPICOM_StrToWstr(disp_str, disp_wstr);
        MMIAPICOM_Wstrcat(cur_start_wstr, disp_wstr);
        MMIAPICOM_Wstrcat(cur_start_wstr, sec_wstr);
    }
    cur_str_t.wstr_ptr = cur_start_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    if(cur_str_t.wstr_len > 0)
    {
        title_rect.top = 86;
        title_rect.bottom = 112;
        GUISTR_DrawTextToLCDInRect( 
                                        MMITHEME_GetDefaultLcdDev(),
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                    );
    }

    //跳绳次数
    cur_str_t.wstr_ptr = cur_num_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    text_style.align = ALIGN_HVMIDDLE;
    if(cur_str_t.wstr_len > 0)
    {
        title_rect.top = 114;
        title_rect.bottom = 140;
        GUISTR_DrawTextToLCDInRect( 
                                        MMITHEME_GetDefaultLcdDev(),
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                    );
    }
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    sprintf(disp_str,"%d",last_ts_data.cur_sum);
    MMIAPICOM_StrToWstr(disp_str, disp_wstr);
    cur_str_t.wstr_ptr = disp_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    text_style.align = ALIGN_HVMIDDLE;
    if(cur_str_t.wstr_len > 0)
    {
        title_rect.top = 142;
        title_rect.bottom = 168;
        GUISTR_DrawTextToLCDInRect( 
                                        MMITHEME_GetDefaultLcdDev(),
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                    );
    }

    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    if(last_ts_data.day_time >= 60)
    {
        sprintf(disp_str," %d",last_ts_data.day_time/60);
        MMIAPICOM_StrToWstr(disp_str, disp_wstr);
        MMIAPICOM_Wstrcat(day_time_wstr, disp_wstr);
        MMIAPICOM_Wstrcat(day_time_wstr, min_wstr);
    }
    else
    {
        sprintf(disp_str," %d",last_ts_data.day_time);
        MMIAPICOM_StrToWstr(disp_str, disp_wstr);
        MMIAPICOM_Wstrcat(day_time_wstr, disp_wstr);
        MMIAPICOM_Wstrcat(day_time_wstr, sec_wstr);
    }
    cur_str_t.wstr_ptr = day_time_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    text_style.font = f_big;
    text_style.align = ALIGN_LVMIDDLE;
    if(cur_str_t.wstr_len > 0)
    {
        title_rect.top = 170;
        title_rect.bottom = 196;
        GUISTR_DrawTextToLCDInRect( 
                                        MMITHEME_GetDefaultLcdDev(),
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                    );
    }
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    sprintf(disp_str,"%d",last_ts_data.day_sum);
    MMIAPICOM_StrToWstr(disp_str, disp_wstr);
    MMIAPICOM_Wstrcat(day_sum_wstr, disp_wstr);
    cur_str_t.wstr_ptr = day_sum_wstr;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    text_style.font = f_big;
    text_style.align = ALIGN_LVMIDDLE;
    if(cur_str_t.wstr_len > 0)
    {
        title_rect.top = 198;
        title_rect.bottom = 224;
        GUISTR_DrawTextToLCDInRect( 
                                        MMITHEME_GetDefaultLcdDev(),
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const GUI_RECT_T      *)&title_rect,
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                    );
    }
    return;
}

LOCAL MMI_RESULT_E  MMIIB_HandleConnectResultWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
	MMI_RESULT_E				recode			=	MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	GUI_RECT_T rect = {0,0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_HEIGHT -1)};
	switch(msg_id) {
		case MSG_OPEN_WINDOW:
                    Beacon_BackLight(TRUE);
                    result_win_is_need_disconnect = TRUE;
			GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);	
			break;
            
           case MSG_FULL_PAINT:
                    MMIIB_ConnectResult_Win_Draw(win_id);
           	     break;
		
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			s_ib_tp_x = point.x;
			s_ib_tp_y = point.y;
			s_is_ib_tp_down = TRUE;
		}
		break;
		
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			int offset_y =	0;//up to down
			int offset_x =	0;//up to down
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if(s_is_ib_tp_down)
			{
				offset_y =	point.y - s_ib_tp_y;
				offset_x =	point.x - s_ib_tp_x;
				if(offset_x <= -(20))	
				{
					MMK_CloseWin(win_id);
				}	
				else if(offset_x >= (20))
				{
					MMK_CloseWin(win_id);
				}

			}
			s_is_ib_tp_down = FALSE;
		}
		break;
		
	case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
	case MSG_TP_PRESS_LONG:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
#endif

		case MSG_LOSE_FOCUS:
                    break;
                
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
                        MMK_CloseWin(win_id);
			break;

		case MSG_CLOSE_WINDOW:
                        if(result_win_is_need_disconnect)
                        {
                            ZDT_IB_AT_Send_DisConnect();
                            MMIIB_StartDisconnectWaitTimer();
                            result_win_is_need_disconnect = FALSE;
                        }
                        Beacon_BackLight(FALSE);
			break;
			
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;

}

WINDOW_TABLE( MMIBEACON_CONNECT_RESULT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) MMIIB_HandleConnectResultWinMsg),    
    WIN_ID(MMIBEACON_CONNECT_RESULT_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIIB_OpenConnectResult_Win(void)
{
    MMK_CreateWin((uint32*)MMIBEACON_CONNECT_RESULT_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIIB_CloseConnectResult_Win(void)
{
    if(MMK_IsOpenWin(MMIBEACON_CONNECT_RESULT_WIN_ID))
    {
        MMK_CloseWin(MMIBEACON_CONNECT_RESULT_WIN_ID);
    }
    return TRUE;
}

BOOLEAN MMIIB_UpdateConnectResult_Win(void)
{
    if(MMK_IsFocusWin(MMIBEACON_CONNECT_RESULT_WIN_ID))
    {
        MMK_PostMsg(MMIBEACON_CONNECT_RESULT_WIN_ID,MSG_FULL_PAINT,PNULL,0);
        return TRUE;
    }
    return FALSE;
}
#endif

static uint8 ib_connect_check_timer_id = 0;
void MMIIB_StopConnectCheckTimer()
{
    if(ib_connect_check_timer_id != 0)
    {
        MMK_StopTimer(ib_connect_check_timer_id);
        ib_connect_check_timer_id = 0;
    }
}

void MMIIB_HandleConnectCheckTimer(uint8 timer_id,uint32 param)
{
    MMIBEACON_SendSigTo_APP(BEACON_APP_SIGNAL_CONN_CHECK,PNULL,0);
    return;
}

void MMIIB_StartConnectCheckTimer()
{
    MMIIB_StopConnectCheckTimer();
    ib_connect_check_timer_id = MMK_CreateTimerCallback(5*1000,
    											   MMIIB_HandleConnectCheckTimer, 
    											    (uint32)PNULL,
    											     TRUE);
    return;
}

BOOLEAN MMIIB_UpdateConnectWin(uint32 status)
{
    BOOLEAN url_res = FALSE;
    MMIIB_StopWaitWinTimer();
    MMIIB_StopConnectWinTimer();
    ib_is_connecting = FALSE;
    s_connect_win_status = status;
    BEACONAPP_TRACE("MMIIB_UpdateConnectWin status=%d",status);
#ifdef ZDT_LCD_NOT_SUPPORT
    if(status == 1)
    {
        //开始扫描
    }
    else if(status == 2)
    {
        //正在连接
    }
    else if(status == 3)
    {
        //连接超时
        if(g_ib_is_timer_connect == FALSE)
        {
            MMIIB_TTS_PlayConnectTimeOut();
        }
    }
    else if(status == 4)
    {
        //连接成功返回名称
        g_ib_last_play_num = 0;

        if(g_ib_is_timer_connect == FALSE)
        {
            MMIIB_TTS_PlayConnectOK();
        }
        MMIIB_StartConnectCheckTimer();
        MMIIB_StartDisconnectCheckTimer();
    }
    else if(status == 10)
    {
        //返回跳绳数据
        if(g_ib_is_timer_connect == FALSE)
        {
            MMIIB_TTS_PlayCurNum();
        }
    }
    else
    {
        //连接失败
        if(g_ib_is_timer_connect == FALSE)
        {
            BOOLEAN hw_is_ok = beacon_hw_is_ok();
            if(hw_is_ok)
            {
                MMIIB_TTS_PlayConnectFail();
            }
            else
            {
                MMIIB_TTS_PlayConnectHWError();
            }
        }
    }
#else
    if(status == 1)
    {
        //开始扫描
        if(FALSE == MMIIB_UpdateConnectStatus_Win())
        {
            MMIIB_OpenConnectStatus_Win();
            MMIIB_CloseConnectWait_Win();
            MMIIB_CloseConnectResult_Win();
        }
    }
    else if(status == 2)
    {
        //正在连接
        if(FALSE == MMIIB_UpdateConnectStatus_Win())
        {
            MMIIB_OpenConnectStatus_Win();
            MMIIB_CloseConnectWait_Win();
            MMIIB_CloseConnectResult_Win();
        }
    }
    else if(status == 3)
    {
        //连接超时
        if(FALSE == MMIIB_UpdateConnectStatus_Win())
        {
            MMIIB_OpenConnectStatus_Win();
            MMIIB_CloseConnectWait_Win();
            MMIIB_CloseConnectResult_Win();
        }
    }
    else if(status == 4)
    {
        //连接成功返回名称
        if(FALSE == MMIIB_UpdateConnectStatus_Win())
        {
            MMIIB_OpenConnectStatus_Win();
            MMIIB_CloseConnectWait_Win();
            MMIIB_CloseConnectResult_Win();
        }
    }
    else if(status == 10)
    {
        //返回跳绳数据
        connect_win_is_need_disconnect = FALSE;
        if(FALSE == MMIIB_UpdateConnectResult_Win())
        {
            MMIIB_OpenConnectResult_Win();
            MMIIB_CloseConnectWait_Win();
            MMIIB_CloseConnectStatus_Win();
        }
    }
    else
    {
        MMIIB_UpdateConnectWait_Win();
    }
#endif
    return TRUE;
}

BOOLEAN MMIIB_UpdateDisConnectWin(void)
{
    uint32 old_status = s_connect_win_status;
    s_connect_win_status = 5;
    if(MMK_IsOpenWin(MMIBEACON_CONNECT_RESULT_WIN_ID))
    {
        MMIIB_OpenConnectStatus_Win();
        MMK_CloseWin(MMIBEACON_CONNECT_RESULT_WIN_ID);
    }
    MMIIB_UpdateConnectStatus_Win();
    if(g_ib_is_timer_connect == FALSE)
    {
        MMIIB_TTS_PlayDisConnect();
    }
    return TRUE;
}
#endif

