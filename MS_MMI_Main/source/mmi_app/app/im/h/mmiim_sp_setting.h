/*==============================================================================
File Name: mmiim_sp_setting.h
Author: Lianxiang.Zhou
Date: 2010/06/03
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/06/03   Lianxiang.Zhou  Create.
==============================================================================*/

#ifndef _MMIIM_SP_SETTING_H_
#define _MMIIM_SP_SETTING_H_


#include "mmi_font.h"
#include "mmiim_sp_tp_basic.h"


//const define
enum
{
    MMIIM_SP_SETTING_COLOR_BG = 0x2104,
    MMIIM_SP_SETTING_COLOR_BORDER = 0xc618,
    MMIIM_SP_SETTING_COLOR_FONT = 0xe71c,
    MMIIM_SP_SETTING_COLOR_HOT = 0xfcc0,
    MMIIM_SP_SETTING_FONT = SONG_FONT_14,
    //item min width
    MMIIM_SP_SETTING_ITEM_WIDTH = 48,
    MMIIM_SP_SETTING_ITEM_MARGIN = 20,
    //item default height
    MMIIM_SP_SETTING_ITEM_HEIGHT = 24,
};


//menu item
typedef struct
{
    wchar const *disp_str;
    size_t str_len;
} MMIIM_SP_SETTING_ITEM_T;


typedef void (*MMIIM_SP_SETTING_HANDLE_FUNC_T)(void* data_ptr, int32 idx);


//win
typedef struct
{
    MMI_HANDLE_T im_handle;
    GUI_LCD_DEV_INFO lcd_dev_info;

    MMI_WIN_ID_T win_id;

    void* user_data_ptr; //窗口的特有数据
    MMIIM_SP_SETTING_HANDLE_FUNC_T handle_func;    

    //panel
    GUI_RECT_T win_rect;
    GUI_RECT_T menu_rect;
    MMIIM_SP_SETTING_ITEM_T *item_ptr;
    int16 item_cnt;
    int16 item_hot;
    
    //color
    GUI_COLOR_T color_bg;
    GUI_COLOR_T color_border;
    GUI_COLOR_T color_font;
    GUI_COLOR_T color_hot;

    GUI_FONT_T font;
} MMIIM_SP_SETTING_WIN_DATA_T;



#endif /*_MMIIM_SP_SETTING_H_*/


