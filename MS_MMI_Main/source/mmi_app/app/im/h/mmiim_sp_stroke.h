/*==============================================================================
File Name: mmiim_sp_stroke.h
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



#ifndef _MMIIM_SP_STROKE_H_
#define _MMIIM_SP_STROKE_H_

#include "mmiim_sp_tp_basic.h"

#if defined(MMIIM_USE_SPIM_KEY)

#include "spim_ch_stroke.h"


//定义笔划所对应的按键
enum
{
    MMIIM_SP_STROKE_9KEY_1 = MMIIM_SP_KEY_1,
    MMIIM_SP_STROKE_9KEY_2 = MMIIM_SP_KEY_2,
    MMIIM_SP_STROKE_9KEY_3 = MMIIM_SP_KEY_3,
    MMIIM_SP_STROKE_9KEY_4 = MMIIM_SP_KEY_4,
    MMIIM_SP_STROKE_9KEY_5 = MMIIM_SP_KEY_5,
    MMIIM_SP_STROKE_9KEY_6 = MMIIM_SP_KEY_6,

    //这个要具体的看键盘设置了，暂定PC键盘中央的6个
    MMIIM_SP_STROKE_26KEY_1 = MMIIM_SP_KEY_V1,
    MMIIM_SP_STROKE_26KEY_2 = MMIIM_SP_KEY_V2,
    MMIIM_SP_STROKE_26KEY_3 = MMIIM_SP_KEY_V3,
    MMIIM_SP_STROKE_26KEY_4 = MMIIM_SP_KEY_V4,
    MMIIM_SP_STROKE_26KEY_5 = MMIIM_SP_KEY_V5,
    MMIIM_SP_STROKE_26KEY_6 = MMIIM_SP_KEY_V6,
};


//ui data
typedef struct
{
    GUIIM_INIT_KERNEL_T init_param;

    SPIM_HANDLE_T core_handle;
    MMIIM_SP_STATE_T state;

    //记录首次按键的时候的按下键
    MMIIM_SP_VKEY_T last_key;

    GUI_RECT_T rect;

    //显示相关的
    MMIIM_SP_TEXT_BAR_T bar_edit;
    MMIIM_SP_TEXT_BAR_T bar_cand;

    wchar asso_chr[MMIIM_SP_ASSO_CNT_MAX];
    MMIIM_SP_BAR_DATA_T asso;

    //draw flag
    BOOLEAN is_rect_change;
    BOOLEAN is_bar_edit_dirty;
    BOOLEAN is_bar_cand_dirty;
}MMIIM_SP_STROKE_DATA_T;



#endif //MMIIM_USE_SPIM_KEY
#endif //_MMIIM_SP_STROKE_H_




