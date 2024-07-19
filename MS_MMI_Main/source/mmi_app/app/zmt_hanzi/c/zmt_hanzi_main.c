#include "std_header.h"
#include <stdlib.h>
#include "cjson.h"
#include "dal_time.h"
#include "zmt_hanzi_id.h"
#include "zmt_hanzi_main.h"
#include "zmt_hanzi_text.h"
#include "zmt_hanzi_image.h"
#include "gps_drv.h"
#include "gps_interface.h"
#include "guibutton.h"
#include "guifont.h"
#include "guilcd.h"
#include "guistring.h"
#include "guitext.h"
#include "mmi_textfun.h"
#include "mmiacc_text.h"
#include "mmicc_export.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmisrvrecord_export.h"
#ifdef LISTENING_PRATICE_SUPPORT
#include "zmt_main_file.h"
#include "zmt_listening_image.h"
#endif
#ifdef WORD_CARD_SUPPORT
#include "zmt_word_text.h"
#include "zmt_word_image.h"
#endif
#ifdef FORMULA_SUPPORT
#include "formula_image.h"
#endif
#define HANZI_WIN_BG_COLOR GUI_RGB2RGB565(80, 162, 254)
#define HANZI_TITLE_BG_COLOR GUI_RGB2RGB565(108, 181, 255)

#define hanzi_msg_tips_rect {HANZI_CARD_LINE_WIDTH, 3*HANZI_CARD_LINE_HIGHT, MMI_MAINSCREEN_WIDTH - HANZI_CARD_LINE_WIDTH, 7*HANZI_CARD_LINE_HIGHT}
#define hanzi_msg_tips_left_rect {1.2*HANZI_CARD_LINE_WIDTH, 5.5*HANZI_CARD_LINE_HIGHT, 2.7*HANZI_CARD_LINE_WIDTH, 6.5*HANZI_CARD_LINE_HIGHT}
#define hanzi_msg_tips_right_rect {3.3*HANZI_CARD_LINE_WIDTH, 5.5*HANZI_CARD_LINE_HIGHT, 4.8*HANZI_CARD_LINE_WIDTH, 6.5*HANZI_CARD_LINE_HIGHT}
#define hanzi_win_rect {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT}//窗口
#define hanzi_title_rect {28, 0, 5*HANZI_CARD_LINE_WIDTH-5, HANZI_CARD_LINE_HIGHT}//顶部
#define hanzi_auto_play_rect {4*HANZI_CARD_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-10, HANZI_CARD_LINE_HIGHT}//自动播放图标
#define hanzi_list_rect {0, HANZI_CARD_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT-5}//列表
#define hanzi_dir_rect {5*HANZI_CARD_LINE_WIDTH-25, 0, MMI_MAINSCREEN_WIDTH, HANZI_CARD_LINE_HIGHT}
#define hanzi_hanzi_rect {5, HANZI_CARD_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, 2*HANZI_CARD_LINE_HIGHT}//汉字
#define hanzi_pinyin_audio_rect {5*HANZI_CARD_LINE_WIDTH, HANZI_CARD_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, 2*HANZI_CARD_LINE_HIGHT}//音标icon
#define hanzi_text_rect {5,2.2*HANZI_CARD_LINE_HIGHT+1,MMI_MAINSCREEN_WIDTH-5,MMI_MAINSCREEN_HEIGHT-1.5*HANZI_CARD_LINE_HIGHT-5}//释义
#define hanzi_msg_rect {HANZI_CARD_LINE_WIDTH, 3*HANZI_CARD_LINE_HIGHT, MMI_MAINSCREEN_WIDTH-HANZI_CARD_LINE_WIDTH, 5*HANZI_CARD_LINE_HIGHT}//界面的信息提示
#define hanzi_left_rect {10, MMI_MAINSCREEN_HEIGHT-1.5*HANZI_CARD_LINE_HIGHT, 3*HANZI_CARD_LINE_WIDTH-10, MMI_MAINSCREEN_HEIGHT-5}//汉字学习/掌握
#define hanzi_right_rect {3*HANZI_CARD_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT-1.5*HANZI_CARD_LINE_HIGHT, MMI_MAINSCREEN_WIDTH-10, MMI_MAINSCREEN_HEIGHT-5}//生词本/未掌握
#define hanzi_pre_rect {0.5*HANZI_CARD_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT-HANZI_CARD_LINE_HIGHT, 1.5*HANZI_CARD_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT-2}//生词本-上一个
#define hanzi_del_rect {2.5*HANZI_CARD_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT-HANZI_CARD_LINE_HIGHT, 3.5*HANZI_CARD_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT-2}//生词本-删除
#define hanzi_next_rect {4.5*HANZI_CARD_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT-HANZI_CARD_LINE_HIGHT, 5.5*HANZI_CARD_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT-2}//生词本-下一个
#define hanzi_tip_rect {HANZI_CARD_LINE_WIDTH,4*HANZI_CARD_LINE_HIGHT,MMI_MAINSCREEN_WIDTH-HANZI_CARD_LINE_WIDTH,6*HANZI_CARD_LINE_HIGHT}//弹框提示
#define hanzi_bottom_rect {1.5*HANZI_CARD_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT-1.5*HANZI_CARD_LINE_HIGHT, 4.5*HANZI_CARD_LINE_WIDTH, MMI_MAINSCREEN_HEIGHT-5}
#define hanzi_form_rect {0, HANZI_CARD_LINE_HIGHT, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT - 1.5*HANZI_CARD_LINE_HIGHT}
#define hanzi_left_button_rect {9, MMI_MAINSCREEN_HEIGHT - 1.5*HANZI_CARD_LINE_HIGHT, 115, MMI_MAINSCREEN_HEIGHT - 2}
#define hanzi_right_button_rect {125, MMI_MAINSCREEN_HEIGHT - 1.5*HANZI_CARD_LINE_HIGHT, MMI_MAINSCREEN_WIDTH-9, MMI_MAINSCREEN_HEIGHT - 2}

LOCAL int16 main_tp_down_x = 0;
LOCAL int16 main_tp_down_y = 0;

extern int8 hanzi_book_count;
extern HANZI_PUBLISH_BOOK_INFO * hanzi_publish_info[HANZI_PUBLISH_BOOK_MAX];
extern HANZI_CONTENT_INFO_T * hanzi_content_info[HANZI_CONTENT_CHAPTER_MAX];
extern int16 hanzi_chapter_count;
extern int16 hanzi_chapter_children_count[20];
extern HANZI_BOOK_HANZI_INFO * hanzi_detail_info[100];
extern int16 hanzi_detail_count;
extern int16 hanzi_detail_cur_idx;

HANZI_LEARN_INFO_T * hanzi_learn_info = NULL;
HANZI_BOOK_INFO_T hanzi_book_info = {0};
BOOLEAN is_open_auto_play = TRUE;
LOCAL uint8 open_auto_play_timer = 0;
LOCAL BOOLEAN is_open_new_hanzi = FALSE;
LOCAL GUI_LCD_DEV_INFO hanzi_chapter_tip_layer = {0};
LOCAL GUI_LCD_DEV_INFO hanzi_detail_tip_layer = {0};
LOCAL int8 hanzi_is_display_tip = 0;
LOCAL uint8 hanzi_tip_timer = 0;
LOCAL MMISRV_HANDLE_T hanzi_player_handle = PNULL;
LOCAL uint8 cur_chapter_total_count = 0;
LOCAL uint8 cur_chapter_unmaster_count = 0;
LOCAL BOOLEAN new_hanzi_haved_delete = FALSE;
uint8 cur_new_hanzi_page_idx = 0;
int cur_chapter_unmaster_idx[HANZI_CHAPTER_WORD_MAX] = {0};
LOCAL int16 hanzi_listen_cur_idx = 0;
LOCAL uint8 hanzi_listen_repeat_count = 0;
LOCAL HANZI_LISTEN_INFO_T hanzi_listen_info = {0};
LOCAL uint8 hanzi_listen_timer_id = 0;
LOCAL uint16 hanzi_listen_set[HANZI_LISTEN_SET_SYMBOL_NUM] = {0};
LOCAL uint16 hanzi_listen_idx[HANZI_CHAPTER_WORD_MAX] = {0};

LOCAL MMI_RESULT_E MMI_CloseHanziWin(void);
LOCAL MMI_RESULT_E MMI_CloseHanziChapterWin(void);
LOCAL MMI_RESULT_E MMI_CloseHanziListenWin(void);
LOCAL MMI_RESULT_E MMI_CloseHanziListenInfoWin(void);
LOCAL BOOLEAN MMI_IsOpenHanziListenWin(void);
LOCAL void HanziListenWin_PlayAudioFail(void);
LOCAL void HanziListenWin_CreateIntervalTimer(void);
LOCAL void HanziDetail_ShowTip(void);

LOCAL void Hanzi_DrawWinTitle(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_STRING_T text_string)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    GUI_RECT_T win_rect = hanzi_win_rect;
    GUI_RECT_T title_rect = hanzi_title_rect;
	GUI_RECT_T title_rect_bg = hanzi_title_rect;
	 title_rect_bg.left-=30;
	 title_rect_bg.right+=70;

    GUI_FillRect(&lcd_dev_info, win_rect, HANZI_WIN_BG_COLOR);
    GUI_FillRect(&lcd_dev_info, title_rect_bg, HANZI_TITLE_BG_COLOR);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_22;
    text_style.font_color = MMI_WHITE_COLOR;

    if(ctrl_id != 0)
    {
        GUIBUTTON_SetText(ctrl_id, text_string.wstr_ptr, text_string.wstr_len); 
    }
    else
    {
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &title_rect,
            &title_rect,
            &text_string,
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
        );
    }
}

LOCAL void Hanzi_DrawWinTips(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T win_rect = hanzi_win_rect;
    GUI_RECT_T title_rect = hanzi_title_rect;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_22;
    text_style.font_color = MMI_WHITE_COLOR;

    MMIRES_GetText(text_id, win_id, &text_string);
		CTRLTEXT_IsDisplayPrg(FALSE,ctrl_id);
    if(ctrl_id != 0)
    {
        GUITEXT_SetString(ctrl_id, text_string.wstr_ptr,text_string.wstr_len, TRUE);
    }
    else
    {
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &win_rect,
            &win_rect,
            &text_string,
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
        );
    }
}

LOCAL void Hanzi_InitListbox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_RECT_T list_rect, uint16 max_item)
{
    GUILIST_INIT_DATA_T list_init = {0};
    GUILIST_ITEM_T item_t = {0};
    GUIITEM_STATE_T item_state = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    if(!MMK_GetCtrlHandleByWin(win_id, ctrl_id)){
        list_init.both_rect.v_rect = list_rect;
        list_init.type = GUILIST_TEXTLIST_E;
        GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUILIST_SetMaxItem(ctrl_id, max_item, FALSE);

        GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, TRUE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_AUTO_SCROLL, TRUE);
        GUILIST_SetListState(ctrl_id, GUILIST_STATE_EFFECT_STR,TRUE);
        GUILIST_SetNeedPrgbarBlock(ctrl_id,FALSE);
			   GUILIST_SetListState(ctrl_id,GUILIST_STATE_NEED_PRGBAR,FALSE);
        GUILIST_SetBgColor(ctrl_id, HANZI_WIN_BG_COLOR);
    }else{
        GUILIST_RemoveAllItems(ctrl_id);
    }
}

LOCAL void Hanzi_InitButtonBg(MMI_CTRL_ID_T ctrl_id)
{
	GUI_FONT_ALL_T font = {0};
	GUI_BG_T bg = {0};
	GUI_BORDER_T btn_border = {1, MMI_WHITE_COLOR, GUI_BORDER_ROUNDED};
	font.font = DP_FONT_18;
	font.color = HANZI_WIN_BG_COLOR;
	GUIBUTTON_SetBorder(ctrl_id, &btn_border, FALSE);
	bg.bg_type = GUI_BG_COLOR;
	bg.color = MMI_WHITE_COLOR;
	GUIBUTTON_SetBg(ctrl_id, &bg);
	GUIBUTTON_SetFont(ctrl_id, &font);
}

LOCAL void Hanzi_InitButton(MMI_CTRL_ID_T ctrl_id, GUI_RECT_T rect, MMI_TEXT_ID_T text_id, GUI_ALIGN_E text_align, BOOLEAN visable, GUIBUTTON_CALLBACK_FUNC func)
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

LOCAL void Hanzi_InitTextBox(MMI_CTRL_ID_T ctrl_id, GUI_RECT_T rect)
{
    GUI_BG_T bg = {0};
    GUI_FONT_ALL_T font = {0};
    
    GUITEXT_SetRect(ctrl_id, &rect);
    GUITEXT_SetAlign(ctrl_id, ALIGN_HVMIDDLE);
    GUITEXT_IsDisplayPrg(FALSE, ctrl_id);
    GUITEXT_SetHandleTpMsg(FALSE, ctrl_id);
    GUITEXT_SetClipboardEnabled(ctrl_id,FALSE);
    GUITEXT_IsSlide(ctrl_id,FALSE);
    
    font.font = DP_FONT_18;
    font.color = MMI_WHITE_COLOR;
    GUITEXT_SetFont(ctrl_id, &font.font,&font.color);
    
    bg.bg_type = GUI_BG_COLOR;
    bg.color = HANZI_WIN_BG_COLOR;
    GUITEXT_SetBg(ctrl_id, &bg);
}

LOCAL void Hanzi_SetDiretionText(MMI_CTRL_ID_T ctrl_id, int cur_idx, int total)
{
    char str[20] = {0};
    MMI_STRING_T text = {0};
    uint16 wstr[10] = {0};
    sprintf(str, "%d/%d", cur_idx + 1, total);

    MMI_STRNTOWSTR(wstr, strlen(str)+1, (uint8 *)str, strlen(str)+1, strlen(str)+1);
    text.wstr_ptr = wstr;
    text.wstr_len = MMIAPICOM_Wstrlen(wstr);
    GUILABEL_SetText(ctrl_id, &text, TRUE);
}

LOCAL void HanziPopupWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T text_style = {0};
    GUI_BORDER_T border  = {0};
    MMI_STRING_T str_left = {0};
    MMI_STRING_T str_right = {0};
    MMI_STRING_T tipstr = {0};
    GUI_RECT_T rect = hanzi_msg_tips_rect;
    GUI_RECT_T tips_left_rect = hanzi_msg_tips_left_rect;
    GUI_RECT_T tips_right_rect = hanzi_msg_tips_right_rect;

    border.width = 1;
    border.color = MMI_WHITE_COLOR;
    border.type =  GUI_BORDER_ROUNDED;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_16;
    text_style.font_color = HANZI_WIN_BG_COLOR;

    LCD_FillRoundedRect(&lcd_dev_info,rect,rect,MMI_WHITE_COLOR);
    
    GUI_DisplayBorder(rect,rect,&border,&lcd_dev_info);
    rect.bottom = tips_left_rect.top;
    MMI_GetLabelTextByLang(WORD_TIPS, &tipstr);

    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &rect,
        &rect,
        &tipstr,
        &text_style,
        GUISTR_STATE_ALIGN,
        GUISTR_TEXT_DIR_AUTO
    );

	
  
    border.color = HANZI_WIN_BG_COLOR;
    GUI_DisplayBorder(tips_left_rect,tips_left_rect,&border,&lcd_dev_info);
    MMI_GetLabelTextByLang(WORD_FALSE,&str_left);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &tips_left_rect,
        &tips_left_rect,
        &str_left,
        &text_style,
        GUISTR_STATE_ALIGN,
        GUISTR_TEXT_DIR_AUTO
    );

    GUI_DisplayBorder(tips_right_rect,tips_right_rect,&border,&lcd_dev_info);
    MMI_GetLabelTextByLang(WORD_TRUE,&str_right);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &tips_right_rect,
        &tips_right_rect,
        &str_right,
        &text_style,
        GUISTR_STATE_ALIGN,
        GUISTR_TEXT_DIR_AUTO
    );

}

LOCAL void HanziPopupWin_TP_PRESS_UP(MMI_WIN_ID_T win_id, GUI_POINT_T point)
{
    GUI_RECT_T tips_left_rect = hanzi_msg_tips_left_rect;
    GUI_RECT_T tips_right_rect = hanzi_msg_tips_right_rect;
    if(GUI_PointIsInRect(point, tips_left_rect))
    {
        MMK_CloseWin(win_id);
    }
    else if(GUI_PointIsInRect(point, tips_right_rect))
    {
        MMK_CloseWin(win_id);
        if(hanzi_learn_info != NULL)
        {
            uint8 i = 0;
            for(i = 0;i < hanzi_book_count && i < HANZI_PUBLISH_BOOK_MAX;i++)
            {
                if(hanzi_publish_info[i]->id == hanzi_learn_info->book_id){
                    hanzi_book_info.cur_book_idx = i;
                    MMI_CreateHanziChapterWin();
                    break;
                }
            }
        }
    }
}

LOCAL MMI_RESULT_E HandleHanziPopupWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:  
            {
                
            }
            break;
        case MSG_FULL_PAINT:
            {    
                HanziPopupWin_FULL_PAINT(win_id);
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
        case MSG_TP_PRESS_UP:
            {
                GUI_RECT_T tips_right_rect = hanzi_msg_tips_right_rect;
                GUI_POINT_T   point = {0};
                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);
                if(point.y > tips_right_rect.top){
                    HanziPopupWin_TP_PRESS_UP(win_id, point);
                }
            }
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

WINDOW_TABLE(MMI_HANZI_TIPS_TAB) = {
    WIN_FUNC((uint32)HandleHanziPopupWinMsg),
    WIN_ID(MMI_HANZI_MAIN_TIPS_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMI_CreateHanziTipsWin(void)
{
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = hanzi_msg_tips_rect;
    if(MMK_IsOpenWin(MMI_HANZI_MAIN_TIPS_WIN_ID)){
        MMK_CloseWin(MMI_HANZI_MAIN_TIPS_WIN_ID);
    }
    win_handle = MMK_CreateWin((uint32 *)MMI_HANZI_TIPS_TAB, PNULL);
    MMK_SetWinRect(win_handle,&rect);
}

LOCAL void Hanzi_DisplayBookList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint8 index = 0;
    uint8 num = 0;
    GUILIST_INIT_DATA_T list_init = {0};
    GUILIST_ITEM_T item_t = {0};
    GUIITEM_STATE_T item_state = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T text_wchar = {0};
    MMI_STRING_T text_str = {0};
    wchar name_wchar[50] = {0};
    wchar name_str[50] = {0};
    uint8 length = 0;
    MMI_STRING_T text_string = {0};
    char tmp[10]={0};
    uint16 wstr0[10] = {0};
    GUI_RECT_T list_rect = hanzi_list_rect;;

    Hanzi_InitListbox(win_id, ctrl_id, list_rect, HANZI_PUBLISH_BOOK_MAX);

    for(index = 0;index < hanzi_book_count && index < HANZI_PUBLISH_BOOK_MAX; index++)
    {
        item_t.item_style = GUIITEM_SYTLE_ZMT_BOOK_LIST_MS;
        item_t.item_data_ptr = &item_data;
        item_t.item_state = GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
		
        memset(name_wchar, 0, 50);
        memset(name_str, 0, 50);
        length = strlen(hanzi_publish_info[index]->book_name);
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = IMG_ZMT_CONTACT_ICON;

        itoa(index+1,tmp, 10);
        GUI_UTF8ToWstr(wstr0,10,tmp, strlen(tmp)+1);
        text_str.wstr_len = MMIAPICOM_Wstrlen(wstr0);
        text_str.wstr_ptr = wstr0;
        item_data.item_content[1].is_default =TRUE;
        item_data.item_content[1].font_color_id = MMITHEME_COLOR_LIGHT_BLUE;
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer = text_str;

        GUI_UTF8ToWstr(name_wchar, 50, hanzi_publish_info[index]->book_name, length);
        text_wchar.wstr_len = MMIAPICOM_Wstrlen(name_wchar);
        text_wchar.wstr_ptr = name_wchar;
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer = text_wchar;

        length = strlen(hanzi_publish_info[index]->publish_name);
        GUI_UTF8ToWstr(name_str, 50, hanzi_publish_info[index]->publish_name, length);
        text_str.wstr_len = MMIAPICOM_Wstrlen(name_str);
        text_str.wstr_ptr = name_str;
        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[3].item_data.text_buffer = text_str;

        GUILIST_AppendItem(ctrl_id, &item_t);
    }
     GUILIST_SetCurItemIndex(ctrl_id, hanzi_book_info.cur_book_idx);
}

LOCAL void HanziWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};

    MMIRES_GetText(HANZI_CARD, win_id, &text_string);
    Hanzi_DrawWinTitle(win_id, 0, text_string);

    if(hanzi_book_count == 0)
    {
        Hanzi_DrawWinTips(win_id, 0, WORD_LOADING);
    }
    else if(hanzi_book_count == -1)
    {
        Hanzi_DrawWinTips(win_id, 0, WORD_LOADING_FAILED);
    }
    else if(hanzi_book_count == -2)
    {
        Hanzi_DrawWinTips(win_id, 0, WORD_NO_DATA);
    }
    else
    {
        Hanzi_DisplayBookList(win_id, MMI_ZMT_HANZI_MAIN_LIST_CTRL_ID);
    }
}

LOCAL void HanziWin_CTL_PENOK(MMI_WIN_ID_T win_id)
{
    uint16 cur_idx = GUILIST_GetCurItemIndex(MMI_ZMT_HANZI_MAIN_LIST_CTRL_ID);
    hanzi_book_info.cur_book_idx = cur_idx;
    SCI_TRACE_LOW("%s: hanzi_book_info.cur_book_idx = %d", __FUNCTION__, hanzi_book_info.cur_book_idx);
    MMI_CreateHanziChapterWin();
}

LOCAL void HanziWin_CLOSE_WINDOW(void)
{
    hanzi_book_count = 0;
    memset(&hanzi_book_info, 0, sizeof(HANZI_BOOK_INFO_T));
    Hanzi_ReleaseBookInfo();
    Hanzi_ReleaseLearnInfo();
}

LOCAL MMI_RESULT_E HandleHanziWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                memset(&hanzi_book_info, 0, sizeof(HANZI_BOOK_INFO_T));
                Hanzi_requestBookInfo();
            }
            break;
        case MSG_FULL_PAINT:
            {
                HanziWin_FULL_PAINT(win_id);
                if(hanzi_book_count > 0 && Hanzi_LoadLearnInfo()){
                    MMI_CreateHanziTipsWin();
                }
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
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            { 
                HanziWin_CTL_PENOK(win_id);
            }
            break;
        case MSG_CLOSE_WINDOW:
            {
                HanziWin_CLOSE_WINDOW();
            }
            break;
         default:
            recode = MMI_RESULT_FALSE;
            break;
    }
      return recode;
}

WINDOW_TABLE(MMI_HANZI_WIN_TAB) = {
    WIN_ID(MMI_HANZI_MAIN_WIN_ID),
    WIN_FUNC((uint32)HandleHanziWinMsg),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC MMI_RESULT_E MMI_CloseHanziWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(MMI_HANZI_MAIN_WIN_ID)){
        MMK_CloseWin(MMI_HANZI_MAIN_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreateHanziWin(void)
{
    MMI_CloseHanziWin();
    MMK_CreateWin((uint32 *)MMI_HANZI_WIN_TAB, PNULL);
}

////////////////////////////////////////////////////
LOCAL void Hanzi_AutoDisplay_Tip_Show(uint8 type)
{
    UILAYER_APPEND_BLT_T append_layer = {0};	
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    wchar text_str[35] = {0};
    char count_str[35] = {0};
    GUI_RECT_T tip_rect = hanzi_tip_rect;

    append_layer.lcd_dev_info = hanzi_chapter_tip_layer;
    append_layer.layer_level = UILAYER_LEVEL_HIGH;
    UILAYER_AppendBltLayer(&append_layer);

    LCD_FillRoundedRect(&hanzi_chapter_tip_layer, tip_rect, tip_rect, MMI_WHITE_COLOR);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_18;
    text_style.font_color = HANZI_WIN_BG_COLOR;

    if(type==1)
    {
        sprintf(count_str,"已开启自动发音");
    }else
    {
        sprintf(count_str,"已关闭自动发音");
    }
    GUI_GBToWstr(text_str, count_str, strlen(count_str));
    text_string.wstr_ptr = text_str;
    text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&hanzi_chapter_tip_layer,
        &tip_rect,
        &tip_rect,
        &text_string,
        &text_style,
        GUISTR_STATE_ALIGN,
        GUISTR_TEXT_DIR_AUTO
    );
}

LOCAL void Hanzi_AutoDisplay_Tip_Timeout(uint8 timer_id,uint32 param)
{
    if(0 != open_auto_play_timer)
    {
        MMK_StopTimer(open_auto_play_timer);
        open_auto_play_timer = 0;
    }
    UILAYER_RemoveBltLayer(&hanzi_chapter_tip_layer);
}

LOCAL void Hanzi_AutoDisplay_Tip(uint8 type)
{
    if(0 != open_auto_play_timer)
    {
        MMK_StopTimer(open_auto_play_timer);
        open_auto_play_timer = 0;
    }
    Hanzi_AutoDisplay_Tip_Show(type);
    open_auto_play_timer = MMK_CreateTimerCallback(2000, Hanzi_AutoDisplay_Tip_Timeout, PNULL, FALSE);
    MMK_StartTimerCallback(open_auto_play_timer, 2000, Hanzi_AutoDisplay_Tip_Timeout, PNULL, FALSE);
    
}

LOCAL MMI_RESULT_E Hanzi_clickAutoPlay()
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	is_open_auto_play = TRUE;
	Hanzi_AutoDisplay_Tip(1);
	MMK_SendMsg(MMI_HANZI_CHAPTER_WIN_ID, MSG_FULL_PAINT, PNULL);
	return result;
}
LOCAL MMI_RESULT_E Hanzi_clickDisAutoPlay()
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	is_open_auto_play = FALSE;
	Hanzi_AutoDisplay_Tip(2);
	MMK_SendMsg(MMI_HANZI_CHAPTER_WIN_ID, MSG_FULL_PAINT, PNULL);
	return result;
}

LOCAL void Hanzi_OpenNormalHanzi(void)
{
    is_open_new_hanzi = FALSE;
    MMI_CreateHanziDetailWin();
}

LOCAL void Hanzi_OpenNewHanzi(void)
{
    is_open_new_hanzi = TRUE;
    cur_new_hanzi_page_idx = 0;
    MMI_CreateHanziDetailWin();
}

LOCAL void Hanzi_DisplayChapterList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint8 i,j = 0;
    uint8 index = 0;
    uint8 num = 0;
    uint16 total_count = 0;
    GUI_RECT_T list_rect = hanzi_list_rect;
    GUILIST_INIT_DATA_T list_init = {0};
    GUILIST_ITEM_T item_t = {0};
    GUIITEM_STATE_T item_state = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T text_str = {0};
    wchar name_wchar[100] = {0};
    char name_str[100] = {0};
    uint16 length = 0;
    MMI_STRING_T text_string = {0};

    list_rect.bottom -= 1.5*HANZI_CARD_LINE_HIGHT;
    for(i = 0;i < hanzi_chapter_count;i++){
        total_count += hanzi_chapter_children_count[i];
    }
    cur_chapter_total_count = total_count;
    Hanzi_InitListbox(win_id, ctrl_id, list_rect, cur_chapter_total_count);

    for(i = 0;i < hanzi_chapter_count;i++)
    {
        for(j = 0; j < hanzi_chapter_children_count[i]; j++)
        {
            length = strlen(hanzi_content_info[i]->chapter[j]->chapter_name);
    		
            item_t.item_style = GUIITEM_SYTLE_ZMT_UNIT_LIST_MS;
            item_t.item_data_ptr = &item_data;
            item_t.item_state = GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
    		
            memset(name_wchar, 0, 100);
            memset(name_str, 0, 100);

            GUI_UTF8ToWstr(name_wchar, 100, hanzi_content_info[i]->chapter[j]->chapter_name, length);
            text_str.wstr_ptr = name_wchar;
            text_str.wstr_len = MMIAPICOM_Wstrlen(text_str.wstr_ptr);
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer = text_str;
        
            item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
            if(hanzi_book_info.cur_section_idx == i && hanzi_book_info.cur_section_children_idx == j){
                item_data.item_content[1].item_data.image_id = IMG_ZMT_SELECTED;
            }else{
                item_data.item_content[1].item_data.image_id = IMG_ZMT_UNSELECTED;
            }
            GUILIST_AppendItem(ctrl_id, &item_t);
        }
    }
    GUILIST_SetTextFont(ctrl_id, DP_FONT_18, MMI_WHITE_COLOR);
    GUILIST_SetCurItemIndex(ctrl_id, hanzi_book_info.cur_chapter_idx);
}

LOCAL void HanziChapterWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUI_FONT_ALL_T font = {0};
    GUI_BORDER_T btn_border = {1, MMI_BLACK_COLOR, GUI_BORDER_SOLID};
    GUI_RECT_T title_rect = hanzi_title_rect;
    GUI_RECT_T auto_play_rect = hanzi_auto_play_rect;
    GUI_RECT_T dir_rect = hanzi_dir_rect;
    GUI_RECT_T left_rect = hanzi_left_rect;
    GUI_RECT_T right_rect = hanzi_right_rect;
    GUI_RECT_T tip_rect = hanzi_tip_rect;
		
    font.font = DP_FONT_20;
    font.color = MMI_WHITE_COLOR;
    GUIBUTTON_SetFont(MMI_ZMT_HANZI_CHAPTER_LABEL_BACK_CTRL_ID, &font);
    Hanzi_InitButton(MMI_ZMT_HANZI_CHAPTER_LABEL_BACK_CTRL_ID, title_rect, NULL, ALIGN_LVMIDDLE, TRUE, MMI_CloseHanziChapterWin);

    GUIBUTTON_SetRect(MMI_ZMT_HANZI_CHAPTER_AUTO_PLAY_CTRL_ID, &auto_play_rect);
    GUIBUTTON_SetVisible(MMI_ZMT_HANZI_CHAPTER_AUTO_PLAY_CTRL_ID,TRUE,TRUE);

    Hanzi_InitButton(MMI_ZMT_HANZI_CHAPTER_LEFT_CTRL_ID, left_rect, HANZI_PACTISE, ALIGN_HVMIDDLE, FALSE, Hanzi_OpenNormalHanzi);
    Hanzi_InitButtonBg(MMI_ZMT_HANZI_CHAPTER_LEFT_CTRL_ID);

    Hanzi_InitButton(MMI_ZMT_HANZI_CHAPTER_RIGHT_CTRL_ID, right_rect, HANZI_NEW_WORD, ALIGN_HVMIDDLE, FALSE, Hanzi_OpenNewHanzi);
    Hanzi_InitButtonBg(MMI_ZMT_HANZI_CHAPTER_RIGHT_CTRL_ID);

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = tip_rect.left;
        create_info.offset_y = tip_rect.top;
        create_info.width = tip_rect.right - tip_rect.left;
        create_info.height = tip_rect.bottom;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;   
        UILAYER_CreateLayer(&create_info, &hanzi_chapter_tip_layer);
    }
}

LOCAL void HanziChapterWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    char text_str[100] = {0};
    wchar text_wchar[100] = {0};

    sprintf(text_str, "%s", hanzi_publish_info[hanzi_book_info.cur_book_idx]->book_name);
    GUI_UTF8ToWstr(text_wchar, 100, text_str, strlen(text_str));
    text_string.wstr_ptr = text_wchar;
    text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);
    Hanzi_DrawWinTitle(win_id, MMI_ZMT_HANZI_CHAPTER_LABEL_BACK_CTRL_ID, text_string);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_20;
    text_style.font_color = MMI_WHITE_COLOR;
                
    SCI_TRACE_LOW("%s: hanzi_chapter_count = %d", __FUNCTION__, hanzi_chapter_count);
    if(hanzi_chapter_count == 0)
    {
        Hanzi_DrawWinTips(win_id, 0, WORD_LOADING);
    }
    else if(hanzi_chapter_count == -1)
    {
        Hanzi_DrawWinTips(win_id, 0, WORD_LOADING_FAILED);
    }
    else if(hanzi_chapter_count == -2)
    {
        Hanzi_DrawWinTips(win_id, 0, WORD_NO_DATA);
    }
    else
    {
        GUI_BG_T auto_play_bg = {0};
        if(is_open_auto_play)
        {
            auto_play_bg.bg_type = GUI_BG_IMG;
            auto_play_bg.img_id = IMG_AUTO_PLAY;
            GUIBUTTON_SetBg(MMI_ZMT_HANZI_CHAPTER_AUTO_PLAY_CTRL_ID, &auto_play_bg);
            GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_CHAPTER_AUTO_PLAY_CTRL_ID, Hanzi_clickDisAutoPlay);
        }
        else
        {
            auto_play_bg.bg_type = GUI_BG_IMG;
            auto_play_bg.img_id = IMG_DISAUTO_PLAY;
            GUIBUTTON_SetBg(MMI_ZMT_HANZI_CHAPTER_AUTO_PLAY_CTRL_ID, &auto_play_bg);
            GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_CHAPTER_AUTO_PLAY_CTRL_ID, Hanzi_clickAutoPlay);
        }
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_CHAPTER_LEFT_CTRL_ID,TRUE,FALSE);
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_CHAPTER_RIGHT_CTRL_ID,TRUE,FALSE);
        Hanzi_DisplayChapterList(win_id, MMI_ZMT_HANZI_CHAPTER_LIST_CTRL_ID);
    }
}

LOCAL void HanziChapterWin_CTL_PENOK(MMI_WIN_ID_T win_id)
{
    uint8 i,j = 0;
    uint8 m = 0;
    BOOLEAN is_get = FALSE;
    uint16 cur_idx = GUILIST_GetCurItemIndex(MMI_ZMT_HANZI_CHAPTER_LIST_CTRL_ID);
    SCI_TRACE_LOW("%s: cur_idx = %d", __FUNCTION__, cur_idx);
    if(cur_idx == hanzi_book_info.cur_chapter_idx)
    {
        is_open_new_hanzi = FALSE;
        Hanzi_OpenNormalHanzi();
    }
    else
    {
        hanzi_book_info.cur_chapter_idx = cur_idx;
        hanzi_book_info.cur_section_idx = 0;
        hanzi_book_info.cur_section_children_idx = 0;
        for(i = 0;i < hanzi_chapter_count;i++){
            for(j = 0;j < hanzi_chapter_children_count[i];j++){
                if(m == cur_idx){
                    hanzi_book_info.cur_section_idx = i;
                    hanzi_book_info.cur_section_children_idx = j;
                    is_get = TRUE;
                    break;
                }
                m++;
            }
            if(is_get){
                break;
            }
        }
        //SCI_TRACE_LOW("%s: cur_section_idx = %d, cur_section_children_idx = %d", 
        //    __FUNCTION__, hanzi_book_info.cur_section_idx, hanzi_book_info.cur_section_children_idx);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
}

LOCAL void HanziChapterWin_CLOSE_WINDOW(void)
{
    hanzi_book_info.cur_section_idx = 0;
    hanzi_book_info.cur_section_children_idx = 0;              
    Hanzi_ReleaseChapterInfo();
    hanzi_chapter_count = 0;
    memset(&hanzi_chapter_children_count, 0, sizeof(hanzi_chapter_children_count));   
}

LOCAL MMI_RESULT_E HandleHanziChapterWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                HanziChapterWin_OPEN_WINDOW(win_id);
                Hanzi_requestChapterInfo(hanzi_publish_info[hanzi_book_info.cur_book_idx]->id);
                Hanzi_UpdateLearnInfo(hanzi_publish_info[hanzi_book_info.cur_book_idx]->id, 0);
            }
            break;
        case MSG_FULL_PAINT:
            {
                HanziChapterWin_FULL_PAINT(win_id);
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
        case MSG_CTL_PENOK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            { 
                HanziChapterWin_CTL_PENOK(win_id);
            }
            break;
        case MSG_CLOSE_WINDOW:
            {
                HanziChapterWin_CLOSE_WINDOW();
                Hanzi_WriteLearnInfo();
            }
            break;
         default:
            recode = MMI_RESULT_FALSE;
            break;
    }
      return recode;
}

WINDOW_TABLE(MMI_HANZI_CHAPTER_WIN_TAB) = {
    WIN_ID(MMI_HANZI_CHAPTER_WIN_ID),
    WIN_FUNC((uint32)HandleHanziChapterWinMsg),
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_CHAPTER_LABEL_BACK_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_CHAPTER_AUTO_PLAY_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_CHAPTER_LEFT_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_CHAPTER_RIGHT_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_CloseHanziChapterWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(MMI_HANZI_CHAPTER_WIN_ID)){
        MMK_CloseWin(MMI_HANZI_CHAPTER_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreateHanziChapterWin(void)
{
    MMI_CloseHanziChapterWin();
    MMK_CreateWin((uint32 *)MMI_HANZI_CHAPTER_WIN_TAB, PNULL);
}

///////////////////////////////////////////////////////////////
LOCAL void Hanzi_StopPlayMp3Data(void)
{
    if(hanzi_player_handle != 0)
    {
        MMISRVAUD_Stop(hanzi_player_handle);
        MMISRVMGR_Free(hanzi_player_handle);
        hanzi_player_handle = 0;
    }
}

LOCAL BOOLEAN Hanzi_ChatPlayMp3DataNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;

    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == hanzi_player_handle)
        {
            switch(report_ptr->report)
                {
                    case MMISRVAUD_REPORT_END:  
                        {
                            Hanzi_StopPlayMp3Data();
                            //SCI_TRACE_LOW("%s: hanzi_listen_info.status = %d", __FUNCTION__, hanzi_listen_info.status);
                            if(MMI_IsOpenHanziListenWin() && hanzi_listen_info.status == HANZI_LISTEN_NOW)
                            {
                                HanziListenWin_CreateIntervalTimer();
                            }
							 if(MMIZDT_IsClassModeWinOpen() )
		{
	             Hanzi_StopPlayMp3Data();
	             MMI_CloseHanziListenWin();
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

LOCAL void Hanzi_ChatPlayMp3Data(uint8 *data,uint32 data_len)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    BOOLEAN result = FALSE;

    Hanzi_StopPlayMp3Data();

    req.is_auto_free = FALSE;
    req.notify = Hanzi_ChatPlayMp3DataNotify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.info.type = MMISRVAUD_TYPE_RING_BUF;
    audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MP3;
    audio_srv.info.ring_buf.data = data;
    audio_srv.info.ring_buf.data_len = data_len;
    audio_srv.volume=MMIAPISET_GetMultimVolume();
    
    SCI_TRACE_LOW("%s: audio_srv.volume=%d", __FUNCTION__, audio_srv.volume);

    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    hanzi_player_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
	
    if(hanzi_player_handle > 0)
    {
        SCI_TRACE_LOW("%s mp3_player_handle > 0", __FUNCTION__);
        result = MMISRVAUD_Play(hanzi_player_handle, 0);
        if(!result)
        {
            SCI_TRACE_LOW("%s chat_player error", __FUNCTION__);
            MMISRVMGR_Free(hanzi_player_handle);
            hanzi_player_handle = 0;
        }
        if(result == MMISRVAUD_RET_OK)
        {
            SCI_TRACE_LOW("%s chat_player success", __FUNCTION__);
        }
    }
    else
    {
        SCI_TRACE_LOW("%s mp3_player_handle <= 0", __FUNCTION__);
    }
}

LOCAL void HanziDetail_NextChapterInfo(void)
{
    
    if(hanzi_book_info.cur_section_children_idx < hanzi_chapter_children_count[hanzi_book_info.cur_section_idx])
    {
        Hanzi_WriteUnmasterHanzi(
            hanzi_publish_info[hanzi_book_info.cur_book_idx]->id,
            hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id, 
            cur_chapter_unmaster_count
        );
		hanzi_book_info.cur_section_children_idx++;
        cur_chapter_unmaster_count = 0;
        hanzi_detail_cur_idx = 0;
        hanzi_book_info.cur_chapter_idx++;
        Hanzi_requestDetailInfo(
            hanzi_book_info.cur_book_idx+1,
            hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id
        );
    }
    else
    {
        hanzi_book_info.cur_section_idx++;
        if(hanzi_book_info.cur_section_idx < hanzi_chapter_count)
        {
            Hanzi_WriteUnmasterHanzi(
                hanzi_publish_info[hanzi_book_info.cur_book_idx]->id,
                hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id, 
                cur_chapter_unmaster_count
            );
            cur_chapter_unmaster_count = 0;
            hanzi_detail_cur_idx = 0;
            hanzi_book_info.cur_section_children_idx = 0;
           hanzi_book_info.cur_chapter_idx++;
            Hanzi_requestDetailInfo(
            hanzi_book_info.cur_book_idx+1,
            hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id
            );
        }
        else
        {

            //书本已经学完
        }
    }
}

LOCAL void HanziDetail_GoToListenWord(void)
{
    if(!is_open_new_hanzi){
        Hanzi_WriteUnmasterHanzi(
            hanzi_publish_info[hanzi_book_info.cur_book_idx]->id,
            hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id, 
            cur_chapter_unmaster_count
        );
        cur_chapter_unmaster_count = 0;
    }
    MMI_CreateHanziListenWin();
}

PUBLIC void HanziDetail_TipTimeout(uint8 timer_id,uint32 param)
{
    if(0 != hanzi_tip_timer)
    {
        MMK_StopTimer(hanzi_tip_timer);
        hanzi_tip_timer = 0;
    }
    hanzi_is_display_tip = 0;
    HanziDetail_ShowTip();
}

LOCAL void HanziDetail_DisplayTip(uint type)
{
    if(0 != hanzi_tip_timer)
    {
        MMK_StopTimer(hanzi_tip_timer);
        hanzi_tip_timer = 0;
    }
    hanzi_is_display_tip = type;
    HanziDetail_ShowTip();
    hanzi_tip_timer = MMK_CreateTimerCallback(2000, HanziDetail_TipTimeout,(uint32)0, FALSE);
    MMK_StartTimerCallback(hanzi_tip_timer, 2000, HanziDetail_TipTimeout, (uint32)0, FALSE);
}

PUBLIC void HanziDetail_PlayPinyinAudio(void)
{
    if(hanzi_detail_info[hanzi_detail_cur_idx] == NULL){
        SCI_TRACE_LOW("%s: empty detail info!!", __FUNCTION__);
        return;
    }
    if(hanzi_detail_info[hanzi_detail_cur_idx]->audio_len == 0)
    {
        char file_path[30] = {0};
        sprintf(file_path, HANZI_CARD_WORD_AUDIO_PATH, hanzi_book_info.cur_book_idx+1, hanzi_detail_info[hanzi_detail_cur_idx]->pingy);
        if(zmt_file_exist(file_path)){
            hanzi_detail_info[hanzi_detail_cur_idx]->audio_data = zmt_file_data_read(file_path, &hanzi_detail_info[hanzi_detail_cur_idx]->audio_len);
            HanziDetail_PlayPinyinAudio();
        }else{
            if(hanzi_detail_info[hanzi_detail_cur_idx]->audio_uri != NULL){
                //SCI_TRACE_LOW("%s: [%d]audio_uri = %s", __FUNCTION__, hanzi_detail_cur_idx, hanzi_detail_info[hanzi_detail_cur_idx]->audio_uri);    
                MMIZDT_HTTP_AppSend(TRUE, hanzi_detail_info[hanzi_detail_cur_idx]->audio_uri, PNULL, 0, 1000, 0, 0, 6000, 0, 0, Hanzi_ParseMp3Response);
            }
        }
        //HanziDetail_DisplayTip(1);
    }
    else if(hanzi_detail_info[hanzi_detail_cur_idx]->audio_len == -1)
    {
        //没有音频uri
        HanziDetail_DisplayTip(2);
        HanziListenWin_PlayAudioFail();
    }
    else if(hanzi_detail_info[hanzi_detail_cur_idx]->audio_len == -2)
    {
        //加载音频失败
        HanziDetail_DisplayTip(3);
        HanziListenWin_PlayAudioFail();
    }
    else
    {
        //加载成功，开始播放
        if(hanzi_detail_info[hanzi_detail_cur_idx]->audio_data != NULL){
            Hanzi_ChatPlayMp3Data(hanzi_detail_info[hanzi_detail_cur_idx]->audio_data ,hanzi_detail_info[hanzi_detail_cur_idx]->audio_len);
        }
    }
}

LOCAL void HanziDetail_LeftDetail(void)//已掌握/上一个
{
    if(!is_open_new_hanzi){
        hanzi_detail_cur_idx++;
        if(is_open_auto_play){
            HanziDetail_PlayPinyinAudio();
        }
    }else{
        if(hanzi_detail_cur_idx == 0){
            return;
        }else{
            hanzi_detail_cur_idx--;
            if(is_open_auto_play){
                HanziDetail_PlayPinyinAudio();
            }
        }
    }
    MMK_PostMsg(MMI_HANZI_DETAIL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}

LOCAL void HanziDetail_RightDetail(void)//未掌握/下一个
{
    if(!is_open_new_hanzi){
        cur_chapter_unmaster_idx[cur_chapter_unmaster_count] = hanzi_detail_cur_idx + 1;
        cur_chapter_unmaster_count++;
        hanzi_detail_cur_idx++;
        if(is_open_auto_play){
            HanziDetail_PlayPinyinAudio();
        }
        MMK_PostMsg(MMI_HANZI_DETAIL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
    }else{
        uint16 cur_idx = cur_new_hanzi_page_idx * HANZI_CHAPTER_WORD_MAX + hanzi_detail_cur_idx;
        if(cur_idx + 1 == hanzi_detail_count){
            hanzi_detail_cur_idx++;
            MMK_PostMsg(MMI_HANZI_DETAIL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }else{
            hanzi_detail_cur_idx++;
            if(hanzi_detail_cur_idx >= HANZI_CHAPTER_WORD_MAX){
                hanzi_detail_cur_idx = 0;
                cur_new_hanzi_page_idx++;
                Hanzi_RequestNewWord(
                    hanzi_publish_info[hanzi_book_info.cur_book_idx]->id,
                    hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id
                );
            }
            else
            {
                if(is_open_auto_play){
                    HanziDetail_PlayPinyinAudio();
                }
                MMK_PostMsg(MMI_HANZI_DETAIL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
            }
        }
    }
}

LOCAL void HanziDetail_DeleteNewHanzi(void)
{
    uint16 idx = cur_new_hanzi_page_idx * HANZI_CHAPTER_WORD_MAX + hanzi_detail_cur_idx;
    new_hanzi_haved_delete = TRUE;
    HanziDetail_DisplayTip(4);
    Hanzi_DeleteOneNewWord(idx, hanzi_detail_count);
}

LOCAL void HanziDetail_ShowTip(void)
{
    if(hanzi_is_display_tip != 0)
    {
        UILAYER_APPEND_BLT_T append_layer = {0};
        GUISTR_STYLE_T text_style = {0};
        MMI_STRING_T text_string = {0};
        wchar text_str[35] = {0};
        char count_str[35] = {0};
        GUI_RECT_T msg_rect = hanzi_msg_rect;

        append_layer.lcd_dev_info = hanzi_detail_tip_layer;
        append_layer.layer_level = UILAYER_LEVEL_HIGH;
        UILAYER_AppendBltLayer(&append_layer);

        LCD_FillRoundedRect(&hanzi_detail_tip_layer, msg_rect, msg_rect, MMI_WHITE_COLOR);

        text_style.align = ALIGN_HVMIDDLE;
        text_style.font = DP_FONT_18;
        text_style.font_color = HANZI_WIN_BG_COLOR;

        if(hanzi_is_display_tip == 1)
        {
            sprintf(count_str,"正在加载，请稍后");
        }else if(hanzi_is_display_tip == 2)
        {
            sprintf(count_str,"暂无音频");
        }else if(hanzi_is_display_tip == 3)
        {
            sprintf(count_str,"音频加载失败，请重试");
        }else if(hanzi_is_display_tip == 4)
        {
            sprintf(count_str,"正在删除，请稍等");
        }
        GUI_GBToWstr(text_str, count_str, strlen(count_str));
        text_string.wstr_ptr = text_str;
        text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&hanzi_detail_tip_layer,
            &msg_rect,
            &msg_rect,
            &text_string,
            &text_style,
            GUISTR_STATE_ALIGN,
            GUISTR_TEXT_DIR_AUTO
        );
    }
    else
    {
        UILAYER_RemoveBltLayer(&hanzi_detail_tip_layer);
    }
}

LOCAL void HanziDetail_DisplayDtailInfo(
    MMI_WIN_ID_T win_id, 
    MMI_CTRL_ID_T ctrl_label_hanzi, 
    MMI_CTRL_ID_T ctrl_label_pinyin, 
    MMI_CTRL_ID_T ctrl_button_audio, 
    MMI_CTRL_ID_T ctrl_text_info)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_FONT_T text_font = DP_FONT_18;
    GUI_COLOR_T text_color = MMI_WHITE_COLOR;
    
    MMI_STRING_T text_hanzi = {0};
    MMI_STRING_T text_pinyin = {0};
    MMI_STRING_T text_remark = {0};
    wchar wstr_hanzi[10] = {0};
    wchar wstr_pinyin[30] = {0};
    wchar wstr_remark[2048] = {0};
    uint16 size = 0;
    uint16 text_width_piex = 0;
    GUI_RECT_T hanzi_rect = hanzi_hanzi_rect;
    GUI_RECT_T line_rect = hanzi_hanzi_rect;
    GUI_RECT_T pinyin_rect = {0};
    GUI_RECT_T audio_rect = {0};
    GUI_RECT_T pinyin_audio_rect = hanzi_pinyin_audio_rect;
    GUI_RECT_T text_rect = hanzi_text_rect;

    size = strlen(hanzi_detail_info[hanzi_detail_cur_idx]->word);
    GUI_UTF8ToWstr(wstr_hanzi, 10, hanzi_detail_info[hanzi_detail_cur_idx]->word, size); 
    text_hanzi.wstr_ptr = wstr_hanzi;
    text_hanzi.wstr_len = MMIAPICOM_Wstrlen(text_hanzi.wstr_ptr);
    text_width_piex = GUI_CalculateStringPiexlNum(text_hanzi.wstr_ptr, text_hanzi.wstr_len, DP_FONT_22, 1);
    hanzi_rect.right = hanzi_rect.left + text_width_piex + 10;
    GUILABEL_SetRect(ctrl_label_hanzi, &hanzi_rect, FALSE);
    GUILABEL_SetText(ctrl_label_hanzi, &text_hanzi, FALSE);
    
    size = strlen(hanzi_detail_info[hanzi_detail_cur_idx]->pingy);
    GUI_UTF8ToWstr(wstr_pinyin, 30, hanzi_detail_info[hanzi_detail_cur_idx]->pingy, size);
    text_pinyin.wstr_ptr = wstr_pinyin;
    text_pinyin.wstr_len = MMIAPICOM_Wstrlen(text_pinyin.wstr_ptr);
    text_width_piex = GUI_CalculateStringPiexlNum(text_pinyin.wstr_ptr, text_pinyin.wstr_len, DP_FONT_22, 1);
    pinyin_rect = hanzi_rect;
    pinyin_rect.left = hanzi_rect.right;
    pinyin_rect.right = pinyin_rect.left + text_width_piex + 10;
    GUILABEL_SetRect(ctrl_label_pinyin, &pinyin_rect, FALSE);
    GUILABEL_SetText(ctrl_label_pinyin, &text_pinyin, FALSE);

    GUIBUTTON_SetVisible(ctrl_button_audio,TRUE,TRUE);
    GUIBUTTON_SetRect(ctrl_button_audio, &pinyin_audio_rect);

    LCD_DrawHLine(&lcd_dev_info, line_rect.left, line_rect.bottom, line_rect.right, MMI_WHITE_COLOR);

    size = strlen(hanzi_detail_info[hanzi_detail_cur_idx]->remark); 
    GUI_UTF8ToWstr(wstr_remark, 2048, hanzi_detail_info[hanzi_detail_cur_idx]->remark, size);
    text_remark.wstr_ptr = wstr_remark;
    text_remark.wstr_len = MMIAPICOM_Wstrlen(text_remark.wstr_ptr);
    GUITEXT_SetRect(ctrl_text_info, &text_rect);
    GUITEXT_SetAlign(ctrl_text_info, ALIGN_LEFT);
    GUITEXT_SetString(ctrl_text_info, text_remark.wstr_ptr,text_remark.wstr_len, FALSE);
    GUITEXT_SetResetTopDisplay(ctrl_text_info, 0);
}

LOCAL void HanziDetailWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUI_FONT_ALL_T font = {0};
    GUI_BORDER_T btn_border = {1, MMI_BLACK_COLOR, GUI_BORDER_SOLID};
    GUI_FONT_T text_font = DP_FONT_20;
    GUI_COLOR_T text_color = MMI_WHITE_COLOR;
    GUI_BG_T bg = {0};
    GUI_RECT_T title_rect = hanzi_title_rect;
    GUI_RECT_T text_rect = hanzi_text_rect;
    GUI_RECT_T msg_rect = hanzi_msg_rect;
    GUI_RECT_T dir_rect = hanzi_dir_rect;
    GUI_RECT_T left_rect = hanzi_left_rect;
    GUI_RECT_T right_rect = hanzi_right_rect;
    GUI_RECT_T pre_rect = hanzi_pre_rect;
    GUI_RECT_T next_rect = hanzi_next_rect;
    GUI_RECT_T del_rect = hanzi_del_rect;
		
    font.font = DP_FONT_20;
    font.color = MMI_WHITE_COLOR;
    GUIBUTTON_SetFont(MMI_ZMT_HANZI_DETAIL_LABEL_BACK_CTRL_ID, &font);
    Hanzi_InitButton(MMI_ZMT_HANZI_DETAIL_LABEL_BACK_CTRL_ID, title_rect, NULL, ALIGN_LVMIDDLE, TRUE, MMI_CloseHanziDetailWin);

    Hanzi_InitTextBox(MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID, text_rect);
    
    bg.bg_type = GUI_BG_COLOR;
    bg.color = HANZI_WIN_BG_COLOR;
    GUITEXT_SetBg(MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID, &bg);

    Hanzi_InitButton(MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID, msg_rect, NULL, ALIGN_HVMIDDLE, FALSE, NULL);
    Hanzi_InitButtonBg(MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID);

    GUIBUTTON_SetVisible(MMI_ZMT_HANZI_DETAIL_DELETE_CTRL_ID,FALSE,FALSE);
    GUIBUTTON_SetVisible(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID,FALSE,FALSE);
    GUIBUTTON_SetVisible(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID,FALSE,FALSE);

    GUILABEL_SetRect(MMI_ZMT_HANZI_DETAIL_LABEL_NUM_CTRL_ID, &dir_rect, FALSE);
    GUILABEL_SetFont(MMI_ZMT_HANZI_DETAIL_LABEL_NUM_CTRL_ID, DP_FONT_18,MMI_WHITE_COLOR);
    GUILABEL_SetAlign(MMI_ZMT_HANZI_DETAIL_LABEL_NUM_CTRL_ID, GUILABEL_ALIGN_MIDDLE);

    GUILABEL_SetFont(MMI_ZMT_HANZI_DETAIL_LABEL_HANZI_CTRL_ID, DP_FONT_22,MMI_WHITE_COLOR);
    GUILABEL_SetFont(MMI_ZMT_HANZI_DETAIL_LABEL_PINYIN_CTRL_ID, DP_FONT_22,MMI_WHITE_COLOR);

    new_hanzi_haved_delete = FALSE;
    memset(cur_chapter_unmaster_idx, 0, sizeof(cur_chapter_unmaster_idx));
    if(is_open_new_hanzi){
        Hanzi_RequestNewWord(
            hanzi_publish_info[hanzi_book_info.cur_book_idx]->id,
            hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id
        );
        Hanzi_InitButton(MMI_ZMT_HANZI_DETAIL_DELETE_CTRL_ID, del_rect, NULL, ALIGN_HVMIDDLE, FALSE, HanziDetail_DeleteNewHanzi);
        Hanzi_InitButton(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, pre_rect, NULL, ALIGN_HVMIDDLE, FALSE, HanziDetail_LeftDetail);
        Hanzi_InitButton(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, next_rect, NULL, ALIGN_HVMIDDLE, FALSE, HanziDetail_RightDetail);
    }else{
        Hanzi_requestDetailInfo(
            hanzi_book_info.cur_book_idx+1,
            hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id
        );                     
        Hanzi_InitButton(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, left_rect, NULL, ALIGN_HVMIDDLE, FALSE, HanziDetail_LeftDetail);
        Hanzi_InitButton(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, right_rect, NULL, ALIGN_HVMIDDLE, FALSE, HanziDetail_RightDetail);
        Hanzi_InitButtonBg(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID);
        Hanzi_InitButtonBg(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID);
    }
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = msg_rect.left;
        create_info.offset_y = msg_rect.top;
        create_info.width = msg_rect.right - msg_rect.left;
        create_info.height = msg_rect.bottom;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(&create_info, &hanzi_detail_tip_layer);
    }
}

LOCAL void HanziDetailWin_NewHanziFullPaint(MMI_WIN_ID_T win_id, BOOLEAN is_end)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    if(!is_end)
    {
        GUI_BG_T but_bg = {0};
        uint16 cur_idx = cur_new_hanzi_page_idx * HANZI_CHAPTER_WORD_MAX + hanzi_detail_cur_idx;
        but_bg.bg_type = GUI_BG_IMG;
        if(cur_idx > 0){
            but_bg.img_id = WORD_PRE_ICON;
            GUIBUTTON_SetBg(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, &but_bg);
            GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, HanziDetail_LeftDetail);
        }else{
            GUIBUTTON_SetVisible(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID,FALSE,FALSE);
        }
        but_bg.img_id = WORD_NEXT_ICON;
        GUIBUTTON_SetBg(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, &but_bg);                          
        GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, HanziDetail_RightDetail);
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_DETAIL_DELETE_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_DELETE_CTRL_ID, HanziDetail_DeleteNewHanzi);
        Hanzi_SetDiretionText(MMI_ZMT_HANZI_DETAIL_LABEL_NUM_CTRL_ID, cur_new_hanzi_page_idx * HANZI_CHAPTER_WORD_MAX + hanzi_detail_cur_idx, hanzi_detail_count);
        HanziDetail_DisplayDtailInfo(win_id,
            MMI_ZMT_HANZI_DETAIL_LABEL_HANZI_CTRL_ID, 
            MMI_ZMT_HANZI_DETAIL_LABEL_PINYIN_CTRL_ID,
            MMI_ZMT_HANZI_DETAIL_BUTTON_AUDIO_CTRL_ID,
            MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID);
    }
    else
    {
        GUI_RECT_T hanzi_rect = hanzi_hanzi_rect;
        GUI_RECT_T left_rect = hanzi_left_rect;
        GUI_RECT_T right_rect = hanzi_right_rect;
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_DETAIL_DELETE_CTRL_ID,FALSE,FALSE);
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetTextId(MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID, NEW_WORD_BOOK_FINISH);
        GUIBUTTON_SetRect(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, &left_rect);
        Hanzi_InitButtonBg(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID);
        GUIBUTTON_SetTextId(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, WORD_TO_LISTENING);
        GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, HanziDetail_GoToListenWord);
        GUIBUTTON_SetRect(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, &right_rect);
        GUIBUTTON_SetTextAlign(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID,ALIGN_HVMIDDLE);
        Hanzi_InitButtonBg(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID);
        GUIBUTTON_SetTextId(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, WORD_BACK_CH);            
        GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, MMI_CloseHanziDetailWin);
        LCD_DrawHLine(&lcd_dev_info, hanzi_rect.left, hanzi_rect.bottom, hanzi_rect.right, MMI_WHITE_COLOR);
    }
}

LOCAL void HanziDetailWin_NormalHanziFullPaint(MMI_WIN_ID_T win_id, BOOLEAN is_end)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    if(!is_end)
    {
        GUIBUTTON_SetTextId(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, HANZI_MASTERED);
        GUIBUTTON_SetTextId(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, HANZI_UNMASTERED);
        GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, HanziDetail_LeftDetail);
        GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, HanziDetail_RightDetail);
        Hanzi_SetDiretionText(MMI_ZMT_HANZI_DETAIL_LABEL_NUM_CTRL_ID, hanzi_detail_cur_idx, hanzi_detail_count);
        HanziDetail_DisplayDtailInfo(win_id,
            MMI_ZMT_HANZI_DETAIL_LABEL_HANZI_CTRL_ID, 
            MMI_ZMT_HANZI_DETAIL_LABEL_PINYIN_CTRL_ID,
            MMI_ZMT_HANZI_DETAIL_BUTTON_AUDIO_CTRL_ID,
            MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID);
    }
    else
    {
        GUI_RECT_T hanzi_rect = hanzi_hanzi_rect;
        if(hanzi_book_info.cur_chapter_idx + 1 < cur_chapter_total_count)
        {
            GUIBUTTON_SetVisible(MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID, TRUE, TRUE);
            GUIBUTTON_SetTextId(MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID, WORD_FINISH);
            GUIBUTTON_SetTextId(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, WORD_TO_LISTENING);
            GUIBUTTON_SetTextId(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, HANZI_NEXT_CHAPTER);
            GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, HanziDetail_GoToListenWord);
            GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, HanziDetail_NextChapterInfo);
        }
        else
        {
            GUIBUTTON_SetVisible(MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID, TRUE, TRUE);
            GUIBUTTON_SetTextId(MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID, WORD_BOOK_FINISH);
            GUIBUTTON_SetTextId(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, WORD_TO_LISTENING);
            GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID, HanziDetail_GoToListenWord);
            GUIBUTTON_SetTextId(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, WORD_BACK_CH);         
            GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID, MMI_CloseHanziDetailWin);
        }
        LCD_DrawHLine(&lcd_dev_info, hanzi_rect.left, hanzi_rect.bottom, hanzi_rect.right, MMI_WHITE_COLOR);
    }
}

LOCAL void HanziDetailWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    char text_str[100] = {0};
    wchar text_wchar[100] = {0};
    BOOLEAN is_end = FALSE;

    GUIBUTTON_SetVisible(MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID, FALSE, FALSE);
    if(is_open_new_hanzi){
        MMIRES_GetText(HANZI_NEW_WORD, win_id, &text_string);
    }else{
        sprintf(text_str, "%s", hanzi_content_info[hanzi_book_info.cur_section_idx]->chapter[hanzi_book_info.cur_section_children_idx]->chapter_name);
        GUI_UTF8ToWstr(text_wchar, 100, text_str, strlen(text_str));
        text_string.wstr_ptr = text_wchar;
        text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);                  
    }
    Hanzi_DrawWinTitle(win_id, MMI_ZMT_HANZI_DETAIL_LABEL_BACK_CTRL_ID, text_string);

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_20;
    text_style.font_color = MMI_WHITE_COLOR;

    //SCI_TRACE_LOW("%s: hanzi_detail_count = %d", __FUNCTION__, hanzi_detail_count);
    if(hanzi_detail_count <= 0){
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_DETAIL_BUTTON_AUDIO_CTRL_ID, FALSE, FALSE);
    }
    if(hanzi_detail_count == 0)
    {
        Hanzi_DrawWinTips(win_id, MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID, WORD_LOADING);
    }
    else if(hanzi_detail_count == -1)
    {
        Hanzi_DrawWinTips(win_id, MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID, WORD_LOADING_FAILED);
    }
    else if(hanzi_detail_count == -2)
    {
        Hanzi_DrawWinTips(win_id, MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID, WORD_NO_DATA);
    }
    else if(hanzi_detail_count == -3)
    {
        Hanzi_DrawWinTips(win_id, MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID, HANZI_NO_UNMASTER);
    }
    else
    {
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID,TRUE,FALSE);
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID,TRUE,FALSE);
        GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_DETAIL_BUTTON_AUDIO_CTRL_ID, HanziDetail_PlayPinyinAudio);
        if(!is_open_new_hanzi){
            SCI_TRACE_LOW("%s: hanzi_detail_cur_idx = %d, hanzi_detail_count = %d", __FUNCTION__, hanzi_detail_cur_idx, hanzi_detail_count);
            if(hanzi_detail_cur_idx + 1 > hanzi_detail_count){
                is_end = TRUE;
            }
        }else{
            uint16 cur_idx = cur_new_hanzi_page_idx * HANZI_CHAPTER_WORD_MAX + hanzi_detail_cur_idx;
            SCI_TRACE_LOW("%s: cur_idx = %d, hanzi_detail_count = %d", __FUNCTION__, cur_idx, hanzi_detail_count);
            if(cur_idx + 1 > hanzi_detail_count){
                is_end = TRUE;
            }
        }
        if(is_open_new_hanzi){
            HanziDetailWin_NewHanziFullPaint(win_id, is_end);
        }else{
            HanziDetailWin_NormalHanziFullPaint(win_id, is_end);
        }
    }
}

LOCAL void HanziDetailWin_TP_PRESS_UP(MMI_WIN_ID_T win_id, DPARAM param)
{
    int16 tp_offset_x = 0;
    int16 tp_offset_y = 0;
    GUI_POINT_T point = {0};
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    tp_offset_x = point.x - main_tp_down_x;
    tp_offset_y = point.y - main_tp_down_y;
    if(ABS(tp_offset_x) <= ABS(tp_offset_y))
    {
        if(tp_offset_y > 40)
        {
            MMK_PostMsg(MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID, MSG_KEYREPEAT_UP, PNULL, 0);
        }
        else if(tp_offset_y < -40)
        {
            MMK_PostMsg(MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID, MSG_KEYREPEAT_DOWN, PNULL, 0);
        }
    }
}

LOCAL void HanziDetailWin_CLOSE_WINDOW(void)
{
    if(is_open_new_hanzi && new_hanzi_haved_delete)
    {
        Hanzi_SaveDeleteNewWord(
            hanzi_publish_info[hanzi_book_info.cur_book_idx]->id,
            hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id
        );
    }
    else if(!is_open_new_hanzi && cur_chapter_unmaster_count > 0)
    {
        Hanzi_WriteUnmasterHanzi(
            hanzi_publish_info[hanzi_book_info.cur_book_idx]->id,
            hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id, 
            cur_chapter_unmaster_count
        );
    }
    Hanzi_StopPlayMp3Data();
    Hanzi_ReleaseDetailInfo();
    new_hanzi_haved_delete = FALSE;
    hanzi_detail_cur_idx = 0;
    hanzi_detail_count = 0;
    cur_chapter_unmaster_count = 0;
    cur_new_hanzi_page_idx = 0;
    memset(cur_chapter_unmaster_idx, 0, sizeof(cur_chapter_unmaster_idx));
}

LOCAL MMI_RESULT_E HandleHanziDetailWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                HanziDetailWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {
                HanziDetailWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_TP_PRESS_UP:
            {
                HanziDetailWin_TP_PRESS_UP(win_id, param);
            }
            break;
        case MSG_TP_PRESS_DOWN:
            {
                main_tp_down_x = MMK_GET_TP_X(param);
                main_tp_down_y = MMK_GET_TP_Y(param);
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
                HanziDetailWin_CLOSE_WINDOW();
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(MMI_HANZI_DETAIL_WIN_TAB) = {
    WIN_ID(MMI_HANZI_DETAIL_WIN_ID),
    WIN_FUNC((uint32)HandleHanziDetailWinMsg),
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_DETAIL_LABEL_BACK_CTRL_ID),//单元名称
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMI_ZMT_HANZI_DETAIL_LABEL_NUM_CTRL_ID),//导航页
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMI_ZMT_HANZI_DETAIL_LABEL_HANZI_CTRL_ID),//汉字
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMI_ZMT_HANZI_DETAIL_LABEL_PINYIN_CTRL_ID),//拼音
    CREATE_BUTTON_CTRL(ZMT_LISTEN_VOLUME, MMI_ZMT_HANZI_DETAIL_BUTTON_AUDIO_CTRL_ID),//音频图标
    CREATE_TEXT_CTRL(MMI_ZMT_HANZI_DETAIL_TEXT_INFO_CTRL_ID),//释义
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_DETAIL_LEFT_CTRL_ID),//已掌握/上一个
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_DETAIL_RIGHT_CTRL_ID),//未掌握/下一个
    CREATE_BUTTON_CTRL(WORD_DELETE_ICON, MMI_ZMT_HANZI_DETAIL_DELETE_CTRL_ID),//删除
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_MSG_TIPS_CTRL_ID),//已学完tip
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC MMI_RESULT_E MMI_CloseHanziDetailWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(MMI_HANZI_DETAIL_WIN_ID)){
        MMK_CloseWin(MMI_HANZI_DETAIL_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreateHanziDetailWin(void)
{
    MMK_CreateWin((uint32 *)MMI_HANZI_DETAIL_WIN_TAB, PNULL);
}
//////////////////////////////////////////////////////////////////////////////
LOCAL void HanziListenInfoWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUI_FONT_ALL_T font = {0};
    GUI_BORDER_T btn_border = {1, MMI_BLACK_COLOR, GUI_BORDER_SOLID};
    GUI_FONT_T text_font = DP_FONT_20;
    GUI_COLOR_T text_color = MMI_WHITE_COLOR;
    GUI_BG_T bg = {0};
    GUI_RECT_T title_rect = hanzi_title_rect;
    GUI_RECT_T text_rect = hanzi_text_rect;
    GUI_RECT_T dir_rect = hanzi_dir_rect;
    GUI_RECT_T bottom_rect = hanzi_bottom_rect;
		
    font.font = DP_FONT_20;
    font.color = MMI_WHITE_COLOR; 
    GUIBUTTON_SetFont(MMI_ZMT_HANZI_LISTEN_INFO_LABEL_BACK_CTRL_ID, &font);
    Hanzi_InitButton(MMI_ZMT_HANZI_LISTEN_INFO_LABEL_BACK_CTRL_ID, title_rect, NULL, ALIGN_LVMIDDLE, TRUE, MMI_CloseHanziListenInfoWin);

    Hanzi_InitTextBox(MMI_ZMT_HANZI_LISTEN_INFO_TEXT_INFO_CTRL_ID, text_rect);

    GUILABEL_SetRect(MMI_ZMT_HANZI_LISTEN_INFO_LABEL_NUM_CTRL_ID, &dir_rect, FALSE);
    GUILABEL_SetFont(MMI_ZMT_HANZI_LISTEN_INFO_LABEL_NUM_CTRL_ID, DP_FONT_18,MMI_WHITE_COLOR);
    GUILABEL_SetAlign(MMI_ZMT_HANZI_LISTEN_INFO_LABEL_NUM_CTRL_ID, GUILABEL_ALIGN_MIDDLE);

    GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_LISTEN_INFO_BUTTON_AUDIO_CTRL_ID, HanziDetail_PlayPinyinAudio);
    GUILABEL_SetFont(MMI_ZMT_HANZI_LISTEN_INFO_LABEL_HANZI_CTRL_ID, DP_FONT_20,MMI_WHITE_COLOR);
    GUILABEL_SetFont(MMI_ZMT_HANZI_LISTEN_INFO_LABEL_PINYIN_CTRL_ID, DP_FONT_20,MMI_WHITE_COLOR);

    Hanzi_InitButton(MMI_ZMT_HANZI_LISTEN_INFO_BUTTON_STATUS_CTRL_ID, bottom_rect, WORD_LEARN_LISTENING_BACK, ALIGN_HVMIDDLE, TRUE, MMI_CloseHanziListenInfoWin);
    Hanzi_InitButtonBg(MMI_ZMT_HANZI_LISTEN_INFO_BUTTON_STATUS_CTRL_ID);
}

LOCAL void HanziListenInfoWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    char text_str[100] = {0};
    wchar text_wchar[100] = {0};
    BOOLEAN is_end = FALSE;

    if(is_open_new_hanzi){
        MMIRES_GetText(HANZI_PACTISE, win_id, &text_string);
    }else{
        strcpy(text_str, hanzi_content_info[hanzi_book_info.cur_section_idx]->chapter[hanzi_book_info.cur_section_children_idx]->chapter_name);
        GUI_UTF8ToWstr(text_wchar, 100, text_str, strlen(text_str));
        text_string.wstr_ptr = text_wchar;
        text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);                  
    }
    Hanzi_DrawWinTitle(win_id, MMI_ZMT_HANZI_LISTEN_INFO_LABEL_BACK_CTRL_ID, text_string);
    
    if(hanzi_detail_cur_idx < hanzi_detail_count)
    {
        Hanzi_SetDiretionText(MMI_ZMT_HANZI_LISTEN_INFO_LABEL_NUM_CTRL_ID, hanzi_detail_cur_idx, hanzi_detail_count);    
        HanziDetail_DisplayDtailInfo(win_id,
            MMI_ZMT_HANZI_LISTEN_INFO_LABEL_HANZI_CTRL_ID,
            MMI_ZMT_HANZI_LISTEN_INFO_LABEL_PINYIN_CTRL_ID,
            MMI_ZMT_HANZI_LISTEN_INFO_BUTTON_AUDIO_CTRL_ID,
            MMI_ZMT_HANZI_LISTEN_INFO_TEXT_INFO_CTRL_ID
        );
    }
}

LOCAL void HanziListenInfoWin_TP_PRESS_UP(MMI_WIN_ID_T win_id, DPARAM param)
{
    int16 tp_offset_x = 0;
    int16 tp_offset_y = 0;
    GUI_POINT_T point = {0};
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    tp_offset_x = point.x - main_tp_down_x;
    tp_offset_y = point.y - main_tp_down_y;
    if(ABS(tp_offset_x) <= ABS(tp_offset_y))
    {
        if(tp_offset_y > 40)
        {
            MMK_PostMsg(MMI_ZMT_HANZI_LISTEN_INFO_TEXT_INFO_CTRL_ID, MSG_KEYREPEAT_UP, PNULL, 0);
        }
        else if(tp_offset_y < -40)
        {
            MMK_PostMsg(MMI_ZMT_HANZI_LISTEN_INFO_TEXT_INFO_CTRL_ID, MSG_KEYREPEAT_DOWN, PNULL, 0);
        }
    }
}

LOCAL MMI_RESULT_E HandleHanziListenInfoWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                HanziListenInfoWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {
                HanziListenInfoWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_TP_PRESS_UP:
            {
                HanziListenInfoWin_TP_PRESS_UP(win_id, param);
            }
            break;
        case MSG_TP_PRESS_DOWN:
            {
                main_tp_down_x = MMK_GET_TP_X(param);
                main_tp_down_y = MMK_GET_TP_Y(param);
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

WINDOW_TABLE(MMI_HANZI_LISTEN_INFO_WIN_TAB) = {
    WIN_ID(MMI_HANZI_LISTEN_INFO_WIN_ID),
    WIN_FUNC((uint32)HandleHanziListenInfoWinMsg),
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_LISTEN_INFO_LABEL_BACK_CTRL_ID),//单元名称
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMI_ZMT_HANZI_LISTEN_INFO_LABEL_NUM_CTRL_ID),//导航页
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMI_ZMT_HANZI_LISTEN_INFO_LABEL_HANZI_CTRL_ID),//汉字
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMI_ZMT_HANZI_LISTEN_INFO_LABEL_PINYIN_CTRL_ID),//拼音
    CREATE_BUTTON_CTRL(ZMT_LISTEN_VOLUME, MMI_ZMT_HANZI_LISTEN_INFO_BUTTON_AUDIO_CTRL_ID),//音频图标
    CREATE_TEXT_CTRL(MMI_ZMT_HANZI_LISTEN_INFO_TEXT_INFO_CTRL_ID),//释义
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_LISTEN_INFO_BUTTON_STATUS_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_CloseHanziListenInfoWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(MMI_HANZI_LISTEN_INFO_WIN_ID)){
        MMK_CloseWin(MMI_HANZI_LISTEN_INFO_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreateHanziListenInfoWin(void)
{
    MMI_CloseHanziListenInfoWin();
    MMK_CreateWin((uint32 *)MMI_HANZI_LISTEN_INFO_WIN_TAB, PNULL);
}

/////////////////////////////////////////////////////////////////////////
LOCAL void HanziListenSetWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_WIDTH_T list_ctrl_width  = {0};
    GUIFORM_CHILD_HEIGHT_T list_ctrl_height = {0};
    MMI_CTRL_ID_T form_ctrl_id = 0;
    MMI_CTRL_ID_T label_ctrl_id = 0;
    MMI_CTRL_ID_T list_ctrl_id = 0;
    GUI_RECT_T form_rect = hanzi_form_rect;
    GUI_BG_T form_bg = {GUI_BG_COLOR, GUI_SHAPE_ROUNDED_RECT, 0, MMI_BLACK_COLOR, FALSE};
    uint16 form_hspace = 0;
    uint16 form_vspave = 10;
    CAF_COLOR_T bg_color = HANZI_WIN_BG_COLOR;
    MMI_STRING_T text_string = {0};
    GUISTR_STYLE_T text_style = {0};
    wchar text_str[50] = {0};
    uint8 i,j = 0;
    MMI_TEXT_ID_T symbol_text[HANZI_LISTEN_SET_SYMBOL_NUM] = {
        WORD_LISTENING_STYLE, WORD_LISTENING_TIMES, WORD_LISTENING_REPEAT
    };

    memset(hanzi_listen_set, 0, sizeof(hanzi_listen_set));
    hanzi_listen_set[0] = hanzi_listen_info.style;
    hanzi_listen_set[1] = hanzi_listen_info.interval = HANZI_LISTEN_SET_INTERVAL_3;
    hanzi_listen_set[2] = hanzi_listen_info.repeat = HANZI_LISTEN_SET_REPEAT_1;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_22;
    text_style.font_color = MMI_WHITE_COLOR;

    form_bg.color = bg_color;
    GUIFORM_SetBg(MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID, &form_bg);
    GUIFORM_SetRect(MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID, &form_rect);
    GUIFORM_PermitChildBg(MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID,FALSE);
    GUIFORM_PermitChildFont(MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID,FALSE);
    GUIFORM_PermitChildBorder(MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID, FALSE);
    GUIFORM_SetDisplayScrollBar(MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID, FALSE);
    for(i = 0;i < HANZI_LISTEN_SET_SYMBOL_NUM;i++)
    {
        form_ctrl_id = MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_CTRL_ID + i;
        GUIFORM_SetBg(form_ctrl_id, &form_bg);
        GUIFORM_SetRect(form_ctrl_id, &form_rect);
        for(j = 0;j < HANZI_LISTEN_SET_SYMBOL_NUM;j++)
        {
            label_ctrl_id = MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_LABEL_CTRL_ID + j;
            GUILABEL_SetTextById(label_ctrl_id, symbol_text[j], TRUE);
            GUILABEL_SetFont(label_ctrl_id, text_style.font, text_style.font_color);
            list_ctrl_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            list_ctrl_height.add_data = HANZI_CARD_LINE_HIGHT;
            GUIFORM_SetChildHeight(form_ctrl_id, label_ctrl_id, &list_ctrl_height);
            list_ctrl_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            list_ctrl_width.add_data = form_rect.right - form_rect.left;
            GUIFORM_SetChildWidth(form_ctrl_id, label_ctrl_id, &list_ctrl_width);
            
            list_ctrl_id = MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_LIST_CTRL_ID + j;
            GUILIST_SetListState(list_ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
            GUILIST_SetNeedHiLightBar(list_ctrl_id,FALSE);
            
            GUILIST_SetNeedPrgbarBlock(list_ctrl_id,FALSE);
            GUILIST_SetUserBg(list_ctrl_id,TRUE);
            GUILIST_SetBgColor(list_ctrl_id,HANZI_WIN_BG_COLOR);
            GUILIST_SetTextFont(list_ctrl_id, DP_FONT_18, MMI_WHITE_COLOR);
            GUILIST_PermitBorder(list_ctrl_id, FALSE);
            GUILIST_SetSlideState(list_ctrl_id, FALSE);
				   GUILIST_SetListState(list_ctrl_id,GUILIST_STATE_NEED_PRGBAR,FALSE);
            list_ctrl_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            if(j == 0){
                list_ctrl_height.add_data = 3* HANZI_CARD_LINE_HIGHT;
                GUILIST_SetMaxItem(list_ctrl_id, HANZI_LISTEN_SET_SYMBOL_NUM-1, FALSE);
            }else{
                list_ctrl_height.add_data = 4* HANZI_CARD_LINE_HIGHT + 10;
                GUILIST_SetMaxItem(list_ctrl_id, HANZI_LISTEN_SET_SYMBOL_NUM, FALSE);
            }
            GUIFORM_SetChildHeight(form_ctrl_id, list_ctrl_id, &list_ctrl_height);
            list_ctrl_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            list_ctrl_width.add_data = form_rect.right - form_rect.left;
            GUIFORM_SetChildWidth(form_ctrl_id, list_ctrl_id, &list_ctrl_width);
        }
    }
    GUIFORM_SetActiveChild(MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_CTRL_ID);
}

LOCAL void HanziListenSetWin_DisplayOption( MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T form_ctrl_id = MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID;
    MMI_CTRL_ID_T list_ctrl_id = 0;
    MMI_CTRL_ID_T label_ctrl_id = 0;
    GUI_BG_T form_bg = {GUI_BG_COLOR, GUI_SHAPE_ROUNDED_RECT, 0, HANZI_WIN_BG_COLOR, FALSE};
    GUI_RECT_T form_rect = {0};
    MMI_HANDLE_T ctrl_handle = 0;
    MMI_STRING_T text_string = {0};
    wchar text_str[50] = {0};
    uint8 i = 0;
    uint8 j = 0;
    MMI_TEXT_ID_T item_text[HANZI_LISTEN_SET_SYMBOL_NUM][HANZI_LISTEN_SET_SYMBOL_NUM] = {
        {WORD_LISTENING_NORMAL, WORD_LISTENING_RANDOM, WORD_LISTENING_RANDOM},
        {WORD_LISTENING_3S, WORD_LISTENING_5S, WORD_LISTENING_10S},
        {WORD_LISTENING_REPEAT_1, WORD_LISTENING_REPEAT_3, WORD_LISTENING_REPEAT_5},
    };
    uint16 item_num[HANZI_LISTEN_SET_SYMBOL_NUM][HANZI_LISTEN_SET_SYMBOL_NUM] = {
        {0, 1, 2},
        {HANZI_LISTEN_SET_INTERVAL_3, HANZI_LISTEN_SET_INTERVAL_5, HANZI_LISTEN_SET_INTERVAL_10}, 
        {HANZI_LISTEN_SET_REPEAT_1, HANZI_LISTEN_SET_REPEAT_3, HANZI_LISTEN_SET_REPEAT_5}
    };
    
    for(i = 0;i < HANZI_LISTEN_SET_SYMBOL_NUM;i++)
    {
        GUILIST_ITEM_T item_info = {0};
        GUILIST_ITEM_DATA_T item_data= {0};
        uint8 list_num = 0;
        if(i == 0){
            list_num = HANZI_LISTEN_SET_SYMBOL_NUM-1;
        }else{
            list_num = HANZI_LISTEN_SET_SYMBOL_NUM;
        }
        list_ctrl_id = MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_LIST_CTRL_ID + i;
        GUILIST_RemoveAllItems(list_ctrl_id);
        for(j = 0;j < list_num;j++)
        {
            item_info.item_style = GUIITEM_SYTLE_ZMT_UNIT_LIST_MS;
            item_info.item_data_ptr = &item_data;

            memset(&text_str, 0, 50);
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id= item_text[i][j];

            item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
            if(item_num[i][j] == hanzi_listen_set[i]){
                item_data.item_content[1].item_data.image_id = IMG_ZMT_SELECTED;
            }else{
                item_data.item_content[1].item_data.image_id = IMG_ZMT_UNSELECTED;
            }
            GUILIST_AppendItem(list_ctrl_id, &item_info);
        }
    }
}

LOCAL void HanziListenSetWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    uint8 i = 0;
    GUI_RECT_T lef_button_rect = hanzi_left_button_rect;
    GUI_RECT_T right_button_rect = hanzi_right_button_rect;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_22;
    text_style.font_color = MMI_WHITE_COLOR;
    
    MMIRES_GetText(WORD_LISTENING_SETTING, win_id, &text_string);
    Hanzi_DrawWinTitle(win_id, 0, text_string);

    HanziListenSetWin_DisplayOption(win_id);

    text_style.font = DP_FONT_22;
  //  GUIRES_DisplayImg(PNULL, &lef_button_rect, PNULL, win_id, FORMULA_BOTTOM_BG_IMG, &lcd_dev_info);
    MMIRES_GetText(WORD_FALSE, win_id, &text_string);
    text_style.font_color = HANZI_WIN_BG_COLOR;
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &lef_button_rect,
        &lef_button_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );

 //   GUIRES_DisplayImg(PNULL, &right_button_rect, PNULL, win_id, FORMULA_BOTTOM_BG_IMG, &lcd_dev_info);
    MMIRES_GetText(WORD_TRUE, win_id, &text_string);
    text_style.font_color = HANZI_WIN_BG_COLOR;
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &right_button_rect,
        &right_button_rect,
        &text_string,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
    );
}

LOCAL void HanziListenSetWin_CTL_PENOK(MMI_WIN_ID_T win_id, DPARAM param)
{
    uint16 cur_idx = 0;
    uint8 cur_ctrl_id_idx = 0;
    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T *)param)->src_id;
    uint16 interval[HANZI_LISTEN_SET_SYMBOL_NUM] = {HANZI_LISTEN_SET_INTERVAL_3, HANZI_LISTEN_SET_INTERVAL_5, HANZI_LISTEN_SET_INTERVAL_10};
    uint8 repeat[HANZI_LISTEN_SET_SYMBOL_NUM] = {HANZI_LISTEN_SET_REPEAT_1, HANZI_LISTEN_SET_REPEAT_3, HANZI_LISTEN_SET_REPEAT_5};

    cur_idx = GUILIST_GetCurItemIndex(ctrl_id);
    cur_ctrl_id_idx = ctrl_id - MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_LIST_CTRL_ID;
    if(cur_ctrl_id_idx == 0)
    {
        hanzi_listen_set[0]  = cur_idx;
    }
    else if(cur_ctrl_id_idx == 1)
    {
        hanzi_listen_set[1] = interval[cur_idx];
    }
    else if(cur_ctrl_id_idx == 2)
    {
        hanzi_listen_set[2] = repeat[cur_idx];
    }
    SCI_TRACE_LOW("%s: style = %d, interval = %d, repeat = %d", __FUNCTION__,
        hanzi_listen_set[0], hanzi_listen_set[1], hanzi_listen_set[2]);
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

LOCAL void HanziListenSetWin_TP_PRESS_UP(MMI_WIN_ID_T win_id, GUI_POINT_T point)
{
    GUI_RECT_T lef_button_rect = hanzi_left_button_rect;
    GUI_RECT_T right_button_rect = hanzi_right_button_rect;
    if(GUI_PointIsInRect(point, lef_button_rect))
    {
        MMK_CloseWin(win_id);
    }
    else if(GUI_PointIsInRect(point, right_button_rect))
    {
        hanzi_listen_info.style = hanzi_listen_set[0];
        hanzi_listen_info.interval = hanzi_listen_set[1];
        hanzi_listen_info.repeat = hanzi_listen_set[2];
        hanzi_listen_info.listen_idx = 0;
        hanzi_listen_cur_idx = 0;
        MMK_CloseWin(win_id);
    }
}

LOCAL MMI_RESULT_E HandleHanziListenSetWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                HanziListenSetWin_OPEN_WINDOW(win_id);
            }
            break;
        case MSG_FULL_PAINT:
            {
                HanziListenSetWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_TP_PRESS_UP:
            {
                GUI_RECT_T lef_button_rect = hanzi_left_button_rect;
                GUI_POINT_T point = {0};
                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);
                if(point.y >= lef_button_rect.top)
                {
                    HanziListenSetWin_TP_PRESS_UP(win_id, point);
                }
            }
            break;
        case MSG_CTL_PENOK:
            {
                HanziListenSetWin_CTL_PENOK(win_id, param);
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
                memset(hanzi_listen_set, 0, sizeof(hanzi_listen_set));
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(MMI_HANZI_LISTEN_SET_WIN_TAB) = {
    WIN_ID(MMI_HANZI_LISTEN_SET_WIN_ID),
    WIN_FUNC((uint32)HandleHanziListenSetWinMsg),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_CTRL_ID,MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_LABEL_CTRL_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_CTRL_ID),
            CHILD_LIST_CTRL(TRUE, GUILIST_TYPE_TEXT_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_LIST_CTRL_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_1_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_ZMT_HANZI_LISTEN_FORM_CHILD_2_CTRL_ID,MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_2_LABEL_CTRL_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_2_CTRL_ID),
            CHILD_LIST_CTRL(TRUE, GUILIST_TYPE_TEXT_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_2_LIST_CTRL_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_2_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_ZMT_HANZI_LISTEN_FORM_CHILD_3_CTRL_ID,MMI_ZMT_HANZI_LISTEN_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_3_LABEL_CTRL_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_3_CTRL_ID),
            CHILD_LIST_CTRL(TRUE, GUILIST_TYPE_TEXT_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_3_LIST_CTRL_ID, MMI_ZMT_HANZI_LISTEN_FORM_CHILD_3_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_CloseHanziListenSetWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(MMI_HANZI_LISTEN_SET_WIN_ID)){
        MMK_CloseWin(MMI_HANZI_LISTEN_SET_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreateHanziListenSetWin(void)
{
    MMI_CloseHanziListenSetWin();
    MMK_CreateWin((uint32 *)MMI_HANZI_LISTEN_SET_WIN_TAB, PNULL);
}


//////////////////////////////////////////////////////////////////////////////////
LOCAL void Hanzi_GetOneArrayRandNum(uint8 count, uint8 min, uint8 max)
{
    int i = 0;
    int j = 0;
    int temp = 0;
    
    srand(MMIAPICOM_GetCurTime());
    for (i = 0; i < count; i++) {
        hanzi_listen_idx[i] = i + min;
    }
    for (i = max; i > 0; i--) {
        j = rand() % (i + 1);
        temp = hanzi_listen_idx[i];
        hanzi_listen_idx[i] = hanzi_listen_idx[j];
        hanzi_listen_idx[j] = temp;
    }
}

LOCAL void HanziListenWin_PlayAudioFail(void)
{
    if(MMI_IsOpenHanziListenWin() && hanzi_listen_info.status == HANZI_LISTEN_NOW)
    {
        uint8 listen_idx = 0;
        if(hanzi_listen_info.style == 0){
            hanzi_listen_cur_idx++;
        }else{
            hanzi_listen_info.listen_idx++;
            hanzi_listen_cur_idx = hanzi_listen_idx[hanzi_listen_info.listen_idx];
        }
        hanzi_detail_cur_idx = hanzi_listen_cur_idx;
        MMK_SendMsg(MMI_HANZI_LISTEN_WIN_ID, MSG_FULL_PAINT, PNULL);
        SCI_TRACE_LOW("%s: hanzi_listen_cur_idx = %d", __FUNCTION__, hanzi_listen_cur_idx);
        if(hanzi_listen_info.style == 0){
            listen_idx = hanzi_listen_cur_idx;
        }else{
            listen_idx = hanzi_listen_info.listen_idx;
        }
        if(listen_idx < hanzi_detail_count){
            HanziDetail_PlayPinyinAudio();
        }
    }
}

LOCAL void HanziListenWin_StopIntervalTimer(void)
{
    if(hanzi_listen_timer_id != 0)
    {
        MMK_StopTimer(hanzi_listen_timer_id);
        hanzi_listen_timer_id = 0;
    }
}

LOCAL void HanziListenWin_IntervalTimerCallback(uint8 timer_id, uint32 param)
{
    if(hanzi_listen_timer_id == timer_id)
    {
        uint8 listen_idx = 0;
        HanziListenWin_StopIntervalTimer();
        SCI_TRACE_LOW("%s: hanzi_listen_repeat_count= %d", __FUNCTION__, hanzi_listen_repeat_count);
        if(hanzi_listen_repeat_count + 1 >= hanzi_listen_info.repeat){
            hanzi_listen_repeat_count = 0;
            if(hanzi_listen_info.style == 0){
                hanzi_listen_cur_idx++;
            }else{
                hanzi_listen_info.listen_idx++;
                hanzi_listen_cur_idx = hanzi_listen_idx[hanzi_listen_info.listen_idx];
            }
            hanzi_detail_cur_idx = hanzi_listen_cur_idx;
            if(MMK_IsFocusWin(MMI_HANZI_LISTEN_WIN_ID)){
            MMK_SendMsg(MMI_HANZI_LISTEN_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
            SCI_TRACE_LOW("%s: hanzi_listen_cur_idx = %d", __FUNCTION__, hanzi_listen_cur_idx);
        }else{
            hanzi_listen_repeat_count++;
        }
        if(hanzi_listen_info.style == 0){
            listen_idx = hanzi_listen_cur_idx;
        }else{
            listen_idx = hanzi_listen_info.listen_idx;
        }
        if(listen_idx < hanzi_detail_count){
            HanziDetail_PlayPinyinAudio();
        }
    }
}

LOCAL void HanziListenWin_CreateIntervalTimer(void)
{
    SCI_TRACE_LOW("%s: start", __FUNCTION__);
    HanziListenWin_StopIntervalTimer();
    hanzi_listen_timer_id = MMK_CreateTimerCallback(hanzi_listen_info.interval, HanziListenWin_IntervalTimerCallback, PNULL, FALSE);
    MMK_StartTimerCallback(hanzi_listen_timer_id, hanzi_listen_info.interval, HanziListenWin_IntervalTimerCallback, PNULL, FALSE);
}

LOCAL void HanziListenWin_SetClickFunc(void)
{
    if(hanzi_listen_info.status == HANZI_LISTEN_NOW)
    {
        hanzi_listen_info.status = HANZI_LISTEN_PAUSE;
    }
    HanziListenWin_StopIntervalTimer();
    Hanzi_StopPlayMp3Data();
    MMI_CreateHanziListenSetWin();
}

LOCAL void HanziListenWin_ImgClickFunc(void)
{
    if(hanzi_listen_info.status == HANZI_LISTEN_NOW || hanzi_listen_info.status == HANZI_LISTEN_PAUSE)
    {
        hanzi_listen_info.status = HANZI_LISTEN_PAUSE;
        HanziListenWin_StopIntervalTimer();
        Hanzi_StopPlayMp3Data();
        hanzi_detail_cur_idx = hanzi_listen_cur_idx;
        if(hanzi_detail_cur_idx < hanzi_detail_count){
        MMI_CreateHanziListenInfoWin();
        }
    }
}

LOCAL void HanziListenWin_BottomActionFunc(void)
{
    if(hanzi_listen_info.status == HANZI_LISTEN_NONE)
    {
        hanzi_listen_info.status = HANZI_LISTEN_NOW;
        hanzi_listen_cur_idx = 0;
        hanzi_detail_cur_idx = hanzi_listen_cur_idx;
        HanziDetail_PlayPinyinAudio();
    }
    else if(hanzi_listen_info.status == HANZI_LISTEN_PAUSE)
    {
        hanzi_listen_info.status = HANZI_LISTEN_NOW;
        if(hanzi_listen_info.style == 1){
            hanzi_listen_cur_idx = hanzi_listen_idx[hanzi_listen_info.listen_idx];
        }
        hanzi_detail_cur_idx = hanzi_listen_cur_idx;
        HanziDetail_PlayPinyinAudio();
    }
    else if(hanzi_listen_info.status == HANZI_LISTEN_NOW)
    {
        hanzi_listen_info.status = HANZI_LISTEN_PAUSE;
        HanziListenWin_StopIntervalTimer();
        Hanzi_StopPlayMp3Data();
    }
    MMK_SendMsg(MMI_HANZI_LISTEN_WIN_ID, MSG_FULL_PAINT, PNULL);
}

LOCAL void HanziListenWin_ListenAgainFunc(void)
{
    hanzi_listen_info.status = HANZI_LISTEN_NOW;
    hanzi_listen_cur_idx = 0;
    hanzi_listen_info.listen_idx = 0;
    memset(hanzi_listen_idx, 0, sizeof(hanzi_listen_idx));
    Hanzi_GetOneArrayRandNum(hanzi_detail_count, 0, hanzi_detail_count-1);
    if(hanzi_listen_info.style == 1){
        hanzi_detail_cur_idx = hanzi_listen_idx[0];
    }
    hanzi_detail_cur_idx = hanzi_listen_cur_idx;
    HanziDetail_PlayPinyinAudio();
    MMK_SendMsg(MMI_HANZI_LISTEN_WIN_ID, MSG_FULL_PAINT, PNULL);
}

LOCAL void HanziListenWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    GUI_FONT_ALL_T font = {0};
    GUI_BORDER_T btn_border = {1, MMI_BLACK_COLOR, GUI_BORDER_SOLID};
    GUI_FONT_T text_font = DP_FONT_20;
    GUI_COLOR_T text_color = MMI_WHITE_COLOR;
    GUI_RECT_T img_rect = {0};
    GUI_RECT_T tip_rect = {0};
    GUI_RECT_T dir_rect = hanzi_dir_rect;
    GUI_RECT_T text_rect = hanzi_text_rect;
    GUI_RECT_T left_rect = hanzi_left_rect;
    GUI_RECT_T right_rect = hanzi_right_rect;
    GUI_RECT_T bottom_rect = hanzi_bottom_rect;

    GUIBUTTON_SetRect(MMI_ZMT_HANZI_LISTEN_BUTTON_SET_CTRL_ID, &dir_rect);               
    GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_LISTEN_BUTTON_SET_CTRL_ID, HanziListenWin_SetClickFunc);

    dir_rect.top = dir_rect.bottom;
    dir_rect.bottom = dir_rect.top + HANZI_CARD_LINE_HIGHT;
    GUILABEL_SetRect(MMI_ZMT_HANZI_LISTEN_LABEL_NUM_CTRL_ID, &dir_rect, FALSE);
    GUILABEL_SetFont(MMI_ZMT_HANZI_LISTEN_LABEL_NUM_CTRL_ID, DP_FONT_20,MMI_WHITE_COLOR);
    GUILABEL_SetAlign(MMI_ZMT_HANZI_LISTEN_LABEL_NUM_CTRL_ID, GUILABEL_ALIGN_MIDDLE);

    img_rect = text_rect;
    img_rect.top = 2*HANZI_CARD_LINE_HIGHT;
    img_rect.bottom = 7*HANZI_CARD_LINE_HIGHT;
    GUIBUTTON_SetRect(MMI_ZMT_HANZI_LISTEN_BUTTON_IMG_CTRL_ID, &img_rect);               
    GUIBUTTON_SetCallBackFunc(MMI_ZMT_HANZI_LISTEN_BUTTON_IMG_CTRL_ID, HanziListenWin_ImgClickFunc);

    tip_rect = text_rect;
    tip_rect.top = 7*HANZI_CARD_LINE_HIGHT;
    tip_rect.bottom = tip_rect.top + HANZI_CARD_LINE_HIGHT;
    GUILABEL_SetRect(MMI_ZMT_HANZI_LISTEN_LABEL_TIPS_CTRL_ID, &tip_rect, FALSE);
    GUILABEL_SetFont(MMI_ZMT_HANZI_LISTEN_LABEL_TIPS_CTRL_ID, DP_FONT_18,MMI_WHITE_COLOR);
    GUILABEL_SetAlign(MMI_ZMT_HANZI_LISTEN_LABEL_TIPS_CTRL_ID, GUILABEL_ALIGN_MIDDLE);

    Hanzi_InitButton(MMI_ZMT_HANZI_LISTEN_BUTTON_STATUS_CTRL_ID, bottom_rect, NULL, ALIGN_HVMIDDLE, TRUE, HanziListenWin_BottomActionFunc);
    Hanzi_InitButtonBg(MMI_ZMT_HANZI_LISTEN_BUTTON_STATUS_CTRL_ID);

    Hanzi_InitButton(MMI_ZMT_HANZI_LISTEN_LEFT_CTRL_ID, left_rect, WORD_LISTENING_EXIT, ALIGN_HVMIDDLE, FALSE, MMI_CloseHanziListenWin);
    Hanzi_InitButtonBg(MMI_ZMT_HANZI_LISTEN_LEFT_CTRL_ID);
    Hanzi_InitButton(MMI_ZMT_HANZI_LISTEN_RIGHT_CTRL_ID, right_rect, WORD_LISTENING_AGAIN, ALIGN_HVMIDDLE, FALSE, HanziListenWin_ListenAgainFunc);
    Hanzi_InitButtonBg(MMI_ZMT_HANZI_LISTEN_RIGHT_CTRL_ID);
    
    memset(&hanzi_listen_info, 0, sizeof(HANZI_LISTEN_INFO_T));
    hanzi_listen_info.interval = HANZI_LISTEN_SET_INTERVAL_3;
    hanzi_listen_info.repeat = hanzi_listen_repeat_count = HANZI_LISTEN_SET_REPEAT_1;
    hanzi_listen_info.style = 0;
    hanzi_listen_info.listen_idx = 0;
}

LOCAL void HanziListenWin_DrawListenStatus(MMI_WIN_ID_T win_id, HANZI_LISTEN_STAUS_TYPE_E status)
{
    MMI_TEXT_ID_T label_text = 0;
    MMI_TEXT_ID_T button_text = 0;

    switch(status)
    {
        case HANZI_LISTEN_NONE:
            {
                label_text = WORD_LEARN_LISTENING_TIPS_1;
                button_text = WORD_LEARN_LISTENING_ACTION;
            }
            break;
        case HANZI_LISTEN_NOW:
            {
                label_text = WORD_LEARN_LISTENING_TIPS_2;
                button_text = WORD_LEARN_LISTENING;
            }
            break;
        case HANZI_LISTEN_PAUSE:
            {
                label_text = WORD_LEARN_LISTENING_TIPS_2;
                button_text = WORD_LEARN_LISTENING_PAUSE;
            }
            break;
        default:
            return;
    }
    GUILABEL_SetTextById(MMI_ZMT_HANZI_LISTEN_LABEL_TIPS_CTRL_ID, label_text, TRUE);
    GUIBUTTON_SetTextId(MMI_ZMT_HANZI_LISTEN_BUTTON_STATUS_CTRL_ID, button_text);
    GUIBUTTON_SetVisible(MMI_ZMT_HANZI_LISTEN_BUTTON_STATUS_CTRL_ID, TRUE, TRUE);
}

LOCAL void HanziListenWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS_EX;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    char text_str[100] = {0};
    wchar text_wchar[100] = {0};
    BOOLEAN is_end = FALSE;
    uint8 listen_idx = 0;

    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = DP_FONT_20;
    text_style.font_color = MMI_WHITE_COLOR;

    if(is_open_new_hanzi){
        MMIRES_GetText(HANZI_PACTISE, win_id, &text_string);
    }else{
        sprintf(text_str, "%s", hanzi_content_info[hanzi_book_info.cur_section_idx]->chapter[hanzi_book_info.cur_section_children_idx]->chapter_name);
        GUI_UTF8ToWstr(text_wchar, 100, text_str, strlen(text_str));
        text_string.wstr_ptr = text_wchar;
        text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);
    }
    Hanzi_DrawWinTitle(win_id, 0, text_string);

    if(hanzi_listen_info.style == 0){
        listen_idx = hanzi_listen_cur_idx;
    }else{
        listen_idx = hanzi_listen_info.listen_idx;
    }
    if(listen_idx < hanzi_detail_count)
    {
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_LISTEN_LEFT_CTRL_ID,FALSE,FALSE);
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_LISTEN_RIGHT_CTRL_ID,FALSE,FALSE);
        
        HanziListenWin_DrawListenStatus(win_id, hanzi_listen_info.status);
        
        if(hanzi_listen_info.style == 1){
            GUILABEL_SetVisible(MMI_ZMT_HANZI_LISTEN_LABEL_NUM_CTRL_ID, FALSE, TRUE);
        }else{
            GUILABEL_SetVisible(MMI_ZMT_HANZI_LISTEN_LABEL_NUM_CTRL_ID, TRUE, TRUE);
            //Hanzi_SetDiretionText(MMI_ZMT_HANZI_LISTEN_LABEL_NUM_CTRL_ID, hanzi_listen_cur_idx, hanzi_detail_count);
        }
    }
    else
    {
        GUILABEL_SetVisible(MMI_ZMT_HANZI_LISTEN_LABEL_NUM_CTRL_ID, FALSE, TRUE);
        GUILABEL_SetTextById(MMI_ZMT_HANZI_LISTEN_LABEL_TIPS_CTRL_ID, WORD_LISTENING_END, TRUE);
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_LISTEN_BUTTON_STATUS_CTRL_ID, FALSE, TRUE);

        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_LISTEN_LEFT_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMI_ZMT_HANZI_LISTEN_RIGHT_CTRL_ID,TRUE,TRUE);
    }
}

LOCAL void HanziListenWin_CLOSE_WINDOW(void)
{
    hanzi_listen_cur_idx = 0;
    hanzi_detail_cur_idx = 0;
    hanzi_listen_repeat_count = 0;
    memset(&hanzi_listen_info, 0, sizeof(HANZI_LISTEN_INFO_T));
    memset(hanzi_listen_idx, 0, sizeof(hanzi_listen_idx));
    Hanzi_StopPlayMp3Data();
    HanziListenWin_StopIntervalTimer();
    MMI_CloseHanziDetailWin();
}

LOCAL MMI_RESULT_E HandleHanziListenWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                HanziListenWin_OPEN_WINDOW(win_id);
           //     WATCHCOM_Backlight(TRUE);
                Hanzi_GetOneArrayRandNum(hanzi_detail_count, 0, hanzi_detail_count-1);
            }
            break;
        case MSG_FULL_PAINT:
            {
                HanziListenWin_FULL_PAINT(win_id);
            }
            break;
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_CTL_PENOK:
            {
                HanziListenWin_BottomActionFunc();
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
                HanziListenWin_CLOSE_WINDOW();
              //  WATCHCOM_Backlight(FALSE);
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(MMI_HANZI_LISTEN_WIN_TAB) = {
    WIN_ID(MMI_HANZI_LISTEN_WIN_ID),
    WIN_FUNC((uint32)HandleHanziListenWinMsg),
    CREATE_BUTTON_CTRL(IMG_ZMT_LISTEN_SET, MMI_ZMT_HANZI_LISTEN_BUTTON_SET_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_ZMT_HANZI_LISTEN_LABEL_NUM_CTRL_ID),
    CREATE_BUTTON_CTRL(IMG_ZMT_LISTEN_PEN, MMI_ZMT_HANZI_LISTEN_BUTTON_IMG_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_ZMT_HANZI_LISTEN_LABEL_TIPS_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_LISTEN_BUTTON_STATUS_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_LISTEN_LEFT_CTRL_ID),//退出
    CREATE_BUTTON_CTRL(PNULL, MMI_ZMT_HANZI_LISTEN_RIGHT_CTRL_ID),//再听一遍
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E MMI_CloseHanziListenWin(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    if(MMK_IsOpenWin(MMI_HANZI_LISTEN_WIN_ID)){
        MMK_CloseWin(MMI_HANZI_LISTEN_WIN_ID);
    }
    return result;
}

PUBLIC void MMI_CreateHanziListenWin(void)
{
    MMI_CloseHanziListenWin();
    MMK_CreateWin((uint32 *)MMI_HANZI_LISTEN_WIN_TAB, PNULL);
}

LOCAL BOOLEAN MMI_IsOpenHanziListenWin(void)
{
    if(MMK_IsOpenWin(MMI_HANZI_LISTEN_WIN_ID)){
        return TRUE;
    }
    return FALSE;
}

PUBLIC void MMIZMT_CloseHanziPlayer(void)
{
    Hanzi_StopPlayMp3Data();    
}

