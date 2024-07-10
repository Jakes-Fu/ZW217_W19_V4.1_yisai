/*****************************************************************************
** File Name:      ctrlpswedit.h                                               *
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

#ifndef _CTRLPSWEDIT_H_
#define _CTRLPSWEDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseflex.h"
#include "ctrlpswedit_export.h"

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
// pswedit type info
#define CTRL_PSWEDIT_TYPE     (PSWEDIT_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// pswedit init param
typedef struct
{
    CTRLBASEFLEX_INIT_PARAM_T       baseflex_ctrl_param;

    // self inif param ...
} CTRLPSWEDIT_INIT_PARAM_T;

//pswedit control
typedef struct
{
    CTRLBASEFLEX_OBJ_T              baseflex_ctrl;

    uint8                           pw_timer_id;        /*!<密码定时器*/

    BOOLEAN                         is_edit_commit; /*!<是否edit本身提交字符,定时器到时强制提交不确定字符*/
    wchar                           *password_ptr;  // password string
    uint16                          visible_start;      /*!<可见字符起始位置*/
    uint16                          visible_len;    /*!<可见字符长度*/
    GUIEDIT_PASSWORD_STYLE_E        psw_style;          /*!<密码风格*/

    MMITHEME_EDIT_PSW_T             theme;   //password theme
} CTRLPSWEDIT_OBJ_T;

// pswedit class for public or protected method
typedef struct
{
    CTRLBASEFLEX_VTBL_T             baseflex_ctrl_vtbl;

    // self vtbl ...
} CTRLPSWEDIT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get pswedit type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T PSWEDIT_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
