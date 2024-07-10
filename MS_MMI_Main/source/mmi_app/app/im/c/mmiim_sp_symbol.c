/*==============================================================================
File Name: mmiim_sp_symbol.c
Author: Lianxiang.Zhou
Date: 2010/05/26
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/26   Lianxiang.Zhou  Create.
==============================================================================*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_string.h"
#include "mmiim_sp_symbol.h"
#include "mmiim_sp_symbol_data.h"

#include "window_parse.h"
#include "guistring.h"
#include "guiblock.h"
#include "mmitheme_pos.h"
#include "mmk_type.h"
#include "guires.h"
#include "mmiim_id.h"
#include "mmiim_image.h"
#include "mmitheme_im.h"

//翻页的时候焦点的类型
typedef enum
{
    MMIIM_SP_SYMB_HOT_BEGIN,
    MMIIM_SP_SYMB_HOT_HOLD,
    MMIIM_SP_SYMB_HOT_HOLD_COL,
    MMIIM_SP_SYMB_HOT_END
} MMIIM_SP_SYMB_PG_CH_HOT_TYPE_T;


//请注意与MMIIM_SP_SYMB_IMG_SPACE_HALF定义顺序的对应性，否则导致显示错误
static MMI_IMAGE_ID_T const symb_img_id[] =
{
    IMG_IMSP_SYMBOL_SPACE,  //MMIIM_SP_SYMB_IMG_SPACE_HALF
    IMG_IMSP_SYMBOL_ENTER,  //MMIIM_SP_SYMB_IMG_ENTER
    IMG_IMSP_SYMBOL_SPACE,  //MMIIM_SP_SYMB_IMG_SPACE
};


LOCAL MMI_HANDLE_T SymbCreateWin(MMI_HANDLE_T im_handle, MMIIM_SP_SYMB_TYPE_T type);
LOCAL MMIIM_SP_SYMB_TYPE_T GetType(GUIIM_LANGUAGE_T lang);
LOCAL void InitData(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr);

/*==============================================================================
Function Name: LoadSymbData
Description: load symbol and setting symbol data
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void LoadSymbData(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr);

/*==============================================================================
Function Name: HandleSymbWinMsg
Description: HandleSymbWinMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMI_RESULT_E HandleSymbWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

#if (defined (TOUCH_PANEL_SUPPORT))
/*==============================================================================
Function Name: HandleTpDown
Description: HandleTpDown
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleTpDown(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    DPARAM param
);

/*==============================================================================
Function Name: HandleTpMove
Description: HandleTpMove
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleTpMove(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    DPARAM param
);

/*==============================================================================
Function Name: HandleTpUp
Description: HandleTpUp
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleTpUp(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    DPARAM param
);

/*==============================================================================
Function Name: GetTpPos
Description: GetTpPos
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL int32 GetTpPos(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    DPARAM param
);
#endif

/*==============================================================================
Function Name: DrawWin
Description: DrawWin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DrawWin(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr);

/*==============================================================================
Function Name: CommitSymb
Description: 提交当前窗口的索引所指定的符号串
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitSymb(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr, uint32 idx);

/*==============================================================================
Function Name: GetHotPos
Description: GetHotPos
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN GetHotPos(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    int16 *row_ptr,
    int16 *col_ptr
);

/*==============================================================================
Function Name: GetSymbRect
Description: 获取指定位置的背景框大小
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN GetSymbRect(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    int16 row,
    int16 col,
    GUI_RECT_T *rect_ptr
);

/*==============================================================================
Function Name: ResetPageFlag
Description: ResetPageFlag
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ResetPageFlag(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr);

/*==============================================================================
Function Name: PageDown
Description: PageDown
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN PageDown(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    MMIIM_SP_SYMB_PG_CH_HOT_TYPE_T hot_type
);

/*==============================================================================
Function Name: PageUp
Description: PageUp
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN PageUp(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    MMIIM_SP_SYMB_PG_CH_HOT_TYPE_T hot_type
);


/*==============================================================================
Function Name: GetSymbImgeID
Description: 获取对应的图片
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 
==============================================================================*/
LOCAL MMI_IMAGE_ID_T GetSymbImgeID(wchar symb);






WINDOW_TABLE(MMIIM_SP_SYMB_WIN_TAB) =
{
    WIN_ID(MMIIM_SP_SYMB_WIN_ID),
    WIN_FUNC((uint32)HandleSymbWinMsg),
    WIN_HIDE_STATUS,
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    END_WIN
};



/*==============================================================================
Description: 根据类型打开符号面板
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
MMI_HANDLE_T SymbCreateWin(MMI_HANDLE_T im_handle, MMIIM_SP_SYMB_TYPE_T type)
{
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr = PNULL;
    MMI_HANDLE_T win_handle = 0;

    win_data_ptr = (MMIIM_SP_SYMB_WIN_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SP_SYMB_WIN_DATA_T));
    SCI_MEMSET(win_data_ptr, 0, sizeof(MMIIM_SP_SYMB_WIN_DATA_T));

    win_data_ptr->im_handle = im_handle;
    win_data_ptr->symb_type = type;

    win_handle = MMK_CreateWin((uint32*)MMIIM_SP_SYMB_WIN_TAB, win_data_ptr);
    GUIIM_AppendWin(im_handle, win_handle);
    
    return win_handle;
}




/*==============================================================================
Description: 根据语言获取符号面板的类型
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMIIM_SP_SYMB_TYPE_T GetType(GUIIM_LANGUAGE_T lang)
{
    MMIIM_SP_SYMB_TYPE_T type = MMIIM_SP_SYMB_TYPE_HALF;

    switch (lang)
    {
    case GUIIM_LANG_CHS:
    case GUIIM_LANG_CHT:
        type = MMIIM_SP_SYMB_TYPE_FULL;
        break;

    case GUIIM_LANG_HINDI:
        type = MMIIM_SP_SYMB_TYPE_HINDI;
        break;

    default:
        break;
    }
    
    return type;
}


/*==============================================================================
Description: 初始化窗口数据
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void InitData(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T key_rect = {0};

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return;
    }
    
    win_data_ptr->lcd_dev_info = lcd_dev_info;

    //该窗口无title
    win_data_ptr->win_rect = MMITHEME_GetClientRectEx(win_data_ptr->win_id);
    win_data_ptr->win_rect.top = 0;

    win_data_ptr->rect_symb = win_data_ptr->win_rect;
    win_data_ptr->rect_symb.top += MMIIM_SP_SYMB_MARGIN_V;
    win_data_ptr->rect_symb.bottom -= MMIIM_SP_SYMB_MARGIN_V;
    win_data_ptr->rect_symb.left += MMIIM_SP_SYMB_MARGIN_H;
    win_data_ptr->rect_symb.right -= MMIIM_SP_SYMB_MARGIN_H;

    win_data_ptr->row = MMIIM_SP_SYMB_PANEL_ROW;
    win_data_ptr->col = MMIIM_SP_SYMB_PANEL_COL;

    win_data_ptr->color_bg = MMIIM_SP_SYMB_COLOR_BG;
    win_data_ptr->color_border = MMIIM_SP_SYMB_COLOR_BORDER;
    win_data_ptr->color_font = MMIIM_SP_SYMB_COLOR_FONT;
    win_data_ptr->color_hot = MMIIM_SP_SYMB_COLOR_HOT;

    win_data_ptr->font = MMIIM_SP_SYMB_FONT;
    win_data_ptr->font_min = MMIIM_SP_SYMB_FONT_STR;
    win_data_ptr->font_label = MMIIM_SP_SYMB_FONT_LABEL;

    LoadSymbData(win_data_ptr);

    //空出地方来放按钮
    win_data_ptr->rect_symb.top += MMIIM_SP_SYMB_MARGIN_V;
    win_data_ptr->rect_symb.top += MMIIM_SP_SYMB_KEY_HEIGHT;

    key_rect.top = win_data_ptr->win_rect.top + MMIIM_SP_SYMB_MARGIN_V;
    key_rect.left = win_data_ptr->win_rect.left + MMIIM_SP_SYMB_MARGIN_H;
    key_rect.bottom = key_rect.top + MMIIM_SP_SYMB_KEY_HEIGHT - 1;
    key_rect.right = win_data_ptr->win_rect.right - MMIIM_SP_SYMB_MARGIN_V;
    //page up
    win_data_ptr->key_pageup.rect = key_rect;
    win_data_ptr->key_pageup.rect.right = win_data_ptr->key_pageup.rect.left + MMIIM_SP_SYMB_KEY_WIDTH - 1;
    win_data_ptr->key_pageup.str[0] = '<';
    win_data_ptr->key_pageup.str[1] = '<';
    win_data_ptr->key_pageup.str[2] = '[';
    win_data_ptr->key_pageup.str[3] = '*';
    win_data_ptr->key_pageup.str[4] = ']';
    win_data_ptr->key_pageup.str_len = 5;
    //page down
    win_data_ptr->key_pagedown.rect = key_rect;
    win_data_ptr->key_pagedown.rect.left = win_data_ptr->key_pagedown.rect.right - MMIIM_SP_SYMB_KEY_WIDTH + 1;
    win_data_ptr->key_pagedown.str[0] = '[';
    win_data_ptr->key_pagedown.str[1] = '#';
    win_data_ptr->key_pagedown.str[2] = ']';
    win_data_ptr->key_pagedown.str[3] = '>';
    win_data_ptr->key_pagedown.str[4] = '>';
    win_data_ptr->key_pagedown.str_len = 5;

    ResetPageFlag(win_data_ptr);

    //page tips
    //close
    win_data_ptr->rect_page_tip = key_rect;

    return;
}


/*==============================================================================
Function Name: LoadSymbData
Description: load symbol and setting symbol data
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void LoadSymbData(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr)
{
    uint32 symb_cnt = 0;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return;
    }
    
    win_data_ptr->symb_cnt = 0;

    switch (win_data_ptr->symb_type)
    {
    case MMIIM_SP_SYMB_TYPE_FULL:

#if MMIIM_SP_SYMBOL_SUPPORT_FULL
        
        //1 标点符号
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_CH, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;

        //3 特殊字符
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_SPECIAL, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;
        //4 数字字符
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_NUM, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;
        //5 数学符号
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_MATH, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;
        //6 单位字符
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_UNIT, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;
        //7 拼音字符
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_PYIN, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;
#endif

        break;

    case MMIIM_SP_SYMB_TYPE_HINDI:

#if MMIIM_SP_SYMBOL_SUPPORT_HINDI
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_HINDI, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;

        //1 标点符号
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_EN, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;

        //2 标签单词
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_SIGN, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;
#endif

        break;

    case MMIIM_SP_SYMB_TYPE_EUR:

#if MMIIM_SP_SYMBOL_SUPPORT_EUR
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_EUR, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;

        //1 标点符号
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_EN, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;

        //2 标签单词
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_SIGN, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;
#endif

        break;

    default:
        //case MMIIM_SP_SYMB_TYPE_HALF:
        //set default as half symbols
        //1 标点符号
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_EN, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;

        //2 标签单词
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_SIGN, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;

#if MMIIM_SP_SYMBOL_SUPPORT_FACE
        //3 表情字符
        symb_cnt = MMIIM_SP_SYMB_CNT_LIMIT - win_data_ptr->symb_cnt;
        MMIIM_SP_SYMB_GetSymbols(MMIIM_SP_SYMBOL_TYPE_FACE, &symb_cnt,
                             win_data_ptr->symb_list + win_data_ptr->symb_cnt);
        win_data_ptr->symb_cnt += symb_cnt;
#endif

        break;
    }

    win_data_ptr->row_cnt = win_data_ptr->symb_cnt / win_data_ptr->col;

    if ((win_data_ptr->symb_cnt % win_data_ptr->col) > 0)
    {
        win_data_ptr->row_cnt += 1;
    }


    return;
}




/*==============================================================================
Function Name: HandleSymbWinMsg
Description: HandleSymbWinMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMI_RESULT_E HandleSymbWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr = PNULL;
    int16 hot_row = 0;
    int16 hot_col = 0;
    int16 short_cmt_idx = -1;

    win_data_ptr = MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return  res;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        win_data_ptr->win_id = win_id;
        InitData(win_data_ptr);
        break;

    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;

    case MSG_LOSE_FOCUS:
        //close
        MMK_CloseWin(win_id);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_FULL_PAINT:
        //draw
        DrawWin(win_data_ptr);
        break;
        
    case MSG_LCD_SWITCH:
        InitData(win_data_ptr);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //confirm hot
        CommitSymb(win_data_ptr, win_data_ptr->hot_idx);
        MMK_CloseWin(win_id);
        break;

    case MSG_KEYDOWN_RED:
        {
            if(!MMK_IsOpenWin(MAIN_IDLE_WIN_ID))
            {
                MMK_CloseWin(win_id);
            }
            else
            {
                return FALSE;
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_LEFT:

        if (win_data_ptr->hot_idx > 0)
        {
            win_data_ptr->hot_idx--;
            GetHotPos(win_data_ptr, &hot_row, &hot_col);
            //SCI_ASSERT(hot_row <= win_data_ptr->row); /*assert verified*/
            if(hot_row > win_data_ptr->row)
            {
                break;
            }
            
            if (hot_row < 0)
            {
                PageUp(win_data_ptr, MMIIM_SP_SYMB_HOT_END);
            }

            DrawWin(win_data_ptr);
        }

        break;

    case MSG_APP_RIGHT:

        if (win_data_ptr->hot_idx < win_data_ptr->symb_cnt - 1)
        {
            win_data_ptr->hot_idx++;
            GetHotPos(win_data_ptr, &hot_row, &hot_col);
            //SCI_ASSERT(hot_row <= win_data_ptr->row); /*assert verified*/
            if(hot_row > win_data_ptr->row)
            {
                break;
            }
            
            if (hot_row == win_data_ptr->row)
            {
                PageDown(win_data_ptr, MMIIM_SP_SYMB_HOT_BEGIN);
            }

            DrawWin(win_data_ptr);
        }

        break;

    case MSG_APP_UP:

        if (win_data_ptr->hot_idx >= win_data_ptr->col)
        {
            win_data_ptr->hot_idx -= win_data_ptr->col;
            GetHotPos(win_data_ptr, &hot_row, &hot_col);
            //SCI_ASSERT(hot_row <= win_data_ptr->row); /*assert verified*/
            if(hot_row > win_data_ptr->row)
            {
                break;
            }
            
            if (hot_row < 0)
            {
                PageUp(win_data_ptr, MMIIM_SP_SYMB_HOT_HOLD_COL);
            }

            DrawWin(win_data_ptr);
        }

        break;

    case MSG_APP_DOWN:

        if (win_data_ptr->hot_idx < win_data_ptr->symb_cnt - win_data_ptr->col)
        {
            win_data_ptr->hot_idx += win_data_ptr->col;
            GetHotPos(win_data_ptr, &hot_row, &hot_col);
            //SCI_ASSERT(hot_row <= win_data_ptr->row); /*assert verified*/
            if(hot_row > win_data_ptr->row)
            {
                break;
            }
            
            if (hot_row == win_data_ptr->row)
            {
                PageDown(win_data_ptr, MMIIM_SP_SYMB_HOT_HOLD_COL);
            }

            DrawWin(win_data_ptr);
        }

        break;
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_U:
#endif
    case MSG_APP_STAR:

        if (PageUp(win_data_ptr, MMIIM_SP_SYMB_HOT_HOLD))
        {
            DrawWin(win_data_ptr);
        }

        break;
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_J:
#endif
    case MSG_APP_HASH:

        if (PageDown(win_data_ptr, MMIIM_SP_SYMB_HOT_HOLD))
        {
            DrawWin(win_data_ptr);
        }

        break;
        
    //short confirm    
    case MSG_APP_1:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_R:
#endif
        short_cmt_idx = 0;
        break;
    case MSG_APP_2:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_T:
#endif
        short_cmt_idx = 1;
        break;
    case MSG_APP_3:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_Y:
#endif
        short_cmt_idx = 2;
        break;
    case MSG_APP_4:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_F:
#endif
        short_cmt_idx = 3;
        break;
    case MSG_APP_5:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_G:
#endif
        short_cmt_idx = 4;
        break;
    case MSG_APP_6:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_H:
#endif
        short_cmt_idx = 5;
        break;
    case MSG_APP_7:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_V:
#endif
        short_cmt_idx = 6;
        break;
    case MSG_APP_8:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_B:
#endif
        short_cmt_idx = 7;
        break;
    case MSG_APP_9:
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
    case MSG_APP_N:
#endif
        short_cmt_idx = 8;
        break;

    case MSG_APP_0:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        //choose hot
        res = HandleTpDown(win_data_ptr, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        res = HandleTpMove(win_data_ptr, param);
        //choose hot
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        //confirm
        res = HandleTpUp(win_data_ptr, param);
        break;
#endif

    default:
        res = MMI_RESULT_FALSE;
        break;
    }

    if (-1 != short_cmt_idx)
    {
        //3*3 or 3*4 的键盘
        uint32 short_col = short_cmt_idx % 3;
        uint32 short_row = short_cmt_idx / 3;
        uint32 cmt_idx = 0;

        cmt_idx = short_col;
        cmt_idx += short_row * win_data_ptr->col;
        cmt_idx += win_data_ptr->first_row * win_data_ptr->col;

        if (cmt_idx < win_data_ptr->symb_cnt)
        {
            CommitSymb(win_data_ptr, cmt_idx);
            MMK_CloseWin(win_id);
        }
    }

    return res;
}




#if (defined (TOUCH_PANEL_SUPPORT))
/*==============================================================================
Function Name: HandleTpDown
Description: HandleTpDown
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleTpDown(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    DPARAM param
)
{
    int32 tp_pos = 0;
    BOOLEAN is_win_dirty = FALSE;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param); /*assert verified*/
    if(PNULL == win_data_ptr ||PNULL == param )
    {
        return FALSE;
    }
            
    tp_pos = GetTpPos(win_data_ptr, param);

    if (0 <= tp_pos && tp_pos < (int32)win_data_ptr->symb_cnt)
    {
        if (win_data_ptr->hot_idx != tp_pos)
        {
            win_data_ptr->hot_idx = tp_pos;
            is_win_dirty = TRUE;
        }
    }
    else if (MMIIM_SP_SYMB_BOTTOM_IDX_PD == tp_pos)
    {
        is_win_dirty = MMIIM_SP_ComKeyPress(&win_data_ptr->key_pagedown);
        win_data_ptr->last_key_ptr = &win_data_ptr->key_pagedown;
    }
    else if (MMIIM_SP_SYMB_BOTTOM_IDX_PU == tp_pos)
    {
        is_win_dirty = MMIIM_SP_ComKeyPress(&win_data_ptr->key_pageup);
        win_data_ptr->last_key_ptr = &win_data_ptr->key_pageup;
    }

    if (is_win_dirty)
    {
        DrawWin(win_data_ptr);
    }


    return TRUE;
}





/*==============================================================================
Function Name: HandleTpMove
Description: HandleTpMove
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleTpMove(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    DPARAM param
)
{
    int32 tp_pos = 0;
    MMIIM_SP_COMMON_KEY_T *tp_key_ptr = PNULL;
    BOOLEAN is_win_dirty = FALSE;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param); /*assert verified*/
    if(PNULL == win_data_ptr ||PNULL == param )
    {
        return FALSE;
    }
    
    tp_pos = GetTpPos(win_data_ptr, param);

    if (0 <= tp_pos && tp_pos < (int32)win_data_ptr->symb_cnt)
    {
        if (win_data_ptr->hot_idx != tp_pos)
        {
            win_data_ptr->hot_idx = tp_pos;
            is_win_dirty = TRUE;
        }
    }
    else if (MMIIM_SP_SYMB_BOTTOM_IDX_PD == tp_pos)
    {
        is_win_dirty = MMIIM_SP_ComKeyPress(&win_data_ptr->key_pagedown);
        tp_key_ptr = &win_data_ptr->key_pagedown;
    }
    else if (MMIIM_SP_SYMB_BOTTOM_IDX_PU == tp_pos)
    {
        is_win_dirty = MMIIM_SP_ComKeyPress(&win_data_ptr->key_pageup);
        tp_key_ptr = &win_data_ptr->key_pageup;
    }

    if (win_data_ptr->last_key_ptr != tp_key_ptr && PNULL != win_data_ptr->last_key_ptr)
    {
        is_win_dirty = (is_win_dirty || MMIIM_SP_ComkeyRelease(win_data_ptr->last_key_ptr));
    }

    win_data_ptr->last_key_ptr = tp_key_ptr;

    if (is_win_dirty)
    {
        DrawWin(win_data_ptr);
    }


    return TRUE;
}





/*==============================================================================
Function Name: HandleTpUp
Description: HandleTpUp
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleTpUp(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    DPARAM param
)
{
    int32 tp_pos = 0;
    BOOLEAN is_win_closed = FALSE;
    BOOLEAN is_win_dirty = FALSE;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param); /*assert verified*/
    if(PNULL == win_data_ptr ||PNULL == param )
    {
        return FALSE;
    }
    
    tp_pos = GetTpPos(win_data_ptr, param);

    if (0 <= tp_pos && tp_pos < (int32)win_data_ptr->symb_cnt)
    {
        CommitSymb(win_data_ptr, tp_pos);
        MMK_CloseWin(win_data_ptr->win_id);
        is_win_closed = TRUE;
    }
    else if (MMIIM_SP_SYMB_BOTTOM_IDX_PD == tp_pos)
    {
        if (PageDown(win_data_ptr, MMIIM_SP_SYMB_HOT_HOLD))
        {
            is_win_dirty = TRUE;
        }
    }
    else if (MMIIM_SP_SYMB_BOTTOM_IDX_PU == tp_pos)
    {
        if (PageUp(win_data_ptr, MMIIM_SP_SYMB_HOT_HOLD))
        {
            is_win_dirty = TRUE;
        }
    }

    if (!is_win_closed && PNULL != win_data_ptr->last_key_ptr)
    {
        is_win_dirty = (is_win_dirty || MMIIM_SP_ComkeyRelease(win_data_ptr->last_key_ptr));
        win_data_ptr->last_key_ptr = PNULL;
    }


    if (is_win_dirty)
    {
        DrawWin(win_data_ptr);
    }


    return TRUE;
}



/*==============================================================================
Function Name: GetTpPos
Description: GetTpPos
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL int32 GetTpPos(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    DPARAM param
)
{
    GUI_POINT_T tp_point = {0};
    int32 pos = -1;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param); /*assert verified*/
    if(PNULL == win_data_ptr ||PNULL == param )
    {
        return pos;
    }
    
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    //按几率大小排列
    if (GUI_PointIsInRect(tp_point, win_data_ptr->rect_symb))
    {
        int16 rect_width = win_data_ptr->rect_symb.right - win_data_ptr->rect_symb.left + 1;
        int16 rect_height = win_data_ptr->rect_symb.bottom - win_data_ptr->rect_symb.top + 1;
        int16 dx = tp_point.x -  win_data_ptr->rect_symb.left;
        int16 dy = tp_point.y - win_data_ptr->rect_symb.top;
        int16 col = dx * win_data_ptr->col / rect_width;
        int16 row = dy * win_data_ptr->row / rect_height;

        //SCI_ASSERT(row < win_data_ptr->row); /*assert verified*/
        if(row >= win_data_ptr->row)
        {
            return -1;
        }
    
        pos = 0;
        pos += row * win_data_ptr->col;
        pos += col;

        //统一处理页面偏移
        pos += win_data_ptr->first_row * win_data_ptr->col;
    }
    else if (GUI_PointIsInRect(tp_point, win_data_ptr->key_pagedown.rect))
    {
        pos = MMIIM_SP_SYMB_BOTTOM_IDX_PD;
    }
    else if (GUI_PointIsInRect(tp_point, win_data_ptr->key_pageup.rect))
    {
        pos = MMIIM_SP_SYMB_BOTTOM_IDX_PU;
    }

    return pos;
}
#endif


/*==============================================================================
Function Name: DrawWin
Description: DrawWin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DrawWin(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr)
{
    MMI_STRING_T tip_info = {0};
    GUISTR_STYLE_T tip_style = {0};
    char c_tip_str[8] = {0}; //存放tip字符 the limit is (99/99)
    wchar tip_str[8] = {0}; //存放tip字符 the limit is (99/99)
    int32 i = 0;
    int16 unit_width = 0;
    int16 unit_height = 0;
    int16 rect_width = 0;
    int16 rect_height = 0;
    int16 row_idx = 0;
    int16 col_idx = 0;
    GUI_RECT_T hot_rect = {0};
    int16 hot_row = 0;
    int16 hot_col = 0;
    uint32 symb_first = 0;
    uint32 symb_per_page = 0;
    uint16 symb_idx = 0;
    uint16 symb_row = 0;
    uint16 symb_col = 0;
    wchar cur_symb = 0;
    GUI_RECT_T rect_symb = {0};
    MMI_STRING_T symb_info = {0};
    GUISTR_STYLE_T symb_style = {0};
    wchar symb_str[2] = {0}; //存放单字符
    MMI_STRING_T label_info = {0};
    GUISTR_STYLE_T label_style = {0};
    wchar label_str[1] = {0};


    if (PNULL == win_data_ptr)
    {
        return;
    }

    //draw...
    //draw panel
    LCD_FillRect(&(win_data_ptr->lcd_dev_info),
                 win_data_ptr->win_rect,
                 win_data_ptr->color_bg);

    //draw key
    MMIIM_SP_ComKeyDraw(&win_data_ptr->lcd_dev_info, &win_data_ptr->key_pageup);
    MMIIM_SP_ComKeyDraw(&win_data_ptr->lcd_dev_info, &win_data_ptr->key_pagedown);

    //draw page tip
    tip_style.font = win_data_ptr->font;
    tip_style.font_color = win_data_ptr->color_font;
    tip_style.align = ALIGN_HVMIDDLE;
    sprintf(c_tip_str, "(%d/%d)",
             win_data_ptr->first_row / win_data_ptr->row + 1,
             win_data_ptr->row_cnt / win_data_ptr->row + ((win_data_ptr->row_cnt % win_data_ptr->row) ? 1 : 0));
    for (i=0; i<8; i++) tip_str[i] = c_tip_str[i];
    tip_info.wstr_ptr = tip_str;
    tip_info.wstr_len = MMIAPICOM_Wstrlen(tip_str);
    GUISTR_DrawTextToLCDInRect(&win_data_ptr->lcd_dev_info,
                               &win_data_ptr->rect_page_tip,
                               &win_data_ptr->rect_page_tip,
                               &tip_info,
                               &tip_style,
                               GUISTR_STATE_NONE,
                               GUISTR_TEXT_DIR_AUTO);

    //cal row and column
    rect_width = win_data_ptr->rect_symb.right - win_data_ptr->rect_symb.left + 1;
    rect_height = win_data_ptr->rect_symb.bottom - win_data_ptr->rect_symb.top + 1;
    unit_width = rect_width / win_data_ptr->col;
    unit_height = rect_height / win_data_ptr->row;

    //draw rect
    LCD_DrawRect(&(win_data_ptr->lcd_dev_info),
                 win_data_ptr->rect_symb,
                 win_data_ptr->color_border);

    //draw grid
    for (row_idx = 1; row_idx < win_data_ptr->row; row_idx++)
    {
        LCD_DrawHLine(&(win_data_ptr->lcd_dev_info),
                      win_data_ptr->rect_symb.left,
                      win_data_ptr->rect_symb.top + unit_height*row_idx,
                      win_data_ptr->rect_symb.right,
                      win_data_ptr->color_border);
    }

    for (col_idx = 1; col_idx < win_data_ptr->col; col_idx++)
    {
        LCD_DrawVLine(&(win_data_ptr->lcd_dev_info),
                      win_data_ptr->rect_symb.left + unit_width*col_idx,
                      win_data_ptr->rect_symb.top,
                      win_data_ptr->rect_symb.bottom,
                      win_data_ptr->color_border);
    }

    //hot
    GetHotPos(win_data_ptr, &hot_row, &hot_col);

    //SCI_ASSERT(hot_row >= 0); /*assert verified*/
    if(hot_row < 0)
    {
        return;
    }
    
    hot_rect.top = win_data_ptr->rect_symb.top + unit_height * hot_row;
    hot_rect.left = win_data_ptr->rect_symb.left + unit_width * hot_col;
    hot_rect.bottom = (hot_row == win_data_ptr->row - 1) ?
                      win_data_ptr->rect_symb.bottom : (hot_rect.top + unit_height);
    hot_rect.right = (hot_col == win_data_ptr->col - 1) ?
                     win_data_ptr->rect_symb.right : (hot_rect.left + unit_width);
    hot_rect.top++;
    hot_rect.left++;
    hot_rect.bottom--;
    hot_rect.right--;

    LCD_FillRect(&(win_data_ptr->lcd_dev_info),
                 hot_rect,
                 win_data_ptr->color_hot);

    //draw symbol
    symb_first = win_data_ptr->first_row * win_data_ptr->col;

    symb_per_page = win_data_ptr->row * win_data_ptr->col;
    symb_style.font = win_data_ptr->font;
    symb_style.font_color = win_data_ptr->color_font;
    symb_style.align = ALIGN_HVMIDDLE;

    label_style.font = win_data_ptr->font_label;
    label_style.font_color = win_data_ptr->color_font;
    label_info.wstr_ptr = label_str;
    label_info.wstr_len = 1;

    for (symb_idx = 0; symb_idx < symb_per_page; symb_idx++)
    {
        //没有符号了，则不画了
        if (symb_first + symb_idx >= win_data_ptr->symb_cnt) break;

        symb_row = symb_idx / win_data_ptr->col;
        symb_col = symb_idx % win_data_ptr->col;
        GetSymbRect(win_data_ptr, symb_row, symb_col, &rect_symb);

        cur_symb = win_data_ptr->symb_list[symb_first + symb_idx];

        if (MMIIM_SP_SYMB_DEF_STR_START <= cur_symb && cur_symb < MMIIM_SP_SYMB_DEF_STR_END)
        {
            symb_info.wstr_ptr = MMIIM_SP_SYMB_GetSymbStrPtr(cur_symb); //lint !e605
            symb_info.wstr_len = MMIAPICOM_Wstrlen(symb_info.wstr_ptr);
            symb_style.font = win_data_ptr->font_min;
			//NEWMS00232164 [ST][6530][32X32_220X176BAR_QW][DE Multitap]字符输入时，“_”不显示
			symb_style.font = symb_style.font < rect_symb.bottom - rect_symb.top - 2 ? symb_style.font : rect_symb.bottom - rect_symb.top - 2;

            //string
            GUISTR_DrawTextToLCDInRect(&win_data_ptr->lcd_dev_info,
                                       &rect_symb,
                                       &rect_symb,
                                       &symb_info,
                                       &symb_style,
                                       GUISTR_STATE_NONE,
                                       GUISTR_TEXT_DIR_AUTO);            
        }
        else if (MMIIM_SP_SYMB_IMG_START<= cur_symb && cur_symb < MMIIM_SP_SYMB_IMG_MAX)
        {
            //image
            MMI_IMAGE_ID_T img_id = GetSymbImgeID(cur_symb);
            GUI_RECT_T img_rect = {0};
            uint16 image_width = 0;
            uint16 image_height = 0;
            GUIRES_GetImgWidthHeight(&image_width, &image_height, img_id, win_data_ptr->win_id);

            img_rect.left = rect_symb.left + (rect_symb.right - rect_symb.left - image_width)/2;
            img_rect.right = img_rect.left + image_width;
            img_rect.top = rect_symb.top + (rect_symb.bottom - rect_symb.top - image_height)/2;
            img_rect.bottom = img_rect.top + image_height;            

            GUIRES_DisplayImg(PNULL, &img_rect, PNULL, win_data_ptr->win_id, img_id, &win_data_ptr->lcd_dev_info);
        }
        else
        {
            symb_str[0] = cur_symb;
            symb_info.wstr_ptr = symb_str;
            symb_info.wstr_len = 1;
            symb_style.font = win_data_ptr->font;

            //string
            GUISTR_DrawTextToLCDInRect(&win_data_ptr->lcd_dev_info,
                                       &rect_symb,
                                       &rect_symb,
                                       &symb_info,
                                       &symb_style,
                                       GUISTR_STATE_NONE,
                                       GUISTR_TEXT_DIR_AUTO);
        }

        //label
        if (symb_row < 3 && symb_col < 3)
        {
            label_str[0] = symb_col + symb_row * 3 + '1';
            GUISTR_DrawTextToLCDInRect(&win_data_ptr->lcd_dev_info,
                                       &rect_symb,
                                       &rect_symb,
                                       &label_info,
                                       &label_style,
                                       GUISTR_STATE_NONE,
                                       GUISTR_TEXT_DIR_AUTO);
        }
    }

    return;
}


/*==============================================================================
Function Name: CommitSymb
Description: 提交当前窗口的索引所指定的符号串
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CommitSymb(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr, uint32 idx)
{
    wchar cur_symb = 0;
    wchar const *symb_str = PNULL;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return;
    }
    
    cur_symb = win_data_ptr->symb_list[idx];

    if (MMIIM_SP_SYMB_DEF_STR_START <= cur_symb && cur_symb < MMIIM_SP_SYMB_DEF_STR_END)
    {
        symb_str = MMIIM_SP_SYMB_GetSymbStrPtr(cur_symb);
        notify_pack.data.commit_data.commit_len = MMIAPICOM_Wstrlen(symb_str);
        SCI_MEMCPY(notify_pack.data.commit_data.commit, symb_str,
                   notify_pack.data.commit_data.commit_len * sizeof(wchar));
    }
    else if (MMIIM_SP_SYMB_IMG_START <= cur_symb && cur_symb < MMIIM_SP_SYMB_IMG_MAX)
    {
        symb_str = MMIIM_SP_SYMB_GetSymbStrPtr(cur_symb);
        notify_pack.data.commit_data.commit_len = 1;
        notify_pack.data.commit_data.commit[0] = *symb_str;
    }
    else
    {
        notify_pack.data.commit_data.commit[0] = cur_symb;
        notify_pack.data.commit_data.commit_len = 1;
    }

    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    GUIIM_CommitData(win_data_ptr->im_handle, &notify_pack);

    return;
}



/*==============================================================================
Function Name: GetHotPos
Description: GetHotPos
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 负行的修正
==============================================================================*/
LOCAL BOOLEAN GetHotPos(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    int16 *row_ptr,
    int16 *col_ptr
)
{
    int16 hot_idx;

    if (PNULL == win_data_ptr)
    {
        return FALSE;
    }

    hot_idx = win_data_ptr->hot_idx;
    hot_idx -= win_data_ptr->first_row * win_data_ptr->col;

    if (hot_idx < 0)
    {
        //如果是负的，则从-1行还是算，因为没有-0行
        hot_idx -= win_data_ptr->col;
    }

    *row_ptr = hot_idx / win_data_ptr->col;
    *col_ptr = hot_idx % win_data_ptr->col;

    return TRUE;
}



/*==============================================================================
Function Name: GetSymbRect
Description: 获取指定位置的背景框大小
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN GetSymbRect(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    int16 row,
    int16 col,
    GUI_RECT_T *rect_ptr
)
{
    int16 rect_width = 0;
    int16 rect_height = 0;
    int16 unit_width = 0;
    int16 unit_height = 0;

    if (row < 0 || row >= win_data_ptr->row
            || col < 0 || col >= win_data_ptr->col)
    {
        return FALSE;
    }

    rect_width = win_data_ptr->rect_symb.right - win_data_ptr->rect_symb.left + 1;
    rect_height = win_data_ptr->rect_symb.bottom - win_data_ptr->rect_symb.top + 1;
    unit_width = rect_width / win_data_ptr->col;
    unit_height = rect_height / win_data_ptr->row;

    rect_ptr->top = win_data_ptr->rect_symb.top + unit_height * row;
    rect_ptr->left = win_data_ptr->rect_symb.left + unit_width * col;
    rect_ptr->bottom = (row == win_data_ptr->row - 1) ?
                       win_data_ptr->rect_symb.bottom : (rect_ptr->top + unit_height);
    rect_ptr->right = (col == win_data_ptr->col - 1) ?
                      win_data_ptr->rect_symb.right : (rect_ptr->left + unit_width);

    rect_ptr->top++;
    rect_ptr->left++;
    rect_ptr->bottom--;
    rect_ptr->right--;

    return TRUE;
}



/*==============================================================================
Function Name: ResetPageFlag
Description: ResetPageFlag
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void ResetPageFlag(MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr)
{
    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return;
    }
    
    //set page up-down state
    win_data_ptr->key_pagedown.state = (win_data_ptr->first_row + win_data_ptr->row < win_data_ptr->row_cnt) ?
                                       MMIIM_SP_TP_KEY_STATE_ACTIVE : MMIIM_SP_TP_KEY_STATE_INACTIVE;

    win_data_ptr->key_pageup.state = (win_data_ptr->first_row > 0) ?
                                     MMIIM_SP_TP_KEY_STATE_ACTIVE : MMIIM_SP_TP_KEY_STATE_INACTIVE;

    return;
}



/*==============================================================================
Function Name: PageDown
Description: PageDown
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN PageDown(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    MMIIM_SP_SYMB_PG_CH_HOT_TYPE_T hot_type
)
{
    int16 page_cnt = 0;
    int16 cur_page = 0;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return  FALSE;
    }
    
    page_cnt = win_data_ptr->row_cnt / win_data_ptr->row;
    page_cnt += ((win_data_ptr->row_cnt % win_data_ptr->row) ? 1 : 0);

    //0 or 1 page
    if (page_cnt <= 1)
    {
        return FALSE;
    }

    cur_page = win_data_ptr->first_row / win_data_ptr->row;

    if (cur_page + 1 == page_cnt)
    {
        return FALSE;
    }

    win_data_ptr->first_row += win_data_ptr->row;

    switch (hot_type)
    {
    case MMIIM_SP_SYMB_HOT_BEGIN:
        //set default as end
        win_data_ptr->hot_idx = win_data_ptr->first_row * win_data_ptr->col;
        break;

    case MMIIM_SP_SYMB_HOT_HOLD:
        win_data_ptr->hot_idx += win_data_ptr->row * win_data_ptr->col;
        break;

    case MMIIM_SP_SYMB_HOT_HOLD_COL:
        win_data_ptr->hot_idx %= win_data_ptr->col;
        win_data_ptr->hot_idx += win_data_ptr->first_row * win_data_ptr->col;
        break;

    case MMIIM_SP_SYMB_HOT_END:
    default:
        //set default as end
        win_data_ptr->hot_idx = (win_data_ptr->first_row + win_data_ptr->row) * win_data_ptr->col;
        win_data_ptr->hot_idx -= 1;
        break;
    }

    if (win_data_ptr->hot_idx >= win_data_ptr->symb_cnt)
    {
        win_data_ptr->hot_idx = win_data_ptr->symb_cnt - 1;
    }

    ResetPageFlag(win_data_ptr);


    return TRUE;
}






/*==============================================================================
Function Name: PageUp
Description: PageUp
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN PageUp(
    MMIIM_SP_SYMB_WIN_DATA_T *win_data_ptr,
    MMIIM_SP_SYMB_PG_CH_HOT_TYPE_T hot_type
)
{
    int16 page_cnt = 0;
    int16 cur_page = 0;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return  FALSE;
    }
    
    page_cnt = win_data_ptr->row_cnt / win_data_ptr->row;
    page_cnt += ((win_data_ptr->row_cnt % win_data_ptr->row) ? 1 : 0);

    //0 or 1 page
    if (page_cnt <= 1)
    {
        return FALSE;
    }

    cur_page = win_data_ptr->first_row / win_data_ptr->row;

    if (0 == cur_page)
    {
        return FALSE;
    }

    win_data_ptr->first_row -= win_data_ptr->row;

    switch (hot_type)
    {
    case MMIIM_SP_SYMB_HOT_BEGIN:
        //set default as end
        win_data_ptr->hot_idx = win_data_ptr->first_row * win_data_ptr->col;
        break;

    case MMIIM_SP_SYMB_HOT_HOLD:
        win_data_ptr->hot_idx -= win_data_ptr->row * win_data_ptr->col;
        break;

    case MMIIM_SP_SYMB_HOT_HOLD_COL:
        win_data_ptr->hot_idx %= win_data_ptr->col;
        win_data_ptr->hot_idx += (win_data_ptr->first_row + win_data_ptr->row - 1) * win_data_ptr->col;
        break;


    case MMIIM_SP_SYMB_HOT_END:
    default:
        //set default as end
        win_data_ptr->hot_idx = (win_data_ptr->first_row + win_data_ptr->row) * win_data_ptr->col;
        win_data_ptr->hot_idx -= 1;
        break;
    }

    if (win_data_ptr->hot_idx >= win_data_ptr->symb_cnt)
    {
        win_data_ptr->hot_idx = win_data_ptr->symb_cnt - 1;
    }

    ResetPageFlag(win_data_ptr);


    return TRUE;
}

/*==============================================================================
Function Name: GetSymbImgeID
Description: 获取对应的图片
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 
==============================================================================*/
LOCAL MMI_IMAGE_ID_T GetSymbImgeID(wchar symb)
{
    MMI_IMAGE_ID_T img_id = 0;
    
    if (MMIIM_SP_SYMB_IMG_START <= symb && symb < MMIIM_SP_SYMB_IMG_MAX)
    {
        img_id = symb_img_id[symb - MMIIM_SP_SYMB_IMG_START];
    }

    return img_id;
}


/*==============================================================================
Function Name: MMIIM_SP_CreateSymbWin
Description: MMIIM_SP_CreateSymbWin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
MMI_HANDLE_T MMIIM_SP_SymbCreateWin(MMI_HANDLE_T im_handle, MMIIM_SP_SYMB_TYPE_T type)
{
    return SymbCreateWin(im_handle, type);
}


/*==============================================================================
Description: 根据输入语言选择打开的符号面板
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
MMI_HANDLE_T MMIIM_SP_SymbCreateWinByLang(MMI_HANDLE_T im_handle, GUIIM_LANGUAGE_T lang)
{
    return SymbCreateWin(im_handle, GetType(lang));
}


/*==============================================================================
Function Name: MMIIM_SP_GetSymbType
Description: 通过输入法获取符号类型
Author: Haiwu.Chen
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC MMIIM_SP_SYMB_TYPE_T MMIIM_GetSymbType(GUIIM_METHOD_T method)
{    
    MMIIM_SP_SYMB_TYPE_T symb_type = MMIIM_SP_SYMB_TYPE_NONE;
    
    if (GUIIM_M_TP_PINYIN == method ||
        GUIIM_M_TP_STROKE == method ||
        GUIIM_M_TP_ZHUYIN == method ||
        GUIIM_M_TP_STROKE_CHT == method ||
        GUIIM_M_TPQ_PINYIN == method ||
        GUIIM_M_TPQ_STROKE == method ||
        GUIIM_M_TPQ_ZHUYIN == method ||
        GUIIM_M_TPQ_STROKE_CHT == method ||
        GUIIM_M_PINYIN == method ||
        GUIIM_M_STROKE == method ||
        GUIIM_M_ZHUYIN == method ||
        GUIIM_M_STROKE_CHT == method ||
        GUIIM_M_Q_PINYIN == method ||
        GUIIM_M_Q_STROKE == method ||
        GUIIM_M_Q_ZHUYIN == method ||
        GUIIM_M_Q_STROKE_CHT == method)
    {
        symb_type = MMIIM_SP_SYMB_TYPE_FULL;
    }
    else
    {
        symb_type = MMIIM_SP_SYMB_TYPE_HALF;
    }
    
    return symb_type;
}

#if (defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT)
/*==============================================================================
Function Name: MMIIM_ConvertToFullSymbol
Description: 如果存在全角符号，将半角符号转换为全角符号函数，如果对应的
                    符号不在此函数列，则返回原值
Author: yaoguang.chen
Global resource dependence: 应该仅仅中文存在全角符号吧
Note:
==============================================================================*/
PUBLIC wchar MMIIM_ConvertToFullSymbol (wchar symbol)
{
    wchar full_symbol = symbol;

    switch (symbol)
    {
        case ',':
            full_symbol = 0xff0c;
            break;

        case '.':
            full_symbol = 0x3002;
            break;

        case '?':
            full_symbol = 0xff1f;
            break;

        case '(':
            full_symbol = 0xff08;
            break;

        case ')':
            full_symbol = 0xff09;
            break;

        case '"':
            full_symbol = 0x201c;
            break;

        case ';':
            full_symbol = 0xff1b;
            break;

        case ':':
            full_symbol = 0xff1a;
            break;

        case '\\':
            full_symbol = 0x3001;
            break;
            
        default:
            break;
    }
    
    return full_symbol;
}
#endif




/*Edit by script, ignore 3 case. Thu Apr 26 19:00:56 2012*/ //IGNORE9527
