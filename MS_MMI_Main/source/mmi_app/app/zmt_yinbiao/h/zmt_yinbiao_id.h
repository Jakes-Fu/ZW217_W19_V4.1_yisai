/*****************************************************************************
** File Name:      zmt_pinyin_id.h                                               *
** Author:         fys                                               *
** Date:           2024/07/08                                                *
******************************************************************************/
#ifndef ZMT_YINBIAO_ID_H
#define ZMT_YINBIAO_ID_H

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
#define CLASS_READ_CTRL_ID_MAX 50

#define WIN_ID_DEF(win_id) win_id

// window ID
typedef enum {
    MMIZMT_YINBIAO_WIN_ID_START = (MMI_MODULE_ZMT_YINBIAO << 16),

    ZMT_YINBIAO_MAIN_WIN_ID,
    ZMT_YINBIAO_READ_WIN_ID,
    ZMT_YINBIAO_TABLE_WIN_ID,
    ZMT_YINBIAO_TABLE_TIP_WIN_ID,

    MMIZMT_YINBIAO_MAX_WIN_ID
} MMIZMT_YINBIAO_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum {
    MMIZMT_YINBIAO_CTRL_ID_START = MMIZMT_YINBIAO_MAX_WIN_ID,

    ZMT_YINBIAO_BUTTON_1_CTRL_ID,
    ZMT_YINBIAO_BUTTON_2_CTRL_ID,
    ZMT_YINBIAO_BUTTON_3_CTRL_ID,
    ZMT_YINBIAO_BUTTON_4_CTRL_ID,
    ZMT_YINBIAO_BUTTON_5_CTRL_ID,
    ZMT_YINBIAO_BUTTON_6_CTRL_ID,
     ZMT_YINBIAO_BUTTON_7_CTRL_ID,

    ZMT_YINBIAO_READ_TABLE_CTRL_ID,
    ZMT_YINBIAO_READ_YINBIAO_CTRL_ID,
    ZMT_YINBIAO_READ_CIRCULATE_CTRL_ID,
    ZMT_YINBIAO_READ_SINGLE_CTRL_ID,
    ZMT_YINBIAO_READ_NEXT_CTRL_ID,
    ZMT_YINBIAO_READ_PLAY_CTRL_ID,
    ZMT_YINBIAO_READ_PRE_CTRL_ID,

    ZMT_YINBIAO_TABLE_ICONLIST_CTRL_ID,
    ZMT_YINBIAO_TABLE_FORM_CTRL_ID,
    ZMT_YINBIAO_TABLE_FORM_CHILD_CTRL_ID,
    ZMT_YINBIAO_TABLE_FORM_LEFT_CTRL_ID,
    ZMT_YINBIAO_TABLE_FORM_RIGHT_CTRL_ID,

    ZMT_YINBIAO_TABLE_TIP_LEFT_CTRL_ID,
    ZMT_YINBIAO_TABLE_TIP_RIGHT_CTRL_ID,
    
    MMIZMT_YINBIAO_MAX_CTRL_ID
} MMIZMT_YINBIAO_CONTROL_ID_E;

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