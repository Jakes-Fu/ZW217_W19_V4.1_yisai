/*****************************************************************************
** File Name:      zmt_pinyin_win.c                                              *
** Author:         fys                                               *
** Date:           2024/07/08                                                *
******************************************************************************/
#include "std_header.h"
#include <stdlib.h>
#include "cjson.h"
#include "sci_api.h"
#include "dal_time.h"
#include "guibutton.h"
#include "guifont.h"
#include "guilcd.h"
#include "guitext.h"
#include "guiiconlist.h"
#include "mmi_textfun.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmi_filemgr.h"
#include "mmisrvrecord_export.h"
#include "zmt_pinyin_main.h"
#include "zmt_pinyin_id.h"
#include "zmt_pinyin_text.h"
#include "zmt_pinyin_image.h"
#ifdef LISTENING_PRATICE_SUPPORT
#include "zmt_listening_export.h"
#endif

#define pinyin_win_rect {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT}//窗口
#define pinyin_title_rect {0, 0, MMI_MAINSCREEN_WIDTH, PINYIN_LINE_HIGHT}//顶部
#define pinyin_list_rect {0, PINYIN_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT-5}//列表
#define pinyin_yinbiao_rect {10, 2.5*PINYIN_LINE_HIGHT, MMI_MAINSCREEN_WIDTH-10, 8*PINYIN_LINE_HIGHT}
#define pinyin_msg_rect {PINYIN_LINE_WIDTH, 3*PINYIN_LINE_HIGHT, MMI_MAINSCREEN_WIDTH - PINYIN_LINE_WIDTH, 7*PINYIN_LINE_HIGHT}
#define pinyin_msg_tips_left_rect {1.2*PINYIN_LINE_WIDTH, 5.5*PINYIN_LINE_HIGHT, 2.7*PINYIN_LINE_WIDTH, 7*PINYIN_LINE_HIGHT}
#define pinyin_msg_tips_right_rect {3.3*PINYIN_LINE_WIDTH, 5.5*PINYIN_LINE_HIGHT, 4.8*PINYIN_LINE_WIDTH, 7*PINYIN_LINE_HIGHT}


PINYIN_INFO_TEXT_T pinyin_info_text[PINYIN_ICON_LIST_ITEM_MAX][PINYIN_SHENG_ITEM_MAX] = {
    {"a", "o", "e", "i", "u", "ü"}, {"ai", "ei", "ao", "ou",  "ie", "iu",  "üe"}, {"an", "en", "in", "un",  "ün"},
    {"ang", "eng", "ing", "ong"}, {"b", "p", "m", "f", "d", "t", "n", "l", "g", "k", "h", "j", "q", "x", "zh", "ch", "sh", "r", "z", "c", "s"},
    {"zhi", "chi", "shi", "ri", "zi", "ci", "si", "yi", "wu", "yu", "ye", "yue", "yuan", "yin", "yun", "ying"}
};

PINYIN_INFO_NUM_T pinyin_info_num[PINYIN_ICON_LIST_ITEM_MAX] = {
    PINYIN_DAN_ITEM_MAX, PINYIN_FU_ITEM_MAX, PINYIN_QIAN_ITEM_MAX, 
    PINYIN_HOU_ITEM_MAX, PINYIN_SHENG_ITEM_MAX, PINYIN_ZHENG_ITEM_MAX
};

PINYIN_AUDIO_INFO_T * pinyin_audio_info[PINYIN_AUDIO_ITEM_MAX];
int pinyin_request_status = 0;
int pinyin_request_idx = 0;
BOOLEAN pinyin_request_now = FALSE;
PINYIN_READ_INFO_T pinyin_read_info = {0};

LOCAL MMISRV_HANDLE_T pinyin_player_handle = PNULL;
LOCAL uint8 pinyin_player_timer_id = 0;
LOCAL MMI_CTRL_ID_T pinyin_cur_select_id = ZMT_PINYIN_BUTTON_1_CTRL_ID;
LOCAL int8 pinyin_table_play_status = 0;

LOCAL void Pinyin_StopMp3Data(void);
LOCAL void Pinyin_PlayMp3Data(uint8 idx, char * text);
LOCAL void PinyinTableTipWin_UpdateButton(BOOLEAN status);
LOCAL void MMI_ClosePinyinTableTipWin(void);
LOCAL void PinyinReadWin_PreCallback(void);
LOCAL void PinyinReadWin_PlayCallback(void);
LOCAL void PinyinReadWin_NextCallback(void);
LOCAL void PinyinReadWin_UpdateButtonBgWin(BOOLEAN is_play);

LOCAL void Pinyin_InitIconlist(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_RECT_T list_rect, uint16 max_item)
{
    GUI_BORDER_T border={0};
    GUI_BG_T ctrl_bg = {0};
    GUI_FONT_ALL_T font_all = {0};
    GUIICONLIST_MARGINSPACE_INFO_T margin_space = {5,15,0,0};
    
    GUIICONLIST_SetTotalIcon(ctrl_id,max_item);
    GUIICONLIST_SetCurIconIndex(0,ctrl_id);
    GUIICONLIST_SetStyle(ctrl_id,GUIICONLIST_STYLE_ICON_UIDT);
    GUIICONLIST_SetIconWidthHeight(ctrl_id, 110, 48);
    GUIICONLIST_SetLayoutStyle(ctrl_id,GUIICONLIST_LAYOUT_V);
    border.type = GUI_BORDER_NONE;
    GUIICONLIST_SetItemBorderStyle(ctrl_id,FALSE,&border);
    border.type =GUI_BORDER_ROUNDED;
    border.width = 1;
    border.color = MMI_WHITE_COLOR;
    GUIICONLIST_SetItemBorderStyle(ctrl_id,TRUE,&border);
    GUIICONLIST_SetLoadType(ctrl_id,GUIICONLIST_LOAD_ALL);
    ctrl_bg.bg_type = GUI_BG_COLOR;
    ctrl_bg.color = PINYIN_WIN_BG_COLOR;
    GUIICONLIST_SetBg(ctrl_id, &ctrl_bg);
    GUIICONLIST_SetSlideState(ctrl_id, FALSE);
    GUIICONLIST_SetIconItemSpace(ctrl_id, margin_space);
    GUIICONLIST_SetRect(ctrl_id, &list_rect);
    font_all.color = MMI_WHITE_COLOR;
    font_all.font = DP_FONT_24;
    GUIICONLIST_SetIconListTextInfo(ctrl_id, font_all);
    MMK_SetAtvCtrl(win_id, ctrl_id);
}

LOCAL void Pinyin_InitButton(MMI_CTRL_ID_T ctrl_id, GUI_RECT_T rect, MMI_TEXT_ID_T text_id, GUI_ALIGN_E text_align, BOOLEAN visable, GUIBUTTON_CALLBACK_FUNC func)
{
    GUIBUTTON_SetRect(ctrl_id, &rect);
    GUIBUTTON_SetTextAlign(ctrl_id, text_align);
    GUIBUTTON_SetVisible(ctrl_id, visable, visable);
    if(func != NULL){
        GUIBUTTON_SetCallBackFunc(ctrl_id, func);
    }
    if(text_id != NULL){
        GUIBUTTON_SetTextId(ctrl_id, text_id);
    }
}

LOCAL void Pinyin_DrawWinTitle(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_STRING_T text_string, GUI_RECT_T title_rect, GUI_FONT_T font)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUISTR_STYLE_T text_style = {0};
    GUI_RECT_T win_rect = pinyin_win_rect;
    GUI_RECT_T text_rect = title_rect;

    GUI_FillRect(&lcd_dev_info, win_rect, PINYIN_WIN_BG_COLOR);
    GUI_FillRect(&lcd_dev_info, title_rect, PINYIN_TITLE_BG_COLOR);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = font;
    text_style.font_color = MMI_WHITE_COLOR;

    if(ctrl_id != 0)
    {
        GUILABEL_SetText(ctrl_id, &text_string, TRUE);
    }
    else
    {
        text_rect.left += 10;
        text_rect.right -= 10;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_rect,
            &text_rect,
            &text_string,
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
        );
    }
}

LOCAL void Pinyin_StopIntervalTimer(void)
{
    if(pinyin_player_timer_id != 0)
    {
        MMK_StopTimer(pinyin_player_timer_id);
        pinyin_player_timer_id = 0;
    }
}

LOCAL void Pinyin_IntervalTimerCallback(uint8 timer_id, uint32 param)
{
    if(pinyin_player_timer_id == timer_id)
    {
        uint8 idx = MMK_GetWinAddDataPtr(ZMT_PINYIN_READ_WIN_ID);
        Pinyin_StopIntervalTimer();
        if(pinyin_read_info.is_single)
        {
            pinyin_read_info.is_play = TRUE;
            if(!MMK_IsOpenWin(ZMT_PINYIN_TABLE_WIN_ID)){
                Pinyin_PlayAudioMp3(idx, pinyin_info_text[idx][pinyin_read_info.cur_read_idx].text);
            }
        }
        else if(pinyin_read_info.is_circulate)
        {
            pinyin_read_info.is_play = TRUE;
            if(!MMK_IsOpenWin(ZMT_PINYIN_TABLE_WIN_ID)){
                PinyinReadWin_NextCallback();
            }
        }
        else
        {
            pinyin_read_info.is_play = FALSE;
            Pinyin_StopMp3Data();
        }
        if(MMK_IsFocusWin(ZMT_PINYIN_READ_WIN_ID)){
            PinyinReadWin_UpdateButtonBgWin(pinyin_read_info.is_play);
        }
    }
}

LOCAL void Pinyin_CreateIntervalTimer(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_PINYIN_READ_WIN_ID);
    
    Pinyin_StopIntervalTimer();
    pinyin_player_timer_id = MMK_CreateTimerCallback(2000, Pinyin_IntervalTimerCallback, PNULL, FALSE);
    MMK_StartTimerCallback(pinyin_player_timer_id, 2000, Pinyin_IntervalTimerCallback, PNULL, FALSE);
    
    pinyin_read_info.is_play = FALSE;
    if(MMK_IsFocusWin(ZMT_PINYIN_READ_WIN_ID)){
        PinyinReadWin_UpdateButtonBgWin(pinyin_read_info.is_play);
    }
}

LOCAL BOOLEAN PinyinTableTipWin_PlayRingCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == pinyin_player_handle)
        {
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    {
                        pinyin_table_play_status = 0;
                        if(MMK_IsFocusWin(ZMT_PINYIN_TABLE_TIP_WIN_ID)){
                            PinyinTableTipWin_UpdateButton(pinyin_table_play_status);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

LOCAL BOOLEAN Pinyin_PlayMp3DataCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == pinyin_player_handle)
        {
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    {
                        if(MMK_IsOpenWin(ZMT_PINYIN_READ_WIN_ID)){
                            Pinyin_CreateIntervalTimer();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

LOCAL void Pinyin_StopMp3Data(void)
{
    if(pinyin_player_handle != 0)
    {
        MMISRVAUD_Stop(pinyin_player_handle);
        MMISRVMGR_Free(pinyin_player_handle);
        pinyin_player_handle = 0;
    }
}


LOCAL void Pinyin_PlayMp3Data(uint8 idx, char * text)
{
    MMIAUD_RING_DATA_INFO_T ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    BOOLEAN result = FALSE;
    char file_path[50] = {0};
    wchar file_name[50] = {0};
    char text_str[20] = {0};
    
    Pinyin_StopMp3Data();

    Pinyin_urlencode(text, text_str);
    sprintf(file_path, PINYIN_MP3_DATA_BASE_PATH, idx, text_str, 20);
    GUI_UTF8ToWstr(file_name, 50, file_path, strlen(file_path));
    if(!MMIFILE_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name)))
    {
        if(MMK_IsOpenWin(ZMT_PINYIN_TABLE_TIP_WIN_ID))
        {
            pinyin_table_play_status = -1;
            PinyinTableTipWin_UpdateButton(pinyin_table_play_status);
            MMK_SendMsg(ZMT_PINYIN_TABLE_TIP_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            pinyin_read_info.is_play = FALSE;
            PinyinReadWin_UpdateButtonBgWin(pinyin_read_info.is_play);
        }
        return;
    }

    if(MMK_IsOpenWin(ZMT_PINYIN_TABLE_TIP_WIN_ID))
    {
        req.is_auto_free = TRUE;
        req.notify = PinyinTableTipWin_PlayRingCallback;
    }
    else
    {
        req.is_auto_free = FALSE;
        req.notify = Pinyin_PlayMp3DataCallback;
    }
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.info.type = MMISRVAUD_TYPE_RING_FILE;
    audio_srv.info.ring_file.fmt  = MMISRVAUD_RING_FMT_MP3;
    audio_srv.info.ring_file.name = file_name;
    audio_srv.info.ring_file.name_len = MMIAPICOM_Wstrlen(file_name);
    audio_srv.volume=MMIAPISET_GetMultimVolume();

    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    pinyin_player_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(pinyin_player_handle > 0)
    {
        result = MMISRVAUD_Play(pinyin_player_handle, 0);
        if(!result)
        {
            SCI_TRACE_LOW("%s pinyin_player_handle error", __FUNCTION__);
            MMISRVMGR_Free(pinyin_player_handle);
            pinyin_player_handle = 0;
        }
        if(result == MMISRVAUD_RET_OK)
        {
            SCI_TRACE_LOW("%s pinyin_player_handle = %d", __FUNCTION__, pinyin_player_handle);
        }
    }
    else
    {
        SCI_TRACE_LOW("%s pinyin_player_handle <= 0", __FUNCTION__);
    }
}

LOCAL void Pinyin_PlayAudioMp3Data(uint8 *data,uint32 data_len)
{
    MMIAUD_RING_DATA_INFO_T ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    BOOLEAN result = FALSE;

    Pinyin_StopMp3Data();

    if(MMK_IsOpenWin(ZMT_PINYIN_TABLE_TIP_WIN_ID))
    {
        req.is_auto_free = TRUE;
        req.notify = PinyinTableTipWin_PlayRingCallback;
    }
    else
    {
        req.is_auto_free = FALSE;
        req.notify = Pinyin_PlayMp3DataCallback;
    }
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.info.type = MMISRVAUD_TYPE_RING_BUF;
    audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MP3;
    audio_srv.info.ring_buf.data = data;
    audio_srv.info.ring_buf.data_len = data_len;
    audio_srv.volume=MMIAPISET_GetMultimVolume();

    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    pinyin_player_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(pinyin_player_handle > 0)
    {
        result = MMISRVAUD_Play(pinyin_player_handle, 0);
        if(!result)
        {
            SCI_TRACE_LOW("%s pinyin_player_handle error", __FUNCTION__);
            MMISRVMGR_Free(pinyin_player_handle);
            pinyin_player_handle = 0;
        }
        if(result == MMISRVAUD_RET_OK)
        {
            SCI_TRACE_LOW("%s pinyin_player_handle = %d", __FUNCTION__, pinyin_player_handle);
        }
    }
    else
    {
        SCI_TRACE_LOW("%s pinyin_player_handle <= 0", __FUNCTION__);
    }
}

LOCAL void Pinyin_PlayAudioMp3Error(int error_id)
{
    SCI_TRACE_LOW("%s: error_id = %d", __FUNCTION__, error_id);
    pinyin_read_info.is_play = FALSE;
    Pinyin_StopMp3Data();
    Pinyin_StopIntervalTimer();
    if(pinyin_read_info.is_circulate)
    {
        pinyin_read_info.is_play = TRUE;
        PinyinReadWin_NextCallback();
    }
    PinyinReadWin_UpdateButtonBgWin(pinyin_read_info.is_play);
}

PUBLIC void Pinyin_PlayAudioMp3(uint8 idx, char * text)
{
    if(pinyin_audio_info[pinyin_read_info.cur_read_idx] == NULL){
        SCI_TRACE_LOW("%s: empty audio info!!", __FUNCTION__);
        return;
    }
    if(pinyin_audio_info[pinyin_read_info.cur_read_idx]->audio_len == 0)
    {
        if(pinyin_audio_info[pinyin_read_info.cur_read_idx]->audio_uri != NULL)
        {
            char file_path[50] = {0};
            wchar file_name[50] = {0};
            char text_str[20] = {0};
            Pinyin_urlencode(text, text_str);
            sprintf(file_path, PINYIN_MP3_DATA_BASE_PATH, idx, text_str, 20);
            GUI_UTF8ToWstr(file_name, 50, file_path, strlen(file_path));
            if(!MMIFILE_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name)))
            {
                Pinyin_RequestAudioData(pinyin_read_info.cur_read_idx, pinyin_audio_info[pinyin_read_info.cur_read_idx]->audio_uri);
            }
            else
            {
                Pinyin_PlayMp3Data(idx, text);
            }
        }
        else
        {
            SCI_TRACE_LOW("%s: empty audio uri!!", __FUNCTION__);
            Pinyin_PlayAudioMp3Error(0);
        }
    }
    else if(pinyin_audio_info[pinyin_read_info.cur_read_idx]->audio_len == -1)
    {
        Pinyin_PlayAudioMp3Error(-1);
    }
    else if(pinyin_audio_info[pinyin_read_info.cur_read_idx]->audio_len == -2)
    {
        Pinyin_PlayAudioMp3Error(-2);
    }
    else
    {
        if(pinyin_audio_info[pinyin_read_info.cur_read_idx]->audio_data != NULL){
            Pinyin_PlayAudioMp3Data(pinyin_audio_info[pinyin_read_info.cur_read_idx]->audio_data ,pinyin_audio_info[pinyin_read_info.cur_read_idx]->audio_len);
        }
    }
}

LOCAL void PinyinTableTipWin_UpdateButton(BOOLEAN status)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_BG_T bg = {GUI_BG_IMG, 0};
    GUI_RECT_T text_left_rect = pinyin_msg_tips_left_rect;

    text_left_rect.bottom -= 2;
    GUI_FillRect(&lcd_dev_info, text_left_rect, MMI_WHITE_COLOR);

    if(status){
        bg.img_id = IMG_PINYIN_TABLE_TIPS_START_IMG;
    }else{
        bg.img_id = IMG_PINYIN_TABLE_TIPS_STOP_IMG;
    }
    GUIBUTTON_SetBg(ZMT_PINYIN_TABLE_TIP_LEFT_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_PINYIN_TABLE_TIP_LEFT_CTRL_ID);
}

LOCAL void PinyinTableTipWin_LeftButtonCallback(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_PINYIN_TABLE_TIP_WIN_ID);
    if(pinyin_table_play_status == 0){
        pinyin_table_play_status = 1;
        pinyin_read_info.cur_read_idx = idx;
        Pinyin_PlayAudioMp3(pinyin_read_info.cur_icon_idx, pinyin_info_text[pinyin_read_info.cur_icon_idx][idx].text);
    }else{
        pinyin_table_play_status = 0;
        Pinyin_StopMp3Data();
    }
    PinyinTableTipWin_UpdateButton(pinyin_table_play_status);
}

LOCAL void PinyinTableTipWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T text_left_rect = pinyin_msg_tips_left_rect;
    GUI_RECT_T text_right_rect = pinyin_msg_tips_right_rect;
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMG_PINYIN_TABLE_TIPS_STOP_IMG;
    GUIBUTTON_SetRect(ZMT_PINYIN_TABLE_TIP_LEFT_CTRL_ID, &text_left_rect);
    GUIBUTTON_SetCallBackFunc(ZMT_PINYIN_TABLE_TIP_LEFT_CTRL_ID, PinyinTableTipWin_LeftButtonCallback);
    GUIBUTTON_SetBg(ZMT_PINYIN_TABLE_TIP_LEFT_CTRL_ID, &bg);

    GUIBUTTON_SetRect(ZMT_PINYIN_TABLE_TIP_RIGHT_CTRL_ID, &text_right_rect);
    GUIBUTTON_SetCallBackFunc(ZMT_PINYIN_TABLE_TIP_RIGHT_CTRL_ID, MMI_ClosePinyinTableTipWin);
}

LOCAL void PinyinTableTipWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T text_rect = pinyin_msg_rect;
    GUI_RECT_T text_left_rect = pinyin_msg_tips_left_rect;
    GUI_RECT_T text_right_rect = pinyin_msg_tips_right_rect;
    wchar text_wchar[20] = {0};
    uint8 idx = MMK_GetWinAddDataPtr(win_id);
    uint8 size = 0;

    LCD_FillRoundedRect(&lcd_dev_info, text_rect, text_rect, MMI_WHITE_COLOR);
    LCD_DrawRoundedRect(&lcd_dev_info, text_rect, text_rect, PINYIN_TITLE_BG_COLOR);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font_color = PINYIN_WIN_BG_COLOR;

    text_rect.bottom = text_left_rect.top;
    text_rect.left += 10;
    text_rect.right -= 10;
    if(pinyin_table_play_status == -1)
    {
        GUIBUTTON_SetVisible(ZMT_PINYIN_TABLE_TIP_LEFT_CTRL_ID, FALSE, FALSE);
        MMIRES_GetText(PINYIN_MP3_ERROR_TIP_TXT, win_id, &text_string);
        text_style.font = DP_FONT_22;
    }
    else
    {
        GUIBUTTON_SetVisible(ZMT_PINYIN_TABLE_TIP_LEFT_CTRL_ID, TRUE, TRUE);
        size = strlen(pinyin_info_text[pinyin_read_info.cur_icon_idx][idx].text);
        GUI_UTF8ToWstr(&text_wchar, 20, pinyin_info_text[pinyin_read_info.cur_icon_idx][idx].text, size);
        text_string.wstr_ptr = text_wchar;
        text_string.wstr_len = MMIAPICOM_Wstrlen(text_wchar);
        text_style.font = DP_FONT_28;
    }
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &text_rect,
        &text_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );
}

LOCAL MMI_RESULT_E HandlePinyinTableTipWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:  
            {
                PinyinTableTipWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {    
                PinyinTableTipWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            {
                PinyinTableTipWin_LeftButtonCallback();
            }
            break;
        case MSG_KEYDOWN_CANCEL:
            break;
        case MSG_KEYUP_RED:
        case MSG_KEYUP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            {
                pinyin_table_play_status = 0;
                Pinyin_StopMp3Data();
            }
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

WINDOW_TABLE(PINYIN_TABLE_TIP_WIN_TAB) =
{
    WIN_ID(ZMT_PINYIN_TABLE_TIP_WIN_ID),
    WIN_FUNC((uint32)HandlePinyinTableTipWinMsg),
    CREATE_BUTTON_CTRL(PNULL, ZMT_PINYIN_TABLE_TIP_LEFT_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_PINYIN_TABLE_TIPS_CLOSE_IMG, ZMT_PINYIN_TABLE_TIP_RIGHT_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL void MMI_ClosePinyinTableTipWin(void)
{
    if(MMK_IsOpenWin(ZMT_PINYIN_TABLE_TIP_WIN_ID))
    {
        MMK_CloseWin(ZMT_PINYIN_TABLE_TIP_WIN_ID);
    }
}

PUBLIC void MMI_CreatePinyinTableTipWin(uint8 idx)
{
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = pinyin_msg_rect;
    
    MMI_ClosePinyinTableTipWin();
    win_handle = MMK_CreateWin(PINYIN_TABLE_TIP_WIN_TAB, (ADD_DATA)idx);
    MMK_SetWinRect(win_handle,&rect);
}

LOCAL void PinyinTableWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    uint8 i = 0;
    uint8 idx = MMK_GetWinAddDataPtr(win_id);
    uint8 num = pinyin_info_num[idx].num;
    GUIFORM_CHILD_WIDTH_T list_ctrl_width  = {0};
    GUIFORM_CHILD_HEIGHT_T list_ctrl_height = {0};
    MMI_CTRL_ID_T form_id = ZMT_PINYIN_TABLE_FORM_CTRL_ID;
    MMI_CTRL_ID_T form_ctrl_id = ZMT_PINYIN_TABLE_FORM_CHILD_CTRL_ID;
    MMI_CTRL_ID_T list_ctrl_id = 0;
    GUI_RECT_T form_rect = pinyin_list_rect;
    GUI_BG_T form_bg = {GUI_BG_COLOR, GUI_SHAPE_ROUNDED_RECT, 0, PINYIN_WIN_BG_COLOR, FALSE};
    uint16 form_hspace = 0;
    uint16 form_vspave = 10;

    GUIFORM_SetBg(form_id, &form_bg);
    GUIFORM_SetRect(form_id, &form_rect);
    GUIFORM_PermitChildBg(form_id,FALSE);
    GUIFORM_PermitChildFont(form_id,FALSE);
    GUIFORM_PermitChildBorder(form_id, FALSE);
    GUIFORM_SetDisplayScrollBar(form_id, FALSE);
    MMK_SetActiveCtrl(form_id, FALSE);

    GUIFORM_SetBg(form_ctrl_id, &form_bg);
    GUIFORM_SetRect(form_ctrl_id, &form_rect);
    for(i = 0;i < 2;i++)
    {
        list_ctrl_id = ZMT_PINYIN_TABLE_FORM_LEFT_CTRL_ID + i;
        GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        GUILIST_SetNeedHiLightBar(list_ctrl_id,FALSE);
        GUILIST_SetMaxItem(list_ctrl_id, 11, FALSE);
        GUILIST_SetNeedPrgbarBlock(list_ctrl_id,FALSE);
        GUILIST_SetUserBg(list_ctrl_id,TRUE);
        GUILIST_SetBgColor(list_ctrl_id,PINYIN_WIN_BG_COLOR);
        GUILIST_SetTextFont(list_ctrl_id, DP_FONT_24, PINYIN_WIN_BG_COLOR);
        GUILIST_PermitBorder(list_ctrl_id, FALSE);
        GUILIST_SetSlideState(list_ctrl_id, FALSE);
        list_ctrl_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        list_ctrl_height.add_data = 1.5*PINYIN_LINE_HIGHT*(num/2+num%2);
        GUIFORM_SetChildHeight(form_ctrl_id, list_ctrl_id, &list_ctrl_height);
        list_ctrl_width.type = GUIFORM_CHILD_WIDTH_AUTO;
        GUIFORM_SetChildWidth(form_ctrl_id, list_ctrl_id, &list_ctrl_width);
    }
}

LOCAL void PinyinTableWin_DisplayTableList(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T list_ctrl_id = 0;
    MMI_STRING_T text_string = {0};
    wchar text_str[20] = {0};
    uint8 idx = MMK_GetWinAddDataPtr(win_id);
    uint8 num = pinyin_info_num[idx].num;
    uint8 list_idx = 0;
    uint8 list_num = 0;
    uint8 i,j = 0;
    for(i = 0;i < 2;i++)
    {
        GUILIST_ITEM_T item_info = {0};
        GUILIST_ITEM_DATA_T item_data= {0};
        uint8 size = 0;

        list_ctrl_id = ZMT_PINYIN_TABLE_FORM_LEFT_CTRL_ID + i;
        GUILIST_RemoveAllItems(list_ctrl_id);
        if(i == 0){
            list_num = num/2 + num%2;
        }else{
            list_num = num/2;
        }
        for(j = 0;j < list_num;j++)
        {
            item_info.item_style = GUIITEM_STYLE_PINYIN_TABLE_LIST_MS;
            item_info.item_data_ptr = &item_data;
            if(i == 0){
                list_idx = j*2;
            }else{
                list_idx = j*2 + 1;
            }

            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = IMG_PINYIN_TABLE_ITEM_BG;

            memset(text_str, 0, 20);
            size = strlen(pinyin_info_text[idx][list_idx].text);
            GUI_UTF8ToWstr(text_str, 20, pinyin_info_text[idx][list_idx].text, size);
            text_string.wstr_ptr = text_str;
            text_string.wstr_len = MMIAPICOM_Wstrlen(text_str);
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer = text_string;

            GUILIST_AppendItem(list_ctrl_id, &item_info);
        }
    }
}

LOCAL void PinyinTableWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T title_rect = pinyin_title_rect;
    MMI_TEXT_ID_T text_id[PINYIN_ICON_LIST_ITEM_MAX] = {
        PINYIN_DAN_YM_TXT, PINYIN_FU_YM_TXT, PINYIN_QIAN_YM_TXT,
        PINYIN_HOU_YM_TXT, PINYIN_SEHNG_MU_TXT, PINYIN_ZHENG_TI_TXT
    };
    uint8 idx = (uint8)MMK_GetWinAddDataPtr(win_id);

    MMIRES_GetText(text_id[idx], win_id, &text_string);
    Pinyin_DrawWinTitle(win_id, 0, text_string, title_rect, DP_FONT_24);

    PinyinTableWin_DisplayTableList(win_id);
}

LOCAL void PinyinTableWin_CTL_PENOK(MMI_WIN_ID_T win_id, DPARAM param)
{
    int cur_ctrl_idx = 0;
    uint16 cur_idx = 0;
    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T *)param)->src_id;
    cur_ctrl_idx = ctrl_id - ZMT_PINYIN_TABLE_FORM_LEFT_CTRL_ID;
    if(cur_ctrl_idx < 0){
        cur_ctrl_idx = 0;
        ctrl_id = ZMT_PINYIN_TABLE_FORM_LEFT_CTRL_ID;
    }
    cur_idx = GUILIST_GetCurItemIndex(ctrl_id);
    cur_idx *= 2;
    cur_idx += cur_ctrl_idx;
    SCI_TRACE_LOW("%s: cur_idx = %d", __FUNCTION__, cur_idx);
    MMI_CreatePinyinTableTipWin(cur_idx);
}

LOCAL MMI_RESULT_E HandlePinyinTableWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                PinyinTableWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {
                PinyinTableWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_APP_WEB:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            { 
                PinyinTableWin_CTL_PENOK(win_id, param);
            }
            break;
         case MSG_KEYDOWN_RED:
            break;
             case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            {
                MMK_CloseWin(win_id);
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

WINDOW_TABLE(MMI_PINYIN_TABLE_WIN_TAB) = {
    WIN_ID(ZMT_PINYIN_TABLE_WIN_ID),
    WIN_FUNC((uint32)HandlePinyinTableWinMsg),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, ZMT_PINYIN_TABLE_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, ZMT_PINYIN_TABLE_FORM_CHILD_CTRL_ID, ZMT_PINYIN_TABLE_FORM_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, ZMT_PINYIN_TABLE_FORM_LEFT_CTRL_ID, ZMT_PINYIN_TABLE_FORM_CHILD_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, ZMT_PINYIN_TABLE_FORM_RIGHT_CTRL_ID, ZMT_PINYIN_TABLE_FORM_CHILD_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_ClosePinyinTableWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(ZMT_PINYIN_TABLE_WIN_ID)){
        MMK_CloseWin(ZMT_PINYIN_TABLE_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreatePinyinTableWin(uint8 idx)
{
    MMI_ClosePinyinTableWin();
    MMK_CreateWin((uint32 *)MMI_PINYIN_TABLE_WIN_TAB, (ADD_DATA)idx);
}

LOCAL void PinyinReadWin_UpdateTopButton(BOOLEAN is_circulate, BOOLEAN is_single)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T single_rect = pinyin_list_rect;
    GUI_RECT_T fill_rect = {0};
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_IMG;
    
    single_rect.bottom = single_rect.top + PINYIN_LINE_HIGHT;
    single_rect.left = MMI_MAINSCREEN_WIDTH - PINYIN_LINE_WIDTH;
    single_rect.right = MMI_MAINSCREEN_WIDTH;

    fill_rect.top = single_rect.top;
    fill_rect.bottom = single_rect.bottom;
    fill_rect.left = 0;
    fill_rect.right = MMI_MAINSCREEN_WIDTH;
    GUI_FillRect(&lcd_dev_info, fill_rect, PINYIN_WIN_BG_COLOR);

    if(is_circulate){
        bg.img_id = IMG_PINYIN_CIRCULATE_PRE;
    }else{
        bg.img_id = IMG_PINYIN_CIRCULATE_DEF;
    }
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_CIRCULATE_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_PINYIN_READ_CIRCULATE_CTRL_ID);

    if(is_single){
        bg.img_id = IMG_PINYIN_SINGLE_PRE;
    }else{
        bg.img_id = IMG_PINYIN_SINGLE_DEF;
    }
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_SINGLE_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_PINYIN_READ_SINGLE_CTRL_ID);
}

LOCAL void PinyinReadWin_UpdateButtonBgWin(BOOLEAN is_play)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T button_rect = pinyin_list_rect;
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_IMG;
    
    button_rect.top = button_rect.bottom - 2*PINYIN_LINE_HIGHT + 5;
    button_rect.bottom = MMI_MAINSCREEN_HEIGHT;
    
    GUI_FillRect(&lcd_dev_info, button_rect, PINYIN_WIN_BG_COLOR);

    if(is_play){
        bg.img_id = IMG_PINYIN_PLAY;
    }else{
        bg.img_id = IMG_PINYIN_STOP;
    }
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_PLAY_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_PINYIN_READ_PLAY_CTRL_ID);
    
    bg.img_id = IMG_PINYIN_PREVIEW;
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_PRE_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_PINYIN_READ_PRE_CTRL_ID);

    bg.img_id = IMG_PINYIN_NEXT;
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_NEXT_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_PINYIN_READ_NEXT_CTRL_ID);
}

LOCAL void PinyinReadWin_TableCallback(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_PINYIN_READ_WIN_ID);
    MMI_CreatePinyinTableWin(idx);
    pinyin_read_info.is_play = FALSE;
    Pinyin_StopMp3Data();
}

LOCAL void PinyinReadWin_CirculateCallback(void)
{
    if(pinyin_read_info.is_circulate){
        pinyin_read_info.is_circulate = FALSE;
    }else{
        pinyin_read_info.is_circulate = TRUE;
        pinyin_read_info.is_single = FALSE;
    }
    PinyinReadWin_UpdateTopButton(pinyin_read_info.is_circulate, pinyin_read_info.is_single);
}

LOCAL void PinyinReadWin_SingleCallback(void)
{
    if(pinyin_read_info.is_single){
        pinyin_read_info.is_single = FALSE;
    }else{
        pinyin_read_info.is_circulate = FALSE;
        pinyin_read_info.is_single = TRUE;
    }
    PinyinReadWin_UpdateTopButton(pinyin_read_info.is_circulate, pinyin_read_info.is_single);
}

LOCAL void PinyinReadWin_PlayCallback(void)
{
    SCI_TRACE_LOW("%s: pinyin_read_info.cur_read_idx = %d", __FUNCTION__, pinyin_read_info.cur_read_idx);
    if(pinyin_read_info.is_play){
        pinyin_read_info.is_play = FALSE;
        Pinyin_StopMp3Data();
    }else{
        uint8 idx = MMK_GetWinAddDataPtr(ZMT_PINYIN_READ_WIN_ID);
        pinyin_read_info.is_play = TRUE;
        Pinyin_PlayAudioMp3(idx, pinyin_info_text[idx][pinyin_read_info.cur_read_idx].text);
    }
    Pinyin_StopIntervalTimer();
    PinyinReadWin_UpdateButtonBgWin(pinyin_read_info.is_play);
}

LOCAL void PinyinReadWin_PreCallback(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_PINYIN_READ_WIN_ID);
    Pinyin_StopMp3Data();
    Pinyin_StopIntervalTimer();
    if(pinyin_read_info.cur_read_idx > 0)
    {
        pinyin_read_info.cur_read_idx--;
    }
    else
    {
        pinyin_read_info.cur_read_idx = pinyin_info_num[idx].num - 1;
    }
    if(pinyin_read_info.is_play){
        Pinyin_PlayAudioMp3(idx, pinyin_info_text[idx][pinyin_read_info.cur_read_idx].text);
    }
    if(MMK_IsFocusWin(ZMT_PINYIN_READ_WIN_ID)){
        MMK_SendMsg(ZMT_PINYIN_READ_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

LOCAL void PinyinReadWin_NextCallback(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_PINYIN_READ_WIN_ID);
    Pinyin_StopMp3Data();
    Pinyin_StopIntervalTimer();
    if(pinyin_read_info.cur_read_idx + 1 < pinyin_info_num[idx].num)
    {
        pinyin_read_info.cur_read_idx++;
    }
    else
    {
        pinyin_read_info.cur_read_idx = 0;
    }
    if(pinyin_read_info.is_play){
        Pinyin_PlayAudioMp3(idx, pinyin_info_text[idx][pinyin_read_info.cur_read_idx].text);
    }
    if(MMK_IsFocusWin(ZMT_PINYIN_READ_WIN_ID)){
        MMK_SendMsg(ZMT_PINYIN_READ_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

LOCAL void PinyinReadWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T table_rect = pinyin_title_rect;
    GUI_RECT_T yinbiao_rect = pinyin_yinbiao_rect;
    GUI_RECT_T single_rect = pinyin_list_rect;
    GUI_RECT_T button_rect = pinyin_list_rect;
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_IMG;

    single_rect.top += 1;
    single_rect.bottom = single_rect.top + 1.5*PINYIN_LINE_HIGHT;
    single_rect.left = 0;
    single_rect.right = 2*PINYIN_LINE_WIDTH;
    Pinyin_InitButton(ZMT_PINYIN_READ_CIRCULATE_CTRL_ID, single_rect, NULL, ALIGN_HVMIDDLE, FALSE, PinyinReadWin_CirculateCallback);
    bg.img_id = IMG_PINYIN_CIRCULATE_DEF;
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_CIRCULATE_CTRL_ID, &bg);
    
    single_rect.left = MMI_MAINSCREEN_WIDTH - 2*PINYIN_LINE_WIDTH;
    single_rect.right = MMI_MAINSCREEN_WIDTH;
    Pinyin_InitButton(ZMT_PINYIN_READ_SINGLE_CTRL_ID, single_rect, NULL, ALIGN_HVMIDDLE, FALSE, PinyinReadWin_SingleCallback);
    bg.img_id = IMG_PINYIN_SINGLE_DEF;
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_SINGLE_CTRL_ID, &bg);

    button_rect.top = button_rect.bottom - 2*PINYIN_LINE_HIGHT + 5;
    button_rect.bottom = MMI_MAINSCREEN_HEIGHT;
    button_rect.left = 0;
    button_rect.right = 2*PINYIN_LINE_WIDTH;
    Pinyin_InitButton(ZMT_PINYIN_READ_PRE_CTRL_ID, button_rect, NULL, ALIGN_HVMIDDLE, FALSE, PinyinReadWin_PreCallback);
    bg.img_id = IMG_PINYIN_PREVIEW;
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_PRE_CTRL_ID, &bg);

    button_rect.left = button_rect.right;
    button_rect.right += 2*PINYIN_LINE_WIDTH;
    Pinyin_InitButton(ZMT_PINYIN_READ_PLAY_CTRL_ID, button_rect, NULL, ALIGN_HVMIDDLE, FALSE, PinyinReadWin_PlayCallback);
    bg.img_id = IMG_PINYIN_STOP;
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_PLAY_CTRL_ID, &bg);

    button_rect.left = button_rect.right;
    button_rect.right += 2*PINYIN_LINE_WIDTH;
    Pinyin_InitButton(ZMT_PINYIN_READ_NEXT_CTRL_ID, button_rect, NULL, ALIGN_HVMIDDLE, FALSE, PinyinReadWin_NextCallback);
    bg.img_id = IMG_PINYIN_NEXT;
    GUIBUTTON_SetBg(ZMT_PINYIN_READ_NEXT_CTRL_ID, &bg);

    GUIBUTTON_SetRect(ZMT_PINYIN_READ_YINBIAO_CTRL_ID, &yinbiao_rect);
    GUIBUTTON_SetVisible(ZMT_PINYIN_READ_YINBIAO_CTRL_ID, FALSE, FALSE);
    
    table_rect.left = MMI_MAINSCREEN_WIDTH - PINYIN_LINE_WIDTH-20;
	table_rect.right-=20;
    GUIBUTTON_SetRect(ZMT_PINYIN_READ_TABLE_CTRL_ID, &table_rect);
    GUIBUTTON_SetCallBackFunc(ZMT_PINYIN_READ_TABLE_CTRL_ID, PinyinReadWin_TableCallback);
    GUIBUTTON_SetVisible(ZMT_PINYIN_READ_TABLE_CTRL_ID, FALSE, FALSE);
    
    Pinyin_RequestAudioPath();
}

LOCAL void PinyinReadWin_DisplayPinyinTie(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T yinbiao_rect = pinyin_yinbiao_rect;
    GUI_FONT_ALL_T font = {DP_FONT_24, MMI_WHITE_COLOR};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    wchar text_str[20] = {0};
    uint8 size = 0;
    uint8 idx = MMK_GetWinAddDataPtr(win_id);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;

    LCD_FillRoundedRect(&lcd_dev_info, yinbiao_rect, yinbiao_rect, PINYIN_TITLE_BG_COLOR);

    size = strlen(pinyin_info_text[idx][pinyin_read_info.cur_read_idx].text);
    GUI_UTF8ToWstr(text_str, 20, pinyin_info_text[idx][pinyin_read_info.cur_read_idx].text, size);
    text_string.wstr_ptr = text_str;
    text_string.wstr_len = MMIAPICOM_Wstrlen(text_str);
    yinbiao_rect.top -= 5;
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &yinbiao_rect,
        &yinbiao_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
}

LOCAL void PinyinReadWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T win_rect = pinyin_win_rect;
    GUI_RECT_T title_rect = pinyin_title_rect;
    MMI_TEXT_ID_T text_id[PINYIN_ICON_LIST_ITEM_MAX] = {
        PINYIN_DAN_YM_TXT, PINYIN_FU_YM_TXT, PINYIN_QIAN_YM_TXT,
        PINYIN_HOU_YM_TXT, PINYIN_SEHNG_MU_TXT, PINYIN_ZHENG_TI_TXT
    };
    uint8 idx = (uint8)MMK_GetWinAddDataPtr(win_id);

    MMIRES_GetText(text_id[idx], win_id, &text_string);
    Pinyin_DrawWinTitle(win_id, 0, text_string, title_rect, DP_FONT_24);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;
    if(pinyin_request_status <= 0)
    {
        switch(pinyin_request_status)
        {
            case 0:
                MMIRES_GetText(TXT_PINYIN_INFO_LOADING, win_id, &text_string);
                break;
            case -1:
                MMIRES_GetText(TXT_PINYIN_INFO_LOAD_FAIL, win_id, &text_string);
                break;
            case -2:
                MMIRES_GetText(TXT_PINYIN_INFO_REQUESET_FAIL, win_id, &text_string);
                break;
            default:
                break;
        }
        text_style.font = DP_FONT_24;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &win_rect,
            &win_rect,
            &text_string,
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
        );
        return;
    }
    
    GUIBUTTON_SetVisible(ZMT_PINYIN_READ_CIRCULATE_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_PINYIN_READ_SINGLE_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_PINYIN_READ_NEXT_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_PINYIN_READ_PLAY_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_PINYIN_READ_PRE_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_PINYIN_READ_YINBIAO_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_PINYIN_READ_TABLE_CTRL_ID, TRUE, TRUE);
    
    PinyinReadWin_DisplayPinyinTie(win_id);
}

LOCAL void PinyinReadWin_CLOSE_WINDOW(void)
{
    pinyin_read_info.cur_read_idx = 0;
    Pinyin_StopMp3Data();
    Pinyin_StopIntervalTimer();
    Pinyin_ReleaseAudioPath();
    pinyin_request_status = 0;
    pinyin_request_now = FALSE;
}

LOCAL MMI_RESULT_E HandlePinyinReadWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                PinyinReadWin_OPEN_WINDOW(win_id);
          //      WATCHCOM_Backlight(TRUE);
            }
            break;
        case MSG_FULL_PAINT:
            {
                PinyinReadWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            { 
                PinyinReadWin_PlayCallback();
            }
            break;
        case MSG_KEYDOWN_BACKWARD:
        case MSG_KEYDOWN_FORWARD:
            {
                PinyinReadWin_SingleCallback();
            }
            break;
        case MSG_APP_LEFT:
            {
                PinyinReadWin_PreCallback();
            }
            break;
        case MSG_APP_RIGHT:
            {
                PinyinReadWin_NextCallback();
            }
            break;
       case MSG_KEYDOWN_RED:
            break;
             case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            {
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_CLOSE_WINDOW:
            {
                PinyinReadWin_CLOSE_WINDOW();
               // WATCHCOM_Backlight(FALSE);
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(MMI_PINYIN_READ_WIN_TAB) = {
    WIN_ID(ZMT_PINYIN_READ_WIN_ID),
    WIN_FUNC((uint32)HandlePinyinReadWinMsg),
    CREATE_BUTTON_CTRL(IMG_PINYIN_TABLE_BG, ZMT_PINYIN_READ_TABLE_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_PINYIN_READ_CIRCULATE_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_PINYIN_READ_SINGLE_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_PINYIN_TIE_BG, ZMT_PINYIN_READ_YINBIAO_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_PINYIN_READ_NEXT_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_PINYIN_READ_PLAY_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_PINYIN_READ_PRE_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_ClosePinyinReadWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(ZMT_PINYIN_READ_WIN_ID)){
        MMK_CloseWin(ZMT_PINYIN_READ_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreatePinyinReadWin(uint8 idx)
{
    MMI_ClosePinyinReadWin();
    MMK_CreateWin((uint32 *)MMI_PINYIN_READ_WIN_TAB, (ADD_DATA)idx);
}

LOCAL void PinyinMainWin_DisplayButton(MMI_WIN_ID_T win_id)
{
    uint8 i = 0;
    uint8 j = 0;
    uint8 k = 0;
    MMI_CTRL_ID_T ctrl_id = ZMT_PINYIN_BUTTON_1_CTRL_ID;
    GUI_FONT_ALL_T font = {DP_FONT_24, PINYIN_WIN_BG_COLOR};
    GUI_BORDER_T last_border = {1, MMI_WHITE_COLOR, GUI_BORDER_NONE};
    GUI_BORDER_T border = {1, MMI_WHITE_COLOR, GUI_BORDER_ROUNDED};
    GUI_RECT_T button_rect = pinyin_list_rect;
    MMI_TEXT_ID_T text_id[PINYIN_ICON_LIST_ITEM_MAX] = {
        PINYIN_DAN_YM_TXT, PINYIN_FU_YM_TXT, PINYIN_QIAN_YM_TXT,
        PINYIN_HOU_YM_TXT, PINYIN_SEHNG_MU_TXT, PINYIN_ZHENG_TI_TXT
    };
    button_rect.top += 10;
    button_rect.bottom = button_rect.top + 1.5*PINYIN_LINE_HIGHT + 2;
    for(i = 0;i < 3;i++)
    {
        button_rect.left = 5;
        button_rect.right = MMI_MAINSCREEN_WIDTH / 2 - 3;
        for(j = 0;j < 2;j++)
        {
            ctrl_id = ZMT_PINYIN_BUTTON_1_CTRL_ID + k;
            Pinyin_InitButton(ctrl_id, button_rect, text_id[k], ALIGN_HVMIDDLE, TRUE, NULL);
            if(k == 5){
                font.font = DP_FONT_18;
            }else{
                font.font = DP_FONT_24;
            }
            GUIBUTTON_SetFont(ctrl_id, &font);
            MMK_SetActiveCtrl(ctrl_id, FALSE);
            button_rect.left = MMI_MAINSCREEN_WIDTH / 2 + 3;
            button_rect.right = MMI_MAINSCREEN_WIDTH - 5;
            k++;
        }
        button_rect.top = button_rect.bottom + 10;
        button_rect.bottom = button_rect.top + 1.5*PINYIN_LINE_HIGHT;
    }
    GUIBUTTON_SetBorder(pinyin_cur_select_id, &border, FALSE);
}

LOCAL void PinyinMainWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    memset(&pinyin_read_info, 0, sizeof(PINYIN_READ_INFO_T));
    pinyin_cur_select_id = ZMT_PINYIN_BUTTON_1_CTRL_ID;

    PinyinMainWin_DisplayButton(win_id);
}

LOCAL void PinyinMainWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_BORDER_T border = {1, MMI_WHITE_COLOR, GUI_BORDER_ROUNDED};
    GUI_RECT_T title_rect = pinyin_title_rect;
    uint8 i = 0;

    MMIRES_GetText(PINYIN_CLASS_TXT, win_id, &text_string);
    Pinyin_DrawWinTitle(win_id, 0, text_string, title_rect, DP_FONT_22);

    for(i = 0;i < PINYIN_ICON_LIST_ITEM_MAX;i++)
    {
        MMI_CTRL_ID_T ctrl_id = ZMT_PINYIN_BUTTON_1_CTRL_ID + i;
        GUIBUTTON_SetVisible(ctrl_id, TRUE, TRUE);
    }
    GUIBUTTON_SetBorder(pinyin_cur_select_id, &border, FALSE);
}

LOCAL void PinyinMainWin_UpdateSelectButton(MMI_CTRL_ID_T ctrl_id)
{
    GUI_BORDER_T last_border = {1, MMI_WHITE_COLOR, GUI_BORDER_NONE};
    GUI_BORDER_T border = {1, MMI_WHITE_COLOR, GUI_BORDER_ROUNDED};
    
    GUIBUTTON_SetBorder(pinyin_cur_select_id, &last_border, TRUE);
    GUIBUTTON_Update(pinyin_cur_select_id);
    GUIBUTTON_SetBorder(ctrl_id, &border, TRUE);
    GUIBUTTON_Update(ctrl_id);
}

LOCAL void PinyinMainWin_APP_LEFT(MMI_WIN_ID_T win_id)
{
    int8 idx = 0;
    if(pinyin_cur_select_id == ZMT_PINYIN_BUTTON_1_CTRL_ID){
        idx = ZMT_PINYIN_BUTTON_6_CTRL_ID - ZMT_PINYIN_BUTTON_1_CTRL_ID;
    }else{
        idx = -1;
    }
    PinyinMainWin_UpdateSelectButton(pinyin_cur_select_id+idx);
    pinyin_cur_select_id += idx;
    PinyinMainWin_FULL_PAINT(win_id);
}

LOCAL void PinyinMainWin_APP_RIGHT(MMI_WIN_ID_T win_id)
{
    int8 idx = 0;
    if(pinyin_cur_select_id == ZMT_PINYIN_BUTTON_6_CTRL_ID){
        idx =  ZMT_PINYIN_BUTTON_1_CTRL_ID - ZMT_PINYIN_BUTTON_6_CTRL_ID;
    }else{
        idx = 1;
    }
    PinyinMainWin_UpdateSelectButton(pinyin_cur_select_id+idx);
    pinyin_cur_select_id += idx;
    PinyinMainWin_FULL_PAINT(win_id);
}

LOCAL void PinyinMainWin_APP_UP(MMI_WIN_ID_T win_id)
{
    int8 idx = 0;
    if(pinyin_cur_select_id == ZMT_PINYIN_BUTTON_1_CTRL_ID ||
        pinyin_cur_select_id == ZMT_PINYIN_BUTTON_2_CTRL_ID ){
        idx = 4;
    }else{
        idx = -2;
    }
    PinyinMainWin_UpdateSelectButton(pinyin_cur_select_id+idx);
    pinyin_cur_select_id += idx;
    PinyinMainWin_FULL_PAINT(win_id);
}

LOCAL void PinyinMainWin_APP_DOWN(MMI_WIN_ID_T win_id)
{
    int8 idx = 0;
    if(pinyin_cur_select_id == ZMT_PINYIN_BUTTON_5_CTRL_ID ||
        pinyin_cur_select_id == ZMT_PINYIN_BUTTON_6_CTRL_ID ){
        idx = -4;
    }else{
        idx = 2;
    }
    PinyinMainWin_UpdateSelectButton(pinyin_cur_select_id+idx);
    pinyin_cur_select_id += idx;
    PinyinMainWin_FULL_PAINT(win_id);
}

LOCAL void PinyinMainWin_APP_OK(MMI_WIN_ID_T win_id)
{
    int8 cur_idx = pinyin_cur_select_id - ZMT_PINYIN_BUTTON_1_CTRL_ID;
    if(cur_idx < 0){
        cur_idx = 0;
    }
    SCI_TRACE_LOW("%s: cur_idx = %d", __FUNCTION__, cur_idx);
    pinyin_read_info.cur_icon_idx = cur_idx;
    MMI_CreatePinyinReadWin(cur_idx);
}

LOCAL void PinyinMainWin_CTL_PENOK(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T *)param)->src_id;
    int8 cur_idx = ctrl_id - ZMT_PINYIN_BUTTON_1_CTRL_ID;
    if(cur_idx < 0){
        cur_idx = 0;
        ctrl_id = ZMT_PINYIN_BUTTON_1_CTRL_ID;
    }
    PinyinMainWin_UpdateSelectButton(ctrl_id);
    pinyin_cur_select_id = ctrl_id;
    SCI_TRACE_LOW("%s: cur_idx = %d", __FUNCTION__, cur_idx);
    pinyin_read_info.cur_icon_idx = cur_idx;
    MMI_CreatePinyinReadWin(cur_idx);
}

LOCAL MMI_RESULT_E HandlePinyinMainWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                PinyinMainWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {
                PinyinMainWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_APP_LEFT:
            {
                PinyinMainWin_APP_LEFT(win_id);
            }
            break;
        case MSG_APP_RIGHT:
            {
                PinyinMainWin_APP_RIGHT(win_id);
            }
            break;
        case MSG_APP_UP:
            {
                PinyinMainWin_APP_UP(win_id);
            }
            break;
        case MSG_APP_DOWN:
            {
                PinyinMainWin_APP_DOWN(win_id);
            }
            break;
        case MSG_APP_WEB:
        case MSG_APP_OK:
            {
                PinyinMainWin_APP_OK(win_id);
            }
            break;
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            { 
                PinyinMainWin_CTL_PENOK(win_id, param);
            }
            break;
        case MSG_KEYDOWN_CANCEL:
            break;
        case MSG_KEYUP_RED:
        case MSG_KEYUP_CANCEL:
            {
                MMK_CloseWin(win_id);
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

WINDOW_TABLE(MMI_PINYIN_MAIN_WIN_TAB) = {
    WIN_ID(ZMT_PINYIN_MAIN_WIN_ID),
    WIN_FUNC((uint32)HandlePinyinMainWinMsg),
    CREATE_BUTTON_CTRL(IMG_PINYIN_ICON_BG, ZMT_PINYIN_BUTTON_1_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_PINYIN_ICON_BG, ZMT_PINYIN_BUTTON_2_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_PINYIN_ICON_BG, ZMT_PINYIN_BUTTON_3_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_PINYIN_ICON_BG, ZMT_PINYIN_BUTTON_4_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_PINYIN_ICON_BG, ZMT_PINYIN_BUTTON_5_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_PINYIN_ICON_BG, ZMT_PINYIN_BUTTON_6_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_ClosePinyinMainWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(ZMT_PINYIN_MAIN_WIN_ID)){
        MMK_CloseWin(ZMT_PINYIN_MAIN_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreatePinyinMainWin(void)
{
#ifdef LISTENING_PRATICE_SUPPORT
    if(!zmt_tfcard_exist())
    {
        MMI_CreateListeningTipWin(PALYER_PLAY_NO_TFCARD_TIP);
    }
    else
#endif
    {
        MMI_ClosePinyinMainWin();
        MMK_CreateWin((uint32 *)MMI_PINYIN_MAIN_WIN_TAB, PNULL);
    }
}

