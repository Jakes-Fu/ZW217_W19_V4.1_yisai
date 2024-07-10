/*****************************************************************************
** File Name:      mmirecord_id.h                                            *
** Author:         haiyang.hu                                                *
** Date:           2006/09/22                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe RECORD win and control id   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/22     haiayng.hu        Create                                   *
******************************************************************************/

#ifndef _MMIRECORD_ID_H_
#define _MMIRECORD_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

    /**--------------------------------------------------------------------------*
     **                         MACRO DEFINITION                                 *
     **--------------------------------------------------------------------------*/


    /**--------------------------------------------------------------------------*
     **                         TYPE AND CONSTANT                                *
     **--------------------------------------------------------------------------*/

#define WIN_ID_DEF(win_id)          win_id
// window ID
    typedef enum
    {
        MMIRECORD_WIN_ID_START = (MMI_MODULE_RECORD << 16),

#include "mmirecord_id.def"

        MMIRECORD_MAX_WIN_ID
    }
    MMIRECORD_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
    typedef enum
    {
        MMIRECORD_CTRL_ID_ID_START = MMIRECORD_MAX_WIN_ID, //(MMI_MODULE_RECORD << 16),
        MMIRECORD_MENU_CTRL_ID,         //录音菜单控件
        MMIRECORD_OPTIONMENU_CTRL_ID,   //录音选项菜单控件
        MMIRECORD_LIST_CTRL_ID,         //录音list控件
        MMIRECORD_STORAGE_LIST_CTRL_ID, //录音存储位置选择
        MMIRECORD_SAVE_EDITBOX_CTRL_ID,  //录音保存命名控件
        MMIRECORD_RENAME_EDITBOX_CTRL_ID,  //录音重命名控件
        MMIRECORD_RECORDANIM_CTRL_ID,   //录音动画
        MMIRECORD_PLAYANIM_CTRL_ID,   //播放动画
        MMIRECORD_RECORDHINT_CTRL_ID,   //录音提示
        MMIRECORD_PLAYHINT_CTRL_ID,   //播放提示
        MMIRECORD_STARTSTATUS_OPTIONMENU_CTRL_ID,
        MMIRECORD_DETAIL_NAME_TITLE_CTRL_ID,    //录音详情名字标题
        MMIRECORD_DETAIL_MEM_TITLE_CTRL_ID,     //录音详情大小标题
        MMIRECORD_DETAIL_POS_TITLE_CTRL_ID,
        MMIRECORD_DETAIL_DATE_TITLE_CTRL_ID,    //录音详情日期标题
        MMIRECORD_DETAIL_TIME_TITLE_CTRL_ID,    //录音详情时间标题
        MMIRECORD_DETAIL_TYPE_TITLE_CTRL_ID,    //录音详情格式标题
        MMIRECORD_DETAIL_NAME_CTRL_ID,    //录音详情名字
        MMIRECORD_DETAIL_POS_CTRL_ID,
        MMIRECORD_DETAIL_MEM_CTRL_ID,     //录音详情大小
        MMIRECORD_DETAIL_DATE_CTRL_ID,    //录音详情日期
        MMIRECORD_DETAIL_TIME_CTRL_ID,    //录音详情时间
        MMIRECORD_DETAIL_TYPE_CTRL_ID,    //录音详情格式
        MMIRECORD_SEND_FILE_CTRL_ID,
        MMIRECORD_FILE_NAME_CTRL_ID,
        MMIRECORD_NAME_LABEL_CTRL_ID,    // 录音显示文件名
        MMIRECORD_BK_LABEL_CTRL_ID,
        MMIRECORD_PLAY_TITLE_LABEL_CTRL_ID,
        MMIRECORD_PLAY_CURTIME_LABEL_CTRL_ID,
        MMIRECORD_PLAY_TOLTIME_LABEL_CTRL_ID,

        MMIRECORD_MAINPLAY_FORM_CTRL_ID,
        MMIRECORD_FORM_ANIM_CTRL_ID,

        MMIRECORD_TIME_FORM_CTRL_ID,
        MMIRECORD_FORM_ANIM_H_CTRL_ID,
        MMIRECORD_TIME_ORDER_FORM_CTRL_ID,
        MMIRECORD_HSPACE_OWNDRAW_H_CTRL_ID,
        MMIRECORD_TIME_OWNDRAW_H_CTRL_ID,
        MMIRECORD_NAME_LABEL_H_CTRL_ID,

        MMIRECORD_BUTTON_FORM_CTRL_ID,
        MMIRECORD_PLAY_BUTTON_CTRL_ID,
        MMIRECORD_STOP_BUTTON_CTRL_ID,
        MMIRECORD_RECORD_BUTTON_CTRL_ID,
        MMIRECORD_LIST_BUTTON_CTRL_ID,
        MMIRECORD_OPT_BUTTON_CTRL_ID,
        MMIRECORD_VOLUME_BUTTON_CTRL_ID,
        MMIRECORD_OPT_LIST_CTRL_ID,
        MMIRECORD_PAUSE_BUTTON_CTRL_ID,
        MMIRECORD_DELETE_BUTTON_CTRL_ID,
        MMIRECORD_RESUME_BUTTON_CTRL_ID,

        MMIRECORD_TIME_OWNDRAW_CTRL_ID,
#ifndef MMI_GUI_STYLE_TYPICAL
        MMIRECORD_POINTER_OWNDRAW_CTRL_ID,
#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
        
#endif
        MMIRECORD_TIME_VOLUME_FORM_CTRL_ID,
        MMIRECORD_PLAY_PANEL_FORM_CTRL_ID,
        MMIRECORD_PLAY_NAME_LABEL_FORM_CTRL_ID,
        MMIRECORD_PROGRESS_OWNDRAW_CTRL_ID,
        MMIRECORD_PLAY_TIME_FORM_CTRL_ID,
        MMIRECORD_PLAY_TIME_LABEL_CTRL_ID,
        MMIRECORD_PLAY_ALL_TIME_LABEL_CTRL_ID,
#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
        MMIRECORD_PLAY_STOP_BUTTON_CTRL_ID,
#endif
        MMIRECORD_BLANK_OWNDRAW_CTRL_ID,
//    MMIRECORD_PDA_ANIM_CTRL_ID,
#endif
        MMIRECORD_LEFT_BUTTON_CTRL_ID,
        MMIRECORD_RIGHT_BUTTON_CTRL_ID,
        MMIRECORD_BACK_BUTTON_CTRL_ID,
        MMIRECORD_OPTION_BUTTON_CTRL_ID,
        WATCHRECORD_LIST_CTRL_ID,
        MMIRECORD_MAX_CTRL_ID
    } MMIRECORD_CONTROL_ID_E;

    /**--------------------------------------------------------------------------*
     **                         FUNCTION DEFINITION                              *
     **--------------------------------------------------------------------------*/

    /*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
    /*****************************************************************************/
    PUBLIC void MMIRECORD_RegWinIdNameArr(void);

    /**--------------------------------------------------------------------------*
     **                         Compiler Flag                                    *
     **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_MMIRECORD_ID_H_
