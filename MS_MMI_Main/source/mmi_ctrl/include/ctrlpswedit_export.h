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
@brief ����༭������
*/
typedef enum
{
    GUIEDIT_PASSWORD_STYLE_DIGITAL,     /*!<ֻ��������*/
    GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN, /*!<�������ֺ�# (\#Ϊȷ��)*/
    GUIEDIT_PASSWORD_STYLE_DIGITAL_PUK, /*!<�������ֺ�,*,# (\#Ϊȷ��)*/
    GUIEDIT_PASSWORD_STYLE_DIGITAL_EX , /*!<�������ֺ�,*,#*/
    GUIEDIT_PASSWORD_STYLE_ALL,         /*!<���԰�������*/
    GUIEDIT_PASSWORD_STYLE_MAX          /*!<����λ*/
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
