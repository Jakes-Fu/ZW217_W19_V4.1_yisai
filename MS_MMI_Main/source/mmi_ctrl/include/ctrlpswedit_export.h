/*****************************************************************************
** File Name:      ctrlpswedit_export.h                                        *
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

#ifndef _CTRLPSWEDIT_EXPORT_H_
#define _CTRLPSWEDIT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseflex_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*! @enum GUIEDIT_PASSWORD_STYLE_E
@brief 密码编辑框类型
*/
typedef enum
{
    GUIEDIT_PASSWORD_STYLE_DIGITAL,     /*!<只包含数字*/
    GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN, /*!<包含数字和# (\#为确定)*/
    GUIEDIT_PASSWORD_STYLE_DIGITAL_PUK, /*!<包含数字和,*,# (\#为确定)*/
    GUIEDIT_PASSWORD_STYLE_DIGITAL_EX , /*!<包含数字和,*,#*/
    GUIEDIT_PASSWORD_STYLE_ALL,         /*!<可以包含所有*/
    GUIEDIT_PASSWORD_STYLE_MAX          /*!<保留位*/
} GUIEDIT_PASSWORD_STYLE_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set edit password style
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPSWEDIT_SetPasswordStyle (
    MMI_CTRL_ID_T              ctrl_id,
    GUIEDIT_PASSWORD_STYLE_E   style
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
