/*==============================================================================
File Name: mmiim_sp_tp_setting.c
Author: Lianxiang.Zhou
Date: 2010/06/02
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/06/02   Lianxiang.Zhou  Create.
==============================================================================*/

#ifdef WIN32
#include "std_header.h"
#endif

#include "window_parse.h"
#include "guistring.h"
#include "guiblock.h"
#include "mmitheme_pos.h"
#include "mmiim_id.h"
#include "mmiim_sp_setting.h"



/*==============================================================================
Function Name: HandleSettingWinMsg
Description: HandleSettingWinMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMI_RESULT_E HandleSettingWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

/*==============================================================================
Function Name: HandleTpDown
Description: HandleTpDown
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleTpDown(
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr,
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
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr,
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
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr,
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
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr,
    DPARAM param
);

/*==============================================================================
Function Name: CloseWin
Description: CloseWin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CloseWin(MMI_WIN_ID_T win_id);

/*==============================================================================
Function Name: DrawWin
Description: DrawWin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DrawWin(MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr);


/*==============================================================================
Function Name: TpSettingHandle
Description: TpSettingHandle
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TpSettingHandle(void* data_ptr, int32 idx);

/*==============================================================================
Function Name: KbSettingHandle
Description: KbSettingHandle
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbSettingHandle(void* data_ptr, int32 idx);





WINDOW_TABLE(MMIIM_SP_SETTING_WIN_TAB) =
{
    WIN_ID(MMIIM_SP_TP_SETTING_WIN_ID),
    WIN_FUNC((uint32)HandleSettingWinMsg),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    END_WIN
};


/*==============================================================================
Function Name: HandleSettingWinMsg
Description: HandleSettingWinMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL MMI_RESULT_E HandleSettingWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr = PNULL;

    win_data_ptr = MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return res;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        win_data_ptr->win_id = win_id;
        //draw bg,只需要画一次，且只能画一次
        LCD_MistBmp(&(win_data_ptr->lcd_dev_info), win_data_ptr->win_rect, win_data_ptr->color_bg);
       break;

    case MSG_CLOSE_WINDOW:
        //free data
        if(PNULL != win_data_ptr->user_data_ptr)
        {
            SCI_FREE(win_data_ptr->user_data_ptr);
        }
        
        if(PNULL != win_data_ptr->item_ptr)
        {
            SCI_FREE(win_data_ptr->item_ptr);
        }
        
        SCI_FREE(win_data_ptr);
        break;

    case MSG_LOSE_FOCUS:
        //close
        CloseWin(win_id);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_FULL_PAINT:
        //draw
        DrawWin(win_data_ptr);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //set im
        win_data_ptr->handle_func(win_data_ptr, win_data_ptr->item_hot);
        CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        CloseWin(win_id);
        break;

    case MSG_APP_LEFT:
        break;

    case MSG_APP_RIGHT:
        break;

    case MSG_APP_UP:

        if (win_data_ptr->item_hot > 0)
        {
            win_data_ptr->item_hot -= 1;
            DrawWin(win_data_ptr);
        }

        break;

    case MSG_APP_DOWN:

        if (win_data_ptr->item_hot < win_data_ptr->item_cnt - 1)
        {
            win_data_ptr->item_hot += 1;
            DrawWin(win_data_ptr);
        }

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
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        res = MMI_RESULT_FALSE;
        break;
    }

    return res;
}





/*==============================================================================
Function Name: HandleTpDown
Description: HandleTpDown
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL BOOLEAN HandleTpDown(
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr,
    DPARAM param
)
{
    int32 tp_pos = 0;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param); /*assert verified*/
    if(PNULL == win_data_ptr||PNULL == param)
    {
        return FALSE;
    }
    
    tp_pos = GetTpPos(win_data_ptr, param);

    if (0 <= tp_pos && tp_pos < win_data_ptr->item_cnt
            && win_data_ptr->item_hot != tp_pos)
    {
        win_data_ptr->item_hot = tp_pos;
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
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr,
    DPARAM param
)
{
    int32 tp_pos = 0;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param); /*assert verified*/
    if(PNULL == win_data_ptr||PNULL == param)
    {
        return FALSE;
    }
    
    tp_pos = GetTpPos(win_data_ptr, param);

    if (0 <= tp_pos && tp_pos < win_data_ptr->item_cnt
            && win_data_ptr->item_hot != tp_pos)
    {
        win_data_ptr->item_hot = tp_pos;
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
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr,
    DPARAM param
)
{
    int32 tp_pos = 0;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param); /*assert verified*/
    if(PNULL == win_data_ptr||PNULL == param)
    {
        return FALSE;
    }
    
    tp_pos = GetTpPos(win_data_ptr, param);

    win_data_ptr->handle_func(win_data_ptr, tp_pos);
    CloseWin(win_data_ptr->win_id);

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
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr,
    DPARAM param
)
{
    GUI_POINT_T tp_point = {0};
    int32 pos = -1;

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != param); /*assert verified*/
    if(PNULL == win_data_ptr||PNULL == param)
    {
        return pos;
    }
    
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    if (GUI_PointIsInRect(tp_point, win_data_ptr->menu_rect))
    {
        int16 rect_height = win_data_ptr->menu_rect.bottom - win_data_ptr->menu_rect.top + 1;
        int16 dy = tp_point.y - win_data_ptr->menu_rect.top;

        pos = dy * win_data_ptr->item_cnt / rect_height;
        SCI_ASSERT(pos < win_data_ptr->item_cnt); /*assert verified*/
    }


    return pos;
}



/*==============================================================================
Function Name: CloseWin
Description: CloseWin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void CloseWin(MMI_WIN_ID_T win_id)
{
 #if 0
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr = PNULL;

    win_data_ptr = MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return;
    }
    
    //free data
    SCI_FREE(win_data_ptr->user_data_ptr);
    SCI_FREE(win_data_ptr->item_ptr);
    SCI_FREE(win_data_ptr);
#endif
    //close win
    MMK_CloseWin(win_id);

    return;
}


/*==============================================================================
Function Name: DrawWin
Description: DrawWin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DrawWin(MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr)
{
    int16 i = 0;
    int16 rect_width = 0;
    int16 rect_height = 0;
    int16 unit_width = 0;
    int16 unit_height = 0;
    int16 item_idx = 0;
    GUI_RECT_T menu_rect = {0};
    GUI_RECT_T item_rect = {0};
    MMI_STRING_T item_str_info = {0};
    GUISTR_STYLE_T item_str_style = {0};


    if (PNULL == win_data_ptr)
    {
        return;
    }

    //draw bg - remove to msg_open_win
    //LCD_MistBmp(&(win_data_ptr->lcd_dev_info), win_data_ptr->win_rect, win_data_ptr->color_bg);

    //draw...
    //draw panel
    LCD_FillRect(&(win_data_ptr->lcd_dev_info),
                 win_data_ptr->menu_rect,
                 win_data_ptr->color_bg);

    //draw rect
    //加粗边框
    menu_rect = win_data_ptr->menu_rect;

    for (i = 0; i < 3; i++)
    {
        menu_rect.top++;
        menu_rect.left++;
        menu_rect.bottom--;
        menu_rect.right--;
        LCD_DrawRect(&(win_data_ptr->lcd_dev_info),
                     menu_rect,
                     win_data_ptr->color_border);
    }

    //cal row and column
    rect_width = menu_rect.right - menu_rect.left + 1;
    rect_height = menu_rect.bottom - menu_rect.top + 1;
    unit_width = rect_width;
    unit_height = rect_height / win_data_ptr->item_cnt;

    //draw grid
    for (item_idx = 1; item_idx < win_data_ptr->item_cnt; item_idx++)
    {
        LCD_DrawHLine(&(win_data_ptr->lcd_dev_info),
                      menu_rect.left,
                      menu_rect.top + item_idx * unit_height,
                      menu_rect.right,
                      win_data_ptr->color_border);
    }

    item_rect.left = menu_rect.left + 1;
    item_rect.right = menu_rect.right - 1;
    item_str_style.font = win_data_ptr->font;
    item_str_style.font_color = win_data_ptr->color_font;
    item_str_style.align = ALIGN_HVMIDDLE;

    //draw item
    for (item_idx = 0; item_idx < win_data_ptr->item_cnt; item_idx++)
    {
        item_rect.top = menu_rect.top + item_idx * unit_height + 1;
        item_rect.bottom = item_rect.top + unit_height - 2;

        if (item_idx == win_data_ptr->item_hot)
        {
            if (item_idx == win_data_ptr->item_cnt - 1)
            {
                item_rect.bottom = menu_rect.bottom - 1;
            }

            LCD_FillRect(&(win_data_ptr->lcd_dev_info),
                         item_rect,
                         win_data_ptr->color_hot);
        }

        item_str_info.wstr_ptr = win_data_ptr->item_ptr[item_idx].disp_str; /*lint !e605*/
        item_str_info.wstr_len = win_data_ptr->item_ptr[item_idx].str_len;

        //string
        GUISTR_DrawTextToLCDInRect(&win_data_ptr->lcd_dev_info,
                                   &item_rect,
                                   &item_rect,
                                   &item_str_info,
                                   &item_str_style,
                                   GUISTR_STATE_SINGLE_LINE,
                                   GUISTR_TEXT_DIR_AUTO);


    }


    return;
}



////////////////////////////////////////////////////////////////////////////////
//创建输入法设置菜单窗口
//data of tp setting
//全屏手写输入
//拼音输入(9/26)
//笔画输入
//En输入(9/26)
//abc输入(9/26)
//数字
#define MMIIM_SP_SETTING_TP_ITEM_MAX (6)

//string
static wchar const *tp_item_disp_str[] =
{
    L"\x5168\x5c4f\x624b\x5199\x8f93\x5165",
    L"\x62fc\x97f3\x8f93\x5165\x0028\x0039\x002f\x0032\x0036\x0029",
    L"\x7b14\x753b\x8f93\x5165",
    L"\x0045\x006e\x8f93\x5165\x0028\x0039\x002f\x0032\x0036\x0029",
    L"\x0061\x0062\x0063\x8f93\x5165\x0028\x0039\x002f\x0032\x0036\x0029",
    L"\x6570\x5b57",
};

//im list
static GUIIM_METHOD_T const tp_im_list[] =
{
    GUIIM_M_HANDWRITING,
    GUIIM_M_TP_PINYIN,
    GUIIM_M_TP_STROKE,
    GUIIM_M_TP_ENGLISH,
    GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER,
    GUIIM_M_TP_123,
};


/*==============================================================================
Function Name: MMIIM_SP_TpSettingCreateWin
Description: 创建输入法设置菜单窗口
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
MMI_HANDLE_T MMIIM_SP_TpSettingCreateWin(MMI_HANDLE_T im_handle)
{
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr = PNULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    int32 i = 0;
    uint16 str_len = 0;
    uint16 max_str_len = MMIIM_SP_SETTING_ITEM_WIDTH;

    win_data_ptr = (MMIIM_SP_SETTING_WIN_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SP_SETTING_WIN_DATA_T));
    SCI_MEMSET(win_data_ptr, 0, sizeof(MMIIM_SP_SETTING_WIN_DATA_T));

    win_data_ptr->im_handle = im_handle;
    win_data_ptr->lcd_dev_info = lcd_dev_info;

    win_data_ptr->user_data_ptr = PNULL;
    win_data_ptr->handle_func = TpSettingHandle;

    win_data_ptr->color_bg = MMIIM_SP_SETTING_COLOR_BG;
    win_data_ptr->color_border = MMIIM_SP_SETTING_COLOR_BORDER;
    win_data_ptr->color_font = MMIIM_SP_SETTING_COLOR_FONT;
    win_data_ptr->color_hot = MMIIM_SP_SETTING_COLOR_HOT;

    win_data_ptr->font = MMIIM_SP_SETTING_FONT;

    win_data_ptr->item_cnt = MMIIM_SP_SETTING_TP_ITEM_MAX;
    win_data_ptr->item_hot = 0;
    win_data_ptr->item_ptr = (MMIIM_SP_SETTING_ITEM_T *)SCI_ALLOC_APP(MMIIM_SP_SETTING_TP_ITEM_MAX * sizeof(MMIIM_SP_SETTING_ITEM_T));

    for (i = 0; i < MMIIM_SP_SETTING_TP_ITEM_MAX; i++)
    {
        win_data_ptr->item_ptr[i].disp_str = tp_item_disp_str[i];
        win_data_ptr->item_ptr[i].str_len = MMIAPICOM_Wstrlen(tp_item_disp_str[i]);
        str_len = GUI_GetStringWidth(win_data_ptr->font,
                                     win_data_ptr->item_ptr[i].disp_str,
                                     win_data_ptr->item_ptr[i].str_len);

        if (str_len > max_str_len) max_str_len = str_len;
    }

    //无标题
    win_data_ptr->win_rect = MMITHEME_GetClientRect();
    win_data_ptr->win_rect.top = 0;

    win_data_ptr->menu_rect = win_data_ptr->win_rect;
    win_data_ptr->menu_rect.top = win_data_ptr->menu_rect.bottom - win_data_ptr->item_cnt * MMIIM_SP_SETTING_ITEM_HEIGHT + 1;
    win_data_ptr->menu_rect.right = win_data_ptr->menu_rect.left + max_str_len + MMIIM_SP_SETTING_ITEM_MARGIN;

    if (win_data_ptr->menu_rect.right > win_data_ptr->win_rect.right)
    {
        win_data_ptr->menu_rect.right = win_data_ptr->win_rect.right;
    }


    return MMK_CreateWin((uint32*)MMIIM_SP_SETTING_WIN_TAB, win_data_ptr);
}


/*==============================================================================
Function Name: TpSettingHandle
Description: TpSettingHandle
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void TpSettingHandle(void* data_ptr, int32 idx)
{
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr = data_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return;
    }
    
    if (idx < 0 || idx >= win_data_ptr->item_cnt)
    {
        return;
    }

    notify_pack.data.set_method = tp_im_list[idx];
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_CommitData(win_data_ptr->im_handle, &notify_pack);

    return;
}




////////////////////////////////////////////////////////////////////////////////
//创建输入法设置菜单窗口
//data of kb setting
//拼音
//笔画
//En
//en
//EN
//Abc
//abc
//ABC
//123
#define MMIIM_SP_SETTING_ITEM_MAX (9)

//string
static wchar const *kb_item_disp_str[] =
{
    L"\x62fc\x97f3",
    L"\x7b14\x753b",
    L"En",
    L"en",
    L"EN",
    L"Abc",
    L"abc",
    L"ABC",
    L"123",
};

//im list
static GUIIM_METHOD_T const kb_im_list[] =
{
    GUIIM_M_PINYIN,
    GUIIM_M_STROKE,
    GUIIM_M_ENGLISH_LEAD_UPPER,
    GUIIM_M_ENGLISH_LOWER,
    GUIIM_M_ENGLISH_UPPER,
    GUIIM_M_ABC_LEAD_UPPER,
    GUIIM_M_ABC_LOWER,
    GUIIM_M_ABC_UPPER,
    GUIIM_M_123,
};


/*==============================================================================
Function Name: MMIIM_SP_SettingCreateWin
Description: MMIIM_SP_SettingCreateWin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
MMI_HANDLE_T MMIIM_SP_SettingCreateWin(MMI_HANDLE_T im_handle)
{
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr = PNULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    int32 i = 0;
    uint16 str_len = 0;
    uint16 max_str_len = MMIIM_SP_SETTING_ITEM_WIDTH;

    win_data_ptr = (MMIIM_SP_SETTING_WIN_DATA_T*)SCI_ALLOC_APP(sizeof(MMIIM_SP_SETTING_WIN_DATA_T));
    SCI_MEMSET(win_data_ptr, 0, sizeof(MMIIM_SP_SETTING_WIN_DATA_T));

    win_data_ptr->im_handle = im_handle;
    win_data_ptr->lcd_dev_info = lcd_dev_info;

    win_data_ptr->user_data_ptr = PNULL;
    win_data_ptr->handle_func = KbSettingHandle;

    win_data_ptr->color_bg = MMIIM_SP_SETTING_COLOR_BG;
    win_data_ptr->color_border = MMIIM_SP_SETTING_COLOR_BORDER;
    win_data_ptr->color_font = MMIIM_SP_SETTING_COLOR_FONT;
    win_data_ptr->color_hot = MMIIM_SP_SETTING_COLOR_HOT;

    win_data_ptr->font = MMIIM_SP_SETTING_FONT;

    win_data_ptr->item_cnt = MMIIM_SP_SETTING_ITEM_MAX;
    win_data_ptr->item_hot = 0;
    win_data_ptr->item_ptr = (MMIIM_SP_SETTING_ITEM_T *)SCI_ALLOC_APP(MMIIM_SP_SETTING_ITEM_MAX * sizeof(MMIIM_SP_SETTING_ITEM_T));

    for (i = 0; i < MMIIM_SP_SETTING_ITEM_MAX; i++)
    {
        win_data_ptr->item_ptr[i].disp_str = kb_item_disp_str[i];
        win_data_ptr->item_ptr[i].str_len = MMIAPICOM_Wstrlen(kb_item_disp_str[i]);
        str_len = GUI_GetStringWidth(win_data_ptr->font,
                                     win_data_ptr->item_ptr[i].disp_str,
                                     win_data_ptr->item_ptr[i].str_len);

        if (str_len > max_str_len) max_str_len = str_len;
    }

    //无标题
    win_data_ptr->win_rect = MMITHEME_GetClientRect();
    win_data_ptr->win_rect.top = 0;

    win_data_ptr->menu_rect = win_data_ptr->win_rect;
    win_data_ptr->menu_rect.top = win_data_ptr->menu_rect.bottom - win_data_ptr->item_cnt * MMIIM_SP_SETTING_ITEM_HEIGHT + 1;
    win_data_ptr->menu_rect.left = win_data_ptr->menu_rect.right - (max_str_len + MMIIM_SP_SETTING_ITEM_MARGIN);

    if (win_data_ptr->menu_rect.left < win_data_ptr->win_rect.left)
    {
        win_data_ptr->menu_rect.right = win_data_ptr->win_rect.left;
    }


    return MMK_CreateWin((uint32*)MMIIM_SP_SETTING_WIN_TAB, win_data_ptr);
}


/*==============================================================================
Function Name: KbSettingHandle
Description: KbSettingHandle
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbSettingHandle(void* data_ptr, int32 idx)
{
    MMIIM_SP_SETTING_WIN_DATA_T *win_data_ptr = data_ptr;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    //SCI_ASSERT(PNULL != win_data_ptr); /*assert verified*/
    if(PNULL == win_data_ptr)
    {
        return;
    }
    
    if (idx < 0 || idx >= win_data_ptr->item_cnt)
    {
        return;
    }

    notify_pack.data.set_method = kb_im_list[idx];
    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    GUIIM_CommitData(win_data_ptr->im_handle, &notify_pack);

    return;
}









/*Edit by script, ignore 3 case. Thu Apr 26 19:00:56 2012*/ //IGNORE9527
