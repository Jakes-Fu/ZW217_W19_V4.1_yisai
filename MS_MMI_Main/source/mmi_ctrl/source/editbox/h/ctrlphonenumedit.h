/*****************************************************************************
** File Name:      ctrlphonenumedit.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLPHONENUMEDIT_H_
#define _CTRLPHONENUMEDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseflex.h"
#include "ctrlphonenumedit_export.h"

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
// phonenumedit type info
#define CTRL_PHONENUMEDIT_TYPE     (PHONENUMEDIT_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// phonenumedit init param
typedef struct
{
    CTRLBASEFLEX_INIT_PARAM_T       baseflex_ctrl_param;

    // self inif param ...
} CTRLPHONENUMEDIT_INIT_PARAM_T;

//phonenumedit control
typedef struct
{
    CTRLBASEFLEX_OBJ_T              baseflex_ctrl;

    // ����basetextedit��
    BOOLEAN                         is_font_checked;    // �����Ƿ�����ˣ���ֹ��εݹ����
    GUIEDIT_FONT_T                  edit_font;          // ��С����

    BOOLEAN                         is_dial_font_adaptive;   /*!<�Ƿ��Զ��л����ִ�С������*/
    BOOLEAN                         is_dial_line_adaptive; /*!<�Ƿ��Զ��л�˫����ʾ(����С����ʱ)*/
    BOOLEAN                         is_dtmf_num;

    MMITHEME_EDIT_TEXT_T            theme;   //phone number theme
} CTRLPHONENUMEDIT_OBJ_T;

// phonenumedit class for public or protected method
typedef struct
{
    CTRLBASEFLEX_VTBL_T            baseflex_ctrl_vtbl;

    // self vtbl ...
} CTRLPHONENUMEDIT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get phonenumedit type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T PHONENUMEDIT_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
