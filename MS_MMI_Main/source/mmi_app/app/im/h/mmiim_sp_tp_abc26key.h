/*==============================================================================
File Name: mmiim_sp_tp_abc26key.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _MMIIM_SP_TP_ABC26KEY_H_
#define _MMIIM_SP_TP_ABC26KEY_H_


/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmiim_sp_tp_basic.h"
#include "mmiim_sp_ml_data.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#if defined(MMIIM_USE_SPIM_TP)

#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef struct
{
    CAF_HANDLE_T            ctrl_handle;            // ctrl handle
    GUIIM_INIT_KERNEL_T     init_param;             // init param
    GUIIM_CAPITAL_MODE_T    cap_mode;               // current capital mode
    GUI_RECT_T              rect;                   // ime rect

    BOOLEAN                 is_timer_active;        // is time active

    MMIIM_SP_ML_DATA_T      language_data;          // language data

    //显示相关的
    MMIIM_SP_TP_KB_INFO_T   kb;                     // keyboard panel

    wchar                   last_press_index;       // 上一次的有效按键，此值是"kb"定义的wchar值
    uint16                  last_char_index;        // 字母索引

    //draw flag
    BOOLEAN                 is_kb_dirty;            // 是否需要重绘
} MMIIM_SP_TP_ABC26KEY_DATA_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif //MMIIM_USE_SPIM_TP
#endif //_MMIIM_SP_TP_ABC26KEY_H_

