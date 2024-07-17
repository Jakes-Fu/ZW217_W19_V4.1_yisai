/*****************************************************************************
** File Name:      zmt_pinyin_id.h                                               *
** Author:         fys                                               *
** Date:           2024/07/08                                                *
******************************************************************************/
#ifndef ZMT_PINYIN_ID_H
#define ZMT_PINYIN_ID_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmk_type.h"
#include "os_api.h"
#include "sci_types.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define PINYIN_BUTTON_CTRL_ID_MAX 25

#define WIN_ID_DEF(win_id) win_id

// window ID
typedef enum {
    MMIZMT_PINYIN_WIN_ID_START = (MMI_MODULE_ZMT_PINYIN << 16),

    ZMT_PINYIN_MAIN_WIN_ID,
    ZMT_PINYIN_READ_WIN_ID,
    ZMT_PINYIN_TABLE_WIN_ID,
    ZMT_PINYIN_TABLE_TIP_WIN_ID,

    MMIZMT_PINYIN_MAX_WIN_ID
} MMIZMT_PINYIN_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum {
    MMIZMT_PINYIN_CTRL_ID_START = MMIZMT_PINYIN_MAX_WIN_ID,

    ZMT_PINYIN_BUTTON_1_CTRL_ID,
    ZMT_PINYIN_BUTTON_2_CTRL_ID,
    ZMT_PINYIN_BUTTON_3_CTRL_ID,
    ZMT_PINYIN_BUTTON_4_CTRL_ID,
    ZMT_PINYIN_BUTTON_5_CTRL_ID,
    ZMT_PINYIN_BUTTON_6_CTRL_ID,
    
    ZMT_PINYIN_READ_TABLE_CTRL_ID,
    ZMT_PINYIN_READ_YINBIAO_CTRL_ID,
    ZMT_PINYIN_READ_CIRCULATE_CTRL_ID,
    ZMT_PINYIN_READ_SINGLE_CTRL_ID,
    ZMT_PINYIN_READ_NEXT_CTRL_ID,
    ZMT_PINYIN_READ_PLAY_CTRL_ID,
    ZMT_PINYIN_READ_PRE_CTRL_ID,

    ZMT_PINYIN_TABLE_ICONLIST_CTRL_ID,
    ZMT_PINYIN_TABLE_FORM_CTRL_ID,
    ZMT_PINYIN_TABLE_FORM_CHILD_CTRL_ID,
    ZMT_PINYIN_TABLE_FORM_LEFT_CTRL_ID,
    ZMT_PINYIN_TABLE_FORM_RIGHT_CTRL_ID,

    ZMT_PINYIN_TABLE_TIP_LEFT_CTRL_ID,
    ZMT_PINYIN_TABLE_TIP_RIGHT_CTRL_ID,
    
    MMIZMT_PINYIN_MAX_CTRL_ID
} MMIZMT_PINYIN_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif