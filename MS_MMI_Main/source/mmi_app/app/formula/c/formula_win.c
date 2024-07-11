#include "mmk_type.h"
#include "graphics_draw.h"
#include "mmk_app.h"
#include "mmidisplay_color.h"
#include "window_parse.h"
#include "formula_export.h"
#include "formula_image.h"
#include "formula_text.h"
#include "formula_id.h"
#include "os_api.h"
#include "dal_time.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "guibutton.h"
#include "guifont.h"
#include "mmi_theme.h"
#include "mmidisplay_data.h"
#include "mmi_timer_export.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_default.h"
#include "mmiacc_id.h"
#include "mmiacc_position.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "mmiset_export.h"
#include "mmimp3_export.h"
#include "mmiacc_nv.h"
#include "guistring.h"
#include "mmimp3_export.h"
#ifdef MATH_COUNT_SUPPORT
#include "math_count_export.h"
#include "math_count_image.h"
#include "math_count_text.h"
#endif
#ifdef ZMT_CLASS_SUPPORT
#include "zmt_class_image.h"
#endif

LOCAL GUI_RECT_T formula_win_rect = {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT};//´°¿Ú
LOCAL GUI_RECT_T formula_title_rect = {0, 0, MMI_MAINSCREEN_WIDTH, FORMULA_LINE_HIGHT};//¶¥²¿
LOCAL GUI_RECT_T formula_action_tip_rect = {0, 2*FORMULA_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, 3*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_multi_text_rect = {0, 3*FORMULA_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, 4*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_action_play_rect = {2.5*FORMULA_LINE_WIDTH, 6*FORMULA_LINE_HIGHT, 4*FORMULA_LINE_WIDTH, 8*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_action_stop_rect = {FORMULA_LINE_WIDTH, 6*FORMULA_LINE_HIGHT, 3*FORMULA_LINE_WIDTH, 8*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_action_reset_rect = {4*FORMULA_LINE_WIDTH, 6*FORMULA_LINE_HIGHT, 6*FORMULA_LINE_WIDTH, 8*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_action_pratice_rect = {9, 10*FORMULA_LIST_ITEM_HIGHT, 115, 11.5*FORMULA_LIST_ITEM_HIGHT};
LOCAL GUI_RECT_T formula_action_table_rect = {125, 10*FORMULA_LIST_ITEM_HIGHT, MMI_MAINSCREEN_WIDTH-9, 11.5*FORMULA_LIST_ITEM_HIGHT};
LOCAL GUI_RECT_T formula_multi_table_rect = {0, FORMULA_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT - 1.5*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_multi_left_list_rect = {0, FORMULA_LINE_HIGHT, 3*FORMULA_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT - 1.5*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_multi_right_list_rect = {3*FORMULA_LINE_WIDTH, FORMULA_LINE_HIGHT, 6*FORMULA_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT - 1.5*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_left_button_rect = {9, MMI_MAINSCREEN_HEIGHT - 1.5*FORMULA_LINE_HIGHT, 115, MMI_MAINSCREEN_HEIGHT - 2};
LOCAL GUI_RECT_T formula_right_button_rect = {125, MMI_MAINSCREEN_HEIGHT - 1.5*FORMULA_LINE_HIGHT, MMI_MAINSCREEN_WIDTH-9, MMI_MAINSCREEN_HEIGHT - 2};
LOCAL GUI_RECT_T formula_msg_rect = {FORMULA_LINE_WIDTH, 3*FORMULA_LINE_HIGHT, MMI_MAINSCREEN_WIDTH - FORMULA_LINE_WIDTH, 7*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_msg_tips_left_rect = {1.2*FORMULA_LINE_WIDTH, 5.5*FORMULA_LINE_HIGHT, 2.7*FORMULA_LINE_WIDTH, 7*FORMULA_LINE_HIGHT};
LOCAL GUI_RECT_T formula_msg_tips_right_rect = {3.3*FORMULA_LINE_WIDTH, 5.5*FORMULA_LINE_HIGHT, 4.8*FORMULA_LINE_WIDTH, 7*FORMULA_LINE_HIGHT};

LOCAL FORMULA_MULTI_TEXT_T formula_multi_text[FORMULA_COUNT_ITEM_MAX] = {
    "1  x  1  =  1", "1  x  2  =  2", "1  x  3  =  3", "1  x  4  =  4", "1  x  5  =  5", "1  x  6  =  6", "1  x  7  =  7", "1  x  8  =  8", "1  x  9  =  9",
    "2  x  2  =  4", "2  x  3  =  6", "2  x  4  =  8", "2  x  5  =  10", "2  x  6  =  12", "2  x  7  =  14", "2  x  8  =  16", "2  x  9  =  18", 
    "3  x  3  =  9", "3  x  4  =  12", "3  x  5  =  15", "3  x  6  =  18", "3  x  7  =  21", "3  x  8  =  24", "3  x  9  =  27",
    "4  x  4  =  16", "4  x  5  =  20", "4  x  6  =  24", "4  x  7  =  28", "4  x  8  =  32", "4  x  9  =  36", 
    "5  x  5  =  25", "5  x  6  =  30", "5  x  7  =  35", "5  x  8  =  40", "5  x  9  =  45", 
    "6  x  6  =  36", "6  x  7  =  42", "6  x  8  =  48", "6  x  9  =  54", 
    "7  x  7  =  49", "7  x  8  =  56", "7  x  9  =  63", 
    "8  x  8  =  64", "8  x  9  =  72", "9  x  9  =  81"
};

LOCAL FORMULA_PLAY_INFO_T formula_play_info = {0};
LOCAL MMISRV_HANDLE_T formula_player_handle = PNULL;
LOCAL uint8 formula_table_play_status = 0;

LOCAL void MMI_CloseFormulaTableTipWin(void);
LOCAL void FormulaWin_StopRing(void);
LOCAL void FormulaWin_PlayRing(uint8 idx);
LOCAL void FormulaWin_ShowMultiText(MMI_WIN_ID_T win_id);
LOCAL void FormulaTableTipWin_UpdateButton(BOOLEAN status);
LOCAL BOOLEAN FormulaTableTipWin_PlayRingCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == formula_player_handle)
        {
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    {
                        formula_table_play_status = 0;
                        if(MMK_IsFocusWin(FORMULA_TABLE_TIP_WIN_ID)){
                            FormulaTableTipWin_UpdateButton(formula_table_play_status);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

LOCAL BOOLEAN FormulaWin_PlayRingCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == formula_player_handle)
        {
            //SCI_TRACE_LOW("%s: report_ptr->report = %d", __FUNCTION__, report_ptr->report);
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    {
                        if(MMK_IsOpenWin(FORMULA_WIN_ID) && formula_play_info.play_status == FORMULA_ACTION_PLAY)
                        {
                            if(formula_play_info.play_idx + 1 >= FORMULA_COUNT_ITEM_MAX){
                                FormulaWin_StopRing();
                                formula_play_info.play_status = FORMULA_ACTION_END;
                                if(MMK_IsFocusWin(FORMULA_WIN_ID)){
                                MMK_SendMsg(FORMULA_WIN_ID, MSG_FULL_PAINT, PNULL);
                                }
                                MMI_CreateFormulaTableWin();
                            }else{
                                formula_play_info.play_idx++;
                                SCI_SLEEP(1000);
                                FormulaWin_PlayRing(formula_play_info.play_idx);
                                if(MMK_IsFocusWin(FORMULA_WIN_ID)){
                                FormulaWin_ShowMultiText(FORMULA_WIN_ID);
                                }
                            }
                        }
                        else
                        {
                            FormulaWin_StopRing();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return TRUE;
}

LOCAL void FormulaWin_StopRing(void)
{
    if(formula_player_handle != 0)
    {
        MMISRVAUD_Stop(formula_player_handle);
        MMISRVMGR_Free(formula_player_handle);
        formula_player_handle = 0;
    }
}

LOCAL void FormulaWin_PlayRing(uint8 idx)
{
    MMIAUD_RING_DATA_INFO_T ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    BOOLEAN result = FALSE;

    FormulaWin_StopRing();

    MMI_GetRingInfo(FORMULA_RING_START_ID+idx, &ring_data);
    if (PNULL == ring_data.data_ptr)
    {
        SCI_TRACE_LOW("%s ring_data.data_ptr is empty!!", __FUNCTION__);
        return;
    }

    if(MMK_IsOpenWin(FORMULA_TABLE_TIP_WIN_ID)){
        req.is_auto_free = TRUE;
        req.notify = FormulaTableTipWin_PlayRingCallback;
    }else{
    req.is_auto_free = FALSE;
    req.notify = FormulaWin_PlayRingCallback;
    }
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.info.type = MMISRVAUD_TYPE_RING_BUF;
    audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MIDI;
    audio_srv.info.ring_buf.data = ring_data.data_ptr;
    audio_srv.info.ring_buf.data_len = ring_data.data_len;
    audio_srv.volume=MMIAPISET_GetMultimVolume();

    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    formula_player_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(formula_player_handle > 0)
    {
        result = MMISRVAUD_Play(formula_player_handle, 0);
        if(!result)
        {
            SCI_TRACE_LOW("%s formula_player_handle error", __FUNCTION__);
            MMISRVMGR_Free(formula_player_handle);
            formula_player_handle = 0;
        }
        if(result == MMISRVAUD_RET_OK)
        {
            SCI_TRACE_LOW("%s formula_player_handle = %d", __FUNCTION__, formula_player_handle);
        }
    }
    else
    {
        SCI_TRACE_LOW("%s formula_player_handle <= 0", __FUNCTION__);
    }
}

LOCAL void FormulaWin_ShowMultiText(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    wchar text_str[50] = {0};

    GUI_FillRect(&lcd_dev_info, formula_multi_text_rect, GUI_RGB2RGB565(80, 162, 254));

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;

    GUI_GBToWstr(text_str, &formula_multi_text[formula_play_info.play_idx].text, strlen(formula_multi_text[formula_play_info.play_idx].text));
    text_string.wstr_ptr = text_str;
    text_string.wstr_len = MMIAPICOM_Wstrlen(text_str);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &formula_multi_text_rect,
        &formula_multi_text_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );
}

LOCAL void FormulaWin_FULL_PAINT(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
                
    GUI_FillRect(&lcd_dev_info, formula_win_rect, GUI_RGB2RGB565(80, 162, 254));
    GUI_FillRect(&lcd_dev_info, formula_title_rect, GUI_RGB2RGB565(108, 181, 255));

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_22;
    text_style.font_color = MMI_WHITE_COLOR;

    MMIRES_GetText(TXT_FORMULA_TITLE, win_id, &text_string);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &formula_title_rect,
        &formula_title_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );

    text_style.font = DP_FONT_20;
    text_style.font_color = GUI_RGB2RGB565(80, 162, 254);

    GUIRES_DisplayImg(PNULL, &formula_action_pratice_rect, PNULL, win_id, FORMULA_BOTTOM_BG_IMG, &lcd_dev_info);
    MMIRES_GetText(TXT_FORMULA_TO_PRATCIE, win_id, &text_string);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &formula_action_pratice_rect,
        &formula_action_pratice_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );

    GUIRES_DisplayImg(PNULL, &formula_action_table_rect, PNULL, win_id, FORMULA_BOTTOM_BG_IMG, &lcd_dev_info);
    MMIRES_GetText(TXT_FORMULA_TABLE, win_id, &text_string);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &formula_action_table_rect,
        &formula_action_table_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );

    text_style.font_color = MMI_WHITE_COLOR;
    switch(formula_play_info.play_status)
    {
        case FORMULA_ACTION_NONE:
            {
                MMIRES_GetText(TXT_FORMULA_ACTION, win_id, &text_string);
                GUISTR_DrawTextToLCDInRect(
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    &formula_action_tip_rect,
                    &formula_action_tip_rect,
                    &text_string,
                    &text_style,
                    text_state,
                    GUISTR_TEXT_DIR_AUTO
                );

                GUIRES_DisplayImg(PNULL, &formula_action_play_rect, PNULL, win_id, FORMULA_START_IMG, &lcd_dev_info);
            }
            break;
        case FORMULA_ACTION_PLAY:
        case FORMULA_ACTION_STOP:
            {
                FormulaWin_ShowMultiText(win_id);
                if(formula_play_info.play_status == FORMULA_ACTION_PLAY)
                {
                    GUIRES_DisplayImg(PNULL, &formula_action_stop_rect, PNULL, win_id, FORMULA_STOP_IMG, &lcd_dev_info);
                }
                else
                {
                    GUIRES_DisplayImg(PNULL, &formula_action_stop_rect, PNULL, win_id, FORMULA_START_IMG, &lcd_dev_info);
                }
                GUIRES_DisplayImg(PNULL, &formula_action_reset_rect, PNULL, win_id, FORMULA_RESET_IMG, &lcd_dev_info);      
            }
            break;
        case FORMULA_ACTION_END:
            {
                MMIRES_GetText(TXT_FORMULA_LISTEN_END, win_id, &text_string);
                GUISTR_DrawTextToLCDInRect(
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    &formula_action_tip_rect,
                    &formula_action_tip_rect,
                    &text_string,
                    &text_style,
                    text_state,
                    GUISTR_TEXT_DIR_AUTO
                );

                GUIRES_DisplayImg(PNULL, &formula_action_play_rect, PNULL, win_id, FORMULA_RESET_IMG, &lcd_dev_info);
            }
            break;
        default:
            break;
    }
}
LOCAL void FormulaWin_CTL_PENOK(MMI_WIN_ID_T win_id)
{
    if(formula_play_info.play_status == FORMULA_ACTION_NONE)
    {
        formula_play_info.play_status = FORMULA_ACTION_PLAY;
        formula_play_info.play_idx = 0;
        FormulaWin_PlayRing(0);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else
    {
        if(formula_play_info.play_status == FORMULA_ACTION_PLAY)
        {
            formula_play_info.play_status = FORMULA_ACTION_STOP;
            FormulaWin_StopRing();
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            formula_play_info.play_status = FORMULA_ACTION_PLAY;
            FormulaWin_PlayRing(formula_play_info.play_idx);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
    }
}
LOCAL void FormulaWin_KEYDOWN_FORWARD(MMI_WIN_ID_T win_id)
{
    if(formula_play_info.play_status == FORMULA_ACTION_PLAY || formula_play_info.play_status == FORMULA_ACTION_STOP)
    {
        formula_play_info.play_status = FORMULA_ACTION_NONE;
        formula_play_info.play_idx = 0;
        FormulaWin_StopRing();
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
}

LOCAL void FormulaWin_HANDLE_UP(MMI_WIN_ID_T win_id, GUI_POINT_T point)
{
    if(GUI_PointIsInRect(point, formula_action_play_rect) && formula_play_info.play_status == FORMULA_ACTION_NONE)
    {
        formula_play_info.play_status = FORMULA_ACTION_PLAY;
        formula_play_info.play_idx = 0;
        FormulaWin_PlayRing(0);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else if(GUI_PointIsInRect(point, formula_action_stop_rect) && 
        (formula_play_info.play_status == FORMULA_ACTION_PLAY || formula_play_info.play_status == FORMULA_ACTION_STOP))
    {
        if(formula_play_info.play_status == FORMULA_ACTION_PLAY)
        {
            formula_play_info.play_status = FORMULA_ACTION_STOP;
            FormulaWin_StopRing();
        }
        else
        {
            formula_play_info.play_status = FORMULA_ACTION_PLAY;
            FormulaWin_PlayRing(formula_play_info.play_idx);
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    else if(GUI_PointIsInRect(point, formula_action_reset_rect) && 
        (formula_play_info.play_status == FORMULA_ACTION_PLAY || formula_play_info.play_status == FORMULA_ACTION_STOP))
    {
        formula_play_info.play_status = FORMULA_ACTION_NONE;
        formula_play_info.play_idx = 0;
        FormulaWin_StopRing();
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
#ifdef MATH_COUNT_SUPPORT
    else if(GUI_PointIsInRect(point, formula_action_pratice_rect))
    {
        FormulaWin_StopRing();
        if(formula_play_info.play_status == FORMULA_ACTION_PLAY){
            formula_play_info.play_status = FORMULA_ACTION_STOP;
        }
        MMIZMT_CreateMathCountWin(1);
    }
#endif
    else if(GUI_PointIsInRect(point, formula_action_table_rect))
    {
        FormulaWin_StopRing();
        if(formula_play_info.play_status == FORMULA_ACTION_PLAY){
            formula_play_info.play_status = FORMULA_ACTION_STOP;
        }
        MMI_CreateFormulaTableWin();
    }
}

LOCAL MMI_RESULT_E HandleFormulaWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                memset(&formula_play_info, 0, sizeof(FORMULA_PLAY_INFO_T));
            }
            break;
        case MSG_FULL_PAINT:
            {
                FormulaWin_FULL_PAINT(win_id, lcd_dev_info);
            }
            break;
        case MSG_TP_PRESS_UP:
            {
                GUI_POINT_T point = {0};
                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);
                if(point.y >= formula_action_play_rect.top)
                {
                    FormulaWin_HANDLE_UP(win_id, point);
                }
            }
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            {
                FormulaWin_CTL_PENOK(win_id);
            }           
            break;
        case MSG_KEYDOWN_BACKWARD:
        case MSG_KEYDOWN_FORWARD:
            {
                FormulaWin_KEYDOWN_FORWARD(win_id);
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
                memset(&formula_play_info, 0, sizeof(FORMULA_PLAY_INFO_T));
                FormulaWin_StopRing();
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(FORMULA_WIN_TAB) =
{
    WIN_ID(FORMULA_WIN_ID),
    WIN_FUNC((uint32)HandleFormulaWinMsg),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMI_CreateFormulaWin(void)
{
    if(MMK_IsOpenWin(FORMULA_WIN_ID))
    {
        MMK_CloseWin(FORMULA_WIN_ID);
    }
    MMK_CreateWin(FORMULA_WIN_TAB, NULL);
}
LOCAL void FormulaTableTipWin_UpdateButton(BOOLEAN status)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_BG_T bg = {0};
    GUI_FillRect(&lcd_dev_info, formula_msg_tips_left_rect, MMI_WHITE_COLOR);
    bg.bg_type = GUI_BG_IMG;
    if(status){
        bg.img_id = FORMULA_TIPS_STOP_IMG;
    }else{
        bg.img_id = FORMULA_TIPS_START_IMG;
    }
    GUIBUTTON_SetBg(FORMULA_TABLE_TIP_LEFT_BUTTON_CTRL_ID, &bg);
    GUIBUTTON_Update(FORMULA_TABLE_TIP_LEFT_BUTTON_CTRL_ID);
}
LOCAL void FormulaTableTipWin_LeftButtonCallback(void)
{
    uint8 idx = MMK_GetWinAddDataPtr(FORMULA_TABLE_TIP_WIN_ID);
    if(formula_table_play_status == 0){
        formula_table_play_status = 1;
        FormulaWin_PlayRing(idx);
    }else{
        formula_table_play_status = 0;
        FormulaWin_StopRing();
    }
    FormulaTableTipWin_UpdateButton(formula_table_play_status);
}
LOCAL void FormulaTableTipWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = FORMULA_TIPS_START_IMG;
    GUIBUTTON_SetRect(FORMULA_TABLE_TIP_LEFT_BUTTON_CTRL_ID, &formula_msg_tips_left_rect);
    GUIBUTTON_SetCallBackFunc(FORMULA_TABLE_TIP_LEFT_BUTTON_CTRL_ID, FormulaTableTipWin_LeftButtonCallback);
    GUIBUTTON_SetBg(FORMULA_TABLE_TIP_LEFT_BUTTON_CTRL_ID, &bg);
    GUIBUTTON_SetRect(FORMULA_TABLE_TIP_RIGHT_BUTTON_CTRL_ID, &formula_msg_tips_right_rect);
    GUIBUTTON_SetCallBackFunc(FORMULA_TABLE_TIP_RIGHT_BUTTON_CTRL_ID, MMI_CloseFormulaTableTipWin);
}
LOCAL void FormulaTableTipWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T text_rect = {0};
    wchar text_wchar[100] = {0};
    uint8 idx = MMK_GetWinAddDataPtr(win_id);
    LCD_FillRoundedRect(&lcd_dev_info, formula_msg_rect, formula_msg_rect, MMI_WHITE_COLOR);
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_26;
    text_style.font_color = GUI_RGB2RGB565(80, 162, 254);
    text_rect = formula_msg_rect;
    text_rect.bottom = formula_msg_tips_right_rect.top;
    GUI_GBToWstr(&text_wchar, formula_multi_text[idx].text, strlen(formula_multi_text[idx].text));
    text_string.wstr_ptr = text_wchar;
    text_string.wstr_len = MMIAPICOM_Wstrlen(text_wchar);
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
LOCAL MMI_RESULT_E HandleFormulaTableTipWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:  
            {
                FormulaTableTipWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {    
                FormulaTableTipWin_FULL_PAINT(win_id);
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
                formula_table_play_status = 0;
                FormulaWin_StopRing();
            }
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}
WINDOW_TABLE(FORMULA_TABLE_TIP_WIN_TAB) =
{
    WIN_ID(FORMULA_TABLE_TIP_WIN_ID),
    WIN_FUNC((uint32)HandleFormulaTableTipWinMsg),
    CREATE_BUTTON_CTRL(PNULL, FORMULA_TABLE_TIP_LEFT_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(FORMULA_TIPS_CLOSE_IMG, FORMULA_TABLE_TIP_RIGHT_BUTTON_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};
LOCAL void MMI_CloseFormulaTableTipWin(void)
{
    if(MMK_IsOpenWin(FORMULA_TABLE_TIP_WIN_ID))
    {
        MMK_CloseWin(FORMULA_TABLE_TIP_WIN_ID);
    }
}
PUBLIC void MMI_CreateFormulaTableTipWin(uint8 idx)
{
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = formula_msg_rect;
    MMI_CloseFormulaTableTipWin();
    win_handle = MMK_CreateWin(FORMULA_TABLE_TIP_WIN_TAB, (ADD_DATA)idx);
    MMK_SetWinRect(win_handle,&rect);
}

LOCAL void FormulaTableWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_WIDTH_T list_ctrl_width  = {0};
    GUIFORM_CHILD_HEIGHT_T list_ctrl_height = {0};
    MMI_CTRL_ID_T form_ctrl_id = 0;
    MMI_CTRL_ID_T list_ctrl_id = 0;
    GUI_RECT_T form_rect = {0};
    GUI_BG_T form_bg = {GUI_BG_COLOR, GUI_SHAPE_ROUNDED_RECT, 0, MMI_BLACK_COLOR, FALSE};
    uint16 form_hspace = 0;
    uint16 form_vspave = 10;
    CAF_COLOR_T bg_color = GUI_RGB2RGB565(80, 162, 254);
    MMI_STRING_T text_string = {0};
    wchar text_str[50] = {0};
    uint8 i,j = 0;
    uint8 m = 0;

    form_rect = formula_multi_table_rect;
    form_bg.color = bg_color;
    GUIFORM_SetBg(FORMULA_FORM_CTRL_ID, &form_bg);
    GUIFORM_SetRect(FORMULA_FORM_CTRL_ID, &form_rect);
    GUIFORM_PermitChildBg(FORMULA_FORM_CTRL_ID,FALSE);
    GUIFORM_PermitChildFont(FORMULA_FORM_CTRL_ID,FALSE);
    GUIFORM_PermitChildBorder(FORMULA_FORM_CTRL_ID, FALSE);
    GUIFORM_SetDisplayScrollBar(FORMULA_FORM_CTRL_ID, FALSE);
    MMK_SetActiveCtrl(FORMULA_FORM_CTRL_ID, FALSE);
    for(i = 0;i < 4;i++)
    {
        form_ctrl_id = FORMULA_FORM_CHILD_1_CTRL_ID + i;
        GUIFORM_SetBg(form_ctrl_id, &form_bg);
        GUIFORM_SetRect(form_ctrl_id, &form_rect);
        for(j = 0;j < 2;j++)
        {
            list_ctrl_id = FORMULA_FORM_CHILD_1_LIST_1_CTRL_ID + m;
            GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            GUILIST_SetNeedHiLightBar(list_ctrl_id,FALSE);
            GUILIST_SetMaxItem(list_ctrl_id, 10, FALSE);
            GUILIST_SetNeedPrgbarBlock(list_ctrl_id,FALSE);
            GUILIST_SetUserBg(list_ctrl_id,TRUE);
            GUILIST_SetBgColor(list_ctrl_id,GUI_RGB2RGB565(80, 162, 254));
            GUILIST_SetTextFont(list_ctrl_id, DP_FONT_20, MMI_WHITE_COLOR);
            GUILIST_PermitBorder(list_ctrl_id, FALSE);
            GUILIST_SetSlideState(list_ctrl_id, FALSE);
            list_ctrl_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            list_ctrl_height.add_data = (formula_multi_table_rect.bottom - formula_multi_table_rect.top) - m * FORMULA_LIST_ITEM_HIGHT;
            GUIFORM_SetChildHeight(form_ctrl_id, list_ctrl_id, &list_ctrl_height);
            list_ctrl_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            list_ctrl_width.add_data = (formula_multi_table_rect.right - formula_multi_table_rect.left) / 2;
            GUIFORM_SetChildWidth(form_ctrl_id, list_ctrl_id, &list_ctrl_width);
            m++;
        }
    }
}

LOCAL void FormulaTableWin_ShowMultiTable(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T form_ctrl_id = FORMULA_FORM_CTRL_ID;
    MMI_CTRL_ID_T list_ctrl_id = 0;
    GUI_BG_T form_bg = {GUI_BG_COLOR, GUI_SHAPE_ROUNDED_RECT, 0, GUI_RGB2RGB565(80, 162, 254), FALSE};
    GUI_RECT_T form_rect = {0};
    MMI_HANDLE_T ctrl_handle = 0;
    MMI_STRING_T text_string = {0};
    wchar text_str[50] = {0};
    uint8 i = 0;
    uint8 j = 0;
    uint8 m = 0;

    for(i = 0;i < 8;i++)
    {
        GUILIST_ITEM_T item_info = {0};
        GUILIST_ITEM_DATA_T item_data= {0};
        uint8 list_num = 9 - i;
        if(i % 2 == 1){
            list_num++;
        }
        list_ctrl_id = FORMULA_FORM_CHILD_1_LIST_1_CTRL_ID + i;
        GUILIST_RemoveAllItems(list_ctrl_id);
        for(j = 0;j < list_num;j++)
        {
            item_info.item_style = GUIITEM_STYLE_FORMULA_ITEM_LIST_MS;
            item_info.item_data_ptr = &item_data;

            memset(&text_str, 0, 50);
            if((i % 2 == 1 && i != 7) && j == 0){
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id= NULL;
            }else{
                GUI_GBToWstr(text_str, formula_multi_text[m].text, strlen(formula_multi_text[m].text));
                text_string.wstr_ptr = text_str;
                text_string.wstr_len = MMIAPICOM_Wstrlen(text_str);
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[0].item_data.text_buffer = text_string;
                m++;
            }

            GUILIST_AppendItem(list_ctrl_id, &item_info);
            
        }
    }
}

LOCAL void FormulaTableWin_FULL_PAINT(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
                
    GUI_FillRect(&lcd_dev_info, formula_win_rect, GUI_RGB2RGB565(80, 162, 254));
    GUI_FillRect(&lcd_dev_info, formula_title_rect, GUI_RGB2RGB565(108, 181, 255));

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;

    MMIRES_GetText(TXT_FORMULA_TABLE_TITLE, win_id, &text_string);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &formula_title_rect,
        &formula_title_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );

    FormulaTableWin_ShowMultiTable(win_id);

    text_style.font = DP_FONT_20;
    GUIRES_DisplayImg(PNULL, &formula_left_button_rect, PNULL, win_id, FORMULA_BOTTOM_BG_IMG, &lcd_dev_info);
    MMIRES_GetText(TXT_FORMULA_TO_PRATCIE, win_id, &text_string);
    text_style.font_color = GUI_RGB2RGB565(80, 162, 254);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &formula_left_button_rect,
        &formula_left_button_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );

    GUIRES_DisplayImg(PNULL, &formula_right_button_rect, PNULL, win_id, FORMULA_BOTTOM_BG_IMG, &lcd_dev_info);
    MMIRES_GetText(TXT_FORMULA_TO_LISTEN, win_id, &text_string);
    text_style.font_color = GUI_RGB2RGB565(80, 162, 254);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &formula_right_button_rect,
        &formula_right_button_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );
}

LOCAL void FormulaTableWin_CTL_PENOK(MMI_WIN_ID_T win_id, DPARAM param)
{
    uint8 i = 0;
    uint8 num = 9;
    int8 play_idx = 0;
    int8 cur_idx = 0;
    int8 cur_ctrl_idx = 0;
    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T *)param)->src_id;

    cur_ctrl_idx = ctrl_id - FORMULA_FORM_CHILD_1_LIST_1_CTRL_ID;
    if(cur_ctrl_idx < 0){
        ctrl_id = FORMULA_FORM_CHILD_1_LIST_1_CTRL_ID;
        cur_ctrl_idx = 0;
    }
    cur_idx = GUILIST_GetCurItemIndex(ctrl_id);
    for(i = 0;i < cur_ctrl_idx;i++)
    {
        play_idx += num - i;
    }
    play_idx += cur_idx;
    if(cur_ctrl_idx % 2 == 1 && cur_idx != 0 && ctrl_id != FORMULA_FORM_CHILD_4_LIST_2_CTRL_ID)
    {
        play_idx--;
    }
    SCI_TRACE_LOW("%s: play_idx = %d", __FUNCTION__, play_idx);
    MMI_CreateFormulaTableTipWin(play_idx);
}

LOCAL void FormulaTableWin_TP_PRESS_UP(MMI_WIN_ID_T win_id, GUI_POINT_T point)
{
    if(GUI_PointIsInRect(point, formula_left_button_rect))
    {
        MMIZMT_CreateMathCountWin(1);
        FormulaWin_StopRing();
    }
    else if(GUI_PointIsInRect(point, formula_right_button_rect))
    {
        formula_play_info.play_status = FORMULA_ACTION_PLAY;
        formula_play_info.play_idx = 0;
        FormulaWin_PlayRing(0);
        MMK_CloseWin(win_id);
    }
}

LOCAL MMI_RESULT_E HandleFormulaTableWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                FormulaTableWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {
                FormulaTableWin_FULL_PAINT(win_id, lcd_dev_info);
            }
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            {
                FormulaTableWin_CTL_PENOK(win_id, param);
            }
            break;
        case MSG_TP_PRESS_UP:
            {
                GUI_POINT_T point = {0};
                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);
                if(point.y >= formula_left_button_rect.top)
                {
                    FormulaTableWin_TP_PRESS_UP(win_id, point);
                }
            }
            break;
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

WINDOW_TABLE(FORMULA_TABLE_WIN_TAB) =
{
    WIN_ID(FORMULA_TABLE_WIN_ID),
    WIN_FUNC((uint32)HandleFormulaTableWinMsg),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,FORMULA_FORM_CTRL_ID),
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,FORMULA_FORM_CHILD_1_CTRL_ID,FORMULA_FORM_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, FORMULA_FORM_CHILD_1_LIST_1_CTRL_ID, FORMULA_FORM_CHILD_1_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, FORMULA_FORM_CHILD_1_LIST_2_CTRL_ID, FORMULA_FORM_CHILD_1_CTRL_ID),
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,FORMULA_FORM_CHILD_2_CTRL_ID,FORMULA_FORM_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, FORMULA_FORM_CHILD_2_LIST_1_CTRL_ID, FORMULA_FORM_CHILD_2_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, FORMULA_FORM_CHILD_2_LIST_2_CTRL_ID, FORMULA_FORM_CHILD_2_CTRL_ID),
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,FORMULA_FORM_CHILD_3_CTRL_ID,FORMULA_FORM_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, FORMULA_FORM_CHILD_3_LIST_1_CTRL_ID, FORMULA_FORM_CHILD_3_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, FORMULA_FORM_CHILD_3_LIST_2_CTRL_ID, FORMULA_FORM_CHILD_3_CTRL_ID),
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,FORMULA_FORM_CHILD_4_CTRL_ID,FORMULA_FORM_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, FORMULA_FORM_CHILD_4_LIST_1_CTRL_ID, FORMULA_FORM_CHILD_4_CTRL_ID),
            CHILD_LIST_CTRL(FALSE, GUILIST_TYPE_TEXT_ID, FORMULA_FORM_CHILD_4_LIST_2_CTRL_ID, FORMULA_FORM_CHILD_4_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMI_CreateFormulaTableWin(void)
{
    if(MMK_IsOpenWin(FORMULA_TABLE_WIN_ID))
    {
        MMK_CloseWin(FORMULA_TABLE_WIN_ID);
    }
    MMK_CreateWin(FORMULA_TABLE_WIN_TAB, NULL);
}

LOCAL void FormulaMnemonicWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T ctrl_id = FORMULA_MNEMONIC_LIST_CTRL_ID;
    GUILIST_ITEM_T item_t = {0};
    GUIITEM_STATE_T item_state = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUI_RECT_T list_rect = {0};
    uint8 i = 0;
    MMI_IMAGE_ID_T img_id[FORMULA_APP_COUNT_MAX] = {FORMULA_MNEMONIC_IMG, FORMULA_ICON_IMG};
    MMI_TEXT_ID_T text_id[FORMULA_APP_COUNT_MAX] = {MATH_COUNT_TITLE, TXT_FORMULA_TITLE};
    
    GUILIST_SetMaxItem(ctrl_id, 2, FALSE);
    for(i = 0; i < 2;i++)
    {
        item_t.item_style = GUIITEM_STYLE_CLASS_MAIN_LIST_MS;
        item_t.item_data_ptr = &item_data;
        item_t.item_state = GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;

        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = IMG_FORMULA_MAIN_ITEM_BG;

        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = img_id[i];

        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = text_id[i];

        GUILIST_AppendItem(ctrl_id, &item_t);
    }

    list_rect = formula_win_rect;
    list_rect.top = FORMULA_LINE_HIGHT + 10;
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
    GUILIST_SetNeedPrgbarBlock(ctrl_id,FALSE);
    GUILIST_SetBgColor(ctrl_id,GUI_RGB2RGB565(80, 162, 254));
    GUILIST_SetTextFont(ctrl_id, DP_FONT_24, GUI_RGB2RGB565(80, 162, 254));
    GUILIST_SetSlideState(ctrl_id, FALSE);
    GUILIST_SetRect(ctrl_id, &list_rect);
    
    MMK_SetAtvCtrl(win_id, ctrl_id);
}

LOCAL void FormulaMnemonicWin_FULL_PAINT(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
    
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
                
    GUI_FillRect(&lcd_dev_info, formula_win_rect, GUI_RGB2RGB565(80, 162, 254));
    GUI_FillRect(&lcd_dev_info, formula_title_rect, GUI_RGB2RGB565(108, 181, 255));

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_22;
    text_style.font_color = MMI_WHITE_COLOR;

    MMIRES_GetText(TXT_FORMULA_MNEMONIC_TITLE, win_id, &text_string);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &formula_title_rect,
        &formula_title_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );
}

LOCAL void FormulaMnemonicWin_CTL_PENOK(MMI_WIN_ID_T win_id)
{
    uint16 cur_idx = 0;
    cur_idx = GUILIST_GetCurItemIndex(FORMULA_MNEMONIC_LIST_CTRL_ID);
    if(cur_idx == 0){
        MMIZMT_CreateMathCountWin(0);
    }else{
        MMI_CreateFormulaWin();
    }
}

LOCAL MMI_RESULT_E HandleFormulaMnemonicWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                FormulaMnemonicWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {
                FormulaMnemonicWin_FULL_PAINT(win_id, lcd_dev_info);
            }
            break;
        case MSG_CTL_PENOK:
            {
                FormulaMnemonicWin_CTL_PENOK(win_id);
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

WINDOW_TABLE(FORMULA_MNEMONIC_WIN_TAB) =
{
    WIN_ID(FORMULA_MNEMONIC_WIN_ID),
    WIN_FUNC((uint32)HandleFormulaMnemonicWinMsg),
    CREATE_LISTBOX_CTRL(GUILIST_TYPE_TEXT_ID, FORMULA_MNEMONIC_LIST_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMI_CreateMathMnemonicWin(void)
{
    if(MMK_IsOpenWin(FORMULA_MNEMONIC_WIN_ID))
    {
        MMK_CloseWin(FORMULA_MNEMONIC_WIN_ID);
    }
    MMK_CreateWin(FORMULA_MNEMONIC_WIN_TAB, NULL);
}
PUBLIC void MMIZMT_CloseFormulaPlayer(void)
{
    FormulaWin_StopRing();
}
