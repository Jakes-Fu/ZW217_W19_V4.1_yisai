/*==============================================================================
File Name: mmiim_sp_tp_digital.h
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


#ifndef _MMIIM_SP_TP_DIGITAL_H_
#define _MMIIM_SP_TP_DIGITAL_H_


#include "mmiim_sp_tp_basic.h"

#if defined(MMIIM_USE_SPIM_TP)


typedef struct
{
    GUIIM_INIT_KERNEL_T init_param;

    GUI_RECT_T rect;

    //显示相关的
    MMIIM_SP_TP_KB_INFO_T kb;

    BOOLEAN is_symb_kb;
    BOOLEAN is_symb_lock;

    //draw flag
    BOOLEAN is_kb_dirty;
} MMIIM_SP_TP_DIGITAL_DATA_T;


#endif //MMIIM_USE_SPIM_TP
#endif /*_MMIIM_SP_TP_DIGITAL_H_*/

