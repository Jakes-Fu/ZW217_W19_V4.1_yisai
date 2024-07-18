/*****************************************************************************
** File Name:      zmt_yinbiao_win.c                                              *
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
#include "mmiacc_text.h"
#include "mmicc_export.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmi_filemgr.h"
#include "mmisrvrecord_export.h"
#include "zmt_yinbiao_main.h"
#include "zmt_yinbiao_id.h"
#include "zmt_yinbiao_text.h"
#include "zmt_yinbiao_image.h"
#include "mmi_default.h"
#include "mmiacc_id.h"
#include "mmiacc_position.h"
#include "mmiset_export.h"
#include "mmiacc_nv.h"
#include "guistring.h"
#include "mmi_theme.h"
#ifdef LISTENING_PRATICE_SUPPORT
#include "zmt_listening_export.h"
#endif


#define yinbiao_win_rect {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT}//窗口
#define yinbiao_title_rect {0, 0, MMI_MAINSCREEN_WIDTH, YINBIAO_LINE_HIGHT}//顶部
#define yinbiao_list_rect {0, YINBIAO_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT-5}//列表
#define yinbiao_yinbiao_rect {10, 2.5*YINBIAO_LINE_HIGHT, MMI_MAINSCREEN_WIDTH-10, 8*YINBIAO_LINE_HIGHT}
#define yinbiao_msg_rect {YINBIAO_LINE_WIDTH, 3*YINBIAO_LINE_HIGHT, MMI_MAINSCREEN_WIDTH - YINBIAO_LINE_WIDTH, 7*YINBIAO_LINE_HIGHT}
#define yinbiao_msg_tips_left_rect {1.2*YINBIAO_LINE_WIDTH, 5.5*YINBIAO_LINE_HIGHT, 2.7*YINBIAO_LINE_WIDTH, 7*YINBIAO_LINE_HIGHT}
#define yinbiao_msg_tips_right_rect {3.3*YINBIAO_LINE_WIDTH, 5.5*YINBIAO_LINE_HIGHT, 4.8*YINBIAO_LINE_WIDTH, 7*YINBIAO_LINE_HIGHT}

YINBIAO_INFO_TEXT_T yinbiao_info_text[YINBIAO_ICON_LIST_ITEM_MAX][YINBIAO_DAN_ITEM_MAX] = {
	{"ɒ", "æ", "e", "ə", "ɪ", "u","ʌ","aː","ɔː","ɜː","iː","uː"},{"eɪ","aɪ","ɔɪ","ɪə","eə","ʊə","əʊ","aʊ"}, 
	{"m","n","ŋ"} ,{"p","b","t","d","k","g"},{"dr","dz","dʒ","tr","ts","tʃ"},
	{"f","v","s","z","ʃ","ʒ","θ","ð","h","r"},{"j","w"},
};

YINBIAO_INFO_NUM_T yinbiao_info_num[YINBIAO_ICON_LIST_ITEM_MAX] = {
    YINBIAO_DAN_ITEM_MAX, YINBIAO_FU_ITEM_MAX, YINBIAO_QIAN_ITEM_MAX, 
    YINBIAO_HOU_ITEM_MAX, YINBIAO_SHENG_ITEM_MAX, YINBIAO_ZHENG_ITEM_MAX,
    YINBIAO_HALF_ITEM_MAX
};

YINBIAO_AUDIO_INFO_T * yinbiao_audio_info[YINBIAO_AUDIO_ITEM_MAX];
int yinbiao_request_status = 0;
int yinbiao_request_idx = 0;
BOOLEAN yinbiao_request_now = FALSE;
YINBIAO_READ_INFO_T yinbiao_read_info = {0};

LOCAL MMISRV_HANDLE_T yinbiao_player_handle = PNULL;
LOCAL uint8 yinbiao_player_timer_id = 0;
LOCAL MMI_CTRL_ID_T yinbiao_cur_select_id = ZMT_YINBIAO_BUTTON_1_CTRL_ID;
LOCAL int8 yinbiao_table_play_status = 0;

LOCAL void Yinbiao_StopMp3Data(void);
LOCAL void Yinbiao_PlayMp3Data(uint8 idx, char * text);
LOCAL void YinbiaoTableTipWin_UpdateButton(BOOLEAN status);
LOCAL void MMI_CloseYinbiaoTableTipWin(void);
LOCAL void YinbiaoReadWin_PreCallback(void);
LOCAL void YinbiaoReadWin_PlayCallback(void);
LOCAL void YinbiaoReadWin_NextCallback(void);
LOCAL void YinbiaoReadWin_UpdateButtonBgWin(BOOLEAN is_play);

LOCAL void Yinbiao_InitIconlist(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_RECT_T list_rect, uint16 max_item)
{
    GUI_BORDER_T border={0};
    GUI_BG_T ctrl_bg = {0};
    GUI_FONT_ALL_T font_all = {0};
    GUIICONLIST_MARGINSPACE_INFO_T margin_space = {10,15,0,2};
    
    GUIICONLIST_SetTotalIcon(ctrl_id,max_item);
    GUIICONLIST_SetCurIconIndex(0,ctrl_id);
    GUIICONLIST_SetStyle(ctrl_id,GUIICONLIST_STYLE_ICON);
    GUIICONLIST_SetIconWidthHeight(ctrl_id, 100, 48);
    GUIICONLIST_SetLayoutStyle(ctrl_id,GUIICONLIST_LAYOUT_V);
    border.type = GUI_BORDER_NONE;
    GUIICONLIST_SetItemBorderStyle(ctrl_id,FALSE,&border);
    border.type =GUI_BORDER_ROUNDED;
    border.width = 1;
    border.color = MMI_WHITE_COLOR;
    GUIICONLIST_SetItemBorderStyle(ctrl_id,TRUE,&border);
    GUIICONLIST_SetLoadType(ctrl_id,GUIICONLIST_LOAD_ALL);
    ctrl_bg.bg_type = GUI_BG_COLOR;
    ctrl_bg.color = YINBIAO_WIN_BG_COLOR;
    GUIICONLIST_SetBg(ctrl_id, &ctrl_bg);
    GUIICONLIST_SetSlideState(ctrl_id, FALSE);
    GUIICONLIST_SetIconItemSpace(ctrl_id, margin_space);
    GUIICONLIST_SetRect(ctrl_id, &list_rect);
    font_all.color = MMI_WHITE_COLOR;
    font_all.font = DP_FONT_22;
    GUIICONLIST_SetIconListTextInfo(ctrl_id, font_all);
    MMK_SetAtvCtrl(win_id, ctrl_id);
}

LOCAL void Yinbiao_InitButton(MMI_CTRL_ID_T ctrl_id, GUI_RECT_T rect, MMI_TEXT_ID_T text_id, GUI_ALIGN_E text_align, BOOLEAN visable, GUIBUTTON_CALLBACK_FUNC func)
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

LOCAL void Yinbiao_DrawWinTitle(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_STRING_T text_string, GUI_RECT_T title_rect, GUI_FONT_T font)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
 GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    GUI_RECT_T win_rect = yinbiao_win_rect;
    GUI_RECT_T text_rect = title_rect;
	MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    GUI_FillRect(&lcd_dev_info, win_rect, YINBIAO_WIN_BG_COLOR);
    GUI_FillRect(&lcd_dev_info, title_rect, YINBIAO_TITLE_BG_COLOR);

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

LOCAL void Yinbiao_StopIntervalTimer(void)
{
    if(yinbiao_player_timer_id != 0)
    {
        MMK_StopTimer(yinbiao_player_timer_id);
        yinbiao_player_timer_id = 0;
    }
}

LOCAL void Yinbiao_IntervalTimerCallback(uint8 timer_id, uint32 param)
{
    if(yinbiao_player_timer_id == timer_id)
    {
        uint8 idx = MMK_GetWinAddDataPtr(ZMT_YINBIAO_READ_WIN_ID);
        Yinbiao_StopIntervalTimer();
        if(yinbiao_read_info.is_single)
        {
            yinbiao_read_info.is_play = TRUE;
            if(!MMK_IsOpenWin(ZMT_YINBIAO_TABLE_WIN_ID)){
                Yinbiao_PlayAudioMp3(idx, yinbiao_info_text[idx][yinbiao_read_info.cur_read_idx].text);
            }
        }
        else if(yinbiao_read_info.is_circulate)
        {
            yinbiao_read_info.is_play = TRUE;
            if(!MMK_IsOpenWin(ZMT_YINBIAO_TABLE_WIN_ID)){
                YinbiaoReadWin_NextCallback();
            }
        }
        else
        {
            yinbiao_read_info.is_play = FALSE;
            Yinbiao_StopMp3Data();
        }
        if(MMK_IsFocusWin(ZMT_YINBIAO_READ_WIN_ID)){
            YinbiaoReadWin_UpdateButtonBgWin(yinbiao_read_info.is_play);
        }
    }
}

LOCAL void Yinbiao_CreateIntervalTimer(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_YINBIAO_READ_WIN_ID);
    
    Yinbiao_StopIntervalTimer();
    yinbiao_player_timer_id = MMK_CreateTimerCallback(2000, Yinbiao_IntervalTimerCallback, PNULL, FALSE);
    MMK_StartTimerCallback(yinbiao_player_timer_id, 2000, Yinbiao_IntervalTimerCallback, PNULL, FALSE);
    
    yinbiao_read_info.is_play = FALSE;
    if(MMK_IsFocusWin(ZMT_YINBIAO_READ_WIN_ID)){
        YinbiaoReadWin_UpdateButtonBgWin(yinbiao_read_info.is_play);
    }
}

LOCAL BOOLEAN YinbiaoTableTipWin_PlayRingCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == yinbiao_player_handle)
        {
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    {
                        yinbiao_table_play_status = 0;
                        if(MMK_IsFocusWin(ZMT_YINBIAO_TABLE_TIP_WIN_ID)){
                            YinbiaoTableTipWin_UpdateButton(yinbiao_table_play_status);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

LOCAL BOOLEAN Yinbiao_PlayMp3DataCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == yinbiao_player_handle)
        {
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    {
                        if(MMK_IsOpenWin(ZMT_YINBIAO_READ_WIN_ID)){
                            Yinbiao_CreateIntervalTimer();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

LOCAL void Yinbiao_StopMp3Data(void)
{
    if(yinbiao_player_handle != 0)
    {
        MMISRVAUD_Stop(yinbiao_player_handle);
        MMISRVMGR_Free(yinbiao_player_handle);
        yinbiao_player_handle = 0;
    }
}

LOCAL void Yinbiao_PlayMp3Data(uint8 idx, char * text)
{
    MMIAUD_RING_DATA_INFO_T ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    BOOLEAN result = FALSE;
    char file_path[50] = {0};
    wchar file_name[50] = {0};
    char text_str[20] = {0};
    
    Yinbiao_StopMp3Data();

    Yinbiao_urlencode(text, text_str, 20);
    sprintf(file_path, YINBIAO_MP3_DATA_BASE_PATH, idx, text_str);
    GUI_UTF8ToWstr(file_name, 50, file_path,strlen(file_path));
    if(!MMIFILE_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name))){
        yinbiao_read_info.is_play = FALSE;
        if(MMK_IsFocusWin(ZMT_YINBIAO_READ_WIN_ID)){
            YinbiaoReadWin_UpdateButtonBgWin(yinbiao_read_info.is_play);
        }
        return;
    }
    
    if(MMK_IsOpenWin(ZMT_YINBIAO_TABLE_TIP_WIN_ID))
    {
        req.is_auto_free = TRUE;
        req.notify = YinbiaoTableTipWin_PlayRingCallback;
    }
    else
    {
        req.is_auto_free = FALSE;
        req.notify = Yinbiao_PlayMp3DataCallback;
    }
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.info.type = MMISRVAUD_TYPE_RING_FILE;
    audio_srv.info.ring_file.fmt  = MMISRVAUD_RING_FMT_MP3;
    audio_srv.info.ring_file.name = file_name;
    audio_srv.info.ring_file.name_len = MMIAPICOM_Wstrlen(file_name);
    audio_srv.volume=MMIAPISET_GetMultimVolume();

    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    yinbiao_player_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(yinbiao_player_handle > 0)
    {
        result = MMISRVAUD_Play(yinbiao_player_handle, 0);
        if(!result)
        {
            SCI_TRACE_LOW("%s yinbiao_player_handle error", __FUNCTION__);
            MMISRVMGR_Free(yinbiao_player_handle);
            yinbiao_player_handle = 0;
        }
        if(result == MMISRVAUD_RET_OK)
        {
            SCI_TRACE_LOW("%s yinbiao_player_handle = %d", __FUNCTION__, yinbiao_player_handle);
        }
    }
    else
    {
        SCI_TRACE_LOW("%s yinbiao_player_handle <= 0", __FUNCTION__);
    }
}

LOCAL void Yinbiao_PlayAudioMp3Data(uint8 *data,uint32 data_len)
{
    MMIAUD_RING_DATA_INFO_T ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    BOOLEAN result = FALSE;

    Yinbiao_StopMp3Data();

    if(MMK_IsOpenWin(ZMT_YINBIAO_TABLE_TIP_WIN_ID))
    {
        req.is_auto_free = TRUE;
        req.notify = YinbiaoTableTipWin_PlayRingCallback;
    }
    else
    {
        req.is_auto_free = FALSE;
        req.notify = Yinbiao_PlayMp3DataCallback;
    }
    req.pri = MMISRVAUD_PRI_NORMAL;
    
    audio_srv.info.type = MMISRVAUD_TYPE_RING_BUF;
    audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MP3;
    audio_srv.info.ring_buf.data = data;
    audio_srv.info.ring_buf.data_len = data_len;
    audio_srv.volume=MMIAPISET_GetMultimVolume();

    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    yinbiao_player_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(yinbiao_player_handle > 0)
    {
        result = MMISRVAUD_Play(yinbiao_player_handle, 0);
        if(!result)
        {
            SCI_TRACE_LOW("%s yinbiao_player_handle error", __FUNCTION__);
            MMISRVMGR_Free(yinbiao_player_handle);
            yinbiao_player_handle = 0;
        }
        if(result == MMISRVAUD_RET_OK)
        {
            SCI_TRACE_LOW("%s yinbiao_player_handle = %d", __FUNCTION__, yinbiao_player_handle);
        }
    }
    else
    {
        SCI_TRACE_LOW("%s yinbiao_player_handle <= 0", __FUNCTION__);
    }
}

LOCAL void Yinbiao_PlayAudioMp3Error(int error_id)
{
    SCI_TRACE_LOW("%s: error_id = %d", __FUNCTION__, error_id);
    yinbiao_read_info.is_play = FALSE;
    Yinbiao_StopMp3Data();
    Yinbiao_StopIntervalTimer();
    if(yinbiao_read_info.is_circulate)
    {
        yinbiao_read_info.is_play = TRUE;
        YinbiaoReadWin_NextCallback();
    }
    YinbiaoReadWin_UpdateButtonBgWin(yinbiao_read_info.is_play);
}

PUBLIC void Yinbiao_PlayAudioMp3(uint8 idx, char * text)
{
    if(yinbiao_audio_info[yinbiao_read_info.cur_read_idx] == NULL){
        SCI_TRACE_LOW("%s: empty audio info!!", __FUNCTION__);
        return;
    }
    if(yinbiao_audio_info[yinbiao_read_info.cur_read_idx]->audio_len == 0)
    {
        if(yinbiao_audio_info[yinbiao_read_info.cur_read_idx]->audio_uri != NULL)
        {
            char file_path[50] = {0};
            wchar file_name[50] = {0};
            char text_str[20] = {0};
            Yinbiao_urlencode(text, text_str, 20);
            sprintf(file_path, YINBIAO_MP3_DATA_BASE_PATH, idx, text_str);
            GUI_UTF8ToWstr(file_name, 50, file_path, strlen(file_path));
            if(!MMIFILE_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name)))
            {
                Yinbiao_RequestAudioData(yinbiao_read_info.cur_read_idx, yinbiao_audio_info[yinbiao_read_info.cur_read_idx]->audio_uri);
            }
            else
            {
                Yinbiao_PlayMp3Data(idx, text);
            }
        }
        else
        {
            SCI_TRACE_LOW("%s: empty audio uri!!", __FUNCTION__);
            Yinbiao_PlayAudioMp3Error(0);
        }
    }
    else if(yinbiao_audio_info[yinbiao_read_info.cur_read_idx]->audio_len == -1)
    {
        Yinbiao_PlayAudioMp3Error(-1);
    }
    else if(yinbiao_audio_info[yinbiao_read_info.cur_read_idx]->audio_len == -2)
    {
        Yinbiao_PlayAudioMp3Error(-2);
    }
    else
    {
        if(yinbiao_audio_info[yinbiao_read_info.cur_read_idx]->audio_data != NULL){
            Yinbiao_PlayAudioMp3Data(yinbiao_audio_info[yinbiao_read_info.cur_read_idx]->audio_data ,yinbiao_audio_info[yinbiao_read_info.cur_read_idx]->audio_len);
        }
    }
}
LOCAL void YinbiaoTableTipWin_UpdateButton(BOOLEAN status)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_BG_T bg = {GUI_BG_IMG, 0};
    GUI_RECT_T text_left_rect = yinbiao_msg_tips_left_rect;

    text_left_rect.bottom -= 2;
    GUI_FillRect(&lcd_dev_info, text_left_rect, MMI_WHITE_COLOR);

    if(status){
        bg.img_id = IMG_YINBIAO_TABLE_TIPS_START_IMG;
    }else{
        bg.img_id = IMG_YINBIAO_TABLE_TIPS_STOP_IMG;
    }
    GUIBUTTON_SetBg(ZMT_YINBIAO_TABLE_TIP_LEFT_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_YINBIAO_TABLE_TIP_LEFT_CTRL_ID);
}

LOCAL void YinbiaoTableTipWin_LeftButtonCallback(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_YINBIAO_TABLE_TIP_WIN_ID);
    if(yinbiao_table_play_status == 0){
        yinbiao_table_play_status = 1;
        yinbiao_read_info.cur_read_idx = idx;
        Yinbiao_PlayAudioMp3(yinbiao_read_info.cur_icon_idx, yinbiao_info_text[yinbiao_read_info.cur_icon_idx][idx].text);
    }else{
        yinbiao_table_play_status = 0;
        Yinbiao_StopMp3Data();
    }
    YinbiaoTableTipWin_UpdateButton(yinbiao_table_play_status);
}

LOCAL void YinbiaoTableTipWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T text_left_rect = yinbiao_msg_tips_left_rect;
    GUI_RECT_T text_right_rect = yinbiao_msg_tips_right_rect;
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMG_YINBIAO_TABLE_TIPS_STOP_IMG;
    GUIBUTTON_SetRect(ZMT_YINBIAO_TABLE_TIP_LEFT_CTRL_ID, &text_left_rect);
    GUIBUTTON_SetCallBackFunc(ZMT_YINBIAO_TABLE_TIP_LEFT_CTRL_ID, YinbiaoTableTipWin_LeftButtonCallback);
    GUIBUTTON_SetBg(ZMT_YINBIAO_TABLE_TIP_LEFT_CTRL_ID, &bg);

    GUIBUTTON_SetRect(ZMT_YINBIAO_TABLE_TIP_RIGHT_CTRL_ID, &text_right_rect);
    GUIBUTTON_SetCallBackFunc(ZMT_YINBIAO_TABLE_TIP_RIGHT_CTRL_ID, MMI_CloseYinbiaoTableTipWin);
}

LOCAL void YinbiaoTableTipWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T text_rect = yinbiao_msg_rect;
    GUI_RECT_T text_left_rect = yinbiao_msg_tips_left_rect;
    GUI_RECT_T text_right_rect = yinbiao_msg_tips_right_rect;
    wchar text_wchar[20] = {0};
    char text[20] = {0};
    uint8 idx = MMK_GetWinAddDataPtr(win_id);
    uint8 size = 0;

    LCD_FillRoundedRect(&lcd_dev_info, text_rect, text_rect, MMI_WHITE_COLOR);
    LCD_DrawRoundedRect(&lcd_dev_info, text_rect, text_rect, YINBIAO_TITLE_BG_COLOR);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font_color = YINBIAO_WIN_BG_COLOR;

    text_rect.bottom = text_left_rect.top;
    text_rect.left += 10;
    text_rect.right -= 10;
    if(yinbiao_table_play_status == -1)
    {
        GUIBUTTON_SetVisible(ZMT_YINBIAO_TABLE_TIP_LEFT_CTRL_ID, FALSE, FALSE);
        MMIRES_GetText(YINBIAO_MP3_ERROR_TIP_TXT, win_id, &text_string);
        text_style.font = DP_FONT_22;
    }
    else
    {
        GUIBUTTON_SetVisible(ZMT_YINBIAO_TABLE_TIP_LEFT_CTRL_ID, TRUE, TRUE);
        sprintf(text, "[%s]", yinbiao_info_text[yinbiao_read_info.cur_icon_idx][idx].text);
        size = strlen(text);
        GUI_UTF8ToWstr(text_wchar, 20, text, size);
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

LOCAL MMI_RESULT_E HandleYinbiaoTableTipWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:  
            {
                YinbiaoTableTipWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {    
                YinbiaoTableTipWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            {
                YinbiaoTableTipWin_LeftButtonCallback();
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
                yinbiao_table_play_status = 0;
                Yinbiao_StopMp3Data();
            }
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

WINDOW_TABLE(YINBIAO_TABLE_TIP_WIN_TAB) =
{
    WIN_ID(ZMT_YINBIAO_TABLE_TIP_WIN_ID),
    WIN_FUNC((uint32)HandleYinbiaoTableTipWinMsg),
    CREATE_BUTTON_CTRL(PNULL, ZMT_YINBIAO_TABLE_TIP_LEFT_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_YINBIAO_TABLE_TIPS_CLOSE_IMG, ZMT_YINBIAO_TABLE_TIP_RIGHT_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL void MMI_CloseYinbiaoTableTipWin(void)
{
    if(MMK_IsOpenWin(ZMT_YINBIAO_TABLE_TIP_WIN_ID))
    {
        MMK_CloseWin(ZMT_YINBIAO_TABLE_TIP_WIN_ID);
    }
}

PUBLIC void MMI_CreateYinbiaoTableTipWin(uint8 idx)
{
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = yinbiao_msg_rect;
    
    MMI_CloseYinbiaoTableTipWin();
    win_handle = MMK_CreateWin(YINBIAO_TABLE_TIP_WIN_TAB, (ADD_DATA)idx);
    MMK_SetWinRect(win_handle,&rect);
}

LOCAL void YinbiaoTableWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    uint8 i = 0;
    uint8 idx = MMK_GetWinAddDataPtr(win_id);
    uint8 num = yinbiao_info_num[idx].num;
    GUIFORM_CHILD_WIDTH_T list_ctrl_width  = {0};
    GUIFORM_CHILD_HEIGHT_T list_ctrl_height = {0};
    MMI_CTRL_ID_T form_id = ZMT_YINBIAO_TABLE_FORM_CTRL_ID;
    MMI_CTRL_ID_T form_ctrl_id = ZMT_YINBIAO_TABLE_FORM_CHILD_CTRL_ID;
    MMI_CTRL_ID_T list_ctrl_id = 0;
    GUI_RECT_T form_rect = yinbiao_list_rect;
    GUI_BG_T form_bg = {GUI_BG_COLOR, GUI_SHAPE_ROUNDED_RECT, 0, YINBIAO_WIN_BG_COLOR, FALSE};
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
        list_ctrl_id = ZMT_YINBIAO_TABLE_FORM_LEFT_CTRL_ID + i;
        GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
        GUILIST_SetNeedHiLightBar(list_ctrl_id,FALSE);
        GUILIST_SetMaxItem(list_ctrl_id, 11, FALSE);
        GUILIST_SetNeedPrgbarBlock(list_ctrl_id,FALSE);
        GUILIST_SetUserBg(list_ctrl_id,TRUE);
        GUILIST_SetBgColor(list_ctrl_id,YINBIAO_WIN_BG_COLOR);
        GUILIST_SetTextFont(list_ctrl_id, DP_FONT_24, YINBIAO_WIN_BG_COLOR);
        GUILIST_PermitBorder(list_ctrl_id, FALSE);
        GUILIST_SetSlideState(list_ctrl_id, FALSE);
        list_ctrl_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        list_ctrl_height.add_data = 1.5*YINBIAO_LINE_HIGHT*(num/2+num%2);
        GUIFORM_SetChildHeight(form_ctrl_id, list_ctrl_id, &list_ctrl_height);
        list_ctrl_width.type = GUIFORM_CHILD_WIDTH_AUTO;
        GUIFORM_SetChildWidth(form_ctrl_id, list_ctrl_id, &list_ctrl_width);
    }
}

LOCAL void YinbiaoTableWin_DisplayTableList(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T list_ctrl_id = 0;
    MMI_STRING_T text_string = {0};
    wchar text_str[20] = {0};
    char text[20] = {0};
    uint8 idx = MMK_GetWinAddDataPtr(win_id);
    uint8 num = yinbiao_info_num[idx].num;
    uint8 list_idx = 0;
    uint8 list_num = 0;
    uint8 i,j = 0;
    
    for(i = 0;i < 2;i++)
    {
        GUILIST_ITEM_T item_info = {0};
        GUILIST_ITEM_DATA_T item_data= {0};
        uint8 size = 0;

        list_ctrl_id = ZMT_YINBIAO_TABLE_FORM_LEFT_CTRL_ID + i;
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
            item_data.item_content[0].item_data.image_id = IMG_YINBIAO_TABLE_ITEM_BG;

            memset(text_str, 0, 20);
            memset(text, 0, 20);
            sprintf(text, "[%s]", yinbiao_info_text[idx][list_idx].text);
            size = strlen(text);
            GUI_UTF8ToWstr(text_str, 20, text, size);
            text_string.wstr_ptr = text_str;
            text_string.wstr_len = MMIAPICOM_Wstrlen(text_str);
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer = text_string;

            GUILIST_AppendItem(list_ctrl_id, &item_info);
        }
    }
}

LOCAL void YinbiaoTableWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T title_rect = yinbiao_title_rect;
    MMI_TEXT_ID_T text_id[YINBIAO_ICON_LIST_ITEM_MAX] = {
        YINBIAO_DAN_YM_TXT, YINBIAO_FU_YM_TXT, YINBIAO_QIAN_YM_TXT,
        YINBIAO_HOU_YM_TXT, YINBIAO_SEHNG_MU_TXT, YINBIAO_ZHENG_TI_TXT
    };
    uint8 idx = (uint8)MMK_GetWinAddDataPtr(win_id);

    MMIRES_GetText(text_id[idx], win_id, &text_string);
    Yinbiao_DrawWinTitle(win_id, 0, text_string, title_rect, DP_FONT_24);

    YinbiaoTableWin_DisplayTableList(win_id);
}

LOCAL void YinbiaoTableWin_CTL_PENOK(MMI_WIN_ID_T win_id, DPARAM param)
{
    int cur_ctrl_idx = 0;
    uint16 cur_idx = 0;
    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T *)param)->src_id;
    cur_ctrl_idx = ctrl_id - ZMT_YINBIAO_TABLE_FORM_LEFT_CTRL_ID;
    if(cur_ctrl_idx < 0){
        cur_ctrl_idx = 0;
        ctrl_id = ZMT_YINBIAO_TABLE_FORM_LEFT_CTRL_ID;
    }
    cur_idx = GUILIST_GetCurItemIndex(ctrl_id);
    cur_idx *= 2;
    cur_idx += cur_ctrl_idx;
    SCI_TRACE_LOW("%s: cur_idx = %d", __FUNCTION__, cur_idx);
    MMI_CreateYinbiaoTableTipWin(cur_idx);
}

LOCAL MMI_RESULT_E HandleYinbiaoTableWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                YinbiaoTableWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {
                YinbiaoTableWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_APP_WEB:
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            { 
                YinbiaoTableWin_CTL_PENOK(win_id, param);
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

WINDOW_TABLE(MMI_YINBIAO_TABLE_WIN_TAB) = {
    WIN_ID(ZMT_YINBIAO_TABLE_WIN_ID),
    WIN_FUNC((uint32)HandleYinbiaoTableWinMsg),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, ZMT_YINBIAO_TABLE_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, ZMT_YINBIAO_TABLE_FORM_CHILD_CTRL_ID, ZMT_YINBIAO_TABLE_FORM_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, ZMT_YINBIAO_TABLE_FORM_LEFT_CTRL_ID, ZMT_YINBIAO_TABLE_FORM_CHILD_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, ZMT_YINBIAO_TABLE_FORM_RIGHT_CTRL_ID, ZMT_YINBIAO_TABLE_FORM_CHILD_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_CloseYinbiaoTableWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(ZMT_YINBIAO_TABLE_WIN_ID)){
        MMK_CloseWin(ZMT_YINBIAO_TABLE_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreateYinbiaoTableWin(uint8 idx)
{
    MMI_CloseYinbiaoTableWin();
    MMK_CreateWin((uint32 *)MMI_YINBIAO_TABLE_WIN_TAB, (ADD_DATA)idx);
}

LOCAL void YinbiaoReadWin_UpdateTopButton(BOOLEAN is_circulate, BOOLEAN is_single)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T single_rect = yinbiao_list_rect;
    GUI_RECT_T fill_rect = {0};
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_IMG;
    
    single_rect.bottom = single_rect.top + YINBIAO_LINE_HIGHT;
    single_rect.left = MMI_MAINSCREEN_WIDTH - YINBIAO_LINE_WIDTH;
    single_rect.right = MMI_MAINSCREEN_WIDTH;

    fill_rect.top = single_rect.top;
    fill_rect.bottom = single_rect.bottom;
    fill_rect.left = 0;
    fill_rect.right = MMI_MAINSCREEN_WIDTH;
    GUI_FillRect(&lcd_dev_info, fill_rect, YINBIAO_WIN_BG_COLOR);

    if(is_circulate){
        bg.img_id = IMG_YINBIAO_CIRCULATE_PRE;
    }else{
        bg.img_id = IMG_YINBIAO_CIRCULATE_DEF;
    }
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_CIRCULATE_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_YINBIAO_READ_CIRCULATE_CTRL_ID);

    if(is_single){
        bg.img_id = IMG_YINBIAO_SINGLE_PRE;
    }else{
        bg.img_id = IMG_YINBIAO_SINGLE_DEF;
    }
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_SINGLE_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_YINBIAO_READ_SINGLE_CTRL_ID);
}

LOCAL void YinbiaoReadWin_UpdateButtonBgWin(BOOLEAN is_play)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T button_rect = yinbiao_list_rect;
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_IMG;
    
    button_rect.top = button_rect.bottom - 2*YINBIAO_LINE_HIGHT + 5;
    button_rect.bottom = MMI_MAINSCREEN_HEIGHT;
    
    GUI_FillRect(&lcd_dev_info, button_rect, YINBIAO_WIN_BG_COLOR);

    if(is_play){
        bg.img_id = IMG_YINBIAO_PLAY;
    }else{
        bg.img_id = IMG_YINBIAO_STOP;
    }
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_PLAY_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_YINBIAO_READ_PLAY_CTRL_ID);
    
    bg.img_id = IMG_YINBIAO_PREVIEW;
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_PRE_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_YINBIAO_READ_PRE_CTRL_ID);

    bg.img_id = IMG_YINBIAO_NEXT;
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_NEXT_CTRL_ID, &bg);
    GUIBUTTON_Update(ZMT_YINBIAO_READ_NEXT_CTRL_ID);
}

LOCAL void YinbiaoReadWin_TableCallback(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_YINBIAO_READ_WIN_ID);
    MMI_CreateYinbiaoTableWin(idx);
    yinbiao_read_info.is_play = FALSE;
    Yinbiao_StopMp3Data();
}

LOCAL void YinbiaoReadWin_CirculateCallback(void)
{
    if(yinbiao_read_info.is_circulate){
        yinbiao_read_info.is_circulate = FALSE;
    }else{
        yinbiao_read_info.is_circulate = TRUE;
        yinbiao_read_info.is_single = FALSE;
    }
    YinbiaoReadWin_UpdateTopButton(yinbiao_read_info.is_circulate, yinbiao_read_info.is_single);
}

LOCAL void YinbiaoReadWin_SingleCallback(void)
{
    if(yinbiao_read_info.is_single){
        yinbiao_read_info.is_single = FALSE;
    }else{
        yinbiao_read_info.is_circulate = FALSE;
        yinbiao_read_info.is_single = TRUE;
    }
    YinbiaoReadWin_UpdateTopButton(yinbiao_read_info.is_circulate, yinbiao_read_info.is_single);
}

LOCAL void YinbiaoReadWin_PlayCallback(void)
{
    SCI_TRACE_LOW("%s: yinbiao_read_info.cur_read_idx = %d", __FUNCTION__, yinbiao_read_info.cur_read_idx);
    if(yinbiao_read_info.is_play){
        yinbiao_read_info.is_play = FALSE;
        Yinbiao_StopMp3Data();
    }else{
        uint8 idx = MMK_GetWinAddDataPtr(ZMT_YINBIAO_READ_WIN_ID);
        yinbiao_read_info.is_play = TRUE;
        Yinbiao_PlayAudioMp3(idx, yinbiao_info_text[idx][yinbiao_read_info.cur_read_idx].text);
    }
    Yinbiao_StopIntervalTimer();
    if(MMK_IsFocusWin(ZMT_YINBIAO_READ_WIN_ID)){
        YinbiaoReadWin_UpdateButtonBgWin(yinbiao_read_info.is_play);
    }
}

LOCAL void YinbiaoReadWin_PreCallback(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_YINBIAO_READ_WIN_ID);
    Yinbiao_StopMp3Data();
    Yinbiao_StopIntervalTimer();
    if(yinbiao_read_info.cur_read_idx > 0)
    {
        yinbiao_read_info.cur_read_idx--;
    }
    else
    {
        yinbiao_read_info.cur_read_idx = yinbiao_info_num[idx].num - 1;
    }
    if(yinbiao_read_info.is_play){
        Yinbiao_PlayAudioMp3(idx, yinbiao_info_text[idx][yinbiao_read_info.cur_read_idx].text);
    }
    if(MMK_IsFocusWin(ZMT_YINBIAO_READ_WIN_ID)){
        MMK_SendMsg(ZMT_YINBIAO_READ_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

LOCAL void YinbiaoReadWin_NextCallback(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_YINBIAO_READ_WIN_ID);
    Yinbiao_StopMp3Data();
    Yinbiao_StopIntervalTimer();
    if(yinbiao_read_info.cur_read_idx + 1 < yinbiao_info_num[idx].num)
    {
        yinbiao_read_info.cur_read_idx++;
    }
    else
    {
        yinbiao_read_info.cur_read_idx = 0;
    }
    if(yinbiao_read_info.is_play){
        Yinbiao_PlayAudioMp3(idx, yinbiao_info_text[idx][yinbiao_read_info.cur_read_idx].text);
    }
    if(MMK_IsFocusWin(ZMT_YINBIAO_READ_WIN_ID)){
        MMK_SendMsg(ZMT_YINBIAO_READ_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

LOCAL void YinbiaoReadWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T table_rect = yinbiao_title_rect;
    GUI_RECT_T yinbiao_rect = yinbiao_yinbiao_rect;
    GUI_RECT_T single_rect = yinbiao_list_rect;
    GUI_RECT_T button_rect = yinbiao_list_rect;
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_IMG;

    single_rect.top += 1;
    single_rect.bottom = single_rect.top + 1.5*YINBIAO_LINE_HIGHT;
    single_rect.left = 0;
    single_rect.right = 2*YINBIAO_LINE_WIDTH;
    Yinbiao_InitButton(ZMT_YINBIAO_READ_CIRCULATE_CTRL_ID, single_rect, NULL, ALIGN_HVMIDDLE, FALSE, YinbiaoReadWin_CirculateCallback);
    bg.img_id = IMG_YINBIAO_CIRCULATE_DEF;
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_CIRCULATE_CTRL_ID, &bg);
    
    single_rect.left = MMI_MAINSCREEN_WIDTH - 2*YINBIAO_LINE_WIDTH;
    single_rect.right = MMI_MAINSCREEN_WIDTH;
    Yinbiao_InitButton(ZMT_YINBIAO_READ_SINGLE_CTRL_ID, single_rect, NULL, ALIGN_HVMIDDLE, FALSE, YinbiaoReadWin_SingleCallback);
    bg.img_id = IMG_YINBIAO_SINGLE_DEF;
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_SINGLE_CTRL_ID, &bg);

    button_rect.top = button_rect.bottom - 2*YINBIAO_LINE_HIGHT + 5;
    button_rect.bottom = MMI_MAINSCREEN_HEIGHT;
    button_rect.left = 0;
    button_rect.right = 2*YINBIAO_LINE_WIDTH;
    Yinbiao_InitButton(ZMT_YINBIAO_READ_PRE_CTRL_ID, button_rect, NULL, ALIGN_HVMIDDLE, FALSE, YinbiaoReadWin_PreCallback);
    bg.img_id = IMG_YINBIAO_PREVIEW;
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_PRE_CTRL_ID, &bg);

    button_rect.left = button_rect.right;
    button_rect.right += 2*YINBIAO_LINE_WIDTH;
    Yinbiao_InitButton(ZMT_YINBIAO_READ_PLAY_CTRL_ID, button_rect, NULL, ALIGN_HVMIDDLE, FALSE, YinbiaoReadWin_PlayCallback);
    bg.img_id = IMG_YINBIAO_STOP;
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_PLAY_CTRL_ID, &bg);

    button_rect.left = button_rect.right;
    button_rect.right += 2*YINBIAO_LINE_WIDTH;
    Yinbiao_InitButton(ZMT_YINBIAO_READ_NEXT_CTRL_ID, button_rect, NULL, ALIGN_HVMIDDLE, FALSE, YinbiaoReadWin_NextCallback);
    bg.img_id = IMG_YINBIAO_NEXT;
    GUIBUTTON_SetBg(ZMT_YINBIAO_READ_NEXT_CTRL_ID, &bg);

    yinbiao_rect.bottom -= YINBIAO_LINE_HIGHT;
    GUIBUTTON_SetRect(ZMT_YINBIAO_READ_YINBIAO_CTRL_ID, &yinbiao_rect);
    GUIBUTTON_SetVisible(ZMT_YINBIAO_READ_YINBIAO_CTRL_ID, FALSE, FALSE);

  table_rect.left = MMI_MAINSCREEN_WIDTH - YINBIAO_LINE_WIDTH-20;
	table_rect.right-=20;
    GUIBUTTON_SetRect(ZMT_YINBIAO_READ_TABLE_CTRL_ID, &table_rect);
    GUIBUTTON_SetCallBackFunc(ZMT_YINBIAO_READ_TABLE_CTRL_ID, YinbiaoReadWin_TableCallback);
    GUIBUTTON_SetVisible(ZMT_YINBIAO_READ_TABLE_CTRL_ID, FALSE, FALSE);

    Yinbiao_RequestAudioPath();
}

LOCAL void YinbiaoReadWin_DisplayYinbiaoTie(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T yinbiao_rect = yinbiao_yinbiao_rect;
    GUI_FONT_ALL_T font = {DP_FONT_22, MMI_WHITE_COLOR};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    char text[20] = {0}; 
    wchar text_str[20] = {0};
    uint8 size = 0;
    uint8 idx = MMK_GetWinAddDataPtr(win_id);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_28;
    text_style.font_color = MMI_WHITE_COLOR;
    
    LCD_FillRoundedRect(&lcd_dev_info, yinbiao_rect, yinbiao_rect, YINBIAO_TITLE_BG_COLOR);

    sprintf(text, "[%s]", yinbiao_info_text[idx][yinbiao_read_info.cur_read_idx].text);
    size = strlen(text);
    GUI_UTF8ToWstr(text_str, 20, text, strlen(text));
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

LOCAL void YinbiaoReadWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T win_rect = yinbiao_win_rect;
    GUI_RECT_T title_rect = yinbiao_title_rect;
    MMI_TEXT_ID_T text_id[YINBIAO_ICON_LIST_ITEM_MAX] = {
        YINBIAO_DAN_YM_TXT, YINBIAO_FU_YM_TXT, YINBIAO_QIAN_YM_TXT,
        YINBIAO_HOU_YM_TXT, YINBIAO_SEHNG_MU_TXT, YINBIAO_ZHENG_TI_TXT,
        YINBIAO_HALF_TXT
    };
    uint8 idx = (uint8)MMK_GetWinAddDataPtr(win_id);

    MMIRES_GetText(text_id[idx], win_id, &text_string);
    Yinbiao_DrawWinTitle(win_id, 0, text_string, title_rect, DP_FONT_22);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;
    if(yinbiao_request_status <= 0)
    {
        switch(yinbiao_request_status)
        {
            case 0:
                MMIRES_GetText(TXT_YINBIAO_INFO_LOADING, win_id, &text_string);
                break;
            case -1:
                MMIRES_GetText(TXT_YINBIAO_INFO_LOAD_FAIL, win_id, &text_string);
                break;
            case -2:
                MMIRES_GetText(TXT_YINBIAO_INFO_REQUESET_FAIL, win_id, &text_string);
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
    
    GUIBUTTON_SetVisible(ZMT_YINBIAO_READ_CIRCULATE_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_YINBIAO_READ_SINGLE_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_YINBIAO_READ_NEXT_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_YINBIAO_READ_PLAY_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_YINBIAO_READ_PRE_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_YINBIAO_READ_YINBIAO_CTRL_ID, TRUE, TRUE);
    GUIBUTTON_SetVisible(ZMT_YINBIAO_READ_TABLE_CTRL_ID, TRUE, TRUE);
    
    YinbiaoReadWin_DisplayYinbiaoTie(win_id);
}

LOCAL void YinbiaoReadWin_CLOSE_WINDOW(void)
{
    yinbiao_read_info.cur_read_idx = 0;
    Yinbiao_StopMp3Data();
    Yinbiao_StopIntervalTimer();
    Yinbiao_ReleaseAudioPath();
    yinbiao_request_status = 0;
    yinbiao_request_now = FALSE;
}

LOCAL MMI_RESULT_E HandleYinbiaoReadWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                YinbiaoReadWin_OPEN_WINDOW(win_id);
             //   WATCHCOM_Backlight(TRUE);
            }
            break;
        case MSG_FULL_PAINT:
            {
                YinbiaoReadWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            { 
                YinbiaoReadWin_PlayCallback();
            }
            break;
        case MSG_KEYDOWN_BACKWARD:
        case MSG_KEYDOWN_FORWARD:
            {
                YinbiaoReadWin_SingleCallback();
            }
            break;
        case MSG_APP_LEFT:
            {
                YinbiaoReadWin_PreCallback();
            }
            break;
        case MSG_APP_RIGHT:
            {
                YinbiaoReadWin_NextCallback();
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
                YinbiaoReadWin_CLOSE_WINDOW();
           //     WATCHCOM_Backlight(FALSE);
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(MMI_YINBIAO_READ_WIN_TAB) = {
    WIN_ID(ZMT_YINBIAO_READ_WIN_ID),
    WIN_FUNC((uint32)HandleYinbiaoReadWinMsg),
    CREATE_BUTTON_CTRL(IMG_YINBIAO_TABLE_BG, ZMT_YINBIAO_READ_TABLE_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_YINBIAO_READ_CIRCULATE_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_YINBIAO_READ_SINGLE_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_YINBIAO_TIE_BG, ZMT_YINBIAO_READ_YINBIAO_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_YINBIAO_READ_NEXT_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_YINBIAO_READ_PLAY_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, ZMT_YINBIAO_READ_PRE_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_CloseYinbiaoReadWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(ZMT_YINBIAO_READ_WIN_ID)){
        MMK_CloseWin(ZMT_YINBIAO_READ_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreateYinbiaoReadWin(uint8 idx)
{
    MMI_CloseYinbiaoReadWin();
    MMK_CreateWin((uint32 *)MMI_YINBIAO_READ_WIN_TAB, (ADD_DATA)idx);
}

LOCAL void YinbiaoMainWin_DisplayButton(MMI_WIN_ID_T win_id)
{
    uint8 i = 0;
    uint8 j = 0;
    uint8 k = 0;
    MMI_CTRL_ID_T ctrl_id = ZMT_YINBIAO_BUTTON_1_CTRL_ID;
    GUI_FONT_ALL_T font = {DP_FONT_22, YINBIAO_WIN_BG_COLOR};
    GUI_BORDER_T last_border = {1, MMI_WHITE_COLOR, GUI_BORDER_NONE};
    GUI_BORDER_T border = {1, MMI_WHITE_COLOR, GUI_BORDER_ROUNDED};
    GUI_RECT_T button_rect = yinbiao_list_rect;
    MMI_TEXT_ID_T text_id[YINBIAO_ICON_LIST_ITEM_MAX] = {
        YINBIAO_DAN_YM_TXT, YINBIAO_FU_YM_TXT, YINBIAO_QIAN_YM_TXT,
        YINBIAO_HOU_YM_TXT, YINBIAO_SEHNG_MU_TXT, YINBIAO_ZHENG_TI_TXT,
		YINBIAO_HALF_TXT
    };
    button_rect.top += 5;
    button_rect.bottom = button_rect.top + 1.5*YINBIAO_LINE_HIGHT;
    for(i = 0;i < 4;i++)
    {
		
        button_rect.left = 5;
        button_rect.right = MMI_MAINSCREEN_WIDTH / 2 - 5;
        for(j = 0;j < 2;j++)
        {
            ctrl_id = ZMT_YINBIAO_BUTTON_1_CTRL_ID + k;
            Yinbiao_InitButton(ctrl_id, button_rect, text_id[k], ALIGN_HVMIDDLE, TRUE, NULL);
            GUIBUTTON_SetFont(ctrl_id, &font);
            MMK_SetActiveCtrl(ctrl_id, FALSE);
            button_rect.left = button_rect.right + 5;
            button_rect.right = MMI_MAINSCREEN_WIDTH - 5;
            k++;
        }
        button_rect.top = button_rect.bottom + 10;
        button_rect.bottom = button_rect.top + 1.5*YINBIAO_LINE_HIGHT;
    }
    GUIBUTTON_SetBorder(yinbiao_cur_select_id, &border, FALSE);
}

LOCAL void YinbiaoMainWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    memset(&yinbiao_read_info, 0, sizeof(YINBIAO_READ_INFO_T));
    yinbiao_cur_select_id = ZMT_YINBIAO_BUTTON_1_CTRL_ID;

    YinbiaoMainWin_DisplayButton(win_id);
}

LOCAL void YinbiaoMainWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_BORDER_T border = {1, MMI_WHITE_COLOR, GUI_BORDER_ROUNDED};
    GUI_RECT_T title_rect = yinbiao_title_rect;
    uint8 i = 0;
    
    MMIRES_GetText(YINBIAO_CLASS_TXT, win_id, &text_string);
    Yinbiao_DrawWinTitle(win_id, 0, text_string, title_rect, DP_FONT_22);

    for(i = 0;i < YINBIAO_ICON_LIST_ITEM_MAX;i++)
    {
        MMI_CTRL_ID_T ctrl_id = ZMT_YINBIAO_BUTTON_1_CTRL_ID + i;
        GUIBUTTON_SetVisible(ctrl_id, TRUE, TRUE);
    }
    GUIBUTTON_SetBorder(yinbiao_cur_select_id, &border, FALSE);
}

LOCAL void YinbiaoMainWin_UpdateSelectButton(MMI_CTRL_ID_T ctrl_id)
{
    GUI_BORDER_T last_border = {1, MMI_WHITE_COLOR, GUI_BORDER_NONE};
    GUI_BORDER_T border = {1, MMI_WHITE_COLOR, GUI_BORDER_ROUNDED};
    
    GUIBUTTON_SetBorder(yinbiao_cur_select_id, &last_border, TRUE);
    GUIBUTTON_Update(yinbiao_cur_select_id);
    GUIBUTTON_SetBorder(ctrl_id, &border, TRUE);
    GUIBUTTON_Update(ctrl_id);
}

LOCAL void YinbiaoMainWin_APP_LEFT(MMI_WIN_ID_T win_id)
{
    int8 idx = 0;
    if(yinbiao_cur_select_id == ZMT_YINBIAO_BUTTON_1_CTRL_ID){
        idx = ZMT_YINBIAO_BUTTON_7_CTRL_ID - ZMT_YINBIAO_BUTTON_1_CTRL_ID;
    }else{
        idx = -1;
    }
    YinbiaoMainWin_UpdateSelectButton(yinbiao_cur_select_id+idx);
    yinbiao_cur_select_id += idx;
    YinbiaoMainWin_FULL_PAINT(win_id);
}

LOCAL void YinbiaoMainWin_APP_RIGHT(MMI_WIN_ID_T win_id)
{
    int8 idx = 0;
    if(yinbiao_cur_select_id == ZMT_YINBIAO_BUTTON_7_CTRL_ID){
        idx = ZMT_YINBIAO_BUTTON_1_CTRL_ID - ZMT_YINBIAO_BUTTON_7_CTRL_ID;
    }else{
        idx = 1;
    }
    YinbiaoMainWin_UpdateSelectButton(yinbiao_cur_select_id+idx);
    yinbiao_cur_select_id += idx;
    YinbiaoMainWin_FULL_PAINT(win_id);
}

LOCAL void YinbiaoMainWin_APP_UP(MMI_WIN_ID_T win_id)
{
    int8 idx = 0;
    if(yinbiao_cur_select_id == ZMT_YINBIAO_BUTTON_1_CTRL_ID){
        idx = 6;
    }else if(yinbiao_cur_select_id == ZMT_YINBIAO_BUTTON_2_CTRL_ID){
        idx = 4;
    }else{
        idx = -2;
    }
    YinbiaoMainWin_UpdateSelectButton(yinbiao_cur_select_id+idx);
    yinbiao_cur_select_id += idx;
    YinbiaoMainWin_FULL_PAINT(win_id);
}

LOCAL void YinbiaoMainWin_APP_DOWN(MMI_WIN_ID_T win_id)
{
    int8 idx = 0;
    if(yinbiao_cur_select_id == ZMT_YINBIAO_BUTTON_7_CTRL_ID){
        idx = -6;
    }else if(yinbiao_cur_select_id == ZMT_YINBIAO_BUTTON_6_CTRL_ID){
        idx = -4;
    }else{
        idx = 2;
    }
    YinbiaoMainWin_UpdateSelectButton(yinbiao_cur_select_id+idx);
    yinbiao_cur_select_id += idx;
    YinbiaoMainWin_FULL_PAINT(win_id);
}

LOCAL void YinbiaoMainWin_APP_OK(MMI_WIN_ID_T win_id)
{
    int8 cur_idx = yinbiao_cur_select_id - ZMT_YINBIAO_BUTTON_1_CTRL_ID;
    if(cur_idx < 0){
        cur_idx = 0;
    }
    SCI_TRACE_LOW("%s: cur_idx = %d", __FUNCTION__, cur_idx);
    yinbiao_read_info.cur_icon_idx = cur_idx;
    MMI_CreateYinbiaoReadWin(cur_idx);
}

LOCAL void YinbiaoMainWin_CTL_PENOK(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T *)param)->src_id;
    int8 cur_idx = ctrl_id - ZMT_YINBIAO_BUTTON_1_CTRL_ID;
    if(cur_idx < 0){
        cur_idx = 0;
        ctrl_id = ZMT_YINBIAO_BUTTON_1_CTRL_ID;
    }
    YinbiaoMainWin_UpdateSelectButton(ctrl_id);
    yinbiao_cur_select_id = ctrl_id;
    SCI_TRACE_LOW("%s: cur_idx = %d", __FUNCTION__, cur_idx);
    yinbiao_read_info.cur_icon_idx = cur_idx;
    MMI_CreateYinbiaoReadWin(cur_idx);
}

LOCAL MMI_RESULT_E HandleYinbiaoMainWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                YinbiaoMainWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {
                YinbiaoMainWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_APP_LEFT:
            {
                YinbiaoMainWin_APP_LEFT(win_id);
            }
            break;
        case MSG_APP_RIGHT:
            {
                YinbiaoMainWin_APP_RIGHT(win_id);
            }
            break;
        case MSG_APP_UP:
            {
                YinbiaoMainWin_APP_UP(win_id);
            }
            break;
        case MSG_APP_DOWN:
            {
                YinbiaoMainWin_APP_DOWN(win_id);
            }
            break;
        case MSG_APP_WEB:
        case MSG_APP_OK:
            {
                YinbiaoMainWin_APP_OK(win_id);
            }
            break;
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            { 
                YinbiaoMainWin_CTL_PENOK(win_id, param);
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

WINDOW_TABLE(MMI_YINBIAO_MAIN_WIN_TAB) = {
    WIN_ID(ZMT_YINBIAO_MAIN_WIN_ID),
    WIN_FUNC((uint32)HandleYinbiaoMainWinMsg),
    CREATE_BUTTON_CTRL(IMG_YINBIAO_ICON_BG, ZMT_YINBIAO_BUTTON_1_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_YINBIAO_ICON_BG, ZMT_YINBIAO_BUTTON_2_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_YINBIAO_ICON_BG, ZMT_YINBIAO_BUTTON_3_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_YINBIAO_ICON_BG, ZMT_YINBIAO_BUTTON_4_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_YINBIAO_ICON_BG, ZMT_YINBIAO_BUTTON_5_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_YINBIAO_ICON_BG, ZMT_YINBIAO_BUTTON_6_CTRL_ID),
	 CREATE_BUTTON_CTRL(IMG_YINBIAO_ICON_BG, ZMT_YINBIAO_BUTTON_7_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_CloseYinbiaoMainWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(ZMT_YINBIAO_MAIN_WIN_ID)){
        MMK_CloseWin(ZMT_YINBIAO_MAIN_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreateYinbiaoMainWin(void)
{
#ifdef LISTENING_PRATICE_SUPPORT
    if(!zmt_tfcard_exist())
    {
        MMI_CreateListeningTipWin(PALYER_PLAY_NO_TFCARD_TIP);
    }
    else
#endif
    {
        MMI_CloseYinbiaoMainWin();
        MMK_CreateWin((uint32 *)MMI_YINBIAO_MAIN_WIN_TAB, PNULL);
    }
}

