/*==============================================================================
File Name: mmiim_sp_en9key.h
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
#ifndef _MMIIM_SP_EN9KEY_H_
#define _MMIIM_SP_EN9KEY_H_


#include "mmiim_sp_tp_basic.h"

#if defined(MMIIM_USE_SPIM_KEY)

#include "spim_english9k.h"


typedef struct
{
    GUIIM_INIT_KERNEL_T init_param;
    
    SPIM_HANDLE_T core_handle;
    MMIIM_SP_STATE_T state;

    SPIM_CAPITAL_MODE_T cap_mode;

    //记录首次按键的时候的按下键
    MMIIM_SP_VKEY_T last_key;

    GUI_RECT_T rect;
    
    MMIIM_SP_TEXT_BAR_T bar_edit;
    MMIIM_SP_TEXT_BAR_T bar_cand;

    //draw flag
    BOOLEAN is_rect_change;
    BOOLEAN is_bar_edit_dirty;
    BOOLEAN is_bar_cand_dirty;
}MMIIM_SP_EN9K_DATA_T;


#endif //MMIIM_USE_SPIM_KEY
#endif //_MMIIM_SP_EN9KEY_H_



