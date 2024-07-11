#ifndef ZMT_WORD_ID_H
#define ZMT_WORD_ID_H
/*****************************************************************************
** File Name:      mmiacc_id.h                                               *
** Author:         xiaoqing.lu                                               *
** Date:           2009/04/03                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ACC win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/04/03     xiaoqing.lu       Create                                   *
******************************************************************************/


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

#define WIN_ID_DEF(win_id) win_id

// window ID
typedef enum {
    MMIZMT_WORD_WIN_ID_START = (MMI_MODULE_ZMT_WORD << 16),

    MMI_WORD_MAIN_TIPS_WIN_ID,
    MMI_WORD_MAIN_WIN_ID,
    MMI_WORD_BOOK_MAIN_WIN_ID,
    MMI_WORD_CHAPTER_WIN_ID,
    MMI_WORD_DETAIL_WIN_ID,

    MMI_WORD_LISTEN_WIN_ID,
    MMI_WORD_LISTEN_INFO_WIN_ID,
    MMI_WORD_LISTEN_SET_WIN_ID,
  
    MMIZMT_WORD_MAX_WIN_ID

} MMIZMT_WORD_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum {
    MMIZMT_WORD_WIN_CTRL_ID_START = MMIZMT_WORD_MAX_WIN_ID,

    MMI_ZMT_WORD_MAIN_LABEL_BACK_CTRL_ID,
    MMI_ZMT_WORD_MAIN_LIST_CTRL_ID,
    MMI_ZMT_WORD_MAIN_LIST_BOOK_CTRL_ID,
    MMI_ZMT_WORD_MAIN_LABEL_NUM_CTRL_ID,

    MMI_ZMT_WORD_CHAPTER_LABEL_BACK_CTRL_ID,
    MMI_ZMT_WORD_CHAPTER_AUTO_PLAY_CTRL_ID,
    MMI_ZMT_WORD_CHAPTER_LIST_CTRL_ID,
    MMI_ZMT_WORD_CHAPTER_LABEL_NUM_CTRL_ID,
    MMI_ZMT_WORD_CHAPTER_LEFT_CTRL_ID,
    MMI_ZMT_WORD_CHAPTER_RIGHT_CTRL_ID,
    
    MMI_ZMT_WORD_DETAIL_LABEL_BACK_CTRL_ID,
    MMI_ZMT_WORD_DETAIL_LABEL_NUM_CTRL_ID,
    MMI_ZMT_WORD_DETAIL_LABEL_WORD_CTRL_ID,
    MMI_ZMT_WORD_DETAIL_LABEL_PINYIN_CTRL_ID,
    MMI_ZMT_WORD_DETAIL_BUTTON_AUDIO_CTRL_ID,
    MMI_ZMT_WORD_DETAIL_TEXT_INFO_CTRL_ID,
    MMI_ZMT_WORD_MSG_TIPS_CTRL_ID,
    MMI_ZMT_WORD_DETAIL_LEFT_CTRL_ID,
    MMI_ZMT_WORD_DETAIL_RIGHT_CTRL_ID,
    MMI_ZMT_WORD_DETAIL_DELETE_CTRL_ID,

    MMI_ZMT_WORD_LISTEN_BUTTON_SET_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_LABEL_NUM_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_BUTTON_IMG_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_LABEL_TIPS_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_BUTTON_STATUS_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_LEFT_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_RIGHT_CTRL_ID,

    MMI_ZMT_WORD_LISTEN_INFO_LABEL_BACK_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_INFO_LABEL_NUM_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_INFO_LABEL_WORD_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_INFO_LABEL_PINYIN_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_INFO_BUTTON_AUDIO_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_INFO_TEXT_INFO_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_INFO_BUTTON_STATUS_CTRL_ID,

    MMI_ZMT_WORD_LISTEN_FORM_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_FORM_CHILD_1_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_FORM_CHILD_2_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_FORM_CHILD_3_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_FORM_CHILD_1_LABEL_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_FORM_CHILD_2_LABEL_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_FORM_CHILD_3_LABEL_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_FORM_CHILD_1_LIST_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_FORM_CHILD_2_LIST_CTRL_ID,
    MMI_ZMT_WORD_LISTEN_FORM_CHILD_3_LIST_CTRL_ID,
  
    MMIZMT_WORD_MAX_WIN_CTRL_ID
} MMIZMT_WORD_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  //_MMIHELLO_ID_H_
