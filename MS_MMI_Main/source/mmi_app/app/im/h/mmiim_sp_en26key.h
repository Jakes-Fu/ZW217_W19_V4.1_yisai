/*==============================================================================
File Name: mmiim_sp_en26key.h
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
#if (!defined KEYPAD_TYPE_TYPICAL_KEYPAD ||defined  MMI_IM_PDA_SUPPORT)
#ifndef _MMIIM_SP_EN26KEY_H_
#define _MMIIM_SP_EN26KEY_H_


#include "mmiim_sp_tp_basic.h"

#if defined(MMIIM_USE_SPIM_QWERT)

#include "spim_english.h"


typedef struct
{
    GUIIM_INIT_KERNEL_T init_param;
    
    SPIM_HANDLE_T core_handle;
    MMIIM_SP_STATE_T state;

    GUIIM_CAPITAL_MODE_T cap_mode;

    GUI_RECT_T rect;
    
    MMIIM_SP_TEXT_BAR_T bar_edit;
    MMIIM_SP_TEXT_BAR_T bar_cand;

    //draw flag
    BOOLEAN is_rect_change;
    BOOLEAN is_bar_edit_dirty;
    BOOLEAN is_bar_cand_dirty;
}MMIIM_SP_EN26K_DATA_T;


#endif //MMIIM_USE_SPIM_QWERT
#endif //_MMIIM_SP_EN26KEY_H_

#endif




