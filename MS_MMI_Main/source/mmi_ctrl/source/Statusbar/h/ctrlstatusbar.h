/*****************************************************************************
** File Name:      ctrlstatusbar.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012     xiyuan.ma      Create
******************************************************************************/

#ifndef _CTRLSTATUSBAR_H_
#define _CTRLSTATUSBAR_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlstatusbar_export.h"

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
// statusbar type info
#define CTRL_STATUSBAR_TYPE (STATUSBAR_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// statusbar init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
} CTRLSTATUSBAR_INIT_PARAM_T;

//statusbar control
typedef struct _statusbar_obj_tag
{
    CTRLBASE_OBJ_T          base_ctrl;

    uint8                   timer_id;		                         /*!< ʱ�� id*/
    uint8                   scroll_msg_timer_id;		 /*!< ������Ϣʱ�� id*/
    MMI_STATUSBAR_STYLE_T   style_info;                               /*!< Ĭ�Ϸ����Ϣ*/
    GUISTATUSBAR_STATE_T    ctrl_state;                               /*!< �ؼ�״̬*/
    GUISTATUSBAR_TYPE_E     ctrl_type;                                /*!< �ؼ����ͣ�ˮƽ��ʾ*/
    UILAYER_APPEND_BLT_T    statusbar_layer;                     /*!< ״̬��֧���Դ���*/
    BOOLEAN                 is_display_bg;  			/*!< �Ƿ���ʾ�������������ʾ����ֱ��ʹ�ô��ڱ���*/
    BOOLEAN                 is_shown;                               /*!< �Ƿ���ʾ*/
    BOOLEAN                 clear_customer_bg;               /*!< �Ƿ�����û���ı���*/
#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
    BOOLEAN                 is_press_down;
    BOOLEAN                 dropdown_state;
#endif
} CTRLSTATUSBAR_OBJ_T;

// statusbar class for public or protected method
typedef struct _statusbar_vtbl_tag
{
    CTRLBASE_VTBL_T         base_ctrl_vtbl;

    // self vtbl ...
} CTRLSTATUSBAR_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get statusbar type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T STATUSBAR_GetType (void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
