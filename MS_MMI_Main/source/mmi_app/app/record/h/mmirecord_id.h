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
        MMIRECORD_MENU_CTRL_ID,         //¼���˵��ؼ�
        MMIRECORD_OPTIONMENU_CTRL_ID,   //¼��ѡ��˵��ؼ�
        MMIRECORD_LIST_CTRL_ID,         //¼��list�ؼ�
        MMIRECORD_STORAGE_LIST_CTRL_ID, //¼���洢λ��ѡ��
        MMIRECORD_SAVE_EDITBOX_CTRL_ID,  //¼�����������ؼ�
        MMIRECORD_RENAME_EDITBOX_CTRL_ID,  //¼���������ؼ�
        MMIRECORD_RECORDANIM_CTRL_ID,   //¼������
        MMIRECORD_PLAYANIM_CTRL_ID,   //���Ŷ���
        MMIRECORD_RECORDHINT_CTRL_ID,   //¼����ʾ
        MMIRECORD_PLAYHINT_CTRL_ID,   //������ʾ
        MMIRECORD_STARTSTATUS_OPTIONMENU_CTRL_ID,
        MMIRECORD_DETAIL_NAME_TITLE_CTRL_ID,    //¼���������ֱ���
        MMIRECORD_DETAIL_MEM_TITLE_CTRL_ID,     //¼�������С����
        MMIRECORD_DETAIL_POS_TITLE_CTRL_ID,
        MMIRECORD_DETAIL_DATE_TITLE_CTRL_ID,    //¼���������ڱ���
        MMIRECORD_DETAIL_TIME_TITLE_CTRL_ID,    //¼������ʱ�����
        MMIRECORD_DETAIL_TYPE_TITLE_CTRL_ID,    //¼�������ʽ����
        MMIRECORD_DETAIL_NAME_CTRL_ID,    //¼����������
        MMIRECORD_DETAIL_POS_CTRL_ID,
        MMIRECORD_DETAIL_MEM_CTRL_ID,     //¼�������С
        MMIRECORD_DETAIL_DATE_CTRL_ID,    //¼����������
        MMIRECORD_DETAIL_TIME_CTRL_ID,    //¼������ʱ��
        MMIRECORD_DETAIL_TYPE_CTRL_ID,    //¼�������ʽ
        MMIRECORD_SEND_FILE_CTRL_ID,
        MMIRECORD_FILE_NAME_CTRL_ID,
        MMIRECORD_NAME_LABEL_CTRL_ID,    // ¼����ʾ�ļ���
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
