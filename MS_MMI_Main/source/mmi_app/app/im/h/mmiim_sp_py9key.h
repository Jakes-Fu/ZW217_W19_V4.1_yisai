/*==============================================================================
File Name: mmiim_sp_py9key.h
Author: Lianxiang.Zhou
Date: 2010/05/18
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/18   Lianxiang.Zhou  Create.
==============================================================================*/

#ifndef _MMIIM_SP_PY9KEY_H_
#define _MMIIM_SP_PY9KEY_H_


#include "mmiim_sp_tp_basic.h"

#if defined(MMIIM_USE_SPIM_KEY)

#include "spim_ch_py9k.h"



#define MMIIM_SP_PY9K_PY_PAGE_MAX (24)



typedef struct
{
    SPIM_UNICODE_T py_choice[SPIM_PY_CHOICE_LEN_MAX];
    uint32 py_cnt;
    uint8 page_cnt;
    uint8 cur_page;
    struct {
        uint8 offset; //该页字符的起点
        uint8 cnt; //该页拼音的个数
    } page_info[MMIIM_SP_PY9K_PY_PAGE_MAX];
}MMIIM_SP_PY9K_PY_DATA_T;

typedef struct
{
    GUIIM_INIT_KERNEL_T init_param;
    
    SPIM_HANDLE_T core_handle;
    MMIIM_SP_STATE_T state;

    //记录首次按键的时候的按下键
    MMIIM_SP_VKEY_T last_key;

    GUI_RECT_T rect;
    GUI_RECT_T bar_rect_0;
    GUI_RECT_T bar_rect_1;
    GUI_RECT_T bar_rect_2;
    int32 is_bar_hide;

    wchar asso_chr[MMIIM_SP_ASSO_CNT_MAX];
    MMIIM_SP_BAR_DATA_T asso;
    MMIIM_SP_PY9K_PY_DATA_T py_cand;

    MMIIM_SP_TEXT_BAR_T bar_edit;
    MMIIM_SP_TEXT_BAR_T bar_pyin;
    MMIIM_SP_TEXT_BAR_T bar_cand;

    //draw flag
    BOOLEAN is_rect_change;
    BOOLEAN is_bar_edit_dirty;
    BOOLEAN is_bar_pyin_dirty;
    BOOLEAN is_bar_cand_dirty;
}MMIIM_SP_CHPY9K_DATA_T;




#endif //MMIIM_USE_SPIM_KEY
#endif //_MMIIM_SP_PY9KEY_H_



